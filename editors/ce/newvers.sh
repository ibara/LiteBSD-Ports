#!/bin/sh -
#
# newversion -- increment the version number (stored in `version') and make
#		a new vers.c
# Given an argument of `x11', uses and creates the X11 versions of these files
#

if [ "$1" = "x11" ]; then
	VFILE=xversion
	VC=xvers.c
else
	VFILE=version
	VC=vers.c
fi

if [ ! -r $VFILE ]; then
	echo 0 > $VFILE
fi

touch $VFILE

vn=`cat $VFILE`
if [ -z "$vn" ]; then
	vn=0
fi
version=`expr $vn + 1`
echo $version > $VFILE

DATE=`date +'%Y %b %d %H:%M:%S %Z'`

echo "int versnum = ${version};" > $VC
echo 'char datecompiled[] = "'$DATE'";' >> $VC
