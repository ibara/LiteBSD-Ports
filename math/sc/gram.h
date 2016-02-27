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

#ifndef YY_YY_GRAM_H_INCLUDED
# define YY_YY_GRAM_H_INCLUDED
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
    STRING = 258,
    NUMBER = 259,
    FNUMBER = 260,
    RANGE = 261,
    VAR = 262,
    WORD = 263,
    PLUGIN = 264,
    COL = 265,
    S_SHOW = 266,
    S_HIDE = 267,
    S_INSERTCOL = 268,
    S_OPENCOL = 269,
    S_DELETECOL = 270,
    S_YANKCOL = 271,
    S_GETFORMAT = 272,
    S_FORMAT = 273,
    S_FMT = 274,
    S_LET = 275,
    S_LABEL = 276,
    S_LEFTSTRING = 277,
    S_RIGHTSTRING = 278,
    S_LEFTJUSTIFY = 279,
    S_RIGHTJUSTIFY = 280,
    S_CENTER = 281,
    S_COLOR = 282,
    S_ADDNOTE = 283,
    S_DELNOTE = 284,
    S_GET = 285,
    S_PUT = 286,
    S_MERGE = 287,
    S_WRITE = 288,
    S_TBL = 289,
    S_COPY = 290,
    S_MOVE = 291,
    S_ERASE = 292,
    S_YANK = 293,
    S_FILL = 294,
    S_SORT = 295,
    S_LOCK = 296,
    S_UNLOCK = 297,
    S_GOTO = 298,
    S_DEFINE = 299,
    S_UNDEFINE = 300,
    S_ABBREV = 301,
    S_UNABBREV = 302,
    S_FRAME = 303,
    S_FRAMETOP = 304,
    S_FRAMEBOTTOM = 305,
    S_FRAMELEFT = 306,
    S_FRAMERIGHT = 307,
    S_UNFRAME = 308,
    S_VALUE = 309,
    S_MDIR = 310,
    S_AUTORUN = 311,
    S_FKEY = 312,
    S_SCEXT = 313,
    S_ASCEXT = 314,
    S_TBL0EXT = 315,
    S_TBLEXT = 316,
    S_LATEXEXT = 317,
    S_SLATEXEXT = 318,
    S_TEXEXT = 319,
    S_SET = 320,
    S_UP = 321,
    S_DOWN = 322,
    S_LEFT = 323,
    S_RIGHT = 324,
    S_ENDUP = 325,
    S_ENDDOWN = 326,
    S_ENDLEFT = 327,
    S_ENDRIGHT = 328,
    S_SELECT = 329,
    S_INSERTROW = 330,
    S_OPENROW = 331,
    S_DELETEROW = 332,
    S_YANKROW = 333,
    S_PULL = 334,
    S_PULLMERGE = 335,
    S_PULLROWS = 336,
    S_PULLCOLS = 337,
    S_PULLXCHG = 338,
    S_PULLTP = 339,
    S_PULLFMT = 340,
    S_PULLCOPY = 341,
    S_WHEREAMI = 342,
    S_GETNUM = 343,
    S_FGETNUM = 344,
    S_GETSTRING = 345,
    S_GETEXP = 346,
    S_GETFMT = 347,
    S_GETFRAME = 348,
    S_GETRANGE = 349,
    S_EVAL = 350,
    S_SEVAL = 351,
    S_QUERY = 352,
    S_GETKEY = 353,
    S_ERROR = 354,
    S_RECALC = 355,
    S_REDRAW = 356,
    S_QUIT = 357,
    S_STATUS = 358,
    S_RUN = 359,
    S_PLUGIN = 360,
    S_PLUGOUT = 361,
    K_ERROR = 362,
    K_INVALID = 363,
    K_FIXED = 364,
    K_SUM = 365,
    K_PROD = 366,
    K_AVG = 367,
    K_STDDEV = 368,
    K_COUNT = 369,
    K_ROWS = 370,
    K_COLS = 371,
    K_ABS = 372,
    K_ACOS = 373,
    K_ASIN = 374,
    K_ATAN = 375,
    K_ATAN2 = 376,
    K_CEIL = 377,
    K_COS = 378,
    K_EXP = 379,
    K_FABS = 380,
    K_FLOOR = 381,
    K_HYPOT = 382,
    K_LN = 383,
    K_LOG = 384,
    K_PI = 385,
    K_POW = 386,
    K_SIN = 387,
    K_SQRT = 388,
    K_TAN = 389,
    K_DTR = 390,
    K_RTD = 391,
    K_MAX = 392,
    K_MIN = 393,
    K_RND = 394,
    K_ROUND = 395,
    K_IF = 396,
    K_PV = 397,
    K_FV = 398,
    K_PMT = 399,
    K_HOUR = 400,
    K_MINUTE = 401,
    K_SECOND = 402,
    K_MONTH = 403,
    K_DAY = 404,
    K_YEAR = 405,
    K_NOW = 406,
    K_DATE = 407,
    K_DTS = 408,
    K_TTS = 409,
    K_FMT = 410,
    K_SUBSTR = 411,
    K_UPPER = 412,
    K_LOWER = 413,
    K_CAPITAL = 414,
    K_STON = 415,
    K_EQS = 416,
    K_EXT = 417,
    K_NVAL = 418,
    K_SVAL = 419,
    K_LOOKUP = 420,
    K_HLOOKUP = 421,
    K_VLOOKUP = 422,
    K_INDEX = 423,
    K_STINDEX = 424,
    K_AUTO = 425,
    K_AUTOCALC = 426,
    K_AUTOINSERT = 427,
    K_AUTOWRAP = 428,
    K_CSLOP = 429,
    K_BYROWS = 430,
    K_BYCOLS = 431,
    K_OPTIMIZE = 432,
    K_ITERATIONS = 433,
    K_NUMERIC = 434,
    K_PRESCALE = 435,
    K_EXTFUN = 436,
    K_CELLCUR = 437,
    K_TOPROW = 438,
    K_COLOR = 439,
    K_COLORNEG = 440,
    K_COLORERR = 441,
    K_BRAILLE = 442,
    K_BLACK = 443,
    K_RED = 444,
    K_GREEN = 445,
    K_YELLOW = 446,
    K_BLUE = 447,
    K_MAGENTA = 448,
    K_CYAN = 449,
    K_WHITE = 450,
    K_TBLSTYLE = 451,
    K_TBL = 452,
    K_LATEX = 453,
    K_SLATEX = 454,
    K_TEX = 455,
    K_FRAME = 456,
    K_RNDTOEVEN = 457,
    K_FILENAME = 458,
    K_MYROW = 459,
    K_MYCOL = 460,
    K_LASTROW = 461,
    K_LASTCOL = 462,
    K_COLTOA = 463,
    K_CRACTION = 464,
    K_CRROW = 465,
    K_CRCOL = 466,
    K_ROWLIMIT = 467,
    K_COLLIMIT = 468,
    K_PAGESIZE = 469,
    K_NUMITER = 470,
    K_ERR = 471,
    K_SCRC = 472,
    K_LOCALE = 473
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 8 "gram.y" /* yacc.c:1909  */

    int ival;
    double fval;
    struct ent_ptr ent;
    struct enode *enode;
    char *sval;
    struct range_s rval;

#line 282 "gram.h" /* yacc.c:1909  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_GRAM_H_INCLUDED  */
