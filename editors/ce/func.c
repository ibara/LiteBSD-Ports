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
 * func.c -- ce function declarations and names for binding table
 */

#include "ce.h"

#ifndef NULL
#define NULL 0
#endif

#include "externs.h"

/*
 * Name to function binding table
 *
 * This table gives the names of all the bindable functions
 * and their C function address. These are used for the bind-to-key
 * function and the execute-named-command function.
 */

NBIND   names[] = {
        {"add-mode",			setemode},
	{"add-global-mode",		setgmode},
	{"auto-fill-mode",		wrapmode},
	{"auto-save-mode",		autosavemode},
	{"backward-character",		backchar},
	{"backward-sentence",           backsentence},
	{"begin-macro",			ctlxlp},
	{"begining-of-file",		gotobob},
	{"begining-of-line",		gotobol},
	{"bind-to-key",			bindtokey},
	{"buffer-position",		showcpos},
	{"c-mode",			cmode},
	{"capitalize-word",		capword},
	{"center-buffer",		centerbuffer},
	{"center-line",                 centerline},
	{"center-paragraph",		centerpara},
	{"center-region",		centerregion},
	{"change-directory",            changedir},
	{"change-file-name",		filename},
	{"chase-symlinks",		chaselinks},
	{"clear-and-redraw",		refresh},
	{"clear-pattern",		clearpat},
	{"copy-region",			copyregion},
	{"delete-blank-lines",		deblank},
	{"delete-buffer",		killbuffer},
	{"delete-global-mode",		delgmode},
	{"delete-horizontal-space",	delspace},
	{"delete-mode",			delemode},
	{"delete-next-character",	forwdel},
	{"delete-next-word",		delfword},
	{"delete-other-windows",	onlywind},
	{"delete-previous-character",	backdel},
	{"delete-previous-word",	delbword},
	{"delete-sentence-back",	delbsentence},
	{"delete-window",		delwind},
	{"describe-bindings",		wallchart},
	{"describe-key",		deskey},
	{"emacs-version",		showversion},
	{"end-macro",			ctlxrp},
	{"end-of-file",			gotoeob},
	{"end-of-line",			gotoeol},
	{"eval-file",                   evalfile},
	{"exchange-point-and-mark",	swapmark},
	{"execute-command-line",	execcmd},
	{"execute-macro",		ctlxe},
	{"execute-named-command",	namedcmd},
	{"exit-emacs",			saveandexit},
	{"fill-mode",			wrapmode},
	{"fill-paragraph",		fillpara},
	{"filter-buffer",		filterbuffer},
	{"filter-region",		filterregion},
	{"find-file",			filefind},
	{"forward-character",		forwchar},
	{"forward-sentence",		forwsentence},
	{"global-set-key",		bindtokey},
	{"goto-char",			gotochar},
	{"goto-line",			gotoline},
	{"grow-window",			enlargewind},
	{"handle-tab",			tab},
	{"help",			help},
	{"insert-buffer",		insbuffer},
	{"insert-file",			fileinsert},
	{"insert-space",		insspace},
	{"insert-shell-command-output",	insshoutput},
	{"interactive-shell",		spawncli},
	{"introduction",		intro},
	{"isearch-backward",		backisearch},
	{"isearch-forward",		forwisearch},
	{"kill-paragraph",		killpara},
	{"kill-region",			killregion},
	{"kill-sentence",		killsentence},
	{"kill-to-end-of-line",		killtext},
	{"list-buffers",		listbuffers},
	{"lower-case-region",		lowerregion},
	{"lower-case-word",		lowerword},
	{"make-backup-files",		makebackups},
	{"make-bug-report",		makebug},
	{"mark-buffer",			markbuffer},
	{"mark-paragraph",		markpara},
	{"mark-word",			markword},
	{"match-mode",			matchmode},
	{"move-down-vertically",	openvert},
	{"move-window-down",		mvdnwind},
	{"move-window-up",		mvupwind},
	{"newline",			newline},
	{"newline-and-indent",		indent},
	{"next-buffer",			nextbuffer},
	{"next-line",			forwline},
	{"next-page",			forwpage},
	{"next-paragraph",		gotoeop},
	{"next-window",			nextwind},
	{"next-word",			forwword},
	{"no-auto-save-mode",		noautosavemode},
	{"no-c-mode",			nocmode},
	{"no-chase-symlinks",		nochaselinks},
	{"no-fill-mode",		nowrapmode},
	{"no-match-mode",		nomatchmode},
	{"no-view-mode",		noviewmode},
	{"no-wrap-mode",		nowrapmode},
	{"not-modified",                notmodified},
	{"open-line",			openline},
	{"prefix-region",               prefixregion},
	{"previous-line",		backline},
	{"previous-page",		backpage},
	{"previous-paragraph",		gotobop},
	{"previous-window",		prevwind},
	{"previous-word",		backword},
	{"query-replace",       	queryrepl},
	{"query-replace-regexp",        regexqueryrepl},
	{"quick-exit",			quickexit},
	{"quote-character",		quote},
	{"read-file",			fileread},
	{"read-only",			readonly},
	{"redraw-display",		reposition},
	{"regexp-search-forward",       regexfsearch},
	{"regexp-search-backward",      regexbsearch},
	{"rename-buffer",		namebuffer},
	{"replace-regexp",		regexrepl},
	{"replace-string",		searchrepl},
	{"revert-buffer-from-file",	filerevert},
	{"save-buffers-exit-emacs",	saveandexit},
	{"save-file",			filesave},
	{"save-some-buffers",		savechanged},
	{"scroll-next-up",		scrnextup},
	{"scroll-next-down",		scrnextdw},
	{"scroll-other-window",		scrnextdw},
	{"search-again",		searchagain},
	{"search-backward",		backsearch},
	{"search-forward",		forwsearch},
	{"select-buffer",		usebuffer},
	{"self-insert",			selfinsert},
	{"send-bug-report",		sendbug},
	{"set-fill-column",		setfillcol},
	{"set-mark",			setmark},
	{"set-prefix",                  setprefix},
	{"set-visited-file-name",	filename},
	{"shell-command",		spawn},
	{"shell-command-on-region",	regionshellcmd},
	{"show-current-directory",      showcwd},
	{"show-functions",		showfuncs},
	{"show-matching-brace",		showbrace},
	{"show-matching-character",	charmatch},
	{"show-version",                showversion},
	{"shrink-window",		shrinkwind},
	{"split-current-window",	splitwind},
	{"suspend-emacs",		spawncli},
	{"switch-to-buffer",		usebuffer},
	{"transpose-characters",	twiddle},
	{"transpose-lines",		twiddlelines},
	{"tutorial",			intro},
	{"unbind-key",			unbindkey},
	{"universal-argument",          getarg},   /* is this at all useful? */
	{"upper-case-region",		upperregion},
	{"upper-case-word",		upperword},
	{"view-file",			fileview},
	{"view-mode",			viewmode},
	{"what-line",			whatline},
	{"wrap-mode",			wrapmode},
	{"write-file",			filewrite},
	{"yank",			yank},
	{NULL,				NULL}
};
