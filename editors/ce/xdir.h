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

/* xdir.h - encapsulate differences in the directory-reading interface. */

#ifndef _XDIR_H
#define _XDIR_H

#ifdef HAVE_DIRENT_H
#  include <dirent.h>
#else
#  define dirent direct
#  ifdef HAVE_SYS_NDIR_H
#    include <sys/ndir.h>
#  endif
#  ifdef HAVE_SYS_DIR_H
#    include <sys/dir.h>
#  endif
#  ifdef HAVE_NDIR_H
#    include <ndir.h>
#  endif
#endif

#ifndef S_ISDIR
#  if defined (_S_IFMT) && !defined (S_IFMT)
#    define S_IFMT _S_IFMT
#  endif
#  if defined (_S_IFDIR) && !defined (S_IFDIR)
#    define S_IFDIR _S_IFDIR
#  endif
#  if defined (S_IFDIR)
#    define S_ISDIR(m)	(((m) & S_IFMT) == S_IFDIR)
#  endif
#endif

#endif
