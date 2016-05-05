
#define C_STATUS	0x00
#define C_INCHAT	0x01
#define C_INUSER	0x02
#define C_NOTSTATUS	(C_INCHAT|C_INUSER)
#define C_ANYWHERE	0xFF

#define CONIOARGS	(conn_t *conn, int argc, const char **args)
#define CONIOFUNC2(x)	void x CONIOARGS

#ifndef CONIOCPP
# define CONIOFUNC(x)	void conio_ ## x CONIOARGS
# define CONIOALIA(x)
# define CONIOWHER(x)
# define CONIODESC(x)
# define CONIOAREQ(x,y)
# define CONIOAOPT(x,y)
#endif

#ifndef CONIO_NOPROTOS

CONIOFUNC(jump);
CONIOFUNC(msg);
CONIOFUNC(addbuddy);
CONIOFUNC(exit);
CONIOFUNC(save);
CONIOFUNC(connect);
CONIOFUNC(jumpback);
CONIOFUNC(info);
CONIOFUNC(eval);
CONIOFUNC(say);
CONIOFUNC(me);
CONIOFUNC(open);
CONIOFUNC(close);
CONIOFUNC(closeall);
CONIOFUNC(ctcp);
CONIOFUNC(clear);
CONIOFUNC(clearall);
CONIOFUNC(load);
CONIOFUNC(away);
CONIOFUNC(names);
CONIOFUNC(join);
CONIOFUNC(namebuddy);
CONIOFUNC(tagbuddy);
CONIOFUNC(delbuddy);
CONIOFUNC(op);
CONIOFUNC(deop);
CONIOFUNC(topic);
CONIOFUNC(kick);
CONIOFUNC(invite);
CONIOFUNC(help);
CONIOFUNC(unblock);
CONIOFUNC(block);
CONIOFUNC(ignore);
CONIOFUNC(chains);
CONIOFUNC(filter);
CONIOFUNC(warn);
CONIOFUNC(nick);
CONIOFUNC(echo);
CONIOFUNC(readprofile);
CONIOFUNC(accept);
CONIOFUNC(offer);
CONIOFUNC(setcol);
CONIOFUNC(setpriv);
CONIOFUNC(bind);
CONIOFUNC(alias);
CONIOFUNC(set);
CONIOFUNC(newconn);
CONIOFUNC(delconn);
CONIOFUNC(server);
CONIOFUNC(disconnect);
CONIOFUNC(winlist);
CONIOFUNC(exec);
CONIOFUNC(detach);
CONIOFUNC(dlsym);
CONIOFUNC(modlist);
CONIOFUNC(modload);
CONIOFUNC(modunload);
CONIOFUNC(resize);
CONIOFUNC(status);

#endif

typedef struct {
	const char
		*c;
	CONIOFUNC2((*func));
	const char
		*aliases[CONIO_MAXPARMS],
		*desc;
	const struct {
		const char
			required,
			type,
			*name;
	}	args[CONIO_MAXPARMS];
	int	minarg,
		maxarg,
		where;
} cmdar_t;

#ifndef CONIO_C
extern cmdar_t
	cmdar[];
extern const int
	cmdc;
#else
cmdar_t	cmdar[] = {

	{ "jump",	conio_jump,	{ NULL },	"Go to the specified window or the next 'active' one",	{ { 0, 'W', "winname" }, { -1, -1, NULL } },	0,	1,	C_ANYWHERE },
	{ "msg",	conio_msg,	{ "m", "im", NULL },	"Send a message; as in /msg naimhelp naim is cool!",	{ { 1, 'W', "name" }, { 1, 's', "message" }, { -1, -1, NULL } },	2,	2,	C_ANYWHERE },
	{ "addbuddy",	conio_addbuddy,	{ "add", "friend", "groupbuddy", NULL },	"Add someone to your buddy list or change their group membership",	{ { 1, 'B', "account" }, { 0, 's', "group" }, { 0, 's', "realname" }, { -1, -1, NULL } },	1,	3,	C_ANYWHERE },
	{ "exit",	conio_exit,	{ "quit", NULL },	"Disconnect and exit naim",	{ { -1, -1, NULL } },	0,	0,	C_ANYWHERE },
	{ "save",	conio_save,	{ NULL },	"Write current settings to ~/.naimrc to be loaded at startup",	{ { 0, 's', "filename" }, { -1, -1, NULL } },	0,	1,	C_ANYWHERE },
	{ "connect",	conio_connect,	{ NULL },	"Connect to a service",	{ { 0, 's', "name" }, { 0, 's', "server" }, { 0, 'i', "port" }, { -1, -1, NULL } },	0,	3,	C_ANYWHERE },
	{ "jumpback",	conio_jumpback,	{ NULL },	"Go to the previous window",	{ { -1, -1, NULL } },	0,	0,	C_ANYWHERE },
	{ "info",	conio_info,	{ "whois", "wi", NULL },	"Retrieve a user profile",	{ { 0, 'E', "name" }, { -1, -1, NULL } },	0,	1,	C_ANYWHERE },
	{ "eval",	conio_eval,	{ NULL },	"Evaluate a command with $-variable substitution",	{ { 1, 's', "script" }, { -1, -1, NULL } },	1,	1,	C_ANYWHERE },
	{ "say",	conio_say,	{ NULL },	"Send a message to the current window; as in /say I am happy",	{ { 1, 's', "message" }, { -1, -1, NULL } },	1,	1,	C_NOTSTATUS },
	{ "me",	conio_me,	{ NULL },	"Send an 'action' message to the current window; as in /me is happy",	{ { 1, 's', "message" }, { -1, -1, NULL } },	1,	1,	C_NOTSTATUS },
	{ "open",	conio_open,	{ "window", NULL },	"Open a query window",	{ { 1, 'B', "name" }, { -1, -1, NULL } },	1,	1,	C_ANYWHERE },
	{ "close",	conio_close,	{ "endwin", "part", NULL },	"Close a query window or leave a discussion",	{ { 0, 'W', "winname" }, { -1, -1, NULL } },	0,	1,	C_NOTSTATUS },
	{ "closeall",	conio_closeall,	{ NULL },	"Close stale windows for offline buddies",	{ { -1, -1, NULL } },	0,	0,	C_ANYWHERE },
	{ "ctcp",	conio_ctcp,	{ NULL },	"Send Client To Client Protocol request to someone",	{ { 1, 'W', "name" }, { 0, 's', "requestname" }, { 0, 's', "message" }, { -1, -1, NULL } },	1,	3,	C_ANYWHERE },
	{ "clear",	conio_clear,	{ NULL },	"Temporarily blank the scrollback for the current window",	{ { -1, -1, NULL } },	0,	0,	C_ANYWHERE },
	{ "clearall",	conio_clearall,	{ NULL },	"Perform a /clear on all open windows",	{ { -1, -1, NULL } },	0,	0,	C_ANYWHERE },
	{ "load",	conio_load,	{ NULL },	"Load a command file (such as .naimrc)",	{ { 1, 'F', "filename" }, { -1, -1, NULL } },	1,	1,	C_ANYWHERE },
	{ "away",	conio_away,	{ NULL },	"Set or unset away status",	{ { 0, 's', "message" }, { -1, -1, NULL } },	0,	1,	C_ANYWHERE },
	{ "names",	conio_names,	{ "buddylist", NULL },	"Display buddy list or members of a chat",	{ { 0, 'C', "chat" }, { -1, -1, NULL } },	0,	1,	C_ANYWHERE },
	{ "join",	conio_join,	{ NULL },	"Participate in a chat",	{ { 1, 's', "chat" }, { 0, 's', "key" }, { -1, -1, NULL } },	1,	2,	C_ANYWHERE },
	{ "namebuddy",	conio_namebuddy,	{ NULL },	"Change the real name for a buddy",	{ { 1, 'B', "name" }, { 0, 's', "realname" }, { -1, -1, NULL } },	1,	2,	C_ANYWHERE },
	{ "tagbuddy",	conio_tagbuddy,	{ "tag", NULL },	"Mark a buddy with a reminder message",	{ { 1, 'B', "name" }, { 0, 's', "note" }, { -1, -1, NULL } },	1,	2,	C_ANYWHERE },
	{ "delbuddy",	conio_delbuddy,	{ NULL },	"Remove someone from your buddy list",	{ { 0, 'B', "name" }, { -1, -1, NULL } },	0,	1,	C_ANYWHERE },
	{ "op",	conio_op,	{ NULL },	"Give operator privilege",	{ { 1, 'B', "name" }, { -1, -1, NULL } },	1,	1,	C_INCHAT },
	{ "deop",	conio_deop,	{ NULL },	"Remove operator privilege",	{ { 1, 'B', "name" }, { -1, -1, NULL } },	1,	1,	C_INCHAT },
	{ "topic",	conio_topic,	{ NULL },	"View or change current chat topic",	{ { 0, 's', "topic" }, { -1, -1, NULL } },	0,	1,	C_INCHAT },
	{ "kick",	conio_kick,	{ NULL },	"Temporarily remove someone from a chat",	{ { 1, 'B', "name" }, { 0, 's', "reason" }, { -1, -1, NULL } },	1,	2,	C_INCHAT },
	{ "invite",	conio_invite,	{ NULL },	"Invite someone to a chat",	{ { 1, 'B', "name" }, { 0, 's', "chat" }, { -1, -1, NULL } },	1,	2,	C_INCHAT },
	{ "help",	conio_help,	{ "about", NULL },	"Display topical help on using naim",	{ { 0, 's', "topic" }, { -1, -1, NULL } },	0,	1,	C_ANYWHERE },
	{ "unblock",	conio_unblock,	{ "unignore", NULL },	"Remove someone from the ignore list",	{ { 1, 'B', "name" }, { -1, -1, NULL } },	1,	1,	C_ANYWHERE },
	{ "block",	conio_block,	{ NULL },	"Server-enforced /ignore",	{ { 1, 'B', "name" }, { 0, 's', "reason" }, { -1, -1, NULL } },	1,	2,	C_ANYWHERE },
	{ "ignore",	conio_ignore,	{ NULL },	"Ignore all private/public messages",	{ { 0, 'B', "name" }, { 0, 's', "reason" }, { -1, -1, NULL } },	0,	2,	C_ANYWHERE },
	{ "chains",	conio_chains,	{ "tables", NULL },	"Manipulate data control tables",	{ { 0, 's', "chain" }, { -1, -1, NULL } },	0,	1,	C_ANYWHERE },
	{ "filter",	conio_filter,	{ NULL },	"Manipulate content filters",	{ { 0, 's', "table" }, { 0, 's', "target" }, { 0, 's', "action" }, { -1, -1, NULL } },	0,	3,	C_ANYWHERE },
	{ "warn",	conio_warn,	{ NULL },	"Send a warning about someone",	{ { 1, 'B', "name" }, { -1, -1, NULL } },	1,	1,	C_ANYWHERE },
	{ "nick",	conio_nick,	{ NULL },	"Change or reformat your name",	{ { 1, 's', "name" }, { -1, -1, NULL } },	1,	1,	C_ANYWHERE },
	{ "echo",	conio_echo,	{ NULL },	"Display something on the screen with $-variable expansion",	{ { 1, 's', "script" }, { -1, -1, NULL } },	1,	1,	C_ANYWHERE },
	{ "readprofile",	conio_readprofile,	{ NULL },	"Read your profile from disk",	{ { 1, 'F', "filename" }, { -1, -1, NULL } },	1,	1,	C_ANYWHERE },
	{ "accept",	conio_accept,	{ NULL },	"EXPERIMENTAL Accept a file transfer request in the current window",	{ { 1, 'W', "filename" }, { -1, -1, NULL } },	1,	1,	C_NOTSTATUS },
	{ "offer",	conio_offer,	{ NULL },	"EXPERIMENTAL Offer a file transfer request to someone",	{ { 1, 'B', "name" }, { 1, 'F', "filename" }, { -1, -1, NULL } },	2,	2,	C_ANYWHERE },
	{ "setcol",	conio_setcol,	{ NULL },	"View or change display colors",	{ { 0, 's', "eventname" }, { 0, 's', "colorname" }, { 0, 's', "colormodifier" }, { -1, -1, NULL } },	0,	3,	C_ANYWHERE },
	{ "setpriv",	conio_setpriv,	{ NULL },	"Change your privacy mode",	{ { 1, 's', "mode" }, { -1, -1, NULL } },	1,	1,	C_ANYWHERE },
	{ "bind",	conio_bind,	{ NULL },	"View or change key bindings",	{ { 0, 's', "keyname" }, { 0, 's', "script" }, { -1, -1, NULL } },	0,	2,	C_ANYWHERE },
	{ "alias",	conio_alias,	{ NULL },	"Create a new command alias",	{ { 1, 's', "commandname" }, { 1, 's', "script" }, { -1, -1, NULL } },	2,	2,	C_ANYWHERE },
	{ "set",	conio_set,	{ NULL },	"View or change the value of a configuration or session variable; see /help settings",	{ { 0, 'V', "varname" }, { 0, 's', "value" }, { 0, 's', "dummy" }, { -1, -1, NULL } },	0,	3,	C_ANYWHERE },
	{ "newconn",	conio_newconn,	{ NULL },	"Open a new connection window",	{ { 0, 's', "label" }, { 0, 's', "protocol" }, { -1, -1, NULL } },	0,	2,	C_ANYWHERE },
	{ "delconn",	conio_delconn,	{ NULL },	"Close a connection window",	{ { 0, 's', "label" }, { -1, -1, NULL } },	0,	1,	C_ANYWHERE },
	{ "server",	conio_server,	{ NULL },	"Connect to a service",	{ { 0, 's', "server" }, { 0, 'i', "port" }, { -1, -1, NULL } },	0,	2,	C_ANYWHERE },
	{ "disconnect",	conio_disconnect,	{ NULL },	"Disconnect from a server",	{ { -1, -1, NULL } },	0,	0,	C_ANYWHERE },
	{ "winlist",	conio_winlist,	{ NULL },	"Switch the window list window between always visible or always hidden or auto-hidden",	{ { 0, 's', "visibility" }, { -1, -1, NULL } },	0,	1,	C_ANYWHERE },
	{ "exec",	conio_exec,	{ NULL },	"Execute a shell command; as in /exec -o uname -a",	{ { 1, 's', "command" }, { -1, -1, NULL } },	1,	1,	C_ANYWHERE },
	{ "detach",	conio_detach,	{ NULL },	"Disconnect from the current session",	{ { -1, -1, NULL } },	0,	0,	C_ANYWHERE },
	{ "dlsym",	conio_dlsym,	{ NULL },	NULL,	{ { 1, 's', "symbol" }, { -1, -1, NULL } },	1,	1,	C_ANYWHERE },
	{ "modlist",	conio_modlist,	{ NULL },	"Search for and list all potential and resident naim modules",	{ { -1, -1, NULL } },	0,	0,	C_ANYWHERE },
	{ "modload",	conio_modload,	{ NULL },	"Load and initialize a dynamic module",	{ { 1, 'F', "module" }, { 0, 's', "options" }, { -1, -1, NULL } },	1,	2,	C_ANYWHERE },
	{ "modunload",	conio_modunload,	{ NULL },	"Deinitialize and unload a resident module",	{ { 1, 's', "module" }, { -1, -1, NULL } },	1,	1,	C_ANYWHERE },
	{ "resize",	conio_resize,	{ NULL },	"Resize all windows",	{ { 0, 'i', "height" }, { -1, -1, NULL } },	0,	1,	C_ANYWHERE },
	{ "status",	conio_status,	{ NULL },	"Connection status report",	{ { 0, 's', "connection" }, { -1, -1, NULL } },	0,	1,	C_ANYWHERE },

};
const int
	cmdc = sizeof(cmdar)/sizeof(*cmdar);
#endif

