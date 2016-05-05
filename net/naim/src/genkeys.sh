#!/bin/sh

echo "struct {"
echo "	int	code;"
echo "	const char	*name;"
echo "} conio_bind_names[] = {"
echo "	{ '\t',	\"TAB\" },"
echo "	{ ' ',	\"SPACE\" },"
echo "	{ 27,	\"ESCAPE\" },"
echo "#ifdef KEY_MAX"
echo "# ifdef SIGHUP"
echo "#  define KEY_SIGHUP (2*KEY_MAX+SIGHUP)"
echo "	{ KEY_SIGHUP,	\"SIGHUP\" },"
echo "# endif"
echo "# ifdef SIGUSR1"
echo "#  define KEY_SIGUSR1 (2*KEY_MAX+SIGUSR1)"
echo "	{ KEY_SIGUSR1,	\"SIGUSR1\" },"
echo "# endif"
echo "# ifdef SIGUSR2"
echo "#  define KEY_SIGUSR2 (2*KEY_MAX+SIGUSR2)"
echo "	{ KEY_SIGUSR2,	\"SIGUSR2\" },"
echo "# endif"
echo "#endif"
echo '#include <ncurses.h>' \
	| ${CPP} -dD - \
	| grep '^#define KEY_' \
	| grep -v '^#define KEY_CODE_YES' \
	| grep -v '^#define KEY_MIN' \
	| ${AWK} '{printf("%s\n", $2);}' \
	| grep -v '(.*)' \
	| sed 's/^KEY_\(.*\)$/\1/g' \
	| ${AWK} '{printf("#ifdef KEY_%s\n\t{ KEY_%s,\t\"%s\" },\n#endif\n", \
		$1, $1, $1);}'
echo "};"
echo ""
grep 'CONIO_KEY_.*:' "${SRCDIR}/conio.c" \
	| sed 's/^.*CONIO_KEY_\(.*\):.*$/\1/g' \
	| sort -f \
	| uniq \
	| ${AWK} '{printf("#define CONIO_KEY_%s %d\n", $1, num++);}'
echo ""
echo "struct {"
echo "	const char	*name,"
echo "			*desc;"
echo "} conio_key_names[] = {"
grep 'CONIO_KEY_.*:' "${SRCDIR}/conio.c" \
	| sed 's#^.*CONIO_KEY_\(.*\):.*/\* \(.*\) \*/.*$#\1	\2#g' \
	| sed 's/^.*CONIO_KEY_\(.*\):.*$/\1	/g' \
	| sort -f \
	| uniq \
	| ${AWK} -F '	' '{printf("\t{ \":%s\",\t\"%s\" },\n", $1, $2);}'
echo "};"
