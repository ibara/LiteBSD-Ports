/*
 * This file is part of ce.
 *
 * Copyright (c) 1990 by Chester Ramey.
 *
 * Permission is hereby granted to copy, reproduce, redistribute or
 * otherwise use this software subject to the following: 
 * 	1) That there be no monetary profit gained specifically from 
 *	   the use or reproduction of this software.
 * 	2) This software may not be sold, rented, traded or otherwise 
 *	   marketed.
 * 	3) This copyright notice must be included prominently in any copy
 * 	   made of this software.
 *
 * The author makes no claims as to the fitness or correctness of
 * this software for any use whatsoever, and it is provided as is. 
 * Any use of this software is at the user's own risk.
 */
/*
 * search.c --  Search commands.
 *
 * The functions in this file implement the search commands (both plain 
 * and incremental searches are found here; look in the file rearch.c for
 * the regular-expression search code) and the query-replace command.
 *
 * The plain old search code is part of the original MicroEMACS (v30)
 * "distribution". The incremental search code, and the query-replace code,
 * is by Rich Ellison.
 *
 * Some improvements by Chet Ramey; more from mg2a
 */
#include "celibc.h"
#include "ce.h"

#ifndef NULL
#define NULL 0
#endif

#define SRCH_BEGIN	(0)			/* Search sub-codes.	*/
#define	SRCH_FORW	(-1)
#define SRCH_BACK	(-2)
#define SRCH_PREV	(-3)
#define SRCH_NEXT	(-4)
#define SRCH_NOPR	(-5)
#define SRCH_ACCM	(-6)

extern char	casemap[];	/* From cinfo.c */

typedef struct {
	int	s_code;
	LINE	*s_dotp;
	int	s_doto;
} SRCHCOM;

#define NSRCH  128

static	SRCHCOM	cmds[NSRCH];
static	int	cip;
static 	int	srch_lastdir = SRCH_NOPR;	/* Last search flags.	*/

static	int	isearch();
static	void	is_cpush();
static	void	is_lpush();
static	void	is_pop();
static	int	is_peek();
static	int	is_undo();
static	int	is_find();
static	void	is_prompt();
static	void	is_dspl();
static	int	forwsrch();
static	int	backsrch();

#define EQ(bc, pc)	(casemap[bc & 0xFF] == casemap[pc & 0xFF])

/*
 * Search forward.
 * Get a search string from the user, and search for it, starting at ".". 
 * If found, "." gets moved to just after the matched characters, and display 
 * does all the hard stuff.  If not found, it just prints a message.
 */
/*ARGSUSED*/
int
forwsearch(f, n)
int	f, n;
{
	register int	s;

	if ((s = readpattern("Search")) != TRUE)
		return (s);
	if (forwsrch() == FALSE) {
		mlwrite("Not found");
		return (FALSE);
	}
	srch_lastdir = SRCH_FORW;
	return (TRUE);
}

/*
 * Reverse search.
 * Get a search string from the  user, and search, starting at "."
 * and proceeding toward the front of the buffer. If found "." is left
 * pointing at the first character of the pattern [the last character that
 * was matched].
 */
/*ARGSUSED*/
int
backsearch(f, n)
int	f, n;
{
	register int	s;

	if ((s = readpattern("Reverse search")) != TRUE)
		return (s);
	if (backsrch() == FALSE) {
		mlwrite("Not found");
		return (FALSE);
	}
	srch_lastdir = SRCH_BACK;
	return (TRUE);
}

/* 
 * Search again, using the same search string
 * and direction as the last search command. The direction
 * has been saved in "srch_lastdir", so you know which way
 * to go.
 */
/*ARGSUSED*/
int
searchagain(f, n)
int	f, n;
{
	if (srch_lastdir == SRCH_FORW) {
		if (forwsrch() == FALSE) {
			mlwrite("Not found");
			return (FALSE);
		}
		return (TRUE);
	}
	if (srch_lastdir == SRCH_BACK) {
		if (backsrch() == FALSE) {
			mlwrite("Not found");
			return (FALSE);
		}
		return (TRUE);
	}
	mlwrite("No last search");
	return (FALSE);
}

/*
 * Use incremental searching, initially in the forward direction.
 * isearch ignores any explicit arguments.
 */
/*ARGSUSED*/
int
forwisearch(f, n)
int	f, n;
{
	return (isearch(SRCH_FORW));
}

/*
 * Use incremental searching, initially in the reverse direction.
 * isearch ignores any explicit arguments.
 */
/*ARGSUSED*/
int
backisearch(f, n)
int	f, n;
{
	return (isearch(SRCH_BACK));
}

/*
 * Incremental Search.
 *	dir is used as the initial direction to search.
 *	^S	switch direction to forward, find next
 *	^R	switch direction to reverse, find prev
 *	^Q	quote next character (allows searching for ^S etc.)
 *	<ESC>	exit from Isearch.
 *	<DEL>	undoes last character typed. (tricky job to do this correctly).
 *	else	accumulate into search string
 *
 */
static int
isearch(dir)
int	dir;
{
	register int	c;
	register LINE	*clp;
	register int	cbo;
	register int	success;
	int		pptr;
	int		srchseq, bsrchseq, qsrchseq;
	char		opat[NPAT];

	if (kbdmip != NULL) {
		mlwrite("Cannot i-search in a macro!");
		return (FALSE);
	}
	strcpy(opat, pat);
	for (cip=0; cip<NSRCH; cip++)
		cmds[cip].s_code = SRCH_NOPR;
	cip = 0;
	pptr = -1;
	clp = curwp->w_dotp;
	cbo = curwp->w_doto;
	is_lpush();
	is_cpush(SRCH_BEGIN);
	success = TRUE;
	srchseq = keybinding("isearch-forward");
	if (srchseq & CNTL) {
		srchseq &= ~CNTL;		/* remove control modifier */
		srchseq = CCHR(srchseq);	/* make control character */
	}
	bsrchseq = keybinding("isearch-backward");
	if (bsrchseq & CNTL) {
		bsrchseq &= ~CNTL;
		bsrchseq = CCHR(bsrchseq);
	}
	qsrchseq = keybinding("quote-character");
	if (qsrchseq & CNTL) {
		qsrchseq &= ~CNTL;
		qsrchseq = CCHR(qsrchseq);
	}
	is_prompt(dir, TRUE, success);
	for (;;) {
		update();
		c = (*term.t_getchar)();
		if (c == srchseq)
			c = CCHR('S');
		else if (c == bsrchseq)
			c = CCHR('R');
		else if (c == qsrchseq)
			c = CCHR('Q');
		switch(c) {
		case METACH:
			srch_lastdir = dir;
			setmark(FALSE,1);
			return (TRUE);
		case CCHR('G'):
		        if (success != TRUE) {
				while (is_peek() == SRCH_ACCM)
					(void) is_undo(&pptr,&dir);
				success = TRUE;
				is_prompt(dir, pptr < 0, success);
				break;
			}
			curwp->w_dotp = clp;
			curwp->w_doto = cbo;
			curwp->w_flag |= (WFMOVE|WFMODE);  /* to change ratio */
			srch_lastdir = dir;
			ctrlg(FALSE, 0);
			strcpy(pat, opat);
			return (FALSE);
		case CCHR('S'):
			if (dir == SRCH_BACK) {
				dir = SRCH_FORW;
				is_lpush();
				pptr = strlen(pat);
				is_cpush(SRCH_FORW);
				success = TRUE;
			}
			/* Drop through to find next. */
			if (success == FALSE && dir == SRCH_FORW)
				break;
			is_lpush();
			pptr = strlen(pat);
			forwchar(FALSE, 1);
			if (is_find(SRCH_NEXT) != FALSE)
				is_cpush(SRCH_NEXT);
			else {
				backchar(FALSE, 1);
				(*term.t_beep)();
				success = FALSE;
			}
			is_prompt(dir, pptr < 0, success);
			break;
		case CCHR('R'):
			if (dir == SRCH_FORW) {
				dir = SRCH_BACK;
				is_lpush();
				pptr = strlen(pat);
				is_cpush(SRCH_BACK);
				success = TRUE;
			}
			/* Drop through to find previous. */
			if (success == FALSE && dir == SRCH_BACK)
				break;
			is_lpush();
			backchar(FALSE, 1);
			pptr = strlen(pat);
			if (is_find(SRCH_PREV) != FALSE)
				is_cpush(SRCH_PREV);
			else {
				forwchar(FALSE,1);
				(*term.t_beep)();
				success = FALSE;
			}
			is_prompt(dir, pptr < 0, success);
			break;
		case 0x7F:			/* CCHR('?') */
		case CCHR('H'):		/* For us IBM-RT 4.3BSD folks */
			if (is_undo(&pptr, &dir) != TRUE)
				return (ABORT);
			if (is_peek() != SRCH_ACCM)
				success = TRUE;
			is_prompt(dir, FALSE, success);
			break;
		case CCHR('^'):          /* should this be CCHR('\\') ? */
		case CCHR('Q'):
			c = (*term.t_getchar)();
		case CCHR('U'):
		case CCHR('X'):
		case CCHR('J'):
			goto  addchar;
		default:
			if (ISCTRL(c) != FALSE) {
				c += '@';
				c |= CNTL;
				success = execute(c, FALSE, 1);
				curwp->w_markp = clp;
				curwp->w_marko = cbo;
				mlwrite("Mark Set");
				curwp->w_flag |= (WFMOVE|WFMODE);
				return (success);
			}				
		addchar:
			if (pptr == -1)
				pptr = 0;
			if (pptr == 0)
				success = TRUE;
			pat[pptr++] = c;
			if (pptr == NPAT) {
				mlwrite("Pattern too long");
				ctrlg(FALSE, 0);
				return (ABORT);
			}
			pat[pptr] = '\0';
			is_lpush();
			if (success != FALSE) {
				if (is_find(dir) != FALSE)
					is_cpush(c);
				else {
					success = FALSE;
					(*term.t_beep)();
					is_cpush(SRCH_ACCM);
				}
			} else
				is_cpush(SRCH_ACCM);
			is_prompt(dir, FALSE, success);
		}
	}
}

static void
is_cpush(cmd)
register int	cmd;
{
	if (++cip >= NSRCH)
		cip = 0;
	cmds[cip].s_code = cmd;
}

static void
is_lpush()
{
	register int	ctp;

	ctp = cip+1;
	if (ctp >= NSRCH)
		ctp = 0;
	cmds[ctp].s_code = SRCH_NOPR;
	cmds[ctp].s_doto = curwp->w_doto;
	cmds[ctp].s_dotp = curwp->w_dotp;
}

static void
is_pop()
{
	if (cmds[cip].s_code != SRCH_NOPR) {
		curwp->w_doto  = cmds[cip].s_doto; 
		curwp->w_dotp  = cmds[cip].s_dotp;
		curwp->w_flag |= WFMOVE;
		cmds[cip].s_code = SRCH_NOPR;
	}
	if (--cip <= 0)
		cip = NSRCH-1;
}

static int
is_peek()	
{
	if (cip == 0)
		return (cmds[NSRCH-1].s_code);
	else
		return (cmds[cip-1].s_code);
}

static int
is_undo(pptr, dir)
register int	*pptr;
register int	*dir;
{
	switch (cmds[cip].s_code) {
	    case SRCH_NOPR:
	    case SRCH_BEGIN:
	    case SRCH_NEXT:
	    case SRCH_PREV:
		break;

	    case SRCH_FORW:
		*dir = SRCH_BACK; 
		break;

	    case SRCH_BACK:
		*dir = SRCH_FORW;
		break;

	    case SRCH_ACCM:
	    default:
		*pptr -= 1;
		if (*pptr < 0)
			*pptr = 0;
		pat[*pptr] = '\0';
		break;
	}
	is_pop();
	return (TRUE);
}

/*
 * should we reset "." right before we fail?
 */
static int
is_find(dir)
register int	dir;
{
	register int	plen;

	plen = strlen(pat);
	if (plen != 0) {
		if (dir==SRCH_FORW || dir==SRCH_NEXT) {
			backchar(FALSE, plen);
			if (forwsrch() == FALSE) {
				forwchar(FALSE, plen);
				return (FALSE);
			}
			return (TRUE);
		}
		if (dir==SRCH_BACK || dir==SRCH_PREV) {
			forwchar(FALSE, plen);
			if (backsrch() == FALSE) {
				backchar(FALSE, plen);
				return (FALSE);
			}
			return (TRUE);
		}
		mlwrite("bad call to is_find");
		ctrlg(FALSE, 0);
		return (FALSE);
	}
	return (FALSE);
}

/*
 * If called with "dir" not one of SRCH_FORW or SRCH_BACK, this routine
 * used to print an error message. It also used to return TRUE or FALSE,
 * depending on if it liked the "dir". However, none of the callers looked 
 * at the status, so I just made the checking vanish.
 */
static void
is_prompt(dir, flag, success)
int	dir, flag, success;
{
	if (dir == SRCH_FORW) {
		if (success != FALSE)
			is_dspl("I-search", flag);
		else
			is_dspl("Failing I-search forward", flag);
	} else if (dir == SRCH_BACK) {
		if (success != FALSE)
			is_dspl("I-search backward", flag);
		else
			is_dspl("Failing I-search backward", flag);
	} else mlwrite("Bad call to is_prompt");
}

/*
 * Prompt writing routine for the incremental search.  The "prompt" is just 
 * a string. The "flag" determines if a "[ ]" or ":" embellishment is used.
 */
static void
is_dspl(prompt, flag)
char	*prompt;
{
	char	npat[NPAT*2];

	(void) expandpat(pat, npat, sizeof(npat));
	if (flag != FALSE)
		mlwrite("%s [%s]", prompt, npat);
	else
		mlwrite("%s: %s", prompt, npat);
}

/*
 * Query Replace.
 *	Replace strings selectively.  Does a search and replace operation.
 *	A space or a comma replaces the string, a period replaces and quits,
 *	an n doesn't replace, a C-G quits.
 */
int
queryrepl(f, n)
int	f, n;
{
	register int	s, c;
	char		news[NPAT];	/* replacement string		*/
	char		expat[2*NPAT];  /* pattern with chars expanded	*/
	char		exnews[2*NPAT];	/* expanded replacement string	*/
	register int	kludge;		/* Watch for saved line move	*/
	LINE		*clp;		/* saved line pointer		*/
	int		cbo;		/* offset into the saved line	*/
	int		rcnt = 0;	/* Replacements made so far	*/
	int		plen;		/* length of found string	*/
	char            pbuf[NLINE];    /* to hold the prompt           */
	
	if (kbdmip != NULL) {
		mlwrite("Cannot query-replace in a macro");
		return(FALSE);
	}
	if (curbp->b_mode & MDVIEW) 
		return(rdonly());
	if (f && n <= 0) return (FALSE);
	if ((s=readpattern("Query Replace")) != TRUE)
		return (s);
	(void) expandpat(pat, expat, sizeof(expat));
	(void) sprintf(pbuf,"Query Replace %s with: ", expat);
	if ((s=mlreply(pbuf, news, NPAT)) == ABORT)
		return (s);
	if (s == FALSE) news[0] = '\0';
	(void) expandpat(news, exnews, sizeof(exnews));
	mlwrite("Query Replacing %s with %s", expat, exnews);
	plen = strlen(pat);

	/*
	 * Search forward repeatedly, checking each time whether to insert
	 * or not.  The "!" case makes the check always true, so it gets put
	 * into a tighter loop for efficiency.
	 *
	 * If we change the line that is the remembered value of dot, then
	 * it is possible for the remembered value to move.  This causes great
	 * pain when trying to return to the non-existant line.
	 *
	 * possible fixes:
	 * 1) put a single, relocated marker in the WINDOW structure, handled
	 *    like mark.  The problem now becomes a what if two are needed...
	 * 2) link markers into a list that gets updated (auto structures for
	 *    the nodes)
	 * 3) Expand the mark into a stack of marks and add pushmark, popmark.
	 */

	clp = curwp->w_dotp;		/* save the return location	*/
	cbo = curwp->w_doto;
	while (forwsrch() == TRUE) {
	retry:
		update();
		switch (c = (*term.t_getchar)()) {
		    case ' ':
		    case ',':
		    case '.':
			kludge = (curwp->w_dotp == clp);
			if (lreplace(plen, news, f) == FALSE)
				return (FALSE);
			rcnt++;
			if (kludge != FALSE)
				clp = curwp->w_dotp;
			if (c != '.')
				break;
			else
				goto stopsearch;
		    case CCHR('G'):
			ctrlg(FALSE, 0);
		    case CCHR('['):
			goto stopsearch;
		    case '!':
			do {
				kludge = (curwp->w_dotp == clp);
				if (lreplace(plen, news, f) == FALSE)
					return (FALSE);
				rcnt++;
				if (kludge != FALSE)
					clp = curwp->w_dotp;
			} while (forwsrch() == TRUE);
			goto stopsearch;
	            case 0x7F:       /* DEL -- could also be CCHR('?') */
		    case CCHR('H'):  /* for us IBM-RT 4.3BSD folks */
		    case 'n':
			break;

		    default:
mlwrite("<SP>[,] replace, [.] rep-end, <DEL> don't, [!] repl rest <ESC> quit");
			goto retry;
		}
	}
stopsearch:
	curwp->w_dotp = clp;
	curwp->w_doto = cbo;
	curwp->w_flag |= (WFHARD|WFMODE);	/* MODE to change the ratio */
	update();
	if (rcnt == 0)
		mlwrite("No replacements done");
	else if (rcnt == 1)
		mlwrite("1 replacement done");
	else
		mlwrite("%d replacements done", rcnt);
	return (TRUE);
}

/*
 * This routine does the real work of a forward search. The pattern is 
 * sitting in the external variable "pat". If found, dot is updated, the 
 * window system is notified of the change, and TRUE is returned. If the
 * string isn't found, FALSE is returned.
 *
 * While it is true that this method is quadratic in the worst case (the
 * number of comparisons is on the order of i * patlen, where patlen is the
 * length of the pattern in pat and i is the position of the leftmost 
 * character in the first occurrence of pat in the search string), that worst
 * case appears only when initial substrings of pat occur in the search
 * string.  Because this is a relatively rare phenomenon in common English
 * text, this simple algorithm is `practically' linear in i + patlen and
 * therefore acceptable for this application.
 */
static int
forwsrch()
{
	register LINE	*clp;
	register int	cbo;
	register LINE	*tlp;
	register int	tbo;
	register char	*pp;
	register int	c;

	clp = curwp->w_dotp;
	cbo = curwp->w_doto;
	while (clp != curbp->b_linep) {
		if (cbo == llength(clp)) {
			clp = lforw(clp);
			cbo = 0;
			c = '\n';
		} else
			c = lgetc(clp, cbo++);
		if (EQ(c, pat[0]) != FALSE) {
			tlp = clp;
			tbo = cbo;
			pp  = &pat[1];
			while (*pp != 0) {
				if (tlp == curbp->b_linep)
					goto fail;
				if (tbo == llength(tlp)) {
					tlp = lforw(tlp);
					if (tlp == curbp->b_linep)
						goto fail;
					tbo = 0;
					c = '\n';
				} else
					c = lgetc(tlp, tbo++);
				c = EQ(c, *pp);
				pp++;
				if (c == FALSE)
					goto fail;
			}
			curwp->w_dotp  = tlp;
			curwp->w_doto  = tbo;
			curwp->w_flag |= (WFMOVE|WFMODE);  /* to change ratio */
			return (TRUE);
		}
	fail:	;
	}
	return (FALSE);
}

/*
 * This routine does the real work of a backward search. The pattern is 
 * sitting in the external variable "pat". If found, dot is updated, the
 * window system is notified of the change, and TRUE is returned. If the
 * string isn't found, FALSE is returned.
 */
static int
backsrch()
{
	register LINE	*clp;
	register int	cbo;
	register LINE	*tlp;
	register int	tbo;
	register int	c;
	register char	*epp;
	register char	*pp;

	for (epp = &pat[0]; epp[1] != 0; ++epp)
		;
	clp = curwp->w_dotp;
	cbo = curwp->w_doto;
	for (;;) {
		if (cbo == 0) {
			clp = lback(clp);
			if (clp == curbp->b_linep)
				return (FALSE);
			cbo = llength(clp)+1;
		}
		if (--cbo == llength(clp))
			c = '\n';
		else
			c = lgetc(clp, cbo);
		if (EQ(c, *epp) != FALSE) {
			tlp = clp;
			tbo = cbo;
			pp  = epp;
			while (pp != &pat[0]) {
				if (tbo == 0) {
					tlp = lback(tlp);
					if (tlp == curbp->b_linep)
						goto fail;
					tbo = llength(tlp)+1;
				}
				if (--tbo == llength(tlp))
					c = '\n';
				else
					c = lgetc(tlp, tbo);
				--pp;
				if (EQ(c, *pp) == FALSE)
					goto fail;
			}
			curwp->w_dotp  = tlp;
			curwp->w_doto  = tbo;
			curwp->w_flag |= (WFMOVE|WFMODE);  /* to change ratio */
			return (TRUE);
		}
	fail:	;
	}
}

/*
 * Read a pattern.
 * Stash it in the external variable "pat". The "pat" is
 * not updated if the user types in an empty line. If the user typed
 * an empty line, and there is no old pattern, it is an error.
 * Display the old pattern, in the style of Jeff Lomicka. There is
 * some do-it-yourself control expansion.
 */
int
readpattern(prompt)
char	*prompt;
{
	register int	s;
	char		tpat[2*NPAT];
	char		npat[2*NPAT];

	tpat[0] = '\0';
	expandpat(pat, npat, sizeof(npat));
	(void) sprintf(tpat, "%s [%s]: ", prompt, npat);
        s = mlreply(tpat, tpat, sizeof(tpat));
	if (s == TRUE)				/* Specified		*/
		strcpy(pat, tpat);
	else if (s == FALSE && pat[0] != 0)	/* CR, but old one	*/
		s = TRUE;
	return (s);
}

/*
 * Clear the search pattern
 */
/*ARGSUSED*/
int
clearpat(f, n)
int	f, n;
{
	pat[0] = '\0';
	return TRUE;
}

/* 	
 * expandpat:   expand control key sequences for output
 */
int
expandpat(srcstr, deststr, maxlength)
char	*srcstr;	/* string to expand */
char	*deststr;	/* destination of expanded string */
int 	maxlength;	/* maximum chars in destination */
{
	char c;		/* current char to translate */

	/* scan through the string */
	while ((c = *srcstr++) != 0) {
		if (c == '\n') {
			*deststr++ = '<';
			*deststr++ = 'N';
			*deststr++ = 'L';
			*deststr++ = '>';
			maxlength -= 4;
		} else if (ISCTRL(c)) {         /* control character */
			*deststr++ = '^';
			*deststr++ = c ^ 0x40;
			maxlength -= 2;
		} else if (c == '%') {
			*deststr++ = '%';
			*deststr++ = '%';
			maxlength -= 2;

		} else {			/* any other character */
			*deststr++ = c;
			maxlength--;
		}
		/* check for maxlength */
		if (maxlength < 4) {
			*deststr++ = '$';
			*deststr = '\0';
			return(FALSE);
		}
	}
	*deststr = '\0';
	return(TRUE);
}

/*
 * Do a search and replace without query (just do it)
 */
int
searchrepl(f, n)
int f, n;
{
	char	extpat[2*NPAT], exrpat[2*NPAT];
	int 	s, slen;
	int 	nsub, nmatch;
	char 	buf[NSTRING];
	static char rpat[NPAT];

	if (curbp->b_mode & MDVIEW) 
		return(rdonly());
	if (f && n <= 0) 
		return (FALSE);
	if ((s = readpattern("Replace String")) != TRUE)
		return (s);
	(void) expandpat(pat, extpat, sizeof(extpat));
	(void) sprintf(buf, "Replace String %s with: ", extpat);
	if (( s= mlreply(buf, rpat, NPAT)) == ABORT)
		return (s);
	if (s == FALSE) 
		rpat[0] = '\0';
	(void) expandpat(rpat, exrpat, sizeof(exrpat));
	mlwrite("Replacing %s with %s", extpat, exrpat);
	slen = strlen(pat);
	nsub = nmatch = 0;
	while (1) {
		if ((f == TRUE) && (n == nmatch))
			break;
        	if (forwsrch() != TRUE)
			break;
		++nmatch;
		if (lreplace(slen, rpat, f) == FALSE)    /* do the replace  */
			return(FALSE);
		++nsub;
	}
	curwp->w_flag |= (WFHARD|WFMODE);	      /* MODE to change the ratio */
	update();
	if (nsub == 0) 
		mlwrite("No replacements done");
	else if (nsub == 1)
		mlwrite("1 replacement done");
	else
		mlwrite("%d replacements done", nsub);
	return(TRUE);
}
