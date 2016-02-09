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
 * region.c -- routines to deal with the current region
 *
 * The routines in this file deal with the region, that magic space
 * between "." and mark. Some functions are commands. Some functions are 
 * just for internal use.
 */
#include <stdio.h>
#include "celibc.h"
#include "ce.h"

/*
 * Kill the region. Ask "getregion"  to figure out the bounds of the region.
 * Move "." to the start, and kill the characters.  Bound to "C-W".
 */
/*ARGSUSED*/
int
killregion(f, n)
int	f, n;
{
	register int    s;
	REGION	  region;

	if (curbp->b_mode&MDVIEW)	/* don't allow this command if	*/
		return(rdonly());	/* we are in read only mode	*/
	if ((s = getregion(&region)) != TRUE)
		return (s);
	if ((lastflag&CFKILL) == 0)	     /* This is a kill type  */
		kdelete();		      /* command, so do magic */
	thisflag |= CFKILL;		     /* kill buffer stuff.   */
	curwp->w_dotp = region.r_linep;
	curwp->w_doto = region.r_offset;
	mlwrite("[Killing %d characters...]", region.r_size);
	s = fdelete(region.r_size, TRUE);
	if (s == TRUE)
		mlwrite("[Killed %d characters]", region.r_size);
	return(s);
}

/*
 * Copy all of the characters in the region to the kill buffer. Don't move dot
 * at all. This is a bit like a kill region followed by a yank. Bound to "M-W".
 */
/*ARGSUSED*/
int
copyregion(f, n)
int	f, n;
{
	register LINE   *linep;
	register int    loffs;
	register int    s;
	REGION	  region;

	if ((s=getregion(&region)) != TRUE)
		return (s);
	if ((lastflag&CFKILL) == 0)	     /* Kill type command.   */
		kdelete();
	thisflag |= CFKILL;
	linep = region.r_linep;		 /* Current line.	*/
	loffs = region.r_offset;		/* Current offset.      */
	while (region.r_size--) {
		if (loffs == llength(linep)) {  /* End of line.	 */
			if ((s=kinsert('\n')) != TRUE)
				return (s);
			linep = lforw(linep);
			loffs = 0;
		} else {			/* Middle of line.      */
			if ((s=kinsert(lgetc(linep, loffs))) != TRUE)
				return (s);
			++loffs;
		}
	}
	return (TRUE);
}

/*
 * Lower case region. Zap all of the upper case characters in the region 
 * to lower case. Use the region code to set the limits. Scan the buffer,
 * doing the changes. Call "lchange" to ensure that redisplay is done in 
 * all buffers. Bound to "C-X C-L".
 */
/*ARGSUSED*/
int
lowerregion(f, n)
int	f, n;
{
	register LINE   *linep;
	register int    loffs;
	register int    c;
	register int    s;
	REGION	  region;

	if (curbp->b_mode&MDVIEW)	/* don't allow this command if	*/
		return(rdonly());	/* we are in read only mode	*/
	if ((s=getregion(&region)) != TRUE)
		return (s);
	lchange(WFHARD);
	linep = region.r_linep;
	loffs = region.r_offset;
	while (region.r_size--) {
		if (loffs == llength(linep)) {
			linep = lforw(linep);
			loffs = 0;
		} else {
			c = lgetc(linep, loffs);
			if (ISUPPER(c) != FALSE)
				lputc(linep, loffs, TOLOWER(c));
			++loffs;
		}
	}
	return (TRUE);
}

/*
 * Upper case region. Zap all of the lower case characters in the region 
 * to upper case. Use the region code to set the limits. Scan the buffer,
 * doing the changes. Call "lchange" to ensure that redisplay is done in all
 * buffers. Bound to "C-X C-U".
 */
/*ARGSUSED*/
int
upperregion(f, n)
int	f, n;
{
	register LINE   *linep;
	register int    loffs;
	register int    c;
	register int    s;
	REGION	  region;

	if (curbp->b_mode&MDVIEW)	/* don't allow this command if	*/
		return(rdonly());	/* we are in read only mode	*/
	if ((s=getregion(&region)) != TRUE)
		return (s);
	lchange(WFHARD);
	linep = region.r_linep;
	loffs = region.r_offset;
	while (region.r_size--) {
		if (loffs == llength(linep)) {
			linep = lforw(linep);
			loffs = 0;
		} else {
			c = lgetc(linep, loffs);
			if (ISLOWER(c) != FALSE)
				lputc(linep, loffs, TOUPPER(c));
			++loffs;
		}
	}
	return (TRUE);
}

/*
 * This routine figures out the bounds of the region in the current window, 
 * and fills in the fields of the "REGION" structure pointed to by "rp". 
 * Because the dot and mark are usually very close together, we scan outward 
 * from dot looking for mark. This should save time. Return a standard code.
 * Callers of this routine should be prepared to get an "ABORT" status; we 
 * might make this have the conform thing later.
 */
int
getregion(rp)
register REGION *rp;
{
	register LINE   *flp;
	register LINE   *blp;
	register int    fsize;
	register int    bsize;

	if (curwp->w_markp == NULL) {
		mlwrite("No mark set in this window");
		return (FALSE);
	}
	if (curwp->w_dotp == curwp->w_markp) {
		rp->r_linep = curwp->w_dotp;
		if (curwp->w_doto < curwp->w_marko) {
			rp->r_offset = curwp->w_doto;
			rp->r_size = curwp->w_marko-curwp->w_doto;
		} else {
			rp->r_offset = curwp->w_marko;
			rp->r_size = curwp->w_doto-curwp->w_marko;
		}
		return (TRUE);
	}
	blp = curwp->w_dotp;
	bsize = curwp->w_doto;
	flp = curwp->w_dotp;
	fsize = llength(flp)-curwp->w_doto+1;
	while (flp!=curbp->b_linep || lback(blp)!=curbp->b_linep) {
		if (flp != curbp->b_linep) {
			flp = lforw(flp);
			if (flp == curwp->w_markp) {
				rp->r_linep = curwp->w_dotp;
				rp->r_offset = curwp->w_doto;
				rp->r_size = fsize+curwp->w_marko;
				/*
				 * This is to make files that do not end in a
				 * newline work right.
				 */
				if (lforw(flp) == curbp->b_linep)
					rp->r_size++;
				return (TRUE);
			}
			fsize += llength(flp)+1;
		}
		if (lback(blp) != curbp->b_linep) {
			blp = lback(blp);
			bsize += llength(blp)+1;
			if (blp == curwp->w_markp) {
				rp->r_linep = blp;
				rp->r_offset = curwp->w_marko;
				rp->r_size = bsize - curwp->w_marko;
				return (TRUE);
			}
		}
	}
	mlwrite("Bug: lost mark");     /* AARRRRGGGHHHHHHH! */
	return (FALSE);
}


#define PREFIXLEN 40
static char pref_string[PREFIXLEN] = {'>','\0'};

/*
 *  Add a prefix to all the lines in the region.  Great for composing
 *  mail replies.
 */
/*ARGSUSED*/
int
prefixregion(f, n)
int	f, n;
{
	register int s;
	register LINE *first, *last;
	register int nline;
	REGION region;
	char *prefix;

	if ((f == TRUE) && ((s = setprefix(FALSE,1)) != TRUE))
		return (s);

	/*  get the # of lines */
	if ((s = getregion(&region)) != TRUE)
		return (s);
	first = region.r_linep;
	last = (first == curwp->w_dotp) ? curwp->w_markp : curwp->w_dotp;
	for (nline = 1; first != last; nline++)
		first = lforw(first);

	/* set "." at beginning of region */
	curwp->w_dotp = region.r_linep;
	curwp->w_doto = region.r_offset;

	/* add prefix to each line */
	while (nline--) {
		gotobol(FALSE,1);
		for (prefix = pref_string; *prefix; prefix++) 
			linsert(1,*prefix);
		forwline(FALSE,1);
	}
	gotobol(FALSE,1);
	return(TRUE);
}

/*
 *  Set the prefix for prefix-region.
 */
/*ARGSUSED*/
int
setprefix(f, n)
int	f, n;
{
	char buf[PREFIXLEN];
	char pmt[PREFIXLEN+20];
	register int s;

	if (pref_string[0] == '\0') 
		s = mlreply("Prefix String: ",buf,sizeof(buf));
	else {
		(void) sprintf(pmt,"Prefix String (%s): ",pref_string);
		s = mlreply(pmt,buf,sizeof(buf));
	}
	if (s == TRUE) 
		strcpy(pref_string,buf);
	else if (s == FALSE && (pref_string[0] != '\0')) 
		s = TRUE;
	return s;
}

/*
 * Count the number of lines in the region.  Bound to "M-="
 */
/*ARGSUSED*/
int
countregion(f, n)
int	f, n;
{
	register LINE 	*first, *last;
	register int	s, nline;
	REGION		region;

	if ((s = getregion(&region)) != TRUE) 
		return (s);
	first = region.r_linep;
	last = (first == curwp->w_dotp) ? curwp->w_markp : curwp->w_dotp;
	for (nline = 1; first != last; nline++)
		first = lforw(first);
	mlwrite("region has %d line%s", nline, (nline == 1) ? " " : "s");
	return TRUE;
}


/* 
 * Fill the current region according to the current fill column.
 */
/*ARGSUSED*/
int
fillregion(f, n)
int	f, n;
{
	register int c;			/* current char durring scan	*/
	register int wordlen;		/* length of current word	*/
	register int clength;		/* position on line during fill	*/
	register int i;			/* index during word copy	*/
	register int newlength;		/* tentative new line length	*/
	register int eorflag;		/* Are we at the End-Of-Region? */
	register int eolflag;		/* Are we at the End-of-Line?	*/
	register int firstflag;		/* first word? (needs no space)	*/
	register LINE *eorline;		/* pointer to line just past EOR*/
	register int dotflag;		/* was the last char a period?	*/
	char wbuf[NSTRING];		/* buffer for current word	*/
	REGION   region;		/* the current region	   */
	int      s;
	LINE     *first, *last;		/* boundaries of the region     */

	if (curbp->b_mode&MDVIEW)	/* don't allow this command if	*/
		return(rdonly());	/* we are in read only mode	*/
	if (fillcol == 0) {	/* no fill column set */
		mlwrite("No fill column set");
		return(FALSE);
	}
	/* 
	 * Get the current region
	 */
	if ((s = getregion(&region)) != TRUE)
		return(s);
	first = region.r_linep;
	if (first == curwp->w_dotp) 
		last =  curwp->w_markp;
	else
		last =  curwp->w_dotp;
	eorline = lforw(last);
	/* Go back to the begining of the region */
	curwp->w_dotp = first;
	curwp->w_doto = 0;
	/* initialize various info */
	clength = curwp->w_doto;
	if (clength && curwp->w_dotp->l_text[0] == TAB)
		clength = 8;
	wordlen = 0;
	dotflag = FALSE;
	/* scan through lines, filling words */
	firstflag = TRUE;
	eorflag = FALSE;
	while (!eorflag) {
		/* get the next character in the region */
		if (eolflag = (curwp->w_doto == llength(curwp->w_dotp))) {
			c = ' ';
			if (lforw(curwp->w_dotp) == eorline)
				eorflag = TRUE;
		} else
			c = lgetc(curwp->w_dotp, curwp->w_doto);
		/* and then delete it */
		if (fdelete(1, FALSE) == FALSE && !eorflag)
			return FALSE;
		/* if not a separator, just add it in */
		if (c != ' ' && c != '\t') {
#if 0
			dotflag = (c == '.');		/* was it a dot */
#else
			dotflag = ISEOSP(c);
#endif
			if (wordlen < NSTRING - 1)
				wbuf[wordlen++] = c;
			else
				mlwrite("Word too long for fill buffer");
		} else if (wordlen) {
			/* calculate tentitive new length with word added */
			newlength = clength + 1 + wordlen;
#if 0
			/*
			 * If we're at the end of a line or at a doublespace,
			 * and the previous character was one that ends a
			 * sentence ('.', '?', '!', then we doublespace here.
			 */
			if ((eolflag
			    || (curwp->w_doto == llength(curwp->w_dotp))
			    || ((c = lgetc(curwp->w_dotp, curwp->w_doto)) == ' ')
			    || (c == '\t'))
			    && ISEOSP(wbuf[wordlen - 1])
			    && (wordlen < NSTRING - 1))
				wbuf[wordlen++] = ' ';
#endif
			/* at a word break with a word waiting */
			if (newlength <= fillcol) {
				/* add word to current line */
				if (!firstflag) {
					linsert(1, ' '); /* the space */
					++clength;
				}
				firstflag = FALSE;
			} else {
				/* start a new line */
				lnewline();
				clength = 0;
			}
			/* and add the word in in either case */
			for (i = 0; i < wordlen; i++) {
				linsert(1, wbuf[i]);
				++clength;
			}
			if (dotflag) {
				linsert(1, ' ');
				++clength;
			}
			wordlen = 0;
		}
	}
	/* and add a last newline for the end of our new region */
	lnewline();
	/* and move back up to the end of the last line */
	backchar(FALSE, 1);
	/* remove any trailing spaces after a period, for instance at the
	   end of a paragraph. */
	if (dotflag) {
		s = lgetc (curwp->w_dotp, curwp->w_doto);
		if ((s == 0 || s == ' ') &&
			lgetc (curwp->w_dotp, curwp->w_doto - 1) == ' ' &&
			ISEOSP(lgetc (curwp->w_dotp, curwp->w_doto - 2)))
			backchar(FALSE, 1);
	}
	curwp->w_flag |= (WFMOVE|WFMODE);
	return (TRUE);
}

/*
 * Dump the current region to the file named by the argument fn.  Used by
 * the "spawn" commands.
 */
/*ARGSUSED*/
int
dumpregion(fn)
char	*fn;
{
	register int	s, lasto;
	register LINE	*lp, *last;
	REGION		region;

	if ((s = getregion(&region)) != TRUE)
		return (s);
	lp = region.r_linep;
	last = (lp == curwp->w_dotp) ? curwp->w_markp : curwp->w_dotp;
	lasto = (lp == curwp->w_dotp) ? curwp->w_marko : curwp->w_doto;
	if ((s = ffwopen(fn,FALSE)) != FIOSUC)
		return(FALSE);
	while (lp != last) {
		if ((s = ffputline(ltext(lp), llength(lp))) != FIOSUC)
			break;
		lp = lforw(lp);
	}
	if (s == FIOSUC) {
		if (lasto == llength(lp))
			s = ffputline(ltext(lp), lasto);
		else
			s = ffputbuf(ltext(lp), lasto);	/* no newline */
	}
	if (s == FIOSUC)
		s = ffclose();
	else 
		(void) ffclose();
	if (s != FIOSUC) 
		return (FALSE);
	return(TRUE);
}

/*
 * Center the lines in the current region.
 */
/*ARGSUSED*/
int
centerregion(f, n)
int	f, n;
{
	register LINE *first, *last;
	register int s, nlines;
	REGION	region;

	if (curbp->b_mode & MDVIEW)
		return(rdonly());
	if ((s = getregion(&region)) != TRUE)
		return s;
	first = region.r_linep;
	last = (first == curwp->w_dotp) ? curwp->w_markp : curwp->w_dotp;
	last = lforw(last);
	nlines = 0;
	curwp->w_dotp = first;
	while (curwp->w_dotp != last) {
		if ((s = center1()) != TRUE)
			return s;
		nlines++;
		curwp->w_dotp = lforw(curwp->w_dotp);
	}
	while (nlines--) 
		curwp->w_dotp = lback(curwp->w_dotp);
	curwp->w_doto = 0;
	curwp->w_flag |= WFHARD;
	return TRUE;
}
