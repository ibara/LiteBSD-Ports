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
 *  bind.c -- functions dealing with key bindings
 *
 *  This file is for functions having to do with key bindings,
 *  descriptions, and command line execution.
 * 
 */
#include <config.h>
#include <stdio.h>
#include "celibc.h"
#include "ce.h"

#define STREQ(a,b)      (((*a) == (*b)) && (strcmp((a), (b)) == 0))

extern char	home[];		/* from dir.c */

extern Function *fncmatch();
extern char *gettok();

/*
 *  Match a string name (fname) to a function in the names table and 
 *  return it or NULL if nothing matches.  This declaration is crying
 *  out for a typedef.
 */

Function *
fncmatch(fname)
char	*fname;	/* name to attempt to match */
{
	register NBIND *ffp;	/* pointer to entry in name binding table */

	/* scan through the table, returning any match */
	ffp = &names[0];
	while (ffp->n_func != NULL) {
		if (STREQ(fname, ffp->n_name))
			return(ffp->n_func);
		++ffp;
	}
	return(NULL);
}

/*
 *  bindtokey:	add a new key to the key binding table
 */

/*ARGSUSED*/
bindtokey(f, n)
int f, n;	/* command arguments [n IGNORED] */
{
	register int 	c;		/* command key to bind */
	register Function *kfunc;	/* ptr to the requested function to bind to */
	register char 	*ptr;		/* ptr to dump out input key string */
	register KEYTAB *ktp;		/* pointer into the command table */
	register int 	found;		/* matched command flag */
	char 		outseq[80];	/* output buffer for keystroke sequence */
	Function	*mlgetcmd();

	/* 
	 * prompt the user to type in a key to bind, if we're not doing 
 	 * this from a startup file.
 	 */
	if (f == FALSE)
		mlwrite("Key to bind function to: ");
	/* get the command sequence to bind */
	c = getckey();			/* get a command sequence */
	/* change it to something we can print as well */
	if (clexec == FALSE) {
		cmdstr(c, &outseq[0]);
		/* and dump it out */
		ptr = &outseq[0];
		while (*ptr)
			(*term.t_putchar)(*ptr++);
		(*term.t_putchar)(' ');		/* space it out */
		(*term.t_flush)();
	}
	/* get the function name to bind it to */
	if (clexec == FALSE)
		mlwrite("Function to bind to: ");
	kfunc = mlgetcmd();
	if (kfunc == NULL) {
		if (clexec == FALSE)
			mlwrite("[No such function]");
		return(FALSE);
	}
	if (clexec == FALSE) {
		(*term.t_putchar)(' ');		/* space it out */
		(*term.t_flush)();
	}
	/* search the table to see if it exists */
	ktp = &keytab[0];
	found = FALSE;
	while (ktp->k_fp != NULL) {
		if (ktp->k_code == c) {
			found = TRUE;
			break;
		}
		++ktp;
	}
	if (found) {	/* it exists, just change it then */
		ktp->k_fp = kfunc;
	} else {	/* otherwise we need to add it to the end */
		/* if we run out of binding room, bitch */
		if (ktp >= &keytab[NBINDS]) {
			if (clexec == FALSE)
				mlwrite("Binding table Full!");
			return(FALSE);
		}
		ktp->k_code = c;	/* add keycode */
		ktp->k_fp = kfunc;	/* and the function pointer */
		++ktp;			/* and make sure the next is null */
		ktp->k_code = 0;
		ktp->k_fp = NULL;
	}
	return(TRUE);
}

/*
 *  unbindkey:	delete a key from the key binding table
 */

/*ARGSUSED*/
unbindkey(f, n)
int f, n;	/* command arguments [n IGNORED] */
{
	register int c;		/* command key to unbind */
	register char *ptr;	/* ptr to dump out input key string */
	register KEYTAB *ktp;	/* pointer into the command table */
	register KEYTAB *sktp;	/* saved pointer into the command table */
	register int found;	/* matched command flag */
	char outseq[80];	/* output buffer for keystroke sequence */

	/* 
	 * prompt the user to type in a key to unbind, if
	 * we're not doing this from a startup file.
	 */
	if (f == FALSE)
		mlwrite("Unbind Key: ");
	/* get the command sequence to unbind */
	c = getckey();			/* get a command sequence */
	/* change it to something we can print as well */
	cmdstr(c, &outseq[0]);
	/* and dump it out */
	ptr = &outseq[0];
	while (*ptr)
		(*term.t_putchar)(*ptr++);
	/* search the table to see if the key exists */
	ktp = &keytab[0];
	found = FALSE;
	while (ktp->k_fp != NULL) {
		if (ktp->k_code == c) {
			found = TRUE;
			break;
		}
		++ktp;
	}
	/* if it isn't bound, bitch */
	if (!found) {
	        (*term.t_beep)();
		mlwrite("Key not bound");
		return(FALSE);
	}
	/* save the pointer and scan to the end of the table */
	sktp = ktp;
	while (ktp->k_fp != NULL)
		++ktp;
	--ktp;		/* backup to the last legit entry */
	/* copy the last entry to the current one */
	sktp->k_code = ktp->k_code;
	sktp->k_fp   = ktp->k_fp;
	/* null out the last one */
	ktp->k_code = 0;
	ktp->k_fp = NULL;
	return(TRUE);
}

/*
 *  namedcmd:	execute a named command even if it is not bound
 */

namedcmd(f, n)
int f, n;	/* command arguments [passed through to command executed] */
{
	register Function *kfunc;	/* ptr to the requexted function to bind to */
	extern Function	  *mlgetcmd();   /* in ml.c */

	/* prompt the user to type a named command */
	mlwrite("M-x ");
	/* and now get the function name to execute */
	kfunc = mlgetcmd();
	if (kfunc == NULL) {
		mlwrite("[No such function]");
		return(FALSE);
	}
	/* and then execute the command */
	return((*kfunc)(f, n));
}

/*
 *  execcmd:	Execute a command line command to be typed in
 *              by the user
 */
/*ARGSUSED*/
execcmd(f, n)
int f, n;
{
	register int status;		/* status return */
	char cstr[NSTRING];		/* string holding command to execute */

	/* get the line wanted */
	if ((status = mlreply("Command: ", cstr, NSTRING)) != TRUE)
		return(status);
	return(docmd(cstr));
}

/*
 * Take a passed string as a command line and translate
 * it to be executed as a command. This function will be
 * used by execute-command-line.
 *
 * format of the command line is:
 *
 *     {# arg} <command-name> {<argument string(s)>}
 */
docmd(cline)
char	*cline;	/* command line to execute */
{
	register char *cp;	/* pointer to current position in command */
	register char *tp;	/* pointer to current position in token */
	register int f;		/* default argument flag */
	register int n;		/* numeric repeat value */
	register int sign;	/* sign of numeric argument */
	register Function *fnc;	/* function to execute */
	register int status;	/* return status of function */
	register int oldcle;	/* old contents of clexec flag */
	char token[NSTRING];	/* next token off of command line */

	/* first set up the default command values */
	f = FALSE;
	n = 1;
	cp = cline;		/* start at the begining of the line */
	cp = gettok(cp, token, sizeof(token));	/* and grab the first token */
	/* check for and process numeric leadin argument */
	if ((token[0] >= '0' && token[0] <= '9') || token[0] == '-') {
		f = TRUE;
		n = 0;
		tp = &token[0];
		/* check for a sign! */
		sign = 1;
		if (*tp == '-') {
			++tp;
			sign = -1;
		}
		/* calc up the digits in the token string */
		while (*tp) {
			if (*tp >= '0' && *tp <= '9')
				n = n * 10 + *tp - '0';
			++tp;
		}
		n *= sign;	/* adjust for the sign */
		/* and now get the command to execute */
		cp = gettok(cp, token, sizeof(token));		/* grab the next token */
	}

	/* and match the token to see if it exists */
	if ((fnc = fncmatch(token)) == NULL) {
		mlwrite("[No such Function]");
		return(FALSE);
	}
	/* save the arguments and go execute the command */
	strcpy(sarg, cp);		/* save the rest */
	oldcle = clexec;		/* save old clexec flag */
	clexec = TRUE;			/* in cline execution */
	status = (*fnc)(f, n);		/* call the function */
	clexec = oldcle;		/* restore clexec flag */
	return(status);
}

/* 
 * Chop a token off a string and return a pointer past the token
 */
char *
gettok(src, tok, toklen)
char	*src, *tok;	/* source string, destination token */
int	toklen;
{
	char	*tokbound;

	tokbound = tok + toklen;

	/* first scan past any whitespace in the source string */
	while (*src == ' ' || *src == '\t')
		++src;
	/* if quoted, go till next quote */
	if (*src == '"') {
		++src;		/* past the quote */
		/* Consume everything, but only copy TOKLEN chars to TOK */
		while (*src != 0 && *src != '"') {
			if (tok < tokbound - 1)
				*tok++ = *src++;
			else
				src++;
		}
		++src;		/* past the last quote */
		*tok = 0;	/* terminate token and return */
		return(src);
	}
	/* Copy until we find the end or whitespace.  Consume everything, but
	   only copy TOKLEN chars to TOK. */
	while (*src != 0 && *src != ' ' && *src != '\t') {
		if (tok < tokbound - 1)
			*tok++ = *src++;
		else
			src++;
	}
	/* terminate tok and return */
	*tok = 0;
	return(src);
}

/* 
 * Grab the next token out of sarg, return it, and chop it off sarg
 */

nxtarg(tok, toklen)
char	*tok;	/* buffer to put token into */
int	toklen;	/* token buffer size */
{
	char	*newsarg;	/* pointer to new begining of sarg */

	newsarg = gettok(sarg, tok, toklen);	/* grab the token */
	strcpy(sarg, newsarg);		/* and chop it off sarg */
	return(TRUE);
}

getckey()	/* get a command key sequence from the keyboard	*/
{
	register int c;		/* character fetched */
	register char *tp;	/* pointer into the token */
	char tok[NSTRING];	/* command incoming */

	/* check to see if we are executing a command line */
	if (clexec) {
		(void) nxtarg(tok, sizeof(tok));	/* get the next token */
		tp = &tok[0];
		c = 0;
		while (*tp) {
			if (*tp == 'M' && tp[1] == '-') {
				/* first, the META prefix */
				c = META;
				tp += 2;
			} else if (*tp == '^' && tp[1] == 'X' && tp[2]) {
				/* control-x, but make sure it's a prefix */
				c |= CTLX;
				tp += 2;
			} else if (*tp == '^' && tp[1] != 0) {
				/* a control char? */
				if (tp[1] == '?') {
					c |= 0x7f;
					tp++;	/* skip over the ? */
				} else
					c |= CNTL;
				tp++;
			} else if (*tp == 'C' && tp[1] == '-') {
				if ((tp[2] == 'X' || tp[2] == 'x') && tp[3]) {
					c |= CTLX;
					tp += 3;
				} else {
					if (tp[2] == '?') {
						c |= 0x7f;
						tp++;
					} else
						c |= CNTL;
					tp += 2;
				}
			} else {
				/* add a `normal' key to the sequence... */
				/* make sure we are not lower case */
				/* c |= (ISLOWER(*tp) ? TOUPPER(*tp) : *tp; */
				if (ISLOWER(*tp) != FALSE)
					*tp = TOUPPER(*tp);
				c |= *tp++;
			}
		}
		return(c);
	}
	/* or the normal way */
	c = getkey();			/* get a command sequence */
	if (c == (CNTL|'X'))		/* get control-x sequence */
		c = CTLX | getctl();
	return(c);
}

/* 
 * Execute the startup file $HOME/.ceinit, if it exists.
 * Don't fret if it doesn't.
 */

startup()
{
	char	fn[NFILEN];
	int	hlen;

	hlen = (home && *home) ? strlen(home) : 0;

	if ((hlen + 8) >= NFILEN)
		return FALSE;

	if (home && *home)
		(void) sprintf(fn, "%s/.ceinit", home);
	else {
		mlwrite("Can't find $HOME to look for startup file");
		return(FALSE);
	}
	if (access(fn, 0) == 0)
		(void) load(fn);
	return(TRUE);
}

/*
 *  Evaluate a file as a series of command lines.  
 */
/*ARGSUSED*/
evalfile(f,n)
{
	register int s;
	char 	fn[NFILEN];

	if (kbdmip != NULL) {
		mlwrite("Cannot load a file from a keyboard macro");
		return(FALSE);
	}
	if (curbp->b_flag & MDVIEW)
    		return(rdonly());
	if ((s = mlreply("Load File: ", fn, NFILEN)) != TRUE)
		return(s);
	return(load(fn));
}

/*
 * Do the actual execute-commands-from-a-file stuff
 */
load(fn)
char *fn;
{
	char	*execbuf;
	int	s, z;

    	if (ffropen(fn) != FIOSUC)
		return(FALSE);
	while ((s = ffgetline(&execbuf, &z)) != FIOERR) {
		if ((z == 0) && (s == FIOEOF))
			break;
		if (execline(execbuf) != TRUE) {
			s = FIOERR;
			free(execbuf);
			mlwrite("Error loading %s",fn);
			break;
		}
		free(execbuf);
		if (s == FIOEOF)
			break;
	}
	z = ffclose();
	if ((s == FIOERR) || (z != FIOSUC))
		return(FALSE);
	return(TRUE);
}


/*
 * Skip white space and comments (all chars after a semicolon, just like lisp
 * or Macro-11) on a command line passed by execline.
 */
char *
skipwhite(s)
register char *s;
{
	while (*s == ' ' || *s == '\t')
		s++;
	if (*s == ';')
		*s = '\0';
	return s;
}

/*
 * Execute a command line (from a file).  Do a little preprocessing, then
 * pass the whole thing on to doline.
 */
execline(line)
register char *line;
{
	register char *p,*q;
	int qst;

   	p = skipwhite(line);      /* skip whitespace and comments at beginning */
	if (*p == '\0') 
        	return(TRUE);         /* whole line was empty or a comment */
	/* 
	 * Cut out stuff after any trailing semicolons.
	 */
   	qst = FALSE;
    	for (q = p; q && *q; q++) {
		if (*q == '"')
			qst = !qst;
		else if (*q == ';') {
	    		if (qst == FALSE) {
				*q = '\0';
				return TRUE;
			}
		}
	}
	return(doline(p));
}

/*
 * To use later when interpreting octal sequences as given in C, to wit:
 * '\077'
 */

#define ISOCTAL(c) (((c) <= '7') && ((c) >= '0'))

/*
 * Do a command line from a file.  When this gets the command line, it
 * should be free of comments, etc.
 */
doline(l)
register char *l;
{
	char 		token[NSTRING];
	register char 	*cp, *tp, *ap;
	register int 	c;  
	int 		sign, binding, oldcle, status;
	char 		argp[NSTRING];
	int 		f, n; 
    	register Function *fnc;

	f = FALSE;
	n = 1;
	cp = l;
	cp = gettok(cp, token, sizeof(token));
/*	bzero(argp, sizeof(argp)); */
	if (token[0] == '\0')
		return(TRUE);	/* empty line */
	if (ISDIGIT(token[0]) || token[0] == '-') {
        	f = TRUE;
		n = 0;
		tp = &(token[0]);
		sign = 1;
		if (*tp == '-') {    /* if it's a minus sign, note for later */
			tp++;
			sign = -1;
		}
		while (*tp) {         /* while still digits in the current token */
			if (ISDIGIT(*tp))
	        		n = n*10 + *tp - '0';
			++tp;
		}
		n *= sign;
		cp = gettok(cp, token, sizeof(token));
	}
	if ((fnc = fncmatch(token)) == NULL) {    /* assume it's a function name */
        	mlwrite("Unknown function: %s", token);
		return (FALSE);
	}
	ap = &(argp[0]);

	/*
	 * Note whether or not we are binding a key to a function,
	 * to turn on some character interpretation
	 */
	binding = (fnc == bindtokey || fnc == unbindkey);

	/*
	 *  If we are binding, turn on some character translation to make the
	 *  arguments look like GNU:  \e -> ESC, for example.
	 *  If binding is TRUE, next arg is a character sequence to translate.
	 *  There are at most two arguments (a rather arbitrary decision).
	 */
	cp = skipwhite(cp);		/* skip white space and comments */
	if (cp)
		cp = gettok(cp, token, sizeof(token));
	else
		token[0] = '\0';
	/*
	 *  Token holds the arg string, if any.  If we are binding, we
	 *  translate the characters.  In any case, we then copy the
	 *  whole thing to sarg for later processing.
	 */
	tp = &token[0];  ap = &argp[0];
	if (binding) {
		while (*tp != '"' && *tp) {
			if (*tp != '\\')
				c = *tp++;     /* no translating needed */
			else {
	        		switch(*++tp) {
				case 't':
				case 'T':  
					c = CCHR('I');
					break;
				case 'n':
				case 'N':
					c = CCHR('J');
					break;
				case 'r':
				case 'R':
					c = CCHR('M');
					break;
				case 'e':
				case 'E':
					c = CCHR('[');
					break;
				case '^':
					c = CMASK(*++tp);
		          		c = (ISLOWER(c)) ? CCHR(TOUPPER(c)) : CCHR(c);
		          		break;
				case '0': case '1': case '2': case '3': 
				case '4': case '5': case '6': case '7': 
					c = *tp - '0';     /* octal digit */
					while (ISOCTAL(tp[1])) {
						c *= 8;
						c += *++tp - '0';
					}
					break;
				default:
					c = CMASK(*tp);
					break;
				}
				tp++;
			}
			*ap++ = c;
		}
		/*
		 * Done translating the token, it's now sitting in argp 
		 * If there is a second argument, we skip whitespace until
		 * we get to it.  This also consumes trailing white space
		 * on the line.
		 */
		*ap++ = '\0';
		if (*cp && (*cp == ' ' || *cp == '\t'))
			cp++;       /* should be a blank */
		strcpy(sarg, argp);
	} else
		strcpy(sarg, token);
	/*
	 * Now we are done with the first arg.  Is there a second?
	 */
	if (cp && *cp)
		cp = gettok(cp, token, sizeof(token));
	if (token[0]) {
        	strcat(sarg, " ");
		strcat(sarg, token);
	}
	oldcle = clexec;
	clexec = TRUE;
	if (binding == FALSE && f == TRUE)
		binding = TRUE;
	status = (*fnc)(binding, n);  /* if binding, don't prompt the user */
	clexec = oldcle;
	return (status);
}

/*
 * Take a function name (like "isearch-forward") and return a keybinding for
 * it (doing all the CTLX and META stuff).
 */

int
keybinding(name)
char	*name;
{
	register Function *fp;
	register KEYTAB	  *ktp;
	
	fp = fncmatch(name);
	if (fp == NULL)
		return -1;
	ktp = &keytab[0];
	while (ktp->k_fp && (ktp->k_fp != fp))
		ktp++;
	if (ktp == NULL)
		return -1;
	return ktp->k_code;
}
