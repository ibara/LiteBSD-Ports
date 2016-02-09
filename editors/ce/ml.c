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
 * ml.c -- functions to handle the message line
 *
 * The functions in this file handle the displaying of messages and the
 * return of user input from the message line.  Used by the entire known
 * universe.
 */

#include <stdio.h>
#include "celibc.h"
#include "ce.h"

extern int ttrow;
extern int ttcol;

#ifdef PREFER_STDARG
void mlwrite(const char *, ...);
#else
void mlwrite();
#endif

/*
 * Erase the message line. This is a special routine because the message line
 * is not considered to be part of the virtual screen. It always works
 * immediately; the terminal buffer is flushed via a call to the flusher.
 */
void
mlerase()
{
	movecursor(term.t_nrow, 0);
	(*term.t_eeol)();
	(*term.t_flush)();
	mpresf = FALSE;
}

/*
 * Output a character to the message line, doing control-character translation
 * as necessary.
 */
void
mlputc(c)
int	c;
{
	if (ttcol >= (term.t_ncol - 2))
		return;

	if (c < 0x20) {
		(*term.t_putchar)('^');
		ttcol++;
		c ^= 0x40;
	}
	(*term.t_putchar)(c);
	ttcol++;
}

/*
 * Refresh the message line from the characters in `buf', using `prompt' as 
 * the prompt.  
 */
void
mlrefresh(prompt, buf, cpos)
char	*prompt;
char	*buf;
int	cpos;
{
	register int	i;

	mlerase();
	ttcol = 0;
	mlwrite(prompt);
	for (i = 0; i < cpos; i++)
		mlputc(buf[i]);
	(*term.t_flush)();
}
	
/*
 * Ask a yes or no question in the message line. Return either TRUE, FALSE, or
 * ABORT. The ABORT status is returned if the user bumps out of the question
 * with a ^G. Used any time a confirmation is required.
 */
int
mlyesno(prompt)
char	*prompt;
{
	char c;			/* input character */
	char buf[NSTRING];	/* prompt to user */

	for (;;) {
		/* build and prompt the user */
		strncpy(buf, prompt, sizeof(buf) - 10);
		strcat(buf, " [y/n]? ");
		mlwrite(buf);

		/* get the response */
		c = (*term.t_getchar)();

		if (c == CCHR('G'))		/* Bail out! */
			return(ABORT);

		if (c=='y' || c=='Y')
			return(TRUE);

		if (c=='n' || c=='N')
			return(FALSE);

		(*term.t_beep)();
	}
}

typedef int	COMPFUNC();	/* completion function */

static COMPFUNC *compfunc = (COMPFUNC *) NULL;
static int	componspc = FALSE;	/* if TRUE, do completion on SPACE */

extern int	fcomplete();
extern int	bcomplete();
extern int	cmdcomplete();

/*
 * Get a file name from the message line, with completion.
 */
int
mlgetfile(prompt, buf, nbuf)
char *prompt;
char *buf;
{
	register int s;

	compfunc = fcomplete;
	s = mlreplyt(prompt, buf, nbuf, '\n');
	compfunc = (COMPFUNC *) NULL;
	return s;
}

/*
 * Get a buffer name from the message line, with completion.
 */
int
mlgetbuf(prompt, buf, nbuf)
char	*prompt;
char	*buf;
int	nbuf;
{
	register int s;

	compfunc = bcomplete;
	s = mlreplyt(prompt, buf, nbuf, '\n');
	compfunc = (COMPFUNC *) NULL;
	return s;
}

/* 
 * Get a command name from the command line. Command completion means
 * that pressing a <SPACE>, <TAB>, or <ESC> will attempt to complete an 
 * unfinished command name if it is unique.
 */
Function *
mlgetcmd()
{
	char		buf[NSTRING];
	Function	*fncmatch();    /* in bind.c */
	Function	*fn;
	int		s, nm;

	compfunc = cmdcomplete;
	componspc = TRUE;
	fn = (Function *) NULL;
	s = mlreplyt("M-x ", buf, sizeof(buf), '\n');
	if (s == TRUE) {
		fn = fncmatch(buf);
		if (fn == NULL) {
			nm = cmdcomplete(buf);
			if (nm == 1)
				fn = fncmatch(buf);
		}
	}
	compfunc = (COMPFUNC *) NULL;
	componspc = FALSE;
	return fn;
}

/*
 * Write a prompt into the message line, then read back a response. Keep
 * track of the physical position of the cursor. If we are in a keyboard
 * macro throw the prompt away, and return the remembered response. This
 * lets macros run at full speed. The reply is always terminated by a carriage
 * return. Handle erase, kill, and abort keys.
 */
int
mlreply(prompt, buf, nbuf)
char *prompt;
char *buf;
{
	return(mlreplyt(prompt, buf, nbuf, '\n'));
}

/* 
 * A more generalized prompt/reply function allowing the caller
 * to specify the proper terminator. If the terminator is not
 * a return ('\n') it will echo as "<NL>".  The interface presented
 * mimics the standard Berkeley Unix tty line editing interface (^U, ^W, etc.)
 */

#define erasechar()	do { \
				(*term.t_putchar)('\b');  \
				(*term.t_putchar)(' ');   \
				(*term.t_putchar)('\b');  \
				--ttcol; \
			} while (0)

#define CPLTCHAR(c)	((c) == CCHR('I') || (c) == CCHR('['))

int
mlreplyt(prompt, buf, nbuf, eolchar)
char	*prompt;
char	*buf;
char	eolchar;
{
	register int cpos;
	register int i;
	register int c;

	cpos = 0;
	if (kbdmop != NULL) {
		while (((c = *kbdmop++) != '\0') && cpos < nbuf-1)
			buf[cpos++] = c;
		buf[cpos] = 0;
		if (buf[0] == 0)
			return(FALSE);
		return(TRUE);
	}
	/* check to see if we are executing a command line */
	if (clexec) {
		(void) nxtarg(buf, nbuf);
		return(TRUE);
	}
	mlwrite(prompt);
	for (;;) {
		/* 
		 * get a character from the user. if it is a <ret>, change it
		 * to a <NL>
		 */
		c = (*term.t_getchar)();
		if (c == 0x0d)
			c = '\n';
		if (c == eolchar) {
			buf[cpos++] = 0;
			if (kbdmip != NULL) {
				if (kbdmip+cpos > &kbdm[NKBDM-3]) {
					ctrlg(FALSE, 0);
					(*term.t_flush)();
					return(ABORT);
				}
				for (i=0; i<cpos; ++i)
					*kbdmip++ = buf[i];
			}
			(*term.t_putchar)('\r');
			ttcol = 0;
			(*term.t_flush)();
			if (buf[0] == 0)
				return(FALSE);
			return(TRUE);
		} else if (c == CCHR('G')) {		/* Bell, abort */
			mlputc(CCHR('G'));
			ctrlg(FALSE, 0);
			(*term.t_flush)();
			return(ABORT);
		} else if (c == CCHR('?') || c == CCHR('H')) { /* rubout*/
			if (cpos != 0) {
				erasechar();
				if (buf[--cpos] < 0x20)
					erasechar();
				(*term.t_flush)();
			}
		} else if ((compfunc && CPLTCHAR(c)) || (componspc && c == ' ')) {
			int nmatch;
			register int j;

			if (cpos != 0) {
				buf[cpos] = '\0';
				nmatch = (*compfunc)(buf, cpos);
				if (nmatch >= 1) {
					for (j = cpos; buf[j] != '\0'; j++) {
						(*term.t_putchar)(buf[j]);
						++ttcol;
					}
					cpos = j;
				}
				if (nmatch != 1)
					(*term.t_beep)();
			} else
				(*term.t_beep)();
			(*term.t_flush)();
		} else if (c == CCHR('U')) {		/* C-U, kill */
			while (cpos != 0) {
				erasechar();
				if (buf[--cpos] < 0x20)	/* Control char */
					erasechar();
			}
			(*term.t_flush)();
		} else if (c == CCHR('W')) {		/* C-W, word erase */
			/* back up to first word char, if not in a word now */
			while (cpos != 0 && (ISWORD(buf[cpos-1]) == FALSE)) {
				erasechar();
				if (buf[--cpos] < 0x20)
					erasechar();
			}
			/* back up to first char in the word */
			while (cpos > 0 && (ISWORD(buf[cpos-1]) == TRUE)) {
				erasechar();
				if (buf[--cpos] < 0x20)
					erasechar();
			}
			(*term.t_flush)();
		} else if (c == CCHR('L') || c == CCHR('R')) {	/* C-L, refresh */
			mlrefresh(prompt, buf, cpos);
		} else if (c == CCHR('Z')) {		/* C-Z, suspend */
			mlputc(CCHR('Z'));
			spawncli();
			update();
			mlrefresh(prompt, buf, cpos);
		} else {
			if (c == CCHR('Q')) {		/* C-Q, quote next */
				mlputc(CCHR('Q'));
				c = (*term.t_getchar)();
				erasechar();
				erasechar();
				(*term.t_flush)();
			}
			if (cpos < nbuf-1) {
				buf[cpos++] = c;
				mlputc(c);
				(*term.t_flush)();
			}
		}
	}
}

/*
 * Write a message into the message line. Keep track of the physical cursor
 * position. The full range of printf like format items is handled.  Set the 
 * "message line" flag TRUE.
 *
 * The original version has been replaced with the "pwprintf" routine from 
 * the tcsh sources; it has been explicitly released to the public by its 
 * author, Paul Placeway (paul@tut.cis.ohio-state.edu)
 */

#ifdef PREFER_STDARG
#  include <stdarg.h>
#else
#  include <varargs.h>
#endif

/*VARARGS0*/
void
#ifdef PREFER_STDARG
mlwrite(const char *fp, ...)
#else
mlwrite(va_alist)
va_dcl
#endif /* !PREFER_STDARG */
{
#ifndef PREFER_STDARG
	register char *fp;
#endif
	va_list ap;

	if (clexec)
		return;
#ifdef PREFER_STDARG
	va_start(ap, fp);
#else
	va_start(ap);
	fp = va_arg(ap, char *);
#endif
	movecursor(term.t_nrow,0);
	(*term.t_color)(CTEXT);
	(void) mlprintf(fp, &ap);
	va_end(ap);
	(*term.t_eeol)();
	(*term.t_flush)();
	mpresf = TRUE;
}

/* 
 * Use varargs since it's the RIGHT WAY, and assuming things about parameters
 * on the stack will be wrong on a register passing machine (Pyramid)
 */

#define INF	32766		/* should be bigger than any field to print */

#define mlputchar(c)	((*term.t_putchar)((c)), ++ttcol)
#define CHKTTCOL()	if (ttcol >= (term.t_ncol - 2)) break

static char prbuf[128];

/*VARARGS*/
int
mlprintf (fp, ap)
register char	*fp;
va_list		*ap;
{
	register char *f, *bp;
	register long l;
	register unsigned long u;
	register int i;
	register int fmt;
	register char pad = ' ';
	int flush_left = 0, f_width = 0, prec = INF, hash = 0, do_long = 0;
	int sign = 0;

	f = fp;
	for (; *f; f++) {
		CHKTTCOL();

		if (*f != '%') {	/* then just out the char */
			mlputchar(*f);
		} else {
	    		f++;		/* skip the % */
			if (*f == '-') {	/* minus: flush left */
				flush_left = 1;
				f++;
			}
			if (*f == '0') {	/* padding with 0 rather than blank */
				pad = '0';
				f++;
			}
			if (*f == '*') {	/* field width */
				f_width = va_arg(*ap, int);
				f++;
			} else if (ISDIGIT(*f)) {
				f_width = atoi (f);
				while (ISDIGIT(*f))
					f++; /* skip the digits */
			}
			if (*f == '.') {	/* precision */
				f++;
				if (*f == '*') {
					prec = va_arg(*ap, int);
					f++;
				} else if (ISDIGIT(*f)) {
					prec = atoi (f);
					while (ISDIGIT(*f))
						f++; /* skip the digits */
				}
			}
			if (*f == '#') {	/* alternate form */
				hash = 1;
				f++;
			}
			if (*f == 'l') {	/* long format */
				do_long = 1;
				f++;
			}
			fmt = *f;
			if (ISUPPER(fmt)) {
				do_long = 1;
				fmt = TOLOWER(fmt);
			}
			bp = prbuf;
			switch (fmt) {	/* do the format */
			    case 'd':
				if (do_long)
					l = va_arg(*ap, long);
				else
					l = (long) ( va_arg(*ap, int) );
				if (l < 0) {
					sign = 1;		    
					l = -l;
				}
				do {
					*bp++ = l % 10 + '0';
				} while ((l /= 10) > 0);
				if (sign)
					*bp++ = '-';
				f_width = f_width - (bp - prbuf);
				if (!flush_left)
					while (f_width-- > 0) {
						mlputchar(pad);
						CHKTTCOL();
					}
				for (bp--; bp >= prbuf; bp--) {
					mlputchar(*bp);
					CHKTTCOL();
				}
				if (flush_left)
					while (f_width-- > 0) {
						mlputchar(' ');
					}
				break;
			    case 'o':
			    case 'x':
			    case 'u':
				if (do_long)
					u = va_arg(*ap, unsigned long);
				else
					u = (unsigned long) (va_arg(*ap, unsigned));
				if (fmt == 'u') { /* unsigned decimal */
					do {
						*bp++ = u % 10 + '0';
					} while ((u /= 10) > 0);
				} else if (fmt == 'o') { /* octal */
					do {
						*bp++ = u % 8 + '0';
					} while ((u /= 8) > 0);
					if (hash)
						*bp++ = '0';
				} else if (fmt == 'x') { /* hex */
					do {
						i = u % 16;
						if (i < 10)
							*bp++ = i + '0';
						else
							*bp++ = i - 10 + 'a';
					} while ((u /= 16) > 0);
					if (hash) {
						*bp++ = 'x';
						*bp++ = '0';
					}
				}
				i = f_width - (bp - prbuf);
				if (!flush_left)
				while (i-- > 0) {
					mlputchar (pad);
				}
				for (bp--; bp >= prbuf; bp--) {
		    			mlputchar(*bp);
				}
				if (flush_left)
					while (i-- > 0) {
						mlputchar(' ');
					}
				break;
			    case 'c':
				i = va_arg(*ap, int);
				mlputchar(i);
				break;
		  	    case 's':
				bp = va_arg(*ap, char *);
				f_width = f_width - strlen(bp);
				if (!flush_left)
					while (f_width-- > 0) {
						mlputchar(pad);
					}
				for (i = 0; *bp && i < prec; i++, bp++) {
		    			mlputchar(*bp);
		    			CHKTTCOL();
				}
				if (flush_left)
					while (f_width-- > 0)
						mlputchar(' ');
				break;
			    case '%':
				mlputchar('%');
				break;
			}
			flush_left = f_width = hash = do_long = 0;
			prec = INF;
			sign = 0;
			pad = ' ';
		}
	}
	return 0;
}

/*
 * Add this text string to the current keyboard macro definition.
 */
int
kbdtext(buf)	
char *buf;	/* text to add to keyboard macro */
{
	/* if we are defining a keyboard macro, save it */
	if (kbdmip != NULL) {
		if (kbdmip+strlen(buf) > &kbdm[NKBDM-4]) {
			ctrlg(FALSE, 0);
			(*term.t_flush)();
			return(ABORT);
		}
		/* copy string in and null terminate it */
		while (*buf)
			*kbdmip++ = *buf++;
		*kbdmip++ = 0;
	}
	return(TRUE);
}
