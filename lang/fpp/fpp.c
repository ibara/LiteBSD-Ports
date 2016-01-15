/* @(#)fpp.c 1.11 97/03/14 */
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
#include "fpp.h"
#include "service.h"


char	*progname = "fpp";	/* the name of the program */

static char	*filein;	/* name of input file*/
static char	*fileout;	/* name of output file*/
static char	*stdinclude;	/* default directory searched for include files */
static StrList	*defs;		/* list of D options from command line */
static StrList	*undefs;	/* list of U options from command line */
static int	versionfl;	/* generate version info */

static int	
args( int argc, char *argv[]) {
	int	i;
	char	*p;
	StrList	*t;

	filein = NULL;
	fileout = NULL;
	stdinclude = STDINCLUDE;
	defs = NULL;
	undefs = NULL;
	versionfl = 0;

	for (i=0;i<argc;i++) {
		p = argv[i];
		if (*p=='-') {
			switch(*++p) {
			case 'I': 
				if (add_path(++p)) return 1;
				break;
			case 'D':
				t = my_alloc(sizeof(StrList),1);
				t->str = ++p;
				t->next = defs;
				defs = t;
				break;
			case 'M':
				if (*++p) 
					fppmess(WARN_BADOPT,argv[i]);
				else 
					mdepfl = 1;
				break;
			case 'P':
				if (*++p) 
					fppmess(WARN_BADOPT,argv[i]);
				else {
					linefl = 1;
					ignorelinefl = 1;
				}
				break;
			case 'V':
				if (*++p) 
					fppmess(WARN_BADOPT,argv[i]);
				else 
					versionfl = 1;
				break;
			case 'U':
				if (*++p) {
					t = my_alloc(sizeof(StrList),1);
					t->str = p;
					t->next = undefs;
					undefs = t;
				}
				else
					fppmess(WARN_BADOPT,argv[i]);
				break;
			case 'X':
				switch (*++p) {
				case 'w':	if (*++p) 
							fppmess(WARN_BADOPT,argv[i]);
						else 
							mmode |= MOD_SPACE;
						break;
				case 'u':	if (*++p)
							fppmess(WARN_BADOPT,argv[i]);
						else 
							mmode |= MOD_LCASE;
						break;
				default:	fppmess(WARN_BADOPT,argv[i]);
				}
				break;
			case 'Y':
				stdinclude = ++p;
				break;
			case 'c':
			        if (!strcmp(p,"c_com=yes"))
				        no_ccom_fl = 0;
				else if (!strcmp(p,"c_com=no"))
				        no_ccom_fl = 1;
				else
					fppmess(WARN_BADOPT,argv[i]);
			        break;
#if DEBUG
			case 'd':
			        debug = atoi(++p);
				break;
#endif
			case 'e':
			        switch (*++p) {
				case '\0':  /* -e */
					line_width0 = LINE_WIDTH_EXT;
					break;
				default:
					fppmess(WARN_BADOPT,argv[i]);
					break;
				}
				break;

			case 'f':
				if (!strcmp(p,"free"))
					fixedformfl = 0;
				else if (!strcmp(p,"fixed"))
					fixedformfl = 1;
				else if (!strcmp(p,"f77"))
					f77fl = 1;
				else if (!strcmp(p,"f90"))
					f77fl = 0;
				else
					fppmess(WARN_BADOPT,argv[i]);
				break;
			case 'm':
			        if (!strcmp(p,"macro=yes"))
				        substfl = 2;
				else if (!strcmp(p,"macro=no_com"))
				        substfl = 1;
				else if (!strcmp(p,"macro=no"))
				        substfl = 0;
				else 	   
					fppmess(WARN_BADOPT,argv[i]);
				break;
			case 'w':
				++p;
				if (*p && *p!= '0') 
					fppmess(WARN_BADOPT,argv[i]);
				else
					warnfl = 1;
				break;
			default : 
				if (!strcmp(p,"undef")) {
					predefinefl = 0;
					break;
				}
				fppmess(WARN_BADOPT,argv[i]);
			}
		}
		else if (filein) {
			if (fileout) 
				return 1;
			fileout = p;
		}
		else
			filein = p;
	}
	return 0;
}

int
main( int argc, char *argv[] ) {
        char    *s;
	int	i;

	init0();


	err_init(0);    /* before args() */
	if (args(argc-1,argv+1)) 
		fppmess(FERR_ARGS);
	err_init(1);    /* and after     */

	if (versionfl) {

#ifndef RELEASE
#define RELEASE ""
#endif
		fppmess(MSG_PDVER, progname, RELEASE, __DATE__);
	}

	add_path(stdinclude);
	init1(filein, fileout, defs, undefs);
	process();
	my_exit(errnum);

	return 1;
}
