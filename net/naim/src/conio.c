/*  _ __   __ _ ___ __  __
** | '_ \ / _` |_ _|  \/  | naim
** | | | | (_| || || |\/| | Copyright 1998-2006 Daniel Reed <n@ml.org>
** |_| |_|\__,_|___|_|  |_| ncurses-based chat client
*/
#include <naim/naim.h>
#include <naim/modutil.h>
#include <ltdl.h>

#include <fcntl.h>
#include <signal.h>
#include <unistd.h>

#include "naim-int.h"
#include "conio_keys.h"
#include "snapshot.h"

#ifdef HAVE_HSTRERROR
# include <netdb.h>
#endif

#define CTRL(x)	((x)-'A'+1)

#ifndef CONIOCPP
# define CONIO_C
# include "conio_cmds.h"
#endif

extern win_t	win_input, win_buddy, win_info;
extern conn_t	*curconn;
extern faimconf_t	faimconf;
extern int	stayconnected, quakeoff;
extern time_t	now, awaytime;
extern double	nowf, changetime;
extern const char	*home, *sty;
extern char	*statusbar_text;

extern int
	scrollbackoff G_GNUC_INTERNAL,
	needpass G_GNUC_INTERNAL,
	doredraw G_GNUC_INTERNAL,
	inpaste G_GNUC_INTERNAL,
	withtextcomp G_GNUC_INTERNAL,
	namec;
extern char
	**names G_GNUC_INTERNAL,
	*lastclose G_GNUC_INTERNAL;
extern namescomplete_t
	namescomplete G_GNUC_INTERNAL;
int	scrollbackoff = 0,
	needpass = 0,
	doredraw = 0,
	consolescroll = -1,
	inpaste = 0,
	withtextcomp = 0,
	namec = 0;
char	**names = NULL,
	*lastclose = NULL;
namescomplete_t
	namescomplete;

static const char
	*collist[] = {
	"CLEAR/BLACK",
	"RED",
	"GREEN",
	"YELLOW/BROWN",
	"BLUE",
	"MAGENTA/PINK",
	"CYAN",
	"WHITE/GREY"
};

static const char
	*forelist[] = {
	"EVENT",
	"EVENT_ALT",
	"TEXT",
	"SELF",
	"BUDDY",
	"BUDDY_WAITING",
	"BUDDY_ADDRESSED",
	"BUDDY_IDLE",
	"BUDDY_AWAY",
	"BUDDY_OFFLINE",
	"BUDDY_QUEUED",
	"BUDDY_TAGGED",
	"BUDDY_MOBILE",
};

static const char
	*backlist[] = {
	"INPUT",
	"WINLIST",
	"WINLISTHIGHLIGHT",
	"CONN",
	"IMWIN",
	"STATUSBAR",
};

static struct {
	const char
		*key,
		*cmd;
} conio_bind_defaultar[] = {
	{ "up",		":INPUT_SCROLL_BACK" },
	{ "down",	":INPUT_SCROLL_FORWARD" },
	{ "left",	":INPUT_CURSOR_LEFT" },
	{ "right",	":INPUT_CURSOR_RIGHT" },
	{ "tab",	":TAB_BUDDY_NEXT" },
	{ "btab",	":BUDDY_PREV" },
	{ "M-tab",	":BUDDY_PREV" },
	{ "space",	":SPACE_OR_NBSP" },
	{ "end",	":BUDDY_NEXT" },
	{ "c1",		":BUDDY_NEXT" },
	{ "home",	":BUDDY_PREV" },
	{ "a1",		":BUDDY_PREV" },
	{ "dc",		":CONN_NEXT" },
	{ "ic",		":CONN_PREV" },
	{ "backspace",	":INPUT_BACKSPACE" },
	{ "^D",		":INPUT_DELETE" },
	{ "enter",	":INPUT_ENTER" },
	{ "ppage",	":BUDDY_SCROLL_BACK" },
	{ "a3",		":BUDDY_SCROLL_BACK" },
	{ "^R",		":BUDDY_SCROLL_BACK" },
	{ "npage",	":BUDDY_SCROLL_FORWARD" },
	{ "c3",		":BUDDY_SCROLL_FORWARD" },
	{ "^Y",		":BUDDY_SCROLL_FORWARD" },
	{ "^A",		":INPUT_CURSOR_HOME" },
	{ "^E",		":INPUT_CURSOR_END" },
	{ "b2",		":INPUT_CURSOR_HOME_END" },
	{ "^P",		":INPUT_PASTE" },
	{ "<",		":INPUT_SYM_LT" },
	{ ">",		":INPUT_SYM_GT" },
	{ "&",		":INPUT_SYM_AMP" },
	{ "F1",		":STATUS_DISPLAY" },
	{ "^K",		":INPUT_KILL_EOL" },
	{ "F2",		":INPUT_ENT_BOLD" },
	{ "^B",		":INPUT_ENT_BOLD" },
	{ "F3",		":INPUT_ENT_ITALIC" },
	{ "^V",		":INPUT_ENT_ITALIC" },
	{ "F4",		":WINLIST_HIDE" },
	{ "M-w",	":WINLIST_HIDE" },
	{ "F5",		":STATUS_POKE" },
	{ "M-s",	":STATUS_POKE" },
	{ "F6",		"/eval /clear; echo Screen cleared" },
	{ "^L",		":REDRAW" },
	{ "F7",		"/jumpback" },
	{ "M-b",	"/jumpback" },
	{ "F8",		"/jump" },
	{ "^N",		"/jump" },
	{ "M-n",	"/jump" },
	{ "F9",		"/away" },
	{ "M-A",	"/away" },
	{ "F10",	"/close" },
	{ "^W",		":INPUT_KILL_WORD" },
	{ "^T",		":INPUT_KILL" },
	{ "^U",		":INPUT_KILL" },
	{ "sighup",	"/readprofile .naimprofile" },
	{ "sigusr1",	"/echo User signal 1 received (SIGUSR1)" },
	{ "sigusr2",	"/echo User signal 2 received (SIGUSR2)" },
};
static const int
	conio_bind_defaultc = sizeof(conio_bind_defaultar)/sizeof(*conio_bind_defaultar);

static void
	conio_bind_defaults(void) {
	int	i;

	for (i = 0; i < conio_bind_defaultc; i++) {
		char	buf[1024];

		snprintf(buf, sizeof(buf), "bind %s %s", conio_bind_defaultar[i].key,
			conio_bind_defaultar[i].cmd);
		conio_handlecmd(buf);
	}
}

static const char
	*conio_bind_getdef(const char *key) {
	int	i;

	for (i = 0; i < conio_bind_defaultc; i++)
		if (strcasecmp(key, conio_bind_defaultar[i].key) == 0)
			return(conio_bind_defaultar[i].cmd);
	return(NULL);
}

static struct {
	int	key;
	char	*script;
	void	(*func)(char *, int *);
} *conio_bind_ar = NULL;
static int
	conio_bind_arlen = 0;

void	*conio_bind_func(int key) {
	int	i;

	for (i = 0; i < conio_bind_arlen; i++)
		if (key == conio_bind_ar[i].key)
			return(conio_bind_ar[i].func);
	return(NULL);
}

const char	*conio_bind_get(int key) {
	int	i;

	for (i = 0; i < conio_bind_arlen; i++)
		if (key == conio_bind_ar[i].key)
			return(conio_bind_ar[i].script);
	return(NULL);
}

const char	*conio_bind_get_pretty(int key) {
	const char	*binding = conio_bind_get(key);

	if (binding == NULL)
		return(NULL);
	if (binding[0] != ':')
		return(binding);
	return(conio_key_names[atoi(binding+1)].name);
}

const char	*conio_bind_get_informative(int key) {
	static char	buf[1024];
	const char	*binding = conio_bind_get(key);
	int	i;

	if (binding == NULL)
		return(NULL);
	if (binding[0] == '/') {
		const char
			*end = strchr(binding, ' ');
		int	i;

		if (end == NULL)
			end = binding+strlen(binding);
		for (i = 0; i < cmdc; i++)
			if (strncasecmp(binding+1, cmdar[i].c, end-(binding+1)) == 0) {
				if (cmdar[i].desc == NULL)
					return(binding);
				else {
					snprintf(buf, sizeof(buf), "%-25s  %s", binding, cmdar[i].desc);
					return(buf);
				}
			}
		return(binding);
	}
	if (binding[0] != ':')
		return(binding);
	i = atoi(binding+1);
	if (*(conio_key_names[i].desc) == 0)
		return(conio_key_names[i].name);
	else {
		snprintf(buf, sizeof(buf), "%-25s  %s", conio_key_names[i].name, conio_key_names[i].desc);
		return(buf);
	}
}

void	conio_bind_set(int key, const char *script, void (*func)(char *, int *)) {
	int	i;

	for (i = 0; (i < conio_bind_arlen) && (conio_bind_ar[i].key != key); i++);
	if (i == conio_bind_arlen) {
		conio_bind_arlen++;
		conio_bind_ar = realloc(conio_bind_ar,
			conio_bind_arlen*sizeof(*conio_bind_ar));
		conio_bind_ar[i].key = key;
		conio_bind_ar[i].script = NULL;
		conio_bind_ar[i].func = NULL;
	}
	free(conio_bind_ar[i].script);
	if (script != NULL)
		conio_bind_ar[i].script = strdup(script);
	else
		conio_bind_ar[i].script = NULL;
	if (func != NULL)
		conio_bind_ar[i].func = func;
}





CONIOFUNC(jump) {
CONIODESC(Go to the specified window or the next 'active' one)
CONIOAOPT(window,winname)
	buddywin_t	*bwin = NULL;
	conn_t	*c = conn;

	if (argc == 0) {
		do {
			if ((bwin = c->curbwin) != NULL)
			  do {
				if (bwin->waiting == 1)
					break;
			  } while ((bwin = bwin->next) != c->curbwin);
			if (bwin != NULL) {
				if (bwin->waiting == 1)
					break;
				else
					bwin = NULL;
			}
		} while ((c = c->next) != conn);
	} else
		if ((bwin = bgetanywin(conn, args[0])) == NULL) {
			char	*colon;

			if ((colon = strchr(args[0], ':')) != NULL) {
				*colon++ = 0;
				do {
					if (strcasecmp(args[0], c->winname) == 0)
						break;
				} while ((c = c->next) != conn);
				if (strcasecmp(args[0], c->winname) != 0) {
					echof(conn, "JUMP", "No connection named %s.\n",
						args[0]);
					return;
				}
				if ((bwin = bgetanywin(c, colon)) == NULL) {
					echof(conn, "JUMP", "No window in %s named %s.\n",
						c->winname, colon);
					return;
				}
			} else {
				echof(conn, "JUMP", "No window in %s named %s.\n",
					conn->winname, args[0]);
				return;
			}
		}
	if ((bwin != NULL) && (bwin != curconn->curbwin)) {
		if (c != curconn)
			curconn = c;
		assert(curconn->curbwin != NULL);	/* no way for curconn->curbwin to be NULL if we found a valid bwin! */
		c->curbwin = bwin;
		scrollbackoff = 0;
		bupdate();
		nw_touchwin(&(bwin->nwin));
	}
}

CONIOFUNC(msg) {
CONIOALIA(m)
CONIOALIA(im)
CONIODESC(Send a message; as in /msg naimhelp naim is cool!)
CONIOAREQ(window,name)
CONIOAREQ(string,message)
	buddywin_t	*bwin;
	struct tm	*tmptr = NULL;
	const char	*pre = getvar(conn, "im_prefix"),
			*post = getvar(conn, "im_suffix");

	if (pre == NULL)
		pre = "";
	if (post == NULL)
		post = "";

	if (args[0] == NULL) {
		bwin = conn->curbwin;
		args[0] = bwin->winname;
	} else if ((bwin = bgetwin(conn, args[0], BUDDY)) == NULL)
		bwin = bgetwin(conn, args[0], CHAT);

	tmptr = localtime(&now);
	assert(tmptr != NULL);

	if (bwin != NULL) {
		const char	*format = NULL;

		switch (bwin->et) {
		  case CHAT:
			chat_flush(bwin);
			format = "&lt;<B>%s</B>&gt;";
			break;
		  case BUDDY:
			if (bwin->e.buddy->docrypt)
				format = "<B>%s:</B>";
			else
				format = "<B>%s</B>:";
			break;
		  case TRANSFER:
			echof(conn, "MSG", "You can't send a message to a file transfer!\n");
			return;
		}

		WINTIME(&(bwin->nwin), IMWIN);
		hwprintf(&(bwin->nwin), C(IMWIN,SELF),
			format, (conn->sn != NULL)?conn->sn:"(me)");
		hwprintf(&(bwin->nwin), C(IMWIN,TEXT),
			" %s%s%s<br>", pre, args[1], post);
	}
	if ((conn != curconn) || (conn->curbwin == NULL)
		|| (firetalk_compare_nicks(conn->conn, conn->curbwin->winname, args[0]) != FE_SUCCESS)) {
		WINTIME(&(conn->nwin), CONN);
		hwprintf(&(conn->nwin), C(CONN,SELF), "-&gt; *<B>%s</B>*", args[0]);
		hwprintf(&(conn->nwin), C(CONN,TEXT), " %s%s%s<br>", pre, args[1], post);
		naim_lastupdate(conn);
	}

	logim(conn, conn->sn, args[0], args[1]);
	naim_send_im(conn, args[0], args[1], 0);
}

CONIOFUNC(addbuddy) {
CONIOALIA(add)
CONIOALIA(friend)
CONIOALIA(groupbuddy)
CONIODESC(Add someone to your buddy list or change their group membership)
CONIOAREQ(buddy,account)
CONIOAOPT(string,group)
CONIOAOPT(string,realname)
	buddylist_t	*blist = rgetlist(conn, args[0]);
	const char	*group = "Buddy", *name = NULL;
	int	already = blist?1:0,
		changed = 0, perm = 1;

	if ((blist != NULL) && !USER_PERMANENT(blist))
		perm = 0;

	switch (argc) {
	  default:
		name = args[2];
	  case 2:
		group = args[1];
	  case 1:
		break;
	}

	/* matching groups must be *exact* matches or AOL rejects the add */
	if (conn->buddyar != NULL) {
		buddylist_t	*iter;

		for (iter = conn->buddyar; iter != NULL; iter = iter->next)
			if (firetalk_compare_nicks(conn->conn, iter->_group, group) == FE_SUCCESS) {
				if (strcmp(group, iter->_group) != 0)
					group = iter->_group;
				break;
			}
	}

	blist = raddbuddy(conn, args[0], group, name);
	assert(blist != NULL);

	if (argc == 1) {
		if (blist->_name != NULL) {
			echof(conn, NULL, "Removed <font color=\"#00FFFF\">%s</font>'s name (was \"<font color=\"#00FFFF\">%s</font>\").\n",
				blist->_account, blist->_name);
			free(blist->_name);
			blist->_name = NULL;
			changed = 1;
		}
	} else if (argc == 2) {
		if ((blist->_group == NULL) || (group == NULL) || (strcmp(blist->_group, group) != 0)) {
			if (group == NULL) {
				free(blist->_group);
				blist->_group = NULL;
			} else
				STRREPLACE(blist->_group, group);
			echof(conn, NULL, "Changed <font color=\"#00FFFF\">%s</font>'s group to \"<font color=\"#FF0000\">%s</font>\".\n",
				USER_ACCOUNT(blist), USER_GROUP(blist));
			changed = 1;
		}
	} else {
		if ((blist->_name == NULL) || (strcmp(blist->_name, args[2]) != 0)) {
			echof(conn, NULL, "Changed <font color=\"#00FFFF\">%s</font>'s name from \"<font color=\"#00FFFF\">%s</font>\" to \"<font color=\"#00FFFF\">%s</font>\".\n",
				USER_ACCOUNT(blist), USER_NAME(blist), args[2]);
			STRREPLACE(blist->_name, args[2]);
			changed = 1;
		}
	}

	if (USER_PERMANENT(blist)) {
		if ((changed == 0) || (perm == 0)) {
			if (already)
				echof(conn, NULL, "<font color=\"#00FFFF\">%s</font> <font color=\"#800000\">[<B>%s</B>]</font> is now a permanent buddy.\n",
					user_name(NULL, 0, conn, blist), USER_GROUP(blist));
			else
				echof(conn, NULL, "Added <font color=\"#00FFFF\">%s</font> <font color=\"#800000\">[<B>%s</B>]</font> to your permanent buddy list.\n",
					user_name(NULL, 0, conn, blist), USER_GROUP(blist));
		}
	} else {
		if ((changed == 0) || (perm == 1)) {
			if (already)
				echof(conn, NULL, "<font color=\"#00FFFF\">%s</font> <font color=\"#800000\">[<B>%s</B>]</font> is now a non-permanent buddy.\n",
					user_name(NULL, 0, conn, blist), USER_GROUP(blist));
			else
				echof(conn, NULL, "Added <font color=\"#00FFFF\">%s</font> <font color=\"#800000\">[<B>%s</B>]</font> to your non-permanent buddy list.\n",
					user_name(NULL, 0, conn, blist), USER_GROUP(blist));
		}
	}

	{
		fte_t	ret;

		if ((ret = firetalk_im_add_buddy(conn->conn, args[0], USER_GROUP(blist), blist->_name)) != FE_SUCCESS)
			echof(conn, "ADDBUDDY", "Unable to add buddy: %s.\n", firetalk_strerror(ret));
	}
}

static void do_delconn(conn_t *conn) {
	bclearall(conn, 1);

	firetalk_disconnect(conn->conn);
	firetalk_destroy_handle(conn->conn);
	conn->conn = NULL;

	if (conn->next == conn) {
		assert(conn == curconn);
		curconn = NULL;
	} else {
		conn_t	*prev;

		for (prev = conn->next; prev->next != conn; prev = prev->next)
			;
		prev->next = conn->next;

		if (curconn == conn)
			curconn = conn->next;
		conn->next = conn;
	}

	FREESTR(conn->sn);
	FREESTR(conn->password);
	FREESTR(conn->winname);
	FREESTR(conn->server);
	FREESTR(conn->profile);

	if (conn->logfile != NULL) {
		fclose(conn->logfile);
		conn->logfile = NULL;
	}

	nw_delwin(&(conn->nwin));

	assert(conn->buddyar == NULL);
//	assert(conn->idiotar == NULL);
	assert(conn->curbwin == NULL);

	free(conn);
}

CONIOFUNC(exit) {
CONIOALIA(quit)
CONIODESC(Disconnect and exit naim)
	conn_t	*c;

	if (secs_getvar_int("autosave") != 0)
		conio_save(conn, 0, NULL);

	c = conn;
	do {
		firetalk_disconnect(c->conn);
	} while ((c = c->next) != conn);

	while (curconn != NULL) {
		do_delconn(curconn);
		statrefresh();
	}
	stayconnected = 0;
}

CONIOFUNC(save) {
CONIODESC(Write current settings to ~/.naimrc to be loaded at startup)
CONIOAOPT(string,filename)
	conn_t	*c = conn;
	FILE	*file;
	int	i;
	extern rc_var_s_t
		rc_var_s_ar[];
	extern const int
		rc_var_s_c;
	extern rc_var_i_t
		rc_var_i_ar[];
	extern const int
		rc_var_i_c;
	extern rc_var_i_t
		rc_var_b_ar[];
	extern const int
		rc_var_b_c;
	extern char
		naimrcfilename[];
	const char
		*filename;

	if (argc == 0)
		filename = naimrcfilename;
	else
		filename = args[0];

	if ((file = fopen(filename, "w")) == NULL) {
		echof(conn, "SAVE", "Error %i opening file for write: %s",
			errno, strerror(errno));
		return;
	}

	fchmod(fileno(file), 0600);

	fprintf(file,
"# naim configuration file.\n"
"# This file was automatically generated by using the /save command from within\n"
"# naim. Feel free to modify it with any text editor.\n"
"# \n"
"# Lines beginning with a # are ignored by naim, so I will use those to document\n"
"# each line I write out. In most cases I will attempt to skip over writing out\n"
"# redundant information, such as when your settings matched the defaults, but\n"
"# in situations where I do include such information I will also place a # in\n"
"# front, to show you what the default value is without actually setting it.\n"
"# \n"
"# The structure of this file is fairly simple: Lines that start with a # are\n"
"# ignored, lines that don't are treated as if you had typed them when first\n"
"# starting naim (so anything that can go after a forward slash while running\n"
"# naim is fair game for inclusion here).\n"
"# \n"
"# If you have any questions about this file, or naim in general, feel free to\n"
"# contact Daniel Reed <n@ml.org>, or visit:\n"
"# 	http://naim.n.ml.org/\n"
"\n");

	fprintf(file, "# The window list can either be always visible, always hidden, or auto-hidden.\n");
	if (changetime == -1)
		fprintf(file, "WINLIST Hidden\n\n");
	else if (changetime == 0)
		fprintf(file, "WINLIST Visible\n\n");
	else
		fprintf(file, "#WINLIST Auto\n\n");

	for (i = 0; i < rc_var_s_c; i++) {
		const char
			*glob = secs_getvar(rc_var_s_ar[i].var),
			*def = rc_var_s_ar[i].val,
			*use, *cm, *q;

		if (glob != NULL) {
			use = glob;
			if (strcmp(glob, def) == 0)
				cm = "#";
			else
				cm = "";
		} else if (def != NULL) {
			use = def;
			cm = "#";
		} else {
			use = "";
			cm = "";
		}

		q = ((*use == 0) || ((strchr(use, ' ') != NULL))?"\"":"");

		if (rc_var_s_ar[i].desc != NULL)
			fprintf(file, "# %s (%s)\n", rc_var_s_ar[i].var, rc_var_s_ar[i].desc);
		else
			fprintf(file, "# %s\n", rc_var_s_ar[i].var);
		fprintf(file, "%sset %s %s%s%s\n\n", cm, rc_var_s_ar[i].var, q, use, q);
	}

	for (i = 0; i < rc_var_i_c; i++) {
		const char
			*glob = secs_getvar(rc_var_i_ar[i].var),
			*cm;
		const int
			globi = atoi(glob),
			defi = rc_var_i_ar[i].val;
		int	usei;

		if (glob != NULL) {
			usei = globi;
			if (globi == defi)
				cm = "#";
			else
				cm = "";
		} else {
			usei = defi;
			cm = "#";
		}

		if (rc_var_i_ar[i].desc != NULL)
			fprintf(file, "# %s (%s)\n", rc_var_i_ar[i].var, rc_var_i_ar[i].desc);
		else
			fprintf(file, "# %s\n", rc_var_i_ar[i].var);
		fprintf(file, "%sset %s %i\n\n", cm, rc_var_i_ar[i].var, usei);
	}

	for (i = 0; i < rc_var_b_c; i++) {
		const char
			*glob = secs_getvar(rc_var_b_ar[i].var),
			*cm;
		const int
			globi = atoi(glob),
			defi = rc_var_b_ar[i].val;
		int	usei;

		if (glob != NULL) {
			usei = globi;
			if (globi == defi)
				cm = "#";
			else
				cm = "";
		} else {
			usei = defi;
			cm = "#";
		}

		if (rc_var_b_ar[i].desc != NULL)
			fprintf(file, "# %s (%s)\n", rc_var_b_ar[i].var, rc_var_b_ar[i].desc);
		else
			fprintf(file, "# %s\n", rc_var_b_ar[i].var);
		fprintf(file, "%sset %s %i\n\n", cm, rc_var_b_ar[i].var, usei);
	}

	{
		faimconf_t	fc;

		rc_initdefs(&fc);

		fprintf(file, "# Colors\n");
		for (i = 0; i < sizeof(forelist)/sizeof(*forelist); i++)
			fprintf(file, "%ssetcol %s %s%s\n", (fc.f[i] == faimconf.f[i])?"#":"",
				forelist[i], collist[faimconf.f[i]%(nw_COLORS*nw_COLORS)],
				(faimconf.f[i]>=2*(nw_COLORS*nw_COLORS))?" BOLD":(faimconf.f[i]>=(nw_COLORS*nw_COLORS))?" DULL":"");
		for (i = 0; i < sizeof(backlist)/sizeof(*backlist); i++)
			fprintf(file, "%ssetcol %s %s\n", (fc.b[i] == faimconf.b[i])?"#":"",
				backlist[i], collist[faimconf.b[i]]);
		fprintf(file, "\n");
	}

	{
		const char	*binding, *def;
		char	key[10];
		int	base;

		fprintf(file, "# Key bindings.\n");
		for (base = 0; base <= KEY_MAX; base += KEY_MAX) {
			for (i = 0; i < sizeof(conio_bind_names)/sizeof(*conio_bind_names); i++)
				if ((binding = conio_bind_get_pretty(base+conio_bind_names[i].code)) != NULL) {
					snprintf(key, sizeof(key), "%s%s", (base?"M-":""), conio_bind_names[i].name);

					if (((def = conio_bind_getdef(key)) == NULL)
						|| (strcmp(def, binding) != 0))
						fprintf(file, "bind %s %s\n", key, binding);
				}
			for (i = 1; i <= 12; i++)
				if ((binding = conio_bind_get_pretty(base+KEY_F(i))) != NULL) {
					snprintf(key, sizeof(key), "%sF%i", (base?"M-":""), i);

					if (((def = conio_bind_getdef(key)) == NULL)
						|| (strcmp(def, binding) != 0))
						fprintf(file, "bind %s %s\n", key, binding);
				}
			for (i = 'A'; i <= 'Z'; i++)
				if ((binding = conio_bind_get_pretty(base+CTRL(i))) != NULL) {
					snprintf(key, sizeof(key), "%s^%c", (base?"M-":""), i);

					if (((def = conio_bind_getdef(key)) == NULL)
						|| (strcmp(def, binding) != 0))
						fprintf(file, "bind %s %s\n", key, binding);
				}
		}
		for (i = 'a'; i <= 'z'; i++)
			if ((binding = conio_bind_get_pretty(KEY_MAX+i)) != NULL) {
				char	key[4] = { 'M', '-', i, 0 };

				if (((def = conio_bind_getdef(key)) == NULL)
					|| (strcmp(def, binding) != 0))
					fprintf(file, "bind %s %s\n", key, binding);
			}
		fprintf(file, "\n");
	}

	if (awaytime > 0)
		fprintf(file, "# You were away when you /saved.\nAWAY\n\n");

	{
		extern script_t
			*scriptar;
		extern int
			scriptc;
		int	i;

		if (scriptc > 0) {
			fprintf(file, "# Aliases.\n");
			for (i = 0; i < scriptc; i++)
				fprintf(file, "ALIAS %s %s\n", scriptar[i].name, scriptar[i].script);
			fprintf(file, "\n");
		}
	}

	{
		extern html_clean_t
			*html_cleanar;
		extern int
			html_cleanc;
		int	i;

		fprintf(file, "# Filters.\n");
		fprintf(file, "FILTER REPLACE :FLUSH\n");
		for (i = 0; i < html_cleanc; i++)
			if (*html_cleanar[i].from != 0)
				fprintf(file, "FILTER REPLACE %s %s\n",
					html_cleanar[i].from, html_cleanar[i].replace);
		fprintf(file, "\n");
	}

	fprintf(file, "\n");

	do {
		fprintf(file,
"\n"
"\n"
"# Create a new connection called %s of type %s.\n"
"newconn %s %s\n"
"\n",				c->winname, firetalk_strprotocol(c->proto),
				c->winname, firetalk_strprotocol(c->proto));

		fprintf(file,
"# naim will automatically attempt to read your profile from\n"
"# ~/.naimprofile, though if you prefer you can specify one\n"
"# to replace it here.\n"
"#%s:readprofile %s/.naimprofile\n"
"\n",				c->winname, home);

		{
			buddylist_t	*blist = c->buddyar;

			if (blist != NULL) {
			  fprintf(file, "# Add your buddies.\n");
			  do {
			    if (USER_PERMANENT(blist)) {
				char	*b = USER_ACCOUNT(blist),
 					*g = USER_GROUP(blist),
					*n = (blist->_name != NULL)?blist->_name:"",
					*bq = (strchr(b, ' ') != NULL)?"\"":"",
					*gq = ((*g == 0)
						|| (strchr(g, ' ') != NULL))?"\"":"";

				fprintf(file,
"%s:addbuddy\t%s%s%s\t%s%s%s\t%s\n",
					c->winname, bq, b, bq, gq, g, gq, n);
			    }
			  } while ((blist = blist->next) != NULL);
			  fprintf(file, "\n");
			}
		}

		{
			ignorelist_t	*blist = c->idiotar;

			if (blist != NULL) {
			  fprintf(file, "# Add your ignore list.\n");
			  do {
				char	*b = blist->screenname,
					*n = (blist->notes != NULL)?blist->notes:"",
					*bq = (strchr(b, ' ') != NULL)?"\"":"";

				if (strcasecmp(n, "block") == 0)
					fprintf(file,
"%s:block\t%s\n",
					c->winname, b);
				else
					fprintf(file,
"%s:ignore\t%s%s%s\t%s\n",
					c->winname, bq, b, bq, n);
			  } while ((blist = blist->next) != NULL);
			  fprintf(file, "\n");
			}
		}

		{
			buddywin_t	*bwin = c->curbwin;

			if (bwin != NULL) {
			  fprintf(file, "# Rejoin your channels.\n");
			  do {
				if ((bwin->et == CHAT) && (*(bwin->winname) != ':'))
					fprintf(file,
"%s:join %s%s%s%s%s\n",
						c->winname,
						strchr(bwin->winname, ' ')?"\"":"", bwin->winname, strchr(bwin->winname, ' ')?"\"":"",
						bwin->e.chat->key?" ":"", bwin->e.chat->key?bwin->e.chat->key:"");
				else if (bwin->pouncec > 0) {
					int	i;

					fprintf(file,
"%s:open %s\n",
						c->winname, bwin->winname);
					for (i = 0; i < bwin->pouncec; i++)
						fprintf(file,
"%s:msg \"%s\" %s\n",
							c->winname, bwin->winname, bwin->pouncear[i]);
				}
			  } while ((bwin = bwin->next) != c->curbwin);
			  fprintf(file, "\n");
			}
		}

		{
			int	log = getvar_int(c, "log");

			fprintf(file,
"# Logging\n"
"%sset %s:log\t%i\n"
"%sset %s:logfile\t%s%s%s%s\n"
"\n",				log?"":"#", c->winname, log,
				log?"":"#", c->winname,
					log?getvar(c, "logfile"):home,
					log?"":"/",
					log?"":c->winname,
					log?"":".log");
		}

		for (i = 0; i < rc_var_s_c; i++) {
			const char
				*loc = getvar(c, rc_var_s_ar[i].var),
				*glob = secs_getvar(rc_var_s_ar[i].var),
				*q;

			if (loc != glob) {
				q = ((*loc == 0) || ((strchr(loc, ' ') != NULL))?"\"":"");

				if (rc_var_s_ar[i].desc != NULL)
					fprintf(file, "# %s-specific %s (see above)\n", c->winname, 
						rc_var_s_ar[i].var);
				else
					fprintf(file, "# %s-specific %s\n", c->winname,
						rc_var_s_ar[i].var);
				fprintf(file, "set %s:%s %s%s%s\n\n", c->winname,
					rc_var_s_ar[i].var, q, loc, q);
			}
		}

		for (i = 0; i < rc_var_i_c; i++) {
			const char
				*loc = getvar(c, rc_var_i_ar[i].var),
				*glob = secs_getvar(rc_var_i_ar[i].var);

			if (loc != glob) {
				if (rc_var_i_ar[i].desc != NULL)
					fprintf(file, "# %s-specific %s (see above)\n", c->winname,
						rc_var_i_ar[i].var);
				else
					fprintf(file, "# %s-specific %s\n", c->winname,
						rc_var_i_ar[i].var);
				fprintf(file, "set %s:%s %s\n\n", c->winname,
					rc_var_i_ar[i].var, loc);
			}
		}

		for (i = 0; i < rc_var_b_c; i++) {
			const char
				*loc = getvar(c, rc_var_b_ar[i].var),
				*glob = secs_getvar(rc_var_b_ar[i].var);

			if (loc != glob) {
				if (rc_var_b_ar[i].desc != NULL)
					fprintf(file, "# %s-specific %s (see above)\n", c->winname,
						rc_var_b_ar[i].var);
				else
					fprintf(file, "# %s-specific %s\n", c->winname,
						rc_var_b_ar[i].var);
				fprintf(file, "set %s:%s %s\n\n", c->winname,
					rc_var_b_ar[i].var, loc);
			}
		}

		{
			char	*name = c->sn,
				*nameq = ((name == NULL) || (strchr(name, ' ') != NULL))?"\"":"",
				*pass = getvar(c, "password");

			fprintf(file, "# Connect to %s.\n", c->winname);
			if (pass != NULL) {
				char	*passq = (strchr(pass, ' ') != NULL)?"\"":"";

				fprintf(file, "set %s:password %s%s%s\nclear\n",
					c->winname, passq, pass, passq);
			} else
				fprintf(file,	"#set %s:password mypass\n"
						"#clear\n"
						"# naim will prompt you for your password if it's not supplied here.\n",
					c->winname);
			fprintf(file, "%s%s:connect %s%s%s", ((name != NULL) && (conn->online > 0))?"":"#",
				c->winname, nameq, name?name:"myname", 
				nameq);
			if (c->server != NULL) {
				fprintf(file, " %s", c->server);
				if (c->port > 0)
					fprintf(file, ":%i", c->port);
			}
		}

		fprintf(file, "\n");
	} while ((c = c->next) != conn);

	fclose(file);
	echof(conn, NULL, "Settings saved to %s.\n", filename);
}

CONIOFUNC(connect) {
CONIODESC(Connect to a service)
CONIOAOPT(string,name)
CONIOAOPT(string,server)
CONIOAOPT(int,port)
	fte_t	fte;

	if (conn->online > 0) {
		echof(conn, "CONNECT", "Please <font color=\"#00FF00\">/%s:disconnect</font> first (just so there's no confusion).\n",
			conn->winname);
		return;
	}

	if ((argc == 1) && (strchr(args[0], '.') != NULL) && (strchr(args[0], '@') == NULL)) {
		if (conn->sn != NULL) {
			echof(conn, "CONNECT", "It looks like you're specifying a server, so I'll use %s as your name.\n",
				conn->sn);
			args[1] = args[0];
			args[0] = conn->sn;
			argc = 2;
		} else {
			echof(conn, "CONNECT", "It looks like you're specifying a server, but I don't have a default name to use.\n");
			echof(conn, "CONNECT", "Please specify a name to use (<font color=\"#00FF00\">/%s:connect myname %s</font>).\n",
				conn->winname, args[0]);
			return;
		}
	}

	switch(argc) {
	  case 3:
		conn->port = atoi(args[2]);
	  case 2: {
		char	*tmp;

		if ((tmp = strchr(args[1], ':')) != NULL) {
			conn->port = atoi(tmp+1);
			*tmp = 0;
		}
		STRREPLACE(conn->server, args[1]);
	  }
	  case 1:
		if ((conn->sn == NULL) || ((conn->sn != args[0]) && (strcmp(conn->sn, args[0]) != 0)))
			STRREPLACE(conn->sn, args[0]);
	}

	if (conn->sn == NULL) {
		echof(conn, "CONNECT", "Please specify a name to use (<font color=\"#00FF00\">/%s:connect myname</font>).\n",
			conn->winname);
		return;
	}

	if (conn->port == 0) {
		if (conn->server == NULL)
			echof(conn, NULL, "Connecting to %s.\n",
				firetalk_strprotocol(conn->proto));
		else
			echof(conn, NULL, "Connecting to %s on server %s.\n",
				firetalk_strprotocol(conn->proto), conn->server);
	} else {
		if (conn->server == NULL)
			echof(conn, NULL, "Connecting to %s on port %i.\n",
				firetalk_strprotocol(conn->proto), conn->port);
		else
			echof(conn, NULL, "Connecting to %s on port %i of server %s.\n",
				firetalk_strprotocol(conn->proto), conn->port, conn->server);
	}

	statrefresh();

#ifdef HAVE_HSTRERROR
	h_errno = 0;
#endif
	errno = 0;
	switch ((fte = firetalk_signon(conn->conn, conn->server, conn->port, conn->sn))) {
	  case FE_SUCCESS:
		break;
	  case FE_CONNECT:
#ifdef HAVE_HSTRERROR
		if (h_errno != 0)
			echof(conn, "CONNECT", "Unable to connect: %s (%s).\n",
				firetalk_strerror(firetalkerror), hstrerror(h_errno));
		else
#endif
		if (errno != 0)
			echof(conn, "CONNECT", "Unable to connect: %s (%s).\n",
				firetalk_strerror(firetalkerror), strerror(errno));
		else
			echof(conn, "CONNECT", "Unable to connect: %s.\n",
				firetalk_strerror(firetalkerror));
		break;
	  default:
		echof(conn, "CONNECT", "Connection failed in startup, %s.\n",
			firetalk_strerror(fte));
		break;
	}
}

CONIOFUNC(jumpback) {
CONIODESC(Go to the previous window)
	conn_t	*c = conn,
		*newestconn = NULL;
	buddywin_t *newestbwin = NULL;
	double	newestviewtime = 0;

	do {
		buddywin_t	*bwin;

		if ((bwin = c->curbwin) != NULL)
		  do {
			if (((c != conn) || (bwin != conn->curbwin)) && (bwin->viewtime > newestviewtime)) {
				newestconn = c;
				newestbwin = bwin;
				newestviewtime = bwin->viewtime;
			}
		  } while ((bwin = bwin->next) != c->curbwin);
	} while ((c = c->next) != conn);

	if (newestconn != NULL) {
		assert(newestbwin != NULL);
		if (newestconn != curconn)
			curconn = newestconn;
		assert(curconn->curbwin != NULL);	/* no way for curconn->curbwin to be NULL if we found a valid bwin! */
		newestconn->curbwin = newestbwin;
		scrollbackoff = 0;
		bupdate();
		nw_touchwin(&(newestbwin->nwin));
	}
}

CONIOFUNC(info) {
CONIOALIA(whois)
CONIOALIA(wi)
CONIODESC(Retrieve a user profile)
CONIOAOPT(entity,name)
	fte_t	ret;

	if (argc == 0) {
		if (!inconn || (conn->curbwin->et != BUDDY)) {
			if ((ret = firetalk_im_get_info(conn->conn, conn->sn)) != FE_SUCCESS)
				echof(conn, "INFO", "Unable to retrieve user information for %s: %s.\n",
					conn->sn, firetalk_strerror(ret));
		} else {
			if ((ret = firetalk_im_get_info(conn->conn, conn->curbwin->winname)) != FE_SUCCESS)
				echof(conn, "INFO", "Unable to retrieve user information for %s: %s.\n",
					conn->curbwin->winname, firetalk_strerror(ret));
		}
	} else {
		if ((ret = firetalk_im_get_info(conn->conn, args[0])) != FE_SUCCESS)
			echof(conn, "INFO", "Unable to retrieve user information for %s: %s.\n",
				args[0], firetalk_strerror(ret));
	}
}

CONIOFUNC(eval) {
CONIODESC(Evaluate a command with $-variable substitution)
CONIOAREQ(string,script)
	secs_script_parse(args[0]);
}

CONIOFUNC(say) {
CONIODESC(Send a message to the current window; as in /say I am happy)
CONIOAREQ(string,message)
CONIOWHER(NOTSTATUS)
	const char
		*newargs[2] = { conn->curbwin->winname, args[0] };

	conio_msg(conn, 2, newargs);
}

CONIOFUNC(me) {
CONIODESC(Send an 'action' message to the current window; as in /me is happy)
CONIOAREQ(string,message)
CONIOWHER(NOTSTATUS)
	WINTIME(&(conn->curbwin->nwin), IMWIN);
	hwprintf(&(conn->curbwin->nwin), C(IMWIN,SELF), "* <B>%s</B>", conn->sn);
	hwprintf(&(conn->curbwin->nwin), C(IMWIN,TEXT), "%s%s<br>", (strncmp(args[0], "'s ", 3) == 0)?"":" ", args[0]);
	logim(conn, conn->sn, conn->curbwin->winname, args[0]);
	naim_send_act(conn, conn->curbwin->winname, args[0]);
}

CONIOFUNC(open) {
CONIOALIA(window)
CONIODESC(Open a query window)
CONIOAREQ(buddy,name)
	buddywin_t	*bwin;

	if ((bwin = bgetanywin(conn, args[0])) != NULL)
		conio_jump(conn, argc, args);
	else {
		buddylist_t	*blist;
		int	added;

		blist = rgetlist(conn, args[0]);
		if (blist == NULL) {
			blist = raddbuddy(conn, args[0], DEFAULT_GROUP, NULL);
			firetalk_im_add_buddy(conn->conn, args[0], USER_GROUP(blist), NULL);
			added = 1;
		} else
			added = 0;

		bnewwin(conn, USER_ACCOUNT(blist), BUDDY);
		bwin = bgetwin(conn, USER_ACCOUNT(blist), BUDDY);
		assert(bwin != NULL);
		if (added)
			window_echof(bwin, "Query window created and user added as a temporary buddy.\n");
		else
			window_echof(bwin, "Query window created.\n");
		conn->curbwin = bwin;
		nw_touchwin(&(bwin->nwin));
		scrollbackoff = 0;
		naim_changetime();
		bupdate();
	}
}

CONIOFUNC(close) {
CONIOALIA(endwin)
CONIOALIA(part)
CONIODESC(Close a query window or leave a discussion)
CONIOAOPT(window,winname)
CONIOWHER(NOTSTATUS)
	buddywin_t	*bwin;

	if (argc == 1) {
		if ((bwin = bgetanywin(conn, args[0])) == NULL) {
			echof(conn, "CLOSE", "No window is open for <font color=\"#00FFFF\">%s</font>.\n",
				args[0]);
			return;
		}
	} else
		bwin = conn->curbwin;

	bclose(conn, bwin, 0);
	bwin = NULL;
}

CONIOFUNC(closeall) {
CONIODESC(Close stale windows for offline buddies)
	int	i, l;
	buddywin_t	*bwin;

	if (conn->curbwin == NULL)
		return;

	l = 0;
	bwin = conn->curbwin;
	do {
		l++;
	} while ((bwin = bwin->next) != conn->curbwin);
	for (i = 0; i < l; i++) {
		buddywin_t
			*bnext = bwin->next;
		const char
			*_args[1];

		_args[0] = bwin->winname;
		if ((bwin->et == BUDDY) && (bwin->e.buddy->offline > 0) && (bwin->pouncec == 0))
			conio_close(conn, 1, _args);
		else if ((bwin->et == CHAT) && (bwin->e.chat->offline > 0))
			conio_close(conn, 1, _args);
		bwin = bnext;
	}
}

CONIOFUNC(ctcp) {
CONIODESC(Send Client To Client Protocol request to someone)
CONIOAREQ(window,name)
CONIOAOPT(string,requestname)
CONIOAOPT(string,message)
	if (argc == 1)
		firetalk_subcode_send_request(conn->conn, args[0],
			"VERSION", NULL);
	else if (argc == 2)
		firetalk_subcode_send_request(conn->conn, args[0],
			args[1], NULL);
	else
		firetalk_subcode_send_request(conn->conn, args[0],
			args[1], args[2]);
}

CONIOFUNC(clear) {
CONIODESC(Temporarily blank the scrollback for the current window)
	win_t	*win;
	int	i;

	if (inconn) {
		assert(conn->curbwin != NULL);
		win = &(conn->curbwin->nwin);
	} else
		win = &(conn->nwin);

	nw_erase(win);
	for (i = 0; i < faimconf.wstatus.pady; i++)
		nw_printf(win, 0, 0, "\n");
}

CONIOFUNC(clearall) {
CONIODESC(Perform a /clear on all open windows)
	conn_t	*c = conn;

	do {
		buddywin_t	*bwin = c->curbwin;

		if (bwin != NULL)
			do {
				int	i;

				nw_erase(&(bwin->nwin));
				for (i = 0; i < faimconf.wstatus.pady; i++)
					nw_printf(&(bwin->nwin), 0, 0, "\n");
				bwin->nwin.dirty = 0;
			} while ((bwin = bwin->next) != c->curbwin);
	} while ((c = c->next) != conn);
}

CONIOFUNC(load) {
CONIODESC(Load a command file (such as .naimrc))
CONIOAREQ(filename,filename)
	naim_read_config(args[0]);
}

CONIOFUNC(away) {
CONIODESC(Set or unset away status)
CONIOAOPT(string,message)
	if (argc == 0) {
		if (awaytime > 0)
			unsetaway();
		else
			setaway(0);
	} else {
		secs_setvar("awaymsg", secs_script_expand(NULL, args[0]));
		setaway(0);
	}
}

static int qsort_strcasecmp(const void *p1, const void *p2) {
	register char
		**b1 = (char **)p1,
		**b2 = (char **)p2;

	return(strcasecmp(*b1, *b2));
}

CONIOFUNC(names) {
CONIOALIA(buddylist)
CONIODESC(Display buddy list or members of a chat)
CONIOAOPT(chat,chat)
	const char	*chat;

	if ((argc == 0) || (strcasecmp(args[0], "ON") == 0) || (strcasecmp(args[0], "OFF") == 0)) {
		int	showon = 1, showoff = 1;

		if ((argc > 0) && (strcasecmp(args[0], "ON") == 0))
			showoff = 0;
		else if ((argc > 0) && (strcasecmp(args[0], "OFF") == 0))
			showon = 0;

		if (!inconn || !showon || !showoff || (conn->curbwin->et != CHAT) || (*(conn->curbwin->winname) == ':')) {
			buddylist_t	*blist;
			int	maxname = strlen("Account"), maxgroup = strlen("Group"), maxnotes = strlen("Name"), max;
			char	*spaces;

			if (conn->buddyar == NULL) {
				echof(conn, NULL, "Your buddy list is empty, try <font color=\"#00FF00\">/addbuddy buddyname</font>.\n");
				return;
			}

			echof(conn, NULL, "Buddy list:");

			blist = conn->buddyar;
			do {
				const char	*name = USER_ACCOUNT(blist),
						*nameq = ((*name == 0) || strchr(name, ' '))?"\"":"",
						*group = USER_GROUP(blist),
						*groupq = ((*group == 0) || strchr(group, ' '))?"\"":"",
						*notes = USER_NAME(blist),
						*notesq = ((*notes == 0) || strchr(notes, ' '))?"\"":"";
				int		namelen = strlen(name)+2*strlen(nameq),
						grouplen = strlen(group)+2*strlen(groupq),
						noteslen = strlen(notes)+2*strlen(notesq);

				if (namelen > maxname)
					maxname = namelen;
				if (grouplen > maxgroup)
					maxgroup = grouplen;
				if (noteslen > maxnotes)
					maxnotes = noteslen;
			} while ((blist = blist->next) != NULL);

			if (maxname > maxgroup)
				max = maxname;
			else
				max = maxgroup;
			if (maxnotes > max)
				max = maxnotes;

			if ((spaces = malloc(max*6 + 1)) == NULL)
				return;
			*spaces = 0;
			while (max > 0) {
				strcat(spaces, "&nbsp;");
				max--;
			}

			echof(conn, NULL, "</B>&nbsp; %s"
				" <font color=\"#800000\">%s<B>%s</B>%s</font>%.*s"
				" <font color=\"#008000\">%s<B>%s</B>%s</font>%.*s"
				" <font color=\"#000080\">%s<B>%s</B>%s</font>%.*s<B>%s\n",
				"&nbsp; &nbsp;",
				"", "Group", "",
				6*(maxgroup - strlen("Group")), spaces,
				"", "Account", "",
				6*(maxname - strlen("Account")), spaces,
				"", "Name", "",
				6*(maxnotes - strlen("Name")), spaces,
				" flags");

			blist = conn->buddyar;
			do {
				const char	*name = USER_ACCOUNT(blist),
						*nameq = ((*name == 0) || strchr(name, ' '))?"\"":"",
						*group = USER_GROUP(blist),
						*groupq = ((*group == 0) || strchr(group, ' '))?"\"":"",
						*notes = USER_NAME(blist),
						*notesq = ((*notes == 0) || strchr(notes, ' '))?"\"":"";
				int		namelen = strlen(name)+2*strlen(nameq),
						grouplen = strlen(group)+2*strlen(groupq),
						noteslen = strlen(notes)+2*strlen(notesq);

				if ((blist->offline == 0) && !showon)
					continue;
				else if ((blist->offline != 0) && !showoff)
					continue;
				echof(conn, NULL, "</B>&nbsp; %s"
					" <font color=\"#800000\">%s<B>%s</B>%s</font>%.*s"
					" <font color=\"#008000\">%s<B>%s</B>%s</font>%.*s"
					" <font color=\"#000080\">%s<B>%s</B>%s</font>%.*s<B>%s%s%s%s%s%s%s\n",
					blist->offline?"OFF":"<B>ON</B>&nbsp;",
					groupq, group, groupq,
					6*(maxgroup - grouplen), spaces,
					nameq, name, nameq,
					6*(maxname - namelen), spaces,
					notesq, notes, notesq,
					6*(maxnotes - noteslen), spaces,

					USER_PERMANENT(blist)?"":" NON-PERMANENT",
					blist->crypt?" CRYPT":"",
					blist->tzname?" TZNAME":"",
					blist->tag?" TAGGED":"",
					blist->isaway?" AWAY":"",
					blist->isidle?" IDLE":"",
					blist->warnval?" WARNED":"",
					blist->typing?" TYPING":"",
					(blist->peer > 0)?" PEER":"");
			} while ((blist = blist->next) != NULL);
			free(spaces);
			spaces = NULL;
			echof(conn, NULL, "Use the <font color=\"#00FF00\">/namebuddy</font> command to change a buddy's name, or <font color=\"#00FF00\">/groupbuddy</font> to change a buddy's group.");
			return;
		}
		chat = conn->curbwin->winname;
	} else
		chat = args[0];

	firetalk_chat_listmembers(conn->conn, chat);
	{
		buddywin_t	*bwin = cgetwin(conn, chat);

		assert(bwin != NULL);
		if (names == NULL)
			window_echof(bwin, "Nobody on %s\n", chat);
		else {
			int	i, namesbuflen = 0;
			char	*namesbuf = NULL;

			qsort(names, namec, sizeof(*names), qsort_strcasecmp);
			for (i = 0; i < namec; i++) {
				int	len = strlen(names[i])+1;

				namesbuf = realloc(namesbuf, (namesbuflen+len+1)*sizeof(*namesbuf));
				sprintf(namesbuf+namesbuflen, "%s ", names[i]);
				namesbuflen += len;
				free(names[i]);
				names[i] = NULL;
			}
			free(names);
			names = NULL;
			namec = 0;
			window_echof(bwin, "Users on %s: %s\n", chat, namesbuf);
			free(namesbuf);
		}
	}
}

CONIOFUNC(join) {
CONIODESC(Participate in a chat)
CONIOAREQ(string,chat)
CONIOAOPT(string,key)
	buddywin_t	*cwin;

	if (((cwin = bgetwin(conn, firetalk_chat_normalize(conn->conn, args[0]), CHAT)) == NULL) || (cwin->e.chat->offline != 0)) {
		char	buf[1024];

		cwin = cgetwin(conn, args[0]);
		if (argc > 1) {
			window_echof(cwin, "Entering chat \"%s\" with a key of \"%s\"; if you intended to join chat \"%s %s\", please use <font color=\"#00FF00\">/join \"%s %s\"</font> in the future.\n",
				args[0], args[1], args[0], args[1], args[0], args[1]);
			STRREPLACE(cwin->e.chat->key, args[1]);
		}
		if (cwin->e.chat->key != NULL) {
			snprintf(buf, sizeof(buf), "%s %s", args[0], cwin->e.chat->key);
			args[0] = buf;
		}

		if (conn->online > 0) {
			fte_t	ret;

			if ((ret = firetalk_chat_join(conn->conn, args[0])) != FE_SUCCESS)
				echof(conn, "JOIN", "Unable to join %s: %s.\n", args[0], firetalk_strerror(ret));
		}
	}
}

CONIOFUNC(namebuddy) {
CONIODESC(Change the real name for a buddy)
CONIOAREQ(buddy,name)
CONIOAOPT(string,realname)
	if (argc == 1)
		conio_addbuddy(conn, 1, args);
	else {
		const char	*_args[3];

		_args[0] = args[0];
		_args[1] = "Buddy";
		_args[2] = args[1];
		conio_addbuddy(conn, 3, _args);
	}
}

CONIOFUNC(tagbuddy) {
CONIOALIA(tag)
CONIODESC(Mark a buddy with a reminder message)
CONIOAREQ(buddy,name)
CONIOAOPT(string,note)
	buddylist_t	*blist = rgetlist(conn, args[0]);

	if (blist == NULL) {
		echof(conn, "TAGBUDDY", "<font color=\"#00FFFF\">%s</font> is not in your buddy list.\n",
			args[0]);
		return;
	}
	if (argc > 1) {
		STRREPLACE(blist->tag, args[1]);
		echof(conn, NULL, "<font color=\"#00FFFF\">%s</font> is now tagged (%s).\n",
			user_name(NULL, 0, conn, blist), blist->tag);
	} else if (blist->tag != NULL) {
		free(blist->tag);
		blist->tag = NULL;
		echof(conn, NULL, "<font color=\"#00FFFF\">%s</font> is no longer tagged.\n",
			user_name(NULL, 0, conn, blist));
	} else
		echof(conn, "TAGBUDDY", "<font color=\"#00FFFF\">%s</font> is not tagged.\n",
			user_name(NULL, 0, conn, blist));
}

CONIOFUNC(delbuddy) {
CONIODESC(Remove someone from your buddy list)
CONIOAOPT(buddy,name)
	buddylist_t	*blist;
	const char	*name;

	if (argc == 0)
		name = lastclose;
	else
		name = args[0];

	if (name == NULL) {
		echof(conn, "DELBUDDY", "No buddy specified.\n");
		return;
	}

	if (bgetwin(conn, name, BUDDY) != NULL) {
		echof(conn, "DELBUDDY", "You can not delete people from your tracking list with a window open. Please <font color=\"#00FF00\">/close %s</font> first.\n",
			name);
		return;
	}

	if ((blist = rgetlist(conn, name)) != NULL) {
		fte_t	ret;

		if ((ret = firetalk_im_remove_buddy(conn->conn, name)) == FE_SUCCESS)
			status_echof(conn, "Removed <font color=\"#00FFFF\">%s</font> from your buddy list.\n",
				user_name(NULL, 0, conn, blist));
		else
			status_echof(conn, "Removed <font color=\"#00FFFF\">%s</font> from naim's buddy list, but the server wouldn't remove %s%s%s from your session buddy list: %s.\n", 
				user_name(NULL, 0, conn, blist), strchr(name, ' ')?"\"":"", name, strchr(name, ' ')?"\"":"",
				firetalk_strerror(ret));
		rdelbuddy(conn, name);
		blist = NULL;
	} else if (firetalk_im_remove_buddy(conn->conn, name) == FE_SUCCESS)
		status_echof(conn, "Removed <font color=\"#00FFFF\">%s</font> from your session buddy list, but <font color=\"#00FFFF\">%s</font> isn't in naim's buddy list.\n",
			name, name);
	else
		status_echof(conn, "<font color=\"#00FFFF\">%s</font> is not in your buddy list.\n",
			name);

	if ((argc == 0) && (lastclose != NULL)) {
		free(lastclose);
		lastclose = NULL;
	}
}

CONIOFUNC(op) {
CONIODESC(Give operator privilege)
CONIOAREQ(buddy,name)
CONIOWHER(INCHAT)
	fte_t	ret;

	if ((ret = firetalk_chat_op(conn->conn, conn->curbwin->winname, args[0])) != FE_SUCCESS)
		echof(conn, "OP", "Unable to op %s: %s.\n", args[0], firetalk_strerror(ret));
}

CONIOFUNC(deop) {
CONIODESC(Remove operator privilege)
CONIOAREQ(buddy,name)
CONIOWHER(INCHAT)
	fte_t	ret;

	if ((ret = firetalk_chat_deop(conn->conn, conn->curbwin->winname, args[0])) != FE_SUCCESS)
		echof(conn, "DEOP", "Unable to deop %s: %s.\n", args[0], firetalk_strerror(ret));
}

CONIOFUNC(topic) {
CONIODESC(View or change current chat topic)
CONIOAOPT(string,topic)
CONIOWHER(INCHAT)
	if (argc == 0) {
		if (conn->curbwin->blurb == NULL)
			echof(conn, NULL, "No topic set.\n");
		else
			echof(conn, NULL, "Topic for %s: </B><body>%s</body><B>.\n",
				conn->curbwin->winname,
				conn->curbwin->blurb);
	} else {
		fte_t	ret;

		if ((ret = firetalk_chat_set_topic(conn->conn, conn->curbwin->winname, args[0])) != FE_SUCCESS)
			echof(conn, "TOPIC", "Unable to change topic: %s.\n", firetalk_strerror(ret));
	}
}

CONIOFUNC(kick) {
CONIODESC(Temporarily remove someone from a chat)
CONIOAREQ(buddy,name)
CONIOAOPT(string,reason)
CONIOWHER(INCHAT)
	fte_t	ret;

	if ((ret = firetalk_chat_kick(conn->conn, conn->curbwin->winname, args[0], (argc == 2)?args[1]:conn->sn)) != FE_SUCCESS)
		echof(conn, "KICK", "Unable to kick %s: %s.\n", args[0], firetalk_strerror(ret));
}

CONIOFUNC(invite) {
CONIODESC(Invite someone to a chat)
CONIOAREQ(buddy,name)
CONIOAOPT(string,chat)
CONIOWHER(INCHAT)
	fte_t	ret;

	if ((ret = firetalk_chat_invite(conn->conn, conn->curbwin->winname, args[0],
		(argc == 2)?args[1]:"Join me in this Buddy Chat.")) != FE_SUCCESS)
		echof(conn, "INVITE", "Unable to invite %s: %s.\n", args[0], firetalk_strerror(ret));
}

CONIOFUNC(help) {
CONIOALIA(about)
CONIODESC(Display topical help on using naim)
CONIOAOPT(string,topic)
	if (argc == 0)
		help_printhelp(NULL);
	else
		help_printhelp(args[0]);
}

CONIOFUNC(unblock) {
CONIOALIA(unignore)
CONIODESC(Remove someone from the ignore list)
CONIOAREQ(buddy,name)
	echof(conn, NULL, "No longer blocking <font color=\"#00FFFF\">%s</font>.\n", args[0]);
	if (conn->online > 0)
		if (firetalk_im_remove_deny(conn->conn, args[0]) != FE_SUCCESS)
			status_echof(conn, "Removed <font color=\"#00FFFF\">%s</font> from naim's block list, but the server wouldn't remove %s from your session block list.\n",
				args[0], args[0]);
	rdelidiot(conn, args[0]);
}

CONIOFUNC(block) {
CONIODESC(Server-enforced /ignore)
CONIOAREQ(buddy,name)
CONIOAOPT(string,reason)
	if (conn->online > 0) {
		fte_t	ret;

		if ((ret = firetalk_im_add_deny(conn->conn, args[0])) != FE_SUCCESS)
			echof(conn, "BLOCK", "Unable to block %s: %s.\n", args[0], firetalk_strerror(ret));
		else
			echof(conn, NULL, "Now blocking <font color=\"#00FFFF\">%s</font>.\n", args[0]);
	} else
		echof(conn, NULL, "Now blocking <font color=\"#00FFFF\">%s</font>.\n", args[0]);
	raddidiot(conn, args[0], "block");
}

CONIOFUNC(ignore) {
CONIODESC(Ignore all private/public messages)
CONIOAOPT(buddy,name)
CONIOAOPT(string,reason)
	if (argc == 0) {
		ignorelist_t
			*idiotar = conn->idiotar;

		if (idiotar == NULL)
			echof(conn, NULL, "Ignore list is empty.\n");
		else {
			echof(conn, NULL, "Ignore list:\n");
			do {
				if (idiotar->notes != NULL)
					echof(conn, NULL, "&nbsp; %s (%s)\n", idiotar->screenname, idiotar->notes);
				else
					echof(conn, NULL, "&nbsp; %s\n", idiotar->screenname);
			} while ((idiotar = idiotar->next) != NULL);
		}
	} else if (argc == 1) {
		if (args[0][0] == '-') {
			const char
				*newargs[] = { args[0]+1 };

			conio_unblock(conn, 1, newargs);
		} else {
			echof(conn, NULL, "Now ignoring <font color=\"#00FFFF\">%s</font>.\n", args[0]);
			raddidiot(conn, args[0], NULL);
		}
	} else if (strcasecmp(args[1], "block") == 0)
		conio_block(conn, 1, args);
	else {
		echof(conn, NULL, "Now ignoring <font color=\"#00FFFF\">%s</font> (%s).\n", args[0], args[1]);
		raddidiot(conn, args[0], args[1]);
	}
}

//extern void	(*conio_chains)();
CONIOFUNC(chains) {
CONIOALIA(tables)
CONIODESC(Manipulate data control tables)
CONIOAOPT(string,chain)
	char	buf[1024];
	chain_t	*chain;
	lt_dlhandle self;
	int	i;

	if (argc == 0) {
		const char *chains[] = { "getcmd", "notify", "periodic", "recvfrom", "sendto", "proto_user_onlineval" };

		for (i = 0; i < sizeof(chains)/sizeof(*chains); i++) {
			if (i > 0)
				echof(conn, NULL, "-\n");
			conio_chains(conn, 1, chains+i);
		}
		echof(conn, NULL, "See <font color=\"#00FF00\">/help chains</font> for more information.\n");
		return;
	}
#ifdef DLOPEN_SELF_LIBNAIM_CORE
	self = lt_dlopen("cygnaim_core-0.dll");
#else
	self = lt_dlopen(NULL);
#endif
	if (self == NULL) {
		echof(conn, "TABLES", "Unable to perform self-symbol lookup: %s.\n",
			lt_dlerror());
		return;
	}
	snprintf(buf, sizeof(buf), "chain_%s", args[0]);
	if ((chain = lt_dlsym(self, buf)) == NULL) {
		echof(conn, "TABLES", "Unable to find chain %s (%s): %s.\n", 
			args[0], buf, lt_dlerror());
		lt_dlclose(self);
		return;
	}
	lt_dlclose(self);

	echof(conn, NULL, "Chain %s, containing %i hook%s.\n",
		args[0], chain->count, (chain->count==1)?"":"s");
	for (i = 0; i < chain->count; i++) {
		const char
			*modname, *hookname;

		if (chain->hooks[i].mod == NULL)
			modname = "core";
		else {
			const lt_dlinfo
				*dlinfo = lt_dlgetinfo(chain->hooks[i].mod);

			modname = dlinfo->name;
		}
		hookname = chain->hooks[i].name;
		if (*hookname == '_')
			hookname++;
		if ((strncmp(hookname, modname, strlen(modname)) == 0) && (hookname[strlen(modname)] == '_'))
			hookname += strlen(modname)+1;
		echof(conn, NULL, " <font color=\"#808080\">%i: <font color=\"#FF0000\">%s</font>:<font color=\"#00FFFF\">%s</font>() weight <B>%i</B> at <B>%#p</B> (%lu passes, %lu stops)</font>\n",
			i, modname, hookname, chain->hooks[i].weight, chain->hooks[i].func, chain->hooks[i].passes, chain->hooks[i].hits);
	}
}

CONIOFUNC(filter) {
CONIODESC(Manipulate content filters)
CONIOAOPT(string,table)
CONIOAOPT(string,target)
CONIOAOPT(string,action)
	if (argc == 0) {
		echof(conn, NULL, "Current filter tables: REPLACE.\n");
		return;
	}

	if (strcasecmp(args[0], "REPLACE") == 0) {
		extern html_clean_t
			*html_cleanar;
		extern int
			html_cleanc;
		int	i;

		if (argc == 1) {
//			echof("REPLACE: Table commands: :FLUSH :LIST :APPEND :DELETE");
			if (html_cleanc > 0) {
				for (i = 0; i < html_cleanc; i++)
					if (*html_cleanar[i].from != 0)
						echof(conn, "FILTER REPLACE", "= %s -> %s\n",
							html_cleanar[i].from, html_cleanar[i].replace);
			} else
				echof(conn, "FILTER REPLACE", "Table empty.\n");
		} else if (argc == 2) {
			const char	*arg;
			char	action;

			switch (args[1][0]) {
			  case '-':
			  case '+':
			  case '=':
			  case ':':
				action = args[1][0];
				arg = args[1]+1;
				break;
			  default:
				action = '=';
				arg = args[1];
				break;
			}

			if (action == ':') {
				if (strcasecmp(arg, "FLUSH") == 0) {
					for (i = 0; i < html_cleanc; i++) {
						free(html_cleanar[i].from);
						html_cleanar[i].from = NULL;
						free(html_cleanar[i].replace);
						html_cleanar[i].replace = NULL;
					}
					free(html_cleanar);
					html_cleanar = NULL;
					html_cleanc = 0;
				} else
					echof(conn, "FILTER REPLACE", "Unknown action ``%s''.\n",
						arg);
			} else if (action == '+')
				echof(conn, "FILTER REPLACE", "Must specify rewrite rule.\n");
			else if ((action == '=') || (action == '-')) {
				for (i = 0; i < html_cleanc; i++)
					if (strcasecmp(html_cleanar[i].from, arg) == 0) {
						if (args[1][0] == '-') {
							echof(conn, "FILTER REPLACE", "- %s -> %s\n",
								html_cleanar[i].from, html_cleanar[i].replace);
							STRREPLACE(html_cleanar[i].from, "");
							STRREPLACE(html_cleanar[i].replace, "");
						} else
							echof(conn, "FILTER REPLACE", "= %s -> %s\n",
								html_cleanar[i].from, html_cleanar[i].replace);
						return;
					}
				echof(conn, "FILTER REPLACE", "%s is not in the table.\n",
					arg);
			} else
				echof(conn, "FILTER REPLACE", "Unknown modifier %c (%s).\n",
					action, arg);
		} else {
			for (i = 0; i < html_cleanc; i++)
				if (strcasecmp(html_cleanar[i].from, args[1]) == 0) {
					echof(conn, "FILTER REPLACE", "- %s -> %s\n",
						html_cleanar[i].from, html_cleanar[i].replace);
					break;
				}
			if (i == html_cleanc)
				for (i = 0; i < html_cleanc; i++)
					if (*html_cleanar[i].from == 0)
						break;
			if (i == html_cleanc) {
				html_cleanc++;
				html_cleanar = realloc(html_cleanar, html_cleanc*sizeof(*html_cleanar));
				html_cleanar[i].from = html_cleanar[i].replace = NULL;
			}
			STRREPLACE(html_cleanar[i].from, args[1]);
			STRREPLACE(html_cleanar[i].replace, args[2]);
			echof(conn, "FILTER REPLACE", "+ %s -> %s\n",
				html_cleanar[i].from, html_cleanar[i].replace);
		}
	} else
		echof(conn, "FILTER", "Table %s does not exist.",
			args[0]);
}

static html_clean_t
	conio_filter_defaultar[] = {
	{ "u",		"you"		},
	{ "ur",		"your"		},
	{ "lol",	"<grin>"	},
	{ "lawlz",	"<grin>"	},
	{ "lolz",	"<grin>"	},
	{ "r",		"are"		},
	{ "ru",		"are you"	},
	{ "some1",	"someone"	},
	{ "sum1",	"someone"	},
	{ "ne",		"any"		},
	{ "ne1",	"anyone"	},
	{ "im",		"I'm"		},
	{ "b4",		"before"	},
};

static void
	conio_filter_defaults(void) {
	int	i;

	for (i = 0; i < sizeof(conio_filter_defaultar)/sizeof(*conio_filter_defaultar); i++) {
		char	buf[1024];

		snprintf(buf, sizeof(buf), "filter replace %s %s", conio_filter_defaultar[i].from,
			conio_filter_defaultar[i].replace);
		conio_handlecmd(buf);
	}
}

CONIOFUNC(warn) {
CONIODESC(Send a warning about someone)
CONIOAREQ(buddy,name)
	fte_t	ret;

	if ((ret = firetalk_im_evil(conn->conn, args[0])) == FE_SUCCESS)
		echof(conn, NULL, "Eek, stay away, <font color=\"#00FFFF\">%s</font> is EVIL!\n", args[0]);
	else
		echof(conn, "WARN", "Unable to warn %s: %s.\n", args[0], firetalk_strerror(ret));
}

CONIOFUNC(nick) {
CONIODESC(Change or reformat your name)
CONIOAREQ(string,name)
	if (conn->online > 0) {
		fte_t	ret;

		if ((ret = firetalk_set_nickname(conn->conn, args[0])) != FE_SUCCESS)
			echof(conn, "NICK", "Unable to change names: %s.\n", firetalk_strerror(ret));
	} else
		echof(conn, "NICK", "Try <font color=\"#00FF00\">/connect %s</font>.\n", args[0]);
}

CONIOFUNC(echo) {
CONIODESC(Display something on the screen with $-variable expansion)
CONIOAREQ(string,script)
	echof(conn, NULL, "%s\n", secs_script_expand(NULL, args[0]));
}

CONIOFUNC(readprofile) {
CONIODESC(Read your profile from disk)
CONIOAREQ(filename,filename)
	const char *filename = args[0];
	struct stat statbuf;
	size_t	len;
	int	pfd;

	if ((len = strlen(filename)) == 0) {
		echof(conn, "READPROFILE", "Please specify a real file.\n");
		return;
	}
	if (stat(filename, &statbuf) == -1) {
		echof(conn, "READPROFILE", "Can't read %s: %s.\n",
			filename, strerror(errno));
		return;
	}
	if ((len = statbuf.st_size) < 1) {
		echof(conn, "READPROFILE", "Profile file too small.\n");
		return;
	}
	if ((pfd = open(filename, O_RDONLY)) < 0) {
		echof(conn, "READPROFILE", "Unable to open %s.\n",
			filename);
		return;
	}
	free(conn->profile);
	if ((conn->profile = malloc(len+1)) == NULL) {
		echof(conn, "READPROFILE", "Fatal error in malloc(%i): %s\n",
			len+1, strerror(errno));
		return;
	}
	if (read(pfd, conn->profile, len) < len) {
		echof(conn, "READPROFILE", "Fatal error in read(%i): %s\n",
			pfd, strerror(errno));
		free(conn->profile);
		conn->profile = NULL;
		close(pfd);
		return;
	}
	close(pfd);
	conn->profile[len] = 0;
	if (conn->online > 0)
		naim_set_info(conn->conn, conn->profile);
}

CONIOFUNC(accept) {
CONIODESC(EXPERIMENTAL Accept a file transfer request in the current window)
CONIOAREQ(window,filename)
CONIOWHER(NOTSTATUS)
	if (conn->curbwin->et != TRANSFER) {
		echof(conn, "ACCEPT", "You must be in a file transfer window.\n");
		return;
	}
	firetalk_file_accept(conn->conn, conn->curbwin->e.transfer->handle,
		conn->curbwin, args[0]);
	STRREPLACE(conn->curbwin->e.transfer->local, args[0]);
}

CONIOFUNC(offer) {
CONIODESC(EXPERIMENTAL Offer a file transfer request to someone)
CONIOAREQ(buddy,name)
CONIOAREQ(filename,filename)
	const char
		*from = args[0],
		*filename = args[1];

	if (bgetwin(conn, filename, TRANSFER) == NULL) {
		buddywin_t	*bwin;

		bnewwin(conn, filename, TRANSFER);
		bwin = bgetwin(conn, filename, TRANSFER);
		assert(bwin != NULL);
		bwin->e.transfer = fnewtransfer(NULL, filename, from, -1);
		echof(conn, "OFFER", "Offering file transfer request to <font color=\"#00FFFF\">%s</font> (%s).\n",
			from, filename);
		firetalk_file_offer(conn->conn, from, filename, bwin);
	} else
		echof(conn, "OFFER", "Ignoring duplicate file transfer request to <font color=\"#00FFFF\">%s</font> (%s).\n",
			from, filename);
}

CONIOFUNC(setcol) {
CONIODESC(View or change display colors)
CONIOAOPT(string,eventname)
CONIOAOPT(string,colorname)
CONIOAOPT(string,colormodifier)
	int	i, col;

	if (argc == 0) {
		echof(conn, NULL, "Available colors include:\n");
		for (i = 0; i < sizeof(collist)/sizeof(*collist); i++)
			echof(conn, NULL, "&nbsp;%-2i&nbsp;%s\n", i, collist[i]);
		echof(conn, NULL, "Foregrounds:\n");
		for (i = 0; i < sizeof(forelist)/sizeof(*forelist); i++)
			echof(conn, NULL, "&nbsp;%-2i&nbsp;%s text is %s.\n", i, forelist[i],
				collist[faimconf.f[i]%(nw_COLORS*nw_COLORS)]);
		echof(conn, NULL, "Backgrounds:\n");
		for (i = 0; i < sizeof(backlist)/sizeof(*backlist); i++)
			echof(conn, NULL, "&nbsp;%-2i&nbsp;%s window is %s.\n", i, backlist[i],
				collist[faimconf.b[i]]);
		return;
	} else if (argc == 1) {
		echof(conn, "SETCOL", "Please see <font color=\"#00FF00\">/help setcol</font>.\n");
		return;
	}

	if (args[1][0] == '0')
		col = 0;
	else if ((col = atoi(args[1])) == 0) {
		for (i = 0; i < sizeof(collist)/sizeof(*collist); i++)
			if (strncasecmp(collist[i], args[1], strlen(args[1])) == 0)
				break;
		if (i < sizeof(collist)/sizeof(*collist))
			col = i;
	}

	for (i = 0; i < sizeof(forelist)/sizeof(*forelist); i++)
		if (strcasecmp(forelist[i], args[0]) == 0)
			break;
	if (i < sizeof(forelist)/sizeof(*forelist)) {
		if (argc >= 3) {
			if (strcasecmp(args[2], "BOLD") == 0)
				col += 2*(nw_COLORS*nw_COLORS);
			else if (strcasecmp(args[2], "DULL") == 0)
				col += (nw_COLORS*nw_COLORS);
			else {
				echof(conn, "SETCOL", "Unknown modifier %s.\n",
					args[2]);
				return;
			}
		}

		if (faimconf.f[i] != col) {
			faimconf.f[i] = col;
			echof(conn, NULL, "Foreground %s set to %s.\n", forelist[i],
				collist[col%(nw_COLORS*nw_COLORS)]);
		}
	} else {
		for (i = 0; i < sizeof(backlist)/sizeof(*backlist); i++)
			if (strcasecmp(backlist[i], args[0]) == 0)
				break;
		if (i < sizeof(backlist)/sizeof(*backlist)) {
			if (argc >= 3) {
				echof(conn, "SETCOL", "You can not force bold/dull for backgrounds.\n");
				return;
			}

			if (faimconf.b[i] != col) {
				conn_t	*c = conn;

				faimconf.b[i] = col;
				if ((i == cINPUT) || (i == cSTATUSBAR))
					nw_flood(&win_info, CB(STATUSBAR,INPUT));

				if (i == cINPUT)
					nw_flood(&win_input, (COLORS*col + faimconf.f[cTEXT]%(nw_COLORS*nw_COLORS)));
				else if (i == cWINLIST)
					nw_flood(&win_buddy, (COLORS*col + faimconf.f[cBUDDY]%(nw_COLORS*nw_COLORS)));
				else if ((i != cWINLISTHIGHLIGHT) && (i != cSTATUSBAR))
				  do {
					if (i == cCONN)
						nw_flood(&(conn->nwin), (COLORS*col + faimconf.f[cTEXT]%(nw_COLORS*nw_COLORS)));
					else if (c->curbwin != NULL) {
						buddywin_t	*bwin = c->curbwin;

						do {
							nw_flood(&(bwin->nwin), (COLORS*col + faimconf.f[cTEXT]%(nw_COLORS*nw_COLORS)));
							bwin->nwin.dirty = 1;
						} while ((bwin = bwin->next) != c->curbwin);
					}
				  } while ((c = c->next) != conn);
				echof(conn, NULL, "Background %s set to %s.\n", backlist[i],
					collist[col]);
			}
		} else
			echof(conn, "SETCOL", "Unknown window/event: %s.\n",
				args[0]);
	}
}

CONIOFUNC(setpriv) {
CONIODESC(Change your privacy mode)
CONIOAREQ(string,mode)
	fte_t	ret;

	if ((ret = firetalk_set_privacy(conn->conn, args[0])) == FE_SUCCESS)
		echof(conn, NULL, "Privacy mode changed.\n");
	else
		echof(conn, "SETPRIV", "Privacy mode not changed: %s.\n", firetalk_strerror(ret));
}

CONIOFUNC(bind) {
CONIODESC(View or change key bindings)
CONIOAOPT(string,keyname)
CONIOAOPT(string,script)
	int	i, key = 0;

	if (argc == 0) {
		const char	*binding;
		int	base;

		set_echof("    %s%-9s%s  %-25s  %s\n", "", "Key name", "  ", "Script", "Description");
		for (base = 0; base <= KEY_MAX; base += KEY_MAX) {
			for (i = 0; i < sizeof(conio_bind_names)/sizeof(*conio_bind_names); i++)
				if ((binding = conio_bind_get_informative(base+conio_bind_names[i].code))
					!= NULL)
					set_echof("    %s%-9s%s  %s\n", (base?"M-":""), conio_bind_names[i].name,
						(base?"":"  "), binding);
			for (i = 1; i <= 9; i++)
				if ((binding = conio_bind_get_informative(base+KEY_F(i))) != NULL)
					set_echof("    %sF%i%s         %s\n", (base?"M-":""), i, (base?"":"  "), binding);
			for (i = 10; i <= 12; i++)
				if ((binding = conio_bind_get_informative(base+KEY_F(i))) != NULL)
					set_echof("    %sF%i%s        %s\n", (base?"M-":""), i, (base?"":"  "), binding);
			for (i = 'A'; i <= 'Z'; i++)
				if ((binding = conio_bind_get_informative(base+CTRL(i))) != NULL)
					set_echof("    %s^%c%s         %s\n", (base?"M-":""), i, (base?"":"  "),
						binding);
		}
		for (i = 'a'; i <= 'z'; i++)
			if ((binding = conio_bind_get_informative(KEY_MAX+i)) != NULL)
				set_echof("    M-%c          %s\n", i, binding);
		return;
	}

	if ((key = atoi(args[0])) == 0) {
		int	j;

		if ((args[0][0] == 'M') && (args[0][1] == '-')) {
			key = KEY_MAX;
			j = 2;
		} else
			j = 0;
		for (i = 0; i < sizeof(conio_bind_names)/sizeof(*conio_bind_names); i++)
			if (strcasecmp(conio_bind_names[i].name, args[0]+j) == 0) {
				key += conio_bind_names[i].code;
				break;
			}
		if (i == sizeof(conio_bind_names)/sizeof(*conio_bind_names))
			key = 0;
	}

	if (key > 0);
	else {
		if ((args[0][0] == 'M') && (args[0][1] == '-')) {
			key = KEY_MAX;
			i = 2;
		} else
			i = 0;

		if (args[0][i+1] == 0) {
			if (key == 0)
				key += args[0][i];
			else
				key += tolower(args[0][i]);
		} else if (args[0][i] == 'F')
			key += KEY_F(atoi(args[0]+i+1));
		else if (args[0][i] == '^')
			key += CTRL(toupper(args[0][i+1]));
		else if ((args[0][i] == 'C') && (args[0][i+1] == '-'))
			key += CTRL(toupper(args[0][i+2]));
	}

	if (argc == 1) {
		const char	*binding = conio_bind_get_pretty(key);

		if (binding != NULL)
			echof(conn, NULL, "Key %i (%s) bound to ``%s''\n",
				key, args[0], binding);
		else
			echof(conn, NULL, "Key %i (%s) is unbound.\n",
				key, args[0]);
	} else {
		if (args[1][0] == ':') {
			char	buf[20];

			for (i = 0; i < sizeof(conio_key_names)/sizeof(*conio_key_names); i++)
				if (strcasecmp(args[1]+1, conio_key_names[i].name+1) == 0)
					break;
			snprintf(buf, sizeof(buf), ":%i", i);
			conio_bind_set(key, buf, NULL);
		} else
			conio_bind_set(key, args[1], NULL);
		echof(conn, NULL, "Key %i (%s) now bound to ``%s''\n",
			key, args[0], args[1]);
	}
}

CONIOFUNC(alias) {
CONIODESC(Create a new command alias)
CONIOAREQ(string,commandname)
CONIOAREQ(string,script)
	script_makealias(args[0], args[1]);
	echof(conn, NULL, "Aliased <font color=\"#00FF00\">/%s</font> to: %s\n", args[0], args[1]);
}

CONIOFUNC(set) {
CONIODESC(View or change the value of a configuration or session variable; see /help settings)
CONIOAOPT(varname,varname)
CONIOAOPT(string,value)
CONIOAOPT(string,dummy)
	if (argc == 0)
		set_setvar(NULL, NULL);
	else if (argc == 1)
		set_setvar(args[0], NULL);
	else if (argc == 2)
		set_setvar(args[0], args[1]);
	else
		echof(conn, "SET", "Try <font color=\"#00FF00\">/set %s \"%s %s\"</font>.\n",
			args[0], args[1], args[2]);
}

static void conio_listprotocols(conn_t *conn, const char *prefix) {
	int	i;
	const char *str;

	echof(conn, prefix, "Protocol drivers are currently loaded for:\n");
	for (i = 0; (str = firetalk_strprotocol(i)) != NULL; i++)
		echof(conn, prefix, "&nbsp; %s\n", str);
}

CONIOFUNC(newconn) {
CONIODESC(Open a new connection window)
CONIOAOPT(string,label)
CONIOAOPT(string,protocol)
	int	i, proto;
	conn_t	*newconn = curconn;
	const char *protostr;

	if (argc == 0) {
		conio_listprotocols(conn, NULL);
		echof(conn, NULL, "See <font color=\"#00FF00\">/help newconn</font> for more help.\n");
		return;
	} else if (argc == 1) {
		if ((strcasecmp(args[0], "Undernet") == 0)
			|| (strcasecmp(args[0], "EFnet") == 0))
			protostr = "IRC";
		else if ((strcasecmp(args[0], "AIM") == 0)
			|| (strcasecmp(args[0], "ICQ") == 0))
			protostr = "TOC2";
		else if (strcasecmp(args[0], "lily") == 0)
			protostr = "SLCP";
		else {
			echof(conn, "NEWCONN", "Please supply a protocol name:");
			conio_listprotocols(conn, "NEWCONN");
			echof(conn, "NEWCONN", "See <font color=\"#00FF00\">/help newconn</font> for more help.\n");
			return;
		}
	} else {
		if ((strcasecmp(args[1], "AIM") == 0)
			|| (strcasecmp(args[1], "AIM/TOC") == 0)
			|| (strcasecmp(args[1], "AIM/TOC2") == 0)
			|| (strcasecmp(args[1], "ICQ") == 0)
			|| (strcasecmp(args[1], "ICQ/TOC") == 0)
			|| (strcasecmp(args[1], "ICQ/TOC2") == 0)
			|| (strcasecmp(args[1], "TOC") == 0))
			protostr = "TOC2";
		else if (strcasecmp(args[1], "Lily") == 0)
			protostr = "SLCP";
		else
			protostr = args[1];
	}

	if ((proto = firetalk_find_protocol(protostr)) == -1) {
		echof(conn, "NEWCONN", "Invalid protocol %s.", protostr);
		conio_listprotocols(conn, "NEWCONN");
		echof(conn, "NEWCONN", "See <font color=\"#00FF00\">/help newconn</font> for more help.\n");
		return;
	}

	if (newconn != NULL)
		do {
			if (strcasecmp(args[0], newconn->winname) == 0) {
				echof(conn, "NEWCONN", "A window for connection %s (%s) is already open.\n",
					newconn->winname, firetalk_strprotocol(newconn->proto));
				return;
			}
		} while ((newconn = newconn->next) != curconn);

	newconn = naim_newconn(proto);
	assert(newconn != NULL);

	nw_newwin(&(newconn->nwin));
	nw_initwin(&(newconn->nwin), cCONN);
	nw_erase(&(newconn->nwin));
	for (i = 0; i < faimconf.wstatus.pady; i++)
		nw_printf(&(newconn->nwin), 0, 0, "\n");
	if (curconn == NULL)
		newconn->next = newconn;
	else {
		newconn->next = curconn->next;
		curconn->next = newconn;
	}
	curconn = newconn;
	STRREPLACE(newconn->winname, args[0]);
	if (newconn->next != newconn) {
		echof(newconn, NULL, "A new connection of type %s has been created.\n",
			firetalk_strprotocol(proto));
		echof(newconn, NULL, "Ins and Del will switch between connections, and /jump (^N, M-n, F8) will jump across connections if there are no active windows in the current one.\n");
		echof(newconn, NULL, "You can now <font color=\"#00FF00\">/connect &lt;name&gt; [&lt;server&gt;]</font> to log on.\n");
	}
	bupdate();
}

CONIOFUNC(delconn) {
CONIODESC(Close a connection window)
CONIOAOPT(string,label)
	if (curconn->next == curconn) {
		echof(conn, "DELCONN", "You must always have at least one connection open at all times.\n");
		return;
	}

	if (argc > 0) {
		int	found = 0;

		do {
			if (strcasecmp(args[0], conn->winname) == 0) {
				found = 1;
				break;
			}
		} while ((conn = conn->next) != curconn);

		if (found == 0) {
			echof(conn, "DELCONN", "Unable to find connection %s.\n",
				args[0]);
			return;
		}
	}

	do_delconn(conn);
}

CONIOFUNC(server) {
CONIODESC(Connect to a service)
CONIOAOPT(string,server)
CONIOAOPT(int,port)
	const char	*na[3];

	if (argc == 0) {
		if (conn->port != 0)
			echof(conn, NULL, "Current server: %s %i\n",
				(conn->server == NULL)?"(default host)":conn->server,
				conn->port);
		else
			echof(conn, NULL, "Current server: %s (default port)\n",
				(conn->server == NULL)?"(default host)":conn->server);
		return;
	}
	if (conn->sn == NULL) {
		echof(conn, "SERVER", "Please try to <font color=\"#00FF00\">/%s:connect</font> first.\n",
			conn->winname);
		return;
	}
	na[0] = conn->sn;
	na[1] = args[0];
	if (argc == 2);
		na[2] = args[1];
	conio_connect(conn, argc+1, na);
}

CONIOFUNC(disconnect) {
CONIODESC(Disconnect from a server)
	if (conn->online <= 0)
		echof(conn, "DISCONNECT", "You aren't connected.\n");
	else if (firetalk_disconnect(conn->conn) == FE_SUCCESS) {
		bclearall(conn, 0);
		echof(conn, NULL, "You are now disconnected.\n");
	}
	conn->online = 0;
}

CONIOFUNC(winlist) {
CONIODESC(Switch the window list window between always visible or always hidden or auto-hidden)
CONIOAOPT(string,visibility)
	if (argc == 0) {
		if (changetime == -1)
			echof(conn, NULL, "Window list window is always hidden (possible values are HIDDEN, VISIBLE, or AUTO).\n");
		else if (changetime == 0)
			echof(conn, NULL, "Window list window is always visible (possible values are HIDDEN, VISIBLE, or AUTO).\n");
		else
			echof(conn, NULL, "Window list window is auto-hidden (possible values are HIDDEN, VISIBLE, or AUTO).\n");
	} else {
		if (strncasecmp(args[0], "hid", sizeof("hid")-1) == 0) {
			if (changetime != -1) {
				echof(conn, NULL, "Window list window is now always hidden.\n");
				changetime = -1;
			}
		} else if (strncasecmp(args[0], "vis", sizeof("vis")-1) == 0) {
			if (changetime != 0) {
				echof(conn, NULL, "Window list window is now always visible.\n");
				changetime = 0;
			}
		} else if ((changetime == -1) || (changetime == 0)) {
			echof(conn, NULL, "Window list window is now auto-hidden.\n");
			changetime = nowf;
		} else
			echof(conn, "WINLIST", "Hmm? Possible values are HIDDEN, VISIBLE, or AUTO.\n");
	}
}

#ifdef HAVE_WORKING_FORK
static void
	exec_read(int _i, int fd, void *_buf, int _buflen) {
	conn_t	*conn = (conn_t *)_buf;
	char	buf[1024], *ptr, *n;
	int	i, buflen = sizeof(buf),
		sayit = _buflen;

	i = read(fd, buf, buflen-1);
	if (i == 0) {
		close(fd);
		mod_fd_unregister(_i);
		return;
	}
	buf[i] = 0;
	ptr = buf;
	while ((n = strchr(ptr, '\n')) != NULL) {
		*n = 0;
		if (*(n-1) == '\r')
			*(n-1) = 0;
		if ((sayit == 0) || (conn->curbwin == NULL))
			echof(conn, "_", "%s\n", ptr);
		else {
			char	buf2[1024];

			snprintf(buf2, sizeof(buf2), "/m \"%s\" %s",
				conn->curbwin->winname, ptr);
			conio_handleline(buf2);
		}
		ptr = n+1;
	}
	if (*ptr != 0) {
		if ((sayit == 0) || (conn->curbwin == NULL))
			echof(conn, "_", "%s\n", ptr);
		else {
			char	buf2[1024];

			snprintf(buf2, sizeof(buf2), "/m \"%s\" %s",
				conn->curbwin->winname, ptr);
			conio_handleline(buf2);
		}
	}
}

CONIOFUNC(exec) {
CONIODESC(Execute a shell command; as in /exec -o uname -a)
CONIOAREQ(string,command)
	int	sayit = (strncmp(args[0], "-o ", 3) == 0),
		pi[2];
	pid_t	pid;

	if (pipe(pi) != 0) {
		echof(conn, "EXEC", "Error creating pipe: %s.\n",
			strerror(errno));
		return;
	}
	if ((pid = fork()) == -1) {
		echof(conn, "EXEC", "Error in fork: %s, closing pipe.\n",
			strerror(errno));
		close(pi[0]);
		close(pi[1]);
	} else if (pid > 0) {
		close(pi[1]);
		mod_fd_register(pi[0], (O_RDONLY+1), (char *)conn, sayit,
			exec_read);
	} else {
		char	*exec_args[] = { "/bin/sh", "-c", NULL, NULL };

		close(pi[0]);
		close(STDIN_FILENO);
		dup2(pi[1], STDOUT_FILENO);
		dup2(pi[1], STDERR_FILENO);
		if (sayit)
			exec_args[2] = strdup(args[0]+3);
		else
			exec_args[2] = strdup(args[0]);
		execvp(exec_args[0], exec_args);
		/* NOTREACHED */
		printf("%s\n", strerror(errno));
		free(exec_args[2]);
		exit(0);
	}
}
#endif

#ifdef ALLOW_DETACH
CONIOFUNC(detach) {
CONIODESC(Disconnect from the current session)
	if (sty == NULL)
		echof(conn, "DETACH", "You can only <font color=\"#00FF00\">/detach</font> when naim is run under screen.\n");
	else {
		echof(conn, NULL, "Type ``screen -r'' to re-attach.\n");
		statrefresh();
		doupdate();
		kill(getppid(), SIGHUP);
		echof(conn, NULL, "Welcome back to naim.\n");
	}
}
#endif


static int	modlist_filehelper(const char *path, lt_ptr data) {
	conn_t	*conn = (conn_t *)data;
	const char *filename = naim_basename(path);

	if (strncmp(filename, "lib", 3) == 0)
		return(0);
	if (strstr(path, "/naim/") == NULL)
		return(0);
	echof(conn, NULL, "&nbsp; <font color=\"#FF0000\">%s</font> (<font color=\"#808080\">%s</font>)\n", filename, path);
	return(0);
}

static int	modlist_helper(lt_dlhandle mod, lt_ptr data) {
	conn_t	*conn = (conn_t *)data;
	const lt_dlinfo
		*dlinfo = lt_dlgetinfo(mod);
	char	*tmp;
	double	*ver;

	if (dlinfo->ref_count > 0) {
		assert(dlinfo->ref_count == 1);
		echof(conn, NULL, "&nbsp; <font color=\"#FF0000\">%s</font> (<font color=\"#808080\">%s</font>)\n",
			dlinfo->name, dlinfo->filename);
		if ((tmp = lt_dlsym(mod, "module_category")) != NULL)
			echof(conn, NULL, "&nbsp; &nbsp; &nbsp; &nbsp;Category: <font color=\"#FFFFFF\">%s</font>\n", tmp);
		if ((tmp = lt_dlsym(mod, "module_description")) != NULL)
			echof(conn, NULL, "&nbsp; &nbsp; Description: <font color=\"#FFFFFF\">%s</font>\n", tmp);
		if ((tmp = lt_dlsym(mod, "module_license")) != NULL)
			echof(conn, NULL, "&nbsp; &nbsp; &nbsp; &nbsp; License: <font color=\"#FFFFFF\">%s</font>\n", tmp);
		if ((tmp = lt_dlsym(mod, "module_author")) != NULL)
			echof(conn, NULL, "&nbsp; &nbsp; &nbsp; &nbsp; &nbsp;Author: <font color=\"#FFFFFF\">%s</font>\n", tmp);
		if (((ver = lt_dlsym(mod, "module_version")) != NULL) && (*ver >= 1.0))
			echof(conn, NULL, "&nbsp; &nbsp; &nbsp; &nbsp;Code Age: <font color=\"#FFFFFF\">%s</font>\n", dtime(nowf - *ver));
	}

	return(0);
}

CONIOFUNC(dlsym) {
CONIOAREQ(string,symbol)
	lt_dlhandle mod;
	lt_ptr	ptr;

#ifdef DLOPEN_SELF_LIBNAIM_CORE
	mod = lt_dlopen("cygnaim_core-0.dll");
#else
	mod = lt_dlopen(NULL);
#endif
	ptr = lt_dlsym(mod, args[0]);
	if (ptr != NULL)
		echof(conn, NULL, "Symbol %s found at %#p.\n", args[0], ptr);
	else
		echof(conn, "DLSYM", "Symbol %s not found.\n", args[0]);
	lt_dlclose(mod);
}

CONIOFUNC(modlist) {
CONIODESC(Search for and list all potential and resident naim modules)
	echof(conn, NULL, "Modules found in the default search path:\n");
	lt_dlforeachfile(NULL, modlist_filehelper, conn);
	echof(conn, NULL, "Additional modules can be loaded using their explicit paths, as in <font color=\"#00FF00\">/modload %s/mods/mymod.la</font>.\n",
		home);
	echof(conn, NULL, "Modules currently resident:\n");
	lt_dlforeach(modlist_helper, conn);
	echof(conn, NULL, "See <font color=\"#00FF00\">/help modload</font> for more information.\n");
}

CONIOFUNC(modload) {
CONIODESC(Load and initialize a dynamic module)
CONIOAREQ(filename,module)
CONIOAOPT(string,options)
	lt_dlhandle mod;
	const lt_dlinfo
		*dlinfo;
	int	(*naim_init)(lt_dlhandle mod, const char *str);
	const char
		*options;

	if (argc > 1)
		options = args[1];
	else
		options = NULL;

	mod = lt_dlopenext(args[0]);
	if (mod == NULL) {
		echof(conn, "MODLOAD", "Unable to load module <font color=\"#FF0000\">%s</font>: %s.\n",
			args[0], lt_dlerror());
		return;
	}
	dlinfo = lt_dlgetinfo(mod);
	assert(dlinfo->ref_count > 0);
	if (dlinfo->ref_count > 1) {
		echof(conn, NULL, "Module <font color=\"#FF0000\">%s</font> (<font color=\"#808080\">%s</font>) already loaded.\n",
			dlinfo->name, dlinfo->filename);
		lt_dlclose(mod);
		return;
	}
	echof(conn, NULL, "Module <font color=\"#FF0000\">%s</font> (<font color=\"#808080\">%s</font>) loaded.\n",
		dlinfo->name, dlinfo->filename);

	naim_init = lt_dlsym(mod, "naim_init");
	if (naim_init != NULL) {
		if (naim_init(mod, options) != MOD_FINISHED)
			echof(conn, NULL, "Module <font color=\"#FF0000\">%s</font> initialized, leaving resident.\n",
				dlinfo->name);
		else {
			echof(conn, NULL, "Module <font color=\"#FF0000\">%s</font> initialized, unloading.\n",
				dlinfo->name);
			echof(conn, NULL, "Module <font color=\"#FF0000\">%s</font> (<font color=\"#808080\">%s</font>) unloaded.\n",
				dlinfo->name, dlinfo->filename);
			lt_dlclose(mod);
		}
	} else
		echof(conn, NULL, "Module <font color=\"#FF0000\">%s</font> has no initializer (<font color=\"#FF0000\">%s</font>:naim_init()): %s, leaving resident.\n",
			dlinfo->name, args[0], lt_dlerror());
}

CONIOFUNC(modunload) {
CONIODESC(Deinitialize and unload a resident module)
CONIOAREQ(string,module)
	lt_dlhandle mod;

	for (mod = lt_dlhandle_next(NULL); mod != NULL; mod = lt_dlhandle_next(mod)) {
		const lt_dlinfo
			*dlinfo = lt_dlgetinfo(mod);

		if ((dlinfo->name != NULL) && (strcasecmp(args[0], dlinfo->name) == 0)) {
			int	(*naim_exit)(lt_dlhandle mod, const char *str);

			naim_exit = lt_dlsym(mod, "naim_exit");
			if (naim_exit != NULL) {
				if (naim_exit(mod, args[0]) != MOD_FINISHED) {
					echof(conn, "MODUNLOAD", "Module <font color=\"#FF0000\">%s</font> is busy (<font color=\"#FF0000\">%s</font>:naim_exit() != MOD_FINISHED), leaving resident.\n",
						dlinfo->name, dlinfo->name);
					return;
				} else
					echof(conn, NULL, "Module <font color=\"#FF0000\">%s</font> deinitialized, unloading.\n",
						dlinfo->name);
			}
			echof(conn, NULL, "Module <font color=\"#FF0000\">%s</font> (<font color=\"#808080\">%s</font>) unloaded.\n",
				dlinfo->name, dlinfo->filename);
			lt_dlclose(mod);
			return;
		}
	}
	echof(conn, "MODUNLOAD", "No module named %s loaded.\n", args[0]);
}

CONIOFUNC(resize) {
CONIODESC(Resize all windows)
CONIOAOPT(int,height)
	char	buf[20];
	int	scrollback;

	if (argc == 0)
		scrollback = secs_getvar_int("scrollback");
	else
		scrollback = atoi(args[0]);

	if (scrollback < (LINES-1)) {
		echof(conn, "RESIZE", "Window height (%i) must be greater than %i.", scrollback, LINES-2);
		scrollback = LINES-1;
	} else if (scrollback > 5000) {
		echof(conn, "RESIZE", "Window height (%i) must be less than 5001.", scrollback);
		scrollback = 5000;
	}

	snprintf(buf, sizeof(buf), "%i", scrollback);
	secs_setvar("scrollback", buf);

	faimconf.wstatus.pady = scrollback;
	win_resize();
	echof(conn, NULL, "Windows resized.");
}

//extern void	(*conio_status)();
CONIOFUNC(status) {
CONIODESC(Connection status report)
CONIOAOPT(string,connection)
	buddywin_t	*bwin;
	conn_t	*c;
	int	discussions = 0,
		users = 0,
		files = 0;

	if (argc >= 0) {
		extern char naimrcfilename[];
		extern time_t startuptime;

		echof(conn, NULL, "Running " PACKAGE_STRING NAIM_SNAPSHOT " for %s.\n",
			dtime(now - startuptime));
		echof(conn, NULL, "Config file: %s\n", naimrcfilename);
	}

	if (argc == 0) {
		const char *args[] = { conn->winname };

		conio_status(conn, -1, args);
		for (c = conn->next; c != conn; c = c->next) {
			args[0] = c->winname;
			echof(conn, NULL, "-");
			conio_status(conn, -1, args);
		}
		echof(conn, NULL, "See <font color=\"#00FF00\">/help status</font> for more information.\n");
		return;
	}
	if (strcasecmp(args[0], conn->winname) != 0) {
		for (c = conn->next; c != conn; c = c->next)
			if (strcasecmp(args[0], c->winname) == 0)
				break;
		if (c == conn) {
			echof(conn, "STATUS", "Unknown connection %s.\n", args[0]);
			return;
		}
	} else
		c = conn;

	echof(conn, NULL, "Information for %s:\n", c->winname);
	if (c->online > 0) {
		assert(c->sn != NULL);
		echof(conn, NULL, "Online as %s for %s.\n", c->sn, dtime(now - c->online));
	} else {
		if (c->sn != NULL)
			echof(conn, NULL, "Not online (want to be %s)\n", c->sn);
		else
			echof(conn, NULL, "Not online.\n");
	}

	if ((bwin = c->curbwin) != NULL) {
	  echof(conn, NULL, "Windows:\n");
	  do {
		char	*type = NULL,
			*close;

		if (bwin->keepafterso)
			close = " OPEN_INDEFINITELY";
		else
			close = " QUICK_CLOSE";
		switch (bwin->et) {
		  case CHAT:
			discussions++;
			type = "DISCUSSION_WINDOW";
			break;
		  case BUDDY:
			users++;
			type = "USER_WINDOW";
			if (!USER_PERMANENT(bwin->e.buddy))
				close = " SLOW_CLOSE";
			break;
		  case TRANSFER:
			files++;
			type = "TRANSFER_WINDOW";
			break;
		}
		if ((bwin->viewtime > 0.0) && ((nowf - bwin->viewtime) > 2.0))
			echof(conn, NULL, "&nbsp; <font color=\"#00FFFF\">%s</font> (%s%s%s%s) [last viewed %s]\n", bwin->winname,
				type, close,
				bwin->waiting?" LIVE_MESSAGES_WAITING":"",
				bwin->pouncec?" QUEUED_MESSAGES_WAITING":"",
				dtime(nowf - bwin->viewtime));
		else
			echof(conn, NULL, "&nbsp; <font color=\"#00FFFF\">%s</font> (%s%s%s%s)\n", bwin->winname,
				type, close,
				bwin->waiting?" LIVE_MESSAGES_WAITING":"",
				bwin->pouncec?" QUEUED_MESSAGES_WAITING":"");
	  } while ((bwin = bwin->next) != c->curbwin);
	}
	echof(conn, NULL, "%i discussions, %i users, %i file transfers.\n", discussions, users, files);
	if (c == conn)
		echof(conn, NULL, "See <font color=\"#00FF00\">/names</font> for buddy list information.\n");
	else
		echof(conn, NULL, "See <font color=\"#00FF00\">/%s:names</font> for buddy list information.\n", c->winname);
}



static int
	cmd_unknown(conn_t *c, const char *cmd, int argc, const char **args) {
	echof(c, cmd, "Unknown command.\n");
	return(HOOK_STOP);
}

void	conio_hook_init(void) {
	void	*mod = NULL;

	HOOK_ADD(getcmd, mod, cmd_unknown, 1000);
}

HOOK_DECLARE(getcmd);
void	conio_handlecmd(const char *buf) {
	conn_t	*c = NULL;
	char	line[1024], *cmd, *arg, *tmp;
	const char	*args[CONIO_MAXPARMS];
	int	i, a = 0;

	assert(buf != NULL);

	while (*buf == '/')
		buf++;

	if (*buf == 0)
		return;

	strncpy(line, buf, sizeof(line)-1);
	line[sizeof(line)-1] = 0;

	while ((strlen(line) > 0) && isspace(line[strlen(line)-1]))
		line[strlen(line)-1] = 0;

	if (*line == 0)
		return;

	cmd = atom(line);
	arg = firstwhite(line);

	if ((tmp = strchr(cmd, ':')) != NULL) {
		conn_t	*conn = curconn;

		*tmp = 0;
		if (conn != NULL)
			do {
				if (strcasecmp(cmd, conn->winname) == 0) {
					c = conn;
					break;
				}
			} while ((conn = conn->next) != curconn);
		if (c == NULL) {
			echof(curconn, NULL, "[%s:%s] Unknown connection.\n", cmd, tmp+1);
			return;
		}
		cmd = tmp+1;
	} else
		c = curconn;

	if (script_doalias(cmd, arg) == 1)
		return;

	for (i = 0; i < cmdc; i++)
		if (strcasecmp(cmdar[i].c, cmd) == 0)
			break;
		else {
			int	j;

			for (j = 0; cmdar[i].aliases[j] != NULL; j++)
				if (strcasecmp(cmdar[i].aliases[j], cmd) == 0)
					break;
			if (cmdar[i].aliases[j] != NULL)
				break;
		}
	if (i == cmdc) {
		HOOK_CALL(getcmd, (c, cmd, a, args));
		return;
	}
	assert(cmdar[i].maxarg <= CONIO_MAXPARMS);

	if ((cmdar[i].where != C_STATUS) && (cmdar[i].where != C_ANYWHERE) && !inconn) {
		if (cmdar[i].where == C_INCHAT)
			echof(c, cmdar[i].c, "You must be in a chat.\n");
		else if (cmdar[i].where == C_INUSER)
			echof(c, cmdar[i].c, "You must be in a query.\n");
		else
			echof(c, cmdar[i].c, "You can not be in status.\n");
		return;
	} else if ((cmdar[i].where == C_STATUS) && inconn) {
		assert(curconn->curbwin != NULL);
		echof(c, cmdar[i].c, "You must be in status.\n");
		return;
	} else if ((cmdar[i].where == C_INUSER) && (c->curbwin->et == CHAT)) {
		echof(c, cmdar[i].c, "You must be in a query.\n");
		return;
	} else if ((cmdar[i].where == C_INCHAT) && (c->curbwin->et != CHAT)) {
		echof(c, cmdar[i].c, "You must be in a chat.\n");
		return;
	}

	for (a = 0; (a < cmdar[i].maxarg) && (arg != NULL); a++) {
		args[a] = atom(arg);
		if (a < cmdar[i].maxarg-1)
			arg = firstwhite(arg);
	}

	if (a < cmdar[i].minarg) {
		echof(c, cmdar[i].c, "Command requires at least %i arguments.\n",
			cmdar[i].minarg);
		return;
	}

	cmdar[i].func(c, a, args);
}

void	(*secs_client_cmdhandler)(const char *) = conio_handlecmd;

void	conio_handleline(const char *line) {
	if (*line == '/')
		conio_handlecmd(line+1);
	else if (!inconn)
		conio_handlecmd(line);
	else {
		const char	*args[] = { NULL, line };

		curconn->curbwin->keepafterso = 1;
		conio_msg(curconn, 2, args);
	}
}

static const char
	*n_strnrchr(const char *const str, const char c, const int len) {
	const char
		*s = str+len;

	while (s >= str) {
		if (*s == c)
			return(s);
		s--;
	}
	return(NULL);
}

#define ADDTOBUF(ch)	do { \
	if (strlen(buf) < sizeof(buf)-1) { \
		if (buf[bufloc] != 0) \
			memmove(buf+bufloc+1, buf+bufloc, sizeof(buf)-bufloc-1); \
		buf[bufloc++] = (ch); \
		histpos = histc-1; \
		STRREPLACE(histar[histpos], buf); \
	} else \
		beep(); \
} while (0)

const char
	*window_tabcomplete(conn_t *const conn, const char *start, const char *buf, const int bufloc, int *const match, const char **desc) {
	static char
		str[1024];
	buddywin_t
		*bwin;
	size_t	startlen = strlen(start);

	if ((bwin = conn->curbwin) == NULL)
		return(NULL);
	bwin = bwin->next;
	do {
		if (aimncmp(bwin->winname, start, startlen) == 0) {
			char	*name = bwin->winname;
			int	j;

			if (match != NULL)
				*match = bufloc - (start-buf);
			if (desc != NULL) {
				if (bwin->et == BUDDY)
					*desc = bwin->e.buddy->_name;
				else
					*desc = NULL;
			}

			for (j = 0; (j < sizeof(str)-1) && (*name != 0); j++) {
				while (*name == ' ')
					name++;
				str[j] = *(name++);
			}
			str[j] = 0;
			return(str);
		}
		if ((bwin->et == BUDDY) && (bwin->e.buddy->_name != NULL) && (aimncmp(bwin->e.buddy->_name, start, startlen) == 0)) {
			char	*name = bwin->e.buddy->_name;
			int	j;

			if (match != NULL)
				*match = bufloc - (start-buf);
			if (desc != NULL)
				*desc = bwin->winname;

			for (j = 0; (j < sizeof(str)-1) && (*name != 0); j++) {
				while (*name == ' ')
					name++;
				str[j] = *(name++);
			}
			str[j] = 0;
			return(str);
		}
	} while ((bwin = bwin->next) != conn->curbwin->next);

	return(NULL);
}

const char
	*chat_tabcomplete(conn_t *const conn, const char *start, const char *buf, const int bufloc, int *const match, const char **desc) {
	static char
		str[1024];
	buddywin_t
		*bwin;
	size_t	startlen = strlen(start);

	if ((bwin = conn->curbwin) == NULL)
		return(NULL);
	bwin = bwin->next;
	do {
		if ((bwin->et == CHAT) && (aimncmp(bwin->winname, start, startlen) == 0)) {
			char	*name = bwin->winname;
			int	j;

			if (match != NULL)
				*match = bufloc - (start-buf);
			if (desc != NULL)
				*desc = NULL;

			for (j = 0; (j < sizeof(str)-1) && (*name != 0); j++) {
				while (*name == ' ')
					name++;
				str[j] = *(name++);
			}
			str[j] = 0;
			return(str);
		}
	} while ((bwin = bwin->next) != conn->curbwin->next);

	return(NULL);
}

const char
	*filename_tabcomplete(conn_t *const conn, const char *start, const char *buf, const int bufloc, int *const match, const char **desc) {
	static char
		str[1024];
	struct dirent
		*dire;
	DIR	*dir;
	const char
		*end;
	size_t	startlen = strlen(start),
		endlen;

	if ((dir = opendir(start)) == NULL) {
		if ((end = strrchr(start, '/')) != NULL) {
			char	buf[1024];

			snprintf(buf, sizeof(buf), "%.*s", end-start+1, start);
			dir = opendir(buf);
		} else {
			end = start-1;
			dir = opendir(".");
		}
	} else
		end = start+startlen;

	if (dir == NULL)
		return(NULL);

	endlen = strlen(end+1);
	while ((dire = readdir(dir)) != NULL) {
		if ((strcmp(dire->d_name, ".") == 0) || (strcmp(dire->d_name, "..") == 0))
			continue;
		if ((*end == 0) || (strncmp(dire->d_name, end+1, endlen) == 0)) {
			struct stat
				statbuf;

			if (((end-start) > 0) && (start[end-start-1] == '/'))
				snprintf(str, sizeof(str)-1, "%.*s%s", end-start, start, dire->d_name);
			else if (end == (start-1))
				snprintf(str, sizeof(str)-1, "%s", dire->d_name);
			else
				snprintf(str, sizeof(str)-1, "%.*s/%s", end-start, start, dire->d_name);
			if (stat(str, &statbuf) == -1)
				continue;
			if (S_ISDIR(statbuf.st_mode))
				strcat(str, "/");
			if (match != NULL)
				*match = bufloc-(start-buf);
			if (desc != NULL)
				*desc = NULL;
			closedir(dir);
			return(str);
		}
	}
	closedir(dir);
	return(NULL);
}

const char
	*conio_tabcomplete(const char *buf, const int bufloc, int *const match, const char **desc) {
	char	*sp = memchr(buf, ' ', bufloc);

	assert(*buf == '/');

	if (sp == NULL) {
		extern script_t
			*scriptar;
		extern int
			scriptc;
		conn_t	*conn;
		const char
			*co = memchr(buf, ':', bufloc);
		int	i;

		if (co == NULL)
			co = buf+1;
		else
			co++;

		for (i = 0; i < scriptc; i++)
			if (strncasecmp(scriptar[i].name, co, bufloc-(co-buf)) == 0) {
				if (match != NULL)
					*match = bufloc-(co-buf);
				if (desc != NULL)
					*desc = scriptar[i].script;
				return(scriptar[i].name);
			}

		for (i = 0; i < cmdc; i++)
			if (strncasecmp(cmdar[i].c, co, bufloc-(co-buf)) == 0) {
				if (match != NULL)
					*match = bufloc-(co-buf);
				if (desc != NULL)
					*desc = cmdar[i].desc;
				return(cmdar[i].c);
			} else {
				int	j;

				for (j = 0; cmdar[i].aliases[j] != NULL; j++)
					if (strncasecmp(cmdar[i].aliases[j], co, bufloc-(co-buf)) == 0) {
						if (match != NULL)
							*match = bufloc-(co-buf);
						if (desc != NULL)
							*desc = cmdar[i].desc;
						return(cmdar[i].aliases[j]);
					}
			}

		conn = curconn->next;
		do {
			if (strncasecmp(conn->winname, co, bufloc-(co-buf)) == 0) {
				static char
					str[1024];

				if (match != NULL)
					*match = bufloc-(co-buf);
				if (desc != NULL)
					*desc = firetalk_strprotocol(conn->proto);
				strncpy(str, conn->winname, sizeof(str)-2);
				str[sizeof(str)-2] = 0;
				strcat(str, ":");
				return(str);
			}
		} while ((conn = conn->next) != curconn->next);

		return(NULL);
	} else {
		conn_t	*conn = curconn;
		const char
			*sp = memchr(buf, ' ', bufloc),
			*co = memchr(buf, ':', bufloc),
			*start = n_strnrchr(buf, ' ', bufloc)+1;
		char	type;
		int	cmd;

		if ((co != NULL) && (co < sp)) {
			size_t	connlen = (co - buf)-1;

			do {
				if ((strncasecmp(buf+1, conn->winname, connlen) == 0) && (conn->winname[connlen] == 0))
					break;
			} while ((conn = conn->next) != curconn);
			co++;
		} else
			co = buf+1;

		for (cmd = 0; cmd < cmdc; cmd++)
			if ((strncasecmp(cmdar[cmd].c, co, sp-co) == 0) && (cmdar[cmd].c[sp-co] == 0))
				break;
			else {
				int	alias;

				for (alias = 0; cmdar[cmd].aliases[alias] != NULL; alias++)
					if ((strncasecmp(cmdar[cmd].aliases[alias], co, sp-co) == 0) && (cmdar[cmd].aliases[alias][sp-co] == 0))
						break;
				if (cmdar[cmd].aliases[alias] != NULL)
					break;
			}

		if (cmd < cmdc)
			type = cmdar[cmd].args[0].type;
		else
			type = 's';

		switch (type) {
		  case 'W':
			return(window_tabcomplete(conn, start, buf, bufloc, match, desc));
		  case 'B':
			return(buddy_tabcomplete(conn, start, buf, bufloc, match, desc));
		  case 'C':
			return(chat_tabcomplete(conn, start, buf, bufloc, match, desc));
		  case 'F':
			return(filename_tabcomplete(conn, start, buf, bufloc, match, desc));
		  case 'V':
			return(set_tabcomplete(conn, start, buf, bufloc, match, desc));
		  case 'E': {
				const char *ret = buddy_tabcomplete(conn, start, buf, bufloc, match, desc);

				if (ret != NULL)
					return(ret);
			}
			return(chat_tabcomplete(conn, start, buf, bufloc, match, desc));
		  case 's': case 'i': case 'b': case '?':
		  case -1:
			return(NULL);
		  default:
			abort();
		}
	}
}

static void
	gotkey_real(int c) {
	static char	**histar = NULL;
	static int	histc = 0,
			histpos = 0;
	static int	bufloc = 0,
			bufmatchpos = -1;
	static char	buf[1024] = { 0 },
			inwhite = 0;
	const char	*binding;
	void	(*bindfunc)(char *, int *);

	if (histar == NULL) {
		histpos = 0;
		histc = 1;
		histar = realloc(histar, histc*sizeof(*histar));
		histar[histpos] = NULL;

		conio_bind_defaults();
		conio_filter_defaults();
		return;
	}
	if (c == 0)
		return;

	if ((c == '\b') || (c == 0x7F))
		c = KEY_BACKSPACE;
	if ((c == '\r') || (c == '\n'))
		c = KEY_ENTER;
	if (c == CTRL('[')) {
		int	k;

		k = getch();
		if (k != ERR) {
			char	buf[256];

			snprintf(buf, sizeof(buf), "KEY_MAX plus %s", keyname(k));
			secs_setvar("lastkey", buf);
			c = KEY_MAX + k;
		} else
			secs_setvar("lastkey", keyname(c));
	} else
		secs_setvar("lastkey", keyname(c));

	binding = conio_bind_get(c);
	bindfunc = conio_bind_func(c);
	if ((binding != NULL) || (bindfunc != NULL)) {
		if ((binding != NULL) && (binding[0] == ':')) {
		    int	bindid = atoi(binding+1);

		    if (bindid != CONIO_KEY_SPACE_OR_NBSP)
			inwhite = 0;
		    if ((bindid != CONIO_KEY_INPUT_SCROLL_BACK) && (bindid != CONIO_KEY_INPUT_SCROLL_FORWARD))
			bufmatchpos = -1;
		    switch(bindid) {
			case CONIO_KEY_REDRAW: /* Redraw the screen */
				doredraw = 1;
				break;
			case CONIO_KEY_INPUT_SCROLL_BACK: /* Search back through your input history */
				if (histpos > 0) {
					int	tmp;

					if (bufmatchpos == -1)
						bufmatchpos = bufloc;
					for (tmp = histpos-1; tmp >= 0; tmp--)
						if (strncasecmp(histar[tmp], buf, bufmatchpos) == 0) {
							assert(histar[tmp] != NULL);
							if (buf[bufloc] == 0)
								bufloc = strlen(histar[tmp]);
							strncpy(buf, histar[tmp], sizeof(buf)-1);
							histpos = tmp;
							break;
						}
				}
				break;
			case CONIO_KEY_INPUT_SCROLL_FORWARD: /* Search forward through your input history */
				if (histpos < histc) {
					int	tmp;

					if (bufmatchpos == -1)
						bufmatchpos = bufloc;
					for (tmp = histpos+1; tmp < histc; tmp++)
						if (histar[tmp] == NULL) {
							assert(tmp == histc-1);
							memset(buf, 0, sizeof(buf));
							bufloc = 0;
							histpos = tmp;
							break;
						} else if (strncasecmp(histar[tmp], buf, bufmatchpos) == 0) {
							if (buf[bufloc] == 0)
								bufloc = strlen(histar[tmp]);
							strncpy(buf, histar[tmp], sizeof(buf)-1);
							histpos = tmp;
							break;
						}
				}
				break;
			case CONIO_KEY_INPUT_CURSOR_LEFT: /* Move left in the input line */
				if (bufloc > 0)
					bufloc--;
				break;
			case CONIO_KEY_INPUT_CURSOR_RIGHT: /* Move right in the input line */
				if (buf[bufloc] != 0)
					bufloc++;
				break;
			case CONIO_KEY_SPACE_OR_NBSP: /* In paste mode, an HTML hard space, otherwise a literal space */
				if ((inpaste == 0) || (inwhite == 0)) {
					ADDTOBUF(' ');
					inwhite = 1;
				} else {
					ADDTOBUF('&');
					ADDTOBUF('n');
					ADDTOBUF('b');
					ADDTOBUF('s');
					ADDTOBUF('p');
					ADDTOBUF(';');
					inwhite = 0;
				}
				break;
			case CONIO_KEY_TAB: /* An HTML tab (8 hard spaces) */
				ADDTOBUF('&');
				ADDTOBUF('n');
				ADDTOBUF('b');
				ADDTOBUF('s');
				ADDTOBUF('p');
				ADDTOBUF(';');
				ADDTOBUF(' ');
				ADDTOBUF('&');
				ADDTOBUF('n');
				ADDTOBUF('b');
				ADDTOBUF('s');
				ADDTOBUF('p');
				ADDTOBUF(';');
				ADDTOBUF(' ');
				ADDTOBUF('&');
				ADDTOBUF('n');
				ADDTOBUF('b');
				ADDTOBUF('s');
				ADDTOBUF('p');
				ADDTOBUF(';');
				ADDTOBUF(' ');
				ADDTOBUF('&');
				ADDTOBUF('n');
				ADDTOBUF('b');
				ADDTOBUF('s');
				ADDTOBUF('p');
				ADDTOBUF(';');
				ADDTOBUF('&');
				ADDTOBUF('n');
				ADDTOBUF('b');
				ADDTOBUF('s');
				ADDTOBUF('p');
				ADDTOBUF(';');
				break;
			case CONIO_KEY_TAB_BUDDY_NEXT: { /* In paste mode, an HTML tab (8 hard spaces), otherwise perform command completion or advance to the next window */
					int	temppaste = inpaste;
					char	*ptr;

					if (!temppaste && (buf[0] != 0) && (buf[0] != '/')) {
						fd_set	rfd;
						struct timeval	timeout;

						FD_ZERO(&rfd);
						FD_SET(STDIN_FILENO, &rfd);
						timeout.tv_sec = 0;
						timeout.tv_usec = 1;
						select(STDIN_FILENO+1, &rfd, NULL, NULL, &timeout);
						if (FD_ISSET(STDIN_FILENO, &rfd))
							temppaste = 1;
					}
					if (temppaste) {
						ADDTOBUF('&');
						ADDTOBUF('n');
						ADDTOBUF('b');
						ADDTOBUF('s');
						ADDTOBUF('p');
						ADDTOBUF(';');
						ADDTOBUF(' ');
						ADDTOBUF('&');
						ADDTOBUF('n');
						ADDTOBUF('b');
						ADDTOBUF('s');
						ADDTOBUF('p');
						ADDTOBUF(';');
						ADDTOBUF(' ');
						ADDTOBUF('&');
						ADDTOBUF('n');
						ADDTOBUF('b');
						ADDTOBUF('s');
						ADDTOBUF('p');
						ADDTOBUF(';');
						ADDTOBUF(' ');
						ADDTOBUF('&');
						ADDTOBUF('n');
						ADDTOBUF('b');
						ADDTOBUF('s');
						ADDTOBUF('p');
						ADDTOBUF(';');
						ADDTOBUF('&');
						ADDTOBUF('n');
						ADDTOBUF('b');
						ADDTOBUF('s');
						ADDTOBUF('p');
						ADDTOBUF(';');
						break;
					} else if ((bufloc > 0) && (buf[0] == '/')
						&& (buf[bufloc-1] != ' ')) {
						int	match;
						const char
							*str;

						if ((str = conio_tabcomplete(buf, bufloc, &match, NULL)) != NULL) {
							int	i;

							bufloc -= match;
							for (i = 0; i < match; i++)
								buf[bufloc++] = str[i];
							for (; str[i] != 0; i++)
								ADDTOBUF(str[i]);
							if (isalnum(str[i-1]))
								ADDTOBUF(' ');
						} else
							beep();

						break;
					} else if ((bufloc > 0) && (buf[0] != '/')
						&& inconn && (curconn->curbwin->et == CHAT)
						&& (*buf != ',')
						&& (((ptr = strchr(buf, ' ')) == NULL) || (*(ptr+1) == 0))) {
						int	i;

						namescomplete.buf = strdup(buf);
						namescomplete.len = bufloc;
						namescomplete.foundfirst = namescomplete.foundmatch = namescomplete.foundmult = 0;
						firetalk_chat_listmembers(curconn->conn, curconn->curbwin->winname);
						if (namescomplete.foundfirst && !namescomplete.foundmatch)
							firetalk_chat_listmembers(curconn->conn, curconn->curbwin->winname);
						if (namescomplete.foundfirst && !namescomplete.foundmatch)
							bufloc = strlen(buf);
						else if (namescomplete.foundmatch) {
							static char *lastcomplete = NULL;
							static int numcompletes = 1;

							if (numcompletes && (lastcomplete != NULL) && (strcmp(namescomplete.buf, lastcomplete) == 0)) {
								if (numcompletes == 1)
									echof(curconn, "TAB", "Please do not complete the same name twice. It is likely that %s already saw your initial address and can track your messages without additional help, or %s did not see your initial message and is not going to hear your followup messages either.\n", lastcomplete, lastcomplete);
								else if (numcompletes == 2)
									echof(curconn, "TAB", "If you find yourself sending a large number of messages to the same person in a group, it might be best to take the person aside to finish your conversation. Online, this can be accomplished by using <font color=\"#00FF00\">/msg %s yourmessage</font>.\n", lastcomplete);
								else if (numcompletes == 3)
									echof(curconn, "TAB", "Remember: This is a group you are addressing. If your conversation is directed at only one person, it may be best to hold it in private, using <font color=\"#00FF00\">/msg</font>. If the conversation is for the benefit of the entire audience, you should not prefix every message with an individual's name.\n");
								else {
									echof(curconn, "TAB", "If you really want to complete the same name multiple times, I'm not going to stop you. However, please try to think of how you would act if you were holding this conversation in person--including moving aside if the group area is too noisy.\n");
									numcompletes = 0;
								}

								if (numcompletes) {
									memset(buf, 0, sizeof(buf));
									inwhite = inpaste = bufloc = 0;
									numcompletes++;
								}
								free(lastcomplete);
								lastcomplete = NULL;
							} else {
								memset(buf, 0, sizeof(buf));
								inwhite = inpaste = bufloc = 0;
								for (i = 0; namescomplete.buf[i] != 0; i++)
									ADDTOBUF(namescomplete.buf[i]);
								if (namescomplete.foundmatch) {
									char	*delim = getvar(curconn, "addressdelim");

									if ((delim != NULL) && (*delim != 0))
										ADDTOBUF(*delim);
									else
										ADDTOBUF(',');
									ADDTOBUF(' ');
								}
								if (namescomplete.foundmult)
									bufloc = namescomplete.len;
								else
									bufloc = strlen(buf);
								if (numcompletes)
									STRREPLACE(lastcomplete, namescomplete.buf);
							}
						}
						free(namescomplete.buf);
						namescomplete.buf = NULL;
						namescomplete.foundfirst = namescomplete.foundmatch = namescomplete.foundmult = namescomplete.len = 0;
						break;
					}
				}
			case CONIO_KEY_BUDDY_NEXT: /* Advance to the next window */
				if (inconn_real) {
					assert(curconn->curbwin != NULL);
					curconn->curbwin = curconn->curbwin->next;
					nw_touchwin(&(curconn->curbwin->nwin));
					scrollbackoff = 0;
				}
				naim_changetime();
				bupdate();
				break;
			case CONIO_KEY_BUDDY_PREV: /* Go to the previous window */
				if (inconn_real) {
					buddywin_t	*bbefore = curconn->curbwin->next;
	
					while (bbefore->next != curconn->curbwin)
						bbefore = bbefore->next;
					curconn->curbwin = bbefore;
					nw_touchwin(&(bbefore->nwin));
					scrollbackoff = 0;
				}
				naim_changetime();
				bupdate();
				break;
			case CONIO_KEY_CONN_NEXT: /* Go to the next connection window (AIM, EFnet, etc.) */
				curconn = curconn->next;
				if (curconn->curbwin != NULL)
					nw_touchwin(&(curconn->curbwin->nwin));
				scrollbackoff = 0;
				naim_changetime();
				bupdate();
				break;
			case CONIO_KEY_CONN_PREV: { /* Go to the previous connection window */
					conn_t	*cbefore = curconn->next;
	
					while (cbefore->next != curconn)
						cbefore = cbefore->next;
					curconn = cbefore;
					scrollbackoff = 0;
					if (cbefore->curbwin != NULL)
						nw_touchwin(&(cbefore->curbwin->nwin));
				}
				naim_changetime();
				bupdate();
				break;
			case CONIO_KEY_INPUT_BACKSPACE: /* Delete the previous character in the input line */
				if (bufloc < 1)
					break;
				memmove(buf+bufloc-1, buf+bufloc,
					sizeof(buf)-bufloc);
				bufloc--;
				break;
			case CONIO_KEY_INPUT_DELETE: /* Delete the current character in the input line */
				memmove(buf+bufloc, buf+bufloc+1,
					sizeof(buf)-bufloc-1);
				break;
			case CONIO_KEY_INPUT_ENTER: /* In paste mode, an HTML newline, otherwise sends the current IM or executes the current command */
				{
					int	temppaste = inpaste;

					if (!temppaste && (buf[0] != 0) && (buf[0] != '/') && (getvar_int(curconn, "autopaste") != 0)) {
						fd_set	rfd;
						struct timeval	timeout;

						FD_ZERO(&rfd);
						FD_SET(STDIN_FILENO, &rfd);
						timeout.tv_sec = 0;
						timeout.tv_usec = 1;
						select(STDIN_FILENO+1, &rfd, NULL, NULL, &timeout);
						if (FD_ISSET(STDIN_FILENO, &rfd))
							temppaste = 1;
					}
					if (temppaste) {
						ADDTOBUF('<');
						ADDTOBUF('b');
						ADDTOBUF('r');
						ADDTOBUF('>');
						inwhite = 1;
						break;
					}
				}
				if (buf[0] == 0)
					break;

				histpos = histc-1;
				STRREPLACE(histar[histpos], buf);
				histpos = histc++;
				histar = realloc(histar, histc*sizeof(*histar));
				histar[histpos] = NULL;
				conio_handleline(buf);
			case CONIO_KEY_INPUT_KILL: /* Delete the entire input line */
				memset(buf, 0, sizeof(buf));
				inwhite = inpaste =
					bufloc = 0;
				break;
			case CONIO_KEY_INPUT_KILL_WORD: /* Delete the input line from the current character to the beginning of the previous word */
				if (bufloc > 0) {
					int	end = bufloc;

					bufloc--;
					while ((bufloc > 0) && isspace(buf[bufloc]))
						bufloc--;
					while ((bufloc > 0) && !isspace(buf[bufloc]))
						bufloc--;
					if (isspace(buf[bufloc]))
						bufloc++;
					//memmove(buf+bufloc, buf+end, strlen(buf+end)+1);
					memmove(buf+bufloc, buf+end, strlen(buf+end));
					memset(buf+strlen(buf)-(end-bufloc), 0, end-bufloc);
				}
				break;
			case CONIO_KEY_INPUT_KILL_EOL: /* Delete the input line from the current character to the end of the line */
				memset(buf+bufloc, 0, strlen(buf+bufloc));
				break;
			case CONIO_KEY_INPUT_PASTE: /* Alter the input handler to handle pasted text better */
				inpaste = (inpaste == 0);
				break;
			case CONIO_KEY_INPUT_SYM_LT: /* In paste mode, an HTML less-than, otherwise a literal less-than */
				if (inpaste)
					ADDTOBUF('<');
				else {
					ADDTOBUF('&');
					ADDTOBUF('l');
					ADDTOBUF('t');
					ADDTOBUF(';');
				}
				break;
			case CONIO_KEY_INPUT_SYM_GT: /* In paste mode, an HTML greater-than, otherwise a literal greater-than */
				if (inpaste)
					ADDTOBUF('>');
				else {
					ADDTOBUF('&');
					ADDTOBUF('g');
					ADDTOBUF('t');
					ADDTOBUF(';');
				}
				break;
			case CONIO_KEY_INPUT_SYM_AMP: /* In paste mode, an HTML ampersand, otherwise a literal ampersand */
				if (inpaste)
					ADDTOBUF('&');
				else {
					ADDTOBUF('&');
					ADDTOBUF('a');
					ADDTOBUF('m');
					ADDTOBUF('p');
					ADDTOBUF(';');
				}
				break;
			case CONIO_KEY_INPUT_ENT_BOLD: /* Toggle HTML bold mode */
				if (strncasecmp(buf+bufloc, "</B>", 4) == 0)
					bufloc += 4;
				else if (bufloc+sizeof("<I></I>")-1 < sizeof(buf)-1) {
					ADDTOBUF('<');
					ADDTOBUF('B');
					ADDTOBUF('>');
					ADDTOBUF('<');
					ADDTOBUF('/');
					ADDTOBUF('B');
					ADDTOBUF('>');
					bufloc -= 4;
				} else
					beep();
				break;
			case CONIO_KEY_INPUT_ENT_ITALIC: /* Toggle HTML italic (inverse) mode */
				if (strncasecmp(buf+bufloc, "</I>", 4) == 0)
					bufloc += 4;
				else if (bufloc+sizeof("<I></I>")-1 < sizeof(buf)-1) {
					ADDTOBUF('<');
					ADDTOBUF('I');
					ADDTOBUF('>');
					ADDTOBUF('<');
					ADDTOBUF('/');
					ADDTOBUF('I');
					ADDTOBUF('>');
					bufloc -= 4;
				} else
					beep();
				break;
			case CONIO_KEY_WINLIST_HIDE: /* Cycle between always visible, always hidden, or auto-hidden */
				if (changetime == 0)
					changetime = nowf;
				else if (changetime == -1)
					changetime = 0;
				else
					changetime = -1;
				break;
			case CONIO_KEY_STATUS_DISPLAY: /* Display or hide the status console */
				if (consolescroll == -1) {
					if (secs_getvar_int("quakestyle") == 1) {
						quakeoff = 2;
						nw_mvwin(&win_info, faimconf.wstatus.starty+2*faimconf.wstatus.widthy/3+1,
							faimconf.winfo.startx);
						nw_mvwin(&win_input, faimconf.wstatus.starty+2*faimconf.wstatus.widthy/3,
							faimconf.winput.startx);
					}
					consolescroll = 0;
				} else {
					if (secs_getvar_int("quakestyle") == 1) {
						nw_mvwin(&win_info, faimconf.winfo.starty,
							faimconf.winfo.startx);
						nw_mvwin(&win_input, faimconf.winput.starty,
							faimconf.winput.startx);
					}
					quakeoff = 0;
					consolescroll = -1;
				}
				bupdate();
				break;
			case CONIO_KEY_STATUS_POKE: /* Bring the status console down for $autohide seconds */
				naim_lastupdate(curconn);
				bupdate();
				break;
			case CONIO_KEY_BUDDY_SCROLL_BACK: /* Scroll the current window backwards (up) */
				if (consolescroll == -1) {
					scrollbackoff += faimconf.wstatus.widthy-2;
					if (scrollbackoff >= faimconf.wstatus.pady-faimconf.wstatus.widthy)
						scrollbackoff = faimconf.wstatus.pady-faimconf.wstatus.widthy-1;
				} else {
					consolescroll += 2*faimconf.wstatus.widthy/3-2;
					if (consolescroll >= faimconf.wstatus.pady-2*faimconf.wstatus.widthy/3)
						consolescroll = faimconf.wstatus.pady-2*faimconf.wstatus.widthy/3-1;
				}
				break;
			case CONIO_KEY_BUDDY_SCROLL_FORWARD: /* Scroll the current window forwards in time */
				if (consolescroll == -1) {
					scrollbackoff -= faimconf.wstatus.widthy-2;
					if (scrollbackoff < 0)
						scrollbackoff = 0;
				} else {
					consolescroll -= 2*faimconf.wstatus.widthy/3-2;
					if (consolescroll < 0)
						consolescroll = 0;
				}
				break;
			case CONIO_KEY_INPUT_CURSOR_HOME: /* Jump to the beginning of the input line */
				bufloc = 0;
				break;
			case CONIO_KEY_INPUT_CURSOR_END: /* Jump to the end of the input line */
				bufloc = strlen(buf);
				break;
			case CONIO_KEY_INPUT_CURSOR_HOME_END: /* Jump between the beginning and end of the input line */
				if (bufloc == 0)
					bufloc = strlen(buf);
				else
					bufloc = 0;
				break;
		    }
		} else if (binding != NULL)
			secs_script_parse(binding);
		if (bindfunc != NULL)
			bindfunc(buf, &bufloc);
	} else if (naimisprint(c)) {
		ADDTOBUF(c);
		inwhite = 0;
	} else {
		char	numbuf[5];
		int	i;

		snprintf(numbuf, sizeof(numbuf), "%i", c);
		ADDTOBUF('&');
		ADDTOBUF('#');
		for (i = 0; numbuf[i] != 0; i++)
			ADDTOBUF(numbuf[i]);
		ADDTOBUF(';');
	}

	withtextcomp = 0;
	nw_erase(&win_input);
	{
		int	off = (bufloc < faimconf.winput.widthx)?0:
				faimconf.winput.widthx+(faimconf.winput.widthx-10)*((bufloc-faimconf.winput.widthx)/(faimconf.winput.widthx-10))-10;

		if (buf[bufloc] == 0) {
			int	match;
			const char
				*str,
				*desc;

			nw_printf(&win_input, C(INPUT,TEXT), 1, "%s", buf+off);
			if ((bufloc > 1) && (buf[0] == '/') && (buf[bufloc-1] != ' ')
				&& ((str = conio_tabcomplete(buf, bufloc, &match, &desc)) != NULL)
				&& (str[match] != 0)) {
				nw_printf(&win_input, CI(INPUT,EVENT), 0, "%c", 
					str[match]);
				nw_printf(&win_input, C(INPUT,EVENT), 0, "%s", 
					str+match+1);
				if (desc != NULL)
					nw_printf(&win_input, C(INPUT,TEXT), 0, " %s", desc);
				withtextcomp = 1;
			} else if ((bufloc > 5) && (buf[0] != '/')) {
				static int	tmp = -1;

				if ((tmp == -1) || (strncasecmp(histar[tmp], buf, bufloc) != 0))
					for (tmp = histpos-1; tmp >= 0; tmp--)
						if (strncasecmp(histar[tmp], buf, bufloc) == 0)
							break;

				if ((tmp >= 0) && (histar[tmp][bufloc] != 0)) {
					withtextcomp = 2;
					nw_printf(&win_input, CI(INPUT,EVENT), 0, "%c", 
						histar[tmp][bufloc]);
					nw_printf(&win_input, C(INPUT,EVENT), 0, "%s", 
						histar[tmp]+bufloc+1);
				} else
					nw_printf(&win_input, CI(INPUT,TEXT), 0, " ");
			} else
				nw_printf(&win_input, CI(INPUT,TEXT), 0, " ");
		} else {
			int	len = bufloc-off;

			nw_printf(&win_input, C(INPUT,TEXT), 1, "%.*s", len,
				buf+off);
			nw_printf(&win_input, CI(INPUT,TEXT), 0, "%c",
				buf[bufloc]);
			if (len < faimconf.winput.widthx-1)
				nw_printf(&win_input, C(INPUT,TEXT), 1, "%s", 
					buf+off+len+1);
		}
	}
	nw_touchwin(&win_input);
	nw_touchwin(&win_info);
}

void	gotkey(int c) {
	fd_set	rfd;
	struct timeval	timeout = { 0, 0 };

	if (statusbar_text != NULL) {
		free(statusbar_text);
		statusbar_text = NULL;
	}

	gotkey_real(c);

	FD_ZERO(&rfd);
	FD_SET(STDIN_FILENO, &rfd);
	while (select(STDIN_FILENO+1, &rfd, NULL, NULL, &timeout) > 0)
		gotkey_real(nw_getch());
}
