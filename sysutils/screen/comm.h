/*
 * This file is automagically created from comm.c -- DO NOT EDIT
 */

struct comm
{
  char *name;
  int flags;
#ifdef MULTIUSER
  AclBits userbits[ACL_BITS_PER_CMD];
#endif
};

#define ARGS_MASK	(3)

#define ARGS_ZERO	(0)
#define ARGS_ONE	(1)
#define ARGS_TWO	(2)
#define ARGS_THREE	(3)

#define ARGS_PLUSONE	(1<<2)
#define ARGS_PLUSTWO	(1<<3)
#define ARGS_ORMORE	(1<<4)

#define NEED_FORE	(1<<5)	/* this command needs a fore window */
#define NEED_DISPLAY	(1<<6)	/* this command needs a display */

#define ARGS_ZEROONE	(ARGS_ZERO|ARGS_PLUSONE)
#define ARGS_ONETWO	(ARGS_ONE |ARGS_PLUSONE)
#define ARGS_TWOTHREE	(ARGS_TWO |ARGS_PLUSONE)
#define ARGS_ZEROTWO	(ARGS_ZERO|ARGS_PLUSTWO)
#define ARGS_ZEROONETWO	(ARGS_ZERO|ARGS_PLUSONE|ARGS_PLUSTWO)

struct action
{
  int nr;
  char **args;
};

#define RC_ILLEGAL -1

#define RC_ACTIVITY 0
#define RC_AKA 1
#define RC_ALLPARTIAL 2
#define RC_AT 3
#define RC_AUTODETACH 4
#define RC_BELL 5
#define RC_BELL_MSG 6
#define RC_BIND 7
#define RC_BREAK 8
#define RC_C1 9
#define RC_CHARSET 10
#define RC_CHDIR 11
#define RC_CLEAR 12
#define RC_COLON 13
#define RC_COMMAND 14
#define RC_CONSOLE 15
#define RC_DEBUG 16
#define RC_DEFC1 17
#define RC_DEFCHARSET 18
#define RC_DEFESCAPE 19
#define RC_DEFFLOW 20
#define RC_DEFGR 21
#define RC_DEFHSTATUS 22
#define RC_DEFKANJI 23
#define RC_DEFMODE 24
#define RC_DEFMONITOR 25
#define RC_DEFOBUFLIMIT 26
#define RC_DEFWRAP 27
#define RC_DEFWRITELOCK 28
#define RC_DETACH 29
#define RC_DIGRAPH 30
#define RC_DISPLAYS 31
#define RC_DUMPTERMCAP 32
#define RC_ECHO 33
#define RC_ESCAPE 34
#define RC_FLOW 35
#define RC_GR 36
#define RC_HARDCOPY 37
#define RC_HARDCOPY_APPEND 38
#define RC_HARDCOPYDIR 39
#define RC_HARDSTATUS 40
#define RC_HEIGHT 41
#define RC_HELP 42
#define RC_INFO 43
#define RC_KANJI 44
#define RC_KILL 45
#define RC_LASTMSG 46
#define RC_LICENSE 47
#define RC_LOG 48
#define RC_LOGFILE 49
#define RC_META 50
#define RC_MONITOR 51
#define RC_MSGMINWAIT 52
#define RC_MSGWAIT 53
#define RC_NETHACK 54
#define RC_NEXT 55
#define RC_NUMBER 56
#define RC_OBUFLIMIT 57
#define RC_OTHER 58
#define RC_PARTIAL 59
#define RC_POW_BREAK 60
#define RC_PREV 61
#define RC_PRINTCMD 62
#define RC_PROCESS 63
#define RC_QUIT 64
#define RC_READREG 65
#define RC_REDISPLAY 66
#define RC_REGISTER 67
#define RC_RESET 68
#define RC_SCREEN 69
#define RC_SELECT 70
#define RC_SESSIONNAME 71
#define RC_SETENV 72
#define RC_SHELL 73
#define RC_SHELLAKA 74
#define RC_SHELLTITLE 75
#define RC_SILENCE 76
#define RC_SILENCEWAIT 77
#define RC_SLEEP 78
#define RC_SLOWPASTE 79
#define RC_SORENDITION 80
#define RC_STARTUP_MESSAGE 81
#define RC_STUFF 82
#define RC_SUSPEND 83
#define RC_TERM 84
#define RC_TERMCAP 85
#define RC_TERMCAPINFO 86
#define RC_TERMINFO 87
#define RC_TIME 88
#define RC_TITLE 89
#define RC_UNSETENV 90
#define RC_VBELL 91
#define RC_VBELL_MSG 92
#define RC_VBELLWAIT 93
#define RC_VERSION 94
#define RC_WALL 95
#define RC_WIDTH 96
#define RC_WINDOWS 97
#define RC_WRAP 98
#define RC_WRITELOCK 99
#define RC_XOFF 100
#define RC_XON 101
#define RC_ZOMBIE 102

#define RC_LAST 102
