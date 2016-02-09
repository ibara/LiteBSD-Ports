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
 *  paragraph.c -- functions that act on paragraph-sized chunks of text
 */

#include <stdio.h>
#include "ce.h"

/* 
 * Go back to the begining of the current paragraph
 * here we look for a <NL><NL> or <NL><TAB> or <NL><SPACE>
 * combination to delimit the begining of a paragraph
 */
int
gotobop(f,n)
int	f, n;	/* default Flag & Numeric argument */
{
	register int suc;	/* success of last backchar */

	if (n < 0)	/* the other way...*/
		return(gotoeop(f, -n));
	while (n-- > 0) {	/* for each one asked for */
		/* first scan back until we are in a word */
		suc = backchar(FALSE, 1);
		while (!inword() && suc)
			suc = backchar(FALSE, 1);
		curwp->w_doto = 0;	/* and go to the B-O-Line */
		/* and scan back until we hit a <NL><NL> or <NL><TAB> */
		/* or a <NL><SPACE>				      */
		while (lback(curwp->w_dotp) != curbp->b_linep)
			if (llength(curwp->w_dotp) != 0 &&
			    lgetc(curwp->w_dotp, curwp->w_doto) != TAB &&
			    lgetc(curwp->w_dotp, curwp->w_doto) != ' ')
				curwp->w_dotp = lback(curwp->w_dotp);
			else
				break;
		/* and then forward until we are in a word */
		suc = forwchar(FALSE, 1);
		while (suc && !inword())
			suc = forwchar(FALSE, 1);
	}
	curwp->w_flag |= (WFMOVE|WFMODE);	/* force screen update */
	return (TRUE);
}

/* 
 * Go forword to the end of the current paragraph
 * here we look for a <NL><NL> or <NL><TAB> or <NL><SPACE>
 * combination to delimit the begining of a paragraph	.
 */
int
gotoeop(f, n)	
int	f, n;	/* default Flag & Numeric argument */
{
	register int suc;	/* success of last backchar */

	if (n < 0)	/* the other way...*/
		return(gotobop(f, -n));
	while (n-- > 0) {	/* for each one asked for */
		/* first scan forward until we are in a word */
		suc = forwchar(FALSE, 1);
		while (!inword() && suc)
			suc = forwchar(FALSE, 1);
		curwp->w_doto = 0;	/* and go to the B-O-Line */
		if (suc)	/* of next line if not at EOF */
			curwp->w_dotp = lforw(curwp->w_dotp);
		/* and scan forword until we hit a <NL><NL> or <NL><TAB>
		   or a <NL><SPACE>					*/
		while (curwp->w_dotp != curbp->b_linep) {
			if (llength(curwp->w_dotp) != 0 &&
			    lgetc(curwp->w_dotp, curwp->w_doto) != TAB &&
			    lgetc(curwp->w_dotp, curwp->w_doto) != ' ')
				curwp->w_dotp = lforw(curwp->w_dotp);
			else
				break;
		}
		/* and then backward until we are in a word */
		suc = backchar(FALSE, 1);
		while (suc && !inword())
			suc = backchar(FALSE, 1);
		curwp->w_doto = llength(curwp->w_dotp);	/* and to the EOL */
	}
	curwp->w_flag |= (WFMOVE|WFMODE);	/* force screen update */
	return (TRUE);
}

/* 
 * Fill the current paragraph according to the current fill column.
 */
int
fillpara(f, n)
int	f, n;	/* deFault flag and Numeric argument */
{
	markpara(f,n);
	return(fillregion(f,n));
}

/* 
 * Delete n paragraphs starting with the current one 
 */
/*ARGSUSED*/
int
killpara(f, n)	
int	f, n;	/* default flag and # of paras to delete */
{
	register int status;	/* returned status of functions */

	if (curbp->b_mode & MDVIEW)
	        return(rdonly());
	while (n--) {		/* for each paragraph to delete */
		/* mark out the end and begining of the para to delete */
		gotoeop(FALSE, 1);
		/* set the mark here */
	        curwp->w_markp = curwp->w_dotp;
	        curwp->w_marko = curwp->w_doto;
		/* go to the begining of the paragraph */
		gotobop(FALSE, 1);
		curwp->w_doto = 0;	/* force us to the begining of line */
		/* and delete it */
		if ((status = killregion(FALSE, 1)) != TRUE)
			return(status);
		/* and clean up the 2 extra lines */
		(void) fdelete(2, TRUE);
	}
	return(TRUE);
}


/*
 * Mark the current paragraph.  Make the region encompass the current
 * paragraph and only that paragraph.
 *
 */
/*ARGSUSED*/
int
markpara(f,n)
int	f, n;
{
	gotoeop(FALSE, 1);
	isetmark();		/* silently set the mark at the end  */
	gotobop(FALSE, 1);	/* and "." at the beginning */
	return TRUE;
}

/*
 * Center the current paragraph on the display, according to the fill
 * column.
 */
int
centerpara(f,n)
int	f, n;
{
	markpara(f,n);
	return(centerregion(f,n));
}
