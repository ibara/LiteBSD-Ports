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
 * window.c -- window management functions
 *
 * Window management. Some of the functions are internal, and some are
 * attached to keys that the user actually types.
 */

#include <stdio.h>
#include "celibc.h"
#include "ce.h"

/*
 * Reposition dot in the current window to line "n". If the argument is
 * positive, it is that line. If it is negative it is that line from the
 * bottom. If it is 0 the window is centered (this is what the standard
 * redisplay code does). With no argument it defaults to 1. Bound to M-!.
 * Because of the default, it works like in Gosling.
 */
int
reposition(f, n)
int	f, n;
{
	if (f == FALSE)	/* default to 0 to center screen */
		n = 0;
	curwp->w_force = n;
	curwp->w_flag |= WFFORCE;
	return (TRUE);
}

/*
 * Refresh the screen. With an argument, it just does the refresh. With no
 * argument it recenters "." in the current window. Bound to "C-L".  If the
 * window size has been changed, resize the terminal.
 *
 * The sense of the argument was switched to make it more like GNU.
 */
/*ARGSUSED*/
int
refresh(f, n)
int	f, n;
{
	register WINDOW *wp;
	register int onrow, oncol;

	onrow = term.t_nrow;
	oncol = term.t_ncol;
	(*term.t_resize)();
	if (term.t_nrow != onrow || term.t_ncol != oncol) {
		wp = wheadp;
		while (wp->w_wndp != NULL) 
			wp = wp->w_wndp;
		if (term.t_nrow < wp->w_toprow + 3) {
			mlwrite("Display too small");
			return (FALSE);
		}
		wp->w_ntrows = term.t_nrow - wp->w_toprow - 1;
		sgarbf = TRUE;
		update(); 
	}
	sgarbf = TRUE;
	if (f == FALSE) {
		curwp->w_force = 0;	     /* Center dot. */
		curwp->w_flag |= WFFORCE;
	}
	update();
	return (TRUE);
}

/*
 * The command make the next window (next => down the screen) the current
 * window. There are no real errors, although the command does nothing if
 * there is only 1 window on the screen. Bound to "C-X C-N".
 */
/*ARGSUSED*/
int
nextwind(f, n)
int	f, n;
{
	register WINDOW *wp;

	if ((wp = curwp->w_wndp) == NULL)
		wp = wheadp;
	curwp = wp;
	curbp = wp->w_bufp;
	upmode();
	return (TRUE);
}

/*
 * This command makes the previous window (previous => up the screen) the
 * current window. There arn't any errors, although the command does not do a
 * lot if there is 1 window.  This is from Gosling, not GNU.
 */
/*ARGSUSED*/
int
prevwind(f, n)
int	f, n;
{
	register WINDOW *wp1;
	register WINDOW *wp2;

	wp1 = wheadp;
	wp2 = curwp;
	if (wp1 == wp2)
		wp2 = NULL;
	while (wp1->w_wndp != wp2)
		wp1 = wp1->w_wndp;
	curwp = wp1;
	curbp = wp1->w_bufp;
	upmode();
	return (TRUE);
}

/*
 * This command moves the current window down by "arg" lines. Recompute the
 * top line in the window. The move up and move down code is almost completely
 * the same; most of the work has to do with reframing the window, and picking
 * a new dot. We share the code by having "move down" just be an interface to
 * "move up". Magic. Bound to "C-X C-N".
 */
int
mvdnwind(f, n)
int	f, n;
{
	return (mvupwind(f, -n));
}

/*
 * Move the current window up by "arg" lines. Recompute the new top line of
 * the window. Look to see if "." is still on the screen. If it is, you win.
 * If it isn't, then move "." to center it in the new framing of the window
 * (this command does not really move "."; it moves the frame). Bound to
 * "C-X C-P".
 */
/*ARGSUSED*/
int
mvupwind(f, n)
int	f, n;
{
	register LINE *lp;
	register int i;

	lp = curwp->w_linep;
	if (n < 0) {
		while (n++ && lp!=curbp->b_linep)
		lp = lforw(lp);
	} else {
		while (n-- && lback(lp)!=curbp->b_linep)
		lp = lback(lp);
	}
	curwp->w_linep = lp;
	curwp->w_flag |= WFHARD;	    /* Mode line is OK. */
	for (i = 0; i < curwp->w_ntrows; ++i) {
		if (lp == curwp->w_dotp)
			return (TRUE);
		if (lp == curbp->b_linep)
			break;
		lp = lforw(lp);
	}
	lp = curwp->w_linep;
	i  = curwp->w_ntrows/2;
	while (i-- && lp != curbp->b_linep)
		lp = lforw(lp);
	curwp->w_dotp  = lp;
	curwp->w_doto  = 0;
	return (TRUE);
    }

/*
 * This command makes the current window the only window on the screen. Bound
 * to "C-X 1". Try to set the framing so that "." does not have to move on the
 * display. Some care has to be taken to keep the values of dot and mark in
 * the buffer structures right if the distruction of a window makes a buffer
 * become undisplayed.
 */
/*ARGSUSED*/
int
onlywind(f, n)
int	f, n;
{
	register WINDOW *wp; 
	register LINE   *lp;
	register int    i;

	while (wheadp != curwp) {
		wp = wheadp;
		wheadp = wp->w_wndp;
		if (--wp->w_bufp->b_nwnd == 0) {
			wp->w_bufp->b_dotp  = wp->w_dotp;
			wp->w_bufp->b_doto  = wp->w_doto;
			wp->w_bufp->b_markp = wp->w_markp;
			wp->w_bufp->b_marko = wp->w_marko;
		}
		free((char *) wp);
	}
	while (curwp->w_wndp != NULL) {
		wp = curwp->w_wndp;
		curwp->w_wndp = wp->w_wndp;
		if (--wp->w_bufp->b_nwnd == 0) {
			wp->w_bufp->b_dotp  = wp->w_dotp;
			wp->w_bufp->b_doto  = wp->w_doto;
			wp->w_bufp->b_markp = wp->w_markp;
			wp->w_bufp->b_marko = wp->w_marko;
		}
		free((char *) wp);
	}
	lp = curwp->w_linep;
	i  = curwp->w_toprow;
	while (i != 0 && lback(lp) != curbp->b_linep) {
		--i;
		lp = lback(lp);
	}
	curwp->w_toprow = 0;
	curwp->w_ntrows = term.t_nrow - 1;
	curwp->w_linep  = lp;
	curwp->w_flag |= WFMODE|WFHARD;
	update();
	return (TRUE);
}

/*
 * Delete the current window.
 *
 * XXX -- there are still problems with this, which is why it's unbound
 * by default.  The policy for deciding who gets the space needs a lot of
 * work, as does the buffer management.
 */
int
delwind(f, n)
int	f, n;
{
	WINDOW	*nw, *wp, *pw;
	LINE	*lp;
	int	nt, tr, ishead, i;

	if (wheadp->w_wndp == NULL) {
		mlwrite("Cannot delete sole window");
		return (FALSE);
	}
	ishead = curwp == wheadp;
	/*
	 * If the window to be killed is not the first window in the list
	 * (at the top of the screen), pw points to the previous window in
	 * the list.
	 */
	if (!ishead)
		for (pw = wheadp; pw->w_wndp != curwp; pw = pw->w_wndp)
			;
	wp = curwp;
	/* 
	 * nw is the next window in the list; it will be the new current
	 * window
	 */
	if ((nw = wp->w_wndp) == NULL)
		nw = wheadp;
	/*
	 * Save values from old window if necessary, and free the window.
	 */
	lp = wp->w_linep;
	nt = wp->w_ntrows;
	tr = wp->w_toprow;
	if (--wp->w_bufp->b_nwnd == 0) {
		wp->w_bufp->b_dotp  = wp->w_dotp;
		wp->w_bufp->b_doto  = wp->w_doto;
		wp->w_bufp->b_markp = wp->w_markp;
		wp->w_bufp->b_marko = wp->w_marko;
	}
	/*
	 * Remove wp from window list
	 */
	if (ishead)
		wheadp = nw;
	else
		pw->w_wndp = wp->w_wndp;
		
	free((char *) wp);
	/*
	 * Figure out who gets the additional screen space
	 */
	if (ishead) {
		/*
		 * If killed window is wheadp, give the next window the
		 * screen space by adjusting toprow and the number of rows
		 */
		nw->w_toprow = tr;
		nw->w_ntrows += nt + 1;		/* +1 for mode line */
#if 0
	/* This doesn't work yet */
	} else if (pw->w_wndp == NULL) {
		int	nrow, numw;
		int	top, toadd, extra;
		/*
		 * last window on screen, distribute its space among the
		 * other windows.
		 */
		nrow = nt + 1;
		for (numw = 0, wp = wheadp; wp != NULL; numw++, wp = wp->w_wndp)
			;
		toadd = nrow / numw;
		extra = nrow % numw;
		for (top = 0, wp = wheadp; numw > 0; numw--, wp = wp->w_wndp) {
			wp->w_toprow = top;
			wp->w_ntrows += toadd;
			if (extra) {
				wp->w_ntrows++;
				top++;
				extra--;
			}
			top += wp->w_ntrows;
		}
		sgarbf = TRUE;
#endif
	} else {
		/*
		 * give it to window `pw', which is above wp on the screen
		 */
		pw->w_ntrows += nt + 1;
		pw->w_flag |= WFMODE|WFHARD;
	}
	curwp = nw;
	curwp->w_flag |= WFMODE|WFHARD;
	update();
	return TRUE;
}

/*
 * Split the current window. A window smaller than 3 lines cannot be split.
 * The only other error that is possible is a "malloc" failure allocating the
 * structure for the new window. Bound to "C-X 2".
 */
/*ARGSUSED*/
int
splitwind(f, n)
int	f, n;
{
	register WINDOW *wp;
	register LINE   *lp;
	register int    ntru;
	register int    ntrl;
	register int    ntrd;
	register WINDOW *wp1;
	register WINDOW *wp2;

	if (curwp->w_ntrows < 3) {
		mlwrite("Cannot split a %d line window", curwp->w_ntrows);
		return (FALSE);
	}
	if ((wp = (WINDOW *) malloc(sizeof(WINDOW))) == NULL) {
		mlwrite("Cannot allocate WINDOW block");
		return (FALSE);
	}
	++curbp->b_nwnd;			/* Displayed twice.     */
	wp->w_bufp  = curbp;
	wp->w_dotp  = curwp->w_dotp;
	wp->w_doto  = curwp->w_doto;
	wp->w_markp = curwp->w_markp;
	wp->w_marko = curwp->w_marko;
	wp->w_flag  = 0;
	wp->w_force = 0;
	ntru = (curwp->w_ntrows-1) / 2;	 /* Upper size */
	ntrl = (curwp->w_ntrows-1) - ntru;      /* Lower size */
	lp = curwp->w_linep;
	ntrd = 0;
	while (lp != curwp->w_dotp) {
		++ntrd;
		lp = lforw(lp);
	}
	lp = curwp->w_linep;
	if (ntrd <= ntru) {		     /* Old is upper window. */
		if (ntrd == ntru)	       /* Hit mode line.       */
			lp = lforw(lp);
		curwp->w_ntrows = ntru;
		wp->w_wndp = curwp->w_wndp;
		curwp->w_wndp = wp;
		wp->w_toprow = curwp->w_toprow+ntru+1;
		wp->w_ntrows = ntrl;
	} else {				/* Old is lower window  */
		wp1 = NULL;
		wp2 = wheadp;
		while (wp2 != curwp) {
			wp1 = wp2;
			wp2 = wp2->w_wndp;
		}
		if (wp1 == NULL)
			wheadp = wp;
		else
			wp1->w_wndp = wp;
		wp->w_wndp   = curwp;
		wp->w_toprow = curwp->w_toprow;
		wp->w_ntrows = ntru;
		++ntru;			 /* Mode line.	   */
		curwp->w_toprow += ntru;
		curwp->w_ntrows  = ntrl;
		while (ntru--)
			lp = lforw(lp);
	}
	curwp->w_linep = lp;		    /* Adjust the top lines */
	wp->w_linep = lp;		       /* if necessary.	*/
	curwp->w_flag |= WFMODE|WFHARD;
	wp->w_flag |= WFMODE|WFHARD;
	return (TRUE);
}

/*
 * Enlarge the current window. Find the window that loses space. Make sure it
 * is big enough. If so, hack the window descriptions, and ask redisplay to do
 * all the hard work. You don't just set "force reframe" because dot would
 * move. Bound to "C-X Z".
 */
int
enlargewind(f, n)
int	f, n;
{
	register WINDOW *adjwp;
	register LINE   *lp;
	register int    i;

	if (n < 0)
		return (shrinkwind(f, -n));
	if (wheadp->w_wndp == NULL) {
		mlwrite("Only one window");
		return (FALSE);
	}
	if ((adjwp=curwp->w_wndp) == NULL) {
		adjwp = wheadp;
		while (adjwp->w_wndp != curwp)
			adjwp = adjwp->w_wndp;
	}
	if (adjwp->w_ntrows <= n) {
		mlwrite("Impossible change");
		return (FALSE);
	}
	if (curwp->w_wndp == adjwp) {	   /* Shrink below.	*/
		lp = adjwp->w_linep;
		for (i=0; i<n && lp!=adjwp->w_bufp->b_linep; ++i)
			lp = lforw(lp);
		adjwp->w_linep  = lp;
		adjwp->w_toprow += n;
	} else {				/* Shrink above.	*/
		lp = curwp->w_linep;
		for (i=0; i<n && lback(lp)!=curbp->b_linep; ++i)
			lp = lback(lp);
		curwp->w_linep  = lp;
		curwp->w_toprow -= n;
	}
	curwp->w_ntrows += n;
	adjwp->w_ntrows -= n;
	curwp->w_flag |= WFMODE|WFHARD;
	adjwp->w_flag |= WFMODE|WFHARD;
	return (TRUE);
}

/*
 * Shrink the current window. Find the window that gains space. Hack at the
 * window descriptions. Ask the redisplay to do all the hard work. Bound to
 * "C-X C-Z".
 */
int
shrinkwind(f, n)
int	f, n;
{
	register WINDOW *adjwp;
	register LINE   *lp;
	register int    i;

	if (n < 0)
		return (enlargewind(f, -n));
	if (wheadp->w_wndp == NULL) {
		mlwrite("Only one window");
		return (FALSE);
	}
	if ((adjwp=curwp->w_wndp) == NULL) {
		adjwp = wheadp;
		while (adjwp->w_wndp != curwp)
			adjwp = adjwp->w_wndp;
	}
	if (curwp->w_ntrows <= n) {
		mlwrite("Impossible change");
		return (FALSE);
	}
	if (curwp->w_wndp == adjwp) {	   /* Grow below.	  */
		lp = adjwp->w_linep;
		for (i=0; i<n && lback(lp)!=adjwp->w_bufp->b_linep; ++i)
			lp = lback(lp);
		adjwp->w_linep  = lp;
		adjwp->w_toprow -= n;
	} else {				/* Grow above.	  */
		lp = curwp->w_linep;
		for (i=0; i<n && lp!=curbp->b_linep; ++i)
			lp = lforw(lp);
		curwp->w_linep  = lp;
		curwp->w_toprow += n;
	}
	curwp->w_ntrows -= n;
	adjwp->w_ntrows += n;
	curwp->w_flag |= WFMODE|WFHARD;
	adjwp->w_flag |= WFMODE|WFHARD;
	return (TRUE);
}

/*
 * Pick a window for a pop-up. Split the screen if there is only one window.
 * Pick the uppermost window that isn't the current window. An LRU algorithm
 * might be better. Return a pointer, or NULL on error.
 */
WINDOW  *
wpopup()
{
	register WINDOW *wp;

	/*
	 * If there's only 1 window, and it won't split, we fail.
	 */
	if (wheadp->w_wndp == NULL && splitwind(FALSE, 0) == FALSE)
		return (NULL);
	wp = wheadp;			    /* Find window to use   */
	while (wp != NULL && wp == curwp)
		wp = wp->w_wndp;
	return (wp);
}

int
scrnextup(f, n)		/* scroll the next window up (back) a page */
int	f, n;
{
	nextwind(FALSE, 1);
	backpage(f, n);
	prevwind(FALSE, 1);
	return (TRUE);
}

int
scrnextdw(f, n)		/* scroll the next window down (forward) a page */
int	f, n;
{
	nextwind(FALSE, 1);
	forwpage(f, n);
	prevwind(FALSE, 1);
	return (TRUE);
}
