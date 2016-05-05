/*  _ __   __ _ ___ __  __
** | '_ \ / _` |_ _|  \/  | naim
** | | | | (_| || || |\/| | Copyright 1998-2005 Daniel Reed <n@ml.org>
** |_| |_|\__,_|___|_|  |_| ncurses-based chat client
*/
#include <naim/naim.h>
#include <naim/modutil.h>
#include <ltdl.h>

#include <signal.h>
#include <sys/wait.h>

#include "naim-int.h"
#include "naimrc_sample.h"
#include "snapshot.h"
#include "help.h"

extern int
	wsetup_called;
extern mod_fd_list_t
	*mod_fd_listar;
extern int
	mod_fd_listc;

extern faimconf_t
	faimconf G_GNUC_INTERNAL;
extern int
	stayconnected G_GNUC_INTERNAL,
	printtitle G_GNUC_INTERNAL;
extern time_t
	startuptime G_GNUC_INTERNAL,
	now G_GNUC_INTERNAL;
extern double
	nowf G_GNUC_INTERNAL,
	changetime G_GNUC_INTERNAL;
extern const char
	*home G_GNUC_INTERNAL,
	*sty G_GNUC_INTERNAL,
	*invocation G_GNUC_INTERNAL;
extern char
	naimrcfilename[1024] G_GNUC_INTERNAL;
conn_t	*curconn = NULL;
faimconf_t
	faimconf;
int	stayconnected = 0,
	printtitle = 0;
time_t	startuptime = 0,
	now = 0;
double	nowf = 0.0,
	changetime = 0.0;
const char
	*home = NULL,
	*sty = NULL,
	*invocation = NULL;
char	naimrcfilename[1024];




#ifdef KEY_MAX
# ifdef SIGHUP
#  define KEY_SIGHUP (2*KEY_MAX+SIGHUP)
# endif
# ifdef SIGUSR1
#  define KEY_SIGUSR1 (2*KEY_MAX+SIGUSR1)
# endif
# ifdef SIGUSR2
#  define KEY_SIGUSR2 (2*KEY_MAX+SIGUSR2)
# endif
#endif

static void
	dummy(int sig) {
	signal(sig, dummy);
	switch (sig) {
#ifdef KEY_SIGHUP
	  case SIGHUP:
		gotkey(KEY_SIGHUP);
		break;
#endif
#ifdef KEY_SIGUSR1
	  case SIGUSR1:
		gotkey(KEY_SIGUSR1);
		break;
#endif
#ifdef KEY_SIGUSR2
	  case SIGUSR2:
		gotkey(KEY_SIGUSR2);
		break;
#endif
	  default:
		echof(curconn, "SIGNAL", "Got signal %i!\n", sig);
	}
}

#ifdef HAVE_BACKTRACE
static void
	naim_segfault(int sig) {
	void	*bt[25];
	size_t	len;

	wshutitdown();
	len = backtrace(bt, sizeof(bt)/sizeof(*bt));
	fprintf(stderr, "\r\n\r\n\r\nSegmentation violation; partial symbolic backtrace:\r\n");
	backtrace_symbols_fd(bt, len, STDERR_FILENO);
	fprintf(stderr, "\r\nThis information is not a replacement for running naim in gdb. If you are interested in debugging this problem, please re-run naim within gdb and reproduce the fault. When you are presented with the (gdb) prompt again, type \"backtrace\" to receive the full symbolic backtrace and mail this to Daniel Reed <n@ml.org>.\r\n\r\n");
	abort();
}
#endif

static void
	childexit(int sig) {
	int	saveerrno = errno;

	signal(sig, childexit);
	while (waitpid(-1, NULL, WNOHANG) > 0)
		;
	errno = saveerrno;
}

#ifdef HAVE_GETOPT_LONG
# include <getopt.h>
#endif

#ifndef FAKE_MAIN_STUB
int	main(int argc, char **args) {
#else
int	main_stub(int argc, char **args) {
#endif
	time_t	lastcycle = 0;

	{
		char	*term = getenv("TERM");

		if (term != NULL) {
			if (strcmp(term, "ansi") == 0) {
				int	i;

				printf("The environment variable $TERM is set to \"ansi\", but that does not mean anything. I am going to reset it to \"linux\", since that seems to work best in most situations. If you are using the Windows telnet client, you might want to grab PuTTy, available for free from http://www.tucows.com/\n");
				for (i = 5; i > 0; i--) {
					printf("\r%i", i);
					fflush(stdout);
					sleep(1);
				}
				putenv("TERM=linux");
			} else if ((strncmp(term, "xterm", sizeof("xterm")-1) == 0)
				|| (strncmp(term, "screen", sizeof("screen")-1) == 0)) {
				printtitle = 1;
				nw_titlef("");
			}
		}
	}

	sty = getenv("STY");
	if ((home = getenv("HOME")) == NULL)
		home = "/tmp";

#ifdef HAVE_GETOPT_LONG
	while (1) {
		int	i, c,
			option_index = 0;
		static struct option long_options[] = {
# ifdef ALLOW_DETACH
			{ "noscreen",	0,	NULL,	0   },
# endif
			{ "help",	0,	NULL,	'h' },
			{ "version",	0,	NULL,	'V' },
			{ NULL,		0,	NULL,	0   },
		};

		c = getopt_long(argc, args, "hV", long_options, &option_index);
		if (c == -1)
			break;

		switch (c) {
		  case 0:
			break;
		  case 'h':
			printf("Usage:\n");
			printf("    naim [options]\n");
			printf("    nirc [nickname [server]] [options]\n");
			printf("    nicq [options]\n");
			printf("    nlily [options]\n");
			printf("\n");
			printf("Options:\n");
			printf("  -h, --help		Print this message and then exit.\n");
# ifdef ALLOW_DETACH
			printf("      --noscreen	Prevent naim from restarting inside screen.\n");
# endif
			printf("  -V, --version		Print version information and then exit.\n");
			printf("\n");
			printf("See `man naim' for more detailed help.\n");
			printf("\n");
			for (i = 0; about[i] != NULL; i++)
				printf("%s\n", about[i]);
			return(0);
		  case 'V':
			if (strcmp(args[0], "naim") == 0)
				printf("naim " PACKAGE_VERSION NAIM_SNAPSHOT "\n");
			else
				printf("%s (naim) " PACKAGE_VERSION NAIM_SNAPSHOT "\n", args[0]);
			return(0);
		  default:
			printf("Try `%s --help' for more information.\n", args[0]);
			return(1);
		}
	}
#else
	if ((argc > 1) && (args[1][0] == '-') && (strcmp(args[1], "--noscreen") != 0)) {
		if (       (strcasecmp(args[1], "--version") == 0)
			|| (strcasecmp(args[1], "-V") == 0)) {
			if (strcmp(args[0], "naim") == 0)
				printf("naim " PACKAGE_VERSION NAIM_SNAPSHOT "\n");
			else
				printf("%s (naim) " PACKAGE_VERSION NAIM_SNAPSHOT "\n", args[0]);
		} else if ((strcasecmp(args[1], "--help") == 0)
			|| (strcasecmp(args[1], "-H") == 0)) {
			int	i;

			printf("Usage:\n");
			printf("    naim [options]\n");
			printf("    nirc [nickname [server]] [options]\n");
			printf("    nicq [options]\n");
			printf("    nlily [options]\n");
			printf("\n");
			printf("Options:\n");
			printf("  -H, --help		Print this message and then exit.\n");
# ifdef ALLOW_DETACH
			printf("      --noscreen	Prevent naim from restarting inside screen.\n");
# endif
			printf("  -V, --version		Print version information and then exit.\n");
			printf("\n");
			printf("See `man naim' for more detailed help.\n");
			printf("\n");
			for (i = 0; about[i] != NULL; i++)
				printf("%s\n", about[i]);
		} else {
			printf("%s: unrecognized option `%s'\n", args[0], args[1]);
			printf("Try `%s --help' for more information.\n", args[0]);
			return(1);
		}
		return(0);
	}
#endif

#ifdef ALLOW_DETACH
	if ((argc < 2) || (strcmp(args[1], "--noscreen") != 0)) {
		if (sty == NULL) {
			printf("Attempting to restart from within screen (run %s --noscreen to skip this behaviour)...\n",
				args[0]);
			execlp("screen", "screen", "-e", "^Qq", args[0], "--noscreen", NULL);
			printf("Unable to start screen (%s), continuing...\n",
				strerror(errno));
		} else {
			printf("Attempting to restart from within this screen session.\n");
			execlp("screen", "screen", args[0], "--noscreen", NULL);
			printf("Unable to open a new screen window (%s), continuing...\n",
				strerror(errno));
		}
	}
#endif

	changetime = nowf = now = startuptime = time(NULL);

	secs_init();

	initscr();
	wsetup_called = 2;
	rc_initdefs(&faimconf);
	endwin();

	wsetup_called = 0;
	wsetup();
	gotkey(0);	// initialize gotkey() buffer
	updateidletime();

#ifdef HAVE_BACKTRACE
	signal(SIGSEGV, naim_segfault);
#endif
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGPIPE, dummy);
	signal(SIGALRM, dummy);
#ifdef KEY_SIGHUP
	signal(SIGHUP, dummy);
#endif
#ifdef KEY_SIGUSR1
	signal(SIGUSR1, dummy);
#endif
#ifdef KEY_SIGUSR2
	signal(SIGUSR2, dummy);
#endif
	childexit(SIGCHLD);

	if (lt_dlinit() != 0) {
		echof(curconn, NULL, "Unable to initialize module handler: %s.\n",
			lt_dlerror());
		return(1);
	}
	lt_dlsetsearchpath(DLSEARCHPATH);

	{
		const char	*args[] = { "dummy", "AIM" };
		extern void	conio_newconn(void *, int, const char **);

		conio_newconn(NULL, 2, args);
	}

	{
		char	buf[256];
		long	want_aim = 0,
			want_irc = 0,
			want_icq = 0,
			want_lily = 0;

		chdir(home);

		invocation = naim_basename(args[0]);
		if (*invocation == 'n')
			invocation++;

		if (strcmp(invocation, "irc") == 0)
			want_irc = 1;
		else if (strcmp(invocation, "icq") == 0)
			want_icq = 1;
		else if (strcmp(invocation, "lily") == 0)
			want_lily = 1;
		else {
			if (strcmp(invocation, "aim") != 0)
				invocation = naim_basename(args[0]);
			want_aim = 1;
		}

		secs_makevar_int("want_aim", want_aim, 'B', NULL);
		secs_makevar_int("want_irc", want_irc, 'B', NULL);
		secs_makevar_int("want_icq", want_icq, 'B', NULL);
		secs_makevar_int("want_lily", want_lily, 'B', NULL);

		secs_makevar_int("sys_argc", argc-1, 'I', NULL);
		secs_makevar("sys_user", getenv("USER"), 'S');
		if (argc > 1)
			secs_makevar("sys_arg1", args[1], 'S');
		if (argc > 2)
			secs_makevar("sys_arg2", args[2], 'S');

		if (getenv("NAIMRC") != NULL) {
			strncpy(naimrcfilename, getenv("NAIMRC"),
				sizeof(naimrcfilename)-1);
			naimrcfilename[sizeof(naimrcfilename)-1] = 0;
		} else
			snprintf(naimrcfilename, sizeof(naimrcfilename), "%s/.n%src",
				home, invocation);

		echof(curconn, NULL, "Attempting to load %s\n", naimrcfilename);
		if (naim_read_config(naimrcfilename) == 0) {
			int	i;

			for (i = 0; i < sizeof(naimrc_sample)/sizeof(*naimrc_sample); i++)
				conio_handlecmd(naimrc_sample[i]);
		} else {
			conn_t	*conn = curconn;

			do {
				snprintf(buf, sizeof(buf), "%s:READPROFILE %s/.n%sprofile",
					conn->winname, home, invocation);
				conio_handlecmd(buf);
			} while ((conn = conn->next) != curconn);
		}

		secs_setvar("want_aim", "");
		secs_setvar("want_irc", "");
		secs_setvar("want_icq", "");
		secs_setvar("want_lily", "");
		secs_setvar("sys_arg2", "");
		secs_setvar("sys_arg1", "");
		secs_setvar("sys_argc", "");
		secs_setvar("sys_user", "");
	}

	if (curconn == NULL)
		abort();

	if (curconn->next != curconn) {
		const char	*args[] = { "dummy" };
		extern void	conio_delconn(void *, int, const char **);

		conio_delconn(curconn, 1, args);
	}

	statrefresh();
	doupdate();

	stayconnected = 1;

	conio_hook_init();
	fireio_hook_init();
	hamster_hook_init();

	while (stayconnected) {
		fd_set	rfd, wfd;
		struct timeval	timeout;
		time_t	now60;
		int	i, autohide, maxfd = STDIN_FILENO;

		now = time(NULL);
		autohide = secs_getvar_int("autohide");
		if (((nowf - changetime) > autohide) 
			&& ((nowf - curconn->lastupdate) > autohide)) {
			timeout.tv_sec = 60 - (now%60);
			timeout.tv_usec = 0;
		} else {
			if (((nowf - curconn->lastupdate) <= SLIDETIME)
			 || ((nowf - curconn->lastupdate) >= (autohide - SLIDETIME))
			 || ((nowf - changetime) <= autohide)) {
				timeout.tv_sec = 0;
				timeout.tv_usec = 50000;
			} else {
				double	ttt;

				ttt = autohide - (nowf - curconn->lastupdate) - SLIDETIME;
				timeout.tv_sec = ttt;
				timeout.tv_usec = (ttt - timeout.tv_sec)*1000000;
			}
		}

		FD_ZERO(&rfd);
		FD_ZERO(&wfd);
		FD_SET(STDIN_FILENO, &rfd);
		for (i = 0; i < mod_fd_listc; i++) {
			if (mod_fd_listar[i].type & (O_RDONLY+1))
				FD_SET(mod_fd_listar[i].fd, &rfd);
			if (mod_fd_listar[i].type & (O_WRONLY+1))
				FD_SET(mod_fd_listar[i].fd, &wfd);
			if (mod_fd_listar[i].fd > maxfd)
				maxfd = mod_fd_listar[i].fd;
		}
		if (firetalk_select_custom(maxfd+1,
			&rfd, &wfd, NULL, &timeout) != FE_SUCCESS) {
			if (errno == EINTR) { // SIGWINCH
				statrefresh();
				if (rc_resize(&faimconf))
					win_resize();
				statrefresh();
				continue;
			}
			echof(curconn, "MAIN", "firetalk_select_custom() returned error %i: %s\n",
				errno, strerror(errno));
			nw_refresh(&(curconn->nwin));
			statrefresh();
			sleep(1);
			abort();
			exit(1); /* NOTREACH */
		}

		now60 = now-(now%60);
		if ((now60 - lastcycle) >= 60)
			event_handle(lastcycle = now60);
		else if (lastcycle > now60)
			lastcycle = now60;

		if (FD_ISSET(STDIN_FILENO, &rfd)) {
			int	k = nw_getch();

#ifdef KEY_RESIZE
			if (k == KEY_RESIZE) {
				statrefresh();
				if (rc_resize(&faimconf))
					win_resize();
				statrefresh();
			} else
#endif
				gotkey(k);
		}
		for (i = 0; i < mod_fd_listc; i++) {
			if (FD_ISSET(mod_fd_listar[i].fd, &rfd)
				&& (mod_fd_listar[i].type & (O_RDONLY+1)))
				mod_fd_listar[i].func(i, mod_fd_listar[i].fd,
					mod_fd_listar[i].buf,
					mod_fd_listar[i].buflen);
			else if (FD_ISSET(mod_fd_listar[i].fd, &wfd)
				&& (mod_fd_listar[i].type & (O_WRONLY+1))) {
				if (mod_fd_listar[i].type == MOD_FD_CONNECT)
					mod_fd_listar[i].func(mod_fd_listar[i].fd);
				else if (mod_fd_listar[i].type == MOD_FD_WRITE)
					write(mod_fd_listar[i].fd,
						mod_fd_listar[i].buf,
						mod_fd_listar[i].buflen);

				mod_fd_unregister(i);
				break;
			}
		}
		statrefresh();
	}

	firetalk_select();
	echof(curconn, NULL, "Goodbye.\n");
	statrefresh();
	wshutitdown();
	lt_dlexit();
	return(0);
}
