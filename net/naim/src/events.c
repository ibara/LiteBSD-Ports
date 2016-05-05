/*  _ __   __ _ ___ __  __
** | '_ \ / _` |_ _|  \/  | naim
** | | | | (_| || || |\/| | Copyright 1998-2006 Daniel Reed <n@ml.org>
** |_| |_|\__,_|___|_|  |_| ncurses-based chat client
*/
#define _GNU_SOURCE
#include <string.h>

#include <naim/naim.h>
#include <naim/modutil.h>

#include "naim-int.h"
#include "snapshot.h"
#include "conio_cmds.h"

#ifdef ENABLE_DNSUPDATE
# include <netdb.h>
#endif

extern conn_t	*curconn;
extern time_t	startuptime, awaytime;
extern double	nowf;
extern faimconf_t	faimconf;

void	updateidletime(void) {
	if ((awaytime > 0) && (secs_getvar_int("autounaway") != 0))
		unsetaway();
	if (secs_getvar_int("autounidle") > 0)
		secs_setvar("idletime", "0");
	bupdate();
}

HOOK_DECLARE(periodic);
void	event_handle(time_t now) {
	long	idletime = secs_getvar_int("idletime");
	int	tprint, logtprint, autoaway, lagcheck, dailycol, regularcol;
	conn_t	*conn;
	struct tm	*tmptr;
	int	cleanedone = 0;

	whidecursor();

	tprint = secs_getvar_int("tprint");
	logtprint = secs_getvar_int("logtprint");
	lagcheck = secs_getvar_int("lagcheck");

	if (secs_getvar_int("autoidle") > 0) {
		char	buf[1024];

		idletime++;
		snprintf(buf, sizeof(buf), "%lu", idletime);
		secs_setvar("idletime", buf);
	}

	if (awaytime > 0)
		autoaway = 0;
	else
		autoaway = secs_getvar_int("autoaway");

	if ((autoaway > 0) && (idletime >= autoaway)) {
		char	*autoawaymsg = secs_getvar("autoawaymsg");

		echof(curconn, "TIMER", "You have been idle for more than %i minutes, so I'm going to mark you away. If you don't want me to do this in the future, just type ``/set autoaway 0'' (you can put that in your .naimrc).\n",
			autoaway);
		if (autoawaymsg != NULL)
			secs_setvar("awaymsg", autoawaymsg);
		setaway(1);
	}

	tmptr = localtime(&now);
	assert(tmptr != NULL);

	if (logtprint > 1) {
		dailycol = C(IMWIN,TEXT);
		regularcol = C(IMWIN,TEXT);
	} else if (logtprint > 0) {
		dailycol = C(IMWIN,TEXT);
		regularcol = -C(IMWIN,TEXT)-1;
	} else {
		dailycol = -C(IMWIN,TEXT)-1;
		regularcol = -C(IMWIN,TEXT)-1;
	}

	conn = curconn;
	do {
		buddywin_t	*bwin = conn->curbwin;

		if ((conn->online == -1) && (getvar_int(conn, "autoreconnect") != 0)) {
			echof(conn, NULL, "Attempting to reconnect...\n");
			conio_connect(conn, 0, NULL);
		}

		if (conn->curbwin != NULL)
			verify_winlist_sanity(conn, NULL);

		if ((conn->online > 0) && (lagcheck != 0)) {
			char    pingbuf[100];

			snprintf(pingbuf, sizeof(pingbuf), "%lu.%05i", now, (int)(100000*(nowf-((int)nowf))));
			firetalk_subcode_send_request(conn->conn, conn->sn, 
				"LC", pingbuf);
		}

		if (bwin != NULL) {
			buddywin_t	*bnext;
			time_t	nowm = now/60;

			do {
				verify_winlist_sanity(conn, bwin);

				if (bwin != bwin->next)
					bnext = bwin->next;
				else
					bnext = NULL;

				if (!cleanedone && bwin->nwin.dirty) {
					do_resize(conn, bwin);
					cleanedone = 1;
				}

				verify_winlist_sanity(conn, bwin);

				if ((tmptr->tm_hour == 0) && (tmptr->tm_min == 0)) {
					hwprintf(&(bwin->nwin), dailycol, "<I>-----</I>"
						" [<B>%04i</B>-<B>%02i</B>-<B>%02i</B>] <I>-----</I><br>",
						1900+tmptr->tm_year, 1+tmptr->tm_mon, tmptr->tm_mday);
					if (bwin->nwin.logfile != NULL) {
						nw_statusbarf("Flushing log file for %s.", bwin->winname);
						fflush(bwin->nwin.logfile);
					}
				} else if ((tprint != 0) && (nowm%tprint == 0) && ((bwin->et != CHAT) || (*(bwin->winname) != ':')))
					hwprintf(&(bwin->nwin), regularcol, "<I>-----</I>"
						" [<B>%02i</B>:<B>%02i</B>] <I>-----</I><br>",
						tmptr->tm_hour, tmptr->tm_min);

				if ((bwin->closetime > 0) && (bwin->closetime <= now)) {
					assert(bwin->et == BUDDY);
					if ((bwin->e.buddy->offline == 0) || USER_PERMANENT(bwin->e.buddy) || (bwin->pouncec > 0))
						bwin->closetime = 0;
					else {
						char	*name = strdup(bwin->winname);

						bclose(conn, bwin, 1);
						bwin = NULL;
						rdelbuddy(conn, name);
						firetalk_im_remove_buddy(conn->conn, name);
						status_echof(conn, "Cleaning up auto-added buddy %s.\n",
							name);
						free(name);
					}
				}
			} while ((bnext != NULL) && ((bwin = bnext) != conn->curbwin));
		}

		if (conn->curbwin != NULL)
			verify_winlist_sanity(conn, NULL);
	} while ((conn = conn->next) != curconn);

#ifdef ENABLE_DNSUPDATE
	{
		int	updatecheck = secs_getvar_int("updatecheck");

		if ((updatecheck > 0) && (((now-startuptime)/60)%updatecheck == 0)) {
			struct hostent *ent;

			nw_statusbarf("Anonymously checking for the latest version of naim...");
			if ((ent = gethostbyname("latest.naim.n.ml.org")) != NULL) {
				int	i;

				for (i = 0; ent->h_aliases[i] != NULL; i++)
					;
				if ((i > 0) && (strlen(ent->h_aliases[i-1]) > strlen(".naim.n.ml.org"))) {
					char	buf[64];

					snprintf(buf, sizeof(buf), "%.*s", strlen(ent->h_aliases[i-1])-strlen(".naim.n.ml.org"), ent->h_aliases[i-1]);
# ifdef HAVE_STRVERSCMP
					if (strverscmp(PACKAGE_VERSION NAIM_SNAPSHOT, buf) < 0) {
# else
					if (strcmp(PACKAGE_VERSION NAIM_SNAPSHOT, buf) < 0) {
# endif
						echof(curconn, NULL, "Current version: <font color=\"#FF0000\">" PACKAGE_VERSION NAIM_SNAPSHOT "</font>\n");
						echof(curconn, NULL, "&nbsp;Latest version: <font color=\"#00FF00\">%s</font> (reported by naim.n.ml.org)\n", buf);
						echof(curconn, NULL, 
# ifdef DNSUPDATE_MESSAGE
							DNSUPDATE_MESSAGE
# else
							"You may be using an old version of naim. Please visit <font color=\"#0000FF\">http://naim.n.ml.org/</font> or contact your system vendor for more information.\n"
# endif
						);
					}
				}
			}
		}
	}
#endif

	HOOK_CALL(periodic, (NULL, now));

	bupdate();
}
