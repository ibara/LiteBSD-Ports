/*
aim.c - FireTalk generic AIM definitions
Copyright (C) 2000 Ian Gulliver

This program is free software; you can redistribute it and/or modify
it under the terms of version 2 of the GNU General Public License as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*/
#include "firetalk-int.h"
#include "firetalk.h"
#include "aim.h"

#include <stdio.h>
#include <time.h>
#include <string.h>

char	*aim_interpolate_variables(const char *const input, const char *const nickname) {
	static char output[16384]; /* 2048 / 2 * 16 + 1 (max size with a string full of %n's, a 16-char nick and a null at the end) */
	int o = 0,gotpercent = 0;
	size_t nl,dl,tl,l,i;
	char date[15],tim[15];

	{ /* build the date and time */
		int hour;
		int am = 1;
		struct tm *t;
		time_t b;
		b = time(NULL);
		t = localtime(&b);
		if (t == NULL)
			return NULL;
		hour = t->tm_hour;
		if (hour >= 12)
			am = 0;
		if (hour > 12)
			hour -= 12;
		if (hour == 0)
			hour = 12;
		snprintf(tim, sizeof(tim), "%d:%02d:%02d %s", hour, t->tm_min, t->tm_sec, (am == 1)?"AM":"PM");
		snprintf(date, sizeof(date), "%d/%d/%d", t->tm_mon+1, t->tm_mday, t->tm_year+1900);
	}
	nl = strlen(nickname);
	dl = strlen(date);
	tl = strlen(tim);
	l = strlen(input);
	for (i = 0; i < l; i++)
		switch (input[i]) {
		  case '%':
			if (gotpercent == 1) {
				gotpercent = 0;
				output[o++] = '%';
				output[o++] = '%';
			} else
				gotpercent = 1;
			break;
		  case 'n':
			if (gotpercent == 1) {
				gotpercent = 0;
				memcpy(&output[o],nickname,nl);
				o += nl;
			} else
				output[o++] = 'n';
			break;
		  case 'd':
			if (gotpercent == 1) {
				gotpercent = 0;
				memcpy(&output[o],date,dl);
				o += dl;
			} else
				output[o++] = 'd';
			break;
		  case 't':
			if (gotpercent == 1) {
				gotpercent = 0;
				memcpy(&output[o],tim,tl);
				o += tl;
			} else
				output[o++] = 't';
			break;
		  default:
			if (gotpercent == 1) {
				gotpercent = 0;
				output[o++] = '%';
			}
			output[o++] = input[i];
		}
	output[o] = '\0';
	return output;
}

const char *aim_normalize_room_name(const char *const name) {
	static char newname[2048];

	if (name == NULL)
		return(NULL);
	if (strchr(name+1, ':') != NULL)
		return name;
	if (strlen(name) >= (sizeof(newname)-2))
		return(NULL);

	strcpy(newname, "4:");
	strcpy(newname+2, name);

	return(newname);
}

#define STRNCMP(x,y)	(strncmp((x), (y), sizeof(y)-1))
static char
	*htmlclean(const char *str) {
	static char
		buf[2048];
	int	i, b = 0;

	for (i = 0; (str[i] != 0) && (b < sizeof(buf)-1); i++)
		if (STRNCMP(str+i, "&gt;") == 0) {
			buf[b++] = '>';
			i += sizeof("&gt;")-2;
		} else if (STRNCMP(str+i, "&lt;") == 0) {
			buf[b++] = '<';
			i += sizeof("&lt;")-2;
		} else if (STRNCMP(str+i, "&quot;") == 0) {
			buf[b++] = '"';
			i += sizeof("&quot;")-2;
		} else if (STRNCMP(str+i, "&nbsp;") == 0) {
			buf[b++] = ' ';
			i += sizeof("&nbsp;")-2;
		} else if (STRNCMP(str+i, "&amp;") == 0) {
			buf[b++] = '&';
			i += sizeof("&amp;")-2;
		} else
			buf[b++] = str[i];
	buf[b] = 0;

	return(buf);
}
#undef STRNCMP

#define ECT_TOKEN	"<font ECT=\""
#define ECT_ENDING	"\"></font>"
char	*aim_handle_ect(void *conn, const char *const from,
		char *message, const int reply) {
	char	*ectbegin, *ectend, *textbegin, *textend;

	while ((ectbegin = strstr(message, ECT_TOKEN)) != NULL) {
		textbegin = ectbegin+sizeof(ECT_TOKEN)-1;

		if ((textend = strstr(textbegin, ECT_ENDING)) != NULL) {
			char	*arg;

			*textend = 0;
			ectend = textend+sizeof(ECT_ENDING)-1;

			if ((arg = strchr(textbegin, ' ')) != NULL) {
				*arg = '\0';
				arg++;
				if (reply == 1)
					firetalk_callback_subcode_reply(conn, from, textbegin, htmlclean(arg));
				else
					firetalk_callback_subcode_request(conn, from, textbegin, htmlclean(arg));
			} else {
				if (reply == 1)
					firetalk_callback_subcode_reply(conn, from, textbegin, NULL);
				else
					firetalk_callback_subcode_request(conn, from, textbegin, NULL);
			}
			memmove(ectbegin, ectend, strlen(ectend)+1);
		} else
			break;
	}
	return message;
}

const char *firetalk_nhtmlentities(const char *str, int len) {
	static char buf[1024];
	int	i, b = 0;

	for (i = 0; (str[i] != 0) && (b < sizeof(buf)-6-1) && ((len < 0) || (i < len)); i++)
		switch (str[i]) {
		  case '<':
			buf[b++] = '&';
			buf[b++] = 'l';
			buf[b++] = 't';
			buf[b++] = ';';
			break;
		  case '>':
			buf[b++] = '&';
			buf[b++] = 'g';
			buf[b++] = 't';
			buf[b++] = ';';
			break;
		  case '&':
			buf[b++] = '&';
			buf[b++] = 'a';
			buf[b++] = 'm';
			buf[b++] = 'p';
			buf[b++] = ';';
			break;
		  case '"':
			buf[b++] = '&';
			buf[b++] = 'q';
			buf[b++] = 'u';
			buf[b++] = 'o';
			buf[b++] = 't';
			buf[b++] = ';';
			break;
		  case '\n':
			buf[b++] = '<';
			buf[b++] = 'b';
			buf[b++] = 'r';
			buf[b++] = '>';
			break;
		  default:
			buf[b++] = str[i];
			break;
		}
	buf[b] = 0;
	return(buf);
}

const char *firetalk_htmlentities(const char *str) {
	return(firetalk_nhtmlentities(str, -1));
}
