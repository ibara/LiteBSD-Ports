/*
 * This file is part of ce.
 *
 * Copyright (c) 1997 by Chester Ramey.
 *
 * Permission is hereby granted to copy, reproduce, redistribute or
 * otherwise use this software subject to the following: 
 *      1) That there be no monetary profit gained specifically from 
 *         the use or reproduction of this software.
 *      2) This software may not be sold, rented, traded or otherwise 
 *         marketed.
 *      3) This copyright notice must be included prominently in any copy
 *         made of this software.
 *
 * The author makes no claims as to the fitness or correctness of
 * this software for any use whatsoever, and it is provided as is. 
 * Any use of this software is at the user's own risk.
 */

/*
 * ttyio.c -- Unix terminal I/O.
 *
 * The functions in this file negotiate with the operating system for
 * keyboard characters, and write characters to the display in a barely 
 * buffered fashion.
 *
 * This code works with the POSIX.1 (termios), System V (termio), and
 * 4.2 BSD (sgtty) terminal drivers.
 */

/* order is important here... */
#include <stdio.h>
#include "ce.h"
#include <signal.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include "celibc.h"

#ifdef HAVE_LIMITS_H
#include <limits.h>
#endif

#include	"ttyio.h"

/*
 * Forward declarations
 */
extern void	ttsetsize();
extern void	panic();

#ifdef TERMIO_TTY_DRIVER

#define tcgetattr(fd, termiop)        (ioctl((fd), TCGETA, (termiop)))
#define tcsetattr(fd, flag, termiop) \
	(ioctl((fd), \
	       (flag == TCSADRAIN) ? TCSETAW : \
				     ((flag == TCSANOW) ? TCSETA : TCSETAF), \
	       (termiop)))
#endif /* TERMIO_TTY_DRIVER */

#define	NOBUF	512			/* Output buffer size.		*/

static char	obuf[NOBUF];		/* Output buffer.		*/
static int	nobuf;			/* # of chars in output buffer	*/

#ifdef TERMIOS_TTY_DRIVER
static struct	termios	ot, nt;
#endif

#ifdef TERMIO_TTY_DRIVER
static struct termio ot, nt;
#endif

#ifdef BSD_TTY_DRIVER
static struct _bsd_termios ot, nt;
static struct tchars newtchars = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
static struct ltchars newltchars = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

/*
 * Flags telling what parts of the BSD terminal driver structures have been
 * set by ce and need to be reset before exit.  The current set of modes
 * that has been changed is in MODESET.
 */
#define M_SGSET		0x1
#define M_TCSET		0x2
#define M_LTCSET	0x4
#define sgset		(modeset & M_SGSET)
#define tcset		(modeset & M_TCSET)
#define ltcset		(modeset & M_LTCSET)

static short	modeset;
#endif

static int	ttactive = FALSE;
static int	ttsaved = FALSE;

#ifdef TIOCGWINSZ
static struct 	winsize win;
#endif

/*
 * Resize the screen on receipt of a SIGWINCH, assuming that the screen has
 * actually changed its size.  (Window systems have been known to send 
 * spurious resize notices).
 */
#ifdef SIGWINCH
RETSIGTYPE
winresize(s)
int	s;
{
	refresh(TRUE, 1);
}
#endif

#ifdef BSD_TTY_DRIVER

#define TCSADRAIN 0
#define TCSAFLUSH 1
#define TCSANOW 2

int
tcgetattr (fd, tiop)
int	fd;
struct _bsd_termios *tiop;
{
	if (ioctl(fd, TIOCGETP, (caddr_t) &(tiop->_bsd_sgtty)) < 0) {
		mlwrite("ttopen can't get sgtty");
		return (-1);
	}
	if (ioctl(fd, TIOCGETC, (caddr_t) &(tiop->_bsd_tchars)) < 0) {
		mlwrite("ttopen can't get special chars");
		return (-1);
	}
	if (ioctl(fd, TIOCGLTC, (caddr_t) &(tiop->_bsd_ltchars)) < 0) {
		mlwrite("ttopen can't get local chars");
		return (-1);
	}
	return (0);
}

/*
 * If TIOP points to NT, we want to change the terminal parameters
 * unconditionally and set bits in MODESET.  If it points to OT, we
 * want to reset whatever MODESET says has been changed and unset
 * bits in MODESET.
 */
int
tcsetattr (fd, opt, tiop)
int	fd, opt;
struct _bsd_termios *tiop;
{
	if ((tiop == &nt || sgset) && (ioctl(fd, TIOCSETP, (caddr_t) &(tiop->_bsd_sgtty)) < 0)) {
		mlwrite("ttopen can't set sgtty");
		return (-1);
	}
	if (tiop == &nt)
		modeset |= M_SGSET;
	else
		modeset &= ~M_SGSET;
	if ((tiop == &nt || tcset) && (ioctl(fd, TIOCSETC, (caddr_t) &(tiop->_bsd_tchars)) < 0)) {
		mlwrite("ttopen can't set special chars");
		return (-1);
	}
	if (tiop == &nt)
		modeset |= M_TCSET;
	else
		modeset &= ~M_TCSET;
	if ((tiop == &nt || ltcset) && (ioctl(fd, TIOCSLTC, (caddr_t) &(tiop->_bsd_ltchars)) < 0)) {
		mlwrite("ttopen can't set local chars");
		return (-1);
	}
	if (tiop == &nt)
		modeset |= M_LTCSET;
	else
		modeset &= ~M_LTCSET;
	modeset |= M_LTCSET;
	return (0);
}

static void
newtty(tiop)
struct _bsd_termios *tiop;
{
	extern short ospeed;

	tiop->_bsd_sgtty.sg_flags &= ~(ECHO|CRMOD);	/* Kill echo, CR->NL */
	tiop->_bsd_sgtty.sg_flags |= RAW|ANYP;		/* raw mode, 8-bit path */

	ospeed = tiop->_bsd_sgtty.sg_ospeed;

	tiop->_bsd_tchars = newtchars;
	tiop->_bsd_ltchars = newltchars;
}
#endif

#if defined (TERMIOS_TTY_DRIVER) || defined (TERMIO_TTY_DRIVER)
static void
newtty(tiop)
#if defined (TERMIOS_TTY_DRIVER)
struct termios *tiop;
#else
struct termio *tiop;
#endif
{
	tiop->c_cc[VMIN] = 1;
	tiop->c_cc[VTIME] = 0;

#ifdef VLNEXT
	tiop->c_cc[VLNEXT] = _POSIX_VDISABLE;	/* Just in case */
#endif
	tiop->c_iflag |= IGNBRK;
	tiop->c_iflag &= ~(ICRNL | INLCR | ISTRIP | IXON | IXOFF);
	tiop->c_oflag &= ~OPOST;
	tiop->c_cflag |= CS8;
	tiop->c_cflag &= ~PARENB;
	tiop->c_lflag &= ~(ECHO | ICANON | ISIG);
}
#endif /* TERMIOS_TTY_DRIVER || TERMIO_TTY_DRIVER */
	
/*
 * This function gets called once, to set up the terminal channel.  
 * Use 8-bit RAW mode, and set all special characters to 0xFF.
 */
int
ttraw()
{
	if (ttactive)
		return TRUE;

	if (ttsaved == FALSE) {
		if (tcgetattr(0, &ot) < 0) {
			mlwrite("ttraw can't get initial settings!");
			return(FALSE);
		}
		nt = ot;
		newtty(&nt);
		ttsaved++;
	}
	if (tcsetattr(0, TCSADRAIN, &nt) < 0) {	/* initialized above */
		mlwrite("ttraw can't set terminal!");
		return(FALSE);
	}

	ttactive = TRUE;

	ttsetsize();
#ifdef SIGWINCH
	(void) signal(SIGWINCH, winresize);
#endif
	return(TRUE);
}

int
ttopen()
{
	if (ttraw() == FALSE) 
		panic("Terminal Initialization Failure!");
	return (TRUE);
}

/*
 * This function gets called just before we go back home to the shell. 
 * Put all of the terminal parameters back.
 */
int
ttcooked()
{
	ttflush();
	if (ttsaved == 0 || ttactive == 0)
		return(TRUE);
	if (tcsetattr(0, TCSADRAIN, &ot) < 0) {
		mlwrite("ttcooked can't reset terminal!");
		return(FALSE);
	}
	ttactive = FALSE;
	return(TRUE);
}

int
ttclose()
{
	if (ttcooked() == FALSE) 
		panic("");
	return TRUE;
}

/*
 * Write character to the display.
 * Characters are buffered up, to make things a little bit more efficient.
 */
int
ttputc(c)
{
	if (nobuf >= NOBUF)
		ttflush();
	obuf[nobuf++] = c;
	return c;
}

/*
 * Flush output.
 */
int
ttflush()
{
	int	r;

	if (nobuf != 0) {
		r = write(1, obuf, nobuf);
		nobuf = 0;
		return r;
	}
	return 0;
}

/*
 * Read character from terminal.
 * All 8 bits are returned, so that you can use a multi-national terminal.
 */
int
ttgetc()
{
	char	buf[1];

	while (read(0, &buf[0], 1) != 1)
		;
	return (buf[0] & 0xFF);
}

/*
 * Set the number of rows and columns on the terminal via the ioctl 
 * provided by 4.3 BSD (and adopted by SunOS, Ultrix, etc.).  If that
 * fails, or is not defined, we try for $LINES and $COLUMNS.  If all
 * fails, we fall back to the termcap entry (though that's usually
 * wrong for xterms).  The number of rows is set to the window size - 1
 * to leave room for the message line, which has its own set of routines.
 */
void
ttsetsize()
{
#ifdef TIOCGWINSZ
	term.t_nrow = term.t_ncol = -1;
	if (ioctl(0, TIOCGWINSZ, (caddr_t)&win) == 0) {
		term.t_nrow = win.ws_row;
		term.t_ncol = win.ws_col;
	}
#else
	char	*lines, *cols;

	term.t_nrow = term.t_ncol = -1;
	lines = getenv("LINES");
	cols = getenv("COLUMNS");
	if (lines)
		term.t_nrow = atoi(lines);
	if (cols)
		term.t_ncol = atoi(cols);
#endif /* TIOCGWINSZ */

	if (term.t_nrow <= 0)
		term.t_nrow = tgetnum("li");
	if (term.t_ncol <= 0)
		term.t_ncol = tgetnum("co");

	if (term.t_nrow <= 0)
		term.t_nrow = 23;
	else
		term.t_nrow--;
	if (term.t_ncol <= 0)
		term.t_ncol = 80;
}

void
panic(s)
char *s;
{
	if (ttsaved && ttactive)
		ttcooked();
	xpanic(s);
}

int
typeahead()
{
#ifdef FIONREAD
	int x;

	return ((ioctl(0, FIONREAD, (caddr_t)&x) < 0) ? 0 : x);
#else
	return 0;	/* if we can't check, there's never any typeahead */
#endif /* FIONREAD */
}

int
cansusp()
{
#ifdef JOBS
#  ifdef TERMIOS_TTY_DRIVER
#    ifdef VSUSP
	return(nt.c_cc[VSUSP] != _POSIX_VDISABLE);
#    else
	return(0);
#    endif /* VSUSP */
#  endif /* TERMIOS */
#  ifdef TERMIO_TTY_DRIVER
	return (1);
#  endif /* TERMIO */
#  ifdef BSD_TTY_DRIVER
	return ((ot._bsd_ltchars.t_suspc & 0377) != 0377);
#  endif /* BSD */
#else
	return(0);
#endif /* JOBS */
}
