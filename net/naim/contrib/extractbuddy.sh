#!/bin/sh
if [ x"$1" = x"--help" ]; then
	echo "extractbuddy.sh by Daniel Reed <n@ml.org>"
	echo "  Extracts buddy screen names from .naimrc files."
	echo
	echo "Usage: $0 [<infile>]"
	echo "  Where <infile> is a valid .naimrc file, or else stdin is used."
	exit
fi
if [ x"$1" = x"--version" ]; then
	echo "$0 (naim) contrib"
	exit
fi

if [ x"$1" = x"--blt" ]; then
	BLTFORMAT=1
	shift
else
	BLTFORMAT=0
fi

if [ x"$1" = x ]; then
	INPUT=
else
	INPUT="$1"
fi
if [ x"$2" = x ]; then
	OUTPUT=
else
	OUTPUT="$2"
fi

if [ ${BLTFORMAT} = 1 ]; then
	echo "Config {"
	echo "	version 1"
	echo "}"
	USERNAME="`grep '^AIM:connect' ${INPUT} | gawk '{print $2}'`"
	echo "User {"
	echo "	screenname ${USERNAME}"
	echo "}"
	echo "Privacy {"
	echo "	allowList"
	echo "	blockList"
	echo "	pref AllowAll"
	echo "}"
	echo "Buddy {"
	echo "	prefs {"
	echo "		showIdleTime true"
	echo "		showSignonTime true"
	echo "		disclosureMode 3"
	echo "	}"
	echo "	list {"
	echo "		Buddies {"
	grep '^AIM:addbuddy' ${INPUT} | gawk -F '\t' '{printf("\t\t\t%s\n", $2);}'
	echo "		}"
	echo "	}"
	echo "}"
else
	grep '^AIM:addbuddy' ${INPUT} | gawk -F '\t' '{print $2;}'
fi
