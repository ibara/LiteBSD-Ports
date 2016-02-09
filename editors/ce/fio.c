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
 * fio.c -- read and write ASCII disk files
 *
 * Stuff for the fio file i/o interface, a fast, buffered i/o package
 * designed especially for ce.  At the moment, it's pretty close to
 * the stdio library.
 *
 * This should help with big files (small ones are speedy enough)
 */

#include "ce.h"
#include <sys/types.h>
#include <sys/file.h>
#include <fcntl.h>
#include <sys/stat.h>
#include "celibc.h"
#include "fio.h"

extern unsigned char *ustrdup();

struct stat stb;		/* Stat buffer, for the "current file" */

uid_t	UID;
gid_t	GID;
int	ffd = -1;		/* File descriptor, all functions */
int     fiocnt = 0;
int     fiop = 0;
short   fioflg = 0;
short   fiomode = 0;
unsigned char    *fiobuf;
int     fiobsize = 0;

static unsigned char staticbuf[1024];

int
fioflush(c,fd)
int	c, fd;
{
	if (fiop > 0)
		write(fd, fiobuf, fiop);
	if (fiobuf) {
		fiobuf[0] = c;
		fiop = 1;
	}
	return(0);
}

int
fioclose(fd)
int	fd;
{
	register int r;

	if (fd < 0)
		return 0;

	if (fiomode & FIOWRITE)
		fioflush(0, fd);
	r = (close(fd) == 0) ? 0 : EOF;
	if (fd == ffd)
		ffd = -1;
	fioflg = fiomode = 0;
	fiocnt = fiop = 0;
#if 0
	if (fiobsize && fiobuf)		/* BLKSIZE == 0 means we used the static buffer */
		free(fiobuf);
	fiobuf = (unsigned char *) NULL;
#else
	if (fiobuf)
		fiobuf[0] = 0;
#endif
	return r;
}

void
allocbuf()
{
	/*
	 * Try to allocate optimally for the underlying file system.
	 */

	fiobuf = (unsigned char *) malloc(fiobsize);

	while (fiobsize && fiobuf == NULL) {
		fiobsize /= 2;		/* binary exponential backoff */
		fiobuf = (unsigned char *) malloc(fiobsize);
	}
	if (fiobsize == 0 || fiobuf == NULL) {
		fiobsize = 0;			/* just to be sure */
		fiobuf = &staticbuf[0];
	}
}

static int
readbuf(fd)
int fd;
{
	if (fioflg & F_FIOEOF)
		return EOF;

	if (fiobuf == NULL)
		allocbuf();

	/*
	 * fiobsize == 0 means we used the static 1K buffer
	 */

	fiocnt = read(fd, fiobuf, fiobsize ? fiobsize : 1024);
	fiop = 0;
	if (fiocnt <= 0) {
		if (fiocnt == 0)
			fioflg |= F_FIOEOF;
		else
			fioflg |= F_FIOERR;
		fiocnt = 0;
		return EOF;
	}
	return fiocnt;
}

int
repopbuf(fd)
int	fd;
{
	if (readbuf(fd) == EOF)
		return (EOF);
	fiocnt--;
	return (fiobuf[fiop++] & 0xFF);
}

int
fiofmode()
{
	static int um = -1;

	if (um == -1) {
		um = umask(022);
		umask(um);
	}
	return(FIOMODE & ~um);
}

/*
 * Allow other files to set the file descriptor used by the fileio
 * functions (to, e.g., a pipe to another process)
 */
void
fiosetfd(fd, mode)
int	fd, mode;
{
	if (ffd >= 0 && ffd != fd) {
		fioclose(ffd);
		ffd = -1;
	}
	ffd = fd;
	if (mode == 0)
		fiomode = FIOREAD;
	else
		fiomode = FIOWRITE;
	fioflg = 0;
	stb.st_uid = UID;
	stb.st_gid = GID;
#ifdef HAVE_ST_BLKSIZE
	stb.st_blksize = DEFBLKSIZE;
#endif
	stb.st_size = DEFBLKSIZE;
}

/*
 * Get a line from the buffer, returning it in freshly allocated memory
 * and its length in *lenp.
 */

unsigned char *
fiogetline(lenp)
int	*lenp;
{
	register unsigned char	*p, *ret;
	register int	len, c, i;

	/* Make sure there is input in the buffer. */
	if (fiocnt <= 0 && readbuf(ffd) == EOF) {
		if (*lenp)
			*lenp = 0;
		return ((unsigned char *)NULL);
	}

	/* Search forward for a newline in the input */
	if (p = (unsigned char *)memchr(fiobuf + fiop, '\n', fiocnt)) {
		*lenp = p - (fiobuf + fiop);
		*p = 0;
		ret = ustrdup(fiobuf + fiop, *lenp);
		fiop += *lenp + 1;	/* XXX fiop = p - fiobuf + 1 */
		/*
		 * It's possible for fiop == fiobsize if the buffer
		 * ends exactly on a newline, with a newline in
		 * fiobuf[fiobsize - 1]
		 */
		if (fiop > fiobsize) {
			mlwrite("BUG: fiop = %d, > fiobsize", fiop);
			sleep(3);
		}
		fiocnt -= *lenp + 1;
		if (fiocnt < 0) {
			mlwrite("BUG: fiocnt < 0, len = %d", *lenp);
			sleep(3);
		}
		*p = '\n';
		return (ret);
	}
	i = 0;
	ret = (unsigned char *) malloc ((unsigned) NLINE);
	if (ret == NULL) {
		*lenp = 0;
		fioflg |= F_FIONOMEM;
		return (unsigned char *)NULL;
	}
	len = NLINE;
	while ((c = fiogetc(ffd)) != EOF) {
		if (i >= len - 2) {
			ret = (unsigned char *) realloc (ret, (len += NLINE));
			if (ret == NULL) {
				*lenp = 0;
				fioflg |= F_FIONOMEM;
				return (unsigned char *) NULL;
			}
		}
		if (c == '\n') {
			ret[(*lenp = i)] = 0;
			return ret;
		}
		ret[i++] = c;
	}
	/* Must have hit EOF */
	if (i == 0) {
		*lenp = 0;
		return ((unsigned char *) NULL);
	}
	if (i >= len - 2) {
		ret = (unsigned char *) realloc (ret, (len += NLINE));
		if (ret == NULL) {
			*lenp = 0;
			fioflg |= F_FIONOMEM;
			return (unsigned char *) NULL;
		}
	}
	ret[i] = '\0';
	*lenp = i;
	return ret;
}

/*
 * Initialize the file i/o stuff
 */
void
fioinit()
{
	UID = geteuid();
	GID = getegid();
}
