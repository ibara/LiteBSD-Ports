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
 * externs.h -- ce function declarations
 */

/*
 * External function declarations for bindable commands
 */

/*
 * defined in main.c
 */
extern  int     ctrlg();                /* Abort out of things          */
extern  int     quit();                 /* Quit                         */
extern  int     ctlxlp();               /* Begin macro                  */
extern  int     ctlxrp();               /* End macro                    */
extern  int     ctlxe();                /* Execute macro                */
extern  int     getarg();               /* Get a command argument       */
extern  int     quickexit();            /* Quick-and-dirty exit of ce   */
extern  int   	saveandexit();          /* Save-buffers-exit-ce         */
extern	int	savechanged();		/* Save all changed files	*/
extern	int	selfinsert();		/* Insert the character		*/

/*
 * defined in file.c
 */

extern  int     fileread();             /* Get a file, read only        */
extern  int     filefind();		/* Get a file, read write       */
extern  int     filewrite();            /* Write a file                 */
extern  int     filesave();             /* Save current file            */
extern  int     filename();             /* Adjust file name             */
extern	int	fileview();		/* find a file in view mode	*/
extern	int	fileinsert();		/* insert a file		*/
extern	int	filerevert();		/* revert buffer from file	*/
/*
 * defined in basic.c
 */ 

extern  int     getccol();              /* Get current column           */
extern  int     gotobol();              /* Move to start of line        */
extern  int     forwchar();             /* Move forward by characters   */
extern  int     gotoeol();              /* Move to end of line          */
extern  int     backchar();             /* Move backward by characters  */
extern  int     forwline();             /* Move forward by lines        */
extern  int     backline();             /* Move backward by lines       */
extern  int     forwpage();             /* Move forward by pages        */
extern  int     backpage();             /* Move backward by pages       */
extern  int     gotobob();              /* Move to start of buffer      */
extern  int     gotoeob();              /* Move to end of buffer        */
extern  int     setmark();              /* Set mark                     */
extern  int     swapmark();             /* Swap "." and mark            */
extern	int	gotoline();		/* go to a numbered line	*/
extern	int	gotochar();		/* go to a numbered character	*/

/* 
 * defined in random.c
 */

extern  int     setfillcol();           /* Set fill column.             */
extern  int     showcpos();             /* Show the cursor position     */
extern  int     twiddle();              /* Twiddle characters           */
extern  int     tab();                  /* Insert tab                   */
extern  int     newline();              /* Insert CR-LF                 */
extern  int     indent();               /* Insert CR-LF, then indent    */
extern  int     openline();             /* Open up a blank line         */
extern	int	openvert();		/* Open a line vertically	*/
extern  int     deblank();              /* Delete blank lines           */
extern  int     quote();                /* Insert literal               */
extern  int     forwdel();              /* Forward delete               */
extern  int     backdel();              /* Backward delete              */
extern	int	delspace();		/* Delete horizontal space	*/
extern  int     killtext();             /* Kill forward                 */
extern  int     yank();                 /* Yank back from killbuffer	*/
extern	int	setemode();		/* set an editor mode		*/
extern	int	delemode();		/* delete a mode		*/
extern	int	setgmode();		/* set a global mode		*/
extern	int	delgmode();		/* delete a global mode		*/
extern  int     centerline();           /* center text on a line	*/
extern	int	centerbuffer();		/* center all lines in buffer	*/
extern  int	viewmode();		/* put current buffer in View	*/
extern	int	wrapmode();		/* put current buffer in Wrap	*/
extern	int	autosavemode();		/* auto-save the current buffer	*/
extern	int	cmode();		/* put current buffer in C Mode	*/
extern  int	noviewmode();		/* put current buffer in Edit	*/
extern	int	nowrapmode();		/* put current buffer in no Wrap*/
extern	int	noautosavemode();	/* don't auto-save the current buffer */
extern	int	nocmode();		/* current buffer not in C Mode	*/
extern	int	matchmode();		/* current buffer shows balance chars */
extern	int	nomatchmode();		/* don't show balance chars	*/
extern  int	xtermarrowkeys();	/* hack to use xterm arrow keys	*/
extern	int	twiddlelines();		/* Transpose lines		*/
extern  int	makebackups();		/* toggle backup flag on and off*/
extern	int	readonly();		/* toggle read-only flag on/off	*/
extern	int	whatline();		/* display line number in buffer*/
extern	int	chaselinks();		/* follow symlinks when making backup files */
extern	int	nochaselinks();		/* or not */

/*
 * defined in  search.c
 */

extern  int     forwsearch();           /* Search forward               */
extern  int     backsearch();           /* Search backwards             */
extern	int	searchrepl();		/* search and replace		*/
extern	int	queryrepl();		/* search and replace w/query	*/
extern  int     forwisearch();          /* Incremental search forward   */
extern  int     backisearch();          /* Incremental search backward  */
extern  int	searchagain();		/* search for last search string*/
extern	int	clearpat();		/* clear the search pattern	*/

/*
 * defined in window.c
 */

extern  int     nextwind();             /* Move to the next window      */
extern  int     prevwind();             /* Move to the previous window  */
extern  int     onlywind();             /* Make current window only one */
extern  int     splitwind();            /* Split current window         */
extern  int     mvdnwind();             /* Move window down             */
extern  int     mvupwind();             /* Move window up               */
extern  int     enlargewind();          /* Enlarge display window.      */
extern  int     shrinkwind();           /* Shrink window.               */
extern  int     reposition();           /* Reposition window            */
extern  int     refresh();              /* Refresh the screen           */
extern	int	scrnextup();		/* scroll next window back	*/
extern	int	scrnextdw();		/* scroll next window down	*/
extern	int	delwind();

/*
 * defined in buffer.c
 */

extern  int     listbuffers();          /* Display list of buffers      */
extern  int     usebuffer();            /* Switch a window to a buffer  */
extern  int     killbuffer();           /* Make a buffer go away.       */
extern	int	namebuffer();		/* rename the current buffer	*/
extern	int	nextbuffer();		/* switch to the next buffer	*/
extern  int     notmodified();          /* mark buffer as not modified  */
extern	int	insbuffer();		/* insert a buffer at "."	*/
extern	int	markbuffer();		/* make region entire buffer	*/

/*
 * defined in word.c
 */

extern  int     backword();             /* Backup by words              */
extern  int     forwword();             /* Advance by words             */
extern  int     upperword();            /* Upper case word.             */
extern  int     lowerword();            /* Lower case word.             */
extern  int     capword();              /* Initial capitalize word.     */
extern  int     delfword();             /* Delete forward word.         */
extern  int     delbword();             /* Delete backward word.        */
extern	int	markword();		/* Make region current word	*/
extern	int	forwsentence();		/* Go to start of next sentence */
extern	int	backsentence();		/* Go to start of this sentence */
extern	int	killsentence();		/* Delete current sentence	*/
extern	int	delbsentence();		/* Delete back to start of sentence */

/*
 * defined in region.c
 */

extern  int     upperregion();          /* Upper case region.           */
extern  int     lowerregion();          /* Lower case region.           */
extern  int     killregion();           /* Kill region.                 */
extern  int     copyregion();           /* Copy region to kill buffer.  */
extern  int     prefixregion();         /* Prefix each line in region   */
extern  int     setprefix();            /* set prefix for prefixregion  */
extern  int     countregion();          /* Count the lines in region    */
extern  int     fillregion();           /* Fill region to fill column   */
extern	int	centerregion();		/* Center all lines in region	*/

/*
 * defined in spawn.c
 */

extern  int     spawncli();             /* Run CLI in a subjob.         */
extern  int     spawn();                /* Run a command in a subjob.   */
extern	int	regionshellcmd();	/* Run a shell command on region*/
extern	int	filterregion();		/* filter region, replace 	*/
extern	int	filterbuffer();		/* filter buffer, replace	*/
extern	int	insshoutput();		/* insert shell command output  */

/*
 * defined in paragraph.c 
 */

extern	int	gotobop();		/* go to begining/paragraph	*/
extern	int	gotoeop();		/* go to end/paragraph		*/
extern	int	fillpara();		/* fill current paragraph	*/
extern	int	killpara();		/* kill the current paragraph	*/
extern	int	markpara();		/* mark the current paragraph	*/
extern	int	centerpara();		/* Center the current paragraph	*/

/*
 * defined in help.c
 */

extern	int	help();			/* get the help file here	*/
extern	int	deskey();		/* describe a key's binding	*/
extern	int	wallchart();		/* describe bindings		*/
extern  int	intro();		/* display the emacs tutorial   */
extern	int	showfuncs();		/* display the list of functions*/

/*
 * defined in bind.c
 */

extern	int	bindtokey();		/* bind a function to a key	*/
extern	int	unbindkey();		/* unbind a key's function	*/
extern	int	namedcmd();		/* execute named command	*/
extern	int	execcmd();		/* execute a command line	*/
extern	int	evalfile();		/* exec commands from a file	*/

/*
 * defined in line.c
 */

extern	int	insspace();		/* insert a space forword	*/

/*
 * defined in rsearch.c
 */

extern  int     regexfsearch();         /* search forward for a regexp  */
extern  int     regexbsearch();         /* search backward for a regexp */
extern  int     regexqueryrepl();       /* query replace regexp         */
extern	int	regexrepl();		/* replace regexp, no query	*/

/*
 * defined in dir.c
 */

extern  int     changedir();            /* change current working dir   */
extern  int     showcwd();              /* display current working dir  */

/*
 * defined in version.c
 */

extern  int     showversion();          /* show current version of ce   */

/*
 * defined in bug.c
 */
extern	int	makebug();		/* pop to a bug report buffer	*/
extern	int	sendbug();		/* send current buffer as bug report */

/*
 * defined in cmode.c
 */
extern int	insbrace();
extern int	inspound();
extern int	cnewline();
extern int	cfile();
extern int	matchregion();

/*
 * defined in paren.c
 */
extern int	bchar();
extern LMATCH	*findmat();
extern int	balance();
extern int	showmatch();
extern int	charmatch();
extern int	showbrace();
