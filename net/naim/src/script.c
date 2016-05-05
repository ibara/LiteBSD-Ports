/*  _ __   __ _ ___ __  __
** | '_ \ / _` |_ _|  \/  | naim
** | | | | (_| || || |\/| | Copyright 1998-2004 Daniel Reed <n@ml.org>
** |_| |_|\__,_|___|_|  |_| ncurses-based chat client
*/
#include <naim/naim.h>

#include "naim-int.h"

extern conn_t
	*curconn;

extern script_t
	*scriptar G_GNUC_INTERNAL;
extern int
	scriptc G_GNUC_INTERNAL;
script_t
	*scriptar = NULL;
int	scriptc = 0;

void	script_makealias(const char *alias, const char *script) {
	int	i;

	for (i = 0; i < scriptc; i++)
		if (strcasecmp(scriptar[i].name, alias) == 0)
			break;
	if (i == scriptc) {
		scriptc++;
		scriptar = realloc(scriptar, scriptc*sizeof(*scriptar));
		scriptar[i].name = scriptar[i].script = NULL;
		STRREPLACE(scriptar[i].name, alias);
	}
	STRREPLACE(scriptar[i].script, script);
}

int	script_parse(const char *script, const char *_arg) {
	char	*arg = NULL;
	int	a, b;

	if (script == NULL)
		return(0);

	if (_arg != NULL)
		_arg = arg = strdup(_arg);
	for (a = 0; (a < 50) && (arg != NULL); a++) {
		char	buf[1024], *tmp;

		tmp = atom(arg);
		snprintf(buf, sizeof(buf), "args%i*", a+1);
		secs_setvar(buf, tmp);
		arg = firstwhite(arg);
		snprintf(buf, sizeof(buf), "arg%i", a+1);
		secs_setvar(buf, tmp);
        }
	for (b = a; b < 50; b++) {
		char	buf[1024];

		snprintf(buf, sizeof(buf), "args%i*", b+1);
		secs_setvar(buf, "");
		snprintf(buf, sizeof(buf), "arg%i", b+1);
		secs_setvar(buf, "");
	}

	secs_script_parse(script);

	while (a > 0) {
		char	buf[1024];

		snprintf(buf, sizeof(buf), "arg%i", a);
		secs_setvar(buf, "");
		snprintf(buf, sizeof(buf), "args%i*", a);
		secs_setvar(buf, "");
		a--;
	}

	if (_arg != NULL)
		free((void *)_arg);
	return(1);
}

int	script_doalias(const char *alias, const char *args) {
	int	i;

	for (i = 0; i < scriptc; i++)
		if (strcasecmp(alias, scriptar[i].name) == 0)
			if (script_parse(scriptar[i].script, args) == 1)
				return(1);
	return(0);
}
