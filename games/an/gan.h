/* Copyright (C) 1991, 1992 Free Software Foundation, Inc.
This file is part of the GNU C Library.
Written by Douglas C. Schmidt (schmidt@ics.uci.edu).

The GNU C Library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public License as
published by the Free Software Foundation; either version 2 of the
License, or (at your option) any later version.

The GNU C Library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with the GNU C Library; see the file COPYING.LIB.  If
not, write to the Free Software Foundation, Inc., 675 Mass Ave,
Cambridge, MA 02139, USA.  */

#include <stdlib.h>
#include <string.h>

#if defined(_STDC_) || defined (__STDC__)
#define PTR             void *
#define PTRCONST        void *CONST
#define LONG_DOUBLE     long double

#define AND             ,
#define NOARGS          void
#define CONST           const
#define VOLATILE        volatile
#define SIGNED          signed
#define DOTS            , ...

#define EXFUN(name, proto)              name proto
#define DEFUN(name, arglist, args)      name(args)
#define DEFUN_VOID(name)                name(NOARGS)

#else   /* Not ANSI C.  */

#define PTR             char *
#define PTRCONST        PTR
#define LONG_DOUBLE     double

#define AND             ;
#define NOARGS
#define CONST
#define VOLATILE
#define SIGNED
#define DOTS

#define EXFUN(name, proto)              name()
#define DEFUN(name, arglist, args)      name arglist args;
#define DEFUN_VOID(name)                name()

#endif  /* ANSI C.  */

typedef int (*gan_qsort_t) (CONST PTR, CONST PTR);
void
extern DEFUN(gan_qsort, (pbase, total_elems, size, cmp),
      PTR CONST pbase AND size_t total_elems AND size_t size AND
      int EXFUN((*cmp), (CONST PTR, CONST PTR)));
