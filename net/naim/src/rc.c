/*  _ __   __ _ ___ __  __
** | '_ \ / _` |_ _|  \/  | naim
** | | | | (_| || || |\/| | Copyright 1998-2004 Daniel Reed <n@ml.org>
** |_| |_|\__,_|___|_|  |_| ncurses-based chat client
*/
#include <naim/naim.h>
#include <fcntl.h>

#include "naim-int.h"

extern conn_t	*curconn;
extern const char	*home;
extern time_t	now;

buddylist_t
	*raddbuddy(conn_t *conn, const char *screenname, const char *group,
	const char *notes) {
	buddylist_t	*buddy = NULL;

	assert(conn != NULL);
	assert(screenname != NULL);
	assert(group != NULL);

	for (buddy = conn->buddyar; buddy != NULL; buddy = buddy->next)
		if (firetalk_compare_nicks(conn->conn, screenname, buddy->_account) == FE_SUCCESS)
			return(buddy);

	buddy = calloc(1, sizeof(buddylist_t));
	assert(buddy != NULL);
	STRREPLACE(buddy->_account, screenname);
	STRREPLACE(buddy->_group, group);
	if (notes != NULL)
		STRREPLACE(buddy->_name, notes);
	else
		FREESTR(buddy->_name);

	if ((conn->buddyar == NULL) || (aimcmp(screenname, conn->buddyar->_account) <= 0)) {
		buddy->next = conn->buddyar;
		conn->buddyar = buddy;
	} else {
		buddylist_t	*btmp = conn->buddyar;

		while ((btmp->next != NULL) && (aimcmp(screenname, btmp->next->_account) > 0))
			btmp = btmp->next;
		buddy->next = btmp->next;
		btmp->next = buddy;
	}

	buddy->offline = 1;
	buddy->isaway = buddy->isidle = 0;

	return(buddy);
}

void	do_delbuddy(buddylist_t *b) {
	FREESTR(b->_account);
	FREESTR(b->_group);
	FREESTR(b->_name);
	FREESTR(b->crypt);
	FREESTR(b->tzname);
	FREESTR(b->tag);
	FREESTR(b->caps);
	free(b);
}

void	rdelbuddy(conn_t *conn, const char *screenname) {
	buddylist_t	*buddy = NULL;

	assert(conn != NULL);
	assert(screenname != NULL);

	assert(bgetwin(conn, screenname, BUDDY) == NULL);

	if ((buddy = conn->buddyar) == NULL)
		return;

	if (firetalk_compare_nicks(conn->conn, screenname, buddy->_account) == FE_SUCCESS) {
		if ((conn->online > 0) && (buddy->peer > 0))
			firetalk_subcode_send_request(conn->conn, screenname, "AUTOPEER", "-AUTOPEER");
		buddy->peer = 0;
		conn->buddyar = buddy->next;
		do_delbuddy(buddy);
		return;
	}
	while (buddy->next != NULL) {
		if (firetalk_compare_nicks(conn->conn, screenname, buddy->next->_account) == FE_SUCCESS) {
			buddylist_t	*b = buddy->next;

			if ((conn->online > 0) && (b->peer > 0))
				firetalk_subcode_send_request(conn->conn, screenname, "AUTOPEER", "-AUTOPEER");
			b->peer = 0;
			buddy->next = buddy->next->next;
			do_delbuddy(b);
			return;
		}
		buddy = buddy->next;
	}
}

void	raddidiot(conn_t *conn, const char *screenname, const char *notes) {
	ignorelist_t	*idiot = NULL;

	assert(conn != NULL);
	assert(screenname != NULL);
	idiot = conn->idiotar;
	while (idiot != NULL) {
		if (firetalk_compare_nicks(conn->conn, screenname, idiot->screenname) == FE_SUCCESS)
			break;
		idiot = idiot->next;
	}
	if (idiot == NULL) {
		idiot = calloc(1, sizeof(ignorelist_t));
		assert(idiot != NULL);
		idiot->next = conn->idiotar;
		conn->idiotar = idiot;
	}
	STRREPLACE(idiot->screenname, screenname);
	if (notes != NULL)
		STRREPLACE(idiot->notes, notes);
	else
		STRREPLACE(idiot->notes, "There is no reason, you're just"
			" ignored!");
}

void	rdelidiot(conn_t *conn, const char *screenname) {
	ignorelist_t	*idiot = NULL;

	assert(conn != NULL);
	assert(screenname != NULL);

	if ((idiot = conn->idiotar) == NULL)
		return;
	if (firetalk_compare_nicks(conn->conn, screenname, idiot->screenname) == FE_SUCCESS) {
		conn->idiotar = idiot->next;
		free(idiot->screenname);
		idiot->screenname = NULL;
		free(idiot->notes);
		idiot->notes = NULL;
		free(idiot);
		return;
	}
	while (idiot->next != NULL) {
		if (firetalk_compare_nicks(conn->conn, screenname, idiot->next->screenname) == FE_SUCCESS) {
			ignorelist_t	*b = idiot->next;

			idiot->next = idiot->next->next;
			free(b->screenname);
			b->screenname = NULL;
			free(b->notes);
			b->notes = NULL;
			free(b);
			return;
		}
		idiot = idiot->next;
	}
}

buddylist_t
	*rgetlist(conn_t *conn, const char *screenname) {
	buddylist_t	*blist = NULL;

	assert(conn != NULL);
	assert(screenname != NULL);
	if ((blist = conn->buddyar) != NULL)
		do {
			if (firetalk_compare_nicks(conn->conn, blist->_account, screenname) == FE_SUCCESS)
				return(blist);
		} while ((blist = blist->next) != NULL);
	return(NULL);
}

const char
	*buddy_tabcomplete(conn_t *const conn, const char *start, const char *buf, const int bufloc, int *const match, const char **desc) {
	static char
		str[1024];
	buddylist_t	*blist = NULL;
	size_t	slen;

	assert(start != NULL);
	slen = strlen(start);
	if ((blist = conn->buddyar) != NULL)
		do {
			if (aimncmp(blist->_account, start, slen) == 0) {
				const char
					*name = blist->_account;
				int	j;

				if (match != NULL)
					*match = bufloc - (start-buf);
				if (desc != NULL)
					*desc = blist->_name;
				for (j = 0; (j < sizeof(str)-1) && (*name != 0); j++) {
					while (*name == ' ')
						name++;
					str[j] = *(name++);
				}
				str[j] = 0;
				return(str);
			}
		} while ((blist = blist->next) != NULL);
	return(NULL);
}

#if 0
static int
	naim_eval(const char *str) {
	assert(str != NULL);
	if (*str == 0)
		return(0);
	if (strncasecmp(str, "LINES", 5) == 0) {
		str += 5;
		while ((*str != 0) && isspace(*str))
			str++;
		if (*str == 0)
			return(LINES);
		if (*str == '-') {
			str++;
			while ((*str != 0) && isspace(*str))
				str++;
			return(LINES - atoi(str));
		}
		while ((*str != 0) && isspace(*str))
			str++;
		return(atoi(str));
	}
	if (strncasecmp(str, "COLS", 4) == 0) {
		str += 4;
		while ((*str != 0) && isspace(*str))
			str++;
		if (*str == 0)
			return(COLS);
		if (*str == '-') {
			str++;
			while ((*str != 0) && isspace(*str))
				str++;
			return(COLS - atoi(str));
		}
		while ((*str != 0) && isspace(*str))
			str++;
		return(atoi(str));
	}
	return(atoi(str));
}
#endif

int	rc_resize(faimconf_t *conf) {
	static int
		lastCOLS = 0,
		lastLINES = 0;

	if ((lastCOLS == COLS) && (lastLINES == LINES))
		return(0);
	lastCOLS = COLS;
	lastLINES = LINES;

	conf->wstatus.widthx = COLS;
	conf->wstatus.startx = 0;
	conf->wstatus.widthy = LINES-2;
	conf->wstatus.starty = 0;
	conf->wstatus.pady = secs_getvar_int("scrollback");

	conf->winput.widthx = COLS;
	conf->winput.startx = 0;
	conf->winput.widthy = 1;
	conf->winput.starty = LINES - conf->winput.widthy;
	conf->winput.pady = 0;

	conf->winfo.widthx = COLS;
	conf->winfo.startx = 0;
	conf->winfo.widthy = 1;
	conf->winfo.starty = LINES-2;
	conf->winfo.pady = 0;

	conf->waway.widthx = 44;
	conf->waway.startx = COLS/2 - (conf->waway.widthx)/2;
	conf->waway.widthy = 4;
	conf->waway.starty = LINES/2 - 1 - (conf->waway.widthy)/2;
	conf->waway.pady = 0;

	return(1);
}

rc_var_s_t
	rc_var_s_ar[] = {
	{ "nameformat",		"$user_name_account$user_name_ifwarn",
								"window list name format for unnamed users" },
	{ "nameformat_named",	"$user_name_account$user_name_ifwarn ($user_name_name)",
								"window list name format for named buddies" },
	{ "awaymsg",		"I am currently away...",	"auto-response sent when you are away" },
	{ "autoawaymsg",	"Must have stepped out...",	"auto-response used if you idle away (see $autoaway)" },
	{ "autozone",		NULL,				"time zone to send to peers" },
	{ "logdir",		".naimlog/$conn/$cur",		"base directory to store persistent window scrollback" },
	{ "statusbar",		" %I:%M%p $ifoper$SN$ifwarn$ifaway$ifquery$ifchat+$ifpending$iftransfer [$conn $online]$iflag$ifidle naim ",
								"string to use for status bar, replace $if* with $statusbar_*" },
	{ "statusbar_away",	" (away)",			"display if you are away" },
	{ "statusbar_idle",	" [Idle $idle]",		"display if you haven't said anything for 10 minutes" },
	{ "statusbar_lag",	" [Lag $lag]",			"display if there is network delay between you and the server" },
	{ "statusbar_oper",	"@",				"display if the current window is a chat, and you are a channel operator" },
	{ "statusbar_warn",	"/$warnval%",			"if your warning level is above 0%" },
	{ "statusbar_chat",	" on $cur$iftopic",		"display if the current window is a chat" },
	{ "statusbar_crypt",	"ENCRYPTED ",			"display if the current window is a query, and query is automatically encrypted" },
	{ "statusbar_typing",	" TYPING",			"if the current window is a query, and the other person is typing to you" },
	{ "statusbar_tzname",	" <$tzname>",			"display if the current window is a query, and you know the buddy's time zone" },
	{ "statusbar_query",	" [$ifcryptQuery: $cur$iftopic$iftzname]$iftyping",
								"display if the current window is a query" },
	{ "timeformat",		"[%H:%M:%S]&nbsp;",		"strftime format prepended to all messages" },
	{ "im_prefix",		"",				"string added to the beginning of every IM sent" },
	{ "im_suffix",		"",				"string added to the end of every IM sent" },
};
const int
	rc_var_s_c = sizeof(rc_var_s_ar)/sizeof(*rc_var_s_ar);

rc_var_i_t
	rc_var_i_ar[] = {
	{ "chatter",		(IM_MESSAGE | CH_ATTACKED | CH_ATTACKS | CH_MESSAGE),
					"determines which events trigger 'updated' status (yellow)" },
	{ "awaylog",		0,	"1 = log IM's to an :AWAYLOG window when away, 2 = IM's and chat messages" },
	{ "echostyle",		(ALSO_STATUS),
					"determines how /echo behaves" },
	{ "winlistheight",	90,	"percentage of the screen height to use for the window list window" },
	{ "winlistchars",	16,	"number of characters from each window name to display in the window list" },
	{ "beeponim",		1,	"beep whenever an IM is received (0 = never, 1 = if not away, 2 = always)" },
	{ "beeponsignon",	0,	"beep whenever a buddy signs on or off (0 = never, 1 = if not away, 2 = always)" },
	{ "autoaway",		20,	"if non-zero, maximum idle time before automatically marking away" },
	{ "autoreply",		10,	"number of minutes to wait before sending a second auto-response" },
	{ "tprint",		30,	"if non-zero, a timestamp is displayed periodically in all query windows" },
	{ "logtprint",		2,	"1 = log daily timestamps, 2 = log all timestamps" },
	{ "autohide",		3,	"number of seconds to display the status and window list windows" },
	{ "idletime",		0,	"number of minutes you have been idle" },
	{ "scrollback",		500,	"number of lines in the scrollback buffer" },
	{ "autoclose",		15,	"number of minutes to keep a non-permanent (autobuddy'd) window open after the user signs off" },
	{ "autosort",		2,	"string sorting type (0 = none, 1 = by name, 2 = by group)" },
#ifdef ENABLE_DNSUPDATE
	{ "updatecheck",	4*60,	"number of minutes to wait to check for new versions of naim" },
#endif
};
const int
	rc_var_i_c = sizeof(rc_var_i_ar)/sizeof(*rc_var_i_ar);

rc_var_i_t
	rc_var_b_ar[] = {
	{ "autounaway",		0,	"automatically unmark away status whenever an IM is sent" },
	{ "autoidle",		1,	"automatically increment $idletime by 1 every minute" },
	{ "autounidle",		1,	"reset $idletime to 0 whenever an IM is sent" },
	{ "lagcheck",		1,	"periodically determine delay to server" },
	{ "autoquery",		1,	"whenever a buddy signs on, a temporary window will be opened for them" },
	{ "autopaste",		1,	"enable logic to detect accidental pastes without first entering paste mode" },
	{ "autobuddy",		1,	"whenever a non-buddy IM's you, a window will be opened for them" },
	{ "autopeer",		1,	"automatically attempt to negotiate client capabilities" },
	{ "autopeerverbose",	0,	"notify you as client capabilities are negotiated" },
	{ "autocrypt",		1,	"use autopeer to negotiate peer to peer IM encryption" },
	{ "autosave",		0,	"automatically update .naimrc on /quit" },
	{ "ctcpverbose",	0,	"notify you of unknown CTCP requests" },
	{ "color",		1,	"honor <font color=...> tags in IM's" },
	{ "autoreconnect",	0,	"automatically reconnect if disconnected; you may need to /set an appropriate password variable (such as $AIM:password)" },
	{ "autonames",		1,	"automatically /names on /join" },
	{ "showraw",		0,	"display special server messages in a window named :RAW" },
#ifdef ENABLE_FORCEASCII
	{ "forceascii",		0,	"always use ASCII linedraw chars" },
#endif
};
const int
	rc_var_b_c = sizeof(rc_var_b_ar)/sizeof(*rc_var_b_ar);


#define timeoff		(-100*timezone/(60*60))
#define abstimeoff	((timeoff < 0)?-timeoff:timeoff)
#define chrtimeoff	((timeoff < 0)?'-':'+')

void	rc_initdefs(faimconf_t *conf) {
	static char
		autozone[1024];
	int	i;

	memset(conf, 0, sizeof(*conf));

#ifdef HAVE_STRUCT_TM_TM_ZONE
	{
		struct tm
			*tmptr;

		if ((tmptr = localtime(&now)) != NULL) {
			if (strchr(tmptr->tm_zone, ' ') == NULL)
				snprintf(autozone, sizeof(autozone), "%s%c%04li", 
					tmptr->tm_zone, chrtimeoff, abstimeoff);
			else
				*autozone = 0;
		} else
			*autozone = 0;
	}
#elif HAVE_TZNAME
	{
		extern char
			*tzname[2];

		localtime(&now);
		if (strchr(tzname[0], ' ') == NULL)
			snprintf(autozone, sizeof(autozone), "%s%c%04li", tzname[0], chrtimeoff, 
				abstimeoff);
		else
			*autozone = 0;
	}
#else
	*autozone = 0;
#endif
	for (i = 0; i < rc_var_s_c; i++)
		if (strcmp(rc_var_s_ar[i].var, "autozone") == 0) {
			rc_var_s_ar[i].val = autozone;
			break;
		}

#ifdef DEBUG_ECHO
	for (i = 0; i < rc_var_b_c; i++)
		if (strcmp(rc_var_b_ar[i].var, "lagcheck") == 0) {
			rc_var_b_ar[i].val = 0;
			break;
		}
	for (i = 0; i < rc_var_b_c; i++)
		if (strcmp(rc_var_b_ar[i].var, "showraw") == 0) {
			rc_var_b_ar[i].val = 1;
			break;
		}
	for (i = 0; i < rc_var_i_c; i++)
		if (strcmp(rc_var_i_ar[i].var, "autoaway") == 0) {
			rc_var_i_ar[i].val = 0;
			break;
		}
#endif

	if (secs_getvar("lag") == NULL)
		secs_setvar("lag", "0s");
	if (secs_getvar("SN") == NULL)
		secs_setvar("SN", "- Type /connect \"screen name\" (include the quotes)");
	if (secs_getvar("lag") == NULL)
		secs_setvar("online", "(offline)");

	for (i = 0; i < rc_var_s_c; i++)
		if (secs_getvar(rc_var_s_ar[i].var) == NULL)
			secs_setvar(rc_var_s_ar[i].var, rc_var_s_ar[i].val);

	for (i = 0; i < rc_var_i_c; i++)
		if (secs_getvar(rc_var_i_ar[i].var) == NULL)
			secs_makevar_int(rc_var_i_ar[i].var, rc_var_i_ar[i].val, 'I', NULL);

	for (i = 0; i < rc_var_b_c; i++)
		if (secs_getvar(rc_var_b_ar[i].var) == NULL)
			secs_makevar_int(rc_var_b_ar[i].var, rc_var_b_ar[i].val, 'B', NULL);

	conf->f[cEVENT] = 3;
	conf->f[cEVENT_ALT] = 2;
	conf->f[cTEXT] = 7;
	conf->f[cSELF] = 1;
	conf->f[cBUDDY] = 6;
	conf->f[cBUDDY_WAITING] = 3;
	conf->f[cBUDDY_ADDRESSED] = 3;
	conf->f[cBUDDY_IDLE] = 4;
	conf->f[cBUDDY_AWAY] = 2;
	conf->f[cBUDDY_OFFLINE] = 1;
	conf->f[cBUDDY_QUEUED] = 5;
	conf->f[cBUDDY_TAGGED] = 4;
	conf->f[cBUDDY_MOBILE] = 7;

	conf->b[cINPUT] = 0;
	conf->b[cWINLIST] = 6;
	conf->b[cWINLISTHIGHLIGHT] = 0;
	conf->b[cCONN] = 4;
	conf->b[cIMWIN] = 0;
	conf->b[cSTATUSBAR] = 7;

	rc_resize(conf);
}

int	naim_read_config(const char *faimrcfile) {
	FILE	*faimrc = NULL;
	char	buf[1024];
	int	linenum = 0;

	if (strlen(faimrcfile) < 1)
		return(0);
	if (isspace(faimrcfile[strlen(faimrcfile)-1])) {
		strncpy(buf, faimrcfile, sizeof(buf)-1);
		buf[sizeof(buf)-1] = 0;
		while (isspace(buf[strlen(buf)-1]))
			buf[strlen(buf)-1] = 0;
		faimrcfile = buf;
	}
	if ((faimrc = fopen(faimrcfile, "r")) == NULL) {
		echof(curconn, NULL, "Unable to open %s.\n",
			faimrcfile);
		return(0);
	}
	buf[sizeof(buf)-1] = 0;
	while (!feof(faimrc)) {
		fgets(buf, sizeof(buf)-1, faimrc);
		if (feof(faimrc))
			break;
		linenum++;
		if ((*buf == 0) || (*buf == '#'))
			continue;
		while ((*buf != 0) && ((buf[strlen(buf)-1] == '\n')
			|| (buf[strlen(buf)-1] == '\r')))
			buf[strlen(buf)-1] = 0;
		if (*buf == 0)
			continue;
		conio_handlecmd(buf);
	}
	fclose(faimrc);
	return(1);
}
