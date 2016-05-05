/*  _ __   __ _ ___ __  __
** | '_ \ / _` |_ _|  \/  | naim
** | | | | (_| || || |\/| | Copyright 1998-2005 Daniel Reed <n@ml.org>
** |_| |_|\__,_|___|_|  |_| ncurses-based chat client
*/
#include "naim-int.h"
typedef struct {
	WINDOW	*win;
	FILE	*logfile;
	int	height;
	unsigned char
		dirty:1,
		small:1;
} win_t;

#define WIN_T
#include <naim/naim.h>

extern conn_t	*curconn;
extern faimconf_t	faimconf;
extern int	scrollbackoff, buddyc, doredraw, inpaste, withtextcomp;
extern time_t	now, awaytime;
extern double	nowf, changetime;
extern int	wbuddy_widthy;

extern win_t
	win_input G_GNUC_INTERNAL,
	win_buddy G_GNUC_INTERNAL,
	win_info G_GNUC_INTERNAL,
	win_away G_GNUC_INTERNAL;
extern int
	wsetup_called G_GNUC_INTERNAL,
	quakeoff G_GNUC_INTERNAL;
extern char
	*statusbar_text G_GNUC_INTERNAL;
win_t
	win_input,
	win_buddy,
	win_info,
	win_away;
int	wsetup_called = 0,
	quakeoff = 0;
char	*statusbar_text = NULL;


void	do_resize(conn_t *conn, buddywin_t *bwin) {
	int	small, height;

	if ((scrollbackoff == 0) || (conn != curconn) || !inconn || (bwin != curconn->curbwin)) {
		small = 1;
		height = 2*faimconf.wstatus.widthy;
	} else {
		small = 0;
		height = faimconf.wstatus.pady;
	}

	assert(bwin->nwin.win != NULL);
	nw_resize(&(bwin->nwin), height, faimconf.wstatus.widthx);
	werase(bwin->nwin.win);
	nw_move(&(bwin->nwin), height-1, 0);
	nw_printf(&(bwin->nwin), 0, 0, "\n\n\n");
	playback(conn, bwin, height);
	bwin->nwin.small = small;
}

void	statrefresh(void) {
	int	waiting, buddies, autohide = secs_getvar_int("autohide");

	if (curconn == NULL)
		return;

	if (inconn) {
		if ((curconn->curbwin->nwin.dirty != 0) || ((scrollbackoff > 0) && (curconn->curbwin->nwin.small != 0)))
			do_resize(curconn, curconn->curbwin);
		assert(curconn->curbwin->nwin.dirty == 0);
		curconn->curbwin->viewtime = nowf;
	}

	bupdate();

	if (inconn_real) {
		pnoutrefresh(curconn->curbwin->nwin.win,
			curconn->curbwin->nwin.height-faimconf.wstatus.widthy-1-scrollbackoff-quakeoff,
			0,
			faimconf.wstatus.starty,
			faimconf.wstatus.startx,
			faimconf.wstatus.starty+faimconf.wstatus.widthy-1+quakeoff,
			faimconf.wstatus.startx+faimconf.wstatus.widthx-1);
		if (inconsole)
			pnoutrefresh(curconn->nwin.win,
				faimconf.wstatus.pady-2*faimconf.wstatus.widthy/3-1-consolescroll,
				0,
				faimconf.wstatus.starty,
				faimconf.wstatus.startx,
				faimconf.wstatus.starty+2*faimconf.wstatus.widthy/3-1,
				faimconf.wstatus.startx+faimconf.wstatus.widthx-1);
		else if (((nowf - curconn->lastupdate) < autohide) || (curconn->online <= 0)) {
			int	sheight = faimconf.wstatus.widthy/4, sneak;

			if (curconn->online <= 0)
				sneak = sheight;
			else if ((nowf - curconn->lastupdate) <= SLIDETIME)
				sneak = sheight*(nowf - curconn->lastupdate)/SLIDETIME;
			else if ((nowf - curconn->lastupdate) >= (autohide - SLIDETIME))
				sneak = sheight*(autohide - (nowf - curconn->lastupdate))/SLIDETIME+.5;
			else
				sneak = sheight;
			pnoutrefresh(curconn->nwin.win,
				faimconf.wstatus.pady-sneak-1,
				0,
				faimconf.wstatus.starty,
				faimconf.wstatus.startx,
				faimconf.wstatus.starty+sneak-1,
				faimconf.wstatus.startx+faimconf.wstatus.widthx-1);
		}
	} else
		pnoutrefresh(curconn->nwin.win,
			faimconf.wstatus.pady-faimconf.wstatus.widthy-1-scrollbackoff,
			0,
			faimconf.wstatus.starty,
			faimconf.wstatus.startx,
			faimconf.wstatus.starty+faimconf.wstatus.widthy-1,
			faimconf.wstatus.startx+faimconf.wstatus.widthx-1);

	buddies = buddyc;
	if (buddies < 0) {
		waiting = 1;
		buddies = -buddies;
	} else
		waiting = 0;

	if (buddies > wbuddy_widthy)
		buddies = wbuddy_widthy;
	if ((changetime != -1) && (waiting || (changetime == 0)
		|| (autohide == 0) || (curconn->online <= 0)
		|| ((nowf - changetime) < autohide))) {
		int	sheight = secs_getvar_int("winlistchars"), sneak;

		if (waiting || (changetime == 0) || (autohide == 0) || (curconn->online <= 0))
			sneak = sheight;
		else if ((nowf - changetime) <= SLIDETIME)
			sneak = sheight*(nowf - changetime)/SLIDETIME;
		else if ((nowf - changetime) >= (autohide - SLIDETIME))
			sneak = sheight*(autohide - (nowf - changetime))/SLIDETIME+.5;
		else
			sneak = sheight;
		pnoutrefresh(win_buddy.win,
			0, 0,
			faimconf.wstatus.starty,
			faimconf.wstatus.startx+faimconf.wstatus.widthx-sneak-1,
			faimconf.wstatus.starty+buddies-1,
			faimconf.wstatus.startx+faimconf.wstatus.widthx-1);
	}
	if (statusbar_text != NULL) {
		nw_erase(&win_info);
		nw_printf(&win_info, CB(CONN,STATUSBAR), 1, " %*s ", -faimconf.winfo.widthx,
			statusbar_text);
	} else if (withtextcomp == 1) {
		nw_erase(&win_info);
		nw_printf(&win_info, CB(CONN,STATUSBAR), 1, " %*s ", -faimconf.winfo.widthx,
			"I can finish what you're typing; just press Tab.");
	} else if (withtextcomp == 2) {
		nw_erase(&win_info);
		nw_printf(&win_info, CB(CONN,STATUSBAR), 1, " %*s ", -faimconf.winfo.widthx,
			"What you're typing matches something you've typed before; hit Up to complete.");
	} else if (inpaste) {
		nw_erase(&win_info);
		nw_printf(&win_info, CB(CONN,STATUSBAR), 1, " %*s ", -faimconf.winfo.widthx,
			"Paste mode is enabled; press Ctrl-P to return to normal text input.");
	} else if (inconsole) {
		nw_touchwin(&win_input);
		nw_erase(&win_info);
		nw_printf(&win_info, CB(CONN,STATUSBAR), 1, " %*s ", -faimconf.winfo.widthx,
			"Status console is visible; press F1 to hide. Anything typed is a command.");
	} else if (inconn_real && (curconn->curbwin->et == CHAT) && (*(curconn->curbwin->winname) == ':')) {
#ifdef DEBUG_ECHO
		if (strcmp(curconn->curbwin->winname, ":RAW") != 0)
#endif
		{
			nw_erase(&win_info);
			if (strcmp(curconn->curbwin->winname, ":AWAYLOG") == 0)
				nw_printf(&win_info, CB(CONN,STATUSBAR), 1, " %*s ", -faimconf.winfo.widthx,
					"This is your /away log. Type /close to dismiss it, /set awaylog 0 to disable.");
			else
				nw_printf(&win_info, CB(CONN,STATUSBAR), 1, " %*s ", -faimconf.winfo.widthx,
					"This is a special naim window. You may /close this if it is not needed.");
		}
	}
	wnoutrefresh(win_info.win);
	wnoutrefresh(win_input.win);
	if ((awaytime > 0) && (secs_getvar_int("autounaway") != 0)) {
		nw_touchwin(&win_away);
		wnoutrefresh(win_away.win);
	}
	if (doredraw)
		clearok(curscr, TRUE);
	doupdate();
	if (doredraw) {
		clearok(curscr, FALSE);
		doredraw = 0;
	}
}

static void wsetup_colors(void) {
	int	i;

	fprintf(stderr, "Enabling color support...");
	i = start_color();
	if (i == ERR) {
		fprintf(stderr, " failed\r\n");
		fprintf(stderr, "Please contact Daniel Reed <n@ml.org> for assistance.\r\n");
		exit(1);
	}
	fprintf(stderr, " done: COLORS=%i COLOR_PAIRS=%i\r\n", COLORS, COLOR_PAIRS);

	fprintf(stderr, "Checking for enough colors...");
	if ((COLORS < nw_COLORS) || (COLOR_PAIRS < nw_COLORS*nw_COLORS)) {
		fprintf(stderr, " failed\r\n");
		fprintf(stderr, "\r\n"
			"Possible reasons for failure:\r\n"
#ifdef NCURSES_VERSION
			" o NCURSES_VERSION = " NCURSES_VERSION "\r\n"
			"   Check http://freshmeat.net/projects/ncurses/ to make sure this is current.\r\n"
#endif
			" o TERM = %s\r\n"
			"   The $TERM environment variable is used to tell your system what capabilities\r\n"
			"   your physical terminal supports. If you are using a Linux console your $TERM\r\n"
			"   should be \"linux\". On FreeBSD $TERM is frequently \"cons25\". If you are using\r\n"
			"   another type of terminal, such as PuTTy on Windows, it is responsible for\r\n"
			"   setting $TERM automatically based on the capabilities it supports. ncurses\r\n"
			"   looks up $TERM in your system's termcap or terminfo database to determine\r\n"
			"   the capabilities your terminal supports. If your $TERM is improperly set, or\r\n"
			"   if your termcap/terminfo database contains inaccurate information, ncurses\r\n"
			"   will not be able to function properly.\r\n"
			"\r\n"
			"   If you believe your $TERM may be at fault, try first setting it to \"linux\"\r\n"
			"   with the bash command:\r\n"
			"   \tTERM=linux naim\r\n"
			"   or the tcsh command:\r\n"
			"   \t(setenv TERM linux; naim)\r\n"
			"   In addition to linux, try vt100, vt220, cons25, dtterm, xterm-color,\r\n"
			"   vt100-color, and linux-koi8.\r\n",
				getenv("TERM"));
		exit(1);
	}
	fprintf(stderr, " done\r\n");

	fprintf(stderr, "Initializing default colors...");
#ifdef HAVE_USE_DEFAULT_COLORS
	i = use_default_colors();
	if (i == ERR) {
		fprintf(stderr, " failed\r\n");
		fprintf(stderr, "Please contact Daniel Reed <n@ml.org> for assistance.\r\n");
		exit(1);
	}
	fprintf(stderr, " done\r\n");
#else
	fprintf(stderr, " not supported\r\n");
#endif

	fprintf(stderr, "Initializing color pairs...");
	for (i = 0; i < COLOR_PAIRS; i++)
		init_pair(i, i%nw_COLORS, (i/nw_COLORS)==0?-1:i/nw_COLORS);
	fprintf(stderr, " done\r\n");
}

#define NWIN(WIN)	newwin(faimconf.WIN.widthy, faimconf.WIN.widthx, \
	faimconf.WIN.starty, faimconf.WIN.startx);
#define NPAD(WIN)	newpad(faimconf.WIN.pady, faimconf.WIN.widthx);

void	whidecursor(void) {
	if (curs_set(0) != ERR)
		leaveok(stdscr, TRUE);
	else
		leaveok(stdscr, FALSE);
}

void	wsetup(void) {
	void	*ptr;

	if (wsetup_called > 0)
		abort();

	fprintf(stderr, "Initializing ncurses...");
	ptr = initscr();
	if (ptr == NULL) {
		fprintf(stderr, " failed\r\n");
		fprintf(stderr, "Please contact Daniel Reed <n@ml.org> for assistance.\r\n");
		exit(1);
	}
	fprintf(stderr, " done: LINES=%i COLS=%i\r\n", LINES, COLS);

	fprintf(stderr, "Checking for large enough screen dimensions...");
	if ((LINES < 10) || (COLS < 44)) {
		fprintf(stderr, " failed\r\n");
		fprintf(stderr, "naim requires at least 10 rows and at least 44 columns.\r\n");
		exit(1);
	}
	fprintf(stderr, " done\r\n");

	wsetup_colors();
	cbreak();
	noecho();
	nonl();
	timeout(1);
	whidecursor();

	{
		win_t	dummy;

		memset(&dummy, 0, sizeof(dummy));
		dummy.win = stdscr;
		nw_initwin(&dummy, 0);
		nw_refresh(&dummy);
	}

	memset(&win_input, 0, sizeof(win_input));
	win_input.win = NWIN(winput);
	assert(win_input.win != NULL);
	nw_initwin(&win_input, cINPUT);
	scrollok(win_input.win, FALSE);

	memset(&win_buddy, 0, sizeof(win_buddy));
	win_buddy.win = NPAD(wstatus);
	assert(win_buddy.win != NULL);
	nw_initwin(&win_buddy, cWINLIST);
	scrollok(win_buddy.win, FALSE);

	memset(&win_info, 0, sizeof(win_info));
	win_info.win = NWIN(winfo);
	assert(win_info.win != NULL);
	nw_initwin(&win_info, cTEXT);
	scrollok(win_info.win, FALSE);

	memset(&win_away, 0, sizeof(win_away));
	win_away.win = NWIN(waway);
	assert(win_away.win != NULL);
	nw_initwin(&win_away, cWINLIST-1);
	scrollok(win_away.win, FALSE);

	nw_printf(&win_away, CI(INPUT,TEXT), 0, "%44s  ", "");
	nw_printf(&win_away, C(CONN,BUDDY_AWAY), 1, " You are away. ");
	nw_printf(&win_away, C(CONN,BUDDY), 1, "Type /away or send an IM ");
	nw_printf(&win_away, CI(INPUT,TEXT), 0, "    ");
	nw_printf(&win_away, C(CONN,BUDDY), 1, " to let me know you're back.            ");
	nw_printf(&win_away, CI(INPUT,TEXT), 0, "  %44s", "");

	wsetup_called = 1;
}

void	wshutitdown(void) {
	if (wsetup_called == 0) {
		echof(curconn, "WSHUTITDOWN", "wsetup() hasn't been called\n");
		return;
	}
	endwin();
	wsetup_called = 0;
}

void	win_resize(void) {
	conn_t	*conn = curconn;

	if (conn == NULL)
		return;

	nw_resize(&win_input, faimconf.winput.widthy, faimconf.winput.widthx);
	nw_mvwin(&win_input, faimconf.winput.starty, faimconf.winput.startx);
	nw_resize(&win_buddy, faimconf.wstatus.widthy, faimconf.wstatus.widthx);
	nw_mvwin(&win_buddy, faimconf.wstatus.starty, faimconf.wstatus.startx);
	nw_resize(&win_info,  faimconf.winfo.widthy,  faimconf.winfo.widthx);
	nw_mvwin(&win_info, faimconf.winfo.starty, faimconf.winfo.startx);
	do {
		buddywin_t	*bwin = conn->curbwin;

		naim_setversion(conn);
		assert(conn->nwin.win != NULL);
		nw_resize(&(conn->nwin), faimconf.wstatus.pady,
			faimconf.wstatus.widthx);
		nw_move(&(conn->nwin), faimconf.wstatus.pady-1, 0);
		if (bwin != NULL)
			do {
				nw_resize(&(bwin->nwin), 1, 1);
				bwin->nwin.dirty = 1;
			} while ((bwin = bwin->next) != conn->curbwin);
	} while ((conn = conn->next) != curconn);
}

int	nw_printf(win_t *win, int pair, int bold, const unsigned char *format, ...) {
	va_list	msg;

	assert(win != NULL);
	assert(win->win != NULL);
	assert(format != NULL);

	if (pair >= 2*COLOR_PAIRS) {
		bold = 1;
		pair -= 2*COLOR_PAIRS;
	} else if (pair >= COLOR_PAIRS) {
		bold = 0;
		pair -= COLOR_PAIRS;
	}

	va_start(msg, format);
	wattrset(win->win, (bold?A_BOLD:0) | COLOR_PAIR(pair));
	vwprintw(win->win, (char *)format, msg);
	wattrset(win->win, 0);
	va_end(msg);
	return(0);
}

int	nw_titlef(const unsigned char *format, ...) {
	va_list	msg;

	assert(format != NULL);

	if (*format == 0)
		printf("\033]0;" PACKAGE_NAME);
	else {
		printf("\033]0;" PACKAGE_NAME " ");
		va_start(msg, format);
		vprintf((char *)format, msg);
		va_end(msg);
	}
	printf("\033\\");
	return(0);
}

int	nw_statusbarf(const unsigned char *format, ...) {
	va_list	msg;
	char	buf[128];

	assert(format != NULL);

	va_start(msg, format);
	vsnprintf(buf, sizeof(buf), format, msg);
	va_end(msg);
	nw_erase(&win_info);
	nw_printf(&win_info, CB(CONN,STATUSBAR), 1, " %*s ", -faimconf.winfo.widthx,
		buf);
	nw_refresh(&win_info);
	return(0);
}

void	nw_initwin(win_t *win, int bg) {
	assert(win != NULL);

	idlok(win->win, TRUE);
	scrollok(win->win, TRUE);
	intrflush(win->win, FALSE);
	keypad(win->win, TRUE);
	meta(win->win, TRUE);
	wbkgd(win->win, COLOR_PAIR(nw_COLORS*faimconf.b[bg]));
	werase(win->win);
}

void	nw_erase(win_t *win) {
	werase(win->win);
}

void	nw_refresh(win_t *win) {
	wrefresh(win->win);
}

void	nw_attr(win_t *win, char B, char I, char U, char EM, char STRONG, char CODE) {
	int	attrs = A_NORMAL;

	if (B || EM || STRONG)
		attrs |= A_BOLD;
	if (I)
		attrs |= A_STANDOUT;
	if (U)
		attrs |= A_UNDERLINE;
	wattrset(win->win, attrs);
}

void	nw_color(win_t *win, int pair) {
	wcolor_set(win->win, pair, NULL);
}

void	nw_flood(win_t *win, int pair) {
	wbkgd(win->win, COLOR_PAIR(pair));
}

void	nw_addch(win_t *win, const unsigned long ch) {
	waddch(win->win, ch);
}

void	nw_addstr(win_t *win, const unsigned char *str) {
	waddstr(win->win, str);
}

void	nw_move(win_t *win, int row, int col) {
	wmove(win->win, row, col);
}

void	nw_delwin(win_t *win) {
	if (win->win != NULL) {
		delwin(win->win);
		win->win = NULL;
	}
}

void	nw_touchwin(win_t *win) {
	touchwin(win->win);
}

void	nw_newwin(win_t *win) {
	nw_delwin(win);
	win->height = faimconf.wstatus.pady;
	win->win = NPAD(wstatus);
	assert(win->win != NULL);
}

void	nw_hline(win_t *win, unsigned long ch, int row) {
	whline(win->win, ch, row);
}

void	nw_vline(win_t *win, unsigned long ch, int col) {
	wvline(win->win, ch, col);
}

void	nw_mvwin(win_t *win, int row, int col) {
	mvwin(win->win, row, col);
}

void	nw_resize(win_t *win, int row, int col) {
	win->height = row;
	wresize(win->win, row, col);
}

int	nw_getcol(win_t *win) {
	return(getcurx(win->win));
}

int	nw_getrow(win_t *win) {
	return(getcury(win->win));
}

void	nw_getline(win_t *win, char *buf, int buflen) {
	int	row = nw_getrow(win),
		col = nw_getcol(win),
		max = col;

	if (max >= buflen)
		max = buflen-1;
	mvwinnstr(win->win, row, 0, buf, max);
	buf[max] = 0;
	nw_move(win, row, col);
}

int	nw_getch(void) {
	int	k = getch();

	if (k == ERR)
		return(0);
	return(k);
}

void	nw_getpass(win_t *win, char *pass, int len) {
	int	i = -1;

	nw_erase(win);
	statrefresh();

	do {
		fd_set	rfd;
		int	ch;

		FD_ZERO(&rfd);
		FD_SET(STDIN_FILENO, &rfd);
		select(STDIN_FILENO+1, &rfd, NULL, NULL, NULL);

		if ((ch = getch()) == ERR)
			continue;

		if ((ch == '\b') || (ch == 0x7F) || (ch == KEY_BACKSPACE)) {
			if (i > -1) {
				assert(i >= 0);
				assert(i < len);
				pass[i] = 0;
				i--;
				nw_printf(win, C(INPUT,TEXT), 1, "\b \b");
			}
		} else {
			i++;
			assert(i >= 0);
			assert(i < len);
			pass[i] = ch;
			nw_printf(win, C(INPUT,TEXT), 1, ".");
		}
		statrefresh();
	} while ((pass[i] != '\n') && (pass[i] != '\r') && ((i+1) < len));
	pass[i] = 0;
}
