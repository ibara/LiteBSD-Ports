/*
 * This file is automagically created from osdef.sh -- DO NOT EDIT
 */
/* Copyright (c) 1993
 *      Juergen Weigert (jnweiger@immd4.informatik.uni-erlangen.de)
 *      Michael Schroeder (mlschroe@immd4.informatik.uni-erlangen.de)
 * Copyright (c) 1987 Oliver Laumann
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program (see the file COPYING); if not, write to the
 * Free Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 *
 ****************************************************************
 * $Id: osdef.h.in,v 1.2 1994/05/31 12:32:25 mlschroe Exp $ FAU
 */

/****************************************************************
 * Thanks to Christos S. Zoulas (christos@ee.cornell.edu) who 
 * mangled the screen source through 'gcc -Wall'.
 ****************************************************************
 */

#if defined(sun) || defined(_SEQUENT_)
extern int   _flsbuf __P((int, FILE *));
#endif
#ifdef SYSV
#else
#endif

#ifndef USEBCOPY
# ifdef USEMEMCPY
# else
#  ifdef USEMEMMOVE
#  else
#  endif
# endif
#else
#endif

#ifdef BSDWAIT
struct rusage;
union wait;
#else
#endif


#ifdef HAVE_SETREUID
# ifdef hpux
extern int   setresuid __P((int, int, int));
extern int   setresgid __P((int, int, int));
# else
# endif
#endif
#ifdef HAVE_SETEUID
#endif


extern int   tgetent __P((char *, char *));
extern int   tgetnum __P((char *));
extern int   tgetflag __P((char *));
extern void  tputs __P((char *, int, void (*)(int)));
extern char *tgoto __P((char *, int, int));

#ifdef POSIX
#endif


struct passwd;





#ifdef NAMEDPIPE
#else
struct sockaddr;
extern int   socket __P((int, int, int));
extern int   connect __P((int, struct sockaddr *, int));
extern int   bind __P((int, struct sockaddr *, int));
extern int   listen __P((int, int));
extern int   accept __P((int, struct sockaddr *, int *));
#endif

#if defined(UTMPOK) && defined(GETUTENT)
extern void  setutent __P((void));
#endif

#if defined(sequent) || defined(_SEQUENT_)
extern int   getpseudotty __P((char **, char **));
#ifdef _SEQUENT_
extern int   fvhangup __P((char *));
#endif
#endif

#ifdef USEVARARGS
#endif
struct timeval;


# if defined(GETTTYENT) && !defined(GETUTENT) && !defined(UTNOKEEP)
struct ttyent;
extern void  setttyent __P((void));
extern struct ttyent *getttyent __P((void));
# endif

#ifdef SVR4
struct rlimit;
extern int getrlimit __P((int, struct rlimit *));
#endif

struct stat;

