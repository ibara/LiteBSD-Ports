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

/* ttyio.h -- pick which flavor of terminal driver we'll use. */

#ifndef _TTYIO_H

#define _TTYIO_H

#if defined (_POSIX_VERSION) && defined (HAVE_TERMIOS_H) && defined (HAVE_TCGETATTR) && !defined (TERMIOS_MISSING)
#  define TERMIOS_TTY_DRIVER
#else
#  if defined (HAVE_TERMIO_H)
#    define TERMIO_TTY_DRIVER
#  else
#    define BSD_TTY_DRIVER
#  endif
#endif

#ifdef TERMIOS_TTY_DRIVER
#include	<termios.h>
#else
#  ifdef TERMIO_TTY_DRIVER
#    include	<termio.h>
#  else
#    include	<sgtty.h>
#  endif
#endif

#if !defined (BSD_TTY_DRIVER) && !defined (_POSIX_VDISABLE)
#  ifdef _SVR4_DISABLE
#    define _POSIX_VDISABLE _SVR4_VDISABLE
#  else
#    ifdef VDISABLE
#      define _POSIX_VDISABLE VDISABLE
#    else
#      ifdef _POSIX_VERSION
#        define _POSIX_VDISABLE 0
#      else
#        define _POSIX_VDISABLE -1
#      endif /* !_POSIX_VERSION */
#    endif /* !VDISABLE */
#  endif /* !_SVR4_VDISABLE */
#endif

#ifndef TCSADRAIN
#  define TCSADRAIN 0
#  define TCSAFLUSH 1
#  define TCSANOW 2
#endif

#ifdef BSD_TTY_DRIVER
struct _bsd_termios {
	struct sgttyb	_bsd_sgtty;	/* V6/V7 stty data */
	struct tchars	_bsd_tchars;	/* V7 editing */
	struct ltchars	_bsd_ltchars;	/* 4.2 BSD editing */
};
#endif

#endif
