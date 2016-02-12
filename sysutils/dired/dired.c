#define VERSION "3.05 [3-Jan-1997]"
/* Compile Examples: "make sun", "make linux", "make hpux" */
/* See the Makefile for various targets (Beebe 1995) */
/* Command line: See function help() below, or use "dired -h". */
/* Documentation and historical notes appear at the end of the file */
/**********************************************************************
 * DIRED: Directory tree editor and navigator.                        *
 *                                                                    *
 * Authors: 1980 Stuart Cracraft (mclure@sri-unix) (original 9/80)    *
 *          1981-1983 Jay Lepreau      lepreau@cs.utah.edu (1996)     *
 *          1981-1984 Charles Hill     philabs!crh     (1984)         *
 *          1990-1996 Nelson Beebe     beebe@math.utah.edu (1996)     *
 *          1995-1996 Grant Gustafson  gustafso@math.utah.edu (1996)  *
 *                                                                    *
 * Usage: dired [options] [dir|files]                                 *
 *                                                                    *
 * "Dired" is an acronym for "directory editor". It is pronounced     *
 * like "DEAR ED". The main function of dired is navigation           *
 * through large directory structures present in unix systems. The    *
 * original program created in 1980 had only this basic function and  *
 * the ability to tag a file or an entire directory tree, to be       *
 * deleted later. The efficiency of the "tag and delete later"        *
 * design proved itself, by eliminating the typing of long file       *
 * names, and by abandoning the use of wildcard file names in         *
 * delete commands.                                                   *
 *                                                                    *
 * DIRED includes some common unix maintenance commands, such as      *
 * directory tree navigation, viewing file contents, cleaning up a    *
 * directory, moving files, making archives and creating backups. It  *
 * is useful for tagging a group of files and then systematically     *
 * launching a program on each file.                                  *
 **********************************************************************/

/* dired.c 1996 Rewrite by GBG */

/* Basic functions. Greater than 999 to distinguish them from chars. */
/* To add a new function, edit this list, helptxt[], GNUkeys[] and */
/* the function diredLoop(). To add/remove keys, edit GNUkeys[]. */
enum {
ABORTPUSHD=1000,         /* Abort direds, pushd dir */
QUITDELETE=1001,         /* Check 'D' tags, exit. */
SUSPENDPUSHD=1002,       /* Suspend dired and pushd */
SUSPENDONLY=1003,        /* Suspend dired, simulated */
ABORTLEVEL=1004,         /* Abort the current level only */
ABORTALL=1005,           /* Abort the chain of direds  ***crh */
PRINTFILE=1006,          /* Print a file */
PATHSTATS=1007,          /* Display full pathname of the file and system stats */
SEARCHFORWARD=1008,      /* Forward */
SEARCHREVERSE=1009,      /* Reverse */
TAGSEARCH=1010,          /* Search forward and tag */
SEARCHNEXT=1012,         /* Next matching forward search */
SEARCHPREV=1013,         /* Next matching reverse search */
SHELLPAUSE=1014,         /* Toggle pause after shell command */
SYSTEMCMD=1015,          /* Execute a system command */
REPEATCMD=1016,          /* repeat the previous ! command */
REREADDIR=1017,          /* re-read the directory */
SORTFORWARD=1018,        /* Forward sort */
SORTREVERSE=1019,        /* Reverse sort */
EDITFILEDIR=1020,        /* Edit a file or directory */
UPDIR=1021,              /* Go up a directory */
MORELESS=1022,           /* 'more' a file */
GROFFMANUAL=1023,        /* 'groff' a file as a manual page, GBG 12/1995 */
TYPEFILE=1024,           /* Quickly type the file -- added 5/81, J.Lepreau */
REFRESHSCR=1025,         /* Refresh screen */
SETFORMAT=1026,          /* Format setup for screen */
REFRESHLINE=1027,        /* Refresh line, clear lower screen */
WINDOWFORWARD=1028,      /* forward window */
WINDOWBACK=1029,         /* backward window */
NEXTFILE=1030,           /* Next file. **crh***/
PREVFILE=1031,           /* Previous file. ****crh****/
SETPATHSTATS=1032,       /* Toggle stats and pathname display*/
SETCOLORS=1033,          /* Toggle Color display */
GOTOFILE=1034,           /* Goto file number as in vi */
SETTAG=1035,             /* TAB key tags or untags a file */
SETDELETETAG=1036,       /* delete file */
SETDELETEALL=1037,       /* delete all files, Kill */
SETCHANGETAG=1038,       /* Tag change on files */
TAGSQUIGGLE=1039,        /* delete editor backups of form "file~" NHFB */
TAGPOUND=1040,           /* delete editor backups of form "#file" NHFB */
TAGDELSEARCH=1041,       /* Tag 'D' on reg expr search */
UNTAGALL=1042,           /* untag all  */
UNDELETEFILE=1043,       /* undelete file */
GOTOTOP=1044,            /* Goto top of file list */
GOTOBOTTOM=1045,         /* Goto end of file list */
JUMPTOMARK=1046,         /* jump to top mark, pop mark stack */
JUMPTOVIEW=1047,         /* jump to top view, pop view stack */
SETBOOKMARKS=1048,       /* save bookmark */
WRITELIST=1049,          /* Write file list to /tmp */
CHMODFILE=1050,          /* Chmod on current file */
SHOWHELP=1051,           /* Help display "man dired" */
SHOWTUTOR=1052,          /* Command key tutor */
GOTODIR=1053,            /* go to a requested dir */
SETERROR=1054,           /* Error exit */
ESCRETURN=1055,          /* Esc return for key tutor */
DOUBLEESC=1056,          /* Esc Esc for key tutor */
QUOTEKEY=1057,           /* Esc Op, insert next char raw */
SETSLOWSTAT=1058,        /* S, toggle slowstat on read */
COPYFILE=1059,           /* Copy current file somewhere */
MOVEFILE=1060,           /* Move current file somewhere */
};

/* Key tutor list. Used to display key assignments and show duplicate */
/* keys, eg, function keys. See function diredKeyTutor(). */

struct keyNstring {int token; const char *msg;};
static
struct keyNstring helptxt[] = {
{-1,            "Command Key tutor. ESC-ESC=Exit. ESC-RETURN=Execute."},
{SHOWHELP,      "Manual page [January 1997]. Describe DIRED 3.05 in detail."},
{PREVFILE,      "Cursor up one."},
{NEXTFILE,      "Cursor down one."},
{WINDOWBACK,    "Back one page."},
{WINDOWFORWARD, "Forward one page."},
{GOTOTOP,       "Go to first file."},
{GOTOBOTTOM,    "Go to last file."},
{SETTAG,        "Tag '*' or untag a file (toggle). TAB is Ctrl-I."},
{TAGSEARCH,     "Tag '*' on forward search matches for regular expression."},
{SYSTEMCMD,     "Run shell command or alias /w filename substitution."},
{QUOTEKEY,      "Insert next char raw into shell command."},
{REPEATCMD,     "Execute again the last memorized shell command."},
{SETFORMAT,     "Format setup for the screen."},
{SHELLPAUSE,    "Toggle pause and prompting for shell commands."},
{SETSLOWSTAT,   "Toggle stat() calls on readdir."},
{SEARCHFORWARD, "Search forward for file matching regular expression."},
{SEARCHREVERSE, "Search reverse for file matching regular expression."},
{SEARCHNEXT,    "Search forward for next match to saved regexp."},
{SEARCHPREV,    "Search reverse for previous match to saved regexp."},
{SETDELETETAG,  "Tag 'D' on current file or directory (delete later)."},
{SETDELETEALL,  "Tag all entries 'D' (delete later)."},
{TAGDELSEARCH,  "Tag 'D' on forward search matches for regular expression."},
{TAGSQUIGGLE,   "Tag 'D' on all file names that end in '~'."},
{TAGPOUND,      "Tag 'D' on all file names that start with '#'."},
{UNDELETEFILE,  "Untag. New tag is SPACE."},
{UNTAGALL,      "Untag all. New tags are SPACE."},
{SETCHANGETAG,  "Tags: change old tags to new tags."},
{CHMODFILE,     "Chmod: change file protection."},
{COPYFILE,      "Copy a file to somewhere."},
{MOVEFILE,      "Move a file to somewhere."},
{EDITFILEDIR,   "Launch EDITOR on a file or DIRED on a directory."},
{UPDIR,         "Launch DIRED on parent directory."},
{GOTODIR,       "Prompt for directory then launch DIRED."},
{SETPATHSTATS,  "Toggle other display of pathname and stats."},
{SETCOLORS,     "Toggle colors in sequence: Internal, Environment, Off."},
{SHOWTUTOR,     "Key Tutor. Describe key. Execute key."},
{REFRESHLINE,   "Clear lower screen and refresh line."},
{REFRESHSCR,    "Recognize new window size and refresh screen."},
{JUMPTOVIEW,    "Go to file before last search or page move."},
{MORELESS,      "More. Launch PAGER on this file."},
{SETBOOKMARKS,  "Make bookmark"},
{JUMPTOMARK,    "Jump to last bookmark."},
{GOTOFILE,      "Goto file number, eg, 13G --> file 13."},
{PRINTFILE,     "Print file contents on the default printer."},
{PATHSTATS,     "Display full pathname of the file and system stats."},
{ABORTLEVEL,    "Abort this copy of DIRED."},
{ABORTALL,      "Abort the DIRED chain and exit elegantly."},
{ABORTPUSHD,    "Abort DIRED chain and PUSHD to the displayed directory."},
{SUSPENDPUSHD,  "Suspend DIRED and PUSHD to the displayed directory."},
{REREADDIR,     "Reload directory."},
{SORTFORWARD,   "Sort the file list (8 methods)."},
{SORTREVERSE,   "Reverse sort of the file list (8 methods)."},
{GROFFMANUAL,   "Groff current file as a manual page."},
{TYPEFILE,      "Type current file on terminal with pause (q quits)."},
{WRITELIST,     "Write the formatted file list to a unique file in /tmp."},
{QUITDELETE,    "Process tags 'D' and exit."},
{SUSPENDONLY,   "Suspend DIRED."}
};
#define helptxtSIZE (sizeof(helptxt)/sizeof(helptxt[0]))

/* Dired GNU keys */
/* The key list, which may contain escape sequences and duplicates, */
/* is historical more than practical. It is easy to change, and that */
/* is perhaps its main advantage. */
struct Keymap {
 int Code;      /* Internal integer code assigned in enum above */
 char *Str;     /* Character string used to launch the command */
};
struct Keymap GNUkeys[] = {
{  ABORTPUSHD,    "Q"},         /* Abort direds, pushd dir */
{  QUITDELETE,    "q"},         /* Check 'D' tags, exit. */
{  SUSPENDPUSHD,  "z"},         /* Suspend dired and pushd */
{  SUSPENDONLY,   "\032"},      /* Suspend dired, simulated */
{  ABORTLEVEL,    "a"},         /* Abort completely */
{  ABORTALL,      "A"},         /* Abort the chain of direds  ***crh */
{  PRINTFILE,     "P"},         /* Print a file */
{  PATHSTATS,     "p"},         /* Display full pathname of the file and system stats */
{  SEARCHFORWARD, "/"},         /* Forward search regexpr */
{  SEARCHFORWARD, "\023"},      /* ctrl-S Forward search regexpr */
{  SEARCHREVERSE, "\\"},        /* Reverse search regexpr */
{  TAGSEARCH,     "*"},         /* Search forward and tag */
{  SEARCHNEXT,    "n"},         /* Next matching forward search */
{  SEARCHPREV,    "N"},         /* Next matching reverse search */
{  SHELLPAUSE,    "+"},         /* Toggle pause after shell command */
{  SYSTEMCMD,     "x"},         /* Execute a system command */
{  SYSTEMCMD,     "!"},         /* Execute a system command */
{  SYSTEMCMD,     "\033x"},     /* Execute a system command */
{  SYSTEMCMD,     "\033X"},     /* Execute a system command */
{  SYSTEMCMD,     "\030"},      /* Execute a system command */
{  SETSLOWSTAT,   "S"},         /* Toggle stat() calls on readdir. */
{  QUOTEKEY,      "\021"},      /* Insert next char raw into system command */
{  QUOTEKEY,      "\033Op"},    /* Insert next char raw into system command */
{  QUOTEKEY,      "\033[2~"},   /* Insert next char raw into system command */
{  QUOTEKEY,      "\033[2z"},   /* Insert next char raw into system command */
{  REPEATCMD,     "."},         /* repeat the previous ! command */
{  REREADDIR,     "R"},         /* re-read the directory */
{  REREADDIR,     "X"},         /* re-read the directory NHFB */
{  SORTFORWARD,   "s"},         /* Forward sort */
{  SORTREVERSE,   "r"},         /* Reverse sort */
{  EDITFILEDIR,   "e"},         /* Edit a file or directory */
{  UPDIR,         "E"},         /* Go up a directory */
{  MORELESS,      "m"},         /* 'more' a file */
{  GROFFMANUAL,   "g"},         /* 'groff' a file as a manual page, GBG 12/1995 */
{  TYPEFILE,      "t"},         /* quickly type the file -- added 5/81, J.Lepreau */
{  REFRESHSCR,    "\014"},      /* Refresh screen */
{  REFRESHSCR,    "\027"},      /* Refresh screen NHFB */
{  SETFORMAT,     "F"},         /* Format setup for screen */
{  REFRESHLINE,   "l"},         /* Refresh line, clear lower screen */
{  WINDOWFORWARD, "f"},         /* forward window */
{  WINDOWFORWARD, "\006"},      /* forward window */
{  WINDOWFORWARD, "\026"},      /* ctrl-v forward window */
{  WINDOWFORWARD, "\033[6~"},   /* forward window */
{  WINDOWFORWARD, "\033[222z"}, /* forward window */
{  WINDOWFORWARD, "\033OC"},    /* forward window */
{  WINDOWFORWARD, "\033Ov"},    /* forward window */
{  WINDOWFORWARD, "\033[219z"}, /* forward window */
{  WINDOWFORWARD, "\033[C"},    /* forward window */
{  WINDOWFORWARD, "\033[6z"},   /* forward window */
{  WINDOWFORWARD, "\033Os"},    /* forward window */
{  WINDOWFORWARD, "\033Ok"},    /* forward window */
{  WINDOWBACK,    "b"},         /* backward window */
{  WINDOWBACK,    "\002"},      /* backward window */
{  WINDOWBACK,    "\366"},      /* Alt-V backward window */
{  WINDOWBACK,    "\033v"},     /* backward window */
{  WINDOWBACK,    "\033V"},     /* backward window */
{  WINDOWBACK,    "\033[5~"},   /* backward window */
{  WINDOWBACK,    "\033[216z"}, /* backward window */
{  WINDOWBACK,    "\033OD"},    /* backward window */
{  WINDOWBACK,    "\033Ot"},    /* backward window */
{  WINDOWBACK,    "\033[217z"}, /* backward window */
{  WINDOWBACK,    "\033[D"},    /* backward window */
{  WINDOWBACK,    "\033Oy"},    /* backward window */
{  NEXTFILE,      "j"},         /* Next file. **crh***/
{  NEXTFILE,      " "},         /* Next file. */
{  NEXTFILE,      "\r"},        /* Next file. */
{  NEXTFILE,      "\n"},        /* Next file. */
{  NEXTFILE,      "\016"},      /* ctrl-N Next file. */
{  NEXTFILE,      "\033OB"},    /* Next file. */
{  NEXTFILE,      "\033Or"},    /* Next file. */
{  NEXTFILE,      "\033[221z"}, /* Next file. */
{  NEXTFILE,      "\033[B"},    /* Next file. */
{  PREVFILE,      "k"},         /* Previous file. **crh***/
{  PREVFILE,      "^"},         /* Previous file.*/
{  PREVFILE,      "\010"},      /* ctrl-h Previous file.*/
{  PREVFILE,      "\177"},      /* DELETE Previous file.*/
{  PREVFILE,      "\020"},      /* ctrl-p Previous file.*/
{  PREVFILE,      "\033Ox"},    /* Previous file.*/
{  PREVFILE,      "\033OA"},    /* Previous file.*/
{  PREVFILE,      "\033[A"},    /* Previous file.*/
{  PREVFILE,      "\033[215z"}, /* Previous file.*/
{  SETPATHSTATS,  "O"},         /* Toggle stats and pathname display*/
{  SETCOLORS,     "o"},         /* Toggle Color display */
{  GOTOFILE,      "G"},         /* Goto file number as in vi */
{  SETTAG,        "\011"},      /* TAB key tags or untags a file */
{  SETDELETETAG,  "d"},         /* delete file */
{  SETDELETETAG,  "\033On"},    /* delete file */
{  SETDELETETAG,  "\033[3~"},   /* delete file */
{  SETDELETEALL,  "K"},         /* delete all files, Kill */
{  SETCHANGETAG,  "T"},         /* Tag change on files */
{  TAGSQUIGGLE,   "~"},         /* delete editor backups of form "file~" */
{  TAGPOUND,      "#"},         /* delete editor backups of form "#file" */
{  TAGDELSEARCH,  "D"},         /* Tag 'D' on reg expr search */
{  UNTAGALL,      "U"},         /* untag all  */
{  UNDELETEFILE,  "u"},         /* undelete file */
{  GOTOTOP,       "["},         /* Goto top of file list */
{  GOTOTOP,       "\033<"},     /* Goto top of file list */
{  GOTOTOP,       "("},         /* Goto top of file list */
{  GOTOTOP,       "{"},         /* Goto top of file list */
{  GOTOTOP,       "<"},         /* Goto top of file list */
{  GOTOBOTTOM,    "]"},         /* Goto end of file list */
{  GOTOBOTTOM,    ")"},         /* Goto end of file list */
{  GOTOBOTTOM,    "}"},         /* Goto end of file list */
{  GOTOBOTTOM,    ">"},         /* Goto end of file list */
{  GOTOBOTTOM,    "\033>"},     /* Goto end of file list */
{  JUMPTOMARK,    "J"},         /* jump to top mark, pop mark stack */
{  JUMPTOVIEW,    "L"},         /* jump to top view, pop view stack */
{  SETBOOKMARKS,  "B"},         /* save bookmark */
{  WRITELIST,     "w"},         /* Write file list to /tmp */
{  CHMODFILE,     "C"},         /* Chmod on a single file */
{  SHOWHELP,      "?"},         /* Help, display "man dired" */
{  SHOWTUTOR,     "h"},         /* Command key tutor */
{  SHOWTUTOR,     "H"},         /* Command key tutor */
{  GOTODIR,       "V"},         /* go to a requested dir */
{  GOTODIR,       "\033/"},     /* go to a requested dir */
{  SETERROR,      "\005"},      /* Error exit? */
{  DOUBLEESC,     "\033\033"},  /* Esc Esc */
{  ESCRETURN,     "\033\015"},  /* Esc Return */
{  ESCRETURN,     "\033\012"},  /* Esc Linefeed */
{  COPYFILE,      "c"},         /* Copy current file somewhere */
{  MOVEFILE,      "M"},         /* Move current file somewhere */
};
#define GNUkeysSIZE sizeof(GNUkeys)/sizeof(GNUkeys[0])

#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <pwd.h>        /* passwd structure */
#include <grp.h>        /* group structure */
#include <signal.h>

#if defined(__STDC__) || defined(__cplusplus)
#define STDC    1
#define ARGS(s) s
#else
#define STDC    0
#define ARGS(s) ()
#define const
#endif

#define False           0
#define True            1
#define ESC         '\033'
#define NOTDELETED      ' '
#define DELETED         'D'
#define MARKED          '*'
#define MARKEDonce      '%'

#define eq(a, b)        (strcmp(a, b) == 0)


#if STDC
#define HAVE_VOID_HANDLER       1
#endif

#ifndef SIGTSTP
#define SIGTSTP 18
#endif

#include <time.h>

#if STDC
#include <stdlib.h>
#endif

#if defined(__host_mips) && defined(_SYSTYPE_SVR4)
        /* MIPS RC/6280 RISCos 2.1.1 -- no Standard C compiler yet */
#include <stdlib.h>
#endif

#if SOLARIS
#include <sys/sysmacros.h>              /* for major(), minor() prototypes */
#endif

#if !defined(__NeXT__)
#include <unistd.h>                     /* for read() et al prototypes */
#endif

#ifdef ardent
#include <sys/43ioctl.h>                /* for TIOxxxxx */
#include <sys/sysmacros.h>              /* for major(), minor() prototypes */
#else
#include <fcntl.h>
#endif

#if defined(__linux__)
#include <sys/sysmacros.h>              /* for major(), minor() prototypes */
#ifndef ANSICOLOUR
#define ANSICOLOUR 1            /* Ansi color filenames like "ls" */
#endif
#endif

#ifdef ANSICOLOUR
#undef ANSICOLOUR
#define ANSICOLOUR 1
#endif

#if defined(HAVE_TERMIOS_H)
#include <termios.h>
static struct termios tty_save; /* XPG3, POSIX.1, FIPS 151-1 interface */
#else /* NOT defined(HAVE_TERMIOS_H) */
#include <sgtty.h>
static struct sgttyb   ioctlb;
#endif /* defined(HAVE_TERMIOS_H) */

#ifdef __sgi
#include <sys/sysmacros.h>              /* for major(), minor() prototypes */
#endif

#if defined(HAVE_MKDEV_H)
#include <sys/mkdev.h>                  /* for major(), minor() prototypes */
#endif

#if !defined(HAVE_GETWD)
#undef getwd
#define getwd(p)        getcwd(p,sizeof(p))
#endif

#include <curses.h>

#if HAVE_TERM_H
#include <term.h>                       /* for tgetent() et al prototypes */
#endif

#include <sys/param.h>
#include <sys/stat.h>
#include <errno.h>

#if HAVE_DIRENT_H
#include <dirent.h>
#define D_NAMLEN(dp)            (int)strlen((dp)->d_name)
#define HAVE_OPENDIR            1
#else
#include <sys/dir.h>
#define dirent direct                   /* we use dirent in rest of code */
#if defined(HAVE_SYSTEM7_DIR_H)
/* I don't think any systems with <sys/dir.h> but no d_namlen entry
exist any more, but if you have one, this should do the trick. */
#define D_NAMLEN(dp)    14              /* really old UNIX System 7 style: */
                                        /* d_name[] that might lack a NUL */
#else
#define D_NAMLEN(dp)    (int)((dp)->d_namlen) /* newer <sys/dir.h> style */
                                        /* contains explicit length field */
#endif
#endif /* HAVE_DIRENT_H */

#define DIRPBLOCK       (BUFSIZ / sizeof(struct dirent))
                                /* number of directory entries per block */

/* Supply prototypes for library functions that are missing on some systems */

#if defined(__sgi)
extern char*    re_comp ARGS((const char *s));
extern int      re_exec ARGS((const char *s));
#endif

#if defined(__alpha)
extern char*    tgetstr ARGS((const char *s, char **p));
#endif

#if defined(__NeXT__)
extern char*    tgetent ARGS((char *s, char *name));
extern int      tgetnum ARGS((const char id[2]));
extern char*    tgetstr ARGS((const char *s, char **p));
extern char*    tgoto ARGS((const char *s, int column, int line));
extern int      tputs ARGS((char *str, int affcnt, int (*putc_)(char)));
#endif

#if defined(__sgi)
extern char*    tgoto ARGS((const char *s, int column, int line));
extern char*    tgetstr ARGS((const char *s, char **p));
#endif

#if defined(sun)
#if !defined(__solaris__)
extern char*    getenv ARGS((const char *name));
extern char*    tgetstr ARGS((const char *s, char **p));
#endif
extern char*    tgoto ARGS((const char *s, int column, int line));
#endif

#if defined(ultrix)
extern char*    getenv ARGS((const char *name));
extern char*    tgetstr ARGS((const char *s, char **p));
extern char*    tgoto ARGS((const char *s, int column, int line));
#endif

#if defined(__linux__)
#if 0
extern int      tputs ARGS((char *str, int affcnt, int (*putc_)(int)));
extern char*    tgoto ARGS((const char *s, int column, int line));
#endif
extern char*    tgetstr ARGS((const char *s, char **p));
#endif

#ifdef CTRL
#undef CTRL
#endif
#define CTRL(c)         (c & 037)

#define MAXFILEPATHLEN  1024            /* longest file path name on any */
                                        /* current UNIX system */

#define MAXLBUFF MAXFILEPATHLEN
#define MAXTCAP MAXFILEPATHLEN          /* Holds a termcap entry */

#define MAXWIDTH 255                    /* widest screen lines we will use */

static char divider[MAXWIDTH+1];        /* divides the windows */
#define DIVCHAR '-'                     /* makes up the divider */
static char newdivider[MAXWIDTH+1];     /* copy with 'o' inserted */
#define NEWDIVCHAR 'o'                  /* makes up part of newdivider */

#ifndef MOREPGM                         /* can override at compile time */
#if defined(__convex__)
#define MOREPGM "/usr/ucb/more"         /* program to page thru a file */
#else
#define MOREPGM "/usr/local/bin/less"   /* program to page thru a file */
#endif /* MOREPGM */
#endif /* defined(__convex__) */

#ifndef MINFILES                        /* See Makefile for over-ride */
#define MINFILES        250             /* Initial trial dir size */
#endif

#define ISDIGIT(C)      ((C) <= '9') && ((C) >= '0')

/* Globals used for every recursive call to diredMain() */

/* Screen display variables. See 'F' command and setScreenFormat(). */
static int      lflg=0;                 /* Std 'ls -l' display in file entry */
static int      nflg=0;                 /* Nothing displayed but filename */
static int      iflg=0;                 /* Inode display in file entry */
static int      bflg=0;                 /* Block size display in file entry */
static int      sflg=0;                 /* Size display override */
static int      mflg=0;                 /* Chmod prot display override */
static int      uflg=0;                 /* File owner display override */
static int      gflg=0;                 /* Group gid display override */
static int      dflg=0;                 /* Date display override */
static int      pflg=0;                 /* Toggle path/filename display */

static int      init=1;                 /* Initial term init flag */
static struct stat      statbuf;
static int      infoflg=0;              /* Erase flag for tellinfo() */
static int      statFlag=1;             /* Show path and stats (toggle) */
static int      splitflg=1;             /* Split screen is the default. */
static int      sortyp = 'n';           /* Key to sort on, name=default */
static int      rflg = 1;               /* Reverse sort flag (-1=ON,1=OFF)*/
static int      flags;                  /* Gets flags */
static long     ttime;                  /* Temp time variable */
static long     year;                   /* Six months ago */
static int      scrlen = 999;           /* Default length of dired index part of screen: */
                /* 999 ==> 2 windows (half size), 0 ==> 1 window (full size) */
static int      Tscrlen;                /*Total length of screen,minus 1 for cmd line*/
static int      scrwid = 79;            /* Width of screen */
/* NB: Do NOT make CM, CE, PC static, because this conflicts with
       declarations in DEC Alpha <curses.h>! */
char            *CM=0,                    /* cursor motion string */
                *CE=0;                    /* clear to end of line */

char            PC=0;

/* Shell command builtins. See processDIREDSET(). */
char            *CP=0;          /* System command for copying files */
char            *MV=0;          /* System command for moving files */
char            *RM=0;          /* System command for recusive remove */
char            *CH=0;          /* System command for chmod */
char            *PR=0;          /* System command for printing a file */
char            *GR=0;          /* System command for groff of a file */

static char     tbuf[MAXTCAP];
static char     tcapbuf[256];
static int      sigint=0;
#if defined(HAVE_VOID_HANDLER)
#define handler_t void
#else
#define handler_t int
#endif

static int cmdLineTscrlen = -1;
#if !defined(MAX_MARKS)
#define MAX_MARKS       512             /* Size reduced GBG */
#endif

#if !defined(MAX_VIEW)
#define MAX_VIEW        2048            /* Size reduced GBG */
#endif

int pause_return=0;        /*** GBG 12/95 pause after system command ***/
static int updateWinsize=0; /* GBG 1996, schedule window size update */
static int      CURSORcolumn=48;        /* In 1981: #define CURSOR 48. */
static char    *setfmt=0;               /* Pointer to screen fmt setup. */
#ifdef ANSICOLOUR
static int      ansiColors=1;           /* 2=Internal, 1=Environment */
#else
static int      ansiColors=0;           /* 0=OFF, no color */
#endif
static char    *LS_COLOUR=0;            /* Pointer to color string */

static int MAXcountFILES = MINFILES;    /* Number of files in FileList */
static int MAXallocFILES = MINFILES;    /* How many to realloc on demand */

/*  End of global variables common to all recursive calls to diredMain() */

/* union{char lname[MAXDNAME]; char *namep;} Changed by GBG 12/95 */
/* 2.02: For long file names there was too much memory use (3mb+) */

/* The following globals are saved on each call to diredMain() */
/* and restored on exit from diredMain(). Any memory leaks are */
/* plugged in diredMain() using calls to free(). */

struct lbuf
{
    unsigned long       lnum;
    unsigned long       lnl;
    long                lsize;
    long                latime;
    long                lctime;
    long                lmtime;
    int                 ltype;
    short               deleted;
    short               lflags;
    uid_t               luid;
    gid_t               lgid;
    char                *namep;
};

static int oldMAXFILEScount = 0;
static struct lbuf *filep = 0;          /* Initialized by initFileList() */

struct lbuf *file(n) int n; {
 return (filep+n);
}

#define FNAME(N)        (file(N)->namep)

struct systemcommands
{
  char *cmd;
  struct systemcommands *next;
};

static struct systemcommands *cmdlist=0;/* Linked list of saved system cmds */

struct aliascommands
{
  char *key;
  char *cmd;
  struct aliascommands *next;
};

static struct aliascommands *aliaslist=0;/* Linked list of user aliases */
static int      ALIASCOUNT=0;           /* How many alias defined */

static int      totfiles = 0;           /* Total files */
static int      numdeleted = 0;         /* Number of files tagged 'D' */

static int      Curfile = 0;            /* Current file */
static int      topfile;                /* File at top of screen */
static int      curline = 0;            /* Line that we're on */

static int      command = 0;            /* Holds last command */

static int arglist=0;
static char fullpath[MAXLBUFF];
static char startfile[MAXLBUFF];
static int quitok = 1;
static int countarg = 0;
static int camedown = 0;
static int numarg = 0;

static int     view_stack[MAX_VIEW];
static int     view_top = 0;

static int saveposition=0; /* GBG 1996, save large position changes only */

/* Variables made global GBG 1996 */
static int      mark_stack[MAX_MARKS];
static int      mark_top = 0;
static int      didStats=1;             /* 0=lstat() pending */
static int      slowstat=1;             /* Do stat with reads */
static int      nummarked=0;            /* Number files flagged asterisk */

int             main ARGS((int argc, char *argv[]));

static void     blank ARGS((void));
static void     bscreen ARGS((int count));
static handler_t catchint ARGS((int sig));
static void     ceol ARGS((int x, int y));
static int      compar ARGS((const void *pp1, const void *pp2));
static void     curxy ARGS((int col, int lin));
static void     dcanon ARGS((char *cp));
static void     deletefiles ARGS((void));
static void     dispfile ARGS((int fnum));
static void     dispwin ARGS((int filenum));
static void     downline ARGS((void));
static void     expandpath ARGS((char pname[]));
static void     findfile ARGS((int down));
static void     regexpMark ARGS((int c));
static void     fscreen ARGS((int count));
static void     Getcap ARGS((void));
static void     Setcap ARGS((void));
static void     UnSetcap ARGS((void));
static void     getcountarg ARGS((void));
static void     getfiles ARGS((int argc, char *argv[]));
static char    *getfullpath ARGS((const char *start));
static int      getline ARGS((char s[],const char *t));
static int      gstat ARGS((const char *gname,int n));
static int      locstart ARGS((char *Startfile));
static int      match ARGS((int filenum));
static long     nblock ARGS((long size));
static handler_t onctlz ARGS((int sig));
static int      outch ARGS((int c));
static void     overflow ARGS((void));
static void     pmode ARGS((int aflag,char *buf));
static void     printpath ARGS((char str[]));
static void     putpad ARGS((char *str));
static void     Readdir ARGS((const char *dir));
static int      rm ARGS((char arg[]));
static int      Select ARGS((int *pairp));
static void     setdpy ARGS((void));
static void     shortcountmess ARGS((void));
static void     showscreen ARGS((void));
static char    *skipto ARGS((const char *string));
static void     telluser ARGS((const char *msg));
static int      Type ARGS((const char *filestr));
static void     typefiles ARGS((void));
static void     unsetdpy ARGS((void));
static int      waitchk ARGS((const char *msg));
static void     windsize ARGS((void));

static void     newcore ARGS((char **d,const char *s));
static void     setShellPause ARGS((void));
static void     searchRegExp ARGS((int n));
static void     doShellCommand ARGS((char *s));
static int      doShell ARGS((char *cmd));
static void     doPager ARGS((int curfile));
static void     doManualPage ARGS((int curfile));
static void     showStats ARGS((int which));
static void     MarkFile ARGS((int mark));
static int      setFileMark ARGS((int mark,int curfile));
static void     deleteBackups ARGS((int x,int mark));
static void     MarkAllFiles ARGS((int x));
static void     setBookMark ARGS((void));
static void     popBookMark ARGS((void));
static void     pushView ARGS((void));
static void     popView ARGS((void));
static void     PrintFile ARGS((void));
static void     getkey ARGS((int flag));
static void     typein ARGS((char *argv));
static void     clearLastLine ARGS((void));
static void     printLastLine ARGS((char *msg));
static void     stuffPushdDirname ARGS((void));
static void     suspendDired ARGS((int SIG));
static void     initFileList ARGS((void));
static void     FreeFileList ARGS((void));
static int      buildcharstring ARGS((int c,char *buf,int *N));
static char    *endof ARGS((char *s));
static void     restoreCursor ARGS((void));
static void     redraw ARGS((void));
static void     setScreenFormat ARGS((char *setfmt));
static void     doSort ARGS((void));
static void     getWinSize ARGS((void));
static void     ceod ARGS((int x,int y));
static void     help ARGS((void));
static int      processArgs ARGS((int argc,char **argv));
static void     processDIREDSET ARGS((void));
static void     getDIREDSETenv ARGS((void));
static void     getdiredset ARGS((void));
static handler_t win_change ARGS((int sig));
static int      getStats ARGS((int whichone));
static void     getCurfileName ARGS((char *dest,int curfile));
static void     tellinfo ARGS((const char *msg));
static void     setFileDefaults ARGS((int totalfiles));
static void     lowScreenMessage ARGS((const char *str));
static void     clearLowerScreen ARGS((void));
static int      topofLowerScreen ARGS((void));
static void     getEntry ARGS((int whichone,char *lastone,int *len));
static void     ONsignals ARGS((void));
static void     OFFsignals ARGS((int SIG));
static void     printEntry ARGS((int curfile,int row));
static void     Quit ARGS((int exitcode));
static void     changeTags ARGS((void));
static void     writeFileList ARGS((void));
static void     diredKeyTutor ARGS((void));
static int      dired ARGS((char *buf));
static int      diredMain ARGS((int argc, char **argv));
static int      DiredLoop ARGS((int argc, char **argv));
static void     getRETURN ARGS((void));
static void     setTopfile ARGS((int fnum));
static char    *colorString ARGS((char *name,int mode));
static void     lsColoursInit ARGS((char **cp));
static void     ansiBOLD ARGS((void));
static void     ansiUNBOLD ARGS((void));
static char    *Getuser ARGS((uid_t uid));
static char    *Getgroup ARGS((gid_t gid));
static int      getLink ARGS((char *dest));
static void     getChmodString ARGS((char *s));
static void     doChmod ARGS((void));
static void     logger ARGS((int n));
static int      dotname ARGS((char *s));
static int      getkeymatch ARGS((int cc,int MAXKEYS));
static int      ustrncmp ARGS((char *s, char *t, int n));
static int      ustrcmp ARGS((char *s,char *t));
static void     sorttable ARGS((void));
static void     keytostring ARGS((int x,char *buf));
static void     decodekey ARGS((int keycode,char *buf));
static void     decodeall ARGS((int keycode,char *buf));
static void     storeCmd ARGS((char *buf));
static void     doCopyMove ARGS((int n));
static void     setAlias ARGS((char *s));
static int      aliasGet ARGS((char *s,int c));
static void     saveAlias ARGS((int c,char *s));
static void     aliasHelp ARGS((void));
static int      testquit ARGS((void));
static int      buildcommand ARGS((char *buf,char *bldbuf,int i));

static void
help(){
static const char *msg[] = {
"Usage: dired [options] path path ...",
"A path is either a directory name or a filename. The options:",
"",
"-v             Display version, help and quit. Also -h and --h.",
"-O             Toggle OFF pathname and stat display. [Default ON]",
#ifdef ANSICOLOUR
"-o             Display of colors OFF [Default ON].",
"-o3            Display mono color set [o1,o2,o3 possible].",
#else
"-o             Nothing. No ANSI colors in this version.",
#endif
"-p             Turn ON pause before and after system commands.",
"-wh            Set window size to half-screen. [Default]",
"-wf            Set window size to full screen.",
"-w20           Set window size to 20 lines.",
"-s[nesrwciu]   Sort normal by Name, Extension, Size, Read date,",
"               Write date, Creation date or Inode. [Default -sn]",
"-r[nesrwciu]   Sort reverse instead of normal sort.",
"-s             Unsorted disk order (-ri is similar).",
"-S             Stat() on demand for 12x response. [Default OFF]",
"-C250          Set minimum file capacity to 250 files. [Default 100]",
"-F[ibmguspln]  Format of screen: inode, block, modes, gid, uid, ",
"               size, path, long, none. [Default -Fl]",
"-fNAME         Find file with NAME after loading the directory.",
"-D'x=s'        Define shell command with text s and single key x.",
"-D'RM=s'       Define remove command RM with text s [RM=rm -rf].",
"               Complete list is CP, RM, MV, CH, GR, PR.",
"~/.diredset    Options file in home directory, one option per line.",
"DIREDSET       Environment variable used before command line.",
"               Example: setenv DIREDSET '-ss -Fsdp -wf'"
};
int i;
    for(i=0;i<(int)(sizeof(msg)/sizeof(msg[0]));++i)
      printf("%s\n",msg[i]);
}

/* Beginning of main() */
int
main(argc, argv)
int     argc;
char   *argv[];
{
    char        *cp;
    int         i;
    char        tmpbuf[MAXLBUFF];       /* Random temporary buffer */
    int errcode;

    Getcap();                   /* Get terminal type */
    Setcap();                   /* Set application pad */
    re_comp((char *)"/");       /* unmatchable initial file pattern */
    setfmt=(char *)0;           /* newcore() needs NULL address initially */
    newcore(&setfmt, "l");      /* Default screen format is l=long */
    storeCmd("");               /* Initialize stored command list */

    processDIREDSET();          /* Process env variable flags. */
    i=processArgs(argc,argv);   /* Process arg flags. They must be first! */
    lsColoursInit(&cp);         /* Initialize color map for files */
    if(i == -1) Quit(-1);       /* Command line errors cause an exit. */
    argv += (i - 1);            /* Skip over cmd line args */
    argc -= (i - 1);

    windsize();                 /* 8/82 crh. Set scrlen, divider. */
    setScreenFormat(setfmt);    /* Select screen format flags */

    time(&ttime);
    year = ttime - 6L * 30L * 24L * 60L * 60L;/* 6 months ago */
    sorttable();                /* Sort the keys */

    /* diredMain() can call itself recursively, via routine dired() */
    /* The original 1980 dired did system("dired arg1 arg2 ...") to */
    /* do the work now down by recursion. */

    errcode=diredMain(argc, argv);

    Quit(errcode);

} /* End of main() */

#define QuitABORT       3
#define QuitERROR       2
#define QuitNORMAL      1
static int
diredMain(argc, argv)
int     argc;
char   *argv[];
{
/* Begin save variables ======================================== */
int _oldMAXFILEScount=oldMAXFILEScount;
struct lbuf *_filep;
int _totfiles=totfiles;
int _numdeleted=numdeleted;
int _Curfile=Curfile;
int _topfile=topfile;
int _curline=curline;
int _command=command;
int _arglist=arglist;
int _quitok=quitok;
int _countarg=countarg;
int _camedown=camedown;
int _numarg=numarg;
int _view_top=view_top;
int _saveposition=saveposition;
int _mark_top=mark_top;
int _didStats=didStats;
int _slowstat=slowstat;
int _nummarked=nummarked;
int _init=init;
char _fullpath[MAXLBUFF];
char _startfile[MAXLBUFF];
int  _view_stack[MAX_VIEW];
int  _mark_stack[MAX_MARKS];
/* End save variables ======================================== */

int  errcode;

  /* Save variables in arrays */
  _filep=filep;
  memcpy(_fullpath, fullpath, sizeof(char)*MAXLBUFF);
  memcpy(_startfile, startfile, sizeof(char)*MAXLBUFF);
  memcpy(_view_stack, view_stack, sizeof(int)*MAX_VIEW);
  memcpy(_mark_stack, mark_stack, sizeof(int)*MAX_MARKS);
  /* End save variables in arrays */

    errcode=DiredLoop(argc,argv);

  /* Restore variables */
  memcpy(fullpath, _fullpath, sizeof(char)* MAXLBUFF);
  memcpy(startfile, _startfile, sizeof(char)*MAXLBUFF);
  memcpy(view_stack, _view_stack, sizeof(int)*MAX_VIEW);
  memcpy(mark_stack, _mark_stack, sizeof(int)*MAX_MARKS);
  filep = _filep;
  oldMAXFILEScount= _oldMAXFILEScount;
  totfiles= _totfiles;
  numdeleted= _numdeleted;
  Curfile= _Curfile;
  topfile= _topfile;
  curline= _curline;
  command= _command;
  arglist= _arglist;
  quitok= _quitok;
  countarg= _countarg;
  camedown= _camedown;
  numarg= _numarg;
  view_top= _view_top;
  saveposition= _saveposition;
  mark_top= _mark_top;
  didStats= _didStats;
  slowstat= _slowstat;
  nummarked= _nummarked;
  init= _init;
  /* End restore variables */

  return errcode;
} /* End of diredMain() */


/* This is the main loop for dired, where functions are called based */
/* upon keystroke abbreviations of commands. */
static int
DiredLoop(argc,argv)
int argc;
char **argv;
{
    char        *cp;
    int         i;
    char        tmpbuf[MAXLBUFF];
    int         errcode;

    numarg = argc;
    startfile[0]=0;             /* Startfile assumed to be file 1 */

reread:
    oldMAXFILEScount = 0;
    filep = 0;
    arglist=0;
    errcode= mark_top = view_top = nummarked=numdeleted=totfiles = 0;
    /* Initialize filename pointers */
    initFileList();
    ceol(0,Tscrlen);
    printf("Reading");
    getfiles(argc,argv);
    if(init){
      ONsignals();
      setdpy();
    }
    sigint=0;
    /* extract the full path name */
    if(!arglist) {     /* Zero or one dir names on cmd line */
      if(argc==1) cp=getfullpath(".");
      else if(argv[1][0] != '/')  cp=getfullpath(argv[1]);
      else cp=argv[1];
      dcanon(cp);
      strcpy(fullpath,cp);
    } else {
      strcpy(fullpath,getfullpath("."));
    }

    didStats=slowstat;                  /* Mark lstat() pending */
    doSort();
    i=locstart(startfile);
    dispwin(i);

startup:
    showStats(0);
    countarg = 0;
    while(True){        /* GBG: Function getkey() gets command. */
#ifdef _AIX             /* Old IBM AIX had some refresh troubles. */
       showscreen();    /* May go away with the 1996 code. Untested. */
#endif /* _AIX */

        if(init){
          init=0;
          command = -1; /* Print help message first time */
        } else
        getkey(1);      /* Variable 'command' is the char returned. */
AfterGetkey:
        if(sigint) command = QUITDELETE;        /* Two ctrl-C's */
        sigint=0;
        if(updateWinsize) command = REFRESHSCR; /* Schedule update */
        if(infoflg){ tellinfo(""); infoflg=0;}  /* Erase old message */
        switch(command)
        {
            case ABORTPUSHD:            /* Abort direds, pushd dir */
                unsetdpy();             /* Must be before stuffer */
                stuffPushdDirname();
                errcode=QuitABORT; break;

            case QUITDELETE:            /* Check 'D' tags, exit. */
                deletefiles();
                if(numdeleted) goto startup;
                errcode=QuitNORMAL; break;  /* Exit, return errorcode */

            case SUSPENDPUSHD:          /* Suspend dired and pushd */
                unsetdpy();             /* Before the stuffer */
                stuffPushdDirname();
                suspendDired(0);   /* 0=SIG_DFL, 1=SIG_IGN */
                break;

            case SUSPENDONLY:           /* Suspend dired, simulated */
                unsetdpy();             /* See onctlz() for usual one */
                suspendDired(0);   /* 0=SIG_DFL, 1=SIG_IGN */
                break;

            case ABORTLEVEL:            /* Abort completely */
                errcode=QuitERROR; break;

            case ABORTALL:         /* Abort the chain of direds  ***crh */
                errcode=QuitABORT; break;

            case PRINTFILE:        /* Print a file */
                PrintFile();
                break;

            case PATHSTATS:        /* Display full pathname of the file and system stats */
                showStats(1);
                infoflg=0;
                goto justcount;

            case SEARCHFORWARD:   /* Forward */
                searchRegExp(1);
                break;

            case SEARCHREVERSE:  /* Reverse */
                searchRegExp(0);
                break;

            case TAGSEARCH:
                regexpMark('*');
                break;

            case SEARCHNEXT:
                findfile(1);
                break;

            case SEARCHPREV:
                findfile(0);
                break;

            case SHELLPAUSE: /* GBG 12/95. Toggle pause after shell command */
                setShellPause();
                goto justcount;

            case SYSTEMCMD:           /* Execute a system command */
                (void)doShellCommand((char *)0);
                break;

            case REPEATCMD:           /* repeat the previous ! command */
                (void)doShellCommand(cmdlist->cmd);
                break;

            case REREADDIR:           /* re-read the directory */
                deletefiles();  /* delete tagged files */
                if(numdeleted) goto startup;  /* some not deleted */
                strcpy(startfile,FNAME(Curfile));
                clearLastLine();
                FreeFileList(); /* Plug up memory leak */
                goto reread;

            case SORTREVERSE:           /* Reverse sort */
            case SORTFORWARD:           /* Normal sort */
                rflg = (command == SORTFORWARD) ? 1 : -1;
                tellinfo("Name, extension, size, read, write or creation date, inode, user.");
                sprintf(tmpbuf,"Sort %s. Enter the sort key [n,e,s,r,w,c,i,u]: ",(rflg == 1 ? "normal" : "reverse"));
                printLastLine(tmpbuf);
                command = getchar();
                if((char *)strchr("nesrwciu",command) != (char *)0)
                    sortyp = command;
                else { /* abort */
                    tellinfo("Sort aborted");
                    break;
                }
                printf("%c", command);
                doSort();
                topfile = Curfile = curline = 0;
                sprintf(tmpbuf, "%s Sort completed",
                        (command==SORTFORWARD ? "Forward" : "Reverse"));
                redraw();
                tellinfo(tmpbuf);
                break;

            case EDITFILEDIR:           /* Edit a file or directory */
            case UPDIR:                 /* go up a dir */
                errcode=dired(tmpbuf);
                break;

            case MORELESS:           /* 'more' a file */
                doPager(Curfile);
                break;

            case GROFFMANUAL:           /* 'groff' a file as a manual page, GBG 12/1995 */
                doManualPage(Curfile);
                break;

            case TYPEFILE:   /* quickly type the file -- added 5/81, J.Lepreau */
                if(file(Curfile)->ltype == 'd')
                {
                    tellinfo("?This is a directory, 't' works on files only");
                    break;
                }
                getCurfileName(tmpbuf,Curfile);
                i=Type(tmpbuf);
                if(i==1) redraw();
                if(i==0 && splitflg)
                   tellinfo("To clear the lower screen, press l");
                break;

            case REFRESHSCR:           /* Refresh screen */
                getWinSize();   /* May be in X, new window size */
                windsize();     /* New divider */
                redraw();
                updateWinsize=0;/* Turn off the schedule */

                (void)sprintf(tmpbuf,
                "Reset window size: Cols=%3d  Rows=%3d  FileRows=%3d",
                (int)scrwid, (int)Tscrlen, (int)scrlen );
                tellinfo(tmpbuf);
                break;

            case SETFORMAT:           /* Format setup for screen */
                setScreenFormat((char *)0);
                redraw();
                break;

            case REFRESHLINE:           /* Refresh line, clear lower screen */
                printEntry(Curfile,curline);
                clearLowerScreen();
                break;

            case WINDOWFORWARD:           /* forward window */
                  saveposition=1;
                fscreen(countarg);
                break;

            case WINDOWBACK:           /* backward window */
                  saveposition=1;
                bscreen(countarg);
                break;

            case NEXTFILE:   /* Next file. **crh***/
                if(Curfile == totfiles - 1){
                    tellinfo("?At end of files");
                    goto justcount;
                } else {
                   if(countarg < 1) countarg = 1;
                   Curfile = Curfile + countarg;
                   if(Curfile > totfiles - 1) {
                      Curfile = totfiles - 1;
                      dispfile(Curfile);
                      shortcountmess();
                      break;
                   }
                   dispfile(Curfile);
                }
                break;

            case PREVFILE:           /* Previous file. ****crh****/
                if(Curfile == 0){
                    tellinfo("?At start of files");
                    goto justcount;
                } else {
                    if(countarg < 1) countarg = 1;
                    Curfile = Curfile - countarg;
                    if(Curfile < 0) {
                       Curfile=0;
                       dispfile(Curfile);
                       shortcountmess();
                       break;
                    }
                    dispfile(Curfile);
                }
                break;

            case SETSLOWSTAT:           /* Toggle slowstat flag */
                slowstat=1-slowstat;    /* Toggle slow stat() on read */
                sprintf(tmpbuf,"Slow stat() on read: %s",
                        (slowstat ? "ON" : "OFF"));
                tellinfo(tmpbuf);
                break;

            case SETPATHSTATS:          /* Toggle stats and pathname display*/
                statFlag=1-statFlag;    /* Toggle the display of stats */
                sprintf(tmpbuf,"Pathname and stats (toggle): %s",
                        (statFlag ? "ON" : "OFF"));
                tellinfo(tmpbuf);
                break;

#ifdef ANSICOLOUR
            case SETCOLORS:                   /* Toggle Color display */
                if(++ansiColors == 4) ansiColors = 0;
                lsColoursInit(&cp);
                redraw();
                tellinfo(cp);
                break;
#endif

            /* line number addressing as in vi */
            case GOTOFILE:
                if(countarg < 1) dispfile(totfiles-1);
                  else
                if(countarg > totfiles) {
                  dispfile(totfiles - 1);
                  shortcountmess();
                }
                else dispfile(countarg-1);
                break;

            case SETTAG:     /* TAB key tags or untags a file */
                MarkFile('*');
                break;

            case SETDELETETAG:           /* delete file */
                MarkFile('D');
                break;

            case SETDELETEALL:           /* delete all files, Kill */
                MarkAllFiles('D');
                break;

            case SETCHANGETAG:           /* Tag change on files */
                changeTags();
                break;

            case TAGSQUIGGLE:           /* delete editor backups of form "file~" */
                deleteBackups(1,'~');
                break;

            case TAGPOUND:           /* delete editor backups of form "#file" */
                deleteBackups(2,'#');
                break;

            case TAGDELSEARCH:       /* Tag 'D' on reg expr search */
                regexpMark('D');
                break;

            case UNTAGALL:           /* untag all  */
                MarkAllFiles(' ');
                break;

            case UNDELETEFILE:           /* undelete file */
                MarkFile(' ');
                break;

            case GOTOTOP:           /* Goto top of file list */
                  saveposition=1;
                if(countarg <= 0)
                    dispfile(0);
                else
                {
                    if(countarg > 10)
                        countarg = 10;
                    dispfile((int)(((long)countarg * (long)totfiles) / 10L));
                }
                break;

            case GOTOBOTTOM:           /* Goto end of file list */
                  saveposition=1;
                if(countarg <= 0)
                    dispfile(totfiles-1);
                else
                {
                    if(countarg > 10)
                        countarg = 10;
                    countarg = 10 - countarg;
                    dispfile((int)(((long)countarg * (long)totfiles) / 10L));
                }
                break;

            case JUMPTOMARK:                   /* jump to top mark, pop mark stack */
                popBookMark();
                break;

            case JUMPTOVIEW:                   /* jump to top view, pop view stack */
                popView();
                break;

            case SETBOOKMARKS:                /* save mark */
                setBookMark();
                break;

            case WRITELIST:                   /* Write file list to /tmp */
                writeFileList();
                break;

            case CHMODFILE:                   /* Chmod */
                doChmod();
                break;

            case SHOWHELP:           /* Help */
                (void)doShell((char *)"man dired");
                redraw();
                showStats(0);
                command = SETERROR;
                goto AfterGetkey;

            case SHOWTUTOR:                   /* Command key tutor */
                diredKeyTutor();
                showStats(0);
                infoflg=1;
                goto AfterGetkey;

            case GOTODIR:                   /* go to a requested dir */
                errcode=dired(tmpbuf);
                break;

            case COPYFILE:                  /* Copy current file somewhere */
                doCopyMove(0);
                break;

            case MOVEFILE:                  /* Move current file somewhere */
                doCopyMove(1);
                break;

            case SETERROR:
            case -1:                    /* ESC == message about help */
            default:
                tellinfo("Help available - h or ?. Use q to quit.");
                goto justcount;
        }
        if(errcode) break;
        showStats(0);
justcount:
        countarg = 0;
    }

/* Free up any memory used for file list */
   FreeFileList();

    return errcode;
} /* End of DiredLoop() */

/*** new routines crh 8/82 ***/


static handler_t
onctlz(sig)
int sig;
{
  unsetdpy();
  suspendDired(1);   /* 0=SIG_DFL, 1=SIG_IGN */
#if !defined(HAVE_VOID_HANDLER)
  return(0);
#endif
}

# if defined(SIGWINCH)
static handler_t
win_change(sig)
int sig;
{
    signal(SIGWINCH, SIG_IGN);
    updateWinsize=1;        /* Schedule an update */
    signal(SIGWINCH, win_change);
#if !defined(HAVE_VOID_HANDLER)
  return(0);
#endif
}
#endif

static int
match(filenum)
int filenum;
{
    return( re_exec(FNAME(filenum)) );
}

/* move to the indicated file number */
static void
dispfile(fnum) int fnum; {
  if(saveposition) pushView();  /* remember the last view shown */
  /* redraw if file not already on screen */
  if((fnum< topfile) || (fnum >=topfile+scrlen)) {
    setTopfile(fnum); redraw();
  }
  Curfile=fnum;
  curline= fnum - topfile;
}

static void
findfile(down)
int down;
{
int i,status;
  status=0;
  if(down) {
    for(i=Curfile+1; i < totfiles; i++) { /***crh 1/84 ch from <=*/
      status = match(i);
      if(status != 0) break;
    }
  }
  else {
    for(i=Curfile-1; i>= 0 ; i--) {
      status = match(i);
      if(status != 0) break;
    }
  }
  if(status == -1) {
     tellinfo("?Botched regular expression");
     return;
  }
  if(status) dispfile(i);
  else tellinfo((down ? "?No match forward" : "?No match backward"));
}

#if defined(HAVE_TERMIOS_H)
#define KILL_CHAR       tty_save.c_cc[VKILL]
#define ERASE_CHAR      tty_save.c_cc[VERASE]
#else /* NOT defined(HAVE_TERMIOS_H) */
#define KILL_CHAR       ioctlb.sg_kill
#define ERASE_CHAR      ioctlb.sg_erase
#endif /* defined(HAVE_TERMIOS_H) */

static void
windsize()
{
int i;
    if(splitflg && cmdLineTscrlen >= Tscrlen-3) scrlen = Tscrlen-3;
      else
    scrlen = splitflg ? /* Split screen or full screen? */
             ((cmdLineTscrlen != -1) ? cmdLineTscrlen : 999) : 0;
    if(scrlen == 0)                    /* full screen */
        scrlen = Tscrlen-2;
    else if(scrlen == 999)             /* means split in half */
        scrlen = (Tscrlen - 1)/2;
    if(scrlen < 2)                     /* Don't use just one line */
        scrlen = 2;
    if(Tscrlen-2 < scrlen)
        scrlen = Tscrlen-2;
    if(splitflg && scrlen+2 >= Tscrlen)  scrlen = (Tscrlen-1)/2;

    for(i=0;i<scrwid;++i) divider[i]= DIVCHAR;
    divider[scrwid]=0;
}

static int
locstart(Startfile)
char *Startfile;
{
int n,i;
    for(n=i=0; i<totfiles; i++) {
      if(strcmp(FNAME(i),Startfile) == 0){
        n = i;
        break;
      }
    }
    return (n);
}

/*
 * dcanon - canonicalize the pathname, removing excess ./ and ../ etc.  */
static void
dcanon(cp)
        char *cp;
{
        register char *p, *sp;
        register int slash;

        /* assume name begins with a '/' */

        for(p = cp; *p; ) {            /* for each component */
                sp = p;                 /* save slash address */
                while(*++p == '/')      /* flush extra slashes */
                        ;
                if(p != ++sp)
                        strcpy(sp, p);
                p = sp;                 /* save start of component */
                slash = 0;
                while(*++p)             /* find next slash or end of path */
                        if(*p == '/') {
                                slash = 1;
                                *p = 0;
                                break;
                        }
                if(*sp == '\0')        /* if component is null */
                        if(--sp == cp) /* if path is one char (i.e. /) */
                                break;
                        else
                                *sp = '\0';
                else if(eq(".", sp)) {
                        if(slash) {
                                strcpy(sp, ++p);
                                p = --sp;
                        } else if (--sp != cp)
                                *sp = '\0';
                } else if(eq("..", sp)) {
                        if(--sp != cp)
                                while(*--sp != '/')
                                        ;
                        if(slash) {
                                strcpy(++sp, ++p);
                                p = --sp;
                        } else if(cp == sp)
                                *++sp = '\0';
                        else
                                *sp = '\0';
                } else if(slash)
                        *p = '/';
        }
}


static char     wd[MAXLBUFF];

static char *
getfullpath(start)
const char *start;
{
/*  return the full path name in this buffer, must be static. */
static char temp[MAXLBUFF];

  /* Working dir fix by GBG for local unix systems */
  if(getenv("PWD") != (char *)0)
    strcpy(wd,getenv("PWD"));
  else if(getenv("cwd") != (char *)0)
    strcpy(wd,getenv("cwd"));
  else if(!getwd(wd)) {
    telluser("Failed to find the working directory.");
    getline(temp,"Press Return: ");
    strcpy(wd,".");
    return wd;
  }
  if(eq(start,".")) return (wd);
  else {
    sprintf(temp,"%s/%s",wd,start);
    return(temp);
  }
}

static int
topofLowerScreen(){
int cur_scrl;
    if(splitflg){
      cur_scrl = totfiles - topfile + 1;        /* topfile starts at 0 */
      return( (cur_scrl <= scrlen) ? cur_scrl : scrlen+1);
    } else
    return(Tscrlen-2);
}

static void
clearLowerScreen(){
int i,n;
    n = Tscrlen - (splitflg ? 1 : 2);
    for(i=topofLowerScreen()+1;i<n;++i) ceol(0,i);
}


static void
lowScreenMessage(str)
const char *str;
{
int i;
char buf[128];
      /*** 1/84 crh - show embedded control chars - assume ASCII ***/
      ceol(0,topofLowerScreen());
      i=0;
      ansiBOLD();
      while(str[0]){
       buildcharstring(*str++,buf,&i);
       if(i >= scrwid){printf("\n"); i=0;}
       printf("%s",buf);
      }
      ansiUNBOLD();
}

static void
printpath(str) char *str; {
char buf[512]; char *p;
    if(!splitflg) {
      strcpy(p=buf,str); strcpy(&buf[scrwid-2],"!");
    } else p=str;
    lowScreenMessage(p);
}

static void
getCurfileName(dest,curfile)
char *dest; int curfile; {
    strcpy(dest,FNAME(curfile));
    expandpath(dest);
    dcanon(dest);
}

static void
expandpath(pname)
char pname[];
{
char temp[MAXLBUFF];
  if(arglist != 0 && pname[0] == '/') return;
  sprintf(temp,"%s/%s",fullpath,pname);
  strcpy(pname,temp);
}


/* end 8/82 routines */

static void
getfiles(argc,argv)
int argc;
char *argv[];
{
struct stat stbuf;
char buf[MAXLBUFF];

   if(argc == 1){
        Readdir(getfullpath("."));
   } else {
        arglist=1;
        if(argc == 2) {
            strcpy(buf,argv[1]);
            if(lstat(buf,&stbuf)==-1) {
              strcat(buf," is unreadable.");
              telluser(buf);
              sleep(2);
              Quit(1);
            }
            if((stbuf.st_mode & S_IFMT) == S_IFLNK) {
               getLink(buf);
               Readdir(buf);
               arglist=0;
            }
            if((stbuf.st_mode & S_IFMT) == S_IFDIR) {
               Readdir(buf);
               arglist=0;
            }
        }
        if(arglist)
        {
            while(--argc > 0)
            {
                if(totfiles == MAXcountFILES){
                    overflow();
                    initFileList();
                }
                ++argv;
                setFileDefaults(totfiles);
                newcore(&FNAME(totfiles),*argv);
                if(slowstat){ /* Do stats when reading files */
                  if(gstat(FNAME(totfiles),totfiles)) continue;
                }
                totfiles++;
                if((totfiles/20)*20 == totfiles){
                  printf("."); fflush(stdout);
                }
            }
        }
    }
    if(totfiles == 0) {
        telluser("?Empty directory\007");
        sleep(1);               /* So user can see it */
        Quit(0);
    }
}

static void
deletefiles()
{
int i;
int all;
int failures;
int failed;
char nambuf[MAXLBUFF];

    if(numdeleted==0) return;
    blank();
    typefiles();
    printf("\nThe number of files tagged for deletion: %d\n",numdeleted);
    all=0;
    printf("\nDelete all? [Y=yes, N=no]: ");
    command = getchar();
    if(command == 'Y' || command == 'y') all =1;
    else {
      printf("\nReturn to DIRED? [Y=yes, N=no]: ");
      command = getchar();
      if(command == 'Y' || command == 'y'){
        redraw();
        return;
      }
    }
    failures=0;
    printf("\n");
    for(i = 0; i < totfiles; i++){
      if(file(i)->deleted != DELETED) continue;
      getCurfileName(nambuf,i);
      if(all==0){
loop:   if(dotname(nambuf)) continue;   /* Don't remove "." or ".." */
        printf("%s  Delete %s?\r%s",
               "                           ",
               nambuf,
               "[y=delete, A=all, q=quit]:");
        command=getchar();
        if((char *)strchr("AyqQnN \r\n\t",command)==(char *)0){
          printf("\nKeys: space, return, n, y, A (not a), q\n");
          goto loop;
        }
        if((char *)strchr("nN \r\n\t",command)!=(char *)0)
        printf("\r%s"," --Not Deleted--           ");
        printf("\n");
        if(command=='A') all=1;
        if(command== 'q' || command== 'Q') goto exitloop;
        if(command != 'y') continue;
      }/* End if(all==0) */
      if(file(i)->ltype == 'd') failed=rm(nambuf);
        else
      failed=(unlink(nambuf) < 0);
      if(failed) {
        printf("Delete of %s failed.\n", nambuf);
        failures++;
      } else setFileMark((int)'#', i);
    } /* End for(i = 0; i < totfiles; i++) */
exitloop:
    if(failures) getRETURN();
    if(!all) redraw();
}

static void
typefiles()
{
int     longsiz,
        i,
        j,
        maxperln,
        numout,
        longthis;
register char *cp;
char buf[256];

    for(longsiz = i = 0; i < totfiles; i++){
      if(file(i)->deleted != DELETED) continue;
      j=strlen(FNAME(i));
      if(j > longsiz) longsiz=j;
    }

    maxperln = scrwid / (longsiz + 3);
    if(maxperln < 1) maxperln = 1; /* 3/84 */

    for(numout = i = 0; i < totfiles; i++){
      if(file(i)->deleted != DELETED) continue;
      cp = FNAME(i);
      longthis = 0;
      while(cp[0]){
        buildcharstring(*cp++,buf,&longthis);
        printf("%s",buf);
      }
      numout++;
      if((numout % maxperln) == 0) printf("\n");
        else
      if(numout != numdeleted){
        for(j = 0; j < (longsiz + 3 - longthis); j++) printf(" ");
      }
    } /* End for() */
}

/*** shortened for 4.2 by using -rf option of rm to do the work ***/
static int
rm(arg)
char    arg[];
{
char buf[MAXLBUFF];
    sprintf(buf,"%s %s",RM,arg);
    if(system(buf)>>8) {
      printf("?Couldn't delete %s",arg);
      return 1;
    }
    return 0;
}

static void
shortcountmess()
{
  tellinfo("?Used shortened count");
}

static void
dispwin(filenum)
/* forces a new window */
int filenum;
{
  topfile = -999;  /* force a redraw */
  dispfile(filenum);
}

static void
fscreen(count)
int count;
{
  if(topfile + scrlen - 1 >= totfiles - 1) {
      tellinfo("?No remaining windows");
      return;
  }
  if(count <= 1) count=1;
  else if((topfile + count*(scrlen-1)) > totfiles - 1)
  {
     dispwin(totfiles - 1);
     shortcountmess();
     return;
  }
 dispwin(topfile + count*(scrlen-1));
}

static void
bscreen(count)
int count;
{
  if(topfile - scrlen + 1 < 0) {
      tellinfo("?No previous windows");
      return;
  }
  if(count <= 1) count=1;
  else if((topfile -count*(scrlen-1)) < 0) {
    dispwin(0);
    shortcountmess();
    return;
  }
  dispwin(topfile - count*(scrlen-1));
}

static void
showscreen(){
int     i,row,
        x1,x2,
        numprint;
char    buf[MAXLBUFF+128];
    numprint = 0;
    for(row=0, i = topfile; (numprint < scrlen) && (i < totfiles); i++){
      printEntry(i,row++);
      numprint++;
    }
    if(splitflg) {
      strcpy(newdivider,divider);       /* strlen == scrwid */
      x1 = (topfile * scrwid) / totfiles;
      x2 = ((topfile+scrlen-1)*scrwid)/totfiles;
      while(x1 <= x2 && x1 < scrwid) newdivider[x1++] = NEWDIVCHAR;
      curxy(0, row);                    /* position for divider */
      printf("%s", newdivider);
      ceol(scrwid, row);
    } else {
      i=topofLowerScreen();
      while(row<i) ceol(0, row++);      /* Remove part of prev page */
    }
}

static void
Readdir(dir)                   /* Reads directory dir[] */
const char   *dir;
{
    struct dirent *dentryp;
    DIR *dirp;
    char buf[MAXLBUFF];
    char *p;

#if defined(HAVE_OPENDIR)
    if((dirp = opendir(dir)) == NULL)
#else
    FILE *dirf;
    if((dirf = fopen(dir, "r")) == NULL)
#endif
    {
        sprintf(buf,"Sorry, %s unreadable.", dir);
        telluser(buf);
        sleep(1);
        Quit(1);
    }
#if defined(HAVE_OPENDIR)
    while((dentryp = readdir(dirp)) != NULL)
#else
    for(;;)
#endif
    {
#if !defined(HAVE_OPENDIR)
        struct dirent    dentry;

        if(fread((char *) & dentry, sizeof(dentry), 1, dirf) != 1)
            break;
        dentryp = &dentry;
#endif
#if 0   /* NHFB: June 1996, include "." and ".." directories */
        if((dentryp->d_ino == 0)
            || ((dentryp->d_name[0] == '.') &&
                ((dentryp->d_name[1] == '\0') ||
                 ((dentryp->d_name[1] == '.') && (dentryp->d_name[2] == '\0')))))
            continue;
#else
        if(dentryp->d_ino == 0) continue;
#endif

        if(totfiles == MAXcountFILES){
            overflow();                 /* too many files */
            initFileList();             /* Re-alloc a larger list */
        }
        setFileDefaults(totfiles);      /* Stuff default values */
        file(totfiles)->lnum = dentryp->d_ino;

#if defined(HAVE_DIRENT_H)
        p=dentryp->d_name;
#else       /* old <sys/dir.h> style might lack NUL termination */
        memcpy(p=buf, dentryp->d_name, D_NAMLEN(dentryp));
        p[D_NAMLEN(dentryp)]=0; /* Null termination */
#endif
        /*** GBG 12/95 Dynamic memory alloc ***/
        if((totfiles/20)*20 == totfiles){
           printf("."); fflush(stdout);
        }
        newcore(&FNAME(totfiles),p);
        if(slowstat){ /* Do stats when reading files */
          /* Just ignore if can't find the file, dir may be changing */
          sprintf(buf,"%s/%s",dir,FNAME(totfiles));
          if(gstat(buf,totfiles)) continue;
        }
        ++totfiles;
    }

#if defined(HAVE_OPENDIR)
    closedir(dirp);
#else
    fclose(dirf);
#endif
}

static int
getStats(whichone) int whichone; {
char s[MAXLBUFF];
    if(file(whichone)->lsize == (long)-1){
      strcpy(s,FNAME(whichone));
      expandpath(s);
      if(gstat(s,whichone)!=0) return -1;
    }
    return 0;
}

static int
gstat(gname,n)                  /* Stats the file with gname */
const char   *gname; int n;     /* Puts answers into file(n). */
{
int x;
struct lbuf *p;

    if(lstat(gname, &statbuf) < 0) return(-1);
    p=file(n);
    p->lnum = statbuf.st_ino;
    p->lsize = statbuf.st_size;
    switch(statbuf.st_mode & S_IFMT)
    {
        case S_IFREG:          /* A file */
             x = '-'; break;
        case S_IFDIR:          /* A directory */
             x = 'd'; break;
        case S_IFLNK:          /* A link */
             x = 'l'; break;
        case S_IFIFO:          /* A pipe */
             x = 'p'; break;
        case S_IFSOCK:         /* A socket*/
             x = 's'; break;
        case S_IFBLK:          /* A block device */
             x = 'b';
             p->lsize = statbuf.st_rdev;
             break;
        case S_IFCHR:          /* A char device */
             x = 'c';
             p->lsize = statbuf.st_rdev;
             break;
    }
    p->ltype = x;
    p->lflags = statbuf.st_mode;
    p->luid = statbuf.st_uid;
    p->lgid = statbuf.st_gid;
    p->lnl = statbuf.st_nlink;
    p->latime = statbuf.st_atime;
    p->lctime = statbuf.st_ctime;
    p->lmtime = statbuf.st_mtime;
    return(0);
}

static long
nblock(size)
long    size;
{
    return((size + 511) >> 9);
}

static char *endof(s) char *s; {
  return(s+ strlen(s));
}

/* Here's the output of "ls -l":
123456789012345678901234567890123456789012345678901234567890
-rw-r--r--  1 gustafso   105570 Jan 29 10:55 dired.c
              12345678901234567890
This has problems with 10mb files. The size field is too small.
And now the old "dired":
123456789012345678901234567890123456789012345678901234567890
-rw-r--r-- 1 1015           105947 Jan 29 11:34  dired.c
             12345678901234567890

The user name error has been fixed. The link field is supposed
to be one space wider. The user field is too long by 4 chars.
The extra space after the date is used as a tag field by dired.

The fixed version (Jan 1996):
123456789012345678901234567890123456789012345678901234567890
-rw-r--r--  1 gustafso    106233 Jan 29 11:39   dired.c
*/
static void
getEntry(whichone,s,len)
int     whichone;
char    *s;
int     *len;           /* Offset to beginning of the filename, or -1 */
{
int     x,y;
char    *cp, *p, *q;
char buf[MAXLBUFF];
struct lbuf *f;

    s[0]=0; len[0] = -1;
    f=file(whichone);
    if(f->lnum == (unsigned long)(-1)) return;
    if(getStats(whichone)) return;
/* Nothing except file name */
    if(nflg) goto setcolumn;
/* inode number */
    if(iflg)
        sprintf(endof(s),"%5lu ", (unsigned long)f->lnum);
/* Number of blocks */
    if(bflg)
        sprintf(endof(s),"%4ld ", nblock(f->lsize));
/* Protection modes and number of links */
    if(lflg || mflg){ /* GBG: added space to match "ls -l" */
        sprintf(endof(s),"%c",f->ltype);
        pmode(f->lflags,endof(s));
        sprintf(endof(s)," %2lu ", (unsigned long)f->lnl);
    }
/* File owner */
    /* GBG: Field width changed 14 ==> 10. See "ls -l" above. */
    /* 1980 version opened /etc/passwd and /etc/group directly. */
    /* 1996 version uses ideas from GNU fileutil 3.12 (1994) sources. */
    p=0;
    if(uflg) p=Getuser(f->luid);
      else
    if(gflg) p=Getgroup(f->lgid);
    if(p) sprintf(endof(s),"%-10.10s ",p);
/* File size in bytes */
    if(lflg || sflg){
        if(f->ltype == 'b' || f->ltype == 'c')
            sprintf(endof(s),"%3ld,%3ld ",
                   (long)major((int) f->lsize),
                   (long)minor((int) f->lsize));
        else
            sprintf(endof(s),"%7ld ", f->lsize);
    }
/* Date field */
    if(lflg || dflg){
        if((char *)strchr("wnseiu",sortyp) != (char *)0)
        {
            cp = ctime(&f->lmtime);
            if(f->lmtime < year)
                sprintf(endof(s),"%-7.7s %-4.4s ", cp + 4, cp + 20);
            else
                sprintf(endof(s),"%-12.12s ", cp + 4);
        }
        else if(sortyp == 'r')
        {
            cp = ctime(&f->latime);
            if(f->latime < year)
                sprintf(endof(s),"%-7.7s %-4.4s ", cp + 4, cp + 20);
            else
                sprintf(endof(s),"%-12.12s ", cp + 4);
        }
        else if(sortyp == 'c')
        {
            cp = ctime(&f->lctime);
            if(f->lctime < year)
                sprintf(endof(s),"%-7.7s %-4.4s ", cp + 4, cp + 20);
            else
                sprintf(endof(s),"%-12.12s ", cp + 4);
        }
    }
/* Dired's tag field */
setcolumn:

/* Leave white space after flag */
    sprintf(endof(s),"%c ",f->deleted);
    p=endof(s)-1;               /* Point to white space, maybe p[0]='!' */
/* File name in various formats */
    cp = FNAME(whichone);
    x=0; y=strlen(s); CURSORcolumn=y-2;
    len[0]=y;
/* Path or just filename? */
    if(pflg) getCurfileName(cp=buf,whichone);
    else if((q=(char *)strrchr(cp,'/')) != (char *)0) cp = q+1;
    while(cp[0]) buildcharstring(*cp++,endof(s),&x);
    if(f->ltype == 'd') strcat(s,"/");    /* Flag directories */
    if(f->ltype == 'l') strcat(s," ->");  /* Flag symbolic links */
    x=strlen(s);
    if(y>=scrwid) {len[0] = -1; s[scrwid]=0;}
      else
    if(x>scrwid){
      strcpy(s+y,s+x-(scrwid-y));
      p[0]='!';                         /* Mark incomplete displays */
    }
}

static void
printEntry(curfile,row)
int curfile,row;
{
char tmp[MAXLBUFF+512];
char buf[MAXLBUFF];
int col,len;
char *q;
    getEntry(curfile,tmp,&len);
    col=strlen(tmp);
#ifdef ANSICOLOUR
    if(ansiColors && len != -1){
      if(( q=colorString(FNAME(curfile),file(curfile)->lflags)) != NULL){
        strcpy(buf,(char *)&tmp[len]);
        sprintf((char *)&tmp[len],"\033[%sm%s\033[m",q,buf);
      }
    }
#endif
    curxy(0,row); printf("%s",tmp); ceol(col,row);
}

static int     m1[] =
{
    1, S_IREAD >> 0, 'r', '-'
};
static int     m2[] =
{
    1, S_IWRITE >> 0, 'w', '-'
};
static int     m3[] =
{
    2, S_ISUID, 's', S_IEXEC >> 0, 'x', '-'
};
static int     m4[] =
{
    1, S_IREAD >> 3, 'r', '-'
};
static int     m5[] =
{
    1, S_IWRITE >> 3, 'w', '-'
};
static int     m6[] =
{
    2, S_ISGID, 's', S_IEXEC >> 3, 'x', '-'
};
static int     m7[] =
{
    1, S_IREAD >> 6, 'r', '-'
};
static int     m8[] =
{
    1, S_IWRITE >> 6, 'w', '-'
};
static int     m9[] =
{
    2, S_ISVTX, 't', S_IEXEC >> 6, 'x', '-'
};

static int    *m[] =
{
    m1, m2, m3, m4, m5, m6, m7, m8, m9
};

static void
pmode(aflag,buf)
int aflag; char *buf;
{
    register int  **mp;

    flags = aflag & ~S_IFMT;
    for(mp = &m[0]; mp < &m[sizeof(m) / sizeof(m[0])];)
        *buf++ = Select(*mp++);
    *buf=0;
}

static int
Select(pairp)
register int   *pairp;
{
    register int    n;

    n = *pairp++;
    while(--n >= 0 && (flags & *pairp++) == 0)
        pairp++;
    return(int)(*pairp);
}

static int
compar(pp1, pp2)
const void *pp1;
const void *pp2;
{
    register const struct lbuf *p1, *p2;
    char *s1,*s2;

    p1 = (const struct lbuf*)pp1;
    p2 = (const struct lbuf*)pp2;
    if(p1 -> ltype == 'd')
    {
        if(!(p2 -> ltype == 'd')) goto minus;
    }
    else
    {
        if(p2 -> ltype == 'd') goto plus;
    }
    switch(sortyp){
    case 0: break;
    case 'n':  /* File name */
        return(rflg * strcmp(p1->namep,p2->namep));
    case 's': /* Sort on size */
        if(p2 -> lsize == p1 -> lsize) break;
        if(p2 -> lsize > p1 -> lsize) goto plus;
        goto minus;
    case 'w':  /* Write date */
        if(p2 -> lmtime == p1 -> lmtime) break;
        if(p2 -> lmtime > p1 -> lmtime) goto plus;
        goto minus;
    case 'i':  /* inode */
        if(p2 -> lnum == p1 -> lnum) break;
        if(p2 -> lnum > p1 -> lnum) goto plus;
        goto minus;
    case 'r':  /* Read date */
        if(p2 -> latime == p1 -> latime) break;
        if(p2 -> latime > p1 -> latime) goto plus;
        goto minus;
    case 'c':  /* Creation date */
        if(p2 -> lctime == p1 -> lctime) break;
        if(p2 -> lctime > p1 -> lctime) goto plus;
        goto minus;
    case 'e':
      if((s1 = (char *)strrchr(p1->namep,'.'))==(char *)0) s1 = "" ;
      if((s2 = (char *)strrchr(p2->namep,'.'))==(char *)0) s2 = "" ;
      return(rflg * strcmp(s1,s2));
    case 'u':
      s1=Getuser(p1->luid); s2=Getuser(p2->luid);
      return(rflg * strcmp(s1,s2));
    }
    return 0;           /* p2 == p1 */
plus:
    return(rflg);      /* p2 > p1 */
minus:
    return(-rflg);     /* p2 < p1 */
}

static void
ceol(x,y)
int x,y;
{
    int i;
    curxy(x,y);
    if(CE != 0) putpad(CE);
    else {  /***crh for terms w/o ce, put blanks & restore cursor **/
      for(i=x; i <  scrwid ; i++ ) putchar(' ');
      curxy(x,y);
    }
}

static void
ceod(x,y)
int x,y;
{
    int i;
    ceol(x,y);
    for(i=y+1; i <= Tscrlen ; i++ ) ceol(0,i);
    curxy(x,y);
    fflush(stdout);
}

static void
blank()
{
    clearLastLine();
    ceod(0,0);
}

/* 1980, SMC
   Yes, folks, we use direct cursor addressing to get to next line!
   Before you mumble "What sort of cretin would do this?" here's
   the reason. We don't use \n since that obviously won't work.
   We don't use \012 since virgin version 7 makes that into a crlf.
   We don't use raw mode since we type out help files efficently,
   and we don't want to switch modes all the time. So enjoy. -- SMC */

static void
downline()
{
    curxy(CURSORcolumn, ++curline);
    Curfile++;
}

static void
telluser(msg)          /* GBG: Must be able to print msg[] with */
const char   *msg;      /* percent signs. Only need one arg. */
{
    clearLastLine();
    tellinfo(msg);
}

static void
tellinfo(msg)          /* GBG: Must be able to print msg[] with */
const char   *msg;      /* percent signs. */
{
    ceol(0,Tscrlen-1);
#ifdef ANSICOLOUR
    if(ansiColors) printf("\33[01;31m%s\33[m",msg); /* Bright red */
    else
#endif
    printf("%s",msg);
    restoreCursor();
    fflush(stdout);     /* See it now, not later. */
    infoflg=1;
}

static void
curxy(col, lin)
int col;
int lin;
{
    putpad(tgoto(CM, col, lin));
}

/* Expand a char to a printable string 0 to 8 chars long */
/* RETURN: 0=normal char, 1=TAB, 2=Newline, 3= (cc>=127), 4=CTRL */
static int buildcharstring(cc,buf,N)
int cc; char *buf; int *N; {
int n,flag;
        flag=0;
        if(cc == '\t'){
          n=(N[0]/8) * 8 + 8 - N[0];   /* Compute next tab stop */
          strcpy(buf,"        ");       /* eight blanks */
          buf[n]=0; flag=1;
        }
        else if(cc == '\n' || cc == '\r') {buf[0]=0; flag=2;}
        else if(cc >= 127) {
         sprintf(buf,"[%02x]",cc); flag=3;
        }
        else if((cc>=0) && (cc<=31)){
          sprintf(buf,"^%c",cc+64); flag=4;
        } else {
          buf[0]=cc; buf[1]=0;
        }
        N[0] += strlen(buf);
        return flag;
}

static int
Type(filestr)
const char *filestr;
{
    FILE    *fp;
    register int     i,j;
    register int     cc = 0;
    int     flag;
    int     n;
    int     Workbot,Worktop;
    char    buf[128];           /* Temp buffer for char expansion */

    fp = fopen(filestr, "r");
    if(fp==(FILE *)0){
      tellinfo("?Unable to open file for reading.");
      return 2;
    }
    sigint = 0;

    Worktop = (splitflg ? topofLowerScreen() : 0);
    Workbot = (splitflg ? Tscrlen-1 : Tscrlen-2);
    do{
      ceod(0, Worktop);
      ansiBOLD();
      printf("[File=%s]",filestr);
      ansiUNBOLD();
      curxy(0, Worktop+1);
      for(i = Worktop+1; (i < Workbot) && !sigint && (cc != EOF); i++) {
        n = 0;
        while((cc = fgetc(fp)) != EOF) {
          if(cc == '\r'){
            if((j=fgetc(fp)) != EOF && j != '\n') ungetc(j, fp);
            else cc = '\n';
          }
          flag=buildcharstring(cc,buf,&n);
          if(flag==2 || n >= scrwid){
            if(flag != 1 && flag != 2) ungetc(cc, fp);
            printf("\n");
            break;
          }
          printf("%s",buf);
        }
      }
    } while(!sigint && (cc != EOF) && waitchk("--More--"));

    flag = (feof(fp) != 0);
    fclose(fp);
    sigint=0;
    if(!splitflg && flag) {
      waitchk("[End-of-File]: ");
      return(True);
    } else if(flag) tellinfo("[End of File]");
    sigint=0;
    return(splitflg ? False:True);
}

static int
waitchk(msg)
const char *msg;
{
        printLastLine((char *)msg);
        if(getchar() == 'q')
          sigint = 1;           /* simulate interrupt */
        clearLastLine();
        if(sigint) return(0);   /* avoids clear of screen */
        return(1);
}

static int YESsetdpy=0;

static void
setdpy()
{
  if(!YESsetdpy){
#if defined(HAVE_TERMIOS_H)
    struct termios tty;         /* XPG3, POSIX.1, FIPS 151-1 interface */

    if(tcgetattr(fileno(stdin),&tty) != -1)
    {
        tty_save = tty;
        tty.c_iflag &= ~(INLCR | ICRNL | ISTRIP | IXON | BRKINT);

#if defined(IUCLC)
        tty.c_iflag &= ~IUCLC;          /* absent from POSIX */
#endif /* defined(IUCLC) */

        tty.c_lflag &= ~(ECHO | ICANON);
        tty.c_cc[VMIN] = 1;             /* GBG 1/96. Was 5=MIN */
        tty.c_cc[VTIME] = 0;            /* GBG 1/96. Was 2=TIME */
       (void)tcsetattr(fileno(stdin),TCSANOW,&tty);
    }
#else /* NOT defined(HAVE_TERMIOS_H) */
    ioctl(0, TIOCGETP, &ioctlb);
    ioctlb.sg_flags |= CBREAK;
    ioctlb.sg_flags &= ~ECHO;
    ioctl(0, TIOCSETP, &ioctlb);
#endif /* defined(HAVE_TERMIOS_H) */
  }
    YESsetdpy=1;
}

static void
unsetdpy()
{
  if(YESsetdpy){
#if defined(HAVE_TERMIOS_H)
    (void)tcsetattr(fileno(stdin),TCSANOW,&tty_save);
#else /* NOT defined(HAVE_TERMIOS_H) */
    ioctlb.sg_flags &= ~CBREAK;
    ioctlb.sg_flags |= ECHO;
    ioctl(0, TIOCSETP, &ioctlb);
#endif /* defined(HAVE_TERMIOS_H) */
  }
  YESsetdpy=0;
}

static
char * skipdelay(s) char *s; {
char *p;
  p=s;
  while(p[0] && ISDIGIT(p[0])) ++p;
  if(p[0]=='.') ++p;
  while(p[0] && ISDIGIT(p[0])) ++p;
  if(p[0]=='*' && p!=s) ++p;
  return p;
}

int getcaptrue=0;
char   *Ks;
char   *Ke;

static void
Setcap()
{
  putpad(Ks);   /* Set keypad application mode */
}

static void
UnSetcap()
{
  putpad(Ke);   /* Unset keypad application mode */
}

static void
Getcap()
{
    char   *ap;
    char   *term;
    char   *xPC;
    char   *Se;
    int error;

    term = getenv("TERM");
    error=0;
    if(term==0) fprintf(stderr, "TERM variable missing from environment\n");
top:
    if(term == 0){
        fprintf(stderr,"Using TERM=vt100\n");
        term="vt100";
        ++error;
    }

    getcaptrue=0; Ks=Ke=CM=CE=Se=(char *)0;

#if 0
    switch(-1/*(int)(tgetent(tbuf, term))*/)
#else
    switch((int)(tgetent(tbuf, term)))
#endif
    {
        case -1:
            fprintf(stderr, "Cannot open termcap file. ");
setvt100:
            fprintf(stderr, "Using internal vt100.\n");
#if defined(SYSV)
            CM = "\033[%i%p1%d;%p2%dH$<5>";
#else
            CM = "\033[%i%d;%dH";
#endif
            CE = "\033[K";
            Se = "\033[m";
            Ks="\033[?1h\033=";
            Ke="\033[?1l\033>";
            ++error; break;
        case 0:
            fprintf(stderr, "%s: Unknown terminal.\n", term);
            ++error;
            if(error <2){ term=0; goto top;}
            goto setvt100;
        default:
            getcaptrue=1; break;
    }
    if(getcaptrue){
      ap = tcapbuf;

      CE = tgetstr("ce", &ap);   /* Can be simulated */
      CM = tgetstr("cm", &ap);   /* Needed! Can't simulate. */
      Se = tgetstr("se", &ap);   /* Test for ansi color 6429 */
      Ks = tgetstr("ks", &ap);   /* Optional keypad setup */
      Ke = tgetstr("ke", &ap);   /* Optional keypad exit */

      xPC = tgetstr("pc", &ap);
      if(xPC) PC = *xPC;

#if 0
{ char *pp;
  pp=CM; while(pp[0]){printf("%02x [%c]",pp[0],(pp[0]<32 ? ' ' : pp[0])); ++pp;}
getchar();
}
#endif
    }
    /*** crh changed error test. GBG also 1/96. ***/
    if(CM==0){
        fprintf(stderr, "No cursor addressing for terminal %s\n",term);
        goto setvt100;
    }

    getWinSize();                  /* Compensates by 1 for width and length */

    if(error){
       fprintf(stderr,"Press return to continue (ctrl-C aborts): ");
       fflush(stderr);
       getchar();
    }
    /* If terminal doesn't have "\33[m", then it's not ANSI 6429. */
#ifdef __linux__  /* linux has se but it's not \33[m */
    if(Se==0){
#else
    if(Se==0 || strncmp(skipdelay(Se),"\33[m",3) != 0){
#endif
       ansiColors=0;
    }
    clearLastLine();
}

static void
getWinSize(){
  /* On SunOS, tgetnum("li") returns the true window size, but under */
  /* ULTRIX, it returns the value from the termcap entry, which is usually */
  /* wrong in a window environment.  Get the true values if possible. */
  if(getcaptrue){
    Tscrlen = tgetnum("li");  /* Take defaults */
    scrwid = tgetnum("co");   /* Don't scroll in last line */
  }
#ifdef TIOCGWINSZ
    {
        struct winsize win;

        if(ioctl(0, TIOCGWINSZ, &win) == 0)
        {
                if(win.ws_col)
                        scrwid = win.ws_col-1;
                if(win.ws_row)
                        Tscrlen = win.ws_row-1;
        }
    }
#endif
    if(Tscrlen <= 0 || scrwid <= 0)
    {
        fprintf(stderr, "Must know the screen size, assuming 24x80\n");
        Tscrlen=24; scrwid=80;
    }
    if(scrwid > MAXWIDTH) scrwid = MAXWIDTH;
    scrwid--;   /* ULTRIX wraps in last column, safety factor 1 */
    Tscrlen--;  /* don't use final line, in case it is terminal status */
                /* line (e.g. line 25 on VT100 display) */
                /* ULTRIX scrolls when cursor enters last line */

}

static int
outch(int c)
{
    return putchar(c);
}

static void
putpad(str)
char   *str;
{
    if(str)
        tputs(str, 1, outch);
}

static handler_t
catchint(sig)
int sig;
{
        signal(SIGINT, SIG_IGN);        /* reset it */
        ++sigint;
#if defined(TIOCSTI)
        typein("\r");
#else
        if(sigint==1) printf("^C"); fflush(stdout);
#endif
        signal(SIGINT, catchint);        /* reset to default */
#if !defined(HAVE_VOID_HANDLER)
        return(0);
#endif
}

static void
overflow()
{
    oldMAXFILEScount = MAXcountFILES;   /* Save count for initializer */
    MAXcountFILES += MAXallocFILES;     /* Bump up number of files */
}

static char *
skipto(string)
const char *string;
{
        char *strp;
        strp = (char *)strchr(string,'%');
        if(strp != (char *)0){
          if((char *)strchr("nfdp%",strp[1])!=(char *)0)
            return strp;
        }
        return (char *)0;
}

static void
setShellPause(){
#define PAUSING   "Prompting before and pausing after system commands"
#define NOPAUSING "No prompts or pauses in system commands"
        pause_return = (pause_return ? 0 : 1);
        tellinfo((pause_return ? PAUSING : NOPAUSING));
}

static char *core(len) int len; {
char *p;
   if(!len) len=1;
   p=(char *)malloc((size_t)len);       /* Don't malloc(0)! */
   if(p==(char *)0){
     telluser("Out of memory - aborting dired: ");
     getchar();
     Quit(1);
   }
   return p;
}

/* WARNING: free() gets called if d[0]!=0 */
/* When calling newcore(), set first arg=0 before the call */
static void
newcore(d,s) char **d; const char *s; {
char *p;
int len;
        len=strlen(s);          /* This can be ZERO */
        p=(char *)core(len+1);  /* Space for NULL at the end */
        if(d[0]!=0) free(d[0]); /* If d[0]==0, don't free it */
        strcpy(p,s);            /* Copy data set and NULL */
        d[0]=p;                 /* d[0] == buffer address */
}

static void
regexpMark(c) int c;
{
  int i,j,status,new,x;
  char buff[MAXLBUFF];

  sprintf(buff,"Search forward and tag '%c' on regular expression matches",c);
  tellinfo(buff);
  status=getline(buff,"/");
  if(!status){
    tellinfo("?Pattern too long");
    return;
  }
  if(buff[0]==0 || status==2) {
    tellinfo("Search aborted"); return;
  }
  if(re_comp(buff) != (char*)NULL){
    tellinfo("?Invalid regular expression. Get help with 'man ed'.");
    return;
  }
  j=Curfile; new=0;
  while(j<totfiles){
    status=0;
    for(i=j; i < totfiles; i++) {
      status = match(i);
      if(status != 0) break;
    }
    if(status == -1) {
      tellinfo("?Botched regular expression");
      getline(buff,"Press Return: ");
      j=totfiles;
    } else if(status) {
         x  = (c == '*' && file(i)->deleted != MARKED);
         x |= (c == 'D' && file(i)->deleted != DELETED);
         if(x){ setFileMark(c,i);  ++new;}
    }
    j = i+1;
  }
  sprintf(buff,"Number of files tagged '%c': %d",c,new);
  if(new) redraw();

  telluser(buff);
  return;
}

static void
searchRegExp(n) int n; { /* n==0: Reverse search, n==1: Forward search */
int status;
char buff[MAXLBUFF];
    sprintf(buff,"Find %s file matching regular expression (Ctrl-C aborts)",
                  (n==1 ? "next" : "previous"));
    tellinfo(buff);
    status=getline(buff,"/");
    if(status==0) tellinfo("?Pattern too long");
      else
    if(status==2) tellinfo("Search aborted");

    if(buff[0] && (re_comp(buff) != (char*)NULL))
      tellinfo("?Invalid regular expression. Get help with 'man ed'.");
      else
    if(status==1) { saveposition=1; findfile(n); }
}

static void
getChmodString(s) char *s; {
char buf[128]; char *p,*q;
    pmode(file(Curfile)->lflags,buf);
    sprintf(s,"[u=%.3s,g=%.3s,o=%.3s]: ",&buf[0],&buf[3],&buf[6]);
    p=q=s;
    while(q[0]){
      if(q[0] == '-'){ ++q; continue;}
      *p++ = *q++;
    }
    p[0]=0;
}

static void
doChmod(){
char buf[MAXLBUFF];
char Mode[128];
int status;
static char *lastInput=0;
char *p;
    /* Allow chmod on anything? Maybe only files? */
    printf("_");                        /* Show the line we are on. */
    if(!lastInput)
      strcpy(buf,"Chmod example: 'u=rwx,g=,o=rx' produces 'rwx---r-x' (User,Group,Other)");
    else
      sprintf(buf,"Last chmod request: '%s' [Period (.) to repeat]",lastInput);
    tellinfo(buf);
    getChmodString(Mode);
    strcpy(buf,CH); strcat(buf," ");
    status=getline(p=endof(buf),Mode);
    if(status==2) p[0]=0;
    if(p[0]){
      if(p[0] == '.' && lastInput != 0) strcpy(p,lastInput);
      else newcore(&lastInput,p);
      p=endof(buf); *p++ = ' ';         /* Add a space */
      getCurfileName(p,Curfile);
      (void)doShell(buf);
      gstat(p,Curfile);
    } else buf[0]=0;
    printEntry(Curfile,curline);
    tellinfo(buf);
}

static void
doCopyMove(n) int n; {                       /* Target for COPYFILE */
char buf[MAXLBUFF];
char buf1[MAXLBUFF];
int status;
char *p,*q;
    /* Allow COPY on anything? Maybe only files? */
    printf("_");                        /* Show the line we are on. */
    tellinfo((n==0 ? "Copy current file" : "Move current file"));
    status=getline(buf1,"Destination: ");
    if(buf1[0]!=0 && status!=2){
      strcpy(buf,(n==0 ? CP : MV));
      strcat(buf," ");
      getCurfileName(endof(buf),Curfile); strcat(buf," ");
      getCurfileName(p=endof(buf),Curfile);
      if(buf1[0]!='.' && buf1[0]!='/'){
        q=(char *)strrchr(buf,'/');
        if(q!=(char *)0) strcpy(q+1,buf1); else strcpy(p,buf1);
      } else strcpy(p,buf1);
      telluser("");
      (void)doShell(buf);
      strcpy(buf,(n==0 ? "[Copied]" : "[Moved]"));
    } else strcpy(buf,(n==0 ? "[Copy aborted]" : "[Move aborted]"));
    printEntry(Curfile,curline);
    tellinfo(buf);
}

/* Save system commands for reuse */
static void
storeCmd(buf) char *buf; {
struct systemcommands *tail;
  tail = (struct systemcommands *) core(sizeof (struct systemcommands));
  tail->cmd=0;
  newcore(&tail->cmd,buf);
  /* Add to the head of the list, so most recently used is first.  */
  tail->next = cmdlist;
  cmdlist = tail;
}

/* Returns 0, 1, 2: 0==Too wide, 1==Ok, 2==Ctrl-C */
static int
getline(s,t)
char s[]; const char *t;
{
int status,c,i,j;
char buf[128];
char *p;
struct systemcommands *list;
struct systemcommands *tmplist;
int wasfunctionkey;
top:
    sigint = 0;                 /* catchint() does ++sigint on ctrl-C */
    s[0]='\0';
    i=0;
    list=cmdlist;
    if(cmdlist==0) storeCmd("");
redisplay:
    status=1;
    printLastLine((char *)t);
    p=s;
    j=0;
    while(p[0]){ buildcharstring(*p++,buf,&j); printf("%s",buf); }
    while((c=getchar()) != EOF) {
      if(sigint >= 1){status=2; i=0; break;}
      wasfunctionkey=0;
      if(c=='\r' || c=='\n') break;
      if(c == KILL_CHAR) goto top;
      if(c < 32 || c > 127){
        c=getkeymatch(c,GNUkeysSIZE);
        wasfunctionkey=1;
      }
      switch(c){
        case QUOTEKEY:
          c=getchar(); wasfunctionkey=0; break;
        case PREVFILE:
          strcpy(s,list->cmd);
          if(list->next != 0) list = list->next;
          i=strlen(s);
          break;
        case NEXTFILE:
          tmplist = cmdlist;
          while(tmplist->next != list && tmplist->next != 0)
            tmplist=tmplist->next;
          if(tmplist != list){
            list=tmplist;
            strcpy(s,list->cmd);
            i=strlen(s);
          }
          break;
        case 127:
        case   8:
          if(i>0) s[--i]=0;
          wasfunctionkey=1; break;
      }
      if(wasfunctionkey) goto redisplay;
      buildcharstring(c,buf,&j);
      if(j >= scrwid) {status=0; break;}
      printf("%s",buf); s[i++]=c;
    }/* End while() */
    s[i]='\0';
    sigint=0;
    return(status);
}

/* Process a shell string from buf[] to bldbuf[] using file(i). */
/* Substitutes %n, %f, %d, %p, %% totkens. */
static int
buildcommand(buf,bldbuf,i)
char *buf; char *bldbuf; int i;
{
char *op,*np; int special;
char buf1[MAXLBUFF]; int status;  char *p,*q;
  op = buf;
  special = 0;
  bldbuf[0]=0;
  while(1) {
    np=skipto(op);
    if(np==0) break;
    special=1;                /* set flag */
    np[0] = '\0';             /* zap the percent */
    strcat(bldbuf, op);       /* 1st part */
    np[0]='%';                /* Put back the percent */
    switch(np[1]){
    case '%':    /* percent sign request */
      strcat(bldbuf,"%");
      break;
    case 'n':    /* path and file name */
      getCurfileName(endof(bldbuf),i);
      break;
    case 'f':    /* file name only */
      strcat(bldbuf, FNAME(i));
      break;
    case 'd':    /* path name only */
        getCurfileName(p=endof(bldbuf),i);
        /* Trim off the filename, if there is one */
        if(file(i)->ltype != 'd'){
          q=(char *)strrchr(bldbuf,'/');
          if(q!=0 && p!=q) q[0]=0;
        }
       break;
    case 'p':    /* prompt for target */
       status=getline(buf1,"Target: ");
       curxy(0,Tscrlen);
       if(buf1[0]!=0 && status!=2){
         getCurfileName(p=endof(bldbuf),i);
         if(buf1[0]!='.' && buf1[0]!='/'){
           q=(char *)strrchr(buf,'/');
           if(q!=(char *)0) strcpy(q+1,buf1); else strcpy(p,buf1);
         } else strcpy(p,buf1);
       } else {special = -1; goto abort;}
      break;
    } /* end switch */
    op = np+2;
  } /* End while */
  strcat(bldbuf, op);
abort:
  return special;
}

static void
doShellCommand(s) char *s; {
register char *op,      /* old ptr */
*np;                    /* new ptr */
char bldbuf[MAXLBUFF];
char buf[MAXLBUFF];
int markedfiles;        /* Number of files marked */
int i,x,start,cc;
int     special;        /* flag==1 if had %%, %n, %f, %p, %d in cmd */
int status;

top:
        if(s == (char *)0){
          tellinfo("Enter alias letter or a shell command. [?=help]");
          status=getline(buf,": ");
          if(status!=1 || buf[0]==0 || buf[0]=='\n')
            goto abortsyscmd;
          if(buf[0]=='?' && buf[1]==0){ /* Wants help */
            aliasHelp();
            goto top;
          }
          if(buf[1]==0){        /* 1-char commands are aliases */
            if(aliasGet(buf,buf[0])!=0){
              telluser("Alias not found."); goto abortsyscmd;
            }
          } else { /* New shell command string, try to save it. */
            telluser("To save this shell command as an alias,");
            printLastLine("enter one letter (RETURN skips): ");
            cc=getchar();
            if(cc != '\r' && cc!='\n') saveAlias(cc,buf);
          }
        } else strcpy(buf,s);
        /* remember the command */
        if(strcmp(cmdlist->cmd,buf)!=0) storeCmd(buf);
        if(skipto(buf) == (char *)0) markedfiles=0; else
        markedfiles=nummarked;
        if(markedfiles){
          clearLastLine();
          printf("Apply it to %d tagged file%s? [Y/N]: ",
                   nummarked,(nummarked ? "s" : ""));
          i=getchar();
          if(i!='y' && i!='Y') goto abortsyscmd;
          start=0;
        } else  start=Curfile;
        if(pause_return || markedfiles != 0) blank();
        /* Loop over all tagged files */
        for(i = start; i < totfiles; ++i){
          if(markedfiles){
            if(file(i)->deleted != MARKED) continue;
            setFileMark((int)'%',i);
          }
          strcpy(buf,cmdlist->cmd);
          special=buildcommand(buf,bldbuf,i);
          if(special == -1) goto abortsyscmd;
          if(special) printf( "%s\n", bldbuf);
          if(pause_return){
            printf("\nRETURN to begin or q to quit: ");
            x = 'x';
            while((char *)strchr("qQ\r\n",x)==(char *)0) x=getchar();
            printf("\n");
            if(x=='q' || x=='Q') break; /* Exit for-loop */
          }
          (void)doShell(bldbuf);
          if(markedfiles==0) break;
        } /* End of loop on file names */
        if(pause_return || markedfiles != 0){
          printf("\rAny key to return...");
          getchar();
          printf("\r                    ");
        }
        redraw();
        tellinfo("Shell command finished");
        return;
abortsyscmd:
        restoreCursor();
        return;
}

static int
doShell(cmd)
char *cmd; {
int n;
#if 0
 logger(1);             /* log commands to a file, debug */
#endif
    unsetdpy();
    OFFsignals(1);
    ceod(0,Tscrlen-2);  /* Set cursor at bottom of lower screen */
    n=system(cmd);
    setdpy();
    ONsignals();
    return n;
}

static void
doPager(curfile) int curfile;  {
char *p;
char tmp[MAXLBUFF];
        if(file(curfile)->ltype == 'd') {
          tellinfo("?This is a directory, paging works on files only");
        } else {
          /* allow PAGER environment variable to override builtin */
          p = getenv("PAGER");
          sprintf(tmp,"%s ",(p ? p : MOREPGM));
          getCurfileName(endof(tmp),curfile);
          blank();
          (void)doShell(tmp);
          redraw();
          telluser("");
        }
        return;
}

static void
doManualPage(curfile) int curfile;  {
char *p;
char tmp[MAXLBUFF];
        if(file(curfile)->ltype == 'd') {
          tellinfo("?This is a directory, groff works on files only");
        } else {
          strcpy(tmp,GR);
          strcat(tmp," ");
          getCurfileName(endof(tmp),curfile);
          /* allow PAGER environment variable to override builtin */
          p = getenv("PAGER");
          sprintf(endof(tmp)," | %s",(p ? p : MOREPGM));
          blank();
          (void)doShell(tmp);
          redraw();
          telluser("");
        }
        return;
}

static int
getLink(dest) char *dest; {
int n;
char buf[MAXLBUFF];
char *p;
    n = readlink(dest,buf,MAXLBUFF);
    if(n > 0){
      buf[n] = 0;
      if(buf[0] != '/') {
        p=(char *)strrchr(dest,'/');
        if(p==0){
          expandpath(buf);
        } else {
          strcpy(p+1,buf);
          strcpy(buf,dest);
        }
      }
      dcanon(buf);
      strcpy(dest,buf);
    }
    return n;
}

static void
showStats(which) int which; {
char tmp[MAXLBUFF];
char buf[MAXLBUFF];
int i,flag;
long total;
double x;
    clearLastLine();
    flag=(which ? 1 :statFlag);
    if(flag){
      printf("File %d of %d, %d flagged, %d deleted --%d%%--",
              Curfile + 1,totfiles,
              nummarked, numdeleted,((Curfile+1)*100/totfiles));
      if(which){ /* Print total space used by these files */
        for(total=i=0;i<totfiles;++i) total += file(i)->lsize;
        if(total < 1024) printf(" [%ld]",total);
        else
        if(total < 1024L*1024L) printf(" [%ldk]",(512+total)/1024);
        else {
          x=((total/1024.0)/1024.0);
          printf(" [%.3lfMb]",x);
        }
      }
      getCurfileName(tmp,Curfile);
      if(file(Curfile)->ltype == 'd') strcat(tmp,"/");
        else
      if(file(Curfile)->ltype == 'l'){
        if(getLink(tmp)){  /* Expand link name */
          strcpy(buf,tmp); sprintf(tmp,"-> %s",buf);
        }
      }
      if(strcmp(tmp,"/./")==0 || strcmp(tmp,"/../")==0 || strcmp(tmp,"//")==0) strcpy(tmp,"/");
      printpath(tmp);
    }
    restoreCursor();
}

static void
MarkFile(mark) int mark;
{
        printf("%c",setFileMark(mark,Curfile));
        if(Curfile == totfiles - 1) ;
        else {
          if(curline + 1 == scrlen) fscreen(1);
          downline();
        }
}

static int
setFileMark(mark,curfile) int mark,curfile;
{
register int x,fmark,newmark;
        fmark=file(curfile)->deleted;
        newmark=mark;
        x = -1;
        if(dotname(FNAME(curfile))) goto quit;
            /* Don't mark "." or ".." */
        switch(mark){
        case '*':
          if(fmark == MARKED){
            nummarked--;
            x = NOTDELETED;
            newmark = ' ';
          } else {
            if(fmark == DELETED) --numdeleted;
            nummarked++;
            x = MARKED;
          }
          break;
        case 'D':
          if(fmark != DELETED) numdeleted++;
          if(fmark == MARKED) nummarked--;
          x = DELETED;
          break;
        case '%':
          x=MARKEDonce;
          goto space;
        case ' ':
          x = NOTDELETED;
space:
          if(fmark == MARKED) nummarked--;
          if(fmark == DELETED) numdeleted--;
          break;
        default:
          if(' ' <= mark && mark < 127) {
            x=mark;
            goto space;
          } else break;
        }
quit:
        if(x != -1) file(curfile)->deleted = x; else newmark=fmark;
        return newmark;
}

static void
deleteBackups(x,mark) int x,mark;
{
int i,j,cc;
size_t n;
char tmp[256];

        for(cc=i = 0; i < totfiles; ++i){
          j=0;
          switch(x){
          case 1:  /* Test last char of filename */
            n = strlen(FNAME(i));
            j=(FNAME(i)[n-1] == mark);
            break;
          case 2:  /* Test for first char of filename */
            j=(FNAME(i)[0] == mark);
            break;
          } /* End switch */
          if(j){
            setFileMark((int)'D',i);
            ++cc;
          }
        } /* End for */
        sprintf(tmp,"Files matched: %d",cc);
        redraw();
        tellinfo(tmp);
}

static void
MarkAllFiles(x) int x;
{
int i;
const char *p;
        p="";
        switch(x){
        case ' ': /* Unmark all files */
            for(i = 0; i < totfiles; ++i) setFileMark(' ',i);
            p="All tags removed from files";
            break;
        case 'D': /* Mark all files for deletion */
            for(i = 0; i < totfiles; ++i) setFileMark('D',i);
            p="All files tagged 'D'. Untag all with U.";
            break;
        } /* End switch(x) */
        redraw();
        tellinfo(p);
}

static void
popBookMark(){
char s[256];
        if(mark_top > 0){
          dispfile(mark_stack[--mark_top]);
          sprintf(s,"Restored to bookmark %d [%d remain%s]",
                    1+mark_top,mark_top,(mark_top==1 ? "s" : ""));
        } else strcpy(s,"?No bookmarks: save some with M");
        tellinfo(s);
}

static void
pushView() {
  saveposition=0;               /* Mark last position saved */
  if(view_top >= MAX_VIEW){     /* Purge old views */
    register int i,j;
    i=0;j=MAX_VIEW/4;
    while(j<MAX_VIEW) view_stack[i++]=view_stack[j++];
    view_top=i;
  }
  view_stack[view_top++] = Curfile;
}

static void
popView(){
char s[256];
        if(view_top > 0){
          sprintf(s,"History view stack %d [%d possible]",view_top,MAX_VIEW);
          dispfile(view_stack[--view_top]);
        }
        else
        strcpy(s,"?Empty history stack. Searches and large moves add to the stack.");
        tellinfo(s);
}

static void
setBookMark(){
int i,j;
char s[256];
        mark_stack[mark_top++] = Curfile;
        if(mark_top >= MAX_MARKS){
          i=0; j=MAX_MARKS/4;
          while(j<MAX_MARKS){
             mark_stack[i++]=mark_stack[j++];
          }
          mark_top=i;
          sprintf(s,"Purged %d old bookmarks. Last bookmark number is %d.",MAX_MARKS/4,mark_top);
        } else
        sprintf(s,"Bookmark number %d [%d possible], restore it with J",
                   mark_top,MAX_MARKS);
        tellinfo(s);
}

static void
PrintFile(){
char tmp[MAXLBUFF];
int n;

        if(file(Curfile)->ltype == 'd'){
          tellinfo("?This is a directory, only files can be printed");
          return;
        }
        telluser("Printing...");
        strcpy(tmp,PR); strcat(tmp," ");
        getCurfileName(endof(tmp),Curfile);
        n=doShell(tmp);
        if(n>>8){
          sprintf(tmp,"?Couldn't find program '%s'",PR);
          tellinfo(tmp);
        }
        else
          tellinfo("Print job submitted");
}

static void
clearLastLine() {
        ceol(0,Tscrlen);
}

static void
printLastLine(msg) char *msg; {
        clearLastLine();
        printf("%s",msg);
}

static void
typein(argv)
/* puts arg chars into input buffer, i.e., fakes typing them in */
char *argv;
{
#if defined(TIOCSTI)
        while(argv[0]){
                ioctl((int)fileno(stdin), TIOCSTI, argv++);
        }
#else
       telluser("pushd not implemented"); sleep(2);
       argv[0];
#endif
}

static void
stuffPushdDirname() {
char *cp;
char tmp[MAXLBUFF];
char tmp2[MAXLBUFF];
    getCurfileName(tmp,Curfile);
    if(dotname(FNAME(Curfile))==0){
      if((cp=(char *)strrchr(tmp,'/')) != (char *)0) cp[0]=0;
    } else strcpy(tmp,fullpath);
    sprintf(tmp2,"pushd %s\n",tmp);
    typein(tmp2);
}

static void
suspendDired(SIG) int SIG; {  /* 0=default, 1=SIG_IGN */
        ceod(0, Tscrlen-2);
        fflush(stdout);     /* Keep screen update in sync /w signals */
        OFFsignals(SIG);
        kill(0,SIGSTOP); /*** BEEBE 12/95. Fix infinite loop. Was SIGTSTP. ***/
        ONsignals();        /* wake up here ... */
        setdpy();
        redraw();
        showStats(0);
        tellinfo("Help available - h or ?. Use q to quit.");
        fflush(stdout);
}

static void
ONsignals(){
          sigint=0;
          signal(SIGINT, catchint);   /* Grab ctrl-C */
          signal(SIGQUIT, SIG_IGN);
          signal(SIGTSTP, onctlz);
#if defined(SIGWINCH)
          signal(SIGWINCH, win_change);
#endif
}

static void
OFFsignals(SIG) int SIG; {      /* if(SIG == 1) SIG_IGN, else SIG_DFL */
          signal(SIGINT, SIG_DFL);
          signal(SIGQUIT, SIG_DFL);
          signal(SIGTSTP,(SIG ? SIG_IGN : SIG_DFL));
#if defined(SIGWINCH)
          signal(SIGWINCH, SIG_DFL);
#endif
}

static void
restoreCursor(){
        curxy(CURSORcolumn,curline);
}

static void
initFileList(){
int i;
struct lbuf *p;
    /* Get more FileList space or abort */
    p = (struct lbuf *)core((int)sizeof(struct lbuf)*MAXcountFILES);
    /* Copy old structure into new buffer */
    if(oldMAXFILEScount)
      memcpy((void *)p, (void *)filep, sizeof(struct lbuf)*oldMAXFILEScount);
    if(filep != 0) free((void *)filep); /* Free the space */
    filep=p;                            /* Set new pointer */
    /* Initialize new file pointers to zero for later call to malloc() */
    for(i=oldMAXFILEScount;i<MAXcountFILES;++i) FNAME(i)=(char *)0;
}

static void
FreeFileList(){
int i;
#if 1
    /* Free the strings in struct lbuf */
    for(i=0;i<totfiles;++i) {
      free((void *)FNAME(i));
    }
    /* Free the structure itself */
    free((void *)filep);
    filep=0;
#endif
}


struct Format { char key; int *var;};
static struct Format Fmt[] = {
{'i', &iflg},
{'b', &bflg},
{'m', &mflg},
{'u', &uflg},
{'g', &gflg},
{'s', &sflg},
{'d', &dflg},
{'p', &pflg},
{'l', &lflg},
{'n', &nflg}
};
#define FmtSIZE (sizeof(Fmt)/sizeof(Fmt[0]))

static void
setScreenFormat(setfmt) char *setfmt; {
int i;
char tmp[MAXLBUFF];
    if(setfmt == (char *)0){
      tellinfo("Set screen height 2 to 255. Abbreviations: h=half, f=full");
      i=getline(tmp,"Width: ");
      if(tmp[0]!=0){
                if(tmp[0]=='h') {splitflg=1; cmdLineTscrlen = 999;}
                else
                if(tmp[0]=='f') cmdLineTscrlen=splitflg=0;
                else {
                  i=atoi(tmp);
                  if(i>1 && i<256) cmdLineTscrlen = i;
                }
                windsize();                     /* Set scrlen */
                setTopfile(Curfile);
                redraw();
                curline= Curfile - topfile;
      }
      tellinfo("Enter display format keys, i=inode, b=block, m=mode, g=gid, u=uid,");
      i=getline(tmp,"s=size, d=date, p=path, l=long, n=none [ibmgusdpln]: ");
      if(i!=1 || tmp[0]==0) return;
    } else
    strcpy(tmp,setfmt);
    for(i=0;i < FmtSIZE;++i) Fmt[i].var[0]=0;
    for(i=0;i < FmtSIZE;++i)
      if((char *)strchr(tmp,Fmt[i].key) != (char *)0) Fmt[i].var[0]=1;
    /* Default user/group for lflg==1 is uflg==1 */
    if(!uflg){ if(!gflg && lflg) uflg=1;}
}

static void
doSort(){
int i;
    if(didStats==0 && sortyp != 0 && (char *)strchr("nei",sortyp)==(char *)0){
      clearLastLine();
      printf("Stat() on %d files: ",totfiles);
      for(i=0;i<totfiles;++i){
        getStats(i); /* Does nothing if stats were done */
        if((i/20)*20 == i) {printf("."); fflush(stdout);}
      }
      didStats=1;
    }
    if(sortyp)
      qsort(file(0), totfiles, sizeof(struct lbuf), compar);
}

static void
redraw(){
int row,n;
    showscreen();
    n=topofLowerScreen();
/*    if(splitflg) ++n; */
    for(row=n; row<Tscrlen-1; ++row) ceol(0,row);
}

/* Returns 0 for normal functions */
/* Returns QuitNORMAL==1, QuitERROR==2, QuitABORT==3 */
static int
dired(tmp) char *tmp; {
char *cp;
int diredstat;
char tmp2[MAXLBUFF];
char tmp3[MAXLBUFF];
struct lbuf *f;
char *errmsg;
int errcode;
int whichkind;
char *argv[2];

    camedown=errcode=0;
    getCurfileName(tmp2,Curfile);
    if(command == UPDIR) {
       /* if we got here by going down, then 'E' should quit */
       if(camedown) goto quit;
       /* If the directory name is a link, then resolve the link first,
          because we want to follow the tree from this point. */
       if(lstat(tmp2, &statbuf) < 0){
         errmsg="Can't find target"; goto errexit;
       }
       if( (statbuf.st_mode & S_IFMT) == S_IFLNK)
         getLink(tmp2); /* Links can be files or dirs */
       /* Form the full path name of the upper node */
       dcanon(tmp2);
       if( (statbuf.st_mode & S_IFMT) != S_IFDIR) {
         cp=(char *)strrchr(tmp2,'/');
         if(!cp){errmsg="Path syntax error"; goto errexit;}
         cp[0]=0;
       }
       strcat(tmp2,"/..");      /* Go up one dir in the tree */
       dcanon(tmp2);
       if(tmp2[0]==0) strcpy(tmp2,"/"); /* 1/96 GBG */
       goto checklink;
    }
    if (command == GOTODIR) {
       tellinfo("Goto Directory. Enter full path name, eg, /usr.");
       getline(tmp2, ": ");
       tellinfo("");
       if(tmp2[0]==0) goto exitnormal;
       dcanon(tmp2);
       whichkind=0;
       goto checklink;
    } /* end command == GOTODIR */
    if (command == EDITFILEDIR ) {
checklink:
      if(lstat(tmp2, &statbuf) < 0){
        errmsg="Can't find target"; goto errexit;
      }
      if( (statbuf.st_mode & S_IFMT) == S_IFLNK)
        getLink(tmp2); /* Links can be files or dirs */
      /* See if the file is really there */
      /* If it's a directory then switch from EDITOR to dired. */
      if(stat(tmp2, &statbuf) < 0){  /* Something wrong */
        errmsg="Can't find target"; goto errexit;
      }
      /* Don't fork unless a valid target has been selected. */
      switch(statbuf.st_mode & S_IFMT){
        case S_IFREG: camedown=0; whichkind=1;
          /* A file can be edited */
          break;
        case S_IFDIR:          /* A directory. Change to dired. */
          whichkind=0; camedown=1;
          /* Going down the tree */
          if(eq(tmp2,"/") && eq(fullpath,"/")) {
             errmsg= "?Already at root '/'"; goto errexit;
          }
          break;
        default: /* Can't edit */
          errmsg="Not a file or directory";
          goto errexit;
      }
    } /* End command == EDITFILEDIR */
    errcode=0;
    if(whichkind == 1){
      cp = getenv("EDITOR");
      if(cp == NULL || cp[0] == 0) cp="vi";   /* crh: Default vi */
      sprintf(tmp, "%s %s",cp,tmp2);
      doShell(tmp);
    } else {
      argv[0]=0; argv[1]=tmp2;  /* Be careful here. Why only one arg? */
      errcode=diredMain(2, argv);
      if(errcode==QuitNORMAL) errcode=0;
      if(errcode==QuitERROR) errcode=0;
    }
    if(errcode != QuitABORT){
      redraw();
      telluser("");
    }
    return errcode;     /* QuitABORT quits all recursive dired's */
errexit:
   telluser(errmsg);
   sleep(2);
exitnormal:
   return 0;
quit:
   return QuitNORMAL; /* Return 1 to exit calling dired */
}

static void
getRETURN(){
    printf("\nRETURN to continue: ");
    getchar();
}

/* Setup lbuf file structure with default values */
static void
setFileDefaults(totalfiles) int totalfiles; {
register struct lbuf *p;
    p=file(totalfiles);
    p->lsize = (long)-1; /* -1 means not gstat'ed yet. */
    p->lnum = 0;
    p->deleted = NOTDELETED;
    p->lflags = 0;
    p->ltype = '-';
}

static void
Quit(exitcode)
int exitcode;
{
#if 0
 logger(1);             /* log commands to a file, debug */
#endif
    unsetdpy();         /* Set terminal sane */
    OFFsignals(0);      /* All signals previously set are turned off */
    if(exitcode == -1) exit(0);
    ceod(0,Tscrlen-2);  /* Set cursor at bottom of lower screen */
    UnSetcap();         /* Turn off application pad mode */
    exit(exitcode);
}

/* Get command line args from file $HOME/.diredset */
static void
getdiredset() {
char *p;
FILE *fp;
char s[MAXLBUFF];
char *argv[2]; int argc;
    if((p=getenv("HOME")) == (char *)0) p=".";
    sprintf(s,"%s/.diredset",p);
    fp=fopen(s,"r");
    if(fp!=(FILE *)0){
      while(feof(fp)==0){
        fgets(s, MAXLBUFF-1, fp);
        p=(char *)strrchr(s,'\n'); if(p!=(char *)0) p[0]=0;
        p=s;
        while(p[0]!=0 && (char *)strchr(" \t",p[0])!=(char *)0) ++p;
        if(p[0]==0 || p[0]!= '-') continue;
        argc=2; argv[0]=""; argv[1]=p;
        processArgs(argc,argv);
      }
      fclose(fp);
    }
}

static void
getDIREDSETenv(){           /* Get options from DIREDSET env var */
int argc; char *argv[2];
char *p;
    if((p=getenv("DIREDSET")) == (char *)0) return;
    while(p[0]){
      p=(char *)strchr(p,'-');
      if(p == (char *)0) break;
      argc=2; argv[0]=""; argv[1]=p;
      processArgs(argc,argv);
      ++p;
    }
}

static void
processDIREDSET(){

/* Set up the builtin shell commands */
    newcore(&CP,"cp -p -i");            /* Copy command*/
    newcore(&MV,"mv -i");               /* Move command*/
    newcore(&RM,"rm -rf");              /* Remove command */
    newcore(&GR,"groff -s -p -t -e -Tascii -mandoc");   /* Groff command */
    newcore(&CH,"chmod -f");            /* chmod command */
    newcore(&PR,"lpr");                 /* print command */

/* Define aliased shell commands */
    setAlias("c=cp -p %n %p");          /* Predefined shell command */
    setAlias("r=rm -f %n");             /* predefined shell command */
    setAlias("m=mv %n %p");             /* predefined shell command */
    setAlias("u=gzip -d %n");           /* predefined shell command */
    setAlias("z=gzip %n");              /* predefined shell command */
    setAlias("l=ln -s %n %p");          /* Predefined shell command */
    setAlias("L=ln %n %p");             /* Predefined shell command */

    getdiredset();              /* Get options in file $HOME/.diredset */

    getDIREDSETenv();           /* Get options from DIREDSET env var */

}

static void
changeTags() {
int x,y,i,j,fmark;
int status;
char tmp[MAXLBUFF];
    tellinfo("Tag swap. Use ? for any tag.");
    status=getline(tmp,"Old tag: ");
    if(status==2) tmp[0]=0;
    x=tmp[0];
    if(x == 'd') x = 'D';
    if(x < 32 || x > 126){
      tellinfo("Tag swap aborted"); return;
    }
    sprintf(tmp,"Changing file tags '%c' to a new tag.",x);
    tellinfo(tmp);
    status=getline(tmp,"New Tag Char: ");
    if(status==2) tmp[0]=0;
    y=tmp[0];
    if(y == 'd') y = 'D';
    if(y < 32 || y > 126 || y == '?'){
      tellinfo("Flag change aborted"); return;
    }
    printf("%c",y);
   for(j=i=0;i<totfiles;++i){
     if(x == '?' || file(i)->deleted == x) {
        if(setFileMark(y,i) != x) ++j;
     }
   }
   if(j) redraw();
   sprintf(tmp,"Number of tags '%c' changed to tag '%c': %d",x,y,j);
   tellinfo(tmp);
}

static void
setTopfile(fnum)
int fnum;
{
     topfile = (fnum/(scrlen - 1)) * (scrlen - 1);
     /***crh 2/84 get rid of 1 file final windows unless necessary ***/
     if((topfile == totfiles - 1) && (topfile - scrlen + 1 >= 0))
       topfile = topfile - scrlen + 1;
}

static void
writeFileList(){
char s[MAXLBUFF];
char tmp[MAXLBUFF+128];
FILE *fp;
int i;
int len;

      strcpy(s,"/tmp/diredXXXXXX");
      mktemp(s);
      if((fp=fopen(s,"w"))==(FILE *)0){
        sprintf(tmp,"Write to %s failed -- bad open.",s);
        goto quit;
      }
      for(i=0;i<totfiles;++i){  /* Don't print escape codes */
        getEntry(i,tmp,&len); fprintf(fp,"%s\n",tmp);
      }
      fclose(fp);
      sprintf(tmp,"Write to %s completed.",s);
quit: tellinfo(tmp);
}

static void
setAlias(s) char *s;            /* Set up aliased shell cmd with keymap */
{
int i;
char t[3];
struct aliascommands *tail;
  if(s[1]!='='){                /* Parse for equal sign at s[2] */
    t[0]=s[0]; t[1]=s[1]; t[2]=0;
    /* newcore() needs first arg == 0 otherwise free() gets called */
    if(strcmp(t,"CP")==0) newcore(&CP,s+3); /* Re-define the command */
    if(strcmp(t,"MV")==0) newcore(&MV,s+3); /* Re-define the command */
    if(strcmp(t,"RM")==0) newcore(&RM,s+3); /* Re-define the command */
    if(strcmp(t,"CH")==0) newcore(&CH,s+3); /* Re-define the command */
    if(strcmp(t,"GR")==0) newcore(&GR,s+3); /* Re-define the command */
    if(strcmp(t,"PR")==0) newcore(&PR,s+3); /* Re-define the command */
    return;
  }
  t[0]=s[0]; t[1]=0;            /* One-character key */
  tail = aliaslist;             /* See if it is already defined */
  while(tail != 0 && tail->next != 0){
    if(strcmp(tail->key,t)==0){
      tail->key=tail->cmd=0;
      newcore(&tail->cmd,s+2); return;
    }
    tail=tail->next;
  }
  /* Make a new place for it */
  tail = (struct aliascommands *) core(sizeof (struct aliascommands));
  tail->key=tail->cmd=0;
  newcore(&tail->cmd,s+2);
  newcore(&tail->key,t);
  /* Add to the head of the list, so most recently used is first.  */
  tail->next = aliaslist;
  aliaslist = tail;
}

static int
testquit() {
  printLastLine("Press q to quit or RETURN for more: ");
  return ((getchar()=='q') ? 1 : 0);
}

static void
aliasHelp(){
char *p;
struct aliascommands *tmplist;
int x,status;
char tmp[MAXLBUFF];
  telluser("Substitutions: %n=name, %f=tail, %d=dir, %p=prompt, %%=%");
  if(testquit()) return;
  telluser("Sample shell command: cd %d;cp -p -i %f %p");
  if(testquit()) return;
  tmplist = aliaslist;
  while(tmplist != 0 && tmplist->next != 0){
    sprintf(tmp,"%s --> %s\n",tmplist->key,tmplist->cmd);
    tmplist=tmplist->next;
    telluser(tmp);
    if(testquit()) break;
  }
}

static void
saveAlias(c,s) int c; char *s; {
char *p;
struct aliascommands *tmplist;
char tmp[MAXLBUFF];
int x,status;
char buf[2];
  tmp[0]=c; tmp[1]='='; strcpy(tmp+2,s);
  setAlias(tmp);
}

static int
aliasGet(s,c) char *s; int c; {
char *p;
struct aliascommands *tmplist;
char buf[2];
  buf[0]=c; buf[1]=0;           /* 1-char string to match */
  p=0; tmplist = aliaslist;
  while(tmplist != 0 && tmplist->cmd != 0){
    if(strcmp(tmplist->key,buf)==0){strcpy(s,tmplist->cmd); return 0;}
    tmplist=tmplist->next;
  }
  return 1;     /* 1=error, 0=success */
}



static int
processArgs(argc,argv)
int argc; char *argv[]; {
int i,n;
int errcode;
char *p;
    errcode=0;
    for(i=1; i<argc; i++) {
        if(argv[i][0] != '-')
            break;
        switch(argv[i][1]) {
          case 'h':
          case 'H':
          case '-':
          case 'v':                     /* program version */
            printf("%s Version %s\n", argv[0], VERSION);
            help();
            errcode= -1;
            goto quit;
          case 'p':                     /* Pause on system commands */
            pause_return=1;
            break;
          case 'w':                     /* Window size */
            /* Assume split window, splitflg != 0 */
            if(argv[i][2] == 'h')
                cmdLineTscrlen = 999;           /* Half */
            else if(argv[i][2] == 'f')
                splitflg=cmdLineTscrlen = 0;    /* Full */
            else {
                cmdLineTscrlen = atoi(&argv[i][2]);
            }
            break;
          case 's':                     /* Initial sort order */
            sortyp = argv[i][2];
            rflg = 1;
            break;
          case 'r':
            sortyp = argv[i][2];
            rflg = -1;
            break;
          case 'C': /* Set initial file capacity and increment */
            n=atoi(&argv[i][2]);
            if(n>0) MAXallocFILES=MAXcountFILES=n;
            break;
          case 'S': /* Stat() on demand only */
            slowstat=0; break;
          case 'O':  /* Toggle OFF the pathname and stat display */
            statFlag=0;
            break;
          case 'o': /* Set ANSI colors (see ANSICOLOURS) */
            ansiColors=atoi(&argv[i][2]);
            break;
          case 'F': /* Format setup for the screen display. */
                    /* Or private dired FORK switch if no arg */
            p = (char *)&argv[i][2];
            if(p[0]==0) quitok=False;
            else newcore(&setfmt, p);
            break;
          case 'd': /* Private dired switch */
            camedown = True;
            break;
          case 'f':   /* get the start file */
            strcpy(startfile,&argv[i][2]);
            break;
          case 'D':   /* Define a shell cmd */
            setAlias(&argv[i][2]);
            break;
          default:
            printf("Unknown option %s. Use -h for help.\n", argv[i]);
            fflush(stdout);
            errcode= -1;
            goto quit;
         }
    }
quit:
    if(errcode==0) errcode = i;
    return errcode;
}

#if 0                                   /* Log commands to a file, debug */
static void logger(n) int n; {
static FILE *fp=0;
char buf[256];
  if(n){
    fclose(fp); fp=0; return;
  }
  if(fp==0) {
    fp=fopen("/tmp/logger.dat","a");
    fprintf(fp,"dired started\n");
  }
  decodekey(command,buf);
  fprintf(fp,"%s\n",buf);
}
#endif

static void
getcountarg()
{
  countarg = 0;
  while(ISDIGIT(command)) {
    countarg = 10 * countarg + command - (int)'0';
    command = getchar();
  }
}

static void
getkey(flag) int flag; {       /* Command keys, meta-keys, function keys */
int i;
int tmp;
        command=getchar();
        if(flag && ISDIGIT(command)) {
          tellinfo("Enter ESC ESC to cancel count");
          getcountarg();
        }
  command=getkeymatch(command,GNUkeysSIZE);
#if 0
logger(0);      /* Log commands to a file, debug */
#endif
}

static int
ustrncmp(s,t,n) char *s,*t; int n; {
  while(n-- && s[0]==t[0]){ if(!s[0] || !n) return 0; ++s; ++t;}
  return ((unsigned char)s[0]-(unsigned char)t[0]);
}

static
int getkeymatch(cc,MAXKEYS)
int cc; int MAXKEYS; {
#define MKLEN 20
static int nchars;
char buf[MKLEN+2];
int lo,hi,i,j,c,flg;
int ustrncmp();

  nchars = 0; c=cc;
  while(1) {
    buf[nchars] = c;
    buf[++nchars] = 0;
    if(nchars < MKLEN){
      flg = 0;
      hi = MAXKEYS-1; lo = 0;
      while(lo <= hi){    /* requires the keys to be sorted */
        i = (lo+hi)/2;
        if((j=ustrncmp(GNUkeys[i].Str,buf,nchars)) == 0){
          if(strlen(GNUkeys[i].Str)==nchars) return (GNUkeys[i].Code);
          flg=1; break;
        }
        if(j < 0) lo = i+1; else hi = i-1;
      }
      if(flg) {c=getchar(); continue;}
    /*  if(nchars == 1) return c; */
    }
    return -1;
  }
}

ustrcmp(s,t) char *s,*t; {
  while(s[0] && s[0]==t[0]){ ++s; ++t;}
  return ((unsigned char)s[0]-(unsigned char)t[0]);
}

static void
sorttable()
{
int n,gap,i,ig,j,t; char *tmp;
  n=GNUkeysSIZE;
  for (gap = n/2 ; gap > 0 ; gap /= 2)
    for (j = gap ; j < n; j++)
      for (i = j-gap ; i >= 0 ; i -= gap) {
              ig = i + gap;
              if (ustrcmp(GNUkeys[i].Str,GNUkeys[ig].Str) <= 0)
                      break;
              tmp=GNUkeys[i].Str; GNUkeys[i].Str=GNUkeys[ig].Str;
              GNUkeys[ig].Str=tmp;
              t=GNUkeys[i].Code; GNUkeys[i].Code=GNUkeys[ig].Code;
              GNUkeys[ig].Code=t;
      }
}

static void ansiBOLD(){
  if(ansiColors) printf("\33[01m");
}

static void ansiUNBOLD(){
  if(ansiColors) printf("\33[m");
}

#if ANSICOLOUR
/*
 * Attribute codes:
 * 00=none 01=bold 04=underscore 05=blink 07=reverse 08=concealed
 * Text color codes:
 * 30=black 31=red 32=green 33=yellow 34=blue 35=magenta 36=cyan 37=white
 * Background color codes:
 * 40=black 41=red 42=green 43=yellow 44=blue 45=magenta 46=cyan 47=white
*/

static struct {const char *code; int statcode;}
fileCode[]={
{"no", S_IFREG},          /* A regular file or anything else */
{"fi", S_IFREG },         /* A file */
{"di", S_IFDIR },         /* A directory */
{"ln", S_IFLNK },         /* A link */
{"pi", S_IFIFO },         /* A pipe */
{"so", S_IFSOCK},         /* A socket*/
{"bd", S_IFBLK },         /* A block device */
{"cd", S_IFCHR },         /* A char device */
{"ex", S_IEXEC }          /* An executable */
};
#define fileCodeSIZE (int)(sizeof(fileCode)/sizeof(fileCode[0]))
/* Standard colors from color LS slackware 1.2.8 and 1.2.13 */
static char *COLOURS=
"no=00:fi=00:di=01;34:ln=01;36:pi=40;33:\
so=01;35:bd=40;33;01:cd=40;33;01:ex=01;32:\
*.sh=01;32:*.cmd=01;32:*.exe=01;32:*.com=01;32:\
*.btm=01;32:*.bat=01;32:*.tar=01;31:*.tgz=01;31:\
*.arj=01;31:*.taz=01;31:*.lzh=01;31:*.zip=01;31:\
*.z=01;31:*.Z=01;31:*.gz=01;31:*.jpg=01;35:*.gif=01;35:\
*.bmp=01;35:*.xbm=01;35:*.xpm=01;35:*.tif=01;35:";

static char *MONOCHROME=
"no=00:fi=00:di=01:ln=04:pi=05:\
so=01:bd=07:cd=07:ex=01:\
*.sh=01:*.cmd=01:*.exe=01:*.com=01:\
*.btm=01:*.bat=01:*.tar=07:*.tgz=07:\
*.arj=01:*.taz=01:*.lzh=01:*.zip=07:\
*.z=01:*.Z=01:*.gz=07:*.jpg=01:*.gif=01:\
*.bmp=01:*.xbm=01:*.xpm=01:*.tif=01:";

/* Search through LS_COLOUR[] for a match, return color string. */
static char *colourSearch ARGS((const char *s));

static char *colourSearch(s)
const char *s; {
int n;
static char buf[128];
const char *p; char *q;
    sprintf(buf,"%s=",s);       /* Add equal sign to source */
    n=strlen(buf);
    p=LS_COLOUR;
    while(p[0]){                /* This should be fast enough */
      if(p[0]==buf[0] && strncmp(p,buf,n)==0) goto worked;
      ++p;
    }
    goto failed;
worked:
    p += n;                     /* Point to color string */
    q=buf;                      /* Destination buffer */
    while(p[0] && p[0]!=':') *q++ = *p++;
    q[0]=0;
    return buf;                 /* Pointer to static area */
failed:
    return "";  /* Return null string if not found */
}

/* Return ls-colors as an ansi string like "01;33" */
/* Return null string "" if no match at all. */
static char *colorString(name,mode) char *name; int mode; {
int i; char *p,*q;
    for(i=0;i<fileCodeSIZE;++i)
      if((mode & S_IFMT) == fileCode[i].statcode){
        p=colourSearch(fileCode[i].code);
        if(p[0] && strcmp(p,"00") != 0) return p; break;
      }
    q=(char *)strrchr(name,'.');
    if(q != (char *)0){
      p=colourSearch((const char *)q); if(p[0]) return p;
    }
    if((mode & ~S_IFMT & S_IEXEC) != 0) return colourSearch("ex");
    return ("");      /* Default: no color */
}

static void
lsColoursInit(cp) char **cp; {
char *p;
    if(ansiColors){
      p=getenv("LS_COLORS");
      if(ansiColors == 2 || (p==(char *)0 && ansiColors == 1)){
        p=(char *)COLOURS;       /* Default colors */
        cp[0]="Internal default colors";
      } else
      if(ansiColors == 3){
        p=(char *)MONOCHROME;     /* Default mono */
        cp[0]="Internal default mono";
      } else
      cp[0]="Environment colors";
      newcore(&LS_COLOUR,p);
    } else cp[0]="Colors OFF";
}
#else
static void
lsColoursInit(cp) char **cp; {cp[0]="Colors OFF";}
#endif

/* Experiment with keymap tutor. */

static void
keytostring(x,buf)
int x;
char *buf;
{
  buf[0]=0;
  if(x>127) {strcpy(buf,"Alt-"); x -= 128;}
  if(x < 32) {strcat(buf,"Ctrl-"); x += '@';}
  if(x==' ') strcat(buf,"SPACE"); else
  sprintf(endof(buf),"%c",x);
}

static void
decodekey(keycode,buf)
int keycode;
char *buf;
{
int i,x;
  for(x=i=0;i<GNUkeysSIZE;++i){
    if(GNUkeys[i].Code==keycode){
      if(strlen(GNUkeys[i].Str)==1) x=GNUkeys[i].Str[0];
      if('A'<= x && x <= 'Z') break;
      if('a'<= x && x <= 'z') break;
    }
  }
  keytostring(x,buf);
}

static void
decodeall(keycode,buf)
int keycode;
char *buf;
{
int i,j,x,n;
char keybuf[256];
unsigned char *p;
  buf[0]=0;
  for(i=0;i<GNUkeysSIZE;++i){
    if(GNUkeys[i].Code==keycode){
      if(buf[0]) strcat(buf,",");
      p=(unsigned char *)GNUkeys[i].Str;
      if(strlen(p)==1) keytostring(p[0],endof(buf));
      else {
        while(p[0]){
          x= *p++;
          if(x == '\033') strcat(buf,"\\E"); else
          sprintf(endof(buf),( (x<=32 || x >=127) ? "\\%o" : "%c"), x);
        }
      }
    }
  }
}

static void
diredKeyTutor(){
    int     x,i,k,n,r,line;
    int     Workbot,Worktop;
    char    buf[128];           /* Temp buffer for char expansion */
    char    keys[256];
    static int j=1;

    signal(SIGTSTP,SIG_IGN);
    printf("_");
    Worktop = topofLowerScreen();
    Workbot = (splitflg ? Tscrlen-1 : Tscrlen-2);
    clearLowerScreen();
    ceol(0, Worktop);
    ansiBOLD();
    printf("%s",helptxt[0].msg); /* Version of DIRED */
    ansiUNBOLD();
    n=helptxtSIZE;              /* Number of help items */
    r=Workbot-Worktop+1;        /* Number of lines to use */
    while(1){
repaint:
      line = j-r/2; if(line < 1) line = 1;
      for(k=0;k < r;++k){
        ceol(0,k+Worktop+1);
        if(k+line < n){
          decodekey(helptxt[k+line].token,buf);
          printf(" %-6s: :%s", buf,helptxt[k+line].msg);
        }
      }
      while(1){
        decodeall(helptxt[j].token,buf);
        clearLastLine();
        ansiBOLD();
        printf("Keymap: %s",buf);
        ansiUNBOLD();
        curxy(6+2,j-line+Worktop+1);
        sigint=0;
        getkey(0);         /* command */
        if(sigint) command=QUITDELETE;
        if(command == DOUBLEESC) goto quit;    /* ESC ESC */
        if(command == -1){ fprintf(stderr,"\007"); continue;}
        if(command == NEXTFILE){
          if(j==n-1) continue;
          ++j;
          if(j-line < r-1) continue;
          else break;
        }
        if(command == PREVFILE){
          if(j==1) continue;
          --j;
          if(line <= j) continue;
          else break;
        }
        if(command == ESCRETURN) {     /* ESC-RETURN */
          command = helptxt[j].token; goto quit;
        }
        for(i=1;i<n;++i){
          if(helptxt[i].token == command){
            if(line<=i && i<line+r-1){j=i; continue;}
            j=i; goto repaint;
          }
        }
      } /* End while(1) */
    }
quit:
/*    clearLowerScreen(); */
    printEntry(Curfile,curline);
    signal(SIGTSTP,onctlz);  /* Restore ctrl-z function. */
    sigint=0;                /* Forget about past ctrl-C's */
    return;
}

/* Ideas below came from: */
/* /usr/local/gnu/src/fileutils-3.12/fileutils-3.12/lib/idcache.c */

struct userid
{
  union
    {
      uid_t u;
      gid_t g;
    } id;
  char *name;
  struct userid *next;
};

/* Translate UID to a login name or a number string with cache.  */
static char *
Getuser (uid)
uid_t uid;
{
  static struct userid *user_alist;
  register struct userid *tail;
  struct passwd *pwent;
  char usernum_string[20];
  char *p;

  for (tail = user_alist; tail; tail = tail->next)
    if (tail->id.u == uid)
      return tail->name;

  pwent = getpwuid (uid);
  tail = (struct userid *) core(sizeof (struct userid));
  tail->id.u = uid;
  tail->name=0;
  if (pwent == 0)
      sprintf (p=usernum_string, "%u", (unsigned) uid);
  else
    p = pwent->pw_name;
  newcore(&tail->name,p);
  /* Add to the head of the list, so most recently used is first.  */
  tail->next = user_alist;
  user_alist = tail;
  return tail->name;
}


/* Use the same struct as for userids.  */

/* Translate GID to a group name or a number string with cache.  */
static char *
Getgroup (gid)
gid_t gid;
{
  static struct userid *group_alist;
  register struct userid *tail;
  struct group *grent;
  char groupnum_string[20];
  char *p;

  for (tail = group_alist; tail; tail = tail->next)
    if (tail->id.g == gid)
      return tail->name;

  grent = getgrgid (gid);
  tail = (struct userid *) core(sizeof (struct userid));
  tail->id.g = gid;
  tail->name=0;
  if (grent == 0)
    sprintf (p=groupnum_string, "%u", (unsigned int) gid);
  else
    p=grent->gr_name;

  newcore(&tail->name,p);
  /* Add to the head of the list, so most recently used is first.  */
  tail->next = group_alist;
  group_alist = tail;
  return tail->name;
}

static int
dotname (s)
char   *s;
{
  if(strcmp(s,".")==0) return 1;
  if(strcmp(s,"..")==0) return 1;
  return (0);
}


#if 0  /* Historical Notes */

/* Begin historical notes
================================ 1997 =================================
1. Chris Bagwell, cbagwell@fujitsu-fnc.com, got sun solaris 2.3/2.5
   to work [Fujitsu Network Communications, Richardson, Texas]. His edits
   have been added to the Makefile and to dired.c. The sun cc compiler
   caused some error messages with strchr() and strrch(), fixed by casting
   the trouble spots. 2 Jan 1997.
2. Swapped standard key "c" to "l" (REFRESHLINE) for use by COPYFILE.
3. Added COPYFILE and MOVEFILE. Makes it easy to copy and move files,
   without knowing how to use shell commands.
4. SYSTEMCMD. Added aliases, cooked shell commands, defined by syntax
   "-Dx=s" where x is a single char key and s is the command text (%
   substitution syntax allowed). Added default aliases in
   processDIREDSET(). See getdiredset(), aliasHelp(), aliasGet(),
   saveAlias().
5. New variables CP, MV, RM, GR, CH, PR. They select shell builtins. Set
   from the command line. Defaults are: "-DCP=cp -p -i", "-DMV=mv -i",
   "-DRM=rm -rf", "-DGR=groff -s -p -t -e -Tascii -mandoc",
   "-DCH=chmod -f", "-DPR=lpr".
6. Get rid of % and # as special characters in shell commands, replace
   with %%=%, %n=name, %f=file name only, %d=directory path only,
   %p=prompt (causes a prompt for target name). Changed function
   skipto() to reflect new methods.
7. Bug fix to TYPE. Left out file open validity check. Caused a seg fault
   if the file was not readable.
8. Added $HOME/.diredset. It keeps the command line options in a file,
   one line per option. See processDIREDSET().
9. Added sort on user 'u'. Useful for large tmp directories to find who
   owns trash files and remove selected ones.

================================ 1996 =================================
1/96 Major Rewrite by Grant Gustafson
1. Tagging command TAB to put '*' on files, used in '!' shell commands.
   Dired can be used to create shell files and to execute programs on
   a file list, eg, "!echo % >>foo.sh" collects tagged names in a file.
2. RegExp searches to put tags '*' and 'D' on files.
3. Cleanup of 'telluser' to use 2 lines and one argument. Prints
   on line=Tscrlen-1 and erases line=Tscrlen.
4. New function 'tellinfo' to print on line=Tscrlen-1.
5. Undelete 'U' removes all tags.
6. Shell command '!' applies to current file or all tagged files.
7. Prompt before and pause after shell commands (defeated by default).
8. Views saved only for gross cursor moves and searches.
9. Bookmarks use a smaller stack. Stack shrinks when full.
10. New help file. Integrated into command key tutor, Feb 1996.
11. Edited manual page extensively. Now a part of online help.
12. Modular sources for main program functions.
13. Added 'e', 'i' sort options for file extension, inode. Changed sort
    compar() to use raw sort option chars (nesrwc) instead of #define
    symbols, in an effort to simplify the logic of the qsort() call.
14. Empty directory displays didn't work right.
15. Search and shell command prompts were obscure.
16. The type commands 't' and 'T' did not work as claimed. Eliminated
    'T' command as it was useless on modern unix hosts.
17. File deletion done more carefully, with a chance to recover from
    unintentional deletes.
18. Typeahead code 'Q'. The idea is to stuff the typeahead keybuffer
    then exit(69). Normal code for this now appears in typein(), but
    it may not be implemented for certain unix hosts.
19. Removed obsolete, unused code (three short functions).
20. Removed dotname() and prname(). Removed all #ifndef V4P2 code
    because that code is more than 12 years old and untested. For
    a working dired in this case use 1995 source code 2.02. We have
    no public machines for testing such code, hence no maintenance.
21. Added fflush() to onctlz(), to keep display accurate.
22. Function getkey() handles and translates command input. Duplicate
    keys handled here also. More changes 8/96; see below.
23. Function getline() backspace error fixed. Changed number of args.
    Added ctrl-C processing. Changed getline() so it would display all
    chars, especially ctrl chars. No longer use backspace capability of
    the terminal for erase. Tabs are expanded on the screen. CR/LF ends
    input. Hex display "[%02x]" invoked for chars >= 127. Getline has 2
    args; sometimes used to wait for chars after an error.
24. Created functions to replace spaghetti code of the long switch
    statement in main(). This was a major project, as all code was
    written spaghetti-style.
25. Removed unused termcap variables. Only CM is needed. The string
    CE is simulated if not available. Functions insline() and
    home() removed because they were unused. Function ceod() was
    re-instated. If no TERM or termcap, then default to VT100 and
    supply the needed CM and CE vars internally (no termcap required).
26. File sub chars % and # can be passed to the shell as \% and \#.
    See function skipto(), keyword 'literal'.
27. Set tty.c_cc[VMIN] = 1; tty.c_cc[VTIME] = 1; because response times
    to TAB keys and cursor moves were lethargic (were 5, 2 resp).
28. Add 'z' command to suspend dired and pushd to current directory.
    This seems like a natural counterpart to 'Q', for browsing, or marking
    interesting places to return.
29. Dynamic alloc of file list structure and space for file names. The
    structure size is also dynamic, figured out at run time as the
    directory is actually read. The initial file capacity is set by
    MINFILES in the Makefile. Increases are done automatically in
    multiples of MINFILES. Only limited by memory available to malloc().
    Example: On SUN OS, a small directory and then a large directory
             loaded into dired shows these figures:
      Dired 3.00:       SZ    RSS         Dired 2.02:       SZ    RSS
                        76    340                         3028    340
                        136   396                         3028    492
    In short, dired 2.02 used 22 to 40 times more memory, on each and
    every copy of dired. On a SUN ultra, medium load, only 8 copies of
    dired 2.02 could be launched at once!
30. Set minimum file capacity (MINFILES==100 default) by command line
    switch -C, eg, -C250 sets min 250 files. Then file space alloc is
    250, 500, 750, etc, increased on demand as the dir is read.
31. Cleaned up type() so that hex is emitted for 127-255, ^A-^Z for CTRL.
    Eliminated nottext() test, since it may be worthwhile to type binary
    files just to find out what hex and ascii chars are in the file.
32. 1980 prtsplitstr() changed to lowScreenMessage().
33. Eliminated 'special' char code and function Getch(). Back to normal
    C library function fgetc(fp).
34. Function pentry(), which prints one line of file info, reworked to
    never run off the screen. New routine called getEntry(). Long file
    names truncated on the left end. Interactive setup and command line
    setup for amount of displayed info for files. See setScreenFormat()
    and the 'F' command SETFORMAT.
35. Screen refresh ^L also calls getWinSize() to set new window size.
    Useful if dired was suspended while the window size changed.
36. All dired command line options can be set by environment variable
    DIREDSET. Command line help with -v, -h, --.
37. Added window change logic for X, so that window resizing gets
    recognized on the next key-hit. See variable updateWinsize.
    Redid signal handling to handle forks (eg, to emacs) in a
    uniform way that does not conflict with cshsystem(). See 41 below.
38. Quick browser mode -S on startup delays lstat() calls until a
    screen display or sort requires them to be done. About a factor
    of 12 faster, which shows dramatic improvement for directories
    of 1200 or more files on slow nfs filesystems and cdrom.
39. Toggle vars for display of system info. Allow ctrl-C out of a fork
    to dired while read/stat on a long directory. Made setdpy() and
    unsetdpy() preserve order. Encapsulated signal handling. Fix all
    exits to insure signals OFF and keyboard input restored.
40. Eliminated function makename(), which was unused. Eliminated
    function bldnam(), which was duplicated by getCurfileName().
41. Integrated cshsystem() into dired.c, to keep code together.
    Then got rid of it completely, replacing it by system().
    All calls are to stdio via doShell() or system(). No execl.
42. Linux code for color display of file names, using the standard
    set by color "ls". Ideas of Steve Worcester implemented to swap
    in the environment variable LS_COLORS, if available. Also useful
    on monochrome systems, by setting the default colors to simple
    attributes like bold, underline, inverse video.
43. Command key tutor. Uses the lower half of the screen to inspect
    key actions and optionally execute the key. By using the tutor,
    dired teaches keystrokes and displays duplicate keys.
44. Help file "dired.hlp" eliminated. The internal help keys are:
    h == tutor, ? == man dired. The tutor can be expanded to serve
    the same purpose as "dired.hlp" (the tutor is always correct).
45. Command line options in help(). Documented also in manual page.
    Several were added to allow setup of new commands.
46. Had to dump getname(), because the cache was 1 user/group. Use a
    linked list of cached user/group ID's /w translation names. Get
    names with std unix functions getgrgid() and getpwuid(), instead
    of the 1980 method of a direct read of /etc/passwd and /etc/group.
47. Replaced 1980 stat() calls by lstat() and readlink(). Links are
    printed in the main display and translated on the lower screen
    information line, on demand only. The translation can be toggled
    on/off with the "O" command. See gstat() and showStats().
48. Added a convenience 'chmod' command for fixing protections of
    individual files. For a group of files, marking and using a shell
    command is more efficient.
49. March 1996. When dired is running, ctrl-C maps into SPACE, if
    keyboard stuffing is possible. In the main loop, this is translated
    into 'q', which is the quit key. If keyboard stuffing is impossible,
    then two ctrl-C's plus a third character are required. Compatibile
    with most unix programs that stop on ctrl-C.
50. May 1996. Keypad setup under linux is needed for X11R6, because the
    default setup leaves the PgUp/PgDn and arrow keys on the keypad
    mapped to numbers. Probably doesn't hurt under other flavors of unix.
    Added routines Setcap() and UnSetcap() to set/unset terminal keypad.

51. June 1996. Added 'X' and ctrl-W duplicate commands from Nelson
    Beebe's June 1996 edit of dired 2.03. NHFB: In Readdir(), do not
    skip ".." directory entry, so the dired listing now contains it,
    just like Emacs's dired. In 'd' (delete) and 'K' (delete all),
    ignore attempts to delete ".." directory.

52. July 1996.  By ccm@shentel.net. Add ESC-/ to go to a user-specified
    directory. Fix -d: was not being added for 'e' commands.  Fix linux
    check for se, so colors are on at startup. Fixed -wf weird stuff,
    found by Charles Moschel (ccm); it was a missing " ".

53. July 1996. Do away with system("dired ...") calls by a rewrite of
    main() using recursive function diredMain(). The basic idea is to
    save globals on entry to diredMain() and restore them on exit. An
    advantage to using one copy of dired() is that settable options
    like colors and screen size persist, along with shell commands.
    Memory savings are realized in most cases and system resources
    are used better than with the system() approach of 1980. Stack
    space issues are probably non-existent: most unix systems set the
    stack limit at 8mb; linux is unlimited. See also DiredLoop(), the
    main loop that calls subroutines based on keystrokes.

54. Bug fix: getlink(). It incorrectly translated links like foo.c -->
    foo.orig into $CWD/foo.orig. Standard of readlink() is to strip
    the path if it's the same as the source! Changed "rm -r" to
    "rm -rf" (suggested by Charles Moschel).

55. August 1996. Introduced key map table to handle regular and function
    keys. It is now possible to easily add and remove duplicate keys.
    This also improved spurious handling of unknown function keys.
    See GNUkeys structure, sorttable(), ustrncmp(), getkeymatch().
    Rework of the key tutor was required: see ustrcmp keytostring
    decodekey decodeall. Bug fix: "/./" and "//" are no longer displayed
    as targets. UPDIR and EDITFILEDIR perform as advertised (maybe). Add
    linked list of previous SYSTEMCMD strings, which can be recalled
    with UP and DOWN arrow key commands. Added "S" command to toggle
    slow read flag. Moved full/half screen setup to 'F' screen format
    command. Added VI keys ctrl-F and ctrl-B for VI compatibility. Fixed
    temp filename bug for 'w' command.

================================ 1995 =================================
12/95 by Nelson Beebe
1. Infinite loop in ^Z fixed. Changed SIGTSTP to SIGSTOP.

12/95 by Grant Gustafson
1. Connected directory code.
2. Memory use reduction from 3.0mb to 0.5mb on average use, by dynamic
   alloc of string size. (see also 1/96 above for further reduction)
3. Linux Makefile changes, code changes for linux.
4. Function keys and meta keys.
5. 'E' didn't allow climb to root '/'.
6. Groff manual page 'g' command.
7. PAGER command ('more'/'less') changed to use cshsystem(). But
   cshsystem() was eliminated in Feb 1996.
8. Plans to write version 3.00, a complete rewrite of dired.

6/95 by Nelson Beebe
1. Makefile (sun-solaris): In machine-specific targets, make $(TARGETS)
   instead of all.  Add new flags, and new sun-solaris-gcc target with
   extensive GCCFLAGS settings for maximal error checking. Put quotes
   around $(OPT) to allow embedded spaces. Add -DV4P2 for sun.
2. Add ARGS() macro and Standard C prototypes for all functions. Change
   many char* pointers to const char*.  Do some local variable renaming
   to reduce shadowing of global variables. Add explicit typing of all
   function arguments.  Use HAVE_xxx preprocessor symbols to control
   inclusion of header files or features that are not universally
   available. Rename TRUE/FALSE to True/False to avoid redefinition
   clashes with symbols in curses.h on several platforms.  Remove
   unused, and conflicting, typedef of bool.  Include additional header
   files for library function prototypes.  Make all file-local variables
   and functions static to reduce global name space pollution.  Change
   types of members lnl and lnum in struct lbuf from short to unsigned
   long, because modern file systems require that, and reorder the
   members to improve memory alignment.
3. Change non-standard varargs use of telluser() to fixed two-argument
   form.
4. Parenthesize some complicated expressions to improve readability.
   Change dired's exit code to errcode, instead of 0.  Revise to make
   dirent-style directory processing the default, since most machines
   have it now. Capitalize a few function names (Getcap, Getch, Readdir,
   Rmdir, Select) to avoid conflicts with library functions in system
   header files.  Fix some printf() format items and arguments to remove
   length mismatches.
5. Add wait_arg_t generalization for NeXT systems.
6. cshsystem.c: Add wait_arg_t generalization for NeXT systems.
7. regexpr.c, regexpr.h: Add these new files to provide a portable
   solution to regular-expression parsing. Include string.h for function
   prototypes. In Standard C environment, include stdlib.h instead of
   using explicit declarations of library functions.
   (re_compile_pattern()) : Add one typecast in comparison of int with
   unsigned char.
8. dired.c (main()): allow empty regular expression in search to mean
   re-use last one.
9. dired.c (main()): add VERSION and -v option. (getfullpath()): make
   temp[] static, because its address is returned (gcc 2.6.3 with -O4
   caught this long-standing bug).
10. (getline()): Recognize CR (\r) as end of line too; HP-UX returns \r
    instead of \n.
11. Reduce returned screen length by 1 to avoid use of status line on
    25-line displays (e.g. in kermit, only the upper 24 are cleared).
    Add input mappings of common ESCape sequences for arrow, PageUp, and
    PageDown keys.
12. (match()): Revise to use re_exec(), and add call to re_comp() in
    main() after /pattern has been collected.  This restores the
    long-missing filename pattern search capability in dired.
13. (getfullpath()): Try getenv() for "PWD" and "cwd" before calling
    getwd().

1/95 by Nelson Beebe
1. Add support for sorting by creation date.
2. Correct off-by-one error that made half-screen divider line wrap
   because it was too long.
3. Make some minor typographical corrections in dired.1, and clarify
   some if-then-else blocks in dired.c.

================================ 1993 =================================
Edits by Nelson H.F. Beebe

1. Add support for ~, #, D, and U commands.
2. Correct grammar in help file.
3. Remove a useless declaration in dired.c.
4. Reduce full-screen display size by 1 to avoid loss of line at top of
   screen.
5. Add Makefile support for DEC Alpha AXP OSF/1. Add OPT flag to all
   $(MAKE) lines in Makefile.
6. Increase size of userbuf[] in dired.c, so that long lines in
   /etc/passwd do not clobber data outside userbuf[] in getname().
   1996: userbuf[] eliminated.

================================ 1992 =================================
Edits by Nelson H.F. Beebe

1. Add support for compilation on IBM PS/2 and IBM 370 AIX systems.
2. New convenience targets: aixps2 aix370.
3. Add support for compilation under ANSI/ISO Standard C and POSIX
   terminal interface when BSD sgtty interface is unavailable, or
   incomplete (e.g. for Silicon Graphics).  New convenience targets:
   fips posix sgi stdc xpg3.
4. The Berkeley-style signal() returns the address of the current signal
   handler which is used in cshsystem() to restore signal handling; the
   ANSI/ISO Standard C and POSIX signal() does not return a value.
5. The choice between the two terminal interfaces is determined by the
   HAVE_TERMIOS_H symbol; when it is defined, ANSI/ISO Standard C and
   POSIX code (<termio.h>) will be used.  Otherwise, the Berkeley style
   (<sys/sgtty.h>) will be used.
6. Revise to make MOREPGM settable at compile time, as well as at run
   time from PAGER environment variable.
7. Merge in separate IBM RS/6000 version into master sources.
8. Cleanup Makefile and add convenience targets for specific systems.
9. Add Convex C1 and C2 support.
10. Put code under RCS control.

================================ 1991 =================================
Edits by Nelson H.F. Beebe
1. Off by one error in screen size.
2. Add code to use ioctl(...TIOCGWINSZ...)

================================ 1984 =================================
3/84 by Charles Hill (Unix Version 4.2)

1. "/" command, with reg expr pattern matching
2. Use of C shell in shell escapes
3. Commands to print the full pathname of the current file and the
   current node.  This is also done automatically on startup of any
   node.
4. Command to toggle between full and split screen modes.
5. Can now be used on any terminal with random cursor addressing.
6. Several bug fixes.
7. Some additional minor fixes & enhancements since the version posted
   by me in November 83.

1/84 by Charles Hill (crh)

1) show embedded control chars
2) eliminate 1 line windows except where necessary
3) cursor bug on deleting last file

================================ 1983 =================================
8/83 by Charles Hill (crh) *****

This version is shorter because of system routines opendir,
  readir, etc. and -r option of rm

================================ 1982 =================================
8/82 by Charles Hill (crh)
1. Added pattern matching in the form of /, n, and N commands.
2. Added toggle split-screen command \.
3. Added p command to print path name.
4. Change to "c" shell for ! command.
5. Fixed bug for single file arg when not a dir.
6. Added 'A', 'Q' commands and "linear window indicator".
7. Added counts to <up>, <down>, f, and b commands.
8. Added "G" as in "vi".
9. Added "E" command to go up a dir, and code to print node name.
10.Added "R" command to re-read dir.

6/82 by Charles Hill (crh)
                 Philips Labs
                 345 Scarborough Rd.
                 Briarcliff Manor, N.Y. 10510
                 philabs!crh

1. Eliminated need for terminal home capability. (Many termcap entries
   leave this out when there is cm). Changed body of home ()
   routine to a call to curxy(0,0).
2. Made default editor "vi".
3. On exit, screen is not blanked; cursor moves to last line. Line is
   cleared
4. In "type", only outputs 79 chars/line so terms with automatic margins
   will work ok.
5. Changed type () to handle tabs better.
6. Changed ceol to work for terms w/o clear to eoln. Single call to ceod
   eliminated. If no clear to eod, this is simulated. But it's better to
   use full screen on such terminals.
7. Changed type () to check if a file is a text file first. On some
   terminals, outputting arbitrary binary can put them in a weird mode,
   and screw up the display.

================================ 1981 =================================
12/81 by Lepreau:

1. Fix up aborting of command escapes, with mildy tricky re-display.
2. Make default window size be half-screen (2 window mode).
   Added option -wf to get full screen.
3. Add check for too many files to avoid core-dump.
4. Fix bug in cmd escapes: must reset caught signals before invocation.

10/8/81 by J.Lepreau, Univ of Utah:

   --bunch of stuff, including more cmds, 2 window mode,
   buffering output, pathname substitution in !escapes,
   initial sort options, escape from sorts, escape
   from !cmds, empty dir check.

================================ 1980 =================================
9/80 by Stuart Cracraft

Original version of dired

End Historical Notes */
#endif
