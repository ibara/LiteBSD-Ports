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
 *  version.c -- show current version of ce, as well as the date compiled 
 *		 (created dynamically each time ce is compiled).
 */
#ifndef HOSTINFO
#  define HOSTINFO "unknown"
#endif
#include <stdio.h>

char 	*version = "version 4.6";
char	*dversion = "ce 4.6";
char	*xdversion = "xce 4.6";

extern int	versnum;
extern char	datecompiled[];

char *
versionstring()
{
	static char	vstr[128] = {'\0'};

	if (vstr[0] == '\0')
#ifdef X11
		sprintf(vstr, "X11 %s (%d) of %s (%s)", version, versnum, datecompiled, HOSTINFO);
#else
		sprintf(vstr, "%s (%d) of %s (%s)", version, versnum, datecompiled, HOSTINFO);
#endif
	return (&vstr[0]);
}

/*ARGSUSED*/
int
showversion(f,n)
int	f, n;
{
	mlwrite("Chet's Emacs, %s", versionstring());
	return (1);
}
