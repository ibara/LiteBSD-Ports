/* @(#)symtab.h 1.4 97/03/05 */
/*
 * ====================================================
 * Copyright (C) 1995 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunSoft, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

#define MAXNAMELEN	31
#define MAXSYMTAB	1500

#define CL_FPP		1	/* fpp directives */
#define CL_KEY		2	/* Fortran keywords */
#define CL_FOP		4	/* Foprtran operations */
#define CL_NM		8	/* macro name */

#define FPPDEF		1
#define FPPUNDEF	2
#define FPPINCL		3
#define FPPIF		4
#define FPPELIF		5
#define FPPELSE		6
#define FPPENDIF	7
#define FPPIFDEF	8
#define FPPIFNDEF	9
#define FPPLINE		10
#define FPPDYNMACRO	11

#define FTN_TRUE	50
#define FTN_FALSE	51
#define FTN_EQ		52
#define FTN_NE		53
#define FTN_LT		54
#define FTN_LE		55
#define FTN_GT		56
#define FTN_GE		57
#define FTN_AND		58
#define FTN_OR		59
#define FTN_NEQV	60
#define FTN_XOR		61
#define FTN_EQV		62
#define FTN_NOT		63
#define FTN_PRAGMA	64

#define FTN_ACCEPT	100
#define FTN_ASSIGN	101
#define FTN_TO		102
#define FTN_AUTOMATIC	103
#define FTN_BACKSPACE	104
#define FTN_BLOCKDATA	106
#define FTN_DATA	107
#define FTN_BYTE	108
#define FTN_CALL	109
#define FTN_CHARACTER	110
#define FTN_CLOSE	111
#define FTN_COMMON	112
#define FTN_COMPLEX	113
#define FTN_CONTINUE	114
#define FTN_DECODE	115
#define FTN_DIMENSION	116
#define FTN_DO		117
#define FTN_DOWHILE	118
#define FTN_DOUBLECOMP	119
#define FTN_DOUBLEPREC	120
#define FTN_ELSE	121
#define FTN_ELSEIF	122
#define FTN_END		123
#define FTN_ENDDO	124
#define FTN_ENDFILE	125
#define FTN_ENDIF	126
#define FTN_ENDMAP	127
#define FTN_ENDSTRUCTURE 128
#define FTN_ENDUNION	129
#define FTN_ENCODE	130
#define FTN_ENTRY	131
#define FTN_EQUIVALENCE	132
#define FTN_EXTERNAL	133
#define FTN_FORMAT	134
#define FTN_FUNCTION	135
#define FTN_GOTO	136
#define FTN_IF		137
#define FTN_IMPLICIT	138
#define FTN_INCLUDE	139 
#define FTN_INQUIRE	140
#define FTN_INTEGER	141
#define FTN_INTRINSIC	142
#define FTN_LOGICAL	143
#define FTN_MAP		144
#define FTN_NAMELIST	145
#define FTN_OPEN	146
#define FTN_OPTIONS	147
#define FTN_PARAMETER	148
#define FTN_PAUSE	149
#define FTN_POINTER	150
#define FTN_PROGRAM	151
#define FTN_PRINT	152
#define FTN_READ	153
#define FTN_REAL	154
#define FTN_RECORD	155
#define FTN_RETURN	156
#define FTN_REWIND	157
#define FTN_SAVE	158
#define FTN_STATIC	159
#define FTN_STOP	160
#define FTN_STRUCTURE	161
#define FTN_SUBROUTINE	162
#define FTN_TYPE	163
#define FTN_UNION	164
#define FTN_VIRTUAL	165
#define FTN_VOLATILE	166
#define FTN_WRITE	167

#define FTN77_FST	500
#define FTN90_FST	1000

typedef struct _sym Sym;
typedef Sym *SymPtr;

SymPtr symget( char*, int);
SymPtr symgetm( char*, int);
char *symname(SymPtr);
char *symvals(SymPtr);
int   symvali(SymPtr);
int   symtype(SymPtr s);
int   symflag(SymPtr s);
int   symrecurse(SymPtr);
void  sympush();
void  sympop();
void  symhide();
void  symunhide();
void  symsetnm( char *, char *);
void  symsetkw( char *, int, int);
void  symsetvals(SymPtr, char *);
void  symdel( char *);
