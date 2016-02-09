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
 * random.c -- random command processing functions
 *
 * This file contains the command processing functions for a number of random
 * commands. There is no functional grouping here, for sure.
 */

#include <stdio.h>
#include <string.h>
#include "ce.h"

extern int	explarg;
extern int	kused;
extern int	followlinks;

int     tabstop;			/* Tab size (0: use real tabs)  */

/*
 * Update all the mode lines for all the windows
 */
void
upmode()
{
	register WINDOW *wp;

	wp = wheadp;
	while (wp != NULL) {
		wp->w_flag |= WFMODE;
		wp = wp->w_wndp;
	}
}

/*
 * Set fill column to n.  If explarg == 1 (set in main.c), then an explicit
 * argument was given and we should use it.  If not, then prompt for a new
 * value.
 */
/*ARGSUSED*/
int
setfillcol(f, n)
int	f, n;
{
	int		col, s;
	char		buf[NSTRING];

	if (explarg == FALSE) {
		s = mlreply("Set Fill column to: ", buf, sizeof(buf));
		if (s != TRUE)
			return s;
		col = atoi(buf);
		if (col <= 0) {
			(*term.t_beep)();
			mlwrite("Preposterous value, fill column unchanged");
			return (TRUE);
		}
		fillcol = col;
	} else
		fillcol = n;
	mlwrite("Fill column set to %d", fillcol);
	return(TRUE);
}

/*
 * Display the current position of the cursor, in origin 1 X-Y coordinates,
 * the character that is under the cursor (in octal), and the fraction of the
 * text that is before the cursor. The displayed column is not the current
 * column, but the column that would be used on an infinite width display.
 * Normally this is bound to "C-X =".
 */

static char *
cstr(c, str)
int	c;
char	*str;
{
	register char	*p = str;

	if (c < 0x20 || c == 0x7F) {
		*p++ = '^';
		*p++ = CCHR(c);
	} else
		*p++ = c;
	*p = '\0';
	return str;
}

	
/*ARGSUSED*/
int
showcpos(f, n)
int	f, n;
{
	register LINE   *clp;
	register long   nch;
	register int	nli;
	register int    cac;
	register int    ratio;
	register int    col;
	register int	currow;
	int		cli,cch;
	char		chseq[8];		/* Actual char sequence printed */

	clp = lforw(curbp->b_linep);	    /* Grovel the data.     */
	nch = cch = 0;
	nli = cli = 0;
	cac = '\n';
	for (;;) {
		++nli;
		if (clp == curwp->w_dotp) {       /* got to the current line */
			cli = nli;
			cch = nch + curwp->w_doto;      /* this is "." */
			if (curwp->w_doto == llength(clp))
				cac = '\n';
			else
				cac = lgetc(clp, curwp->w_doto);
		}
		if (clp == curbp->b_linep)
			break;
		nch += llength(clp);
		clp = lforw(clp);
		++nch;
	}
	currow = curwp->w_toprow + 1;
	clp = curwp->w_linep;
	while (clp != curbp->b_linep && clp != curwp->w_dotp) {
		++currow;
		clp = lforw(clp);
	}
	col = getccol(FALSE);		   /* Get real column.     */
	ratio = 0;			      /* Ratio before dot.    */
	if (nch != 0)
		ratio = (100L*cch) / nch;
	if (cli == 0)
		cli = 1;			/* Boundary condition 	*/
	mlwrite("line=%d (of %d) col=%d row=%d C=%s (0x%x) point=%D (%d%% of %D)",
		cli, nli, col+1, currow, cstr(cac,&chseq[0]), cac, cch, ratio, nch);
	return (TRUE);
}

/*
 * Print out what line we are on.
 */
int
whatline(f, n)
int	f, n;
{
	int	l = 1;
	register LINE	*lp;

	lp = lforw(curbp->b_linep);
	while (lp != curwp->w_dotp) {
		if (lp == curbp->b_linep)
			break;
		l++;
		lp = lforw(lp);
	}
	mlwrite("Current line = %d", l);
	return (TRUE);
}

/*
 * Return display column described by DOTP and DOTO.  Stop at first
 * non-blank if BFLG == TRUE.
 */
int
colpos(dotp, doto, bflg)
register LINE	*dotp;
int	doto, bflg;
{
	register int c, i, col = 0;
	int ts;

	ts = (tabstop == 0) ? 8 : tabstop;
	for (i = 0; i< doto; ++i) {
		c = lgetc(dotp, i);
		if (c != ' ' && c != '\t' && bflg)
			break;
		if (c == '\t')
			col |= (ts - 1);
		else if (ISCTRL(c) != FALSE)
			++col;
		++col;
	}
	return col;
}

/*
 * Return current column.  Stop at first non-blank given TRUE argument.
 */
int
getccol(bflg)
int bflg;
{
	return(colpos(curwp->w_dotp, curwp->w_doto, bflg));
}

/*
 * Twiddle the two characters on either side of dot. If dot is at the end of
 * the line twiddle the two characters before it. Return with an error if dot
 * is at the beginning of line; it seems to be a bit pointless to make this
 * work. This fixes up a very common typo with a single stroke. Normally bound
 * to "C-T". This always works within a line, so "WFEDIT" is good enough.
 */
/*ARGSUSED*/
int
twiddle(f, n)
int	f, n;
{
	register LINE   *dotp;
	register int    doto;
	register int    cl;
	register int    cr;

	if (curbp->b_mode&MDVIEW)	/* don't allow this command if	*/
		return(rdonly());	/* we are in read only mode	*/
	dotp = curwp->w_dotp;
	doto = curwp->w_doto;
	if (doto==llength(dotp) && --doto<0)
		return (FALSE);
	cr = lgetc(dotp, doto);
	if (--doto < 0)
		return (FALSE);
	cl = lgetc(dotp, doto);
	lputc(dotp, doto+0, cr);
	lputc(dotp, doto+1, cl);
	lchange(WFEDIT);
	return (TRUE);
}

/*
 * Quote the next character, and insert it into the buffer. All the characters
 * are taken literally, with the exception of the newline, which always has
 * its line splitting meaning. The character is always read, even if it is
 * inserted 0 times, for regularity. Bound to "C-Q"
 */
/*ARGSUSED*/
int
quote(f, n)
int	f, n;
{
	register int    s;
	register int    c;

	if (curbp->b_mode&MDVIEW)	/* don't allow this command if	*/
		return(rdonly());	/* we are in read only mode	*/
	c = (*term.t_getchar)();
	if (n < 0)
		return (FALSE);
	if (n == 0)
		return (TRUE);
	if (c == '\n') {
		do {
			s = lnewline();
		} while (s==TRUE && (--n > 0));
		return (s);
	}
	return (linsert(n, c));
}

/*
 * Set tab size if given non-default argument (n <> 1).  Otherwise, insert a
 * tab into file.  If given argument, n, of zero, change to true tabs.
 * If n > 1, simulate tab stop every n-characters using spaces. This has to be
 * done in this slightly funny way because the tab (in ASCII) has been turned
 * into "C-I" (in 10 bit code) already. Bound to "C-I".
 */
/*ARGSUSED*/
int
tab(f, n)
int	f, n;
{
	if (n < 0)
		return (FALSE);
	if (n == 0 || n > 1) {
		tabstop = n;
		return(TRUE);
	}
	if (tabstop == 0)
		return(linsert(1, '\t'));
	return(linsert(tabstop - (getccol(FALSE) % tabstop), ' '));
}

/*
 * Open up some blank space. The basic plan is to insert a bunch of newlines,
 * and then back up over them. Everything is done by the subcommand
 * procerssors. They even handle the looping. Normally this is bound to "C-O".
 */
int
openline(f, n)
int	f, n;
{
	register int    i;
	register int    s;

	if (curbp->b_mode&MDVIEW)	/* don't allow this command if	*/
		return(rdonly());	/* we are in read only mode	*/
	if (n < 0)
		return (FALSE);
	if (n == 0)
		return (TRUE);
	i = n;				  /* Insert newlines.     */
	do {
		s = lnewline();
	} while (s==TRUE && --i);
	if (s == TRUE)			  /* Then back up overtop */
		s = backchar(f, n);	     /* of them all.	 */
	return (s);
}

/*
 * Open vertical space.  Insert a newline, then enough space on the next
 * line to get to the current offset, then back up to the original point.
 * This ignores numeric arguments.
 */
int
openvert(f, n)
int	f, n;
{
	register WINDOW *wp;
	int	s, doto, nc, ts;

	if (curbp->b_mode&MDVIEW)	/* don't allow this command if	*/
		return(rdonly());	/* we are in read only mode	*/
	doto = curwp->w_doto;
	ts = (tabstop == 0) ? 8 : tabstop;
	s = lnewline();
	if (s != TRUE)
		return (s);
	nc = 1;
	n = doto / ts;			/* tabs to insert */
	while (n--) {
		if (linsert(1, '\t') == FALSE)
			break;
		nc++;
	}
	n = doto % ts;
	while (n--) {
		if (linsert(1, ' ') == FALSE)
			break;
		nc++;
	}
	s = backchar(1, nc);
	if ((curbp->b_flag & BFCHG) == 0) {
		curbp->b_flag |= BFCHG;
		wp = wheadp;
		while (wp != NULL) {
			if (wp->w_bufp == curbp)
				wp->w_flag |= WFMODE;
			wp = wp->w_wndp;
		}
	}
	return (s);
}

/*
 * Insert a newline. Bound to "C-M".
 */
/*ARGSUSED*/
int
newline(f, n)
int	f, n;
{
	register int    s;
	register WINDOW *wp;
	int	addch = 0;		/* if set to 2, do not add newline */

	if (curbp->b_mode&MDVIEW)	/* don't allow this command if	*/
		return(rdonly());	/* we are in read only mode	*/
	if (n < 0)
		return (FALSE);
	if (n == 1 && (curbp->b_mode & MDCMODE) && curwp->w_dotp != curwp->w_linep)
		return(cnewline());
	while (n--) {	       	/* insert some lines */
		if ((curwp->w_bufp->b_mode & MDWRAP) && (fillcol > 0) &&
		    (getccol(FALSE) > fillcol))
			addch = wrapword();
		if ((addch != 2) && (s = lnewline()) != TRUE)
			return (s);
	}
	if ((curbp->b_flag & BFCHG) == 0) {
		curbp->b_flag |= BFCHG;
		wp = wheadp;
		while (wp != NULL) {
			if (wp->w_bufp == curbp)
				wp->w_flag |= WFMODE;
			wp = wp->w_wndp;
		}
	}
	update();	       /* let's see if this one works... */
	return (TRUE);
}

/*
 * Delete blank lines around dot. What this command does depends if dot is
 * sitting on a blank line. If dot is sitting on a blank line, this command
 * deletes all the blank lines above and below the current line. If it is
 * sitting on a non blank line then it deletes all of the blank lines after
 * the line. Normally this command is bound to "C-X C-O". Any argument is
 * ignored.
 */
/*ARGSUSED*/
int
deblank(f, n)
int	f, n;
{
	register LINE   *lp1;
	register LINE   *lp2;
	register int    nld;

	if (curbp->b_mode&MDVIEW)	/* don't allow this command if	*/
		return(rdonly());	/* we are in read only mode	*/
	lp1 = curwp->w_dotp;
	while (llength(lp1)==0 && (lp2=lback(lp1))!=curbp->b_linep)
		lp1 = lp2;
	lp2 = lp1;
	nld = 0;
	while ((lp2=lforw(lp2))!=curbp->b_linep && llength(lp2)==0)
		++nld;
	if (nld == 0)
		return (TRUE);
	curwp->w_dotp = lforw(lp1);
	curwp->w_doto = 0;
	return (fdelete(nld,FALSE));
}

/*
 * Insert a newline, then enough tabs and spaces to duplicate the indentation
 * of the previous line. Assumes tabs are every eight characters. Quite simple.
 * Figure out the indentation of the current line. Insert a newline by calling
 * the standard routine. Insert the indentation by inserting the right number
 * of tabs and spaces. Return TRUE if all ok. Return FALSE if one of the
 * subcomands failed. Normally bound to "C-J".
 */
/*ARGSUSED*/
int
indent(f, n)
int	f, n;
{
	register int    nicol;
	register int    c;
	register int    i;
	register WINDOW *wp;
	register int	ts = (tabstop == 0) ? 8 : tabstop;

	if (curbp->b_mode&MDVIEW)	/* don't allow this command if	*/
		return(rdonly());	/* we are in read only mode	*/
	if (n < 0)
		return (FALSE);
	while (n--) {
		nicol = 0;
		for (i = 0; i < llength(curwp->w_dotp); ++i) {
			c = lgetc(curwp->w_dotp, i);
			if (c != ' ' && c != '\t')
				break;
			if (c == '\t')
				nicol |= (ts - 1);
			++nicol;
		}
		if (lnewline() == FALSE ||
		    (((i = nicol / ts) != 0) && linsert(i, '\t') == FALSE) ||
		    (((i = nicol % ts) != 0) && linsert(i, ' ') == FALSE))
			return (FALSE);
	}
	if ((curbp->b_flag & BFCHG) == 0) {
		curbp->b_flag |= BFCHG;
		wp = wheadp;
		while (wp != NULL) {
			if (wp->w_bufp == curbp)
				wp->w_flag |= WFMODE;
			wp = wp->w_wndp;
		}
	}
	update();		     /* let's see what this does... */
	return (TRUE);
}

/*
 * Delete forward. This is real easy, because the basic delete routine does
 * all of the work. Watches for negative arguments, and does the right thing.
 * If any argument is present, it kills rather than deletes, to prevent loss
 * of text if typed with a big argument. Normally bound to "C-D".
 */
int
forwdel(f, n)
int	f, n;
{
	if (curbp->b_mode&MDVIEW)	/* don't allow this command if	*/
		return(rdonly());	/* we are in read only mode	*/
	if (n < 0)
		return (backdel(f, -n));
	if (f != FALSE) {		       /* Really a kill.       */
		if ((lastflag&CFKILL) == 0)
			kdelete();
		thisflag |= CFKILL;
	}
	return (fdelete(n, f));
}

/*
 * Delete backwards. This is quite easy too, because it's all done with other
 * functions. Just move the cursor back, and delete forwards. Like delete
 * forward, this actually does a kill if presented with an argument. Bound to
 * both "RUBOUT" and "C-H".
 */
int
backdel(f, n)
int	f, n;
{
	if (curbp->b_mode&MDVIEW)	/* don't allow this command if	*/
		return(rdonly());	/* we are in read only mode	*/
	if (n < 0)
		return (forwdel(f, -n));
	if (f != FALSE) {		       /* Really a kill.       */
		if ((lastflag&CFKILL) == 0)
			kdelete();
		thisflag |= CFKILL;
	}
	return (bdelete(n, f));
}

/*
 * Delete white space around dot.  This ignores arguments.  This is a
 * GNU emacs-compatible function.  Bound to M-\.
 */
int
delspace(f, n)
int	f, n;
{
	int	c, doto, tokill;

	if (curbp->b_mode & MDVIEW)	/* don't allow this command if	*/
		return(rdonly());	/* we are in read only mode	*/
	if ((lastflag & CFKILL) == 0)
		kdelete();
	thisflag |= CFKILL;
	doto = curwp->w_doto;
	while (doto != llength(curwp->w_dotp)) {
		c = lgetc(curwp->w_dotp, doto);
		if (ISWHSP(c) != TRUE)
			break;
		doto++;
	}
	tokill = doto - curwp->w_doto;
	doto = curwp->w_doto;
	while (doto) {
		c = lgetc(curwp->w_dotp, doto);
		if (ISWHSP(c) != TRUE)
			break;
		doto--;
	}
	tokill += curwp->w_doto - doto - 1;
	backchar(1, curwp->w_doto - doto - 1);
	return (fdelete(tokill, 1));
}
	
/*
 * Kill text. If called without an argument, it kills from dot to the end of
 * the line, unless it is at the end of the line, when it kills the newline.
 * If called with an argument of 0, it kills from the start of the line to dot.
 * If called with a positive argument, it kills from dot forward over that
 * number of newlines. If called with a negative argument it kills backwards
 * that number of newlines. Normally bound to "C-K".
 */
int
killtext(f, n)
int	f, n;
{
	register int    chunk;
	register LINE   *nextp;
	register int	i;

	if (curbp->b_mode&MDVIEW)	/* don't allow this command if	*/
		return(rdonly());	/* we are in read only mode	*/
	if ((lastflag&CFKILL) == 0)	     /* Clear kill buffer if */
		kdelete();		      /* last wasn't a kill.  */
	thisflag |= CFKILL;
	if (f == FALSE) {	/* no arg given, do the current line */
		chunk = llength(curwp->w_dotp) - curwp->w_doto;
		if (chunk == 0)
			chunk = 1;
		i = fdelete(chunk, TRUE);
	} else if (n > 0) {	/* C-U xx C-K */
		chunk = llength(curwp->w_dotp) - curwp->w_doto + 1;
		nextp = lforw(curwp->w_dotp);
		i = n;
		while (--i != 0) {
			if (nextp == curbp->b_linep) {
#if 0
				(*term.t_beep)();
				mlwrite("End of Buffer");
#endif
				break;
			}
			chunk += llength(nextp) + 1;
			nextp = lforw(nextp);
		}
		i = fdelete(chunk, TRUE);
	} else {		/* negative kill, back up over n newlines */
		chunk = curwp->w_doto;
		nextp = lback(curwp->w_dotp);
		i = n;
		while (i++ != 0) {
			if (nextp == curbp->b_linep)
				break;

			chunk += llength(nextp) + 1;
			nextp = lback(nextp);
			/* curwp->w_flag |= WFMOVE; */
		}
		i = bdelete(chunk, TRUE);
	}
	return i;
}

/*
 * Yank text back from the kill buffer. This is really easy. All of the work
 * is done by the standard insert routines. All you do is run the loop, and
 * check for errors. Bound to "C-Y".
 */
/*ARGSUSED*/
int
yank(f, n)
int	f, n;
{
	register int    c;
	register int    i;

	if (curbp->b_mode&MDVIEW)	/* don't allow this command if	*/
		return(rdonly());	/* we are in read only mode	*/
	if (n < 0)
		return (FALSE);
	while (n--) {
		mlwrite("[Yanking %d characters...]", kused);
		isetmark();
		i = 0;
		while ((c = kremove(i)) >= 0) {
			if (c == '\n') {
				if (lnewline() == FALSE)
					return (FALSE);
			} else {
				if (linsert(1, c) == FALSE)
					return (FALSE);
			}
			++i;
		}
		mlwrite("[Yanked %d characters]", kused);
	}
	mlwrite("Mark Set");
	return (TRUE);
}

/*
 * Toggle read-only status on the current buffer
 */
int
readonly(f, n)
int	f, n;
{
	register BUFFER	*bp;

	bp = curwp->w_bufp;
	if (bp->b_mode & MDVIEW)
		bp->b_mode &= ~MDVIEW;
	else
		bp->b_mode |= MDVIEW;
	upmode();
	return (TRUE);
}

/*ARGSUSED*/
int
setemode(f, n)	/* prompt and set an editor mode */
int	f, n;	/* default and argument */
{
	return(adjustmode(TRUE, FALSE));
}

/*ARGSUSED*/
int
delemode(f, n)	/* prompt and delete an editor mode */
int	f, n;	/* default and argument */
{
	return(adjustmode(FALSE, FALSE));
}

/*ARGSUSED*/
int
setgmode(f, n)	/* prompt and set a global editor mode */
int	f, n;	/* default and argument */
{
	return(adjustmode(TRUE, TRUE));
}

/*ARGSUSED*/
int
delgmode(f, n)	/* prompt and delete a global editor mode */
int 	f, n;	/* default and argument */
{
	return(adjustmode(FALSE, TRUE));
}

/*
 * Set current buffer to View mode.
 */
/*ARGSUSED*/
int
viewmode(f,n)
int	f,n;
{
	return(chmode("View", TRUE, FALSE));
}

/*ARGSUSED*/
int
noviewmode(f,n)
int	f,n;
{
	return(chmode("View", FALSE, FALSE));
}

/*
 * Set current buffer to Wrap mode.
 */
/*ARGSUSED*/
int
wrapmode(f,n)
int	f,n;
{
	return(chmode("Fill", TRUE, FALSE));
}

/*ARGSUSED*/
int
nowrapmode(f,n)
int	f,n;
{
	return(chmode("Fill", FALSE, FALSE));
}

/* ARGSUSED */
int
cmode(f, n)
int	f, n;
{
	return(chmode("C", TRUE, FALSE));
}

/* ARGSUSED */
int
nocmode(f, n)
int	f, n;
{
	return(chmode("C", FALSE, FALSE));
}

/*
 * Set the current buffer to Auto-Save mode.
 */
/*ARGSUSED*/
int
autosavemode(f,n)
int	f,n;
{
	return(chmode("Save", TRUE, FALSE));
}

/*ARGSUSED*/
int
noautosavemode(f,n)
int	f,n;
{
	return(chmode("Save", FALSE, FALSE));
}

/*
 * Set current buffer to Match mode.
 */
/*ARGSUSED*/
int
matchmode(f,n)
int	f,n;
{
	return(chmode("Match", TRUE, FALSE));
}

/*ARGSUSED*/
int
nomatchmode(f,n)
int	f,n;
{
	return(chmode("Match", FALSE, FALSE));
}

/*
 * Change the editor mode status.
 */
int
adjustmode(kind, global)
int	kind;	/* true = set,		false = delete */
int	global;	/* true = global flag,	false = current buffer flag */
{
	register char *scan;		/* scanning pointer to convert prompt */
	char 	prompt[50];		/* string to prompt user with */
	char 	cbuf[NPAT];		/* buffer to recieve mode name into */
	int	s;

	/* build the proper prompt string */
	if (global)
		strcpy(prompt,"Global mode to ");
	else
		strcpy(prompt,"Mode to ");
	if (kind == TRUE)
		strcat(prompt, "add: ");
	else
		strcat(prompt, "delete: ");
	/* prompt the user and get an answer */
	if ((s = mlreply(prompt, cbuf, NPAT - 1)) != TRUE)
		return s;
	cbuf[NPAT - 1] = '\0';
	/* make it a capitalized word, like in ce.h */
	scan = cbuf;
	while (*scan && (*scan == ' ' || *scan == '\t'))
		scan++;
	if (*scan == '\0')
		return FALSE;
	if (ISLOWER(*scan) != FALSE)
		*scan = TOUPPER(*scan);
	scan++;
	while (*scan != 0) {
		if (ISUPPER(*scan) != FALSE)
			*scan = TOLOWER(*scan);
		scan++;
	}
	return(chmode(cbuf, kind, global));
}

int
chmode(name, setting, global)
char	*name;
int 	setting;
int	global;
{
	register int i;

	/*
	 * If executing a startup file, all changes are global
	 */
	if (clexec)
		global = TRUE;

	for (i = 0; i < nummodes; i++) {
		if (strcmp(name, modename[i]) == 0) {
			/* finding a match, we process it */
			if (setting == TRUE) {
				if (global)
					gmode |= (1 << i);
				if (curwp)
					curwp->w_bufp->b_mode |= (1 << i);
				if (curbp)
					curbp->b_mode |= (1 << i);		
			} else {
				if (global)
					gmode &= ~(1 << i);
				if (curwp)
					curwp->w_bufp->b_mode &= ~(1 << i);
				if (curbp)
					curbp->b_mode &= ~(1 << i);
			}
			/* display new mode line */
			upmode();
			mlerase();	/* erase the junk */
			return(TRUE);
		}
	}
	(*term.t_beep)();
	mlwrite("No such mode!");
	return(FALSE);
}

/*
 * Center the current line (or a number of lines starting with the current
 * one) on the display, according to the current fill column.  Due to the
 * fucked-up way I did this originally, doto is restored if only one line is
 * centered, and set to the beginning of the first line centered when multiple
 * lines are centered.
 */
/*ARGSUSED*/
int
centerline(f,n)
int	f,n;
{
	register int  s,i;

	if (curbp->b_mode & MDVIEW)
		return(rdonly());
	if (n <= 0)
		return (FALSE);
	i = n;
	if (i == 1) {		/* special case, restore doto */
		if ((s = center1()) != TRUE)
			return(s);
		return (TRUE);
	} 
	while (i--) {
		if ((s = center1()) != TRUE)
			return(s);
		curwp->w_dotp = lforw(curwp->w_dotp);
	}
	while (n--) 
		curwp->w_dotp = lback(curwp->w_dotp);
	curwp->w_doto = 0;
	curwp->w_flag |= WFHARD;
	return(TRUE);
}

/*
 * The derivation of startpos (the column position of the first non-space char
 * after the centering) is as follows:
 *	leading_whitespace = i;
 *	screen_middle = fillcol / 2;
 *	amount_of_text = len - leading_whitespace;	(non-whitespace text)
 *	text_start_pos_after_center = screen_middle - (amount_of_text / 2);
 *				    = (fillcol / 2) - (amount_of_text / 2);
 *				    = (fillcol - amount_of_text) / 2;
 *				    = (fillcol - (len - leading_whitespace)) / 2
 *				    = (fillcol - (len - i)) / 2
 *
 * Pretty neat, huh?
 */
int 
center1()
{
	register LINE	*lp;
	register int	i;
	int 		s, c, doto, len, startpos, nblanks, ntab, ts;

	lp = curwp->w_dotp;
	doto = curwp->w_doto;
	len = llength(lp);
	i = 0;
	while (i < doto) {
		c = lgetc(lp,i++);
		if (c == '\t')
			i |= 0x07;
		if (ISWORD(c) == TRUE)
			break;
	}
	startpos = (fillcol - (len - i)) / 2;

	nblanks = startpos - i;
	ts = (tabstop == 0) ? 8 : tabstop;
	ntab = nblanks / ts;
	if (nblanks > 0)
		nblanks %= ts;

	curwp->w_doto = 0;	/* so linsert and ldelete will work OK */
	if (nblanks > 0 || ntab > 0) {
		if ((s = linsert(ntab, '\t')) != TRUE) {
			curwp->w_doto = doto;
			return s;
		}
		if ((s = linsert(nblanks, ' ')) != TRUE) {
			curwp->w_doto = doto;
			return s;
		}
	} else if (nblanks < 0) {
		if ((s = fdelete(-nblanks, FALSE)) != TRUE) {
			curwp->w_doto = doto;
			return s;
		}
	}
	doto += ntab + nblanks;
	if (doto < 0)
		doto = 0;
	if (doto > llength(curwp->w_dotp))
		doto = llength(curwp->w_dotp);
	curwp->w_doto = doto;
	return (TRUE);
}

/*
 * Center all of the lines in the buffer, according to the current fill
 * column.
 */
int
centerbuffer(f,n)
int 	f,n;
{
	igotoeob();
	isetmark();
	igotobob();
	return(centerregion(f,n));
}


/*
 *  This is a hack put in (by Chet) so that I can use the xterm/vt100 
 *  arrow keys without having to change the way I do key reading.
 *  Bound to "M-O", which is what the arrow keys put out first; the next
 *  character determines the key and function.
 */
/*ARGSUSED*/
int
xtermarrowkeys(f,n)
int	f, n;
{
	register int c;

	c = getkey();
	switch(c) {
	    case 'A':	/* up arrow    */
	    case 'a':
		backline(f,1); 
		break;
	    case 'B':	/* down arrow  */
	    case 'b':
		forwline(f,1);
		break;
	    case 'C':	/* right arrow */
	    case 'c':
		forwchar(f,1);
		break;
	    case 'D':   /* left arrow  */
	    case 'd':
		backchar(f,1);
		break;
	    default:
		(*term.t_beep)();
		 mlwrite("Key not bound");
		 return (FALSE);
	}
	return (TRUE);
}

/*
 * Transpose the current and previous lines.
 * Error if you're at the start of the buffer.
 */
/*ARGSUSED*/
int
twiddlelines(f, n)
int	f, n;
{
	register LINE	*this, *next, *prev;
	LINE 		*savedbp;

	if (curbp->b_mode & MDVIEW)
		return (rdonly());
	while (n--) {
		this = curwp->w_dotp;
		prev = lback(this);
		next = lforw(this);
		if (this == curwp->w_linep)
			return (FALSE);
		savedbp = prev->l_bp;
		prev->l_fp = next;
		prev->l_bp = this;
		this->l_fp = prev;
		this->l_bp = savedbp;
		savedbp->l_fp = this;
		next->l_bp = prev;
		curwp->w_dotp = this;
	}
	curwp->w_flag |= (WFHARD|WFMODE);
	return (TRUE);
}

/*
 * Toggle the flag that controls the making of backup files on or off, since
 * we don't have variables in ce.
 */
/*ARGSUSED*/
int
makebackups(f, n)
int	f, n;
{
	doingbackups = doingbackups == FALSE;
	mlwrite("Backup files %sabled", (doingbackups == TRUE) ? "en" : "dis");
	return (TRUE);
}

/*
 * Make the file writing code follow (nor not follow) symlinks when making
 * backup files.
 */
/*ARGSUSED*/
int
chaselinks(f, n)
int	f, n;
{
	followlinks = TRUE;
	return TRUE;
}

/*ARGSUSED*/
int
nochaselinks(f, n)
int	f, n;
{
	followlinks = FALSE;
	return TRUE;
}
