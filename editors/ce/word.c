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
 * word.c -- commands that work a word at a time.
 *
 * The routines in this file implement commands that work word at a time.
 * There are all sorts of word mode commands. All the sentence mode 
 * commands are in this file as well.
 */

#include	<stdio.h>
#include	"ce.h"

/*
 * Return TRUE if the character at dot is a character that is considered to be
 * part of a word. The word character list is hard coded. Should be settable.
 */
int
inword()
{
	return(curwp->w_doto != llength(curwp->w_dotp) &&
	       ISWORD(lgetc(curwp->w_dotp, curwp->w_doto)));
}

static int
wspace()
{
	return(curwp->w_doto >= 0 &&
		ISWHSP(lgetc(curwp->w_dotp, curwp->w_doto)));
}

/* 
 * Word wrap on n-spaces. Back-over whatever precedes the point on the current
 * line and stop on the first word-break or the beginning of the line. If we
 * reach the beginning of the line, jump back to the end of the word and start
 * a new line.  Otherwise, break the line at the word-break, eat it, and jump
 * back to the end of the word. Returns TRUE on success, FALSE on errors.
 */
int
wrapword()
{
	register int cnt;	/* size of word wrapped to next line */
	int	nl, doto;

	/*
	 * Back up to the last character on the line.
	 */

	if (!backchar(0, 1))
		return(FALSE);

	cnt = doto = 0;

	/*
	 * Back up over the possible end of a sentence.
	 */
	if (atendofsent()) {
		cnt++;
		if (!backchar(0, 1))
			goto restor; 
	}

	/*
	 * Then back up to the start of that word.
	 */
	while (inword()) {
		cnt++;
		if (!backchar(0, 1))
			goto restor;
	}
	/*
	 * Now back up to whitespace.  This character might be a left
	 * paren, for instance.
	 */
	while (wspace() == FALSE) {
		cnt++;
		if (!backchar(0, 1))
			goto restor;
	}

	doto = curwp->w_doto;

	/*
	 * Delete the whitespace character.
	 */

	if (!forwdel(0, 1))
		goto restor;

	/*
	 * Put in a end of line
	 */
	newline(0, 1);

restor:
	if (doto == 0)
		nl = 0;
	else
		nl = (curchar == '\n' || curchar == '\r');

	/*
	 * Skip past the word just wrapped
	 */

	while (cnt--) {
		if (forwchar(FALSE, 1) == FALSE)
			return(FALSE);
	}

	if (nl != 0)
		return(2);	/* magic for random.c: newline() */

	return(TRUE);
}

/*
 * Move the cursor backward by "n" words. All of the details of motion are
 * performed by the "backchar" and "forwchar" routines. Error if you try to
 * move beyond the buffers.
 */
int
backword(f, n)
int	f, n;
{
	if (n < 0)
		return (forwword(f, -n));
	if (backchar(FALSE, 1) == FALSE)
		return (FALSE);
	while (n--) {
		while (inword() == FALSE) {
			if (backchar(FALSE, 1) == FALSE)
				return (FALSE);
		}
		while (inword() != FALSE) {
			if (backchar(FALSE, 1) == FALSE)
				return (FALSE);
		}
	}
	return (forwchar(FALSE, 1));
}

/*
 * Move the cursor forward by the specified number of words. All of the motion
 * is done by "forwchar". Error if you try and move beyond the buffer's end.
 */
int
forwword(f, n)
int	f, n;
{
	if (n < 0)
		return (backword(f, -n));
	while (n--) {
		while (inword() != FALSE) {
			if (forwchar(FALSE, 1) == FALSE)
				return (FALSE);
		}
		while (inword() == FALSE) {
			if (forwchar(FALSE, 1) == FALSE)
				return (FALSE);
		}
	}
	return(TRUE);
}

/*
 * Move the cursor forward by the specified number of words. As you move,
 * convert any characters to upper case. Error if you try and move beyond the
 * end of the buffer. Bound to "M-U".
 */
/*ARGSUSED*/
int
upperword(f, n)
int	f, n;
{
	register int    c;

	if (curbp->b_mode&MDVIEW)	/* don't allow this command if	*/
		return(rdonly());	/* we are in read only mode	*/
	if (n < 0)
		return (FALSE);
	while (n--) {
		while (inword() == FALSE) {
			if (forwchar(FALSE, 1) == FALSE)
				return (FALSE);
		}
		while (inword() != FALSE) {
			c = lgetc(curwp->w_dotp, curwp->w_doto);
			if (ISLOWER(c) != FALSE) {
				c = TOUPPER(c);
				lputc(curwp->w_dotp, curwp->w_doto, c);
				lchange(WFHARD);
			}
			if (forwchar(FALSE, 1) == FALSE)
				return (FALSE);
		}
	}
	return (TRUE);
}

/*
 * Move the cursor forward by the specified number of words. As you move
 * convert characters to lower case. Error if you try and move over the end of
 * the buffer. Bound to "M-L".
 */
/*ARGSUSED*/
int
lowerword(f, n)
int	f, n;
{
	register int    c;

	if (curbp->b_mode&MDVIEW)	/* don't allow this command if	*/
		return(rdonly());	/* we are in read only mode	*/
	if (n < 0)
		return (FALSE);
	while (n--) {
		while (inword() == FALSE) {
			if (forwchar(FALSE, 1) == FALSE)
				return (FALSE);
		}
		while (inword() != FALSE) {
			c = lgetc(curwp->w_dotp, curwp->w_doto);
			if (ISUPPER(c) != FALSE) {
				c = TOLOWER(c);
				lputc(curwp->w_dotp, curwp->w_doto, c);
				lchange(WFHARD);
			}
			if (forwchar(FALSE, 1) == FALSE)
				return (FALSE);
		}
	}
	return (TRUE);
}

/*
 * Move the cursor forward by the specified number of words. As you move
 * convert the first character of the word to upper case, and subsequent
 * characters to lower case. Error if you try and move past the end of the
 * buffer. Bound to "M-C".
 */
/*ARGSUSED*/
int
capword(f, n)
int	f, n;
{
	register int    c;

	if (curbp->b_mode&MDVIEW)	/* don't allow this command if	*/
		return(rdonly());	/* we are in read only mode	*/
	if (n < 0)
		return (FALSE);
	while (n--) {
		while (inword() == FALSE) {
			if (forwchar(FALSE, 1) == FALSE)
				return (FALSE);
		}
		if (inword() != FALSE) {
			c = lgetc(curwp->w_dotp, curwp->w_doto);
			if (ISLOWER(c) != FALSE) {
				c = TOUPPER(c);
				lputc(curwp->w_dotp, curwp->w_doto, c);
				lchange(WFHARD);
			}
			if (forwchar(FALSE, 1) == FALSE)
				return (FALSE);
			while (inword() != FALSE) {
				c = lgetc(curwp->w_dotp, curwp->w_doto);
				if (ISUPPER(c) != FALSE) {
					c = TOLOWER(c);
					lputc(curwp->w_dotp, curwp->w_doto, c);
					lchange(WFHARD);
				}
				if (forwchar(FALSE, 1) == FALSE)
					return (FALSE);
			}
		}
	}
	return (TRUE);
}

/*
 * Kill forward by "n" words. Remember the location of dot. Move forward by
 * the right number of words. Put dot back where it was and issue the kill
 * command for the right number of characters. Bound to "M-D".
 */
/*ARGSUSED*/
int
delfword(f, n)
int	f, n;
{
	register int    size;
	register LINE   *dotp;
	register int    doto;

	if (curbp->b_mode&MDVIEW)	/* don't allow this command if	*/
		return(rdonly());	/* we are in read only mode	*/
	if (n < 0)
		return (FALSE);
	if ((lastflag & CFKILL) == 0)
		kdelete();
	thisflag |= CFKILL;
	dotp = curwp->w_dotp;
	doto = curwp->w_doto;
	size = 0;
	while (n--) {
		while (inword() != FALSE) {
			if (forwchar(FALSE,1) == FALSE)
				goto done;
			++size;
		}
		while (inword() == FALSE) {
			if (forwchar(FALSE, 1) == FALSE)
				goto done;
			++size;
		}
	}
done:
	curwp->w_dotp = dotp;
	curwp->w_doto = doto;
	return (fdelete(size, TRUE));
}

/*
 * Kill backwards by "n" words. Move backwards by the desired number of words,
 * counting the characters. When dot is finally moved to its resting place,
 * fire off the kill command. Bound to "M-Rubout" and to "M-Backspace".
 */
/*ARGSUSED*/
int
delbword(f, n)
int	f, n;
{
	register int    size;
	register LINE	*dotp;
	register int	doto;

	if (curbp->b_mode&MDVIEW)	/* don't allow this command if	*/
		return(rdonly());	/* we are in read only mode	*/
	if (n < 0)
		return (FALSE);
	if ((lastflag & CFKILL) == 0)
		kdelete();
	thisflag |= CFKILL;
	dotp = curwp->w_dotp;
	doto = curwp->w_doto;
	size = 0;
	if (backchar(FALSE, 1) == FALSE)	/* hit buffer start */
		return (TRUE);
	size = 1;
	while (n--) {
		while (inword() == FALSE) {
			if (backchar(FALSE, 1) == FALSE) /* hit buffer start */
				goto done;
			++size;
		}
		while (inword() != FALSE) {
			if (backchar(FALSE, 1) == FALSE) /* hit buffer end */
				goto done;
			++size;
		}
	}
	if (forwchar(FALSE, 1) == FALSE)
		return (FALSE);
	--size;			/* undo assumed delete */
done:
	curwp->w_dotp = dotp;
	curwp->w_doto = doto;
	return (bdelete(size, TRUE));
}

/*
 * Go to the beginning of the next sentence.
 */
int
forwsentence(f,n)
int	f, n;
{
	if (n < 0) 
		return(backsentence(f,-n));
	while (n--) {
		while (atendofsent() != TRUE)
			if (forwchar(FALSE,1) == FALSE)
				return (FALSE);
		while (inword() != TRUE)
			if (forwchar(FALSE,1) == FALSE)
				return (FALSE);
	}
	return (TRUE);
}

/*
 * Go to the beginning of the current sentence.
 */
int
backsentence(f,n)
int	f, n;
{
	if (n < 0)
		return(forwsentence(f,-n));
	while (n--) {
		while (atendofsent() != TRUE)
			if (backchar(FALSE,1) == FALSE) {
				if (curwp->w_dotp == curwp->w_linep)
					break;
				else
					return (FALSE);
			}
		while (inword() != TRUE)
			if (forwchar(FALSE,1) == FALSE)
				return(FALSE);
	}
	return (TRUE);
}

/*
 * Return TRUE if the current char marks the end if the sentence (if it is a
 * ".", a "?", or a "!".
 */
int
atendofsent()
{
	return((curwp->w_doto != llength(curwp->w_dotp)) &&
	       (ISEOSP(lgetc(curwp->w_dotp, curwp->w_doto))));
}

/*
 * Delete the current sentence. Do this by making the region be the current
 * sentence and calling the kill-region code.
 */
/*ARGSUSED*/
int
killsentence(f,n)
int	f, n;
{
	register int s;

	if (curbp->b_mode & MDVIEW)
		return(rdonly());
	if (n < 0)
		return (TRUE);
	while (n--) {
		if ((s = forwsentence(FALSE,1)) != TRUE)
			return s;
		isetmark();
		if ((s = backsentence(FALSE,1)) != TRUE)
			return s;
		if ((s = killregion(FALSE,1)) != TRUE)
			return(s);
	}
	return (TRUE);
}

/*
 * Kill from "." to the beginning of the sentence.
 */
/*ARGSUSED*/
int
delbsentence(f, n)
int	f, n;
{

	register int s;

	if (curbp->b_mode & MDVIEW)
		return(rdonly());
	isetmark();
	if ((s = backsentence(FALSE,1)) != TRUE)
		return s;
	if ((s = killregion(FALSE,1)) != TRUE)
		return s;
	return TRUE;
}

/*
 * Make the region encompass the current word (or n words, if arg given).
 */
int
markword(f,n)
int	f, n;
{
	/*
	 * Set mark, move forward n words, then swap point and mark
	 */
	isetmark();
	forwword(f,n);
	swapmark(FALSE,1);
	return (TRUE);
}
