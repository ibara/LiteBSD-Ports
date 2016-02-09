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
 * key.c  -- Initial default key to function bindings for ce
 *
 */

#include "ce.h"

#ifndef NULL
#define NULL 0
#endif

#include "externs.h"

/*
 * Command table.
 * This table  is *roughly* in ASCII order, left to right across the
 * characters of the command. This expains the funny location of the
 * control-X commands.
 */
KEYTAB  keytab[NBINDS] = {
        {CNTL|'@',              setmark},
	{CNTL|'A',		gotobol},
	{CNTL|'B',		backchar},
	{CNTL|'C',		showversion},
	{CNTL|'D',		forwdel},
	{CNTL|'E',		gotoeol},
	{CNTL|'F',		forwchar},
	{CNTL|'G',		ctrlg},
	{CNTL|'H',		backdel},
	{CNTL|'I',		tab},
	{CNTL|'J',		indent},
	{CNTL|'K',		killtext},
	{CNTL|'L',		refresh},
	{CNTL|'M',		newline},
	{CNTL|'N',		forwline},
	{CNTL|'O',		openline},
	{CNTL|'P',		backline},
	{CNTL|'Q',		quote},
	{CNTL|'R',		backisearch},
	{CNTL|'S',		forwisearch},
	{CNTL|'T',		twiddle},
	{CNTL|'U',              getarg},
	{CNTL|'V',		forwpage},
	{CNTL|'W',		killregion},
	{CNTL|'Y',		yank},
	{CNTL|'Z',		spawncli},
	{CTLX|CNTL|'B',		listbuffers},
	{CTLX|CNTL|'C',		saveandexit},    /* Hard quit.           */
	{CTLX|CNTL|'F',		filefind},
	{CTLX|CNTL|'G',		ctrlg},
	{CTLX|CNTL|'I',		insbuffer},
	{CTLX|CNTL|'L',		lowerregion},
	{CTLX|CNTL|'M',		delemode},
	{CTLX|CNTL|'N',		mvdnwind},
	{CTLX|CNTL|'O',		deblank},
	{CTLX|CNTL|'P',		mvupwind},
	{CTLX|CNTL|'Q',		readonly},
	{CTLX|CNTL|'R',		fileview},
	{CTLX|CNTL|'S',		filesave},
	{CTLX|CNTL|'T',         twiddlelines},  
	{CTLX|CNTL|'U',		upperregion},
	{CTLX|CNTL|'V',		fileread},
	{CTLX|CNTL|'W',		filewrite},
	{CTLX|CNTL|'X',		swapmark},
	{CTLX|CNTL|'Z',		shrinkwind},    /* non-GNU */
	{CTLX|'!',		spawn},         /* Run 1 command.       */
	{CTLX|'%',		searchrepl},    /* non-GNU */
	{CTLX|'(',		ctlxlp},
	{CTLX|')',		ctlxrp},
	{CTLX|'.',		setprefix},
	{CTLX|'=',		showcpos},
	{CTLX|'?',		deskey},
	{CTLX|'^',		enlargewind},
	{CTLX|'1',		onlywind},
	{CTLX|'2',		splitwind},
/*      {CTLX|'A',              appendtobuffer},  */
	{CTLX|'B',		usebuffer},
	{CTLX|'C',		spawncli},      /* non-GNU */
	{CTLX|'E',		ctlxe},
	{CTLX|'F',		setfillcol},
	{CTLX|'H',		markbuffer},
	{CTLX|'I',              fileinsert},
	{CTLX|'K',		killbuffer},
	{CTLX|'M',		setemode},
	{CTLX|'N',		filename},
	{CTLX|'O',		nextwind},
	{CTLX|'P',		prevwind},
	{CTLX|'S',		savechanged},
	{CTLX|'X',		nextbuffer},
	{CTLX|'Z',		enlargewind},
	{CTLX|0x7F,		delbsentence},
	{META|CNTL|'G',		ctrlg},
	{META|CNTL|'H',		delbword},
	{META|CNTL|'K',		unbindkey},
	{META|CNTL|'L',		reposition},
	{META|CNTL|'M',		delgmode},
	{META|CNTL|'N',		namebuffer},
	{META|CNTL|'O',		openvert},
	{META|CNTL|'R',		queryrepl},
	{META|CNTL|'S',		regexfsearch},
	{META|CNTL|'V',		scrnextdw},
	{META|CNTL|'W',		killpara},
	{META|CNTL|'Z',		scrnextup},
	{META|'@',		markword},
	{META|' ',		setmark},
	{META|'%',              queryrepl},
	{META|'?',		help},
	{META|'!',		spawn},      /* GNU has it this way */
	{META|'.',		setmark},
	{META|'>',		gotoeob},
	{META|'=',              countregion},
	{META|'<',		gotobob},
	{META|'|',              regionshellcmd},
	{META|'[',		xtermarrowkeys},
	{META|')',		charmatch},
	{META|'A',              backsentence},
	{META|'B',		backword},
	{META|'C',		capword},
	{META|'D',		delfword},
	{META|'E',              forwsentence},
	{META|'F',		forwword},
	{META|'G',		fillregion},   
	{META|'H',		markpara},
	{META|'I',              tab},            /* GNU seems to do this */
	{META|'K',		killsentence},
	{META|'L',		lowerword},
	{META|'M',		setgmode},
	{META|'N',		gotoeop},
	{META|'O',		xtermarrowkeys}, /* this is a hack [CR] */
	{META|'P',		gotobop},
	{META|'Q',		fillpara},
	{META|'R',		reposition},     /* GNU has it this way */
	{META|'S',		centerline},     /* like Twenex EMACS */
	{META|'U',		upperword},
	{META|'V',		backpage},
	{META|'W',		copyregion},
	{META|'X',		namedcmd},
	{META|'Z',		quickexit},
	{META|'\\',		delspace},
	{META|'~',              notmodified},
	{META|0x7F,             delbword},
	{0x7F,			backdel},
	{0,			(Function *) 0}
};
