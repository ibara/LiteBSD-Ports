/* @(#)eval.y 1.9 96/09/10 */
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

%term YYQWE YYCOLON 
%term YYOR YYAND YYNOT YYEQV YYNEQV
%term YYBITOR YYBITAND YYBITXOR YYBITNOT
%term YYEQ YYNE YYLT YYGT YYLE YYGE
%term YYLS YYRS YYADD YYSUB YYMUL YYDIV YYREM YYDEG
%term YYLPAR YYRPAR YYNUM YYCOMMA
%term YYSTOP YYBADLEX

%right YYQWE YYCOLON
%left YYAND YYOR
%left YYEQV YYNEQV
%left YYBITAND YYBITOR YYBITXOR
%binary YYEQ YYNE
%binary YYLT YYGT YYLE YYGE
%left YYLS YYRS
%left YYADD YYSUB 
%left YYMUL YYDIV YYREM
%right YYDEG
%right YYNOT YYBITNOT
%left YYCOMMA 
%%
S:	exp YYSTOP = {return $1;}

exp:	  exp YYDEG exp = { 
		int i, res = 1;
		if ($3 < 0) res = 0;
		else {
			for (i=0; i < $3; i++) 
				res *= $1;
		}
		$$ = res; }
	| exp YYMUL exp = { $$ = $1 * $3; }
	| exp YYDIV exp = { 
		if ($3 == 0) {
			fppmess(WARN_FPP_EXPR);
			$$ = 0;
		}
		else
			$$ = $1 / $3; }
	| exp YYREM exp = { $$ = $1 % $3; }
	| exp YYADD exp = { $$ = $1 + $3; }
	| exp YYSUB exp = { $$ = $1 - $3; }
	| exp YYLS  exp = { $$ = $1 << $3; }
	| exp YYRS  exp = { $$ = $1 >> $3; }
	| exp YYLT  exp = { $$ = $1 < $3; }
	| exp YYLE  exp = { $$ = $1 <= $3; }
	| exp YYGT  exp = { $$ = $1 > $3; }
	| exp YYGE  exp = { $$ = $1 >= $3; }
	| exp YYEQ  exp = { $$ = $1 == $3; }
	| exp YYNE  exp = { $$ = $1 != $3; }
	| exp YYBITAND exp = { $$ = $1 & $3; }
	| exp YYBITXOR exp = { $$ = $1 ^ $3; }
	| exp YYBITOR  exp = { $$ = $1 | $3; }
	| exp YYAND exp = { $$ = $1 && $3; }
	| exp YYOR  exp = { $$ = $1 || $3; }
	| exp YYNEQV exp = { $$ = !$1 && $3 || $1 && !$3; }
	| exp YYEQV exp = { $$ = !$1 && !$3 || $1 && $3; }
	| exp YYQWE exp YYCOLON exp = { $$ = $1 ? $3 : $5; }
	| exp YYCOMMA  exp = { $$ = $3; }
	| term = { $$ = $1; }

term:	  YYSUB term = { $$ = -$2; }
	| YYNOT    term = { $$ = !$2; }
	| YYBITNOT term = { $$ = ~$2; }
	| YYLPAR exp YYRPAR = { $$ = $2; }
	| YYNUM = { $$ = $1; }
%%
#include "fpp.h"
#include "symtab.h"
#include "rgram.h"
#include "service.h"
#include "sb.h"

void
yyerror( const char *s ) {
	fppmess(ERR_IF,s);
}

int
yylex() {
	int	lmode;
	int	n;
	char	c, *s;
	SymPtr	sym;
	Token   *tokp;

	lmode = (mmode & ~(MOD_SPACE | MOD_CONT)) | MOD_IF;
	tokp = skipbl(lmode);
	switch (tokp->token[0]) {
	case '(':	return YYLPAR;
	case ')':	return YYRPAR;
	case ',':	return YYCOMMA;
	case '%':	return YYREM;
	case '/':	return YYDIV;
	case '*':	if (tokp->token[1] == '*')
				return YYDEG;
			else	return YYMUL;
	case '-':	return YYSUB;
	case '+':	return YYADD;
	case '~':	return YYBITNOT;
	case '^':	return YYBITXOR;
	case '?':	return YYQWE;
	case ':':	return YYCOLON;
	case '|':	
		if (tokp->token[1] == '|')
			return YYOR;
		else	return YYBITOR;
	case '&':
		if (tokp->token[1] == '&')
			return YYAND;
		else	return YYBITAND;
	case '!':
		if (tokp->token[1] == '=')
			return YYNE;
		else	return YYNOT;
	case '=':
		if (tokp->token[1] == '=')
			return YYEQ;
		else 	return YYSTOP;
	case '<':
		if (tokp->token[1] == '=')
			return YYLE;
		else if (tokp->token[1] == '<') 
			return YYLS;
		else 	return YYLT;
	case '>':
		if (tokp->token[1] == '=')
			return YYGE;
		else if (tokp->token[1] == '>') 
			return YYRS;
		else 	return YYGT;
	case '\n':	return YYSTOP;
	case '.':
		skipbl(lmode);
		sym = symget(tokp->token,CL_FOP);
		if (sym == NULL) return YYBADLEX;
		n = symvali(sym);
		switch (n) {
		case FTN_TRUE: n = YYNUM; yylval = 1;
			break;
		case FTN_FALSE: n = YYNUM; yylval = 0;
			break;
		case FTN_EQ : n = YYEQ;
			break;
		case FTN_NE : n = YYNE;
			break;
		case FTN_LT : n = YYLT;
			break;
		case FTN_LE : n = YYLE;
			break;
		case FTN_GT : n = YYGT;
			break;
		case FTN_GE : n = YYGE;
			break;
		case FTN_AND : n = YYAND;
			break;
		case FTN_OR : n = YYOR;
			break;
		case FTN_NEQV :
		case FTN_XOR : n = YYNEQV;	/* these two are the same */
			break;
		case FTN_EQV : n = YYEQV;
			break;
		case FTN_NOT : n = YYNOT;
			break;
		default:
			return YYBADLEX;
		}
		skipbl(lmode);
		if (tokp->token[0] != '.') 
			return YYBADLEX;
		else	return n;
	default: 
		if (tokp->val == TK_NAME) {
			if (!strcmp(tokp->token,"defined")) {
				int save;

				save = dosubstfl;
				dosubstfl = 0;
				tokp = skipbl(lmode);
				if (tokp->val == TK_NAME) {
					dosubstfl = save;
					if (symget(tokp->token,CL_NM))
						yylval = 1;
					else
						yylval = 0;
					return YYNUM;
				}	
				else if (tokp->token[0] != '(') {
					dosubstfl = save;
					return YYBADLEX;
				}
				tokp = skipbl(lmode);
				dosubstfl = save;
				if (tokp->val == TK_NAME && symget(tokp->token,CL_NM))
					yylval = 1;
				else
					yylval = 0;
				tokp = skipbl(lmode);
				if (tokp->token[0] != ')') 
					return YYBADLEX;
				return YYNUM;
			}
			else {
				if (sbfl) {
					sb_mref(tokp->token, 0, 0);
					sb_mref_end();
				}
				yylval = 0;
				return YYNUM;
			}
		}
		else if (tokp->val == TK_NUM) {
			s = tokp->token;
			while (c=*s++) {
				if (!is_num(c))
					return YYBADLEX;
			}
#if USE_C_HEX_CONST
			if (tokp->token[0] == '0') {
				/* C octal constant is allowed 
				 * in #if expression */
				strtoi(tokp->token,&yylval,8);
			}
			else
#endif /* USE_C_HEX_CONST */
				strtoi(tokp->token,&yylval,10);
			return YYNUM;
		}
		else if (tokp->val == TK_BOZ) {
			int	err;

			switch (lowcase(tokp->token[0])) {
			case 'b':
				tokp = skipbl(lmode);
				CHECK(tokp->token[0] == '\'' || tokp->token[0] == '"');

				/* Erase the trailing quote */
				tokp->token[--tokp->length] = 0;
				err = strtoi(tokp->token+1, &yylval, 2);
				if (err) 
					return YYBADLEX;
				else	return YYNUM;
				break;
			case 'o':
				tokp = skipbl(lmode);
				CHECK(tokp->token[0] == '\'' || tokp->token[0] == '"');
				tokp->token[--tokp->length] = 0;
				err = strtoi(tokp->token+1, &yylval, 8);
				if (err) 
					return YYBADLEX;
				else	return YYNUM;
				break;
			case 'x':
			case 'z':
				tokp = skipbl(lmode);
				CHECK(tokp->token[0] == '\'' || tokp->token[0] == '"');
				tokp->token[--tokp->length] = 0;
				err = strtoi(tokp->token+1, &yylval, 16);
				if (err) 
					return YYBADLEX;
				else	return YYNUM;
				break;
			case '\'':
			case '"':
				c = tokp->token[tokp->length - 1];
				if (lowcase(c) == 'o') {
					tokp->length -= 2;
					tokp->token[tokp->length] = 0;
					err = strtoi(tokp->token+1, &yylval, 8);
				}
				else if (lowcase(c) == 'x') {
					tokp->length -= 2;
					tokp->token[tokp->length] = 0;
					err = strtoi(tokp->token+1, &yylval, 16);
				}
				else return YYBADLEX;
				if (err) 
					return YYBADLEX;
				else	return YYNUM;
#if USE_C_HEX_CONST
			case '0':
				CHECK(lowcase(tokp->token[1]) == 'x');
				err = strtoi(tokp->token+2, &yylval, 16);
				if (err) 
					return YYBADLEX;
				else	return YYNUM;
				break;
#endif /* USE_C_HEX_CONST */
			default:
				CHECK(0);
				return YYBADLEX;
			}
		}
		else
			return YYBADLEX;
	}
/*	return YYBADLEX; */
}
