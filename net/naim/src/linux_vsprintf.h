/*
 *  linux/lib/vsprintf.c
 *
 *  Copyright (C) 1991, 1992  Linus Torvalds
 */

/* vsprintf.c -- Lars Wirzenius & Linus Torvalds. */
/*
 * Wirzenius wrote this portably, Torvalds fucked it up :-)
 */

/* 
 * Fri Jul 13 2001 Crutcher Dunnavant <crutcher+kernel@datastacks.com>
 * - changed to provide snprintf and vsnprintf functions
 */

#include <ctype.h>
#include <stdarg.h>
#include <string.h>

#define do_div(x, y)	({ \
	unsigned long long	__res; \
	\
	__res = ((unsigned long long)(x)) % ((unsigned)(y)); \
	x = ((unsigned long long)(x)) / ((unsigned)(y)); \
	__res; \
})

#define strnlen(buf, maxlen)	({ \
	size_t	len; \
	\
	for (len = 0; (len < ((size_t)maxlen)) && (buf[len] != 0); len++) \
		; \
	len; \
})

static int skip_atoi(const char **s);

#define ZEROPAD	1		/* pad with zero */
#define SIGN	2		/* unsigned/signed long */
#define PLUS	4		/* show plus */
#define SPACE	8		/* space if plus */
#define LEFT	16		/* left justified */
#define SPECIAL	32		/* 0x */
#define LARGE	64		/* use 'ABCDEF' instead of 'abcdef' */

static char *number(char * buf, char * end, long long num, int base, int size, int precision, int type);
int vsnprintf(char *buf, size_t size, const char *fmt, va_list args);
int snprintf(char * buf, size_t size, const char *fmt, ...);
int vsprintf(char *buf, const char *fmt, va_list args);
int sprintf(char * buf, const char *fmt, ...);
