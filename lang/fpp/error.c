/* @(#)error.c 1.7 97/01/14 */
/*
 * ====================================================
 * Copyright (C) 1995 by Sun Microsystems, Inc. All rights reserved.
 *
 * Developed at SunSoft, a Sun Microsystems, Inc. business.
 * Permission to use, copy, modify, and distribute this
 * software is freely granted, provided that this notice
 * is preserved.
 * ====================================================
 */

#include <stdio.h>
#include <stdarg.h>
#include "fpp.h"

int	warnfl = 0;		/* Do not print warnings */
int	errnum = 0;

#include "msgno.h"
#include "msgtxt.h"

void
err_init(int fl) {
        if (fl == 0) {

	  /* First init before args being processed */
	}
	else {

	  /* Second init after args being processed */
	}
}

void
err_fini() {
}

unsigned int
get_msg_severity(int msgno) {
    return(I18N_msg[msgno].severity);
}

const char *
get_msg(int msgno) {
    return(I18N_msg[msgno].txt);
}

void
error_vprint(const char *fname,
	      int lineno,
	      int colmno,
	      const char *msg_kind,
	      const char *message,
	      va_list plist)
{

	if (fname) {
	        if (lineno)
		        fprintf(stderr, "\"%s\", line %d: ", fname, lineno);
		else 
		        fprintf(stderr,"%s: ", fname);
	}
	if (msg_kind)
	        fprintf(stderr, "%s: ", msg_kind);
	vfprintf(stderr, message, plist);
}

void 
fppmess( int errno, ...) {
	va_list	plist;
	char   *fname;
	char const *msg_kind;
	int    lineno;

	if (!errno) return;
	va_start(plist,errno);

	switch (get_msg_severity(errno)) {
	case 1:
	        if (warnfl) return;
	        msg_kind = get_msg(MSG_WARNING);
		break;
	case 2:
	        msg_kind = get_msg(MSG_ERROR);
		errnum++;
		break;
	case 3:
	        msg_kind = get_msg(MSG_FATAL);
		break;
	default:
	        error_vprint(NULL, 0, 0, NULL, get_msg(errno), plist);
	        return;
	}

	fname  = get_cur_fname();
	lineno = get_err_lineno();

       	error_vprint(fname, lineno, 0, msg_kind, get_msg(errno), plist);

	if (get_msg_severity(errno) == 3)
	        my_exit(1);
	return;
}
