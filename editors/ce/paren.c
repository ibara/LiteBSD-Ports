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
 * paren.c -- Functions to implement paren (and other matching character
 *	      pair) balancing
 */

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#ifdef FD_SET
#  include <sys/time.h>
#endif
#include "ce.h"

static void showmatch();

/*
 * Find the matching character in a balanced pair (open, close).  If
 * FORW is true, search forward looking for close; otherwise search
 * back looking for open.
 */
LMATCH *
findmat(open, close, forw)
int	open;	/* opening character of matching region */
int	close;	/* closing character of matching region */
int	forw;
{
	LINE		*dotp;
	int		doto;
	static LMATCH	ret;
	int		count, c;

	dotp = curwp->w_dotp;
	doto = curwp->w_doto;
	if (forw == 0)
		doto--;

	count = 0;

	for (;;) {
		if (forw ? (doto > llength(dotp)) : (doto <= 0)) {
			dotp = forw ? lforw(dotp) : lback(dotp);
			if (dotp == curbp->b_linep) {
				count = 1;	/* Force no match */
				break;
			}
			doto = forw ? 0 : llength(dotp) + 1;
		}
		if (forw == 0)
			doto--;
		if (doto == llength(dotp))
			c = '\n';		/* end of line */
		else
			c = lgetc(dotp, doto);

		if (c == (forw ? close : open)) {
			if (count == 0)
				break;
			else
				count--;
		} else if (c == (forw ? open : close))
			count++;
		if (forw)
			doto++;
	}

	if (count == 0) {
		ret.lp = dotp;
		ret.lo = doto;
		return (&ret);
	} else
		return ((LMATCH *) NULL);
}

/*
 * Given CH, which is the first (ISOPEN == 1) or second in a matching pair
 * of balance characters, return the other character.
 */
int
bchar(ch, isopen)
int	ch, isopen;
{
	int	ret;

	ret = -1;
	if (isopen) {
		if (ch == '{')
			ret = '}';
		else if (ch == '(')
			ret = ')';
		else if (ch == '[')
			ret = ']';
	} else {
		if (ch == '}')
			ret = '{';
		else if (ch == ')')
			ret = '(';
		else if (ch == ']')
			ret = '[';
	}
	return ret;
}

/*
 * Basic fencepost matching
 */
int
balance(ch)
int	ch;
{
	register char	open;
	LMATCH		*match;

	update();

	open = bchar(ch, 0);

	match = findmat(open, ch, 0);

	if (match)
		showmatch(match->lp, match->lo);
	else {
		mlwrite("No match");
		(*term.t_beep)();
	}
	
	return(TRUE);
}

/*
 * Display the matching character at the current position in the buffer.  If
 * the matching line is off the screen, it will be shown in the modeline.
 */
static void
showmatch(clp, cbo)
register LINE	*clp;
register int	cbo;
{
	register LINE	*tlp;
	register int	tbo, cp, bufo;
	int		inwin = FALSE;
	char		buf[NLINE];

	for (tlp = curwp->w_linep; tlp != lforw(curwp->w_dotp); tlp = lforw(tlp))
		if (tlp == clp)
			inwin = TRUE;

	if (inwin) {
		tlp = curwp->w_dotp;
		tbo = curwp->w_doto;
		
		curwp->w_dotp = clp;
		curwp->w_doto = cbo;
		curwp->w_flag |= WFMOVE;

		update();

#ifdef FD_SET
		{
			fd_set fds;
			struct timeval tv;

			tv.tv_sec = 1;
			tv.tv_usec = 0;
			FD_ZERO(&fds);
			FD_SET(0, &fds);
			select(1, &fds, (fd_set *) 0, (fd_set *) 0, &tv);
		}
#else
		sleep(1);
#endif

		curwp->w_dotp = tlp;
		curwp->w_doto = tbo;
		curwp->w_flag |= WFMOVE;
		update();
	} else {
		bufo = cp = 0;
		while (cp < llength(clp)) {
			buf[bufo++] = lgetc(clp, cp);
			cp++;
		}
		buf[bufo] = '\0';
		if (bufo > (term.t_ncol - 2 - 8))  /* strlen("Matches ") == 8 */
			buf[term.t_ncol - 10] = '\0';
		mlwrite("Matches %s", buf);
	}
}

/*
 * Show the matching character for the character at dot in a character pair
 * (e.g., { and }.  The matching code looks at the previous character,
 * since it is normally called after a character is inserted and point has
 * moved, rather than the character at dot, so move forward and back around
 * the call to balance().
 */
int
charmatch(f, n)
int	f, n;
{
	int	r, ch;

	ch = lgetc(curwp->w_dotp, curwp->w_doto);
	forwchar(FALSE, 1);
	r = balance(ch);
	backchar(FALSE, 1);
	return r;
}

/*
 * Actually move point to show the matching brace.  This is a bindable
 * command.
 */
int
showbrace(f, n)
int	f, n;
{
	register int	ch;
	register char	open, close;
	int		pos;
	LMATCH		*match;

	update();		/* flush out pending screen changes */

	ch = lgetc(curwp->w_dotp, curwp->w_doto);

	if ((open = bchar(ch, 0)) == -1) {
		if ((close = bchar(ch, 1)) == -1) {
			mlwrite("[No match]");
			(*term.t_beep)();
			return TRUE;
		} else {
			pos = curwp->w_doto++;
			match = findmat(ch, close, 1);
		}
	} else {
		pos = curwp->w_doto++;
		match = findmat(open, ch, 0);
	}
	curwp->w_doto = pos;

	if (match) {
		curwp->w_markp = match->lp;
		curwp->w_marko = match->lo;
		swapmark(FALSE, 1);
	} else {
		mlwrite("[No match]");
		(*term.t_beep)();
		return FALSE;
	}
	return TRUE;
}
