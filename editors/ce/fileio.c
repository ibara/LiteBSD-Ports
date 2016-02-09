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
 * fileio.c -- read and write ASCII disk files
 *
 * The routines in this file read and write ASCII files from the disk. All of
 * the knowledge about files is here. A better message writing scheme should
 * be used.
 *
 * N.B. This is somewhat specific to 4.2 BSD-flavored Posix.1 systems
 */

#include "ce.h"
#include <sys/types.h>
#include <sys/file.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <pwd.h>
#include <errno.h>
#include "celibc.h"
#include "fio.h"

extern char	*tilde();

#ifndef errno
extern int	errno;
#endif

extern int	ffd;		/* File descriptor, all functions */
extern struct stat stb;		/* Stat buffer, for the "current file" */
extern char     *wdir;
extern int	followlinks;

static char	curfn[NFILEN];	/* Buffer for the "current file" name */

static int
chkfile(fn, stb)
char	*fn;
struct stat stb;
{
	u_short	m;
	char	*err = "non-editable file";
	/*
	 * Check that the file is a regular file before we try to do
	 * something stupid like edit a directory or a device file.
	 */
	m = stb.st_mode & S_IFMT;
	if (m != S_IFREG && m != S_IFLNK) {
		switch (m) {
		case S_IFDIR:
			err = "directory";
			break;
		case S_IFCHR:
			err = "character special device";
			break;
		case S_IFBLK:
			err = "block special device";
			break;
		case S_IFSOCK:
			err = "socket";
			break;
#ifdef S_IFIFO
		case S_IFIFO:
			err = "FIFO";
			break;
#endif

		}
		(*term.t_beep)();
		mlwrite("cannot edit %s: file is a %s", fn, err);
		return (FIOERR);
	}
	return (FIOSUC);
}

/*
 * Open a file for reading.
 */
int
ffropen(fn)
char    *fn;
{
	char	buf[NFILEN], *d;

	d = tilde(fn, buf, sizeof(buf));
	if (d == 0)
		d = fn;
	if (stat(d, &stb) < 0) {
		stb.st_mode = fiofmode() | S_IFREG;
		stb.st_uid = UID;
		stb.st_gid = GID;
#ifdef HAVE_ST_BLKSIZE
		stb.st_blksize = stb.st_size = DEFBLKSIZE;
#else
		stb.st_size = DEFBLKSIZE;
#endif
	} else if (chkfile(d, stb) == FIOERR)
		return FIOERR;

#ifdef HAVE_ST_BLKSIZE
	if (fiobsize && stb.st_blksize > fiobsize) {
#else
	if (fiobsize && fiobsize < DEFBLKSIZE) {
#endif
		/*
		 * If we've allocated a buffer already, but that buffer is
		 * too small for this file system's blocksize, force a
		 * reallocation with the new (bigger) size.
		 */
		if (fiobuf)
			free(fiobuf);
		fiobuf = (unsigned char *)NULL;
	}
#ifdef HAVE_ST_BLKSIZE
	fiobsize = stb.st_blksize;
#else
	fiobsize = DEFBLKSIZE;
#endif
	if ((ffd = fioropen(d)) < 0) {
		mlwrite("Cannot open %s for reading: %s", d, strerror(errno));
		return (FIOFNF);
	}
	(void) strcpy(curfn, d);
	fiomode |= FIOREAD;
	return (writable(d) ? FIOSUC : FIORDONLY);
}

/*
 * Open a file for writing. Return TRUE if all is well, and FALSE on error
 * (cannot create).
 */
int
ffwopen(fn, backup)
char    *fn;
int	backup;		/* should we make a backup before we open it again? */
{
	char	buf[NFILEN], *d;
	int	s;
#ifdef HAVE_LSTAT
	int	namelen;
	char	nbuf[NFILEN];
#endif

	d = tilde(fn, buf, sizeof(buf));
	if (d == 0)
		d = fn;
	if (writable(d) == FALSE) {
		mlwrite("Cannot open %s for writing: file is read-only", d);
		return(FIOERR);
	}
#ifdef HAVE_LSTAT
	if (followlinks && (s = lstat(d, &stb)) == 0 && S_ISLNK(stb.st_mode)) {
		/* Translate d to a new pathname using readlink(2). */
		namelen = readlink(d, nbuf, sizeof(nbuf));
		if (namelen >= sizeof(nbuf)) {
			mlwrite("Cannot open %s for writing: file name is too long", d);
			return (FIOERR);
		} else if (namelen < 0) {
			mlwrite("Cannot open %s for writing: %s", d, strerror(errno));
			return (FIOERR);
		}
		nbuf[namelen] = '\0';
		d = nbuf;
	}
#endif
	s = stat(d, &stb);
	if (s < 0) {	/* preserve any existing modes */
		stb.st_mode = fiofmode() | S_IFREG;
		stb.st_uid = UID;
		stb.st_gid = GID;
#ifdef HAVE_ST_BLKSIZE
		stb.st_blksize = stb.st_size = DEFBLKSIZE; /* safe assumption? */
#else
		stb.st_size = DEFBLKSIZE; /* safe assumption? */
#endif
	} else if (chkfile(d, stb) == FIOERR)
		return FIOERR;

	/*
	 * Try to write output in chunks equal in size to the optimal file
	 * system blocksize.
	 */
#ifdef HAVE_ST_BLKSIZE
	if (fiobsize && fiobsize < stb.st_blksize) {
#else
	if (fiobsize && fiobsize < DEFBLKSIZE) {
#endif
		/*
		 * If we've allocated a buffer already, but that buffer is
		 * too small for this file system's blocksize, force a
		 * reallocation with the new (bigger) size.
		 */
		if (fiobuf)
			free(fiobuf);
		fiobuf = (unsigned char *)NULL;
	}
#ifdef HAVE_ST_BLKSIZE
	fiobsize = stb.st_blksize;
#else
	fiobsize = DEFBLKSIZE;
#endif
	if (backup) {			/* back it up before we blow it away */
		s = fbackupfile(d);
		if (s == ABORT || s == FALSE) {
			if (mlyesno("Backup error.  Save anyway") == FALSE)
				return FIOERR;
		}
	}
	if ((ffd = fiowopen(d)) < 0) {
		mlwrite("Cannot open %s for writing: %s", d, strerror(errno));
		return (FIOERR);
	}
	(void) strcpy(curfn, d);
	fiomode |= FIOWRITE;
	return (FIOSUC);
}

/*
 * Return TRUE if a file can be written to.  If the file does not exist,
 * return TRUE if we can write to the directory.
 */

int
writable(fname)
char    *fname;
{
	char		*fn, buf[NFILEN], dir[NFILEN], *cp;

	if (fname == NULL || *fname == '\0')
		return FALSE;
	fn = tilde(fname, buf, sizeof(buf));
	if (fn == 0)
		fn = fname;
	if (access(fn, F_OK) >= 0)		/* File exists */
		return (access(fn, W_OK) >= 0);
	cp = strrchr(fn, '/');
	if (cp == NULL) {
		strncpy(dir, wdir, sizeof(dir) - 1);
		dir[sizeof(dir) - 1] = '\0';
	} else {
		*cp = '\0';
		strncpy(dir, fn, sizeof(dir) - 1);
		dir[sizeof(dir) - 1] = '\0';
		*cp = '/';
	}
	return (access(dir, W_OK) >= 0);
}

/*
 * Close a file. Should look at the status in all systems, especially NFS.
 * Restore the modes (so we can edit executable shell scripts and have 
 * them remain executable) and the owner and group ids.  These attributes
 * should be in a structure along with the file name; serious problems 
 * will arise when multiple files are edited.
 */
int
ffclose()
{
	char	w = fiomode & FIOWRITE;

#if defined (HAVE_FCHMOD)
	if (w) {
		if (fchmod(ffd, stb.st_mode & 0777) < 0)
			failed("fchmod", curfn, errno);
	}
#endif /* HAVE_FCHMOD */

#if defined (HAVE_FCHOWN)
	if (w) {
#if defined (_POSIX_CHOWN_RESTRICTED)
		if ((UID == 0) && (fchown(ffd, (int) stb.st_uid, (int) stb.st_gid) < 0))
#else
		if (fchown(ffd, (int) stb.st_uid, (int) stb.st_gid) < 0)
#endif /* _POSIX_CHOWN_RESTRICTED */
			failed("fchown (uid/gid)", curfn, errno);
	}
#endif /* HAVE_FCHOWN */

	if (fioclose(ffd) != 0) {
		mlwrite("Error closing file: %s", strerror(errno));
		return(FIOERR);
	}

#if !defined (HAVE_FCHMOD)
	if (w) {
		if (chmod(curfn, stb.st_mode & 0777) < 0)
			failed("chmod", curfn, errno);
	}
#endif

#if !defined (HAVE_FCHOWN)
	if (w) {
		if (chown(curfn, (int) stb.st_uid, (int) stb.st_gid) < 0)
			failed("chown (uid/gid)", curfn, errno);
	}
#endif
	ffd = -1;
	return(FIOSUC);
}

/*
 * Write text to the already opened file. The "buf" points to the buffer,
 * and the "nbuf" is its length.
 */
int
ffputbuf(buf, nbuf)
char    buf[];
int	nbuf;
{
	register int    i;

	if (fiobuf == NULL)
		allocbuf();
	for (i = 0; i < nbuf; ++i)
		fioputc(buf[i]&0xFF, ffd);
	return (fioerr(ffd) ? FIOERR : FIOSUC);
}

/*
 * Write a line to the already opened file. The "buf" points to the buffer,
 * and the "nbuf" is its length, less the free newline. Return the status.
 * Check only at the newline.
 */
int
ffputline(buf, nbuf)
char    buf[];
{
	ffputbuf(buf, nbuf);
	fioputc('\n', ffd);
	if (fioerr(ffd)) {
		mlwrite("Write I/O error: %s", strerror(errno));
		return (FIOERR);
	}
	return (FIOSUC);
}

/*
 * Read a line from a file, dynamically allocating storage for the characters,
 * and store the bytes in the supplied buffer. Complain about lines at the end
 * of the file that don't have a newline present. Check for I/O errors too.
 * Return status of the read.  Return the actual number of bytes read in the
 * integer pointed to by `nbytes'.  FIOEOF is returned with the buffer full;
 * the eof is not `saved'.  This has implications for all routines that call
 * this one; they should all handle it compatibly.
 */
int
ffgetline(bufp, nbytes)
register char   **bufp;
int 		*nbytes;
{
	register char	*line;

	errno = 0;
	line = (char *)fiogetline(nbytes);
	if (line == NULL) {
		*bufp = (char *) NULL;
		*nbytes = 0;
		if (fioeof(ffd))
			return (FIOEOF);
		else {
			(*term.t_beep)();
			if (fionomem())
				mlwrite("Out of memory reading file");
			else
				mlwrite("File read error: %s", strerror(errno));
			return FIOERR;
		}
	}
	*bufp = line;
	return (FIOSUC);
}

/*
 * Make a backup copy of the file fn.  The name is suffixed with a '~', as 
 * this seems to be the way GNU does it.
 */
int
fbackupfile(fn)
char *fn;
{
	register char *name;
	int	len, maxlen;

	len = strlen(fn);
	if ((name = (char *) malloc(len + 2)) == NULL) {
		mlwrite("Cannot allocate %d bytes for backup", len + 2);
		return(ABORT);
	}
#if 0
	maxlen = pathconf(fn, _PC_NAME_MAX);
	if (len >= maxlen) {
		mlwrite("backup name too long; truncating name");
		len = maxlen - 1;
	}
#endif
	(void) strcpy(name, fn);
	name[len] = '~';
	name[len+1] = '\0';
	if (rename(fn, name) < 0) {
		(*term.t_beep)();
		mlwrite("Could not make backup file: %s", strerror(errno));
		free(name);
		return(FALSE);
	}
	free(name);
	return(TRUE);
}
