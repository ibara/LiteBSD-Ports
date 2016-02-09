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
 * complete.c -- functions to do file name and command completion for ce, 
 * 		 mostly for interaction via the message line.
 */

#include "celibc.h"
#include "ce.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "xdir.h"

extern char	*tilde();

static void
splitname(fn, dir, name)
char	*fn, dir[], name[];
{
	register char *cp;

	cp = strrchr(fn, '/');
	if (cp == NULL) {
		(void) strcpy(name, fn);
		dir[0] = '\0';
	} else {
		++cp;
		(void) strcpy(name, cp);
	    	(void) strncpy(dir, fn, (cp - fn));
		dir[cp - fn] = '\0';
	}
}

static char *
getdirent(dfd)
DIR	*dfd;
{
	register struct dirent *dirp;

	dirp = readdir(dfd);
	if (dirp)
		return(dirp->d_name);
	return (char *) NULL;
}

static int
strprefix(name, dent)
char	*name, *dent;
{
	register char *n, *d;

	n = name;
	d = dent;
	do {
		if (*n == 0)
			return TRUE;
	} while (*n++ == *d++);
	return FALSE;
}

static int
recognize(exname, name, namlen, nmatch)
char	*exname, *name;
int	namlen, nmatch;
{
	if (nmatch == 1)
		(void) strcpy(exname, name);
	else {
		register char *x, *n;
		register int len = 0;

		for (x = exname,n = name; *x && *x == *n++; x++, len++)
			;
		*x = '\0';
		if (len == namlen)
			return FALSE;
	}
	return TRUE;
}

int
fcomplete(fname, nlen)
char	*fname;
int	nlen;
{
	int 	nmatch;
	char 	dir[512], ndir[512], name[512], exname[512];
	char	*dent;
	DIR 	*dfd;
	struct stat sb;

	splitname(fname, dir, name);
	if (dir[0] && dir[0] == '~') {
		dent = tilde(dir, ndir, sizeof(ndir));
		if (dent == 0)
			strcpy(ndir, dir);
	} else if (dir[0]) 
		(void) strcpy(ndir, dir);
	else {
		ndir[0] = '.';
		ndir[1] = '\0';
	}
	dfd = opendir(ndir);
	if (dfd == NULL)
		return 0;
	nmatch = 0;
	while (1) {
		dent = getdirent(dfd);
		if (dent == NULL)
			break;
		if (dent[0] == '.' && nlen == 0)
			continue;
		if (strprefix(name, dent) == FALSE)
			continue;
		if (recognize(exname, dent, nlen, ++nmatch) == FALSE)
			break;
	}
	(void) closedir(dfd);
	if (nmatch > 0) {
		if ((strlen(dir) + strlen(exname) + 2) > NFILEN-1)
			return 0;
		(void) strcpy(fname, dir);
		(void) strcat(fname, exname);
		/* Append a `/' to directory names. */
		(void) strcat(ndir, "/");
		(void) strcat(ndir, exname);
		if ((stat (ndir, &sb) == 0) && S_ISDIR(sb.st_mode))
			(void) strcat(fname, "/");
	}
	return nmatch;
}

int
bcomplete(bname, blen)
char	*bname;
int	blen;
{
	char	exname[512];
	register BUFFER	*bp;
	int	nmatch;

	bp = bheadp;
	nmatch = 0;
	while (bp) {
		if (strprefix(bname, bp->b_bname) == FALSE) {
			bp = bp->b_bufp;
			continue;
		}
		if (recognize(exname, bp->b_bname, blen, ++nmatch) == FALSE)
			break;
		bp = bp->b_bufp;	/* onward */
	}
	if (nmatch > 0) {
		(void) strncpy(bname, exname, NBUFN-1);
		bname[NBUFN-1] = '\0';
	}
	return nmatch;
}

int
cmdcomplete(buf, blen)
char	*buf;
int	blen;
{
	char	exbuf[NSTRING];
	register NBIND *ntp;
	register int nm;

	ntp = &(names[0]);
	nm = 0;
	while (ntp && (ntp->n_name) && (ntp->n_name != "")) {
		if (strprefix(buf, ntp->n_name) != FALSE)
			break;
		ntp++;
	}
	/* we have hit the first match */
	if ((ntp == NULL) || (ntp->n_name == NULL) || (ntp->n_name == ""))
		return 0;
	nm++;
	(void) strcpy(exbuf, ntp->n_name);
	ntp++;
	while (strprefix(buf, ntp->n_name) == TRUE) {
		if (recognize(exbuf, ntp->n_name, blen, ++nm) == FALSE)
			break;
		ntp++;
	}
	if (nm > 0)
		(void) strcpy(buf, exbuf);
	return nm;
}
