#!/bin/sh

echo '
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
'

echo '#include "conio.c"' \
        | ${CPP} -DCONIOCPP -dD - \
	| grep '^CONIOFUNC(.*).*$' \
	| sed 's/^\(CONIOFUNC(.*)\).*$/\1;/g'

echo '
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
'

echo '#include "conio.c"' \
        | ${CPP} -DCONIOCPP -dD - \
	| sed 's/^CONIO\(....\)(\(.*\)).*$/\1,\2/g' \
	| ${AWK} -F ',' '{
		if ((inalia == 1) && ($1 != "ALIA")) {
			inalia = 0;
			printf(" NULL },");
			indesc = 1;
			descs = 0;
		}
		if ((indesc == 1) && ($1 != "DESC")) {
			indesc = 0;
			if (descs == 0)
				printf("	NULL,");
			inargs = 1;
			printf("	{");
		}
		if ((inargs == 1) && ($1 != "AREQ") && ($1 != "AOPT") && ($1 != "WHER")) {
			inargs = 0;
			printf(" { -1, -1, NULL } },");
			printf("	%d,	%d,	C_%s },\n", minarg, minarg+maxarg, funcwhere);
		}

		if ($1 == "FUNC") {
			funcn = $2;
			minarg = 0;
			maxarg = 0;
			funcwhere = "ANYWHERE";
			printf("	{ \"%s\",	conio_%s,	{", $2, $2);
			inalia = 1;
		} else if ($1 == "WHER")
			funcwhere = $2;
		else if ($1 == "ALIA")
			printf(" \"%s\",", $2);
		else if ($1 == "DESC") {
			if (descs == 0)
				printf("	\"%s\",", $2);
			descs++;
		} else if ($1 == "AREQ") {
			if ($2 == "string")
				atype = "s";
			else if ($2 == "int")
				atype = "i";
			else if ($2 == "bool")
				atype = "b";
			else if ($2 == "window")
				atype = "W";
			else if ($2 == "buddy")
				atype = "B";
			else if ($2 == "chat")
				atype = "C";
			else if ($2 == "filename")
				atype = "F";
			else if ($2 == "varname")
				atype = "V";
			else if ($2 == "entity")
				atype = "E";
			else
				atype = "?";
			printf(" { 1, '"'"'%c'"'"', \"%s\" },", atype, $3);
			minarg++;
		} else if ($1 == "AOPT") {
			if ($2 == "string")
				atype = "s";
			else if ($2 == "int")
				atype = "i";
			else if ($2 == "bool")
				atype = "b";
			else if ($2 == "window")
				atype = "W";
			else if ($2 == "buddy")
				atype = "B";
			else if ($2 == "chat")
				atype = "C";
			else if ($2 == "filename")
				atype = "F";
			else if ($2 == "varname")
				atype = "V";
			else if ($2 == "entity")
				atype = "E";
			else
				atype = "?";
			printf(" { 0, '"'"'%c'"'"', \"%s\" },", atype, $3);
			maxarg++;
		}
	}'

echo '
};
const int
	cmdc = sizeof(cmdar)/sizeof(*cmdar);
#endif
'
