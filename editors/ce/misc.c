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
 * misc.c -- miscellaneous utility routines for ce.  Some of these should be
 *	     in the C library.
 */
   
#include "ce.h"
#include "celibc.h"
#include <pwd.h>

#ifndef NULL
#define NULL	0x0
#endif

extern char	home[];

extern struct passwd *getpwnam();

#ifndef HAVE_STRERROR
/*
 * strerror -- return a string error message corresponding to the error code
 *	       e.
 */

extern int	sys_nerr;
extern char	*sys_errlist[];

char *
strerror(e)
int	e;
{
	static char	ebuf[40];

	if (e < 0 || e > sys_nerr) {
		(void) sprintf(ebuf,"Unknown error code %d", e);
		return (&ebuf[0]);
	}
	return (sys_errlist[e]);
}
#endif

#ifndef HAVE_STRDUP
char *
strdup(s)
char	*s;
{
	char	*r;
	int	len;

	if (s == NULL)
		return ((char *) NULL);
	r = (char *) malloc((unsigned) ((len = strlen(s)) + 1));
	if (r == NULL)
		return ((char *) NULL);
	bcopy(s, r, len);
	r[len] = '\0';
	return r;
}
#endif

#ifndef HAVE_STRNDUP
char *
strndup(str, n)
char	*str;
int	n;
{
	char   *t;

	t = (char *) malloc(n);
	if (t == NULL)
		return t;
	bcopy(str, t, n);
	str[n] = '\0';
	return t;
}
#endif

unsigned char *
ustrdup(s, len)
unsigned char	*s;
int	len;
{
	unsigned char *r;

	r = (unsigned char *) malloc((unsigned)(len + 1));
	if (r == NULL)
		return ((unsigned char *) NULL);
	bcopy(s, r, len);
	r[len] = '\0';
	return r;
}

#ifndef HAVE_MEMCHR
/*
 * Find next occurrence of c in s, searching at most n characters
 */
char *
memchr(s, c, n)
char	*s;
int	c, n;
{
	if (n != 0) {
		register char *p = s;

		do {
			if (*p++ == c)
				return ((char *)(p - 1));
		} while (--n != 0);
	}
	return (NULL);
}
#endif

void
failed(call, fn, err)
char	*call;
char	*fn;
int	err;
{
	mlwrite("%s: %s failed: %s", fn, call, strerror(err));
	sleep(1);
}

void
xpanic(s)
char	*s;
{
	extern char *progname;

	(void) write(2, progname, strlen(progname));
	(void) write(2, ": panic: ", 9);
	(void) write(2, s, strlen(s));
	(void) write(2, "\n", 1);
	(void) abort();			/* to leave a core image */
}

#if !defined (HAVE_DUP2) || defined (DUP2_BROKEN)
#include <fcntl.h>
#include <sys/param.h>

int
dup2(fd1, fd2)
int	fd1, fd2;
{
	int	e, r;

	if (fcntl (fd1, F_GETFL, 0) == -1)
		return(-1);

	if (fd2 < 0 || fd2 >= NOFILE) {
		errno = EBADF;
		return(-1);
	}

	if (fd1 == fd2)
		return(0);

	e = errno;
	(void) close(fd2);
	r = fcntl (fd1, F_DUPFD, fd2);

	if (r >= 0)
		errno = e;
	else
		if (errno == EINVAL)
			errno = EBADF;

	return(r);
}
#endif /* !DUP2 */

/*
 * Expand "~" in file names (actually only at the beginning) like some
 * shells.  Handle ~user as well as ~ alone.
 */

char *
tilde(fn, buf, buflen)
char	*fn, *buf;
int	buflen;
{
	register char	*cp;
	struct passwd	*pwent;
	int	hlen, flen;

	if (fn == NULL) {
		buf[0] = '\0';
		return ((char *) NULL);
	}
	if (*fn != '~')
		return (fn);
	else {
		hlen = (home && *home) ? strlen(home) : 0;
		fn++;
		flen = (fn && *fn) ? strlen(fn) : 0;
		if (*fn == '\0' || *fn == '/') {
			if ((hlen + flen + 2) > buflen) {
				(*term.t_beep)();
				mlwrite("expanded file name too long");
				buf[0] = '\0';
				return ((char *)NULL);
			}
			(void) strcpy(buf, home);
			if (*fn == '/')
				fn++;
			(void) strcat(buf, "/");
			(void) strcat(buf, fn);
			return (buf);
		} else {
			/*
			 * This cannot overflow on all real systems, since
			 * the maximum length of a username is less than
			 * BUFLEN, which is at least 80.
			 */
			cp = buf;
			while (*fn && *fn != '/') 
				*cp++ = *fn++;
			*cp = '\0';
			if (*fn == '/') 
				fn++;
			if ((pwent = getpwnam(buf)) != NULL) {
				if ((strlen(pwent->pw_dir) + strlen(fn) + 2) >= buflen) {
					(*term.t_beep)();
					mlwrite("expanded file name too long");
					buf[0] = '\0';
					return ((char *)NULL);
				}
				(void) strcpy(buf, pwent->pw_dir);
				(void) strcat(buf, "/");
				(void) strcat(buf, fn);
				return(buf);
			} else {
				(*term.t_beep)();
				mlwrite("%s: no such user", buf);
				buf[0] = '\0';
				return((char *) NULL);
			}
		}
	}
}

#ifndef HAVE_GETHOSTNAME
#include <sys/utsname.h>

int
gethostname(name, namelen)
char	*name;
int	namelen;
{
	int	i;
	struct utsname ut;

	--namelen;
	uname(&ut);
	i = strlen(ut.nodename) + 1;
	strncpy(name, ut.nodename, i < namelen ? i : namelen);
	name[namelen] = '\0';
	return 0;
}
#endif
