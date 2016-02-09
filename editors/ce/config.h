/* config.h.  Generated by configure.  */
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
 * config.h -- some configuration constants to tailor the editor for a
 *	     particular machine/environment.
 */

#ifndef _CONFIG_H_
#define _CONFIG_H_

/* Define if you have the <unistd.h> header file.  */
#define HAVE_UNISTD_H 1

/* Define if you have the <limits.h> header file.  */
#define HAVE_LIMITS_H 1

/* Define if you have the <stdarg.h> header file. */
#define HAVE_STDARG_H 1

/* Define if you have the <stdlib.h> header file. */
#define HAVE_STDLIB_H 1

/* Define if you have the <string.h> header file.  */
#define HAVE_STRING_H 1

/* Define if you have the <termios.h> header file.  */
#define HAVE_TERMIOS_H 1

/* Define if you have the <termio.h> header file.  */
#define HAVE_TERMIO_H 1

/* Define if you have the <sys/wait.h> header file.  */
#define HAVE_SYS_WAIT_H 1

/* Define if you have the dup2 function. */
#define HAVE_DUP2 1

/* Define if you have the gethostname function. */
#define HAVE_GETHOSTNAME 1

/* Define if you have the getpagesize function. */
#define HAVE_GETPAGESIZE 1

/* Define if you have the getcwd function */
#define HAVE_GETCWD 1

/* Define if you have the memchr function */
#define HAVE_MEMCHR 1

/* Define if you have the strdup function */
#define HAVE_STRDUP 1

/* Define if you have the strerror function */
#define HAVE_STRERROR 1

/* Define if you have the strndup function */
#define HAVE_STRNDUP 1

/* Define if you have the tcgetattr function */
#define HAVE_TCGETATTR 1

/* Define if you have the bcopy function */
#define HAVE_BCOPY 1

/* Define if you have the bzero function */
#define HAVE_BZERO 1

/* Define if you have the memmove function */
#define HAVE_MEMMOVE 1

/* Define if you have the fchown function */
#define HAVE_FCHOWN 1

/* Define if you have the fchmod function */
#define HAVE_FCHMOD 1

/* Define if you have the lstat function */
#define HAVE_LSTAT 1

#define HAVE_DECL_SBRK 1

/* #undef DUP2_BROKEN */

/* Define if you have the <dirent.h> header file.  */
#define HAVE_DIRENT_H 1

/* #undef HAVE_SYS_NDIR_H */

/* #undef HAVE_SYS_DIR_H */

/* #undef HAVE_NDIR_H */

/* Define to `int' if <sys/types.h> doesn't define.  */
/* #undef uid_t */

/* Define to `int' if <sys/types.h> doesn't define.  */
/* #undef gid_t */

/* Define to `unsigned short' if <sys/types.h> doesn't define.  */
/* #undef u_bits16_t */

/* Define to `unsigned int' if <sys/types.h> doesn't define.  */
/* #undef u_bits32_t */

/* Define to `double' if <sys/types.h> doesn't define. */
#define bits64_t double

/* #undef size_t */

/* #undef off_t */

/* #undef int32_t */

/* #undef u_int32_t */

/* #undef u_int16_t */

#define bits64_t double

/* The return type of signal handlers. */
#define RETSIGTYPE void

/* #undef const */

#define HAVE_STRINGIZE 1

#define PROTOTYPES 1

#define HAVE_POSIX_SIGNALS 1

/* #undef HAVE_BSD_SIGNALS */

/* #undef HAVE_USG_SIGHOLD */

/* The number of bytes in a int.  */
#define SIZEOF_INT 4

/* The number of bytes in a long.  */
#define SIZEOF_LONG 4

/* The number of bytes in a char *. */
#define SIZEOF_CHAR_P 4

/* The number of bytes in a double. */
#define SIZEOF_DOUBLE 8

/* Define if `struct stat' has an `st_blksize' member. */
#define HAVE_ST_BLKSIZE 1

/* defined if job control facilities are present */
#define JOBS 1

/* config.h.bot */

/* Ultrix botches type-ahead when switching from canonical to
   non-canonical mode, at least through version 4.3 */
#if !defined (HAVE_TERMIOS_H) || !defined (HAVE_TCGETATTR) || defined (ultrix)
#  define TERMIOS_MISSING
#endif

#if defined (__STDC__) && defined (HAVE_STDARG_H)
#  define PREFER_STDARG
#endif

/* See if we have POSIX job control and the other autoconf test failed */
#ifndef JOBS
#  ifdef HAVE_UNISTD_H
#    include <unistd.h>
#  endif

#  ifdef _POSIX_JOB_CONTROL
#    define JOBS 1
#  endif
#endif /* !JOBS */

#ifndef HAVE_BCOPY
#define bcopy(s,d,n)	(memcpy((d), (s), (n)))
#endif

#ifndef HAVE_BZERO
#define bzero(s, n)	(memset((s), 0, (n)))
#endif

#endif /* _CONFIG_H_ */
