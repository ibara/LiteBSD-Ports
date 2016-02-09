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
 * bug.c -- functions for making and sending bug reports to ce's maintainer
 */

#include <stdio.h>
#include "celibc.h"
#include "ce.h"

#if 0
#define BUG_REPORT	"/usr/local/lib/ce.bug-report"
#endif

extern BUFFER	*bfind();
extern FILE	*popen();

static BUFFER	*savedbp = (BUFFER *) NULL;

extern char	*versionstring();

/*
 * Pop to a "bug report" buffer, populate it with a template (maybe), and
 * let the user fill it in.
 */
int
makebug(f,n)
int	f, n;
{
	register BUFFER	*bp;
#if 0
	int		s, nbytes;
	char		*line, buf[NBUFN];
#endif

	savedbp = curbp;
	bp = bfind("*Bug Report*", TRUE, 0);
	if (bp == NULL) {
		mlwrite("Cannot create bug report buffer");
		savedbp = (BUFFER *) NULL;
		return (FALSE);
	}
#if 0
	s = ffropen(BUG_REPORT);
	if (s != FIOFNF && s != FIOERR) {
		while ((s = ffgetline(&line, &nbytes)) != FIOERR) {
			if (s == FIOEOF && nbytes == 0)
				break;
			addline(bp, line, nbytes);
			free(line);
			if (s == FIOEOF)
				break;
		}
		s = ffclose();
	}
#endif
	strcpy(bp->b_bname, "*Bug Report*");	/* to make sure... */
	return (switchbuffer(bp));
}

/*
 * Send the contents of the current buffer to the maintainer as a bug
 * report.  If there is a saved buffer, return to it.
 */
int
sendbug(f, n)
int	f, n;
{
	register int	i;
	register LINE	*lp;
	FILE		*mailer;

	mlwrite("Sending current buffer as bug report...");
	mailer = popen("/bin/rmail chet@ins.CWRU.Edu", "w");
	if (mailer == NULL) {
		mlwrite("Cannot open pipe to mailer, sendbug aborted");
		return (FALSE);
	}
	fprintf(mailer,"To: CE Maintainer <chet@ins.CWRU.Edu>\n");
	fprintf(mailer,"Subject: CE bug report (%s)\n", versionstring());
	fprintf(mailer,"\n");
	lp = lforw(curbp->b_linep);
	while (lp != curbp->b_linep) {
		for (i = 0; i < llength(lp); i++)
			fputc(lgetc(lp,i), mailer);
		fputc('\n', mailer);
		lp = lforw(lp);
	}
	pclose(mailer);
	mlwrite("Sending current buffer as bug report...done");
	if (savedbp) {
		register BUFFER	*bp;

		bp = curbp;			/* the bug report buffer */
		switchbuffer(savedbp);
		savedbp = (BUFFER *) NULL;
		bp->b_flag &= ~BFCHG;
		zotbuf(bp); 
	}
	curwp->w_flag |= (WFHARD|WFMODE);
	update();
	return (TRUE);
}
