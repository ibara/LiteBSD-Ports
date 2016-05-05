/*  _ __   __ _ ___ __  __
** | '_ \ / _` |_ _|  \/  | naim
** | | | | (_| || || |\/| | Copyright 1998-2006 Daniel Reed <n@ml.org>
** |_| |_|\__,_|___|_|  |_| ncurses-based chat client
*/
#include <naim/naim.h>
#include <naim/modutil.h>
#include <pwd.h>
#include <sys/utsname.h>

#include "naim-int.h"
#include "snapshot.h"
#include "conio_cmds.h"

extern win_t	win_input;
extern conn_t	*curconn;
extern time_t	now, awaytime;
extern double	nowf;
extern int	namec;
extern char	**names;
extern namescomplete_t	namescomplete;
extern faimconf_t	faimconf;
extern char	*sty, *statusbar_text;

extern int awayc G_GNUC_INTERNAL;
extern awayar_t *awayar G_GNUC_INTERNAL;
int	awayc = 0;
awayar_t	*awayar = NULL;

#define NAIM_VERSION_STRING	"naim:" PACKAGE_VERSION NAIM_SNAPSHOT
static char naim_version[1024];

#define nFIRE_HANDLER(func) static void func (void *sess, void *client, ...)
#define nFIRE_CTCPHAND(func) static void func (void *sess, void *client, \
	const char *from, const char *command, const char *args)

static void do_replace(unsigned char *dest, const unsigned char *new, int wordlen, int len) {
	int	newlen = strlen(new);

	if (newlen > wordlen)
		memmove(dest+newlen, dest+wordlen, len-newlen);
	else if (newlen < wordlen)
		memmove(dest+newlen, dest+wordlen, len-wordlen);

	memmove(dest, new, newlen);
}

static void str_replace(const unsigned char *orig, const unsigned char *new, unsigned char *str, int strsize) {
	int	i,
		l = strlen(orig);

	assert(*str != 0);

	for (i = 0; (str[i] != 0) && (i+l < strsize); i++) {
		if (i > 0)
			switch (str[i-1]) {
				case ' ':
				case '>':
				case '(':
					break;
				default:
					continue;
			}
		switch (str[i+l]) {
			case ' ':
			case ',':
			case '.':
			case '!':
			case '?':
			case '<':
			case ')':
			case 0:
				break;
			default:
				continue;
		}
		if (strncmp(str+i, orig, l) == 0)
			do_replace(str+i, new, l, strsize-i);
	}
}

html_clean_t *html_cleanar = NULL;
int	html_cleanc = 0;

static const unsigned char *html_clean(const unsigned char *str) {
	static unsigned char
		buf[1024*4];
	int	i;

	assert(str != NULL);
	if (*str == 0)
		return(str);
	strncpy(buf, str, sizeof(buf)-1);
	buf[sizeof(buf)-1] = 0;
	for (i = 0; i < html_cleanc; i++)
		str_replace(html_cleanar[i].from, html_cleanar[i].replace, buf, sizeof(buf));
	return(buf);
}

nFIRE_HANDLER(naim_newnick) {
	conn_t		*conn = (conn_t *)client;
	va_list		msg;
	const char	*newnick;

	va_start(msg, client);
	newnick = va_arg(msg, const char *);
	va_end(msg);

	STRREPLACE(conn->sn, newnick);

	secs_setvar("SN", newnick);
}

nFIRE_HANDLER(naim_nickchange) {
	conn_t		*conn = (conn_t *)client;
	va_list		msg;
	const char	*oldnick, *newnick;
	buddywin_t	*bwin;
	buddylist_t	*buddy = conn->buddyar;

	va_start(msg, client);
	oldnick = va_arg(msg, const char *);
	newnick = va_arg(msg, const char *);
	va_end(msg);

	if (buddy != NULL)
		do {
			if (firetalk_compare_nicks(conn->conn, buddy->_account, oldnick) == FE_SUCCESS) {
				STRREPLACE(buddy->_account, newnick);
				break;
			}
		} while ((buddy = buddy->next) != NULL);

	if ((bwin = bgetwin(conn, oldnick, BUDDY)) != NULL) {
		window_echof(bwin, "<font color=\"#00FFFF\">%s</font> is now known as <font color=\"#00FFFF\">%s</font>.\n",
			oldnick, newnick);
		STRREPLACE(bwin->winname, newnick);
		bupdate();
	}
}

nFIRE_HANDLER(naim_buddylist) {
	conn_t		*conn = (conn_t *)client;
	va_list		msg;
	const char	*screenname, *group, *friendly;
	int		online, away;
	buddylist_t	*blist;

	va_start(msg, client);
	screenname = va_arg(msg, const char *);
	group = va_arg(msg, const char *);
	friendly = va_arg(msg, const char *);
	online = va_arg(msg, int);
	away = va_arg(msg, int);
	va_end(msg);

	if ((blist = rgetlist(conn, screenname)) == NULL)
		blist = raddbuddy(conn, screenname, group, friendly);

	STRREPLACE(blist->_group, group);
	if (friendly != NULL)
		STRREPLACE(blist->_name, friendly);
	else
		FREESTR(blist->_name);
}

#define STANDARD_TRAILER	\
	"&nbsp;<br>"		\
	"Visit <a href=\"http://naim.n.ml.org/\">http://naim.n.ml.org/</a> for the latest naim."

void	naim_set_info(void *sess, const char *str) {
	const char *defar[] = {
"Tepid! Tepid is no good for a star, but it'll do for stardust.<br>\n"
			STANDARD_TRAILER,
"Resisting temptation is easier when you think you'll probably get "
"another chance later on.<br>\n"
			STANDARD_TRAILER,
"A little inaccuracy sometimes saves tons of explanation.<br>\n"
"&nbsp; &nbsp; &nbsp; &nbsp; -- H. H. Munroe<br>\n"
			STANDARD_TRAILER,
"What have you done today to make you feel proud?<br>\n"
			STANDARD_TRAILER,
"<a href=\"http://creativecommons.org/\">Creativity always builds on the past.</a><br>\n"
			STANDARD_TRAILER,
"Those who make peaceful revolution impossible will make violent "
"revolution inevitable.<br>\n"
"&nbsp; &nbsp; &nbsp; &nbsp; -- John F. Kennedy<br>\n"
			STANDARD_TRAILER,
"The question of whether computers can think is just like the question of whether submarines can swim.<br>\n"
"&nbsp; &nbsp; &nbsp; &nbsp; -- Edsger W. Dijkstra<br>\n"
			STANDARD_TRAILER,
"People sleep peaceably in their beds at night only because "
"rough men stand ready to do violence on their behalf.<br>\n"
			STANDARD_TRAILER,
"If you can't fix it, you gotta stand it.<br>\n"
			STANDARD_TRAILER,
"While you don't greatly need the outside world, it's still very "
"reassuring to know that it's still there.<br>\n"
			STANDARD_TRAILER,
	};

	if (str != NULL)
		firetalk_set_info(sess, str);
	else {
		int	d;

		d = rand()%sizeof(defar)/sizeof(*defar);
		firetalk_set_info(sess, defar[d]);
	}
}

nFIRE_HANDLER(naim_postselect) {
	struct timeval	tv;
	char	buf[1024];

	gettimeofday(&tv, NULL);
	now = tv.tv_sec;
	nowf = tv.tv_usec/1000000. + ((double)now);
	snprintf(buf, sizeof(buf), "%lu", now);
	secs_setvar("nowi", buf);
}

void	naim_setversion(conn_t *conn) {
	const char	*where,
			*where2,
			*term,
			*lang;
	struct utsname	unbuf;

	if ((where = getenv("DISPLAY")) != NULL) {
		if ((strncmp(where, ":0", 2) == 0) && (getenv("SSH_CLIENT") == NULL))
			where = " xterm";
		else
			where = " ssh -X";
	} else {
		if (getenv("SSH_CLIENT") != NULL)
			where = " ssh";
		else
			where = "";
	}
	if (sty != NULL)
		where2 = " screen";
	else
		where2 = "";
	if ((term = getenv("TERM")) == NULL)
		term = "(unknown terminal)";
	if ((lang = getenv("LANG")) == NULL)
		lang = "(default language)";

	if (uname(&unbuf) == 0)
		snprintf(naim_version, sizeof(naim_version), "%s:%s %s %s, %ix%i%s%s, %s %s", NAIM_VERSION_STRING,
			unbuf.sysname, unbuf.release, unbuf.machine, COLS, LINES, where, where2, term, lang);
	else
		snprintf(naim_version, sizeof(naim_version), "%s:unknown, %ix%i%s%s, %s %s", NAIM_VERSION_STRING,
			COLS, LINES, where, where2, term, lang);

	firetalk_subcode_register_request_reply(conn->conn, "VERSION", naim_version);
}

nFIRE_HANDLER(naim_doinit) {
	conn_t		*conn = (conn_t *)client;
	va_list		msg;
	buddylist_t	*buddy;
	ignorelist_t	*idiot;
	const char	*screenname;

	va_start(msg, client);
	screenname = va_arg(msg, const char *);
	va_end(msg);

	naim_setversion(conn);

	STRREPLACE(conn->sn, screenname);

	status_echof(conn, "Uploading buddy/block list and profile...\n");

	for (buddy = conn->buddyar; buddy != NULL; buddy = buddy->next) {
		fte_t	ret;

		ret = firetalk_im_add_buddy(sess, USER_ACCOUNT(buddy), USER_GROUP(buddy), buddy->_name);
		assert(ret == FE_SUCCESS);
	}

	for (idiot = conn->idiotar; idiot != NULL; idiot = idiot->next)
		if (strcasecmp(idiot->notes, "block") == 0) {
			fte_t	ret;

			ret = firetalk_im_internal_add_deny(sess, idiot->screenname);
			assert(ret == FE_SUCCESS);
		}

	naim_set_info(sess, conn->profile);

	if (awaytime > 0)
		firetalk_set_away(sess, secs_getvar("awaymsg"), 0);
}

nFIRE_HANDLER(naim_setidle) {
/*	conn_t	*conn = (conn_t *)client; */
	va_list	msg;
	long	*idle, idletime = secs_getvar_int("idletime");

	va_start(msg, client);
	idle = va_arg(msg, long *);
	va_end(msg);

	if ((*idle)/60 != idletime)
		*idle = 60*idletime;
}

nFIRE_HANDLER(naim_warned) {
	conn_t	*conn = (conn_t *)client;
	va_list		msg;
	const char	*who;
	int		newlev;

	va_start(msg, client);
	newlev = va_arg(msg, int);
	who = va_arg(msg, const char *);
	va_end(msg);

	echof(conn, NULL, "<font color=\"#00FFFF\">%s</font> just warned you (%i).\n",
		who, newlev);
	conn->warnval = newlev;
}

nFIRE_HANDLER(naim_buddy_idle) {
	conn_t		*conn = (conn_t *)client;
	va_list		msg;
	const char	*who;
	long		idletime;

	va_start(msg, client);
	who = va_arg(msg, const char *);
	idletime = va_arg(msg, long);
	va_end(msg);

	if (idletime >= 10)
		bidle(conn, who, 1);
	else
		bidle(conn, who, 0);
}

nFIRE_HANDLER(naim_buddy_status) {
	conn_t		*conn = (conn_t *)client;
	va_list		msg;
	const char	*who, *message;
	buddywin_t	*bwin;

	va_start(msg, client);
	who = va_arg(msg, const char *);
	message = va_arg(msg, const char *);
	va_end(msg);

	if ((bwin = bgetwin(conn, who, BUDDY)) != NULL) {
		if (*message == 0)
			FREESTR(bwin->status);
		else {
			if (!bwin->e.buddy->isaway && ((bwin->status == NULL) || (strcmp(bwin->status, message) != 0)))
				window_echof(bwin, "<font color=\"#00FFFF\">%s</font> is available: %s\n",
					user_name(NULL, 0, conn, bwin->e.buddy), message);
			STRREPLACE(bwin->status, message);
		}
	}
}

nFIRE_HANDLER(naim_buddy_eviled) {
	conn_t		*conn = (conn_t *)client;
	va_list		msg;
	const char	*who;
	long		warnval;
	buddylist_t	*blist;

	va_start(msg, client);
	who = va_arg(msg, const char *);
	warnval = va_arg(msg, long);
	va_end(msg);

	if ((blist = rgetlist(conn, who)) != NULL)
		blist->warnval = warnval;
}

nFIRE_HANDLER(naim_buddy_caps) {
	conn_t		*conn = (conn_t *)client;
	va_list		msg;
	const char	*who, *caps;
	buddylist_t	*blist;

	va_start(msg, client);
	who = va_arg(msg, const char *);
	caps = va_arg(msg, const char *);
	va_end(msg);

	if ((blist = rgetlist(conn, who)) != NULL) {
		int	i, j, strtolower = 1;

		blist->caps = realloc(blist->caps, 2*strlen(caps)+1);

		for (i = 0; (caps[i] != 0) && (caps[i] != ' '); i++)
			if (islower(caps[i])) {
				strtolower = 0;
				break;
			}

		for (j = i = 0; caps[i] != 0; i++)
			if (caps[i] == ' ') {
				int	x;

				strtolower = 1;
				for (x = i+1; (caps[x] != 0) && (caps[x] != ' '); x++)
					if (islower(caps[x])) {
						strtolower = 0;
						break;
					}
				blist->caps[j++] = ',';
				blist->caps[j++] = ' ';
			} else if (caps[i] == '_')
				blist->caps[j++] = ' ';
			else if (strtolower && (j > 0) && (blist->caps[j-1] != ' '))
				blist->caps[j++] = tolower(caps[i]);
			else
				blist->caps[j++] = caps[i];
		blist->caps[j] = 0;
	}
}

nFIRE_HANDLER(naim_buddy_typing) {
	conn_t		*conn = (conn_t *)client;
	va_list		msg;
	const char	*who;
	int		typing;
	buddylist_t	*blist;

	va_start(msg, client);
	who = va_arg(msg, const char *);
	typing = va_arg(msg, int);
	va_end(msg);

	if ((blist = rgetlist(conn, who)) != NULL)
		blist->typing = typing;
}

nFIRE_HANDLER(naim_buddy_flags) {
	conn_t		*conn = (conn_t *)client;
	va_list		msg;
	const char	*buddy;
	int		flags, isadmin, ismobile;
	buddywin_t	*bwin;
	buddylist_t	*blist;

	va_start(msg, client);
	buddy = va_arg(msg, const char *);
	flags = va_arg(msg, int);
	va_end(msg);

	isadmin = (flags & FF_ADMIN)?1:0;
	ismobile = (flags & FF_MOBILE)?1:0;

	bwin = bgetwin(conn, buddy, BUDDY);
	if (bwin == NULL)
		blist = rgetlist(conn, buddy);
	else
		blist = bwin->e.buddy;
	assert(blist != NULL);

	if (bwin != NULL) {
		if (isadmin != blist->isadmin) {
			if (isadmin)
				window_echof(bwin, "<font color=\"#00FFFF\">%s</font> is now an administrator.\n",
                                        user_name(NULL, 0, conn, blist));
			else
				window_echof(bwin, "<font color=\"#00FFFF\">%s</font> is no longer an administrator.\n",
                                        user_name(NULL, 0, conn, blist));
		}

		if (ismobile != blist->ismobile) {
			if (ismobile)
				window_echof(bwin, "<font color=\"#00FFFF\">%s</font> is now mobile.\n",
                                        user_name(NULL, 0, conn, blist));
			else
				window_echof(bwin, "<font color=\"#00FFFF\">%s</font> is no longer mobile.\n",
                                        user_name(NULL, 0, conn, blist));
		}
	}

	blist->isadmin = isadmin;
	blist->ismobile = ismobile;
}

nFIRE_HANDLER(naim_buddy_away) {
	conn_t		*conn = (conn_t *)client;
	va_list		msg;
	const char	*who;

	va_start(msg, client);
	who = va_arg(msg, const char *);
	va_end(msg);

	baway(conn, who, 1);
}

nFIRE_HANDLER(naim_buddy_unaway) {
	conn_t		*conn = (conn_t *)client;
	va_list		msg;
	const char	*who;

	va_start(msg, client);
	who = va_arg(msg, const char *);
	va_end(msg);

	baway(conn, who, 0);
}

nFIRE_HANDLER(naim_buddyadded) {
	conn_t		*conn = (conn_t *)client;
	va_list		msg;
	const char	*screenname, *group, *friendly;
	buddylist_t	*blist;

	va_start(msg, client);
	screenname = va_arg(msg, const char *);
	group = va_arg(msg, const char *);
	friendly = va_arg(msg, const char *);
	va_end(msg);

	if ((blist = rgetlist(conn, screenname)) == NULL) {
		if (conn->online > 0)
			status_echof(conn, "User <font color=\"#00FFFF\">%s</font> was added to your buddy list by another client signed on as you.\n",
				screenname);
		blist = raddbuddy(conn, screenname, group, friendly);
	}

	STRREPLACE(blist->_group, group);
	if (friendly != NULL)
		STRREPLACE(blist->_name, friendly);
	else
		FREESTR(blist->_name);
}

nFIRE_HANDLER(naim_buddyremoved) {
	conn_t		*conn = (conn_t *)client;
	va_list		msg;
	const char	*screenname;
	buddywin_t	*bwin;
	buddylist_t	*blist;

	va_start(msg, client);
	screenname = va_arg(msg, const char *);
	va_end(msg);

	if ((bwin = bgetwin(conn, screenname, BUDDY)) != NULL) {
		status_echof(conn, "Closed window <font color=\"#00FFFF\">%s</font> due to buddy removal.\n",
			bwin->winname);
		bclose(conn, bwin, 0);
	}

	if ((blist = rgetlist(conn, screenname)) != NULL) {
		if (conn->online > 0)
			status_echof(conn, "User <font color=\"#00FFFF\">%s</font> was removed from your buddy list by another client signed on as you.\n",
				screenname);
		rdelbuddy(conn, screenname);
	}
}

HOOK_DECLARE(proto_user_onlineval);

nFIRE_HANDLER(naim_buddy_coming) {
	conn_t		*conn = (conn_t *)client;
	va_list		msg;
	const char	*who;

	va_start(msg, client);
	who = va_arg(msg, const char *);
	va_end(msg);

	bcoming(conn, who);
	HOOK_CALL(proto_user_onlineval, (client, conn, who, NULL, NULL, 1));
}

nFIRE_HANDLER(naim_buddy_going) {
	conn_t		*conn = (conn_t *)client;
	va_list		msg;
	const char	*who;

	va_start(msg, client);
	who = va_arg(msg, const char *);
	va_end(msg);

	bgoing(conn, who);
	HOOK_CALL(proto_user_onlineval, (client, conn, who, NULL, NULL, 0));
}



HOOK_DECLARE(recvfrom);
static void naim_recvfrom(conn_t *const conn,
		const char *const _name, 
		const char *const _dest,
		const unsigned char *_message, int len,
		int flags) {
 	char	*name = NULL, *dest = NULL;
	unsigned char *message = malloc(len+1);

	if (_name != NULL)
		name = strdup(_name);
	if (_dest != NULL)
		dest = strdup(_dest);

	memmove(message, _message, len);
	message[len] = 0;
	HOOK_CALL(recvfrom, (conn, &name, &dest, &message, &len, &flags));
	free(name);
	free(dest);
	free(message);
}

static int recvfrom_ignorelist(conn_t *conn, char **name, char **dest, 
		unsigned char **message, int *len, int *flags) {
	ignorelist_t	*ig;

	if ((*dest != NULL) && (strcmp(*dest, ":RAW") == 0) && (getvar_int(conn, "showraw") == 0))
		return(HOOK_STOP);

	for (ig = conn->idiotar; ig != NULL; ig = ig->next)
		if (firetalk_compare_nicks(conn->conn, *name, ig->screenname) == FE_SUCCESS) {
			if (*dest == NULL)
				firetalk_im_evil(conn->conn, *name);
			return(HOOK_STOP);
		}
	return(HOOK_CONTINUE);
}

static int recvfrom_decrypt(conn_t *conn, char **name, char **dest,
		unsigned char **message, int *len, int *flags) {
	if ((*dest == NULL) && !(*flags & RF_ACTION)) {
		buddylist_t	*blist = rgetlist(conn, *name);

		// Note that docrypt is not checked here; we decrypt if we have a key. docrypt only applies to outgoing messages.
		if ((blist != NULL) && (blist->crypt != NULL) && ((blist->peer <= 3) || (*flags & RF_ENCRYPTED))) {
			int	i, j = 0;

			for (i = 0; i < *len; i++) {
				(*message)[i] = (*message)[i] ^ (unsigned char)blist->crypt[j++];
				if (blist->crypt[j] == 0)
					j = 0;
			}
			if ((*message)[i] != 0) {
				echof(conn, "recvfrom_decrypt", "Invalid message: len=%i, i=%i, msg[i]=%i\n",
					*len, i, (*message)[i]);
				return(HOOK_STOP);
			}
			if (!blist->docrypt) {
				buddywin_t *bwin = bgetwin(conn, *name, BUDDY);

				blist->docrypt = 1;
				if (bwin != NULL)
					window_echof(bwin, "%s just sent an encrypted message, but encryption has been temporarily disabled. Reenabling.\n",
						*name);
				else
					status_echof(conn, "%s just sent an encrypted message, but encryption has been temporarily disabled. Reenabling.\n",
						*name);
			}
		}

		// On the other hand, unencrypted messages will cause us to not send encrypted messages either.
		if ((blist != NULL) && blist->docrypt && !(*flags & (RF_AUTOMATIC|RF_ENCRYPTED))) {
			buddywin_t *bwin = bgetwin(conn, *name, BUDDY);

			blist->docrypt = 0;
			if (bwin != NULL)
				window_echof(bwin, "%s just sent an unencrypted message, but encryption has been negotiated. Temporarily disabling encryption.\n",
					*name);
			else
				status_echof(conn, "%s just sent an unencrypted message, but encryption has been negotiated. Temporarily disabling encryption.\n",
					*name);
		}
	}
	return(HOOK_CONTINUE);
}

static int recvfrom_log(conn_t *conn, char **name, char **dest,
		unsigned char **message, int *len, int *flags) {
	if (!(*flags & RF_NOLOG))
		logim(conn, *name, *dest, *message);
	return(HOOK_CONTINUE);
}

static int recvfrom_beep(conn_t *conn, char **name, char **dest,
		unsigned char **message, int *len, int *flags) {
	if (*dest == NULL) {
		int	beeponim = getvar_int(conn, "beeponim");

		if ((beeponim > 1) || ((awaytime == 0) && (beeponim == 1)))
			beep();
	}
	return(HOOK_CONTINUE);
}

static int recvfrom_autobuddy(conn_t *conn, char **name, char **dest,
		unsigned char **message, int *len, int *flags) {
	if (*dest == NULL) {
		buddylist_t	*blist = rgetlist(conn, *name);

		if ((getvar_int(conn, "autobuddy") == 1)
			&& (bgetwin(conn, *name, BUDDY) == NULL)) {
			if (blist == NULL) {
				status_echof(conn, "Adding <font color=\"#00FFFF\">%s</font> to your buddy list due to autobuddy.\n", *name);
				blist = raddbuddy(conn, *name, DEFAULT_GROUP, NULL);
				assert(blist->offline == 1);
				bnewwin(conn, *name, BUDDY);
				firetalk_im_add_buddy(conn->conn, *name, USER_GROUP(blist), NULL);
			} else {
				buddywin_t	*bwin;

				bnewwin(conn, *name, BUDDY);
				bwin = bgetwin(conn, *name, BUDDY);
				window_echof(bwin, "<font color=\"#00FFFF\">%s</font> <font color=\"#800000\">[<B>%s</B>]</font> is still online...\n",
					user_name(NULL, 0, conn, blist), USER_GROUP(blist));
			}
		}
	}
	return(HOOK_CONTINUE);
}

static int recvfrom_display_user(conn_t *conn, char **name, char **dest,
		unsigned char **message, int *len, int *flags) {
	buddywin_t	*bwin;

	if (*dest != NULL)
		return(HOOK_CONTINUE);

	bwin = bgetwin(conn, *name, BUDDY);

	if (bwin == NULL) {
		const char *format;

		naim_lastupdate(conn);

		if (*flags & RF_ACTION) {
			if (*flags & RF_ENCRYPTED)
				format = "<B>*&gt; %s</B>";
			else
				format = "*&gt; <B>%s</B>";
		} else if (*flags & RF_AUTOMATIC) {
			if (*flags & RF_ENCRYPTED)
				format = "<B>-%s-</B>";
			else
				format = "-<B>%s</B>-";
		} else {
			if (*flags & RF_ENCRYPTED)
				format = "<B>[%s]</B>";
			else
				format = "[<B>%s</B>]";
		}
		WINTIME(&(conn->nwin), CONN);
		/* Use name here instead of USER_NAME(blist) even if blist is
		** not NULL, since these messages are going to the status
		** window, and we want them to remain unambiguous.
		*/
		hwprintf(&(conn->nwin), C(CONN,BUDDY), format, *name);
		hwprintf(&(conn->nwin), C(CONN,TEXT), "%s<body>%s</body><br>", (strncmp(*message, "'s ", 3) == 0)?"":" ", *message);
	} else {
		const char *format;

		assert(bwin->et == BUDDY);

		if (getvar_int(conn, "chatter") & IM_MESSAGE)
			bwin->waiting = 1;
		bwin->keepafterso = 1;

		if (*flags & RF_ACTION) {
			if (*flags & RF_ENCRYPTED)
				format = "<B>* %s</B>";
			else
				format = "* <B>%s</B>";
		} else if (*flags & RF_AUTOMATIC) {
			if (*flags & RF_ENCRYPTED)
				format = "<B>-%s-</B>";
			else
				format = "-<B>%s</B>-";
		} else {
			if (*flags & RF_ENCRYPTED)
				format = "<B>%s:</B>";
			else
				format = "<B>%s</B>:";
		}

		WINTIME(&(bwin->nwin), IMWIN);
		hwprintf(&(bwin->nwin), C(IMWIN,BUDDY), format, USER_NAME(bwin->e.buddy));
		hwprintf(&(bwin->nwin), C(IMWIN,TEXT), "%s<body>%s</body><br>", (strncmp(*message, "'s ", 3) == 0)?"":" ", *message);

		if (!(*flags & RF_AUTOMATIC)) {
			int	autoreply = getvar_int(conn, "autoreply");

			if ((autoreply > 0) && (awaytime > 0) && (*secs_getvar("awaymsg") != 0)
				&& ((now - bwin->informed) > 60*autoreply)
				&& (firetalk_compare_nicks(conn->conn, *name, conn->sn) != FE_SUCCESS)) {
				int	autoaway = secs_getvar_int("autoaway"),
					idletime = secs_getvar_int("idletime");

				if ((autoaway == 0) || (idletime >= (now-awaytime)/60) || (idletime >= 10)) {
					sendaway(conn, *name);
					bwin->informed = now;
				}
			}
		}
	}
	bupdate();
	return(HOOK_CONTINUE);
}

static void recvfrom_display_chat_print(buddywin_t *bwin, const int flags, const int istome, const char *name, const char *prefix, const unsigned char *message) {
	const char	*format;

	if (prefix == NULL)
		prefix = "&gt;";

	if (flags & RF_ACTION)
		format = "* <B>%s</B>";
	else if (flags & RF_AUTOMATIC)
		format = "-<B>%s</B>-";
	else
		format = "&lt;<B>%s</B>%s";
	hwprintf(&(bwin->nwin), istome?C(IMWIN,BUDDY_ADDRESSED):C(IMWIN,BUDDY), 
		format, name, prefix);

	hwprintf(&(bwin->nwin), C(IMWIN,TEXT), " <body>%s%s%s</body><br>",
		istome?"<B>":"", message, istome?"</B>":"");
}

void	chat_flush(buddywin_t *bwin) {
	assert(bwin->et == CHAT);
	assert(bwin->e.chat->last.reps >= 0);
	if (bwin->e.chat->last.reps > 0) {
		if (bwin->e.chat->last.reps == 1) {
			assert(bwin->e.chat->last.lasttime != 0);
			WINTIME_NOTNOW(&(bwin->nwin), IMWIN, bwin->e.chat->last.lasttime);
			recvfrom_display_chat_print(bwin, bwin->e.chat->last.flags, bwin->e.chat->last.istome, bwin->e.chat->last.name, NULL, bwin->e.chat->last.line);
		} else {
			assert(bwin->e.chat->last.lasttime != 0);
			WINTIME_NOTNOW(&(bwin->nwin), IMWIN, bwin->e.chat->last.lasttime);
			hwprintf(&(bwin->nwin), C(IMWIN,TEXT), "<B>[Last message repeated %i more times]</B><br>", bwin->e.chat->last.reps);
		}
		bwin->e.chat->last.reps = 0;
	}
	FREESTR(bwin->e.chat->last.line);
	FREESTR(bwin->e.chat->last.name);
}

static int recvfrom_display_chat(conn_t *conn, char **name, char **dest,
		unsigned char **message, int *len, int *flags) {
	buddywin_t	*bwin;
	int		istome;
	char		*prefix = NULL;
	unsigned char	*message_save;

	if (*dest == NULL)
		return(HOOK_CONTINUE);

	{
		unsigned char *match;

		if (conn->sn == NULL)
			istome = 0;
		else if ((aimncmp(*message, conn->sn, strlen(conn->sn)) == 0) && !isalpha(*(*message+strlen(conn->sn))))
			istome = 1;
		else if (((match = strstr(*message, conn->sn)) != NULL)
			&& ((match == *message) || !isalpha(*(match-1)))
			&& !isalpha(*(match+strlen(conn->sn))))
			istome = 1;
		else
			istome = 0;
	}

	bwin = cgetwin(conn, *dest);
	assert(bwin->et == CHAT);
	if (getvar_int(conn, "chatter") & CH_MESSAGE) {
		bwin->waiting = 1;
		if (istome)
			bwin->e.chat->isaddressed = 1;
	}

	if (bwin->e.chat->last.line != NULL)
		assert(bwin->e.chat->last.name != NULL);
	if ((bwin->e.chat->last.line != NULL) && (*flags == bwin->e.chat->last.flags)
		&& (firetalk_compare_nicks(conn->conn, *name, bwin->e.chat->last.name) == FE_SUCCESS)
		&& (strcasecmp(*message, bwin->e.chat->last.line) == 0)) {
		bwin->e.chat->last.reps++;
		bwin->e.chat->last.lasttime = now;
		return(HOOK_CONTINUE);
	}
	if (bwin->e.chat->last.reps > 0)
		chat_flush(bwin);
	assert(bwin->e.chat->last.reps == 0);

	message_save = strdup(*message);

	if ((bwin->winname[0] != ':') && (bwin->e.chat->last.line != NULL)) {
		char	*tmp = strdup(*message);
		size_t	off, add;

		htmlreplace(tmp, '_');
		off = strspn(tmp, "_");
		add = strcspn(tmp+off, " ");
		assert(bwin->e.chat->last.name != NULL);
		if ((add > off) && isalpha(tmp[off]) && !isalpha(tmp[off+add-1])) {
			if (strncasecmp(tmp+off, bwin->e.chat->last.name, strlen(bwin->e.chat->last.name)) == 0) {
				static int	sent_carat_desc = 0;

				if (!sent_carat_desc) {
					statusbar_text = strdup("A ^ near a speaker name indicates that message was addressed to the previous speaker.");
					sent_carat_desc = 1;
				}
				prefix = "^";
				memmove((*message)+off, (*message)+off+add, strlen((*message)+off+add)+1);
			} else if (strncasecmp(*message, bwin->e.chat->last.line, off+add) == 0) {
				static int	sent_plus_desc = 0;

				if (!sent_plus_desc) {
					statusbar_text = strdup("A + near a speaker name indicates that message was addressed to the same person as the previous message.");
					sent_plus_desc = 1;
				}
				if (firetalk_compare_nicks(conn->conn, *name, bwin->e.chat->last.name) != FE_SUCCESS)
					prefix = "+";
				memmove((*message)+off, (*message)+off+add, strlen((*message)+off+add)+1);
			}
		}
		free(tmp);
	}

	WINTIME(&(bwin->nwin), IMWIN);
	recvfrom_display_chat_print(bwin, *flags, istome, *name, prefix, *message);

	free(bwin->e.chat->last.line);
	bwin->e.chat->last.line = message_save;
	STRREPLACE(bwin->e.chat->last.name, (*name));
	bwin->e.chat->last.reps = 0;
	bwin->e.chat->last.lasttime = now;
	bwin->e.chat->last.flags = *flags;
	bwin->e.chat->last.istome = istome;

	bupdate();
	return(HOOK_CONTINUE);
}

void	fireio_hook_init(void) {
	void	*mod = NULL;

	HOOK_ADD(recvfrom, mod, recvfrom_ignorelist, 10);
	HOOK_ADD(recvfrom, mod, recvfrom_decrypt, 20);
	HOOK_ADD(recvfrom, mod, recvfrom_log, 50);
	HOOK_ADD(recvfrom, mod, recvfrom_beep, 50);
	HOOK_ADD(recvfrom, mod, recvfrom_autobuddy, 50);
	HOOK_ADD(recvfrom, mod, recvfrom_display_user, 100);
	HOOK_ADD(recvfrom, mod, recvfrom_display_chat, 150);
}




nFIRE_HANDLER(naim_im_handle) {
	conn_t		*conn = (conn_t *)client;
	va_list		msg;
	const char	*name;
	const unsigned char
			*message;
	int		isautoreply;

	va_start(msg, client);
	name = va_arg(msg, const char *);
	isautoreply = va_arg(msg, int);
	message = html_clean(va_arg(msg, const unsigned char *));
	va_end(msg);

	assert(message != NULL);

	naim_recvfrom(conn, name, NULL, message, strlen(message),
		isautoreply?RF_AUTOMATIC:RF_NONE);
}

nFIRE_HANDLER(naim_act_handle) {
	conn_t		*conn = (conn_t *)client;
	va_list		msg;
	const char	*who;
	const unsigned char
			*message;
	int		isautoreply;

	va_start(msg, client);
	who = va_arg(msg, const char *);
	isautoreply = va_arg(msg, int);
	message = va_arg(msg, const unsigned char *);
	va_end(msg);

	if (message == NULL)
		message = "";

	naim_recvfrom(conn, who, NULL, message, strlen(message),
		isautoreply?RF_AUTOMATIC:RF_NONE | RF_ACTION);
}

nFIRE_HANDLER(naim_chat_getmessage) {
	conn_t		*conn = (conn_t *)client;
	va_list		msg;
	const char	*room, *who;
	const unsigned char
			*message;
	int		isautoreply;

	va_start(msg, client);
	room = va_arg(msg, const char *);
	who = va_arg(msg, const char *);
	isautoreply = va_arg(msg, int);
	message = html_clean(va_arg(msg, const unsigned char *));
	va_end(msg);

	if (firetalk_compare_nicks(conn->conn, who, conn->sn) == FE_SUCCESS)
		return;

	assert(message != NULL);

	naim_recvfrom(conn, who, room, message, strlen(message),
		isautoreply?RF_AUTOMATIC:RF_NONE);
}

nFIRE_HANDLER(naim_chat_act_handle) {
	conn_t		*conn = (conn_t *)client;
	va_list		msg;
	const char	*room, *who;
	const unsigned char
			*message;
	int		isautoreply;

	va_start(msg, client);
	room = va_arg(msg, const char *);
	who = va_arg(msg, const char *);
	isautoreply = va_arg(msg, int);
	message = va_arg(msg, const unsigned char *);
	va_end(msg);

	if (firetalk_compare_nicks(conn->conn, who, conn->sn) == FE_SUCCESS)
		return;

	if (message == NULL)
		message = "";

	naim_recvfrom(conn, who, room, message, strlen(message),
		isautoreply?RF_AUTOMATIC:RF_NONE | RF_ACTION);
}

void	naim_awaylog(conn_t *conn, const char *src, const char *msg) {
	naim_recvfrom(conn, src, ":AWAYLOG", msg, strlen(msg), RF_NOLOG);
}

nFIRE_HANDLER(naim_connected) {
	conn_t		*conn = (conn_t *)client;
	buddywin_t	*bwin = conn->curbwin;

	if (conn->online > 0) {
		status_echof(conn, "naim just received notification that you have connected to %s at %lu,"
			" but I'm pretty sure you've been connected since %lu."
			" This is a bug, and your session may be unstable.\n",
			conn->winname, now, conn->online);
		return;
	}

	echof(conn, NULL, "You are now connected.\n");

	conn->online = now;
	if (bwin != NULL)
		do {
			if ((bwin->et == CHAT) && (*(bwin->winname) != ':')) {
				char	buf[1024], *name = buf;

				if (bwin->e.chat->key != NULL)
					snprintf(buf, sizeof(buf), "%s %s", bwin->winname, bwin->e.chat->key);
				else
					name = bwin->winname;
				firetalk_chat_join(conn->conn, name);
			}
		} while ((bwin = bwin->next) != conn->curbwin);

	firetalk_im_list_buddies(conn->conn);
}

static const char naim_tolower_first(const char *const str) {
	if ((*str != 0) && !isupper(str[1]))
		return(tolower(*str));
	return(*str);
}

nFIRE_HANDLER(naim_connectfailed) {
	conn_t		*conn = (conn_t *)client;
	int		err;
	va_list		msg;
	const char	*reason;

	va_start(msg, client);
	err = va_arg(msg, int);
	reason = va_arg(msg, const char *);
	va_end(msg);

	if (reason != NULL)
		echof(conn, "CONNECT", "Unable to connect to %s: %s, %c%s.\n",
			firetalk_strprotocol(conn->proto),
			firetalk_strerror(err), naim_tolower_first(reason), reason+1);
	else
		echof(conn, "CONNECT", "Unable to connect to %s: %s.\n",
			firetalk_strprotocol(conn->proto),
			firetalk_strerror(err));

	if (err == FE_BADUSER) {
		char	*str;

		echof(conn, NULL, "Attempting to reconnect using a different name...\n");
		str = malloc(strlen(conn->sn)+2);
		strcpy(str, conn->sn);
		if (strlen(conn->sn) > 8) {
			int	pos = rand()%(strlen(conn->sn)-1)+1;

			if (str[pos] == '_')
				str[pos] = rand()%9+'1';
			else
				str[pos] = '_';
		} else
			strcat(str, "_");
		free(conn->sn);
		conn->sn = str;
		conio_connect(conn, 0, NULL);
	}
}

nFIRE_HANDLER(naim_error_msg) {
	conn_t		*conn = (conn_t *)client;
	va_list		msg;
	int		error;
	const char	*target, *desc;
	buddywin_t	*bwin;

	va_start(msg, client);
	error = va_arg(msg, int);
	target = va_arg(msg, const char *);
	desc = va_arg(msg, const char *);
	va_end(msg);

	if ((error == FE_MESSAGETRUNCATED) && (awaytime > 0))
		return;

	if ((error == FE_INVALIDFORMAT) && (target != NULL) && (awayc > 0)) {
		int	i;

		assert(awayar != NULL);

		for (i = 0; i < awayc; i++)
			if (firetalk_compare_nicks(conn->conn, target, awayar[i].name) == FE_SUCCESS) {
				if (awayar[i].gotaway == 0) {
					buddywin_t *bwin = bgetwin(conn, target, BUDDY);

					if (bwin == NULL)
						status_echof(conn, "<font color=\"#00FFFF\">%s</font> is now away.\n",
							target);
					else
						window_echof(bwin, "<font color=\"#00FFFF\">%s</font> is now away.\n",
							user_name(NULL, 0, conn, bwin->e.buddy));
				}
				free(awayar[i].name);
				memmove(awayar+i, awayar+i+1, (awayc-i-1)*sizeof(*awayar));
				awayc--;
				awayar = realloc(awayar, awayc*sizeof(*awayar));
				return;
			}
	}


	if ((target != NULL) && ((bwin = bgetanywin(conn, target)) != NULL)) {
		if (desc != NULL)
			window_echof(bwin, "ERROR: %s, %c%s\n",
				firetalk_strerror(error), naim_tolower_first(desc), desc+1);
		else
			window_echof(bwin, "ERROR: %s\n",
				firetalk_strerror(error));
		bwin->waiting = 1;
		bupdate();
	} else if (target != NULL) {
		if (desc != NULL)
			status_echof(conn, "ERROR: %s: %s, %c%s\n",
				target, firetalk_strerror(error), naim_tolower_first(desc), desc+1);
		else
			status_echof(conn, "ERROR: %s: %s\n",
				target, firetalk_strerror(error));
	} else {
		if (desc != NULL)
			status_echof(conn, "ERROR: %s, %c%s\n",
				firetalk_strerror(error), naim_tolower_first(desc), desc+1);
		else
			status_echof(conn, "ERROR: %s\n",
				firetalk_strerror(error));
	}
}

nFIRE_HANDLER(naim_error_disconnect) {
	conn_t	*conn = (conn_t *)client;
	va_list	msg;
	int	error;

	va_start(msg, client);
	error = va_arg(msg, int);
	va_end(msg);

	echof(conn, NULL, "Disconnected from %s: %s.\n",
		conn->winname, firetalk_strerror(error));
	conn->online = -1;
	bclearall(conn, 0);

	if (error == FE_RECONNECTING)
		echof(conn, NULL, "Please wait...\n");
	else if ((error != FE_USERDISCONNECT) && (getvar_int(conn, "autoreconnect") != 0)) {
		echof(conn, NULL, "Attempting to reconnect...\n");
		conio_connect(conn, 0, NULL);
	} else
		echof(conn, NULL, "Type <font color=\"#00FF00\">/%s:connect</font> to reconnect.\n", conn->winname);
}

nFIRE_HANDLER(naim_needpass) {
	conn_t		*conn = (conn_t *)client;
	va_list		msg;
	char		*pass;
	int		len;
	const char	*mypass;

	va_start(msg, client);
	pass = va_arg(msg, char *);
	len = va_arg(msg, int);
	va_end(msg);

	assert(len > 1);

	if ((mypass = getvar(conn, "password")) == NULL) {
		if (conn != curconn)
			curconn = conn;
		echof(conn, NULL, "Password required to connect to %s.\n",
			conn->winname);
		echof(conn, NULL, "Please type your password and press Enter.\n");
		nw_getpass(&win_input, pass, len);
		nw_erase(&win_input);
		statrefresh();
	} else {
		strncpy(pass, mypass, len-1);
		pass[len-1] = 0;
	}
}

nFIRE_HANDLER(naim_userinfo_handler) {
	conn_t		*conn = (conn_t *)client;
	va_list		msg;
	const char	*SN;
	const unsigned char
			*info;
	long		warning, online, idle, class;

	va_start(msg, client);
	SN = va_arg(msg, const char *);
	info = va_arg(msg, const unsigned char *);
	warning = va_arg(msg, long);
	online = va_arg(msg, long);
	idle = va_arg(msg, long);
	class = va_arg(msg, long);
	va_end(msg);

	if (awayc > 0) {
		int	i;

		assert(awayar != NULL);

		for (i = 0; i < awayc; i++)
			if (firetalk_compare_nicks(conn->conn, SN, awayar[i].name) == FE_SUCCESS) {
				if (awayar[i].gotaway == 0) {
					buddywin_t	*bwin = bgetwin(conn, SN, BUDDY);

					if (bwin == NULL)
						status_echof(conn, "<font color=\"#00FFFF\">%s</font> is now away.\n",
							SN);
					else
						window_echof(bwin, "<font color=\"#00FFFF\">%s</font> is now away.\n",
							user_name(NULL, 0, conn, bwin->e.buddy));
				}
				free(awayar[i].name);
				memmove(awayar+i, awayar+i+1, (awayc-i-1)*sizeof(*awayar));
				awayc--;
				awayar = realloc(awayar, awayc*sizeof(*awayar));
				return;
			}
	}

	echof(conn, NULL, "Information about %s:\n",
		SN);

	if (class & (FF_SUBSTANDARD|FF_NORMAL|FF_ADMIN|FF_MOBILE))
	  echof(conn, NULL,
		"</B>&nbsp; &nbsp; &nbsp; &nbsp; &nbsp; <B>Class</B>:%s%s%s%s",
			(class&FF_SUBSTANDARD)?" AIM":"",
			(class&FF_NORMAL)?" AOLamer":"",
			(class&FF_ADMIN)?" Operator":"",
			(class&FF_MOBILE)?" Mobile":"");
	{
		buddylist_t *blist = rgetlist(conn, SN);

		if ((blist != NULL) && (blist->caps != NULL))
		  echof(conn, NULL,
			"</B><B>Client features</B>: %s", blist->caps);
	}
	if (warning > 0)
	  echof(conn, NULL,
		"</B>&nbsp; <B>Warning level</B>: %i", warning);
	if (online > 0)
	  echof(conn, NULL,
		"</B>&nbsp; &nbsp; <B>Online time</B>: <B>%s</B>",
		dtime(now-online));
	if (idle > 0)
	  echof(conn, NULL,
		"</B>&nbsp; &nbsp; &nbsp; <B>Idle time</B>: <B>%s</B>",
		dtime(60*idle));
	if (info != NULL)
	  echof(conn, NULL,
		"</B>&nbsp; &nbsp; &nbsp; &nbsp; <B>Profile</B>:<br> %s<br> <hr>", info);
	bupdate();
}


buddywin_t *cgetwin(conn_t *conn, const char *roomname) {
	buddywin_t	*bwin;

	if (*roomname != ':')
		roomname = firetalk_chat_normalize(conn->conn, roomname);
	if ((bwin = bgetwin(conn, roomname, CHAT)) == NULL) {
		bnewwin(conn, roomname, CHAT);
		bwin = bgetwin(conn, roomname, CHAT);
		assert(bwin != NULL);
		bwin->keepafterso = 1;
		bupdate();
	} else
		assert(bwin->keepafterso == 1);
	return(bwin);
}

nFIRE_HANDLER(naim_chat_joined) {
	conn_t		*conn = (conn_t *)client;
	va_list		msg;
	buddywin_t	*bwin;
	const char	*room;

	va_start(msg, client);
	room = va_arg(msg, const char *);
	va_end(msg);

	bwin = cgetwin(conn, room);
	bwin->e.chat->offline = 0;
	if (getvar_int(conn, "autonames") == 0)
		window_echof(bwin, "You are now participating in the %s discussion.\n", room);
	else {
		const char	*args[1] = { bwin->winname };

		window_echof(bwin, "You are now participating in the %s discussion. Checking for current participants...\n", room);
		conio_names(conn, 1, args);
	}
	bupdate();
}

nFIRE_HANDLER(naim_chat_left) {
	conn_t		*conn = (conn_t *)client;
	va_list		msg;
	const char	*room;
	buddywin_t	*bwin;

	va_start(msg, client);
	room = va_arg(msg, const char *);
	va_end(msg);

	if ((bwin = bgetwin(conn, room, CHAT)) != NULL) {
		bwin->e.chat->offline = 1;
		firetalk_chat_join(conn->conn, room);
	}
	bupdate();
}

nFIRE_HANDLER(naim_chat_kicked) {
	conn_t		*conn = (conn_t *)client;
	va_list		msg;
	buddywin_t	*bwin;
	const char	*room, *by, *reason, *q;

	va_start(msg, client);
	room = va_arg(msg, const char *);
	by = va_arg(msg, const char *);
	reason = va_arg(msg, const char *);
	va_end(msg);

	q = (strchr(room, ' ') != NULL)?"\"":"";

	bwin = cgetwin(conn, room);
	bwin->e.chat->offline = 1;
	bwin->e.chat->isoper = 0;
	if (getvar_int(conn, "chatter") & CH_ATTACKED)
		bwin->waiting = 1;
	window_echof(bwin, "You have been kicked from chat %s%s%s by <font color=\"#00FFFF\">%s</font> (</B><body>%s</body><B>).\n",
			q, room, q, by, reason);
	firetalk_chat_join(sess, room);
	bupdate();
}

nFIRE_HANDLER(naim_chat_invited) {
	conn_t		*conn = (conn_t *)client;
	va_list		msg;
	const char	*room, *who, *message, *q;

	va_start(msg, client);
	room = va_arg(msg, const char *);
	who = va_arg(msg, const char *);
	message = va_arg(msg, const char *);
	va_end(msg);

	q = (strchr(room, ' ') != NULL)?"\"":"";

	echof(conn, NULL, "<font color=\"#00FFFF\">%s</font> invites you to chat %s%s%s: </B><body>%s</body><B>.\n", who, q, room, q, message);
}

nFIRE_HANDLER(naim_chat_JOIN) {
	conn_t		*conn = (conn_t *)client;
	va_list		msg;
	buddywin_t	*bwin;
	const char	*room, *who, *q, *extra;

	va_start(msg, client);
	room = va_arg(msg, const char *);
	who = va_arg(msg, const char *);
	extra = va_arg(msg, const char *);
	va_end(msg);

	q = (strchr(room, ' ') != NULL)?"\"":"";

	bwin = cgetwin(conn, room);

	if (getvar_int(conn, "chatverbose") & CH_USERS)
		bwin->waiting = 1;

	if (extra == NULL)
		window_echof(bwin, "<font color=\"#00FFFF\">%s</font> has joined chat %s%s%s.\n",
			who, q, room, q);
	else
		window_echof(bwin, "<font color=\"#00FFFF\">%s</font> (%s) has joined chat %s%s%s.\n",
			who, extra, q, room, q);
	bupdate();
}

nFIRE_HANDLER(naim_chat_PART) {
	conn_t		*conn = (conn_t *)client;
	va_list		msg;
	buddywin_t	*bwin;
	const char	*room, *who, *reason, *q;

	va_start(msg, client);
	room = va_arg(msg, const char *);
	who = va_arg(msg, const char *);
	reason = va_arg(msg, const char *);
	va_end(msg);

	q = (strchr(room, ' ') != NULL)?"\"":"";

	bwin = cgetwin(conn, room);
	if (getvar_int(conn, "chatverbose") & CH_USERS)
		bwin->waiting = 1;
	if (reason == NULL)
		window_echof(bwin, "<font color=\"#00FFFF\">%s</font> has left chat %s%s%s.\n",
			who, q, room, q);
	else
		window_echof(bwin, "<font color=\"#00FFFF\">%s</font> has left chat %s%s%s (</B><body>%s</body><B>).\n",
			who, q, room, q, (*reason != 0)?reason:"quit");
	bupdate();
}

nFIRE_HANDLER(naim_chat_KICK) {
	conn_t		*conn = (conn_t *)client;
	va_list		msg;
	buddywin_t	*bwin;
	const char	*room, *who, *by, *reason, *q;

	va_start(msg, client);
	room = va_arg(msg, const char *);
	who = va_arg(msg, const char *);
	by = va_arg(msg, const char *);
	reason = va_arg(msg, const char *);
	va_end(msg);

	q = (strchr(room, ' ') != NULL)?"\"":"";

	bwin = cgetwin(conn, room);
	if (getvar_int(conn, "chatter") & CH_ATTACKS)
		bwin->waiting = 1;
	window_echof(bwin, "<font color=\"#00FFFF\">%s</font> has been kicked off chat %s%s%s by <font color=\"#00FFFF\">%s</font> (</B><body>%s</body><B>).\n",
		who, q, room, q, by, reason);
	bupdate();
}

nFIRE_HANDLER(naim_chat_KEYCHANGED) {
	conn_t		*conn = (conn_t *)client;
	va_list		msg;
	buddywin_t	*bwin;
	const char	*room, *what, *by;

	va_start(msg, client);
	room = va_arg(msg, const char *);
	what = va_arg(msg, const char *);
	by = va_arg(msg, const char *);
	va_end(msg);

	bwin = cgetwin(conn, room);
	if (getvar_int(conn, "chatverbose") & CH_MISC)
		bwin->waiting = 1;

	if (what != NULL) {
		if (bwin->e.chat->key != NULL)
			window_echof(bwin, "<font color=\"#00FFFF\">%s</font> has changed the channel key from %s to %s.\n",
				by, bwin->e.chat->key, what);
		else
			window_echof(bwin, "<font color=\"#00FFFF\">%s</font> has changed the channel key to %s.\n",
				by, what);
		STRREPLACE(bwin->e.chat->key, what);
	} else if (bwin->e.chat->key != NULL) {
		window_echof(bwin, "<font color=\"#00FFFF\">%s</font> has cleared the channel key (was %s).\n",
			by, bwin->e.chat->key);
		FREESTR(bwin->e.chat->key);
	}
	bupdate();
}

#ifdef RAWIRCMODES
nFIRE_HANDLER(naim_chat_MODECHANGED) {
	conn_t		*conn = (conn_t *)client;
	va_list		msg;
	buddywin_t	*bwin;
	const char	*room, *mode, *by;

	va_start(msg, client);
	room = va_arg(msg, const char *);
	mode = va_arg(msg, const char *);
	by = va_arg(msg, const char *);
	va_end(msg);

	bwin = cgetwin(conn, room);
	if (getvar_int(conn, "chatverbose") & CH_MISC)
		bwin->waiting = 1;

	if (mode != NULL)
		window_echof(bwin, "<font color=\"#00FFFF\">%s</font> has set mode <font color=\"#FF00FF\">%s</font>.\n",
			by, mode);
	bupdate();
}
#endif

nFIRE_HANDLER(naim_chat_oped) {
	conn_t		*conn = (conn_t *)client;
	va_list		msg;
	buddywin_t	*bwin;
	const char	*room, *by;

	va_start(msg, client);
	room = va_arg(msg, const char *);
	by = va_arg(msg, const char *);
	va_end(msg);

	bwin = cgetwin(conn, room);
	if (getvar_int(conn, "chatter") & CH_ATTACKED)
		bwin->waiting = 1;
	bwin->e.chat->isoper = 1;
	bupdate();
}

nFIRE_HANDLER(naim_chat_OP) {
	conn_t		*conn = (conn_t *)client;
	va_list		msg;
	buddywin_t	*bwin;
	const char	*room, *who, *by;

	va_start(msg, client);
	room = va_arg(msg, const char *);
	who = va_arg(msg, const char *);
	by = va_arg(msg, const char *);
	va_end(msg);

	bwin = cgetwin(conn, room);
	if (getvar_int(conn, "chatverbose") & CH_MISC)
		bwin->waiting = 1;
#ifndef RAWIRCMODES
	if (by != NULL)
		window_echof(bwin, "<font color=\"#00FFFF\">%s</font> has been oped by <font color=\"#00FFFF\">%s</font>.\n",
			who, by);
#endif
	bupdate();
}

nFIRE_HANDLER(naim_chat_deoped) {
	conn_t		*conn = (conn_t *)client;
	va_list		msg;
	buddywin_t	*bwin;
	const char	*room, *by;

	va_start(msg, client);
	room = va_arg(msg, const char *);
	by = va_arg(msg, const char *);
	va_end(msg);

	bwin = cgetwin(conn, room);
	if (getvar_int(conn, "chatter") & CH_ATTACKED)
		bwin->waiting = 1;
	bwin->e.chat->isoper = 0;
	bupdate();
}

nFIRE_HANDLER(naim_chat_DEOP) {
	conn_t		*conn = (conn_t *)client;
	va_list		msg;
	buddywin_t	*bwin;
	const char	*room, *who, *by;

	va_start(msg, client);
	room = va_arg(msg, const char *);
	who = va_arg(msg, const char *);
	by = va_arg(msg, const char *);
	va_end(msg);

	bwin = cgetwin(conn, room);
	if (getvar_int(conn, "chatter") & CH_ATTACKS)
		bwin->waiting = 1;
	if (firetalk_compare_nicks(conn->conn, conn->sn, who) == FE_SUCCESS)
		bwin->e.chat->isoper = 0;
#ifndef RAWIRCMODES
	window_echof(bwin, "<font color=\"#00FFFF\">%s</font> has been deoped by <font color=\"#00FFFF\">%s</font>.\n",
		who, by);
#endif
	bupdate();
}

nFIRE_HANDLER(naim_chat_TOPIC) {
	conn_t		*conn = (conn_t *)client;
	va_list		msg;
	buddywin_t	*bwin;
	const char	*room, *topic, *by, *q;

	va_start(msg, client);
	room = va_arg(msg, const char *);
	topic = va_arg(msg, const char *);
	by = va_arg(msg, const char *);
	va_end(msg);

	q = (strchr(room, ' ') != NULL)?"\"":"";

	bwin = cgetwin(conn, room);
	if (getvar_int(conn, "chatter") & CH_MISC)
		bwin->waiting = 1;
	STRREPLACE(bwin->blurb, topic);
	if (by != NULL)
		window_echof(bwin, "<font color=\"#00FFFF\">%s</font> has changed the topic on %s%s%s to </B><body>%s</body><B>.\n",
			by, q, room, q, topic);
	else
		window_echof(bwin, "Topic for %s: </B><body>%s</body><B>.\n",
			room, topic);
	bupdate();
}

nFIRE_HANDLER(naim_chat_NICK) {
	conn_t		*conn = (conn_t *)client;
	va_list		msg;
	buddywin_t	*bwin;
	const char	*room, *oldnick, *newnick;

	va_start(msg, client);
	room = va_arg(msg, const char *);
	oldnick = va_arg(msg, const char *);
	newnick = va_arg(msg, const char *);
	va_end(msg);

	bwin = cgetwin(conn, room);
	if (getvar_int(conn, "chatter") & CH_MISC)
		bwin->waiting = 1;
	window_echof(bwin, "<font color=\"#00FFFF\">%s</font> is now known as <font color=\"#00FFFF\">%s</font>.\n",
		oldnick, newnick);
	bupdate();
}

nFIRE_HANDLER(naim_chat_NAMES) {
	va_list		msg;
	const char	*room, *nick;
	int		oped;
//	int		i, j;

	va_start(msg, client);
	room = va_arg(msg, const char *);
	nick = va_arg(msg, const char *);
	oped = va_arg(msg, int);
	va_end(msg);

	if (namescomplete.buf != NULL) {
		assert(namescomplete.len > 0);
		if (namescomplete.foundmatch) {
			if (!namescomplete.foundmult && (strncasecmp(nick, namescomplete.buf, namescomplete.len) == 0))
				namescomplete.foundmult = 1;
			return;
		}
		if (strlen(namescomplete.buf) > namescomplete.len) {
			int	len = strlen(namescomplete.buf);

			assert(len > 0);
			if (namescomplete.buf[len-1] == ' ');
				len--;
			assert(len > 0);
			if (namescomplete.buf[len-1] == ',');
				len--;
			assert(len > 0);
			if (strncmp(namescomplete.buf, nick, len) == 0) {
				namescomplete.foundmult = namescomplete.foundfirst = 1;
				return;
			} else if (!namescomplete.foundfirst) {
				if (!namescomplete.foundmult && (strncasecmp(nick, namescomplete.buf, namescomplete.len) == 0))
					namescomplete.foundmult = 1;
				return;
			}
		}
		if (strncasecmp(nick, namescomplete.buf, namescomplete.len) == 0) {
			free(namescomplete.buf);
			namescomplete.buf = strdup(nick);
			namescomplete.foundmatch = 1;
		}
		return;
	}

	namec++;
	names = realloc(names, namec*sizeof(*names));
	names[namec-1] = malloc(strlen(nick) + oped + 1);
	sprintf(names[namec-1], "%s%s", oped?"@":"", nick);
//	for (i = 0, j = strlen(namesbuf); nick[i] != 0; i++, j++)
//		if (isspace(nick[i]))
//			namesbuf[j] = '_';
//		else
//			namesbuf[j] = nick[i];
//	namesbuf[j] = ' ';
//	namesbuf[j+1] = 0;
}

transfer_t *fnewtransfer(void *handle, const char *filename,
		const char *from, long size) {
	transfer_t	*transfer;

	transfer = calloc(1, sizeof(transfer_t));
	assert(transfer != NULL);
	transfer->handle = handle;
	transfer->size = size;
	STRREPLACE(transfer->from, from);
	STRREPLACE(transfer->remote, filename);
	transfer->lastupdate = 0;

	return(transfer);
}

void	fremove(transfer_t *transfer) {
	FREESTR(transfer->from);
	FREESTR(transfer->remote);
	free(transfer);
}

nFIRE_HANDLER(naim_file_offer) {
	conn_t		*conn = (conn_t *)client;
	va_list		msg;
	buddywin_t	*bwin;
	void		*handle;
	const char	*from,
			*filename;
	long		size;

	va_start(msg, client);
	handle = va_arg(msg, void *);
	from = va_arg(msg, const char *);
	filename = va_arg(msg, const char *);
	size = va_arg(msg, long);
	va_end(msg);

	if (bgetwin(conn, filename, TRANSFER) == NULL) {
		bnewwin(conn, filename, TRANSFER);
		bwin = bgetwin(conn, filename, TRANSFER);
		assert(bwin != NULL);
		bwin->waiting = 1;
		bwin->e.transfer = fnewtransfer(handle, filename, from, size);
		echof(conn, NULL, "File transfer request from <font color=\"#00FFFF\">%s</font> (%s, %lu B).\n",
			from, filename, size);
		window_echof(bwin, "File transfer request from <font color=\"#00FFFF\">%s</font> (%s, %lu B).\n",
			from, filename, size);
		window_echof(bwin, "Type <font color=\"#00FF00\">/accept filename</font> to begin the transfer.\n");
	} else {
		firetalk_file_cancel(conn->conn, handle);
		echof(conn, NULL, "Ignoring duplicate file transfer request from <font color=\"#00FFFF\">%s</font> (%s, %lu B).\n",
			from, filename, size);
	}
}

nFIRE_HANDLER(naim_file_start) {
/*	conn_t		*conn = (conn_t *)client; */
	va_list		msg;
	buddywin_t	*bwin;
	void		*handle;

	va_start(msg, client);
	handle = va_arg(msg, void *);
	bwin = (buddywin_t *)va_arg(msg, void *);
	va_end(msg);

	window_echof(bwin, "Transfer of %s has begun.\n", bwin->e.transfer->remote);
	bwin->e.transfer->started = nowf-0.1;
}

nFIRE_HANDLER(naim_file_progress) {
/*	conn_t		*conn = (conn_t *)client; */
	va_list		msg;
	buddywin_t	*bwin;
	void		*handle;
	long		bytes,
			size;

	va_start(msg, client);
	handle = va_arg(msg, void *);
	bwin = (buddywin_t *)va_arg(msg, void *);
	bytes = va_arg(msg, long);
	size = va_arg(msg, long);
	va_end(msg);

	assert(bwin->et == TRANSFER);
	if (bwin->e.transfer->handle == NULL)
		bwin->e.transfer->handle = handle;
	assert(handle == bwin->e.transfer->handle);

	if (size != bwin->e.transfer->size) {
		window_echof(bwin, "File size for %s changed from %lu to %lu.\n",
			bwin->e.transfer->remote, bwin->e.transfer->size, size);
		bwin->e.transfer->size = size;
	}
	bwin->e.transfer->bytes = bytes;

	if ((bwin->e.transfer->lastupdate+5) < now) {
		window_echof(bwin, "STATUS %s/s, %lu/%lu (%.0f%%)\n",
			dsize(bytes/(nowf-bwin->e.transfer->started)),
			bytes, size, 100.0*bytes/size);
		bwin->e.transfer->lastupdate = now;
	}
}

nFIRE_HANDLER(naim_file_finish) {
	conn_t		*conn = (conn_t *)client;
	va_list		msg;
	buddywin_t	*bwin;
	void		*handle;
	long		size;

	va_start(msg, client);
	handle = va_arg(msg, void *);
	bwin = (buddywin_t *)va_arg(msg, void *);
	size = va_arg(msg, long);
	va_end(msg);

	assert(handle == bwin->e.transfer->handle);
	if (size != bwin->e.transfer->size) {
		window_echof(bwin, "File size for %s changed from %lu to %lu.\n",
			bwin->e.transfer->remote, bwin->e.transfer->size, size);
		bwin->e.transfer->size = size;
	}
	window_echof(bwin, "STATUS %s/s, %lu/%lu (%i%%)\n",
		dsize(size/(nowf-bwin->e.transfer->started)),
		size, size, 100);
	echof(conn, NULL, "File transfer (%s -> %s) completed.\n",
		bwin->e.transfer->remote, bwin->e.transfer->local);
	bwin->waiting = 1;
	bwin->e.transfer->bytes = bwin->e.transfer->size;
	bwin->e.transfer->size *= -1;
}

nFIRE_HANDLER(naim_file_error) {
	conn_t		*conn = (conn_t *)client;
	va_list		msg;
	buddywin_t	*bwin;
	void		*handle;
	int		error;

	va_start(msg, client);
	handle = va_arg(msg, void *);
	bwin = (buddywin_t *)va_arg(msg, void *);
	error = va_arg(msg, int);
	va_end(msg);

	assert(handle == bwin->e.transfer->handle);
	echof(conn, NULL, "Error receiving %s: %s.\n",
		bwin->e.transfer->remote, firetalk_strerror(error));
	bwin->waiting = 1;
}

static time_t	lastctcp = 0;

nFIRE_CTCPHAND(naim_ctcp_VERSION) {
	conn_t	*conn = (conn_t *)client;

	if (lastctcp < now-1) {
		firetalk_subcode_send_reply(sess, from, "VERSION", (*naim_version)?naim_version:NAIM_VERSION_STRING);
		lastctcp = now;
		echof(conn, "CTCP", "<font color=\"#00FFFF\">%s</font> requested your version.\n",
			from);
	}
}

nFIRE_CTCPHAND(naim_ctcp_PING) {
	conn_t	*conn = (conn_t *)client;

	if (lastctcp < now-1) {
		firetalk_subcode_send_reply(sess, from, "PING", args);
		lastctcp = now;
		echof(conn, "CTCP", "<font color=\"#00FFFF\">%s</font> pinged you.\n",
			from);
	}
}

nFIRE_CTCPHAND(naim_ctcp_LC) {
	conn_t	*conn = (conn_t *)client;

	if ((args == NULL) || (*args == 0))
		return;

	if (firetalk_compare_nicks(conn->conn, conn->sn, from) == FE_SUCCESS) {
		conn->lag = nowf - atof(args);
		bupdate();
	}
}

nFIRE_CTCPHAND(naim_ctcp_HEXTEXT) {
	unsigned char
		buf[4*1024];
	int	i;

	if ((args == NULL) || (*args == 0))
		return;

	for (i = 0; (i/2 < sizeof(buf)-1) && (args[i] != 0) && (args[i+1] != 0); i += 2)
		buf[i/2] = (hexdigit(args[i]) << 4) | hexdigit(args[i+1]);
	buf[i/2] = 0;

#if 0
	echof(curconn, "HEXTEXT", "<-- %s %s %s", from, args, buf);
#endif

	naim_recvfrom(client, from, NULL, buf, i/2, RF_ENCRYPTED);
}

nFIRE_CTCPHAND(naim_ctcprep_HEXTEXT) {
	char	buf[4*1024];
	int	i;

	if ((args == NULL) || (*args == 0))
		return;

	for (i = 0; (i/2 < sizeof(buf)-1) && (args[i] != 0) && (args[i+1] != 0); i += 2)
		buf[i/2] = (hexdigit(args[i]) << 4) | hexdigit(args[i+1]);
	buf[i/2] = 0;

#if 0
	echof(curconn, "HEXTEXT", "<-- %s %s %s", from, args, buf);
#endif

	naim_recvfrom(client, from, NULL, buf, i/2, RF_ENCRYPTED | RF_AUTOMATIC);
}

nFIRE_CTCPHAND(naim_ctcp_AUTOPEER) {
	conn_t		*conn = (conn_t *)client;
	buddylist_t	*blist;
	char		*str;

	if ((args == NULL) || (*args == 0))
		return;

	if ((blist = rgetlist(conn, from)) == NULL) {
		if (getvar_int(conn, "autopeerverbose") > 0)
			status_echof(conn, "Received autopeer message (%s) from non-buddy %s.\n",
				args, from);
		if ((strcmp(args, "-AUTOPEER") != 0) && (strcmp(args, "-AUTOCRYPT") != 0)) {
			if (getvar_int(conn, "autobuddy") != 0) {
				status_echof(conn, "Adding <font color=\"#00FFFF\">%s</font> to your buddy list due to autopeer.\n",
					from);
				blist = raddbuddy(conn, from, DEFAULT_GROUP, NULL);
				bnewwin(conn, from, BUDDY);
				firetalk_im_add_buddy(conn->conn, from, USER_GROUP(blist), NULL);
			} else {
				if (getvar_int(conn, "autopeerverbose") > 0)
					status_echof(conn, "Declining automatic negotiation with <font color=\"#00FFFF\">%s</font> (add <font color=\"#00FFFF\">%s</font> to your buddy list).\n", 
						from, from);
				firetalk_subcode_send_request(conn->conn, from, "AUTOPEER", "-AUTOPEER");
				return;
			}
		} else {
			if (getvar_int(conn, "autopeerverbose") > 0)
				status_echof(conn, "... ignored.\n");
			return;
		}
	}
	assert(blist != NULL);

	str = strdup(args);
	args = str;
	while (args != NULL) {
		char	buf[1024],
			*sp = strchr(args, ' '),
			*co;

		if (sp != NULL)
			*sp = 0;

		if ((co = strchr(args, ':')) != NULL) {
			*co = 0;
			co++;
			if (*co == 0)
				co = NULL;
		}

		if (strcmp(args, "--") == 0)
			break;
		else if (strcasecmp(args, "+AUTOPEER") == 0) {
			int	lev;

			if (co != NULL)
				lev = atoi(co);
			else
				lev = 1;

			if (blist->peer != lev) {
				const char
					*autocrypt_flag,
					*autozone_flag1,
					*autozone_flag2;

				if (getvar_int(conn, "autopeerverbose") > 0)
					status_echof(conn, "Peer level %i automatically negotiated with %s.\n",
						lev, from);

				if ((lev > 2) && (blist->peer == 0) && (blist->crypt == NULL) && (getvar_int(conn, "autocrypt") > 0))
					autocrypt_flag = " +AUTOCRYPT";
				else
					autocrypt_flag = "";

				if ((lev > 2) && ((autozone_flag2 = getvar(conn, "autozone")) != NULL) && (*autozone_flag2 != 0))
					autozone_flag1 = " +AUTOZONE:";
				else {
					autozone_flag1 = "";
					autozone_flag2 = "";
				}

				snprintf(buf, sizeof(buf), "+AUTOPEER:%i%s%s%s", 4,
					autocrypt_flag, autozone_flag1, autozone_flag2);

				blist->peer = lev;
				firetalk_subcode_send_request(conn->conn, from, "AUTOPEER", buf);
			}
		} else if (strcasecmp(args, "-AUTOPEER") == 0) {
			if (getvar_int(conn, "autopeerverbose") > 0) {
				if (blist->peer == 0)
					status_echof(conn, "Automatic negotiation with <font color=\"#00FFFF\">%s</font> declined (you are probably not on <font color=\"#00FFFF\">%s</font>'s buddy list).\n", 
						from, from);
				else if (blist->peer > 0)
					status_echof(conn, "Negotiated session with <font color=\"#00FFFF\">%s</font> terminated.\n",
						from);
			}
			if (blist->crypt != NULL) {
				free(blist->crypt);
				blist->crypt = NULL;
				firetalk_subcode_send_request(conn->conn, from, "AUTOPEER", "-AUTOCRYPT");
			}
			blist->docrypt = blist->peer = 0;
		} else if (strcasecmp(args, "+AUTOCRYPT") == 0) {
			if (getvar_int(conn, "autocrypt") == 0)
				firetalk_subcode_send_request(conn->conn, from, "AUTOPEER", "-AUTOCRYPT");
			else {
				if (co == NULL) {
					char	key[21],
						buf[1024];
					int	i = 0;

					while (i < sizeof(key)-1) {
						key[i] = 1 + rand()%255;
						if (!isspace(key[i]) && (firetalk_isprint(conn->conn, key[i]) == FE_SUCCESS))
							i++;
					}
					key[i] = 0;

					snprintf(buf, sizeof(buf), "+AUTOCRYPT:%s", key);
					firetalk_subcode_send_request(conn->conn, from, "AUTOPEER", buf);

					co = key;
				}

				if ((blist->crypt == NULL) || (strcmp(blist->crypt, co) != 0)) {
					blist->docrypt = 1;
					STRREPLACE(blist->crypt, co);
					if (getvar_int(conn, "autopeerverbose") > 0)
						status_echof(conn, "Now encrypting messages sent to <font color=\"#00FFFF\">%s</font> with XOR [%s].\n",
							from, co);
				}
			}
		} else if (strcasecmp(args, "+AUTOZONE") == 0) {
			if (co == NULL)
				status_echof(conn, "Received blank time zone from peer <font color=\"#00FFFF\">%s</font> <scratches head>.\n",
					from);
			else
				STRREPLACE(blist->tzname, co);
		}

		if ((strcasecmp(args, "-AUTOCRYPT") == 0) || (strcasecmp(args, "-AUTOPEER") == 0)) {
			if (blist->crypt != NULL) {
				blist->docrypt = 0;
				free(blist->crypt);
				blist->crypt = NULL;
				firetalk_subcode_send_request(conn->conn, from, "AUTOPEER", "-AUTOCRYPT");
				if (getvar_int(conn, "autopeerverbose") > 0)
					status_echof(conn, "No longer encrypting messages sent to %s.\n",
						from);
			}
		}

		if (sp != NULL) {
			args = sp+1;
			while (isspace(*args))
				args++;
			if (*args == 0)
				args = NULL;
		} else
			args = NULL;
	}
	free(str);
}

nFIRE_CTCPHAND(naim_ctcp_default) {
	conn_t	*conn = (conn_t *)client;

	if (getvar_int(conn, "ctcpverbose") > 0) {
		if (args == NULL)
			echof(conn, "CTCP", "Unknown CTCP %s from <font color=\"#00FFFF\">%s</font>.\n",
				command, from);
		else
			echof(conn, "CTCP", "Unknown CTCP %s from <font color=\"#00FFFF\">%s</font>: %s.\n",
				command, from, args);
	}
}

nFIRE_CTCPHAND(naim_ctcprep_VERSION) {
	conn_t	*conn = (conn_t *)client;
	char	*str = strdup(args), *ver, *env;
	int	i, show = 1;

	for (i = 0; i < awayc; i++)
		if (firetalk_compare_nicks(conn->conn, from, awayar[i].name) == FE_SUCCESS) {
			show = 0;
			break;
		}

	if (((ver = strchr(str, ':')) != NULL)
	 && ((env = strchr(ver+1, ':')) != NULL)
	 && (strchr(env+1, ':') == NULL)) {
		*ver++ = 0;
		*env++ = 0;
		if (show)
			echof(conn, NULL, "<font color=\"#00FFFF\">%s</font> is running %s version %s (%s).\n",
				from, str, ver, env);
	} else if (show)
		echof(conn, NULL, "CTCP VERSION reply from <font color=\"#00FFFF\">%s</font>: %s.\n",
			from, args);
	free(str);
}

nFIRE_CTCPHAND(naim_ctcprep_AWAY) {
	conn_t	*conn = (conn_t *)client;
	int	time;
	const char	*rest;
	buddywin_t	*bwin;

	if ((args == NULL) || (*args == 0))
		return;

	time = atoi(args);

	if (((time > 0) || (strncmp(args, "0 ", 2) == 0))
		&& ((rest = strchr(args, ' ')) != NULL)) {
		rest++;
		if (*rest == ':')
			rest++;
	} else {
		time = -1;
		rest = args;
	}

	if ((bwin = bgetwin(conn, from, BUDDY)) != NULL)
		STRREPLACE(bwin->blurb, rest);

	if (awayc > 0) {
		int	i;

		assert(awayar != NULL);

		for (i = 0; i < awayc; i++)
			if (firetalk_compare_nicks(conn->conn, from, awayar[i].name) == FE_SUCCESS) {
				if (bwin == NULL)
					status_echof(conn, "<font color=\"#00FFFF\">%s</font> is now away: %s.\n",
						from, rest);
				else
					window_echof(bwin, "<font color=\"#00FFFF\">%s</font> is now away: %s.\n",
						user_name(NULL, 0, conn, bwin->e.buddy), rest);
				awayar[i].gotaway = 1;
				return;
			}
	}

	if (time >= 0)
		echof(conn, NULL, "<font color=\"#00FFFF\">%s</font> has been away for %s: %s.\n",
			from, dtime(time*60), rest);
	//else
	//	echof(conn, NULL, "CTCP AWAY reply from <font color=\"#00FFFF\">%s</font>: %s.\n",
	//		from, rest);
}

nFIRE_CTCPHAND(naim_ctcprep_default) {
	conn_t	*conn = (conn_t *)client;

	if (args == NULL)
		echof(conn, NULL, "CTCP %s reply from <font color=\"#00FFFF\">%s</font>.\n",
			command, from);
	else
		echof(conn, NULL, "CTCP %s reply from <font color=\"#00FFFF\">%s</font>: %s.\n",
			command, from, args);
}

conn_t	*naim_newconn(int proto) {
	conn_t	*conn = calloc(1, sizeof(conn_t));

	assert(conn != NULL);

	conn->proto = proto;
	naim_lastupdate(conn);

	{
		conn->conn = firetalk_create_handle(proto, conn);

		firetalk_register_callback(conn->conn, FC_DOINIT,
			naim_doinit);
		firetalk_register_callback(conn->conn, FC_POSTSELECT,
			naim_postselect);
		firetalk_register_callback(conn->conn, FC_CONNECTED,
			naim_connected);
		firetalk_register_callback(conn->conn, FC_CONNECTFAILED,
			naim_connectfailed);
		firetalk_register_callback(conn->conn, FC_ERROR,
			naim_error_msg);
		firetalk_register_callback(conn->conn, FC_DISCONNECT,
			naim_error_disconnect);
		firetalk_register_callback(conn->conn, FC_SETIDLE,
			naim_setidle);

		firetalk_register_callback(conn->conn, FC_EVILED,
			naim_warned);
		firetalk_register_callback(conn->conn, FC_NEWNICK,
			naim_newnick);
		/* FC_PASSCHANGED */

		firetalk_register_callback(conn->conn, FC_IM_GOTINFO,
			naim_userinfo_handler);
		firetalk_register_callback(conn->conn, FC_IM_USER_NICKCHANGED,
			naim_nickchange);
		firetalk_register_callback(conn->conn, FC_IM_GETMESSAGE,
			naim_im_handle);
		firetalk_register_callback(conn->conn, FC_IM_GETACTION,
			naim_act_handle);
		firetalk_register_callback(conn->conn, FC_IM_BUDDYADDED,
			naim_buddyadded);
		firetalk_register_callback(conn->conn, FC_IM_BUDDYREMOVED,
			naim_buddyremoved);
		firetalk_register_callback(conn->conn, FC_IM_BUDDYONLINE,
			naim_buddy_coming);
		firetalk_register_callback(conn->conn, FC_IM_BUDDYOFFLINE,
			naim_buddy_going);
		firetalk_register_callback(conn->conn, FC_IM_BUDDYFLAGS,
			naim_buddy_flags);
		firetalk_register_callback(conn->conn, FC_IM_BUDDYAWAY,
			naim_buddy_away);
		firetalk_register_callback(conn->conn, FC_IM_BUDDYUNAWAY,
			naim_buddy_unaway);
		firetalk_register_callback(conn->conn, FC_IM_IDLEINFO,
			naim_buddy_idle);
		firetalk_register_callback(conn->conn, FC_IM_STATUSINFO,
			naim_buddy_status);
		firetalk_register_callback(conn->conn, FC_IM_TYPINGINFO,
			naim_buddy_typing);
		firetalk_register_callback(conn->conn, FC_IM_EVILINFO,
			naim_buddy_eviled);
		firetalk_register_callback(conn->conn, FC_IM_CAPABILITIES,
			naim_buddy_caps);
		firetalk_register_callback(conn->conn, FC_IM_LISTBUDDY,
			naim_buddylist);

		firetalk_register_callback(conn->conn, FC_CHAT_JOINED,
			naim_chat_joined);
		firetalk_register_callback(conn->conn, FC_CHAT_LEFT,
			naim_chat_left);
		firetalk_register_callback(conn->conn, FC_CHAT_KICKED,
			naim_chat_kicked);
		firetalk_register_callback(conn->conn, FC_CHAT_KEYCHANGED,
			naim_chat_KEYCHANGED);
		firetalk_register_callback(conn->conn, FC_CHAT_GETMESSAGE,
			naim_chat_getmessage);
		firetalk_register_callback(conn->conn, FC_CHAT_GETACTION,
			naim_chat_act_handle);
		firetalk_register_callback(conn->conn, FC_CHAT_INVITED,
			naim_chat_invited);
#ifdef RAWIRCMODES
		firetalk_register_callback(conn->conn, FC_CHAT_MODECHANGED,
			naim_chat_MODECHANGED);
#endif
		firetalk_register_callback(conn->conn, FC_CHAT_OPPED,
			naim_chat_oped);
		firetalk_register_callback(conn->conn, FC_CHAT_DEOPPED,
			naim_chat_deoped);
		firetalk_register_callback(conn->conn, FC_CHAT_USER_JOINED,
			naim_chat_JOIN);
		firetalk_register_callback(conn->conn, FC_CHAT_USER_LEFT,
			naim_chat_PART);
		firetalk_register_callback(conn->conn, FC_CHAT_GOTTOPIC,
			naim_chat_TOPIC);
		firetalk_register_callback(conn->conn, FC_CHAT_USER_OPPED,
			naim_chat_OP);
		firetalk_register_callback(conn->conn, FC_CHAT_USER_DEOPPED,
			naim_chat_DEOP);
		firetalk_register_callback(conn->conn, FC_CHAT_USER_KICKED,
			naim_chat_KICK);
		firetalk_register_callback(conn->conn, FC_CHAT_USER_NICKCHANGED,
			naim_chat_NICK);
		firetalk_register_callback(conn->conn, FC_CHAT_LISTMEMBER,
			naim_chat_NAMES);

		firetalk_register_callback(conn->conn, FC_FILE_OFFER,
			naim_file_offer);
		firetalk_register_callback(conn->conn, FC_FILE_START,
			naim_file_start);
		firetalk_register_callback(conn->conn, FC_FILE_PROGRESS,
			naim_file_progress);
		firetalk_register_callback(conn->conn, FC_FILE_FINISH,
			naim_file_finish);
		firetalk_register_callback(conn->conn, FC_FILE_ERROR,
			naim_file_error);

		firetalk_register_callback(conn->conn, FC_NEEDPASS,
			naim_needpass);

		firetalk_subcode_register_request_callback(conn->conn, "VERSION",
			naim_ctcp_VERSION);
		firetalk_subcode_register_request_callback(conn->conn, "PING",
			naim_ctcp_PING);
		firetalk_subcode_register_request_callback(conn->conn, "LC",
			naim_ctcp_LC);
		firetalk_subcode_register_request_callback(conn->conn, "HEXTEXT",
			naim_ctcp_HEXTEXT);
		firetalk_subcode_register_request_callback(conn->conn, "AUTOPEER",
			naim_ctcp_AUTOPEER);
		firetalk_subcode_register_request_callback(conn->conn, NULL,
			naim_ctcp_default);

		firetalk_subcode_register_reply_callback(conn->conn, "HEXTEXT",
			naim_ctcprep_HEXTEXT);
		firetalk_subcode_register_reply_callback(conn->conn, "VERSION",
			naim_ctcprep_VERSION);
		firetalk_subcode_register_reply_callback(conn->conn, "AWAY",
			naim_ctcprep_AWAY);
		firetalk_subcode_register_reply_callback(conn->conn, NULL,
			naim_ctcprep_default);
	}

	return(conn);
}

void	naim_lastupdate(conn_t *conn) {
	int	autohide = secs_getvar_int("autohide");

	if ((conn->lastupdate + autohide) < nowf)
		conn->lastupdate = nowf;
	else if ((conn->lastupdate + SLIDETIME) < nowf)
		conn->lastupdate = nowf - SLIDETIME - SLIDETIME/autohide;
}
