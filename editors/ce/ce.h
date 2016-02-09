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
 *  ce.h -- Global variable definitions for ce
 */

#include "copyright.h"
#include <config.h>

/*
 * Location of the local library directory that contains the ce help files
 */

#ifndef LIBDIR
#define LIBDIR "/usr/local/lib/ce"
#endif

#define TUTORIAL	"/TUTORIAL"
#define FUNCTIONS	"/FUNCTIONS"
#define HELPFILE	"/ce.help"

/*	internal constants	*/

#define NSHASH  31		/* Symbol table hash size.*/
#define	NBINDS	200		/* max # of bound keys		*/
#define NNAMES  256		/* max # of callable functions  */
#define NFILEN  512		/* # of bytes, file name	*/
#define NBUFN   32		/* # of bytes, buffer name*/
#define NLINE   256		/* # of bytes, line	*/
#define	NSTRING	256		/* # of bytes, string buffers	*/
#define NKBDM   256		/* # of strokes, keyboard macro */
#define NPAT    80		/* # of bytes, pattern	  */
#define HUGE    1000		/* Huge number		  */
#define	NLOCKS	100		/* max # of file locks active	*/

#define AGRAVE  0x60		/* M- prefix,   Grave (LK201)   */
#define METACH  0x1B		/* M- prefix,   Control-[, ESC  */
#define CTMECH  0x1C		/* C-M- prefix, Control-\*/
#define EXITCH  0x1D		/* Exit level,  Control-]*/
#define CTRLCH  0x1E		/* C- prefix,   Control-^*/
#define HELPCH  0x1F		/* Help key,    Control-_*/

#define CNTL    0x0100		/* Control flag, or'ed in*/
#define META    0x0200		/* Meta flag, or'ed in	  */
#define CTLX    0x0400		/* ^X flag, or'ed in	    */
#define	SPEC	0x0800		/* special key (function keys)	*/

#define FALSE   0		/* False, no, bad, etc.	 */
#define TRUE    1		/* True, yes, good, etc.	*/
#define ABORT   2		/* Death, ^G, abort, etc.*/

#define FIOSUC  	0	/* File I/O, success.	   */
#define FIOFNF  	1	/* File I/O, file not found.    */
#define FIOEOF  	2	/* File I/O, end of file.*/
#define FIOERR  	3	/* File I/O, error.	*/
#define	FIOLNG		4	/* line longer than allowed len	*/
#define FIORDONLY	5	/* File I/O, file is read-only */

#define CFCPCN  0x0001		/* Last command was C-P, C-N    */
#define CFKILL  0x0002		/* Last command was a kill*/

#define	BELL	0x07		/* a bell character		*/
#define	TAB	0x09		/* a tab character		*/

#define NROW    150		/* Maximum number of rows*/
#define NCOL    150		/* Maximum number of columns    */
#define MARGIN  8		/* Margin Size		  */
#define SCRSIZE 64		/* Size of scroll region (unused) */

#define CNONE   0		/* Unknown color		*/
#define CTEXT   1		/* Color of text		*/
#define CMODE   2		/* Mode line color	*/

#define CCHR(x) ((x) ^ 0x40)

#define CMASK(c) ((unsigned char) (c))
/*
 * These flags, and the macros below them,
 * make up a do-it-yourself set of "ctype" macros that
 * understand the DEC multinational set, and let me ask
 * a slightly different set of questions.
 */
#define	_W	0x01			/* Word.			*/
#define	_U	0x02			/* Upper case letter.		*/
#define	_L	0x04			/* Lower case letter.		*/
#define	_C	0x08			/* Control.			*/
#define _D	0x10			/* Decimal Digit (0-9)		*/
#define _P	0x20			/* End-of sentence punctuation  */
#define _S	0x40			/* Whitespace character.	*/

#define	ISWORD(c)	((cinfo[CMASK(c)]&_W)!=0)
#define	ISCTRL(c)	((cinfo[CMASK(c)]&_C)!=0)
#define	ISUPPER(c)	((cinfo[CMASK(c)]&_U)!=0)
#define	ISLOWER(c)	((cinfo[CMASK(c)]&_L)!=0)
#define ISDIGIT(c)	((cinfo[CMASK(c)]&_D)!=0)
#define ISEOSP(c)	((cinfo[CMASK(c)]&_P)!=0)
#define ISWHSP(c)	((cinfo[CMASK(c)]&_S)!=0)
#define	TOUPPER(c)	((c)-0x20)
#define	TOLOWER(c)	((c)+0x20)

/*
 * There is a window structure allocated for every active display window. The
 * windows are kept in a big list, in top to bottom screen order, with the
 * listhead at "wheadp". Each window contains its own values of dot and mark.
 * The flag field contains some bits that are set by commands to guide
 * redisplay; although this is a bit of a compromise in terms of decoupling,
 * the full blown redisplay is just too expensive to run for every input
 * character.
 */
typedef struct  WINDOW {
	struct  WINDOW *w_wndp;		/* Next window		  */
	struct  BUFFER *w_bufp;		/* Buffer displayed in window   */
	struct  LINE *w_linep;		/* Top line in the window*/
	struct  LINE *w_dotp;		/* Line containing "."	  */
	int	w_doto;			/* Byte offset for "."	  */
	struct  LINE *w_markp;		/* Line containing "mark"*/
	int	w_marko;		/* Byte offset for "mark"*/
	char    w_toprow;		/* Origin 0 top row of window   */
	char    w_ntrows;		/* # of rows of text in window  */
	char    w_force;		/* If NZ, forcing row.	  */
	char    w_flag;			/* Flags.		*/
} WINDOW;

#define WFFORCE 0x01		    /* Window needs forced reframe  */
#define WFMOVE  0x02		    /* Movement from line to line   */
#define WFEDIT  0x04		    /* Editing within a line	*/
#define WFHARD  0x08		    /* Better to a full display*/
#define WFMODE  0x10		    /* Update mode line.	    */

/*
 * Text is kept in buffers. A buffer header, described below, exists for every
 * buffer in the system. The buffers are kept in a big list, so that commands
 * that search for a buffer by name can find the buffer header. There is a
 * safe store for the dot and mark in the header, but this is only valid if
 * the buffer is not being displayed (that is, if "b_nwnd" is 0). The text for
 * the buffer is kept in a circularly linked list of lines, with a pointer to
 * the header line in "b_linep".
 *
 * Buffers may be "Inactive" which means the files associated with them
 * have not been read in yet. These get read in at "use buffer" time.
 */
typedef struct  BUFFER {
	struct  BUFFER *b_bufp;		/* Link to next BUFFER	  */
	struct  LINE *b_dotp;		/* Link to "." LINE structure   */
	int	b_doto;			/* Offset of "." in above LINE  */
	struct  LINE *b_markp;		/* The same as the above two,   */
	int	b_marko;		/* but for the "mark"	   */
	struct  LINE *b_linep;		/* Link to the header LINE*/
	char	b_active;		/* window activated flag	*/
	char    b_nwnd;			/* Count of windows on buffer   */
	char    b_flag;			/* Flags			*/
	char	b_mode;			/* editor mode of this buffer	*/
	char    b_fname[NFILEN];	/* File name		    */
	char    b_bname[NBUFN];		/* Buffer name		  */
} BUFFER;

#define BFTEMP  0x01			/* Internal temporary buffer    */
#define BFCHG   0x02			/* Changed since last write*/
#define BFBAK	0x04			/* Need a backup		*/

#define	MDVIEW		0x0001		/* read-only buffer		*/
#define	MDWRAP		0x0002		/* word wrap			*/
#define MDAUTOSAVE	0x0004		/* we are auto-saving		*/
#define MDCMODE		0x0008		/* we are in C-Mode		*/
#define MDPMATCH	0x0010		/* show matching balance chars	*/

/*
 * The starting position of a region, and the size of the region in
 * characters, is kept in a region structure.  Used by the region commands.
 */
typedef struct  {
	struct  LINE *r_linep;		/* Origin LINE address.	 */
	int	r_offset;		/* Origin LINE offset.	  */
	int	r_size;			/* Length in characters.	*/
} REGION;

/*
 * All text is kept in circularly linked lists of "LINE" structures. These
 * begin at the header line (which is the blank line beyond the end of the
 * buffer). This line is pointed to by the "BUFFER". Each line contains a the
 * number of bytes in the line (the "used" size), the size of the text array,
 * and the text. The end of line is not stored as a byte; it's implied. Future
 * additions will include update hints, and a list of marks into the line.
 */
typedef struct  LINE {
	struct  LINE *l_fp;		/* Link to the next line	*/
	struct  LINE *l_bp;		/* Link to the previous line    */
	int	l_size;			/* Allocated size	*/
	int	l_used;			/* Used size		    */
	char    l_text[1];		/* A bunch of characters.*/
} LINE;

#define lforw(lp)	((lp)->l_fp)
#define lback(lp)	((lp)->l_bp)
#define lgetc(lp, n)    ((lp)->l_text[(n)]&0xFF)
#define lputc(lp, n, c) ((lp)->l_text[(n)]=(c))
#define llength(lp)	((lp)->l_used)
#define ltext(lp)	(&((lp)->l_text[0]))

/*
 * The editor communicates with the display using a high level interface. A
 * "TERM" structure holds useful variables, and indirect pointers to routines
 * that do useful operations. The low level get and put routines are here too.
 * This lets a terminal, in addition to having non standard commands, have
 * funny get and put character code too. The calls might get changed to
 * "termp->t_field" style in the future, to make it possible to run more than
 * one terminal type.
 */
typedef struct  {
	short   t_nrow;			/* Number of rows.		*/
	short   t_ncol;			/* Number of columns.		*/
	int	(*t_init)();		/* Initialize the terminal	*/
	int	(*t_open)();	    	/* Open terminal at the start.  */
	int	(*t_close)();		/* Close terminal at end.	*/
	int	(*t_getchar)();		/* Get character from keyboard. */
	int	(*t_putchar)();		/* Put character to display.    */
	int	(*t_flush)();		/* Flush output buffers.	*/
	int	(*t_move)();		/* Move the cursor, origin 0.   */
	int	(*t_eeol)();		/* Erase to end of line.	*/
	int	(*t_eeop)();		/* Erase to end of page.	*/
	int	(*t_beep)();		/* Beep.			*/
	int	(*t_color)();		/* set reverse video state	*/
	int	(*t_resize)();		/* resize and set the new size	*/
} TERM;

typedef int	Function();
typedef int	(*Funcptr)();

/*	structure for the table of initial key bindings		*/

typedef struct  {
	short	 k_code;	/* Key code			*/
	Function *k_fp;		/* Routine to handle it		*/
} KEYTAB;

/*	structure for the name binding table		*/

typedef struct NBIND {
	char	*n_name;	/* name of function key		*/
	Function *n_func;	/* function name is bound to	*/
} NBIND;

/*
 * Structure for a matching character -- used with the paren balancing
 * code
 */
typedef struct _lmatch {
	LINE	*lp;	/* matching line */
	int	lo;	/* matching offset within lp */
} LMATCH;

/* initialized global external declarations */

extern  char    cinfo[];	/* character classification table */
extern	KEYTAB  keytab[];	/* key bind to functions table	*/
extern	NBIND   names[];	/* name to function table	*/

#ifndef MAIN
extern  int	fillcol;	/* Fill column		  	*/
extern  short   kbdm[];		/* Holds kayboard macro data    */
extern  char    pat[];		/* Search pattern		*/
extern	char	sarg[];		/* string argument for line exec*/
extern	char    *modename[];	/* text names of modes		*/
extern	char	modecode[];	/* letters to represent modes	*/
extern	int	nummodes;	/* number of modes		*/
extern	int	gmode;		/* global editor mode		*/
extern  int	sgarbf;		/* State of screen unknown	*/
extern  int	mpresf;		/* Stuff in message line	*/
extern	int	clexec;		/* command line execution flag	*/
extern  int	doingbackups;	/* are we doing backups?	*/
extern	int	onxterm;	/* are we running in an xterm?	*/
extern	int	explarg;	/* was an explicit arg given?	*/
extern	int	curchar;	/* the last character read	*/

/* uninitialized global external declarations */

extern  int	thisflag;	/* Flags, this command		*/
extern  int	lastflag;	/* Flags, last command		*/
extern  int	curgoal;	/* Goal for C-P, C-N		*/
extern  WINDOW  *curwp;		/* Current window		*/
extern  BUFFER  *curbp;		/* Current buffer		*/
extern  WINDOW  *wheadp;	/* Head of list of windows	*/
extern  BUFFER  *bheadp;	/* Head of list of buffers	*/
extern  BUFFER  *blistp;	/* Buffer for C-X C-B		*/
extern  short   *kbdmip;	/* Input pointer for above	*/
extern  short   *kbdmop;	/* Output pointer for above	*/

extern  BUFFER  *bfind();	/* Lookup a buffer by name	*/
extern  WINDOW  *wpopup();	/* Pop up window creation	*/
extern  LINE    *lalloc();	/* Allocate a line		*/
#endif

/*  FIXME !!!  terminal table defined only in TERM.C */
#ifndef	termdef
extern  TERM    term;		/* Terminal information.	*/
#endif
