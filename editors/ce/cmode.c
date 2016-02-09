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
 * cmode.c -- Functions to implement a major mode for writing C code
 *
 * TODO: decide when we're in a comment, and disable this stuff
 * 	 appropriately.
 */
#include "celibc.h"
#include "ce.h"

#define WHITE(c)	((c) == ' ' || (c) == '\t')

extern LMATCH	*findmat();

/*
 * Return TRUE if the current line contains all white space
 */

static int
allwhite()
{
	register int i, c;
	
	for (i = llength(curwp->w_dotp) - 1; i >= 0; --i) {
		c = lgetc(curwp->w_dotp, i);
		if (WHITE(c) == FALSE)
			return(FALSE);
	}
	return(TRUE);
}

/*
 * Insert a newline and appropriate indentation for C.  The `appropriate'
 * indentation is the indent of the previous line + a tab if the last
 * character on the previous line was a `{'.
 */
int
cnewline(f, n)
int	f, n;
{
	register char	*cp;	/* Pointer to text to copy */
	register int	i, t;
	register int	wasbrc;	/* Was the last char of the prev line a `{' ? */
	char		ind[NSTRING];

	cp = ltext(curwp->w_dotp);
	t = curwp->w_doto - 1;
	wasbrc = cp[t] == '{';

	for (i = 0; (i < t) && (WHITE(cp[i])) && (i < sizeof(ind) - 1); i++)
		ind[i] = cp[i];

	ind[i] = '\0';

	/*
	 * Add the newline, then the saved indentation.  If there was a
	 * brace, then add an extra tab.
	 */

	if (lnewline() == FALSE)
		return(FALSE);
	for (i = 0; ind[i]; i++)
		if (linsert(1, ind[i]) == FALSE)
			break;
	if (wasbrc)
		tab(FALSE, 1);

	return(TRUE);
}

/*
 * Insert a closing brace into the text.  Find the line with the matching
 * opening brace, and insert the same amount of whitespace.  Then insert
 * the closing brace.
 */
int
insbrace(f, n)
int	f, n;
{
	register int	i, t;
	LMATCH		*match;
	char		*cp, ind[NSTRING];

	/*
	 * If there is not all whitespace before this brace, then we just
	 * insert.
	 */

	if (allwhite() == FALSE)
		return(linsert(1, '}'));

	/*
	 * Otherwise, we find the line with the matching open brace, and
	 * insert the same indentation, then the matching close brace.
	 * If there is no match, we just insert.
	 */
	match = findmat('{', '}', 0);

	if (match == 0)
		return(linsert(1, '}'));

	cp = ltext(match->lp);
	t = llength(match->lp) - 1;
	for (i = 0; (i < t) && (WHITE(cp[i])) && (i < sizeof(ind) - 1); i++)
		ind[i] = cp[i];
	ind[i] = '\0';

	/*
	 * Now we kill all leading whitespace on the current line and insert
	 * the matching amount of whitespace from IND.
	 */

	while (getccol(FALSE) > 0)
		backdel(FALSE, 1);

	for (i = 0; ind[i]; i++)
		if (linsert(1, ind[i]) == FALSE)
			break;

	return(linsert(1, '}'));
}

/*
 * Insert a `#' into the code.  We force it to be the first character on the
 * line.
 */
int
inspound(f, n)
int	f, n;
{
	if (curwp->w_doto == 0)
		return(linsert(1, '#'));

	if (allwhite() == FALSE)
		return(linsert(1, '#'));

	while (getccol(FALSE) > 0)
		backdel(FALSE, 1);

	return(linsert(1, '#'));
}

int
cfile(fn)
char	*fn;
{
	char	*dot = strrchr(fn, '.');

	if (dot) {
		dot++;
		if (dot && *dot && (*dot == 'c' || *dot == 'h' || *dot == 'y')) {
			if (*++dot == '\0')
				return(TRUE);
			else
				return(FALSE);
		} else
			return(FALSE);
	}
	return(FALSE);
}

/*
 * Given CH, the start of a C construct, find the matching end and set the
 * region to the text enclosed.
 */
int
matchregion(ch)
int	ch;
{
	register int	close;
	LMATCH		*match;

	if (ch == '"')
		close = ch;
	else
		close = bchar(ch, 1);

	match = findmat(ch, close, 1);

	if (match) {
		curwp->w_markp = match->lp;
		curwp->w_marko = match->lo;
	} else
		(*term.t_beep)();

	return (TRUE);
}
