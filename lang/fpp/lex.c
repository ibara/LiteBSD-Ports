/* @(#)lex.c 1.17 96/09/11 */
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

#include "fpp.h"
#include "service.h"
#include "symtab.h"
#include "rgram.h"
#include "lex.h"
#include "sb.h"

extern uchar	*curp;
extern Comment	*comments;

/* ltok is a static memory to hold the current token.
 * As long as we don't expect pushtok() to be called more 
 * than once the size is equal to 2.
 */
static Token    ltok[2];
static Token    *curtok = &ltok[0];
static int      cur_stmt_id = 0;        /* current statement id */
static int      sym_line;               /* current character lineno */
static char     cont_symbol;            /* symbol used in 6th column (fixed mode) */

#define nextch(c) \
        while (!*curp) refill();\
        c = *curp++;\
        if (c == SYM_CTRL && ctrl_mode) {\
	        while (!*curp) refill();\
		if (*curp != SYM_CTRL) {\
		        c = (c << 8) | *curp++;\
			ctrl_mode--;\
		}\
		else curp++;\
	}

#define unnextch(c) \
        *--curp = (char)c;\
        if (is_special(c)) {\
	        *--curp = SYM_CTRL;\
		ctrl_mode++;\
	}


static int
skip_comment( unsigned mode ) {
        wchar   c;
	int	i,n;
	int	locfl;
	int     lineno;
	char	*p;
	static char buffer[BUFSIZE];

loop:		locfl = 0;
		p = buffer;
		lineno = line;
		nextch(c);

		switch (c) {
		case 'c':
		case 'C':
		case 'd':
		case 'D':
		case '*':
			if (!f77fl && !fixedformfl) {
			        unnextch(c);
				return 0;
			}
			if (colm != 0) {
			        unnextch(c);
				return 0;
			}
			/* no break! */
		case '!':
			*p++ = (char)c;
			for (;;) {
				nextch(c);
				if (c == '\n') {
 					line++; colm=0;
 					*p++ = (char)c;
					break;
				}
				else if (c == SYM_EOF) {
 					colm=0;
 				        *p++ = '\n';
					break;
				}
				*p++ = (char)c;
			}
			*p = '\0';
			for (i=0;i<8;i++) {	/* we treat it as pragma if it has '$' */
				if (buffer[i] == '\0') break;
				if (buffer[i] == '$') {
					if (mode & MOD_LCASE)
						tolowcase(buffer);
					locfl = 1;
					break;
				}
			}
			break;
		case '\n':
		        if (colm != 0) {
			        unnextch(c);
				return 0;
			}
			*p++ = (char)c;
			line++;
			break;
		default:
		        if ( !is_blank(c) ) {
			        unnextch(c);
				return 0;
			}
			CHECK(colm==0);
			for (n = 1;;n++) {
				colm++; 
				*p++ = (char)c;
				nextch(c);
				if (c == '\n') {
					line++; colm=0;
					*p++ = (char)c;
					break;
				}
				else if (c == SYM_EOF) {
				        colm = 0;
				        *p++ = '\n';
					break;
				}
				else if (c == '!') {
				        if (!(fixedformfl && n == 5)) {

					        /* Skip the leading spaces. 
						 */
					        p = buffer;
						unnextch(c);
						goto loop;
					}
				}
				if (!is_blank(c)) {
				        unnextch(c);
					while (n--) ungetsym(*(--p));
					CHECK(colm==0);
					return 0;
				}
			}
			break;
		}

		*p = '\0';

		if ( !(mode & MOD_FPP) && !falselvl &&
		     (!rmcommfl || locfl) && (p != buffer)) {
			Comment	**t;

			/* Place t onto the end of the list */
			for (t=&comments; *t; t=&((*t)->next));

			(*t) = my_alloc(sizeof(Comment),1);
			(*t)->str = my_strdup(buffer);
			(*t)->next = NULL;
			(*t)->lineno = lineno;
			(*t)->length = p - buffer;
		}

	return 1;
}

static int
skipcont() {
	char	locbuf[6];
	char	*p = locbuf;
	wchar	c;
	int 	n;

	CHECK(colm==0);
	nextch(c);
	if (c == '&') {
		colm = 6;
		line_width = LINE_WIDTH_MAX;
		return 1;
	}
	else if (c == SYM_FPP) {
	        unnextch(c);
		return 0;
	}
	colm++;
	for (n=5;;n--) {
		if (c == '\t') {
		        *p++ = (char)c;
			nextch(c);
			if (is_num(c) && c != '0') {
				colm = 6;
				line_width = LINE_WIDTH_MAX;
				return 1;
			}
			else {
			        unnextch(c);
				ungetstr(locbuf, p - locbuf);
				return 0;
			}
		}
		else if (c == '/') {
		        *p++ = (char)c;
			nextch(c);
			if (c == '*') {
			        unnextch(c);
				ungetstr(locbuf, p - locbuf);
				return 0;
			}
			unnextch(c);
			c = *--p;
		}
		else if (c == '\n' || c == SYM_EOF) {
		        unnextch(c);
			ungetstr(locbuf ,p - locbuf);
			colm = 0;
			return 0;
		}
		*p++ = (char)c;
		if (n == 0) break;
		nextch(c); colm++;
	}
	if (is_blank(c) || c == '0') {
		ungetstr(locbuf, p - locbuf);
		return 0;
	}
	line_width = line_width0;
	return 1;
}

static int
skipcont90(unsigned mode) {
        wchar   c;
	int	locfl=0;
	char	locbuf[BUFSIZE];
	char	*p;

 	p = locbuf;

	if (mode & MOD_RAW) {
		for (;;) {
			nextch(c);
			switch (c) {
			case ' ' :
			case '\t': break;
			case '&' :
				if (locfl==1) {
					colm++;
					return 1;
				}
				else {
				        unnextch(c);
					ungetstr(locbuf, p - locbuf);
					return 0;
				}
				break;
			case '\n':
				if (!locfl) {
					locfl++;
					line++; colm = 0;
					p = locbuf;
					continue;
				}
				/* no break */
			default  :
			        unnextch(c);
				ungetstr(locbuf, p - locbuf);
				return locfl;
			}
			*p++ = (char)c;
			colm++;
		}
	}
	else {
		for (;;) {
			nextch(c);
			switch (c) {
			case ' ' :
			case '\t': break;
			case '!' :
				if (!locfl) {
				        unnextch(c);
					while (skip_comment(mode));
				 	locfl++;
				 	p = locbuf;
					continue;
				}
				goto exit;
			case '\n':
				if (!locfl) {
					locfl++;
					p = locbuf;
					line++; colm = 0;
					continue;
				}
				goto exit;
			case '&' :
				if (locfl==1) {
					colm++;
					return 1;
				}
				goto exit;
			case SYM_FPP:
				if (locfl==1 && colm==0) {
				        unnextch(c);
					ungetsym('\n');
					return 0;
				}
				goto exit;
			default:
				goto exit;
			}
			*p++ = (char)c;
			colm++;
		}
exit:           unnextch(c);
		ungetstr(locbuf, p - locbuf);
		if (locfl)
			return 1;
		else	return 0;
	}
}

static wchar
getsym( unsigned mode ) {
	wchar 	c;

	for (;;) {
	        nextch(c);
		colm++;
		sym_line = line;

		if (colm > line_width) {
			if (c != '\n' && c!= SYM_EOF)
				continue;
		}
		switch (c) {
		case ' ':
		case '\t':
			if (mode & MOD_SPACE)
				continue;
			break;
		case '\n':
		case SYM_EOF:
			if ((mode & MOD_RAW) && (mode & MOD_CONT) && colm <= line_width) {
				if (falselvl == 0) {
					unnextch(c);
					c = ' ';
				}
				else if (c == '\n') {
					line++; colm = 0;
				}
				else {
				        colm = 0;
				}
			}
			else if (c == '\n') {
				line++; colm = 0;
				if (fixedformfl && (mode & MOD_CONT)) {
					while (skip_comment(mode));
					if (skipcont()) {
						CHECK(colm==6);
						continue;
					}
				}
			}
			else {
			        colm = 0;
			}
			break;
		case '!':
			if (!(mode & MOD_RAW) && !(fixedformfl && colm==6) 
				&& !(mode & MOD_IF)) {

			        /* Process a trailing comment */
				unnextch(c);
 				skip_comment(mode);
				CHECK(colm == 0);
				if (fixedformfl && (mode & MOD_CONT)) {
					while (skip_comment(mode));
					if (skipcont()) {
						CHECK(colm==6);
						continue;
					}
				}
				c = '\n';
			}
			break;
		case '&':
			if (!fixedformfl) {
				if (mode & MOD_CONT) {
					if (skipcont90(mode)) 
						continue;
				}
			}
			break;
		case '/':
			if (!(mode & MOD_RAW) && !no_ccom_fl) {
				nextch(c);
				if (c == '*') {
					int	loclvl=0;
	
					colm++;
					for (;;) {
						nextch(c);
						if (c == '*') {
							colm++;
							nextch(c);
							if (c == '/') {
								colm++;
								if (!loclvl) break;
								else loclvl--;
							}
							else {
							        unnextch(c);
							}
						}
						else if (c == '\n') {
							colm = 0;
							line++;
						}
						else if(c == SYM_EOF) {
							fppmess(ERR_LCOMM);
							break;
						}
						else if(c == '/') {
							colm++;
							nextch(c);
							if (c == '*') {
								colm++;
								loclvl++;
							}
							else {
							        unnextch(c);
							}
						}
						else {
							colm++;
						}
					}
					c = SYM_DUMMY;
				}
				else {
				        unnextch(c);
					c = '/';
				}
			}
			break;
		default:
			if (!is_special(c) && (mode & MOD_LCASE)) 
				c = lowcase(c);
			break;
		}
		return c;
	}
}		

static int
get_tkop(SymPtr symp) {
	switch (symvali(symp)) {
	case FTN_FORMAT:
		return(TK_FORMAT);
	case FTN_IMPLICIT:
		return(TK_IMPLICIT);
	case FTN_DO:
		return(TK_DO);
	case FTN_READ:
	case FTN_WRITE:
		return(TK_RDWR);
	case FTN_ASSIGN:
		return(TK_ASGN);
	case FTN_TYPE:			/* only for F90 */
		return(TK_TYPE0);
	case FTN_BYTE:
	case FTN_CHARACTER:
	case FTN_COMPLEX:
	case FTN_DOUBLECOMP:
	case FTN_DOUBLEPREC:
	case FTN_INTEGER:
	case FTN_LOGICAL:
	case FTN_REAL:
		return(TK_TYPE);
	default:
		return(TK_KEY);
	}
/*	return(TK_KEY);	*/
}

static int
isfspec( char *s ) {
	char	c;

	c = lowcase(*s); s++;
	switch (c) {
	case 'b':
		c = lowcase(*s);
		if (c == 'n' || c== 'z') {
			s++;
			if ( *s == '\0') return 1;
			else break;
		}
		while (is_num(*s)) s++;
		if (*s == '\0') return 1;
		break;
	case 't':
		c = lowcase(*s);
		if (c == 'l' || c== 'r') 
			s++;
		while (is_num(*s)) s++;
		if (*s == '\0') return 1;
		break;
	case 'e':
		c = lowcase(*s);
		if (c == 'n' || c== 's') 
			s++;
		while (is_num(*s)) s++;
		if (*s == '\0') return 1;
		break;
	case 'a': case 'd': case 'f': case 'g':
	case 'i': case 'l': case 'o': case 'z':
		while (is_num(*s)) s++;
		if (*s == '\0') return 1;
		break;
	case 's':
		c = lowcase(*s);
		if (c == 'p' || c == 's' || c == 'u')
			s++;
		if (*s == '\0')
			return 1;
		break;		
	case 'x': case 'r': case 'q': case 'p':
		if (*s == '\0')
			return 1;
		break;		
	default: 
		return 0;
	}
	return 0;
}

#define st_move1(x)  if(!(mode & MOD_NOGRAM)) {st_move(x);}

Token   *
get_token(unsigned mode) {
	wchar 	c,c0;
	char    *endp;
	int	flreal;			/* used in number processing */
	SymPtr	symp;
	static	int fllogic = 0;	/* to process kind in logical constants */
	static	int flimpl = 0;		/* to process implicit statement */
	static	int flformat = 0;	/* to process format statement */
	unsigned lmode;                 /* local mode */

	lmode = mode /* & ~MOD_CONT */;

loop:	endp = curtok->token;
	curtok->val = TK_DUMMY;

	c = getsym(mode);
	CHECK(c);
	curtok->lineno  = sym_line;
	curtok->stid    = cur_stmt_id;
	switch (c) {

	case '\n':	
	case SYM_EOF:	curtok->val = TK_NL;
	                st_move1(curtok->val);
			*endp++ = (char)c;
			onequalsym = NULL;	/* break assignment subst control */
			flimpl = 0;		/* break implicit processing */
			flformat = 0;		/* break format processing */
			cur_stmt_id++;
			break;
	                
	case SYM_FPP:	curtok->val = TK_FPP;
	                st_move1(curtok->val);
			*endp++ = (char)c;
			break;

	case SYM_DUMMY: *endp++ = ' ';
			break;

	case SYM_BOS:   curtok->val = TK_BOS;
	                st_move1(curtok->val);
			if (fixedformfl) {
				CHECK(colm == 6);
			}
			*endp++ = cont_symbol;
			break;

	case SYM_EOMS:  /* it also delimits lexems */
	                sympop();	
			if (sbfl) sb_mref_end();
			goto loop;

	case SYM_BF:	/* it simply delimits lexems */
	                symhide();
			goto loop;

	case SYM_EF:    /* it simply delimits lexems */
	                symunhide();
			goto loop;

	case SYM_EOC:   curtok->val = TK_EOC;
			break;

	case '\\':	*endp++ = (char)c;
			c = getsym(MOD_RAW);
			if (is_special(c)) {
				ungetsym(c);
			}
			else if (c == '\n' || c == SYM_EOF) {
			        goto loop;
				/* endp[-1] = c = ' '; */
			}
			else {
				*endp++ = (char)c;
				st_move1(curtok->val);  /* TK_DUMMY */
			}
			break;
/*	case ' ':
	case '\t':	*endp++ = c;
			break;
			while (is_blank(c)) {
				*endp++ = c;
				if (endp - curtok->token >= MAXTOKENSIZE) {
					curtok->length = MAXTOKENSIZE;
					outtok(curtok);
					endp = curtok->token;
				}
				c = getsym(lmode);
			}
			ungetsym(c);
			break;
*/
	case '(':	curtok->val = TK_LPAR;
			st_move1(curtok->val);
			*endp++ = (char)c;
			if (flimpl) {
				flimpl++;
				if (flimpl == 2 && !st_is(ST_NOIMPL)) {
					st_move1(TK_IMPLICIT1);
				}
			}
			if (flformat) {
				flformat++;
			}
			break;
	case ')':	curtok->val = TK_RPAR;
			st_move1(curtok->val);
			*endp++ = (char)c;
			if (flimpl) {
				flimpl--;
			}
			if (flformat) {
				flformat--;
			}
			break;
	case ',':	curtok->val = TK_COMMA;
			st_move1(curtok->val);
			*endp++ = (char)c;
			if (flimpl == 1) {
				st_move1(TK_IMPLICIT);
			}
			break;
	case '-':	curtok->val = TK_MINUS;
			st_move1(curtok->val);
			*endp++ = (char)c;
			break;
	case ';':	curtok->val = TK_SCLN;
			st_move1(curtok->val);
			*endp++ = (char)c;
			if (!f77fl) {
				onequalsym = NULL;	/* they are stoppers */
				flimpl = 0;
				flformat = 0;
			}
			break;
	case '*':	*endp++ = (char)c;
			c = getsym(lmode);
			if (c == '*') 
				*endp++ = (char)c;
			else {
				curtok->val = TK_STAR;
				ungetsym(c);
			}
			st_move1(curtok->val);
			break;
	case '|':	
	case '&':	*endp++ = (char)c;
			c0 = c;
			c = getsym(lmode);
			if (c == c0) 
				*endp++ = (char)c;
			else
				ungetsym(c);
			st_move1(curtok->val);
			break;
	case '!':
	case '=':	*endp++ = (char)c;
			if (c == '=' && onequalsym) {
				set_pos(onequalline);
				fppmess(WARN_PINSUB,symname(onequalsym));
				onequalsym = NULL;
			}
			c = getsym(lmode);
			if (c == '=') 
				*endp++ = (char)c;
			else
				ungetsym(c);
			st_move1(curtok->val);
			break;
	case '<':
	case '>':	*endp++ = (char)c;
			c0 = c;
			c = getsym(lmode);
			if (c == c0 || c == '=')
				*endp++ = (char)c;
			else
				ungetsym(c);
			st_move1(curtok->val);
			break;
	case '.':	*endp++ = (char)c;
			if (!f77fl) {
				if (!fllogic || fllogic == 2) fllogic++;
			}
			*endp = 0;
			st_move1(curtok->val);
			break;
	case '"':
	case '\'':	c0 = c;
			*endp++ = (char)c;
			if (c0 == '\'' && st_is(ST_RDWR)) 	/* READ(10'3) syntax exception */
				break;
			for (;;) {
				c = getsym((mode&MOD_CONT)|MOD_RAW);
				if (c=='\n' || c==SYM_EOF || c==SYM_EOC) {
					ungetsym(c);
					if (falselvl == 0 && c != SYM_EOC)
						fppmess(ERR_STR,c0);
					if (mode & MOD_FPP) 
						*endp++ = (char)c0;
					break;
				}
				else if (c == c0) {
					*endp++ = (char)c;
					break;
				}
				else if (is_special(c))
					c = ' ';
				*endp++ = (char)c;
				if (endp - curtok->token >= MAXTOKENSIZE) {
					curtok->length = MAXTOKENSIZE;
					outtok(curtok);
					endp = curtok->token;
				}
			}
			if (fixedformfl && c0 == '\'') {
				c = getsym(lmode);
				if (lowcase(c) == 'o' || lowcase(c) == 'x') {
					curtok->val = TK_BOZ;
					*endp++ = (char)c;
				}
				else
					ungetsym(c);
			}
			st_move1(curtok->val);
			break;
	case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
	case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
	case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U': 
	case 'V': case 'W': case 'X': case 'Y': case 'Z':
	case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g':
	case 'h': case 'i': case 'j': case 'k': case 'l': case 'm': case 'n':
	case 'o': case 'p': case 'q': case 'r': case 's': case 't': case 'u': 
	case 'v': case 'w': case 'x': case 'y': case 'z': case '_':

                       	/* Check if we are dealing with *kind* of a logical constant
			 * which should be taken without the leading underscore
			 */
			if (!f77fl && c == '_' && fllogic == 3) {
				*endp++ = (char)c;
				*endp = 0;
				fllogic = 0;
				goto exit;
			}

			/* Collect a name */
			while (is_alphanum(c)) {
				*endp++ = (char)c;
				if (endp - curtok->token >= MAXTOKENSIZE) {
					curtok->length = MAXTOKENSIZE;
					outtok(curtok);
					endp = curtok->token;
				}
				c = getsym(lmode);
			}
			*endp = 0;
			ungetsym(c);
			if ((lmode & MOD_SPACE) && line > sym_line) {
			        while (sym_line < line) {
				        ungetsym('\n'); /* it decreases the line */
					ungetsym('\\');
				}
			}

			/* Check if we are dealing with a BOZ constant
			 */
			if (c == '\'' || c== '"') {
				if ((endp - curtok->token) == 1) { 
				 	if (lowcase(curtok->token[0]) == 'b' ||
					    lowcase(curtok->token[0]) == 'o' ||
					    lowcase(curtok->token[0]) == 'z') {
						curtok->val = TK_BOZ;
						goto exit;
					}
					if (fixedformfl && lowcase(curtok->token[0]) == 'x') {
						curtok->val = TK_BOZ;
						goto exit;
					}
				}
				else if (!f77fl && endp[-1] == '_') {
					ungetsym(*--endp);
					*endp = 0;
				}
			}

			curtok->val = TK_NAME;
			if ((outfl || falselvl || !substfl) && !dosubstfl) {

			        /* The following is for syntax like WRITE(10'3), etc.
				 */
				if (fixedformfl && st_is(ST_BOS)) {
					symp = symget(curtok->token,CL_KEY);
					if (symp)
						curtok->val = get_tkop(symp);
				}
			}
			else {
				int	n;
				
				if (!(mode & MOD_SPACE)) {
					if (st_is(ST_BOS)) 
						symp = symget(curtok->token,CL_NM|CL_KEY);
					else if (st_is(ST_IMPL0)) {
						symp = symget(curtok->token,CL_NM|CL_KEY);
						if (symp && (symtype(symp) & CL_KEY)) {
							curtok->val = get_tkop(symp);
							if (curtok->val != TK_TYPE0 && 
							    curtok->val != TK_TYPE ) {
								curtok->val = TK_NAME;
								symp = NULL;
							}
						}
					}
					else
						symp = symget(curtok->token,CL_NM);
					if ((endp - curtok->token) == 1 && st_is(ST_IMPL))
						curtok->val = TK_NAME0;
					if (symp) {
						if (symtype(symp) & CL_NM) {
							if (curtok->val == TK_NAME0) {
								fppmess(WARN_IMPL,curtok->token);
							}
							else if (flformat > 1 && isfspec(curtok->token)) {
								fppmess(WARN_FORMAT,curtok->token);
							}
							else if (substitute(symp, lmode)) {
								st_move1(curtok->val);
								goto exit;
							}
							else goto loop;
						}
						else {
							curtok->val = get_tkop(symp);
							if (curtok->val == TK_IMPLICIT)
								flimpl = 1;
							else if (curtok->val == TK_FORMAT)
								flformat = 1;
						}
					}
				}
				else {
					if (st_is(ST_BOS)) {
						symp = symgetm(curtok->token,CL_NM|CL_KEY);
						if (symp) {
							n = strlen(symname(symp));
							if (n < endp - curtok->token && n < MAXNAMELEN) {
								if (symtype(symp) & CL_NM && !onequalsym) { 
									onequalsym = symp;
									onequalline = curtok->lineno;
								}
								ungetstr(curtok->token+n, 
									 (endp - curtok->token) - n);
								ungetsym(SYM_DUMMY);
								endp = curtok->token + n;
								*endp = 0;
							}
							if (symtype(symp) & CL_NM) {
								if (symflag(symp)) 
									fppmess(WARN_PINSUB,curtok->token);
								if (substitute(symp, lmode)) {
									st_move1(curtok->val);
									goto exit;
								}
								else goto loop;
							}
							else {
								curtok->val = get_tkop(symp);
								if (curtok->val == TK_IMPLICIT)
									flimpl = 1;
								else if (curtok->val == TK_FORMAT)
									flformat = 1;
							}
						}
					}
					else if (st_is(ST_DO) || st_is(ST_ASSIGN)) {
						symp = symgetm(curtok->token,CL_NM);
						if (symp) {
							n = strlen(symname(symp));
							ungetstr(curtok->token + n,
								 (endp - curtok->token) - n);
							ungetsym(SYM_DUMMY);
							endp = curtok->token + n;
							*endp = 0;
							if (substitute(symp, lmode)) {
								st_move1(curtok->val);
								goto exit;
							}
							else goto loop;
						}
						if (st_is(ST_DO))
							fppmess(WARN_PIOP,"do");
						else
							fppmess(WARN_PIOP,"assign");
						curtok->val = TK_DUMMY;
					}
					else if (st_is(ST_ASSIGNUM)) {
						if (!strncmp(curtok->token,"to",2)) {
							ungetstr(curtok->token+2,
								 (endp - curtok->token) - 2);
							ungetsym(SYM_DUMMY);
							endp = curtok->token+2;
							*endp = 0;
							curtok->val = TK_DUMMY;
						}
						else {
							symp = symgetm(curtok->token,CL_NM);
							if (symp) {
								if (substitute(symp, lmode)) {
									st_move1(curtok->val);
									goto exit;
								}
								else goto loop;
							}
							fppmess(WARN_PIOP,"assign");
							curtok->val = TK_DUMMY;
						}
					}
					else if (st_is(ST_TYPE1)) {
						if (!strncmp(curtok->token,"function",8)) {
							ungetstr(curtok->token+8,
								 (endp - curtok->token) - 8);
							ungetsym(SYM_DUMMY);
							endp = curtok->token+8;
							*endp = 0;
							curtok->val = TK_KEY;
						}
						else if (!strncmp(curtok->token,"recursive",9)) {
							ungetstr(curtok->token+9,
								 (endp - curtok->token) - 9);
							ungetsym(SYM_DUMMY);
							endp = curtok->token+9;
							*endp = 0;
							curtok->val = TK_KEY;
						}
						else {
							symp = symgetm(curtok->token,CL_NM);
							if (symp) {
								if (substitute(symp, lmode)) {
									st_move1(curtok->val);
									goto exit;
								}
								else goto loop;
							}
						}
					}
					else if (st_is(ST_IMPL0)) {
						symp = symget(curtok->token,CL_KEY|CL_NM);
						if (symp) {
							if (symtype(symp) & CL_KEY) {
								curtok->val = get_tkop(symp);
								if (curtok->val != TK_TYPE &&
								    curtok->val != TK_TYPE0)
									curtok->val = TK_NAME;
							}
							else if (substitute(symp, lmode)) {
								st_move1(curtok->val);
								goto exit;
							}
							else goto loop;
						}
					}
					else {
						if (endp - curtok->token == 1 && st_is(ST_IMPL))
							curtok->val = TK_NAME0;
						if (symp = symget(curtok->token,CL_NM)) {
							if (curtok->val == TK_NAME0) {
								fppmess(WARN_IMPL,curtok->token);
							}
							else if (flformat > 1 && isfspec(curtok->token)) {
								fppmess(WARN_FORMAT,curtok->token);
							}
							else if (substitute(symp, lmode)) {
								st_move1(curtok->val);
								goto exit;
							}
							else goto loop;
						}
					}
				}
			}
			if (!f77fl && fllogic == 1) {
				*endp = 0;
				if (!strcmp(curtok->token,"true") ||
				    !strcmp(curtok->token,"false")) 
					fllogic++;
				else fllogic = 0;
				/* st_move1(curtok->val);
				goto exit;
				*/
			}
			st_move1(curtok->val);
			break;
	case '0': case '1': case '2': case '3': case '4': 
	case '5': case '6': case '7': case '8': case '9': 
			*endp++ = (char)c;
			flreal=0;
			for (;;) {
			        if (endp - curtok->token >= MAXTOKENSIZE) {
					curtok->length = MAXTOKENSIZE;
					outtok(curtok);
					endp = curtok->token;
				}
				c = getsym(lmode);
				if (is_num(c)) ;	/* that's it */
				else if (c == '.') {
					if (flreal) break;
					if (mode & MOD_IF) break;
					flreal = 1;
				}
				else if (lowcase(c) == 'e'||
					 lowcase(c) == 'd'||
					 lowcase(c) == 'q') {
					if (flreal > 1) break;
					if (flreal == 0 && st_is(ST_TYPE)) break;
					flreal = 2;
					*endp++ = (char)c;
					c = getsym(lmode);
					if (is_alpha0(c)) {
						ungetsym(c);
						ungetsym(*--endp);
						if (endp[-1] == '.') c = *--endp;
						else c = SYM_DUMMY;
						break;
					}
					if (c != '-' && c!='+') {
						ungetsym(c);
						continue;
					}
				}
				else if (fixedformfl && lowcase(c) == 'h') {
					int	count;
					
					/*
					 * Hollerith constant 
					 */
					if (flreal) break;
					if (st_is(ST_TYPE)) break;
					*endp = 0;
					strtoi(curtok->token,&count,10);
					*endp++ = (char)c;
					while (count--) {
						c = getsym((mode&MOD_CONT)|MOD_RAW);
						if (c=='\n' || c==SYM_EOF || c==SYM_EOC) {
							ungetsym(c);
							if (!falselvl && c!=SYM_EOC)
							        fppmess(ERR_HRTH);
							break;
						}
						else if (is_special(c))
							c = ' ';
						*endp++ = (char)c;
						if (endp - curtok->token >= MAXTOKENSIZE) {
						        curtok->length = MAXTOKENSIZE;
							outtok(curtok);
							endp = curtok->token;
						}
					}
					curtok->val = TK_DUMMY;
					st_move1(curtok->val);
					goto exit;
				}
				else if (c == '_' && !f77fl) {		/* kind processing */
					do {
						*endp++ = (char)c;
						if (endp - curtok->token >= MAXTOKENSIZE) {
						        curtok->length = MAXTOKENSIZE;
							outtok(curtok);
							endp = curtok->token;
						}
						c = getsym(lmode);
					} while (is_num(c));
					break;
				}
#if USE_C_HEX_CONST
				else if ((mode & MOD_IF) && 
					 (lowcase(c) == 'x') &&
					 (curtok->token[0] == '0') && 
					 (endp - curtok->token == 1) ) {

					/* C hexadecimal constants are allowed
					 * in #if expression */
					do {
						*endp++ = c;
						if (endp - curtok->token >= MAXTOKENSIZE) {
						        curtok->length = MAXTOKENSIZE;
							outtok(curtok);
							endp = curtok->token;
						}
						c = getsym(lmode);
					} while (is_alphanum(c));
					ungetsym(c);
					curtok->val = TK_BOZ;
					goto exit;
				}
#endif /* USE_C_HEX_CONST */
				else break;
				*endp++ = (char)c;
			}
			ungetsym(c);
			if (flreal) {
				curtok->val = TK_DUMMY;
			}
			else {
				curtok->val = TK_NUM;
			}
			st_move1(curtok->val);
			break;
	default:	if (!is_blank(c)) 
				st_move1(TK_DUMMY);
			*endp++ = (char)c;
			break;
	}
exit:	*endp = 0;
        curtok->length = endp - curtok->token;
#if DEBUG
	if (debug >= 2) {
	  printf("line %d stid %d token %d: %s\n",
		 curtok->lineno,
		 curtok->stid,
		 curtok->val,
		 curtok->token );
	}
#endif
	return curtok;
}

Token   *
get_token_nl(unsigned mode) {
	wchar 	c;
	char    *endp;
	int	i;

	/* The current state is ST_NL */

	while (skip_comment(mode));	
		
	endp = curtok->token;
	CHECK(colm == 0);
	c = getsym(mode & MOD_LCASE);
	curtok->lineno = line;
	curtok->stid   = cur_stmt_id;
	curtok->val    = TK_DUMMY;
	CHECK(c);
	switch (c) {
	case SYM_FPP:
		curtok->val = TK_FPP;
		st_move(curtok->val);
		*endp++ = (char)c;
		*endp =0;
		curtok->length = 1;
		return curtok;
        case SYM_EOF:
	case '\n':
		curtok->val = TK_NL;
		cur_stmt_id++;
		/* st_move(curtok->val); the state remains ST_NL */
		*endp++ = (char)c;
		*endp = 0;
		curtok->length = 1;
		return curtok;
	case '&':
	        /* continuation symbol in first column;
		 * fixed or free form it is we do the same.
		 */
	        ungetsym(c);
		st_move(TK_SKIP);
		return get_token(mode);
	}

	/* The following moves us in the
	 * BeginOfStatement state
	 */
	st_move(TK_DUMMY);

	if (!fixedformfl) {	/* F90 */
		ungetsym(c);
		return get_token(mode);
	}

	/* Collect first 6 symbols */
	for (i=0;;i++) {
		if (c == '\t') {
			colm = 6;
			line_width = LINE_WIDTH_MAX;
			break;
		}
		else if (c == SYM_DUMMY) {	/* a comment */
			c = ' ';
			colm = 6;
			break;
		}
		else if (c == SYM_EOF || c == '\n') 
			break;
		if (i == 5) break;
		*endp++ = (char)c;
		c = getsym(mode & MOD_LCASE);
	}
	*endp = 0;
	if (is_blank(c) || c == '0') {
	        cont_symbol = (char)c;
		ungetsym(SYM_BOS);
		while (is_blank(endp[-1]) && endp > curtok->token) endp--;
		ungetstr(curtok->token, endp - curtok->token);
	}
	else if (c == SYM_EOF || c == '\n') {
		ungetsym(c);
		if (c == '\n') {
			colm = 6;
			cont_symbol = ' ';
			ungetsym(SYM_BOS);
			while (is_blank(endp[-1]) && endp > curtok->token) endp--;
		}
		ungetstr(curtok->token, endp - curtok->token);
	}
	else {
	        /* We've got a symbol in the 6th column
		 * so this line is a continuation card
		 * no matter how could it happen.
		 * Output collected symbols as is.
		 */
		*endp++ = (char)c;
		curtok->length = endp - curtok->token;
		outtok(curtok);

		/* The state can't be BegOfStatement */
		st_move(TK_SKIP);
	}
	return get_token(mode);
}

void
unget_tok(Token *tokp) {
        if (tokp->val == TK_NL) {
	        cur_stmt_id--;
		if (tokp->token[0] == '\n') {
		        ungetsym(tokp->token[0]);
		}
		else {
		        ungetsym(SYM_EOF);
		}
	}
	else {
	        ungetstr(tokp->token, tokp->length);
	}
}

Token   *
get_cur_tok() {
        return curtok;
}

void
pushtok() {
        curtok++;
	CHECK(curtok == &ltok[1]);
}

void
poptok() {
        curtok--;
	CHECK(curtok == &ltok[0]);
}
