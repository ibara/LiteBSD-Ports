/*  _ __   __ _ ___ __  __
** | '_ \ / _` |_ _|  \/  | naim
** | | | | (_| || || |\/| | Copyright 1998-2004 Daniel Reed <n@ml.org>
** |_| |_|\__,_|___|_|  |_| ncurses-based chat client
*/
#include <naim/naim.h>

#include "naim-int.h"

extern conn_t		*curconn;
extern faimconf_t	faimconf;
extern rc_var_s_t	rc_var_s_ar[];
extern const int	rc_var_s_c;
extern rc_var_i_t	rc_var_i_ar[];
extern const int	rc_var_i_c;
extern rc_var_i_t	rc_var_b_ar[];
extern const int	rc_var_b_c;

const char
	*set_tabcomplete(conn_t *const conn, const char *start, const char *buf, const int bufloc, int *const match, const char **desc) {
	const char
		*var;
	size_t	stublen;

	if (*start == '$')
		start++;
	stublen = strlen(start);
	if (stublen == 0)
		return(NULL);

	{
		static char
			descbuf[1024];
		int	i;

		for (i = 0; i < rc_var_b_c; i++)
			if (strncasecmp(start, rc_var_b_ar[i].var, stublen) == 0) {
				if (match != NULL)
					*match = bufloc - (start-buf);
				if (desc != NULL) {
					if (rc_var_i_ar[i].desc != NULL)
						snprintf(descbuf, sizeof(descbuf), "[0/1] (%s)", rc_var_b_ar[i].desc);
					else
						snprintf(descbuf, sizeof(descbuf), "[0/1]");
					*desc = descbuf;
				}
				return(rc_var_b_ar[i].var);
			}
		for (i = 0; i < rc_var_i_c; i++)
			if (strncasecmp(start, rc_var_i_ar[i].var, stublen) == 0) {
				if (match != NULL)
					*match = bufloc - (start-buf);
				if (desc != NULL) {
					if (rc_var_i_ar[i].desc != NULL)
						snprintf(descbuf, sizeof(descbuf), "[0-*] (%s)", rc_var_i_ar[i].desc);
					else
						snprintf(descbuf, sizeof(descbuf), "[0-*]");
					*desc = descbuf;
				}
				return(rc_var_i_ar[i].var);
			}
		for (i = 0; i < rc_var_s_c; i++)
			if (strncasecmp(start, rc_var_s_ar[i].var, stublen) == 0) {
				if (match != NULL)
					*match = bufloc - (start-buf);
				if (desc != NULL) {
					if (rc_var_s_ar[i].desc != NULL)
						snprintf(descbuf, sizeof(descbuf), "[str] (%s)", rc_var_s_ar[i].desc);
					else
						snprintf(descbuf, sizeof(descbuf), "[str]");
					*desc = descbuf;
				}
				return(rc_var_s_ar[i].var);
			}
	}

	{
		void	*_listvarsarg;

		secs_listvars(0, NULL, &_listvarsarg);
		while ((var = secs_listvars(1, NULL, &_listvarsarg)) != NULL)
			if (strncasecmp(start, var, stublen) == 0) {
				if (match != NULL)
					*match = bufloc - (start-buf);
				if (desc != NULL)
					*desc = NULL;
				return(var);
			}
	}
	return(NULL);
}

void	set_echof(const char *const format, ...) {
	char	buf[1024],
		buf2[1024];
	int	i, i2;
	va_list	msg;

	if (curconn == NULL)
		return;

	va_start(msg, format);
	vsnprintf(buf, sizeof(buf), format, msg);
	va_end(msg);

	for (i2 = i = 0; (buf[i] != 0) && (i < (sizeof(buf2)-1)); i++)
		if (buf[i] == ' ') {
			if ((i2 + sizeof("&nbsp;")) < sizeof(buf2)) {
				strcpy(buf2+i2, "&nbsp;");
				i2 += sizeof("&nbsp;")-1;
			} else
				break;
		} else if (buf[i] == '&') {
			if ((i2 + sizeof("&amp;")) < sizeof(buf2)) {
				strcpy(buf2+i2, "&amp;");
				i2 += sizeof("&amp;")-1;
			} else
				break;
		} else if (buf[i] == '<') {
			if ((i2 + sizeof("&lt;")) < sizeof(buf2)) {
				strcpy(buf2+i2, "&lt;");
				i2 += sizeof("&lt;")-1;
			} else
				break;
		} else if (buf[i] == '>') {
			if ((i2 + sizeof("&gt;")) < sizeof(buf2)) {
				strcpy(buf2+i2, "&gt;");
				i2 += sizeof("&gt;")-1;
			} else
				break;
		} else
			buf2[i2++] = buf[i];
	assert(i2 < sizeof(buf2));
	buf2[i2] = 0;

	if (inconn) {
		assert(curconn->curbwin != NULL);

		hwprintf(&(curconn->curbwin->nwin), C(IMWIN,EVENT), "<B>%s</B><br>", buf2);
	} else
		hwprintf(&(curconn->nwin), C(CONN,EVENT), "<B>%s</B><br>", buf2);
}

void	set_setvar(const char *var, const char *val) {
	if (var == NULL) {
		void	*_listvarsarg;

		secs_listvars(0, NULL, &_listvarsarg);
		set_echof(" %-16.16s %-30s[type] Description\n", "Variable name", "Value");
		while ((var = secs_listvars(1, NULL, &_listvarsarg)) != NULL) {
			val = secs_getvar(var);
			if ((val != NULL) && (*val != 0)) {
				char	*prot;

				if (((prot = strchr(var, ':')) == NULL)
					|| (strcasecmp(prot+1, "password") != 0)) {
					const char
						*ret,
						*desc;
					char	buf[1024];

					if (strchr(val, ' ') != NULL) {
						snprintf(buf, sizeof(buf), "\"%s\"", val);
						val = buf;
					}

					ret = set_tabcomplete(curconn, var, var, strlen(var), NULL, &desc);
					if ((ret == NULL) || (desc == NULL) || (strcasecmp(ret, var) != 0))
						desc = "[str]";

					if ((1 + 16 + 1 + 30 + 1 + strlen(desc)) >= faimconf.wstatus.widthx) {
						set_echof(" %-16.16s %-30s\n", var, val);
						set_echof("     %s\n", desc);
					} else if (strlen(val) <= 30)
						set_echof(" %-16.16s %-30s %s\n", var, val, desc);
					else {
						set_echof(" %-16.16s %-30s\n", var, val);
						set_echof(" %-16.16s %-30s %s\n", "", "", desc);
					}
					set_echof("\n");
				}
			}
		}
		return;
	}

	if (*var == '$')
		var++;

	if (val == NULL)
		echof(curconn, NULL, "$%s is \"%s\"\n", var, secs_getvar(var));
	else if (secs_setvar(var, val) == 0)
		echof(curconn, NULL, "\"%s\" is an invalid input for $%s\n", val, var);
	else
		echof(curconn, NULL, "$%s is now \"%s\"\n", var, secs_getvar(var));
}
