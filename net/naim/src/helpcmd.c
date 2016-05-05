/*  _ __   __ _ ___ __  __
** | '_ \ / _` |_ _|  \/  | naim
** | | | | (_| || || |\/| | Copyright 1998-2004 Daniel Reed <n@ml.org>
** |_| |_|\__,_|___|_|  |_| ncurses-based chat client
*/
#include <naim/naim.h>

#include "naim-int.h"
#include "help.h"
#include "conio_cmds.h"

extern conn_t	*curconn;
extern faimconf_t	faimconf;

static HelpType
	help_findtopic(const char *topic) {
	int	i = 0;

	while (h_texts[i].tag != NULL) {
		if (strcasecmp(h_texts[i].tag, topic) == 0)
			return(h_texts[i].help_text);
		i++;
	}
	return(NULL);
}

static void
	help_printcmd(win_t *nwin, int col, int cmd) {
	int	i;

	hwprintf(nwin, col, "&nbsp; <font color=\"#FF00FF\">SYNTAX</font>: <font color=\"#00FF00\">/%s</font>", cmdar[cmd].c);
	if (cmdar[cmd].maxarg > 0) {
		int	numop = 0;

		for (i = 0; cmdar[cmd].args[i].name != NULL; i++)
			if (cmdar[cmd].args[i].required)
				hwprintf(nwin, col, " <font color=\"#00FF00\">%s</font>",
					cmdar[cmd].args[i].name);
			else {
				numop++;
				hwprintf(nwin, col, " [<font color=\"#FFFF00\">%s</font>",
					cmdar[cmd].args[i].name);
			}
		for (i = 0; i < numop; i++)
			hwprintf(nwin, col, "]");
	}
	hwprintf(nwin, col, "<br>");
	hwprintf(nwin, col, "&nbsp; <font color=\"#FF00FF\">DESCRIPTION</font>: <B>%s</B><br>", cmdar[cmd].desc);
	if (cmdar[cmd].aliases[0] != NULL) {
		hwprintf(nwin, col, "&nbsp; <font color=\"#FF00FF\">ALIASES</font>: <B>%s</B>", cmdar[cmd].aliases[0]);
		for (i = 1; cmdar[cmd].aliases[i] != NULL; i++)
			hwprintf(nwin, col, ", <B>%s</B>", cmdar[cmd].aliases[i]);
		hwprintf(nwin, col, "<br>");
	}
}

void	help_printhelp(const char *topic) {
	int	col,
		lines = 0;
	win_t	*nwin;

	if (inconn) {
		col = C(IMWIN,TEXT);
		nwin = &(curconn->curbwin->nwin);
	} else {
		col = C(CONN,TEXT);
		nwin = &(curconn->nwin);
	}

	if (topic == NULL)
		topic = "topics";

	if (strcasecmp(topic, "keys") == 0) {
		hwprintf(nwin, col, "Help on <B>%s</B>:<br>", topic);
		hwprintf(nwin, col, " &nbsp;Current key bindings can be viewed at any time with <font color=\"#00FF00\">/bind</font>:<br>\n");
		conio_bind(curconn, 0, NULL);
		hwprintf(nwin, col, " &nbsp;Key names beginning with ^ are entered by holding down the Ctrl key while pressing the listed key: ^N is Ctrl+N.<br>\n");
		hwprintf(nwin, col, " &nbsp;Key names beginning with M- are entered by holding down the Alt key while pressing the key, or by pressing Esc first, then typing the key: M-a is Alt+A.<br>\n");
		hwprintf(nwin, col, " &nbsp;IC is Ins and DC is Del on the numeric keypad. NPAGE and PPAGE are PgDn and PgUp.<br>\n");
		hwprintf(nwin, col, " &nbsp;Type <font color=\"#00FF00\">/bind keyname \"script\"</font> to change a key binding.<br>");
		hwprintf(nwin, col, "Use the scroll keys (PgUp and PgDn or Ctrl-R and Ctrl-Y) to view the entire help.<br>");
		hwprintf(nwin, col, "Type <font color=\"#00FF00\">/help</font> or visit <font color=\"#0000FF\">http://naim.n.ml.org/</font> for more information.<br>");
		return;
	} else if ((strcasecmp(topic, "settings") == 0) || (strcasecmp(topic, "variables") == 0)) {
		hwprintf(nwin, col, "Help on <B>%s</B>:<br>", topic);
		hwprintf(nwin, col, " &nbsp;Current configuration settings can be viewed at any time with <font color=\"#00FF00\">/set</font>:<br>\n");
		conio_set(curconn, 0, NULL);
		hwprintf(nwin, col, " &nbsp;Type <font color=\"#00FF00\">/set varname \"new value\"</font> to change a configuration variable.<br>");
		hwprintf(nwin, col, "Use the scroll keys (PgUp and PgDn or Ctrl-R and Ctrl-Y) to view the entire help.<br>");
		hwprintf(nwin, col, "Type <font color=\"#00FF00\">/help</font> or visit <font color=\"#0000FF\">http://naim.n.ml.org/</font> for more information.<br>");
		return;
	} else if (strcasecmp(topic, "commands") == 0) {
		int	cmd;

		for (cmd = cmdc-1; cmd >= 0; cmd--) {
			help_printcmd(nwin, col, cmd);
			hwprintf(nwin, col, "&nbsp;<br>");
		}
		hwprintf(nwin, col, "Use the scroll keys (PgUp and PgDn or Ctrl-R and Ctrl-Y) to view the entire help.<br>");
		hwprintf(nwin, col, "Type <font color=\"#00FF00\">/help</font> or visit <font color=\"#0000FF\">http://naim.n.ml.org/</font> for more information.<br>");
		return;
	}

	{
		HelpType
			helptext;

		if ((helptext = help_findtopic(topic)) != NULL) {
			hwprintf(nwin, col, "Help on <B>%s</B>:<br>", topic);
			for (lines = 0; helptext[lines] != NULL; lines++)
				hwprintf(nwin, col, " &nbsp;%s<br>", helptext[lines]);
		} else {
			int	cmd;

			for (cmd = 0; cmd < cmdc; cmd++)
				if (strcasecmp(cmdar[cmd].c, topic) == 0)
					break;
				else {
					int	al;

					for (al = 0; cmdar[cmd].aliases[al] != NULL; al++)
						if (strcasecmp(cmdar[cmd].aliases[al], topic) == 0)
							break;
					if (cmdar[cmd].aliases[al] != NULL)
						break;
				}

			if (cmd < cmdc) {
				hwprintf(nwin, col, "Help on /<font color=\"#00FF00\">%s</font>:<br>", cmdar[cmd].c);
				help_printcmd(nwin, col, cmd);
			} else {
				hwprintf(nwin, col,
					"No help available on <B>%s</B>.<br>", topic);
				return;
			}
		}
	}

	if (strcasecmp(topic, "commands") == 0)
		hwprintf(nwin, col, "<br>\n &nbsp;All commands start with a slash, so to add My Buddy to your buddylist you would type:<br> &nbsp;/addbuddy MyBuddy<br>");
	if (lines >= (faimconf.wstatus.widthy-1))
		hwprintf(nwin, col, "<br>\nUse the scroll keys (PgUp and PgDn or Ctrl-R and Ctrl-Y) to view the entire help.<br>\n");

	if (strcasecmp(topic, "topics") == 0) {
		int	i;
		char	buf[1024];
		size_t	buflen = 0;

		*buf = 0;
		hwprintf(nwin, col, "<br>Topics:<br>");
		for (i = 0; h_texts[i].tag != NULL; i++) {
			if ((2 + buflen + 1 + sizeof("/help ")-1 + 8) >= faimconf.wstatus.widthx) {
				set_echof(" %s\n", buf);
				*buf = 0;
				buflen = 0;
			}
			snprintf(buf, sizeof(buf), "%s /help %-8.8s", buf, h_texts[i].tag);
			buflen += 1 + sizeof("/help ")-1 + 8;
		}
		set_echof(" %s\n", buf);
	}
	hwprintf(nwin, col, "Type <font color=\"#00FF00\">/help</font> or visit <font color=\"#0000FF\">http://naim.n.ml.org/</font> for more information.<br>");
	if (inconn && (curconn->curbwin != NULL))
		hwprintf(nwin, col, "To reduce clutter, you may wish to bring down the status window. To do this, press the F1 key before using /help.<br>");
}
