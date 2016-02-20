/* config.h.  Generated automatically by configure.  */
/********************************************
config_h.in
copyright 2009-2013,2014 Thomas E. Dickey
 vile:cmode

This is a source file for mawk, an implementation of
the AWK programming language.

Mawk is distributed without warranty under the terms of
the GNU General Public License, version 2, 1991.
********************************************/

/*
 * $MawkId: config_h.in,v 1.29 2014/09/07 16:56:38 tom Exp $
 * template for config.h
 */

/* #undef DECL_ENVIRON */
/* #undef FPE_TRAPS_ON */
/* #undef GCC_NORETURN */
/* #undef GCC_PRINTF */
/* #undef GCC_PRINTFLIKE */
/* #undef GCC_SCANF */
/* #undef GCC_SCANFLIKE */
/* #undef GCC_UNUSED */
/* #undef HAVE_BSD_STDLIB_H */
#define HAVE_ERRNO_H 1
#define HAVE_FCNTL_H 1
#define HAVE_FORK 1
#define HAVE_ISNAN 1
#define HAVE_MATHERR 1
#define HAVE_MATH__LIB_VERSION 1
#define HAVE_MKTIME 1
#define HAVE_PIPE 1
#define HAVE_REAL_PIPES 1
/* #undef HAVE_REGEXPR_H_FUNCS */
/* #undef HAVE_REGEXP_H_FUNCS */
/* #undef HAVE_REGEX_H_FUNCS */
#define HAVE_SIGACTION 1
#define HAVE_SIGACTION_SA_SIGACTION 1
/* #undef HAVE_SIGINFO_H */
#define HAVE_STRFTIME 1
/* #undef HAVE_STRTOD_OVF_BUG */
#define HAVE_SYS_WAIT_H 1
#define HAVE_TDESTROY 1
#define HAVE_TSEARCH 1
#define HAVE_UNISTD_H 1
#define HAVE_WAIT 1
#define LOCALE 1
#define MAWK_RAND_MAX (1L<<31)-1
/* #undef MAX__INT */
/* #undef MAX__LONG */
/* #undef MAX__UINT */
#define NAME_RANDOM "srandom/random"
/* #undef NOINFO_SIGFPE */
/* #undef NO_LEAKS */
#define SIZE_T_STDDEF_H 1
/* #undef SIZE_T_TYPES_H */
#define SYSTEM_NAME "litebsd"
/* #undef TURN_OFF_FPE_TRAPS */
/* #undef TURN_ON_FPE_TRAPS */
/* #undef USE_IEEEFP_H */
/* #undef YY_NO_LEAKS */
/* #undef const */
#define mawk_rand random
#define mawk_srand srandom

#ifndef GCC_NORETURN
#define GCC_NORETURN /* nothing */
#endif

#ifndef GCC_UNUSED
#define GCC_UNUSED /* nothing */
#endif

#ifndef OPT_TRACE
#define OPT_TRACE 0
#endif
