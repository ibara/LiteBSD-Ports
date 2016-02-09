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
 * tty.c -- UNIX termcap/terminfo display driver
 *
 * Termcap is a terminal information database and routines to describe 
 * terminals on a UNIX system.  This should ALWAYS be used on a UNIX system
 * that has it.
 */
#include "celibc.h"
#define termdef
#include "ce.h"
#undef termdef

#ifndef NULL
#define NULL 	0
#endif

#define	BEL	0x07			/* BEL character.		*/
#define	ESC	0x1B			/* ESC character.		*/
#define	LF	0x0A			/* Line feed.			*/

/*
 * This was made into a macro in the interest of efficiency
 */

#define putpad(s)	tputs(s, 1, ttputc);

extern	int	ttrow;
extern	int	ttcol;
extern	int	tttop;
extern	int	ttbot;
extern	int	tthue;

int		tceeol;			/* Costs are set later */
int		tcinsl;
int		tcdell;

extern char	*tgetstr();
extern char 	*tgoto();

#define TCAPSLEN 1024

char 		tcapbuf[TCAPSLEN];
char    	 PC;
char 		*BC,			/* termcap(3) uses these for tgoto */
		*UP;

short		ospeed;

static char 	*CM,		/* the rest can be static */
		*CE,
		*IM,		/* insert mode */
		*IC,		/* insert a single space */
		*EI,		/* end insert mode */
		*DC,
		*AL,		/* add line */
		*DL,		/* del line */
		*TI,		/* term init -- start using cursor motion */
		*TE,		/* term end --- end using cursor motion */
		*SO,
		*SE,
		*CD,
		*KS,
		*KE;

int     	ttinit();
int     	ttmove();
int     	tteeop();
int     	tteeol();
int     	ttbeep();
int     	ttcolor();
int		ttresize();

void		ttsetup();
void		ttinsl();
void		ttdell();
void		tttidy();
void		ttykeytidy();

extern  int	ttopen();
extern  int	ttclose();
extern  int	ttflush();
extern  int	ttputc();
extern  int	ttgetc();

TERM term = {
	NROW - 1,
	NCOL,
	ttinit,
	ttopen,
	ttclose,
	ttgetc,
	ttputc,
	ttflush,
	ttmove,
	tteeol,
	tteeop,
	ttbeep,
	ttcolor,
	ttresize
};

	  
/*
 * Initialize the terminal when the editor gets started up.
 */

static char tcbuf[TCAPSLEN];

int
ttinit()
{
	char *getenv();
	char *t, *p, *tgetstr();
	char *tt_type;

	tt_type = getenv("TERM");
	if (tt_type == NULL || *tt_type == '\0') {
		(void) write(2, "ce: environment variable TERM null or not set\r\n", 47);
		exit(1);
	}

	onxterm = strncmp("xterm", tt_type, 5) == 0;

	if (tgetent(tcbuf, tt_type) != 1) {
		(void) write(2, "ce: unknown terminal type: ", 27);
		(void) write(2, tt_type, strlen(tt_type));
		(void) write(2, "\r\n", 2);
		exit(1);
	}
	
	p = tcapbuf;
	t = tgetstr("pc", &p);
	if (t)
		PC = *t;

	BC = tgetstr("bc", &p);
	CD = tgetstr("cd", &p);
	CM = tgetstr("cm", &p);
	CE = tgetstr("ce", &p);
	UP = tgetstr("up", &p);
	IM = tgetstr("im", &p);
	IC = tgetstr("ic", &p);
	EI = tgetstr("ei", &p);
	DC = tgetstr("dc", &p);
	AL = tgetstr("al", &p);
	DL = tgetstr("dl", &p);
	TI = tgetstr("ti", &p);
	TE = tgetstr("te", &p);
	SO = tgetstr("so", &p);
	SE = tgetstr("se", &p);
	KS = tgetstr("ks", &p);
	KE = tgetstr("ke", &p);

	if (CD == NULL || CM == NULL || CE == NULL || UP == NULL) {
		(void) write(2, "This terminal (\"", 16);
		(void) write(2, tt_type, strlen(tt_type));
		(void) write(2, "\") is not powerful enough to run ce\r\n", 37);
		exit(1);
	}
	ttresize();
	if (*CE == 0)
	   	tceeol = term.t_ncol;
	else
		tceeol = charcost(CE);
	if (AL == 0 || *AL == 0)       /* make this cost high enough that it */
		tcinsl = term.t_nrow * term.t_ncol;  /* won't ever happen */
	else
		tcinsl = charcost(AL);
	if (DL == 0 || *DL == 0)	   /* make this cost high enough that it */
		tcdell = term.t_nrow * term.t_ncol;  /* won't ever happen */
	else
		tcdell = charcost(DL);

	if (p >= &tcapbuf[TCAPSLEN]) {
		(void) puts("ce: Terminal description too big.\r\n");
		ttclose();
		exit(1);
	}
	ttsetup();
	return (0);
}

/*
 * Send the initialization string to the terminal to set it up.
 */
void
ttsetup()
{
	if (TI && *TI)
		putpad (TI);	/* init the term */
}

/*
 * Clean up the terminal, in anticipation of
 * a return to the command interpreter. This is a no-op
 * on the ANSI display. On the SCALD display, it sets the
 * window back to half screen scrolling. Perhaps it should
 * query the display for the increment, and put it
 * back to what it was.
 */
void
tttidy()
{
	if (TE && *TE) 
		putpad (TE);	/* set the term back to normal mode */
	ttykeytidy();	   /* stop using keypad keys	   */
}

/*
 * Move the cursor to the specified origin 0 row and column position. Try to
 * optimize out extra moves; redisplay may have left the cursor in the right
 * location last time!
 */
int
ttmove(row, col)
{
	putpad(tgoto(CM, col, row));
	ttrow = row;
	ttcol = col;
	return 0;
}

/*
 * Erase to end of line.
 */
int
tteeol()
{
	putpad(CE);
	return 0;
}

/*
 * Erase to end of page.
 */
int
tteeop()
{
	putpad(CD);
	return 0;
}

/*
 * Make a noise.
 */
int
ttbeep()
{
	ttputc(BEL);
	ttflush();
	return 0;
}

/*
 * Insert nchunk blank line(s) onto the screen, scrolling the last line on the
 * screen off the bottom. This is done with a cluster of clever insert and 
 * delete commands, because there are no scroll regions (while scroll regions
 * might be a win on other terminals, they are terrible on big bitmapped
 * screens like the Sun or VAXstation, which are what we use around here).
 */
void
ttinsl(row, bot, nchunk)
int	row, bot, nchunk;
{
	register int	i;
    
	if (row == bot) {		/* Case of one line insert is 	*/
		ttmove(row, 0);		/*	special			*/
		tteeol();
		return;
	}
	ttmove(1+bot-nchunk, 0);
	for (i=0; i < nchunk; i++)	/* For all lines in the chunk	*/
		putpad(DL);
	ttmove(row, 0);
	for (i=0; i < nchunk; i++)	/* For all lines in the chunk	*/
		putpad(AL);
	ttrow = row;			/* End up on current line	*/
	ttcol = 0;
}

/*
 * Delete nchunk line(s) "row", replacing the bottom line on the screen 
 * with a blank line. This is done with a crafty sequences of insert and 
 * delete line; We assume that the terminal is like the Heath (ie no scrolling
 * region). The presence of the echo area makes a boundry condition go away.
 */
void
ttdell(row, bot, nchunk)
int	row, bot, nchunk;
{
	register int	i;
    
	if (row == bot) {		/* One line special case	*/
		ttmove(row, 0);
		tteeol();
		return;
	}
	ttmove(row, 0);
	for (i=0; i < nchunk; i++)	/* For all lines in chunk	*/
		putpad(DL);
	ttmove(1+bot-nchunk, 0);
	for (i=0; i < nchunk; i++)	/* For all lines in chunk	*/
		putpad(AL);
	ttrow = bot-nchunk;
	ttcol = 0;
}

/*
 * Set the current writing color to the specified color. Watch for color 
 * changes that are not going to do anything (the color is already right)
 * and don't send anything to the display.
 */
int
ttcolor(color)
register int	color;
{
	if (color != tthue) {
		if (color == CTEXT) {		/* Normal video.	*/
			putpad(SE);
		} else if (color == CMODE) {	/* Reverse video.	*/
			putpad(SO);
		}
		tthue = color;			/* Save the color.	*/
	}
	return 0;
}

/*
 * This routine is called by the "refresh the screen" command to try and resize
 * the display. The new size, which must be deadstopped to not exceed the NROW
 * and NCOL limits, is stored back into the "term" structure. Display can 
 * always deal with a screen NROW by NCOL. Look in "window.c" to see how the 
 * caller deals with a change.
 */
int
ttresize()
{
	ttsetsize();				/* set the size and */
	if (term.t_nrow >= NROW)
		term.t_nrow = NROW - 1;
	if (term.t_ncol > NCOL)
		term.t_ncol = NCOL;
	if (term.t_nrow < 1)			/* check limits.    */
		term.t_nrow = 1;
	if (term.t_ncol < 1)
		term.t_ncol = 1;
	return 0;
}

static int cci;

/*ARGSUSED*/
static int
fakec(c)			/* fake char output for charcost() */
char	c;
{
	cci++;
	return cci;
}

int
charcost (s)			/* calculate the cost of doing string s */
char	*s;
{
	cci = 0;
	tputs(s, term.t_nrow, fakec);
	return cci;
}

/*
 * End use of the keypad.
 */
void
ttykeytidy()
{
	if (KE && *KE)
		putpad(KE);
}
