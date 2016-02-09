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
 * line.c -- line management routines for ce
 *
 * Part of the original v30 uemacs
 *
 * The functions in this file are a general set of line management utilities. 
 * They are the only routines that touch the text. They also touch the buffer
 * and window structures, to make sure that the necessary updating gets done.
 * There are routines in this file that handle the kill buffer too.  It isn't 
 * here for any good reason.
 *
 * Note that this code only updates the dot and mark values in the window 
 * list. Since all the code acts on the current window, the buffer that we
 * are editing must be being displayed, which means that "b_nwnd" is non zero,
 * which means that the dot and mark values in the buffer headers are
 * nonsense.
 */
#include "celibc.h"
#include "ce.h"

#define	NBLOCK	16			/* Line block chunk size	*/

#ifndef	KBLOCK
#define	KBLOCK	8192			/* Kill buffer block size.	*/
#endif

#ifndef NULL
#define NULL 0
#endif

static  char	*kbufp	= NULL;		/* Kill buffer data.		*/
int	kused	= 0;			/* # of bytes used in KB.	*/
int	ksize	= 0;			/* # of bytes allocated in KB.	*/

/*
 * This routine allocates a block of memory large enough to hold a LINE
 * containing "used" characters. The block is always rounded up a bit. 
 * Return a pointer to the new block, or NULL if there isn't any memory 
 * left. Print a message in the message line if no space.
 */
LINE *
lalloc(used)
register int	used;
{
	register LINE	*lp;
	register int	size;

	size = (used+NBLOCK-1) & ~(NBLOCK-1);
	if (size == 0)				/* Assume that an empty	*/
		size = NBLOCK;			/* line is for type-in.	*/
	if ((lp=(LINE *)malloc((unsigned) (1+sizeof(LINE)+size))) == NULL) {
		mlwrite("Cannot allocate %d bytes", size);
		return (NULL);
	}
	lp->l_size = size;
	lp->l_used = used;
	return (lp);
}

/*
 *  Allocate a line that is expected to grow in the future.
 *  (0 is no longer a special case).
 */
LINE *
lallocx(used)
register int used;
{
	register int	size;
	register LINE	*lp;

	size = (NBLOCK + used) & ~(NBLOCK-1);
	if ((lp = lalloc(size)) != NULL) 
		lp->l_used = used;
	return lp;
}

/*
 * Delete line "lp". Fix all of the links that might point at it (they are
 * moved to offset 0 of the next line).  Unlink the line from whatever buffer
 * it might be in. Release the memory. The buffers are updated too; the magic
 * conditions described in the above comments don't hold here.
 */
void
lfree(lp)
register LINE	*lp;
{
	register BUFFER	*bp;
	register WINDOW	*wp;

	wp = wheadp;
	while (wp != NULL) {
		if (wp->w_linep == lp)
			wp->w_linep = lp->l_fp;
		if (wp->w_dotp  == lp) {
			wp->w_dotp  = lp->l_fp;
			wp->w_doto  = 0;
		}
		if (wp->w_markp == lp) {
			wp->w_markp = lp->l_fp;
			wp->w_marko = 0;
		}
		wp = wp->w_wndp;
	}
	bp = bheadp;
	while (bp != NULL) {
		if (bp->b_nwnd == 0) {
			if (bp->b_dotp  == lp) {
				bp->b_dotp = lp->l_fp;
				bp->b_doto = 0;
			}
			if (bp->b_markp == lp) {
				bp->b_markp = lp->l_fp;
				bp->b_marko = 0;
			}
		}
		bp = bp->b_bufp;
	}
	lp->l_bp->l_fp = lp->l_fp;
	lp->l_fp->l_bp = lp->l_bp;
	free((char *) lp);
}

/*
 * This routine gets called when a character is changed in place in the
 * current buffer. It updates all of the required flags in the buffer and
 * window system. The flag used is passed as an argument; if the buffer is
 * being displayed in more than 1 window we change EDIT to HARD. Set MODE 
 * if the mode line needs to be updated (the "*" has to be set).
 */
void
lchange(flag)
register int	flag;
{
	register WINDOW	*wp;

	if (curbp->b_nwnd != 1)			/* Ensure hard.		*/
		flag = WFHARD;
	if ((curbp->b_flag&BFCHG) == 0) {	/* First change, so 	*/
		flag |= WFMODE;			/* update mode lines.	*/
		curbp->b_flag |= BFCHG;
	}
	wp = wheadp;
	while (wp != NULL) {
		if (wp->w_bufp == curbp)
			wp->w_flag |= flag;
		wp = wp->w_wndp;
	}
}

/*
 * Insert spaces forward into the text.
 */
int
insspace(f, n) 
int	f, n;
{
	int s;

	s = linsert(n,' ');
	if (s != TRUE)
		return(s);
	return(backchar(f,n));
}

/*
 * Insert "n" copies of the character "c" at the current location of dot.  
 * In the easy case all that happens is the text is stored in the line.
 * In the hard case, the line has to be reallocated.  When the window list
 * is updated, take special care; I screwed it up once. You always update dot
 * in the current window. You update mark, and a dot in another window, if it
 * is greater than the place where you did the insert. Return TRUE if all is
 * well, and FALSE on errors.
 */
int
linsert(n, c)
int	n, c;
{
	register char	*cp1;
	register char	*cp2;
	register LINE	*lp1;
	register LINE	*lp2;
	register LINE	*lp3;
	register int	doto;
	register int	i;
	register WINDOW	*wp;

	if (curbp->b_mode & MDVIEW)
		return(rdonly());
	lchange(WFEDIT);
	lp1 = curwp->w_dotp;			/* Current line		*/
	if (lp1 == curbp->b_linep) {		/* At the end: special	*/
		if (curwp->w_doto != 0) {
			mlwrite("bug: linsert");
			return (FALSE);
		}
		if ((lp2=lalloc(n)) == NULL)	/* Allocate new line	*/
			return (FALSE);
		lp3 = lp1->l_bp;		/* Previous line	*/
		lp3->l_fp = lp2;		/* Link in		*/
		lp2->l_fp = lp1;
		lp1->l_bp = lp2;
		lp2->l_bp = lp3;
		for (i=0; i<n; ++i)
			lp2->l_text[i] = c;
		for (wp = wheadp; wp != NULL; wp = wp->w_wndp) {
			if (wp->w_linep == lp1) wp->w_linep = lp2;
			if (wp->w_dotp  == lp1) wp->w_dotp  = lp2;
			if (wp->w_markp == lp1) wp->w_markp = lp2;
		}
		curwp->w_doto = n;
		return (TRUE);
	}
	doto = curwp->w_doto;			/* Save for later.	*/
	if (lp1->l_used+n > lp1->l_size) {	/* Hard: reallocate	*/
		if ((lp2=lalloc(lp1->l_used+n)) == NULL)
			return (FALSE);
		cp1 = &lp1->l_text[0];
		cp2 = &lp2->l_text[0];
		while (cp1 != &lp1->l_text[doto])
			*cp2++ = *cp1++;
		cp2 += n;
		while (cp1 != &lp1->l_text[lp1->l_used])
			*cp2++ = *cp1++;
		lp1->l_bp->l_fp = lp2;
		lp2->l_fp = lp1->l_fp;
		lp1->l_fp->l_bp = lp2;
		lp2->l_bp = lp1->l_bp;
		free((char *) lp1);
	} else {				/* Easy: in place	*/
		lp2 = lp1;			/* Pretend new line	*/
		lp2->l_used += n;
		cp2 = &lp1->l_text[lp1->l_used];
		cp1 = cp2-n;
		while (cp1 != &lp1->l_text[doto])
			*--cp2 = *--cp1;
	}
	for (i=0; i<n; ++i)			/* Add the characters	*/
		lp2->l_text[doto+i] = c;
	wp = wheadp;				/* Update windows	*/
	while (wp != NULL) {
		if (wp->w_linep == lp1)
			wp->w_linep = lp2;
		if (wp->w_dotp == lp1) {
			wp->w_dotp = lp2;
			if (wp==curwp || wp->w_doto>doto)
				wp->w_doto += n;
		}
		if (wp->w_markp == lp1) {
			wp->w_markp = lp2;
			if (wp->w_marko > doto)
				wp->w_marko += n;
		}
		wp = wp->w_wndp;
	}
	return (TRUE);
}

/*
 * Insert a newline into the buffer at the current location of dot in the
 * current window. The funny ass-backwards way it does things is not a botch;
 * it just makes the last line in the file not a special case. Return TRUE if
 * everything works out and FALSE on error (memory allocation failure). The
 * update of dot and mark is a bit easier then in the above case, because the
 * split forces more updating.
 */
int
lnewline()
{
	register LINE	*lp1;
	register LINE	*lp2;
	register int	doto;
	register WINDOW	*wp;
	int	     nlen;

	if (curbp->b_mode & MDVIEW)
		return(rdonly());
	lchange(WFHARD);
	lp1  = curwp->w_dotp;			/* Get the address and	*/
	doto = curwp->w_doto;			/* offset of "."	*/
	if (doto == 0) {
		if ((lp2=lalloc(doto)) == NULL)	/* New line	     */
			return (FALSE);
		lp2->l_bp = lp1->l_bp;
		lp1->l_bp->l_fp = lp2;
		lp2->l_fp = lp1;
		lp1->l_bp = lp2;
		for (wp = wheadp; wp != NULL; wp = wp->w_wndp) 
			if (wp->w_linep == lp1)
				wp->w_linep = lp2;
		return (TRUE);
	}
	nlen = llength(lp1) - doto;	     /* length of new part */

	if ((lp2 = lalloc(nlen)) == FALSE)
		return (FALSE);
	if (nlen != 0)
		bcopy(&lp1->l_text[doto],&lp2->l_text[0],nlen);
	lp1->l_used = doto;
	lp2->l_bp = lp1;
	lp2->l_fp = lp1->l_fp;
	lp1->l_fp = lp2;
	lp2->l_fp->l_bp = lp2;
	wp = wheadp;				/* Windows		*/
	while (wp != NULL) {
		if (wp->w_dotp == lp1 && wp->w_doto >= doto) {
		    wp->w_dotp = lp2;
		    wp->w_doto -= doto;
		}
		if (wp->w_markp == lp1 && wp->w_marko >= doto) {
		    wp->w_markp = lp2;
		    wp->w_marko -= doto;
		}
		wp = wp->w_wndp;
	}	
	return (TRUE);
}

/*
 * This function deletes "n" bytes, starting at dot. It understands how to 
 * deal with end of lines, etc. It returns TRUE if all of the characters were
 * deleted, and FALSE if they were not (because dot ran into the end of the 
 * buffer). The "kflag" is TRUE if the text should be put in the kill buffer.
 */
int
fdelete(n, kflag)
int	n, kflag;
{
	register char	*cp1;
	register char	*cp2;
	register LINE	*dotp;
	register int	doto;
	register int	chunk;
	register WINDOW	*wp;

	if (curbp->b_mode & MDVIEW)
		return(rdonly());
	while (n != 0) {
		dotp = curwp->w_dotp;
		doto = curwp->w_doto;
		if (dotp == curbp->b_linep) {	/* Hit end of buffer.	*/
			(*term.t_beep)();
			mlwrite("End of Buffer");
			return (FALSE);
		}
		chunk = dotp->l_used - doto;	/* Size of chunk.	*/
		if (chunk > n)
			chunk = n;
		if (chunk == 0) {		/* End of line, merge.	*/
			lchange(WFHARD);
			if (dotp == lback(curbp->b_linep)) { /* End of buffer */
				(*term.t_beep)();
				mlwrite("End of Buffer");
				return(FALSE);
			}
			if (ldelnewline() == FALSE
			|| (kflag!=FALSE && kinsert('\n') == FALSE))
				return (FALSE);
			--n;
			continue;
		}
		lchange(WFEDIT);
		cp1 = &dotp->l_text[doto];	/* Scrunch text.	*/
		cp2 = cp1 + chunk;
		if (kflag != FALSE) {		/* Kill?		*/
			while (cp1 != cp2) {
				if (kinsert(*cp1) == FALSE)
					return (FALSE);
				++cp1;
			}
			cp1 = &dotp->l_text[doto];
		}
		while (cp2 != &dotp->l_text[dotp->l_used])
			*cp1++ = *cp2++;
		dotp->l_used -= chunk;
		wp = wheadp;			/* Fix windows		*/
		while (wp != NULL) {
			if (wp->w_dotp == dotp && wp->w_doto >= doto) {
				wp->w_doto -= chunk;
				if (wp->w_doto < doto)
					wp->w_doto = doto;
			}	
			if (wp->w_markp==dotp && wp->w_marko>=doto) {
				wp->w_marko -= chunk;
				if (wp->w_marko < doto)
					wp->w_marko = doto;
			}
			wp = wp->w_wndp;
		}
		n -= chunk;
	}
	return (TRUE);
}
/*
 * This function deletes "n" bytes backwards from dot. It understands how to
 * deal with end of lines, etc. It returns TRUE if all of the characters were
 * deleted, and FALSE if they were not (because dot ran into the end of the
 * buffer). The "kflag" is TRUE if the text should be put in the kill buffer.
 */
int
bdelete(n, kflag)
int	n, kflag;
{
	register char	*cp1;
	register char	*cp2;
	register LINE	*dotp;
	register int	doto;
	register int	chunk;
	register WINDOW	*wp;

	while (n != 0) {
		dotp = curwp->w_dotp;
		doto = curwp->w_doto;
#if 0
		if (dotp == curbp->b_linep)     /* Hit end of buffer.    */
			return FALSE;
#endif
		chunk = doto;   /* Size of chunk.	*/
		if (chunk > n)
			chunk = n;
		if (chunk == 0) {       /* End of line, merge.   */
			if (backchar(FALSE, 1) == FALSE)
				return FALSE;   /* End of buffer.	*/
			lchange(WFHARD);
			if (ldelnewline() == FALSE
			    || (kflag && kinsert('\n') == FALSE))
				return FALSE;
			--n;
			continue;
		}
		lchange(WFEDIT);
		cp2 = &dotp->l_text[doto];      /* Scrunch text.	 */
		cp1 = cp2 - chunk;
		if (kflag) {
			while (cp1 != cp2) {
				if (kinsert(*cp1) == FALSE)
					return (FALSE);
				cp1++;
			}
			cp1 = &dotp->l_text[doto] - chunk; /* back where we started */
		}
		while (cp2 != &dotp->l_text[dotp->l_used])
			*cp1++ = *cp2++;
		dotp->l_used -= (int) chunk;
		doto -= chunk;
		wp = wheadp;
		while (wp != NULL) {
			if (wp->w_dotp == dotp && wp->w_doto >= doto) {
				wp->w_doto -= chunk;
				if (wp->w_doto < doto)
					wp->w_doto = doto;
			}
			if (wp->w_markp == dotp && wp->w_marko >= doto) {
				wp->w_marko -= chunk;
				if (wp->w_marko < doto)
					wp->w_marko = doto;
			}
			wp = wp->w_wndp;
		}
		n -= chunk;
	}
	return (TRUE);
}

/*
 * Delete a newline. Join the current line with the next line. If the next 
 * line is the magic header line always return TRUE; merging the last line
 * with the header line can be thought of as always being a successful 
 * operation, even if nothing is done, and this makes the kill buffer work
 * "right". Easy cases can be done by shuffling data around. Hard cases 
 * require that lines be moved about in memory. Return FALSE on error and TRUE
 * if all looks ok.
 */
int
ldelnewline()
{
	register LINE	*lp1;
	register LINE	*lp2;
	register LINE	*lp3;
	register WINDOW	*wp;

	if (curbp->b_mode & MDVIEW)
		return(rdonly());
	lp1 = curwp->w_dotp;
	lp2 = lp1->l_fp;
	if (lp2 == curbp->b_linep) {		/* At the buffer end.	*/
		if (lp1->l_used == 0)		/* Blank line.		*/
			lfree(lp1);
		return (TRUE);
	}
	if (lp2->l_used <= lp1->l_size-lp1->l_used) {
		bcopy(&lp2->l_text[0], &lp1->l_text[lp1->l_used], lp2->l_used);
		wp = wheadp;
		while (wp != NULL) {
			if (wp->w_linep == lp2)
				wp->w_linep = lp1;
			if (wp->w_dotp == lp2) {
				wp->w_dotp  = lp1;
				wp->w_doto += lp1->l_used;
			}
			if (wp->w_markp == lp2) {
				wp->w_markp  = lp1;
				wp->w_marko += lp1->l_used;
			}
			wp = wp->w_wndp;
		}		
		lp1->l_used += lp2->l_used;
		lp1->l_fp = lp2->l_fp;
		lp2->l_fp->l_bp = lp1;
		free((char *) lp2);
		return (TRUE);
	}
	if ((lp3=lalloc(lp1->l_used+lp2->l_used)) == NULL)
		return (FALSE);
	bcopy(&lp1->l_text[0], &lp3->l_text[0], lp1->l_used);
	bcopy(&lp2->l_text[0], &lp3->l_text[lp1->l_used], lp2->l_used);
	lp1->l_bp->l_fp = lp3;
	lp3->l_fp = lp2->l_fp;
	lp2->l_fp->l_bp = lp3;
	lp3->l_bp = lp1->l_bp;
	wp = wheadp;
	while (wp != NULL) {
		if (wp->w_linep==lp1 || wp->w_linep==lp2)
			wp->w_linep = lp3;
		if (wp->w_dotp == lp1)
			wp->w_dotp  = lp3;
		else if (wp->w_dotp == lp2) {
			wp->w_dotp  = lp3;
			wp->w_doto += lp1->l_used;
		}
		if (wp->w_markp == lp1)
			wp->w_markp  = lp3;
		else if (wp->w_markp == lp2) {
			wp->w_markp  = lp3;
			wp->w_marko += lp1->l_used;
		}
		wp = wp->w_wndp;
	}
	free((char *) lp1);
	free((char *) lp2);
	return (TRUE);
}

/*
 * Replace plen characters before dot with argument string. Control-J 
 * characters in st are interpreted as newlines. There is a casehack disable 
 * flag (normally it likes to match case of replacement to what was there).
 */
int
lreplace(plen, st, f)
register int	plen;			/* length to remove		*/
char		*st;			/* replacement string		*/
int		f;			/* case hack disable		*/
{
	register int	rlen;		/* replacement length		*/
	register int	rtype;		/* capitalization 		*/
	register int	c;		/* used for random characters	*/
	register int	doto;		/* offset into line		*/

	/*
	 * Find the capitalization of the word that was found.
	 * f says use exact case of replacement string (same thing that
	 * happens with lowercase found), so bypass check.
	 */
	backchar(TRUE, plen);
	rtype = _L;
	c = lgetc(curwp->w_dotp, curwp->w_doto);
	if (ISUPPER(c)!=FALSE  &&  f==FALSE) {
		rtype = _U|_L;
		if (curwp->w_doto+1 < llength(curwp->w_dotp)) {
			c = lgetc(curwp->w_dotp, curwp->w_doto+1);
			if (ISUPPER(c) != FALSE)
				rtype = _U;
		}
	}

	/*
	 * make the string lengths match (either pad the line
	 * so that it will fit, or scrunch out the excess).
	 * be careful with dot's offset.
	 */
	rlen = strlen(st);
	doto = curwp->w_doto;
	if (plen > rlen)
		(void) fdelete(plen-rlen, FALSE);
	else if (plen < rlen) {
		if (linsert(rlen-plen, ' ') == FALSE)
			return (FALSE);
	}
	curwp->w_doto = doto;

	/*
	 * do the replacement:  If was capital, then place first 
	 * char as if upper, and subsequent chars as if lower.  
	 * If inserting upper, check replacement for case.
	 */
	while ((c = *st++&0xff) != '\0') {
		if ((rtype&_U)!=0  &&  ISLOWER(c)!=0)
			c = TOUPPER(c);
		if (rtype == (_U|_L))
			rtype = _L;
		if (c == '\n') {
			if (curwp->w_doto == llength(curwp->w_dotp))
				forwchar(FALSE, 1);
			else {
				if (fdelete(1, FALSE) != FALSE)
					lnewline();
			}
		} else if (curwp->w_dotp == curbp->b_linep) {
			linsert(1, c);
		} else if (curwp->w_doto == llength(curwp->w_dotp)) {
			if (fdelete(1, FALSE) != FALSE)
				linsert(1, c);
		} else
			lputc(curwp->w_dotp, curwp->w_doto++, c);
	}
	lchange(WFHARD);
	return (TRUE);
}

/*
 * Delete all of the text saved in the kill buffer. Called by commands
 * when a new kill context is being created. The kill buffer array is 
 * released, just in case the buffer has grown to immense size. No errors.
 */
void
kdelete()
{
	if (kbufp != NULL) {
		free((char *) kbufp);
		kbufp = (char *) NULL;
		kused = ksize = 0;
	}
}

/*
 * Insert a character to the kill buffer, enlarging the buffer if there isn't
 * any room. Always grow the buffer in chunks, on the assumption that if you
 * put something in the kill buffer you are going to put more stuff there too
 * later. Return TRUE if all is well, and FALSE on errors. Print a message on
 * errors.
 */
int
kinsert(c)
{
	register char	*nbufp;

	if (kused == ksize) {
		if (ksize == 0)      /* First time through */
			nbufp = (char *) malloc((unsigned) (ksize = KBLOCK));
		else
			nbufp = (char *) realloc(kbufp,(unsigned) (ksize += KBLOCK));
		if (nbufp == NULL) {
			mlwrite("Too many kills");
			if (kbufp)
				free(kbufp);
			kused = ksize = 0;
			kbufp = (char *) NULL;
			return (FALSE);
		}
		kbufp  = nbufp;
	}
	kbufp[kused++] = c;
	return (TRUE);
}

/*
 * This function gets characters from the kill buffer. If the character index
 * "n" is off the end, it returns "-1". This lets the caller just scan along 
 * until it gets a "-1" back.
 */
int
kremove(n)
{
	if (n >= kused)
		return (-1);
	return (kbufp[n] & 0xFF);
}
