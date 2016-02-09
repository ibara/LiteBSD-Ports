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
 * x11.c -- support for running standalone in an X11 window.  Originally
 * from Dave Lemke (now of NCD), 11/91.  Stolen from vile.
 */

#include "ce.h"

/*
 * undef for the benefit of some X header files -- if you really _are_ both
 * ISC and X11, well, you know what to do. 
 */
#undef ISC

/* redefined in X11/Xos.h */
#undef strchr
#undef strrchr

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/keysym.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>

#include <signal.h>
#include <stdio.h>

#include "celibc.h"

extern char	*progname;
extern int	isofont;

#define METABIT	0x80

/* These can go bigger, but they suck up lots of VM if they do. */
#define XMAXCOL	160	/* XXX */
#define XMAXROW 150	/* XXX */

#define DEFBORDERWIDTH	1

#define	SCRSIZ	64
#define	NPAUSE	10		/* # times thru update to pause */

#define min(x,y)	((x) < (y) ? (x) : (y))
#define max(x,y)	((x) > (y) ? (x) : (y))
#define	absol(x)	((x) > 0 ? (x) : -(x))
#define CEIL(a,b)	((a + b - 1) / (b))

/*
 * hack to support Saber since it doesn't do selections right
 *
 * XXX -- use xcutsel instead
 */
#undef	SABER_HACK

/* local screen rep flags */
#define	CELL_DIRTY	0x1
#define	CELL_REVERSE	0x2
#define	CELL_CURSOR	0x4
#define	CELL_SELECTION	0x8

/* local screen rep line flags */
#define	LINE_DIRTY	0x1

static int	x_refresh();
static int	set_character_class();

static XComposeStatus compst = {NULL, 0};
static char *displayname;
static Display *dpy;

typedef struct _text_win {
	/* X stuff */
	Display *dpy;
	int     screen;
	Window  win;
	int     bw;
	Bool	exposed;	/* Have we been exposed via an expose event? */
	int	visibility;	/* how visible are we? */

	XFontStruct *pfont;
	GC      textgc;
	GC      reversegc;
	unsigned long fg, bg;
	int     char_width, char_ascent, char_height;
	char   *fontname;

	/* text stuff */
	Bool    reverse;
	int     rows, cols;
	int     last_col, last_row;
	Bool    show_cursor;
	int     cur_row, cur_col;
	unsigned char **sc;	/* what the screen looks like */
	unsigned char **attr;	/* per-char cell flags */
	unsigned char *line_attr;	/* per-line attributes */

	/* selection stuff */
	Time    lasttime;	/* for multi-click */
	Time    click_timeout;
	int     numclicks;
	int     sel_start_col, sel_start_row;
	int     sel_end_col, sel_end_row;
	int     wipe_row, wipe_col;
	Bool    have_selection;
	Bool    show_selection;
	Atom    sel_prop;
	unsigned char *selection_data;
	int     selection_len;
}       TextWindowRec, *TextWindow;

static TextWindow cur_win;
static char *paste;
static char *pp;
static int plen;

/* Atoms for window manager `delete window' protocol. */
static Atom	atom_WM_PROTOCOLS;
static Atom	atom_WM_DELETE_WINDOW;

/* Atoms for selection. */
static Atom	atom_TARGETS;
static Atom	atom_MULTIPLE;
static Atom	atom_TIMESTAMP;
static Atom	atom_TEXT;
static Atom	atom_CLIPBOARD;

static int x_init(),
	x_open(),
	x_close(),
	x_getc(),
	x_putc(),
	x_flush(),
	x_move(),
	x_eeol(),
	x_eeop(),
	x_beep(),
	x_visibeep(),
	x_rev(),
	x_cres();

#if COLOR
static int x_fcol(), x_bcol();
#endif

#ifdef SCROLLCODE
static int x_scroll();
#endif

static void change_selection();
static void x_stash_selection();

#define	DEFAULT_FONTNAME "-misc-fixed-medium-r-normal--13-120-75-75-c-80-iso8859-1"

static char *fontname;

static char *geometry = NULL;

static Bool reverse_video;
static Bool audible_bell = FALSE;	/* as opposed to visible bell */
static int foreground = -1, background = -1;

static int multi_click_time = 500;

static int startx = 100, starty = 100;

static int start_rows = 36, start_cols = 80;

void	x_set_rv();
void	x_set_dpy();
void	x_set_font();
void	x_set_geometry();

TERM    term =
{
	NULL,
	NULL,
	x_init,
	x_open,
	x_close,
	x_getc,
	x_putc,
	x_flush,
	x_move,
	x_eeol,
	x_eeop,
	x_visibeep,
	x_rev,
	x_cres,
};

#define	x_width(tw)		((tw)->cols * (tw)->char_width)
#define	x_height(tw)		((tw)->rows * (tw)->char_height)
#define	x_pos(tw, c)		((c) * (tw)->char_width)
#define	y_pos(tw, r)		((r) * (tw)->char_height)
#define	text_y_pos(tw, r)	(y_pos((tw), (r)) + (tw)->char_ascent)

static void
message(s)
char	*s;
{
	fprintf(stderr, "%s: %s\n", progname, s);
}

/* ARGSUSED */
void
x_preparse_args(argc, argv)
int	argc;
char	**argv;
{
	register int	i;

	if (!progname) {
		progname = argv[0];
		if (*progname == '/')
			progname = strrchr(progname, '/');
	}
	for (i = 1; i < argc; i++) {
		if (argv[i][0] == '-')
			switch(argv[i][1]) {
			case 'd':
			case 'D':
				if (argv[i + 1])
					x_set_dpy(argv[++i]);
				else
					usage();
				break;
			case 'R':
				x_set_rv();
				break;
			case 'f':
			case 'F':
				if (argv[i + 1])
					x_set_font(argv[++i]);
				else
					usage();
				break;
			case 'g':
			case 'G':
				if (argv[i + 1])
					x_set_geometry(argv[++i]);
				else
					usage();
				break;
			default:
				break;
			}
	}
}

char *
x_current_fontname()
{
	return cur_win->fontname;
}

void
x_set_rv()
{
	reverse_video = !reverse_video;
}

void
x_set_geometry(g)
char	*g;
{
	geometry = g;
}

void
x_set_dpy(dname)
char	*dname;
{
	displayname = dname;
}

void
x_set_font(fname)
char	*fname;
{
	fontname = fname;
	isofont = isisofont(fname);
}

int
x_setfont(fname)
char	*fname;
{
	XFontStruct	*pfont;
	XSizeHints	xsh;
	int		oldw, oldh;

	fontname = fname;
	isofont = isisofont(fname);
	if (cur_win) {
		pfont = XLoadQueryFont(dpy, fontname);
		if (pfont) {
			oldw = x_width(cur_win);
			oldh = x_height(cur_win);

			if (pfont->max_bounds.width != pfont->min_bounds.width)
				message("proportional font, things will be miserable");
			cur_win->pfont = pfont;
			cur_win->char_width = pfont->max_bounds.width;
			/*
			 * using maxbounds instead of font ascent/descent, so
			 * it may be widely spaced, but won't screw up
			 * accented chars 
			 */

			cur_win->char_height = pfont->ascent + pfont->descent;
			cur_win->char_ascent = pfont->max_bounds.ascent;

			XSetFont(dpy, cur_win->textgc, pfont->fid);
			XSetFont(dpy, cur_win->reversegc, pfont->fid);

			/* is size changed, resize it, otherwise refresh */
			if (oldw != x_width(cur_win) || oldh != x_height(cur_win)) {
				XResizeWindow(dpy, cur_win->win,
				       x_width(cur_win), x_height(cur_win));
			} else {
				x_refresh(cur_win, 0, 0, cur_win->cols, cur_win->rows);
			}
			xsh.flags = PResizeInc | PMaxSize;
			xsh.width_inc = cur_win->char_width;
			xsh.height_inc = cur_win->char_height;
			xsh.max_width = XMAXROW * cur_win->char_height;
			xsh.max_height = XMAXCOL * cur_win->char_width;
			XSetNormalHints(dpy, cur_win->win, &xsh);

			if (cur_win->fontname)
				free(cur_win->fontname);
			cur_win->fontname = strdup(fontname);
			return 1;
		}
		return 0;
	}
	return 1;
}

static RETSIGTYPE
x_quit(s)
int	s;
{
	x_close();
	exit(0);
	/*NOTREACHED*/
}

static void
x_resize_screen(tw, rows, cols)
TextWindow	tw;
int		rows, cols;
{
	int	r, c;

	if (rows != tw->rows) {
		if (tw->sc) {
			for (r = 0; r < tw->rows; r++) {
				free(tw->sc[r]);
				free(tw->attr[r]);
			}
			free(tw->sc);
			free(tw->attr);
			free(tw->line_attr);
		}
		tw->rows = rows;
		/* allocate screen */
		tw->sc = (unsigned char **) malloc(sizeof(unsigned char *) * tw->rows);
		tw->attr = (unsigned char **) malloc(sizeof(unsigned char *) * tw->rows);
		tw->line_attr = (unsigned char *) malloc(sizeof(unsigned char) * tw->rows);
	}
	tw->cols = cols;

	if (!tw->sc || !tw->attr || !tw->line_attr) {
		message("couldn't allocate memory for screen");
		exit(3);
	}
	/* init it */
	for (r = 0; r < tw->rows; r++) {
		tw->sc[r] = (unsigned char *) malloc(sizeof(unsigned char) * tw->cols);
		tw->attr[r] = (unsigned char *) malloc(sizeof(unsigned char) * tw->cols);
		if (!tw->sc[r] || !tw->attr[r]) {
			message("couldn't allocate memory for screen");
			exit(3);
		}
		tw->line_attr[r] = LINE_DIRTY;
		memset((char *) tw->sc[r], ' ', tw->cols);
		for (c = 0; c < tw->cols; c++)
			tw->attr[r][c] = CELL_DIRTY;
	}
}

struct {
	char   *name;
	int     val;
} name_val[] = {
	"yes", TRUE,
	"on", TRUE,
	"1", TRUE,
	"true", TRUE,
	"no", FALSE,
	"off", FALSE,
	"0", FALSE,
	"false", FALSE,
	(char *) 0, 0,
};

static  Bool
bool_name(t)
char	*t;
{
	int     len = strlen(t);
	int     i;

	for (i = 0; name_val[i].name; i++) {
		if (!strncmp(t, name_val[i].name, len))
			return name_val[i].val;
	}
	return False;
}

static int
color_value(dpy, screen, t)
Display	*dpy;
int	screen;
char	*t;
{
	XColor	xc;
	Colormap cmap;

	cmap = DefaultColormap(dpy, screen);

	XParseColor(dpy, cmap, t, &xc);
	XAllocColor(dpy, cmap, &xc);
	return xc.pixel;
}

static long	resopts = 0L;

#define _RES_FONT	00000001
#define _RES_GEOMETRY	00000002
#define _RES_CCLASS	00000004
#define _RES_MCTIME	00000010
#define _RES_REVERSE	00000020
#define _RES_FGCOLOR	00000040
#define _RES_BGCOLOR	00000100
#define _RES_AUDBELL	00000200

static void
x_get_defaults(dpy, screen)
Display	*dpy;
int	screen;
{
	char	*t;

	if (!fontname) {
		t = XGetDefault(dpy, progname, "font");
		if (t)
			fontname = t;
		else {
			t = XGetDefault(dpy, "XCe", "font");
			if (t)
				fontname = t;
		}
	}
	if (!geometry) {
		t = XGetDefault(dpy, progname, "geometry");
		if (t)
			geometry = t;
		else {
			t = XGetDefault(dpy, progname, "geometry");
			if (t)
				geometry = t;
		}
	}

	t = XGetDefault(dpy, progname, "charClass");
	if (t)
		set_character_class(t);
	else {
		t = XGetDefault(dpy, "XCe", "charClass");
		if (t)
			set_character_class(t);
	}

	t = XGetDefault(dpy, progname, "multiClickTime");
	if (t)
		multi_click_time = atoi(t);
	else {
		t = XGetDefault(dpy, "XCe", "multiClickTime");
		if (t)
			multi_click_time = atoi(t);
	}

	t = XGetDefault(dpy, progname, "reverseVideo");
	if (t)
		reverse_video = bool_name(t);
	else {
		t = XGetDefault(dpy, "XCe", "reverseVideo");
		if (t)
			reverse_video = bool_name(t);
	}

	t = XGetDefault(dpy, progname, "foreground");
	if (t)
		foreground = color_value(dpy, screen, t);
	else {
		t = XGetDefault(dpy, "XCe", "foreground");
		if (t)
			foreground = color_value(dpy, screen, t);
	}

	t = XGetDefault(dpy, progname, "background");
	if (t)
		background = color_value(dpy, screen, t);
	else {
		t = XGetDefault(dpy, "XCe", "background");
		if (t)
			background = color_value(dpy, screen, t);
	}

	t = XGetDefault(dpy, progname, "audibleBell");
	if (t)
		audible_bell = bool_name(t);
	else {
		t = XGetDefault(dpy, "XCe", "audibleBell");
		if (t)
			audible_bell = bool_name(t);
	}
}

static int
x_init()
{
	return(0);
}

static int
x_open()
{
	TextWindow	tw;
	int		screen, flags;
	XFontStruct	*pfont;
	XGCValues	gcvals;
	unsigned long	gcmask;
	XSetWindowAttributes swat;
	unsigned long	winmask;
	XSizeHints	xsh;
	XWMHints	xwmh;

	tw = (TextWindow) calloc(1, sizeof(TextWindowRec));
	if (!tw) {
		message("insufficient memory to allocate windows, exiting");
		exit(3);
	}
	dpy = XOpenDisplay(displayname);
	if (!dpy) {
		message("couldn't open X display");
		exit(0);
	}
	tw->dpy = dpy;
	tw->screen = screen = DefaultScreen(dpy);

	x_get_defaults(dpy, screen);

	if (audible_bell)
		term.t_beep = x_beep;

	if (!fontname) {
		fontname = DEFAULT_FONTNAME;
		isofont = 1;	/* default font is ISO8859-1 font */
	}
	pfont = XLoadQueryFont(dpy, fontname);
	if (!pfont) {
		fprintf(stderr, "%s: can't find font %s, using %s\n", progname, fontname, DEFAULT_FONTNAME);
		pfont = XLoadQueryFont(dpy, DEFAULT_FONTNAME);
		if (!pfont) {
			fprintf(stderr, "%s: couldn't get font \"%s\" or \"%s\", exiting\n",
				progname, fontname, DEFAULT_FONTNAME);
			exit(3);
		}
		fontname = DEFAULT_FONTNAME;
		isofont = 1;	/* default font is ISO8859-1 font */
	} else
		isofont = isisofont(fontname);
	tw->fontname = strdup(fontname);

	if (pfont->max_bounds.width != pfont->min_bounds.width)
		message("proportional font, things will be miserable");
	tw->pfont = pfont;
	tw->char_width = pfont->max_bounds.width;

	/*
	 * using maxbounds instead of font ascent/descent, so it may be
	 * widely spaced, but won't screw up accented chars 
	 */

	tw->char_height = pfont->ascent + pfont->descent;
	tw->char_ascent = pfont->max_bounds.ascent;

	if (foreground == -1)
		foreground = BlackPixel(dpy, 0);
	if (background == -1)
		background = WhitePixel(dpy, 0);
	if (reverse_video) {
		tw->bg = foreground;
		tw->fg = background;
	} else {
		tw->fg = foreground;
		tw->bg = background;
	}

	gcmask = GCForeground | GCBackground | GCFont | GCGraphicsExposures;
	gcvals.foreground = tw->fg;
	gcvals.background = tw->bg;
	gcvals.font = pfont->fid;
	gcvals.graphics_exposures = False;
	tw->textgc = XCreateGC(dpy, RootWindow(dpy, screen), gcmask, &gcvals);

	gcvals.foreground = tw->bg;
	gcvals.background = tw->fg;
	gcvals.font = pfont->fid;
	gcvals.graphics_exposures = False;
	tw->reversegc = XCreateGC(dpy, RootWindow(dpy, screen), gcmask, &gcvals);

	if (geometry) {
		flags = XParseGeometry(geometry, &startx, &starty,
				       (unsigned int *)&start_cols,
				       (unsigned int *)&start_rows);
	} else
		flags = 0;
	x_resize_screen(tw, start_rows, start_cols);

	if ((flags & XValue) && (flags & XNegative))
		startx = DisplayWidth(dpy, screen) - x_width(tw);
	if ((flags & YValue) && (flags & YNegative))
		starty = DisplayHeight(dpy, screen) - x_height(tw);

	tw->reverse = False;

	tw->bw = DEFBORDERWIDTH;

	tw->click_timeout = multi_click_time;

	swat.background_pixel = tw->bg;
	swat.border_pixel = tw->fg;
	swat.event_mask = ExposureMask | KeyPressMask | StructureNotifyMask |
		ButtonPress | ButtonRelease | ButtonMotionMask |
		FocusChangeMask | EnterWindowMask | LeaveWindowMask;
	winmask = CWBackPixel | CWBorderPixel | CWEventMask;
	tw->win = XCreateWindow(dpy, RootWindow(dpy, screen), startx, starty,
	     x_width(tw), x_height(tw), tw->bw, CopyFromParent, InputOutput,
				CopyFromParent, winmask, &swat);


	xsh.flags = /* PPosition | */ PResizeInc | PSize | PMaxSize;
	if (flags & (XValue | YValue))
		xsh.flags |= USPosition;
	if (flags & (WidthValue | HeightValue))
		xsh.flags |= USSize;

	xsh.width_inc = tw->char_width;
	xsh.height_inc = tw->char_height;
	xsh.x = startx;
	xsh.y = starty;
	xsh.width = x_width(tw);
	xsh.height = x_height(tw);
	xsh.max_width = XMAXROW * tw->char_height;
	xsh.max_height = XMAXCOL * tw->char_width;
	XSetStandardProperties(dpy, tw->win, "XCe", "XCe", (Pixmap) 0,
			       NULL, 0, &xsh);

	xwmh.flags = InputHint;
	xwmh.input = True;
	XSetWMHints(dpy, tw->win, &xwmh);

	{
		XClassHint *class_hints;
		class_hints = XAllocClassHint();
		class_hints->res_name = strdup("XCe");
		class_hints->res_class = strdup("XCe");
		XSetClassHint(dpy, tw->win, class_hints);
		free(class_hints->res_name);
		free(class_hints->res_class);
		XFree((char *)class_hints);
	}
	cur_win = tw;
	XMapWindow(dpy, tw->win);

	cur_win->exposed = FALSE;
	cur_win->visibility = VisibilityUnobscured;

	XSync(dpy, 0);

	tw->sel_prop = XInternAtom(dpy, "XCE_SELECTION", False);

	atom_WM_PROTOCOLS = XInternAtom(dpy, "WM_PROTOCOLS", False);
	atom_WM_DELETE_WINDOW = XInternAtom(dpy, "WM_DELETE_WINDOW", False);

	{
		Atom atoms[2];
		int i;

		i = 0;
		atoms[i++] = atom_WM_DELETE_WINDOW;
		XSetWMProtocols(dpy, tw->win, atoms, i);
	}
	

	signal(SIGHUP, x_quit);
	signal(SIGINT, x_quit);
	signal(SIGTERM, x_quit);

	/* main code assumes that it can access a cell at nrow x ncol */
	term.t_ncol = tw->cols;
	term.t_nrow = tw->rows - 1;
	return (0);
}

static int
x_close()
{
	XCloseDisplay(dpy);
	return(0);
}

static int
x_touch(tw, sc, sr, ec, er)
TextWindow tw;
int	sc, sr;
int	ec, er;
{
	int	r, c;

	if (er > tw->rows)
		er = tw->rows;
	if (ec > tw->cols)
		ec = tw->cols;

	for (r = sr; r < er; r++) {
		tw->line_attr[r] |= LINE_DIRTY;
		for (c = sc; c < ec; c++) {
			tw->attr[r][c] |= CELL_DIRTY;
		}
	}
	return(0);
}

static int
x_refresh(tw, sc, sr, ec, er)
TextWindow tw;
int	sc, sr;
int	ec, er;
{
	x_touch(tw, sc, sr, ec, er);
	x_flush();
	return 0;
}

#ifdef SCROLLCODE
/*
 * XXX This mostly works, except for cursor dirt.  Doesn't seem to be a
 * noticable win, however... 
 *
 * Appears to give a apparent win under Xremote, but seems to repaint the
 * stuff that's scrolled... 
 */
#define	copy_area

#ifdef copy_area
static void
wait_for_scroll(tw)
TextWindow tw;
{
	XEvent  ev;
	int     sc, sr, ec, er;
	XGraphicsExposeEvent *gev;

	while (1) {		/* loop looking for a gfx expose or no expose */
		if (XCheckTypedEvent(tw->dpy, NoExpose, &ev))
			return;
		if (XCheckTypedEvent(tw->dpy, GraphicsExpose, &ev)) {
			gev = (XGraphicsExposeEvent *) & ev;
			sc = gev->x / tw->char_width;
			sr = gev->y / tw->char_height;
			ec = sc + gev->width / tw->char_width;
			er = sr + gev->height / tw->char_height;
			x_refresh(tw, sc, sr, ec, er);
			return;
		}
		XSync(tw->dpy, 0);
	}
	return(0);
}

#endif

/*
 * XXX This may not be any faster than having the guts do the scrolling
 * instead.
 */
static int
x_scroll(from, to, count)
int     from, to, count;
{
	int     rf, rt, fst, tst, finc, tinc, i, diff, c;

	/*
	 * XXX since there aren't any hooks (yet) for scrolling, stop showing
	 * the selection as soon as the text changes 
	 */
	if (cur_win->show_selection)
		change_selection(cur_win, False, True);

	/*
	 * figure out what lines to move first, to prevent being hosed if the
	 * scroll overlaps itself 
	 */
	if (from < to) {
		fst = from + count - 1;
		finc = -1;
		tst = to + count - 1;
		tinc = -1;
	} else {
		fst = from;
		finc = 1;
		tst = to;
		tinc = 1;
	}

#ifndef copy_area
	cur_win->line_attr[cur_win->cur_row] |= LINE_DIRTY;
	cur_win->attr[cur_win->cur_row][cur_win->cur_col] |= CELL_DIRTY;
	cur_win->attr[cur_win->cur_row][cur_win->cur_col] &= ~CELL_CURSOR;
#endif

	for (rf = fst, rt = tst, i = 0; i < count; i++, rf += finc, rt += tinc) {
		bcopy((char *) cur_win->sc[rf], (char *) cur_win->sc[rt], cur_win->cols);
		memset((char *) cur_win->sc[rf], ' ', cur_win->cols);

		/*
		 * only mark row if it isn't going to be overwritten during
		 * this scroll 
		 */
		if (!((rf > tst && rf < (tst + count)) ||
		      (rf < tst && rf > (tst - count))))
			cur_win->line_attr[rf] |= LINE_DIRTY;
#ifndef copy_area
		cur_win->line_attr[rt] |= LINE_DIRTY;
#else
		if (rf == cur_win->cur_row) {	/* erase scrolled cursor */
			cur_win->line_attr[rt] |= LINE_DIRTY;
			cur_win->attr[rt][cur_win->cur_col] |= CELL_DIRTY;
			cur_win->attr[rt][cur_win->cur_col] &= ~CELL_CURSOR;
		}
#endif

		for (c = 0; c < cur_win->cols; c++) {

#ifndef copy_area
			cur_win->attr[rt][c] = cur_win->attr[rf][c] | CELL_DIRTY;
			cur_win->attr[rt][c] &= ~CELL_CURSOR;
#endif

			/* memor() would be useful here... */
			cur_win->attr[rf][c] |= CELL_DIRTY;
		}
	}

#ifdef copy_area
	XCopyArea(cur_win->dpy, cur_win->win, cur_win->win, cur_win->textgc,
		  x_pos(cur_win, 0), y_pos(cur_win, from),
		  x_width(cur_win), count * cur_win->char_height,
		  x_pos(cur_win, 0), y_pos(cur_win, to));
	XFlush(dpy);
	wait_for_scroll(cur_win);
#endif

	/* some lines weren't scrolled, so we need to wipe them */
	/*
	 * XXX - why is this necessary?  it only gets hit when the scrolling
	 * code gets 'smart' and realizes it doesn't have to copy all the
	 * lines around, but then it doesn't bother to clean them either. 
	 */
	if (absol(from - to) != count) {
		diff = absol(from - to) - count;
		for (i = 0, rf = fst; i <= diff; i++, rf -= finc) {
			memset((char *) cur_win->sc[rf], ' ', cur_win->cols);
			cur_win->line_attr[rf] |= LINE_DIRTY;
			for (c = 0; c < cur_win->cols; c++) {
				/* memor() would be useful here... */
				cur_win->attr[rf][c] |= CELL_DIRTY;
			}
		}
	}
	return(0);
}
#endif /* SCROLLCODE */

#define	CLEAR_THRESH	8

static void
flush_line(text, len, rev, sr, sc)
unsigned char *text;
int     len;
Bool    rev;
int     sr, sc;
{
	unsigned char *p;
	int     cc, tlen, i = 0;

	/* break line into TextStrings and FillRects */
	p = text;
	cc = 0;
	tlen = 0;
	for (i = 0; i < len; i++) {
		if (text[i] == ' ') {
			cc++;
			tlen++;
		} else {
			if (cc >= CLEAR_THRESH) {
				tlen -= cc;
				XDrawImageString(dpy, cur_win->win,
					(rev ? cur_win->reversegc : cur_win->textgc),
					x_pos(cur_win, sc), text_y_pos(cur_win, sr),
					(char *) p, tlen);
				p += tlen + cc;
				sc += tlen;
				XFillRectangle(dpy, cur_win->win,
					(!rev ? cur_win->reversegc : cur_win->textgc),
					x_pos(cur_win, sc), y_pos(cur_win, sr),
					(cc * cur_win->char_width), cur_win->char_height);
				sc += cc;
				tlen = 1;	/* starting new run */
			} else
				tlen++;
			cc = 0;
		}
	}
	if (cc >= CLEAR_THRESH) {
		tlen -= cc;
		XDrawImageString(dpy, cur_win->win,
			       (rev ? cur_win->reversegc : cur_win->textgc),
				 x_pos(cur_win, sc), text_y_pos(cur_win, sr),
				 (char *) p, tlen);
		sc += tlen;
		XFillRectangle(dpy, cur_win->win,
			       (!rev ? cur_win->reversegc : cur_win->textgc),
			       x_pos(cur_win, sc), y_pos(cur_win, sr),
			  (cc * cur_win->char_width), cur_win->char_height);
	} else if (tlen > 0) {
		XDrawImageString(dpy, cur_win->win,
			       (rev ? cur_win->reversegc : cur_win->textgc),
				 x_pos(cur_win, sc), text_y_pos(cur_win, sr),
				 (char *) p, tlen);
	}
}

#ifdef old
static void
clear_line(row, start, count)
int     row, start, count;
{
	XClearArea(dpy, cur_win->win, x_pos(cur_win, start), y_pos(cur_win, row),
		(count * cur_win->char_width), cur_win->char_height, False);
}

#endif

#undef	clear_hack

/* make sure the screen looks like we want it to */
static int
x_flush()
{
	unsigned char *start;
	int     len;
	int     r, c;
	int     sc;
	Bool    isrev;
	Bool    flush;

#ifdef clear_hack
	int     clear_count;
	int     clear_start;

#endif

#define	reversed(c)	((c) & (CELL_REVERSE | CELL_CURSOR | CELL_SELECTION))

	if (cur_win->visibility == VisibilityFullyObscured || cur_win->exposed == 0)
		return 0;

	for (r = 0; r < cur_win->rows; r++) {
		if (!(cur_win->line_attr[r] & LINE_DIRTY))
			continue;
		start = NULL;
		len = 0;
		isrev = False;
		flush = False;
		sc = 0;

#ifdef clear_hack
		clear_count = 0;
		clear_start = -1;
#endif

		cur_win->line_attr[r] &= ~LINE_DIRTY;
		for (c = 0; c < cur_win->cols;) {
			if (cur_win->attr[r][c] & CELL_DIRTY) {

#ifdef clear_hack
				if (cur_win->sc[r][c] == ' ' && clear_start <= 0) {
					if (clear_start == -1)
						clear_start = c;
					clear_count++;
					c++;
					continue;
				}
#endif

				if (isrev != (reversed(cur_win->attr[r][c]) ? True : False)) {
					if (len) {
						flush_line(start, len, isrev, r, sc);
						start = NULL;
						len = 0;
						flush = False;
					}
					isrev = !isrev;
					continue;
				}
				if (!len) {
					start = &(cur_win->sc[r][c]);
					sc = c;
				}
				len++;
				cur_win->attr[r][c] &= ~CELL_DIRTY;
			} else if (len)
				flush = True;
			c++;

#ifdef clear_hack
			if (clear_count && clear_start == 0) {
				clear_line(r, clear_start, clear_count);
				clear_count = 0;
				clear_start = -1;
			}
#endif

			if (flush && len) {
				flush_line(start, len, isrev, r, sc);
				len = 0;
				flush = False;
			}
		}

#ifdef clear_hack
		if (clear_count && clear_start == 0) {
			clear_line(r, clear_start, clear_count);
			clear_count = 0;
			clear_start = -1;
		}
#endif

		if (len) {
			flush_line(start, len, isrev, r, sc);
		}
	}
	/* last bit for cursor -- pretty sick, but it works */
	if (!cur_win->show_cursor)
		XDrawRectangle(dpy, cur_win->win, cur_win->textgc,
			       x_pos(cur_win, cur_win->cur_col), y_pos(cur_win, cur_win->cur_row),
			 cur_win->char_width - 1, cur_win->char_height - 1);
	XFlush(dpy);
	return(0);
}

static int
x_move(row, col)
int     row, col;
{
	cur_win->attr[cur_win->cur_row][cur_win->cur_col] &= ~CELL_CURSOR;
	cur_win->attr[cur_win->cur_row][cur_win->cur_col] |= CELL_DIRTY;
	cur_win->line_attr[cur_win->cur_row] |= LINE_DIRTY;
	cur_win->cur_col = col;
	cur_win->cur_row = row;
	cur_win->attr[row][col] |= (CELL_CURSOR | CELL_DIRTY);
	cur_win->line_attr[row] |= LINE_DIRTY;
	return(0);
}

#define	in_selection(tw, r)	((r) >= (tw)->sel_start_row && \
				 (r) <= (tw)->sel_end_row)

/* ARGSUSED */
int
x_putline(row, str, len)
int     row;
char   *str;
{
	int     c, i;

	/*
	 * XXX since there aren't any hooks (yet) for scrolling, stop showing
	 * the selection as soon as the text changes 
	 */
	if (cur_win->show_selection && in_selection(cur_win, row))
		change_selection(cur_win, False, True);

	cur_win->attr[cur_win->cur_row][cur_win->cur_col] &= ~CELL_CURSOR;
	cur_win->attr[cur_win->cur_row][cur_win->cur_col] |= CELL_DIRTY;

	bcopy((char *) str, (char *) &(cur_win->sc[row][cur_win->cur_col]), len);
	for (i = 0, c = cur_win->cur_col; i < len; c++, i++) {
		if (cur_win->reverse)
			cur_win->attr[row][c] |= CELL_REVERSE;
		else
			cur_win->attr[row][c] &= ~CELL_REVERSE;
		cur_win->attr[row][c] |= CELL_DIRTY;
		cur_win->attr[row][c] |= CELL_DIRTY;
	}
	cur_win->cur_row = row;
	cur_win->cur_col = c - 1;

	cur_win->attr[row][cur_win->cur_col] |= (CELL_CURSOR | CELL_DIRTY);
	cur_win->line_attr[row] |= LINE_DIRTY;
	return(0);
}

static int
x_putc(c)
char    c;
{
	/*
	 * XXX since there aren't any hooks (yet) for scrolling, stop showing
	 * the selection as soon as the text changes 
	 */
	if (cur_win->show_selection && in_selection(cur_win, cur_win->cur_row))
		change_selection(cur_win, False, True);

	cur_win->attr[cur_win->cur_row][cur_win->cur_col] &= ~CELL_CURSOR;
	cur_win->attr[cur_win->cur_row][cur_win->cur_col] |= CELL_DIRTY;

	if (!ISCTRL(c)) {
		if (cur_win->reverse)
			cur_win->attr[cur_win->cur_row][cur_win->cur_col] |= CELL_REVERSE;
		else
			cur_win->attr[cur_win->cur_row][cur_win->cur_col] &= ~CELL_REVERSE;
		cur_win->sc[cur_win->cur_row][cur_win->cur_col] = c;
		cur_win->cur_col++;
	} else if (c == '\b') {
		cur_win->cur_col--;
	}
	cur_win->attr[cur_win->cur_row][cur_win->cur_col] |=
		(CELL_CURSOR | CELL_DIRTY);
	cur_win->line_attr[cur_win->cur_row] |= LINE_DIRTY;
	return(0);
}

/*
 * clear to end of line 
 */
static int
x_eeol()
{
	int     c;

	c = cur_win->cur_col;

	memset((char *) &(cur_win->sc[cur_win->cur_row][c]), ' ',
	       cur_win->cols - c);

#ifdef old
	/* XXX this might be faster, but it looks worse */
	clear_line(cur_win->cur_row, c, cur_win->cols - c);
#else
	while (c < cur_win->cols)
		cur_win->attr[cur_win->cur_row][c++] |= CELL_DIRTY;
#endif
	return(0);
}

/*
 * clear to end of page 
 */
static int
x_eeop()
{
	int     r, sc, c;

	r = cur_win->cur_row;
	sc = cur_win->cur_col;
	while (r < cur_win->rows) {
		cur_win->line_attr[r] |= LINE_DIRTY;
		for (c = sc; c < cur_win->cols; c++)
			cur_win->attr[r][c] |= CELL_DIRTY;
		memset((char *) &(cur_win->sc[r][0]), ' ', cur_win->cols);
		r++;
		sc = 0;
	}

	return(0);
}


/* selection processing code; multi-click code stolen from xterm */

/*
 * double click table for cut and paste in 8 bits 
 *
 * This table is divided in four parts : 
 *
 * - control characters	[0,0x1f] U [0x80,0x9f] - separators
 * 0x3f] U [0xa0,0xb9] - binding characters	[0x40,0x7f] U [0xc0,0xff] -
 * execeptions 
 */
static int charClass[256] = {
/* NUL  SOH  STX  ETX  EOT  ENQ  ACK  BEL */
			     32, 1, 1, 1, 1, 1, 1, 1,
/* BS   HT   NL   VT   NP   CR   SO   SI */
			     1, 32, 1, 1, 1, 1, 1, 1,
/* DLE  DC1  DC2  DC3  DC4  NAK  SYN  ETB */
			     1, 1, 1, 1, 1, 1, 1, 1,
/* CAN   EM  SUB  ESC   FS   GS   RS   US */
			     1, 1, 1, 1, 1, 1, 1, 1,
/* SP    !    "    #    $    %    &    ' */
			     32, 33, 34, 35, 36, 37, 38, 39,
/* (    )    *    +    ,    -    .    / */
			     40, 41, 42, 43, 44, 45, 46, 47,
/* 0    1    2    3    4    5    6    7 */
			     48, 48, 48, 48, 48, 48, 48, 48,
/* 8    9    :    ;    <    =    >    ? */
			     48, 48, 58, 59, 60, 61, 62, 63,
/* @    A    B    C    D    E    F    G */
			     64, 48, 48, 48, 48, 48, 48, 48,
/* H    I    J    K    L    M    N    O */
			     48, 48, 48, 48, 48, 48, 48, 48,
/* P    Q    R    S    T    U    V    W */
			     48, 48, 48, 48, 48, 48, 48, 48,
/* X    Y    Z    [    \    ]    ^    _ */
			     48, 48, 48, 91, 92, 93, 94, 48,
/* `    a    b    c    d    e    f    g */
			     96, 48, 48, 48, 48, 48, 48, 48,
/* h    i    j    k    l    m    n    o */
			     48, 48, 48, 48, 48, 48, 48, 48,
/* p    q    r    s    t    u    v    w */
			     48, 48, 48, 48, 48, 48, 48, 48,
/* x    y    z    {    |    }    ~  DEL */
			     48, 48, 48, 123, 124, 125, 126, 1,
/* x80  x81  x82  x83  IND  NEL  SSA  ESA */
			     1, 1, 1, 1, 1, 1, 1, 1,
/* HTS  HTJ  VTS  PLD  PLU   RI  SS2  SS3 */
			     1, 1, 1, 1, 1, 1, 1, 1,
/* DCS  PU1  PU2  STS  CCH   MW  SPA  EPA */
			     1, 1, 1, 1, 1, 1, 1, 1,
/* x98  x99  x9A  CSI   ST  OSC   PM  APC */
			     1, 1, 1, 1, 1, 1, 1, 1,
/* -    i   c/    L   ox   Y-    |   So */
			     160, 161, 162, 163, 164, 165, 166, 167,
/* ..   c0   ip   <<    _	R0    - */
			     168, 169, 170, 171, 172, 173, 174, 175,
/* o   +-    2    3    '    u   q|    . */
			     176, 177, 178, 179, 180, 181, 182, 183,
/* ,    1    2   >>  1/4  1/2  3/4    ? */
			     184, 185, 186, 187, 188, 189, 190, 191,
/* A`   A'   A^   A~   A:   Ao   AE   C, */
			     48, 48, 48, 48, 48, 48, 48, 48,
/* E`   E'   E^   E:   I`   I'   I^   I: */
			     48, 48, 48, 48, 48, 48, 48, 48,
/* D-   N~   O`   O'   O^   O~   O:    X */
			     48, 48, 48, 48, 48, 48, 48, 216,
/* O/   U`   U'   U^   U:   Y'    P    B */
			     48, 48, 48, 48, 48, 48, 48, 48,
/* a`   a'   a^   a~   a:   ao   ae   c, */
			     48, 48, 48, 48, 48, 48, 48, 48,
/* e`   e'   e^   e:    i`  i'   i^   i: */
			     48, 48, 48, 48, 48, 48, 48, 48,
/* d   n~   o`   o'   o^   o~   o:   -: */
			     48, 48, 48, 48, 48, 48, 48, 248,
/* o/   u`   u'   u^   u:   y'    P   y: */
			     48, 48, 48, 48, 48, 48, 48, 48};

static int
set_character_class_range(low, high, value)
register int low, high;		/* in range of [0..255] */
register int value;		/* arbitrary */
{

	if (low < 0 || high > 255 || high < low)
		return (-1);

	for (; low <= high; low++)
		charClass[low] = value;

	return (0);
}

/*
 * set_character_class - takes a string of the form 
 *
 * low[-high]:val[,low[-high]:val[...]] 
 *
 * and sets the indicated ranges to the indicated values. 
 */

static int
set_character_class(s)
register char *s;
{
	register int i;		/* iterator, index into s */
	int     len;		/* length of s */
	int     acc;		/* accumulator */
	int     low, high;	/* bounds of range [0..127] */
	int     base;		/* 8, 10, 16 (octal, decimal, hex) */
	int     numbers;	/* count of numbers per range */
	int     digits;		/* count of digits in a number */
	static char *errfmt = "%s:  %s in range string \"%s\" (position %d)\n";

	if (!s || !s[0])
		return -1;

	base = 10;		/* in case we ever add octal, hex */
	low = high = -1;	/* out of range */

	for (i = 0, len = strlen(s), acc = 0, numbers = digits = 0;
	     i < len; i++) {
		char    c = s[i];

		if (ISWHSP(c)) {
			continue;
		} else if (ISDIGIT(c)) {
			acc = acc * base + (c - '0');
			digits++;
			continue;
		} else if (c == '-') {
			low = acc;
			acc = 0;
			if (digits == 0) {
				fprintf(stderr, errfmt, progname, "missing number", s, i);
				return (-1);
			}
			digits = 0;
			numbers++;
			continue;
		} else if (c == ':') {
			if (numbers == 0)
				low = acc;
			else if (numbers == 1)
				high = acc;
			else {
				fprintf(stderr, errfmt, progname, "too many numbers",
					s, i);
				return (-1);
			}
			digits = 0;
			numbers++;
			acc = 0;
			continue;
		} else if (c == ',') {
			/*
			 * now, process it 
			 */

			if (high < 0) {
				high = low;
				numbers++;
			}
			if (numbers != 2) {
				fprintf(stderr, errfmt, progname, "bad value number",
					s, i);
			} else if (set_character_class_range(low, high, acc) != 0) {
				fprintf(stderr, errfmt, progname, "bad range", s, i);
			}
			low = high = -1;
			acc = 0;
			digits = 0;
			numbers = 0;
			continue;
		} else {
			fprintf(stderr, errfmt, progname, "bad character", s, i);
			return (-1);
		}		/* end if else if ... else */

	}

	if (low < 0 && high < 0)
		return (0);

	/*
	 * now, process it 
	 */

	if (high < 0)
		high = low;
	if (numbers < 1 || numbers > 2) {
		fprintf(stderr, errfmt, progname, "bad value number", s, i);
	} else if (set_character_class_range(low, high, acc) != 0) {
		fprintf(stderr, errfmt, progname, "bad range", s, i);
	}
	return (0);
}

static void
change_selection(tw, set, save)
TextWindow tw;
Bool    set;
Bool    save;
{
	int     r, c, start, end;

	tw->show_selection = set;
	if (save) {
		x_stash_selection(tw);
		tw->have_selection = False;
	}
	start = tw->sel_start_col;
	for (r = tw->sel_start_row; r <= tw->sel_end_row; r++) {
		end = (r == tw->sel_end_row) ? tw->sel_end_col : (tw->cols - 1);
		tw->line_attr[r] |= LINE_DIRTY;
		for (c = start; c <= end; c++) {
			tw->attr[r][c] |= CELL_DIRTY;
			if (set)
				tw->attr[r][c] |= CELL_SELECTION;
			else
				tw->attr[r][c] &= ~CELL_SELECTION;
		}
		start = 0;
	}
}

static void
x_lose_selection(tw)
TextWindow tw;
{
	if (tw->have_selection)
		change_selection(tw, False, False);
	tw->have_selection = False;
	tw->selection_len = 0;
	x_flush();		/* show the changes */
}

/* ARGSUSED */
static void
x_get_selection(tw, selection, type, value, length, format)
TextWindow tw;
Atom    selection;
Atom    type;
long    length;
char   *value;
int     format;
{
	if (format != 8 || type != XA_STRING)
		return;		/* can't handle incoming data */

	if (length) {
		pp = paste = (char *) malloc(length);
		if (!paste)
			goto bail;
		bcopy((char *) value, pp, length);
		plen = length;
	}
bail:
	free(value);
}

static void
x_paste_selection(tw)
TextWindow tw;
{
	if (tw->have_selection) {
		/* local transfer */
		if (tw->selection_len == 0)	/* stash it if it hasn't been */
			x_stash_selection(tw);
		x_get_selection(tw, XA_PRIMARY, XA_STRING,
		    strndup((char *) tw->selection_data, tw->selection_len),
				tw->selection_len, 8);
	} else {
		XConvertSelection(tw->dpy, XA_PRIMARY, XA_STRING, tw->sel_prop,
				  tw->win, CurrentTime);
	}
}

static void
x_stash_selection(tw)
TextWindow tw;
{
	unsigned char *data, *dp;
	int     r;
	int     length = 0;
	int     start, end;

	if (!tw->have_selection)
		return;
	if (tw->selection_len) {
		free(tw->selection_data);
		tw->selection_len = 0;
	}
	if (tw->sel_start_row == tw->sel_end_row) {
		length = tw->sel_end_col - tw->sel_start_col + 1;
		data = (unsigned char *) malloc(length);
		if (!data)
			return;
		bcopy((char *) &(tw->sc[tw->sel_start_row][tw->sel_start_col]),
		      data, length);
		tw->selection_len = length;
	} else {
		start = tw->sel_start_col;
		for (r = tw->sel_start_row; r <= tw->sel_end_row; r++) {
			end = (r == tw->sel_end_row) ? tw->sel_end_col : (tw->cols - 1);
			length += end - start + 2;	/* add CR */
			start = 0;
		}
		dp = data = (unsigned char *) malloc(length);
		if (!data)
			return;
		tw->selection_len = length;
		start = tw->sel_start_col;
		for (r = tw->sel_start_row; r <= tw->sel_end_row; r++) {
			end = (r == tw->sel_end_row) ? tw->sel_end_col : (tw->cols - 1);
			length = end - start + 1;
			bcopy((char *) &(tw->sc[r][start]), dp, length);
			dp += length;
			*dp++ = '\n';	/* add CR */
			start = 0;
		}
	}
	tw->selection_data = data;

#ifdef SABER_HACK
	XChangeProperty(dpy, RootWindow(tw->dpy, tw->screen), XA_CUT_BUFFER0,
			XA_STRING, 8, PropModeReplace,
			tw->selection_data, tw->selection_len);
#endif
}

/* ARGSUSED */
static  Bool
x_give_selection(tw, ev, target, prop)
TextWindow tw;
XSelectionRequestEvent *ev;
Atom    target;
Atom    prop;
{
	if (tw->show_selection)
		x_stash_selection(tw);
	XChangeProperty(dpy, ev->requestor, prop, XA_STRING, 8,
		    PropModeReplace, tw->selection_data, tw->selection_len);
	return True;
}

static void
x_own_selection(tw)
TextWindow tw;
{
	if (tw->selection_len) {/* get rid of old one */
		tw->selection_len = 0;
		free(tw->selection_data);
	}
	if (!tw->have_selection) {
		XSetSelectionOwner(tw->dpy, XA_PRIMARY, tw->win, CurrentTime);
	}
	change_selection(tw, True, False);
	x_flush();		/* show the changes */
}

static void
extend_selection(tw, nr, nc, wipe)
TextWindow tw;
int     nr, nc;
Bool    wipe;
{
	if (tw->have_selection)	/* erase any old one */
		change_selection(tw, False, False);

	if (wipe) {		/* a wipe is always relative to its starting
				 * point */
		if (nr > tw->wipe_row) {
			tw->sel_end_row = nr;
			tw->sel_end_col = nc;
			tw->sel_start_col = tw->wipe_col;
			tw->sel_start_row = tw->wipe_row;
		} else if (nr == tw->wipe_row) {
			tw->sel_end_row = tw->sel_start_row = nr;
			if (nc > tw->wipe_col) {
				tw->sel_start_col = tw->wipe_col;
				tw->sel_end_col = nc;
			} else {
				tw->sel_start_col = nc;
				tw->sel_end_col = tw->wipe_col;
			}
		} else {
			tw->sel_start_row = nr;
			tw->sel_start_col = nc;
			tw->sel_end_col = tw->wipe_col;
			tw->sel_end_row = tw->wipe_row;
		}
	} else {
		if (nr < tw->sel_start_row) {
			if (!tw->have_selection)
				tw->sel_end_row = tw->sel_start_row;
			tw->sel_start_row = nr;
		} else {
			tw->sel_end_row = nr;
		}

		if (nc < tw->sel_start_col) {
			if (!tw->have_selection)
				tw->sel_end_col = tw->sel_start_col;
			tw->sel_start_col = nc;
		} else {
			tw->sel_end_col = nc;
		}
	}

	tw->show_selection = True;
	x_own_selection(tw);
	tw->have_selection = True;

#ifdef SABER_HACK
	x_stash_selection(tw);
#endif
}

#define CLANG_MODE(wp)	((wp)->w_bufp->b_mode & MDCMODE)
#define MATCH_MODE(wp)	((wp)->w_bufp->b_mode & MDPMATCH)

static void
multi_click(tw, nr, nc)
TextWindow tw;
int     nr, nc;
{
	unsigned char *p, *pp;
	int     sc;
	int     cclass;

	tw->numclicks++;

	sc = tw->sel_start_col;
	switch (tw->numclicks) {
	case 0:
	case 1:
		mlwrite("multi_click: mouse botch: numclicks %d", tw->numclicks);
		return;
	case 2:		/* word or C construct */
		p = &(tw->sc[nr][sc]);
		if (sc)
			pp = &(tw->sc[nr][sc - 1]);
		else
			pp = (unsigned char *)0;
		/*
		 * Highlight syntactic units if we're in C mode or match
		 * mode.
		 */
		if (pp && ((CLANG_MODE(curwp) && strchr("\"{([", *pp)) || /* )]} */
			  (MATCH_MODE(curwp) && strchr("{([", *pp)))) /* )}] */ {
			register LINE *lp;
			LINE *slp, *smp;
			int slo, smo;
			int er;

#define setwpos(dp,do,mp,mo) \
	{ \
		curwp->w_dotp = dp; \
		curwp->w_doto = do; \
		curwp->w_markp = mp; \
		curwp->w_marko = mo; \
	}

			/* Save current point and mark values */
			slp = curwp->w_dotp;
			slo = curwp->w_doto;
			smp = curwp->w_markp;
			smo = curwp->w_marko;

			/*
			 * We have the beginning of a C construct.  Set the
			 * current file position to the point of the click,
			 * and attempt to set the region to the text in the
			 * construct.
			 */
			setcursor(nr, sc);
			if (matchregion(*pp) < 0) {
				setwpos(slp, slo, smp, smo);
				return;
			}
			/*
			 * If the region is wholly within the current visible
			 * window, set the selection to the region.  If it is
			 * not, write a message to the message line.
			 */
			lp = curwp->w_dotp;
			er = nr;
			while (lp != curwp->w_markp) {
				er++;
				lp = lforw(lp);
			}
			if (er > tw->rows) {
				mlwrite("cannot set selection: region exceeds screen boundaries");
				return;
			}
			/*
			 * The region ends at the matching close char, but we
			 * don't want to highlight that character.
			 */
			if (curwp->w_marko == 0) {
				lp = lback(curwp->w_markp);
				er--;
				nc = colpos(lp, llength(lp), 0);
			} else
				nc = colpos(curwp->w_markp, curwp->w_marko, 0) - 1;
			tw->sel_start_row = nr;
			tw->sel_end_row = er;
		} else if (ISWORD(*p)) {
			/* find word start */
			cclass = charClass[*p];
			do {
				--sc;
				--p;
			} while (sc >= 0 && charClass[*p] == cclass);
			sc++;
			/* and end */
			p = &(tw->sc[nr][nc]);
			cclass = charClass[*p];
			do {
				++nc;
				++p;
			} while (nc < tw->cols && charClass[*p] == cclass);
			--nc;
		} else {
			/* Simply select the current line by default. */
			sc = 0;
			nc = tw->cols - 1;
		}
		break;
	case 3:		/* line */
		sc = 0;
		nc = tw->cols - 1;
		break;
	case 4:		/* screen */
		/*
		 * XXX blow off till we can figure out where screen starts &
		 * ends 
		 */
	default:
		break;
	}
	tw->sel_start_col = sc;
	tw->sel_end_col = nc;
	tw->show_selection = True;
	x_own_selection(tw);
	tw->have_selection = True;

#ifdef SABER_HACK
	x_stash_selection(tw);
#endif
}

static void
start_selection(tw, ev, nr, nc)
TextWindow tw;
XButtonPressedEvent *ev;
int     nr, nc;
{
	if (tw->lasttime && (absol(ev->time - tw->lasttime) < tw->click_timeout)) {
		/* ignore extra clicks on other rows */
		if (nr == tw->sel_start_row) {
			multi_click(tw, nr, nc);
			return;
		}
	}
	tw->lasttime = ev->time;
	tw->numclicks = 1;

	if (tw->have_selection) {
		change_selection(tw, False, True);
		tw->show_selection = False;
	}
	tw->sel_start_row = nr;
	tw->sel_start_col = nc;
	tw->sel_end_row = nr;
	tw->sel_end_col = nc;
	tw->wipe_row = nr;
	tw->wipe_col = nc;

	if (tw->cur_row == (tw->rows - 1)) {
		return;
	}
	setcursor(nr, nc);
	/* 'False' forces the point to be centered */
	refresh((ev->state ? False : True), 0);
	(void) update();
}

static XMotionEvent *
compress_motion(ev)
XMotionEvent *ev;
{
	XEvent  nev;

	while (XPending(ev->display)) {
		XPeekEvent(ev->display, &nev);
		if (nev.type == MotionNotify &&
		    nev.xmotion.window == ev->window &&
		    nev.xmotion.subwindow == ev->subwindow)
			XNextEvent(ev->display, (XEvent *) ev);
		else
			break;
	}
	return ev;
}

/*
 * handle any non keyboard events 
 */
static void
x_process_event(ev)
XEvent *ev;
{
	int     nr, nc;
	Bool    changed = False;
	XSelectionEvent event;
	XMotionEvent *mev;
	XExposeEvent *gev;
	int	sc, sr, ec, er;

	switch (ev->type) {
	case SelectionClear:
		x_lose_selection(cur_win);
		break;
	case SelectionRequest:
		event.type = SelectionNotify;
		event.display = ev->xselectionrequest.display;
		event.requestor = ev->xselectionrequest.requestor;
		event.selection = ev->xselectionrequest.selection;
		event.time = ev->xselectionrequest.time;
		event.target = ev->xselectionrequest.target;
		event.target = ev->xselectionrequest.target;
		if (ev->xselectionrequest.property == None)	/* obsolete requestor */
			ev->xselectionrequest.property = ev->xselectionrequest.target;
		if (x_give_selection(cur_win, (XSelectionRequestEvent *) ev,
				     ev->xselectionrequest.target,
				     ev->xselectionrequest.property)) {
			event.property = ev->xselectionrequest.property;
		} else {
			event.property = None;
		}
		(void) XSendEvent(dpy, event.requestor, False, (unsigned long) 0,
				  (XEvent *) & event);
		break;
	case SelectionNotify:
		if (ev->xselection.property == None) {
			x_get_selection(cur_win, ev->xselection.selection,
					None, (char *) 0, 0, 8);
		} else {
			unsigned long bytesafter;
			unsigned long length;
			int     format;
			Atom    type;
			unsigned char *value;

			(void) XGetWindowProperty(dpy, cur_win->win,
						  ev->xselection.property,
			 0L, 100000, False, AnyPropertyType, &type, &format,
					      &length, &bytesafter, &value);
			XDeleteProperty(dpy, cur_win->win, ev->xselection.property);
			x_get_selection(cur_win, ev->xselection.selection,
				      type, (char *) value, length, format);
		}
		break;

	case Expose:
		gev = (XExposeEvent *)ev;

		sc = gev->x / cur_win->char_width;
		sr = gev->y / cur_win->char_height;
		ec = CEIL(gev->x + gev->width, cur_win->char_width);
		er = CEIL(gev->y + gev->height, cur_win->char_height);

		x_touch(cur_win, sc, sr, ec, er);
		cur_win->exposed = TRUE;

		if (ev->xexpose.count == 0)
			x_flush();
		break;
	case VisibilityNotify:
		cur_win->visibility = ev->xvisibility.state;
		XSetGraphicsExposures(dpy, cur_win->textgc,
				cur_win->visibility != VisibilityUnobscured);
		break;
	case EnterNotify:
	case FocusIn:
		cur_win->show_cursor = True;
		cur_win->line_attr[cur_win->cur_row] |= LINE_DIRTY;
		cur_win->attr[cur_win->cur_row][cur_win->cur_col] |=
			CELL_DIRTY | CELL_CURSOR;
		x_flush();
		break;
	case LeaveNotify:
	case FocusOut:
		cur_win->line_attr[cur_win->cur_row] |= LINE_DIRTY;
		cur_win->show_cursor = False;
		cur_win->attr[cur_win->cur_row][cur_win->cur_col] |= CELL_DIRTY;
		cur_win->attr[cur_win->cur_row][cur_win->cur_col] &= ~CELL_CURSOR;
		x_flush();
		break;
	case ConfigureNotify:
		nr = ev->xconfigure.height / cur_win->char_height;
		nc = ev->xconfigure.width / cur_win->char_width;

		if (nc != cur_win->cols) {
			changed = True;
			if (nc > XMAXCOL)
				nc = XMAXCOL - 1;
			if (nc > 0)
				term.t_ncol = nc;
		}
		if (nr != cur_win->rows) {
			changed = True;
			if (nr > XMAXROW)
				nr = XMAXROW - 1;
			if (nr > 0)
				term.t_nrow = nr - 1;		
		}
		if (changed) {
			x_resize_screen(cur_win, nr, nc);
			refresh(True, 1);
			(void) update();
		}
		break;
	case MotionNotify:
		if (ev->xmotion.state != Button1Mask)
			return;
		mev = compress_motion((XMotionEvent *) ev);
		nc = mev->x / cur_win->char_width;
		nr = mev->y / cur_win->char_height;
		/* bounds check */
		if (nr < 0 || nc < 0 || nr >= cur_win->rows || nc >= cur_win->cols)
			return;
		/* ignore any spurious motion during a multi-click */
		if (cur_win->numclicks > 1)
			return;
		extend_selection(cur_win, nr, nc, True);
		break;
	case ButtonPress:
		nc = ev->xbutton.x / cur_win->char_width;
		nr = ev->xbutton.y / cur_win->char_height;
		switch (ev->xbutton.button) {
		case Button1:	/* move button and set selection point */
			start_selection(cur_win, (XButtonPressedEvent *) ev, nr, nc);
			break;
		case Button2:	/* paste selection */
			/* if modifier, paste at mouse */
			if (ev->xbutton.state)
				setcursor(nr, nc);
			x_paste_selection(cur_win);
			break;
		case Button3:	/* end selection */
			extend_selection(cur_win, nr, nc, False);
			cur_win->wipe_row = 0;
			cur_win->wipe_col = 0;
			break;
		}
		break;
	case ClientMessage:
		if (ev->xclient.message_type == atom_WM_PROTOCOLS &&
		    (Atom) ev->xclient.data.l[0] == atom_WM_DELETE_WINDOW) {
		    	quit(FALSE, 0);	/* quit() could return */
		    	update();
		}
		break;
	}
}

/*
 * main event loop.  this means we'll be stuck if an event that needs instant
 * processing comes in while its off doing other work, but there's no (easy)
 * way around that. 
 */
static int
x_getc()
{
	XEvent  ev;
	XKeyEvent kev;
	char    buffer[10];
	KeySym  keysym;
	int     num;
	int     c;

	while (1) {
		if (plen) {	/* handle any queued pasted text */
			c = *pp++;
			if (--plen == 0)
				free(paste);
			return c;
		}
		XNextEvent(dpy, &ev);
		if (ev.type == KeyPress) {
			num = XLookupString((XKeyPressedEvent *) &ev.xkey,
					    buffer, 10, &keysym, &compst);
			/*
			 * fake arrow keys
			 */
			kev = ev.xkey;
			switch (keysym) {
			case XK_Left:
				return CCHR('B');
			case XK_Right:
				return CCHR('F');
			case XK_Up:
				return CCHR('P');
			case XK_Down:
				return CCHR('N');
			default:
				if (num == 1 && (kev.state & Mod1Mask))
					buffer[0] |= METABIT;
				if (num)
					return buffer[0];
			}
		} else
			x_process_event(&ev);
	}
	return EOF;
}

/* ARGSUSED */
static  Bool
check_kbd_ev(dpy, ev, arg)
Display *dpy;
XEvent *ev;
char   *arg;
{
	return (ev->type == KeyPress);
}

int
x_key_events_ready()
{
	XEvent  ev;

	/* XXX may want to use another mode */
	if (XEventsQueued(dpy, QueuedAlready))
		return XPeekIfEvent(dpy, &ev, check_kbd_ev, NULL);
	return FALSE;
}

/*
 * change reverse video status 
 */
static int
x_rev(state)
int     state;
{
	/* If state == CMODE, reverse video is on */
	cur_win->reverse = (state == CMODE);
	return(0);
}

/* change screen resolution */
static int
x_cres()
{
	return TRUE;
}

#if COLOR
static int
x_fcol()
{
	return(0);
}

static int
x_bcol()
{
	return(0);
}
#endif /* COLOR */

/* beep */
static int
x_beep()
{
	XBell(cur_win->dpy, 0);
	return(0);
}

static int
x_visibeep()
{
	XGCValues	gcval;
	static GC	vgc = 0;

	if (vgc == 0) {
		gcval.function = GXxor;
		gcval.foreground = (unsigned long) cur_win->fg ^ cur_win->bg;
		vgc = XCreateGC(cur_win->dpy,
				RootWindow(cur_win->dpy, cur_win->screen),
				GCFunction | GCForeground,
				&gcval);
	}

	XFillRectangle(cur_win->dpy, cur_win->win, vgc, 0, 0,
			(unsigned int) x_width(cur_win),
			(unsigned int) x_height(cur_win));
	XFlush(cur_win->dpy);
	XFillRectangle(cur_win->dpy, cur_win->win, vgc, 0, 0,
			(unsigned int) x_width(cur_win),
			(unsigned int) x_height(cur_win));
	return(0);
}	

/*
 * Set the name in the title bar and the icon name
 */
static char	winname[80] = "XCE@";
static char	hostname[64] = "";
static int	hostlen, winlen;

void
xsetname(s)
char	*s;
{
	if (hostname[0] == 0) {
		gethostname(hostname, sizeof(hostname));
		hostlen = strlen(hostname);
		strcpy(winname + 4, hostname);
		winlen = 4 + hostlen;
		strcpy(winname + winlen, " - ");
		winlen += 3;
	}
	if (winname[winlen] == *s && (strncmp(winname + winlen, s, sizeof(winname) - winlen - 1) == 0))
		return;

	strncpy(winname + winlen, s, sizeof(winname) - winlen - 1);
	XStoreName(cur_win->dpy, cur_win->win, winname);
	XSetIconName(cur_win->dpy, cur_win->win, winname);
}
	
/* Three functions from ttyio.c that must be available in an X11 environment */
int
typeahead()
{
	return (x_key_events_ready());
}

void
panic(s)
{
	xpanic(s);
}

int
cansusp()
{
	return 1;
}

int
isisofont(fn)
char	*fn;
{
	int	n;

	n = strlen(fn);
	if (strcmp(fn+n-9, "iso8859-1") == 0)
		return 1;
	return 0;
}
