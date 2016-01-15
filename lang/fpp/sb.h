/* @(#)sb.h 1.2 95/09/14 */
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

#ifdef BROWSER

void sb_version();
void sb_lineno(int);
void sb_sayline();
void sb_lineid(char *, int);
void sb_flush_ids(int);
void sb_set_inactive(int);
void sb_include(int, char *);
void sb_ifndef(int, char *);
void sb_ifdef(int, char *);
void sb_if();
void sb_if_end(int);
void sb_mdef(char *, int);
void sb_mdef_end();
void sb_mformal_def(char *);
void sb_mformal_ref(char *);
void sb_mnonformal_ref(char *);
void sb_mref(char *, int, int);
void sb_mref_end();
void sb_mactual_ref(char *);
void sb_inactive();
void sb_inactive_end();
void sb_undef(char *);
void sb_pound_line();

#else

#define sb_version()
#define sb_lineno(x)
#define sb_sayline()
#define sb_lineid(x, y)
#define sb_flush_ids(x)
#define sb_set_inactive(x)
#define sb_include(x, y)
#define sb_ifndef(x, y)
#define sb_ifdef(x, y)
#define sb_if()
#define sb_if_end(x)
#define sb_mdef(x, y)
#define sb_mdef_end()
#define sb_mformal_def(x)
#define sb_mformal_ref(x)
#define sb_mnonformal_ref(x)
#define sb_mref(x, y, z)
#define sb_mref_end()
#define sb_mactual_ref(x)
#define sb_inactive()
#define sb_inactive_end()
#define sb_undef(x)
#define sb_pound_line()

#endif
