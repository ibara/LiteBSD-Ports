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
 * dir.c -- directory management functions
 *
 * originally by Ron Flax (ron@vsedev.vse.com) and  Mic Kaczmarczik, from mg2a
 */

#include "celibc.h"
#include "ce.h"

#ifndef NULL
#define NULL	0
#endif

char		*wdir;
char		home[512];
static 	char	cwd[512];

extern	char	*tilde();

/*
 * Initialize anything the directory management routines need
 */
void
dirinit()
{
	char	*pwd;

	pwd = getenv("PWD");
	if (pwd == NULL)
		pwd = getenv("CWD");
	if (pwd) {
		strncpy(cwd, pwd, sizeof(cwd));
		wdir = cwd;
	} else {
		if ((wdir = getcwd(cwd, sizeof(cwd))) == NULL)
			panic("Can't get current directory!");
	}
	strncpy(home, getenv("HOME"), sizeof(home) - 1);
	home[sizeof(home) -1] = '\0';
}

/*
 * Change current working directory
 */
/*ARGSUSED*/
int
changedir(f, n)
int	f, n;
{
	register int s;
	char bufc[NPAT], dbuf[NPAT], *d;

	if ((s = mlgetfile("Change working directory: ", bufc, NPAT)) != TRUE)
		return(s);
	if (bufc[0] == '\0')
		(void) strcpy(dbuf, wdir);
	else {
		d = tilde(bufc, dbuf, sizeof(dbuf));
		if (d == 0) {
			mlwrite("%s: no such directory", bufc);
			return(FALSE);
		}
	}
	if (chdir(dbuf) == -1) {
		mlwrite("Can't change dir to %s", bufc);
		return(FALSE);
	} else {
		if ((wdir = getcwd(cwd, sizeof(cwd))) == NULL)
			panic("Can't get current directory!");
		mlwrite("Current directory is now %s", wdir);
		return(TRUE);
	}
}

/*
 * Show current directory
 */
/*ARGSUSED*/
int
showcwd(f, n)
int	f, n;
{
	mlwrite("Current directory: %s", wdir);
	return(TRUE);
}

#ifndef HAVE_GETCWD
/*
 * getcwd -- Posix 1003.1 get current directory function, built atop the
 *	     4.2 BSD getwd() function.
 *
 * This departs from the 1003.1 spec in that it returns an error message
 * in `dir' if it fails.  As in the spec, getcwd() returns NULL and sets
 * errno appropriately on failure.
 */

#include <errno.h>
#include <sys/param.h>

#ifndef NULL
#define NULL	0x0
#endif

#ifndef MAXPATHLEN
#define MAXPATHLEN	1024
#endif

extern int	errno;
extern char	*getwd();

static char	wd[MAXPATHLEN];

char *
getcwd(buf, size)
char	*buf;
int	size;
{
	char	*d;

	if (size <= 0) {
		errno = EINVAL;
		return ((char *) NULL);
	}
	d = getwd(wd);
	strncpy(buf, wd, size - 1);
	buf[size - 1] = '\0';
	if (d == NULL)
		return ((char *) NULL);
	if (strlen(wd) >= (size - 1)) {
		errno = ERANGE;
		return ((char *) NULL);
	}
	return (&buf[0]);
}
#endif
