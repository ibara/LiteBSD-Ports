/* @(#)rgram.c 1.3 96/07/24 */
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

#include "fpp.h"
#include "rgram.h"

extern int	f77fl;		/* 1 - F77, 0 - F90 */
extern int	fixedformfl;	/* 1 - fixed form, 0 - free form */

typedef struct _node {
	int	token;
	struct _node *next;
	struct _node *alt;
}	Node;

static Node st0;

/****************************** F77 reduced grammar **************************/

/* here we wait for a preprocessor directive */
static Node stFPP = { TK_DUMMY, &st0, &st0 };

/* ASSIGN lab TO ... possible name splitting */
static Node stASGNex= { TK_DUMMY, &st0, &st0 };
static Node stASGN = { TK_NUM, &stASGNex, &st0 };

/* DO lab I= ... possible name splitting */
static Node stDO = { TK_DUMMY, &st0, &st0 };

/* READ(10'3) syntax exception */
static Node stRDWRex= { TK_DUMMY, &st0, &st0 };
static Node stRDWR2 = { TK_NAME, &stRDWRex, &st0};	/* READ(UNIT ... */
static Node stRDWR1 = { TK_NUM,  &stRDWRex, &stRDWR2};	/* READ(10 ... */
static Node stRDWR  = { TK_LPAR, &stRDWR1, &st0};	/* READ( ... */

/* format exceptions of macro substitution */
static Node stFMTex = { TK_DUMMY, &st0, &st0};		
static Node stFMT = { TK_LPAR, &stFMTex, &st0};	/* FORMAT( ... */

/* CHARACTER*8H12 mustn't produce hollerith constant */
static Node stTYPEex1 =  {TK_DUMMY, &st0, &st0};
static Node stTYPEex = {TK_NUM, &stTYPEex1, &st0};	/* TYPE*8 ... */
static Node stTYPE = {TK_STAR, &stTYPEex, &st0 };	/* TYPE* ... */

/* being in stBOS means to be at the beginning of a statement */
static Node st90IMPL;
static Node st90IMPLa;

static Node stBOS6 = {TK_DO, &stDO, &st0 };
static Node stBOS5 = {TK_ASGN, &stASGN, &stBOS6 };
static Node stBOS4 = {TK_RDWR, &stRDWR, &stBOS5 };
static Node stBOS3 = {TK_IMPLICIT, &st90IMPL, &stBOS4 };
static Node stBOS2 = {TK_FORMAT, &stFMT, &stBOS3 };
static Node stBOS1 = {TK_TYPE, &stTYPE, &stBOS2 };
static Node stBOS  = {TK_KEY, &st0, &stBOS1 };

/* being in stNL means to be at the beginning of a line */
static Node stNL3 = { TK_BOS, &stBOS , &st0 };
static Node stNL2 = { TK_NUM, &stNL3 , &stNL3 };
static Node stNL1 = { TK_DUMMY, &stNL2, &st0 };
static Node stNL  = { TK_FPP, &stFPP, &stNL1 };

/* here we wait only for newline & implicit tokens*/
static Node st0_2 = { TK_IMPLICIT,  &st90IMPL, 0 };
static Node st0_1 = { TK_IMPLICIT1, &st90IMPLa, &st0_2 };
static Node st0   = { TK_NL, &stNL, &st0_1 };

/****************************** F90 fixed form reduced grammar ***************/

static Node st90TYPEex1= {TK_DUMMY, &st0, &st0};
static Node st90TYPEex = {TK_NUM, &stTYPEex1, &st0};	/* TYPE*8 ... */
static Node st90TYPE3  = {TK_STAR, &stTYPEex, &st0 };	/* TYPE* ... */
static Node st90TYPE2  = {TK_RPAR, &st90TYPEex1, &st0 };
static Node st90TYPE1  = {TK_NAME, &st90TYPE2, &st0 };
static Node st90TYPE   = {TK_LPAR, &st90TYPE1, &st0 };

static Node st90REC1 = {TK_TYPE, &st90TYPE3, &st0 };
static Node st90REC = {TK_TYPE0, &st90TYPE,  &st90REC1 };

static Node stx90BOS8 = {TK_RECUR,&st90REC,     &st0 };
static Node stx90BOS7 = {TK_DO,   &stDO,     &stx90BOS8 };
static Node stx90BOS6 = {TK_TYPE0,&st90TYPE, &stx90BOS7 };
static Node stx90BOS5 = {TK_ASGN, &stASGN,   &stx90BOS6 };
static Node stx90BOS4 = {TK_RDWR, &stRDWR,   &stx90BOS5 };
static Node stx90BOS3 = {TK_IMPLICIT, &st90IMPL, &stx90BOS4 };
static Node stx90BOS2 = {TK_FORMAT,&stFMT,   &stx90BOS3 };
static Node stx90BOS1 = {TK_TYPE, &st90TYPE3, &stx90BOS2 };
static Node stx90BOS  = {TK_KEY,  &st0,      &stx90BOS1 };

static Node stx90NL3 = { TK_BOS, &stx90BOS , &st0 };
static Node stx90NL2 = { TK_NUM, &stx90NL3 , &stx90NL3 };
static Node stx90NL1 = { TK_DUMMY, &stx90NL2 , &st0 };
static Node stx90NL  = { TK_FPP, &stFPP, &stx90NL1 };

static Node stx90_3= { TK_IMPLICIT, &st90IMPL, 0 };
static Node stx90_2= { TK_IMPLICIT1, &st90IMPLa, &stx90_3 };
static Node stx90_1= { TK_SCLN, &stx90BOS, &stx90_2 };
static Node stx90  = { TK_NL, &stx90NL, &stx90_1 };

/****************************** F90 free form reduced grammar ****************/
static Node st90;

static Node st90IMPLa;
static Node st90IMPLa2	= { TK_MINUS, &st90IMPLa, &st0 };
static Node st90IMPLa1	= { TK_COMMA, &st90IMPLa, &st90IMPLa2 };
static Node st90IMPLa	= { TK_NAME0, &st90IMPLa1, &st0 };

static Node st90IMPLno	= { TK_DUMMY, &st0, &st0 }; 
static Node st90IMPL3	= { TK_LPAR, &st90IMPLno, &st0 }; 
static Node st90IMPL2	= { TK_STAR, &st90IMPL3, &st0 };
static Node st90IMPL1	= { TK_TYPE, &st90IMPL2, &st0 };
static Node st90IMPL	= { TK_TYPE0, &st90IMPL3, &st90IMPL1 };

static Node st90BOS2= { TK_IMPLICIT, &st90IMPL, &st0 };
static Node st90BOS1= { TK_FORMAT, &stFMT, &st90BOS2 };
static Node st90BOS = { TK_NUM, &st90BOS1, &st90BOS1 };

static Node st90NL1 = { TK_DUMMY, &st90BOS, &st0 };
static Node st90NL  = { TK_FPP, &stFPP, &st90NL1 };

static Node st90_3= { TK_IMPLICIT, &st90IMPL, 0 };
static Node st90_2= { TK_IMPLICIT1, &st90IMPLa, &st90_3 };
static Node st90_1= { TK_SCLN, &st90BOS, &st90_2 };
static Node st90  = { TK_NL, &st90NL, &st90_1 };

static Node *curst = &st0;

/**************************************************************** Functions **/
void
st_init( int n ) {
	CHECK(n>=0 && n<=2);
	switch (n) {
	case 0:	break;		/* f77 fixed form */
	case 1: st0 = stx90;	/* f90 fixed form */
		break;
	case 2: st0 = st90;	/* f90 free form */
		break;
	}
	st_move(TK_NL);
}

void
st_move( int tk) {
	if (tk == TK_SKIP) {
		curst = &st0;
		return;
	}
	for (;;) { 
		if (curst->token == tk) {
			curst = curst->next;
			break;
		}
		if (!curst->alt) {
			curst = &st0;
			break;
		}
		curst = curst->alt;
	}
}

int
st_is(int st) {

	if (f77fl) {
		switch (st) {
		case ST_FPP:	if(curst == &stFPP) return 1;
				break;
		case ST_NL:	if(curst == &stNL) return 1;
				break;
		case ST_BOS:	if(curst == &stBOS) 
					return 1;		
				break;
		case ST_DO:	if(curst == &stDO) 
					return 1;		
				break;
		case ST_ASSIGN: if(curst == &stASGN) 
					return 1;		
				break;
		case ST_ASSIGNUM: if(curst == &stASGNex) 
					return 1;		
				break;
		case ST_EXCEPT:	if(curst == &stFMTex) 
					return 1;
				break;
		case ST_TYPE:	if(curst == &stTYPEex) return 1;
				break;
		case ST_TYPE1:	if(curst == &stTYPE || curst == &stTYPEex1) 
					return 1;
				break;
		case ST_RDWR:	if(curst == &stRDWRex) return 1;
				break;
		case ST_IMPL:	if (curst == &st90IMPLa) return 1;
				break;
		case ST_IMPL0:	if (curst == &st90IMPL) return 1;
				break;
		case ST_NOIMPL:	if (curst == &st90IMPLno) return 1;
				break;
		default:	break;
		}
		return 0;
	} else if (fixedformfl) {
		switch (st) {
		case ST_FPP:	if(curst == &stFPP) return 1;
				break;
		case ST_NL:	if(curst == &stx90NL) return 1;
				break;
		case ST_BOS:	if(curst == &stx90BOS || curst == &st90REC) 
					return 1;
				break;
		case ST_DO:	if(curst == &stDO) 
					return 1;		
				break;
		case ST_ASSIGN: if(curst == &stASGN) 
					return 1;		
				break;
		case ST_ASSIGNUM: if(curst == &stASGNex) 
					return 1;		
				break;
		case ST_EXCEPT:	if(curst == &stFMTex) 
					return 1;
				break;
		case ST_TYPE:	if(curst == &stTYPEex) return 1;
				break;
		case ST_TYPE1:	if(curst == &stTYPE || curst == &stTYPEex1) 
					return 1;
				break;
		case ST_RDWR:	if(curst == &stRDWRex) return 1;
				break;
		case ST_IMPL:	if (curst == &st90IMPLa) return 1;
				break;
		case ST_IMPL0:	if (curst == &st90IMPL) return 1;
				break;
		case ST_NOIMPL:	if (curst == &st90IMPLno) return 1;
				break;
		default:	break;
		}
		return 0;
	} else {
		switch (st) {
		case ST_FPP:	if(curst == &stFPP) return 1;
				break;
		case ST_NL:	if(curst == &st90NL) return 1;
				break;
		case ST_BOS:	if(curst == &st90BOS || curst == &st90BOS1)
					return 1;
				break;
		case ST_EXCEPT:	if(curst == &stFMTex) return 1;
				break;
		case ST_IMPL:	if (curst == &st90IMPLa) return 1;
				break;
		case ST_IMPL0:	if (curst == &st90IMPL) return 1;
				break;
		case ST_NOIMPL:	if (curst == &st90IMPLno) return 1;
				break;
		default:	break;
		}
		return 0;
	}
}
