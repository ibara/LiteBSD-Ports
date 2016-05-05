/*  _ __   __ _ ___ __  __
** | '_ \ / _` |_ _|  \/  | naim
** | | | | (_| || || |\/| | Copyright 1998-2003 Daniel Reed <n@ml.org>
** |_| |_|\__,_|___|_|  |_| ncurses-based chat client
*/
#include <naim/naim.h>

#include "naim-int.h"

extern conn_t	*curconn;
extern int	wsetup_called, inplayback;
extern time_t	now;
extern double	nowf;
extern faimconf_t	faimconf;

void	status_echof(conn_t *conn, const unsigned char *format, ...) {
	char	buf[1024*8];
	va_list	msg;

	assert(conn != NULL);
	assert(format != NULL);

	inplayback = 1;
	WINTIME(&(conn->nwin), CONN);
	vhwprintf(&(conn->nwin), C(CONN,EVENT_ALT), "<B>***</B>", msg);
	snprintf(buf, sizeof(buf), "<B>&nbsp;%s</B><br>", format);
	va_start(msg, format);
	vhwprintf(&(conn->nwin), C(CONN,EVENT), buf, msg);
	va_end(msg);
	inplayback = 0;

	naim_lastupdate(conn);
}

void	window_echof(buddywin_t *bwin, const unsigned char *format, ...) {
	char	buf[1024*8];
	va_list	msg;

	assert(bwin != NULL);
	assert(format != NULL);

	if (bwin->et == CHAT)
		chat_flush(bwin);

	inplayback = 1;
	WINTIME(&(bwin->nwin), IMWIN);
	vhwprintf(&(bwin->nwin), C(IMWIN,EVENT_ALT), "<B>***</B>", msg);
	snprintf(buf, sizeof(buf), "<B>&nbsp;%s</B><br>", format);
	va_start(msg, format);
	vhwprintf(&(bwin->nwin), C(IMWIN,EVENT), buf, msg);
	va_end(msg);
	inplayback = 0;

	bupdate();
}

void	echof(conn_t *conn, const unsigned char *where, const unsigned char *format, ...) {
	char	buf[1024*8];
	va_list	msg;
	int	echostyle,
		statusecho;

	if (curconn == NULL)
		return;

	echostyle = getvar_int(conn, "echostyle");
	statusecho = (echostyle & ALSO_STATUS)?1:0;
	snprintf(buf, sizeof(buf), "<B>&nbsp;%s</B><br>", format);

	inplayback = 1;
	if (!(echostyle & ALWAYS_STATUS) && inconn) {
		int	col, col_alt;

		assert(curconn->curbwin != NULL);

		if (echostyle & NOLOG) {
			WINTIMENOLOG(&(curconn->curbwin->nwin), IMWIN);
			col = -C(IMWIN,EVENT)-1;
			col_alt = -C(IMWIN,EVENT_ALT)-1;
		} else {
			WINTIME(&(curconn->curbwin->nwin), IMWIN);
			col = C(IMWIN,EVENT);
			col_alt = C(IMWIN,EVENT_ALT);
		}

		hwprintf(&(curconn->curbwin->nwin), col_alt, "<B>***</B>");
		if ((where != NULL) || (conn != curconn)) {
			hwprintf(&(curconn->curbwin->nwin), col, "<B>&nbsp;[</B>");
			if ((where != NULL) && (conn != curconn)) {
				hwprintf(&(curconn->curbwin->nwin), col_alt, "<B>%s</B>", conn->winname);
				hwprintf(&(curconn->curbwin->nwin), col, "<B>:%s</B>", where);
			} else if (where != NULL)
				hwprintf(&(curconn->curbwin->nwin), col, "<B>%s</B>", where);
			else
				hwprintf(&(curconn->curbwin->nwin), col_alt, "<B>%s</B>", conn->winname);
			hwprintf(&(curconn->curbwin->nwin), col, "<B>]</B>");
		}
		va_start(msg, format);
		vhwprintf(&(curconn->curbwin->nwin), col, buf, msg);
		va_end(msg);
	} else
		statusecho = 1;

	if (statusecho) {
		WINTIME(&(conn->nwin), CONN);
		hwprintf(&(conn->nwin), C(CONN,EVENT_ALT), "<B>***</B>");
		if (where != NULL)
			hwprintf(&(conn->nwin), C(CONN,EVENT), "&nbsp;<B>[%s]</B>", where);
		va_start(msg, format);
		vhwprintf(&(conn->nwin), C(CONN,EVENT), buf, msg);
		va_end(msg);
	}
	inplayback = 0;
}
