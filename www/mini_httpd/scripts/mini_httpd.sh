#!/bin/sh
#
# mini_httpd.sh - startup script for mini_httpd on FreeBSD
#
# This goes in /usr/local/etc/rc.d and gets run at boot-time.
#
# Variables available:
#   mini_httpd_enable='YES/NO'
#   mini_httpd_program='path'
#   mini_httpd_pidfile='path'
#   mini_httpd_devfs='path'
#
# PROVIDE: mini_httpd
# REQUIRE: LOGIN FILESYSTEMS
# KEYWORD: shutdown

. /etc/rc.subr

name='mini_httpd'
rcvar='mini_httpd_enable'

load_rc_config "$name"

# Defaults.
mini_httpd_enable="${mini_httpd_enable:-'NO'}"
mini_httpd_program="${mini_httpd_program:-'/usr/local/sbin/mini_httpd'}"
mini_httpd_pidfile="${mini_httpd_pidfile:-'/var/run/mini_httpd.pid'}"

mini_httpd_precmd ()
    {
    if [ '' != "$mini_httpd_devfs" ] ; then
	mount -t devfs devfs "$mini_httpd_devfs"
	devfs -m "$mini_httpd_devfs" rule -s 1 applyset
	devfs -m "$mini_httpd_devfs" rule -s 2 applyset
    fi
    }

mini_httpd_stop ()
    {
    kill -USR1 `cat "$pidfile"`
    }

command="$mini_httpd_program"
pidfile="$mini_httpd_pidfile"
start_precmd='mini_httpd_precmd'
stop_cmd='mini_httpd_stop'

run_rc_command "$1"
