/* @(#)proc.c 1.18 97/03/05 */
/*
 * ====================================================
 * Copyright (C) 1995 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunSoft, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

#include <stdio.h>
#include <unistd.h>	/* for access() */
#include "fpp.h"
#include "service.h"
#include "lex.h"
#include "symtab.h"
#include "rgram.h"	/* for ST_FPP, st_is() */
#include "sb.h"

#if DEBUG
int     debug;          /* debug level */
#endif
unsigned mmode;			/* main mode of input processing */
int	line_width0;		/* predefined line width */
int	line_width;		/* current input  line maximum width */
int	oline_width;		/* current output line maximum width */

int	f77fl;		/* 1 - F77, 0 - F90 */
int	fixedformfl;	/* 1 - fixed form, 0 - free form */
int	outfl;		/* prohibits output when non zero */
int     falselvl;       /* false level in nested #if's */
int     substfl;        /* macro substitution mode (plain text):
			 * 0 - never, 1 - no comments, 2 - always */
int	dosubstfl;	/* force macro substitution, no matter what
			 * value the flag above has */
int	rmcommfl;	/* remove comments */
int     no_ccom_fl;     /* don't recognize c style comments */
int	linefl;		/* output # file, line information */
int	ignorelinefl;	/* ignore input # file directives */
int	sbfl;		/* output for sbrowser */
int	predefinefl;	/* do predefining */
int	mdepfl;		/* generate make dependencies */

void	*onequalsym;	/* assignment substitution */
int	onequalline;	/* assignment substitution line */
int     ctrl_mode;      /* control chars processing mode */

int	inactive;	/* is current line inactive */

StrList	include_path = { NULL, "."};	/* list of include directories */
Comment *comments = NULL;	/* list of comments to be outputed by fpp */

SymPtr	filesymp, FILEsymp;	/* fpp dynamic macro */
SymPtr	linesymp, LINEsymp;

typedef struct _ifnode {	/* stack of nested #if..s */
	int	flvlsaved;
	int	elsefl;
	unsigned ifline;
	struct _ifnode *popif;
} IfNode;

static IfNode	*curif;		/* current #if.. node */

/*************************************************************** Bufers */

#define	INBUFSIZE 1+BUFSIZE+1

typedef struct _inbuf {
	struct _inbuf	*next;
	uchar	*psaved;
	uchar	buffer[INBUFSIZE];
	}	InBuffer;

#define INBUF_BEG(buf)	((buf)->buffer + 1)
#define INBUF_NEXT(buf) ((buf)->next)
#define INBUF_PSAVED(buf) ((buf)->psaved)

static InBuffer *curbuf;
uchar	*curp;

InBuffer    *
new_inbuf() {
	return (InBuffer *)my_alloc(sizeof(InBuffer),1);
}

void
free_inbuf(InBuffer *p) {
	free(p);
}

/*
typedef struct _outbuf {
	struct _outbuf *next;
	char	buffer[INBUFSIZE];
	}	OutBuffer;

static OutBuffer *outbuf;
static OutBuffer *free_outbufs = NULL;
static char	  outp;

OutBuffer    *
new_outbuf() {
	OutBuffer   *res;

	if (free_outbufs != NULL) {
		res = free_outbufs;
		free_outbufs = free_outbufs->next;
	}
	else {
		res = my_alloc(sizeof(OutBuffer),1);
	}
	return res;
}

void
free_outbuf(OutBuffer *p) {
	p->next = free_outbufs;
	free_outbufs = p;
}
*/
/*************************************************************** Input */

typedef struct _infile {
	char		*fname;
	char		*dir;
	unsigned	id;
	unsigned 	line;
	FILE		*fp;
	struct _infile 	*popfile;
	IfNode		*ifnode;
	InBuffer 	*inbuf;
}	InFile;

#define INFILE_FP(infile) ((infile)->fp)
#define INFILE_EOF(infile) (feof(INFILE_FP(infile)))

static InFile	*curfile = NULL;
static InFile	*outfile = NULL;
static InFile	*mfile   = NULL;
static char	*mdfname = NULL;

static unsigned	 curfileid = 0;	/* simple file identification */

unsigned	line;		/* current line of the current file */
int		colm;		/* current column in the line , may be < 0 */
static	int	errline = -1;	/* error line number */

void
set_pos(int eline) {
	errline = eline;
}

char    *
get_cur_fname() {
        if (curfile)
	        return curfile->fname;
	return NULL;
}

get_err_lineno() {
        int     res;

        if (line == 0)
	        return 0;
	if (errline <0)
	        return get_cur_tok()->lineno;
	res = errline;
	errline = -1;
	return res;
}

InFile	*
my_fopen(char *fn, char *mod) {
	InFile	*res;
	FILE	*fp;
	char	*s, *s0;

	fp = fopen(fn,mod);
	if (fp == NULL)
		fppmess(FERR_FOPEN,fn);
	res = my_alloc(sizeof(InFile),1);
	res->fp = fp;
	res->id = curfileid++;
	res->inbuf = new_inbuf();

	s0 = my_strdup(fn);
	s = s0 + strlen(s0);
	while (s > s0 && *--s != '/');
	if ((s == s0) && (*s != '/')) {
		res->dir = my_strdup(CURDIR);
		res->fname = s;
	}
	else {
		CHECK(*s == '/');
		res->dir = s0;
		*s = '\0';
		res->fname = s+1;
	}
	return res;
}

void
my_fclose(InFile *fn) {
	free_inbuf(fn->inbuf);
	if (strcmp(fn->fname,STDINNAME))	fclose(fn->fp);
	free(fn);
}

InFile	*
fopen_stdfile(FILE *fn) {
	InFile	*res;

	res = my_alloc(sizeof(InFile),1);
	res->fp = fn;
	res->id = curfileid++;
	res->inbuf = new_inbuf();
	if (fn == stdin)
		res->fname = my_strdup(STDINNAME);
	else if (fn == stdout)
		res->fname = my_strdup(STDOUTNAME);
	else if (fn == stderr)
		res->fname = my_strdup(STDERRNAME);
	else 
		CHECK(0);
	res->dir = my_strdup(CURDIR);
	return res;
}

void
my_fread(uchar *b) {
	char	*res;

	CHECK(b>=INBUF_BEG(curbuf)&&(b<INBUF_BEG(curbuf)+BUFSIZE));
	res = fgets((char *)b,BUFSIZE-(b-INBUF_BEG(curbuf)),curfile->fp);
	if (res == NULL) {
		if (ferror(curfile->fp)) 
			fppmess(FERR_FREAD,curfile->fname);
		else *b = '\0';
	}
	return;
}

void
my_fwrite(char *b, int c) {
	int	res;

	res = fwrite(b,1,c,outfile->fp);
	if (res != c) 
		fppmess(FERR_FWRITE,outfile->fname);
	return;
}

void
md_fwrite(char *b) {
	int	res;

	res = fprintf(mfile->fp,"%s.o: %s\n",mdfname,b);
	if (res < 0) 
		fppmess(FERR_FWRITE,mfile->fname);
	return;
}

void
my_fflush(int status) {
	if (outfile) {
		if (status) 
			fprintf(outfile->fp,"\n");
		fflush(outfile->fp);
	}
	return;
}

void
refill() {
	InBuffer *tmp;

	CHECK(*curp==0);
	if (curbuf->next != NULL) {
		tmp = curbuf;
		curbuf = curbuf->next;
		curp = curbuf->psaved;
		free_inbuf(tmp);
	}
	else {
		curp = INBUF_BEG(curbuf) + BUFSIZE/2;	/* to reduce ungetstr time */
		my_fread(curp);
		if (INFILE_EOF(curfile)) {
		        curp[0] = SYM_EOF >> 8;
			curp[1] = SYM_EOF & 0xff;
			curp[2] = 0;
			ctrl_mode++;
		}
		if (sbfl) sb_lineid((char *)curp, inactive);
	}
}

static void
hide_buffer() {
        InBuffer *tmp;

	/* If the current buffer is overflown
	 * we use this function to go on with
	 * a new buffer.
	 */
	tmp = new_inbuf();
	curbuf->psaved = curp;
	tmp->next = curbuf;
	curbuf = tmp;
	curp = INBUF_BEG(curbuf)+INBUFSIZE;
}

/*
void 
ungetstr( char *s, int len ) {

	CHECK(len >= 0);
	if (len && s[len-1] == '\n') {
		line--;
		colm = line_width - len;
	}
	else colm -= len;
	s += len;
	while (len--) {
		if (curp <= INBUF_BEG(curbuf)) {
		        hide_buffer();
		}
		*--curp = *--s;
	}
}
*/

void 
ungetsym(wchar c) {
	if (c == '\n') {
		line--;
		colm = line_width;
	}
	else if (c == SYM_EOF) {
		colm = line_width;
	}
	else colm--;

	if (curp <= INBUF_BEG(curbuf)) {
	        hide_buffer();
	}
	*--curp = c & 0xff;
	if (is_special(c)) {
		ctrl_mode++;
	        if (curp <= INBUF_BEG(curbuf)) {
		        hide_buffer();
		}
		*--curp = SYM_CTRL;
	}
	else if (c == SYM_CTRL && ctrl_mode) {
	        if (curp <= INBUF_BEG(curbuf)) {
		        hide_buffer();
		}
		*--curp = SYM_CTRL;
	}
}

void 
ungetstr( char *s, int len ) {

	CHECK(len >= 0);
	s += len;
	while (len--) {
	        ungetsym((wchar) (*--s & 0xff));
	}
}
	
/************************************************************** Output */

static	unsigned ofileid = (unsigned)-1;
static  int  ocolm = 0;
static  int  oline = -1;	/* output line number */
#define NOINCL	""
#define PUSHINCL "1"
#define POPINCL  "2"

void
outpos(int lineno, char *pushpop) {
	static	char buf[BUFSIZE];
	static	int  firsttime = 1;
	
	CHECK(curfile);
	if (firsttime) {
		firsttime = 0;
		if (sbfl) sb_version();
		ofileid = curfile->id;
		oline = lineno;
		if (linefl) {
		        if (!strcmp(curfile->dir,CURDIR)) {
			        sprintf(buf, "# %d \"%s\"\n", oline, curfile->fname);
			}
			else {
			        sprintf(buf, "# %d \"%s/%s\"\n", oline, 
					curfile->dir, curfile->fname);
			}
		}
	}
	else if (curfile->id != ofileid) {
		ofileid = curfile->id;
		oline = lineno;
		if (sbfl) sb_sayline();
		if (linefl) {
		        if (curfile->popfile == NULL && !strcmp(curfile->dir,CURDIR))
			        sprintf(buf, "# %d \"%s\" %s \n", 
					oline, curfile->fname, 
					pushpop);
			else
			        sprintf(buf, "# %d \"%s/%s\" %s \n", 
					oline, curfile->dir, curfile->fname, 
					pushpop);
		}
	}
	else if (lineno != oline) {
		oline = lineno;
		if (sbfl) {
			sb_lineno(oline);
			return;
		}
		else if (linefl)
		        sprintf(buf, "# %d\n", oline);
	}
	else return;
	if (linefl)
	        my_fwrite(buf,strlen(buf));
	return;
}

void
outcomm(Comment *com) {

	if (substfl == 2) {
		Token   *tokp;
		char    *s;
		int     len;
		int line_width_sav;

	        /* Well, well. Here we are going to 
		 * implement "smart" macro expansion
		 * in comments. We'll see.
		 */

	        if (com->lineno != oline) {
			if (ocolm != 0) {
			        my_fwrite("\n",1);
				oline++; ocolm = 0;
			}
			outpos(com->lineno, NOINCL);
		}

		s = com->str; len = com->length;
		CHECK(s[len-1] == '\n');

		if (s[0] != '\n' && !is_blank(s[0])) {

		        /* The first char is something
			 * like '!', 'C' etc. Output it
			 * as is.
			 */
		        my_fwrite(s, 1);
			ocolm++; s++; len--;
		}

		ungetsym(SYM_EOC);
	        ungetstr(s, len-1);

		/* No continuation card until we have finished */
		line_width_sav = oline_width;
		oline_width = LINE_WIDTH_MAX;

		for (;;) {
		        tokp = get_token(MOD_RAW | MOD_NOGRAM | MOD_COM);

			if (tokp->val == TK_EOC) {

			        /* We changed \n to SYM_EOC and
				 * now we are paying our debt.
				 */
			        my_fwrite("\n",1);
				oline++; ocolm = 0;
			        break;
			}

			/* Adjust lineno info */
			tokp->lineno = com->lineno;

			my_fwrite(tokp->token, tokp->length);
			ocolm += tokp->length;
		}

		/* Restore the saved value */
		oline_width = line_width_sav;
	}
	else {
		if (com->lineno != oline) {
			if (ocolm != 0) {
			        my_fwrite("\n",1);
				oline++; ocolm = 0;
			}
		        outpos(com->lineno, NOINCL);
		}
		else if ((ocolm + com->length > oline_width) &&
			 (com->length <= oline_width) ) {

		        /* Try not to loose commented stuf if it
			 * doesn't fit in the remaining of the line.
			 * But if the comment is too long then since
			 * we can't use continuation card for comments
			 * we can do nothing but output it as is.
			 */

		        my_fwrite("\n",1);
			oline++; ocolm = 0;
		        outpos(com->lineno, NOINCL);
		}

		my_fwrite(com->str, com->length);
		if (com->str[com->length - 1] == '\n') {
		        oline++;
		        ocolm = 0;
		}
		else {
		        ocolm += com->length;
		}
	}
}

void
outtok(Token *tokp) {
	static char contline[] = "\n     *";
	static char contline90[] = "&\n&";
	static int  last_stid = -1;
	int	i;
	char	*p, *endp;

	CHECK(outfl==0);
	if (tokp->length == 0) goto exit;

	if (!outfl && !falselvl) {

	        if (!f77fl && !fixedformfl &&
		    tokp->val != TK_NL     &&
		    tokp->stid == last_stid ) {

		        if (tokp->lineno != oline || ocolm >= oline_width - 1) {
			        my_fwrite("&",1);
				ocolm++;
			}
		}

		for (;comments;) {
		        Comment *prev;

			if ((comments->lineno < tokp->lineno) ||
			    (comments->lineno == tokp->lineno && tokp->val == TK_NL))
			{
			        pushtok();
			        outcomm(comments);
				poptok();

				prev = comments;
				comments = comments->next;
				free(prev);
			}
			else break;
		}

	        if ( tokp->lineno != oline   ||
		     tokp->stid != last_stid ||
		     ocolm >= oline_width )
		{
			if (ocolm != 0) {
			        my_fwrite("\n",1);
				oline++; ocolm = 0;
			}

			if (tokp->val == TK_NL &&
			    tokp->lineno < oline)
			        goto exit;

			outpos(tokp->lineno, NOINCL);
		}

		if (fixedformfl && (ocolm == 0) &&
		    tokp->stid == last_stid &&
		    (tokp->val != TK_NL) &&
		    (tokp->val != TK_BOS) )
		{
		        my_fwrite(contline+1+ocolm, 6-ocolm);
			ocolm = 6;
		}

		if (tokp->val == TK_NL) {
		        if (tokp->token[0] == '\n') {
			        oline++;
				ocolm = -1;
			}
			else goto exit; /* EOF */
		}
		else if (tokp->val ==  TK_BOS) {	
			if (ocolm >= 6) {
				fppmess(ERR_LONGLBL);
				tokp->token[0] = ' ';
				tokp->length = 1;
			}
			else if (tokp->token[0] == '\t') {
			        ocolm = 5;
			}
			else {
				for (i=0; i < 6-ocolm; i++)
					tokp->token[i] = ' ';
				tokp->length = 6 - ocolm;
			}
		}
		p = tokp->token;
		endp = tokp->token + tokp->length;
		if (!f77fl && !fixedformfl) {
			while (ocolm +(endp - p) >= oline_width) {
				my_fwrite(p, oline_width - ocolm -1);
				p += oline_width - ocolm -1;
				my_fwrite(contline90, 3);
				ocolm = 1;
				oline++;
			}
		}
		else {
			while (ocolm +(endp - p) > oline_width) {
				my_fwrite(p, oline_width - ocolm);
				p += oline_width - ocolm;
				my_fwrite(contline, 7);
				ocolm = 6;
				oline++;
			}
		}
		my_fwrite(p, endp - p);
		ocolm += endp - p;
	}				
exit:	last_stid = tokp->stid;
	return;
}

/************************************************************** Tokens */

void
skiptok(Token *tokp) {
        if (tokp) {
	        tokp->length = 0;
		tokp->token[0] = 0;
	}
}

Token   *
skipbl(int mode) {
        Token   *tokp;

	for (;;) {
		tokp = get_token(mode);
		if (!is_blank(tokp->token[0]))
			break;
	}
	return tokp;
}

Token   *
skipnl(Token *tokp) {
	if (tokp->val == TK_NL) {
		st_move(TK_NL);
		return tokp;
	}
	for(;;) {
		tokp = get_token(MOD_NOGRAM);
		if (tokp->val == TK_NL) break;
	}
	st_move(TK_NL);
	return tokp;
}

int
substitute(SymPtr symp, unsigned mode) {
	unsigned lmode;
	char	*s,*s1;
	int	nform, nact, n;
	int	plvl;
	char	*mname;
	StrList *actuals,*p;
	char	text[BUFSIZE];
	char	dynval[BUFSIZE];
	char	*d0,*d;
	int	loc_errfl;
	int	linesav;
	Token   *tokp;
	
	lmode = (mode & ~MOD_SPACE) | MOD_NOGRAM;
	tokp = get_cur_tok();
	linesav = tokp->lineno;
	
	s = symvals(symp);
	if (symp == filesymp || symp == FILEsymp) {
		if (*s == FPPDYNMACRO) {
			sprintf(dynval, "%c\"%s\"%c", 0, curfile->fname, -1);
			s = dynval + 1 + strlen(dynval+1);
		}
	}
	else if (symp == linesymp || symp == LINEsymp) {
		if (*s == FPPDYNMACRO) {
			sprintf(dynval, "%c%d%c", 0, linesav, -1);
			s = dynval + 1 + strlen(dynval+1);
		}
	}

	if (symrecurse(symp)) 
		return 1;		/* don't substitute `#define a(x) 1+a(x)` */
	mname = symname(symp);
	outfl++;
	nform = *--s & 0xff;
	if (nform == 255) nform = -1;
	nact = -1;
	if (nform >= 0) {

		/* There must be arguments or () at least */
		tokp = skipbl(lmode);
		if (tokp->token[0] != '(') {
			set_pos(linesav);
			if (!(lmode & MOD_COM))
			        fppmess(ERR_NOPAR);
			unget_tok(tokp);
			goto lab1;
		}
		nact = 0;
		actuals = NULL;
		d0 = text;
		*d0++ = '\0';
		d = d0;
		plvl = 0;
		loc_errfl = 0;
		tokp = skipbl(lmode);
		for (;;) {
			switch(tokp->token[0]) {
			case '(':
				plvl++;
				break;
			case ')': 
				if (!plvl && !nform && !nact && d == d0) {
					plvl--;
					break;
				}		/* no break; */
			case ',':
				if (!plvl) {
					while (is_blank(d[-1])) d--;	/* trailing spaces */
					*d++ = 0;
					p = my_alloc(sizeof(StrList),1);
					p->str = d0;
					p->next = actuals;
					actuals = p;
					d0 = d;
					nact++;
					if (tokp->token[0] == ',') {
						tokp = skipbl(lmode);	/* heading spaces*/
						continue;
					}
				}
				if (tokp->token[0] == ')') plvl--;
				break;
			default:
			        if ((tokp->val == TK_NL)  ||
				    (tokp->val == TK_BOS) ||
				    (tokp->val == TK_EOC))
				{
				        if (!(lmode & MOD_COM)) {
					        set_pos(linesav);
						fppmess(ERR_LONG2, mname);
					}
					unget_tok(tokp);
					outfl--;
					return 0;
				}
				else if (sbfl && is_alpha0(tokp->token[0])) 
					sb_mactual_ref(tokp->token);
				break;
			}
			if (plvl == -1) break;
			if (!loc_errfl) {	/* text overfull */
				if (tokp->length > sizeof(text) - (d - text)-1) {
				        if (!(lmode & MOD_COM))
					        fppmess(ERR_LONG1);		
					loc_errfl = 1;
				}
				else {
					strcpy(d, tokp->token);
					d += tokp->length;
				}
			}
			tokp = get_token(lmode);
		}
		if (sbfl) sb_mref(mname, 1, 1);
		if (loc_errfl) {
			/* skiptok(tokp); */
			outfl--;
			return 0;
		}
	}
	else if (sbfl) sb_mref(mname, 0, 1);
	if (nact != nform && !(lmode & MOD_COM)) {
		set_pos(linesav);
		fppmess(ERR_NOMATCH);
	}
lab1:   n = line - linesav;
	/* skiptok(tokp); */
	ungetsym(SYM_EOMS);
	while (n--) {
	        ungetsym('\n');
		ungetsym('\\');
	}

	for (;;) {
		s1 = s--;
		while (*s && ((*s & 0xff) != SYM_CTRL)) 
			s--;
		ungetstr(s+1, (s1 - s) - 1);
		if (!*s) 
			break;

		n = *--s & 0xff;
		if (n == SYM_CTRL) {
                        ungetsym(SYM_CTRL);
		        continue;
		}

		ungetsym(SYM_EF);
		if (n <= nact) {
			p = actuals;
			n = nact - n;
			while (n--) p = p->next;
			ungetstr(p->str, strlen(p->str)); 
		}
		ungetsym(SYM_BF);
	}
	sympush(symp);
	outfl--;
	return 0;
}

/************************************************************** Directives */

static void
fpp_define(unsigned lmode) {
	char	*mname,*mval;
	int	npar;
	SymPtr	sym;
	StrList *formals,*p;
	int	i, n;
	char	body[BUFSIZE];
	char	*d,*d1;
	Token   *tokp;

	tokp = skipbl(lmode);
	if(!is_alpha0(tokp->token[0])) {
		fppmess(ERR_BADNAM);
		return;
	}
	mname = my_strdup(tokp->token);
	sym = symget(tokp->token, CL_NM|CL_KEY|CL_FOP);
	if (sym && (symtype(sym) & (CL_KEY|CL_FOP))) {
		fppmess(WARN_DUBL1,mname);
		sym = NULL;
	}
	outfl++;
	tokp = get_token(lmode);
	npar = -1;
	formals = NULL;
	if (tokp->token[0] == '(') {		/* macro with arguments */
		if (sbfl) sb_mdef(mname, 1);
		npar = 0;
		for (;;) {
			tokp = skipbl(lmode);
			if (is_alpha0(tokp->token[0])) {
				if (sbfl) sb_mformal_def(tokp->token);
				p = my_alloc(sizeof(StrList),1);
				p->str = my_strdup(tokp->token);
				p->next = formals;
				formals = p;
				npar++;
				tokp = skipbl(lmode);
				if (tokp->token[0] == ',')
					continue;
			}
			if (tokp->token[0] == ')') {
				tokp = skipbl(lmode);
				break;
			}
			else {
				fppmess(ERR_BADS,"define");
				goto exit;
			}
		}
	}
	else {
		if (sbfl) sb_mdef(mname, 0);
		if (is_blank(tokp->token[0])) 
			tokp = skipbl(lmode);
	}
	d = body;
	*d++ = 'b';
	for (;;tokp = get_token(lmode)) {
		if (tokp->val == TK_NL) break;
		if (is_alpha0(tokp->token[0])) {
			n = 0;
			if (npar > 0) {
				p = formals;
				for (n=npar,p=formals;n;n--,p=p->next) {
					if (!strcmp(p->str,tokp->token))
						break;
				}
			}
			if (n) {
				CHECK(p);
				if (sbfl) sb_mformal_ref(tokp->token);
				if (d-body > sizeof(body)-4) { 
					fppmess(ERR_LONG);
					goto exit;
				}
				*d++ = n;
				*d++ = SYM_CTRL;
				continue;
			}
			else if (sbfl) sb_mnonformal_ref(tokp->token);
		}
		if (d-body > sizeof(body) - tokp->length -2) {
			fppmess(ERR_LONG);
			goto exit;
		}
		d1 = tokp->token;
		for (i=0; i < tokp->length; i++) {
		        if ((*d1 & 0xff) == SYM_CTRL) {

			        /* Double SYM_CTRL */
			        *d++ = SYM_CTRL;
			}
		        *d++ = *d1++;
		}
	}
	while (is_blank(d[-1])) d--;	/* remove trailng spaces */
	*d++ = npar & 0xff;
	*d = 0;
	mval = my_strdup(body);
	*mval = 0;
	mval += d - body;
	if (sym) {
		d = symvals(sym) - 1;
		d1 = mval - 1;
		do {
			if (*d != *d1) {
				fppmess(WARN_DUBL,mname);
				break;
			}
		}
		while (*d-- && *d1--);
		symsetvals(sym,mval);
	}
	else 
		symsetnm(mname,mval);
	if (sbfl) sb_mdef_end();
	CHECK(tokp->val == TK_NL);
exit:	outfl--;
        return;
}

static void
fpp_include(unsigned lmode) {
	char	c;
	InFile	*incf;
	char	fname[BUFSIZE];
	char	fullname[BUFSIZE];
	StrList *paths;
	Token   *tokp;

	paths = &include_path;
	fname[0] = 0;
	dosubstfl++;		/* enable to use macro */
	tokp = skipbl(lmode);
	dosubstfl--;
	outfl++;
	c = tokp->token[0];
	if (c == '\"' || c == '\'') {
		strncpy(fname, tokp->token+1, tokp->length-2); 
		fname[tokp->length-2] = 0;
		if (sbfl) sb_include(0,fname);
	}
	else if (c == '<') {
		for (;;) {
			tokp = get_token(0);
			if (tokp->token[0] == '>') break;
			strcat(fname,tokp->token);
		}
		paths = paths->next;
		if (sbfl) sb_include(1,fname);
	}
	else {
		fppmess(ERR_BADS,"#include");
		tokp = skipnl(tokp);
		outfl--;
		return;
	}
	tokp = skipnl(tokp);
	/* skiptok(tokp); */
	outfl--;
	if (fname[0] == '/' ) {	/* absolute name */
		if (access(fname,F_OK) != 0) {
			fppmess(ERR_NOINC,fname);
			return;
		}
		strcpy(fullname,fname);
	}
	else {
		for (;paths;paths=paths->next) {
			strcpy(fullname,paths->str);
			strcat(fullname,"/");
			strcat(fullname,fname);
			if (access(fullname,F_OK)==0)
				break;
		}
		if (paths == NULL) {
			fppmess(ERR_NOINC,fname);
			return;
		}
	}

	incf = my_fopen(fullname,"r");
	incf->ifnode = curif;
	curbuf->psaved = curp;
	curfile->inbuf = curbuf;
	curfile->line = line;
	line = 1;
	incf->popfile = curfile;
	curfile = incf;
	curbuf = curfile->inbuf;
	curp = INBUF_BEG(curbuf);
	outpos(1, PUSHINCL);
	
	if (mdepfl) 
		md_fwrite(fullname);

	return;

}

static void
fpp_if(unsigned lmode) {
	IfNode	*tmp;

	if (curif)
		curif->flvlsaved = falselvl;
	tmp = my_alloc(sizeof(IfNode),1);
	tmp->popif = curif;
	tmp->ifline = line;
	curif = tmp;

	if (falselvl == 0) {
		if (sbfl) sb_if();
		dosubstfl++;
		if (!yyparse()) {
			falselvl++;
			inactive = 1;
		}
		dosubstfl--;
		if (sbfl) sb_if_end(!falselvl);
	}
	else 
		falselvl++;
}

static void
fpp_ifdef(unsigned lmode) {
	IfNode	*tmp;
	Token    *tokp;

	if (curif)
		curif->flvlsaved = falselvl;
	tmp = my_alloc(sizeof(IfNode),1);
	tmp->popif = curif;
	tmp->ifline = line;
	curif = tmp;

	if (falselvl == 0) {
		tokp = skipbl(lmode);
		if (symget(tokp->token,CL_NM) == NULL) {
			falselvl++;
			inactive = 1;
		}
		if (sbfl) sb_ifdef(!falselvl, tokp->token);
	}
	else falselvl++;
}

static void
fpp_ifndef(unsigned lmode) {
	IfNode	*tmp;
	Token   *tokp;

	if (curif)
		curif->flvlsaved = falselvl;
	tmp = my_alloc(sizeof(IfNode),1);
	tmp->popif = curif;
	tmp->ifline = line;
	curif = tmp;

	if (falselvl == 0) {
		tokp = skipbl(lmode);
		if (symget(tokp->token,CL_NM)) {
			falselvl++;
			inactive = 1;
		}
		if (sbfl) sb_ifndef(!falselvl, tokp->token);
	}
	else falselvl++;
}

static void
fpp_else() {
	if (curif == NULL) {
		fppmess(ERR_NOIF1);
		return;
	}
	if (curif->elsefl) {
		fppmess(ERR_ELSE);
		return;
	}
	else curif->elsefl = 1;
	if (falselvl == 0) {
		falselvl++;
		inactive = 1;
	}	
	else if (falselvl == 1) {
		falselvl--;
		inactive = 0;
		if (sbfl) sb_set_inactive(inactive);
	}
}

static void
fpp_elif(unsigned lmode) {
	if (curif == NULL) {
		fppmess(ERR_NOIF2);
		return;
	}
	if (curif->elsefl) {
		fppmess(ERR_ELSE1);
		return;
	}
	if (falselvl == 0) {
		falselvl += 2;
		inactive = 1;
		if (sbfl) sb_set_inactive(1);
	}	
	else if (falselvl == 1) {
		if (sbfl) {
			sb_if();
			sb_set_inactive(0);
		}
		dosubstfl++;
		if (yyparse()) {
			falselvl--;
			inactive = 0;
		}
		dosubstfl--;
		if (sbfl) sb_if_end(!falselvl);
	}
}

static void
fpp_endif() {
	IfNode	*tmp;

	if (curif == NULL) {
		fppmess(ERR_NOIF);
		return;
	}
	tmp = curif->popif;
	free(curif);
	curif = tmp;
	if (curif == NULL) {
		falselvl = 0;
		inactive = 0;
	}
	else {
		falselvl = curif->flvlsaved;
		inactive = (falselvl == 1);
	}
	if (sbfl && !inactive) sb_set_inactive(0);
}

static void
fpp_line(unsigned lmode) {
	Token   *tokp;

	if (ignorelinefl) {
		tokp = get_token(lmode);
		tokp = skipnl(tokp);
		return;
	}

	dosubstfl++;	/* allow to expand macros */
	tokp = skipbl(lmode);
	if (tokp->val != TK_NUM) {
		fppmess(ERR_BADS,"#line");
		goto exit;
	}
	strtoi(tokp->token,(int *)&line,10);
	tokp = skipbl(lmode);
	if (tokp->token[0] == '"') {
	        if (tokp->token[tokp->length - 1] == '"')
		        tokp->token[--tokp->length] = 0;
		if ( tokp->length != 0 &&
		     strcmp(curfile->fname,tokp->token+1) )
		{
			free(curfile->fname);
			curfile->fname = my_strdup(tokp->token+1);
			ofileid = (unsigned)-1;
		}
		tokp = skipbl(lmode);
		if ((tokp->val == TK_NUM) &&
		    ( !strcmp(tokp->token, PUSHINCL) ||
		      !strcmp(tokp->token, POPINCL) ) ) {

			outpos(line, tokp->token);
		}
	}
	line--; /* to compensate the following newline */

exit:	dosubstfl--;
	tokp = skipnl(tokp);
	return;
}

static void
directive() {
	unsigned lmode;
	int	n;
	SymPtr	sym;
	char	locbuf[BUFSIZE];
	Token   *tokp;

	line_width = LINE_WIDTH_MAX;
	lmode = (mmode & ~(MOD_SPACE | MOD_CONT)) | MOD_FPP | MOD_NOGRAM;
	if (!fixedformfl) 		/* why not to use & for continuation? */
		lmode |= MOD_CONT;

	outfl++;
	tokp = skipbl(lmode);

	if (tokp->val == TK_NL) {

		/* Just ignore empty # directve */
	        goto exit;
	}
	else if (tokp->val == TK_NUM) {

		/* This is #<line> [<file>] directive
		 * If we haven't been told to ignore them 
		 * then process it.
		 */
		unget_tok(tokp);
		fpp_line(lmode);
		outfl--;
		return;
	}
	locbuf[0] = '#';
	strcpy(locbuf+1,tokp->token);
	sym = symget(locbuf,CL_FPP);
	if (sym == NULL) {
		fppmess(ERR_FPP);
		goto exit;
	}
	n = symvali(sym);
	switch (n) {
	case FPPDEF: 
		if (falselvl == 0) 
			fpp_define(lmode);
		break;
	case FPPUNDEF: 
		if (falselvl == 0) {
			tokp = skipbl(lmode);
			symdel(tokp->token);
			if (sbfl) sb_undef(tokp->token);
		}
		break;
	case FPPINCL:
		if (falselvl == 0)  {
			fpp_include(lmode);
			outfl--;
			return;
		}
		break;
	case FPPIF: 
		fpp_if(lmode);
		break;
	case FPPELIF: 
		fpp_elif(lmode);
		break;
	case FPPELSE: 
		fpp_else();
		break;
	case FPPENDIF:
		fpp_endif(); 
		break;
	case FPPIFDEF:
		fpp_ifdef(lmode);
		break;
	case FPPIFNDEF: 
		fpp_ifndef(lmode);
		break;
	case FPPLINE:
		fpp_line(lmode);
		outfl--;
		return;
		break;
	default:	
		fppmess(ERR_FPP);
		break;
	}

exit:	tokp = skipnl(tokp);
	outfl--;

	/* Output newline */
	outtok(tokp);
	return;
}

/************************************************************** Process */

void 
process() {
	InFile	*tmpf;
	Token   *tokp;

	for (;;) {
	        if (st_is(ST_NL)) {
			line_width = line_width0;
			if (falselvl) line_width = LINE_WIDTH_MAX;
			tokp = get_token_nl(mmode);
		}
		else 
			tokp = get_token(mmode);

		switch (tokp->val) {
		case TK_NL:
		        if (tokp->token[0] == '\n') {
			        outtok(tokp);
				break;
			}

		        /* Output all remaining comments */
		        if (comments && !outfl && !falselvl) {
			        for (;comments;) {
				        Comment *prev;

					outcomm(comments);
					prev = comments;
					comments = comments->next;
					free(prev);
				}
			}

			/* Adjust output if needed */
			if (ocolm != 0) {
			        my_fwrite("\n",1);
				oline++; ocolm = 0;
			}

			while (curif != curfile->ifnode) {
				fppmess(ERR_NOEND,curif->ifline);
				fpp_endif();
			}
			if (sbfl) sb_flush_ids(1);

			if (curfile->popfile) {
				tmpf = curfile;
				curfile = curfile->popfile;
				my_fclose(tmpf);
				curbuf = curfile->inbuf;
				curp = curbuf->psaved;
				include_path.str = curfile->dir;
				line = curfile->line;
				colm = 0;
			}
			else 	{
				my_fclose(curfile);
				curfile = NULL;
				my_fclose(outfile);
				outfile = NULL;
				return;
			}
			outpos(line, POPINCL);
			break;
		case TK_FPP: 
			if (st_is(ST_FPP)) {

			        /* Output comments if any */
			        if (comments) {
				        for (;comments;) {
					        Comment *prev;

						outcomm(comments);
						prev = comments;
						comments = comments->next;
						free(prev);
					}
				}
				directive();
			}
			else {
			        outtok(tokp);
			}
			break;
		default: 
		        outtok(tokp);
			break;
		}
	}
}

/************************************************************** Init */

StrList *include_path_last = &include_path;

int
add_path(char *p) {

	include_path_last->next = my_alloc(1,sizeof(StrList));
	include_path_last = include_path_last->next;
	include_path_last->str = my_strdup(p);
	return 0;
}

void
add_name(char *name, char *val) {
	SymPtr	sym;
	char	locbuf[BUFSIZE];
	char	*s;
	int	len;

	if (mmode & MOD_LCASE)
		tolowcase(name);
	sym = symget(name,CL_NM|CL_KEY|CL_FOP);        /* it does like fpp_define */
	if (sym && (symtype(sym) & (CL_KEY|CL_FOP))) {
		fppmess(WARN_DUBL1,name);
		sym = NULL;
	}
	locbuf[0]='b';		
	len = strlen(val);
	strcpy(locbuf+1,val);
	locbuf[1+len] = -1;
	locbuf[2+len] = '\0';
	s = my_strdup(locbuf);
	*s = '\0';
	s += len+2;
	if (sym) {
		char *d0 = symvals(sym) - 1;
		char *d1 = s - 1;

		do {
			if (*d0 != *d1) {
				fppmess(WARN_DUBL,name);
				break;
			}
		}
		while (*d0-- && *d1--);
		symsetvals(sym,s);
	}
	else {
		symsetnm(name,s);
	}
}

void
add_defs(StrList *defs) {
	StrList	*prev, *cur, *next;
	char	*name, *val;

	if (predefinefl) {
#ifdef sun
	add_name("sun","1");
#endif
#ifdef sparc
	add_name("sparc","1");
#endif
#ifdef unix
	add_name("unix","1");
#endif
	}

	cur = defs; prev = NULL;		/* list reverse */
	while (cur){ 		
		next = cur->next;
		cur->next = prev;
		prev = cur;
		cur = next;
	}
	cur = prev;
	while (cur) {
		name = cur->str;
		val = strchr(name,'=');
		if (val) {
			*val++ = 0;
			add_name(name,val);
		}
		else {
			add_name(name,"1");
		}
		prev = cur;
		cur = cur->next;
		free (prev);
	}
}

void
add_undefs(StrList *undefs) {
	StrList	*prev, *cur, *next;

	cur = undefs; prev = NULL;		/* list reverse */
	while (cur){ 		
		next = cur->next;
		cur->next = prev;
		prev = cur;
		cur = next;
	}
	cur = prev;
	while (cur) {
		if (mmode & MOD_LCASE)
			tolowcase(cur->str);
		symdel(cur->str);
		prev = cur;
		cur = cur->next;
		free (prev);
	}
}

void init_fpp();
void init_fop();
void init77();
void init90();

void
init1(char *fin, char *fout, StrList *defs, StrList *undefs) {
	char	*s;

	if ( mdepfl && fin == NULL ) 
		fppmess(FERR_MWOFN);
	if ( mdepfl && sbfl )
		sbfl = 0;
	if ( sbfl && linefl )
		linefl = 0;

	if (fin != NULL) { 
		s = fin;
		while (*s) s++;
		while (s!=fin && *s!='.') s--;
		if (*s=='.') {
			if (!strcmp(s,F77EXTENSION)) {
				if (f77fl == -1) 
					f77fl = 1;
			}
		}
	}
	if (f77fl == -1) 
		f77fl = 0;
	if (f77fl) {
		fixedformfl = 1;
		/* linefl = !linefl;	*/
	}
	else {
		if (fixedformfl == -1)
			fixedformfl = 0;
		/* linefl = linefl;  for symmetry */ 
	}

	/* Set extended line width for free form
	 * and then set output line width to be
	 * equal to that of input whatever it is
	 */
	if (!fixedformfl) {
		line_width0 = LINE_WIDTH_EXT;
	}
	oline_width = line_width0;

	if (!fixedformfl) {
		mmode &= ~MOD_SPACE;
	}
	linefl = !linefl;

	init_fpp();
	init_fop();
	if (f77fl) {
		init77();
		st_init(0);
	}
	else if (fixedformfl) {
		init90();
		st_init(1);
	}
	else {
		init90();
		st_init(2);
	}
	add_defs(defs);
	add_undefs(undefs);

	if (fin == NULL) { 
		curfile = fopen_stdfile(stdin);
	}
	else {
		curfile = my_fopen(fin,"r");
	}
	if (fout == NULL) { 
		outfile = fopen_stdfile(stdout);
	}
	else {
		outfile = my_fopen(fout,"w");
	}

	if ( mdepfl ) {
		char	*t;

		mfile = outfile;
		outfile = my_fopen("/dev/null","w");
		mdfname = my_strdup(curfile->fname);
		t = strrchr(mdfname,'.');
		*t = '\0';
		md_fwrite(fin);
	}

	line = 1;
	colm = 0;
	curfile->ifnode = NULL;
	include_path.str = curfile->dir;
	curbuf = curfile->inbuf;
	curp = INBUF_BEG(curbuf);
	curif = NULL;
	outfl = 0;
	falselvl = 0;
 	outpos(1, PUSHINCL);	
}

void
init0() {
	char	*s;

/*	mmode = MOD_CONT | MOD_LCASE;	*/
	mmode = MOD_CONT; 		/* now case-sensitive by default */
	line_width0 = LINE_WIDTH_STD;
	f77fl = -1;
	fixedformfl = -1;
	linefl = 0;
	sbfl = 0;
	predefinefl = 1;
	substfl = 2;                    /* by default, macros are expanded everywhere */
	rmcommfl = 0;
	mdepfl = 0;
	onequalsym = NULL;
	ctrl_mode = 0;                  /* don't process control chars */
	curfile = my_alloc(sizeof(InFile),1);
	curfile->fname = my_strdup("fpp");
	inactive = 0;

	s = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz_$";
	while (*s) chars[*s++] |= S_AL;
	s = "0123456789";
	while (*s) chars[*s++] |= S_NM;
	s = " \t\v\f\r";
	while (*s) chars[*s++] |= S_BL;
}

void
init_fpp() {
	static	char dyn[2] = {FPPDYNMACRO,0};

	symsetkw("#DEFINE",	FPPDEF		,CL_FPP);
	symsetkw("#UNDEF",	FPPUNDEF	,CL_FPP);
	symsetkw("#INCLUDE",	FPPINCL		,CL_FPP);
	symsetkw("#IF",		FPPIF		,CL_FPP);
	symsetkw("#ELIF",	FPPELIF		,CL_FPP);
	symsetkw("#ELSE",	FPPELSE		,CL_FPP);
	symsetkw("#ENDIF",	FPPENDIF	,CL_FPP);
	symsetkw("#IFDEF",	FPPIFDEF	,CL_FPP);
	symsetkw("#IFNDEF",	FPPIFNDEF	,CL_FPP);
	symsetkw("#LINE",	FPPLINE		,CL_FPP);

	symsetkw("__FILE__",	0	,CL_NM);
	symsetkw("__LINE__",	0	,CL_NM);

	filesymp = symget("__file__", CL_NM);
	CHECK(filesymp);
	FILEsymp = symget("__FILE__", CL_NM);
	if (FILEsymp == NULL) 
		FILEsymp = filesymp;
	symsetvals( filesymp, dyn);
	symsetvals( FILEsymp, dyn);
	linesymp = symget("__line__", CL_NM);
	CHECK(linesymp);
	LINEsymp = symget("__LINE__", CL_NM);
	if (LINEsymp == NULL) 
		LINEsymp = linesymp;
	symsetvals( linesymp, dyn);
	symsetvals( LINEsymp, dyn);
}

void
init_fop() {
	symsetkw("TRUE",	FTN_TRUE	,CL_FOP);
	symsetkw("FALSE",	FTN_FALSE	,CL_FOP);
	symsetkw("EQ",		FTN_EQ		,CL_FOP);
	symsetkw("NE",		FTN_NE		,CL_FOP);
	symsetkw("LT",		FTN_LT		,CL_FOP);
	symsetkw("LE",		FTN_LE		,CL_FOP);
	symsetkw("GT",		FTN_GT		,CL_FOP);
	symsetkw("GE",		FTN_GE		,CL_FOP);
	symsetkw("AND",		FTN_AND		,CL_FOP);
	symsetkw("OR",		FTN_OR		,CL_FOP);
	symsetkw("NEQV",	FTN_NEQV	,CL_FOP);
	symsetkw("XOR",		FTN_XOR		,CL_FOP);
	symsetkw("EQV",		FTN_EQV		,CL_FOP);
	symsetkw("NOT",		FTN_NOT		,CL_FOP);
	symsetkw("PRAGMA",	FTN_PRAGMA	,CL_FOP);
}

void
init77() {
	symsetkw("ACCEPT",	FTN_ACCEPT	,CL_KEY);
	symsetkw("ASSIGN",	FTN_ASSIGN	,CL_KEY);
	symsetkw("TO",		FTN_TO		,CL_FOP);
	symsetkw("AUTOMATIC",	FTN_AUTOMATIC	,CL_KEY);
	symsetkw("BACKSPACE",	FTN_BACKSPACE	,CL_KEY);
	symsetkw("BLOCKDATA",	FTN_BLOCKDATA	,CL_KEY);
	symsetkw("DATA",	FTN_DATA	,CL_KEY);
	symsetkw("BYTE",	FTN_BYTE	,CL_KEY);
	symsetkw("CALL",	FTN_CALL	,CL_KEY);
	symsetkw("CHARACTER",	FTN_CHARACTER	,CL_KEY);
	symsetkw("CLOSE",	FTN_CLOSE	,CL_KEY);
	symsetkw("COMMON",	FTN_COMMON	,CL_KEY);
	symsetkw("COMPLEX",	FTN_COMPLEX	,CL_KEY);
	symsetkw("CONTINUE",	FTN_CONTINUE	,CL_KEY);
	symsetkw("DECODE",	FTN_DECODE	,CL_KEY);
	symsetkw("DIMENSION",	FTN_DIMENSION	,CL_KEY);
	symsetkw("DO",		FTN_DO		,CL_KEY);
	symsetkw("DOWHILE",	FTN_DOWHILE	,CL_KEY);
	symsetkw("DOUBLECOMPLEX",	FTN_DOUBLECOMP	,CL_KEY); 
	symsetkw("DOUBLEPRECISION",	FTN_DOUBLEPREC	,CL_KEY);
	symsetkw("ELSE",	FTN_ELSE	,CL_KEY);
	symsetkw("ELSEIF",	FTN_ELSEIF	,CL_KEY);
	symsetkw("END",		FTN_END		,CL_KEY);
	symsetkw("ENDDO",	FTN_ENDDO	,CL_KEY);
	symsetkw("ENDFILE",	FTN_ENDFILE	,CL_KEY);
	symsetkw("ENDIF",	FTN_ENDIF	,CL_KEY);
	symsetkw("ENDMAP",	FTN_ENDMAP	,CL_KEY);
	symsetkw("ENDSTRUCTURE",FTN_ENDSTRUCTURE,CL_KEY);
	symsetkw("ENDUNION",	FTN_ENDUNION	,CL_KEY);
	symsetkw("ENCODE",	FTN_ENCODE	,CL_KEY);
	symsetkw("ENTRY",	FTN_ENTRY	,CL_KEY);
	symsetkw("EQUIVALENCE",	FTN_EQUIVALENCE	,CL_KEY);
	symsetkw("EXTERNAL",	FTN_EXTERNAL	,CL_KEY);
	symsetkw("FORMAT",	FTN_FORMAT	,CL_KEY);
	symsetkw("FUNCTION",	FTN_FUNCTION	,CL_KEY);
	symsetkw("GOTO",	FTN_GOTO	,CL_KEY);
	symsetkw("IF",		FTN_IF		,CL_KEY);
	symsetkw("IMPLICIT",	FTN_IMPLICIT	,CL_KEY);
	symsetkw("INCLUDE",	FTN_INCLUDE	,CL_KEY); 
	symsetkw("INQUIRE",	FTN_INQUIRE	,CL_KEY);
	symsetkw("INTEGER",	FTN_INTEGER	,CL_KEY);
	symsetkw("INTRINSIC",	FTN_INTRINSIC	,CL_KEY);
	symsetkw("LOGICAL",	FTN_LOGICAL	,CL_KEY);
	symsetkw("MAP",		FTN_MAP		,CL_KEY);
	symsetkw("NAMELIST",	FTN_NAMELIST	,CL_KEY);
	symsetkw("OPEN",	FTN_OPEN	,CL_KEY);
	symsetkw("OPTIONS",	FTN_OPTIONS	,CL_KEY);
	symsetkw("PARAMETER",	FTN_PARAMETER	,CL_KEY);
	symsetkw("PAUSE",	FTN_PAUSE	,CL_KEY);
	symsetkw("POINTER",	FTN_POINTER	,CL_KEY);
	symsetkw("PROGRAM",	FTN_PROGRAM	,CL_KEY);
	symsetkw("PRINT",	FTN_PRINT	,CL_KEY);
	symsetkw("READ",	FTN_READ	,CL_KEY);
	symsetkw("REAL",	FTN_REAL	,CL_KEY);
	symsetkw("RECORD",	FTN_RECORD	,CL_KEY);
	symsetkw("RETURN",	FTN_RETURN	,CL_KEY);
	symsetkw("REWIND",	FTN_REWIND	,CL_KEY);
	symsetkw("SAVE",	FTN_SAVE	,CL_KEY);
	symsetkw("STATIC",	FTN_STATIC	,CL_KEY);
	symsetkw("STOP",	FTN_STOP	,CL_KEY);
	symsetkw("STRUCTURE",	FTN_STRUCTURE	,CL_KEY);
	symsetkw("SUBROUTINE",	FTN_SUBROUTINE	,CL_KEY);
	symsetkw("TYPE",	FTN77_FST+63	,CL_KEY);
	symsetkw("UNION",	FTN_UNION	,CL_KEY);
	symsetkw("VIRTUAL",	FTN_VIRTUAL	,CL_KEY);
	symsetkw("VOLATILE",	FTN_VOLATILE	,CL_KEY);
	symsetkw("WRITE",	FTN_WRITE	,CL_KEY);

	symsetkw("UNIT",	FTN77_FST+100	,CL_KEY);
	symsetkw("FILE",	FTN77_FST+101	,CL_KEY);
	symsetkw("ACCESS",	FTN77_FST+102	,CL_KEY);
	symsetkw("BLANK",	FTN77_FST+103	,CL_KEY);
	symsetkw("ERR",		FTN77_FST+104	,CL_KEY);
	symsetkw("FORM",	FTN77_FST+105	,CL_KEY);
	symsetkw("IOSTAT",	FTN77_FST+106	,CL_KEY);
	symsetkw("RECL",	FTN77_FST+107	,CL_KEY);
	symsetkw("STATUS",	FTN77_FST+108	,CL_KEY);
	symsetkw("FILEOPT",	FTN77_FST+109	,CL_KEY);
	symsetkw("FMT",		FTN77_FST+110	,CL_KEY);
	symsetkw("REC",		FTN77_FST+111	,CL_KEY);
	symsetkw("NML",		FTN77_FST+112	,CL_KEY);
	symsetkw("EXIST",	FTN77_FST+113	,CL_KEY);
	symsetkw("OPENED",	FTN77_FST+114	,CL_KEY);
	symsetkw("NAMED",	FTN77_FST+115	,CL_KEY);
	symsetkw("ACCESS",	FTN77_FST+116	,CL_KEY);
	symsetkw("SEQUENTIAL",	FTN77_FST+117	,CL_KEY);
	symsetkw("DIRECT",	FTN77_FST+118	,CL_KEY);
	symsetkw("FORM",	FTN77_FST+119	,CL_KEY);
	symsetkw("FORMATTED",	FTN77_FST+120	,CL_KEY);
	symsetkw("UNFORMATTED",	FTN77_FST+121	,CL_KEY);
	symsetkw("NAME",	FTN77_FST+122	,CL_KEY);
	symsetkw("OSTAT",	FTN77_FST+123	,CL_KEY);
	symsetkw("NUMBER",	FTN77_FST+124	,CL_KEY);
	symsetkw("NEXTREC",	FTN77_FST+125	,CL_KEY);

	symsetkw("DOUBLE",	FTN77_FST+126	,CL_FOP);
	symsetkw("PRECISION",	FTN77_FST+127	,CL_FOP);
	symsetkw("BLOCK",	FTN77_FST+128	,CL_FOP);
	symsetkw("WHILE",	FTN77_FST+129	,CL_FOP);
}

void
init90() {
	symsetkw("INCLUDE",	FTN90_FST	,CL_KEY);
	symsetkw("TYPE",	FTN_TYPE	,CL_KEY);
	symsetkw("PRIVATE",	FTN90_FST+2	,CL_KEY);
	symsetkw("SEQUENCE",	FTN90_FST+3	,CL_KEY);
	symsetkw("ENDTYPE",	FTN90_FST+4	,CL_KEY);
	symsetkw("POINTER",	FTN90_FST+5	,CL_KEY);
	symsetkw("DIMENSION",	FTN90_FST+6	,CL_KEY);
	symsetkw("INTEGER",	FTN_INTEGER	,CL_KEY);
	symsetkw("REAL",	FTN_REAL	,CL_KEY);
	symsetkw("DOUBLEPRECISION", FTN_DOUBLEPREC ,CL_KEY);
	symsetkw("COMPLEX",	FTN_COMPLEX	,CL_KEY);
	symsetkw("CHARACTER",	FTN_CHARACTER	,CL_KEY);
	symsetkw("LOGICAL",	FTN_LOGICAL	,CL_KEY);
	symsetkw("PARAMETER",	FTN90_FST+13	,CL_KEY);
	symsetkw("ALLOCATABLE",	FTN90_FST+14	,CL_KEY);
	symsetkw("EXTERNAL",	FTN90_FST+15	,CL_KEY);
	symsetkw("INTENT",	FTN90_FST+16	,CL_KEY);
	symsetkw("INTRINSIC",	FTN90_FST+17	,CL_KEY);
	symsetkw("OPTIONAL",	FTN90_FST+18	,CL_KEY);
	symsetkw("SAVE",	FTN90_FST+19	,CL_KEY);
	symsetkw("TARGET",	FTN90_FST+20	,CL_KEY);
	symsetkw("PUBLIC",	FTN90_FST+21	,CL_KEY);
	symsetkw("IN",		FTN90_FST+22	,CL_FOP);
	symsetkw("OUT",		FTN90_FST+23	,CL_FOP);
	symsetkw("INOUT",	FTN90_FST+24	,CL_FOP);
	symsetkw("OPTIONAL",	FTN90_FST+25	,CL_KEY);
	symsetkw("DATA",	FTN90_FST+26	,CL_KEY);
	symsetkw("IMPLICIT",	FTN_IMPLICIT	,CL_KEY);
	symsetkw("IMPLICITNONE",FTN90_FST+28	,CL_KEY);
	symsetkw("NAMELIST",	FTN90_FST+29	,CL_KEY);
	symsetkw("EQUIVALENCE",	FTN90_FST+30	,CL_KEY);
	symsetkw("COMMON",	FTN90_FST+31	,CL_KEY);
	symsetkw("ALLOCATE",	FTN90_FST+32	,CL_KEY);
	symsetkw("STAT",	FTN90_FST+33	,CL_FOP);
	symsetkw("NULLIFY",	FTN90_FST+34	,CL_KEY);
	symsetkw("DEALLOCATE",	FTN90_FST+35	,CL_KEY);
	symsetkw("WHERE",	FTN90_FST+36	,CL_KEY);
	symsetkw("ELSEWHERE",	FTN90_FST+37	,CL_KEY);
	symsetkw("ENDWHERE",	FTN90_FST+38	,CL_KEY);
	symsetkw("IF",		FTN90_FST+39	,CL_KEY);
	symsetkw("THEN",	FTN90_FST+40	,CL_KEY);
	symsetkw("ELSE",	FTN90_FST+41	,CL_KEY);
	symsetkw("ELSEIF",	FTN90_FST+42	,CL_KEY);
	symsetkw("ENDIF",	FTN90_FST+43	,CL_KEY);
	symsetkw("SELECTCASE",	FTN90_FST+44	,CL_KEY);
	symsetkw("CASE",	FTN90_FST+45	,CL_KEY);
	symsetkw("ENDSELECT",	FTN90_FST+46	,CL_KEY);
	symsetkw("DEFAULT",	FTN90_FST+47	,CL_KEY);
	symsetkw("DO",		FTN_DO		,CL_KEY);
	symsetkw("WHILE",	FTN90_FST+49	,CL_KEY);
	symsetkw("ENDDO",	FTN90_FST+50	,CL_KEY);
	symsetkw("CYCLE",	FTN90_FST+51	,CL_KEY);
	symsetkw("EXIT",	FTN90_FST+52	,CL_KEY);
	symsetkw("GOTO",	FTN90_FST+53	,CL_KEY);
	symsetkw("ASSIGN",	FTN_ASSIGN	,CL_KEY);
	symsetkw("TO",		FTN90_FST+55	,CL_FOP);
	symsetkw("CONTINUE",	FTN90_FST+56	,CL_KEY);
	symsetkw("STOP",	FTN90_FST+57	,CL_KEY);
	symsetkw("PAUSE",	FTN90_FST+58	,CL_KEY);
	symsetkw("OPEN",	FTN90_FST+59	,CL_KEY);
	symsetkw("CLOSE",	FTN90_FST+60	,CL_KEY);
	symsetkw("READ",	FTN_READ	,CL_KEY);
	symsetkw("WRITE",	FTN_WRITE	,CL_KEY);
	symsetkw("PRINT",	FTN90_FST+63	,CL_KEY);
	symsetkw("BACKSPACE",	FTN90_FST+64	,CL_KEY);
	symsetkw("ENDFILE",	FTN90_FST+65	,CL_KEY);
	symsetkw("REWIND",	FTN90_FST+66	,CL_KEY);
	symsetkw("INQUIRE",	FTN90_FST+67	,CL_KEY);
	symsetkw("FORMAT",	FTN_FORMAT	,CL_KEY);
	symsetkw("PROGRAM",	FTN90_FST+69	,CL_KEY);
	symsetkw("ENDPROGRAM",	FTN90_FST+70	,CL_KEY);
	symsetkw("MODULE",	FTN90_FST+71	,CL_KEY);
	symsetkw("ENDMODULE",	FTN90_FST+72	,CL_KEY);
	symsetkw("END",		FTN90_FST+73	,CL_KEY);
	symsetkw("USE",		FTN90_FST+74	,CL_KEY);
	symsetkw("ONLY",	FTN90_FST+75	,CL_FOP);
	symsetkw("BLOCKDATA",	FTN90_FST+76	,CL_KEY);
	symsetkw("ENDBLOCKDATA",FTN90_FST+77	,CL_KEY);
	symsetkw("INTERFACE",	FTN90_FST+78	,CL_KEY);
	symsetkw("ENDINTERFACE",FTN90_FST+79	,CL_KEY);
	symsetkw("MODULEPROCEDURE",FTN90_FST+80	,CL_KEY);
	symsetkw("OPERATOR",	FTN90_FST+81	,CL_KEY);
	symsetkw("ASSIGNMENT",	FTN90_FST+82	,CL_KEY);
	symsetkw("CALL",	FTN90_FST+83	,CL_KEY);
	symsetkw("FUNCTION",	FTN90_FST+84	,CL_KEY);
	symsetkw("RESULT",	FTN90_FST+85	,CL_FOP);
	symsetkw("RECURSIVE",	FTN90_FST+86	,CL_KEY);
	symsetkw("ENDFUNCTION",	FTN90_FST+87	,CL_KEY);
	symsetkw("SUBROUTINE",	FTN90_FST+88	,CL_KEY);
	symsetkw("ENDSUBROUTINE", FTN90_FST+89	,CL_KEY);
	symsetkw("ENTRY",	FTN90_FST+90	,CL_KEY);
	symsetkw("RETURN",	FTN90_FST+91	,CL_KEY);
	symsetkw("CONTAINS",	FTN90_FST+92	,CL_KEY);

	symsetkw("KIND",	FTN90_FST+93	,CL_FOP);
	symsetkw("LEN",		FTN90_FST+94	,CL_FOP);

	symsetkw("UNIT",	FTN90_FST+95	,CL_FOP);
	symsetkw("IOSTAT",	FTN90_FST+96	,CL_FOP);
	symsetkw("ERR",		FTN90_FST+97	,CL_FOP);
	symsetkw("FILE",	FTN90_FST+98	,CL_FOP);
	symsetkw("STATUS",	FTN90_FST+99	,CL_FOP);
	symsetkw("ACCESS",	FTN90_FST+100	,CL_FOP);
	symsetkw("FORM",	FTN90_FST+101	,CL_FOP);
	symsetkw("RECL",	FTN90_FST+102	,CL_FOP);
	symsetkw("BLANK",	FTN90_FST+103	,CL_FOP);
	symsetkw("POSITION",	FTN90_FST+104	,CL_FOP);
	symsetkw("ACTION",	FTN90_FST+105	,CL_FOP);
	symsetkw("DELIM",	FTN90_FST+106	,CL_FOP);
	symsetkw("PAD",		FTN90_FST+107	,CL_FOP);
	symsetkw("FMT",		FTN90_FST+108	,CL_FOP);
	symsetkw("NML",		FTN90_FST+109	,CL_FOP);
	symsetkw("REC",		FTN90_FST+110	,CL_FOP);
	symsetkw("ADVANCE",	FTN90_FST+111	,CL_FOP);
	symsetkw("SIZE",	FTN90_FST+112	,CL_FOP);
	symsetkw("EOR",		FTN90_FST+113	,CL_FOP);
	symsetkw("EXIST",	FTN90_FST+114	,CL_FOP);
	symsetkw("OPENED",	FTN90_FST+115	,CL_FOP);
	symsetkw("NUMBER",	FTN90_FST+116	,CL_FOP);
	symsetkw("NAMED",	FTN90_FST+117	,CL_FOP);
	symsetkw("NAME",	FTN90_FST+118	,CL_FOP);
	symsetkw("SEQUENTIAL",	FTN90_FST+119	,CL_FOP);
	symsetkw("DIRECT",	FTN90_FST+120	,CL_FOP);
	symsetkw("FORMATTED",	FTN90_FST+121	,CL_FOP);
	symsetkw("UNFORMATTED",	FTN90_FST+122	,CL_FOP);
	symsetkw("NEXTREC",	FTN90_FST+123	,CL_FOP);
	symsetkw("READWRITE",	FTN90_FST+124	,CL_FOP);

	symsetkw("NONE",	FTN90_FST+125	,CL_FOP);
	symsetkw("DOUBLE",	FTN90_FST+126	,CL_FOP);
	symsetkw("PRECISION",	FTN90_FST+127	,CL_FOP);
	symsetkw("SELECT",	FTN90_FST+128	,CL_FOP);
	symsetkw("PROCEDURE",	FTN90_FST+129	,CL_FOP);
	symsetkw("BLOCK",	FTN90_FST+130	,CL_FOP);
	
}
