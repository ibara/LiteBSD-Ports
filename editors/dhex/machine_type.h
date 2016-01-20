#ifndef MACHINE_TYPE_H
#define MACHINE_TYPE_H
#include <stdio.h>
#include <ncurses.h>
// number 1: datatypes

typedef	unsigned char 		tBool;
typedef	unsigned char 		tUInt8;
typedef unsigned short		tUInt16;
typedef	unsigned int		tUInt32;
typedef	unsigned long long	tUInt64;

typedef	signed char 		tInt8;
typedef signed short		tInt16;
typedef	signed int		tInt32;
typedef	signed long long	tInt64;

typedef	FILE*			tFptr;

// number 2: file operations
tUInt64	getfilepos(tFptr f);
tUInt64 getfilesize(tFptr f);
void	setfilepos(tFptr f,tUInt64 pos);

#define	MIN(a,b)	(((a)<(b))?(a):(b))
#define	MAX(a,b)	(((a)>(b))?(a):(b))
#define	RETOK	0
#define	RETNOK	-1
#endif
