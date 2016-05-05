/* lily.c - FireTalk SLCP protocol definitions
** Copyright 2002-2006 Daniel Reed <n@ml.org>
*/
#include <assert.h>	/* assert() */
#include <ctype.h>	/* isspace() */
#include <string.h>	/* strcasecmp() */
#include <stdarg.h>	/* va_* */
#include <stdio.h>	/* vsnprintf() */
#include <stdlib.h>
#include <time.h>	/* time_t */

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#define ROOMSTARTS "-"

typedef enum {
	UNKNOWN,
	HERE,
	AWAY,
	OFFLINE
} lily_state_t;

typedef struct {
	int		 handle;
	char		*name;
#if 0
	unsigned char	*blurb,
			*attrib,
			*pronoun;
#endif
#ifdef ENABLE_FT_LILY_CTCPMAGIC
	unsigned char	 ctcpmagic;
#endif
	time_t		 login,
			 idle;
	lily_state_t	 state;
} lily_user_t;

typedef struct {
	int		 handle;
	char		*name,
			*title;
	time_t		 creation,
			 idle;
	int		 users;
	char		 ismember,
			 isemote;
} lily_chat_t;

typedef struct {
	char	*nickname,
		*password,
		 buffer[1024+1];
	lily_user_t
		*lily_userar;
	int	 lily_userc;
	lily_chat_t
		*lily_chatar;
	int	 lily_chatc;
	struct {
		int	key;
		char	str[1024];
	}	*qar;
	int	 qc;
} lily_conn_t;

static lily_user_t
	*lily_user_find_hand(lily_conn_t *c, int handle) {
	int	i;

	for (i = 0; i < c->lily_userc; i++)
		if (c->lily_userar[i].handle == handle)
			return(c->lily_userar+i);
	return(NULL);
}

static lily_user_t
	*lily_user_find_name(lily_conn_t *c, const char *name) {
	int	i;

	for (i = 0; i < c->lily_userc; i++)
		if (strcasecmp(c->lily_userar[i].name, name) == 0)
			return(c->lily_userar+i);
	return(NULL);
}

static const char *lily_normalize_user_name(const char *const name) {
	static char	newname[2048];
	char		*ptr = strchr(name, ' ');

	if (ptr == NULL)
		return(name);

	strncpy(newname, name, sizeof(newname)-1);
	newname[sizeof(newname)-1] = 0;
	while ((ptr = strchr(newname, ' ')) != NULL)
		*ptr = '_';
	return(newname);
}

static void lily_user_add(lily_conn_t *c, const int handle, const char *const name, const char *const blurb,
	const time_t login, const time_t idle, lily_state_t state, const char *const attrib, const char *const pronoun) {

	assert(name != NULL);

	if ((lily_user_find_hand(c, handle) != NULL) || (lily_user_find_name(c, name) != NULL))
		return;

#if 0
	if (blurb == NULL)
		blurb = "";
	if (attrib == NULL)
		attrib = "";
	if (pronoun == NULL)
		pronoun = "its";
#endif

	c->lily_userc++;
	c->lily_userar = realloc(c->lily_userar, (c->lily_userc)*sizeof(*(c->lily_userar)));
	if (c->lily_userar == NULL)
		abort();
	c->lily_userar[c->lily_userc-1].handle = handle;
	c->lily_userar[c->lily_userc-1].name = strdup(lily_normalize_user_name(name));
	if (c->lily_userar[c->lily_userc-1].name == NULL)
		abort();
#if 0
	c->lily_userar[c->lily_userc-1].blurb = strdup(blurb);
	if (c->lily_userar[c->lily_userc-1].blurb == NULL)
		abort();
#endif
	c->lily_userar[c->lily_userc-1].login = login;
	c->lily_userar[c->lily_userc-1].idle = idle;
	c->lily_userar[c->lily_userc-1].state = state;
#if 0
	c->lily_userar[c->lily_userc-1].attrib = strdup(attrib);
	if (c->lily_userar[c->lily_userc-1].attrib == NULL)
		abort();
	c->lily_userar[c->lily_userc-1].pronoun = strdup(pronoun);
	if (c->lily_userar[c->lily_userc-1].pronoun == NULL)
		abort();
#endif
#ifdef ENABLE_FT_LILY_CTCPMAGIC
	c->lily_userar[c->lily_userc-1].ctcpmagic = 0;
#endif
}

static lily_state_t lily_user_state(const char *state) {
	if (strcasecmp(state, "here") == 0)
		return(HERE);
	else if (strcasecmp(state, "away") == 0)
		return(AWAY);
	else if (strcasecmp(state, "detach") == 0)
		return(OFFLINE);

	return(UNKNOWN);
}

static lily_chat_t *lily_chat_find_hand(lily_conn_t *c, int handle) {
	int	i;

	for (i = 0; i < c->lily_chatc; i++)
		if (c->lily_chatar[i].handle == handle)
			return(c->lily_chatar+i);
	return(NULL);
}

static lily_chat_t *lily_chat_find_name(lily_conn_t *c, const char *name) {
	int	i;

	for (i = 0; i < c->lily_chatc; i++)
		if (strcasecmp(c->lily_chatar[i].name, name) == 0)
			return(c->lily_chatar+i);
	return(NULL);
}

static const char *lily_normalize_room_name(const char *const name) {
	static char	newname[2048];

	if (strchr(ROOMSTARTS, *name))
		return(name);
	snprintf(newname, sizeof(newname), "-%s", name);
	return(newname);
}

static void lily_chat_add(lily_conn_t *c, int handle, const char *const name, const char *const title,
	const time_t creation, const time_t idle, const char *const attrib, const int users) {
	lily_chat_t	*lily_chat;

	if ((lily_chat = lily_chat_find_hand(c, handle)) != NULL) {
		free(lily_chat->name);
		free(lily_chat->title);
	} else {
		c->lily_chatc++;
		c->lily_chatar = realloc(c->lily_chatar, (c->lily_chatc)*sizeof(*(c->lily_chatar)));
		if (c->lily_chatar == NULL)
			abort();
		lily_chat = &(c->lily_chatar[c->lily_chatc-1]);
	}

	lily_chat->handle = handle;
	lily_chat->name = strdup(lily_normalize_room_name(name));
	if (lily_chat->name == NULL)
		abort();
	lily_chat->title = strdup(title);
	if (lily_chat->title == NULL)
		abort();
	lily_chat->creation = creation;
	lily_chat->idle = idle;
	lily_chat->users = users;
	lily_chat->ismember = 0;
	if (strstr(attrib, "emote") != NULL)
		lily_chat->isemote = 1;
	else
		lily_chat->isemote = 0;
}



typedef lily_conn_t *client_t;
#define _HAVE_CLIENT_T

#include "firetalk-int.h"
#include "firetalk.h"


#ifdef DEBUG_ECHO
static void lily_echof(lily_conn_t *const c, const char *const where, const char *const format, ...) {
	va_list	ap;
	char	buf[513];
	void	statrefresh(void);

	va_start(ap, format);
	vsnprintf(buf, sizeof(buf), format, ap);
	va_end(ap);

	while ((strlen(buf) > 0) && (buf[strlen(buf)-1] == '\n'))
		buf[strlen(buf)-1] = 0;
	if (*buf != 0)
		firetalk_callback_chat_getmessage(c, ":RAW", where, 0, buf);

	statrefresh();
}
#endif

static void lily_chat_joined(lily_conn_t *const c, lily_chat_t *const lily_chat) {
	assert(lily_chat != NULL);

#ifdef DEBUG_ECHO
	lily_echof(c, "chat_joined", "joined %s\n", lily_chat->name);
#endif

	lily_chat->ismember = 1;
}

static void lily_chat_parted(lily_conn_t *const c, lily_chat_t *const lily_chat) {
	assert(lily_chat != NULL);

#ifdef DEBUG_ECHO
	lily_echof(c, "chat_parted", "parted %s\n", lily_chat->name);
#endif

	lily_chat->ismember = 0;
}

static char *lily_html_to_lily(const char *const string) {
	static char	*output = NULL;
	int		o = 0;
	size_t		l, i = 0;

	l = strlen(string);
	output = realloc(output, (l * 3) + 1);
	if (output == NULL)
		abort();

	while (i < l) {
		switch(string[i]) {
			case '&':
				if (!strncasecmp(&string[i], "&amp;", 5)) {
					output[o++] = '&';
					i += 5;
				} else if (!strncasecmp(&string[i], "&gt;", 4)) {
					output[o++] = '>';
					i += 4;
				} else if (!strncasecmp(&string[i], "&lt;", 4)) {
					output[o++] = '<';
					i += 4;
				} else if (!strncasecmp(&string[i], "&nbsp;", 6)) {
					output[o++] = ' ';
					i += 6;
				} else
					output[o++] = string[i++];
				break;
			case '<':
				if (!strncasecmp(&string[i],"<b>",3)) {
					output[o++] = '*';
					i += 3;
				} else if (!strncasecmp(&string[i],"</b>",4)) {
					output[o++] = '*';
					i += 4;
				} else if (!strncasecmp(&string[i],"<i>",3)) {
					output[o++] = '/';
					i += 3;
				} else if (!strncasecmp(&string[i],"</i>",4)) {
					output[o++] = '/';
					i += 4;
				} else if (!strncasecmp(&string[i],"<u>",3)) {
					output[o++] = '_';
					i += 3;
				} else if (!strncasecmp(&string[i],"</u>",4)) {
					output[o++] = '_';
					i += 4;
				} else if (!strncasecmp(&string[i],"<br>",4)) {
					output[o++] = ' ';
					output[o++] = '/';
					output[o++] = ' ';
					i += 4;
				} else
					output[o++] = string[i++];
				break;
			case '\r':
			case '\n':
				output[o++] = ' ';
				output[o++] = '/';
				output[o++] = ' ';
				i++;
				break;
			case '\020':
				output[o++] = '?';
				i++;
				break;
			default:
				output[o++] = string[i++];
				break;
		}
	}
	output[o] = '\0';
	return output;
}

static char *lily_lily_to_html(const char *const string) {
	static char	*output = NULL;
	int		o = 0;
	size_t		l, i = 0;
	int		inbold = 0,
			initalics = 0,
			inunderline = 0;

	l = strlen(string);
	output = realloc(output, (l * 6) + 1);
	if (output == NULL)
		abort();

	while (i < l) {
		const char
			*sp = strchr(string+i, ' '), *ch;
		int	word_end = ((string[i+1] == ' ') || (string[i+1] == ',') || (string[i+1] == '.') || (string[i+1] == 0)),
			word_begin = ((i == 0) || (string[i-1] == ' ')),
			ch_end = 0;

		if (sp == NULL)
			sp = string+strlen(string);

		switch(string[i]) {
			case '*':
				if ((ch = strchr(string+i+1, '*')) == NULL)
					ch = sp;
				else
					ch_end = ((ch[1] == ' ') || (ch[1] == ',') || (ch[1] == '.') || (ch[1] == 0));
				if (inbold && !word_begin && word_end) {
					inbold = 0;
					memcpy(&output[o], "</b>", 4);
					o += 4;
				} else if (!inbold && word_begin && !word_end && (ch < sp) && ch_end) {
					inbold = 1;
					memcpy(&output[o], "<b>", 3);
					o += 3;
				} else
					output[o++] = string[i];
				break;
			case '/':
				if ((ch = strchr(string+i+1, '/')) == NULL)
					ch = sp;
				else
					ch_end = ((ch[1] == ' ') || (ch[1] == ',') || (ch[1] == '.') || (ch[1] == 0));
				if (word_begin && word_end) {
					i++;
					memcpy(&output[o], "<br>", 4);
					o += 4;
				} else if (initalics && word_end) {
					memcpy(&output[o], "</i>", 4);
					o += 4;
					initalics = 0;
				} else if (!initalics && word_begin && (ch < sp) && ch_end) {
					memcpy(&output[o], "<i>", 3);
					o += 3;
					initalics = 1;
				} else
					output[o++] = string[i];
				break;
			case '_':
				if ((ch = strchr(string+i+1, '_')) == NULL)
					ch = sp;
				else
					ch_end = ((ch[1] == ' ') || (ch[1] == ',') || (ch[1] == '.') || (ch[1] == 0));
				if (inunderline && word_end) {
					memcpy(&output[o], "</u>", 4);
					o += 4;
					inunderline = 0;
				} else if (!inunderline && word_begin && (ch < sp) && ch_end) {
					memcpy(&output[o], "<u>", 3);
					o += 3;
					inunderline = 1;
				} else
					output[o++] = string[i];
				break;
			case '&':
				memcpy(&output[o], "&amp;", 5);
				o += 5;
				break;
			case '<':
				memcpy(&output[o], "&lt;", 4);
				o += 4;
				break;
			case '>':
				memcpy(&output[o], "&gt;", 4);
				o += 4;
				break;
			case ' ':
				if (string[i+1] == ' ') {
					memcpy(&output[o], "&nbsp;", 6);
					o += 6;
				} else
					output[o++] = string[i];
				break;
			default:
				output[o++] = string[i];
				break;
		}
		i++;
	}
	output[o] = '\0';
	return output;
}

static fte_t lily_internal_disconnect(lily_conn_t *c, const int error) {
	if (c->nickname) {
		free(c->nickname);
		c->nickname = NULL;
	}
	if (c->password) {
		free(c->password);
		c->password = NULL;
	}
	if (c->qar != NULL) {
		free(c->qar);
		c->qar = NULL;
		c->qc = 0;
	}
	firetalk_callback_disconnect(c, error);

	return(FE_SUCCESS);
}

static fte_t lily_send_printf(lily_conn_t *c, const char *const format, ...) {
	va_list	ap;
	size_t	i,
		datai = 0;
	char	data[513];

	va_start(ap, format);
	for (i = 0; format[i] != 0; i++) {
		if (format[i] == '%') {
			switch (format[++i]) {
				case 'i': {
						int	i = va_arg(ap, int);

						i = snprintf(data+datai, sizeof(data)-2-datai, "%i", i);
						if (i > 0)
							datai += i;
						break;
					}
				case 's': {
						const char
							*s = lily_html_to_lily(va_arg(ap, char *));
						size_t	slen = strlen(s);

						if ((datai+slen) > (sizeof(data)-2-1))
							return(FE_PACKETSIZE);
						strcpy(data+datai, s);
						datai += slen;
						break;
					}
				case '%':
					data[datai++] = '%';
					break;
			}
		} else {
			data[datai++] = format[i];
			if (datai > (sizeof(data)-2-1))
				return(FE_PACKETSIZE);
		}
	}
	va_end(ap);

#ifdef DEBUG_ECHO
	lily_echof(c, "send_printf", "%.*s\n", datai, data);
#endif

	strcpy(data+datai, "\r\n");
	datai += 2;

	{
		struct s_firetalk_handle
			*fchandle;

		fchandle = firetalk_find_handle(c);
		firetalk_internal_send_data(fchandle, data, datai);
	}

	return(FE_SUCCESS);
}

typedef enum {
	QUEUE_JOIN,
} queuekey_t;

static fte_t lily_queue_printf(lily_conn_t *const c, const queuekey_t key, const char *const first, const char *const second, ...) {
	va_list	msg;
	int	i;

	for (i = 0; i < c->qc; i++)
		if (c->qar[i].key == key)
			break;

	va_start(msg, second);
	if (i == c->qc) {
		c->qc++;
		c->qar = realloc(c->qar, (c->qc)*sizeof(*(c->qar)));
		if (c->qar == NULL) {
			lily_internal_disconnect(c, FE_PACKET);
			return(FE_PACKET);
		}
		c->qar[i].key = key;
		vsnprintf(c->qar[i].str, sizeof(c->qar[i].str), first, msg);
	} else
		vsnprintf(c->qar[i].str+strlen(c->qar[i].str), sizeof(c->qar[i].str)-strlen(c->qar[i].str), second, msg);
	va_end(msg);
	return(FE_SUCCESS);
}

static char *lily_recv_line(lily_conn_t *c, char *buffer, unsigned short *bufferpos) {
#if 0
	static unsigned char
			str[1025];
	unsigned char	*r = memchr(buffer, '\r', *bufferpos),
			*n = memchr(buffer, '\n', *bufferpos),
			*split;

	if ((r != NULL) && (r < n)) {
		*r = 0;
		split = r;
	} else if (n != NULL) {
		*n = 0;
		split = n;
	} else
		return(NULL);

	if ((r+1) == n)
		split = n;

	strncpy(str, buffer, sizeof(str));
	str[sizeof(str)-1] = 0;

	*bufferpos -= (split-buffer+1);
	memmove(buffer, split+1, *bufferpos+1);
#else
	static char *str = NULL;
	char	*r;

	r = memchr(buffer, '\r', *bufferpos);
	if (r == NULL)
		return(NULL);
	assert(r < (buffer+*bufferpos));
	if (r == (buffer+*bufferpos-1))
		return(NULL);
	if (*(r+1) != '\n')
		return(NULL);
	*r = 0;
	r += 2;

	free(str);
	str = strdup(buffer);

	*bufferpos -= (r-buffer);
	memmove(buffer, r, *bufferpos);
#endif
	return(str);
}


struct {
	char	*what, *content;
} *lily_recvar = NULL;
int	lily_recvc = 0;

static void lily_recv_parse(lily_conn_t *c, char *line) {
	lily_recvc = 0;

	assert(line != NULL);

	while (*line != 0) {
		int	len;

		while (isspace(*line))
			*line++ = 0;
		if (*line == 0)
			break;

		lily_recvc++;
		lily_recvar = realloc(lily_recvar, lily_recvc*sizeof(*lily_recvar));
		assert(lily_recvar != NULL);

		lily_recvar[lily_recvc-1].what = line;
		lily_recvar[lily_recvc-1].content = "";

		while ((*line != '=') && !isspace(*line) && (*line != 0))
			line++;
		if (isspace(*line)) {
			*line++ = 0;
			continue;
		} else if (*line == 0)
			break;

		assert(*line == '=');
		*line++ = 0;
		/* USER HANDLE=#292 NAME=6=Jordan BLURB=0= LOGIN=1073622148 INPUT=1073622148 STATE= ATTRIB=11=finger,memo PRONOUN=3=his */
		/*                                                                                 ^                                    */
		assert(*line != '=');
		if (isspace(*line)) {
			*line++ = 0;
			continue;
		}
		lily_recvar[lily_recvc-1].content = line;

		while ((*line != '=') && !isspace(*line) && (*line != 0))
			line++;
		if (isspace(*line)) {
			*line++ = 0;
			continue;
		} else if (*line == 0)
			break;

		assert(*line == '=');
		*line++ = 0;
		len = atoi(lily_recvar[lily_recvc-1].content);
		lily_recvar[lily_recvc-1].content = line;
		line += len;
	}
}

static const char *lily_recv_get(const char *what) {
	int	i;

	for (i = 0; i < lily_recvc; i++)
		if (strcasecmp(lily_recvar[i].what, what) == 0)
			return(lily_recvar[i].content);
	return(NULL);
}

static void lily_recv_unhandled(lily_conn_t *c, const char *str) {
	char	buf[1024];
	int	i;

	*buf = 0;
	for (i = 0; i < lily_recvc; i++) {
		int	handle;
		const char
			*r;

		if ((lily_recvar[i].content[0] == '#')
			&& ((handle = atoi(lily_recvar[i].content+1)) > 0)) {
			const lily_user_t
				*lily_user = NULL;
			const lily_chat_t
				*lily_chat = NULL;

			if ((lily_user = lily_user_find_hand(c, handle)) != NULL)
				r = lily_user->name;
			else {
				if ((lily_chat = lily_chat_find_hand(c, handle)) != NULL)
					r = lily_chat->name;
				else
					r = lily_recvar[i].content;
			}
		} else
			r = lily_recvar[i].content;

		snprintf(buf, sizeof(buf), "%s%s=%s%s%s ", buf,
			lily_recvar[i].what,
			strchr(r, ' ')?"\"":"", r, strchr(r, ' ')?"\"":"");
	}
	firetalk_callback_chat_getmessage(c, ":RAW", str, 0, buf);
}




static fte_t lily_set_nickname(lily_conn_t *c, const char *const nickname) {
	lily_send_printf(c, "/reserve %s", nickname);
	lily_send_printf(c, "/rename %s", nickname);
	lily_send_printf(c, "/reserve %s", c->nickname);
	free(c->nickname);
	c->nickname = strdup(nickname);
	firetalk_callback_newnick(c, nickname);
	return(FE_SUCCESS);
}

static fte_t lily_set_password(lily_conn_t *c, const char *const oldpass, const char *const newpass) {
	if (lily_send_printf(c, "/PASSWORD") != FE_SUCCESS)
		return(FE_PACKET);
	if (lily_send_printf(c, "%s", oldpass) != FE_SUCCESS)
		return(FE_PACKET);
	if (lily_send_printf(c, "%s", newpass) != FE_SUCCESS)
		return(FE_PACKET);
	return(lily_send_printf(c, "%s", newpass));
}

static void lily_destroy_handle(lily_conn_t *c) {
	int	i;

	lily_internal_disconnect(c, FE_USERDISCONNECT);
	free(c->nickname);
	free(c->password);
	for (i = 0; i < c->lily_userc; i++)
		free(c->lily_userar[i].name);
	free(c->lily_userar);
	for (i = 0; i < c->lily_chatc; i++) {
		free(c->lily_chatar[i].name);
		free(c->lily_chatar[i].title);
	}
	free(c->lily_chatar);
	free(c->qar);
	free(c);
}

static fte_t lily_disconnect(lily_conn_t *c) {
	lily_send_printf(c, "/detach");
	return lily_internal_disconnect(c, FE_USERDISCONNECT);
}

static lily_conn_t *lily_create_handle(void) {
	lily_conn_t *c;

	c = calloc(1, sizeof(*c));
	if (c == NULL)
		abort();
	c->nickname = NULL;
	c->password = NULL;
	c->lily_userar = NULL;
	c->lily_userc = 0;
	c->lily_chatar = NULL;
	c->lily_chatc = 0;
	c->qar = NULL;
	c->qc = 0;

	return(c);
}

static fte_t lily_signon(lily_conn_t *c, const char *const nickname) {
	c->nickname = strdup(nickname);
	if (c->nickname == NULL)
		abort();

	if (lily_send_printf(c, "#$# options +sender +recip +usertype +whoami +slcp +sendgroup +connected +leaf-msg +prompt +leaf-cmd +leaf +prompt2") != FE_SUCCESS)
		return(FE_PACKET);

	{
		char	password[128];

		firetalk_callback_needpass(c, password, sizeof(password));
		if (lily_send_printf(c, "%s %s", nickname, password) != FE_SUCCESS)
			return(FE_PACKET);
		c->password = strdup(password);
		if (c->password == NULL)
			abort();
	}

	{
		char	*tmp, buf[1024];

		if ((tmp = firetalk_subcode_get_request_reply(c, "VERSION")) == NULL)
			tmp = PACKAGE_NAME ":" PACKAGE_VERSION ":unknown";
		strncpy(buf, tmp, sizeof(buf)-1);
		buf[sizeof(buf)-1] = 0;

		if ((tmp = strchr(buf, ' ')) != NULL)
			*tmp = 0;
		if ((tmp = strchr(buf, ':')) != NULL) {
			*tmp = ' ';
			if ((tmp = strchr(tmp+1, ':')) != NULL)
				*tmp = 0;
		}
		if (lily_send_printf(c, "#$# client %s", buf) != FE_SUCCESS)
			return(FE_PACKET);
	}

	return(FE_SUCCESS);
}

static fte_t lily_set_privacy(lily_conn_t *c, const char *const mode) {
	return(FE_SUCCESS);
}

static fte_t lily_preselect(lily_conn_t *c, fd_set *read, fd_set *write, fd_set *except, int *n) {
	int	i;

	for (i = 0; i < c->qc; i++)
		lily_send_printf(c, "%s", c->qar[i].str);
	free(c->qar);
	c->qar = NULL;
	c->qc = 0;

	return(FE_SUCCESS);
}

static fte_t lily_postselect(lily_conn_t *c, fd_set *read, fd_set *write, fd_set *except) {
	return FE_SUCCESS;
}

static char lily_tolower(const char c) {
	return(tolower(c));
}

static fte_t lily_compare_nicks(const char *const nick1, const char *const nick2) {
	int i = 0;

	while (nick1[i] != '\0') {
		if (lily_tolower(nick1[i]) != lily_tolower(nick2[i]))
			return FE_NOMATCH;
		i++;
	}
	if (nick2[i] != '\0')
		return FE_NOMATCH;

	return FE_SUCCESS;
}



static fte_t lily_got_notify(lily_conn_t *c) {
	const char	*event = lily_recv_get("EVENT"),
			*source,
			*_recips = lily_recv_get("RECIPS"),
			*_value = lily_recv_get("VALUE");
	char		buf[1024],
			*recips,
			reciplist[1024],
			*comma;
	int		handle = atoi(lily_recv_get("SOURCE")+1);
/*	time_t		eventtime = atol(lily_recv_get("TIME")+1); */
	lily_user_t	*lily_user_source = lily_user_find_hand(c, handle);

	if (lily_user_source == NULL)
		return(FE_PACKET);
	source = lily_user_source->name;

	firetalk_callback_im_buddyonline(c, source, 1);
	firetalk_callback_user_nickchanged(c, source, source);

	if (_value != NULL)
		_value = lily_lily_to_html(_value);

	*reciplist = 0;
	if (_recips != NULL) {
		int	showrecip = 0;

		strncpy(buf, _recips, sizeof(buf)-1);
		buf[sizeof(buf)-1] = 0;
		comma = strchr(buf, ',');

		if (comma != NULL) {
			recips = buf;

			while (recips != NULL) {
				const lily_user_t
					*lily_user = NULL;
				const lily_chat_t
					*lily_chat = NULL;
				const char
					*r;

				if (*recips != 0) {
					if (comma != NULL)
						*comma = 0;

					handle = atoi(recips+1);

					if ((lily_user = lily_user_find_hand(c, handle)) == NULL)
						lily_chat = lily_chat_find_hand(c, handle);
				}

				if (lily_user != NULL)
					r = lily_user->name;
				else if (lily_chat != NULL) {
					r = lily_chat->name;
					if (lily_chat->ismember == 0)
						showrecip = 1;
				} else
					r = "(unknown)";

				if (*reciplist == 0)
					snprintf(reciplist, sizeof(reciplist)-2, "[to %s", r);
				else
					snprintf(reciplist, sizeof(reciplist)-2, "%s, %s", reciplist, r);

				if (comma == NULL)
					recips = NULL;
				else {
					recips = comma+1;
					comma = strchr(recips, ',');
				}
			}

			if (*reciplist != 0)
				strcat(reciplist, "] ");

			strncpy(buf, _recips, sizeof(buf)-1);
			comma = strchr(buf, ',');
		}
		if (showrecip == 0)
			*reciplist = 0;
	} else {
		*buf = 0;
		comma = NULL;
	}
	recips = buf;

	while (recips != NULL) {
		const lily_user_t
				*lily_user = NULL;
		lily_chat_t	*lily_chat = NULL;

		if (*recips != 0) {
			if (comma != NULL)
				*comma = 0;

			handle = atoi(recips+1);

			if ((lily_user = lily_user_find_hand(c, handle)) == NULL)
				lily_chat = lily_chat_find_hand(c, handle);
		}

		if ((strcasecmp(event, "public") == 0) || (strcasecmp(event, "private") == 0)) {
			char	value[1024];

			if (*reciplist == 0) {
				strncpy(value, _value, sizeof(value)-1);
				value[sizeof(value)-1] = 0;
			} else
				snprintf(value, sizeof(value), "%s%s", reciplist, _value);

			if (lily_user != NULL) {
				if (strcasecmp(lily_user->name, c->nickname) == 0) {
					if (strncmp(value, "CTCP REQ ", sizeof("CTCP REQ ")-1) == 0) {
						char	*req = value+sizeof("CTCP REQ ")-1,
							*sp = strchr(req, ' ');

						if (sp != NULL)
							*sp++ = 0;
						firetalk_callback_subcode_request(c, source, req, sp);
					} else if (strncmp(value, "CTCP REP ", sizeof("CTCP REP ")-1) == 0) {
						char	*rep = value+sizeof("CTCP REP ")-1,
							*sp = strchr(rep, ' ');

						if (sp != NULL)
							*sp++ = 0;
						firetalk_callback_subcode_reply(c, source, rep, sp);
					} else if ((*value == '(') && (value[strlen(value)-1] == ')')) {
						value[strlen(value)-1] = 0;

						firetalk_callback_im_getmessage(c, source, 1, value+1);
					} else if ((*value == '*') && (value[strlen(value)-1] == '*') && 
						(strchr(value+1, '*') == value+strlen(value)-1)) {
						value[strlen(value)-1] = 0;

						firetalk_callback_im_getaction(c, source, 0, value+1);
					} else if (strncmp(value, "/me ", sizeof("/me ")-1) == 0)
						firetalk_callback_im_getaction(c, source, 0, value+sizeof("/me ")-1);
					else
						firetalk_callback_im_getmessage(c, source, 0, value);
				}
			} else if (lily_chat != NULL) {
				if (lily_chat->ismember != 0) {
					struct s_firetalk_handle	*conn;

					if ((conn = firetalk_find_handle(c)) != NULL)
						firetalk_chat_internal_add_member(conn, lily_chat->name, source);
					firetalk_callback_chat_getmessage(c, lily_chat->name, source, 0, value);
				}
			}
		} else if (strcasecmp(event, "emote") == 0) {
			if ((lily_chat != NULL) && (lily_chat->ismember != 0)) {
				if (strncmp(_value+1, "says, \"", sizeof("says, \"")-1) == 0) {
					char	value[1024];
					int	slen;

					strncpy(value, _value+1+sizeof("says, \"")-1, sizeof(value)-1);
					value[sizeof(value)-1] = 0;

					slen = strlen(value);
					if ((slen > 0) && (value[slen-1] == '"')) {
						value[--slen] = 0;
						if ((slen > 0) && (value[slen-1] == '"'))
							value[--slen] = 0;
						firetalk_callback_chat_getmessage(c, lily_chat->name, source, 0, value);
					} else
						firetalk_callback_chat_getaction(c, lily_chat->name, source, 0, _value+1);
				} else
					firetalk_callback_chat_getaction(c, lily_chat->name, source, 0, _value+1);
			}
		} else if (strcasecmp(event, "away") == 0) {
			lily_user_source->state = AWAY;
			firetalk_callback_im_buddyaway(c, source, 1);
		} else if (strcasecmp(event, "here") == 0) {
			lily_user_source->state = HERE;
			firetalk_callback_im_buddyaway(c, source, 0);
		} else if (strcasecmp(event, "detach") == 0) {
			lily_user_source->state = AWAY;
			firetalk_callback_im_buddyaway(c, source, 1);
		} else if (strcasecmp(event, "attach") == 0) {
			lily_user_source->state = HERE;
			firetalk_callback_im_buddyaway(c, source, 0);
		} else if (strcasecmp(event, "blurb") == 0) {
#ifdef DEBUG_ECHO
			lily_echof(c, "got_notify", "blurb %s = \"%s\"", source, _value);
#endif
		} else if (strcasecmp(event, "unidle") == 0) {
#ifdef DEBUG_ECHO
			lily_echof(c, "got_notify", "unidle %s", source);
#endif
		} else if (strcasecmp(event, "rename") == 0) {
			const char	*newname = lily_normalize_user_name(_value);

#ifdef DEBUG_ECHO
			lily_echof(c, "got_notify", "rename %s -> %s", source, _value);
#endif
			if (lily_compare_nicks(c->nickname, source) == 0) {
				free(c->nickname);
				c->nickname = strdup(_value);
				if (c->nickname == NULL)
					abort();
				firetalk_callback_newnick(c, c->nickname);
			}
			firetalk_callback_user_nickchanged(c, source, newname);
			free(lily_user_source->name);
			source = lily_user_source->name = strdup(newname);
		} else if (strcasecmp(event, "retitle") == 0) {
			assert(lily_chat != NULL);

			firetalk_callback_chat_gottopic(c, lily_chat->name, _value, source);
		} else if (strcasecmp(event, "drename") == 0) {
			const char	*newname = lily_normalize_user_name(_value);

#ifdef DEBUG_ECHO
			lily_echof(c, "got_notify", "rename %s -> %s", lily_chat->name, _value);
#endif
			free(lily_chat->name);
			lily_chat->name = strdup(newname);
		} else if (strcasecmp(event, "disconnect") == 0) {
			lily_user_source->state = OFFLINE;
			firetalk_callback_im_buddyonline(c, source, 0);
			firetalk_callback_chat_user_quit(c, source, "disconnect");
		} else if (strcasecmp(event, "connect") == 0) {
			if (lily_user_source->state == OFFLINE)
				lily_user_source->state = HERE;
			firetalk_callback_im_buddyonline(c, source, 1);
		} else if (strcasecmp(event, "quit") == 0) {
			if (lily_chat != NULL) {
				if (strcasecmp(source, c->nickname) == 0) {
					lily_chat_parted(c, lily_chat);
					firetalk_callback_chat_left(c, lily_chat->name);
				} else
					firetalk_callback_chat_user_left(c, lily_chat->name, source, NULL);
			}
		} else if (strcasecmp(event, "join") == 0) {
			if (lily_chat != NULL) {
				if (strcasecmp(source, c->nickname) == 0) {
					lily_chat_joined(c, lily_chat);
#ifdef DEBUG_ECHO
					lily_echof(c, "join", "#$# what #%i", lily_chat->handle);
#endif
					lily_send_printf(c, "#$# what #%i", lily_chat->handle);
				} else
					firetalk_callback_chat_user_joined(c, lily_chat->name, source, NULL);
			}
		} else if (strcasecmp(event, "depermit") == 0) {
			const char	*targstr = lily_recv_get("TARGETS");

			while (targstr != NULL) {
				int	targnum = atoi(targstr+1);
				lily_user_t
					*target = lily_user_find_hand(c, targnum);

				targstr = strchr(targstr, ',');
				if (lily_user == NULL)
					firetalk_callback_chat_user_kicked(c, lily_chat->name, target->name, source, "depermitted discussion");
			}
#if 0
		} else if (strcasecmp(event, "permit") == 0) {
			const char	*targstr = lily_recv_get("TARGETS");

			while (targstr != NULL) {
				int	targnum = atoi(targstr+1);
				lily_user_t
					*target = lily_user_find_hand(c, targnum);

				targstr = strchr(targstr, ',');
				if ((lily_user == NULL) && (lily_compare_nicks(c->nickname, target->name) == 0))
					firetalk_callback_chat_invited(c, lily_chat->name, source, "permitted discussion");
			}
#endif
		} else if (strcasecmp(event, "destroy") == 0) {
			if ((lily_user == NULL) && (lily_chat != NULL))
				firetalk_callback_chat_kicked(c, lily_chat->name, source, "depermitted/destroyed discussion");
			else
				firetalk_callback_error(c, FE_PACKET, NULL, "Unknown source for depermit: You have been kicked but I'm not sure from where.");
		} else
			lily_recv_unhandled(c, "%notify");

		if (comma == NULL)
			recips = NULL;
		else {
			recips = comma+1;
			comma = strchr(recips, ',');
		}
	}

	return(FE_SUCCESS);
}


static fte_t lily_got_DATA(lily_conn_t *c) {
	const char 	*name = lily_recv_get("NAME"),
			*_value = lily_recv_get("VALUE");
	int		handle = atoi(lily_recv_get("SOURCE")+1);
	const lily_chat_t
			*lily_chat_source = lily_chat_find_hand(c, handle);

	if (lily_chat_source == NULL)
		return(FE_PACKET);

	if (strcasecmp(name, "members") == 0) {
		char	buf[1024],
			*value = buf,
			*comma = NULL;

		firetalk_callback_chat_joined(c, lily_chat_source->name);

		if (_value != NULL) {
			strncpy(buf, _value, sizeof(buf));
			buf[sizeof(buf)-1] = 0;
			comma = strchr(value, ',');
		} else
			*value = 0;

		while (value != NULL) {
			/*const*/ lily_user_t
				*lily_user = NULL;

			if (*value != 0) {
				if (comma != NULL)
					*comma = 0;

				handle = atoi(value+1);

				lily_user = lily_user_find_hand(c, handle);
			}

			if (lily_user == NULL) {
#ifdef DEBUG_ECHO
				lily_echof(c, "got_DATA", "for %s, lily_user_find_hand(c=%#p, handle=%i) = NULL\n", lily_chat_source->name, c, handle);
#endif
			} else {
#ifdef ENABLE_FT_LILY_CTCPMAGIC
				if (strcasecmp(lily_chat_source->name, "-ctcpmagic") == 0)
					lily_user->ctcpmagic = 1;
#endif
				firetalk_callback_chat_user_joined(c, lily_chat_source->name, lily_user->name, NULL);
			}

			if (comma == NULL)
				value = NULL;
			else {
				value = comma+1;
				comma = strchr(value, ',');
			}
		}
		firetalk_callback_chat_user_joined(c, lily_chat_source->name, NULL, NULL);
	}

	return(FE_SUCCESS);
}


static fte_t lily_got_cmd(lily_conn_t *c, char *str) {
	static int	infolen = 0,
			blocknum = -1;
	static char	*blockwhat = NULL,
			*info = NULL;

#define BMATCH(x)	((strncmp(str, "%" x " ", sizeof(x)+1) == 0) \
			&& (str += sizeof(x)+1))
	if BMATCH("NOTIFY") {
#ifdef DEBUG_ECHO
		lily_echof(c, "got_cmd", "NOTIFY [%s]\n", str);
#endif
		lily_recv_parse(c, str);
		return(lily_got_notify(c));
	} else if BMATCH("DATA") {
#ifdef DEBUG_ECHO
		lily_echof(c, "got_cmd", "DATA [%s]\n", str);
#endif
		lily_recv_parse(c, str);
		return(lily_got_DATA(c));
	} else if BMATCH("USER") {
		const char	*login,
				*input;

		lily_recv_parse(c, str);

		login = lily_recv_get("LOGIN");
		input = lily_recv_get("INPUT");
		if (login == NULL)
			login = "0";
		if (input == NULL)
			input = "0";

		lily_user_add(c,
			atoi(lily_recv_get("HANDLE")+1),
			lily_recv_get("NAME"),
			lily_recv_get("BLURB"),
			atol(login),
			atol(input),
			lily_user_state(lily_recv_get("STATE")),
			lily_recv_get("ATTRIB"),
			lily_recv_get("PRONOUN"));
	} else if BMATCH("DISC") {
		const char	*creation,
				*input,
				*users;

		lily_recv_parse(c, str);

		creation = lily_recv_get("CREATION");
		input = lily_recv_get("INPUT");
		users = lily_recv_get("USERS");
		if (creation == NULL)
			creation = "0";
		if (input == NULL)
			input = "0";
		if (users == NULL)
			users = "0";

		lily_chat_add(c,
			atoi(lily_recv_get("HANDLE")+1),
			lily_recv_get("NAME"),
			lily_recv_get("TITLE"),
			atol(creation),
			atol(input),
			lily_recv_get("ATTRIB"),
			atoi(users));			
	} else if BMATCH("command") {
		char	*sp = strchr(str, ' ');

		assert(blocknum != -1);
		assert(blockwhat != NULL);
		//assert(blocknum == atoi(str+1));

		if ((sp != NULL) && (*(sp+1) != 0) && (*blockwhat == '/')) {
			*sp++ = 0;

			if (strncasecmp(blockwhat, "/REVIEW ", sizeof("/REVIEW ")-1) == 0) {
				if (*sp != '#')
					firetalk_callback_chat_getmessage(c, ":REVIEW", "*", 0, sp);
				else
					firetalk_callback_chat_getmessage(c, ":REVIEW", "#", 0, sp+2);
			} else if (strncasecmp(blockwhat, "/MEMO ", sizeof("/MEMO ")-1) == 0) {
				if (*sp != '#')
					firetalk_callback_chat_getmessage(c, ":MEMO", "*", 0, sp);
				else
					firetalk_callback_chat_getmessage(c, ":MEMO", "#", 0, sp+2);
			} else if (strncasecmp(blockwhat, "/FINGER ", sizeof("/FINGER ")-1) == 0) {
				if (infolen == 0) {
					infolen = strlen(sp)+1;
					info = calloc(1, infolen+1);
					if (info == NULL)
						abort();
					*info = ' ';
					strcpy(info+1, sp);
				} else {
					infolen += sizeof("<BR>")-1 + strlen(sp);
					info = realloc(info, infolen+1);
					if (info == NULL)
						abort();
					strcat(info, "<BR>");
					strcat(info, sp);
				}
			} else if (strncasecmp(blockwhat, "/WHAT ", sizeof("/WHAT ")-1) == 0) {
				if (infolen == 0) {
					infolen = strlen(sp)+3;
					info = calloc(1, infolen+1);
					if (info == NULL)
						abort();
					strcpy(info, " * ");
					strcpy(info+3, sp);
				} else {
					infolen += sizeof("<BR>")-1 + 3 + strlen(sp);
					info = realloc(info, infolen+1);
					if (info == NULL)
						abort();
					strcat(info, "<BR> * ");
					strcat(info, sp);
				}
			} else if (strncasecmp(blockwhat, "/WHERE ", sizeof("/WHERE ")-1) == 0) {
				assert(infolen != 0);

				infolen += sizeof("<BR>")-1 + sizeof("<BR>")-1 + strlen(sp) + sizeof("<BR>")-1;
				info = realloc(info, infolen+1);
				if (info == NULL)
					abort();
				strcat(info, "<BR>");
				strcat(info, "<BR>");
				strcat(info, sp);
				strcat(info, "<BR>");
			} else if (strncasecmp(blockwhat, "/INFO ", sizeof("/INFO ")-1) == 0) {
/*				if (strcasecmp(blockwhat, "/INFO SET") != 0) { */
					assert(infolen != 0);

					infolen += sizeof("<BR>")-1 + strlen(sp);
					info = realloc(info, infolen+1);
					if (info == NULL)
						abort();
					strcat(info, "<BR>");
					strcat(info, sp);
/*				} */
			} else if (strncasecmp(blockwhat, "/JOIN ", sizeof("/JOIN ")-1) == 0) {
				if (strncmp(sp, "(could find no discussion to match to ", sizeof("(could find no discussion to match to ")-1) == 0)
					firetalk_callback_error(c, FE_ROOMUNAVAILABLE, blockwhat+sizeof("/JOIN ")-1, sp);
				else if (strncmp(sp, "(you are already a member of ", sizeof("(you are already a member of ")-1) == 0)
					;
				else
					firetalk_callback_error(c, FE_BADROOM, blockwhat+sizeof("/JOIN ")-1, sp);
			} else if (strncasecmp(blockwhat, "/QUIT ", sizeof("/QUIT ")-1) == 0) {
				if (strncmp(sp, "(could find no discussion to match to ", sizeof("(could find no discussion to match to ")-1) == 0)
					firetalk_callback_error(c, FE_ROOMUNAVAILABLE, blockwhat+sizeof("/QUIT ")-1, sp);
				else
					firetalk_callback_error(c, FE_BADROOM, blockwhat+sizeof("/QUIT ")-1, sp);
			} else if (strncasecmp(blockwhat, "/CREATE ", sizeof("/CREATE ")-1) == 0) {
				if (strstr(sp, " is already in use)") != NULL)
					firetalk_callback_error(c, FE_ROOMUNAVAILABLE, blockwhat+sizeof("/CREATE ")-1, sp);
				else
					firetalk_callback_error(c, FE_BADROOM, blockwhat+sizeof("/CREATE ")-1, sp);
			} else if (strcasecmp(blockwhat, "/WHERE") == 0) {
				char	*comma;

				if (strncmp(sp, "You are a member of ", sizeof("You are a member of ")-1) == 0) {

					sp += sizeof("You are a member of ")-1;
					comma = strchr(sp, ',');

					while (sp != NULL) {
						/*const*/ lily_chat_t
							*lily_chat;

						if (comma != NULL)
							*comma = 0;

						while (isspace(*sp))
							sp++;

						if ((lily_chat = lily_chat_find_name(c, lily_normalize_room_name(sp))) == NULL)
							abort();

						lily_chat_joined(c, lily_chat);

						if (lily_send_printf(c, "#$# what #%i", lily_chat->handle) != FE_SUCCESS)
							return(FE_PACKET);

						if (comma == NULL)
							sp = NULL;
						else {
							sp = comma+1;
							comma = strchr(sp, ',');
						}
					}
				}
			} else
				firetalk_callback_chat_getmessage(c, ":RAW", blockwhat, 0, sp);
		} else if (strstr(str, " is ignoring you ") != NULL) {
			char	*co;

			if ((co = strchr(blockwhat, ':')) != NULL) {
				*co = 0;
				firetalk_callback_error(c, FE_USERUNAVAILABLE, blockwhat, "You are being ignored");
			}
		}
	} else if BMATCH("begin") {
		char	*sp = strchr(str, ' ');

		if (sp != NULL) {
			*sp++ = 0;
			blocknum = atoi(str+1);
			free(blockwhat);
			blockwhat = strdup(sp);
#ifdef DEBUG_ECHO
			lily_echof(c, "got_cmd", "beginning [%s]\n", blockwhat);
#endif
		}
	} else if BMATCH("end") {
		//assert(blocknum == atoi(str+1));

		if (infolen) {
			char	*who = strchr(blockwhat, ' ');

			assert(who != NULL);
			*who++ = 0;

			if (strcasecmp(blockwhat, "/FINGER") == 0)
				lily_send_printf(c, "/WHERE %s", who);
			else if (strcasecmp(blockwhat, "/WHERE") == 0)
				lily_send_printf(c, "/INFO %s", who);
			else if (strcasecmp(blockwhat, "/WHAT") == 0)
				lily_send_printf(c, "/INFO %s", who);
			else if (strcasecmp(blockwhat, "/INFO") == 0) {
				firetalk_callback_gotinfo(c, who, info, 0, 0, 0, 0);
				infolen = 0;
				free(info);
				info = NULL;
			}
		}

#ifdef DEBUG_ECHO
		lily_echof(c, "got_cmd", "ending [%s]\n", blockwhat);
#endif
		blocknum = -1;
		free(blockwhat);
		blockwhat = NULL;
	} else if BMATCH("pong") {
		if (strncmp(str, "!LC! ", sizeof("!LC! ")-1) == 0)
			firetalk_callback_subcode_request(c, c->nickname, "LC", str+sizeof("!LC! ")-1);
		else
			firetalk_callback_chat_getmessage(c, ":RAW", "%pong", 0, str);
	} else if BMATCH("prompt") {
		if (lily_send_printf(c, "") != FE_SUCCESS)
			return(FE_PACKET);
	} else if BMATCH("options") {
#ifdef DEBUG_ECHO
		lily_echof(c, "got_cmd", "options [%s]\n", str);
#endif
	} else if BMATCH("connected") {
#ifdef DEBUG_ECHO
		lily_echof(c, "got_cmd", "connected [%s]\n", str);
#endif
	} else if BMATCH("whoami") {
		lily_recv_parse(c, str);

		firetalk_callback_newnick(c, lily_recv_get("name"));
	} else
		firetalk_callback_chat_getmessage(c, ":RAW", "(unrecognized)", 0, str);
#undef BMATCH

	return(FE_SUCCESS);
}

static fte_t lily_got_data(lily_conn_t *c, unsigned char *_buffer, unsigned short *bufferpos) {
	char	*buffer = (char *)_buffer, *str;

	while ((str = lily_recv_line(c, buffer, bufferpos)) != NULL)
		lily_got_cmd(c, str);

	return(FE_SUCCESS);
}

static fte_t lily_got_data_connecting(lily_conn_t *c, unsigned char *_buffer, unsigned short *bufferpos) {
	char	*buffer = (char *)_buffer, *str;

	while ((str = lily_recv_line(c, buffer, bufferpos)) != NULL) {
		if (strncmp(str, "%whoami ", sizeof("%whoami ")-1) == 0) {
			char	*s = strstr(str, " name="),
				buf[1024];
			int	len;

			s += sizeof(" name=")-1;
			len = atoi(s);
			assert(len < sizeof(buf)-1);
			s = strchr(s, '=');
			strncpy(buf, s+1, len);
			buf[len] = 0;
			free(c->nickname);
			c->nickname = strdup(buf);
			if (c->nickname == NULL)
				abort();
		} else if (strncmp(str, "%connected ", sizeof("%connected ")-1) == 0) {
			firetalk_callback_doinit(c, c->nickname);
			firetalk_callback_connected(c);
#ifdef ENABLE_FT_LILY_CTCPMAGIC
/*			if (lily_send_printf(c, "/CREATE ctcpmagic \"For identifying CTCP-aware clients, no discussion\"") != FE_SUCCESS)
				return(FE_PACKET); */
			if (lily_send_printf(c, "/JOIN -ctcpmagic") != FE_SUCCESS)
				return(FE_PACKET);
#endif
			if (lily_send_printf(c, "/WHERE") != FE_SUCCESS)
				return(FE_PACKET);
		} else if (str[0] == '%')
			lily_got_cmd(c, str);
		else if (str[0] == '#') {
			if (str[2] != 0)
				firetalk_callback_chat_getmessage(c, ":REVIEW", "#", 0, str+2);
		}

#if 0
		if (0) {
			firetalk_callback_connectfailed(c,FE_PACKET,"Server returned ERROR");
			return FE_PACKET;
		}
#endif
	}

	return(FE_SUCCESS);
}

static fte_t lily_isprint(const int c) {
	if (isprint(c))
		return(FE_SUCCESS);
	return(FE_INVALIDFORMAT);
}

static fte_t lily_chat_join(lily_conn_t *c, const char *const room) {
	return(lily_queue_printf(c, QUEUE_JOIN, "/JOIN %s", ",%s", room));
/*	return lily_send_printf(c, "/JOIN %s", room); */
}

static fte_t lily_chat_part(lily_conn_t *c, const char *const room) {
	return lily_send_printf(c, "/QUIT %s", room);
}

static fte_t lily_chat_send_message(lily_conn_t *c, const char *const room, const char *const message, const int auto_flag) {
	if (auto_flag == 1)
		return lily_send_printf(c, "%s:(%s)", room, message);
	else if (strcasecmp(room, ":RAW") == 0)
		return lily_send_printf(c, "%s", message);
	else {
		lily_chat_t	*lily_chat;

		if (((lily_chat = lily_chat_find_name(c, room)) != NULL) && (lily_chat->isemote != 0))
			return(lily_send_printf(c, "%s:\"%s", room, message));
		else
			return(lily_send_printf(c, "%s:%s", room, message));
	}
}

static fte_t lily_chat_send_action(lily_conn_t *c, const char *const room, const char *const message, const int auto_flag) {
	if (auto_flag == 1)
		return lily_send_printf(c, "%s:(/me %s)", room, message);
	else {
		lily_chat_t	*lily_chat;

		if (((lily_chat = lily_chat_find_name(c, room)) != NULL) && (lily_chat->isemote != 0))
			return(lily_send_printf(c, "%s:%s", room, message));
		else
			return(lily_send_printf(c, "%s:/me %s", room, message));
	}
}

static fte_t lily_chat_invite(lily_conn_t *c, const char *const room, const char *const who, const char *const message) {
	return lily_send_printf(c, "%s:Join me in %s", who, room);
}

static fte_t lily_subcode_send_request(lily_conn_t *c, const char *const to, const char *const command, const char *const args) {
#ifdef ENABLE_FT_LILY_CTCPMAGIC
	const lily_user_t
		*lily_user = lily_user_find_name(c, to);

	if ((lily_user == NULL) || (lily_user->ctcpmagic == 0))
		return(FE_USERUNAVAILABLE);
#endif

	if (args == NULL) {
		if (lily_send_printf(c, "%s:CTCP REQ %s", to, command) != 0) {
			lily_internal_disconnect(c, FE_PACKET);
			return(FE_PACKET);
		}
	} else {
		if (strcmp(command, "LC") == 0) {
			if (lily_send_printf(c, "#$# ping echo !LC! %s", args) != 0) {
				lily_internal_disconnect(c, FE_PACKET);
				return(FE_PACKET);
			}
		} else {
			if (lily_send_printf(c, "%s:CTCP REQ %s %s", to, command, args) != 0) {
				lily_internal_disconnect(c, FE_PACKET);
				return(FE_PACKET);
			}
		}
	}

	return(FE_SUCCESS);
}

static fte_t lily_subcode_send_reply(lily_conn_t *c, const char *const to, const char *const command, const char *const args) {
	if (to == NULL)
		return(FE_SUCCESS);

#ifdef DEBUG_ECHO
	lily_echof(c, "subcode_send_reply", "c=%#p, to=%#p \"%s\", command=%#p \"%s\", args=%#p \"%s\"", c, to, to, command, 
		command, args, args);
#endif

#ifdef ENABLE_FT_LILY_CTCPMAGIC
	{
		lily_user_t
			*lily_user = lily_user_find_name(c, to);

		if (lily_user != NULL)
			lily_user->ctcpmagic = 1;
	}
#endif

	if (args == NULL) {
		if (lily_send_printf(c, "%s:CTCP REP %s", to, command) != 0) {
			lily_internal_disconnect(c, FE_PACKET);
			return(FE_PACKET);
		}
	} else {
		if (*to != ':')
			if (lily_send_printf(c, "%s:CTCP REP %s %s",to, command, args) != 0) {
				lily_internal_disconnect(c, FE_PACKET);
				return(FE_PACKET);
			}
	}

	return(FE_SUCCESS);
}

static fte_t lily_im_send_message(lily_conn_t *c, const char *const dest, const char *const message, const int auto_flag) {
	if (*message == 0) {
		struct s_firetalk_handle *fchandle = firetalk_find_handle(c);
		char	*data;

		if (auto_flag)
			data = firetalk_dequeue(&(fchandle->subcode_replies), dest);
		else
			data = firetalk_dequeue(&(fchandle->subcode_requests), dest);

		if (data != NULL) {
			fte_t	ret;
			char	*arg;

			if ((arg = strchr(data, ' ')) != NULL)
				*arg++ = 0;

			if (auto_flag)
				ret = lily_subcode_send_reply(c, dest, data, arg);
			else
				ret = lily_subcode_send_request(c, dest, data, arg);
			free(data);
			return(ret);
		}
	}

	if (auto_flag == 1)
		return lily_send_printf(c, "%s:(%s)", dest, message);
	else if (strcasecmp(dest, ":RAW") == 0)
		return lily_send_printf(c, "%s", message);
	else
		return lily_send_printf(c, "%s:%s", dest, message);
}

static fte_t lily_im_send_action(lily_conn_t *c, const char *const dest, const char *const message, const int auto_flag) {
	if (auto_flag == 1)
		return lily_send_printf(c, "%s:(/me %s)", dest, message);
	else
		return lily_send_printf(c, "%s:/me %s", dest, message);
}

static fte_t lily_chat_set_topic(lily_conn_t *c, const char *const room, const char *const topic) {
	return lily_send_printf(c, "/RETITLE %s %s", room, topic);
}

static fte_t lily_chat_op(lily_conn_t *c, const char *const room, const char *const who) {
	return FE_SUCCESS;
}

static fte_t lily_chat_deop(lily_conn_t *c, const char *const room, const char *const who) {
	return FE_SUCCESS;
}

static fte_t lily_chat_kick(lily_conn_t *c, const char *const room, const char *const who, const char *const reason) {
	return FE_SUCCESS;
}

static fte_t lily_im_add_buddy(lily_conn_t *c, const char *const nickname, const char *const group, const char *const friendly) {
	const lily_user_t
		*lily_user = lily_user_find_name(c, nickname);

	if (lily_user != NULL) {
		switch (lily_user->state) {
			case HERE:
				firetalk_callback_im_buddyonline(c, nickname, 1);
				firetalk_callback_user_nickchanged(c, nickname, lily_user->name);
				firetalk_callback_im_buddyaway(c, lily_user->name, 0);
				break;
			case AWAY:
				firetalk_callback_im_buddyonline(c, nickname, 1);
				firetalk_callback_user_nickchanged(c, nickname, lily_user->name);
				firetalk_callback_im_buddyaway(c, lily_user->name, 1);
				break;
			case OFFLINE:
			case UNKNOWN:
				firetalk_callback_im_buddyonline(c, nickname, 0);
				break;
		}
	} else
		firetalk_callback_im_buddyonline(c, nickname, 0);
	return FE_SUCCESS;
}

static fte_t lily_im_remove_buddy(lily_conn_t *c, const char *const nickname, const char *const group) {
	return FE_SUCCESS;
}

static fte_t lily_im_add_deny(lily_conn_t *c, const char *const nickname) {
	return FE_SUCCESS;
}

static fte_t lily_im_remove_deny(lily_conn_t *c, const char *const nickname) {
	return FE_SUCCESS;
}

static fte_t lily_im_upload_buddies(lily_conn_t *c) {
	return FE_SUCCESS;
}

static fte_t lily_im_upload_denies(lily_conn_t *c) {
	return FE_SUCCESS;
}

static fte_t lily_im_evil(lily_conn_t *c, const char *const who) {
	return FE_SUCCESS;
}

static fte_t lily_get_info(lily_conn_t *c, const char *const nickname) {
	if (nickname[0] == '-')
		return(lily_send_printf(c, "/WHAT %s", nickname));
	else
		return(lily_send_printf(c, "/FINGER %s", nickname));
}

static fte_t lily_set_info(lily_conn_t *c, const char *const info) {
#if 0
	if (info != NULL) {
		const char
			*lilyinfo = lily_html_to_lily(info);

		lily_send_printf(c, "/INFO SET");
		lily_send_printf(c, "%s", lilyinfo);
		return(lily_send_printf(c, "SAVE"));
	} else
		return(lily_send_printf(c, "/INFO CLEAR"));
#else
	return(FE_SUCCESS);
#endif
}

static fte_t lily_set_away(lily_conn_t *c, const char *const message, const int auto_flag) {
	if (message) {
		if (auto_flag)
			return lily_send_printf(c, "/AWAY");
		else
			return lily_send_printf(c, "/AWAY %s", message);
	} else
		return lily_send_printf(c, "/HERE off");
}

static fte_t lily_periodic(struct s_firetalk_handle *const c) {
	return(FE_SUCCESS);
}

char	*lily_ctcp_encode(lily_conn_t *c, const char *const command, const char *const args) {
	char	*str;

	if (args != NULL) {
		str = malloc(strlen(command) + 1 + strlen(args) + 1);
		if (str == NULL)
			abort();
		sprintf(str, "%s %s", command, args);
	} else {
		str = strdup(command);
		if (str == NULL)
			abort();
	}

	return(str);
}

const firetalk_protocol_t firetalk_protocol_slcp = {
	strprotocol:		"SLCP",
	default_server:		"slcp.n.ml.org",
	default_port:		7777,
	default_buffersize:	1024*8,
	periodic:		lily_periodic,
	preselect:		lily_preselect,
	postselect:		lily_postselect,
	got_data:		lily_got_data,
	got_data_connecting:	lily_got_data_connecting,
	comparenicks:		lily_compare_nicks,
	isprintable:		lily_isprint,
	disconnect:		lily_disconnect,
	signon:			lily_signon,
	get_info:		lily_get_info,
	set_info:		lily_set_info,
	set_away:		lily_set_away,
	set_nickname:		lily_set_nickname,
	set_password:		lily_set_password,
	im_add_buddy:		lily_im_add_buddy,
	im_remove_buddy:	lily_im_remove_buddy,
	im_add_deny:		lily_im_add_deny,
	im_remove_deny:		lily_im_remove_deny,
	im_upload_buddies:	lily_im_upload_buddies,
	im_upload_denies:	lily_im_upload_denies,
	im_send_message:	lily_im_send_message,
	im_send_action:		lily_im_send_action,
	im_evil:		lily_im_evil,
	chat_join:		lily_chat_join,
	chat_part:		lily_chat_part,
	chat_invite:		lily_chat_invite,
	chat_set_topic:		lily_chat_set_topic,
	chat_op:		lily_chat_op,
	chat_deop:		lily_chat_deop,
	chat_kick:		lily_chat_kick,
	chat_send_message:	lily_chat_send_message,
	chat_send_action:	lily_chat_send_action,
	set_privacy:		lily_set_privacy,
	subcode_encode:		lily_ctcp_encode,
	room_normalize:		lily_normalize_room_name,
	create_handle:		lily_create_handle,
	destroy_handle:		lily_destroy_handle,
};
