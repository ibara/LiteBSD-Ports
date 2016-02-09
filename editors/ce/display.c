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
 * display.c -- Gosling style redisplay.
 *
 * From the original v30 uemacs, with improvements by Chet Ramey
 *
 * Note that the Gosling scheme is used unless the NOGOSLING compilation
 * flag is defined.
 *
 * The functions in this file handle redisplay. The
 * redisplay system knows almost nothing about the editing
 * process; the editing functions do, however, set some
 * hints to eliminate a lot of the grinding. There is more
 * that can be done; the "vtputc" interface is a real
 * pig. Two conditional compilation flags; the NOGOSLING
 * flag disables the dynamic programming redisplay using the
 * algorithm published by Jim Gosling in SIGOA.
 * With NOGOSLING on, the terminal is a VT52 (a dumb one at that).
 *
 * At some point, we might want to look at adding a few efficiency 
 * improvements from GNU Emacs.  The basic idea is to improve scrolling
 * nearly all of the screen on slow terminals (determined by baud rate --
 * xterm could be a candidate here as well) by just redrawing the 
 * entire display if that can be accomplished in less than 0.25 seconds.
 * Since the performance of mg under xterm with this same code is 
 * unacceptable for such scrolls, this should work out well.
 */
#include "celibc.h"
#include "ce.h"

extern char	*dversion;

#ifdef X11
extern char	*xdversion;
#endif

#ifdef X11
#  define NOGOSLING
#  define SETNAME(s)	if (s && *s) xsetname(s)
#else
#  define SETNAME(s)
#endif

/*
 * You can change these back to the types
 * implied by the name if you get tight for space. If you
 * make both of them "int" you get better code on the VAX.
 * They do nothing if this is not Gosling redisplay, except
 * for change the size of a structure that isn't used.
 * A bit of a cheat.
 */
#define	XCHAR	int
#define	XSHORT	int

#ifndef NULL
#define NULL 0
#endif

/*
 * A video structure always holds an array of characters whose length 
 * is equal to the longest line possible. Only some of this is used if 
 * "term.t_ncol" isn't the same as "NCOL".
 */
typedef	struct	{
	short	v_hash;			/* Hash code, for compares.	*/
	short	v_flag;			/* Flag word.			*/
	short	v_color;		/* Color of the line.		*/
	XSHORT	v_cost;			/* Cost of display.		*/
	char	v_text[NCOL];		/* The actual characters.	*/
}	VIDEO;

#define	VFCHG	0x0001			/* Changed.			*/
#define	VFHBAD	0x0002			/* Hash and cost are bad.	*/
#define VFEXT	0x0004			/* Extended line		*/

/*
 * SCORE structures hold the optimal
 * trace trajectory, and the cost of redisplay, when
 * the dynamic programming redisplay code is used.
 * If no fancy redisplay, this isn't used. The trace index
 * fields can be "char", and the score a "short", but
 * this makes the code worse on the VAX.
 */
typedef	struct	{
	XCHAR	s_itrace;		/* "i" index for track back.	*/
	XCHAR	s_jtrace;		/* "j" index for trace back.	*/
	XSHORT	s_cost;			/* Display cost.		*/
}	SCORE;

int	isofont = 0;			/* can we draw 8-bit chars?	*/

int	vtrow	= 0;			/* Virtual cursor row.		*/
int	vtcol	= 0;			/* Virtual cursor column.	*/
int	tthue	= CNONE;		/* Current color.		*/
int	ttrow	= HUGE;			/* Physical cursor row.		*/
int	ttcol	= HUGE;			/* Physical cursor column.	*/
int	tttop	= HUGE;			/* Top of scroll region.	*/
int	ttbot	= HUGE;			/* Bottom of scroll region.	*/

#ifndef NOGOSLING
extern  int tceeol;
extern  int tcinsl;
extern  int tcdell;
#endif

static	int lbound = 0;

/*
 * Should these be static?
 */

VIDEO	*vscreen[NROW-1];		/* Edge vector, virtual.	*/
VIDEO	*pscreen[NROW-1];		/* Edge vector, physical.	*/
VIDEO	video[2*(NROW-1)];		/* Actual screen data.		*/
VIDEO	blanks;				/* Blank line image.		*/

#ifndef NOGOSLING
/*
 * This matrix is written as an array because we do funny things in 
 * the "setscores" routine, which is very compute intensive, to make
 * the subscripts go away. It would be "SCORE score[NROW][NROW]" in old 
 * speak.  Look at "setscores" to understand what is up.
 */
SCORE	score[NROW*NROW];
#endif

/*
 * Forward declarations
 */
extern void movecursor();

static void updatecopy();
static void updateline();
static void updateext();
static void modeline();
static void hash();
static void setscores();
static void traceback();

/*
 * Initialize the data structures used by the display code. The edge
 * vectors used to access the screens are set up. The operating system's 
 * terminal I/O channel is set up. Fill the "blanks" array with ASCII 
 * blanks. The rest is done at compile time. The original window is marked
 * as needing full update, and the physical screen is marked as garbage,
 * so all the right stuff happens on the first call to redisplay.
 */
void
vtinit()
{
	register VIDEO	*vp;
	register int	i;

	(*term.t_init)();
	/*
	 * The `init' must be done before the `open', because the open sets
	 * the screen size, which might use termcap to get lines and cols.
	 * Termcap stuff is initialized in `init'.
	 */
	(*term.t_open)();
	vp = &video[0];
	for (i = 0; i < NROW-1; ++i) {
		vscreen[i] = vp;
		++vp;
		pscreen[i] = vp;
		++vp;
	}
	blanks.v_color = CTEXT;
	for (i = 0; i < NCOL; ++i)
		blanks.v_text[i] = ' ';
}

/*
 * Tidy up the virtual display system in anticipation of a return back 
 * to the host operating system. Right now all we do is position the 
 * cursor to the last line, erase the line, and close the terminal channel.
 */
void
vttidy()
{
#if !defined (X11)
	(*term.t_color)(CTEXT);
	movecursor(term.t_nrow, 0);		/* Echo line.		*/
	(*term.t_eeol)();
	tttidy();
#endif
	(*term.t_flush)();
	(*term.t_close)();
}

/*
 * Move the virtual cursor to an origin 0 spot on the virtual display
 * screen. I could store the column as a character pointer to the spot
 * on the line, which would make "vtputc" a little bit more efficient.
 * No checking for errors or outlandish values.
 */
void
vtmove(row, col)
{
	vtrow = row;
	vtcol = col;
}

/*
 * Write a character to the virtual display, dealing with long lines and
 * the display of unprintable things like control characters. Also expand
 * tabs every 8 columns. This code only puts printing characters into the 
 * virtual display image. Special care must be taken when expanding tabs. 
 * On a screen whose width is not a multiple of 8, it is possible for the
 * virtual cursor to hit the  right margin before the next tab stop is 
 * reached. This makes the tab code loop if you are not careful.
 * Three guesses how we found this.
 */
void
vtputc(c)
register int	c;
{
	register VIDEO	*vp;

	vp = vscreen[vtrow];
	if (vtcol >= term.t_ncol)
		vp->v_text[term.t_ncol-1] = '$';
	else if (c == '\t') {
		do {
			vtputc(' ');
		} while ((vtcol < term.t_ncol) && ((vtcol&0x07)!=0));
	} else if (ISCTRL(c) != FALSE) {
		vtputc('^');
		vtputc(c ^ 0x40);
	} else if (c > 127) {
		register int	n;

		c %= 256;
		if (isofont)
			vp->v_text[vtcol++] = c;
		else {
			vtputc('\\');
			vtputc((n = (c / 64)) + '0');
			c -= n * 64;
			vtputc((c / 8) + '0');
			vtputc((c % 8) + '0');
		}
	} else
		vp->v_text[vtcol++] = c;		
}

/*
 * Write a character to the virtual display, dealing with long lines.
 * This is called to put characters in extended lines only.  
 */
void
vtputext(c)
register int	c;
{
	register VIDEO	*vp;

	vp = vscreen[vtrow];
	if (vtcol >= term.t_ncol)
		vp->v_text[term.t_ncol-1] = '$';
	else if (c == '\t') {
		do
			vtputext(' ');
		while ((((vtcol + lbound)&0x07) != 0) && vtcol < term.t_ncol);
	} else if (ISCTRL(c) != FALSE) {
		vtputext('^');
		vtputext(CCHR(c));
	} else if (c > 127) {
		register int	n;

		c %= 256;
		if (isofont) {
			if (vtcol >= 0)
				vp->v_text[vtcol] = c;
			++vtcol;
		} else {
			vtputext('\\');
			vtputext((n = (c / 64)) + '0');
			c -= n * 64;
			vtputext((c / 8) + '0');
			vtputext((c % 8) + '0');
		}
	} else {
		if (vtcol >= 0) 
			vp->v_text[vtcol] = c;		
		++vtcol;
	}
}

int
vtgetc(col)
int	col;
{
	return vscreen[vtrow]->v_text[col];
}

/*
 * Erase from the end of the software cursor to the end of the line on 
 * which the software cursor is located. The display routines will decide
 * if a hardware erase to end of line command should be used to display this.
 */
void
vteeol()
{
	register VIDEO	*vp;

	vp = vscreen[vtrow];
	while ((vtcol >= 0) && (vtcol < term.t_ncol))
		vp->v_text[vtcol++] = ' ';
}

/*
 * Send a command to the terminal to move the hardware cursor to row "row"
 * and column "col". The row and column arguments are origin 0.  Update
 * "ttrow" and "ttcol".
 */
void
movecursor(row, col)
int	row, col;
{
	ttrow = row;
	ttcol = col;
	(*term.t_move)(row, col);
}

/*
 * Make sure that the display is right. This is a three part process. First,
 * scan through all of the windows looking for dirty ones. Check the framing,
 * and refresh the screen.  Second, make sure that "currow" and "curcol" are
 * correct for the current window. Third, make the virtual and physical 
 * screens the same.
 *
 * Don't do the update if there is pending input -- it's wasteful.
 */
void
update()
{
	register LINE	*lp;
	register WINDOW	*wp;
	register VIDEO	*vp1;
	register VIDEO	*vp2;
	register int	i;
	register int	j;
	register int	c;
	register int	hflag;
	register int	currow;
	register int	curcol;
#ifndef NOGOSLING
	register int	offs;
	register int	size;
#endif

	if (typeahead()) 
		return;
	if (sgarbf) {
		wp = wheadp;
		while (wp != NULL) {
			wp->w_flag |= WFMODE | WFHARD; /* Must do mode lines.*/
			wp = wp->w_wndp;
		}
	}
	hflag = FALSE;				/* Not hard.		*/
	wp = wheadp;
	while (wp != NULL) {
		if (wp->w_flag != 0) {		/* Need update.		*/
			if ((wp->w_flag&WFFORCE) == 0) {
				lp = wp->w_linep;
				for (i = 0; i < wp->w_ntrows; ++i) {
					if (lp == wp->w_dotp)
						goto out;
					if (lp == wp->w_bufp->b_linep)
						break;
					lp = lforw(lp);
				}
			}
			i = wp->w_force;	/* Reframe this one.	*/
			if (i > 0) {
				--i;
				if (i >= wp->w_ntrows)
					i = wp->w_ntrows - 1;
			} else if (i < 0) {
				i += wp->w_ntrows;
				if (i < 0)
					i = 0;
			} else
				i = wp->w_ntrows/2;
			lp = wp->w_dotp;
			while (i != 0 && lback(lp) != wp->w_bufp->b_linep) {
				--i;
				lp = lback(lp);
			}
			wp->w_linep = lp;
			wp->w_flag |= WFHARD;	/* Force full.		*/
		out:
			lp = wp->w_linep;	/* Try reduced update.	*/
			i  = wp->w_toprow;
			if ((wp->w_flag&~WFMODE) == WFEDIT) {
				/*
				 * Update a single line
				 */
				while (lp != wp->w_dotp) {
					++i;
					lp = lforw(lp);
				}
				vscreen[i]->v_color = CTEXT;
				vscreen[i]->v_flag |= (VFCHG|VFHBAD);
				vtmove(i, 0);
				for (j = 0; j < llength(lp); ++j)
					vtputc(lgetc(lp, j));
				vteeol();
			} else if ((wp->w_flag&(WFEDIT|WFHARD)) != 0) {
				/* Update all lines that need it. */
				hflag = TRUE;
				while (i < wp->w_toprow + wp->w_ntrows) {
					vscreen[i]->v_color = CTEXT;
					vscreen[i]->v_flag |= (VFCHG|VFHBAD);
					vtmove(i, 0);
					if (lp != wp->w_bufp->b_linep) {
						for (j = 0; j < llength(lp); ++j)
							vtputc(lgetc(lp, j));
						lp = lforw(lp);
					}
					vteeol();
					++i;
				}
			}
			if ((wp->w_flag&WFMODE) != 0)
				modeline(wp);
			wp->w_flag = 0;
			wp->w_force = 0;
		}		
		wp = wp->w_wndp;
	}
	lp = curwp->w_linep;			/* Cursor location.	*/
	currow = curwp->w_toprow;
	while (lp != curwp->w_dotp) {
		++currow;
		lp = lforw(lp);
	}
	curcol = 0;
	i = 0;
	while (i < curwp->w_doto) {
		c = lgetc(lp, i++);
		if (c == '\t')
			curcol |= 0x07;
		else if (ISCTRL(c) != FALSE)
			++curcol;
		else if (c > 127)
			curcol += isofont ? 0 : 3;
		++curcol;
	}
	if (curcol >= term.t_ncol - 1) {	/* Long line.	        */
		vscreen[currow]->v_flag |= (VFEXT|VFCHG);
		updateext(currow, curcol);	/* Update the extended line */
	} else
		lbound = 0;

	/* 
	 * Now worry about any lines that need to be de-extended because the 
	 * cursor is no longer > term.t_ncol-1.
	 */

	wp = wheadp;
	while (wp) {
		lp = wp->w_linep;
		i = wp->w_toprow;
		while (i < wp->w_toprow + wp->w_ntrows) {
			if (vscreen[i]->v_flag & VFEXT) {	/* an extended line */
				vscreen[i]->v_flag |= VFCHG;
				if ((wp != curwp) || (lp != wp->w_dotp) || (curcol < term.t_ncol-1)) {
					vtmove(i,0);
					if (lp != wp->w_bufp->b_linep)
						for (j = 0; j < llength(lp); ++j)
							vtputc(lgetc(lp,j));
					vteeol();
					vscreen[i]->v_flag &= ~VFEXT;  /* not extended now */
				}
			}
			if (lp != wp->w_bufp->b_linep)
				lp = lforw(lp);
			++i;
		}
		if (sgarbf != FALSE)
			vscreen[i]->v_flag |= VFCHG;
		wp = wp->w_wndp;
	}
	if (sgarbf != FALSE) {			/* Screen is garbage.	*/
		sgarbf = FALSE;			/* Erase-page clears	*/
		mpresf = FALSE;			/* the message area.	*/
#ifndef X11
		tttop  = HUGE;			/* Forget where you set	*/
		ttbot  = HUGE;			/* scroll region.	*/
		tthue  = CNONE;			/* Color unknown.	*/
#endif
		movecursor(0, 0);
		(*term.t_eeop)();
		for (i=0; i < term.t_nrow; ++i) {
			updateline(i, vscreen[i], &blanks);
			updatecopy(vscreen[i], pscreen[i]);
		}
		movecursor(currow, curcol - lbound);
		(*term.t_flush)();
		SETNAME(curbp->b_bname);
		return;
	}
#ifndef NOGOSLING
	if (hflag != FALSE) {			/* Hard update?	*/
		for (i = 0; i < term.t_nrow; ++i) {  /* Compute hash data. */
			hash(vscreen[i]);
			hash(pscreen[i]);
		}
		offs = 0;			/* Get top match.	*/
		while (offs != term.t_nrow) {
			vp1 = vscreen[offs];
			vp2 = pscreen[offs];
 			if (vp1->v_color != vp2->v_color
			||  vp1->v_hash  != vp2->v_hash)
				break;
			updateline(offs, vp1, vp2);
			updatecopy(vp1, vp2);
			++offs;
		}
		if (offs == term.t_nrow) {	/* Might get it all.	*/
			movecursor(currow, curcol - lbound);
			(*term.t_flush)();
			return;
		}
		size = term.t_nrow;		/* Get bottom match.	*/
		while (size != offs) {
			vp1 = vscreen[size-1];
			vp2 = pscreen[size-1];
			if (vp1->v_color != vp2->v_color
			||  vp1->v_hash  != vp2->v_hash)
			        break;          /* quit if no match */
			updateline(size-1, vp1, vp2);
			updatecopy(vp1, vp2);
			--size;
		}
		if ((size -= offs) == 0)	/* Get screen size.	*/
		        panic("Illegal screen size in update()");
		/*
		 * Now, we have determined that "size" lines beginning
		 * at "offs" have changed.  If this is close to the
		 * entire screen, put in a goto to the easy update code.
		 */
		if (size >= term.t_nrow - 3)
			goto easy;
		setscores(offs, size);		/* Do hard update.	*/
		traceback(offs, size, size, size);
		for (i=0; i<size; ++i)
			updatecopy(vscreen[offs+i], pscreen[offs+i]);
		movecursor(currow, curcol - lbound);
		(*term.t_flush)();
		SETNAME(curbp->b_bname);
		return;			
	}
easy:
#endif
	for (i=0; i < term.t_nrow; ++i) {     /* Easy update.	*/
		vp1 = vscreen[i];
		vp2 = pscreen[i];
		if ((vp1->v_flag&VFCHG) != 0) {
			updateline(i, vp1, vp2);
			updatecopy(vp1, vp2);
		}
	}
	movecursor(currow, curcol - lbound);
	(*term.t_flush)();
	SETNAME(curbp->b_bname);
}

/*
 * Update a saved copy of a line, kept in a VIDEO structure. The "vvp" is
 * the one in the "vscreen". The "pvp" is the one in the "pscreen". This 
 * is called to make the virtual and physical screens the same when
 * display has done an update.
 */
static void
updatecopy(vvp, pvp)
register VIDEO	*vvp;
register VIDEO	*pvp;
{
	vvp->v_flag &= ~VFCHG;			/* Changes done.	*/
	pvp->v_flag  = vvp->v_flag;		/* Update modeline.	*/
	pvp->v_hash  = vvp->v_hash;
	pvp->v_cost  = vvp->v_cost;
	pvp->v_color = vvp->v_color;
	bcopy(vvp->v_text, pvp->v_text, term.t_ncol);
}

/*
 * Update a single line. This routine only uses basic functionality (no 
 * insert and delete character, but erase to end of line). The "vvp"
 * points at the VIDEO structure for the line on the virtual screen, and 
 * the "pvp" is the same for the physical screen. Avoid erase to end of
 * line when updating CMODE color lines, because of the way that reverse 
 * video works on most terminals.
 */
static void
updateline(row, vvp, pvp)
VIDEO	*vvp;
VIDEO	*pvp;
{
	register char	*cp1;
	register char	*cp2;
	register char	*cp3;
	register char	*cp4;
	register char	*cp5;
	register int	nbflag;

	if (vvp->v_color != pvp->v_color) {	/* Wrong color, do a	*/
		movecursor(row, 0);		/* full redraw.	        */
		(*term.t_color)(vvp->v_color);
		cp1 = &vvp->v_text[0];
		cp2 = &vvp->v_text[term.t_ncol];
#ifdef X11
		x_putline(row, cp1, cp2 - cp1);
		ttcol += cp2 - cp1;
#else
		while (cp1 < cp2) {
			(*term.t_putchar)(*cp1++);
			++ttcol;
		}
#endif
		return;
	}
	cp1 = &vvp->v_text[0];			/* Compute left match.	*/
	cp2 = &pvp->v_text[0];
	while (cp1 != &vvp->v_text[term.t_ncol] && cp1[0] == cp2[0]) {
		++cp1;
		++cp2;
	}
	if (cp1 == &vvp->v_text[term.t_ncol])	/* All equal.	*/
		return;
	nbflag = FALSE;
	cp3 = &vvp->v_text[term.t_ncol];	/* Compute right match.	*/
	cp4 = &pvp->v_text[term.t_ncol];
	while (cp3[-1] == cp4[-1]) {
		--cp3;
		--cp4;
		if (cp3[0] != ' ')		/* Note non-blanks in	*/
			nbflag = TRUE;		/* the right match.	*/
	}
	cp5 = cp3;				/* Is erase good?	*/
	if (nbflag == FALSE && vvp->v_color == CTEXT) {
		while (cp5 != cp1 && cp5[-1] == ' ')
			--cp5;
#ifndef X11
		/* Alcyon hack */
		if ((int)(cp3-cp5) <= tceeol)
			cp5 = cp3;
#endif
	}

	movecursor(row, (int)(cp1 - &vvp->v_text[0]));
	(*term.t_color)(vvp->v_color);
#ifdef X11
	x_putline(row, cp1, cp5 - cp1 +1);	
	if (cp1 != cp5)
		ttcol += cp5 - cp1;
#else
	while (cp1 != cp5) {
		(*term.t_putchar)(*cp1++);
		++ttcol;
	}
#endif
	if (cp5 != cp3)				/* Do erase. */
		(*term.t_eeol)();
}

/*
 * Update an extended line, scrolling it right if needed, so that the cursor
 * ends up somewhere in the middle of the screen.
 */
static void
updateext(currow,curcol)
int currow,curcol;
{
	register LINE	*lp;
	register int	i;

	lbound = curcol - (curcol % (term.t_ncol >> 1)) - (term.t_ncol >> 2);
	vtmove(currow,-lbound);
	lp = curwp->w_dotp;
	for (i = 0; i < llength(lp); ++i)
		vtputext(lgetc(lp,i));
	vteeol();
	vscreen[currow]->v_text[0] = '$';	/* mark as extended */
}


/*
 * Get the current position of "." as a ratio to the total number of
 * characters in the current buffer.  This is used by the "modeline"
 * routine; it is a stripped-down version of showcpos() in random.c.  It
 * could get pretty expensive to show the ratio for long files; this will
 * have to be considered when optimizing ce.
 */
static int
getratio()
{
	register LINE 	*clp;
	register long 	nch;
	register int	cch = 0, ratio;

	clp = lforw(curbp->b_linep);
	nch = cch = 0;
	for (;;) {
		if (clp == curwp->w_dotp)
			cch = nch + curwp->w_doto;
		if (clp == curbp->b_linep)
			break;
		nch += llength(clp);
		clp = lforw(clp);
		++nch;
	}
	ratio = (nch) ? (100L*cch)/nch : 0;
	return (ratio);
}

/*
 * Redisplay the mode line for the window pointed to by the "wp".
 * This is the only routine that has any idea of how the modeline is 
 * formatted. You can change the modeline format by hacking at this 
 * routine. Called by "update" any time there is a dirty window.
 */
static void
modeline(wp)
register WINDOW	*wp;
{
	register char	*cp;
	register int	c;
	register int	n, i;
	register BUFFER	*bp;
        int 		firstm;
	char		tline[NLINE];
	int 		pos;

	n = wp->w_toprow+wp->w_ntrows;		/* Location.		*/
	vscreen[n]->v_color = CMODE;		/* Mode line color.	*/
	vscreen[n]->v_flag |= (VFCHG|VFHBAD);	/* Recompute, display.	*/
	vtmove(n, 0);				/* Seek to right line.	*/
	bp = wp->w_bufp;
	vtputc('-'); vtputc('-');
	if ((bp->b_flag&BFCHG) != 0) {          /* "*" if changed.	*/
		vtputc('*');
		vtputc('*');
	} else if ((bp->b_mode&MDVIEW) != 0) {
		vtputc('%');
		vtputc('%');
	} else {
		vtputc('-');
		vtputc('-');
	}
	vtputc('-');
	n  = 5;
#if defined (X11)
	cp = xdversion;
#else
	cp = dversion;
#endif /* X11 */
	while ((c = *cp++) != 0) {	
		vtputc(c);
		++n;
	}
	vtputc(':'); n++;
	if (bp->b_bname[0] != 0) {	/* Buffer name.		*/
		vtputc(' ');
		++n;
		cp = &bp->b_bname[0];
		while ((c = *cp++) != 0) {
			vtputc(c);
			++n;
		}
	}
	while (n < 42) {		/* Pad out with blanks like GNU */
	        vtputc(' ');
		++n;
	}
	firstm = TRUE;
	tline[0] = '(';
	tline[1] = '\0';
	if (wp->w_bufp->b_mode == 0)
	        strcat(tline,"Fundamental");
	else {
	        for (i = 0; i < nummodes; i++) {
			if (wp->w_bufp->b_mode & (1 << i)) {
		        	if (firstm != TRUE)
					strcat(tline, " ");
				firstm = FALSE;
				strcat(tline, modename[i]);
			}
		}
	}
	strcat(tline, ")");
	cp = &tline[0];
	while((c = *cp++) != 0) {
	        vtputc(c);
		++n;
	}
	vtputc('-'); vtputc('-'); vtputc('-'); vtputc('-');
	n += 4;
	pos = getratio();
	if (pos <= 1)
		(void) sprintf(tline, "Top");
	else if (pos >= 99)
		(void) sprintf(tline, "Bot");
	else
		(void) sprintf(tline,"%2d%%", pos);
	cp = &tline[0];
	while ((c = *cp++) != 0) {
		vtputc(c);
		++n;
	}	
	while (n < term.t_ncol) {			/* Pad out to eol. */
		vtputc('-');
		++n;
	}
}

#ifndef	NOGOSLING
/*
 * Compute the hash code for
 * the line pointed to by the "vp". Recompute
 * it if necessary. Also set the approximate redisplay
 * cost. The validity of the hash code is marked by
 * a flag bit. The cost understand the advantages
 * of erase to end of line. Tuned for the VAX
 * by Bob McNamara; better than it used to be on
 * just about any machine.
 */
static void
hash(vp)
register VIDEO	*vp;
{
	register int	i;
	register int	n;
	register char	*s;
 
	if ((vp->v_flag&VFHBAD) != 0) {		/* Hash bad.		*/
		s = &vp->v_text[term.t_ncol-1];
		for (i=term.t_ncol; i!=0; --i, --s)
			if (*s != ' ')
				break;
		n = term.t_ncol-i;		/* Erase cheaper?       */
		if (n > tceeol)
			n = tceeol;
		vp->v_cost = i+n;		/* Bytes + blanks.	*/
		for (n=0; i!=0; --i, --s)
			n = (n<<5) + n + *s;
		vp->v_hash = n;			/* Hash code.		*/
		vp->v_flag &= ~VFHBAD;		/* Flag as all done.	*/
	}
}

/*
 * Compute the Insert-Delete
 * cost matrix. The dynamic programming algorithm
 * described by James Gosling is used. This code assumes
 * that the line above the echo line is the last line involved
 * in the scroll region. This is easy to arrange on the VT100
 * because of the scrolling region. The "offs" is the origin 0
 * offset of the first row in the virtual/physical screen that
 * is being updated; the "size" is the length of the chunk of
 * screen being updated. For a full screen update, use offs=0
 * and size=term.t_nrow-1.
 *
 * Older versions of this code implemented the score matrix by
 * a two dimensional array of SCORE nodes. This put all kinds of
 * multiply instructions in the code! This version is written to
 * use a linear array and pointers, and contains no multiplication
 * at all. The code has been carefully looked at on the VAX, with
 * only marginal checking on other machines for efficiency. In
 * fact, this has been tuned twice! Bob McNamara tuned it even
 * more for the VAX, which is a big issue for him because of
 * the 66 line X displays.
 *
 * On some machines, replacing the "for (i=1; i<=size; ++i)" with
 * i = 1; do { } while (++i <=size)" will make the code quite a
 * bit better; but it looks ugly.
 */
static void
setscores(offs, size)
{
	register SCORE	*sp;
	register int	tempcost;
	register int	bestcost;
	register int	j;
	register int	i;
	register VIDEO	**vp;
	register VIDEO	**pp;
	register SCORE	*sp1;
	register VIDEO	**vbase;
	register VIDEO	**pbase;
 
	vbase = &vscreen[offs-1];		/* By hand CSE's.	*/
	pbase = &pscreen[offs-1];
	score[0].s_itrace = 0;			/* [0, 0]		*/
	score[0].s_jtrace = 0;
	score[0].s_cost   = 0;
	sp = &score[1];				/* Row 0, inserts.	*/
	tempcost = 0;
	vp = &vbase[1];
	for (j=1; j<=size; ++j) {
		sp->s_itrace = 0;
		sp->s_jtrace = j-1;
		tempcost += tcinsl;
		tempcost += (*vp)->v_cost;
		sp->s_cost = tempcost;
		++vp;
		++sp;
	}
	sp = &score[NROW];			/* Column 0, deletes.	*/
	tempcost = 0;
	for (i=1; i<=size; ++i) {
		sp->s_itrace = i-1;
		sp->s_jtrace = 0;
		tempcost  += tcdell;
		sp->s_cost = tempcost;
		sp += NROW;
	}
	sp1 = &score[NROW+1];			/* [1, 1].		*/
	pp = &pbase[1];
	for (i=1; i<=size; ++i) {
		sp = sp1;
		vp = &vbase[1];
		for (j=1; j<=size; ++j) {
			sp->s_itrace = i-1;
			sp->s_jtrace = j;
			bestcost = (sp-NROW)->s_cost;
			if (j != size)		/* Cd(A[i])=0 @ Dis.	*/
				bestcost += tcdell;
			tempcost = (sp-1)->s_cost;
			tempcost += (*vp)->v_cost;
			if (i != size)		/* Ci(B[j])=0 @ Dsj.	*/
				tempcost += tcinsl;
			if (tempcost < bestcost) {
				sp->s_itrace = i;
				sp->s_jtrace = j-1;
				bestcost = tempcost;
			}
			tempcost = (sp-NROW-1)->s_cost;
			if ((*pp)->v_color != (*vp)->v_color
			||  (*pp)->v_hash  != (*vp)->v_hash)
				tempcost += (*vp)->v_cost;
			if (tempcost < bestcost) {
				sp->s_itrace = i-1;
				sp->s_jtrace = j-1;
				bestcost = tempcost;
			}
			sp->s_cost = bestcost;
			++sp;			/* Next column.		*/
			++vp;
		}
		++pp;
		sp1 += NROW;			/* Next row.		*/
	}
}

/*
 * Trace back through the dynamic programming cost
 * matrix, and update the screen using an optimal sequence
 * of redraws, insert lines, and delete lines. The "offs" is
 * the origin 0 offset of the chunk of the screen we are about to
 * update. The "i" and "j" are always started in the lower right
 * corner of the matrix, and imply the size of the screen.
 * A full screen traceback is called with offs=0 and i=j=term.t_nrow-1.
 * There is some do-it-yourself double subscripting here,
 * which is acceptable because this routine is much less compute
 * intensive then the code that builds the score matrix!
 */
static void
traceback(offs, size, i, j)
{
	register int	itrace;
	register int	jtrace;
	register int	k;
	register int	ninsl;
	register int	ndraw;
	register int	ndell;

	if (i==0 && j==0)			/* End of update.	*/
		return;
	itrace = score[(NROW*i) + j].s_itrace;
	jtrace = score[(NROW*i) + j].s_jtrace;
	if (itrace == i) {			/* [i, j-1]		*/
		ninsl = 0;			/* Collect inserts.	*/
		if (i != size)
			ninsl = 1;
		ndraw = 1;
		while (itrace!=0 || jtrace!=0) {
			if (score[(NROW*itrace) + jtrace].s_itrace != itrace)
				break;
			jtrace = score[(NROW*itrace) + jtrace].s_jtrace;
			if (i != size)
				++ninsl;
			++ndraw;
		}
		traceback(offs, size, itrace, jtrace);
		if (ninsl != 0) {
			(*term.t_color)(CTEXT);
			ttinsl(offs+j-ninsl, offs+size-1, ninsl);
		}
		do {				/* B[j], A[j] blank.	*/
			k = offs+j-ndraw;
			updateline(k, vscreen[k], &blanks);
		} while (--ndraw > 0);
		return;
	}
	if (jtrace == j) {			/* [i-1, j]		*/
		ndell = 0;			/* Collect deletes.	*/
		if (j != size)
			ndell = 1;
		while (itrace!=0 || jtrace!=0) {
			if (score[(NROW*itrace) + jtrace].s_jtrace != jtrace)
				break;
			itrace = score[(NROW*itrace) + jtrace].s_itrace;
			if (j != size)
				++ndell;
		}
		if (ndell != 0) {
			(*term.t_color)(CTEXT);
			ttdell(offs+i-ndell, offs+size-1, ndell);
		}
		traceback(offs, size, itrace, jtrace);
		return;
	}
	traceback(offs, size, itrace, jtrace);
	k = offs+j-1;
	updateline(k, vscreen[k], pscreen[offs+i-1]);
}
#endif
