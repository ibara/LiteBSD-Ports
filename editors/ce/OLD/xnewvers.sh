#!/bin/sh -
#
# newversion -- increment the version number (stored in `version') and make
#		a new version.c
#

if [ ! -r version ]; then
	echo 0 > version
fi

touch version

awk '	{	version = $1 + 1; } \
END	{	printf "int versnum = %d;\n", version > "xvers.c"; \
		printf "%d\n", version > "version"; }' < version

DATE=`date +'%Y %b %d %H:%M:%S %Z'`

echo 'char datecompiled[] = "'$DATE'";' >> xvers.c
