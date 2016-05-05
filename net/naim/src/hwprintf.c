/*  _ __   __ _ ___ __  __
** | '_ \ / _` |_ _|  \/  | naim
** | | | | (_| || || |\/| | Copyright 1998-2006 Daniel Reed <n@ml.org>
** |_| |_|\__,_|___|_|  |_| ncurses-based chat client
*/
#include <naim/naim.h>
#include <naim/modutil.h>

#include "naim-int.h"

extern faimconf_t	faimconf;
extern conn_t	*curconn;
extern int	inplayback;

typedef struct {
	win_t	*win;
	struct {
		int	len,
			lastwhite,
			firstwhite,
			secondwhite;
		char	buf[1024];
	} addch;
	unsigned char
		white:1,
		inbold:1,
		initalic:1,
		inunderline:1;
	int	pair;
} h_t;

static void h_zero(h_t *h, win_t *win) {
	h->win = win;
	{
		int	i, max;

		max = h->addch.len = nw_getcol(win);
		if (max >= sizeof(h->addch.buf))
			max = sizeof(h->addch.buf)-1;
		memset(h->addch.buf, 0, sizeof(h->addch.buf));
		nw_getline(h->win, h->addch.buf, sizeof(h->addch.buf));
		assert(strlen(h->addch.buf) == h->addch.len);
		h->addch.lastwhite = -1;
		h->addch.firstwhite = -1;
		h->addch.secondwhite = -1;
		for (i = 0; i < max; i++)
			if (isspace(h->addch.buf[i])) {
				h->addch.firstwhite = i;
				break;
			}
		for (i++; i < max; i++)
			if (isspace(h->addch.buf[i])) {
				h->addch.secondwhite = i;
				break;
			}
		for (i++; i < max; i++)
			if (isspace(h->addch.buf[i]))
				h->addch.lastwhite = i;
	}
	h->white = 0;
	h->inbold = 0;
	h->initalic = 0;
	h->inunderline = 0;
	h->pair = 0;
}

HOOK_DECLARE(notify);
static void nw_wrap_addch(h_t *h, unsigned char c) {
	if (h->win->logfile != NULL) {
		char	buf[2] = { c, 0 };

		HOOK_CALL(notify, (NULL, h->win, buf));
	}

	if (h->addch.len >= (faimconf.wstatus.widthx-1)) {
		int	i;

		if ((h->addch.lastwhite > -1) && (h->addch.lastwhite > h->addch.firstwhite) && (h->addch.lastwhite > h->addch.secondwhite)) {
			for (i = h->addch.len; i > h->addch.lastwhite; i--)
				nw_addstr(h->win, "\b \b");
			nw_addch(h->win, '\n');
			for (i = 0; i <= h->addch.secondwhite; i++)
				nw_addch(h->win, ' ');
			for (i = h->addch.lastwhite+1; i < h->addch.len; i++)
				nw_addch(h->win, h->addch.buf[i]);
			h->addch.len -= h->addch.lastwhite-1;
		} else {
			for (i = 0; i <= (h->addch.secondwhite+1); i++)
				nw_addch(h->win, ' ');
			h->addch.len = 0;
		}

		h->addch.len += h->addch.secondwhite+1;
		h->addch.lastwhite = -1;
		memset(h->addch.buf, 0, sizeof(h->addch.buf));
	}

	nw_addch(h->win, c);

	if (c == '\n') {
		h->addch.lastwhite = h->addch.firstwhite = h->addch.secondwhite = -1;
		h->addch.len = 0;
		memset(h->addch.buf, 0, sizeof(h->addch.buf));
	} else if (c == '\b') {
		if (h->addch.len > 0)
			h->addch.len--;
		if (h->addch.firstwhite == h->addch.len)
			h->addch.firstwhite = -1;
		if (h->addch.secondwhite == h->addch.len)
			h->addch.secondwhite = -1;
	} else {
		if (isspace(c)) {
			if (h->addch.firstwhite == -1)
				h->addch.firstwhite = h->addch.len;
			else if (h->addch.secondwhite == -1)
				h->addch.secondwhite = h->addch.len;
			else
				h->addch.lastwhite = h->addch.len;
		}
		h->addch.buf[h->addch.len] = c;
		h->addch.len++;
	}
}

/* this is a terrible way of doing this */
static void nw_wrap_addstr(h_t *h, const unsigned char *str) {
	if (str == NULL)
		nw_addch(h->win, '.');
	else {
		int	i;

		for (i = 0; str[i] != 0; i++)
			nw_wrap_addch(h, str[i]);
	}
}

static const struct {
	int	pair,
		R, G, B;
} colar[] = {
	{	COLOR_BLACK,	0x00, 0x00, 0x00	},
	{	COLOR_BLACK,	0xC0, 0xC0, 0xC0	},
	{	COLOR_RED,	0x80, 0x00, 0x00	},
	{	COLOR_RED,	0xFF, 0x00, 0x00	},
	{	COLOR_GREEN,	0x00, 0x80, 0x00	},
	{	COLOR_GREEN,	0x00, 0xFF, 0x00	},
	{	COLOR_YELLOW,	0x80, 0x80, 0x00	},
	{	COLOR_YELLOW,	0xFF, 0xFF, 0x00	},
	{	COLOR_BLUE,	0x00, 0x00, 0x80	},
	{	COLOR_BLUE,	0x00, 0x00, 0xFF	},
	{	COLOR_MAGENTA,	0x80, 0x00, 0x80	},
	{	COLOR_MAGENTA,	0xFF, 0x00, 0xFF	},
	{	COLOR_CYAN,	0x00, 0x80, 0x80	},
	{	COLOR_CYAN,	0x00, 0xFF, 0xFF	},
	{	COLOR_WHITE,	0x80, 0x80, 0x80	},
	{	COLOR_WHITE,	0xFF, 0xFF, 0xFF	},
};

#define CHECKTAG(tag)	(strcasecmp(tagbase, (tag)) == 0)
#define CHECKAMP(tag)	(strcasecmp(tagbuf, (tag)) == 0)

static const char *const parsehtml_pair_RGB(int pair, char bold) {
	static char	buf[20];
	int	i;

	for (i = 0; i < sizeof(colar)/sizeof(*colar); i++)
		if ((i%2 == bold) && (colar[i].pair == pair)) {
			snprintf(buf, sizeof(buf), "#%02X%02X%02X", colar[i].R, colar[i].G, colar[i].B);
			return(buf);
		}
	return("#FFFFFF");
}

static int parsehtml_pair_closest(int _pair, int R, int G, int B, char *inbold, char foreorback) {
	int	i,
		back = _pair/nw_COLORS,
		fore = _pair%nw_COLORS,
		bestval = 0xFFFFFF;
	char	bold = 0;

	for (i = 0; i < sizeof(colar)/sizeof(*colar); i++) {
		int	val = ((R-colar[i].R)*(R-colar[i].R)) + ((G-colar[i].G)*(G-colar[i].G)) + ((B-colar[i].B)*(B-colar[i].B));

		if (val < bestval) {
			bestval = val;
			if (foreorback == 'B')
				back = colar[i].pair;
			else {
				fore = colar[i].pair;
				bold = (i%2);
			}
		}
	}

	*inbold = bold;
	if (bold || (back != fore))
		return(nw_COLORS*back + fore);
	else if (fore != nw_COLORS-1)
		return(nw_COLORS*back + nw_COLORS-1);
	else
		return(nw_COLORS*back);
}

static int parsehtml_pair(const unsigned char *buf, int _pair, char *inbold, char foreorback) {
	int	R, G, B;

	if (*buf == '#') {
		buf++;
		R = 16*hexdigit(*buf) + hexdigit(*(buf+1));
		buf += 2;
		G = 16*hexdigit(*buf) + hexdigit(*(buf+1));
		buf += 2;
		B = 16*hexdigit(*buf) + hexdigit(*(buf+1));
	} else if (strncasecmp(buf, "rgb(", sizeof("rgb(")-1) == 0) {
		buf += sizeof("rgb(")-1;
		R = atoi(buf);
		buf = strchr(buf, ',');
		if (buf == NULL)
			return(_pair);
		G = atoi(buf);
		buf = strchr(buf, ',');
		if (buf == NULL)
			return(_pair);
		B = atoi(buf);
	} else
		return(_pair);
	return(parsehtml_pair_closest(_pair, R, G, B, inbold, foreorback));
}

static unsigned long parsehtml_tag(h_t *h, unsigned char *text, int backup) {
	unsigned char
		tagbuf[20], argbuf[1024], *textsave = text, *tagbase;
	int	tagpos = 0;
	static char
		last_inunderline = 0;
	static struct {
		int	pair;
		unsigned char
			inbold:1,
			initalic:1,
			inunderline:1;
	} fontstack[20];
	static int
		fontstacklen = 0;

	memset(tagbuf, 0, sizeof(tagbuf));
	memset(argbuf, 0, sizeof(argbuf));

	while (isspace(*text))
		text++;
	while ((*text) && (!isspace(*text)) && (*text != '>')) {
		if (tagpos < sizeof(tagbuf))
			tagbuf[tagpos++] = *text;
		text++;
	}
	while (isspace(*text))
		text++;
	tagpos = 0;
	while ((*text) && (*text != '>')) {
		if (tagpos < sizeof(argbuf))
			argbuf[tagpos++] = *text;
		text++;
	}

	tagbase = tagbuf;
	if (*tagbase == '/')
		tagbase++;

	if CHECKTAG("B") {
		h->inbold = (*tagbuf != '/');
	} else if CHECKTAG("I") {
		h->initalic = (*tagbuf != '/');
	} else if CHECKTAG("U") {
		h->inunderline = (*tagbuf != '/');
	} else if CHECKTAG("A") {
		if (*tagbuf != '/') {
			char	*t = argbuf;
			int	found = 0;

			while ((found == 0) && (*t != 0)) {
				while (isspace(*t))
					t++;
				if ((strncasecmp(t, "href ", sizeof("href ")-1) == 0) || (strncasecmp(t, "href=", sizeof("href=")-1) == 0)) {
					char	refbuf[256];
					int	i = 0;

					t += sizeof("href")-1;
					while (isspace(*t))
						t++;
					if (*t != '=')
						continue;
					t++;
					while (isspace(*t))
						t++;
					if ((*t == '"') || (*t == '\'')) {
						char	q = *t++;

						while ((*t != 0) && (*t != q)) {
							if (i < (sizeof(refbuf)-1))
								refbuf[i++] = *t;
							t++;
						}
						if (*t == q)
							t++;
					} else {
						while ((*t != 0) && (!isspace(*t))) {
							if (i < (sizeof(refbuf)-1))
								refbuf[i++] = *t;
							t++;
						}
					}
					refbuf[i] = 0;
					secs_setvar("lasturl", refbuf);
					last_inunderline = h->inunderline;
					h->inunderline = 1;
					found = 1;
				} else {
					while ((*t != 0) && (!isspace(*t)))
						t++;
					while (isspace(*t))
						t++;
					if (*t == '=') {
						t++;
						while (isspace(*t))
							t++;
						if ((*t == '"') || (*t == '\'')) {
							char	q = *t++;

							while ((*t != 0) && (*t != q))
								t++;
							if (*t == q)
								t++;
						} else
							while ((*t != 0) && (!isspace(*t)))
								t++;
					}
				}
			}
		} else {
			char	*lasturl = secs_getvar("lasturl");

			h->inunderline = last_inunderline;
			if ((lasturl != NULL) && (backup > 1) && (strncmp(textsave-backup, lasturl, strlen(lasturl)) != 0)) {
				nw_wrap_addstr(h, " [");
				nw_wrap_addstr(h, lasturl);
				nw_wrap_addch(h, ']');
			}
		}
	} else if CHECKTAG("IMG") {
		nw_wrap_addstr(h, "[IMAGE:");
		nw_wrap_addstr(h, argbuf);
		nw_wrap_addch(h, ']');
	} else if (CHECKTAG("BR") || CHECKTAG("BR/")) {
		nw_wrap_addstr(h, "\n ");
		h->white = 1;
	} else if CHECKTAG("HR") {
		nw_wrap_addstr(h, "----------------\n");
	} else if CHECKTAG("FONT") {
		if (inplayback || (secs_getvar_int("color") == 1)) {
		    if (*tagbuf != '/') {
			char	*t = argbuf;
			int	found = 0;

			while ((found == 0) && (*t != 0)) {
				while (isspace(*t))
					t++;
				if ((strncasecmp(t, "color ", sizeof("color ")-1) == 0) || (strncasecmp(t, "color=", sizeof("color=")-1) == 0)) {
					char	colbuf[20];
					int	i = 0;

					t += sizeof("color")-1;
					while (isspace(*t))
						t++;
					if (*t != '=')
						continue;
					t++;
					while (isspace(*t))
						t++;
					if ((*t == '"') || (*t == '\'')) {
						char	q = *t++;

						while ((*t != 0) && (*t != q)) {
							if (i < (sizeof(colbuf)-1))
								colbuf[i++] = *t;
							t++;
						}
						if (*t == q)
							t++;
					} else {
						while ((*t != 0) && (!isspace(*t))) {
							if (i < (sizeof(colbuf)-1))
								colbuf[i++] = *t;
							t++;
						}
					}
					colbuf[i] = 0;
					fontstack[fontstacklen].pair = h->pair;
					fontstack[fontstacklen].inbold = h->inbold;
					fontstack[fontstacklen].initalic = h->initalic;
					fontstack[fontstacklen].inunderline = h->inunderline;
					if (fontstacklen < sizeof(fontstack)/sizeof(*fontstack))
						fontstacklen++;
					{
						char	inbold = h->inbold;

						h->pair = parsehtml_pair(colbuf, h->pair, &inbold, 'F');
						h->inbold = inbold?1:0;
					}
					found = 1;
				} else {
					while ((*t != 0) && (!isspace(*t)))
						t++;
					while (isspace(*t))
						t++;
					if (*t == '=') {
						t++;
						while (isspace(*t))
							t++;
						if ((*t == '"') || (*t == '\'')) {
							char	q = *t++;

							while ((*t != 0) && (*t != q))
								t++;
							if (*t == q)
								t++;
						} else
							while ((*t != 0) && (!isspace(*t)))
								t++;
					}
				}
			}
		    } else {
			if (fontstacklen > 0)
				fontstacklen--;
			else
				fontstacklen = 0;
			h->pair = fontstack[fontstacklen].pair;
			h->inbold = fontstack[fontstacklen].inbold;
			h->initalic = fontstack[fontstacklen].initalic;
			h->inunderline = fontstack[fontstacklen].inunderline;
		    }
		}
	} else if CHECKTAG("PRE") {
	} else if CHECKTAG("P") {
	} else if (CHECKTAG("HTML") || CHECKTAG("BODY") || CHECKTAG("DIV") || CHECKTAG("SPAN")) {
		if (inplayback || (secs_getvar_int("color") == 1)) {
		    if (*tagbuf != '/') {
			char	*t = argbuf;
			int	found = 0;

			while ((found == 0) && (*t != 0)) {
				while (isspace(*t))
					t++;
				if ((strncasecmp(t, "bgcolor ", sizeof("bgcolor ")-1) == 0) || (strncasecmp(t, "bgcolor=", sizeof("bgcolor=")-1) == 0)) {
					char	colbuf[20];
					int	i = 0;

					t += sizeof("bgcolor")-1;
					while (isspace(*t))
						t++;
					if (*t != '=')
						continue;
					t++;
					while (isspace(*t))
						t++;
					if ((*t == '"') || (*t == '\'')) {
						char	q = *t++;

						while ((*t != 0) && (*t != q)) {
							if (i < (sizeof(colbuf)-1))
								colbuf[i++] = *t;
							t++;
						}
						if (*t == q)
							t++;
					} else {
						while ((*t != 0) && (!isspace(*t))) {
							if (i < (sizeof(colbuf)-1))
								colbuf[i++] = *t;
							t++;
						}
					}
					colbuf[i] = 0;
					fontstack[fontstacklen].pair = h->pair;
					fontstack[fontstacklen].inbold = h->inbold;
					fontstack[fontstacklen].initalic = h->initalic;
					fontstack[fontstacklen].inunderline = h->inunderline;
					if (fontstacklen < sizeof(fontstack)/sizeof(*fontstack))
						fontstacklen++;
					{
						char	inbold = h->inbold;

						h->pair = parsehtml_pair(colbuf, h->pair, &inbold, 'B');
						h->inbold = inbold?1:0;
					}
					found = 1;
				} else {
					while ((*t != 0) && (!isspace(*t)))
						t++;
					while (isspace(*t))
						t++;
					if (*t == '=') {
						t++;
						while (isspace(*t))
							t++;
						if ((*t == '"') || (*t == '\'')) {
							char	q = *t++;

							while ((*t != 0) && (*t != q))
								t++;
							if (*t == q)
								t++;
						} else
							while ((*t != 0) && (!isspace(*t)))
								t++;
					}
				}
			}
		    } else
			h->inbold = h->initalic = h->inunderline = fontstacklen = 0;
		}
	} else
		return(0);
	return((unsigned long)(text-textsave+1));
}

static unsigned long parsehtml_amp(h_t *h, unsigned char *text) {
	unsigned char
		tagbuf[20], *textsave = text;
	int	tagpos = 0;

	memset(tagbuf, 0, sizeof(tagbuf));

	while (isspace(*text))
		text++;
	while ((*text) && (!isspace(*text)) && (*text != ';')
		&& (*text != '\n')) {
		if (tagpos < sizeof(tagbuf)-1)
			tagbuf[tagpos++] = *text;
		else
			return(0);
		text++;
	}

	if (*text != ';')
		return(0);

	if (*tagbuf == '#') {
		int	c = atoi(tagbuf+1);

		if (naimisprint(c))
			nw_wrap_addch(h, c);
		else
			nw_wrap_addstr(h, keyname(c));
	} else if CHECKAMP("NBSP") {
		nw_wrap_addch(h, ' ');
	} else if CHECKAMP("AMP") {
		nw_wrap_addch(h, '&');
	} else if CHECKAMP("LT") {
		nw_wrap_addch(h, '<');
	} else if CHECKAMP("GT") {
		nw_wrap_addch(h, '>');
	} else if CHECKAMP("QUOT") {
		nw_wrap_addch(h, '"');
	} else
		return(0);

	h->white = 0;
	return((unsigned long)(text-textsave+1));
}

static unsigned long parsehtml(h_t *h, char *str, int backup) {
	if (*str == '<')
		return(parsehtml_tag(h, str+1, backup));
	else if (*str == '&')
		return(parsehtml_amp(h, str+1));
	else
		return(0);
}

int	vhwprintf(win_t *win, int _pair, const unsigned char *format, va_list msg) {
	static unsigned char
		buf[20*1024];
	size_t	len = 0;
	int	pos = -1, lines = 0;
	h_t	h;

	assert(win != NULL);
	assert(format != NULL);

	h_zero(&h, win);

	memset(buf, 0, sizeof(buf));

	len = vsnprintf(buf, sizeof(buf), format, msg);

	if (_pair > -1) {
		if (win->logfile != NULL)
			fprintf(win->logfile, "<font color=\"%s\">%s%s%s%s%s</font>\n",
				parsehtml_pair_RGB(_pair, h.inbold),
				h.initalic?"<I>":"",
				h.inunderline?"<U>":"",
				buf,
				h.inunderline?"</U>":"",
				h.initalic?"</I>":"");
	} else
		_pair = -_pair-1;

	if (_pair >= 2*(nw_COLORS*nw_COLORS)) {
		h.inbold = 1;
		_pair -= 2*(nw_COLORS*nw_COLORS);
	} else if (_pair >= (nw_COLORS*nw_COLORS)) {
		h.inbold = 0;
		_pair -= (nw_COLORS*nw_COLORS);
	}
	h.pair = _pair;

	nw_attr(win, h.inbold, h.initalic, h.inunderline, 0, 0, 0);
	nw_color(win, h.pair);
	while (++pos < len) {
		if ((buf[pos] == '<') || (buf[pos] == '&')) {
			static int	lastpos = 0;
			unsigned long	skiplen = 0;

			if ((skiplen = parsehtml(&h, buf+pos, pos-lastpos)) == 0) {
				nw_wrap_addch(&h, buf[pos]);
				continue;
			}
			pos += skiplen;
			nw_attr(win, h.inbold, h.initalic, h.inunderline, 0, 0, 0);
			nw_color(win, h.pair);
			lastpos = pos;
			continue;
		} else {
			if (buf[pos] == '\r')
				continue;
			if (isspace(buf[pos]) || (buf[pos] == '\n')) {
				if (h.white == 0)
					nw_wrap_addch(&h, ' ');
				h.white = 1;
				continue;
			} else
				h.white = 0;
			if (naimisprint(buf[pos]))
				nw_wrap_addch(&h, buf[pos]);
			else
				nw_wrap_addstr(&h, keyname(buf[pos]));
			continue;
		}
	}
	if (h.white == 1)
		nw_wrap_addch(&h, '\b');
	return(lines);
}

int	hwprintf(win_t *win, int _pair, const unsigned char *format, ...) {
	va_list	msg;
	int	lines;

	va_start(msg, format);
	lines = vhwprintf(win, _pair, format, msg);
	va_end(msg);

	return(lines);
}
