/*  _ __   __ _ ___ __  __
** | '_ \ / _` |_ _|  \/  | naim
** | | | | (_| || || |\/| | Copyright 1998-2006 Daniel Reed <n@ml.org>
** |_| |_|\__,_|___|_|  |_| ncurses-based chat client
*/
#include <naim/naim.h>
#include <naim/modutil.h>
#include <sys/stat.h>

#include "naim-int.h"

extern conn_t	*curconn;
extern time_t	now;
extern faimconf_t	faimconf;

extern time_t awaytime G_GNUC_INTERNAL;
time_t	awaytime = -1;

void	logim(conn_t *conn, const char *source, const char *target,
	const unsigned char *msg) {
	struct tm	*tmptr = NULL;

	if (target == NULL)
		target = conn->sn;
	if ((source == NULL) || (target == NULL))
		return;
	if ((awaytime > 0) && (*target != ':')) {
		int	awaylog = getvar_int(conn, "awaylog"),
			private = (firetalk_compare_nicks(conn->conn, conn->sn, target) == FE_SUCCESS);

		if ((awaylog > 1) || ((awaylog == 1) && private)) {
			char	srcbuf[1024];
			int	srccol;

			srccol = naim_strtocol(source);

			if (private)
				snprintf(srcbuf, sizeof(srcbuf), "<font color=\"#%06X\">%s</font>", srccol, source);
			else
				snprintf(srcbuf, sizeof(srcbuf), "<font color=\"#%06X\">%s:%s</font>", srccol, source, target);
			naim_awaylog(conn, srcbuf, msg);
		}
	}

	if (getvar_int(conn, "log") == 0) {
		if (conn->logfile != NULL)
			fclose(conn->logfile);
		return;
	}

	tmptr = localtime(&now);
	assert(tmptr != NULL);

	if (conn->logfile == NULL) {
		char	*f;

		if ((f = getvar(conn, "logfile")) == NULL) {
			echof(conn, "LOGIM", "$%s:log set to 1, but $%s:logfile not set.\n",
				conn->winname, conn->winname);
			return;
		}
		if ((conn->logfile = fopen(f, "a")) == NULL) {
			echof(conn, "LOGIM", "Unable to open \"%s\": %m", f);
			return;
		}
		fchmod(fileno(conn->logfile), 0600);
		fprintf(conn->logfile, "\n*** Log opened %04i-%02i-%02iT"
			"%02i:%02i.<br>\n", 1900+tmptr->tm_year, tmptr->tm_mon+1,
			tmptr->tm_mday, tmptr->tm_hour, tmptr->tm_min);
	}
	assert(source != NULL);
	assert(*source != 0);
	assert(target != NULL);
	assert(*target != 0);
	assert(msg != NULL);
	fprintf(conn->logfile, "%04i-%02i-%02iT%02i:%02i %s -&gt; %s | %s<br>\n",
		1900+tmptr->tm_year, tmptr->tm_mon+1, tmptr->tm_mday,
		tmptr->tm_hour, tmptr->tm_min, source, target, msg);
	fflush(conn->logfile);
}

HOOK_DECLARE(sendto);
static void naim_sendto(conn_t *conn,
		const char *const _name,
		const char *const _dest,
		const unsigned char *const _message, int len,
		int flags) {
	char	*name = NULL, *dest = NULL;
	unsigned char *message = malloc(len+1);

	if (_name != NULL)
		name = strdup(_name);
	if (_dest != NULL)
		dest = strdup(_dest);

	memmove(message, _message, len);
	message[len] = 0;
	HOOK_CALL(sendto, (conn, &name, &dest, &message, &len, &flags));
	free(name);
	free(dest);
	free(message);
}

static int sendto_encrypt(conn_t *conn, char **name, char **dest,
		unsigned char **message, int *len, int *flags) {
	if (!(*flags & RF_CHAT) && !(*flags & RF_ACTION)) {
		buddylist_t	*blist = rgetlist(conn, *dest);

		if (blist != NULL) {
			if (blist->docrypt) {
				int	i, j = 0;

				assert(blist->crypt != NULL);

				for (i = 0; i < *len; i++) {
					(*message)[i] = (*message)[i] ^ (unsigned char)blist->crypt[j++];
					if (blist->crypt[j] == 0)
						j = 0;
				}
				*flags |= RF_ENCRYPTED;
			} else if (getvar_int(conn, "autopeer") != 0) {
				if (blist->peer == 0) {
					blist->peer = -1;
					firetalk_subcode_send_request(conn->conn, *dest, "AUTOPEER", "+AUTOPEER:4");
				}
			}
		}
	}
	return(HOOK_CONTINUE);
}

static int sendto_send(conn_t *conn, char **name, char **dest,
		unsigned char **message, int *len, int *flags) {
	int	ret;

	if (*flags & RF_ENCRYPTED) {
		char	buf[8*1024];
		int	i;

		for (i = 0; (i < *len) && (i < (sizeof(buf)-1)/2); i++)
			sprintf(buf+i*2, "%02X", (*message)[i]);

		if (*flags & RF_AUTOMATIC)
			ret = firetalk_subcode_send_reply(conn->conn, *dest, "HEXTEXT", buf);
		else
			ret = firetalk_subcode_send_request(conn->conn, *dest, "HEXTEXT", buf);
	} else if (*flags & RF_CHAT) {
		if (*flags & RF_ACTION)
			ret = firetalk_chat_send_action(conn->conn, *dest, *message, (*flags & RF_AUTOMATIC)?1:0);
		else
			ret = firetalk_chat_send_message(conn->conn, *dest, *message, (*flags & RF_AUTOMATIC)?1:0);
	} else {
		if (*flags & RF_ACTION)
			ret = firetalk_im_send_action(conn->conn, *dest, *message, (*flags & RF_AUTOMATIC)?1:0);
		else
			ret = firetalk_im_send_message(conn->conn, *dest, *message, (*flags & RF_AUTOMATIC)?1:0);
	}

	if (ret != FE_SUCCESS) {
		buddywin_t	*bwin = bgetwin(conn, *dest, BUDDY);

		if (bwin == NULL)
			bwin = bgetwin(conn, *dest, CHAT);

		if (bwin == NULL)
			echof(conn, NULL, "Unable to send message to %s: %s.\n", *dest, firetalk_strerror(ret));
		else {
			window_echof(bwin, "Unable to send message: %s.\n", firetalk_strerror(ret));
			if (ret == FE_PACKETSIZE)
				window_echof(bwin, "Try shortening your message or splitting it into multiple messages and resending.\n");
		}
		return(HOOK_STOP);
	}
	return(HOOK_CONTINUE);
}

void	hamster_hook_init(void) {
	void	*mod = NULL;

//	HOOK_ADD(sendto, mod, sendto_log, 20);
	HOOK_ADD(sendto, mod, sendto_encrypt, 50);
	HOOK_ADD(sendto, mod, sendto_send, 100);
}

static void naim_send_message(conn_t *const conn, const char *const dest, const unsigned char *const message, const int ischat, const int isauto, const int isaction) {
	naim_sendto(conn, NULL, dest, message, strlen(message), RF_NONE | (ischat?RF_CHAT:0) | (isauto?RF_AUTOMATIC:0) | (isaction?RF_ACTION:0));
}

void	naim_send_act(conn_t *const conn, const char *const dest, const unsigned char *const message) {
	int	ischat = (bgetwin(conn, dest, CHAT) == NULL)?0:1;

	updateidletime();
	naim_send_message(conn, dest, message, ischat, 0, 1);
}

void	naim_send_im(conn_t *conn, const char *SN, const char *msg, const int _auto) {
	buddywin_t *bwin = bgetwin(conn, SN, BUDDY);
	int	ischat = (bgetwin(conn, SN, CHAT) == NULL)?0:1;
	unsigned char buf[2048];
	const char *pre = getvar(conn, "im_prefix"),
		*post = getvar(conn, "im_suffix");

	assert((bwin == NULL) || (bwin->et == BUDDY));
	if ((bwin == NULL)					// if the target is not queueable (let the protocol layer handle errors)
		|| (	   (conn->online > 0)			// or if you are online
			&& (bwin->e.buddy->offline == 0))) {	//  and the target is also tracked online
		if (_auto == 0)
			updateidletime();
		if ((pre != NULL) || (post != NULL)) {
			snprintf(buf, sizeof(buf), "%s%s%s", pre?pre:"", msg, post?post:"");
			msg = buf;
		}
		naim_send_message(conn, SN, msg, ischat, 0, 0);
								// send the message through the protocol layer
	} else {
		struct tm	*tmptr = NULL;

		tmptr = localtime(&now);
		assert(tmptr != NULL);
		if (strncmp(msg, "[Queued ", sizeof("[Queued ")-1) != 0) {
			snprintf(buf, sizeof(buf), "[Queued %04i-%02i-%02iT%02i:%02i] %s",
				1900+tmptr->tm_year, 1+tmptr->tm_mon, tmptr->tm_mday, 
				tmptr->tm_hour, tmptr->tm_min, msg);
			msg = buf;
		}
		assert(bwin != NULL);
		assert(bwin->et == BUDDY);
		bwin->pouncec++;
		bwin->pouncear = realloc(bwin->pouncear,
			bwin->pouncec*sizeof(*(bwin->pouncear)));
		bwin->pouncear[bwin->pouncec-1] = strdup(msg);
		if (bwin->pouncec == 1)
			window_echof(bwin, "Message queued. Type /close to dequeue pending messages.\n");
	}
}

void	naim_send_im_away(conn_t *conn, const char *SN, const char *msg, int force) {
	struct tm	*tmptr;
	buddywin_t	*bwin;
	static time_t	lastauto = 0;
	const char	*pre,
			*post;

	if (force || (lastauto < now-1))
		lastauto = now;
	else {
		echof(conn, "SEND-IM-AWAY", "Suppressing away message to %s (%s).\n", SN, msg);
		return;
	}

	pre = getvar(conn, "im_prefix"),
	post = getvar(conn, "im_suffix");
	if ((pre != NULL) || (post != NULL)) {
		static unsigned char buf[2048];

		snprintf(buf, sizeof(buf), "%s%s%s", pre?pre:"", msg, post?post:"");
		msg = buf;
	}

	tmptr = localtime(&now);
	assert(tmptr != NULL);
	if ((bwin = bgetwin(conn, SN, BUDDY)) == NULL)
		echof(conn, NULL, "Sent away IM to %s (%s)\n", SN, msg);
	else {
		WINTIME(&(bwin->nwin), IMWIN);
		hwprintf(&(bwin->nwin), C(IMWIN,SELF),
			"-<B>%s</B>-", conn->sn);
		hwprintf(&(bwin->nwin), C(IMWIN,TEXT),
			" %s<br>", msg);
	}
	naim_send_message(conn, SN, msg, 0, 1, 0);
}

void	sendaway(conn_t *conn, const char *SN) {
	char	buf[1124];

	if (awaytime == 0)
		return;
	snprintf(buf, sizeof(buf), "[Away for %s] %s", 
		dtime(now-awaytime), secs_getvar("awaymsg"));
	naim_send_im_away(conn, SN, buf, 0);
}



void	setaway(const int auto_flag) {
	conn_t	*conn = curconn;
	char	*awaymsg = secs_getvar("awaymsg");

	awaytime = now - 60*secs_getvar_int("idletime");
	do {
		status_echof(conn, "You are now away--hurry back!\n");
		firetalk_set_away(conn->conn, awaymsg, auto_flag);
	} while ((conn = conn->next) != curconn);
}

void	unsetaway(void) {
	conn_t	*conn = curconn;

	awaytime = 0;
	updateidletime();
	do {
		status_echof(conn, "You are no longer away--welcome back =)\n");
		firetalk_set_away(conn->conn, NULL, 0);
		if (conn->online > 0)
			naim_set_info(conn->conn, conn->profile);
	} while ((conn = conn->next) != curconn);
}

int	getvar_int(conn_t *conn, const char *str) {
	char	buf[1024], *ptr;

	snprintf(buf, sizeof(buf), "%s:%s", conn->winname, str);
	if ((ptr = secs_getvar(buf)) != NULL)
		return(atoi(ptr));
	return(secs_getvar_int(str));
}

char	*getvar(conn_t *conn, const char *str) {
	char	buf[1024], *val;

	snprintf(buf, sizeof(buf), "%s:%s", conn->winname, str);
	if ((val = secs_getvar(buf)) != NULL)
		return(val);
	return(secs_getvar(str));
}
