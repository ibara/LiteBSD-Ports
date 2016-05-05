#ifndef IRC_SUPPORT_CHANNEL
# define IRC_SUPPORT_CHANNEL "#naim"
#endif

char *naimrc_sample[] = {
	"eval if ($want_aim != 0) /newconn AIM TOC2",
	"eval if ($want_irc != 0) /newconn IRC IRC",
	"eval if ($want_icq != 0) /newconn ICQ TOC2",
	"eval if ($want_lily != 0) /newconn Lily Lily",
	"/clear",

	"eval if ($want_aim != 0) /echo <B>You do not have a .naimrc file, so I am using defaults. You can use the <font color=#00FF00>/save</font> command to create a new .naimrc file.</B>",
	"eval if ($want_irc != 0) /echo <B>You do not have a .nircrc file, so I am using defaults. You can use the <font color=#00FF00>/save</font> command to create a new .nircrc file.</B>",
	"eval if ($want_icq != 0) /echo <B>You do not have a .nicqrc file, so I am using defaults. You can use the <font color=#00FF00>/save</font> command to create a new .nicqrc file.</B>",
	"eval if ($want_lily != 0) /echo <B>You do not have a .nlilyrc file, so I am using defaults. You can use the <font color=#00FF00>/save</font> command to create a new .nlilyrc file.</B>",
	"/help",

	"eval if ($want_aim != 0) /aim:addbuddy \"naim help\" \"naim author\" Dan Reed",
	"eval if ($want_irc != 0) /irc:addbuddy n \"naim author\" Dan Reed",
	"eval if ($want_icq != 0) /icq:addbuddy 19033926 \"naim author\" Dan Reed",
	"eval if ($want_lily != 0) /lily:addbuddy n \"naim author\" Dan Reed",

	"eval if ($want_aim != 0) /echo <B>Commands start with a slash. For example, to sign on, type <font color=#00FF00>/connect \"screen name\"</font>. See also <font color=#00FF00>/help connect</font>.</B>",
	"eval if ($want_irc != 0) /echo <B>I have gone ahead and opened a new \"connection\" window for IRC. I'm going to try to connect to IRC now, so wish me luck.</B>",
	"eval if ($want_icq != 0) /echo <B>Commands start with a slash. For example, to sign on, type <font color=#00FF00>/connect ICQnumber</font>. See also <font color=#00FF00>/help connect</font>.</B>",
	"eval if ($want_lily != 0) /echo <B>Commands start with a slash. For example, to sign on, type <font color=#00FF00>/connect accountname</font>. See also <font color=#00FF00>/help connect</font>.</B>",

	"eval if (($want_irc != 0) && ($sys_argc >= 2)) /IRC:connect $sys_arg1 $sys_arg2",
	"eval if (($want_irc != 0) && ($sys_argc == 1)) /IRC:connect $sys_arg1",
	"eval if (($want_irc != 0) && ($sys_argc == 0)) /IRC:connect $sys_user",
	"eval if ($want_irc != 0) /IRC:join " IRC_SUPPORT_CHANNEL,
};
