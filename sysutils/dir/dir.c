/*
 3-Dec-85 06:53:01-MST,12847;000000000001
Return-Path: <fsbrn@BRL.ARPA>
Received: from BRL-VOC.ARPA by SIMTEL20.ARPA with TCP; Tue 3 Dec 85 06:52:41-MST
Date:     Tue, 3 Dec 85 8:51:36 EST
From:     "Ferd Brundick (VLD/LTTB)" <fsbrn@BRL.ARPA>
To:       Sue <TABRON@simtel20.ARPA>
cc:       Meself <fsbrn@brl-voc.ARPA>
Subject:  add DIR12.C to archives


Haah,

Frank Wancho told me that you are POC for the UN*X archives at
simtel20.  Enclosed below is my version of DIR.C which is based on
DIR11.C (which makes this one DIR12.C).  I added a compile-time flag
to allow DIR to be compiled under 4.2BSD or BRL UN*X on a VAX; it also
works under UTX-32 on our Goulds.  I also cleaned up the code a bit,
deleting some extraneous functions and speeding things up.  I've been
using DIR for several months, so I don't expect any major problems.
Modification details may be found near the beginning of the code.

                                        dsw, fferd
                                        Fred S. Brundick
                                        USABRL, APG, MD.
                                        <fsbrn@brl-voc.arpa>

===============================cut here===============================
*/
/*
 *  DIR by Richard Conn
 *
 * Revision History --
 *  DIR Version 1.2 -- Modified by Ferd Brundick
 *	1. Flags added for compilation under 4.2 BSD with the Berkeley
 *	    or System V compiler.  The default is PDP-11 System V UNIX.
 *	2. sort() function deleted.  File names are inserted into
 *	    linked list in correct position (list is always in order).
 *	3. directory() rewritten to use Berkeley directory functions.
 *	4. Size not displayed if file is a directory.  This may be
 *	    changed by setting the flag SHOWDIRSIZ to TRUE.
 *	5. Since Berkeley UNIX allows file names to have 255 chars,
 *	    the constant NAMOUT has been added.  The default value
 *	    is 14 chars (the PDP-11 max file name length).  If a file
 *	    name has more than NAMOUT chars, a plus '+' is displayed
 *	    in front of the file name.
 *	6. When the '-a' (attribute) option is chosen, the file
 *	    permissions shown depend on the user.  If he is the owner
 *	    of the file, then the Owner bits are checked; if not,
 *	    the Other bits are used.
 *	7. Data structures implemented with "typedef".
 *	8. Minor cosmetic changes have been made and new constants
 *	    have been added to make the program easier to read.
 *
 *  Known bugs:
 *	1. By default, DIR looks at the current directory.  It works
 *	    if you give it a series of data files.  It works if you
 *	    give it a single directory name.  It does NOT work if you
 *	    give it several dir names OR several data files and one or
 *	    more dir names.  I did not fix this bug because the display
 *	    wouldn't make any sense.  DIR generates all the file names
 *	    and displays them in alphabetical order, so there would be
 *	    no way of knowing which directory a file is in.
 *
 *  DIR Version 1.1 -- Modified by Frank Wancho
 *		Several changes made to provide operation with MORE
 *
 *	DIR is an enhanced directory display utility for UNIX.
 * It produces a sorted listing of file names and sizes in two columns,
 * and the files are sorted down the columns.  If a file is a directory,
 * its name is prefixed with a "D".  File count and size totals are
 * displayed at the bottom of the listing.
 *
 *  Compile line : cc -Dx dir.c -o dir
 *		    where 'x' = BSD
 *				BRL
 *		    -Dx is not required for PDP-11
 */

	/*  C Libraries  */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

	/* include VAX Berkeley or PDP-11 files */
#undef NotElse
	/* check for standard Berkeley 4.x */
#ifdef BSD
#define  NotElse
#define  BERKELEY
#include <strings.h>
#include <sys/dir.h>
#undef	 DIRSIZ 		/* delete Berkeley macro */
#define  DIRSIZ   MAXNAMLEN
#define  NAMOUT   14
#endif
	/* check for BRL (Berkeley System V) */
#ifdef BRL
#define  NotElse
#define  BERKELEY
#include <string.h>
#include <dir.h>
#define  DIRSIZ   MAXNAMLEN
#define  NAMOUT   14
#endif
	/* assume PDP-11 System V */
#ifndef NotElse
#undef	 BERKELEY
#include <strings.h>
#include <sys/dir.h>
#define  NAMOUT  DIRSIZ
#endif

	/*  Define miscellaneous constants */
#define  BUFSIZE  256
#ifndef TRUE
#define  TRUE	  (1)
#endif
#ifndef FALSE
#define  FALSE	  (0)
#endif
#define  BLANK	  ' '
#define  EOS	  '\0'
#define  NEWLINE  '\n'

	/*  Define full path of "more" command */
#define MORE	"/usr/bin/more"

	/*  Define permission flags for "Others" */
#define S_OREAD   4
#define S_OWRITE  2
#define S_OEXEC   1

	/*  Set directory size display flag */
#define  SHOWSIZDIR  FALSE

/* structure describing an open directory. */
typedef struct _dirdesc {
    int     dd_fd;      /* file descriptor associated with directory */
    long    dd_loc;     /* offset in current buffer */
    long    dd_size;    /* amount of data returned by getdirentries */
    char    *dd_buf;    /* data buffer */
    int     dd_len;     /* size of data buffer */
    long    dd_seek;    /* magic cookie returned by getdirentries */
    long    dd_rewind;  /* magic cookie for rewinding */
    int     dd_flags;   /* flags for readdir */
} DIR;

	/*  Structure of a Directory Element as Stored for DIR	*/
typedef struct dirnode {
	char  name[DIRSIZ+1];	/* Name of File */
	int   dir;		/* Dir or Not? */
	int   toolong;		/* Name too long or Not ? */
	int   isowner;		/* Is user Owner of file ? */
	off_t size;		/* Size in Bytes */
	short mode;		/* R/W/X */
	struct dirnode *next;	/* Ptr to Next File */
} DIR_NODE, *DIR_PTR;

	/*  Global Environment */
typedef struct environ {
	int   showhid;		/* Show Hidden Files? */
	int   showrwx;		/* Show Attributes? */
	int   filecnt;		/* Number of Files in List */
	off_t totsize;		/* Sum of All File Sizes */
	struct dirnode *first;	/* Ptr to First Elt in List */
} ENV_REC, *ENV_PTR;

	/*  Global Variables */
int  UserId;			/* ID of current User */
FILE *Out;			/* Pointer to output file or pipe */

	/*  Declare external functions */
FILE *popen();

DIR *opendir(const char *);
struct dirent *readdir(register DIR *);

main (argc,argv)

int  argc;
char *argv[];

{
	char	buf[BUFSIZE];
	ENV_REC env;
	int	i, first;

	/* Initialize Environment */
	env.first   = NULL;	/* No First Entry */
	env.showhid = FALSE;	/* No Hidden Files */
	env.showrwx = FALSE;	/* No R/W/X */
	env.filecnt = 0;	/* Number of Files */
	env.totsize = 0;	/* Accumulated File Size */

	/* Get ID of current User */
	UserId = getuid();

	/* Build Sorted Linked List of DIR Elements */
	if (argc == 1) build(".",&env,0);	/* current dir */
	else {
		first = 1;			/* first file is 1 */
		if (*argv[1] == '-') {
			first = 2;		/* first file is 2 */
			opts(argv[1],&env);
			if (argc == 2) build(".",&env,0);	/* curr dir */
		}
		for (i=first; i<argc; i++)	/* process each file */
			build(argv[i],&env,0);
	}

	/* Display Results */
	Out = popen(MORE, "w");
	display(&env);
	pclose(Out);
}  /* end of main */

opts (cmdstr, env)		/*  Process Command Options  */

char	*cmdstr;
ENV_PTR env;

{
	while (*cmdstr != EOS)
		switch (*cmdstr++) {
		case 'A' :
		case 'a' :
			env->showrwx = TRUE;	/* show file attributes */
			break;
		case 'H' :
		case 'h' :
			env->showhid = TRUE;	/* show hidden files */
			break;
		default :
			break;
		}
}  /* end of opts */

display (env)			/*  Display Entries in Linked List  */

ENV_PTR env;

{
	DIR_PTR lptr, rptr, elt();
	char	*fname;
	int	i;

	if (env->filecnt > 0) { 	/* print column 1 header */
		fprintf(Out, "  -- Filename --");
		for (i=0; i < NAMOUT-14; i++)
			fputc(BLANK, Out);
		fprintf(Out, "  - Size -");
		if (env->showrwx) fprintf(Out, "  Atr");
	}
	if (env->filecnt > 1) { 	/* print column 2 header */
		fprintf(Out, "  ");
		fprintf(Out, "  -- Filename --");
		for (i=0; i < NAMOUT-14; i++)
			fputc(BLANK, Out);
		fprintf(Out, "  - Size -");
		if (env->showrwx) fprintf(Out, "  Atr");
	}
	fputc(NEWLINE, Out);
	lptr = elt(0,env);		/* Pt to first element in left col */
	rptr = elt(env->filecnt%2 ? env->filecnt/2+1 : env->filecnt/2, env);
	for (i=0; i < env->filecnt/2; i++) {
		prelt(lptr,env);	/* Print Left Element */
		prelt(rptr,env);	/* Print Right Element */
		fputc(NEWLINE, Out);	/* New Line */
		lptr = lptr->next;	/* Pt to next */
		rptr = rptr->next;
	}
	if (env->filecnt%2) {
		prelt(lptr,env);	/* Print Odd Element */
		fputc(NEWLINE, Out);
	}
	fprintf(Out, "         -- %d Entries Displayed, %ld Bytes --\n",
		env->filecnt, env->totsize);
}  /* end of display */

prelt (ptr, env)		 /*  Print Element Pointed To  */

DIR_PTR ptr;
ENV_PTR env;

{
	char *fname;
	int j;

	if (ptr->dir) fputc('D', Out);		/* Print Dir Flag */
	else fputc(BLANK, Out);
	if (ptr->toolong) fputc('+', Out);	/* Print Length Flag */
	else fputc(BLANK, Out);
	fname = ptr->name;
	for (j=0; j < NAMOUT; j++)		/* Print File Name */
		if (*fname == EOS) fputc(BLANK, Out);
		else fputc(*fname++, Out);
	if (ptr->dir && !SHOWSIZDIR) fprintf(Out, "  %8s", "***");
	else fprintf(Out, "  %8ld", ptr->size);	/* Print File Size */
	if (env->showrwx)			/* Print RWX Flags */
		if (ptr->isowner)
			fprintf(Out, "  %c%c%c",
				(ptr->mode & S_IREAD)  ? 'r' : '-',
				(ptr->mode & S_IWRITE) ? 'w' : '-',
				(ptr->mode & S_IEXEC)  ? 'x' : '-');
		else
			fprintf(Out, "  %c%c%c",
				(ptr->mode & S_OREAD)  ? 'r' : '-',
				(ptr->mode & S_OWRITE) ? 'w' : '-',
				(ptr->mode & S_OEXEC)  ? 'x' : '-');
	fprintf(Out, "  ");
}  /* end of prelt */

DIR_PTR elt (n, env)  /* Point to Nth (Starting at 0) Element in Linked List */

int	n;
ENV_PTR env;

{
	DIR_PTR rover;
	int	i;

	rover = env->first;		/* pt to first element */
	for (i=0; i<n; i++) rover = rover->next;	/* adv thru list */
	return (rover);
}  /* end of elt */

build (name, env, level)  /* Build Linked List Containing Directory Entries */

char	*name;
ENV_PTR env;
int	level;

{
	struct stat stbuf;
	DIR_NODE dir;
	DIR_PTR  Here;		/* pointer to Current Entry */
	DIR_PTR  Prev;		/* pointer to Previous Entry */
	DIR_PTR  NewEnt; 	/* pointer to New Entry */
	char	 *nameptr;
	int	 i, ccnt;

	/* Check for File Existence */
	if (stat(name,&stbuf) == -1) {
		fprintf(stderr,"Can't find %s\n", name);
		return;
	}

	/* Check to See if File is a Directory */
	if ((stbuf.st_mode & S_IFMT) == S_IFDIR) {	/* we have a dir */
		directory(name,env,level);
		if (!level) return;
	}

	/*  Check Hidden Entries */
	if (*name == '.' && !(env->showhid)) return;

	/*  Allocate Memory for New Entry */
	if ((NewEnt = malloc(sizeof(dir))) == NULL)
	{
		fprintf(stderr, "Dynamic Memory Overflow\n");
		exit(0);
	}

	/* Insert new entry in correct position */
	Prev = NULL;
	Here = env->first;
	while (Here != NULL && strcmp(Here->name, name) < 0)
	{
		Prev = Here;
		Here = Here->next;
	}
	if (Prev == NULL)
		env->first = NewEnt;
	else
		Prev->next = NewEnt;
	NewEnt->next = Here;

	/* Store Entry Values */
	env->filecnt++; 		/* Increment File Count */
	nameptr = name; 		/* Pt to first char of file name */
	ccnt = strlen(name);		/* Number of chars in file name */
	for (i=0; i < ccnt; i++)	/* Find Last Part of Name */
		if (*name++ == '/') nameptr = name;
	strcpy(NewEnt->name,nameptr);	/* Save File Name */
	NewEnt->size = stbuf.st_size;	/* Save File Size */
	NewEnt->mode = stbuf.st_mode;	/* Save Mode Bits */
	/* Set Dir, Length, and Owner Flags */
	NewEnt->dir = (stbuf.st_mode & S_IFMT) == S_IFDIR;
	NewEnt->toolong = strlen(nameptr) > NAMOUT;
	NewEnt->isowner = stbuf.st_uid == UserId;
	if (!(NewEnt->dir) || SHOWSIZDIR)
		env->totsize += stbuf.st_size;	/* Increment Total Sizes */
}  /* end of build */

directory (name, env, level)	/* Process All Entries in a Directory */

char	*name;
ENV_PTR env;
int	level;

{
	char *nbp, *nep;
	char dirname[BUFSIZE];
	char filename[DIRSIZ+1];
	int i;

#ifdef BERKELEY
	struct direct *DirEnt;		/* pointer to directory entry */
	DIR	      *DirPtr;		/* pointer to directory stream */
#else
	struct direct dirbuf;		/* directory entry */
	int fd; 			/* directory file descriptor */
#endif

	if (level) return;		/* don't recurse */

	/* Build Name of Directory into DIRNAME */
	strcpy(dirname, name);

	/* Log Into Directory */
	if (chdir(dirname)) {
		fprintf(stderr, "Directory %s Not Found\n", dirname);
		return;
	}

#ifdef BERKELEY
	/* Open Directory File */
	if ((DirPtr = opendir(".")) == NULL) return;

	/* Read Through the Elements in the Directory */
	while ((DirEnt = readdir(DirPtr)) != NULL)
	{
		strcpy(filename, DirEnt->d_name);	/* get file name */
		build(filename,env,level+1);	/* reenter build for new file */
	}
	closedir(DirPtr);
#else
	/* Open Directory File */
	if ((fd = open(".",0)) == -1) return;

	/* Read Through the Elements in the Directory */
	while (read(fd, (char *)&dirbuf, sizeof(dirbuf)) > 0) {
		if (dirbuf.d_ino == 0)	/* slot not in use */
			continue;
		/* get file name */
		strncpy(filename, dirbuf.d_name, DIRSIZ);
		filename[DIRSIZ] = EOS;
		build(filename,env,level+1);	/* reenter build for new file */
	}
	close(fd);
#endif
}  /* end of directory */
/*
======================================================================
*/
