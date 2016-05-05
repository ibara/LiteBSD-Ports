/*  _ __   __ _ ___ __  __
** | '_ \ / _` |_ _|  \/  | naim
** | | | | (_| || || |\/| | Copyright 1998-2005 Daniel Reed <n@ml.org>
** |_| |_|\__,_|___|_|  |_| ncurses-based chat client
*/
#include <naim/naim.h>

#include "naim-int.h"

extern win_t	win_info, win_buddy;
extern conn_t	*curconn;
extern faimconf_t faimconf;
extern time_t	now, awaytime;
extern double	nowf, changetime;
extern char	*lastclose;
extern const char *home;
extern int	awayc;
extern awayar_t	*awayar;
extern int	printtitle;

extern int
	buddyc G_GNUC_INTERNAL,
	wbuddy_widthy G_GNUC_INTERNAL,
	inplayback G_GNUC_INTERNAL;
int	buddyc = -1,
	wbuddy_widthy = -1,
	inplayback = 0;

static void
	iupdate(void) {
	time_t	t;
	long	idletime = secs_getvar_int("idletime");
	struct tm	*tmptr;
	char	buf[1024];

	assert(sizeof(buf) > faimconf.winfo.widthx);

	nw_erase(&win_info);

	tmptr = localtime(&now);
	assert(tmptr != NULL);

	if (curconn->online > 0) {
		t = now - curconn->online;
		secs_setvar("online", dtime(t));
	} else
		secs_setvar("online", "(not connected)");

	secs_setvar("SN", curconn->sn);
	secs_setvar("conn", curconn->winname);

	if (inconn) {
		secs_setvar("cur", curconn->curbwin->winname);
		if ((curconn->curbwin->et == BUDDY) && (curconn->curbwin->e.buddy->tag != NULL)) {
			snprintf(buf, sizeof(buf), " !%.*s!",
				(int)(sizeof(buf)-4),
				curconn->curbwin->e.buddy->tag);
			htmlstrip(buf);
			secs_setvar("iftopic", buf);
		} else if (curconn->curbwin->blurb != NULL) {
			snprintf(buf, sizeof(buf), " (%.*s)",
				(int)(sizeof(buf)-4),
				curconn->curbwin->blurb);
			htmlstrip(buf);
			secs_setvar("iftopic", buf);
		} else if (curconn->curbwin->status != NULL) {
			snprintf(buf, sizeof(buf), " (%.*s)",
				(int)(sizeof(buf)-4),
				curconn->curbwin->status);
			htmlstrip(buf);
			secs_setvar("iftopic", buf);
		} else
			secs_setvar("iftopic", "");

		if ((curconn->curbwin->et != BUDDY) || (curconn->curbwin->e.buddy->typing == 0))
			secs_setvar("iftyping", "");
		else
			secs_setvar("iftyping", secs_script_expand(NULL, getvar(curconn, "statusbar_typing")));

		switch (curconn->curbwin->et) {
		  case BUDDY:
			if (curconn->curbwin->e.buddy->docrypt)
				secs_setvar("ifcrypt", getvar(curconn, "statusbar_crypt"));
			else
				secs_setvar("ifcrypt", "");
			if (curconn->curbwin->e.buddy->tzname != NULL) {
				secs_setvar("tzname", curconn->curbwin->e.buddy->tzname);
				secs_setvar("iftzname", secs_script_expand(NULL, getvar(curconn, "statusbar_tzname")));
			} else {
				secs_setvar("tzname", "");
				secs_setvar("iftzname", "");
			}
			secs_setvar("ifoper", "");
			secs_setvar("ifquery",
				secs_script_expand(NULL, getvar(curconn, "statusbar_query")));
			secs_setvar("ifchat", "");
			break;
		  case CHAT:
			if (curconn->curbwin->e.chat->isoper)
				secs_setvar("ifoper",
					secs_script_expand(NULL, getvar(curconn, "statusbar_oper")));
			else
				secs_setvar("ifoper", "");
			secs_setvar("ifquery", "");
			secs_setvar("ifchat",
				secs_script_expand(NULL, getvar(curconn, "statusbar_chat")));
			break;
		  case TRANSFER:
			secs_setvar("ifoper", "");
			secs_setvar("ifquery", "");
			secs_setvar("ifchat", "");
			break;
		}
	} else {
		secs_setvar("cur", "");
		secs_setvar("ifoper", "");
		secs_setvar("ifquery", "");
		secs_setvar("iftyping", "");
		secs_setvar("ifchat", "");
	}

	secs_setvar("iftransfer", "");

	if (awaytime > 0)
		secs_setvar("ifaway",
			secs_script_expand(NULL, getvar(curconn, "statusbar_away")));
	else
		secs_setvar("ifaway", "");

	if (idletime > 10) {
		secs_setvar("idle", dtime(60*idletime));
		secs_setvar("ifidle",
			secs_script_expand(NULL, getvar(curconn, "statusbar_idle")));
	} else {
		secs_setvar("idle", "");
		secs_setvar("ifidle", "");
	}

	if (curconn->lag > 0.1) {
		secs_setvar("lag", dtime(curconn->lag));
		secs_setvar("iflag",
			secs_script_expand(NULL, getvar(curconn, "statusbar_lag")));
	} else {
		secs_setvar("lag", "0");
		secs_setvar("iflag", "");
	}

	if (curconn->warnval > 0) {
		snprintf(buf, sizeof(buf), "%li", curconn->warnval);
		secs_setvar("warnval", buf);
		secs_setvar("ifwarn",
			secs_script_expand(NULL, getvar(curconn, "statusbar_warn")));
	} else {
		secs_setvar("warnval", "0");
		secs_setvar("ifwarn", "");
	}

	if (strftime(buf, sizeof(buf), getvar(curconn, "statusbar"), tmptr) > 0) {
		char	*left, *right;

		assert(*buf != 0);
		right = secs_script_expand(NULL, buf);
		assert(right != NULL);
		left = strdup(right);
		assert(left != NULL);
		if ((right = strrchr(left, '+')) != NULL) {
			char	*ell;
			int	leftlen, rightlen;

			*right = 0;
			right++;
			leftlen = strlen(left);
			rightlen = strlen(right);
			if (leftlen > (faimconf.winfo.widthx-rightlen)) {
				ell = "...";
				leftlen = faimconf.winfo.widthx-rightlen-3;
			} else {
				ell = "";
				leftlen = faimconf.winfo.widthx-rightlen;
			}
			if (leftlen < 0)
				leftlen = 0;
			snprintf(buf, faimconf.winfo.widthx+1, "%-*.*s%s%s",
				leftlen, leftlen, left, ell, right);
		} else
			snprintf(buf, faimconf.winfo.widthx+1, "%s", left);
		nw_printf(&win_info, CB(STATUSBAR,INPUT), 0, "%-*s", faimconf.winfo.widthx,
			buf);
		free(left);
		left = NULL;
	} else
		status_echof(curconn, "Error in strftime(): %s.\n", strerror(errno));
}

static conn_t *bsort_conn = NULL;

static int bsort_alpha_winname(const void *p1, const void *p2) {
	register buddywin_t
		**bw1 = (buddywin_t **)p1,
		**bw2 = (buddywin_t **)p2;
	char	b1[256], b2[256];
	register const char
		*s1a, *s2a,
		*s1b = (*bw1)->winname,
		*s2b = (*bw2)->winname;
	int	ret;

	assert(bsort_conn != NULL);
	if ((*bw1)->et == BUDDY)
		s1a = user_name(b1, sizeof(b1), bsort_conn, (*bw1)->e.buddy);
	else
		s1a = (*bw1)->winname;
	if ((*bw2)->et == BUDDY)
		s2a = user_name(b2, sizeof(b2), bsort_conn, (*bw2)->e.buddy);
	else
		s2a = (*bw2)->winname;

	ret = strcasecmp(s1a, s2a);
	if (ret != 0)
		return(ret);
	ret = strcasecmp(s1b, s2b);
	return(ret);
}

static int bsort_alpha_group(const void *p1, const void *p2) {
	register buddywin_t
		**b1 = (buddywin_t **)p1,
		**b2 = (buddywin_t **)p2;
	register int
		ret;

	if (((*b1)->et == BUDDY) || ((*b2)->et == BUDDY)) {
		if ((*b1)->et != BUDDY)
			return(1);
		else if ((*b2)->et != BUDDY)
			return(-1);
		ret = strcasecmp(USER_GROUP((*b1)->e.buddy), USER_GROUP((*b2)->e.buddy));
		if (ret != 0)
			return(ret);
	}
	return(bsort_alpha_winname(p1, p2));
}

static void bsort(conn_t *conn) {
	buddywin_t	**ar = NULL,
			*bwin = conn->curbwin;
	int		i, c = 0,
			(*comparator)(const void *, const void *);

	switch (getvar_int(conn, "autosort")) {
	  case 0:
		return;
	  case 1:
		comparator = bsort_alpha_winname;
		break;
	  case 2:
		comparator = bsort_alpha_group;
		break;
	  default:
		abort();
	}

	do {
		ar = realloc(ar, (c+1)*sizeof(*ar));
		if (ar == NULL)
			abort();
		ar[c] = bwin;
		c++;
	} while ((bwin = bwin->next) != conn->curbwin);

	bsort_conn = conn;
	qsort(ar, c, sizeof(*ar), comparator);
	bsort_conn = NULL;

	for (i = 0; i < c; i++)
		ar[i]->next = ar[(i+1)%c];
	free(ar);
}

void	bupdate(void) {
	static win_t	*lwin = NULL;
	int	waiting,
		widthx = secs_getvar_int("winlistchars"),
		M = widthx,
#ifdef ENABLE_FORCEASCII
		fascii = secs_getvar_int("forceascii"),
#endif
		bb, line = 0;
	conn_t	*conn = curconn;

	wbuddy_widthy = secs_getvar_int("winlistheight")*faimconf.wstatus.widthy/100;
	if (wbuddy_widthy > faimconf.wstatus.widthy)
		wbuddy_widthy = faimconf.wstatus.widthy;

#ifdef ENABLE_FORCEASCII
# define LINEDRAW(ch,as)	((fascii == 1) ? as : ch)
#else
# define LINEDRAW(ch,as)	ch
#endif
#define ACS_ULCORNER_C	LINEDRAW(ACS_ULCORNER,',')
#define ACS_LLCORNER_C	LINEDRAW(ACS_LLCORNER,'`')
#define ACS_URCORNER_C	LINEDRAW(ACS_URCORNER,'.')
#define ACS_LRCORNER_C	LINEDRAW(ACS_LRCORNER,'\'')
#define ACS_LTEE_C	LINEDRAW(ACS_LTEE,'+')
#define ACS_RTEE_C	LINEDRAW(ACS_RTEE,'+')
#define ACS_BTEE_C	LINEDRAW(ACS_BTEE,'+')
#define ACS_TTEE_C	LINEDRAW(ACS_TTEE,'+')
#define ACS_HLINE_C	LINEDRAW(ACS_HLINE,'-')
#define ACS_VLINE_C	LINEDRAW(ACS_VLINE,'|')
#define ACS_PLUS_C	LINEDRAW(ACS_PLUS,'+')
#define ACS_RARROW_C	LINEDRAW(ACS_RARROW,'>')

	nw_erase(&win_buddy);
	nw_vline(&win_buddy, ACS_VLINE_C | A_BOLD | COLOR_PAIR(C(WINLIST,TEXT)%COLOR_PAIRS), wbuddy_widthy);
	bb = buddyc;
	buddyc = 0;

	if (conn == NULL)
		return;

	waiting = 0;

	if (inconn) {
		int	autoclose = getvar_int(curconn, "autoclose");

		assert(curconn->curbwin != NULL);
		if ((autoclose > 0) && (curconn->curbwin->et == BUDDY) && !USER_PERMANENT(curconn->curbwin->e.buddy) && (curconn->curbwin->waiting != 0))
			curconn->curbwin->closetime = now + 60*autoclose;
		curconn->curbwin->waiting = 0;
		if ((curconn->curbwin->et == CHAT) && curconn->curbwin->e.chat->isaddressed)
			curconn->curbwin->e.chat->isaddressed = 0;
	}

	do {
		buddywin_t	*bwin = conn->curbwin;
		char	*lastgroup = NULL;
		int	hidegroup = 0,
			autosort = getvar_int(conn, "autosort");

		assert(conn->winname != NULL);

		if (line < wbuddy_widthy) {
			nw_move(&win_buddy, line, 0);
			if (line == 0)
				nw_addch(&win_buddy, ACS_ULCORNER_C | A_BOLD | COLOR_PAIR(C(WINLIST,TEXT)%COLOR_PAIRS));
			else
				nw_addch(&win_buddy, ACS_LTEE_C | A_BOLD | COLOR_PAIR(C(WINLIST,TEXT)%COLOR_PAIRS));
			nw_hline(&win_buddy, ACS_HLINE_C | A_BOLD | COLOR_PAIR(C(WINLIST,TEXT)%COLOR_PAIRS), widthx);

			nw_move(&win_buddy, line++, widthx-strlen(conn->winname));
			if (conn->online <= 0) {
				nw_printf(&win_buddy, C(WINLIST,BUDDY_OFFLINE), 1, " %s", conn->winname);
				if (line < wbuddy_widthy) {
					nw_move(&win_buddy, line++, 1);
					nw_printf(&win_buddy, C(WINLIST,TEXT), 1, "%s", " You are offline");
					buddyc++;
				}
			} else
				nw_printf(&win_buddy, C(WINLIST,TEXT), 1, " %s", conn->winname);
		}

		buddyc++;

		if (bwin == NULL)
			continue;
		if (autosort == 2) {
			if (bwin->et == BUDDY)
				STRREPLACE(lastgroup, USER_GROUP(bwin->e.buddy));
			else
				STRREPLACE(lastgroup, CHAT_GROUP);
		}

		bsort(conn);

		do {
			if ((inconn && (conn == curconn)) || bwin->waiting) {
				char	buf[256], *group;
				int	col = -1;

				assert(bwin->winname != NULL);
				buddyc++;

				if (bwin->et == BUDDY) {
					user_name(buf, sizeof(buf), conn, bwin->e.buddy);
					group = USER_GROUP(bwin->e.buddy);
				} else {
					snprintf(buf, sizeof(buf), "%s", bwin->winname);
					group = CHAT_GROUP;
				}

				if (autosort == 2) {
					if (strcmp(lastgroup, group) != 0) {
						if (line < wbuddy_widthy) {
							nw_move(&win_buddy, line++, widthx-strlen(group)-1);
							nw_printf(&win_buddy, C(WINLIST,TEXT), hidegroup?0:1, "%c%s%c",
								hidegroup?'<':'[', group, hidegroup?'>':']');
							buddyc++;
						}
						STRREPLACE(lastgroup, group);
					}
				}

				if (bwin->waiting && !waiting) {
					char	tmp[64];

					if (conn == curconn)
						snprintf(tmp, sizeof(tmp),
							" [Ctrl-N to %s]", buf);
					else
						snprintf(tmp, sizeof(tmp),
							" [Ctrl-N to %s:%s]", conn->winname, buf);
					secs_setvar("ifpending", tmp);
					waiting = 1;
				}
				if (printtitle && bwin->waiting && (waiting < 2) && ((bwin->et == BUDDY) || ((bwin->et == CHAT) && bwin->e.chat->isaddressed))) {
					nw_titlef("[%s:%s]", conn->winname, buf);
					waiting = 2;
				}

				if (line >= wbuddy_widthy)
					continue;

				if (strlen(buf) > M) {
					buf[M-1] = '>';
					buf[M] = 0;
				}

				if ((bwin->et == CHAT) && bwin->e.chat->isaddressed) {
					assert(bwin->waiting);
					col = C(WINLIST,BUDDY_ADDRESSED);
				} else if (bwin->waiting)
					if (bwin->et == BUDDY)
						col = C(WINLIST,BUDDY_ADDRESSED);
					else
						col = C(WINLIST,BUDDY_WAITING);
				else if (bwin->pouncec > 0)
					col = C(WINLIST,BUDDY_QUEUED);
				else
					switch (bwin->et) {
					  case BUDDY:
						if (bwin->e.buddy->tag != NULL)
							col = CI(WINLIST,BUDDY_TAGGED);
						else if (bwin->e.buddy->offline)
							col = C(WINLIST,BUDDY_OFFLINE);
						else if (bwin->e.buddy->ismobile)
							col = C(WINLIST,BUDDY_MOBILE);
						else if (bwin->e.buddy->isaway)
							col = C(WINLIST,BUDDY_AWAY);
						else if (bwin->e.buddy->isidle)
							col = C(WINLIST,BUDDY_IDLE);
						else
							col = C(WINLIST,BUDDY);
						break;
					  case CHAT:
						assert(bwin->e.chat != NULL);
						if (bwin->e.chat->offline)
							col = C(WINLIST,BUDDY_OFFLINE);
						else
							col = C(WINLIST,BUDDY);
						break;
					  case TRANSFER:
						col = C(WINLIST,BUDDY);
						break;
					}
				assert(col != -1);
				if (bwin == curconn->curbwin) {
					int	affect = col/COLOR_PAIRS,
						back;

#if 0
					if (faimconf.b[cIMWIN] == faimconf.b[cWINLIST])
						back = (faimconf.b[cIMWIN]+1)%nw_COLORS;
					else
						back = faimconf.b[cIMWIN];
#else
						back = faimconf.b[cWINLISTHIGHLIGHT];
#endif

					col %= nw_COLORS;
					col += nw_COLORS*back;
					col += affect*COLOR_PAIRS;
				}
				nw_move(&win_buddy, line, 1);
				if ((col >= 2*COLOR_PAIRS) || (col < COLOR_PAIRS))
					nw_printf(&win_buddy, col%COLOR_PAIRS, 1, "%*s", M, buf);
				else
					nw_printf(&win_buddy, col%COLOR_PAIRS, 0, "%*s", M, buf);
				if (bwin->waiting) {
					nw_move(&win_buddy, line, 0);
					nw_addch(&win_buddy, ACS_LTEE_C   | A_BOLD | COLOR_PAIR(C(WINLIST,TEXT)%COLOR_PAIRS));
					nw_addch(&win_buddy, ACS_RARROW_C | A_BOLD | COLOR_PAIR(col%COLOR_PAIRS));
				}
				line++;
			}
			assert(buddyc < 1000);
		} while ((bwin = bwin->next) != conn->curbwin);

		if (autosort == 2) {
			free(lastgroup);
			lastgroup = NULL;
		} else
			assert(lastgroup == NULL);
	} while ((conn = conn->next) != curconn);

	nw_move(&win_buddy, line-1, 0);
	if (line != 1)
		nw_addch(&win_buddy, ACS_LLCORNER_C | A_BOLD | COLOR_PAIR(C(WINLIST,TEXT)%COLOR_PAIRS));
	else
		nw_addch(&win_buddy, ACS_HLINE_C | A_BOLD | COLOR_PAIR(C(WINLIST,TEXT)%COLOR_PAIRS));

	if (printtitle && (waiting != 2))
		nw_titlef("");
	if (waiting)
		buddyc = -buddyc;
	else
		secs_setvar("ifpending", "");

	if (inconn)
		assert(curconn->curbwin != NULL);

	if ((buddyc != bb)
		||  (inconn && (&(curconn->curbwin->nwin) != lwin))
		|| (!inconn && (&(curconn->nwin) != lwin))) {
		if (inconn)
			lwin = &(curconn->curbwin->nwin);
		else
			lwin = &(curconn->nwin);
		bb = buddyc - bb;
		buddyc = buddyc - bb;
		bb = buddyc + bb;
		naim_changetime();
		buddyc = bb;
	}

	iupdate();
}

buddywin_t
	*bgetwin(conn_t *conn, const char *buddy, et_t et) {
	buddywin_t	*bwin = conn->curbwin;

	assert(buddy != NULL);
	if (bwin == NULL)
		return(NULL);
	do {
		if (bwin->et == et) {
			if (firetalk_compare_nicks(conn->conn, buddy, bwin->winname) == FE_SUCCESS)
				return(bwin);
//			if ((bwin->et == BUDDY) && (firetalk_compare_nicks(conn->conn, buddy, USER_NAME(bwin->e.buddy)) == FE_SUCCESS))
//				return(bwin);
		}
	} while ((bwin = bwin->next) != conn->curbwin);

	return(NULL);
}

buddywin_t
	*bgetanywin(conn_t *conn, const char *buddy) {
	buddywin_t	*bwin = conn->curbwin;

	assert(buddy != NULL);
	if (bwin == NULL)
		return(NULL);
	do {
		if (firetalk_compare_nicks(conn->conn, buddy, bwin->winname) == FE_SUCCESS)
			return(bwin);
		if ((bwin->et == BUDDY) && (firetalk_compare_nicks(conn->conn, buddy, USER_NAME(bwin->e.buddy)) == FE_SUCCESS))
			return(bwin);
	} while ((bwin = bwin->next) != conn->curbwin);

	return(NULL);
}

static void
	bremove(buddywin_t *bwin) {
	int	i;

	assert(bwin != NULL);
	nw_delwin(&(bwin->nwin));
	for (i = 0; i < bwin->pouncec; i++) {
		free(bwin->pouncear[i]);
		bwin->pouncear[i] = NULL;
	}
	free(bwin->pouncear);
	bwin->pouncear = NULL;
	FREESTR(bwin->winname);
	FREESTR(bwin->blurb);
	FREESTR(bwin->status);

	if (bwin->nwin.logfile != NULL) {
		struct tm	*tmptr;

		tmptr = localtime(&now);
		fprintf(bwin->nwin.logfile, "<I>-----</I> <font color=\"#FFFFFF\">Log file closed %04i-%02i-%02iT%02i:%02i</font> <I>-----</I><br>\n",
			1900+tmptr->tm_year, 1+tmptr->tm_mon, tmptr->tm_mday, tmptr->tm_hour, tmptr->tm_min);
		fclose(bwin->nwin.logfile);
		bwin->nwin.logfile = NULL;
	}

	free(bwin);
}

void	verify_winlist_sanity(conn_t *const conn, const buddywin_t *const verifywin) {
	buddywin_t	*bwin;
	int	i = 0, found = 0;

	assert(conn != NULL);
	assert(conn->curbwin != NULL);
	bwin = conn->curbwin;
	do {
		if (bwin == verifywin)
			found = 1;
		assert((bwin->et == CHAT) || (bwin->et == BUDDY) || (bwin->et == TRANSFER));
		if (bwin->et != BUDDY) {
			assert(bwin->informed == 0);
			assert(bwin->closetime == 0);
		}
		if (bwin->et == CHAT)
			assert(bwin->keepafterso == 1);
		assert(strlen(bwin->winname) > 0);
		assert(i++ < 10000);
	} while ((bwin = bwin->next) != conn->curbwin);
	if (verifywin != NULL)
		assert(found == 1);
	else
		assert(found == 0);
}

void	bclose(conn_t *conn, buddywin_t *bwin, int _auto) {
	if (bwin == NULL)
		return;

	verify_winlist_sanity(conn, bwin);

	switch (bwin->et) {
	  case BUDDY:
		if (_auto == 0) {
			assert(bwin->e.buddy != NULL);
			status_echof(conn, "Type /delbuddy to remove %s from your buddy list.\n",
				user_name(NULL, 0, conn, bwin->e.buddy));
			STRREPLACE(lastclose, bwin->winname);
		}
		break;
	  case CHAT:
		if (bwin->winname[0] != ':')
			firetalk_chat_part(conn->conn, bwin->winname);
		free(bwin->e.chat->key);
		bwin->e.chat->key = NULL;
		free(bwin->e.chat->last.line);
		bwin->e.chat->last.line = NULL;
		free(bwin->e.chat->last.name);
		bwin->e.chat->last.name = NULL;
		free(bwin->e.chat);
		bwin->e.chat = NULL;
		break;
	  case TRANSFER:
		assert(bwin->e.transfer != NULL);
		firetalk_file_cancel(conn->conn, bwin->e.transfer->handle);
		echof(conn, NULL, "File transfer aborted.\n");
		fremove(bwin->e.transfer);
		bwin->e.transfer = NULL;
		break;
	}

	if (bwin == bwin->next)
		conn->curbwin = NULL;
	else {
		int	i = 0;
		buddywin_t *bbefore;

		bbefore = bwin->next;
		while (bbefore->next != bwin) {
			bbefore = bbefore->next;
			assert(i++ < 10000);
		}
		bbefore->next = bwin->next;

		if (bwin == conn->curbwin)
			conn->curbwin = bwin->next;
	}

	if (conn->curbwin != NULL)
		verify_winlist_sanity(conn, NULL);

	bremove(bwin);
	bwin = NULL;
	bupdate();

	if (conn->curbwin != NULL)
		verify_winlist_sanity(conn, NULL);

	if (conn->curbwin != NULL)
		nw_touchwin(&(conn->curbwin->nwin));
	else
		nw_touchwin(&(conn->nwin));
}

const unsigned char *naim_normalize(const unsigned char *const name) {
	static char	newname[2048];
	int	i, j = 0;

	for (i = 0; (name[i] != 0) && (j < sizeof(newname)-1); i++)
		if ((name[i] == '/') || (name[i] == '.'))
			newname[j++] = '_';
		else if (name[i] != ' ')
			newname[j++] = tolower(name[i]);
	newname[j] = 0;
	return(newname);
}

static int
	makedir(const char *d) {
	char	*dir;

	if (*d != '/') {
		static char buf[1024];

		snprintf(buf, sizeof(buf), "%s/%s", home, d);
		d = buf;
	}
	dir = strdup(d);
	while (chdir(d) != 0) {
		strcpy(dir, d);
		while (chdir(dir) != 0) {
			char	*pdir = strrchr(dir, '/');

			if (mkdir(dir, 0700) != 0)
				if (errno != ENOENT) {
					chdir(home);
					free(dir);
					return(-1);
				}
			if (pdir == NULL)
				break;
			*pdir = 0;
		}
	}
	chdir(home);
	free(dir);
	return(0);
}

static FILE
	*playback_fopen(conn_t *const conn, buddywin_t *const bwin, const char *const mode) {
	FILE	*rfile;
	char	*n, *nhtml, *ptr,
		buf[256];

	secs_setvar("conn", conn->winname);
	secs_setvar("cur", naim_normalize(bwin->winname));

	n = secs_script_expand(NULL, secs_getvar("logdir"));
	snprintf(buf, sizeof(buf), "%s", n);
	if ((ptr = strrchr(buf, '/')) != NULL) {
		*ptr = 0;
		makedir(buf);
	}

	if (strstr(n, ".html") == NULL) {
		snprintf(buf, sizeof(buf), "%s.html", n);
		nhtml = buf;
	} else {
		nhtml = n;
		snprintf(buf, sizeof(buf), "%s", nhtml);
		if ((ptr = strstr(buf, ".html")) != NULL)
			*ptr = 0;
		n = buf;
	}

	if ((rfile = fopen(n, "r")) != NULL) {
		fclose(rfile);
		if ((rfile = fopen(nhtml, "r")) != NULL) {
			fclose(rfile);
			status_echof(conn, "Warning: While opening logfile for %s, two versions were found: [%s] and [%s]. I will use [%s], but you may want to look into this discrepency.\n",
				bwin->winname, n, nhtml, nhtml);
		} else
			rename(n, nhtml);
	}

	return(fopen(nhtml, mode));
}

void	playback(conn_t *const conn, buddywin_t *const bwin, const int lines) {
	FILE	*rfile;

	assert(bwin->nwin.logfile != NULL);
	fflush(bwin->nwin.logfile);
	bwin->nwin.dirty = 0;

	if ((rfile = playback_fopen(conn, bwin, "r")) != NULL) {
		char	buf[2048];
		int	maxlen = lines*faimconf.wstatus.widthx;
		long	filesize, playbackstart, playbacklen, pos;
		time_t	lastprogress = now;

#ifdef DEBUG_ECHO
		status_echof(conn, "Redrawing window for %s.", bwin->winname);
#endif

		nw_statusbarf("Redrawing window for %s.", bwin->winname);

		fseek(rfile, 0, SEEK_END);
		while (((filesize = ftell(rfile)) == -1) && (errno == EINTR))
			;
		assert(filesize >= 0);
		if (filesize > maxlen) {
			fseek(rfile, -maxlen, SEEK_CUR);
			while ((fgetc(rfile) != '\n') && !feof(rfile))
				;
		} else
			fseek(rfile, 0, SEEK_SET);
		while (((playbackstart = ftell(rfile)) == -1) && (errno == EINTR))
			;
		assert(playbackstart >= 0);
		pos = 0;
		playbacklen = filesize-playbackstart;
		inplayback = 1;
		while (fgets(buf, sizeof(buf), rfile) != NULL) {
			long	len = strlen(buf);

			pos += len;
			hwprintf(&(bwin->nwin), -C(IMWIN,TEXT)-1, "%s", buf);
			if ((now = time(NULL)) > lastprogress) {
				nw_statusbarf("Redrawing window for %s (%li lines left).",
					bwin->winname, lines*(playbacklen-pos)/playbacklen);
				lastprogress = now;
			}
		}
		inplayback = 0;
		fclose(rfile);
	}
}

void	bnewwin(conn_t *conn, const char *name, et_t et) {
	buddywin_t	*bwin;
	int	i;

	assert(name != NULL);

	if (bgetwin(conn, name, et) != NULL)
		return;

	bwin = calloc(1, sizeof(buddywin_t));
	assert(bwin != NULL);

	nw_newwin(&(bwin->nwin));
	nw_initwin(&(bwin->nwin), cIMWIN);
	for (i = 0; i < faimconf.wstatus.pady; i++)
		nw_printf(&(bwin->nwin), 0, 0, "\n");

	bwin->winname = strdup(name);
	assert(bwin->winname != NULL);

	bwin->keepafterso = bwin->waiting = 0;
	bwin->et = et;
	switch (et) {
	  case BUDDY:
		bwin->e.buddy = rgetlist(conn, name);
		assert(bwin->e.buddy != NULL);
		break;
	  case CHAT:
		bwin->e.chat = calloc(1, sizeof(*(bwin->e.chat)));
		assert(bwin->e.chat != NULL);
		bwin->e.chat->offline = 1;
		break;
	  case TRANSFER:
		break;
	}

	if (conn->curbwin == NULL) {
		bwin->next = bwin;
		conn->curbwin = bwin;
	} else {
		buddywin_t	*lastbwin = conn->curbwin,
				*srchbwin = conn->curbwin;

		do {
			if (aimcmp(lastbwin->winname, lastbwin->next->winname) == 1)
				break;
		} while ((lastbwin = lastbwin->next) != srchbwin);

		srchbwin = lastbwin;

		do {
			if (aimcmp(srchbwin->next->winname, bwin->winname) == 1)
				break;
		} while ((srchbwin = srchbwin->next) != lastbwin);

		bwin->next = srchbwin->next;
		srchbwin->next = bwin;
	}

	{
		if ((bwin->nwin.logfile = playback_fopen(conn, bwin, "a")) == NULL)
			status_echof(conn, "Unable to open scrollback buffer file: %s\n",
				strerror(errno));
		else {
			struct tm	*tmptr;

			fchmod(fileno(bwin->nwin.logfile), 0600);
			tmptr = localtime(&now);
			fprintf(bwin->nwin.logfile, "&nbsp;<br>\n<I>-----</I> <font color=\"#FFFFFF\">Log file opened %04i-%02i-%02iT%02i:%02i</font> <I>-----</I><br>\n",
				1900+tmptr->tm_year, 1+tmptr->tm_mon, tmptr->tm_mday, tmptr->tm_hour, tmptr->tm_min);
			if (firetalk_compare_nicks(conn->conn, name, "naim help") == FE_SUCCESS) {
				fprintf(bwin->nwin.logfile, "<I>*****</I> <font color=\"#808080\">Once you have signed on, anything you type that does not start with a slash is sent as a private message to whoever's window you are in.</font><br>\n");
				fprintf(bwin->nwin.logfile, "<I>*****</I> <font color=\"#808080\">Right now you are \"in\" a window for <font color=\"#00FF00\">naim help</font>, which is the screen name of naim's maintainer, <font color=\"#00FFFF\">Daniel Reed</font>.</font><br>\n");
				fprintf(bwin->nwin.logfile, "<I>*****</I> <font color=\"#808080\">If you would like help, first try using naim's online help by typing <font color=\"#00FF00\">/help</font>. If you need further help, feel free to ask your question here, and Mr. Reed will get back to you as soon as possible.</font><br>\n");
				fprintf(bwin->nwin.logfile, "<I>*****</I> <font color=\"#800000\">If you are using Windows telnet to connect to a shell account to run naim, you may notice severe screen corruption. You may wish to try PuTTy, available for free from www.tucows.com. PuTTy handles both telnet and SSH.</font><br>\n");
			}
			nw_resize(&(bwin->nwin), 1, 1);
			bwin->nwin.dirty = 1;
		}
	}
}

void	bcoming(conn_t *conn, const char *buddy) {
	buddywin_t	*bwin = NULL;
	buddylist_t	*blist = NULL;

	assert(buddy != NULL);

	if ((blist = rgetlist(conn, buddy)) == NULL) {
		status_echof(conn, "Adding %s to your buddy list due to sign-on.\n", buddy);
		blist = raddbuddy(conn, buddy, DEFAULT_GROUP, NULL);
	}
	STRREPLACE(blist->_account, buddy);
	if ((bwin = bgetwin(conn, buddy, BUDDY)) == NULL) {
		if (getvar_int(conn, "autoquery") != 0) {
			bnewwin(conn, buddy, BUDDY);
			bwin = bgetwin(conn, buddy, BUDDY);
			assert(bwin != NULL);
		}
	}
	assert((bwin == NULL) || (bwin->e.buddy == blist));

	if (blist->offline == 1) {
		blist->isadmin = blist->ismobile = blist->isidle = blist->isaway = blist->offline = 0;
		status_echof(conn, "<font color=\"#00FFFF\">%s</font> <font color=\"#800000\">[<B>%s</B>]</font> is now online =)\n",
			user_name(NULL, 0, conn, blist), USER_GROUP(blist));
		if (bwin != NULL) {
			window_echof(bwin, "<font color=\"#00FFFF\">%s</font> <font color=\"#800000\">[<B>%s</B>]</font> is now online =)\n",
				user_name(NULL, 0, conn, blist), USER_GROUP(blist));
			if (bwin->pouncec > 0) {
				int	i, pc = bwin->pouncec;

				for (i = 0; i < pc; i++) {
					window_echof(bwin, "Sending queued IM %i/%i [%s].\n",
						i+1, pc, bwin->pouncear[i]);
					naim_send_im(conn, bwin->winname,
						bwin->pouncear[i], 1);
				}
				bwin->pouncec -= pc;
				memmove(bwin->pouncear, bwin->pouncear+pc,
					bwin->pouncec*sizeof(*(bwin->pouncear)));
				bwin->pouncear = realloc(bwin->pouncear,
					bwin->pouncec*sizeof(*(bwin->pouncear)));
			}
		}

		{
			int	beeponsignon = getvar_int(conn, "beeponsignon");

			if ((beeponsignon > 1) || ((awaytime == 0) && (beeponsignon == 1)))
				beep();
		}
	}
	bupdate();
}

void	bgoing(conn_t *conn, const char *buddy) {
	buddywin_t	*bwin = conn->curbwin;
	buddylist_t	*blist = NULL;

	assert(buddy != NULL);

	if (bwin == NULL)
		return;

	if ((blist = rgetlist(conn, buddy)) != NULL) {
		if ((blist->peer <= 0) && (blist->crypt != NULL))
			echof(conn, NULL, "Strangeness while marking %s offline: no autopeer negotiated, but autocrypt set!\n",
				buddy);
		blist->docrypt = blist->peer = 0;
		if (blist->crypt != NULL) {
			free(blist->crypt);
			blist->crypt = NULL;
		}
		if (blist->tzname != NULL) {
			free(blist->tzname);
			blist->tzname = NULL;
		}

		status_echof(conn, "<font color=\"#00FFFF\">%s</font> <font color=\"#800000\">[<B>%s</B>]</font> has just logged off :(\n", 
			user_name(NULL, 0, conn, blist), USER_GROUP(blist));
		blist->offline = 1;
		blist->warnval = blist->typing = blist->isadmin = blist->ismobile = blist->isidle = blist->isaway = 0;
	} else
		return;

	do {
		if ((bwin->et == BUDDY) && (firetalk_compare_nicks(conn->conn, buddy, bwin->winname) == FE_SUCCESS)) {
			int	autoclose = getvar_int(conn, "autoclose"),
				beeponsignon = getvar_int(conn, "beeponsignon");

			assert(bwin->e.buddy == blist);
			window_echof(bwin, "<font color=\"#00FFFF\">%s</font> <font color=\"#800000\">[<B>%s</B>]</font> has just logged off :(\n",
				user_name(NULL, 0, conn, blist), USER_GROUP(blist));
			if ((beeponsignon > 1) || ((awaytime == 0) && (beeponsignon == 1)))
				beep();
			FREESTR(bwin->blurb);
			FREESTR(bwin->status);

			if (bwin->keepafterso == 1) {
				if ((autoclose > 0) && !USER_PERMANENT(bwin->e.buddy) && (bwin->waiting == 0))
					bwin->closetime = now + 60*autoclose;
			} else {
				/* assert(bwin->waiting == 0); */
				bclose(conn, bwin, 1);
				bwin = NULL;
				if ((autoclose > 0) && !USER_PERMANENT(blist)) {
					rdelbuddy(conn, buddy);
					firetalk_im_remove_buddy(conn->conn, buddy);
				}
			}
			bupdate();
			return;
		}
	} while ((bwin = bwin->next) != conn->curbwin);
}

void	bidle(conn_t *conn, const char *buddy, int isidle) {
	buddywin_t	*bwin = NULL;
	buddylist_t	*blist = NULL;

	assert(buddy != NULL);
	bwin = bgetwin(conn, buddy, BUDDY);
	if (bwin == NULL)
		blist = rgetlist(conn, buddy);
	else
		blist = bwin->e.buddy;
	assert(blist != NULL);

	if (bwin != NULL) {
		if ((isidle == 1) && (blist->isidle == 0))
			window_echof(bwin, "<font color=\"#00FFFF\">%s</font> is now idle.\n",
				user_name(NULL, 0, conn, blist));
		else if ((isidle == 0) && (blist->isidle == 1))
			window_echof(bwin, "<font color=\"#00FFFF\">%s</font> is no longer idle!\n",
				user_name(NULL, 0, conn, blist));
	}

	blist->isidle = isidle;
}

void	baway(conn_t *conn, const char *buddy, int isaway) {
	buddywin_t	*bwin = NULL;
	buddylist_t	*blist = NULL;

	assert(buddy != NULL);
	bwin = bgetwin(conn, buddy, BUDDY);
	if (bwin == NULL)
		blist = rgetlist(conn, buddy);
	else
		blist = bwin->e.buddy;
	assert(blist != NULL);

	if (bwin != NULL) {
		if (isaway == 0)
			FREESTR(bwin->blurb);
		if ((isaway == 1) && (blist->isaway == 0)) {
			if ((conn->online+30) < now) {
				awayc++;
				awayar = realloc(awayar, awayc*sizeof(*awayar));
				awayar[awayc-1].name = strdup(buddy);
				awayar[awayc-1].gotaway = 0;
				firetalk_im_get_info(conn->conn, buddy);
			} else
				window_echof(bwin, "<font color=\"#00FFFF\">%s</font> is now away.\n",
					user_name(NULL, 0, conn, blist));
		} else if ((isaway == 0) && (blist->isaway == 1)) {
			if (bwin->status != NULL)
				window_echof(bwin, "<font color=\"#00FFFF\">%s</font> is now available: %s\n",
					user_name(NULL, 0, conn, blist), bwin->status);
			else
				window_echof(bwin, "<font color=\"#00FFFF\">%s</font> is no longer away!\n",
					user_name(NULL, 0, conn, blist));
		}
	}

	blist->isaway = isaway;
}

static void
	bclearall_bwin(conn_t *conn, buddywin_t *bwin, int force) {
	FREESTR(bwin->blurb);
	FREESTR(bwin->status);
	switch (bwin->et) {
	  case BUDDY:
		assert(bwin->e.buddy != NULL);
		if (bwin->e.buddy->offline == 0) {
			bwin->e.buddy->offline = 1;
			window_echof(bwin, "<font color=\"#00FFFF\">%s</font> <font color=\"#800000\">[<B>%s</B>]</font> is no longer available :/\n",
				user_name(NULL, 0, conn, bwin->e.buddy), USER_GROUP(bwin->e.buddy));
		}
		if (bwin->keepafterso == 0) {
			bclose(conn, bwin, 1);
			bwin = NULL;
			return;
		}
		break;
	  case CHAT:
		bwin->e.chat->isoper = 0;
		if (bwin->e.chat->offline == 0) {
			bwin->e.chat->offline = 1;
			window_echof(bwin, "Chat <font color=\"#00FFFF\">%s</font> is no longer available :/\n",
				bwin->winname);
		}
		break;
	  case TRANSFER:
		break;
	}
	if (force) {
		bclose(conn, bwin, 1);
		bwin = NULL;
	}
}

static void
	bclearall_buddy(buddylist_t *buddy) {
	if (buddy->crypt != NULL) {
		free(buddy->crypt);
		buddy->crypt = NULL;
	}
	if (buddy->tzname != NULL) {
		free(buddy->tzname);
		buddy->tzname = NULL;
	}
	buddy->docrypt = buddy->peer = 0;
	buddy->offline = 1;
}

void	bclearall(conn_t *conn, int force) {
	if (conn->curbwin != NULL) {
		buddywin_t	*bwin = conn->curbwin;
		int	i, l = 0;

		do {
			l++;
		} while ((bwin = bwin->next) != conn->curbwin);

		for (i = 0; i < l; i++) {
			buddywin_t	*bnext = bwin->next;

			bclearall_bwin(conn, bwin, force);
			bwin = bnext;
		}
		if (force)
			assert(conn->curbwin == NULL);
	}

	if (conn->buddyar != NULL) {
		buddylist_t	*blist = conn->buddyar;

		if (force) {
			buddylist_t *bnext;

			do {
				bnext = blist->next;
				firetalk_im_remove_buddy(conn->conn, blist->_account);
				do_delbuddy(blist);
			} while ((blist = bnext) != NULL);
			conn->buddyar = NULL;
		} else
			do {
				bclearall_buddy(blist);
			} while ((blist = blist->next) != NULL);
	}

	bupdate();
}

void	naim_changetime(void) {
	int	autohide = secs_getvar_int("autohide");

	if (changetime > 0) {
		if (buddyc < 0)
			changetime = nowf - SLIDETIME - SLIDETIME/autohide;
		else if ((changetime + autohide) < nowf)
			changetime = nowf;
		else if (((changetime + SLIDETIME) < nowf) || (buddyc < 0))
			changetime = nowf - SLIDETIME - SLIDETIME/autohide;
	}
}
