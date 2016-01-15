/* @(#)fpp.h 1.13 96/09/10 */
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

#include <stdlib.h>
#include <string.h>

#ifndef DEBUG
#define DEBUG	0	/* DEBUG level */
#endif

#define STDINNAME   "stdin"
#define STDOUTNAME  "stdout"
#define STDERRNAME  "stderr"
#define CURDIR      "."
#define STDINCLUDE  "/usr/include"
#define F90EXTENSION ".F90"
#define F77EXTENSION ".F"

#define BUFSIZE 512
#define MAXTOKENSIZE BUFSIZE
#define LINE_WIDTH_STD 72
#define LINE_WIDTH_EXT 132
#define LINE_WIDTH_MAX 32767

/* Control characters */
#define SYM_CTRL  0xff
#define	SYM_FPP	  '#'
#define SYM_EOF   ((SYM_CTRL << 8) | 0x01)
#define SYM_DUMMY ((SYM_CTRL << 8) | 0x02)	/* just for lexem delimiting */
#define SYM_BOS   ((SYM_CTRL << 8) | 0x03)	/* begin of a statement */
#define SYM_EOMS  ((SYM_CTRL << 8) | 0x04)	/* end of macro substitution */
#define SYM_BF    ((SYM_CTRL << 8) | 0x05)	/* begin of a formal */
#define SYM_EF	  ((SYM_CTRL << 8) | 0x06)	/* end of a formal */
#define SYM_EOC   ((SYM_CTRL << 8) | 0x07)	/* end of comment */

#define MOD_DUMMY	0	/* dummy mode */
#define MOD_SPACE	0x0001	/* remove all spaces */
#define MOD_LCASE	0x0002	/* to lower case */
#define MOD_CONT	0x0004	/* check if continued to next line */
#define MOD_RAW		0x0008	/* padding spaces to end of FORTRAN line */
#define MOD_FPP		0x0010	/* when in fpp directive */
#define MOD_IF		0x0020	/* when in fpp #if */
#define MOD_NOGRAM	0x0040	/* don't check grammar */
#define MOD_COM         0x0080  /* when macro expanding in comments */

#include "msgno.h"

#if DEBUG
#define CHECK(val) { if (!(val)) \
	fppmess(FERR_CHCK,__FILE__,__LINE__); }
#else
#define CHECK(val)
#endif /* DEBUG */

/************************************************************* Types */

typedef unsigned char uchar;
typedef unsigned int  wchar;

typedef struct _token {
        int     lineno;
        int     val;
        int     length;
        int     stid;
        char    token[MAXTOKENSIZE];
} Token;

typedef struct _comment {
	struct _comment *next;
        int     lineno;
        int     length;
	char	*str;
} Comment;

/************************************************************* Globals */
#if DEBUG
extern int      debug;          /* debug level */
#endif
extern unsigned	mmode;		/* main process mode */
extern unsigned line;		/* current line of the current file */
extern int	colm;		/* current column in the line , may be < 0*/
extern int	line_width0;	/* Fortran line width 72 or 132 */
extern int	line_width;	/* current line maximum width */

extern int	f77fl;		/* 1 - F77, 0 - F90 */
extern int	fixedformfl;	/* 1 - fixed form, 0 - free form */
extern int	outfl;		/* prohibits output when non zero */
extern int      falselvl;       /* false level in nested #if's */
extern int      substfl;        /* macro substitution mode (plain text):
				 * 0 - never, 1 - no comments, 2 - always */
extern int	dosubstfl;	/* force macro substitution, no matter what
				 * value the flag above has */
extern int	rmcommfl;	/* remove comments */
extern int      no_ccom_fl;     /* don't recognize c style comments */
extern int	linefl;		/* output # file, line information */
extern int	ignorelinefl;	/* ignore input #line directives */
extern int	sbfl;		/* output sbrowser info */
extern int	warnfl;		/* do not print warnings */
extern int	predefinefl;	/* do predefining */
extern int	mdepfl;		/* generate make dependencies */
extern int	errnum;		/* number of errors */
extern void	*onequalsym;	/* assignment substitution */
extern int	onequalline;	/* assignment substitution line */
extern int      ctrl_mode;      /* control chars processing mode */
/************************************************************* Functions */

void   my_exit(int);
int    yyparse();
                                /* functions from error.c */
void   err_init(int);
void   err_fini();
void   fppmess(int, ...);
				/* functions from proc.c */
char *get_cur_fname();
int  get_err_lineno();
void my_fflush( int );
void refill();
void ungetstr( char *, int );
void ungetsym( wchar );
void outtok();
void skiptok();
Token *skipbl( int );
int  substitute();
void set_pos(int);
int  add_path( char * );
void init0();
void init1();
void process();
				/* functions from lex.c */
Token  *get_token(unsigned);
Token  *get_cur_tok();
void    unget_tok();
void    pushtok();
void    poptok();
