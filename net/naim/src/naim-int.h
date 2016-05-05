/*  _ __   __ _ ___ __  __
** | '_ \ / _` |_ _|  \/  | naim
** | | | | (_| || || |\/| | Copyright 1998-2004 Daniel Reed <n@ml.org>
** |_| |_|\__,_|___|_|  |_| ncurses-based chat client
*/
#ifndef naim_int_h
#define naim_int_h	1

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifdef HAVE_ERRNO_H
# include <errno.h>
#endif
#ifdef HAVE_SYS_ERRNO_H
# include <sys/errno.h>
#endif
#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif
#ifdef HAVE_EXECINFO_H
# include <execinfo.h>
#endif

#if 1
# undef NCURSES_CONST
# define NCURSES_CONST const
# ifdef HAVE_NCURSES_H
#  include <ncurses.h>
# else
#  ifdef HAVE_CURSES_H
#   include <curses.h>
#  else
#   error Unable to locate ncurses.h; please see http://naim.n.ml.org/
#  endif
# endif
# ifndef KEY_CODE_YES
#  if (KEY_MIN & ~1) > 1
#   define KEY_CODE_YES	(KEY_MIN & ~1)
#  else
#   error Unable to identify ncurses key codes
#  endif
# endif
#endif

#include <assert.h>

#ifdef HAVE_DIRENT_H
# include <dirent.h>
#else
# warning No dirent.h
#endif

#ifdef HAVE_TIME_H
# include <time.h>
# ifdef HAVE_SYS_TIME_H
#  ifdef TIME_WITH_SYS_TIME
#   include <sys/time.h>
#  endif
# endif
#else
# ifdef HAVE_SYS_TIME_H
#  include <sys/time.h>
# endif
#endif

#ifdef HAVE_TIMEZONE
extern long int timezone;
#else
# ifdef HAVE_STRUCT_TM_TM_GMTOFF
#  define timezone	(-(tmptr->tm_gmtoff))
# else
#  define timezone	((long int)0)
# endif
#endif

#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif
#ifdef HAVE_SYS_STAT_H
# include <sys/stat.h>
#endif

#define SLIDETIME	.3

#endif /* naim_int_h */
