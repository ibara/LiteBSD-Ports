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
 * @(#)nmalloc.c 1 (Caltech) 2/21/82
 *
 *	U of M Modified: 20 Jun 1983 ACT: strange hacks for Emacs
 *
 *	Nov 1983, Mike@BRL, Added support for 4.1C/4.2 BSD.
 *
 * This is a very fast storage allocator.  It allocates blocks of a small 
 * number of different sizes, and keeps free lists of each size.  Blocks
 * that don't exactly fit are passed up to the next larger size.  In this 
 * implementation, the available sizes are (2^n)-4 (or -16) bytes long.
 * This is designed for use in a program that uses vast quantities of
 * memory, but bombs when it runs out.  To make it a little better, it
 * warns the user when he starts to get near the end.
 *
 * June 84, ACT: modified rcheck code to check the range given to malloc,
 * rather than the range determined by the 2-power used.
 *
 * Jan 85, RMS: calls mallwarn to issue warning on nearly full.
 * No longer Emacs-specific; can serve as all-purpose malloc for GNU.
 * You should call malloc_init to reinitialize after loading dumped Emacs.
 * Call malloc_stats to get info on memory stats if MSTATS turned on.
 * realloc knows how to return same block given, just changing its size,
 * if the power of 2 is correct.
 */

#include <config.h>
#include <stdio.h>
#include <sys/types.h>

#ifdef M_WARN
#include <sys/time.h>
#include <sys/resource.h>
#endif

#ifndef RLIMIT_DATA
#undef M_WARN
#endif

#define NULL 0

/*
 * nextf[i] is the pointer to the next free block of size 2^(i+3).  The
 * smallest allocatable block is 8 bytes.  The overhead information will
 * go in the first int of the block, and the returned pointer will point
 * to the second.
 */

#define ISALLOC ((char) 0xf7)	/* magic byte that implies allocation */
#define ISFREE ((char) 0x54)	/* magic byte that implies free block */
				/* this is for error checking only */

extern caddr_t sbrk();

extern char	etext;		/* end of program text */
static char	*endofpure = &etext;

static int	mwarn = 0;
/* 
 * If range checking is not turned on, all we have is a flag indicating
 * whether memory is allocated, an index in nextf[], and a size field; to
 * realloc() memory we copy either size bytes or 1<<(index+3) bytes depending
 * on whether the former can hold the exact size (given the value of
 * 'index').  If range checking is on, we always need to know how much space
 * is allocated, so the 'size' field is never used.
 */

struct mhead {
	char     mh_alloc;	/* ISALLOC or ISFREE */
	char     mh_index;	/* index in nextf[] */
/* Remainder are valid only when block is allocated */
	unsigned short mh_size;	/* size, if < 0x10000 */
#ifdef rcheck
	unsigned mh_nbytes;	/* number of bytes allocated */
	int      mh_rmagic;	/* should be == RMAGIC */
#endif /* rcheck */
};

/*
 * Access free-list pointer of a block.  It is stored at block + 4.  This is 
 * not a field in the mhead structure because we want sizeof (struct mhead)
 * to describe the overhead for when the block is in use, and we do not want 
 * the free-list pointer to count in that.
 */

#define CHAIN(a) \
  (*(struct mhead **) (sizeof (char *) + (char *) (a)))

#ifdef rcheck

/* 
 * To implement range checking, we write magic values in at the beginning and
 * end of each allocated block, and make sure they are undisturbed whenever a
 * free or a realloc occurs.
 */

/* Written in each of the 4 bytes following the block's real space */
#define MAGIC 0x55
/* Written in the 4 bytes before the block's real space */
#define RMAGIC 0x55555555
#define ASSERT(p) if (!(p)) botch("p"); else

#define EXTRA  4		/* 4 bytes extra for MAGICs */
#else
#define ASSERT(p)
#define EXTRA  0
#endif /* rcheck */

static
botch(s)
char *s;
{
	fprintf(stderr,"assertion botched: %s\n", s);
	abort();
}

/* nextf[i] is free list of blocks of size 2**(i + 3)  */

#define	NBUCKET	30
static struct mhead *nextf[NBUCKET];

#ifdef	M_WARN
/* Number of bytes of writable memory we can expect to be able to get */
static int  lim_data;
/* Level number of warnings already issued.
  0 -- no warnings issued.
  1 -- 75% warning already issued.
  2 -- 85% warning already issued.
*/
static int  warnlevel;

static void
mallwarn(s)
char	*s;
{
	mlwrite(s);
}
#endif /* M_WARN */

/*
 * Turn on malloc warning
 */
meminit()
{
	mwarn = 1;
}

/* nonzero once initial bunch of free blocks made */
static int gotpool;

static
getpool ()
{
	register int nu;
	register char *cp = (char *) sbrk(0);

	if ((int) cp & 0x3ff)	/* land on 1K boundaries */
	    (void) sbrk (1024 - ((int) cp & 0x3ff));

	/* Get 2k of storage */

	cp = (char *) sbrk(04000);
	if (cp == (char *) -1)
	    return;

	/* Divide it into an initial 8-word block
	plus one block of size 2**nu for nu = 3 ... 10.  */

	CHAIN (cp) = nextf[0];
	nextf[0] = (struct mhead *) cp;
	((struct mhead *) cp) -> mh_alloc = ISFREE;
	((struct mhead *) cp) -> mh_index = 0;
	cp += 8;

	for (nu = 0; nu < 7; nu++) {
		CHAIN (cp) = nextf[nu];
		nextf[nu] = (struct mhead *) cp;
		((struct mhead *) cp) -> mh_alloc = ISFREE;
		((struct mhead *) cp) -> mh_index = nu;
		cp += 8 << nu;
	}
}


static
morecore (nu)			/* ask system for more memory */
register int nu; 		/* size index to get more of  */
{
	register char  *cp;
	register int    nblks;
	register int    siz;

#ifdef	M_WARN
	if (mwarn && lim_data == 0) {
		struct rlimit   XXrlimit;

		getrlimit (RLIMIT_DATA, &XXrlimit);
		lim_data = XXrlimit.rlim_cur;	/* soft limit */
	}
#endif /* M_WARN */

	/* On initial startup, get two blocks of each size up to 1k bytes */
	if (!gotpool) {
		getpool();
		getpool();
		gotpool = 1;
	}

	/* Find current end of memory and issue warning if getting near max */

	cp = (char *) sbrk(0);
	siz = cp - endofpure;
#ifdef	M_WARN
	if (mwarn) {
		switch (warnlevel) {
		    case 0: 
			if (siz > (lim_data / 4) * 3) {
				warnlevel++;
				mallwarn ("Warning: past 75%% of memory limit");
			}
			break;
		    case 1: 
			if (siz > (lim_data / 20) * 17) {
				warnlevel++;
				mallwarn ("Warning: past 85%% of memory limit");
			}
			break;
		    case 2: 
			if (siz > (lim_data / 20) * 19) {
				warnlevel++;
				mallwarn ("Warning: past 95%% of memory limit");
			}
			break;
		}
	}
#endif /* M_WARN */

	if ((int) cp & 0x3ff)	/* land on 1K boundaries */
	    (void) sbrk (1024 - ((int) cp & 0x3ff));

	/* Take at least 2k, and figure out how many blocks of the desired size we're about to get */
	nblks = 1;
	if ((siz = nu) < 8)
	    nblks = 1 << ((siz = 8) - nu);

	if ((cp = (char *) sbrk(1 << (siz + 3))) == (char *) -1)
	    return;			/* no more room! */
	if ((int) cp & 7) {		/* shouldn't happen, but just in case */
		cp = (char *) (((int) cp + 8) & ~7);
		nblks--;
	}
	/* save new header and link the nblks blocks together */
	nextf[nu] = (struct mhead *) cp;
	siz = 1 << (nu + 3);
	while (1) {
		((struct mhead *) cp) -> mh_alloc = ISFREE;
		((struct mhead *) cp) -> mh_index = nu;
		if (--nblks <= 0) break;
		CHAIN ((struct mhead *) cp) = (struct mhead *) (cp + siz);
		cp += siz;
	}
}

char *
malloc (n)		/* get a block */
unsigned n; 
{
	register struct  mhead *p;
	register unsigned int  nbytes;
	register int    nunits = 0;

	/* Figure out how many bytes are required, rounding up to the nearest
	multiple of 4, then figure out which nextf[] area to use */
	nbytes = (n + sizeof *p + EXTRA + 3) & ~3;
	{
		register unsigned int   shiftr = (nbytes - 1) >> 2;

		while (shiftr >>= 1)
		    nunits++;
	}

	/* If there are no blocks of the appropriate size, go get some */
	/* COULD SPLIT UP A LARGER BLOCK HERE ... ACT */
	if (nextf[nunits] == 0)
	    morecore (nunits);

	/* Get one block off the list, and set the new list head */
	if ((p = nextf[nunits]) == 0)
	    return NULL;
	nextf[nunits] = CHAIN (p);

	/* Check for free block clobbered */
	/* If not for this check, we would gobble a clobbered free chain ptr */
	/* and bomb out on the NEXT allocate of this size block */
	if (p -> mh_alloc != ISFREE || p -> mh_index != nunits)
#ifdef rcheck
	    botch ("block on free list clobbered");
#else
	    abort ();
#endif /* rcheck */

	/* Fill in the info, and if range checking, set up the magic numbers */
	p -> mh_alloc = ISALLOC;
#ifdef rcheck
	p -> mh_nbytes = n;
	p -> mh_rmagic = RMAGIC;
	{
		register char  *m = (char *) (p + 1) + n;

		*m++ = MAGIC, *m++ = MAGIC, *m++ = MAGIC, *m = MAGIC;
	}
#else
	p -> mh_size = n;
#endif /* rcheck */
	return (char *) (p + 1);
}

free (mem)
char *mem;
{
	register struct mhead *p;

	if (mem == NULL)
		return;
	{
		register char *ap = mem;

		p = (struct mhead *) ap - 1;
		if (p -> mh_alloc != ISALLOC) {
			if (p->mh_alloc == ISFREE)
				botch("Freeing freed block");
			else
				botch("Bad block passed to free");
		}
#ifdef rcheck
		ASSERT (p -> mh_rmagic == RMAGIC);
		ap += p -> mh_nbytes;
		ASSERT (*ap++ == MAGIC); ASSERT (*ap++ == MAGIC);
		ASSERT (*ap++ == MAGIC); ASSERT (*ap   == MAGIC);
#endif /* rcheck */
	}
	{
		register int nunits = p -> mh_index;

		ASSERT (nunits < NBUCKET);
		p -> mh_alloc = ISFREE;
		CHAIN (p) = nextf[nunits];
		nextf[nunits] = p;
	}
}

char *
realloc (mem, n)
char *mem;
register unsigned n;
{
	register struct mhead *p;
	register unsigned int tocopy;
	register int nbytes;
	register int nunits;

	if ((p = (struct mhead *) mem) == NULL)
	    return malloc (n);
#ifdef ANSI_REALLOC
	if ((p != NULL) && (n == 0)) {	    /* required for ansi behavior */
	    (void) free(p);
	    return ((char *) NULL);
	}
#endif
	p--;
	nunits = p -> mh_index;
	ASSERT (p -> mh_alloc == ISALLOC);
#ifdef rcheck
	ASSERT (p -> mh_rmagic == RMAGIC);
	{
		register char *m = mem + (tocopy = p -> mh_nbytes);

		ASSERT (*m++ == MAGIC); ASSERT (*m++ == MAGIC);
		ASSERT (*m++ == MAGIC); ASSERT (*m   == MAGIC);
	}
#else
	if (p -> mh_index >= 13)
	    tocopy = (1 << (p -> mh_index + 3)) - sizeof *p;
	else
	    tocopy = p -> mh_size;
#endif /* rcheck */

	/* See if desired size rounds to same power of 2 as actual size. */
	nbytes = (n + sizeof *p + EXTRA + 7) & ~7;

	/* If ok, use the same block, just marking its size as changed.  */
	if (nbytes > (4 << nunits) && nbytes <= (8 << nunits)) {
#ifdef rcheck
		register char *m = mem + tocopy;
		*m++ = 0;  *m++ = 0;  *m++ = 0;  *m++ = 0;
		p-> mh_nbytes = n;
		m = mem + n;
		*m++ = MAGIC;  *m++ = MAGIC;  *m++ = MAGIC;  *m++ = MAGIC;
#else
		p -> mh_size = n;
#endif /* rcheck */
		return mem;
	}

	if (n < tocopy)
	    tocopy = n;
	{
		register char *new;
#ifdef HAVE_BCOPY
		extern void bcopy();	/*HMS: here? */
#endif

		if ((new = malloc (n)) == 0)
		    return 0;
		bcopy (mem, new, tocopy);
		free (mem);
		return new;
	}
}

/*
 * calloc - allocate memory block and zero-fill it.
 */

char *
calloc(num, size)
register unsigned num;
register unsigned size;
{
	register char *p;

	size *= num;
	if (p = malloc(size))
		bzero(p, size);
	return (p);
}

/*
 * cfree - free a calloc'd region of memory
 */

cfree(p, num, size)
char *p;
unsigned num;
unsigned size;
{
	free(p);
}
