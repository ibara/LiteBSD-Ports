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
 *  help.c -- help functions
 *
 *  This file is for functions having to do with help commands
 */

#include <stdio.h>
#include "celibc.h"
#include "ce.h"

/*
 * Forward declarations
 */
extern void cmdstr();

/*
 *  Describe the command for a given key
 */
/* ARGSUSED */
int
deskey(f, n)
int	f, n;
{
	register int	c;	/* command character to describe */
	register char	*ptr;	/* string pointer to scan output strings */
	register KEYTAB *ktp;	/* pointer into the command table */
	register int 	found;	/* matched command flag */
	register NBIND	*nptr;	/* pointer into the name binding table */
	char		outseq[80];	/* output buffer for command sequence */

	/* prompt the user to type us a key to describe */
	if (kbdmip != NULL)
		return(FALSE);
	mlwrite("Describe Key: ");
	/* get the command sequence to describe */
	c = getckey();
	/* change it to something we can print as well */
	cmdstr(c, &outseq[0]);
	/* and dump it out */
	ptr = &outseq[0];
	while (*ptr)
		(*term.t_putchar)(*ptr++);
	(*term.t_putchar)(' ');		/* space it out */
	/* find the right ->function */
	ktp = &keytab[0];
	found = FALSE;
	while (ktp->k_fp != NULL) {
		if (ktp->k_code == c) {
			found = TRUE;
			break;
		}
		++ktp;
	}
	if (!found)
		strcpy(outseq, "Key Not Bound");
	else {
		/* match it against the name binding table */
		nptr = &names[0];
		strcpy(outseq, "[Bad binding]");
		while (nptr->n_func != NULL) {
			if (nptr->n_func == ktp->k_fp) {
				strcpy(outseq, nptr->n_name);
				break;
			}
			++nptr;
		}
	}
	/* output the command sequence */
	ptr = &outseq[0];
	while (*ptr)
		(*term.t_putchar)(*ptr++);
	return TRUE;
}

/* 
 * Change a key command to a string we can print out 
 */
void
cmdstr(c, seq)	
int c;		/* sequence to translate */
char *seq;	/* destination string for sequence */
{
	char *ptr;	/* pointer into current position in sequence */

	ptr = seq;
	/* apply meta sequence if needed */
	if (c & META) {
		*ptr++ = 'M';
		*ptr++ = '-';
	}
	/* apply ^X sequence if needed */
	if (c & CTLX) {
		*ptr++ = 'C';
		*ptr++ = '-';
		*ptr++ = 'x';
		*ptr++ = ' ';		/* a space looks nicer */
	}
	/* apply control sequence if needed */
	if (c & CNTL) {
		*ptr++ = 'C';
		*ptr++ = '-';
	}
	c &= 0xFF;	/* strip off all but 8 bits */
	/* and output the final sequence */
	*ptr++ = c;
	*ptr = 0;	/* terminate the string */
}

/*
 *  HELP!  Bring up a fake buffer and read the help file into it
 *  in View mode.
 */
/* ARGSUSED */
int
help(f, n)
int	f, n;
{
	register int s;         /* status of I/O operations */
	register BUFFER *bp;    /* buffer to hold the help file */
	char fname[NSTRING];	/* buffer to construct file name in */
	char *line;		/* buffer to hold lines from help file */
	int  llen;		/* length of line from help file */

	bp = bfind("*help*", TRUE, 0);
	if ((bp == NULL) || (bclear(bp) == FALSE)) {
		(*term.t_beep)();
		mlwrite("Cannot display help file");
		return(FALSE);
	}
	strcpy(fname, LIBDIR);		/* LIBDIR defined in ce.h */
	strcat(fname, HELPFILE);
	s = ffropen(fname);
	if (s == FIOFNF) {
	        (*term.t_beep)();
		mlwrite("Help file is not online");
		return(FALSE);
	}
	while ((s = ffgetline(&line, &llen)) != FIOERR) {
		if ((s == FIOEOF) && (llen == 0)) {
			if (line)
				free(line);
			break;
		}
		(void) addline(bp, line, llen);
		free(line);
		if (s == FIOEOF)
			break;
	}
	s = ffclose();
	if (s != FIOSUC) {
		mlwrite("Error closing help file");
		return(FALSE);
	}
	/* make this buffer in VIEW mode */
	bp->b_mode |= MDVIEW;
	s = popbuftoscreen(bp);
	update();
	return s;
}

/*
 * Describe bindings.  Bring up a fake buffer and list the key bindings
 * into it in view mode.  Lets ce produce its own wall chart.
 *
 * This was redone to be more general, using mg2a as a guide.  Look in
 * buffer.c for popbuftoscreen(), and follow the function chain around
 * in buffer.c and window.c
 */
/* ARGSUSED */
int
wallchart(f, n)
int	f, n;
{
	register KEYTAB *ktp;	/* pointer into the command table */
	register NBIND *nptr;	/* pointer into the name binding table */
	register BUFFER *bp;	/* buffer to put binding list into */
	int cpos;		/* current position to use in outseq */
	char outseq[80];	/* output buffer for keystroke sequence */
	int  s;

	bp = bfind("*help*", TRUE, 0);
	if (bp == NULL || bclear(bp) == FALSE) {
	        (*term.t_beep)();
		mlwrite("Can not display binding list");
		return(FALSE);
	}
	/* let us know this is in progress */
	mlwrite("Building bindings list...");
	/* build the contents of this window, inserting it line by line */
	nptr = &names[0];
	while (nptr->n_func != NULL) {
		/* add in the command name */
		strcpy(outseq, nptr->n_name);
		cpos = strlen(outseq);
		/* search down any keys bound to this */
		ktp = &keytab[0];
		while (ktp->k_fp != NULL) {
			if (ktp->k_fp == nptr->n_func) {
				/* padd out some spaces */
				while (cpos < 25)
					outseq[cpos++] = ' ';

				/* add in the command sequence */
				cmdstr(ktp->k_code, &outseq[cpos]);
				while (outseq[cpos] != 0)
					++cpos;
				/* and add it as a line into the buffer */
				(void) addline(bp, outseq, cpos);
				cpos = 0;	/* and clear the line */
			}
			++ktp;
		}
		/* if no key was bound, we need to dump it anyway */
		if (cpos > 0) {
			outseq[cpos] = 0;
			(void) addline(bp, outseq, cpos);
		}
		/* and on to the next name */
		++nptr;
	}
	mlwrite("Done");	/* signal list completion */
	s = popbuftoscreen(bp);
	update();
	return s;
}

/*
 * Display the ce tutorial (taken almost verbatim from the emacs tutorial) in a
 * window.
 */
/* ARGSUSED */
int
intro(f, n)
int	f, n;
{

	register int 	s;
	char		fname[NFILEN];

	strcpy(fname, LIBDIR);
	strcat(fname, TUTORIAL);
	if ((s = getfile(fname)) != TRUE) 
		return s;
	strcpy(curbp->b_bname, "*TUTORIAL*");
	strcpy(curbp->b_fname, "TUTORIAL");
	return (TRUE);
}

/*
 * Display the list of ce functions in a read-only window.
 */
/* ARGSUSED */
int
showfuncs(f, n)
int	f, n;
{

	register int 	s;
	char		fname[NFILEN];

	strcpy(fname, LIBDIR);
	strcat(fname, FUNCTIONS);
	if ((s = getfile(fname)) != TRUE) 
		return s;
	strcpy(curbp->b_bname, "*FUNCTIONS*");
	strcpy(curbp->b_fname, "FUNCTIONS");
	curbp->b_mode |= MDVIEW;
	return (TRUE);
}
