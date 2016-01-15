/* @(#)rgram.h 1.3 96/09/10 */
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

#define TK_SKIP		-1
#define TK_DUMMY	0
#define TK_NL		1
#define TK_SPACE	2
#define TK_FPP		3
#define TK_NUM		4
#define TK_BOS		5
#define TK_KEY		6
#define TK_FORMAT	7	/* Fortran FORMAT */
#define TK_IMPLICIT	8	/* Fortran IMPLICIT */
#define TK_LPAR		9	/* ( */
#define TK_RPAR	       10	/* ) */
#define TK_NAME	       11
#define TK_COMMA       12	/* , */
#define TK_STAR	       13	/* * */
#define TK_RDWR	       14	/* Fortran READ or WRITE */
#define TK_ASGN	       15	/* Fortran ASSIGN */
#define TK_DO	       16	/* Fortran DO */
#define TK_TYPE0       17	/* Fortran90 TYPE */
#define TK_TYPE	       18	/* Fortran types */
#define TK_SCLN	       19	/* ; */
#define TK_BOZ	       20	/* BOZ constant */
#define TK_IMPLICIT1   21	/* fake token for IMPLICIT */
#define TK_NAME0       22	/* fake token for IMPLICIT */
#define TK_RECUR       23	/* Fortran 90 RECURSIVE */
#define TK_MINUS       24	/* - */
#define TK_EOC         25       /* End of a comment */

#define ST_NULL		0
#define ST_FPP		1 
#define ST_NL		2 
#define ST_BOS		3
#define ST_SPLIT	4
#define ST_SPLIT1	5
#define ST_EXCEPT	6
#define ST_TYPE		7
#define ST_TYPE1	8
#define ST_RDWR		9
#define ST_DO		10
#define ST_ASSIGN	11
#define ST_ASSIGNUM	12
#define ST_IMPL		13
#define ST_IMPL0	14
#define ST_NOIMPL	15


void st_init(int);
void st_move(int);
int  st_is(int);
