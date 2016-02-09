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
 * buffer.c -- buffer management functions
 *
 * Buffer management.
 * Some of the functions are internal, and some are actually attached to user
 * keys. Like everyone else, they set hints for the display system.
 */

#include <stdio.h>
#include "celibc.h"
#include "ce.h"

extern struct WINDOW *wpopup();

/*
 * Forward declarations
 */
extern void itoa();

/*
 * Attach a buffer to a window. The values of dot and mark come from the buffer
 * if the use count is 0. Otherwise, they come from some other window.
 */
/*ARGSUSED*/
int
usebuffer(f, n)
int	f, n;
{
	register BUFFER *bp;
	register int    s;
	char	    bufn[NBUFN];

	if ((s = mlgetbuf("Switch to buffer: ", bufn, NBUFN)) != TRUE)
		return (s);
	if ((bp=bfind(bufn, TRUE, 0)) == NULL)
		return (FALSE);
	return(switchbuffer(bp));
}

/* 
 * Find the next buffer in the buffer list 
 */
BUFFER *
getnextbuffer()
{
	register BUFFER *bp;

	bp = curbp->b_bufp;
	/* cycle through the buffers to find an eligible one */
	while ((bp == NULL) || (bp->b_flag & BFTEMP)) {
		if (bp == NULL)
			bp = bheadp;
		else
			bp = bp->b_bufp;
	}
	return (bp);
}

/*
 * Switch to the next buffer in the buffer list
 */
/*ARGSUSED*/
int
nextbuffer(f,n)
int	f, n;
{
	register BUFFER *bp;

	bp = getnextbuffer();
	return(switchbuffer(bp));
}

/* 
 * Make buffer BP current.  We could put the groups of assignments
 * into macros.
 */
int
switchbuffer(bp)
BUFFER	*bp;
{
	register WINDOW *wp;

	/*
	 * If we're being asked to do essentially nothing, return quickly.
	 */
	if (bp == curbp)
		return TRUE;
	if (--curbp->b_nwnd == 0) {		/* Last use.	*/
		curbp->b_dotp  = curwp->w_dotp;
		curbp->b_doto  = curwp->w_doto;
		curbp->b_markp = curwp->w_markp;
		curbp->b_marko = curwp->w_marko;
	}
	curbp = bp;				/* Switch.	*/
	if (curbp->b_active != TRUE) {		/* buffer not active yet*/
		/* read it in and activate it */
		if (readin(curbp->b_fname) != TRUE)
			return FALSE;
		curbp->b_dotp = lforw(curbp->b_linep);
		curbp->b_doto = 0;
		curbp->b_active = TRUE;
	}
	curwp->w_bufp  = bp;
	curwp->w_linep = bp->b_linep;		/* For macros, ignored. */
	curwp->w_flag |= WFMODE|WFFORCE|WFHARD;	/* Quite nasty.	 */
	if (bp->b_nwnd++ == 0) {	/* First use.	   */
		curwp->w_dotp  = bp->b_dotp;
		curwp->w_doto  = bp->b_doto;
		curwp->w_markp = bp->b_markp;
		curwp->w_marko = bp->b_marko;
		return (TRUE);
	}
	wp = wheadp;				/* Look for old.	*/
	while (wp != NULL) {
		if (wp != curwp && wp->w_bufp == bp) {
			curwp->w_dotp  = wp->w_dotp;
			curwp->w_doto  = wp->w_doto;
			curwp->w_markp = wp->w_markp;
			curwp->w_marko = wp->w_marko;
			break;
		}
		wp = wp->w_wndp;
	}
	return (TRUE);
}

/*
 * Dispose of a buffer, by name.  Ask for the name. Look it up (don't get too
 * upset if it isn't there at all!). Get quite upset  if the buffer is being 
 * displayed.  Clear the buffer (ask if the buffer has been changed). Then 
 * free the header line and the buffer header. Bound to "C-X K".
 */
/*ARGSUSED*/
int
killbuffer(f, n)
int	f, n;
{
	register BUFFER *bp, *bp1;
	register int    s;
	char 		bufn[NBUFN];

	if ((s = mlgetbuf("Kill buffer: ", bufn, NBUFN)) == ABORT)
		return(s);
	if (s == FALSE)
		bp = curbp;
	else if ((bp = bfind(bufn, FALSE, 0)) == NULL)
		return (TRUE);
	if (bp == curbp) {
		/*
		 * Find another buffer to display.  Default is the next
		 * different buffer in the buffer list.  If there is
		 * only one buffer, then try to switch to the scratch
		 * buffer, creating it if necessary.
		 */
		bp1 = getnextbuffer();
		/*
		 * No other buffers; try to use *scratch*.
		 */
		if (bp1 == bp) {
			if (bp == bfind("*scratch*", FALSE))
				return(FALSE);
			if ((bp1 = bfind("*scratch*", TRUE)) == NULL)
				return(FALSE);
		}
	} else
		bp1 = curbp;

	if (zotbuf(bp, bp1, TRUE) != TRUE)
		return FALSE;

	/*
	 * zotbuf()  manipulates the window list, so all we really
	 * need to do is to make sure that bp1 is the current buffer
	 * and that it's active.  update() takes care of the rest.
	 */
	curbp = bp1;
	if (curbp->b_active != TRUE) {
		/* read it in and activate it */
		if (readin(curbp->b_fname) != TRUE)
			return FALSE;
		curbp->b_dotp = lforw(curbp->b_linep);
		curbp->b_doto = 0;
		curbp->b_active = TRUE;
	}
	return (TRUE);
}

/* 
 * Kill the buffer pointed to by bp.  If force is TRUE, kill the buffer even
 * if it is being displayed.  nbp is the new buffer to put into the windows
 * that are displaying bp.
 */
int
zotbuf(bp, nbp, force)
register BUFFER *bp;
register BUFFER *nbp;
int	force;
{
	register BUFFER *bp1;
	register BUFFER *bp2;
	register WINDOW	*wp;
	register int    s;

	if (!force && bp->b_nwnd != 0) {	/* Error if on screen.  */
		mlwrite("Buffer is being displayed");
		return (FALSE);
	}
	if ((s = bclear(bp)) != TRUE)		/* Blow text away.      */
		return (s);
	/*
	 * Make the windows showing the buffer to be killed display
	 * NBP instead.
	 */
	for (wp = wheadp; bp->b_nwnd > 0 && wp; wp = wp->w_wndp) {
		if (wp->w_bufp == bp) {
			wp->w_bufp = (BUFFER *) NULL;
			buftowin(nbp, wp, WFMODE|WFFORCE|WFHARD);
		}
	}
	(void) free((char *) bp->b_linep);	/* Release header line. */
	bp1 = NULL;				/* Find the header.     */
	bp2 = bheadp;
	while (bp2 != bp) {
		bp1 = bp2;
		bp2 = bp2->b_bufp;
	}
	bp2 = bp2->b_bufp;			/* Next one in chain.   */
	if (bp1 == NULL)			/* Unlink it.	   */
		bheadp = bp2;
	else
		bp1->b_bufp = bp2;
	free((char *) bp);			/* Release buffer block */
	return (TRUE);
}

/*
 * Rename the current buffer
 */
/*ARGSUSED*/
int
namebuffer(f,n)
int f, n;
{
	register BUFFER *bp;	/* pointer to scan through all buffers */
	char bufn[NBUFN];	/* buffer to hold buffer name */

	/* prompt for and get the new buffer name */
ask:
	if (mlreply("Change buffer name to: ", bufn, NBUFN) != TRUE)
		return(FALSE);
	/* and check for duplicates */
	bp = bheadp;
	while (bp != NULL) {
		if (bp != curbp) {
			/* if the names the same */
			if (strcmp(bufn, bp->b_bname) == 0) {
				(*term.t_beep)();
				mlwrite("Already a buffer by that name");
				goto ask;  /* try again */
			}
		}
		bp = bp->b_bufp;	/* onward */
	}
	strcpy(curbp->b_bname, bufn);	/* copy buffer name to structure */
	curwp->w_flag |= WFMODE;	/* make mode line replot */
	mlerase();
	return TRUE;
}

/*
 * Insert another buffer at ".".  Bound to "C-X C-I".
 */
/*ARGSUSED*/
int
insbuffer(f,n)
int	f, n;
{
	int s;
	char bufn[NBUFN];

	if (curbp->b_flag & MDVIEW)
		return(rdonly());
	s = mlgetbuf("Insert Buffer: ", bufn, NBUFN);
	if (s != TRUE) 
		return s;
	return(insertbuffer(bufn));
}

int
insertbuffer(bufn)
char bufn[];
{
	register BUFFER *bp;
	register LINE *lp;
	register int lo, nline;

	if ((bp = bfind(bufn, FALSE, 0)) == FALSE) {
		(*term.t_beep)();
		mlwrite("No buffer by that name");
		return (FALSE);
	}
	if (bp == curbp) {
		(*term.t_beep)();
		mlwrite("Cannot insert a buffer into itself");
		return (FALSE);
	}
	nline = 0;
	lp = lforw(bp->b_linep);
	for (;;) {
		for (lo = 0; lo < llength(lp); lo++) 
			if (linsert(1, lgetc(lp, lo)) == FALSE)
				return (FALSE);
		if ((lp = lforw(lp)) == bp->b_linep)
			break;
		if (lnewline() == FALSE)
			return(FALSE);
		nline++;
	}
	if (nline == 1)
		mlwrite("(Inserted 1 line)");
	else
		mlwrite("(Inserted %d lines)",nline);
	lp = curwp->w_linep;
	if (curwp->w_dotp == lp) {
		while (nline-- && lback(lp) != curbp->b_linep)
			lp = lback(lp);
		curwp->w_linep = lp;
		curwp->w_flag |= WFHARD;
	}
	return (TRUE);
}

/*
 * List all of the active buffers. First update the special buffer that holds 
 * the list. Next make sure at least 1 window is displaying the buffer list,
 * splitting the screen if this is what it takes. Lastly, repaint all of the
 * windows that are displaying the list. Bound to "C-X C-B".
 */
/*ARGSUSED*/
int
listbuffers(f, n)
int	f, n;
{
	register WINDOW *wp;
	register BUFFER *bp;
	register int    s;

	if ((s = makelist()) != TRUE)
		return (s);
	if (blistp->b_nwnd == 0) {	      /* Not on screen yet.   */
		if ((wp=wpopup()) == NULL)
			return (FALSE);
		bp = wp->w_bufp;
		if (--bp->b_nwnd == 0) {
			bp->b_dotp  = wp->w_dotp;
			bp->b_doto  = wp->w_doto;
			bp->b_markp = wp->w_markp;
			bp->b_marko = wp->w_marko;
		}
		wp->w_bufp  = blistp;
		++blistp->b_nwnd;
	}
	wp = wheadp;
	while (wp != NULL) {
		if (wp->w_bufp == blistp) {
			wp->w_linep = lforw(blistp->b_linep);
			wp->w_dotp  = lforw(blistp->b_linep);
			wp->w_doto  = 0;
			wp->w_markp = NULL;
			wp->w_marko = 0;
			wp->w_flag |= WFMODE|WFHARD;
		}
		wp = wp->w_wndp;
	}
	return (TRUE);
}

/*
 * Make the region encompass the entire current buffer.
 */
int
markbuffer(f, n)
int	f,n;
{
	igotobob();
	setmark();
	igotoeob();
	return (TRUE);
}

/*
 * This routine rebuilds the text in the special secret buffer that holds the
 * buffer list. It is called by the list buffers command. Return TRUE if
 * everything works. Return FALSE if there is an error (if there is no memory).
 */
int
makelist()
{
	register char   *cp1;
	register char   *cp2;
	register int    c;
	register BUFFER *bp;
	register LINE   *lp;
	register int    nbytes;
	register int    s;
	register int	i;
	char	    b[6+1];
	char	    line[128];

	blistp->b_flag &= ~BFCHG;		/* Don't complain!      */
	if ((s = bclear(blistp)) != TRUE)	/* Blow old text away   */
		return (s);
	blistp->b_fname[0] = '\0';
	if (addline(blistp, "AC MODES    Size Buffer             File", 40) == FALSE
	||  addline(blistp, "-- -----    ---- ------             ----", 40) == FALSE)
		return (FALSE);
	bp = bheadp;				/* For all buffers	*/
	/* output the list of buffers */
	while (bp != NULL) {
#if 0
		if ((bp->b_flag&BFTEMP) != 0) {	/* Skip magic ones.     */
			bp = bp->b_bufp;
			continue;
		}
#endif
		cp1 = &line[0];			/* Start at left edge   */
		/*
		 * output status of ACTIVE flag (has the file been read in?)
		 */
		if (bp->b_active == TRUE)	/* "@" if activated	*/
			*cp1++ = '@';
		else
			*cp1++ = ' ';
		/* output status of changed flag */
		if ((bp->b_flag&BFCHG) != 0)    /* "*" if changed       */
			*cp1++ = '*';
		else
			*cp1++ = ' ';
		*cp1++ = ' ';

		/* output the mode codes */
		for (i = 0; i < nummodes; i++) {
			if (bp->b_mode & (1 << i))
				*cp1++ = modecode[i];
			else
				*cp1++ = '.';
		}
		*cp1++ = ' ';
		*cp1++ = ' ';

		nbytes = 0;			/* Count bytes in buf.  */
		lp = lforw(bp->b_linep);
		while (lp != bp->b_linep) {
			nbytes += llength(lp) + 1;
			lp = lforw(lp);
		}
		itoa(b, 6, nbytes);		/* 6 digit buffer size. */
		cp2 = &b[0];
		while ((c = *cp2++) != 0)
			*cp1++ = c;
		*cp1++ = ' ';
		*cp1++ = ' ';
		cp2 = &bp->b_bname[0];		/* Buffer name	*/
		while ((c = *cp2++) != 0)
			*cp1++ = c;
		cp2 = &bp->b_fname[0];		/* File name	*/
		if (*cp2 != 0) {
			*cp1++ = ' ';	/* ensure at least one separator */
			while (cp1 < &line[2+1+5+1+6+1+20]) /* XXX - was NBUFN instead of 20 */
				*cp1++ = ' ';
			while ((c = *cp2++) != 0) {
				if (cp1 < &line[128-1])
					*cp1++ = c;
			}
		}
		*cp1 = 0;			/* Add to the buffer.   */
		if (addline(blistp, line, strlen(line)) == FALSE)
			return (FALSE);
		bp = bp->b_bufp;
	}
	/* Add a blank line after the buffer list */
	if (addline(blistp, " ", 1) == FALSE)
		return(FALSE);
	/* build line to report global mode settings */
	cp1 = &line[0];
	*cp1++ = ' ';
	*cp1++ = ' ';
	*cp1++ = ' ';
	/* output the mode codes */
	for (i = 0; i < nummodes; i++)
		if (gmode & (1 << i))
			*cp1++ = modecode[i];
		else
			*cp1++ = '.';
	strcpy(cp1, "	Global Modes");
	if (addline(blistp, line, strlen(line)) == FALSE)
		return(FALSE);
	return (TRUE);			  /* All done	     */
}

void
itoa(buf, width, num)
register char   buf[];
register int    width;
register int    num;
{
	buf[width] = 0;			/* End of string.       */
	while (num >= 10) {		/* Conditional digits.  */
		buf[--width] = (num % 10) + '0';
		num /= 10;
	}
	buf[--width] = num + '0';	/* Always 1 digit.      */
	while (width != 0)		/* Pad with blanks.     */
		buf[--width] = ' ';
}

/*
 * The argument "text" points to a string. Append this line to the buffer bp.
 * Handcraft the EOL on the end. Return TRUE if it worked and FALSE if you ran
 * out of room.
 */
int
addline(bp,text, ntext)
register BUFFER *bp;
char    	*text;
int		ntext;
{
	register LINE   *lp;

	if (ntext == 0)
		return (FALSE);
	if ((lp = lalloc(ntext)) == NULL)
		return (FALSE);
	bcopy(text, ltext(lp), ntext);
	bp->b_linep->l_bp->l_fp = lp;		/* Hook onto the end    */
	lp->l_bp = bp->b_linep->l_bp;
	bp->b_linep->l_bp = lp;
	lp->l_fp = bp->b_linep;
	if (bp->b_dotp == bp->b_linep)		/* If "." is at the end */
		bp->b_dotp = lp;		/* move it to new line  */
	return (TRUE);
}

/*
 * Look through the list of buffers. Return TRUE if there are any changed 
 * buffers. Buffers that hold magic internal stuff are not considered; who 
 * cares if the list of buffer names is hacked. Return FALSE if no buffers
 * have been changed.
 */
int
anycb()
{
	register BUFFER *bp;

	bp = bheadp;
	while (bp != NULL) {
		if ((bp->b_flag&BFTEMP)==0 && (bp->b_flag&BFCHG)!=0)
			return (TRUE);
		bp = bp->b_bufp;
	}
	return (FALSE);
}

/*
 * Find a buffer, by name. Return a pointer to the BUFFER structure associated
 * with it. If the named buffer is found, but is a TEMP buffer (like the buffer
 * list) complain. If the buffer is not found and the "cflag" is TRUE, create 
 * it. The "bflag" is the settings for the flags in in buffer.
 */
BUFFER  *
bfind(bname, cflag, bflag)
register char   *bname;
int	cflag, bflag;
{
	register BUFFER *bp;
	register BUFFER *sb;	/* buffer to insert after */
	register LINE   *lp;

	bp = bheadp;
	while (bp != NULL) {
		if (strcmp(bname, bp->b_bname) == 0) {
			if ((bp->b_flag&BFTEMP) != 0) {
				mlwrite("Cannot select builtin buffer");
				return ((BUFFER *) NULL);
			}
			return (bp);
		}
		bp = bp->b_bufp;
	}
	if (cflag != TRUE)
		return((BUFFER *) NULL);
	if ((bp=(BUFFER *)malloc(sizeof(BUFFER))) == NULL) {
		mlwrite("Cannot allocate space for buffer");
		return ((BUFFER *) NULL);
	}
	if ((lp=lalloc(0)) == NULL) {
		free((char *) bp);
		return ((BUFFER *) NULL);
	}
	/* find the place in the list to insert this buffer */
	/* since we keep the buffers alphabetically sorted  */
	if (bheadp == NULL || strcmp(bheadp->b_bname, bname) > 0) {
		/* insert at the begining */
		bp->b_bufp = bheadp;
		bheadp = bp;
	} else {
	    	sb = bheadp;
		while (sb->b_bufp != NULL) {
			if (strcmp(sb->b_bufp->b_bname, bname) > 0)
				break;
			sb = sb->b_bufp;
		}
		/* and insert it */
		bp->b_bufp = sb->b_bufp;
		sb->b_bufp = bp;
	}
	/* and set up the other buffer fields */
	bp->b_active = TRUE;
	bp->b_dotp  = lp;
	bp->b_doto  = 0;
	bp->b_markp = NULL;
	bp->b_marko = 0;
	bp->b_flag  = bflag;
	bp->b_mode  = gmode;
	bp->b_nwnd  = 0;
	bp->b_linep = lp;
	bp->b_fname[0] = '\0';
	strcpy(bp->b_bname, bname);
	lp->l_fp = lp;
	lp->l_bp = lp;
	return (bp);
}

/*
 * This routine blows away all of the text in a buffer. If the buffer is 
 * marked as changed then we ask if it is ok to blow it away; this is to
 * save the user the grief of losing text. The window chain is nearly always 
 * wrong if this gets called; the caller must arrange for the updates that
 * are required. Return TRUE if everything looks good.
 */
int
bclear(bp)
register BUFFER *bp;
{
	register LINE   *lp;
	register int    s;

	if ((bp->b_flag & BFTEMP) == 0 && (bp->b_flag & BFCHG) != 0 &&
	    (s = mlyesno("Buffer is changed, discard anyway")) != TRUE)
		return (s);
	bp->b_flag  &= ~BFCHG;		  /* Not changed	  */
	while ((lp = lforw(bp->b_linep)) != bp->b_linep)
		lfree(lp);
	bp->b_dotp  = bp->b_linep;	      /* Fix "."	      */
	bp->b_doto  = 0;
	bp->b_markp = NULL;		     /* Invalidate "mark"    */
	bp->b_marko = 0;
	return (TRUE);
}

/*
 *  Turn off the modified flag on this buffer.
 */
/*ARGSUSED*/
int
notmodified(f,n)
int	f, n;
{
	register WINDOW *wp;

	curbp->b_flag &= ~BFCHG;
	wp = wheadp;
	while (wp != NULL) {
		if (wp->w_bufp == curbp) 
			wp->w_flag |= WFMODE;
		wp = wp->w_wndp;
	}
	mlwrite("Modified flag cleared");
	return(TRUE);
}

/*
 *  Pop a buffer to the screen, splitting the window if need be.
 */
int
popbuftoscreen(bp)
register BUFFER *bp;
{
	register WINDOW *wp;
	WINDOW	*popupbuffer();

	bp->b_dotp = lforw(bp->b_linep);  /* go to start of buffer */
	bp->b_doto = 0;
	if (bp->b_nwnd != 0) {
		wp = wheadp;
		while (wp != NULL) {
			if (wp->w_bufp == bp) {
				wp->w_dotp = bp->b_dotp;
				wp->w_doto = 0;
				wp->w_flag |= WFHARD;
			}
			wp = wp->w_wndp;
		}
	}
	return(popupbuffer(bp) != NULL);
}

WINDOW *
popupbuffer(bp)
register BUFFER *bp;
{
	register WINDOW *wp;
    
	if (bp->b_nwnd == 0) {
		if ((wp = wpopup()) == NULL)
			return NULL;
	} else {
		wp = wheadp;
		while (wp != NULL) {
	    		if (wp->w_bufp == bp) {
				wp->w_flag |= WFHARD|WFFORCE;
				return wp;
			}
			wp = wp->w_wndp;
		}
	}
	if (buftowin(bp, wp, WFHARD) != TRUE)
		return(NULL);
	return(wp);
}

/*
 * Make buffer BP appear in window WP.
 */
int
buftowin(bp, wp, flags)
register BUFFER *bp;
register WINDOW *wp;
int 		flags;
{
	register BUFFER *obp;
	register WINDOW *owp;

	if (wp->w_bufp == bp) {
		wp->w_flag |= flags;
		return(TRUE);
	}
	/* disconnect old buffer from window */
	if ((obp = wp->w_bufp) != NULL) {
		if (--obp->b_nwnd == 0) {	/* last use */
			obp->b_dotp = wp->w_dotp;
			obp->b_doto = wp->w_doto;
    			obp->b_markp = wp->w_markp;
			obp->b_marko = wp->w_marko;
		}
	}
	/* now attach new buffer to the window */
	wp->w_bufp = bp;
	if (bp->b_nwnd++ == 0) {	     /* first use */
		wp->w_dotp = bp->b_dotp;	
		wp->w_doto = bp->b_doto;	
		wp->w_markp = bp->b_markp;	
		wp->w_marko = bp->b_marko; 
	} else {
		for (owp = wheadp; owp != NULL; owp = wp->w_wndp) 
			if (wp->w_bufp == bp && owp != wp) {
				wp->w_dotp = owp->w_dotp;	
				wp->w_doto = owp->w_doto;	
				wp->w_markp = owp->w_markp;	
				wp->w_marko = owp->w_marko; 
			}
	}
	wp->w_flag |= WFMODE|flags;
	return(TRUE);
}

/*
 * The current buffer's name may have been changed, and since we keep
 * the buffer list sorted alphabetically by buffer name, we need to 
 * take the current buffer out of its present position and reinsert it.
 */
int
bshuffle()
{
	register BUFFER *bp;
	register char	*bname;
	register BUFFER *sb;

	/*
	 * remove the current buffer from the buffer list.
	 */
	bp = bheadp;
	bname = curbp->b_bname;
	if (bp == curbp) 		/* it's first */
		bheadp = bp->b_bufp;
	else {
		while (bp != NULL) {
			if (bp->b_bufp == curbp) {		/* bp->next == curbp */
				bp->b_bufp = curbp->b_bufp;	/* take it out */
				break;
			} else 
				bp = bp->b_bufp;
		}
	}
	/* 
	 * Now reinsert it.
	 */
	if (bheadp == NULL || strcmp(bheadp->b_bname, bname) > 0) {
		/* insert it at the beginning */
		curbp->b_bufp = bheadp;
		bheadp = curbp;
	} else {	
		sb = bheadp;		/* we know it's not first */
		while (sb->b_bufp != NULL) {      /* find right spot in list */
			if (strcmp(sb->b_bufp->b_bname, bname) > 0)
				break;
			sb = sb->b_bufp;
		}
		curbp->b_bufp = sb->b_bufp;       /* and insert it there */
		sb->b_bufp = curbp;
	}
	return(TRUE);
}
