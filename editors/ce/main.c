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
 * main.c -- driver for Chet's Emacs
 *
 * This program is in public domain; originally written by Dave G. Conroy.
 * This file contains the main driving routine, and some keyboard processing
 * code, for the MicroEMACS screen editor.
 */

#include <stdio.h>
#include "celibc.h"

#define MAIN
#include	"ce.h"		/* global structures and defines */
#undef  MAIN

#ifndef GOOD
#define GOOD    0
#endif

/* initialized global definitions */

int     fillcol = 75;			/* Current fill column		*/
short   kbdm[NKBDM] = {CTLX|')'};	/* Macro			*/
char    pat[NPAT];			/* Search pattern		*/
char	sarg[NSTRING] = "";		/* string argument for line exec*/
char	*modename[] = {			/* name of modes		*/
	    "View", "Fill", "Save", "C", "Match"};
char	modecode[] = "VFSCM";		/* letters to represent modes	*/
int	nummodes = (sizeof(modename) / sizeof(modename[0]));
int	gmode = MDAUTOSAVE;		/* global editor mode		*/
int     sgarbf  = TRUE;			/* TRUE if screen is garbage	*/
int     mpresf  = FALSE;		/* TRUE if message in last line */
int	clexec	= FALSE;		/* command line execution flag	*/
int	doingbackups = TRUE;		/* Are we doing backups?	*/
int	onxterm = FALSE;		/* Are we running under X11?	*/
int	explarg = FALSE;		/* Was an explicit argument given? */
int	curchar = 0;			/* the last character read	*/
int	followlinks = 0;		/* follow symlinks when writing */

/* uninitialized global declarations */

int     thisflag;		/* Flags, this command	*/
int     lastflag;		/* Flags, last command	*/
int     curgoal;		/* Goal for C-P, C-N	*/
WINDOW  *curwp;			/* Current window	*/
BUFFER  *curbp;			/* Current buffer	*/
WINDOW  *wheadp;		/* Head of list of windows	*/
BUFFER  *bheadp;		/* Head of list of buffers	*/
BUFFER  *blistp;		/* Buffer for C-X C-B	*/
short   *kbdmip;		/* Input pointer for above	*/
short   *kbdmop;		/* Output pointer for above	*/

BUFFER  *bfind();		/* Lookup a buffer by name	*/
WINDOW  *wpopup();		/* Pop up window creation	*/
LINE    *lalloc();		/* Allocate a line	*/

char	*progname;

/*
 * Forward declarations
 */
static void	edinit();

extern void	usage();

/* external references */

extern int	isofont;

extern void	rmsave();

/*
 * Declarations local to this file.
 */

static int	nautosave = 256;
static int	cmdcount  = 256;

/*
 *  Do argument processing.  Normally bound to "C-U".
 */
/*ARGSUSED*/
int
getarg(f, n)
int	f, n;
{
	register int c;
	int	ff, nn, mflag, s;
	char	cmd[32];

	ff = TRUE;
	nn = 4;			 /* with argument of 4 */
	mflag = 0;		      /* that can be discarded. */
	mlwrite("C-u 4");
	while (((c = getkey()) >= '0' && c <= '9') || c == (CNTL|'U') || c == '-') {
		if (c == (CNTL|'U'))
			nn *= 4;
		/*
		 * If dash, and start of argument string, set arg.
		 * to -1.  Otherwise, insert it.
	 	 */
		else if (c == '-') {
	   		if (mflag)
				break;
			nn = 0;
			mflag = -1;
		}
		/*
		 * If first digit entered, replace previous argument
		 * with digit and set sign.  Otherwise, append to arg.
		 */
		else {
			if (!mflag) {
				nn = 0;
				mflag = 1;
	    		}
			nn = 10*nn + c - '0';
		}
		mlwrite("C-u %d ", (mflag >= 0) ? nn : (nn ? -nn : -1));
	}
	/*
	 * Make arguments preceded by a minus sign negative and change
	 * the special argument "^U -" to an effective "^U -1".
	 */
	if (mflag == -1) {
		if (nn == 0)
			nn++;
		nn = -nn;
	}
	if (kbdmip != NULL) {
		*kbdmip++ = (CNTL | 'U');
		*kbdmip++ = nn;
		s = TRUE;
	} else {
		if (c == (CNTL|'X'))
			c = CTLX | (getctl());
		cmdstr(c, cmd);
		mlwrite("C-u %d %s", nn, cmd);
		explarg = TRUE;
		s = execute(c, ff, nn);
		explarg = FALSE;
	}
	return s;
}

/*
 * Set up to process commands
 */
void
cesetup()
{
	lastflag = 0;			/* Fake last flags      */
	curbp->b_mode |= gmode;		/* and set default modes*/
	curwp->w_flag |= WFMODE;	/* and force an update  */
}

/* 
 * Initialize the editor and process the startup file, if one exists
 */
void
ceinit()
{
	char bname[NBUFN];	/* buffer name of file to read */

	vtinit();		/* Displays.		     */
	dirinit();		/* Get current working directory */
	fioinit();		/* Initialize file i/o		 */
	(void) startup();	/* execute .ceinit if there      */
	strcpy(bname, "*scratch*");	/* default buffer name   */
	edinit(bname);		/* Buffers, windows.	     */
#if 0
	meminit();		/* Malloc and friends		 */
#endif
	update();		/* let the user know we are here */
	showversion(FALSE, 1);  /* gratuitous ego-booster	*/
#ifndef X11
	isofont += onxterm;
#endif
}

/*
 * The body of the ce processing loop.
 */
void
cebody()
{
	register int    c;
	register int    f;
	register int    n;
	register int    mflag;
	int basec;			/* c stripped of meta character */
	
	for (;;) {
loop:
		update();			       /* Fix up the screen    */
		c = getkey();
		if (mpresf != FALSE) {
			mlerase();
			update();
		}
		f = explarg = FALSE;
		n = 1;
		/* do META-# processing if needed */
		basec = c & ~META;		/* strip meta char off if there */
		if ((c & META) && ((ISDIGIT(basec)) || basec == '-')) {
			f = TRUE;		/* there is a # arg */
			n = 0;			/* start with a zero default */
			mflag = 1;		/* current minus flag */
			c = basec;		/* strip the META */
			while (ISDIGIT(c) || (c == '-')) {
				if (c == '-') {
					/* already hit a minus or digit? */
					if ((mflag == -1) || (n != 0))
						break;
					mflag = -1;
				} else
					n = n * 10 + (c - '0');
				if ((n == 0) && (mflag == -1))	/* lonely - */
					mlwrite("Arg:");
				else
					mlwrite("Arg: %d",n * mflag);
				c = getkey();	/* get the next key */
			}
			n = n * mflag;	/* figure in the sign */
			explarg = TRUE;
		}
		if (c == (CNTL|'X'))		/* ^X is a prefix       */
			c = CTLX | (getctl());
		if (kbdmip != NULL) {		/* Save macro strokes.  */
			if (c!=(CTLX|')') && kbdmip>&kbdm[NKBDM-6]) {
				mlwrite("Keyboard macro too long");
				ctrlg(FALSE, 0);
				goto loop;
			}
			*kbdmip++ = c;
		}
		execute(c, f, n);		/* Do it.	       */
	}
}

/* 
 * scan through the command line and get the files to edit
 */
void
ceargs(argc, argv)
int	argc;
char	**argv;
{
	register int 	carg;
	register BUFFER *bp;
	int 		ffile = TRUE;
	char		bname[NBUFN];
	int 		n, goline = FALSE, rdonly = FALSE;

	bname[0] = '\0';
	for (carg = 1; carg < argc; ++carg) {
		/* if it's a switch, process it */
		if (argv[carg][0] == '+') { 	/* line number to jump to */
			n = atoi(&(argv[carg][1]));
			goline = TRUE;
		} else if (argv[carg][0] == '-') {
			switch(argv[carg][1]) {
			case 'r':
				rdonly = TRUE;
				gmode |= MDVIEW;
				break;
			case 'b':
				carg++;
				if (carg < argc) {	/* specifying a buffer name */
					strncpy(bname, argv[carg], NBUFN-1);
					bname[NBUFN-1] = '\0';
				}
				break;
			case '8':
				isofont++;
				break;
#ifdef X11
			/* 
			 * -d, -D, -f, -F, -g, -G, and -R are taken care
			 * of by x_preparse_args
			 */
			case 'd':
			case 'D':
			case 'f':
			case 'F':
			case 'g':
			case 'G':
				carg++;
			case 'R':
				break;
#endif
			default:
				usage();
				break;
			}
		} else {			/* process a file name */
			if (bname[0] == '\0')
				makename(bname, argv[carg]);
			if (ffile) { 	/* if first file, read it in */
				bp = curbp;
				strcpy(bp->b_bname, bname);

				strncpy(bp->b_fname, argv[carg], NFILEN-1);
				bp->b_fname[NFILEN-1] = '\0';

				if (readin(argv[carg]) == ABORT) {
					strcpy(bp->b_bname, "*scratch*");
					bp->b_fname[0] = '\0';
				}
				bp->b_dotp = bp->b_linep;
				bp->b_doto = 0;
				if (rdonly)
					bp->b_mode |= MDVIEW;
				ffile = FALSE;
			} else {	/* set this to inactive */
				bp = bfind(bname, TRUE, 0);
				strncpy(bp->b_fname, argv[carg], NFILEN-1);
				bp->b_fname[NFILEN-1] = '\0';
				
				bp->b_active = FALSE;
				if (rdonly)
					bp->b_mode |= MDVIEW;
			}
			bname[0] = '\0';
		}
	}
	upmode();
	if (goline)
		gotoline(TRUE, n);
	else
		update();   /* gotoline forces its own update; this saves one */
}

/*
 * Driver for ce
 */
main(argc, argv)
int     argc;
char    **argv;
{
	if (progname = strrchr(argv[0], '/'))
		progname++;
	else
		progname = argv[0];

#ifdef X11
	x_preparse_args(argc, argv);
#endif

	ceinit();
	ceargs(argc, argv);	/* do the command line arguments */
	cesetup();		/* set up to process commands    */
	cebody();
	exit(1);		/* shouldn't get here. */
}

/*
 * Initialize all of the buffers and windows. The buffer name is passed down
 * as an argument, because the main routine may have been told to read in a
 * file by default, and we want the buffer name to be right.
 */
static void
edinit(bname)
char    bname[];
{
	register BUFFER *bp;
	register WINDOW *wp;

	bp = bfind(bname, TRUE, 0);	     /* First buffer	 */
	blistp = bfind("*List*", TRUE, BFTEMP); /* Buffer list buffer   */
	wp = (WINDOW *) malloc(sizeof(WINDOW)); /* First window	 */
	if (bp == NULL || wp == NULL || blistp == NULL)
		exit(3);
	curbp  = bp;			/* Make this current */
	wheadp = wp;
	curwp  = wp;
	wp->w_wndp  = NULL;		/* Initialize window */
	wp->w_bufp  = bp;
	bp->b_nwnd  = 1;		/* Displayed */
	wp->w_linep = bp->b_linep;
	wp->w_dotp  = bp->b_linep;
	wp->w_doto  = 0;
	wp->w_markp = NULL;
	wp->w_marko = 0;
	wp->w_toprow = 0;
	wp->w_ntrows = term.t_nrow - 1;	/* "-1" for mode line. */
	wp->w_force = 0;
	wp->w_flag  = WFMODE|WFHARD;	/* Full */
}

/*
 * This is the general command execution routine. It handles the fake binding
 * of all the keys to "self-insert". It also clears out the "thisflag" word,
 * and arranges to move it to the "lastflag", so that the next command can
 * look at it. Return the status of command.
 */
#define selfinserting(c)  (((c) >= 0x20 && (c) <= 0x7E) || ((c) >= 0xA0 && (c) <= 0xFE))

#define shouldwrap(n, c) \
	(ISWHSP(c) && (curwp->w_bufp->b_mode & MDWRAP) && fillcol > 0 && \
	 n >= 0 && getccol(FALSE) > fillcol && \
	 (curwp->w_bufp->b_mode & MDVIEW) == FALSE)

int
selfinsert(n, c)
int	n, c;
{
	int	status;

	/*
	 * If white space was typed, fill column is defined, the argument is
	 * non-negative, wrap mode is enabled, and we are now past fill column,
	 * and we are not read-only, perform word wrap.
	 */
	if (shouldwrap(n, c))
		wrapword();
	if (n <= 0) {		/* Fenceposts */
		lastflag = 0;
		return (n == 0);	/* n < 0 ? FALSE : TRUE */
	}
	thisflag = 0;		/* For the future */
	if (curbp->b_mode & MDCMODE) {
		if (c == '}')
			status = insbrace(n, c);
		else if (c == '#') 
			status = inspound(n, c);
		else
			status = linsert(n, c);
		if (c == '}' || c == ')' || c == ']')
			balance(c);
	} else if (curbp->b_mode & MDPMATCH) {
		status = linsert(n, c);
		if (c == '}' || c == ')' || c == ']')
			balance(c);
	} else
		status = linsert(n, c);
	lastflag = thisflag;
	if ((curbp->b_mode & MDAUTOSAVE) && (--cmdcount == 0)) {
	    (void) fautosave();		/* maybe check errors later */
	    cmdcount = nautosave;
	}
	return (status);
}

int
execute(c, f, n)
int	c, f, n;
{
	register KEYTAB *ktp;
	register int    status;

	if (c < 0)
		c = (unsigned char) c;

	curchar = c & 0xFF;

	if (c & CNTL)
		curchar = CCHR(curchar);	/* undo transformation	*/

	ktp = &keytab[0];			/* Look in key table.	*/
	while (ktp->k_fp != NULL) {
		if (ktp->k_code == c) {
			thisflag = 0;
			if (ktp->k_fp == selfinsert)
				status = selfinsert(n, c);
			else
				status = (*ktp->k_fp)(f, n);
			lastflag = thisflag;
			return (status);
		}
		++ktp;
	}
	if (selfinserting(c))		/* Self inserting.	*/
		return (selfinsert(n, c));
	(*term.t_beep)();
	mlwrite("Key not bound");		/* complain		*/
	lastflag = 0;			   /* Fake last flags.     */
	return (FALSE);
}

/*
 * Read in a key.
 * Do the standard keyboard preprocessing. Convert the keys to the internal
 * character set.
 */
int
getkey()
{
	int    c;

	c = (*term.t_getchar)();
	if (c == METACH) {		/* Apply M- prefix */
		c = getctl();
		return (META | c);
	}
	if (c >= 0x0 && c <= 0x1F)	/* C0 control -> C- */
		c = CNTL | (c + '@');
	return (c);
}

/*
 * Get a key.
 * Apply control modifications to the read key.
 */
int
getctl()
{
	register int    c;

	c = (*term.t_getchar)();
	if (ISLOWER(c) != FALSE)	/* Force to upper       */
		c = TOUPPER(c);
	if (c >= 0x0 && c <= 0x1F)	/* C0 control -> C-     */
		c = CNTL | (c+'@');
	return (c);
}

/*
 * Fancy quit command, as implemented by Norm. If the any buffer has
 * changed do a write on that buffer and exit emacs, otherwise simply exit.
 */
int
quickexit(f, n)
int	f, n;
{
	register BUFFER *bp;	/* scanning pointer to buffers */

	bp = bheadp;
	while (bp != NULL) {
		if ((bp->b_flag&BFCHG) != 0	/* Changed. */
		&& (bp->b_flag&BFTEMP) == 0) {	/* Real. */
			curbp = bp;		/* make that buffer cur	*/
			mlwrite("[Saving %s]", bp->b_fname);
			filesave(f, n);
		}
		rmsave(bp->b_fname);
		bp = bp->b_bufp;		/* on to the next buffer */
	}
	return (quit(f, n));			/* conditionally quit */
}

/*
 * Save changed buffers, querying about each one.  Bound to "C-X S"
 */
int
savechanged(f, n)
int	f, n;
{
	register BUFFER *bp, *savecbp;
	char rbuf[NSTRING];

	bp = bheadp;
	while (bp != NULL) {     /* changed and not scratch */
		if ((bp->b_flag&BFTEMP) == 0 && (bp->b_flag&BFCHG) != 0) {
			(void) sprintf(rbuf,"Buffer %s modified. Save",bp->b_bname);
			if (mlyesno(rbuf) == TRUE) {
				savecbp = curbp;
				curbp = bp;
				filesave(f,n);
				curbp = savecbp;
			}
		}
		rmsave(bp->b_fname);
		bp = bp->b_bufp;
	}
	return TRUE;
}

/*
 * Save changed buffers, querying about each one, and exit ce.  Bound to 
 * "C-X C-C".
 */
int
saveandexit(f,n)
int	f, n;
{
	savechanged(f, n);
	return (quit(f, n));
}


/*
 * Quit command. If an argument, always quit. Otherwise confirm if a buffer
 * has been changed and not written out. Used by both quickexit and 
 * saveandexit.
 */
/*ARGSUSED*/
int
quit(f, n)
int	f, n;
{
	register int    s = TRUE;

	if (f != FALSE			  /* Argument forces it.  */
	|| anycb() == FALSE		     /* All buffers clean.   */
						/* User says it's OK.   */
	|| (s = mlyesno("Modified buffers exist. Leave anyway")) == TRUE) {
		vttidy();
		exit(GOOD);
	}
	mlerase();
	return (s);
}

/*
 * Begin a keyboard macro.
 * Error if not at the top level in keyboard processing. Set up variables and
 * return.
 */
/*ARGSUSED*/
int
ctlxlp(f, n)
int	f, n;
{
	if (kbdmip != NULL || kbdmop != NULL) {
		mlwrite("No Recursive Macros!");
		return (FALSE);
	}
	mlwrite("Defining Keyboard Macro");
	kbdmip = &kbdm[0];
	return (TRUE);
}

/*
 * End keyboard macro. Check for the same limit conditions as the above
 * routine. Set up the variables and return to the caller.
 */
/*ARGSUSED*/
int
ctlxrp(f, n)
int	f, n;
{
	if (kbdmip == NULL) {
		mlwrite("Not Defining Keyboard Macro");
		return (FALSE);
	}
	mlwrite("Keyboard Macro Defined ");
	kbdmip = (short *) NULL;
	return (TRUE);
}

/*
 * Execute a macro.
 * The command argument is the number of times to loop. Quit as soon as a
 * command gets an error. Return TRUE if all ok, else FALSE.
 */
/* ARGSUSED */
int
ctlxe(f, n)
int	f, n;
{
	register int    c;
	register int    af;
	register int    an;
	register int    s;

	if (kbdmip != NULL || kbdmop != NULL) {
		mlwrite("No Recursive Macros!");
		return (FALSE);
	}
	if (n <= 0)
		return (TRUE);
	do {
		kbdmop = &kbdm[0];
		do {
			af = FALSE;
			an = 1;
			if ((c = *kbdmop++) == (CNTL|'U')) {
				af = TRUE;
				an = *kbdmop++;
				c  = *kbdmop++;
			}
			s = TRUE;
		} while (c != (CTLX|')') && (s = execute(c, af, an)) == TRUE);
		kbdmop = NULL;
	} while (s == TRUE && --n);
	return (s);
}

/*
 * Abort.
 * Beep the beeper. Kill off any keyboard macro, etc., that is in progress.
 * Sometimes called as a routine, to do general aborting of stuff.
 */
/*ARGSUSED*/
int
ctrlg(f, n)
int	f, n;
{
	(*term.t_beep)();
	if (kbdmip != NULL) {
		kbdm[0] = (CTLX|')');
		kbdmip  = NULL;
	}
	mlwrite("Quit");
	return (ABORT);
}

/* 
 * Tell the user that this command is illegal while we are in
 * VIEW (read-only) mode
 */
int
rdonly()
{
	(*term.t_beep)();
	mlwrite("Buffer is read-only");
	return(FALSE);
}

#ifdef X11
#  define FLAGS "rbdfR"
#else
#  define FLAGS "rb"
#endif

void
usage()
{
	(*term.t_close)();
	fprintf(stderr, "usage: %s -%s file [file ...]\n", progname, FLAGS);
	fprintf(stderr, "	-r for read-only\n");
	fprintf(stderr, "	-b buffername to set buffer name\n");
#ifdef X11
	fprintf(stderr, "	-f fontname to set X font\n");
	fprintf(stderr, "	-d displayname to change default X display\n");
	fprintf(stderr, "	-g geometry to set the window geometry\n");
	fprintf(stderr, "	-R to set reverse video\n");
#endif
	exit(2);
}
