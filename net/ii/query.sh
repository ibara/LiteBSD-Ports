#!/bin/sh
# ----------------------------------------------------
# Nico Golde <nico@ngolde.de>
# License: do whatever you want with this code
# Purpose: locate new queries for the ii irc client
# ----------------------------------------------------

IRCPATH=$HOME/irc
TMPFILE=$IRCPATH/queries.tmp

if [ ! -f $TMPFILE ]; then
    touch $TMPFILE
fi

echo "searching new query data"
for i in `find $IRCPATH -newer $TMPFILE -name 'out'`
do
    grep -v '\-!\-' $i  > /dev/null 2>&1 # if file doesnt just contain server stuff
    if [ $? -ne 1 ]; then
        # strip server, nickserv and channel out files
        echo $i | egrep -v -i "nickserv|#|$IRCPATH/(irc\.freenode\.net|irc\.oftc\.net)/out" > /dev/null 2>&1
        if [ $? -ne 1 ]; then
            printf "new data in: %s\n========================================================\n" "$i"
            tail -5 $i
        fi
    fi
done

touch $TMPFILE
