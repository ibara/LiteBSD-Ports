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
 * file.c -- ASCII file handling routines
 *
 * The routines in this file handle the reading and writing of disk files. 
 * All of details about the reading and writing of the disk are in "fileio.c".
 */

#include <stdio.h>
#include "celibc.h"
#include "ce.h"

/*
 * Forward declarations
 */
extern void rmsave();
extern void makename();

/*
 * Read a file into the current buffer. This is really easy; all you do is
 * find the name of the file, and call the standard "read a file into the 
 * current buffer" code.  Bound to "C-X C-V".
 */
/*ARGSUSED*/
int
fileread(f, n)
int	f, n;
{
	register int    s;
	register WINDOW *wp;
	char fname[NFILEN];
	char bname[NBUFN];

	if ((s = mlgetfile("Visit File: ", fname, NFILEN)) != TRUE)
		return(s);
	rmsave(curbp->b_fname);		/* remove auto-save files */
	if ((s = readin(fname)) != TRUE) /* too bad, we crushed the buffer */
		return(s);
	makename(bname, fname);
	strcpy(curbp->b_bname, bname);
	strcpy(curbp->b_fname, fname);
	curbp->b_mode = gmode;	   /* reset the modes for the buffer */
	if (cfile(fname))
		curbp->b_mode |= (MDCMODE|MDAUTOSAVE);
	wp = wheadp;
	curbp->b_nwnd = 0;	       /* reset the window count */
	while (wp != NULL) {
		if (wp->w_bufp == curbp) {
			curbp->b_nwnd++;
			wp->w_flag |= WFMODE;
		}
		wp = wp->w_wndp;
	}
	(void) bshuffle();		/* change the buffer list order if needed */
	return(TRUE);
}

/*
 * Insert a file into the current buffer. This is really easy; all you do it
 * find the name of the file, and call the standard "insert a file into the 
 * current buffer" code.  Bound to "C-X I".
 */
/* ARGSUSED */
int
fileinsert(f, n)
int	f, n;
{
	register int    s;
	char fname[NFILEN];

	if (curbp->b_mode&MDVIEW)	/* don't allow this command if	*/
		return(rdonly());	/* we are in read only mode	*/
	if ((s=mlgetfile("Insert file: ", fname, NFILEN)) != TRUE)
		return(s);
	return(insertfile(fname, FALSE));
}

/*
 * Select a file for editing.  Look around to see if you can find the
 * fine in another buffer; if you can find it just switch to the buffer.
 * If you cannot find the file, create a new buffer, read in the text,
 * and switch to the new buffer.  Bound to C-X C-F.
 */
/* ARGSUSED */
int
filefind(f, n)
int	f, n;
{
	char fname[NFILEN];	/* file user wishes to find */
	register int s;		/* status return */

	if ((s=mlgetfile("Find file: ", fname, NFILEN)) != TRUE)
		return(s);
	s = getfile(fname);
	update();
	return s;
}

/* 
 *  Visit a file in VIEW (read-only) mode.
 *  Bound to "C-X C-R".
 */
/* ARGSUSED */
int
fileview(f, n)
int	f, n;
{
	char fname[NFILEN];	/* file user wishes to find */
	register int s;		/* status return */
	register WINDOW *wp;	/* scan for windows that need updating */

	if ((s=mlgetfile("View file: ", fname, NFILEN)) != TRUE)
		return (s);
	s = getfile(fname);
	if (s) {	/* if we succeed, put it in view mode */
		curwp->w_bufp->b_mode |= MDVIEW;
		/* scan through and update mode lines of all windows */
		wp = wheadp;
		while (wp != NULL) {
			wp->w_flag |= WFMODE;
			wp = wp->w_wndp;
		}
	}
	return(s);
}

int
getfile(fname)
char fname[];		/* file name to find */
{
	register BUFFER *bp;
	register LINE   *lp;
	register int    i;
	register int    s;
	char bname[NBUFN];	/* buffer name to put file */

	for (bp=bheadp; bp!=NULL; bp=bp->b_bufp) {
		if ((bp->b_flag&BFTEMP)==0 && strcmp(bp->b_fname, fname)==0) {
			switchbuffer(bp);
			lp = curwp->w_dotp;
			i = curwp->w_ntrows/2;
			while (i-- && lback(lp)!=curbp->b_linep)
				lp = lback(lp);
			curwp->w_linep = lp;
			curwp->w_flag |= WFMODE|WFHARD;
			mlwrite("[Old buffer]");
			sgarbf = TRUE;
			return (TRUE);
		}
	}
	makename(bname, fname);		 /* New buffer name.     */
	while ((bp=bfind(bname, FALSE, 0)) != NULL) {
		s = mlreply("Buffer name: ", bname, NBUFN);
		if (s == ABORT)		 /* ^G to just quit      */
			return (s);
		if (s == FALSE) {	       /* CR to clobber it     */
			makename(bname, fname);
			break;
		}
	}
	if (bp==NULL && (bp=bfind(bname, TRUE, 0))==NULL) {
		mlwrite("Cannot create buffer");
		return (FALSE);
	}
	if (--curbp->b_nwnd == 0) {	     /* Undisplay.	   */
		curbp->b_dotp = curwp->w_dotp;
		curbp->b_doto = curwp->w_doto;
		curbp->b_markp = curwp->w_markp;
		curbp->b_marko = curwp->w_marko;
	}
	curbp = bp;			     /* Switch to it.	*/
	curwp->w_bufp = bp;
	curbp->b_nwnd++;
	return(readin(fname));		   /* Read it in.	  */
}

static void
setwin(bp)
BUFFER	*bp;
{
	register WINDOW *wp;

	for (wp = wheadp; wp!=NULL; wp=wp->w_wndp) {
		if (wp->w_bufp == bp) {
			wp->w_linep = lforw(bp->b_linep);
			wp->w_dotp  = lforw(bp->b_linep);
			wp->w_doto  = 0;
			wp->w_markp = NULL;
			wp->w_marko = 0;
			wp->w_flag |= WFMODE|WFHARD;
		}
	}
}

/*
 * Read file "fname" into the current buffer, blowing away any text found
 * there. Called by both the read and find commands. Return the final status 
 * of the read. Also called by the mainline, to read in a file specified on
 * the command line as an argument.
 */
int
readin(fname)
char    fname[];	/* name of file to read */
{
	register BUFFER *bp;
	register int    s;

	bp = curbp;			     /* Cheap.	       */
	if ((s = bclear(bp)) != TRUE)	   /* Might be old.	*/
		return (s);
	if (cfile(fname))
		bp->b_mode |= (MDCMODE|MDAUTOSAVE);
	bp->b_flag &= ~(BFTEMP|BFCHG|BFBAK);
	/*
	 * This loses if fname is longer than NFILEN characters.  I need
	 * to make b_fname a pointer instead of an array and use malloc.
	 * Someday.
	 */
	strncpy(bp->b_fname, fname, NFILEN-1);
	bp->b_fname[NFILEN-1] = '\0';

	if ((s = ffropen(fname)) == FIOERR) {	/* Hard file open.      */
		setwin(bp);
		goto out;
	}
	if (s == FIOFNF) {		      /* File not found.      */
		mlwrite("(New file)");
		setwin(bp);
		goto out;
	}
	if (s == FIORDONLY)
		bp->b_mode |= MDVIEW;
	mlwrite("[Reading %s...]",fname);
	readfile(bp);
	if ((s = ffclose()) != FIOSUC)
		s = FIOERR;			/* Don't ignore errors. */
out:
	if (s == FIOERR)			/* False if error.      */
		return(FALSE);
	sgarbf = TRUE;
#ifdef XTITLE
	if (onxterm) 
		writetitle(fname);
#endif /* XTITLE */
	return (TRUE);
}

int
readfile(bp)
BUFFER	*bp;
{
	register LINE   *lp1, *lp2;
	char		*line;
	int		nbytes, nline, s;

	nline = 0;
	bp->b_flag |= BFBAK;		/* Need a backup */
	while ((s = ffgetline(&line, &nbytes)) != FIOERR) {
		if (s == FIOEOF && nbytes == 0)
			break;
		if ((lp1 = lalloc(nbytes)) == NULL) {
			s = FIOERR;	/* Keep message on the display */
			break;
		}
		lp2 = lback(bp->b_linep);
		lp2->l_fp = lp1;
		lp1->l_fp = bp->b_linep;
		lp1->l_bp = lp2;
		bp->b_linep->l_bp = lp1;
		bcopy(line, ltext(lp1), nbytes);
		free(line);
		++nline;
		if (s == FIOEOF)
			break;
	}
	if (s == FIOEOF)
		s = FIOSUC;
	if (s == FIOSUC) {		/* Don't zap message! */
		if (nline == 1)
			mlwrite("(Read 1 line)");
		else
			mlwrite("(Read %d lines)", nline);
	} 
	setwin(bp);
	return(s);
}

/*
 * Take a file name, and from it fabricate a buffer name. This routine knows
 * about the syntax of file names on the target system.  Right now, this
 * means Unix.  I suppose that this information could be put in a better
 * place than a line of code.
 */
void
makename(bname, fname)
char    bname[];
char    fname[];
{
	register char   *cp1;
	register char   *cp2;

	cp1 = strrchr(fname, '/');
	if (cp1) {
		++cp1;
		if (*cp1 == '\0') {	/* Something ending in a `/' */
			cp1--;
			while (cp1 && *cp1 && cp1[-1] != '/')
				cp1--;
		}
	} else
		cp1 = &fname[0];

	cp2 = &bname[0];
	while (cp2 != &bname[NBUFN-1] && *cp1 != 0)
		*cp2++ = *cp1++;
	*cp2 = 0;
}

/*
 * Ask for a file name, and write the contents of the current buffer to that 
 * file.  Update the remembered file name and clear the buffer changed flag. 
 * This handling of file names is different from the earlier versions, and
 * is more compatable with Gosling EMACS than with ITS EMACS. Bound to 
 * "C-X C-W".
 */
/* ARGSUSED */
int
filewrite(f, n)
int	f, n;
{
	register WINDOW *wp;
	register int    s;
	char	    	fname[NFILEN];
	char		bname[NFILEN];

	fname[0] = '\0';
	if ((s = mlgetfile("Write file: ", fname, NFILEN)) == ABORT)
		return (s);
	if (fname[0] == '\0') {
		if (curbp->b_fname && curbp->b_fname[0])
			strcpy(fname, curbp->b_fname);
		else
			return(FALSE);
	}
	if ((s = writeout(fname, FALSE)) == TRUE) {
		strcpy(curbp->b_fname, fname);
		makename(bname, fname);
		strcpy(curbp->b_bname, bname);
		curbp->b_flag &= ~(BFCHG|BFBAK);
		wp = wheadp;		    /* Update mode lines.   */
		while (wp != NULL) {
			if (wp->w_bufp == curbp)
				wp->w_flag |= WFMODE;
			wp = wp->w_wndp;
		}
	}
	return (s);
}

/*
 * Save the contents of the current buffer in its associated file. Do nothing
 * if nothing has changed (this may be a bug, not a feature). Error if there 
 * is no remembered file name for the buffer. Bound to "C-X C-S". May get 
 * called by "C-Z".
 */
/* ARGSUSED */
int
filesave(f, n)
int	f, n;
{
	register WINDOW *wp;
	register int    s;
	int 		backup;

	if (curbp->b_mode&MDVIEW)	/* don't allow this command if	*/
		return(rdonly());	/* we are in read only mode	*/
	if ((curbp->b_flag&BFCHG) == 0) {       /* Return, no changes.  */
		mlwrite("(No changes need to be saved)");
		return (TRUE);
	}
	if (curbp->b_fname[0] == 0) {	   /* Must have a name.    */
		mlwrite("No file name");
		return (FALSE);
	}
	backup = doingbackups && ((curbp->b_flag&BFBAK) != 0);
	if ((s = writeout(curbp->b_fname, backup)) == TRUE) {
		curbp->b_flag &= ~BFCHG;
		wp = wheadp;		    /* Update mode lines.   */
		while (wp != NULL) {
			if (wp->w_bufp == curbp)
				wp->w_flag |= WFMODE;
			wp = wp->w_wndp;
		}
	}
	return (s);
}

/*
 * This function performs the details of file writing. Uses the file 
 * management routines in the "fileio.c" package. The number of lines 
 * written is displayed with the file name. Most of the grief is error 
 * checking of some sort.
 */
int
writeout(fn, backup)
char    *fn;
int	backup;
{
	register LINE   *lp;
	register int    nline;
	register int    s;

	if ((s = ffwopen(fn, backup)) != FIOSUC)	/* Open writes message. */
		return (FALSE);
	mlwrite("[Writing %s...]", fn);		/* tell us we're writing*/
	lp = lforw(curbp->b_linep);	     /* First line.	  */
	nline = 0;			      /* Number of lines.     */
	while (lp != curbp->b_linep) {
		if ((lforw(lp) == curbp->b_linep) && (llength(lp) == 0))
			break;
		if ((s = ffputline(ltext(lp), llength(lp))) != FIOSUC)
			break;
		++nline;
		lp = lforw(lp);
	}
	if (s == FIOSUC) {		      /* No write error.      */
		s = ffclose();
		if (s == FIOSUC) {	      /* No close error.      */
			if (nline == 1)
				mlwrite("Wrote %s, 1 line",fn);
			else
				mlwrite("Wrote %s, %d lines", fn,nline);
		}
	} else				  /* Ignore close error   */
		(void) ffclose();	       /* if a write error.    */
	if (s != FIOSUC)			/* Some sort of error.  */
		return (FALSE);
	return (TRUE);
}

/*
 * The command allows the user to modify the file name associated with
 * the current buffer. It is like the "f" command in UNIX "ed". The operation 
 * is simple; just zap the name in the BUFFER structure, and mark the windows
 * as needing an update. You can type a blank line at the prompt if you wish.
 *
 * Changed by Chet to not modify the file name associated with the buffer if
 * no valid string is typed at the prompt.
 */
/* ARGSUSED */
int
filename(f, n)
int	f, n;
{
	register WINDOW *wp;
	register int    s;
	char	    fname[NFILEN];

	if ((s = mlgetfile("Name: ", fname, NFILEN)) == ABORT)
		return (s);
	if (s != FALSE)
		strcpy(curbp->b_fname, fname);
	wp = wheadp;			    /* Update mode lines.   */
	while (wp != NULL) {
		if (wp->w_bufp == curbp)
			wp->w_flag |= WFMODE;
		wp = wp->w_wndp;
	}
	curbp->b_mode &= ~MDVIEW;	/* no longer read only mode */
	return (TRUE);
}

/*
 * Insert file "fname" into the current buffer.  Called by insert file command.
 * Return the final status of the read.
 */
int
insertfile(fname, silent)
char    fname[];
{
	register int    s;

	if ((s = ffropen(fname)) == FIOERR)	/* Hard file open.      */
		goto out;
	if (s == FIOFNF) {			/* File not found.      */
		(*term.t_beep)();
		mlwrite("File not found");
		return(FALSE);
	}
	if (silent == FALSE)
		mlwrite("[Inserting %s]", fname);
	s = finsert(curbp);
	if ((s = ffclose()) != FIOSUC)	   /* Ignore errors.       */
		s = FIOERR;
out:
	if (s == FIOERR)			/* False if error.      */
		return (FALSE);
	return (TRUE);
}

int
finsert(bp)
BUFFER	*bp;
{
	register LINE   *lp0;
	register LINE   *lp1;
	register LINE   *lp2;
	register WINDOW	*wp;
	int    		nbytes, odoto, s, lflag;
	register int    nline;
	char		*line;
	LINE		*olp;

	bp->b_flag |= BFCHG;			/* we have changed	*/
	bp->b_flag &= ~BFTEMP;			/* and are not temporary*/
	olp = curwp->w_dotp;
	odoto = curwp->w_doto;
	/*
	 * Insert a blank line at point, and start inserting after it.
	 *
	 * After the `lnewline()', curwp->w_dotp == the new line just inserted
	 * or the portion of a line split in the middle that's after the
	 * just-inserted newline.
	 *
	 * After the inserting is done, we should be able to go back to
	 * odotp and odoto, remove the newline, and be done.
	 */
	(void) lnewline();
	olp = lback(curwp->w_dotp);
	if (olp == bp->b_linep || curwp->w_dotp == bp->b_linep) {
		/*
		 * If we're at the end of the buffer, then create a new line
		 * to insert before.
		 */
		(void) lnewline();
		curwp->w_dotp = lback(curwp->w_dotp);
	}

	/*
	 * INVARIANT:
	 *	curwp->w_dotp is the line we should be inserting *in front of*
	 */
	nline = 0;
	lflag = FALSE;
	while ((s = ffgetline(&line, &nbytes)) != FIOERR) {
		if (s == FIOEOF && nbytes == 0)
			/*
			 * Don't add the bogus newline; we'll just have to
			 * remove it later...
			 */
			break;
		if ((lp1 = lalloc(nbytes)) == NULL) {
			s = FIOERR;	     /* Keep message on the  */
			break;		  /* display.	     */
		}
		lp0 = lback(curwp->w_dotp);	/* line previous to insert */
		/*
		 * re-link new line between lp0 and curwp->w_dotp
		 */
		lp0->l_fp = lp1;
		curwp->w_dotp->l_bp = lp1;
		lp1->l_fp = curwp->w_dotp;
		lp1->l_bp = lp0;

		bcopy(line, ltext(lp1), nbytes);
		free(line);
		++nline;
		if (s == FIOEOF)
			break;
	}
	if (s == FIOEOF)
		s = FIOSUC;
	if (s == FIOSUC) {		      /* Don't zap message!   */
		if (nline == 1)
			mlwrite("(Inserted 1 line)");
		else
			mlwrite("(Inserted %d lines)", nline);
	}
	if (lflag)
		mlwrite("(Inserted %d line(s), Long lines wrapped)",nline);
	/* 
	 * Go back to where we started and mark the window for changes
	 */
	/*
	 * curwp->w_dotp == line after last line inserted, because of the 
	 * invariant in the above loop.
	 */
	curwp->w_markp = curwp->w_dotp = lback(curwp->w_dotp);
	curwp->w_marko = llength(curwp->w_markp);
	/*
	 * The above insertion loop does not insert a stray newline at the
	 * end of the text, so we should be OK.
	 */
	curwp->w_dotp = olp;
	curwp->w_doto = odoto;

	if (olp == bp->b_linep)
		curwp->w_dotp = lforw(olp);

	curwp->w_flag |= (WFHARD|WFMODE);

	/*
	 * If we inserted at the end of the buffer, set lp1 to the end of
	 * buffer line, and set lp2 to the beginning of the newly inserted
	 * text, otherwise set lp2 to NULL.  This is used to set the pointers
	 * in other windows correctly if they are also at the end of the
	 * buffer.
	 *
	 * Also delete the extraneous newline inserted above.
	 */

	lp1 = bp->b_linep;
	if (curwp->w_markp == lp1)
		lp2 = curwp->w_dotp;
	else {
		(void) ldelnewline();
		lp2 = (LINE *) NULL;
	}
	for (wp = wheadp; wp; wp = wp->w_wndp) {
		if (wp->w_bufp == bp) {
			wp->w_flag |= (WFMODE|WFEDIT);
			if (wp != curwp && lp2) {
				if (wp->w_dotp == lp1)
					wp->w_dotp = lp2;
				if (wp->w_markp == lp1)
					wp->w_markp = lp2;
				if (wp->w_linep == lp1)
					wp->w_linep = lp2;
			}
		}
	}
	return(s);
}

#ifdef XTITLE
extern char *wdir;
extern char home[];

/*
 * If we're running under X, write the current file name to the xterm 
 * titlebar.
 */
void
writetitle(fn)
char	*fn;
{
	char	*wd, file[256], line[256];
	int	wdlen, flen;

	if (fn[0] == '/') {		/* full path name given */
		wd = "";
		wdlen = 0;
	} else if (strcmp(home, wdir) == 0) {
		wd = "~";
		wdlen = 1;
	} else {
		wd = wdir;
		wdlen = strlen(wdir);
	}
	flen = (fn && *fn) ? strlen(fn) : 0;

	if (flen >= sizeof(file)) {
		strncpy(file, fn, sizeof(file) - 4);
		file[sizeof(file) - 4] = '\0';
		strcat(file, "...");
	} else if ((flen + wdlen + 3 > sizeof(file)) {
		strcpy(file, ".../");
		strncat(file, fn, sizeof(file) - 5);
		file[sizeof(file) - 1] = '\0';
	} else if (wd) {
		strcpy(file, wd);
		file[wdlen] = '/';
		strcpy(file+wdlen, fn);
	} else
		(void) strcpy(file, fn);

	(void) strcpy(line, "\033]2;ce: ");
	(void) strncat(line, file, sizeof(line) - 10);
	line[sizeof(line) - 3] = '\0';
	(void) strcat(line, "\007");
	(void) write(2, line, strlen(line));

	(void) strcpy(line, "\033]1;ce\007");		/* change icon title */
	(void) write(2,line,strlen(line));	
}
#endif /* XTITLE */

/*
 * Auto-save the file associated with the current buffer to a file named
 * `#fname#', where fname is the name of the file.
 */
int
fautosave()
{
	register LINE	*lp;
	register int	s;
	char		fn[NFILEN];

	if (curbp->b_mode & MDVIEW)
		return (FALSE);
	if ((curbp->b_flag & BFCHG) == 0)
		return (FALSE);
	if (curbp->b_fname[0] == 0)
		return (FALSE);
	if (mksavename(curbp->b_fname, fn, NFILEN) == 0)
		return FALSE;
	if ((s = ffwopen(fn, FALSE)) != FIOSUC)
		return (FALSE);
	mlwrite("autosaving...");
	lp = lforw(curbp->b_linep);
	while (lp != curbp->b_linep) {
		if ((s = ffputline(ltext(lp), llength(lp))) != FIOSUC)
			break;
		lp = lforw(lp);
	}
	if (s == FIOSUC)
		s = ffclose();
	else
		(void) ffclose();
	mlwrite("autosaving...done");
	if (s != FIOSUC)
		return (FALSE);
	return (TRUE);
}

/* ARGSUSED */
int
mksavename(fn,buf,len)
char	*fn;
char	buf[];
int	len;
{
	register char 	*p, *q;
	int		flen, plen;

	if (fn == 0 || *fn == '\0')	/* no filename */
		return 0;

	p = strrchr(fn, '/');	/* find filename */
	if (p == 0) {
		p = fn;
		plen = 0;
	} else {
		p++;
		plen = p - fn;
	}
	flen = strlen(p);

	if ((plen + flen + 3) >= len)	/* 3 == # + # + '\0' */
		return 0;

	if (plen)
		strncpy(buf, fn, plen);

	buf[plen] = '#';
	strcpy(buf+plen+1, p);
	buf[plen+flen+1] = '#';
	buf[plen+flen+2] = '\0';
	return 1;
}

/*
 * `Revert' the current buffer.
 * This means to toss away all changes we made to this buffer, remove any
 * auto-save files we might have for it, and read in the file anew.
 */
int
filerevert(f, n)
int	f, n;
{
	int	s;
	char	prompt[NLINE];

	if (curbp->b_fname[0] == '\0') {
		mlwrite("Buffer does not have associated file");
		return TRUE;
	}
	strcpy(prompt,"Revert buffer from file ");
	strcat(prompt, curbp->b_fname);
	if ((s = mlyesno(prompt)) != TRUE)
		return s;
	curbp->b_flag &= ~BFCHG;
	rmsave(curbp->b_fname);
	return (readin (curbp->b_fname));
}

/*
 * Delete any auto-save files corresponding to the file name `name'.
 */
void
rmsave(fname)
char	*fname;
{
	char	fn[NFILEN];

	if (mksavename(fname, fn, NFILEN) == 0)
		return;
	(void) unlink(fn);
}
