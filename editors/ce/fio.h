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
 * fio.h -- definitions for the FIO package
 */

#ifndef _FIO_H
#define _FIO_H

#ifdef NULL
#undef NULL
#endif

#define NULL	0
#define EOF	(-1)

#define F_FIOEOF	0x01
#define F_FIOERR	0x02
#define F_FIONOMEM	0x04

#define FIOREAD		0x01
#define FIOWRITE	0x02

#define FIOMODE		0666
#define DEFBLKSIZE	8192

/*
 * These defines are based on those found in the Unix stdio.h file.
 */
extern	int	fioflush();
extern	int	repopbuf();

#define fiogetc(fd)	(--fiocnt >= 0 ? (fiobuf[fiop++] & 0xFF) : repopbuf(fd))
#define fioputc(c,fd)	((fiop < fiobsize) ? fiobuf[fiop++] = (c) : fioflush((c), (fd)))
#define fioeof(fd)	((fioflg & F_FIOEOF) != 0)
#define fioerr(fd)	((fioflg & F_FIOERR) != 0)
#define fionomem()	((fioflg & F_FIONOMEM) != 0)

#define fioropen(fn)	(open(fn, O_RDONLY, FIOMODE))
#define fiowopen(fn)	(open(fn, O_TRUNC|O_CREAT|O_WRONLY, FIOMODE))

#define fiosetbsize(size)	fiobsize = (size)

extern int 	fiocnt;
extern int 	fiop;
extern short	fioflg;
extern short	fiomode;
extern unsigned char	*fiobuf;			/* should it be int? */
extern int	fiobsize;

extern uid_t	UID;
extern uid_t	GID;

extern unsigned char	*fiogetline();

#endif /* _FIO_H */
