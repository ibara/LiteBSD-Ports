/* cfg.h
 * (c) 2002 Mikulas Patocka
 * This file is a part of the Links program, released under GPL.
 */

#ifndef CFG_H
#define CFG_H
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#ifdef HAVE_CONFIG2_H
#include "config2.h"
#endif
#ifdef HAVE_CONFIG_VMS_H
#include "config-vms.h"
#define HAVE_RAND_ADD 1
#define HAVE_RAND_EGD 1
#define HAVE_RAND_FILE_NAME 1
#define HAVE_RAND_LOAD_FILE 1
#define HAVE_RAND_WRITE_FILE 1
#ifdef VMS_DEBUGLEVEL
#undef DEBUGLEVEL
#define DEBUGLEVEL VMS_DEBUGLEVEL
#endif
#ifdef __VAX
#undef HAVE_LONG_LONG
#undef SIZEOF_UNSIGNED_LONG_LONG
#undef HAVE_SNPRINTF
#undef HAVE_STRTOLL
#undef HAVE_STRTOQ
#undef HAVE_SOCKETPAIR
#undef HAVE_GETADDRINFO
#undef HAVE_FREEADDRINFO
#undef SUPPORT_IPV6
#undef SUPPORT_IPV6_SCOPE
#endif
#endif

#if !defined(G) && DEBUGLEVEL >= 0 && defined(HAVE_SETJMP_H)
#undef HAVE_SETJMP_H
#endif

/* no one will probably ever port svgalib on atheos or beos or port atheos
   interface to beos, but anyway: make sure they don't clash */

#if defined(__BEOS__) || defined(__HAIKU__) || defined(__VMS) || defined(__DJGPP)
#ifdef GRDRV_SVGALIB
#undef GRDRV_SVGALIB
#endif
#ifdef GRDRV_ATHEOS
#undef GRDRV_ATHEOS
#endif
#endif

#ifdef GRDRV_ATHEOS
#ifdef GRDRV_SVGALIB
#undef GRDRV_SVGALIB
#endif
#endif

#if !(defined(__EMX__) || defined(_WIN32) || defined(__CYGWIN__))
#ifdef GRDRV_PMSHELL
#undef GRDRV_PMSHELL
#endif
#endif

#ifndef HAVE_VOLATILE
#define volatile
#endif

typedef int cfg_h_no_empty_unit;

#endif
