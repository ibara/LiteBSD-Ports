/* A Bison parser, made by GNU Bison 3.0.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "3.0.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* Copy the first part of user declarations.  */
#line 3 "gram.y" /* yacc.c:339  */

#include "sc.h"
#include <locale.h>

#line 71 "gram.c" /* yacc.c:339  */

# ifndef YY_NULLPTR
#  if defined __cplusplus && 201103L <= __cplusplus
#   define YY_NULLPTR nullptr
#  else
#   define YY_NULLPTR 0
#  endif
# endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* In a future release of Bison, this section will be replaced
   by #include "gram.h".  */
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
#line 8 "gram.y" /* yacc.c:355  */

    int ival;
    double fval;
    struct ent_ptr ent;
    struct enode *enode;
    char *sval;
    struct range_s rval;

#line 339 "gram.c" /* yacc.c:355  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_GRAM_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 354 "gram.c" /* yacc.c:358  */

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif

#ifndef YY_ATTRIBUTE
# if (defined __GNUC__                                               \
      && (2 < __GNUC__ || (__GNUC__ == 2 && 96 <= __GNUC_MINOR__)))  \
     || defined __SUNPRO_C && 0x5110 <= __SUNPRO_C
#  define YY_ATTRIBUTE(Spec) __attribute__(Spec)
# else
#  define YY_ATTRIBUTE(Spec) /* empty */
# endif
#endif

#ifndef YY_ATTRIBUTE_PURE
# define YY_ATTRIBUTE_PURE   YY_ATTRIBUTE ((__pure__))
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# define YY_ATTRIBUTE_UNUSED YY_ATTRIBUTE ((__unused__))
#endif

#if !defined _Noreturn \
     && (!defined __STDC_VERSION__ || __STDC_VERSION__ < 201112)
# if defined _MSC_VER && 1200 <= _MSC_VER
#  define _Noreturn __declspec (noreturn)
# else
#  define _Noreturn YY_ATTRIBUTE ((__noreturn__))
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(E) ((void) (E))
#else
# define YYUSE(E) /* empty */
#endif

#if defined __GNUC__ && 407 <= __GNUC__ * 100 + __GNUC_MINOR__
/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN \
    _Pragma ("GCC diagnostic push") \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")\
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# define YY_IGNORE_MAYBE_UNINITIALIZED_END \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif


#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYSIZE_T yynewbytes;                                            \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / sizeof (*yyptr);                          \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, (Count) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYSIZE_T yyi;                         \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  240
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   2910

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  242
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  13
/* YYNRULES -- Number of rules.  */
#define YYNRULES  424
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  891

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   473

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   227,     2,   230,   241,   233,   223,     2,
     236,   237,   231,   228,   238,   229,   239,   232,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,   221,   219,
     224,   225,   226,   220,   235,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,   234,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,   222,     2,   240,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96,    97,    98,    99,   100,   101,   102,   103,   104,
     105,   106,   107,   108,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   188,   189,   190,   191,   192,   193,   194,
     195,   196,   197,   198,   199,   200,   201,   202,   203,   204,
     205,   206,   207,   208,   209,   210,   211,   212,   213,   214,
     215,   216,   217,   218
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   261,   261,   263,   276,   278,   280,   282,   285,   289,
     292,   296,   299,   303,   318,   327,   328,   334,   336,   338,
     340,   342,   355,   362,   368,   372,   377,   389,   390,   391,
     393,   394,   396,   398,   399,   404,   406,   410,   415,   419,
     422,   424,   426,   430,   432,   435,   437,   439,   443,   445,
     446,   448,   468,   469,   482,   483,   497,   506,   508,   511,
     513,   515,   517,   518,   520,   521,   523,   527,   529,   531,
     532,   534,   536,   538,   540,   542,   544,   548,   551,   555,
     557,   558,   562,   566,   570,   572,   574,   576,   577,   578,
     588,   589,   590,   591,   592,   593,   594,   597,   614,   629,
     632,   639,   642,   649,   652,   659,   662,   669,   671,   678,
     682,   683,   685,   686,   687,   688,   689,   690,   691,   692,
     693,   694,   695,   696,   697,   698,   709,   710,   711,   712,
     713,   714,   715,   716,   717,   729,   730,   735,   749,   761,
     766,   767,   780,   792,   793,   798,   810,   822,   827,   828,
     839,   840,   841,   842,   843,   844,   845,   846,   847,   849,
     855,   861,   865,   870,   872,   874,   878,   883,   885,   887,
     892,   897,   899,   901,   905,   910,   912,   914,   915,   917,
     918,   919,   923,   928,   930,   932,   933,   934,   935,   937,
     938,   939,   941,   942,   943,   945,   946,   948,   949,   950,
     951,   952,   953,   954,   958,   966,   967,   977,   979,   981,
     985,   986,   988,   989,   990,   992,   995,   998,  1001,  1004,
    1007,  1010,  1013,  1016,  1019,  1022,  1025,  1028,  1030,  1033,
    1036,  1038,  1040,  1042,  1043,  1044,  1045,  1046,  1047,  1048,
    1049,  1050,  1051,  1052,  1053,  1054,  1055,  1056,  1057,  1058,
    1059,  1060,  1061,  1062,  1063,  1065,  1066,  1067,  1069,  1070,
    1071,  1072,  1073,  1074,  1075,  1076,  1078,  1082,  1084,  1085,
    1086,  1087,  1088,  1089,  1090,  1091,  1092,  1094,  1096,  1099,
    1101,  1103,  1106,  1109,  1112,  1115,  1117,  1119,  1122,  1123,
    1124,  1125,  1127,  1128,  1129,  1130,  1131,  1132,  1133,  1134,
    1135,  1136,  1137,  1138,  1139,  1140,  1141,  1142,  1143,  1144,
    1145,  1146,  1147,  1148,  1149,  1150,  1151,  1152,  1156,  1157,
    1158,  1159,  1160,  1161,  1162,  1163,  1164,  1165,  1166,  1167,
    1168,  1169,  1170,  1171,  1172,  1173,  1174,  1177,  1178,  1181,
    1182,  1185,  1186,  1188,  1190,  1192,  1195,  1196,  1199,  1200,
    1201,  1202,  1205,  1206,  1218,  1219,  1223,  1224,  1225,  1226,
    1227,  1228,  1229,  1230,  1231,  1232,  1233,  1234,  1235,  1236,
    1237,  1238,  1239,  1240,  1241,  1242,  1243,  1244,  1245,  1246,
    1247,  1248,  1249,  1250,  1251,  1252,  1253,  1254,  1255,  1256,
    1257,  1258,  1265,  1271,  1277,  1278,  1279,  1280,  1281,  1282,
    1283,  1284,  1285,  1286,  1287,  1288,  1289,  1290,  1291,  1292,
    1293,  1294,  1295,  1296,  1297,  1298,  1299,  1300,  1301,  1317,
    1322,  1330,  1334,  1336,  1340
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "STRING", "NUMBER", "FNUMBER", "RANGE",
  "VAR", "WORD", "PLUGIN", "COL", "S_SHOW", "S_HIDE", "S_INSERTCOL",
  "S_OPENCOL", "S_DELETECOL", "S_YANKCOL", "S_GETFORMAT", "S_FORMAT",
  "S_FMT", "S_LET", "S_LABEL", "S_LEFTSTRING", "S_RIGHTSTRING",
  "S_LEFTJUSTIFY", "S_RIGHTJUSTIFY", "S_CENTER", "S_COLOR", "S_ADDNOTE",
  "S_DELNOTE", "S_GET", "S_PUT", "S_MERGE", "S_WRITE", "S_TBL", "S_COPY",
  "S_MOVE", "S_ERASE", "S_YANK", "S_FILL", "S_SORT", "S_LOCK", "S_UNLOCK",
  "S_GOTO", "S_DEFINE", "S_UNDEFINE", "S_ABBREV", "S_UNABBREV", "S_FRAME",
  "S_FRAMETOP", "S_FRAMEBOTTOM", "S_FRAMELEFT", "S_FRAMERIGHT",
  "S_UNFRAME", "S_VALUE", "S_MDIR", "S_AUTORUN", "S_FKEY", "S_SCEXT",
  "S_ASCEXT", "S_TBL0EXT", "S_TBLEXT", "S_LATEXEXT", "S_SLATEXEXT",
  "S_TEXEXT", "S_SET", "S_UP", "S_DOWN", "S_LEFT", "S_RIGHT", "S_ENDUP",
  "S_ENDDOWN", "S_ENDLEFT", "S_ENDRIGHT", "S_SELECT", "S_INSERTROW",
  "S_OPENROW", "S_DELETEROW", "S_YANKROW", "S_PULL", "S_PULLMERGE",
  "S_PULLROWS", "S_PULLCOLS", "S_PULLXCHG", "S_PULLTP", "S_PULLFMT",
  "S_PULLCOPY", "S_WHEREAMI", "S_GETNUM", "S_FGETNUM", "S_GETSTRING",
  "S_GETEXP", "S_GETFMT", "S_GETFRAME", "S_GETRANGE", "S_EVAL", "S_SEVAL",
  "S_QUERY", "S_GETKEY", "S_ERROR", "S_RECALC", "S_REDRAW", "S_QUIT",
  "S_STATUS", "S_RUN", "S_PLUGIN", "S_PLUGOUT", "K_ERROR", "K_INVALID",
  "K_FIXED", "K_SUM", "K_PROD", "K_AVG", "K_STDDEV", "K_COUNT", "K_ROWS",
  "K_COLS", "K_ABS", "K_ACOS", "K_ASIN", "K_ATAN", "K_ATAN2", "K_CEIL",
  "K_COS", "K_EXP", "K_FABS", "K_FLOOR", "K_HYPOT", "K_LN", "K_LOG",
  "K_PI", "K_POW", "K_SIN", "K_SQRT", "K_TAN", "K_DTR", "K_RTD", "K_MAX",
  "K_MIN", "K_RND", "K_ROUND", "K_IF", "K_PV", "K_FV", "K_PMT", "K_HOUR",
  "K_MINUTE", "K_SECOND", "K_MONTH", "K_DAY", "K_YEAR", "K_NOW", "K_DATE",
  "K_DTS", "K_TTS", "K_FMT", "K_SUBSTR", "K_UPPER", "K_LOWER", "K_CAPITAL",
  "K_STON", "K_EQS", "K_EXT", "K_NVAL", "K_SVAL", "K_LOOKUP", "K_HLOOKUP",
  "K_VLOOKUP", "K_INDEX", "K_STINDEX", "K_AUTO", "K_AUTOCALC",
  "K_AUTOINSERT", "K_AUTOWRAP", "K_CSLOP", "K_BYROWS", "K_BYCOLS",
  "K_OPTIMIZE", "K_ITERATIONS", "K_NUMERIC", "K_PRESCALE", "K_EXTFUN",
  "K_CELLCUR", "K_TOPROW", "K_COLOR", "K_COLORNEG", "K_COLORERR",
  "K_BRAILLE", "K_BLACK", "K_RED", "K_GREEN", "K_YELLOW", "K_BLUE",
  "K_MAGENTA", "K_CYAN", "K_WHITE", "K_TBLSTYLE", "K_TBL", "K_LATEX",
  "K_SLATEX", "K_TEX", "K_FRAME", "K_RNDTOEVEN", "K_FILENAME", "K_MYROW",
  "K_MYCOL", "K_LASTROW", "K_LASTCOL", "K_COLTOA", "K_CRACTION", "K_CRROW",
  "K_CRCOL", "K_ROWLIMIT", "K_COLLIMIT", "K_PAGESIZE", "K_NUMITER",
  "K_ERR", "K_SCRC", "K_LOCALE", "';'", "'?'", "':'", "'|'", "'&'", "'<'",
  "'='", "'>'", "'!'", "'+'", "'-'", "'#'", "'*'", "'/'", "'%'", "'^'",
  "'@'", "'('", "')'", "','", "'.'", "'~'", "'$'", "$accept", "command",
  "term", "e", "expr_list", "range", "var", "var_or_range", "num",
  "strarg", "setlist", "setitem", "errlist", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,   352,   353,   354,
     355,   356,   357,   358,   359,   360,   361,   362,   363,   364,
     365,   366,   367,   368,   369,   370,   371,   372,   373,   374,
     375,   376,   377,   378,   379,   380,   381,   382,   383,   384,
     385,   386,   387,   388,   389,   390,   391,   392,   393,   394,
     395,   396,   397,   398,   399,   400,   401,   402,   403,   404,
     405,   406,   407,   408,   409,   410,   411,   412,   413,   414,
     415,   416,   417,   418,   419,   420,   421,   422,   423,   424,
     425,   426,   427,   428,   429,   430,   431,   432,   433,   434,
     435,   436,   437,   438,   439,   440,   441,   442,   443,   444,
     445,   446,   447,   448,   449,   450,   451,   452,   453,   454,
     455,   456,   457,   458,   459,   460,   461,   462,   463,   464,
     465,   466,   467,   468,   469,   470,   471,   472,   473,    59,
      63,    58,   124,    38,    60,    61,    62,    33,    43,    45,
      35,    42,    47,    37,    94,    64,    40,    41,    44,    46,
     126,    36
};
# endif

#define YYPACT_NINF -219

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-219)))

#define YYTABLE_NINF -348

#define yytable_value_is_error(Yytable_value) \
  (!!((Yytable_value) == (-348)))

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    1164,  -219,  -219,    34,    37,  -218,  -191,    -2,    27,     8,
      38,    87,    87,    87,    87,    87,    87,    87,    87,    50,
      89,    89,     4,     9,     4,    20,     4,    87,    89,    87,
      87,    87,    87,    87,    87,   129,     4,    87,    40,    46,
      87,    85,    97,   119,   139,    87,    87,     4,     4,    55,
       4,     4,     4,     4,     4,     4,     4,  -219,    57,    59,
      61,    68,  -219,  -219,  -219,  -219,    70,  -157,  -156,     1,
      29,  -219,  -219,  -219,  -219,  -219,  -219,  -219,    87,  -146,
      22,   155,   162,   182,   304,  -145,    75,   496,   496,    21,
    -219,    99,  -219,  -219,  -219,  -132,   102,   103,   105,   100,
    -110,  -102,  -101,  -100,   118,   120,   -94,   124,  -219,   -91,
     127,   -84,   143,   -77,    30,  -219,  -219,     6,   145,  -219,
     -68,   153,   -67,   -66,   -62,   -61,  -219,  -219,  -219,   -59,
     163,   -68,    87,  -219,  -219,  -219,  -219,    87,   -53,   -51,
     -68,    87,  -219,  -219,  -219,    87,    87,    87,    87,  -219,
    -219,   137,     4,  -219,  -219,    87,  -219,  -219,  -219,    87,
      87,   137,   137,   167,   168,    87,    87,  -219,    87,  -219,
    -219,  -219,    87,  -219,   169,  -219,   178,  -219,   179,  -219,
     183,  -219,  -219,  -219,  -219,   -39,  -219,  -219,  -219,  -219,
    -219,  -219,  -219,   713,  -219,  -219,  -219,  -219,  -219,   186,
     187,   -28,   191,   -25,   193,  -219,   196,   197,   -14,   206,
     -10,   209,    -6,   213,    -4,   215,     0,   216,    11,  -219,
     -18,  -219,  -219,   496,   496,   496,   847,   637,   496,  -219,
     375,  -219,  1139,    33,   219,  -219,   227,  -219,    23,    24,
    -219,   234,   236,   248,   246,  -219,  -219,   247,  -219,   249,
    -219,   258,  -219,   289,   298,   302,  -219,   309,    13,    89,
    -219,   496,   496,   496,   496,   496,  -219,  -219,  -219,    87,
      87,  -219,  -219,  -219,  -219,  -219,   137,  -219,  -219,  -219,
    -219,  -219,  -219,    87,    87,  -219,  -219,  -219,   -68,  -219,
    -219,  -219,  -219,  -219,     4,  -219,  -219,  -219,  -219,  -219,
    -219,  -219,  -219,    90,  -219,  -219,  -219,  -219,  -219,  -219,
    -219,  -219,  -219,    91,  -219,   104,   107,   114,   115,  -219,
    -219,  1105,  1158,  -219,  -219,  -219,   321,  -219,   323,  -219,
    -219,  -219,   344,  -219,   357,  -219,   363,  -219,   364,  -219,
     365,  -219,   367,   369,  -219,  -219,  -219,   496,   150,   156,
     157,   159,   161,   166,   170,   171,   173,   174,   175,   180,
     181,   184,   188,   194,   195,   201,   202,   205,  -219,   207,
     262,   266,   271,   273,   275,   277,   278,   279,   280,   281,
     282,   283,   284,   285,   288,   291,   292,   293,   295,  -219,
     296,   297,   299,   303,   308,   310,   312,   315,   316,   317,
     320,   326,   334,   348,   352,   355,   356,   374,  -219,  -219,
    -219,  -219,  -219,  -219,  -219,  -219,   376,  -219,  -219,  -219,
    -219,   377,  -219,  -219,   955,  1702,  -219,   192,   496,   496,
     496,   496,    -1,   496,   147,   154,   496,   496,   496,   496,
     496,   496,   496,   200,   385,  -219,  -219,   388,   426,  -219,
    -219,  -219,  -219,  -219,  -219,  -219,  -219,   428,   429,  -219,
    -219,   438,  -219,  1139,  1139,  1139,  1139,  1139,  -219,  -219,
    -219,  -219,  -219,  -219,   521,   360,   530,   534,   536,   581,
    -219,  -219,  -219,  -219,  -219,  -219,  -219,  -219,  -219,  -219,
    -219,  -219,  -219,  -219,  -219,  -219,  -219,  -219,  -219,  -219,
    -219,  -219,  -219,  -219,  -219,  -219,  -219,  -219,  -219,  -219,
    -219,  -219,  -219,  -219,  -219,  -219,  -219,  -219,  -219,  -219,
    -219,  -219,   354,  -219,    87,    87,    87,    87,    87,    87,
      87,   496,   496,   496,   496,   496,   496,   496,   496,   496,
     496,   496,   496,   496,   496,   496,   496,   496,   496,   496,
     314,   314,   496,   496,   496,   496,   496,   496,   496,   496,
     496,   496,   496,   496,   496,   496,   496,   496,   496,   496,
     496,   496,   496,   496,   496,   496,   496,   314,   314,   314,
     314,   314,   496,   496,   199,  -219,   592,  1139,   906,  1123,
     718,   496,   496,  2676,  2676,   496,  2676,   496,  -181,  -181,
    -181,   380,   380,   380,  -219,   607,  -219,  -219,  -219,  -219,
     618,  -219,  -219,  -219,  -219,  -219,  -219,  -219,  -219,  -219,
    -219,  -219,  -219,   620,   383,   389,   390,   393,   394,   396,
     397,   399,   400,   406,   409,   412,  1718,  1734,  1750,  1766,
     617,  1782,  1798,  1814,  1830,  1846,  1175,  1862,  1878,  1192,
    1894,  1910,  1926,  1942,  1958,  1209,   413,  -182,   415,  1226,
     416,   419,  1974,  1243,  1260,  1277,  1294,  1311,  1990,  2006,
    2022,  2038,  2054,  2070,   349,  1328,  1345,  1362,  1379,  2086,
    2102,  2118,  2134,  1396,  1413,  1430,  1447,  1464,   421,   -68,
    1481,   422,  1498,   430,  1515,   431,  1532,   432,  2150,  2166,
     496,  -219,   496,  2676,  2676,  2676,  2676,  -219,   630,  -219,
     496,  -219,   496,  -219,   496,  -219,   496,  -219,   496,  -219,
    -219,  -219,  -219,  -219,  -219,  -219,   496,  -219,  -219,  -219,
    -219,  -219,   496,  -219,  -219,   496,  -219,  -219,  -219,  -219,
    -219,   496,   496,  -219,   496,   496,  -219,  -219,   496,   496,
     496,   496,   496,  -219,  -219,  -219,  -219,  -219,  -219,  -219,
     496,   496,   496,   496,   496,  -219,  -219,  -219,  -219,   496,
     496,   496,   496,    87,   496,    87,   496,    87,   496,    87,
     496,    87,   496,  -219,  -219,  -219,  1086,  -219,  2182,  2198,
    2214,  2230,  2246,  2262,  2278,  2294,  1139,  -170,  2310,  -168,
    2326,  2342,  1549,  1566,  1583,  1600,  2358,  1617,  1634,  2374,
    1651,  2390,  2406,  2422,  2438,   420,  2454,   433,  1668,   434,
    1685,   441,   477,   442,  1155,  -219,  -219,  -219,  -219,  -219,
    -219,  -219,  -219,  -219,   496,  -219,  -219,  -219,  -219,   496,
     496,   496,   496,  -219,   496,   496,  -219,   496,  -219,  -219,
    -219,  -219,  -219,  -219,   496,   496,   496,   496,  -219,  -219,
     496,  -219,  -219,   496,  1139,  2470,  2486,  2502,  2518,  2534,
    2550,  2566,  2582,  2598,  2614,  2630,  2646,  2662,  -219,  -219,
    -219,  -219,  -219,  -219,  -219,  -219,  -219,  -219,  -219,  -219,
    -219
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       0,   211,   209,     0,    51,   130,   132,   138,   146,   179,
       0,     0,     0,     0,     0,     0,     8,    10,    12,     0,
       0,    16,     0,    41,     0,    46,     0,    56,     0,    61,
      63,     0,    68,    72,    74,    87,     0,     0,    94,     0,
      98,     0,     0,     0,     0,   108,    65,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   354,   113,   115,
     117,   119,   121,   122,   123,   124,     0,   126,   128,   134,
     142,   150,   151,   152,   153,   154,   155,   156,   157,   159,
     163,   167,   171,   175,   183,   185,     0,     0,     0,   197,
     199,     0,   203,   204,   205,   201,     0,     0,     0,     0,
       0,     0,    54,    52,     0,     0,   139,     0,   147,     0,
       0,   177,     0,     0,     0,   340,   345,     0,     0,   346,
     347,     0,     0,     0,     0,     0,     7,     9,    11,     0,
       0,     0,    13,    15,   352,   353,    22,     0,    40,    36,
     353,    35,    23,    45,    44,    43,    48,    57,    59,    62,
      64,     0,    69,    73,    75,    84,   348,   349,    88,   422,
     424,     0,     0,     0,     0,    77,    79,    80,    89,    92,
      93,    95,    97,   100,     0,   102,     0,   104,     0,   106,
       0,   107,    66,    24,    25,     0,    27,    28,    29,    30,
      31,    32,    33,   112,   114,   116,   118,   120,   125,     0,
       0,   136,     0,   144,     0,   158,     0,     0,   161,     0,
     165,     0,   169,     0,   173,     0,   181,     0,   187,   298,
     295,   296,   309,     0,     0,     0,     0,     0,     0,   324,
     189,   212,   192,   195,     0,   200,     0,   206,     0,     0,
       1,     0,     0,     0,     0,   131,   133,     0,   140,     0,
     148,     0,   180,     0,     0,     0,   341,     0,     0,     0,
      71,     3,     0,     0,     0,   109,   111,    14,    39,     0,
       0,    34,    42,    47,    58,    60,     0,    70,    81,   421,
     423,   351,   350,    85,    86,    76,    78,    90,    91,    96,
      99,   101,   103,   105,     0,   356,   357,   382,   385,   388,
     363,   362,   364,     0,   367,   370,   373,   376,   379,   391,
     394,   397,   400,     0,   410,     0,     0,     0,     0,   417,
     418,     0,     0,   355,   127,   129,     0,   135,     0,   143,
     160,   164,     0,   168,     0,   172,     0,   176,     0,   184,
       0,   186,     0,     0,   300,   293,   294,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   297,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   264,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   310,   311,
     312,   313,   314,   315,   316,   317,     0,   302,   303,   304,
     305,     0,   307,   308,     0,     0,   299,   190,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   193,     0,   198,   202,     0,     0,    50,
      49,    55,    53,   141,   149,   178,    21,    20,     0,   343,
     342,     0,   339,     2,     4,     5,     6,   110,    38,    37,
      67,    82,    83,    26,     0,     0,     0,     0,     0,     0,
     360,   361,   384,   387,   390,   366,   369,   372,   375,   378,
     381,   392,   395,   398,   401,   411,   419,   358,   359,   383,
     386,   389,   365,   368,   371,   374,   377,   380,   393,   396,
     399,   402,   412,   420,   137,   145,   162,   166,   170,   174,
     182,   188,     0,   213,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   292,     0,   326,     0,   331,
     330,     0,     0,   327,   328,     0,   329,     0,   318,   319,
     336,   320,   321,   322,   323,     0,   196,   207,   208,    18,
       0,   344,   403,   404,   405,   406,   407,   408,   409,   413,
     414,   415,   416,     0,   346,     0,   346,     0,   346,     0,
     346,     0,   346,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   346,   212,     0,     0,
     346,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   212,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,   191,     0,   332,   334,   335,   333,   194,    19,   266,
       0,   215,     0,   217,     0,   219,     0,   221,     0,   223,
     231,   232,   233,   234,   235,   236,     0,   238,   239,   240,
     241,   242,     0,   244,   245,     0,   247,   248,   249,   250,
     251,     0,     0,   225,     0,     0,   228,   252,     0,     0,
       0,     0,     0,   258,   259,   260,   261,   262,   263,   270,
       0,     0,     0,     0,     0,   273,   274,   275,   268,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   301,   306,   214,   325,    17,     0,     0,
       0,     0,     0,     0,     0,     0,   337,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   216,   218,   220,   222,   224,
     237,   243,   246,   227,     0,   226,   230,   229,   253,     0,
       0,     0,     0,   271,     0,     0,   272,     0,   269,   288,
     289,   290,   280,   279,     0,     0,     0,     0,   277,   276,
       0,   286,   285,     0,   338,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   254,   255,
     256,   257,   265,   267,   291,   282,   281,   284,   283,   278,
     287
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -219,  -219,  -203,   -87,  -107,    39,   253,   776,  -116,    62,
    -219,  -219,  -219
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,    99,   229,   796,   797,   119,   231,   121,   166,   136,
     193,   323,   167
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
     230,   232,   219,   220,   221,   201,   116,   134,   106,   117,
     256,   116,   134,   104,   117,   115,   116,   460,   111,   117,
     344,   345,   346,   134,   233,   426,   115,   116,   115,   116,
     117,   108,   117,   203,   254,   276,   443,   109,   100,   259,
     105,   102,   113,   170,   101,   281,   282,   103,   114,   171,
     439,   440,   441,   442,   129,  -347,   115,   116,   130,   185,
     117,   194,   139,   195,   144,   196,   147,   833,   834,   836,
     834,   152,   197,   198,   199,   200,   206,   217,   218,   172,
     174,   176,   178,   180,   181,   141,   142,   145,   146,   173,
     236,   115,   116,   115,   116,   117,   116,   117,   168,   117,
     240,   175,   235,   115,   116,   237,   238,   117,   239,   183,
     184,   241,   186,   187,   188,   189,   190,   191,   192,   242,
     243,   244,   245,   177,   246,   115,   116,   247,   248,   117,
     249,   250,   155,   156,   157,   115,   116,   158,   251,   117,
     425,   156,   157,   179,   523,   115,   116,   252,   253,   117,
     219,   220,   221,   259,   116,   258,   260,   117,   261,   262,
     470,   115,   116,   263,   264,   117,   265,   266,   115,   116,
     283,   284,   117,   290,   463,   464,   465,   466,   467,   269,
     271,   270,   291,   292,   272,   273,   294,   293,   115,   116,
     324,   325,   117,   326,   278,   327,   328,   329,   279,   280,
     330,   331,   219,   220,   221,   286,   116,   287,   332,   117,
     333,   289,   334,   335,   277,   222,   336,   337,   338,   339,
     341,   343,   340,   445,   591,   592,   223,   224,   225,   107,
     112,   446,   202,   342,   226,   227,   159,   160,   449,   228,
     118,   137,   138,   234,   207,   118,   450,   257,   447,   448,
     118,   255,   451,   143,   461,   444,   452,   453,   110,   454,
     204,   118,   455,   118,   120,   120,   120,   120,   120,   120,
     120,   120,   131,   132,   133,   135,   140,   135,   140,   135,
     131,   148,   120,   120,   120,   131,   120,   120,   120,   135,
     120,   118,   456,   131,   131,   131,   131,   131,   131,   120,
     135,   135,   457,   135,   135,   135,   135,   135,   135,   135,
     115,   116,   458,   459,   117,   474,   475,   219,   220,   221,
     115,   116,   471,   472,   117,   514,   118,   515,   118,   476,
     118,   120,   477,   120,   120,   120,   120,   120,   118,   478,
     479,   587,   588,   589,   590,   593,   594,   596,   516,   598,
     599,   600,   601,   602,   603,   604,   473,   161,   162,   163,
     118,   517,   164,   222,   613,   161,   162,   518,   519,   520,
     118,   521,   595,   522,   223,   224,   225,   209,   427,   597,
     118,   523,   226,   227,   211,   120,   524,   228,   118,   606,
     120,   607,   525,   526,   131,   527,   118,   528,   131,   131,
     120,   120,   529,   118,   213,   135,   530,   531,   131,   532,
     533,   534,   131,   131,   586,   222,   535,   536,   120,   131,
     537,   288,   605,   118,   538,   131,   223,   224,   225,   608,
     539,   540,   609,   610,   226,   227,   700,   541,   542,   228,
     118,   543,   611,   544,   636,   637,   638,   639,   640,   641,
     642,   643,   644,   645,   646,   647,   648,   649,   650,   651,
     652,   653,   654,   655,   659,   662,   663,   664,   665,   666,
     667,   668,   669,   670,   671,   672,   673,   674,   675,   676,
     677,   678,   679,   680,   681,   682,   683,   684,   685,   686,
     687,   690,   692,   694,   696,   698,   699,   785,   545,   219,
     220,   221,   546,   116,   703,   704,   117,   547,   705,   548,
     706,   549,   462,   550,   551,   552,   553,   554,   555,   556,
     557,   558,   120,   120,   559,   612,   215,   560,   561,   562,
     222,   563,   564,   565,   619,   566,   131,   131,   620,   567,
     621,   223,   224,   225,   568,   118,   569,   135,   570,   226,
     227,   571,   572,   573,   228,   118,   574,   614,   615,   616,
     617,   618,   575,   624,   626,   628,   630,   632,   428,   429,
     576,   430,   431,   432,   433,   434,   435,   436,   437,   438,
     439,   440,   441,   442,   577,   622,   759,   760,   578,   656,
     660,   579,   580,   623,   428,   429,   701,   430,   431,   432,
     433,   434,   435,   436,   437,   438,   439,   440,   441,   442,
     581,   707,   582,   583,   442,   786,   688,   691,   693,   695,
     697,   710,   708,   788,   709,   789,   711,   790,   712,   791,
     713,   792,   714,   715,   787,   716,   717,   799,   718,   793,
     219,   220,   221,   719,   116,   794,   720,   117,   795,   721,
       0,   742,   743,     0,   745,   798,   746,   852,   800,   774,
     776,   801,   802,   803,   804,   805,     0,     0,   778,   780,
     782,   854,   856,   806,   807,   808,   809,   810,   858,   861,
       0,     0,   811,   812,   813,   814,     0,   816,     0,   818,
       0,   820,     0,   822,     0,   824,   428,   429,     0,   430,
     431,   432,   433,   434,   435,   436,   437,   438,   439,   440,
     441,   442,   222,     0,   859,   860,     0,     0,     0,     0,
       0,     0,     0,   223,   224,   225,     0,     0,     0,     0,
       0,   226,   227,     0,     0,     0,   228,   118,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   864,     0,     0,
       0,     0,   865,   866,   867,   868,     0,   869,   870,     0,
     871,     0,     0,     0,     0,     0,     0,   872,   873,   874,
     875,     0,     0,   876,     0,     0,   877,   120,   120,   120,
     120,   120,   120,   120,     0,     0,     0,     0,   122,   123,
     124,   125,   126,   127,   128,     0,     0,     0,     0,     0,
       0,     0,     0,   657,   657,   149,   150,   151,     0,   153,
     154,   165,   815,   169,   817,     0,   819,     0,   821,     0,
     823,     0,   182,     0,     0,     0,     0,     0,     0,     0,
     689,   689,   689,   689,   689,     0,   428,   429,     0,   430,
     431,   432,   433,   434,   435,   436,   437,   438,   439,   440,
     441,   442,     0,   222,   205,   726,   208,   210,   212,   214,
     216,     0,     0,     0,   223,   224,   225,     0,     0,     0,
       0,     0,   424,   227,     0,     0,     0,   228,   118,     0,
       0,     0,     0,   295,   296,   297,   298,   299,   300,   301,
     302,   303,   304,   305,   306,   307,   308,   309,   310,   311,
     312,     0,     0,     0,     0,     0,     0,     0,   267,   313,
       0,     0,     0,   268,     0,   314,     0,     0,     0,     0,
       0,     0,   315,   274,   275,   316,   317,   318,     0,     0,
     319,   320,     0,     0,     0,     0,     0,     0,     0,     0,
     321,   285,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   322,     0,     0,   347,   348,   349,   350,
     351,   352,   353,   354,   355,   356,   357,   358,   359,   360,
     361,   362,   363,   364,   365,   366,   367,   368,   369,   370,
     371,   372,   373,   374,   375,   376,   377,   378,   379,   380,
     381,   382,   383,   384,   385,   386,   387,   388,   389,   390,
     391,   392,   393,   394,   395,   396,   397,   398,   399,   400,
     401,   402,   403,   404,   405,   406,   407,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   131,     0,   131,     0,
     131,     0,   131,     0,   131,   408,   409,   410,   411,   412,
     413,   414,   415,     0,     0,   468,   469,     0,     0,     0,
     416,   417,   418,   419,   420,   421,     0,     0,     0,     0,
       0,     0,   422,   423,   584,   348,   349,   350,   351,   352,
     353,   354,   355,   356,   357,   358,   359,   360,   361,   362,
     363,   364,   365,   366,   367,   368,   369,   370,   371,   372,
     373,   374,   375,   376,   377,   378,   379,   380,   381,   382,
     383,   384,   385,   386,   387,   388,   389,   390,   391,   392,
     393,   394,   395,   396,   397,   398,   399,   400,   401,   402,
     403,   404,   405,   406,   407,   428,   429,   702,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,     0,     0,   408,   409,   410,   411,   412,   413,   414,
     415,     0,     0,     0,     0,     0,     0,     0,   416,   417,
     418,   419,   420,   421,  -210,     1,     0,     0,     0,     0,
     422,   423,     0,     2,     0,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,     0,     0,     0,     0,   480,   481,   482,   483,   484,
       0,     0,   485,     0,   486,   487,   488,   489,   490,   491,
     492,   493,   494,     0,     0,     0,     0,     0,     0,     0,
     625,   627,   629,   631,   633,   634,   635,   495,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,     0,     0,   496,     0,     0,   658,   661,   497,   498,
     499,   500,   501,     0,     0,   502,     0,   503,   504,   505,
     506,   507,   508,   509,   510,   511,   431,   432,   433,   434,
     435,   436,   437,   438,   439,   440,   441,   442,   428,   429,
     512,   430,   431,   432,   433,   434,   435,   436,   437,   438,
     439,   440,   441,   442,   428,   429,   513,   430,   431,   432,
     433,   434,   435,   436,   437,   438,   439,   440,   441,   442,
       0,     0,   862,   863,   428,   429,     0,   430,   431,   432,
     433,   434,   435,   436,   437,   438,   439,   440,   441,   442,
       0,   428,   429,   732,   430,   431,   432,   433,   434,   435,
     436,   437,   438,   439,   440,   441,   442,     0,   428,   429,
     735,   430,   431,   432,   433,   434,   435,   436,   437,   438,
     439,   440,   441,   442,     0,   428,   429,   741,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,     0,   428,   429,   744,   430,   431,   432,   433,   434,
     435,   436,   437,   438,   439,   440,   441,   442,     0,   428,
     429,   748,   430,   431,   432,   433,   434,   435,   436,   437,
     438,   439,   440,   441,   442,     0,   428,   429,   749,   430,
     431,   432,   433,   434,   435,   436,   437,   438,   439,   440,
     441,   442,     0,   428,   429,   750,   430,   431,   432,   433,
     434,   435,   436,   437,   438,   439,   440,   441,   442,     0,
     428,   429,   751,   430,   431,   432,   433,   434,   435,   436,
     437,   438,   439,   440,   441,   442,     0,   428,   429,   752,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,     0,   428,   429,   761,   430,   431,   432,
     433,   434,   435,   436,   437,   438,   439,   440,   441,   442,
       0,   428,   429,   762,   430,   431,   432,   433,   434,   435,
     436,   437,   438,   439,   440,   441,   442,     0,   428,   429,
     763,   430,   431,   432,   433,   434,   435,   436,   437,   438,
     439,   440,   441,   442,     0,   428,   429,   764,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,     0,   428,   429,   769,   430,   431,   432,   433,   434,
     435,   436,   437,   438,   439,   440,   441,   442,     0,   428,
     429,   770,   430,   431,   432,   433,   434,   435,   436,   437,
     438,   439,   440,   441,   442,     0,   428,   429,   771,   430,
     431,   432,   433,   434,   435,   436,   437,   438,   439,   440,
     441,   442,     0,   428,   429,   772,   430,   431,   432,   433,
     434,   435,   436,   437,   438,   439,   440,   441,   442,     0,
     428,   429,   773,   430,   431,   432,   433,   434,   435,   436,
     437,   438,   439,   440,   441,   442,     0,   428,   429,   775,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,     0,   428,   429,   777,   430,   431,   432,
     433,   434,   435,   436,   437,   438,   439,   440,   441,   442,
       0,   428,   429,   779,   430,   431,   432,   433,   434,   435,
     436,   437,   438,   439,   440,   441,   442,     0,   428,   429,
     781,   430,   431,   432,   433,   434,   435,   436,   437,   438,
     439,   440,   441,   442,     0,   428,   429,   839,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,     0,   428,   429,   840,   430,   431,   432,   433,   434,
     435,   436,   437,   438,   439,   440,   441,   442,     0,   428,
     429,   841,   430,   431,   432,   433,   434,   435,   436,   437,
     438,   439,   440,   441,   442,     0,   428,   429,   842,   430,
     431,   432,   433,   434,   435,   436,   437,   438,   439,   440,
     441,   442,     0,   428,   429,   844,   430,   431,   432,   433,
     434,   435,   436,   437,   438,   439,   440,   441,   442,     0,
     428,   429,   845,   430,   431,   432,   433,   434,   435,   436,
     437,   438,   439,   440,   441,   442,     0,   428,   429,   847,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,     0,   428,   429,   855,   430,   431,   432,
     433,   434,   435,   436,   437,   438,   439,   440,   441,   442,
       0,   428,   429,   857,   430,   431,   432,   433,   434,   435,
     436,   437,   438,   439,   440,   441,   442,   428,   429,   585,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   428,   429,   722,   430,   431,   432,   433,
     434,   435,   436,   437,   438,   439,   440,   441,   442,   428,
     429,   723,   430,   431,   432,   433,   434,   435,   436,   437,
     438,   439,   440,   441,   442,   428,   429,   724,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   428,   429,   725,   430,   431,   432,   433,   434,   435,
     436,   437,   438,   439,   440,   441,   442,   428,   429,   727,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   428,   429,   728,   430,   431,   432,   433,
     434,   435,   436,   437,   438,   439,   440,   441,   442,   428,
     429,   729,   430,   431,   432,   433,   434,   435,   436,   437,
     438,   439,   440,   441,   442,   428,   429,   730,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   428,   429,   731,   430,   431,   432,   433,   434,   435,
     436,   437,   438,   439,   440,   441,   442,   428,   429,   733,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   428,   429,   734,   430,   431,   432,   433,
     434,   435,   436,   437,   438,   439,   440,   441,   442,   428,
     429,   736,   430,   431,   432,   433,   434,   435,   436,   437,
     438,   439,   440,   441,   442,   428,   429,   737,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   428,   429,   738,   430,   431,   432,   433,   434,   435,
     436,   437,   438,   439,   440,   441,   442,   428,   429,   739,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   428,   429,   740,   430,   431,   432,   433,
     434,   435,   436,   437,   438,   439,   440,   441,   442,   428,
     429,   747,   430,   431,   432,   433,   434,   435,   436,   437,
     438,   439,   440,   441,   442,   428,   429,   753,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   428,   429,   754,   430,   431,   432,   433,   434,   435,
     436,   437,   438,   439,   440,   441,   442,   428,   429,   755,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   428,   429,   756,   430,   431,   432,   433,
     434,   435,   436,   437,   438,   439,   440,   441,   442,   428,
     429,   757,   430,   431,   432,   433,   434,   435,   436,   437,
     438,   439,   440,   441,   442,   428,   429,   758,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   428,   429,   765,   430,   431,   432,   433,   434,   435,
     436,   437,   438,   439,   440,   441,   442,   428,   429,   766,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   428,   429,   767,   430,   431,   432,   433,
     434,   435,   436,   437,   438,   439,   440,   441,   442,   428,
     429,   768,   430,   431,   432,   433,   434,   435,   436,   437,
     438,   439,   440,   441,   442,   428,   429,   783,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   428,   429,   784,   430,   431,   432,   433,   434,   435,
     436,   437,   438,   439,   440,   441,   442,   428,   429,   825,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   428,   429,   826,   430,   431,   432,   433,
     434,   435,   436,   437,   438,   439,   440,   441,   442,   428,
     429,   827,   430,   431,   432,   433,   434,   435,   436,   437,
     438,   439,   440,   441,   442,   428,   429,   828,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   428,   429,   829,   430,   431,   432,   433,   434,   435,
     436,   437,   438,   439,   440,   441,   442,   428,   429,   830,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   428,   429,   831,   430,   431,   432,   433,
     434,   435,   436,   437,   438,   439,   440,   441,   442,   428,
     429,   832,   430,   431,   432,   433,   434,   435,   436,   437,
     438,   439,   440,   441,   442,   428,   429,   835,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   428,   429,   837,   430,   431,   432,   433,   434,   435,
     436,   437,   438,   439,   440,   441,   442,   428,   429,   838,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   428,   429,   843,   430,   431,   432,   433,
     434,   435,   436,   437,   438,   439,   440,   441,   442,   428,
     429,   846,   430,   431,   432,   433,   434,   435,   436,   437,
     438,   439,   440,   441,   442,   428,   429,   848,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   428,   429,   849,   430,   431,   432,   433,   434,   435,
     436,   437,   438,   439,   440,   441,   442,   428,   429,   850,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   428,   429,   851,   430,   431,   432,   433,
     434,   435,   436,   437,   438,   439,   440,   441,   442,   428,
     429,   853,   430,   431,   432,   433,   434,   435,   436,   437,
     438,   439,   440,   441,   442,   428,   429,   878,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   428,   429,   879,   430,   431,   432,   433,   434,   435,
     436,   437,   438,   439,   440,   441,   442,   428,   429,   880,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   428,   429,   881,   430,   431,   432,   433,
     434,   435,   436,   437,   438,   439,   440,   441,   442,   428,
     429,   882,   430,   431,   432,   433,   434,   435,   436,   437,
     438,   439,   440,   441,   442,   428,   429,   883,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   428,   429,   884,   430,   431,   432,   433,   434,   435,
     436,   437,   438,   439,   440,   441,   442,   428,   429,   885,
     430,   431,   432,   433,   434,   435,   436,   437,   438,   439,
     440,   441,   442,   428,   429,   886,   430,   431,   432,   433,
     434,   435,   436,   437,   438,   439,   440,   441,   442,   428,
     429,   887,   430,   431,   432,   433,   434,   435,   436,   437,
     438,   439,   440,   441,   442,   428,   429,   888,   430,   431,
     432,   433,   434,   435,   436,   437,   438,   439,   440,   441,
     442,   428,   429,   889,   430,   431,   432,   433,   434,   435,
     436,   437,   438,   439,   440,   441,   442,     0,     0,   890,
    -348,  -348,  -348,  -348,   436,   437,   438,   439,   440,   441,
     442
};

static const yytype_int16 yycheck[] =
{
      87,    88,     3,     4,     5,     4,     7,     3,    10,    10,
       4,     7,     3,   231,    10,     6,     7,     4,    10,    10,
     223,   224,   225,     3,     3,   228,     6,     7,     6,     7,
      10,     4,    10,     4,     4,   151,     3,    10,     4,   221,
     231,     4,     4,     3,    10,   161,   162,    10,    10,     3,
     231,   232,   233,   234,     4,   237,     6,     7,    19,     4,
      10,     4,    23,     4,    25,     4,    27,   237,   238,   237,
     238,    32,     4,     3,   231,   231,   222,   222,     3,    40,
      41,    42,    43,    44,    45,    23,    24,    25,    26,     4,
     222,     6,     7,     6,     7,    10,     7,    10,    36,    10,
       0,     4,     3,     6,     7,     3,     3,    10,     3,    47,
      48,   221,    50,    51,    52,    53,    54,    55,    56,   221,
     221,   221,     4,     4,     4,     6,     7,   221,     4,    10,
     221,     4,     3,     4,     5,     6,     7,     8,   222,    10,
     227,     4,     5,     4,   347,     6,     7,     4,   225,    10,
       3,     4,     5,   221,     7,    10,     3,    10,   225,   225,
     276,     6,     7,   225,   225,    10,   225,     4,     6,     7,
       3,     3,    10,     4,   261,   262,   263,   264,   265,   232,
     141,   232,     4,     4,   145,   146,   225,     4,     6,     7,
       4,     4,    10,   221,   155,     4,   221,     4,   159,   160,
       4,     4,     3,     4,     5,   166,     7,   168,   222,    10,
       4,   172,   222,     4,   152,   216,   222,     4,   222,     4,
       4,   239,   222,     4,   225,   226,   227,   228,   229,   231,
     222,     4,   231,   222,   235,   236,   107,   108,     4,   240,
     241,   232,   233,   222,   222,   241,    10,   241,   225,   225,
     241,   221,     4,   233,   241,   222,    10,    10,   231,    10,
     231,   241,     4,   241,    11,    12,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    36,
      37,   241,     3,    40,    41,    42,    43,    44,    45,    46,
      47,    48,     4,    50,    51,    52,    53,    54,    55,    56,
       6,     7,    10,     4,    10,   225,   225,     3,     4,     5,
       6,     7,   283,   284,    10,     4,   241,     4,   241,   225,
     241,    78,   225,    80,    81,    82,    83,    84,   241,   225,
     225,   428,   429,   430,   431,   432,   433,   434,     4,   436,
     437,   438,   439,   440,   441,   442,   294,   228,   229,   230,
     241,     4,   233,   216,     4,   228,   229,     4,     4,     4,
     241,     4,   225,     4,   227,   228,   229,   222,     3,   225,
     241,   584,   235,   236,   222,   132,   236,   240,   241,     4,
     137,     3,   236,   236,   141,   236,   241,   236,   145,   146,
     147,   148,   236,   241,   222,   152,   236,   236,   155,   236,
     236,   236,   159,   160,   222,   216,   236,   236,   165,   166,
     236,   168,   222,   241,   236,   172,   227,   228,   229,     3,
     236,   236,     4,     4,   235,   236,   237,   236,   236,   240,
     241,   236,     4,   236,   531,   532,   533,   534,   535,   536,
     537,   538,   539,   540,   541,   542,   543,   544,   545,   546,
     547,   548,   549,   550,   551,   552,   553,   554,   555,   556,
     557,   558,   559,   560,   561,   562,   563,   564,   565,   566,
     567,   568,   569,   570,   571,   572,   573,   574,   575,   576,
     577,   578,   579,   580,   581,   582,   583,   700,   236,     3,
       4,     5,   236,     7,   591,   592,    10,   236,   595,   236,
     597,   236,   259,   236,   236,   236,   236,   236,   236,   236,
     236,   236,   269,   270,   236,     4,   222,   236,   236,   236,
     216,   236,   236,   236,     4,   236,   283,   284,     4,   236,
       4,   227,   228,   229,   236,   241,   236,   294,   236,   235,
     236,   236,   236,   236,   240,   241,   236,   197,   198,   199,
     200,   201,   236,   524,   525,   526,   527,   528,   219,   220,
     236,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   236,     4,   237,   238,   236,   550,
     551,   236,   236,   239,   219,   220,     4,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
     236,     4,   236,   236,   234,   702,   577,   578,   579,   580,
     581,   238,     4,   710,     4,   712,   237,   714,   238,   716,
     237,   718,   238,   237,     4,   238,   237,   744,   238,   726,
       3,     4,     5,   237,     7,   732,   237,    10,   735,   237,
      -1,   238,   237,    -1,   238,   742,   237,   237,   745,   238,
     238,   748,   749,   750,   751,   752,    -1,    -1,   238,   238,
     238,   238,   238,   760,   761,   762,   763,   764,   237,   237,
      -1,    -1,   769,   770,   771,   772,    -1,   774,    -1,   776,
      -1,   778,    -1,   780,    -1,   782,   219,   220,    -1,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,   216,    -1,   237,   238,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   227,   228,   229,    -1,    -1,    -1,    -1,
      -1,   235,   236,    -1,    -1,    -1,   240,   241,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   834,    -1,    -1,
      -1,    -1,   839,   840,   841,   842,    -1,   844,   845,    -1,
     847,    -1,    -1,    -1,    -1,    -1,    -1,   854,   855,   856,
     857,    -1,    -1,   860,    -1,    -1,   863,   524,   525,   526,
     527,   528,   529,   530,    -1,    -1,    -1,    -1,    12,    13,
      14,    15,    16,    17,    18,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,   550,   551,    29,    30,    31,    -1,    33,
      34,    35,   773,    37,   775,    -1,   777,    -1,   779,    -1,
     781,    -1,    46,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     577,   578,   579,   580,   581,    -1,   219,   220,    -1,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,    -1,   216,    78,   238,    80,    81,    82,    83,
      84,    -1,    -1,    -1,   227,   228,   229,    -1,    -1,    -1,
      -1,    -1,   235,   236,    -1,    -1,    -1,   240,   241,    -1,
      -1,    -1,    -1,   170,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     187,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   132,   196,
      -1,    -1,    -1,   137,    -1,   202,    -1,    -1,    -1,    -1,
      -1,    -1,   209,   147,   148,   212,   213,   214,    -1,    -1,
     217,   218,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     227,   165,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   240,    -1,    -1,   109,   110,   111,   112,
     113,   114,   115,   116,   117,   118,   119,   120,   121,   122,
     123,   124,   125,   126,   127,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,   149,   150,   151,   152,
     153,   154,   155,   156,   157,   158,   159,   160,   161,   162,
     163,   164,   165,   166,   167,   168,   169,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,   773,    -1,   775,    -1,
     777,    -1,   779,    -1,   781,   188,   189,   190,   191,   192,
     193,   194,   195,    -1,    -1,   269,   270,    -1,    -1,    -1,
     203,   204,   205,   206,   207,   208,    -1,    -1,    -1,    -1,
      -1,    -1,   215,   216,   109,   110,   111,   112,   113,   114,
     115,   116,   117,   118,   119,   120,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,   134,
     135,   136,   137,   138,   139,   140,   141,   142,   143,   144,
     145,   146,   147,   148,   149,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,   168,   169,   219,   220,   221,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234,    -1,    -1,   188,   189,   190,   191,   192,   193,   194,
     195,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   203,   204,
     205,   206,   207,   208,     0,     1,    -1,    -1,    -1,    -1,
     215,   216,    -1,     9,    -1,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      36,    37,    38,    39,    40,    41,    42,    43,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    54,    55,
      56,    57,    58,    59,    60,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    75,
      76,    77,    78,    79,    80,    81,    82,    83,    84,    85,
      86,    87,    88,    89,    90,    91,    92,    93,    94,    95,
      96,    97,    98,    99,   100,   101,   102,   103,   104,   105,
     106,    -1,    -1,    -1,    -1,   170,   171,   172,   173,   174,
      -1,    -1,   177,    -1,   179,   180,   181,   182,   183,   184,
     185,   186,   187,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
     524,   525,   526,   527,   528,   529,   530,   202,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234,    -1,    -1,   218,    -1,    -1,   550,   551,   170,   171,
     172,   173,   174,    -1,    -1,   177,    -1,   179,   180,   181,
     182,   183,   184,   185,   186,   187,   223,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,   219,   220,
     202,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,   219,   220,   218,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
      -1,    -1,   237,   238,   219,   220,    -1,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
      -1,   219,   220,   238,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,    -1,   219,   220,
     238,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,    -1,   219,   220,   238,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234,    -1,   219,   220,   238,   222,   223,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,    -1,   219,
     220,   238,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,    -1,   219,   220,   238,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,    -1,   219,   220,   238,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,    -1,
     219,   220,   238,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,    -1,   219,   220,   238,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,    -1,   219,   220,   238,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
      -1,   219,   220,   238,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,    -1,   219,   220,
     238,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,    -1,   219,   220,   238,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234,    -1,   219,   220,   238,   222,   223,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,    -1,   219,
     220,   238,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,    -1,   219,   220,   238,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,    -1,   219,   220,   238,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,    -1,
     219,   220,   238,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,    -1,   219,   220,   238,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,    -1,   219,   220,   238,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
      -1,   219,   220,   238,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,    -1,   219,   220,
     238,   222,   223,   224,   225,   226,   227,   228,   229,   230,
     231,   232,   233,   234,    -1,   219,   220,   238,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234,    -1,   219,   220,   238,   222,   223,   224,   225,   226,
     227,   228,   229,   230,   231,   232,   233,   234,    -1,   219,
     220,   238,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,    -1,   219,   220,   238,   222,
     223,   224,   225,   226,   227,   228,   229,   230,   231,   232,
     233,   234,    -1,   219,   220,   238,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,    -1,
     219,   220,   238,   222,   223,   224,   225,   226,   227,   228,
     229,   230,   231,   232,   233,   234,    -1,   219,   220,   238,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,    -1,   219,   220,   238,   222,   223,   224,
     225,   226,   227,   228,   229,   230,   231,   232,   233,   234,
      -1,   219,   220,   238,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   219,   220,   237,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   219,   220,   237,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   219,
     220,   237,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   219,   220,   237,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   219,   220,   237,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   219,   220,   237,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   219,   220,   237,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   219,
     220,   237,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   219,   220,   237,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   219,   220,   237,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   219,   220,   237,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   219,   220,   237,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   219,
     220,   237,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   219,   220,   237,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   219,   220,   237,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   219,   220,   237,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   219,   220,   237,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   219,
     220,   237,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   219,   220,   237,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   219,   220,   237,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   219,   220,   237,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   219,   220,   237,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   219,
     220,   237,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   219,   220,   237,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   219,   220,   237,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   219,   220,   237,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   219,   220,   237,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   219,
     220,   237,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   219,   220,   237,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   219,   220,   237,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   219,   220,   237,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   219,   220,   237,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   219,
     220,   237,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   219,   220,   237,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   219,   220,   237,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   219,   220,   237,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   219,   220,   237,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   219,
     220,   237,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   219,   220,   237,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   219,   220,   237,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   219,   220,   237,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   219,   220,   237,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   219,
     220,   237,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   219,   220,   237,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   219,   220,   237,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   219,   220,   237,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   219,   220,   237,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   219,
     220,   237,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   219,   220,   237,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   219,   220,   237,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   219,   220,   237,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   219,   220,   237,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   219,
     220,   237,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   219,   220,   237,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   219,   220,   237,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,   219,   220,   237,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,   219,   220,   237,   222,   223,   224,   225,
     226,   227,   228,   229,   230,   231,   232,   233,   234,   219,
     220,   237,   222,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   232,   233,   234,   219,   220,   237,   222,   223,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234,   219,   220,   237,   222,   223,   224,   225,   226,   227,
     228,   229,   230,   231,   232,   233,   234,    -1,    -1,   237,
     224,   225,   226,   227,   228,   229,   230,   231,   232,   233,
     234
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     1,     9,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    31,    32,    33,    34,    35,    36,    37,
      38,    39,    40,    41,    42,    43,    44,    45,    46,    47,
      48,    49,    50,    51,    52,    53,    54,    55,    56,    57,
      58,    59,    60,    61,    62,    63,    64,    65,    66,    67,
      68,    69,    70,    71,    72,    73,    74,    75,    76,    77,
      78,    79,    80,    81,    82,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   243,
       4,    10,     4,    10,   231,   231,    10,   231,     4,    10,
     231,    10,   222,     4,    10,     6,     7,    10,   241,   247,
     248,   249,   249,   249,   249,   249,   249,   249,   249,     4,
     247,   248,   248,   248,     3,   248,   251,   232,   233,   247,
     248,   251,   251,   233,   247,   251,   251,   247,   248,   249,
     249,   249,   247,   249,   249,     3,     4,     5,     8,   107,
     108,   228,   229,   230,   233,   249,   250,   254,   251,   249,
       3,     3,   247,     4,   247,     4,   247,     4,   247,     4,
     247,   247,   249,   251,   251,     4,   251,   251,   251,   251,
     251,   251,   251,   252,     4,     4,     4,     4,     3,   231,
     231,     4,   231,     4,   231,   249,   222,   222,   249,   222,
     249,   222,   249,   222,   249,   222,   249,   222,     3,     3,
       4,     5,   216,   227,   228,   229,   235,   236,   240,   244,
     245,   248,   245,     3,   222,     3,   222,     3,     3,     3,
       0,   221,   221,   221,   221,     4,     4,   221,     4,   221,
       4,   222,     4,   225,     4,   221,     4,   241,    10,   221,
       3,   225,   225,   225,   225,   225,     4,   249,   249,   232,
     232,   247,   247,   247,   249,   249,   250,   251,   247,   247,
     247,   250,   250,     3,     3,   249,   247,   247,   248,   247,
       4,     4,     4,     4,   225,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   187,   196,   202,   209,   212,   213,   214,   217,
     218,   227,   240,   253,     4,     4,   221,     4,   221,     4,
       4,     4,   222,     4,   222,     4,   222,     4,   222,     4,
     222,     4,   222,   239,   244,   244,   244,   109,   110,   111,
     112,   113,   114,   115,   116,   117,   118,   119,   120,   121,
     122,   123,   124,   125,   126,   127,   128,   129,   130,   131,
     132,   133,   134,   135,   136,   137,   138,   139,   140,   141,
     142,   143,   144,   145,   146,   147,   148,   149,   150,   151,
     152,   153,   154,   155,   156,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   169,   188,   189,
     190,   191,   192,   193,   194,   195,   203,   204,   205,   206,
     207,   208,   215,   216,   235,   245,   244,     3,   219,   220,
     222,   223,   224,   225,   226,   227,   228,   229,   230,   231,
     232,   233,   234,     3,   222,     4,     4,   225,   225,     4,
      10,     4,    10,    10,    10,     4,     3,     4,    10,     4,
       4,   241,   248,   245,   245,   245,   245,   245,   249,   249,
     250,   247,   247,   251,   225,   225,   225,   225,   225,   225,
     170,   171,   172,   173,   174,   177,   179,   180,   181,   182,
     183,   184,   185,   186,   187,   202,   218,   170,   171,   172,
     173,   174,   177,   179,   180,   181,   182,   183,   184,   185,
     186,   187,   202,   218,     4,     4,     4,     4,     4,     4,
       4,     4,     4,   244,   236,   236,   236,   236,   236,   236,
     236,   236,   236,   236,   236,   236,   236,   236,   236,   236,
     236,   236,   236,   236,   236,   236,   236,   236,   236,   236,
     236,   236,   236,   236,   236,   236,   236,   236,   236,   236,
     236,   236,   236,   236,   236,   236,   236,   236,   236,   236,
     236,   236,   236,   236,   236,   236,   236,   236,   236,   236,
     236,   236,   236,   236,   109,   237,   222,   245,   245,   245,
     245,   225,   226,   245,   245,   225,   245,   225,   245,   245,
     245,   245,   245,   245,   245,   222,     4,     3,     3,     4,
       4,     4,     4,     4,   197,   198,   199,   200,   201,     4,
       4,     4,     4,   239,   247,   249,   247,   249,   247,   249,
     247,   249,   247,   249,   249,   249,   245,   245,   245,   245,
     245,   245,   245,   245,   245,   245,   245,   245,   245,   245,
     245,   245,   245,   245,   245,   245,   247,   248,   249,   245,
     247,   249,   245,   245,   245,   245,   245,   245,   245,   245,
     245,   245,   245,   245,   245,   245,   245,   245,   245,   245,
     245,   245,   245,   245,   245,   245,   245,   245,   247,   248,
     245,   247,   245,   247,   245,   247,   245,   247,   245,   245,
     237,     4,   221,   245,   245,   245,   245,     4,     4,     4,
     238,   237,   238,   237,   238,   237,   238,   237,   238,   237,
     237,   237,   237,   237,   237,   237,   238,   237,   237,   237,
     237,   237,   238,   237,   237,   238,   237,   237,   237,   237,
     237,   238,   238,   237,   238,   238,   237,   237,   238,   238,
     238,   238,   238,   237,   237,   237,   237,   237,   237,   237,
     238,   238,   238,   238,   238,   237,   237,   237,   237,   238,
     238,   238,   238,   238,   238,   238,   238,   238,   238,   238,
     238,   238,   238,   237,   237,   244,   245,     4,   245,   245,
     245,   245,   245,   245,   245,   245,   245,   246,   245,   246,
     245,   245,   245,   245,   245,   245,   245,   245,   245,   245,
     245,   245,   245,   245,   245,   247,   245,   247,   245,   247,
     245,   247,   245,   247,   245,   237,   237,   237,   237,   237,
     237,   237,   237,   237,   238,   237,   237,   237,   237,   238,
     238,   238,   238,   237,   238,   238,   237,   238,   237,   237,
     237,   237,   237,   237,   238,   238,   238,   238,   237,   237,
     238,   237,   237,   238,   245,   245,   245,   245,   245,   245,
     245,   245,   245,   245,   245,   245,   245,   245,   237,   237,
     237,   237,   237,   237,   237,   237,   237,   237,   237,   237,
     237
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   242,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   243,   243,   243,   243,   243,   243,   243,   243,
     243,   243,   244,   244,   244,   244,   244,   244,   244,   244,
     244,   244,   244,   244,   244,   244,   244,   244,   244,   244,
     244,   244,   244,   244,   244,   244,   244,   244,   244,   244,
     244,   244,   244,   244,   244,   244,   244,   244,   244,   244,
     244,   244,   244,   244,   244,   244,   244,   244,   244,   244,
     244,   244,   244,   244,   244,   244,   244,   244,   244,   244,
     244,   244,   244,   244,   244,   244,   244,   244,   244,   244,
     244,   244,   244,   244,   244,   244,   244,   244,   244,   244,
     244,   244,   244,   244,   244,   244,   244,   244,   244,   244,
     244,   244,   244,   244,   244,   244,   244,   244,   244,   244,
     244,   244,   244,   244,   244,   244,   244,   244,   245,   245,
     245,   245,   245,   245,   245,   245,   245,   245,   245,   245,
     245,   245,   245,   245,   245,   245,   245,   246,   246,   247,
     247,   248,   248,   248,   248,   248,   249,   249,   250,   250,
     250,   250,   251,   251,   252,   252,   253,   253,   253,   253,
     253,   253,   253,   253,   253,   253,   253,   253,   253,   253,
     253,   253,   253,   253,   253,   253,   253,   253,   253,   253,
     253,   253,   253,   253,   253,   253,   253,   253,   253,   253,
     253,   253,   253,   253,   253,   253,   253,   253,   253,   253,
     253,   253,   253,   253,   253,   253,   253,   253,   253,   253,
     253,   253,   253,   253,   253,   253,   253,   253,   253,   253,
     253,   254,   254,   254,   254
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     4,     3,     4,     4,     4,     2,     1,     2,
       1,     2,     1,     2,     3,     2,     1,     7,     5,     6,
       4,     4,     2,     2,     2,     2,     4,     2,     2,     2,
       2,     2,     2,     2,     3,     2,     2,     4,     4,     3,
       2,     1,     3,     2,     2,     2,     1,     3,     2,     4,
       4,     1,     2,     4,     2,     4,     1,     2,     3,     2,
       3,     1,     2,     1,     2,     1,     2,     4,     1,     2,
       3,     3,     1,     2,     1,     2,     3,     2,     3,     2,
       2,     3,     4,     4,     2,     3,     3,     1,     2,     2,
       3,     3,     2,     2,     1,     2,     3,     2,     1,     3,
       2,     3,     2,     3,     2,     3,     2,     2,     1,     3,
       4,     3,     2,     1,     2,     1,     2,     1,     2,     1,
       2,     1,     1,     1,     1,     2,     1,     3,     1,     3,
       1,     3,     1,     3,     1,     3,     2,     4,     1,     2,
       3,     4,     1,     3,     2,     4,     1,     2,     3,     4,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     1,
       3,     2,     4,     1,     3,     2,     4,     1,     3,     2,
       4,     1,     3,     2,     4,     1,     3,     2,     4,     1,
       3,     2,     4,     1,     3,     1,     3,     2,     4,     2,
       3,     5,     2,     3,     5,     2,     4,     1,     3,     1,
       2,     1,     3,     1,     1,     1,     2,     4,     4,     1,
       0,     1,     1,     3,     5,     5,     7,     5,     7,     5,
       7,     5,     7,     5,     7,     5,     7,     7,     5,     7,
       7,     5,     5,     5,     5,     5,     5,     7,     5,     5,
       5,     5,     5,     7,     5,     5,     7,     5,     5,     5,
       5,     5,     5,     7,     9,     9,     9,     9,     5,     5,
       5,     5,     5,     5,     2,     9,     5,     9,     5,     7,
       5,     7,     7,     5,     5,     5,     7,     7,     9,     7,
       7,     9,     9,     9,     9,     7,     7,     9,     7,     7,
       7,     9,     3,     2,     2,     1,     1,     2,     1,     2,
       2,     5,     2,     2,     2,     2,     5,     2,     2,     1,
       2,     2,     2,     2,     2,     2,     2,     2,     3,     3,
       3,     3,     3,     3,     1,     5,     3,     3,     3,     3,
       3,     3,     4,     4,     4,     4,     3,     1,     3,     3,
       1,     2,     3,     3,     4,     1,     1,     1,     1,     1,
       2,     2,     1,     1,     0,     2,     1,     1,     2,     2,
       2,     2,     1,     1,     1,     2,     2,     1,     2,     2,
       1,     2,     2,     1,     2,     2,     1,     2,     2,     1,
       2,     2,     1,     2,     2,     1,     2,     2,     1,     2,
       2,     1,     2,     2,     1,     2,     2,     1,     2,     2,
       1,     2,     2,     3,     3,     3,     3,     3,     3,     3,
       1,     2,     2,     3,     3,     3,     3,     1,     1,     2,
       2,     2,     1,     2,     1
};


#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)
#define YYEMPTY         (-2)
#define YYEOF           0

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                  \
do                                                              \
  if (yychar == YYEMPTY)                                        \
    {                                                           \
      yychar = (Token);                                         \
      yylval = (Value);                                         \
      YYPOPSTACK (yylen);                                       \
      yystate = *yyssp;                                         \
      goto yybackup;                                            \
    }                                                           \
  else                                                          \
    {                                                           \
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;                                                  \
    }                                                           \
while (0)

/* Error token number */
#define YYTERROR        1
#define YYERRCODE       256



/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)

/* This macro is provided for backward compatibility. */
#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


# define YY_SYMBOL_PRINT(Title, Type, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Type, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*----------------------------------------.
| Print this symbol's value on YYOUTPUT.  |
`----------------------------------------*/

static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  FILE *yyo = yyoutput;
  YYUSE (yyo);
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# endif
  YYUSE (yytype);
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyoutput, "%s %s (",
             yytype < YYNTOKENS ? "token" : "nterm", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yytype_int16 *yyssp, YYSTYPE *yyvsp, int yyrule)
{
  unsigned long int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       yystos[yyssp[yyi + 1 - yynrhs]],
                       &(yyvsp[(yyi + 1) - (yynrhs)])
                                              );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
static YYSIZE_T
yystrlen (const char *yystr)
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            /* Fall through.  */
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return 1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return 2 if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYSIZE_T *yymsg_alloc, char **yymsg,
                yytype_int16 *yyssp, int yytoken)
{
  YYSIZE_T yysize0 = yytnamerr (YY_NULLPTR, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yytoken != YYEMPTY)
    {
      int yyn = yypact[*yyssp];
      yyarg[yycount++] = yytname[yytoken];
      if (!yypact_value_is_default (yyn))
        {
          /* Start YYX at -YYN if negative to avoid negative indexes in
             YYCHECK.  In other words, skip the first -YYN actions for
             this state because they are default actions.  */
          int yyxbegin = yyn < 0 ? -yyn : 0;
          /* Stay within bounds of both yycheck and yytname.  */
          int yychecklim = YYLAST - yyn + 1;
          int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
          int yyx;

          for (yyx = yyxbegin; yyx < yyxend; ++yyx)
            if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR
                && !yytable_value_is_error (yytable[yyx + yyn]))
              {
                if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
                  {
                    yycount = 1;
                    yysize = yysize0;
                    break;
                  }
                yyarg[yycount++] = yytname[yyx];
                {
                  YYSIZE_T yysize1 = yysize + yytnamerr (YY_NULLPTR, yytname[yyx]);
                  if (! (yysize <= yysize1
                         && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                    return 2;
                  yysize = yysize1;
                }
              }
        }
    }

  switch (yycount)
    {
# define YYCASE_(N, S)                      \
      case N:                               \
        yyformat = S;                       \
      break
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
# undef YYCASE_
    }

  {
    YYSIZE_T yysize1 = yysize + yystrlen (yyformat);
    if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
      return 2;
    yysize = yysize1;
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return 1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yyarg[yyi++]);
          yyformat += 2;
        }
      else
        {
          yyp++;
          yyformat++;
        }
  }
  return 0;
}
#endif /* YYERROR_VERBOSE */

/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
{
  YYUSE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YYUSE (yytype);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}




/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       'yyss': related to states.
       'yyvs': related to semantic values.

       Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken = 0;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yyssp = yyss = yyssa;
  yyvsp = yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */
  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        YYSTYPE *yyvs1 = yyvs;
        yytype_int16 *yyss1 = yyss;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * sizeof (*yyssp),
                    &yyvs1, yysize * sizeof (*yyvsp),
                    &yystacksize);

        yyss = yyss1;
        yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yytype_int16 *yyss1 = yyss;
        union yyalloc *yyptr =
          (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
        if (! yyptr)
          goto yyexhaustedlab;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
                  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:
#line 262 "gram.y" /* yacc.c:1646  */
    { let((yyvsp[-2].rval).left.vp, (yyvsp[0].enode)); }
#line 2481 "gram.c" /* yacc.c:1646  */
    break;

  case 3:
#line 264 "gram.y" /* yacc.c:1646  */
    { (yyvsp[-1].rval).left.vp->v = (double) 0.0;
				  if ((yyvsp[-1].rval).left.vp->expr &&
					!((yyvsp[-1].rval).left.vp->flags & is_strexpr)) {
				    efree((yyvsp[-1].rval).left.vp->expr);
				    (yyvsp[-1].rval).left.vp->expr = NULL;
				  }
				  (yyvsp[-1].rval).left.vp->cellerror = CELLOK;
				  (yyvsp[-1].rval).left.vp->flags &= ~is_valid;
				  (yyvsp[-1].rval).left.vp->flags |= is_changed;
				  changed++;
				  FullUpdate++;
				  modflg++; }
#line 2498 "gram.c" /* yacc.c:1646  */
    break;

  case 4:
#line 277 "gram.y" /* yacc.c:1646  */
    { slet((yyvsp[-2].rval).left.vp, (yyvsp[0].enode), 0); }
#line 2504 "gram.c" /* yacc.c:1646  */
    break;

  case 5:
#line 279 "gram.y" /* yacc.c:1646  */
    { slet((yyvsp[-2].rval).left.vp, (yyvsp[0].enode), -1); }
#line 2510 "gram.c" /* yacc.c:1646  */
    break;

  case 6:
#line 281 "gram.y" /* yacc.c:1646  */
    { slet((yyvsp[-2].rval).left.vp, (yyvsp[0].enode), 1); }
#line 2516 "gram.c" /* yacc.c:1646  */
    break;

  case 7:
#line 283 "gram.y" /* yacc.c:1646  */
    { ljustify((yyvsp[0].rval).left.vp->row, (yyvsp[0].rval).left.vp->col,
				    (yyvsp[0].rval).right.vp->row, (yyvsp[0].rval).right.vp->col); }
#line 2523 "gram.c" /* yacc.c:1646  */
    break;

  case 8:
#line 286 "gram.y" /* yacc.c:1646  */
    { if (showrange)
				    ljustify(showsr, showsc, currow, curcol);
				}
#line 2531 "gram.c" /* yacc.c:1646  */
    break;

  case 9:
#line 290 "gram.y" /* yacc.c:1646  */
    { rjustify((yyvsp[0].rval).left.vp->row, (yyvsp[0].rval).left.vp->col,
				    (yyvsp[0].rval).right.vp->row, (yyvsp[0].rval).right.vp->col); }
#line 2538 "gram.c" /* yacc.c:1646  */
    break;

  case 10:
#line 293 "gram.y" /* yacc.c:1646  */
    { if (showrange)
				    rjustify(showsr, showsc, currow, curcol);
				}
#line 2546 "gram.c" /* yacc.c:1646  */
    break;

  case 11:
#line 297 "gram.y" /* yacc.c:1646  */
    { center((yyvsp[0].rval).left.vp->row, (yyvsp[0].rval).left.vp->col,
				    (yyvsp[0].rval).right.vp->row, (yyvsp[0].rval).right.vp->col); }
#line 2553 "gram.c" /* yacc.c:1646  */
    break;

  case 12:
#line 300 "gram.y" /* yacc.c:1646  */
    { if (showrange)
				    center(showsr, showsc, currow, curcol);
				}
#line 2561 "gram.c" /* yacc.c:1646  */
    break;

  case 13:
#line 303 "gram.y" /* yacc.c:1646  */
    { if (showrange) {
				    (yyvsp[0].ent).vp->nrow=currow<showsr?currow:showsr;
				    (yyvsp[0].ent).vp->ncol=curcol<showsc?curcol:showsc;
				    (yyvsp[0].ent).vp->nlastrow=currow<showsr?showsr:currow;
				    (yyvsp[0].ent).vp->nlastcol=curcol<showsc?showsc:curcol;
				  } else {
				    (yyvsp[0].ent).vp->nrow=currow;
				    (yyvsp[0].ent).vp->ncol=curcol;
				    (yyvsp[0].ent).vp->nlastrow=currow;
				    (yyvsp[0].ent).vp->nlastcol=curcol;
				  }
				  (yyvsp[0].ent).vp->flags |= is_changed;
				  FullUpdate++;
				  modflg++;
				}
#line 2581 "gram.c" /* yacc.c:1646  */
    break;

  case 14:
#line 319 "gram.y" /* yacc.c:1646  */
    { (yyvsp[-1].ent).vp->nrow = (yyvsp[0].rval).left.vp->row;
				  (yyvsp[-1].ent).vp->ncol = (yyvsp[0].rval).left.vp->col;
				  (yyvsp[-1].ent).vp->nlastrow = (yyvsp[0].rval).right.vp->row;
				  (yyvsp[-1].ent).vp->nlastcol = (yyvsp[0].rval).right.vp->col;
				  (yyvsp[-1].ent).vp->flags |= is_changed;
				  FullUpdate++;
				  modflg++;
				}
#line 2594 "gram.c" /* yacc.c:1646  */
    break;

  case 15:
#line 327 "gram.y" /* yacc.c:1646  */
    { (yyvsp[0].ent).vp->nrow = (yyvsp[0].ent).vp->ncol = -1; }
#line 2600 "gram.c" /* yacc.c:1646  */
    break;

  case 16:
#line 328 "gram.y" /* yacc.c:1646  */
    { struct ent *p;
				  p = lookat(currow, curcol);
				  p->nrow = p->ncol = -1;
				  p->flags |= is_changed;
				  modflg++;
				}
#line 2611 "gram.c" /* yacc.c:1646  */
    break;

  case 17:
#line 335 "gram.y" /* yacc.c:1646  */
    { doformat((yyvsp[-5].ival),(yyvsp[-3].ival),(yyvsp[-2].ival),(yyvsp[-1].ival),(yyvsp[0].ival)); }
#line 2617 "gram.c" /* yacc.c:1646  */
    break;

  case 18:
#line 337 "gram.y" /* yacc.c:1646  */
    { doformat((yyvsp[-3].ival),(yyvsp[-3].ival),(yyvsp[-2].ival),(yyvsp[-1].ival),(yyvsp[0].ival)); }
#line 2623 "gram.c" /* yacc.c:1646  */
    break;

  case 19:
#line 339 "gram.y" /* yacc.c:1646  */
    { doformat((yyvsp[-4].ival),(yyvsp[-2].ival),(yyvsp[-1].ival),(yyvsp[0].ival), REFMTFIX); }
#line 2629 "gram.c" /* yacc.c:1646  */
    break;

  case 20:
#line 341 "gram.y" /* yacc.c:1646  */
    { doformat((yyvsp[-2].ival),(yyvsp[-2].ival),(yyvsp[-1].ival),(yyvsp[0].ival), REFMTFIX); }
#line 2635 "gram.c" /* yacc.c:1646  */
    break;

  case 21:
#line 343 "gram.y" /* yacc.c:1646  */
    { if ((yyvsp[-2].ival) >= 0 && (yyvsp[-2].ival) < 10) {
				    if (colformat[(yyvsp[-2].ival)])
					scxfree(colformat[(yyvsp[-2].ival)]);
				    if (strlen((yyvsp[0].sval)))
					colformat[(yyvsp[-2].ival)] = (yyvsp[0].sval);
				    else
					colformat[(yyvsp[-2].ival)] = NULL;
				    FullUpdate++;
				    modflg++;
				  } else
				    error("Invalid format number");
				}
#line 2652 "gram.c" /* yacc.c:1646  */
    break;

  case 22:
#line 355 "gram.y" /* yacc.c:1646  */
    {  // This tmp hack is because readfile
				   // recurses back through yyparse.
				    char *tmp;
				    tmp = (yyvsp[0].sval);
				    readfile(tmp, 1);
				    scxfree(tmp);
				}
#line 2664 "gram.c" /* yacc.c:1646  */
    break;

  case 23:
#line 362 "gram.y" /* yacc.c:1646  */
    {
				    char *tmp;
				    tmp = (yyvsp[0].sval);
				    readfile(tmp, 0);
				    scxfree(tmp);
				}
#line 2675 "gram.c" /* yacc.c:1646  */
    break;

  case 24:
#line 368 "gram.y" /* yacc.c:1646  */
    { if (mdir) scxfree(mdir);
				  if (strlen((yyvsp[0].sval)))
				    mdir = (yyvsp[0].sval);
				  modflg++; }
#line 2684 "gram.c" /* yacc.c:1646  */
    break;

  case 25:
#line 373 "gram.y" /* yacc.c:1646  */
    { if (autorun) scxfree(autorun);
				  if (strlen((yyvsp[0].sval)))
				    autorun = (yyvsp[0].sval);
				  modflg++; }
#line 2693 "gram.c" /* yacc.c:1646  */
    break;

  case 26:
#line 378 "gram.y" /* yacc.c:1646  */
    { if ((yyvsp[-2].ival) > 0 && (yyvsp[-2].ival) <= FKEYS) {
				    if (fkey[(yyvsp[-2].ival) - 1]) {
					scxfree(fkey[(yyvsp[-2].ival) - 1]);
					fkey[(yyvsp[-2].ival) - 1] = NULL;
				    }
				    if (strlen((yyvsp[0].sval)))
					fkey[(yyvsp[-2].ival) - 1] = (yyvsp[0].sval);
				    modflg++;
				  } else
				    error("Invalid function key");
				}
#line 2709 "gram.c" /* yacc.c:1646  */
    break;

  case 27:
#line 389 "gram.y" /* yacc.c:1646  */
    { if (scext) scxfree(scext); scext = (yyvsp[0].sval); }
#line 2715 "gram.c" /* yacc.c:1646  */
    break;

  case 28:
#line 390 "gram.y" /* yacc.c:1646  */
    { if (ascext) scxfree(ascext); ascext = (yyvsp[0].sval); }
#line 2721 "gram.c" /* yacc.c:1646  */
    break;

  case 29:
#line 392 "gram.y" /* yacc.c:1646  */
    { if (tbl0ext) scxfree(tbl0ext); tbl0ext = (yyvsp[0].sval); }
#line 2727 "gram.c" /* yacc.c:1646  */
    break;

  case 30:
#line 393 "gram.y" /* yacc.c:1646  */
    { if (tblext) scxfree(tblext); tblext = (yyvsp[0].sval); }
#line 2733 "gram.c" /* yacc.c:1646  */
    break;

  case 31:
#line 394 "gram.y" /* yacc.c:1646  */
    { if (latexext) scxfree(latexext);
					    latexext = (yyvsp[0].sval); }
#line 2740 "gram.c" /* yacc.c:1646  */
    break;

  case 32:
#line 396 "gram.y" /* yacc.c:1646  */
    { if (slatexext) scxfree(slatexext);
					    slatexext = (yyvsp[0].sval); }
#line 2747 "gram.c" /* yacc.c:1646  */
    break;

  case 33:
#line 398 "gram.y" /* yacc.c:1646  */
    { if (texext) scxfree(texext); texext = (yyvsp[0].sval); }
#line 2753 "gram.c" /* yacc.c:1646  */
    break;

  case 34:
#line 400 "gram.y" /* yacc.c:1646  */
    { (void) writefile((yyvsp[-1].sval), ((yyvsp[0].rval).left.vp)->row, 
					((yyvsp[0].rval).left.vp)->col, ((yyvsp[0].rval).right.vp)->row,
					((yyvsp[0].rval).right.vp)->col);
					    scxfree((yyvsp[-1].sval)); }
#line 2762 "gram.c" /* yacc.c:1646  */
    break;

  case 35:
#line 404 "gram.y" /* yacc.c:1646  */
    { (void) writefile((yyvsp[0].sval), 0, 0, maxrow, maxcol);
					    scxfree((yyvsp[0].sval)); }
#line 2769 "gram.c" /* yacc.c:1646  */
    break;

  case 36:
#line 406 "gram.y" /* yacc.c:1646  */
    { (void) write_cells(stdout,
					  (yyvsp[0].rval).left.vp->row, (yyvsp[0].rval).left.vp->col,
					  (yyvsp[0].rval).right.vp->row, (yyvsp[0].rval).right.vp->col,
					  (yyvsp[0].rval).left.vp->row, (yyvsp[0].rval).left.vp->col); }
#line 2778 "gram.c" /* yacc.c:1646  */
    break;

  case 37:
#line 411 "gram.y" /* yacc.c:1646  */
    { (void) write_cells(stdout,
					  (yyvsp[-2].rval).left.vp->row, (yyvsp[-2].rval).left.vp->col,
					  (yyvsp[-2].rval).right.vp->row, (yyvsp[-2].rval).right.vp->col,
					  (yyvsp[0].rval).left.vp->row, (yyvsp[0].rval).left.vp->col); }
#line 2787 "gram.c" /* yacc.c:1646  */
    break;

  case 38:
#line 416 "gram.y" /* yacc.c:1646  */
    { (void) write_cells(stdout, 0, 0,
					  maxrow, maxcol,
					  (yyvsp[0].rval).left.vp->row, (yyvsp[0].rval).left.vp->col); }
#line 2795 "gram.c" /* yacc.c:1646  */
    break;

  case 39:
#line 419 "gram.y" /* yacc.c:1646  */
    { (void) write_cells(stdout,
					  showsr, showsc, currow, curcol,
					  (yyvsp[0].rval).left.vp->row, (yyvsp[0].rval).left.vp->col); }
#line 2803 "gram.c" /* yacc.c:1646  */
    break;

  case 40:
#line 422 "gram.y" /* yacc.c:1646  */
    { (void) write_cells(stdout, 0, 0,
					    maxrow, maxcol, 0, 0); }
#line 2810 "gram.c" /* yacc.c:1646  */
    break;

  case 41:
#line 424 "gram.y" /* yacc.c:1646  */
    { (void) write_cells(stdout, 0, 0,
					    maxrow, maxcol, 0, 0); }
#line 2817 "gram.c" /* yacc.c:1646  */
    break;

  case 42:
#line 426 "gram.y" /* yacc.c:1646  */
    { (void) printfile((yyvsp[-1].sval), ((yyvsp[0].rval).left.vp)->row, 
					((yyvsp[0].rval).left.vp)->col, ((yyvsp[0].rval).right.vp)->row,
					((yyvsp[0].rval).right.vp)->col);
					    scxfree((yyvsp[-1].sval)); }
#line 2826 "gram.c" /* yacc.c:1646  */
    break;

  case 43:
#line 430 "gram.y" /* yacc.c:1646  */
    { (void) printfile((yyvsp[0].sval), 0, 0, maxrow, maxcol);
					    scxfree((yyvsp[0].sval)); }
#line 2833 "gram.c" /* yacc.c:1646  */
    break;

  case 44:
#line 432 "gram.y" /* yacc.c:1646  */
    { (void) printfile(NULL,
					  (yyvsp[0].rval).left.vp->row, (yyvsp[0].rval).left.vp->col,
					  (yyvsp[0].rval).right.vp->row, (yyvsp[0].rval).right.vp->col); }
#line 2841 "gram.c" /* yacc.c:1646  */
    break;

  case 45:
#line 435 "gram.y" /* yacc.c:1646  */
    { (void) printfile(NULL, 0, 0,
					    maxrow, maxcol); }
#line 2848 "gram.c" /* yacc.c:1646  */
    break;

  case 46:
#line 437 "gram.y" /* yacc.c:1646  */
    { (void) printfile(NULL, 0, 0,
					    maxrow, maxcol); }
#line 2855 "gram.c" /* yacc.c:1646  */
    break;

  case 47:
#line 439 "gram.y" /* yacc.c:1646  */
    { (void) tblprintfile((yyvsp[-1].sval), ((yyvsp[0].rval).left.vp)->row, 
					((yyvsp[0].rval).left.vp)->col, ((yyvsp[0].rval).right.vp)->row,
					((yyvsp[0].rval).right.vp)->col);
					    scxfree((yyvsp[-1].sval)); }
#line 2864 "gram.c" /* yacc.c:1646  */
    break;

  case 48:
#line 443 "gram.y" /* yacc.c:1646  */
    { (void)tblprintfile((yyvsp[0].sval), 0, 0, maxrow, maxcol);
					    scxfree((yyvsp[0].sval)); }
#line 2871 "gram.c" /* yacc.c:1646  */
    break;

  case 49:
#line 445 "gram.y" /* yacc.c:1646  */
    { showcol((yyvsp[-2].ival), (yyvsp[0].ival)); }
#line 2877 "gram.c" /* yacc.c:1646  */
    break;

  case 50:
#line 447 "gram.y" /* yacc.c:1646  */
    { showrow((yyvsp[-2].ival), (yyvsp[0].ival)); }
#line 2883 "gram.c" /* yacc.c:1646  */
    break;

  case 51:
#line 448 "gram.y" /* yacc.c:1646  */
    { int arg;
	    				  if (showrange == SHOWROWS) {
					    if (showsr < currow) {
						int r = currow;
						currow = showsr;
						showsr = r;
					    }
					    arg = showsr - currow + 1;
					    hiderow(arg);
					  } else if (showrange == SHOWCOLS) {
					    if (showsc < curcol) {
						int c = curcol;
						curcol = showsc;
						showsc = c;
					    }
					    arg = showsc - curcol + 1;
					    hidecol(arg);
					  } else
					    arg = 1;
					}
#line 2908 "gram.c" /* yacc.c:1646  */
    break;

  case 52:
#line 468 "gram.y" /* yacc.c:1646  */
    { hide_col((yyvsp[0].ival)); }
#line 2914 "gram.c" /* yacc.c:1646  */
    break;

  case 53:
#line 469 "gram.y" /* yacc.c:1646  */
    { int c = curcol, arg;
					  if ((yyvsp[-2].ival) < (yyvsp[0].ival)) {
					    curcol = (yyvsp[-2].ival);
					    arg = (yyvsp[0].ival) - (yyvsp[-2].ival) + 1;
					  } else {
					      curcol = (yyvsp[0].ival);
					      arg = (yyvsp[-2].ival) - (yyvsp[0].ival) + 1;
					  }
					  hidecol(arg);
					  curcol = c < curcol ? c :
					      c < curcol + arg ? curcol :
					      c - arg;
					}
#line 2932 "gram.c" /* yacc.c:1646  */
    break;

  case 54:
#line 482 "gram.y" /* yacc.c:1646  */
    { hide_row((yyvsp[0].ival)); }
#line 2938 "gram.c" /* yacc.c:1646  */
    break;

  case 55:
#line 484 "gram.y" /* yacc.c:1646  */
    { int r = currow, arg;
					  if ((yyvsp[-2].ival) < (yyvsp[0].ival)) {
					    currow = (yyvsp[-2].ival);
					    arg = (yyvsp[0].ival) - (yyvsp[-2].ival) + 1;
					  } else {
					      currow = (yyvsp[0].ival);
					      arg = (yyvsp[-2].ival) - (yyvsp[0].ival) + 1;
					  }
					  hiderow(arg);
					  currow = r < currow ? r :
					      r < currow + arg ? currow :
					      r - arg;
					}
#line 2956 "gram.c" /* yacc.c:1646  */
    break;

  case 56:
#line 497 "gram.y" /* yacc.c:1646  */
    { if (showrange) {
					    showrange = 0;
					    copy(lookat(showsr, showsc),
					    lookat(currow, curcol),
					    NULL, NULL);
					  } else
					    copy(lookat(currow, curcol),
					    lookat(currow, curcol),
					    NULL, NULL); }
#line 2970 "gram.c" /* yacc.c:1646  */
    break;

  case 57:
#line 506 "gram.y" /* yacc.c:1646  */
    { copy((yyvsp[0].rval).left.vp, (yyvsp[0].rval).right.vp,
					    NULL, NULL); }
#line 2977 "gram.c" /* yacc.c:1646  */
    break;

  case 58:
#line 509 "gram.y" /* yacc.c:1646  */
    { copy((yyvsp[-1].rval).left.vp, (yyvsp[-1].rval).right.vp,
					    (yyvsp[0].rval).left.vp, (yyvsp[0].rval).right.vp); }
#line 2984 "gram.c" /* yacc.c:1646  */
    break;

  case 59:
#line 511 "gram.y" /* yacc.c:1646  */
    { mover((yyvsp[0].ent).vp, lookat(showsr, showsc),
					    lookat(currow, curcol)); }
#line 2991 "gram.c" /* yacc.c:1646  */
    break;

  case 60:
#line 513 "gram.y" /* yacc.c:1646  */
    { mover((yyvsp[-1].ent).vp, (yyvsp[0].rval).left.vp,
					    (yyvsp[0].rval).right.vp); }
#line 2998 "gram.c" /* yacc.c:1646  */
    break;

  case 61:
#line 515 "gram.y" /* yacc.c:1646  */
    { eraser(lookat(showsr, showsc),
					    lookat(currow, curcol)); }
#line 3005 "gram.c" /* yacc.c:1646  */
    break;

  case 62:
#line 517 "gram.y" /* yacc.c:1646  */
    { eraser((yyvsp[0].rval).left.vp, (yyvsp[0].rval).right.vp); }
#line 3011 "gram.c" /* yacc.c:1646  */
    break;

  case 63:
#line 518 "gram.y" /* yacc.c:1646  */
    { yankr(lookat(showsr, showsc),
					    lookat(currow, curcol)); }
#line 3018 "gram.c" /* yacc.c:1646  */
    break;

  case 64:
#line 520 "gram.y" /* yacc.c:1646  */
    { yankr((yyvsp[0].rval).left.vp, (yyvsp[0].rval).right.vp); }
#line 3024 "gram.c" /* yacc.c:1646  */
    break;

  case 65:
#line 521 "gram.y" /* yacc.c:1646  */
    { valueize_area(showsr, showsc, currow, curcol);
					    modflg++; }
#line 3031 "gram.c" /* yacc.c:1646  */
    break;

  case 66:
#line 523 "gram.y" /* yacc.c:1646  */
    { valueize_area(((yyvsp[0].rval).left.vp)->row,
					    ((yyvsp[0].rval).left.vp)->col,
					    ((yyvsp[0].rval).right.vp)->row,
					    ((yyvsp[0].rval).right.vp)->col); modflg++; }
#line 3040 "gram.c" /* yacc.c:1646  */
    break;

  case 67:
#line 528 "gram.y" /* yacc.c:1646  */
    { fill((yyvsp[-2].rval).left.vp, (yyvsp[-2].rval).right.vp, (yyvsp[-1].fval), (yyvsp[0].fval)); }
#line 3046 "gram.c" /* yacc.c:1646  */
    break;

  case 68:
#line 529 "gram.y" /* yacc.c:1646  */
    { sortrange(lookat(showsr, showsc),
				  lookat(currow, curcol), NULL); }
#line 3053 "gram.c" /* yacc.c:1646  */
    break;

  case 69:
#line 531 "gram.y" /* yacc.c:1646  */
    { sortrange((yyvsp[0].rval).left.vp, (yyvsp[0].rval).right.vp, NULL); }
#line 3059 "gram.c" /* yacc.c:1646  */
    break;

  case 70:
#line 533 "gram.y" /* yacc.c:1646  */
    { sortrange((yyvsp[-1].rval).left.vp, (yyvsp[-1].rval).right.vp, (yyvsp[0].sval)); }
#line 3065 "gram.c" /* yacc.c:1646  */
    break;

  case 71:
#line 535 "gram.y" /* yacc.c:1646  */
    { format_cell((yyvsp[-1].rval).left.vp, (yyvsp[-1].rval).right.vp, (yyvsp[0].sval)); }
#line 3071 "gram.c" /* yacc.c:1646  */
    break;

  case 72:
#line 536 "gram.y" /* yacc.c:1646  */
    { lock_cells(lookat(showsr, showsc),
				    lookat(currow, curcol)); }
#line 3078 "gram.c" /* yacc.c:1646  */
    break;

  case 73:
#line 539 "gram.y" /* yacc.c:1646  */
    { lock_cells((yyvsp[0].rval).left.vp, (yyvsp[0].rval).right.vp); }
#line 3084 "gram.c" /* yacc.c:1646  */
    break;

  case 74:
#line 540 "gram.y" /* yacc.c:1646  */
    { unlock_cells(lookat(showsr, showsc),
					    lookat(currow, curcol)); }
#line 3091 "gram.c" /* yacc.c:1646  */
    break;

  case 75:
#line 543 "gram.y" /* yacc.c:1646  */
    { unlock_cells((yyvsp[0].rval).left.vp, (yyvsp[0].rval).right.vp); }
#line 3097 "gram.c" /* yacc.c:1646  */
    break;

  case 76:
#line 545 "gram.y" /* yacc.c:1646  */
    { moveto((yyvsp[-1].rval).left.vp->row, (yyvsp[-1].rval).left.vp->col,
					    (yyvsp[-1].rval).right.vp->row, (yyvsp[-1].rval).right.vp->col,
					    (yyvsp[0].rval).left.vp->row, (yyvsp[0].rval).left.vp->col); }
#line 3105 "gram.c" /* yacc.c:1646  */
    break;

  case 77:
#line 548 "gram.y" /* yacc.c:1646  */
    { moveto((yyvsp[0].rval).left.vp->row, (yyvsp[0].rval).left.vp->col,
					    (yyvsp[0].rval).right.vp->row, (yyvsp[0].rval).right.vp->col,
					    -1, -1); }
#line 3113 "gram.c" /* yacc.c:1646  */
    break;

  case 78:
#line 551 "gram.y" /* yacc.c:1646  */
    { num_search((yyvsp[-1].fval),
					  (yyvsp[0].rval).left.vp->row, (yyvsp[0].rval).left.vp->col,
					  (yyvsp[0].rval).right.vp->row,
					  (yyvsp[0].rval).right.vp->col, 0); }
#line 3122 "gram.c" /* yacc.c:1646  */
    break;

  case 79:
#line 555 "gram.y" /* yacc.c:1646  */
    { num_search((yyvsp[0].fval), 0, 0,
					  maxrow, maxcol, 0); }
#line 3129 "gram.c" /* yacc.c:1646  */
    break;

  case 81:
#line 558 "gram.y" /* yacc.c:1646  */
    { str_search((yyvsp[-1].sval),
					  (yyvsp[0].rval).left.vp->row, (yyvsp[0].rval).left.vp->col,
					  (yyvsp[0].rval).right.vp->row,
					  (yyvsp[0].rval).right.vp->col, 0); }
#line 3138 "gram.c" /* yacc.c:1646  */
    break;

  case 82:
#line 562 "gram.y" /* yacc.c:1646  */
    { str_search((yyvsp[-1].sval),
					  (yyvsp[0].rval).left.vp->row, (yyvsp[0].rval).left.vp->col,
					  (yyvsp[0].rval).right.vp->row,
					  (yyvsp[0].rval).right.vp->col, 1); }
#line 3147 "gram.c" /* yacc.c:1646  */
    break;

  case 83:
#line 566 "gram.y" /* yacc.c:1646  */
    { str_search((yyvsp[-1].sval),
					  (yyvsp[0].rval).left.vp->row, (yyvsp[0].rval).left.vp->col,
					  (yyvsp[0].rval).right.vp->row,
					  (yyvsp[0].rval).right.vp->col, 2); }
#line 3156 "gram.c" /* yacc.c:1646  */
    break;

  case 84:
#line 570 "gram.y" /* yacc.c:1646  */
    { str_search((yyvsp[0].sval), 0, 0,
					  maxrow, maxcol, 0); }
#line 3163 "gram.c" /* yacc.c:1646  */
    break;

  case 85:
#line 572 "gram.y" /* yacc.c:1646  */
    { str_search((yyvsp[0].sval), 0, 0,
					  maxrow, maxcol, 1); }
#line 3170 "gram.c" /* yacc.c:1646  */
    break;

  case 86:
#line 574 "gram.y" /* yacc.c:1646  */
    { str_search((yyvsp[0].sval), 0, 0,
					  maxrow, maxcol, 2); }
#line 3177 "gram.c" /* yacc.c:1646  */
    break;

  case 87:
#line 576 "gram.y" /* yacc.c:1646  */
    { go_last(); }
#line 3183 "gram.c" /* yacc.c:1646  */
    break;

  case 88:
#line 577 "gram.y" /* yacc.c:1646  */
    { ; }
#line 3189 "gram.c" /* yacc.c:1646  */
    break;

  case 89:
#line 578 "gram.y" /* yacc.c:1646  */
    { struct ent_ptr arg1, arg2;
					  arg1.vp = lookat(showsr, showsc);
					  arg1.vf = 0;
					  arg2.vp = lookat(currow, curcol);
					  arg2.vf = 0;
                                          if (arg1.vp == arg2.vp || !showrange)
                                             add_range((yyvsp[0].sval), arg2, arg2, 0);
                                          else
                                             add_range((yyvsp[0].sval), arg1, arg2, 1); }
#line 3203 "gram.c" /* yacc.c:1646  */
    break;

  case 90:
#line 588 "gram.y" /* yacc.c:1646  */
    { add_range((yyvsp[-1].sval), (yyvsp[0].rval).left, (yyvsp[0].rval).right, 1); }
#line 3209 "gram.c" /* yacc.c:1646  */
    break;

  case 91:
#line 589 "gram.y" /* yacc.c:1646  */
    { add_range((yyvsp[-1].sval), (yyvsp[0].ent), (yyvsp[0].ent), 0); }
#line 3215 "gram.c" /* yacc.c:1646  */
    break;

  case 92:
#line 590 "gram.y" /* yacc.c:1646  */
    { del_range((yyvsp[0].rval).left.vp, (yyvsp[0].rval).right.vp); }
#line 3221 "gram.c" /* yacc.c:1646  */
    break;

  case 93:
#line 591 "gram.y" /* yacc.c:1646  */
    { add_abbr((yyvsp[0].sval)); }
#line 3227 "gram.c" /* yacc.c:1646  */
    break;

  case 94:
#line 592 "gram.y" /* yacc.c:1646  */
    { add_abbr(NULL); }
#line 3233 "gram.c" /* yacc.c:1646  */
    break;

  case 95:
#line 593 "gram.y" /* yacc.c:1646  */
    { del_abbr((yyvsp[0].sval)); }
#line 3239 "gram.c" /* yacc.c:1646  */
    break;

  case 96:
#line 594 "gram.y" /* yacc.c:1646  */
    { add_frange((yyvsp[-1].rval).left.vp, (yyvsp[-1].rval).right.vp,
						(yyvsp[0].rval).left.vp, (yyvsp[0].rval).right.vp,
						0, 0, 0, 0); }
#line 3247 "gram.c" /* yacc.c:1646  */
    break;

  case 97:
#line 597 "gram.y" /* yacc.c:1646  */
    { if (showrange) {
					    showrange = 0;
					    add_frange((yyvsp[0].rval).left.vp, (yyvsp[0].rval).right.vp,
						lookat(showsr, showsc),
						lookat(currow, curcol),
						0, 0, 0, 0);
					  } else {
					    struct frange *cfr;
					    cfr = find_frange(currow, curcol);
					    if (cfr) {
						add_frange(cfr->or_left,
						    cfr->or_right,
						    (yyvsp[0].rval).left.vp, (yyvsp[0].rval).right.vp,
						    0, 0, 0, 0);
					    }
					  }
					}
#line 3269 "gram.c" /* yacc.c:1646  */
    break;

  case 98:
#line 614 "gram.y" /* yacc.c:1646  */
    { struct frange *cfr;
					  // cfr points to current frange
					  cfr = find_frange(currow, curcol);
					  if (showrange && cfr) {
					    showrange = 0;
					    add_frange(cfr->or_left,
						cfr->or_right,
						lookat(showsr, showsc),
						lookat(currow, curcol),
						0, 0, 0, 0);
					  } else {
					    error("Need both outer and inner"
						    " ranges to create frame");
					  }
					}
#line 3289 "gram.c" /* yacc.c:1646  */
    break;

  case 99:
#line 630 "gram.y" /* yacc.c:1646  */
    { add_frange((yyvsp[-1].rval).left.vp, (yyvsp[-1].rval).right.vp,
						NULL, NULL, (yyvsp[0].ival), -1, -1, -1); }
#line 3296 "gram.c" /* yacc.c:1646  */
    break;

  case 100:
#line 632 "gram.y" /* yacc.c:1646  */
    { struct frange *cfr;
					  // cfr points to current frange
					  cfr = find_frange(currow, curcol);
					  if (cfr)
					    add_frange(cfr->or_left,
						cfr->or_right,
						NULL, NULL, (yyvsp[0].ival), -1, -1, -1); }
#line 3308 "gram.c" /* yacc.c:1646  */
    break;

  case 101:
#line 640 "gram.y" /* yacc.c:1646  */
    { add_frange((yyvsp[-1].rval).left.vp, (yyvsp[-1].rval).right.vp,
						NULL, NULL, -1, (yyvsp[0].ival), -1, -1); }
#line 3315 "gram.c" /* yacc.c:1646  */
    break;

  case 102:
#line 642 "gram.y" /* yacc.c:1646  */
    { struct frange *cfr;
					  // cfr points to current frange
					  cfr = find_frange(currow, curcol);
					  if (cfr)
					    add_frange(cfr->or_left,
						cfr->or_right,
						NULL, NULL, -1, (yyvsp[0].ival), -1, -1); }
#line 3327 "gram.c" /* yacc.c:1646  */
    break;

  case 103:
#line 650 "gram.y" /* yacc.c:1646  */
    { add_frange((yyvsp[-1].rval).left.vp, (yyvsp[-1].rval).right.vp,
						NULL, NULL, -1, -1, (yyvsp[0].ival), -1); }
#line 3334 "gram.c" /* yacc.c:1646  */
    break;

  case 104:
#line 652 "gram.y" /* yacc.c:1646  */
    { struct frange *cfr;
					  // cfr points to current frange
					  cfr = find_frange(currow, curcol);
					  if (cfr)
					    add_frange(cfr->or_left,
						cfr->or_right,
						NULL, NULL, -1, -1, (yyvsp[0].ival), -1); }
#line 3346 "gram.c" /* yacc.c:1646  */
    break;

  case 105:
#line 660 "gram.y" /* yacc.c:1646  */
    { add_frange((yyvsp[-1].rval).left.vp, (yyvsp[-1].rval).right.vp,
						NULL, NULL, -1, -1, -1, (yyvsp[0].ival)); }
#line 3353 "gram.c" /* yacc.c:1646  */
    break;

  case 106:
#line 662 "gram.y" /* yacc.c:1646  */
    { struct frange *cfr;
					  // cfr points to current frange
					  cfr = find_frange(currow, curcol);
					  if (cfr)
					    add_frange(cfr->or_left,
						cfr->or_right,
						NULL, NULL, -1, -1, -1, (yyvsp[0].ival)); }
#line 3365 "gram.c" /* yacc.c:1646  */
    break;

  case 107:
#line 669 "gram.y" /* yacc.c:1646  */
    { add_frange((yyvsp[0].rval).left.vp, (yyvsp[0].rval).right.vp,
						NULL, NULL, 0, 0, 0, 0); }
#line 3372 "gram.c" /* yacc.c:1646  */
    break;

  case 108:
#line 671 "gram.y" /* yacc.c:1646  */
    { struct frange *cfr;
					  // cfr points to current frange
					  cfr = find_frange(currow, curcol);
					  if (cfr)
					    add_frange(cfr->or_left,
						cfr->or_right,
						NULL, NULL, 0, 0, 0, 0); }
#line 3384 "gram.c" /* yacc.c:1646  */
    break;

  case 109:
#line 678 "gram.y" /* yacc.c:1646  */
    { if ((yyvsp[-1].ival) > 0 && (yyvsp[-1].ival) < 9)
					    initcolor((yyvsp[-1].ival));
					  else
					    error("Invalid color number"); }
#line 3393 "gram.c" /* yacc.c:1646  */
    break;

  case 110:
#line 682 "gram.y" /* yacc.c:1646  */
    { change_color((yyvsp[-2].ival), (yyvsp[0].enode)); }
#line 3399 "gram.c" /* yacc.c:1646  */
    break;

  case 111:
#line 683 "gram.y" /* yacc.c:1646  */
    { add_crange((yyvsp[-1].rval).left.vp, (yyvsp[-1].rval).right.vp,
						(yyvsp[0].ival)); }
#line 3406 "gram.c" /* yacc.c:1646  */
    break;

  case 112:
#line 685 "gram.y" /* yacc.c:1646  */
    { modflg++; }
#line 3412 "gram.c" /* yacc.c:1646  */
    break;

  case 113:
#line 686 "gram.y" /* yacc.c:1646  */
    { backrow( 1); }
#line 3418 "gram.c" /* yacc.c:1646  */
    break;

  case 114:
#line 687 "gram.y" /* yacc.c:1646  */
    { backrow((yyvsp[0].ival)); }
#line 3424 "gram.c" /* yacc.c:1646  */
    break;

  case 115:
#line 688 "gram.y" /* yacc.c:1646  */
    { forwrow( 1); }
#line 3430 "gram.c" /* yacc.c:1646  */
    break;

  case 116:
#line 689 "gram.y" /* yacc.c:1646  */
    { forwrow((yyvsp[0].ival)); }
#line 3436 "gram.c" /* yacc.c:1646  */
    break;

  case 117:
#line 690 "gram.y" /* yacc.c:1646  */
    { backcol( 1); }
#line 3442 "gram.c" /* yacc.c:1646  */
    break;

  case 118:
#line 691 "gram.y" /* yacc.c:1646  */
    { backcol((yyvsp[0].ival)); }
#line 3448 "gram.c" /* yacc.c:1646  */
    break;

  case 119:
#line 692 "gram.y" /* yacc.c:1646  */
    { forwcol( 1); }
#line 3454 "gram.c" /* yacc.c:1646  */
    break;

  case 120:
#line 693 "gram.y" /* yacc.c:1646  */
    { forwcol((yyvsp[0].ival)); }
#line 3460 "gram.c" /* yacc.c:1646  */
    break;

  case 121:
#line 694 "gram.y" /* yacc.c:1646  */
    { doend(-1, 0); }
#line 3466 "gram.c" /* yacc.c:1646  */
    break;

  case 122:
#line 695 "gram.y" /* yacc.c:1646  */
    { doend( 1, 0); }
#line 3472 "gram.c" /* yacc.c:1646  */
    break;

  case 123:
#line 696 "gram.y" /* yacc.c:1646  */
    { doend( 0,-1); }
#line 3478 "gram.c" /* yacc.c:1646  */
    break;

  case 124:
#line 697 "gram.y" /* yacc.c:1646  */
    { doend( 0, 1); }
#line 3484 "gram.c" /* yacc.c:1646  */
    break;

  case 125:
#line 698 "gram.y" /* yacc.c:1646  */
    { int c;
					  if ((c = *(yyvsp[0].sval)) >= '0' && c <= '9') {
					    qbuf = c - '0' + (DELBUFSIZE - 10);
					  } else if (c >= 'a' && c <= 'z') {
					    qbuf = c - 'a' + (DELBUFSIZE - 36);
					  } else if (c == '"') {
					    qbuf = 0;
					  } else
					    error("Invalid buffer");
					  scxfree((yyvsp[0].sval));
					}
#line 3500 "gram.c" /* yacc.c:1646  */
    break;

  case 126:
#line 709 "gram.y" /* yacc.c:1646  */
    { insertrow( 1, 0); }
#line 3506 "gram.c" /* yacc.c:1646  */
    break;

  case 127:
#line 710 "gram.y" /* yacc.c:1646  */
    { insertrow((yyvsp[0].ival), 0); }
#line 3512 "gram.c" /* yacc.c:1646  */
    break;

  case 128:
#line 711 "gram.y" /* yacc.c:1646  */
    { insertrow( 1, 1); }
#line 3518 "gram.c" /* yacc.c:1646  */
    break;

  case 129:
#line 712 "gram.y" /* yacc.c:1646  */
    { insertrow((yyvsp[0].ival), 1); }
#line 3524 "gram.c" /* yacc.c:1646  */
    break;

  case 130:
#line 713 "gram.y" /* yacc.c:1646  */
    { insertcol( 1, 0); }
#line 3530 "gram.c" /* yacc.c:1646  */
    break;

  case 131:
#line 714 "gram.y" /* yacc.c:1646  */
    { insertcol((yyvsp[0].ival), 0); }
#line 3536 "gram.c" /* yacc.c:1646  */
    break;

  case 132:
#line 715 "gram.y" /* yacc.c:1646  */
    { insertcol( 1, 1); }
#line 3542 "gram.c" /* yacc.c:1646  */
    break;

  case 133:
#line 716 "gram.y" /* yacc.c:1646  */
    { insertcol((yyvsp[0].ival), 1); }
#line 3548 "gram.c" /* yacc.c:1646  */
    break;

  case 134:
#line 717 "gram.y" /* yacc.c:1646  */
    { int arg;
	    				  if (showrange == SHOWROWS) {
					    if (showsr < currow) {
						int r = currow;
						currow = showsr;
						showsr = r;
					    }
					    arg = showsr - currow + 1;
					  } else
					    arg = 1;
					  deleterow(arg);
					}
#line 3565 "gram.c" /* yacc.c:1646  */
    break;

  case 135:
#line 729 "gram.y" /* yacc.c:1646  */
    { deleterow((yyvsp[0].ival)); }
#line 3571 "gram.c" /* yacc.c:1646  */
    break;

  case 136:
#line 730 "gram.y" /* yacc.c:1646  */
    { int r = currow;
					  currow = (yyvsp[0].ival);
					  deleterow(1);
					  currow = r <= currow ? r : r - 1;
					}
#line 3581 "gram.c" /* yacc.c:1646  */
    break;

  case 137:
#line 736 "gram.y" /* yacc.c:1646  */
    { int r = currow, arg;
					  if ((yyvsp[-2].ival) < (yyvsp[0].ival)) {
					    currow = (yyvsp[-2].ival);
					    arg = (yyvsp[0].ival) - (yyvsp[-2].ival) + 1;
					  } else {
					      currow = (yyvsp[0].ival);
					      arg = (yyvsp[-2].ival) - (yyvsp[0].ival) + 1;
					  }
					  deleterow(arg);
					  currow = r < currow ? r :
					      r < currow + arg ? currow :
					      r - arg;
					}
#line 3599 "gram.c" /* yacc.c:1646  */
    break;

  case 138:
#line 749 "gram.y" /* yacc.c:1646  */
    { int arg;
	    				  if (showrange == SHOWCOLS) {
					    if (showsc < curcol) {
						int c = curcol;
						curcol = showsc;
						showsc = c;
					    }
					    arg = showsc - curcol + 1;
					  } else
					    arg = 1;
					  closecol(arg);
					}
#line 3616 "gram.c" /* yacc.c:1646  */
    break;

  case 139:
#line 761 "gram.y" /* yacc.c:1646  */
    { int r = curcol;
					  curcol = (yyvsp[0].ival);
					  closecol(1);
					  curcol = r <= curcol ? r : r - 1;
					}
#line 3626 "gram.c" /* yacc.c:1646  */
    break;

  case 140:
#line 766 "gram.y" /* yacc.c:1646  */
    { closecol((yyvsp[0].ival)); }
#line 3632 "gram.c" /* yacc.c:1646  */
    break;

  case 141:
#line 767 "gram.y" /* yacc.c:1646  */
    { int c = curcol, arg;
					  if ((yyvsp[-2].ival) < (yyvsp[0].ival)) {
					    curcol = (yyvsp[-2].ival);
					    arg = (yyvsp[0].ival) - (yyvsp[-2].ival) + 1;
					  } else {
					      curcol = (yyvsp[0].ival);
					      arg = (yyvsp[-2].ival) - (yyvsp[0].ival) + 1;
					  }
					  closecol(arg);
					  curcol = c < curcol ? c :
					      c < curcol + arg ? curcol :
					      c - arg;
					}
#line 3650 "gram.c" /* yacc.c:1646  */
    break;

  case 142:
#line 780 "gram.y" /* yacc.c:1646  */
    { int r = currow, arg;
	    				  if (showrange == SHOWROWS) {
					    if (showsr < currow) {
						currow = showsr;
						showsr = r;
					    }
					    arg = showsr - currow + 1;
					  } else
					    arg = 1;
					  yankrow(arg);
					  currow = r;
					}
#line 3667 "gram.c" /* yacc.c:1646  */
    break;

  case 143:
#line 792 "gram.y" /* yacc.c:1646  */
    { yankrow((yyvsp[0].ival)); }
#line 3673 "gram.c" /* yacc.c:1646  */
    break;

  case 144:
#line 793 "gram.y" /* yacc.c:1646  */
    { int r = currow;
					  currow = (yyvsp[0].ival);
					  yankrow(1);
					  currow = r;
					}
#line 3683 "gram.c" /* yacc.c:1646  */
    break;

  case 145:
#line 799 "gram.y" /* yacc.c:1646  */
    { int r = currow, arg;
					  if ((yyvsp[-2].ival) < (yyvsp[0].ival)) {
					    currow = (yyvsp[-2].ival);
					    arg = (yyvsp[0].ival) - (yyvsp[-2].ival) + 1;
					  } else {
					      currow = (yyvsp[0].ival);
					      arg = (yyvsp[-2].ival) - (yyvsp[0].ival) + 1;
					  }
					  yankrow(arg);
					  currow = r;
					}
#line 3699 "gram.c" /* yacc.c:1646  */
    break;

  case 146:
#line 810 "gram.y" /* yacc.c:1646  */
    { int c = curcol, arg;
	    				  if (showrange == SHOWCOLS) {
					    if (showsc < curcol) {
						curcol = showsc;
						showsc = c;
					    }
					    arg = showsc - curcol + 1;
					  } else
					    arg = 1;
					  yankcol(arg);
					  curcol = c;
					}
#line 3716 "gram.c" /* yacc.c:1646  */
    break;

  case 147:
#line 822 "gram.y" /* yacc.c:1646  */
    { int c = curcol;
					  curcol = (yyvsp[0].ival);
					  yankcol(1);
					  curcol = c;
					}
#line 3726 "gram.c" /* yacc.c:1646  */
    break;

  case 148:
#line 827 "gram.y" /* yacc.c:1646  */
    { yankcol((yyvsp[0].ival)); }
#line 3732 "gram.c" /* yacc.c:1646  */
    break;

  case 149:
#line 828 "gram.y" /* yacc.c:1646  */
    { int c = curcol, arg;
					  if ((yyvsp[-2].ival) < (yyvsp[0].ival)) {
					    curcol = (yyvsp[-2].ival);
					    arg = (yyvsp[0].ival) - (yyvsp[-2].ival) + 1;
					  } else {
					      curcol = (yyvsp[0].ival);
					      arg = (yyvsp[-2].ival) - (yyvsp[0].ival) + 1;
					  }
					  yankcol(arg);
					  curcol = c;
					}
#line 3748 "gram.c" /* yacc.c:1646  */
    break;

  case 150:
#line 839 "gram.y" /* yacc.c:1646  */
    { pullcells('p'); }
#line 3754 "gram.c" /* yacc.c:1646  */
    break;

  case 151:
#line 840 "gram.y" /* yacc.c:1646  */
    { pullcells('m'); }
#line 3760 "gram.c" /* yacc.c:1646  */
    break;

  case 152:
#line 841 "gram.y" /* yacc.c:1646  */
    { pullcells('r'); }
#line 3766 "gram.c" /* yacc.c:1646  */
    break;

  case 153:
#line 842 "gram.y" /* yacc.c:1646  */
    { pullcells('c'); }
#line 3772 "gram.c" /* yacc.c:1646  */
    break;

  case 154:
#line 843 "gram.y" /* yacc.c:1646  */
    { pullcells('x'); }
#line 3778 "gram.c" /* yacc.c:1646  */
    break;

  case 155:
#line 844 "gram.y" /* yacc.c:1646  */
    { pullcells('t'); }
#line 3784 "gram.c" /* yacc.c:1646  */
    break;

  case 156:
#line 845 "gram.y" /* yacc.c:1646  */
    { pullcells('f'); }
#line 3790 "gram.c" /* yacc.c:1646  */
    break;

  case 157:
#line 846 "gram.y" /* yacc.c:1646  */
    { copy(NULL, NULL, NULL, NULL); }
#line 3796 "gram.c" /* yacc.c:1646  */
    break;

  case 158:
#line 847 "gram.y" /* yacc.c:1646  */
    { copy((yyvsp[0].rval).left.vp, (yyvsp[0].rval).right.vp,
					     NULL, (struct ent *)1); }
#line 3803 "gram.c" /* yacc.c:1646  */
    break;

  case 159:
#line 849 "gram.y" /* yacc.c:1646  */
    { sprintf(line, "%s%d ",
					     coltoa(curcol), currow);
					  sprintf(line + strlen(line), "%s%d\n",
					     coltoa(stcol), strow);
					  write(macrofd, line, strlen(line));
					  line[0] = '\0'; }
#line 3814 "gram.c" /* yacc.c:1646  */
    break;

  case 160:
#line 855 "gram.y" /* yacc.c:1646  */
    { sprintf(line, "%s%d ",
					     coltoa(curcol), currow);
					  sprintf(line + strlen(line), "%s%d\n",
					     coltoa(stcol), strow);
					  write((yyvsp[0].ival), line, strlen(line));
					  line[0] = '\0'; }
#line 3825 "gram.c" /* yacc.c:1646  */
    break;

  case 161:
#line 861 "gram.y" /* yacc.c:1646  */
    { getnum((yyvsp[0].rval).left.vp->row,
						(yyvsp[0].rval).left.vp->col,
						(yyvsp[0].rval).right.vp->row,
						(yyvsp[0].rval).right.vp->col, macrofd); }
#line 3834 "gram.c" /* yacc.c:1646  */
    break;

  case 162:
#line 866 "gram.y" /* yacc.c:1646  */
    { getnum((yyvsp[-2].rval).left.vp->row,
						(yyvsp[-2].rval).left.vp->col,
						(yyvsp[-2].rval).right.vp->row,
						(yyvsp[-2].rval).right.vp->col, (yyvsp[0].ival)); }
#line 3843 "gram.c" /* yacc.c:1646  */
    break;

  case 163:
#line 870 "gram.y" /* yacc.c:1646  */
    { getnum(currow, curcol,
						currow, curcol, macrofd); }
#line 3850 "gram.c" /* yacc.c:1646  */
    break;

  case 164:
#line 872 "gram.y" /* yacc.c:1646  */
    { getnum(currow, curcol,
						currow, curcol, (yyvsp[0].ival)); }
#line 3857 "gram.c" /* yacc.c:1646  */
    break;

  case 165:
#line 874 "gram.y" /* yacc.c:1646  */
    { fgetnum((yyvsp[0].rval).left.vp->row,
						(yyvsp[0].rval).left.vp->col,
						(yyvsp[0].rval).right.vp->row,
						(yyvsp[0].rval).right.vp->col, macrofd); }
#line 3866 "gram.c" /* yacc.c:1646  */
    break;

  case 166:
#line 879 "gram.y" /* yacc.c:1646  */
    { fgetnum((yyvsp[-2].rval).left.vp->row,
						(yyvsp[-2].rval).left.vp->col,
						(yyvsp[-2].rval).right.vp->row,
						(yyvsp[-2].rval).right.vp->col, (yyvsp[0].ival)); }
#line 3875 "gram.c" /* yacc.c:1646  */
    break;

  case 167:
#line 883 "gram.y" /* yacc.c:1646  */
    { fgetnum(currow, curcol,
						currow, curcol, macrofd); }
#line 3882 "gram.c" /* yacc.c:1646  */
    break;

  case 168:
#line 885 "gram.y" /* yacc.c:1646  */
    { fgetnum(currow, curcol,
						currow, curcol, (yyvsp[0].ival)); }
#line 3889 "gram.c" /* yacc.c:1646  */
    break;

  case 169:
#line 888 "gram.y" /* yacc.c:1646  */
    { getstring((yyvsp[0].rval).left.vp->row,
						(yyvsp[0].rval).left.vp->col,
						(yyvsp[0].rval).right.vp->row,
						(yyvsp[0].rval).right.vp->col, macrofd); }
#line 3898 "gram.c" /* yacc.c:1646  */
    break;

  case 170:
#line 893 "gram.y" /* yacc.c:1646  */
    { getstring((yyvsp[-2].rval).left.vp->row,
						(yyvsp[-2].rval).left.vp->col,
						(yyvsp[-2].rval).right.vp->row,
						(yyvsp[-2].rval).right.vp->col, (yyvsp[0].ival)); }
#line 3907 "gram.c" /* yacc.c:1646  */
    break;

  case 171:
#line 897 "gram.y" /* yacc.c:1646  */
    { getstring(currow, curcol,
						currow, curcol, macrofd); }
#line 3914 "gram.c" /* yacc.c:1646  */
    break;

  case 172:
#line 899 "gram.y" /* yacc.c:1646  */
    { getstring(currow, curcol,
						currow, curcol, (yyvsp[0].ival)); }
#line 3921 "gram.c" /* yacc.c:1646  */
    break;

  case 173:
#line 901 "gram.y" /* yacc.c:1646  */
    { getexp((yyvsp[0].rval).left.vp->row,
						(yyvsp[0].rval).left.vp->col,
						(yyvsp[0].rval).right.vp->row,
						(yyvsp[0].rval).right.vp->col, macrofd); }
#line 3930 "gram.c" /* yacc.c:1646  */
    break;

  case 174:
#line 906 "gram.y" /* yacc.c:1646  */
    { getexp((yyvsp[-2].rval).left.vp->row,
						(yyvsp[-2].rval).left.vp->col,
						(yyvsp[-2].rval).right.vp->row,
						(yyvsp[-2].rval).right.vp->col, (yyvsp[0].ival)); }
#line 3939 "gram.c" /* yacc.c:1646  */
    break;

  case 175:
#line 910 "gram.y" /* yacc.c:1646  */
    { getexp(currow, curcol,
						currow, curcol, macrofd); }
#line 3946 "gram.c" /* yacc.c:1646  */
    break;

  case 176:
#line 912 "gram.y" /* yacc.c:1646  */
    { getexp(currow, curcol,
						currow, curcol, (yyvsp[0].ival)); }
#line 3953 "gram.c" /* yacc.c:1646  */
    break;

  case 177:
#line 914 "gram.y" /* yacc.c:1646  */
    { getformat((yyvsp[0].ival), macrofd); }
#line 3959 "gram.c" /* yacc.c:1646  */
    break;

  case 178:
#line 916 "gram.y" /* yacc.c:1646  */
    { getformat((yyvsp[-2].ival), (yyvsp[0].ival)); }
#line 3965 "gram.c" /* yacc.c:1646  */
    break;

  case 179:
#line 917 "gram.y" /* yacc.c:1646  */
    { getformat(curcol, macrofd); }
#line 3971 "gram.c" /* yacc.c:1646  */
    break;

  case 180:
#line 918 "gram.y" /* yacc.c:1646  */
    { getformat(curcol, (yyvsp[0].ival)); }
#line 3977 "gram.c" /* yacc.c:1646  */
    break;

  case 181:
#line 919 "gram.y" /* yacc.c:1646  */
    { getfmt((yyvsp[0].rval).left.vp->row,
						(yyvsp[0].rval).left.vp->col,
						(yyvsp[0].rval).right.vp->row,
						(yyvsp[0].rval).right.vp->col, macrofd); }
#line 3986 "gram.c" /* yacc.c:1646  */
    break;

  case 182:
#line 924 "gram.y" /* yacc.c:1646  */
    { getfmt((yyvsp[-2].rval).left.vp->row,
						(yyvsp[-2].rval).left.vp->col,
						(yyvsp[-2].rval).right.vp->row,
						(yyvsp[-2].rval).right.vp->col, (yyvsp[0].ival)); }
#line 3995 "gram.c" /* yacc.c:1646  */
    break;

  case 183:
#line 928 "gram.y" /* yacc.c:1646  */
    { getfmt(currow, curcol,
						currow, curcol, macrofd); }
#line 4002 "gram.c" /* yacc.c:1646  */
    break;

  case 184:
#line 930 "gram.y" /* yacc.c:1646  */
    { getfmt(currow, curcol,
						currow, curcol, (yyvsp[0].ival)); }
#line 4009 "gram.c" /* yacc.c:1646  */
    break;

  case 185:
#line 932 "gram.y" /* yacc.c:1646  */
    { getframe(macrofd); }
#line 4015 "gram.c" /* yacc.c:1646  */
    break;

  case 186:
#line 933 "gram.y" /* yacc.c:1646  */
    { getframe((yyvsp[0].ival)); }
#line 4021 "gram.c" /* yacc.c:1646  */
    break;

  case 187:
#line 934 "gram.y" /* yacc.c:1646  */
    { getrange((yyvsp[0].sval), macrofd); }
#line 4027 "gram.c" /* yacc.c:1646  */
    break;

  case 188:
#line 936 "gram.y" /* yacc.c:1646  */
    { getrange((yyvsp[-2].sval), (yyvsp[0].ival)); }
#line 4033 "gram.c" /* yacc.c:1646  */
    break;

  case 189:
#line 937 "gram.y" /* yacc.c:1646  */
    { doeval((yyvsp[0].enode), NULL, currow, curcol, macrofd); }
#line 4039 "gram.c" /* yacc.c:1646  */
    break;

  case 190:
#line 938 "gram.y" /* yacc.c:1646  */
    { doeval((yyvsp[-1].enode), (yyvsp[0].sval), currow, curcol, macrofd); }
#line 4045 "gram.c" /* yacc.c:1646  */
    break;

  case 191:
#line 940 "gram.y" /* yacc.c:1646  */
    { doeval((yyvsp[-3].enode), (yyvsp[-2].sval), currow, curcol, macrofd); }
#line 4051 "gram.c" /* yacc.c:1646  */
    break;

  case 192:
#line 941 "gram.y" /* yacc.c:1646  */
    { doseval((yyvsp[0].enode), currow, curcol, macrofd); }
#line 4057 "gram.c" /* yacc.c:1646  */
    break;

  case 193:
#line 942 "gram.y" /* yacc.c:1646  */
    { doquery((yyvsp[-1].sval), (yyvsp[0].sval), macrofd); }
#line 4063 "gram.c" /* yacc.c:1646  */
    break;

  case 194:
#line 944 "gram.y" /* yacc.c:1646  */
    { doquery((yyvsp[-3].sval), (yyvsp[-2].sval), (yyvsp[0].ival)); }
#line 4069 "gram.c" /* yacc.c:1646  */
    break;

  case 195:
#line 945 "gram.y" /* yacc.c:1646  */
    { doquery((yyvsp[0].sval), NULL, macrofd); }
#line 4075 "gram.c" /* yacc.c:1646  */
    break;

  case 196:
#line 947 "gram.y" /* yacc.c:1646  */
    { doquery((yyvsp[-2].sval), NULL, (yyvsp[0].ival)); }
#line 4081 "gram.c" /* yacc.c:1646  */
    break;

  case 197:
#line 948 "gram.y" /* yacc.c:1646  */
    { doquery(NULL, NULL, macrofd); }
#line 4087 "gram.c" /* yacc.c:1646  */
    break;

  case 198:
#line 949 "gram.y" /* yacc.c:1646  */
    { doquery(NULL, NULL, (yyvsp[0].ival)); }
#line 4093 "gram.c" /* yacc.c:1646  */
    break;

  case 199:
#line 950 "gram.y" /* yacc.c:1646  */
    { dogetkey(); }
#line 4099 "gram.c" /* yacc.c:1646  */
    break;

  case 200:
#line 951 "gram.y" /* yacc.c:1646  */
    { error((yyvsp[0].sval)); }
#line 4105 "gram.c" /* yacc.c:1646  */
    break;

  case 201:
#line 952 "gram.y" /* yacc.c:1646  */
    { dostat(macrofd); }
#line 4111 "gram.c" /* yacc.c:1646  */
    break;

  case 202:
#line 953 "gram.y" /* yacc.c:1646  */
    { dostat((yyvsp[0].ival)); }
#line 4117 "gram.c" /* yacc.c:1646  */
    break;

  case 203:
#line 954 "gram.y" /* yacc.c:1646  */
    { EvalAll();
					  update(1);
					  changed = 0;
					}
#line 4126 "gram.c" /* yacc.c:1646  */
    break;

  case 204:
#line 958 "gram.y" /* yacc.c:1646  */
    { if (usecurses) {
					    clearok(stdscr, TRUE);
					    linelim = -1;
					    update(1);
					    refresh();
					    changed = 0;
					  }
					}
#line 4139 "gram.c" /* yacc.c:1646  */
    break;

  case 205:
#line 966 "gram.y" /* yacc.c:1646  */
    { stopdisp(); exit(0); }
#line 4145 "gram.c" /* yacc.c:1646  */
    break;

  case 206:
#line 967 "gram.y" /* yacc.c:1646  */
    { deraw(1);
					  system((yyvsp[0].sval));
					  if (*((yyvsp[0].sval) + strlen((yyvsp[0].sval)) - 1) != '&') {
					    printf("Press any key to continue ");
					    fflush(stdout);
					    cbreak();
					    nmgetch();
					  }
					  goraw();
					  scxfree((yyvsp[0].sval)); }
#line 4160 "gram.c" /* yacc.c:1646  */
    break;

  case 207:
#line 978 "gram.y" /* yacc.c:1646  */
    { addplugin((yyvsp[-2].sval), (yyvsp[0].sval), 'r'); }
#line 4166 "gram.c" /* yacc.c:1646  */
    break;

  case 208:
#line 980 "gram.y" /* yacc.c:1646  */
    { addplugin((yyvsp[-2].sval), (yyvsp[0].sval), 'w'); }
#line 4172 "gram.c" /* yacc.c:1646  */
    break;

  case 209:
#line 981 "gram.y" /* yacc.c:1646  */
    { *line = '|';
					  sprintf(line + 1, (yyvsp[0].sval));
					  readfile(line, 0);
					  scxfree((yyvsp[0].sval)); }
#line 4181 "gram.c" /* yacc.c:1646  */
    break;

  case 212:
#line 988 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new_var(O_VAR, (yyvsp[0].ent)); }
#line 4187 "gram.c" /* yacc.c:1646  */
    break;

  case 213:
#line 989 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new('f', (yyvsp[0].enode), NULL); }
#line 4193 "gram.c" /* yacc.c:1646  */
    break;

  case 214:
#line 991 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new('F', (yyvsp[0].enode), NULL); }
#line 4199 "gram.c" /* yacc.c:1646  */
    break;

  case 215:
#line 993 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(SUM,
					new_range(REDUCE | SUM, (yyvsp[-1].rval)), NULL); }
#line 4206 "gram.c" /* yacc.c:1646  */
    break;

  case 216:
#line 996 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(SUM,
					new_range(REDUCE | SUM, (yyvsp[-3].rval)), (yyvsp[-1].enode)); }
#line 4213 "gram.c" /* yacc.c:1646  */
    break;

  case 217:
#line 999 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(PROD,
					new_range(REDUCE | PROD, (yyvsp[-1].rval)), NULL); }
#line 4220 "gram.c" /* yacc.c:1646  */
    break;

  case 218:
#line 1002 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(PROD,
					new_range(REDUCE | PROD, (yyvsp[-3].rval)), (yyvsp[-1].enode)); }
#line 4227 "gram.c" /* yacc.c:1646  */
    break;

  case 219:
#line 1005 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(AVG,
					new_range(REDUCE | AVG, (yyvsp[-1].rval)), NULL); }
#line 4234 "gram.c" /* yacc.c:1646  */
    break;

  case 220:
#line 1008 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(AVG,
					new_range(REDUCE | AVG, (yyvsp[-3].rval)), (yyvsp[-1].enode)); }
#line 4241 "gram.c" /* yacc.c:1646  */
    break;

  case 221:
#line 1011 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(STDDEV,
					new_range(REDUCE | STDDEV, (yyvsp[-1].rval)), NULL); }
#line 4248 "gram.c" /* yacc.c:1646  */
    break;

  case 222:
#line 1014 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(STDDEV,
					new_range(REDUCE | STDDEV, (yyvsp[-3].rval)), (yyvsp[-1].enode)); }
#line 4255 "gram.c" /* yacc.c:1646  */
    break;

  case 223:
#line 1017 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(COUNT,
					new_range(REDUCE | COUNT, (yyvsp[-1].rval)), NULL); }
#line 4262 "gram.c" /* yacc.c:1646  */
    break;

  case 224:
#line 1020 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(COUNT,
					new_range(REDUCE | COUNT, (yyvsp[-3].rval)), (yyvsp[-1].enode)); }
#line 4269 "gram.c" /* yacc.c:1646  */
    break;

  case 225:
#line 1023 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(MAX,
					new_range(REDUCE | MAX, (yyvsp[-1].rval)), NULL); }
#line 4276 "gram.c" /* yacc.c:1646  */
    break;

  case 226:
#line 1026 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(MAX,
					new_range(REDUCE | MAX, (yyvsp[-3].rval)), (yyvsp[-1].enode)); }
#line 4283 "gram.c" /* yacc.c:1646  */
    break;

  case 227:
#line 1029 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(LMAX, (yyvsp[-1].enode), (yyvsp[-3].enode)); }
#line 4289 "gram.c" /* yacc.c:1646  */
    break;

  case 228:
#line 1031 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(MIN,
					new_range(REDUCE | MIN, (yyvsp[-1].rval)), NULL); }
#line 4296 "gram.c" /* yacc.c:1646  */
    break;

  case 229:
#line 1034 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(MIN,
					new_range(REDUCE | MIN, (yyvsp[-3].rval)), (yyvsp[-1].enode)); }
#line 4303 "gram.c" /* yacc.c:1646  */
    break;

  case 230:
#line 1037 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(LMIN, (yyvsp[-1].enode), (yyvsp[-3].enode)); }
#line 4309 "gram.c" /* yacc.c:1646  */
    break;

  case 231:
#line 1039 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new_range(REDUCE | 'R', (yyvsp[-1].rval)); }
#line 4315 "gram.c" /* yacc.c:1646  */
    break;

  case 232:
#line 1041 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new_range(REDUCE | 'C', (yyvsp[-1].rval)); }
#line 4321 "gram.c" /* yacc.c:1646  */
    break;

  case 233:
#line 1042 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(ABS, (yyvsp[-1].enode), NULL); }
#line 4327 "gram.c" /* yacc.c:1646  */
    break;

  case 234:
#line 1043 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(ACOS, (yyvsp[-1].enode), NULL); }
#line 4333 "gram.c" /* yacc.c:1646  */
    break;

  case 235:
#line 1044 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(ASIN, (yyvsp[-1].enode), NULL); }
#line 4339 "gram.c" /* yacc.c:1646  */
    break;

  case 236:
#line 1045 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(ATAN, (yyvsp[-1].enode), NULL); }
#line 4345 "gram.c" /* yacc.c:1646  */
    break;

  case 237:
#line 1046 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(ATAN2, (yyvsp[-3].enode), (yyvsp[-1].enode)); }
#line 4351 "gram.c" /* yacc.c:1646  */
    break;

  case 238:
#line 1047 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(CEIL, (yyvsp[-1].enode), NULL); }
#line 4357 "gram.c" /* yacc.c:1646  */
    break;

  case 239:
#line 1048 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(COS, (yyvsp[-1].enode), NULL); }
#line 4363 "gram.c" /* yacc.c:1646  */
    break;

  case 240:
#line 1049 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(EXP, (yyvsp[-1].enode), NULL); }
#line 4369 "gram.c" /* yacc.c:1646  */
    break;

  case 241:
#line 1050 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(FABS, (yyvsp[-1].enode), NULL); }
#line 4375 "gram.c" /* yacc.c:1646  */
    break;

  case 242:
#line 1051 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(FLOOR, (yyvsp[-1].enode), NULL); }
#line 4381 "gram.c" /* yacc.c:1646  */
    break;

  case 243:
#line 1052 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(HYPOT, (yyvsp[-3].enode), (yyvsp[-1].enode)); }
#line 4387 "gram.c" /* yacc.c:1646  */
    break;

  case 244:
#line 1053 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(LOG, (yyvsp[-1].enode), NULL); }
#line 4393 "gram.c" /* yacc.c:1646  */
    break;

  case 245:
#line 1054 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(LOG10, (yyvsp[-1].enode), NULL); }
#line 4399 "gram.c" /* yacc.c:1646  */
    break;

  case 246:
#line 1055 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(POW, (yyvsp[-3].enode), (yyvsp[-1].enode)); }
#line 4405 "gram.c" /* yacc.c:1646  */
    break;

  case 247:
#line 1056 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(SIN, (yyvsp[-1].enode), NULL); }
#line 4411 "gram.c" /* yacc.c:1646  */
    break;

  case 248:
#line 1057 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(SQRT, (yyvsp[-1].enode), NULL); }
#line 4417 "gram.c" /* yacc.c:1646  */
    break;

  case 249:
#line 1058 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(TAN, (yyvsp[-1].enode), NULL); }
#line 4423 "gram.c" /* yacc.c:1646  */
    break;

  case 250:
#line 1059 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(DTR, (yyvsp[-1].enode), NULL); }
#line 4429 "gram.c" /* yacc.c:1646  */
    break;

  case 251:
#line 1060 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(RTD, (yyvsp[-1].enode), NULL); }
#line 4435 "gram.c" /* yacc.c:1646  */
    break;

  case 252:
#line 1061 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(RND, (yyvsp[-1].enode), NULL); }
#line 4441 "gram.c" /* yacc.c:1646  */
    break;

  case 253:
#line 1062 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(ROUND, (yyvsp[-3].enode), (yyvsp[-1].enode)); }
#line 4447 "gram.c" /* yacc.c:1646  */
    break;

  case 254:
#line 1063 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(IF,  (yyvsp[-5].enode),new(',',(yyvsp[-3].enode),(yyvsp[-1].enode))); }
#line 4453 "gram.c" /* yacc.c:1646  */
    break;

  case 255:
#line 1065 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(PV,  (yyvsp[-5].enode),new(':',(yyvsp[-3].enode),(yyvsp[-1].enode))); }
#line 4459 "gram.c" /* yacc.c:1646  */
    break;

  case 256:
#line 1066 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(FV,  (yyvsp[-5].enode),new(':',(yyvsp[-3].enode),(yyvsp[-1].enode))); }
#line 4465 "gram.c" /* yacc.c:1646  */
    break;

  case 257:
#line 1067 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(PMT, (yyvsp[-5].enode),new(':',(yyvsp[-3].enode),(yyvsp[-1].enode))); }
#line 4471 "gram.c" /* yacc.c:1646  */
    break;

  case 258:
#line 1069 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(HOUR, (yyvsp[-1].enode), NULL); }
#line 4477 "gram.c" /* yacc.c:1646  */
    break;

  case 259:
#line 1070 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(MINUTE, (yyvsp[-1].enode), NULL); }
#line 4483 "gram.c" /* yacc.c:1646  */
    break;

  case 260:
#line 1071 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(SECOND, (yyvsp[-1].enode), NULL); }
#line 4489 "gram.c" /* yacc.c:1646  */
    break;

  case 261:
#line 1072 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(MONTH, (yyvsp[-1].enode), NULL); }
#line 4495 "gram.c" /* yacc.c:1646  */
    break;

  case 262:
#line 1073 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(DAY, (yyvsp[-1].enode), NULL); }
#line 4501 "gram.c" /* yacc.c:1646  */
    break;

  case 263:
#line 1074 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(YEAR, (yyvsp[-1].enode), NULL); }
#line 4507 "gram.c" /* yacc.c:1646  */
    break;

  case 264:
#line 1075 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(NOW, NULL, NULL);}
#line 4513 "gram.c" /* yacc.c:1646  */
    break;

  case 265:
#line 1077 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(DTS, (yyvsp[-5].enode), new(',', (yyvsp[-3].enode), (yyvsp[-1].enode)));}
#line 4519 "gram.c" /* yacc.c:1646  */
    break;

  case 266:
#line 1078 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(DTS,
				new_const(O_CONST, (double) (yyvsp[-4].ival)),
				new(',', new_const(O_CONST, (double) (yyvsp[-2].ival)),
				new_const(O_CONST, (double) (yyvsp[0].ival))));}
#line 4528 "gram.c" /* yacc.c:1646  */
    break;

  case 267:
#line 1083 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(TTS, (yyvsp[-5].enode), new(',', (yyvsp[-3].enode), (yyvsp[-1].enode)));}
#line 4534 "gram.c" /* yacc.c:1646  */
    break;

  case 268:
#line 1084 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(STON, (yyvsp[-1].enode), NULL); }
#line 4540 "gram.c" /* yacc.c:1646  */
    break;

  case 269:
#line 1085 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(EQS, (yyvsp[-3].enode), (yyvsp[-1].enode)); }
#line 4546 "gram.c" /* yacc.c:1646  */
    break;

  case 270:
#line 1086 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(DATE, (yyvsp[-1].enode), NULL); }
#line 4552 "gram.c" /* yacc.c:1646  */
    break;

  case 271:
#line 1087 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(DATE, (yyvsp[-3].enode), (yyvsp[-1].enode)); }
#line 4558 "gram.c" /* yacc.c:1646  */
    break;

  case 272:
#line 1088 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(FMT, (yyvsp[-3].enode), (yyvsp[-1].enode)); }
#line 4564 "gram.c" /* yacc.c:1646  */
    break;

  case 273:
#line 1089 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(UPPER, (yyvsp[-1].enode), NULL); }
#line 4570 "gram.c" /* yacc.c:1646  */
    break;

  case 274:
#line 1090 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(LOWER, (yyvsp[-1].enode), NULL); }
#line 4576 "gram.c" /* yacc.c:1646  */
    break;

  case 275:
#line 1091 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(CAPITAL, (yyvsp[-1].enode), NULL); }
#line 4582 "gram.c" /* yacc.c:1646  */
    break;

  case 276:
#line 1093 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(INDEX, new_range(REDUCE | INDEX, (yyvsp[-3].rval)), (yyvsp[-1].enode)); }
#line 4588 "gram.c" /* yacc.c:1646  */
    break;

  case 277:
#line 1095 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(INDEX, new_range(REDUCE | INDEX, (yyvsp[-1].rval)), (yyvsp[-3].enode)); }
#line 4594 "gram.c" /* yacc.c:1646  */
    break;

  case 278:
#line 1097 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(INDEX, new_range(REDUCE | INDEX, (yyvsp[-5].rval)),
		    new(',', (yyvsp[-3].enode), (yyvsp[-1].enode))); }
#line 4601 "gram.c" /* yacc.c:1646  */
    break;

  case 279:
#line 1100 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(LOOKUP, new_range(REDUCE | LOOKUP, (yyvsp[-3].rval)), (yyvsp[-1].enode)); }
#line 4607 "gram.c" /* yacc.c:1646  */
    break;

  case 280:
#line 1102 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(LOOKUP, new_range(REDUCE | LOOKUP, (yyvsp[-1].rval)), (yyvsp[-3].enode)); }
#line 4613 "gram.c" /* yacc.c:1646  */
    break;

  case 281:
#line 1104 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(HLOOKUP, new_range(REDUCE | HLOOKUP, (yyvsp[-5].rval)),
		    new(',', (yyvsp[-3].enode), (yyvsp[-1].enode))); }
#line 4620 "gram.c" /* yacc.c:1646  */
    break;

  case 282:
#line 1107 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(HLOOKUP, new_range(REDUCE | HLOOKUP, (yyvsp[-3].rval)),
		    new(',', (yyvsp[-5].enode), (yyvsp[-1].enode))); }
#line 4627 "gram.c" /* yacc.c:1646  */
    break;

  case 283:
#line 1110 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(VLOOKUP, new_range(REDUCE | VLOOKUP, (yyvsp[-5].rval)),
		    new(',', (yyvsp[-3].enode), (yyvsp[-1].enode))); }
#line 4634 "gram.c" /* yacc.c:1646  */
    break;

  case 284:
#line 1113 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(VLOOKUP, new_range(REDUCE | VLOOKUP, (yyvsp[-3].rval)),
		    new(',', (yyvsp[-5].enode), (yyvsp[-1].enode))); }
#line 4641 "gram.c" /* yacc.c:1646  */
    break;

  case 285:
#line 1116 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(STINDEX, new_range(REDUCE | STINDEX, (yyvsp[-3].rval)), (yyvsp[-1].enode)); }
#line 4647 "gram.c" /* yacc.c:1646  */
    break;

  case 286:
#line 1118 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(STINDEX, new_range(REDUCE | STINDEX, (yyvsp[-1].rval)), (yyvsp[-3].enode)); }
#line 4653 "gram.c" /* yacc.c:1646  */
    break;

  case 287:
#line 1120 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(STINDEX, new_range(REDUCE | STINDEX, (yyvsp[-5].rval)),
		    new(',', (yyvsp[-3].enode), (yyvsp[-1].enode))); }
#line 4660 "gram.c" /* yacc.c:1646  */
    break;

  case 288:
#line 1122 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(EXT, (yyvsp[-3].enode), (yyvsp[-1].enode)); }
#line 4666 "gram.c" /* yacc.c:1646  */
    break;

  case 289:
#line 1123 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(NVAL, (yyvsp[-3].enode), (yyvsp[-1].enode)); }
#line 4672 "gram.c" /* yacc.c:1646  */
    break;

  case 290:
#line 1124 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(SVAL, (yyvsp[-3].enode), (yyvsp[-1].enode)); }
#line 4678 "gram.c" /* yacc.c:1646  */
    break;

  case 291:
#line 1126 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(SUBSTR, (yyvsp[-5].enode), new(',', (yyvsp[-3].enode), (yyvsp[-1].enode))); }
#line 4684 "gram.c" /* yacc.c:1646  */
    break;

  case 292:
#line 1127 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = (yyvsp[-1].enode); }
#line 4690 "gram.c" /* yacc.c:1646  */
    break;

  case 293:
#line 1128 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = (yyvsp[0].enode); }
#line 4696 "gram.c" /* yacc.c:1646  */
    break;

  case 294:
#line 1129 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new('m', (yyvsp[0].enode), NULL); }
#line 4702 "gram.c" /* yacc.c:1646  */
    break;

  case 295:
#line 1130 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new_const(O_CONST, (double) (yyvsp[0].ival)); }
#line 4708 "gram.c" /* yacc.c:1646  */
    break;

  case 296:
#line 1131 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new_const(O_CONST, (yyvsp[0].fval)); }
#line 4714 "gram.c" /* yacc.c:1646  */
    break;

  case 297:
#line 1132 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(PI_, NULL, NULL); }
#line 4720 "gram.c" /* yacc.c:1646  */
    break;

  case 298:
#line 1133 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new_str((yyvsp[0].sval)); }
#line 4726 "gram.c" /* yacc.c:1646  */
    break;

  case 299:
#line 1134 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new('!', (yyvsp[0].enode), NULL); }
#line 4732 "gram.c" /* yacc.c:1646  */
    break;

  case 300:
#line 1135 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new('!', (yyvsp[0].enode), NULL); }
#line 4738 "gram.c" /* yacc.c:1646  */
    break;

  case 301:
#line 1136 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(FILENAME, (yyvsp[-1].enode), NULL); }
#line 4744 "gram.c" /* yacc.c:1646  */
    break;

  case 302:
#line 1137 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(MYROW, NULL, NULL);}
#line 4750 "gram.c" /* yacc.c:1646  */
    break;

  case 303:
#line 1138 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(MYCOL, NULL, NULL);}
#line 4756 "gram.c" /* yacc.c:1646  */
    break;

  case 304:
#line 1139 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(LASTROW, NULL, NULL);}
#line 4762 "gram.c" /* yacc.c:1646  */
    break;

  case 305:
#line 1140 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(LASTCOL, NULL, NULL);}
#line 4768 "gram.c" /* yacc.c:1646  */
    break;

  case 306:
#line 1141 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(COLTOA, (yyvsp[-1].enode), NULL);}
#line 4774 "gram.c" /* yacc.c:1646  */
    break;

  case 307:
#line 1142 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(NUMITER, NULL, NULL);}
#line 4780 "gram.c" /* yacc.c:1646  */
    break;

  case 308:
#line 1143 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(ERR_, NULL, NULL); }
#line 4786 "gram.c" /* yacc.c:1646  */
    break;

  case 309:
#line 1144 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(ERR_, NULL, NULL); }
#line 4792 "gram.c" /* yacc.c:1646  */
    break;

  case 310:
#line 1145 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(BLACK, NULL, NULL); }
#line 4798 "gram.c" /* yacc.c:1646  */
    break;

  case 311:
#line 1146 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(RED, NULL, NULL); }
#line 4804 "gram.c" /* yacc.c:1646  */
    break;

  case 312:
#line 1147 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(GREEN, NULL, NULL); }
#line 4810 "gram.c" /* yacc.c:1646  */
    break;

  case 313:
#line 1148 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(YELLOW, NULL, NULL); }
#line 4816 "gram.c" /* yacc.c:1646  */
    break;

  case 314:
#line 1149 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(BLUE, NULL, NULL); }
#line 4822 "gram.c" /* yacc.c:1646  */
    break;

  case 315:
#line 1150 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(MAGENTA, NULL, NULL); }
#line 4828 "gram.c" /* yacc.c:1646  */
    break;

  case 316:
#line 1151 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(CYAN, NULL, NULL); }
#line 4834 "gram.c" /* yacc.c:1646  */
    break;

  case 317:
#line 1152 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(WHITE, NULL, NULL); }
#line 4840 "gram.c" /* yacc.c:1646  */
    break;

  case 318:
#line 1156 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new('+', (yyvsp[-2].enode), (yyvsp[0].enode)); }
#line 4846 "gram.c" /* yacc.c:1646  */
    break;

  case 319:
#line 1157 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new('-', (yyvsp[-2].enode), (yyvsp[0].enode)); }
#line 4852 "gram.c" /* yacc.c:1646  */
    break;

  case 320:
#line 1158 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new('*', (yyvsp[-2].enode), (yyvsp[0].enode)); }
#line 4858 "gram.c" /* yacc.c:1646  */
    break;

  case 321:
#line 1159 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new('/', (yyvsp[-2].enode), (yyvsp[0].enode)); }
#line 4864 "gram.c" /* yacc.c:1646  */
    break;

  case 322:
#line 1160 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new('%', (yyvsp[-2].enode), (yyvsp[0].enode)); }
#line 4870 "gram.c" /* yacc.c:1646  */
    break;

  case 323:
#line 1161 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new('^', (yyvsp[-2].enode), (yyvsp[0].enode)); }
#line 4876 "gram.c" /* yacc.c:1646  */
    break;

  case 325:
#line 1163 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new('?', (yyvsp[-4].enode), new(':', (yyvsp[-2].enode), (yyvsp[0].enode))); }
#line 4882 "gram.c" /* yacc.c:1646  */
    break;

  case 326:
#line 1164 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(';', (yyvsp[-2].enode), (yyvsp[0].enode)); }
#line 4888 "gram.c" /* yacc.c:1646  */
    break;

  case 327:
#line 1165 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new('<', (yyvsp[-2].enode), (yyvsp[0].enode)); }
#line 4894 "gram.c" /* yacc.c:1646  */
    break;

  case 328:
#line 1166 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new('=', (yyvsp[-2].enode), (yyvsp[0].enode)); }
#line 4900 "gram.c" /* yacc.c:1646  */
    break;

  case 329:
#line 1167 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new('>', (yyvsp[-2].enode), (yyvsp[0].enode)); }
#line 4906 "gram.c" /* yacc.c:1646  */
    break;

  case 330:
#line 1168 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new('&', (yyvsp[-2].enode), (yyvsp[0].enode)); }
#line 4912 "gram.c" /* yacc.c:1646  */
    break;

  case 331:
#line 1169 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new('|', (yyvsp[-2].enode), (yyvsp[0].enode)); }
#line 4918 "gram.c" /* yacc.c:1646  */
    break;

  case 332:
#line 1170 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new('!', new('>', (yyvsp[-3].enode), (yyvsp[0].enode)), NULL); }
#line 4924 "gram.c" /* yacc.c:1646  */
    break;

  case 333:
#line 1171 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new('!', new('=', (yyvsp[-3].enode), (yyvsp[0].enode)), NULL); }
#line 4930 "gram.c" /* yacc.c:1646  */
    break;

  case 334:
#line 1172 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new('!', new('=', (yyvsp[-3].enode), (yyvsp[0].enode)), NULL); }
#line 4936 "gram.c" /* yacc.c:1646  */
    break;

  case 335:
#line 1173 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new('!', new('<', (yyvsp[-3].enode), (yyvsp[0].enode)), NULL); }
#line 4942 "gram.c" /* yacc.c:1646  */
    break;

  case 336:
#line 1174 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new('#', (yyvsp[-2].enode), (yyvsp[0].enode)); }
#line 4948 "gram.c" /* yacc.c:1646  */
    break;

  case 337:
#line 1177 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(ELIST, NULL, (yyvsp[0].enode)); }
#line 4954 "gram.c" /* yacc.c:1646  */
    break;

  case 338:
#line 1178 "gram.y" /* yacc.c:1646  */
    { (yyval.enode) = new(ELIST, (yyvsp[-2].enode), (yyvsp[0].enode)); }
#line 4960 "gram.c" /* yacc.c:1646  */
    break;

  case 339:
#line 1181 "gram.y" /* yacc.c:1646  */
    { (yyval.rval).left = (yyvsp[-2].ent); (yyval.rval).right = (yyvsp[0].ent); }
#line 4966 "gram.c" /* yacc.c:1646  */
    break;

  case 340:
#line 1182 "gram.y" /* yacc.c:1646  */
    { (yyval.rval) = (yyvsp[0].rval); }
#line 4972 "gram.c" /* yacc.c:1646  */
    break;

  case 341:
#line 1185 "gram.y" /* yacc.c:1646  */
    { (yyval.ent).vp = lookat((yyvsp[0].ival), (yyvsp[-1].ival)); (yyval.ent).vf = 0; }
#line 4978 "gram.c" /* yacc.c:1646  */
    break;

  case 342:
#line 1186 "gram.y" /* yacc.c:1646  */
    { (yyval.ent).vp = lookat((yyvsp[0].ival), (yyvsp[-1].ival));
					(yyval.ent).vf = FIX_COL; }
#line 4985 "gram.c" /* yacc.c:1646  */
    break;

  case 343:
#line 1188 "gram.y" /* yacc.c:1646  */
    { (yyval.ent).vp = lookat((yyvsp[0].ival), (yyvsp[-2].ival));
					(yyval.ent).vf = FIX_ROW; }
#line 4992 "gram.c" /* yacc.c:1646  */
    break;

  case 344:
#line 1190 "gram.y" /* yacc.c:1646  */
    { (yyval.ent).vp = lookat((yyvsp[0].ival), (yyvsp[-2].ival));
					(yyval.ent).vf = FIX_ROW | FIX_COL; }
#line 4999 "gram.c" /* yacc.c:1646  */
    break;

  case 345:
#line 1192 "gram.y" /* yacc.c:1646  */
    { (yyval.ent) = (yyvsp[0].rval).left; }
#line 5005 "gram.c" /* yacc.c:1646  */
    break;

  case 346:
#line 1195 "gram.y" /* yacc.c:1646  */
    { (yyval.rval) = (yyvsp[0].rval); }
#line 5011 "gram.c" /* yacc.c:1646  */
    break;

  case 347:
#line 1196 "gram.y" /* yacc.c:1646  */
    { (yyval.rval).left = (yyvsp[0].ent); (yyval.rval).right = (yyvsp[0].ent); }
#line 5017 "gram.c" /* yacc.c:1646  */
    break;

  case 348:
#line 1199 "gram.y" /* yacc.c:1646  */
    { (yyval.fval) = (double) (yyvsp[0].ival); }
#line 5023 "gram.c" /* yacc.c:1646  */
    break;

  case 349:
#line 1200 "gram.y" /* yacc.c:1646  */
    { (yyval.fval) = (yyvsp[0].fval); }
#line 5029 "gram.c" /* yacc.c:1646  */
    break;

  case 350:
#line 1201 "gram.y" /* yacc.c:1646  */
    { (yyval.fval) = -(yyvsp[0].fval); }
#line 5035 "gram.c" /* yacc.c:1646  */
    break;

  case 351:
#line 1202 "gram.y" /* yacc.c:1646  */
    { (yyval.fval) = (yyvsp[0].fval); }
#line 5041 "gram.c" /* yacc.c:1646  */
    break;

  case 352:
#line 1205 "gram.y" /* yacc.c:1646  */
    { (yyval.sval) = (yyvsp[0].sval); }
#line 5047 "gram.c" /* yacc.c:1646  */
    break;

  case 353:
#line 1206 "gram.y" /* yacc.c:1646  */
    {
				    char* s;
				    const char* s1;
				    s1 = (yyvsp[0].ent).vp->label;
				    if (!s1)
					s1 = "NULL_STRING";
				    s = strdup(s1);
				    (yyval.sval) = s;
				}
#line 5061 "gram.c" /* yacc.c:1646  */
    break;

  case 356:
#line 1223 "gram.y" /* yacc.c:1646  */
    { setauto(1); }
#line 5067 "gram.c" /* yacc.c:1646  */
    break;

  case 357:
#line 1224 "gram.y" /* yacc.c:1646  */
    { setauto(1); }
#line 5073 "gram.c" /* yacc.c:1646  */
    break;

  case 358:
#line 1225 "gram.y" /* yacc.c:1646  */
    { setauto(0); }
#line 5079 "gram.c" /* yacc.c:1646  */
    break;

  case 359:
#line 1226 "gram.y" /* yacc.c:1646  */
    { setauto(0); }
#line 5085 "gram.c" /* yacc.c:1646  */
    break;

  case 360:
#line 1227 "gram.y" /* yacc.c:1646  */
    { setauto(0); }
#line 5091 "gram.c" /* yacc.c:1646  */
    break;

  case 361:
#line 1228 "gram.y" /* yacc.c:1646  */
    { setauto(0); }
#line 5097 "gram.c" /* yacc.c:1646  */
    break;

  case 362:
#line 1229 "gram.y" /* yacc.c:1646  */
    { setorder(BYCOLS); }
#line 5103 "gram.c" /* yacc.c:1646  */
    break;

  case 363:
#line 1230 "gram.y" /* yacc.c:1646  */
    { setorder(BYROWS); }
#line 5109 "gram.c" /* yacc.c:1646  */
    break;

  case 364:
#line 1231 "gram.y" /* yacc.c:1646  */
    { optimize = 1; }
#line 5115 "gram.c" /* yacc.c:1646  */
    break;

  case 365:
#line 1232 "gram.y" /* yacc.c:1646  */
    { optimize = 0; }
#line 5121 "gram.c" /* yacc.c:1646  */
    break;

  case 366:
#line 1233 "gram.y" /* yacc.c:1646  */
    { optimize = 0; }
#line 5127 "gram.c" /* yacc.c:1646  */
    break;

  case 367:
#line 1234 "gram.y" /* yacc.c:1646  */
    { numeric = 1; }
#line 5133 "gram.c" /* yacc.c:1646  */
    break;

  case 368:
#line 1235 "gram.y" /* yacc.c:1646  */
    { numeric = 0; }
#line 5139 "gram.c" /* yacc.c:1646  */
    break;

  case 369:
#line 1236 "gram.y" /* yacc.c:1646  */
    { numeric = 0; }
#line 5145 "gram.c" /* yacc.c:1646  */
    break;

  case 370:
#line 1237 "gram.y" /* yacc.c:1646  */
    { prescale = 0.01; }
#line 5151 "gram.c" /* yacc.c:1646  */
    break;

  case 371:
#line 1238 "gram.y" /* yacc.c:1646  */
    { prescale = 1.0; }
#line 5157 "gram.c" /* yacc.c:1646  */
    break;

  case 372:
#line 1239 "gram.y" /* yacc.c:1646  */
    { prescale = 1.0; }
#line 5163 "gram.c" /* yacc.c:1646  */
    break;

  case 373:
#line 1240 "gram.y" /* yacc.c:1646  */
    { extfunc = 1; }
#line 5169 "gram.c" /* yacc.c:1646  */
    break;

  case 374:
#line 1241 "gram.y" /* yacc.c:1646  */
    { extfunc = 0; }
#line 5175 "gram.c" /* yacc.c:1646  */
    break;

  case 375:
#line 1242 "gram.y" /* yacc.c:1646  */
    { extfunc = 0; }
#line 5181 "gram.c" /* yacc.c:1646  */
    break;

  case 376:
#line 1243 "gram.y" /* yacc.c:1646  */
    { showcell = 1; }
#line 5187 "gram.c" /* yacc.c:1646  */
    break;

  case 377:
#line 1244 "gram.y" /* yacc.c:1646  */
    { showcell = 0; }
#line 5193 "gram.c" /* yacc.c:1646  */
    break;

  case 378:
#line 1245 "gram.y" /* yacc.c:1646  */
    { showcell = 0; }
#line 5199 "gram.c" /* yacc.c:1646  */
    break;

  case 379:
#line 1246 "gram.y" /* yacc.c:1646  */
    { showtop = 1; }
#line 5205 "gram.c" /* yacc.c:1646  */
    break;

  case 380:
#line 1247 "gram.y" /* yacc.c:1646  */
    { showtop = 0; }
#line 5211 "gram.c" /* yacc.c:1646  */
    break;

  case 381:
#line 1248 "gram.y" /* yacc.c:1646  */
    { showtop = 0; }
#line 5217 "gram.c" /* yacc.c:1646  */
    break;

  case 382:
#line 1249 "gram.y" /* yacc.c:1646  */
    { autoinsert = 1; }
#line 5223 "gram.c" /* yacc.c:1646  */
    break;

  case 383:
#line 1250 "gram.y" /* yacc.c:1646  */
    { autoinsert = 0; }
#line 5229 "gram.c" /* yacc.c:1646  */
    break;

  case 384:
#line 1251 "gram.y" /* yacc.c:1646  */
    { autoinsert = 0; }
#line 5235 "gram.c" /* yacc.c:1646  */
    break;

  case 385:
#line 1252 "gram.y" /* yacc.c:1646  */
    { autowrap = 1; }
#line 5241 "gram.c" /* yacc.c:1646  */
    break;

  case 386:
#line 1253 "gram.y" /* yacc.c:1646  */
    { autowrap = 0; }
#line 5247 "gram.c" /* yacc.c:1646  */
    break;

  case 387:
#line 1254 "gram.y" /* yacc.c:1646  */
    { autowrap = 0; }
#line 5253 "gram.c" /* yacc.c:1646  */
    break;

  case 388:
#line 1255 "gram.y" /* yacc.c:1646  */
    { cslop = 0; }
#line 5259 "gram.c" /* yacc.c:1646  */
    break;

  case 389:
#line 1256 "gram.y" /* yacc.c:1646  */
    { cslop = 1; }
#line 5265 "gram.c" /* yacc.c:1646  */
    break;

  case 390:
#line 1257 "gram.y" /* yacc.c:1646  */
    { cslop = 1; }
#line 5271 "gram.c" /* yacc.c:1646  */
    break;

  case 391:
#line 1258 "gram.y" /* yacc.c:1646  */
    { color = 1;
					  if (usecurses && has_colors()) {
					    color_set(1, NULL);
					    bkgd(COLOR_PAIR(1) | ' ');
					    FullUpdate++;
					  }
					}
#line 5283 "gram.c" /* yacc.c:1646  */
    break;

  case 392:
#line 1265 "gram.y" /* yacc.c:1646  */
    { color = 0;
					  if (usecurses && has_colors()) {
					    color_set(0, NULL);
					    bkgd(COLOR_PAIR(0) | ' ');
					  }
					}
#line 5294 "gram.c" /* yacc.c:1646  */
    break;

  case 393:
#line 1271 "gram.y" /* yacc.c:1646  */
    { color = 0;
					  if (usecurses && has_colors()) {
					    color_set(0, NULL);
					    bkgd(COLOR_PAIR(0) | ' ');
					  }
					}
#line 5305 "gram.c" /* yacc.c:1646  */
    break;

  case 394:
#line 1277 "gram.y" /* yacc.c:1646  */
    { colorneg = 1; }
#line 5311 "gram.c" /* yacc.c:1646  */
    break;

  case 395:
#line 1278 "gram.y" /* yacc.c:1646  */
    { colorneg = 0; }
#line 5317 "gram.c" /* yacc.c:1646  */
    break;

  case 396:
#line 1279 "gram.y" /* yacc.c:1646  */
    { colorneg = 0; }
#line 5323 "gram.c" /* yacc.c:1646  */
    break;

  case 397:
#line 1280 "gram.y" /* yacc.c:1646  */
    { colorerr = 1; }
#line 5329 "gram.c" /* yacc.c:1646  */
    break;

  case 398:
#line 1281 "gram.y" /* yacc.c:1646  */
    { colorerr = 0; }
#line 5335 "gram.c" /* yacc.c:1646  */
    break;

  case 399:
#line 1282 "gram.y" /* yacc.c:1646  */
    { colorerr = 0; }
#line 5341 "gram.c" /* yacc.c:1646  */
    break;

  case 400:
#line 1283 "gram.y" /* yacc.c:1646  */
    { braille = 1; }
#line 5347 "gram.c" /* yacc.c:1646  */
    break;

  case 401:
#line 1284 "gram.y" /* yacc.c:1646  */
    { braille = 0; }
#line 5353 "gram.c" /* yacc.c:1646  */
    break;

  case 402:
#line 1285 "gram.y" /* yacc.c:1646  */
    { braille = 0; }
#line 5359 "gram.c" /* yacc.c:1646  */
    break;

  case 403:
#line 1286 "gram.y" /* yacc.c:1646  */
    { setiterations((yyvsp[0].ival)); }
#line 5365 "gram.c" /* yacc.c:1646  */
    break;

  case 404:
#line 1287 "gram.y" /* yacc.c:1646  */
    { tbl_style = (yyvsp[0].ival); }
#line 5371 "gram.c" /* yacc.c:1646  */
    break;

  case 405:
#line 1288 "gram.y" /* yacc.c:1646  */
    { tbl_style = TBL; }
#line 5377 "gram.c" /* yacc.c:1646  */
    break;

  case 406:
#line 1289 "gram.y" /* yacc.c:1646  */
    { tbl_style = LATEX; }
#line 5383 "gram.c" /* yacc.c:1646  */
    break;

  case 407:
#line 1290 "gram.y" /* yacc.c:1646  */
    { tbl_style = SLATEX; }
#line 5389 "gram.c" /* yacc.c:1646  */
    break;

  case 408:
#line 1291 "gram.y" /* yacc.c:1646  */
    { tbl_style = TEX; }
#line 5395 "gram.c" /* yacc.c:1646  */
    break;

  case 409:
#line 1292 "gram.y" /* yacc.c:1646  */
    { tbl_style = FRAME; }
#line 5401 "gram.c" /* yacc.c:1646  */
    break;

  case 410:
#line 1293 "gram.y" /* yacc.c:1646  */
    { rndtoeven = 1; FullUpdate++; }
#line 5407 "gram.c" /* yacc.c:1646  */
    break;

  case 411:
#line 1294 "gram.y" /* yacc.c:1646  */
    { rndtoeven = 0; FullUpdate++; }
#line 5413 "gram.c" /* yacc.c:1646  */
    break;

  case 412:
#line 1295 "gram.y" /* yacc.c:1646  */
    { rndtoeven = 0; FullUpdate++; }
#line 5419 "gram.c" /* yacc.c:1646  */
    break;

  case 413:
#line 1296 "gram.y" /* yacc.c:1646  */
    { craction = (yyvsp[0].ival); }
#line 5425 "gram.c" /* yacc.c:1646  */
    break;

  case 414:
#line 1297 "gram.y" /* yacc.c:1646  */
    { rowlimit = (yyvsp[0].ival); }
#line 5431 "gram.c" /* yacc.c:1646  */
    break;

  case 415:
#line 1298 "gram.y" /* yacc.c:1646  */
    { collimit = (yyvsp[0].ival); }
#line 5437 "gram.c" /* yacc.c:1646  */
    break;

  case 416:
#line 1299 "gram.y" /* yacc.c:1646  */
    { pagesize = (yyvsp[0].ival); }
#line 5443 "gram.c" /* yacc.c:1646  */
    break;

  case 417:
#line 1300 "gram.y" /* yacc.c:1646  */
    { scrc++; }
#line 5449 "gram.c" /* yacc.c:1646  */
    break;

  case 418:
#line 1301 "gram.y" /* yacc.c:1646  */
    {
					  struct  lconv *locstruct;
					  char    *loc;

					  loc = setlocale(LC_ALL, "");
					  if (loc != NULL) {
					    locstruct = localeconv();
					    dpoint = (locstruct->decimal_point)[0];
					    thsep = (locstruct->thousands_sep)[0];
					  }
					  else {
					    dpoint = '.';
					    thsep = ',';
					  }
					  FullUpdate++;
					}
#line 5470 "gram.c" /* yacc.c:1646  */
    break;

  case 419:
#line 1317 "gram.y" /* yacc.c:1646  */
    {
					  dpoint = '.';
					  thsep = ',';
					  FullUpdate++;
					}
#line 5480 "gram.c" /* yacc.c:1646  */
    break;

  case 420:
#line 1322 "gram.y" /* yacc.c:1646  */
    {
					  dpoint = '.';
					  thsep = ',';
					  FullUpdate++;
					}
#line 5490 "gram.c" /* yacc.c:1646  */
    break;

  case 421:
#line 1330 "gram.y" /* yacc.c:1646  */
    { num_search((double)0,
					  (yyvsp[0].rval).left.vp->row, (yyvsp[0].rval).left.vp->col,
					  (yyvsp[0].rval).right.vp->row, (yyvsp[0].rval).right.vp->col,
					  CELLERROR); }
#line 5499 "gram.c" /* yacc.c:1646  */
    break;

  case 422:
#line 1334 "gram.y" /* yacc.c:1646  */
    { num_search((double)0, 0, 0,
					  maxrow, maxcol, CELLERROR); }
#line 5506 "gram.c" /* yacc.c:1646  */
    break;

  case 423:
#line 1336 "gram.y" /* yacc.c:1646  */
    { num_search((double)0,
					  (yyvsp[0].rval).left.vp->row, (yyvsp[0].rval).left.vp->col,
					  (yyvsp[0].rval).right.vp->row, (yyvsp[0].rval).right.vp->col,
					  CELLINVALID); }
#line 5515 "gram.c" /* yacc.c:1646  */
    break;

  case 424:
#line 1340 "gram.y" /* yacc.c:1646  */
    { num_search((double)0, 0, 0,
					  maxrow, maxcol, CELLINVALID); }
#line 5522 "gram.c" /* yacc.c:1646  */
    break;


#line 5526 "gram.c" /* yacc.c:1646  */
      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
# define YYSYNTAX_ERROR yysyntax_error (&yymsg_alloc, &yymsg, \
                                        yyssp, yytoken)
      {
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = YYSYNTAX_ERROR;
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == 1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = (char *) YYSTACK_ALLOC (yymsg_alloc);
            if (!yymsg)
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = 2;
              }
            else
              {
                yysyntax_error_status = YYSYNTAX_ERROR;
                yymsgp = yymsg;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == 2)
          goto yyexhaustedlab;
      }
# undef YYSYNTAX_ERROR
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYTERROR;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined yyoverflow || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  return yyresult;
}
