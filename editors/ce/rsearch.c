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
 *  rsearch.c  -- regular expression searching commands
 */

#include <stdio.h>
#include "celibc.h"
#include "ce.h"
#include "regexp.h"

#ifndef CHARBITS
#define	UCHARAT(p)	((int)*(unsigned char *)(p))
#else
#define	UCHARAT(p)	((int)*(p)&CHARBITS)
#endif

static regexp	*repat = NULL;		/* compiled reg exp of pat[] */

static char	*regsubst();

/*
 * Make the search pattern case-sensitive by changing all ocurrences
 * of letter x to be "[xX]" (i.e. search for both lower and upper
 * case).  If the search pattern already includes any of the `special'
 * characters, then we leave it alone.
 */

#define METACHARS    "^$.[()|?+*\\"

static void
casesense(tpat)
char	*tpat;
{
	register char  *p, *q, *r, c;

	for (p = tpat; p && *p; p++) {
		if (strchr(METACHARS, *p) != NULL)
			return;		/* Don't change tpat */
	}
	/*
	 * OK, it contains no meta-characters.  Make it case-insensitive.
	 */
	r = (char *) malloc(4 * strlen(tpat) + 1);
	if (r) {
		for (p = r, q = tpat; c = *q++;) {
			if (ISLOWER(c) != FALSE) {
				*p++ = '['; 
				*p++ = c; 
				*p++ = TOUPPER(c);
				*p++ = ']';
			} else if (ISUPPER(c) != FALSE) {
				*p++ = '['; 
				*p++ = TOLOWER(c);
				*p++ = c;
				*p++ = ']';
			} else 
				*p++ = c;
		}
		*p = '\0';
		if (strlen(r) < 2*NPAT)
			strcpy(tpat, r);
		free (r);
	}
}

/*
 * Read a regexp pattern.  Stash it in the external variable "pat".  The
 * "pat" is not updated if the user types in an empty line.  If the user
 * typed an empty line, and there is no old pattern, it is an error.  To
 * escape metachars, you precede them by ^Q; the escape char in regex(3)
 * is '\\' .  Process the pattern read in pat into the regexp pattern in
 * 'repat'.
 */
static int
re_readpattern(prompt)
register char *prompt;
{	
	register int	s;
	register char	*p, *q, c;
	char		tpat[2*NPAT];

	if ((s = readpattern(prompt)) != TRUE)
		return(s);
	if (repat)
		free((char *) repat); 
	repat = (regexp *) NULL;
	if (pat[0]) {	
		for (q = pat, p = tpat; c = *q++; *p++ = c)
			if (c == CCHR('Q'))
				c = '\\';
		*p = '\0';
		casesense(tpat);
		repat = regcomp(tpat);
	}
	return (repat != NULL);
}

/*
 * Make a string for regexec() to use out of a line.  The original version
 * of this code actually malloc'd space for the line; now we cheat by 
 * allocating an extra byte to the line in lalloc().  This fails if the 
 * user types in a '\0' as part of a line, but that doesn't happen too often.
 * (And whoever does it deserves what he gets!)
 */
static char *
linetostr(lp)
register LINE *lp;
{
	register char *s;

	s = lp->l_text;
	s[llength(lp)] = '\0';
	return s;
}

#if 0
/*
 * Convenience functions for use from other than the regular search
 * functions.  Return a pointer to the matching line or NULL on
 * failure.
 */
static LINE *
re_forward(p)
char	*p;
{
	struct regexp	*tmp;
	LINE		*lp;
	char		*s;
	
	tmp = regcomp(p);
	if (tmp == NULL)
		return ((LINE *) NULL);
	lp = curwp->w_dotp;
	while (lp != curbp->b_linep) {
	  	s = linetostr(lp);
		if (regexec(tmp, s) == 1)
			return(lp);
		lp = lforw(lp);
	}
	return ((LINE *) NULL);
}

static LINE *
re_backward(p)
char	*p;
{
	struct regexp	*tmp;
	LINE		*lp;
	char		*s;

	tmp = regcomp(p);
	if (tmp == NULL)
		return((LINE *) NULL);
	lp = curwp->w_dotp;
	if (lp == curbp->b_linep)
		lp = lback(lp);
	while (lp != curbp->b_linep) {	
		s = linetostr(lp); 
		if (regexec(tmp, s) == 1)
			return(lp);
		lp = lback(lp);
	}
	return ((LINE *) NULL);
}
#endif

/* 
 * This routine does the real work of a regexp forward search.  The pattern
 * is sitting in the external variable "repat".  If found, dot is updated,
 * the window system is notified of the change, and TRUE is returned.
 */
static int
re_fsearch()
{	
	register LINE	*lp;
	register int	doto;
	register char	*s;

	lp = curwp->w_dotp;
	doto = curwp->w_doto;
	while (lp != curbp->b_linep) {
	  	s = linetostr(lp);
		if (regexec(repat, s + doto) == 1)
			goto found;
		lp = lforw(lp);
		doto = 0;
	}
	return (FALSE);
found:
	curwp->w_dotp = lp;
	curwp->w_doto = repat->endp[0] - s;
	curwp->w_flag |= (WFMOVE|WFMODE);	/* MODE to change the ratio */
	return (TRUE);
}

/* 
 * This routine does the real work of a regexp backward search.  The pattern
 * is sitting in the external variable "pat".  If found, dot is updated,
 * the window system is notified of the change, and TRUE is returned.  If
 * the string isn't found, FALSE is returned.
 */
static int
re_bsearch()
{
	register LINE	*lp;
	register int	doto;
	register int	i;
	register char	*s, c = 0;

	lp = curwp->w_dotp;
	if (lp == curbp->b_linep)
		lp = lback(lp);
	doto = curwp->w_doto;
	while (lp != curbp->b_linep) {	
		s = linetostr(lp); 
		if (doto >= 0) {
			c = s[doto]; 
			s[doto] = '\0';
		}
		if (regexec(repat, s) == 1)
			goto found;
		if (doto >= 0) {
			s[doto] = c; 
			doto = -1;
		}
		lp = lback(lp);
	}
	return (FALSE);
found:
	for (i = strlen(s); i; i--)
		if (regexec(repat, s + i)==1)
			break;
	if (doto >= 0) 
		s[doto] = c;
	curwp->w_dotp = lp;
	curwp->w_doto = i;
	curwp->w_flag |= (WFMOVE|WFMODE);	/* MODE to change the ratio */
	return (TRUE);
}

/* 
 * Search forward.  Get a search string from the user, and search for
 * it, starting at ".".  If found, "." gets moved to just after the
 * matched characters, and display does all the hard stuff.  If not
 * found, it just prints a message.
 */
/*ARGSUSED*/
int
regexfsearch(f, n)
int	f, n;
{	
	register int	s;

	s = re_readpattern("Regexp Search");
	if (s != TRUE)
		return (s);
	s = re_fsearch();
	if (s == FALSE) 
		mlwrite("Search failed: \"%s\"", pat);
	return s;

}

/* 
 * Reverse search.  Get a search string from the user, and search,
 * starting at "." and proceeding toward the front of the buffer.  If
 * found "." is left pointing at the first character of the pattern [the
 * last character that was matched].
 */
/*ARGSUSED*/
int
regexbsearch(f, n)
int	f, n;
{
	register int	s;

	s = re_readpattern("Regexp Search Backward");
	if (s != TRUE)  
		return (s);
	s = re_bsearch();
	if (s == FALSE) 
		mlwrite("Search failed: \"%s\"", pat);
	return s;
}

/*
 * regsubst -- perform substitutions after a regexp match
 *
 *	Copyright (c) 1986 by University of Toronto.
 *	Written by Henry Spencer.  Not derived from licensed software.
 *
 * Redone by Chet Ramey to return a pointer to the new string.
 *
 */

/*
 - regsubst - perform substitutions after a regexp match
 *
 * Altered by Chet Ramey to allocate and return a pointer to the new
 * string.  The caller (always regexqueryrepl or regexrepl) must free
 * this string.
 */
static char *
regsubst(prog, source)
regexp 	*prog;
char	*source;
{
	register char 	*src, *dst;
	register char	c;
	register int 	no, len;
	char 		*dest;

	if (prog == NULL || source == NULL) {
		regerror("NULL parm to regsub");
		return ((char *) NULL);
	}
	if (UCHARAT(prog->program) != MAGIC) {
		regerror("damaged regexp fed to regsub");
		return ((char *) NULL);
	}
	if ((dest = (char *) malloc (2 * NLINE * sizeof(char))) == NULL) {
		regerror("Could not allocate replacement string");
		return ((char *) NULL);
	}
	src = source;
	dst = dest;
	while ((c = *src++) != '\0') {
		if (c == '&')
			no = 0;
		else if (c == '\\' && '0' <= *src && *src <= '9')
			no = *src++ - '0';
		else
			no = -1;

		if (no < 0)	/* Ordinary character. */
			*dst++ = c;
		else if (prog->startp[no] != NULL && prog->endp[no] != NULL) {
			len = prog->endp[no] - prog->startp[no];
			(void) strncpy(dst, prog->startp[no], len);
			dst += len;
			if (len != 0 && *(dst-1) == '\0') {		/* strncpy hit NUL. */
				regerror("damaged match string");
				if (dest)
					free(dest);
				return ((char *) NULL);
			}
		}
	}
	*dst++ = '\0';
	return dest;
}

int
regerror(s)
char *s;
{
	mlwrite("regexp: %s", s);
	return (0);
}


/* 
 * Query Replace.  Replace strings selectively.  Does a search
 * and replace operation.
 * This needs more work, especially "GNU-ifying" the commands
 */
/*ARGSUSED*/
int
regexqueryrepl(f, n)
int	f, n;
{
	register int	s, c;
	register int	rcnt;		/* Replacements made so far	*/
	register int	plen;		/* length of found string	*/
	char		rpat[NPAT];	/* replacement string		*/
	char		buf[NSTRING];
	char		expat[2*NPAT], exrpat[2*NPAT];
	char		*rep;		/*  replacement string after sub*/
	LINE		*clp;
	int		cbo;

	rpat[0] = '\0';
	if ((s = re_readpattern("Query replace regexp")) != TRUE)
		return (s);
	(void) expandpat(pat, expat, sizeof(expat));	   /* maybe not needed */
	(void) sprintf(buf, "Query replace regexp %s with: ", expat);
	s = mlreply(buf, rpat,  NPAT);
	if (s != TRUE)
		return s;
	(void) expandpat(rpat, exrpat, sizeof(exrpat));	   /* maybe not needed */
	mlwrite("Query replacing regexp %s with %s:", expat, exrpat);
	clp = curwp->w_dotp;
	cbo = curwp->w_doto;
	rcnt = 0;
	while (re_fsearch() == TRUE) {
retry:
		update();
		c = (*term.t_getchar)(); 
		if (c == -1)
			c = ' ';
		switch(c) {
		case ' ':			/* replace */
		case '.':			/* replace and quit */
			rep = regsubst(repat, rpat);
			plen = repat->endp[0] - repat->startp[0];
			s = lreplace(plen, rep, FALSE); 
			if (rep)
				free(rep);
			if (s == FALSE)
				goto stopsearch;
			rcnt++;
			if (c != '.')
				break;
			/* FALLTHROUGH */
		case 0x7F:			/* don't replace */
		case CCHR('H'):
			if ((pat[0] == '^') || (pat[strlen(pat) - 1] == '$')) {
				if (lforw(curwp->w_dotp) == curbp->b_linep)
					goto stopsearch;
				curwp->w_dotp = lforw(curwp->w_dotp);
				curwp->w_doto = 0;
			}
			if (c == '.')
				goto stopsearch;
			break;
		case '!':			/* replace rest w/o asking */
			do {
				rep = regsubst(repat, rpat);
			    	plen = repat->endp[0] - repat->startp[0];
			    	s = lreplace(plen, rep, FALSE); 
				if (rep)
					free(rep);
				if (s == FALSE)
					goto stopsearch;
				rcnt++;
				if ((pat[0] == '^') || (pat[strlen(pat) - 1] == '$')) {
					if (lforw(curwp->w_dotp) == curbp->b_linep)
						goto stopsearch;
					curwp->w_dotp = lforw(curwp->w_dotp);
					curwp->w_doto = 0;
				}
			} while(re_fsearch() == TRUE);
			goto stopsearch;
		case CCHR('G'):		/* abort */
			ctrlg(FALSE, 0);
		case '\033':		/* quit */
			goto stopsearch;
		default:
			mlwrite("<SP> replace, [.] rep-end, <DEL> don't, [!] repl rest <ESC> quit");
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
	return TRUE;
}

/*
 * Search for and replace all occurrences of a regexp without querying (just
 * do it).
 */
int
regexrepl(f, n)
int	f, n;
{
	int		s, rcnt, mcnt, plen;
	char		expat[2*NPAT], exrpat[2*NPAT];
	char		buf[NSTRING];
	char		*rep;		/*  replacement string after sub*/
	LINE		*clp;
	int		cbo;
	static char	rpat[NPAT];

	if (curbp->b_mode & MDVIEW)
		return (rdonly());
	if (f && n <= 0)
		return (FALSE);
	if ((s = re_readpattern("Replace regexp")) != TRUE)
		return s;
	(void) expandpat(pat, expat, sizeof(expat));
	(void) sprintf(buf,"Replace regexp %s with: ", expat);
	if ((s = mlreply(buf, rpat, sizeof(rpat))) == ABORT)
		return ABORT;
	(void) expandpat(rpat, exrpat, sizeof(exrpat));	   /* maybe not needed */
	mlwrite("Replacing regexp %s with %s", expat, exrpat);
	clp = curwp->w_dotp;
	cbo = curwp->w_doto;
	rcnt = mcnt = 0;
	while (1) {
		if ((f == TRUE) && (mcnt == n))
			break;
		if (re_fsearch() != TRUE)
			break;
		++mcnt;
		rep = regsubst(repat, rpat);
		plen = repat->endp[0] - repat->startp[0];
		s = lreplace(plen, rep, FALSE);
		if (s == FALSE)
			break;
		free(rep);
		rcnt++;
		/*
		 * If the pattern requires that we match only on the current
		 * line, move to the next line before calling re_fsearch again.
		 *
		 * This is bad -- it knows about the format of regular
		 * expressions. 
		 */
		if ((pat[0] == '^') || (pat[strlen(pat) - 1] == '$')) {
			if (lforw(curwp->w_dotp) == curbp->b_linep)
				break;
			curwp->w_dotp = lforw(curwp->w_dotp);
			curwp->w_doto = 0;
		}
	}
	curwp->w_dotp = clp;
	curwp->w_doto = cbo;
	curwp->w_flag |= (WFHARD|WFMODE);
	update();
	if (rcnt == 0)
		mlwrite("No replacements done");
	else if (rcnt == 1)
		mlwrite("1 replacement done");
	else
		mlwrite("%d replacements done", rcnt);
	return (TRUE);
}
