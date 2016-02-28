/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2013 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_CC_TAB_H_INCLUDED
# define YY_YY_CC_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    LPE = 258,
    LME = 259,
    LMLE = 260,
    LDVE = 261,
    LMDE = 262,
    LRSHE = 263,
    LLSHE = 264,
    LANDE = 265,
    LXORE = 266,
    LORE = 267,
    LOROR = 268,
    LANDAND = 269,
    LEQ = 270,
    LNE = 271,
    LLE = 272,
    LGE = 273,
    LLSH = 274,
    LRSH = 275,
    LMM = 276,
    LPP = 277,
    LMG = 278,
    LNAME = 279,
    LTYPE = 280,
    LFCONST = 281,
    LDCONST = 282,
    LCONST = 283,
    LLCONST = 284,
    LUCONST = 285,
    LULCONST = 286,
    LVLCONST = 287,
    LUVLCONST = 288,
    LSTRING = 289,
    LLSTRING = 290,
    LAUTO = 291,
    LBREAK = 292,
    LCASE = 293,
    LCHAR = 294,
    LCONTINUE = 295,
    LDEFAULT = 296,
    LDO = 297,
    LDOUBLE = 298,
    LELSE = 299,
    LEXTERN = 300,
    LFLOAT = 301,
    LFOR = 302,
    LGOTO = 303,
    LIF = 304,
    LINT = 305,
    LLONG = 306,
    LREGISTER = 307,
    LRETURN = 308,
    LSHORT = 309,
    LSIZEOF = 310,
    LUSED = 311,
    LSTATIC = 312,
    LSTRUCT = 313,
    LSWITCH = 314,
    LTYPEDEF = 315,
    LTYPESTR = 316,
    LUNION = 317,
    LUNSIGNED = 318,
    LWHILE = 319,
    LVOID = 320,
    LENUM = 321,
    LSIGNED = 322,
    LCONSTNT = 323,
    LVOLATILE = 324,
    LSET = 325,
    LSIGNOF = 326,
    LRESTRICT = 327,
    LINLINE = 328
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 4 "cc.y" /* yacc.c:1909  */

	Node*	node;
	Sym*	sym;
	Type*	type;
	struct
	{
		Type*	t;
		uchar	c;
	} tycl;
	struct
	{
		Type*	t1;
		Type*	t2;
		Type*	t3;
		uchar	c;
	} tyty;
	struct
	{
		char*	s;
		long	l;
	} sval;
	long	lval;
	double	dval;
	vlong	vval;

#line 154 "cc.tab.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_CC_TAB_H_INCLUDED  */
