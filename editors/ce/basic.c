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
 * basic.c -- basic screen routines for ce
 *
 * The routines in this file move the cursor around on the screen. They
 * compute a new value for the cursor, then adjust ".". The display code
 * always updates the cursor location, so only moves between lines, or
 * functions that adjust the top line in the window and invalidate the
 * framing, are hard.
 */

#include <stdio.h>
#include "ce.h"

/*
 * Move the cursor to the beginning of the current line.
 * Trivial.
 */
/*ARGSUSED*/
int
gotobol(f, n)
int	f, n;
{
	curwp->w_doto  = 0;
	return (TRUE);
}

/*
 * Move the cursor backwards by "n" characters. If "n" is less than zero call
 * "forwchar" to actually do the move. Otherwise compute the new cursor
 * location. Error if you try and move out of the buffer. Set the flag if the
 * line pointer for dot changes.
 */
int
backchar(f, n)
register int    f,n;
{
	register LINE   *lp;

	if (n < 0)
		return (forwchar(f, -n));
	while (n--) {
		if (curwp->w_doto == 0) {
			if ((lp=lback(curwp->w_dotp)) == curbp->b_linep)
				return (FALSE);
			curwp->w_dotp  = lp;
			curwp->w_doto  = llength(lp);
			curwp->w_flag |= WFMOVE;
		} else
			curwp->w_doto--;
	}
	return (TRUE);
}

/*
 * Move the cursor to the end of the current line. Trivial. No errors.
 */
/*ARGSUSED*/
int
gotoeol(f, n)
int	f, n;
{
	curwp->w_doto  = llength(curwp->w_dotp);
	return (TRUE);
}

/*
 * Move the cursor forwwards by "n" characters. If "n" is less than zero call
 * "backchar" to actually do the move. Otherwise compute the new cursor
 * location, and move ".". Error if you try and move off the end of the
 * buffer. Set the flag if the line pointer for dot changes.
 */
int
forwchar(f, n)
register int    n;
{
	register LINE *lp;

	if (n < 0)
		return (backchar(f, -n));
	while (n--) {
		lp = curwp->w_dotp;
		if (curwp->w_doto == llength(lp)) {
			if (lp == curbp->b_linep)
				return (FALSE);
			curwp->w_dotp  = lforw(lp);
			curwp->w_doto  = 0;
			curwp->w_flag |= WFMOVE;
		} else
			curwp->w_doto++;
	}
	return (TRUE);
}

/*
 *  Go to a particular line.  If the argument (n) is < 0, then go |n| lines
 *  back from the end.  If f is FALSE, then a prompt is issued and a response
 *  read for the line number to which to move.
 */
int
gotoline(f,n)
int	f, n;
{
	register LINE	*clp;
	register int 	s;
	char 		b[16];

	if (f == FALSE) {
		if ((s = mlreply("Goto Line: ",b,sizeof(b))) != TRUE)
			return(s);
		n = atoi(b);
	}
	if (n > 0) {
		clp = lforw(curbp->b_linep);      /*   first line */
		while (--n) {
			if (lforw(clp) == curbp->b_linep)
				break;
			clp = lforw(clp);
		}
	} else {
		clp = lback(curbp->b_linep);
		while (n < 0) {
			if (lback(clp) == curbp->b_linep)
				break;
			clp = lback(clp);
			n++;
		}
	}
	curwp->w_dotp = clp;
	curwp->w_doto = 0;
	curwp->w_flag |= (WFMOVE|WFMODE);	/* MODE to change the ratio */
	return TRUE;
}


/*
 * Goto the beginning of the buffer. Massive adjustment of dot. This is
 * considered to be hard motion; it really isn't if the original value of dot
 * is the same as the new value of dot. Normally bound to "M-<".
 */
void
igotobob()
{
	curwp->w_dotp  = lforw(curbp->b_linep);
	curwp->w_doto  = 0;
	curwp->w_flag |= (WFHARD|WFMODE);
}

/*ARGSUSED*/
int
gotobob(f,n)
int	f, n;
{
	setmark(FALSE,1);
	igotobob();
	return(TRUE);
}

/*
 * Move to the end of the buffer. Dot is always put at the end of the file.
 * The standard screen code does most of the hard parts of update.
 * Bound to "M->".
 */
void
igotoeob()
{
	curwp->w_dotp  = lback(curbp->b_linep);
	curwp->w_doto  = llength(curwp->w_dotp);
	curwp->w_flag |= (WFHARD|WFMODE);
}

/*ARGSUSED*/
int
gotoeob(f,n)
int	f, n;
{
	setmark(FALSE,1);
	igotoeob();
	return(TRUE);
}

/*
 * Move forward by full lines. If the number of lines to move is less than
 * zero, call the backward line function to actually do it. The last command
 * controls how the goal column is set. Bound to "C-N". No errors are
 * possible.
 */
int
forwline(f, n)
int	f, n;
{
	register LINE   *dlp;
	extern	 LINE	*lallocx();

	if (n < 0)
		return (backline(f, -n));
	if ((lastflag&CFCPCN) == 0)	     /* Reset goal if last   */
		curgoal = getccol(FALSE);       /* not C-P or C-N       */
	thisflag |= CFCPCN;
	dlp = curwp->w_dotp;
	if (n == 0)
		return (TRUE);
	while (dlp != curbp->b_linep && n--)
		dlp = lforw(dlp);
#ifdef C_N_INS_LINE
	/*
	 *  Add lines at the end of the buffer on C-N, like GNU -- CR
	 */
	if (dlp == curbp->b_linep) {
		if ((curbp->b_flag & BFCHG) == 0) {    /* First Change */
			curbp->b_flag |= BFCHG;
			curwp->w_flag |= WFMODE;
		}
		curwp->w_doto = 0;
		while (n-- >= 0) {
			if ((dlp = lallocx(0)) == NULL)
				return(FALSE);
			dlp->l_fp = curbp->b_linep;
			dlp->l_bp = lback(dlp->l_fp);
			dlp->l_bp->l_fp = dlp->l_fp->l_bp = dlp;
		}
		curwp->w_dotp = dlp;
	} else {
		curwp->w_dotp = dlp;
		curwp->w_doto  = getgoal(dlp);
	}
#else
	if (dlp == curbp->b_linep) {
		(*term.t_beep)();
		dlp = lback(dlp);
	}
	curwp->w_dotp = dlp;
	curwp->w_doto  = getgoal(dlp);
#endif /* C_N_INS_LINE */
	curwp->w_flag |= (WFMOVE|WFMODE);
	update();		     		/* is this needed too ?? */
	return (TRUE);
}

/*
 * This function is like "forwline", but goes backwards. The scheme is exactly
 * the same. Check for arguments that are less than zero and call your
 * alternate. Figure out the new line and call the line motion routines
 * to perform the motion. No errors are possible. Bound to "C-P".
 */
int
backline(f, n)
int	f, n;
{
	register LINE   *dlp;

	if (n < 0)
		return (forwline(f, -n));
	if ((lastflag&CFCPCN) == 0)	     /* Reset goal if the    */
		curgoal = getccol(FALSE);       /* last isn't C-P, C-N  */
	thisflag |= CFCPCN;
	dlp = curwp->w_dotp;
	while (n-- && lback(dlp)!=curbp->b_linep)
		dlp = lback(dlp);
	curwp->w_dotp  = dlp;
	curwp->w_doto  = getgoal(dlp);
	curwp->w_flag |= (WFMOVE|WFMODE);
	return (TRUE);
}


/*
 * This routine, given a pointer to a LINE, and the current cursor goal
 * column, return the best choice for the offset. The offset is returned.
 * Used by "C-N" and "C-P".
 */
int
getgoal(dlp)
register LINE   *dlp;
{
	register int    c;
	register int    col;
	register int    newcol;
	register int    dbo;

	col = 0;
	dbo = 0;
	while (dbo != llength(dlp)) {
		c = lgetc(dlp, dbo);
		newcol = col;
		if (c == '\t')
			newcol |= 0x07;
		else if (ISCTRL(c) != FALSE) 
			++newcol;
		++newcol;
		if (newcol > curgoal)
			break;
		col = newcol;
		++dbo;
	}
	return (dbo);
}

/*
 * Scroll forward by a specified number of lines, or by a full page if no
 * argument. Bound to "C-V". The "2" in the arithmetic on the window size is
 * the overlap; this value is the default overlap value in ITS EMACS. Because
 * this zaps the top line in the display window, we have to do a hard update.
 */
int
forwpage(f, n)
int	f;
register int    n;
{
	register LINE   *lp;

	if (f == FALSE) {
		n = curwp->w_ntrows - 2;	/* Default scroll.      */
		if (n <= 0)		     /* Forget the overlap   */
			n = 1;		  /* if tiny window.      */
	} else if (n < 0)
		return (backpage(f, -n));
	else				    /* Convert from pages   */
		n *= curwp->w_ntrows;	   /* to lines.	    */
	lp = curwp->w_linep;
	while (n-- && lp!=curbp->b_linep)
		lp = lforw(lp);
	curwp->w_linep = lp;
	curwp->w_dotp  = lp;
	curwp->w_doto  = 0;
	curwp->w_flag |= (WFHARD|WFMODE);   	/* MODE to chang the ratio */
	return (TRUE);
}

/*
 * This command is like "forwpage", but it goes backwards. The "2", like
 * above, is the overlap between the two windows. The value is from the ITS
 * EMACS manual. Bound to "M-V". We do a hard update for exactly the same
 * reason.
 */
int
backpage(f, n)
int		f;
register int    n;
{
	register LINE   *lp;

	if (f == FALSE) {
		n = curwp->w_ntrows - 2;	/* Default scroll.      */
		if (n <= 0)		     /* Don't blow up if the */
			n = 1;		  /* window is tiny.      */
	} else if (n < 0)
		return (forwpage(f, -n));
	else				    /* Convert from pages   */
		n *= curwp->w_ntrows;	   /* to lines.	    */
	lp = curwp->w_linep;
	while (n-- && lback(lp)!=curbp->b_linep)
		lp = lback(lp);
	curwp->w_linep = lp;
	curwp->w_dotp  = lp;
	curwp->w_doto  = 0;
	curwp->w_flag |= (WFHARD|WFMODE);	/* to change the ratio */
	return (TRUE);
}

/*
 * Set the mark in the current window to the value of "." in the window. No
 * errors are possible. Bound to "M-.".
 */
void
isetmark() 
{
	curwp->w_markp = curwp->w_dotp;
	curwp->w_marko = curwp->w_doto;
}

/*ARGSUSED*/
int
setmark(f, n)
int	f, n;
{
	isetmark();
	mlwrite("Mark Set");
	return (TRUE);
}

/*
 * Swap the values of "." and "mark" in the current window. This is pretty
 * easy, bacause all of the hard work gets done by the standard routine
 * that moves the mark about. The only possible error is "no mark". Bound to
 * "C-X C-X".
 */
/*ARGSUSED*/
int
swapmark(f, n)
int	f, n;
{
	register LINE   *odotp;
	register int    odoto;

	if (curwp->w_markp == NULL) {
		mlwrite("No mark in this window");
		return (FALSE);
	}
	odotp = curwp->w_dotp;
	odoto = curwp->w_doto;
	curwp->w_dotp  = curwp->w_markp;
	curwp->w_doto  = curwp->w_marko;
	curwp->w_markp = odotp;
	curwp->w_marko = odoto;
	curwp->w_flag |= WFMOVE;
	mlwrite("Mark Set");
	return (TRUE);
}

/*
 * Move point to a particular character in the buffer.
 */
int
gotochar(f, n)
int	f, n;
{
	register LINE	*clp;
	int		s;
	char		b[16];

	if (f == FALSE) {
		if ((s = mlreply("Goto char: ",b,sizeof(b))) != TRUE)
			return s;
		n = atoi(b);
	}
	if (n > 0) {
		clp = lforw(curbp->b_linep);
		while (n > llength(clp)) {
			n -= llength(clp) + 1;	/* + 1 for newline */
			clp = lforw(clp);
		}
	} else {
		clp = lback(curbp->b_linep);
		while (n > llength(clp)) {
			n -= llength(clp) + 1;	/* + 1 for newline */
			clp = lback(clp);
		}
		n = llength(clp) - n;
	}
	curwp->w_dotp = clp;
	curwp->w_doto = n;
	curwp->w_flag |= (WFMOVE|WFMODE);
	return TRUE;
}

/*
 * Make the window containing screen row `row' and column `col' the current
 * window and make that location point.
 */
int
setcursor(row, col)
int	row, col;
{
	register LINE	*dlp;
	WINDOW		*wp;

	/* Find right window. */
	wp = curwp;
	while (row < curwp->w_toprow || row > curwp->w_ntrows + curwp->w_toprow) {
    		nextwind(FALSE, 0);
		if (curwp == wp)
			break;	      /* full circle */
	}

	row -= curwp->w_toprow;
	dlp = curwp->w_linep;
	while (row-- && (dlp != curbp->b_linep))
		dlp = lforw(dlp);
	curwp->w_dotp = dlp;
	curgoal = col;
	curwp->w_doto = getgoal(dlp);

	curwp->w_flag |= WFMOVE;
	return (TRUE);
}
