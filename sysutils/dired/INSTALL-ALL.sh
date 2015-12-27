#!/bin/sh
# Build and install dired on all of our local architectures
# [22-Jun-1995]

DIR=$HOME/src/dired

echo ======================================== Making DEC Alpha
rsh -n alab01	"cd $DIR ;
		 echo ==================== "'`hostname`'" ==================== ;
		 make OPT='-O3 -Olimit 3000' realclean alpha install;
		 ls -l /usr/local/bin/dired"

echo ======================================== Making DECstation
rsh -n alfred	"cd $DIR ;
		 echo ==================== "'`hostname`'" ==================== ;
		 make OPT='-O2 -Olimit 3000' realclean ultrix install;
		 ls -l /usr/local/bin/dired"

echo ======================================== Making HP 9000
rsh -n eve	"cd $DIR ;
		 echo ==================== "'`hostname`'" ==================== ;
		 make OPT=+O2 realclean hpux install;
		 ls -l /usr/local/bin/dired"

echo ======================================== Making NeXT
rsh -n jabberwocky	"cd $DIR ;
		 echo ==================== "'`hostname`'" ==================== ;
		 make CC=gcc OPT=-O3 realclean next install;
		 ls -l /usr/local/bin/dired"

echo ======================================== Making Silicon Graphics IRIX 5.3
rsh -n todd	"cd $DIR ;
		 echo ==================== "'`hostname`'" ==================== ;
		 make OPT=-O2 realclean sgi install;
		 ls -l /usr/local/bin/dired"

echo ======================================== Making Sun SunOS
rsh -n reddwarf	"cd $DIR ;
		 echo ==================== "'`hostname`'" ==================== ;
		 make OPT=-O3 realclean sun install;
		 ls -l /usr/local/bin/dired"

rsh -n solitude	"cd $DIR ;
		 echo ==================== "'`hostname`'" ==================== ;
		 make install;
		 ls -l /usr/local/bin/dired"

echo ======================================== Making Sun Solaris
rsh -n sundown	"cd $DIR ;
		 echo ==================== "'`hostname`'" ==================== ;
		 make OPT=-O3 realclean sun-solaris install ;
		 ls -l /usr/local/bin/dired"
rsh -n sunspot	"cd $DIR ;
		 echo ==================== "'`hostname`'" ==================== ;
		 make install ;
		 ls -l /usr/local/bin/dired"
rsh -n sunshine	"cd $DIR ;
		 echo ==================== "'`hostname`'" ==================== ;
		 make install ;
		 ls -l /usr/local/bin/dired"
rsh -n chamberlin	"cd $DIR ;
		 echo ==================== "'`hostname`'" ==================== ;
		 make install ;
		 ls -l /usr/local/bin/dired"
rsh -n plot79	"cd $DIR ;
		 echo ==================== "'`hostname`'" ==================== ;
		 make install ;
		 ls -l /usr/local/bin/dired"

(cd $DIR ; make realclean )
