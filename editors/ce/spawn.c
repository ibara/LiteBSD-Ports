/*
 * This file is part of ce.
 *
 * Copyright (c) 1990 by Chester Ramey.
 *
 * Permission is hereby granted to copy, reproduce, redistribute or
 * otherwise use this software subject to the following: 
 * 	1) That there be no monetary profit gained specifically from 
 *	   the use or reproduction of this software.
 * 	2) This software may not be sold, rented, traded or otherwise 
 *	   marketed.
 * 	3) This copyright notice must be included prominently in any copy
 * 	   made of this software.
 *
 * The author makes no claims as to the fitness or correctness of
 * this software for any use whatsoever, and it is provided as is. 
 * Any use of this software is at the user's own risk.
 */
/*
 * spawn.c -- routines for subjobs and job control
 *
 * The routines in this file are called to create a subjob running a command
 * interpreter, or, if using a shell that understands job control, to 
 * temporarily exit back to that shell. This code is (unfortunately) 
 * BSD-specific, but those are the only kind of machines we have around here.
 */
#include "ce.h"
#include <signal.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include "celibc.h"

typedef	int	ifunc();

extern char	*mktemp();

extern int	readfile();
extern int	finsert();
extern int	cansusp();
extern BUFFER	*bfind();

#ifndef X11
static char 	*shname;
static char 	*shellp;
#endif /* X11 */

static char	*nopipe = "cannot make pipe to shell command";
static char	*nodisp = "Cannot display shell command output";

#ifndef RETSIGTYPE
#define RETSIGTYPE	void
#endif

#define STREQ(a,b)	(((*a) == (*b)) && (strcmp((a),(b)) == 0))

/*
 * Suspend and go back to the shell if using a shell that understands job
 * control (csh, bash, ksh, etc.) on a system that supports it (BSD). 
 * Assume we have job control, but require that the suspend character
 * be defined to use it.  We create a subjob with a copy of the command
 * interpreter in it otherwise (old BSD sh or Sys 5).  When the command
 * interpreter exits, mark the screen as garbage so that you do a full
 * repaint. Bound to "^Z". 
 */
/*ARGSUSED*/
int
spawncli(f, n)
int	f, n;
{
#ifdef X11
	mlwrite("[Not available under X11]");
	return FALSE;
#else
        register int 	pid, wpid, omask;
	RETSIGTYPE	(*oqsig)(), (*oisig)();
	int		status;

	if (shellp == NULL) {
		shellp = getenv("SHELL");
		if (shellp == NULL)
			shellp = getenv("shell");
	    	if (shellp == NULL)
			shellp = "/bin/sh";         /* safest */
	}
	if (shname == NULL) {
		shname = strrchr(shellp,'/');
		if (shname)
			shname++;
		else
			shname = shellp;
	}

	(*term.t_color)(CTEXT);
	if (mpresf != FALSE)
		mlerase();
	movecursor(term.t_nrow-1, 0);
	(*term.t_flush)();
        if ((*term.t_close)() == FALSE) 
		return (FALSE);
#ifdef JOBS
	if (!explarg && cansusp()) {
#ifdef _POSIX_JOB_CONTROL
		sigset_t set, oset;

		sigemptyset(&set);
		sigemptyset(&oset);
		sigprocmask(SIG_BLOCK, &set, &oset);
#else
		omask = sigsetmask(0);
#endif
		(void) kill (0, SIGTSTP);
#ifdef _POSIX_JOB_CONTROL
		sigprocmask(SIG_SETMASK, &oset, (sigset_t *)NULL);
#else
		(void) sigsetmask(omask);
#endif
	} else
#endif
	{
		write(1, "\nExit this shell to return to ce\n", 33);
		oqsig = signal(SIGQUIT,SIG_IGN);
		oisig = signal(SIGINT, SIG_IGN);
		if ((pid = fork()) == 0) {             /* child */
			signal(SIGINT, SIG_DFL);
			signal(SIGQUIT, SIG_DFL);
			signal(SIGTERM, SIG_DFL);
	        	(void) execl(shellp, shname, "-i", (char *) NULL);
			perror(shellp);
			_exit(0);
		}
		while ((wpid = wait(&status)) >= 0 && wpid != pid)
			;
		(void) signal(SIGQUIT,oqsig);
		(void) signal(SIGINT,oisig);
	}
	sgarbf = TRUE;
	return ((*term.t_open)());
#endif /* !X11 */
}

static void
reap(child)
int	child;
{
	int	status, pid;

	while ((pid = wait(&status)) != child && pid != -1)
		;
}

static int
inputpipe(cmd, fn, bp, func)
char	*cmd, *fn;
BUFFER	*bp;
ifunc	*func;
{
	int	child, p[2];

	if (pipe(p) < 0) {
		(*term.t_beep)();
		mlwrite(nopipe);
		return(FALSE);
	}
	(*term.t_flush)();
#ifndef X11
        (*term.t_close)();
#endif

	child = fork();
	if (child == 0) {
		int	fd = open(fn, 0);

		if (fd == -1)
			_exit(127);
		dup2(fd, 0);
		close(fd);
		close(p[0]);
		dup2(p[1], 1);
		close(p[1]);
		execl("/bin/sh", "sh", "-c", cmd, (char *)NULL);
		_exit(127);
	} else if (child == -1) {
		close(p[0]);
		close(p[1]);
#ifndef X11
		(*term.t_open)();
#endif
		return(FALSE);
	} else {
		close(p[1]);
		fiosetfd(p[0], 0);
		(*func)(bp);
		reap(child);
		fioclose(p[0]);
	}
#ifndef X11
	(*term.t_open)();
#endif
	return(TRUE);
}

static int
readpipe(cmd, bp, func)
char	*cmd;
register BUFFER	*bp;
ifunc	*func;
{
	int	child, p[2];

	if (pipe(p) < 0) {
		(*term.t_beep)();
		mlwrite(nopipe);
		return(FALSE);
	}
	(*term.t_flush)();
#ifndef X11
        (*term.t_close)();
#endif
	child = fork();
	if (child == 0) {
		close(p[0]);
		dup2(p[1], 1);
		close(p[1]);
		execl("/bin/sh", "sh", "-c", cmd, (char *) NULL);
		_exit(127);
	} else if (child == -1) {
		close(p[0]);
		close(p[1]);
#ifndef X11
		(*term.t_open)();
#endif
		return(FALSE);
	} else {
		close(p[1]);
		fiosetfd(p[0], 0);
		(*func)(bp);
		reap(child);
		fioclose(p[0]);
	}
#ifndef X11
	(*term.t_open)();
#endif
	return(TRUE);
}

/*
 * Run a one-liner in a subjob.  Bound to "C-X !".  This puts the output in 
 * a buffer named "*Shell Command Output*" and pops to it like in wallchart().  
 */
/*ARGSUSED*/
int
spawn(f, n)
int	f, n;
{
	register BUFFER *bp;
        register int    s;
	char	cmd[NSTRING];

	if (explarg)		/* like GNU Emacs */
		return (insshoutput(f, n));

	if ((s = mlreply("Shell Command: ", cmd, NSTRING)) != TRUE)
		return (s);

	bp = bfind("*Shell Command Output*", TRUE, 0);
	if ((bp == NULL) || (bclear(bp) == FALSE)) {
		(*term.t_beep)();
		mlwrite(nodisp);
		return(FALSE);
	}

	s = readpipe(cmd, bp, readfile);
	if (s != TRUE)
		return(s);

	bp->b_mode |= MDVIEW;
	s = popbuftoscreen(bp);
	update();
	return(s);
}

/*
 * Run a shell command on the contents of the current region, and display the
 * output in the buffer "*Shell Command Output*".
 */
/*ARGSUSED*/
int
regionshellcmd(f, n)
int	f, n;
{
	register BUFFER *bp;
	char 	fn[NFILEN], cmd[NSTRING];
	int	s;

	if (f && n > 1)
		return (filterregion(f, n));
	if ((s = mlreply("Shell Command on region: ", cmd, NSTRING)) != TRUE)
		return (s);
	(void) strcpy(fn, "/tmp/ce.inXXXXXX");
	(void) strcpy(fn, mktemp(fn));
	if (dumpregion(fn) != TRUE)
		return (FALSE);

	bp = bfind("*Shell Command Output*",TRUE,0);
	if ((bp == NULL) || (bclear(bp) == FALSE)) {
		(*term.t_beep)();
		mlwrite(nodisp);
		unlink(fn);
		return(FALSE);
	}

	s = inputpipe(cmd, fn, bp, readfile);
	if (s != TRUE)
		return(s);

	(void) unlink(fn);
	bp->b_mode |= MDVIEW;
	s = popbuftoscreen(bp);
	update();
	return(s);
}

/*
 * Run a shell command on the contents of the region and replace the region
 * with its output.
 */
/*ARGSUSED*/
int
filterregion(f,n)
int	f, n;
{
	char 	fn[NFILEN], cmd[NSTRING];
	int	s;

	if ((s = mlreply("Filter for region: ", cmd, NSTRING)) != TRUE)
		return (s);
	(void) strcpy(fn, "/tmp/ce.inXXXXXX");
	(void) strcpy(fn, mktemp(fn));
	if (dumpregion(fn) != TRUE)
		return (FALSE);
	(void) killregion(FALSE, 1);	/* committed to replacing region */

	s = inputpipe(cmd, fn, curbp, finsert);

	(void) unlink(fn);
	return(s);
}

/*
 * Run the contents of the current buffer through a filter, and replace the
 * contents of the buffer with its output.
 */
int
filterbuffer(f, n)
int	f, n;
{
	int 	s;

	igotoeob();
	isetmark();
	igotobob();
	if ((s = filterregion(f,n)) != TRUE) 
		return s;
	s = gotobob(FALSE,1);
	return (s);
}

/*ARGSUSED*/
int
insshoutput(f, n)
int	f, n;
{
        char 	    cmd[NLINE];
        int 	    s;

	if ((s = mlreply("Shell Command: ", cmd, NLINE)) != TRUE) 
		return s;
	return (readpipe(cmd, curbp, finsert));
}
