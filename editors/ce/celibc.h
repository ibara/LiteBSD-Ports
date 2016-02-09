/*
 * This file is part of ce.
 *
 * Copyright (c) 1997 by Chester Ramey.
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
 *  celibc.h -- encapsulate differences between what libc provides on
 *		different systems.
 */

#ifndef _CELIBC_H_
#define _CELIBC_H_

#include <config.h>

/* Generic pointer type. */
#ifndef PTR_T
#  if defined (__STDC__)
#    define PTR_T void *
#  else
#    define PTR_T char *
#  endif
#endif

#include <stdio.h>

#ifdef HAVE_UNISTD_H
#  include <unistd.h>
#else
extern int chown();
extern char *getcwd();
extern char *mktemp();
#endif

#ifdef HAVE_STRING_H
#  include <string.h>
#else
#  include <strings.h>
#endif

/*
 * String functions that may or may not be provided by the C library
 */
#ifndef HAVE_STRDUP
extern char *strdup();
#endif

#ifndef HAVE_STRNDUP
extern char *strndup();
#endif

#ifndef HAVE_STRERROR
extern char *strerror();
#endif

#ifndef HAVE_MEMCHR
extern char *memchr();
#endif

#ifdef HAVE_STDLIB_H
#  include <stdlib.h>
#else

extern PTR_T malloc();
extern PTR_T realloc();
extern void free();

extern char *getenv();

#endif /* !HAVE_STDLIB_H */

#endif /* _CELIBC_H_ */
