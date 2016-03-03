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
#line 1 "a.y" /* yacc.c:339  */

#include "a.h"

#line 70 "a.tab.c" /* yacc.c:339  */

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
   by #include "a.tab.h".  */
#ifndef YY_YY_A_TAB_H_INCLUDED
# define YY_YY_A_TAB_H_INCLUDED
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
    LMOVW = 258,
    LMOVB = 259,
    LABS = 260,
    LLOGW = 261,
    LSHW = 262,
    LADDW = 263,
    LCMP = 264,
    LCROP = 265,
    LBRA = 266,
    LFMOV = 267,
    LFCONV = 268,
    LFCMP = 269,
    LFADD = 270,
    LFMA = 271,
    LTRAP = 272,
    LXORW = 273,
    LNOP = 274,
    LEND = 275,
    LRETT = 276,
    LWORD = 277,
    LTEXT = 278,
    LDATA = 279,
    LRETRN = 280,
    LCONST = 281,
    LSP = 282,
    LSB = 283,
    LFP = 284,
    LPC = 285,
    LCREG = 286,
    LFLUSH = 287,
    LREG = 288,
    LFREG = 289,
    LR = 290,
    LCR = 291,
    LF = 292,
    LFPSCR = 293,
    LLR = 294,
    LCTR = 295,
    LSPR = 296,
    LSPREG = 297,
    LSEG = 298,
    LMSR = 299,
    LDCR = 300,
    LSCHED = 301,
    LXLD = 302,
    LXST = 303,
    LXOP = 304,
    LXMV = 305,
    LRLWM = 306,
    LMOVMW = 307,
    LMOVEM = 308,
    LMOVFL = 309,
    LMTFSB = 310,
    LMA = 311,
    LFMOVX = 312,
    LFCONST = 313,
    LSCONST = 314,
    LNAME = 315,
    LLAB = 316,
    LVAR = 317
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 5 "a.y" /* yacc.c:355  */

	Sym	*sym;
	long	lval;
	double	dval;
	char	sval[8];
	Gen	gen;

#line 181 "a.tab.c" /* yacc.c:355  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_A_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 196 "a.tab.c" /* yacc.c:358  */

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
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   874

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  81
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  33
/* YYNRULES -- Number of rules.  */
#define YYNRULES  192
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  475

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   317

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,    79,    12,     5,     2,
      77,    78,    10,     8,    76,     9,     2,    11,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    73,    75,
       6,    74,     7,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     4,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     3,     2,    80,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     1,     2,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    33,    33,    34,    38,    37,    45,    44,    50,    55,
      61,    65,    66,    67,    73,    77,    81,    85,    89,    93,
     100,   104,   108,   112,   116,   120,   127,   131,   138,   142,
     146,   150,   157,   161,   168,   172,   176,   180,   184,   188,
     195,   199,   203,   213,   217,   221,   225,   229,   233,   237,
     241,   245,   249,   253,   257,   261,   268,   275,   279,   286,
     290,   298,   302,   306,   310,   314,   321,   328,   332,   336,
     340,   349,   353,   357,   361,   365,   369,   373,   377,   381,
     385,   389,   393,   397,   405,   413,   424,   428,   432,   436,
     443,   447,   451,   455,   459,   463,   470,   474,   478,   482,
     489,   493,   497,   501,   508,   512,   520,   524,   528,   532,
     536,   540,   544,   551,   555,   559,   563,   567,   574,   578,
     585,   592,   596,   600,   604,   611,   615,   619,   626,   632,
     638,   647,   656,   664,   665,   668,   676,   684,   692,   699,
     705,   711,   717,   724,   727,   736,   745,   753,   761,   767,
     775,   781,   789,   797,   818,   823,   831,   837,   844,   852,
     853,   861,   868,   878,   879,   888,   896,   904,   913,   914,
     917,   920,   924,   930,   931,   932,   935,   936,   940,   944,
     948,   952,   958,   959,   963,   967,   971,   975,   979,   983,
     987,   991,   995
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "'|'", "'^'", "'&'", "'<'", "'>'", "'+'",
  "'-'", "'*'", "'/'", "'%'", "LMOVW", "LMOVB", "LABS", "LLOGW", "LSHW",
  "LADDW", "LCMP", "LCROP", "LBRA", "LFMOV", "LFCONV", "LFCMP", "LFADD",
  "LFMA", "LTRAP", "LXORW", "LNOP", "LEND", "LRETT", "LWORD", "LTEXT",
  "LDATA", "LRETRN", "LCONST", "LSP", "LSB", "LFP", "LPC", "LCREG",
  "LFLUSH", "LREG", "LFREG", "LR", "LCR", "LF", "LFPSCR", "LLR", "LCTR",
  "LSPR", "LSPREG", "LSEG", "LMSR", "LDCR", "LSCHED", "LXLD", "LXST",
  "LXOP", "LXMV", "LRLWM", "LMOVMW", "LMOVEM", "LMOVFL", "LMTFSB", "LMA",
  "LFMOVX", "LFCONST", "LSCONST", "LNAME", "LLAB", "LVAR", "':'", "'='",
  "';'", "','", "'('", "')'", "'$'", "'~'", "$accept", "prog", "line",
  "$@1", "$@2", "inst", "rel", "rreg", "xlreg", "lr", "lcr", "ctr", "msr",
  "psr", "seg", "fpscr", "fpscrf", "freg", "creg", "cbit", "mask", "ximm",
  "fimm", "imm", "sreg", "regaddr", "addr", "name", "comma", "offset",
  "pointer", "con", "expr", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   124,    94,    38,    60,    62,    43,    45,
      42,    47,    37,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,    58,    61,    59,    44,    40,    41,    36,
     126
};
# endif

#define YYPACT_NINF -183

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-183)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -183,   515,  -183,   -63,   621,   695,    17,    17,   -24,   -24,
      17,   794,   242,   273,    33,    33,    33,    33,   -13,   153,
     -49,   -40,   769,   769,   -49,   -25,    10,    17,    10,    -9,
     -24,   699,    18,    17,    42,   -28,    27,    38,  -183,  -183,
      52,  -183,   794,   794,  -183,  -183,  -183,  -183,    53,    57,
      61,  -183,  -183,  -183,    72,  -183,    74,  -183,    76,    45,
    -183,   348,   418,   794,    41,    79,  -183,    99,  -183,  -183,
     105,   117,   123,   133,   136,   138,   142,  -183,   145,   154,
    -183,    81,   155,   163,   164,   167,   168,   794,   180,   181,
     183,   188,   192,   794,   194,  -183,    57,    45,    67,   718,
      34,  -183,   199,  -183,    44,     6,   203,   204,   211,   215,
     234,   239,   240,   245,  -183,   256,   263,  -183,   158,   -49,
     -49,  -183,  -183,   -49,   -49,   264,   195,   274,  -183,  -183,
      17,   266,   275,  -183,   278,   286,   287,   298,   302,   303,
     306,   316,   322,   324,  -183,   794,  -183,   794,  -183,  -183,
    -183,   794,   794,   794,   794,   348,   348,   330,   794,   794,
     272,     3,  -183,   361,  -183,  -183,    81,  -183,   669,    17,
      17,   282,    17,   300,   432,   467,    17,   -10,    17,    17,
     338,   695,    17,    17,    17,    17,  -183,    17,    17,   -24,
      17,   -24,   794,   272,  -183,    34,  -183,  -183,   281,   144,
     747,   751,   209,   333,  -183,   681,    33,    33,    33,    33,
      33,    33,    33,    17,  -183,    17,  -183,  -183,  -183,  -183,
    -183,  -183,   776,   196,   794,   -24,    -8,    10,    17,    17,
      17,   769,    17,   794,    17,    10,    33,   578,   407,   578,
     548,   325,   326,   328,   329,   343,   344,   346,   352,    67,
    -183,  -183,   196,    17,  -183,   794,   794,   794,   396,   401,
     794,   794,   794,   794,   794,  -183,  -183,  -183,  -183,  -183,
    -183,  -183,   355,  -183,  -183,  -183,  -183,  -183,  -183,  -183,
    -183,  -183,  -183,   356,  -183,  -183,  -183,  -183,  -183,   366,
    -183,  -183,  -183,  -183,  -183,  -183,  -183,   367,   368,  -183,
    -183,  -183,  -183,  -183,  -183,  -183,   357,  -183,   371,  -183,
     374,  -183,   375,   376,  -183,   377,   379,   382,  -183,   387,
     386,  -183,    34,  -183,  -183,    34,  -183,  -183,    66,   389,
    -183,  -183,  -183,  -183,  -183,  -183,  -183,  -183,   393,   394,
     398,  -183,  -183,   392,   399,   402,  -183,   405,   408,  -183,
    -183,  -183,   409,   410,  -183,  -183,  -183,   413,  -183,  -183,
    -183,  -183,  -183,  -183,  -183,  -183,  -183,  -183,  -183,  -183,
    -183,  -183,   406,   419,   422,   557,   609,   673,   794,   794,
     385,   385,  -183,  -183,  -183,   445,   453,   794,  -183,  -183,
      17,    17,    17,    17,    17,    17,   -23,   -23,   794,  -183,
     425,   427,   765,  -183,   -23,    33,    33,    18,    18,   428,
      17,    10,   776,   776,    17,   468,  -183,  -183,   295,   295,
    -183,  -183,  -183,   439,  -183,  -183,  -183,  -183,  -183,  -183,
    -183,  -183,  -183,  -183,  -183,    34,  -183,  -183,  -183,  -183,
     434,  -183,   447,    56,  -183,  -183,  -183,  -183,  -183,   448,
     449,   450,   451,   494,  -183,   465,  -183,   500,    33,    18,
      82,    17,    17,   794,    17,    17,  -183,  -183,  -183,  -183,
    -183,  -183,  -183,  -183,  -183
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   168,   168,
     168,     0,     0,     0,   168,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    11,     3,
       0,    13,     0,     0,   176,   150,   159,   148,     0,   136,
       0,   146,   135,   137,     0,   139,     0,   138,     0,   170,
     177,     0,     0,     0,     0,     0,   133,     0,   134,   143,
       0,     0,     0,     0,     0,     0,     0,   132,     0,     0,
     163,     0,     0,     0,     0,    55,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   152,     0,   170,   170,     0,
       0,    71,     0,    72,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   169,   168,     0,    89,   169,   168,
     168,   113,   120,   168,   168,     0,     0,     0,   128,    10,
       0,     0,     0,   112,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     6,     0,     4,     0,    12,   179,
     178,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   182,     0,   155,   154,   158,   180,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   158,     0,     0,     0,
       0,     0,     0,   130,   131,     0,    74,    75,     0,     0,
       0,     0,     0,     0,   156,     0,     0,     0,     0,     0,
       0,     0,     0,   169,    88,     0,   116,   117,   114,   115,
     119,   118,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   170,
     171,   172,     0,     0,   161,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   181,    14,    68,    42,    70,
      66,    41,     0,    29,    28,    67,    63,    64,    62,    65,
      34,    37,    35,     0,    33,    32,    69,    61,    58,     0,
      57,    38,    16,    15,   174,   173,   175,     0,     0,    17,
      31,    30,    19,    18,    54,    49,   132,    51,   132,    53,
     132,    46,     0,   132,    47,   132,    96,    97,    59,   152,
       0,    73,     0,    77,    78,     0,    80,    81,     0,     0,
     157,    23,    25,    24,    22,    21,    20,    90,    94,    91,
       0,    86,    87,   121,     0,     0,   106,     0,     0,   108,
     111,   110,     0,     0,   105,   104,    39,     0,    27,    26,
       7,     8,     5,     9,   160,   151,   149,   140,   145,   144,
     142,   141,     0,     0,     0,   192,   191,   190,     0,     0,
     183,   184,   185,   186,   187,     0,     0,     0,   164,   165,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    76,
       0,     0,     0,   129,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   166,   162,   188,   189,
     136,    40,    36,     0,    48,    50,    52,    45,    43,    44,
      98,    99,    60,    79,    82,     0,    83,    84,    95,    92,
       0,   123,   122,     0,   126,   127,   125,   107,   109,     0,
       0,     0,     0,     0,    56,     0,   147,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   167,    85,    93,   124,
     103,   102,   153,   101,   100
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -183,  -183,  -144,  -183,  -183,  -183,   -91,    -5,   -80,  -183,
    -159,  -183,  -183,  -131,   415,  -167,  -183,    88,    40,  -182,
     171,   -15,   179,    64,   146,   103,   261,   147,    37,   -81,
     337,    36,  -116
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    39,   239,   237,    40,   101,    64,    65,    66,
      67,    68,    69,    70,    71,    72,   291,    73,    74,    94,
     449,    75,   107,    76,    77,    78,   165,    80,   117,   160,
     298,   162,   163
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint16 yytable[] =
{
      82,    85,    86,    88,    90,    92,   123,   282,   196,   268,
     318,   253,    41,   115,   119,   203,   193,   194,    45,    46,
     199,    48,   132,    96,   134,   136,   138,   114,   141,   238,
      46,   240,    48,    46,    46,    48,    48,   269,   289,    62,
      81,    81,    42,    43,   286,   144,   145,    95,   104,    81,
     129,   157,   102,   158,   159,    87,   121,   122,   126,   126,
      46,   128,    48,   114,    42,   460,    87,    81,   130,   130,
      44,    87,    89,    91,   204,   158,   159,    47,   149,   150,
      50,   254,   116,    52,    53,   124,    47,   130,   267,    50,
      42,    43,    44,   360,   137,   362,   140,    87,   166,   167,
     146,   106,   110,   111,   112,   113,    60,   120,    83,   323,
     326,    93,   147,   216,    63,   320,   108,   168,    44,   130,
     201,   202,   142,   186,   204,   164,    59,   148,    60,   131,
     151,   133,   135,    93,   152,   198,    63,   143,   153,   375,
     376,   377,   402,   202,   380,   381,   382,   383,   384,   154,
     330,   155,   214,   156,    60,   169,   218,   219,   180,    93,
     220,   221,    63,   266,   275,   276,   277,   279,   372,   125,
     127,   288,   290,   292,   293,   170,   299,   302,   303,   304,
     305,   171,   307,   309,   311,   314,   316,   241,   242,   243,
     244,   246,   248,   172,   250,   251,    46,    47,    48,   173,
      50,    46,    47,    48,    81,    50,   217,   161,   271,   174,
      81,   278,   175,   281,   176,   287,   432,    81,   177,   422,
     346,   178,   321,   351,   352,   353,   421,   355,   319,   118,
     179,   181,   272,   294,   295,   296,   198,   328,   283,   182,
     183,    81,   400,   184,   185,   401,   294,   295,   296,   329,
      42,    43,    46,   312,    48,   317,   187,   188,   344,   189,
     345,   280,   418,   419,   190,    79,    84,    81,   191,   356,
     192,   273,   223,   103,   109,   200,   161,   284,    44,   205,
     206,    42,    43,    45,   300,   224,   343,   207,    96,   347,
     348,   208,   139,   331,   334,   335,   336,   337,   338,   339,
     340,   245,   247,   260,   261,   262,   263,   264,   332,    44,
     209,   436,    97,    98,    60,   210,   211,    47,    99,   100,
      50,   212,    63,    45,   359,    46,   297,    48,    96,   349,
     350,   306,   213,   308,   310,   313,   315,   249,   358,   215,
     222,    45,   225,    59,    47,    60,    96,    50,   297,   252,
      61,   226,   105,    63,   227,   457,    42,    43,   202,   341,
     197,   342,   228,   229,   255,   256,   257,   258,   259,   260,
     261,   262,   263,   264,   230,   294,   295,   296,   231,   232,
     357,    46,   233,    48,    44,   424,   425,   426,   427,   428,
     429,    46,   234,    48,   444,   262,   263,   264,   235,   374,
     236,   330,   378,   364,   365,   447,   366,   367,   379,   454,
     255,   256,   257,   258,   259,   260,   261,   262,   263,   264,
      60,   368,   369,   423,   370,    93,    42,    43,    63,   274,
     371,   385,   386,   390,    95,   285,   430,   431,   198,   265,
      42,    43,   301,   387,   438,   388,   389,   391,   451,   451,
     392,   393,   394,   395,    44,   396,   470,   471,   397,   473,
     474,   324,   327,   398,   399,   407,   333,   403,    44,   404,
     405,   441,   442,   446,   406,   408,   450,   453,   409,   166,
      51,   410,   361,   415,   411,   412,   413,   164,    59,   414,
      60,   420,   354,   439,   440,    93,   150,   416,    63,   472,
     417,    51,    59,   433,    60,   434,   455,   443,    45,    61,
     458,    87,    63,    96,   448,     2,     3,   456,    54,    55,
     459,    57,    58,   469,   461,   462,   463,   464,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,   466,    19,    20,   468,    21,    22,    23,
      24,   255,   256,   257,   258,   259,   260,   261,   262,   263,
     264,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     465,    25,    26,    27,    28,    29,    30,    31,   467,     3,
      32,    33,    34,   270,   452,    35,    36,    37,   445,   373,
      38,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,     0,    19,    20,     0,
      21,    22,    23,    24,   257,   258,   259,   260,   261,   262,
     263,   264,     0,   363,     0,     0,     0,     0,     0,    42,
      43,     0,     0,     0,    25,    26,    27,    28,    29,    30,
      31,     0,     0,    32,    33,    34,     0,     0,    35,    36,
      37,     0,     0,    38,     0,     0,     0,    44,     0,     0,
       0,     0,    45,   437,    46,    47,    48,    49,    50,    51,
      52,    53,    54,    55,    56,    57,    58,    42,    43,   258,
     259,   260,   261,   262,   263,   264,     0,     0,     0,    42,
      43,    59,     0,    60,     0,     0,     0,     0,    61,     0,
      62,    63,     0,    42,    43,    44,     0,    42,    43,     0,
      45,     0,    46,     0,    48,    49,     0,    44,    52,    53,
      54,    55,    56,    57,    58,    47,    42,    43,    50,     0,
       0,    44,     0,     0,     0,    44,     0,     0,    46,    59,
      48,    60,    46,     0,    48,     0,    61,     0,    87,    63,
       0,    59,     0,    60,    44,    42,    43,     0,    61,    42,
      43,    63,     0,     0,     0,    59,     0,    60,     0,    59,
       0,    60,    61,    42,    43,    63,    93,    42,    43,    63,
       0,     0,     0,    44,    42,    43,     0,    44,    97,    98,
      60,     0,     0,     0,     0,   195,     0,     0,    63,     0,
       0,    44,    42,    43,     0,    44,     0,     0,     0,     0,
       0,     0,    44,     0,     0,     0,     0,    97,    98,    60,
       0,    97,    98,    60,   322,     0,     0,    63,   325,     0,
      44,    63,     0,     0,     0,    97,    98,    60,     0,    59,
       0,    60,   435,     0,     0,    63,    93,     0,    60,    63,
       0,     0,     0,    93,     0,    87,    63,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    60,     0,     0,     0,
       0,    93,     0,     0,    63
};

static const yytype_int16 yycheck[] =
{
       5,     6,     7,     8,     9,    10,    21,   174,    99,   168,
     192,     8,    75,    18,    19,     9,    97,    98,    41,    43,
     100,    45,    27,    46,    29,    30,    31,    76,    33,   145,
      43,   147,    45,    43,    43,    45,    45,   168,    48,    79,
       4,     5,     8,     9,   175,    73,    74,    11,    12,    13,
      75,     6,    12,     8,     9,    79,    19,    20,    22,    23,
      43,    24,    45,    76,     8,     9,    79,    31,    77,    77,
      36,    79,     8,     9,    68,     8,     9,    44,    42,    43,
      47,    78,    18,    49,    50,    21,    44,    77,   168,    47,
       8,     9,    36,   237,    30,   239,    32,    79,    62,    63,
      73,    13,    14,    15,    16,    17,    72,    19,     5,   200,
     201,    77,    74,   118,    80,   195,    13,    76,    36,    77,
      76,    77,    34,    87,    68,    69,    70,    75,    72,    26,
      77,    28,    29,    77,    77,    99,    80,    34,    77,   255,
     256,   257,    76,    77,   260,   261,   262,   263,   264,    77,
      68,    77,   115,    77,    72,    76,   119,   120,    77,    77,
     123,   124,    80,   168,   169,   170,   171,   172,   249,    22,
      23,   176,   177,   178,   179,    76,   181,   182,   183,   184,
     185,    76,   187,   188,   189,   190,   191,   151,   152,   153,
     154,   155,   156,    76,   158,   159,    43,    44,    45,    76,
      47,    43,    44,    45,   168,    47,   118,    61,   168,    76,
     174,   171,    76,   173,    76,   175,   398,   181,    76,   386,
     225,    76,    78,   228,   229,   230,   385,   232,   192,    76,
      76,    76,   168,    37,    38,    39,   200,   201,   174,    76,
      76,   205,   322,    76,    76,   325,    37,    38,    39,    40,
       8,     9,    43,   189,    45,   191,    76,    76,   222,    76,
     224,   173,   378,   379,    76,     4,     5,   231,    76,   233,
      76,   168,    77,    12,    13,    76,   130,   174,    36,    76,
      76,     8,     9,    41,   181,    11,   222,    76,    46,   225,
     226,    76,    31,   205,   206,   207,   208,   209,   210,   211,
     212,   155,   156,     8,     9,    10,    11,    12,   205,    36,
      76,   402,    70,    71,    72,    76,    76,    44,    76,    77,
      47,    76,    80,    41,   236,    43,   180,    45,    46,   226,
     227,   185,    76,   187,   188,   189,   190,     7,   235,    76,
      76,    41,    76,    70,    44,    72,    46,    47,   202,    77,
      77,    76,    79,    80,    76,   435,     8,     9,    77,   213,
      99,   215,    76,    76,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    76,    37,    38,    39,    76,    76,
     234,    43,    76,    45,    36,   390,   391,   392,   393,   394,
     395,    43,    76,    45,   409,    10,    11,    12,    76,   253,
      76,    68,     6,    78,    78,   410,    78,    78,     7,   414,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      72,    78,    78,   387,    78,    77,     8,     9,    80,   168,
      78,    76,    76,    76,   398,   174,   396,   397,   402,    78,
       8,     9,   181,    77,   404,    78,    78,    76,   412,   413,
      76,    76,    76,    76,    36,    76,   461,   462,    76,   464,
     465,   200,   201,    76,    78,    73,   205,    78,    36,    76,
      76,   407,   408,   409,    76,    76,   412,   413,    76,   443,
      48,    76,    75,    77,    76,    76,    76,    69,    70,    76,
      72,    46,   231,   405,   406,    77,   460,    78,    80,   463,
      78,    48,    70,    78,    72,    78,    38,    79,    41,    77,
      76,    79,    80,    46,   411,     0,     1,    78,    51,    52,
      73,    54,    55,   459,    76,    76,    76,    76,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    78,    29,    30,   458,    32,    33,    34,
      35,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      76,    56,    57,    58,    59,    60,    61,    62,    78,     1,
      65,    66,    67,   168,   413,    70,    71,    72,   409,   252,
      75,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    -1,    29,    30,    -1,
      32,    33,    34,    35,     5,     6,     7,     8,     9,    10,
      11,    12,    -1,    75,    -1,    -1,    -1,    -1,    -1,     8,
       9,    -1,    -1,    -1,    56,    57,    58,    59,    60,    61,
      62,    -1,    -1,    65,    66,    67,    -1,    -1,    70,    71,
      72,    -1,    -1,    75,    -1,    -1,    -1,    36,    -1,    -1,
      -1,    -1,    41,   402,    43,    44,    45,    46,    47,    48,
      49,    50,    51,    52,    53,    54,    55,     8,     9,     6,
       7,     8,     9,    10,    11,    12,    -1,    -1,    -1,     8,
       9,    70,    -1,    72,    -1,    -1,    -1,    -1,    77,    -1,
      79,    80,    -1,     8,     9,    36,    -1,     8,     9,    -1,
      41,    -1,    43,    -1,    45,    46,    -1,    36,    49,    50,
      51,    52,    53,    54,    55,    44,     8,     9,    47,    -1,
      -1,    36,    -1,    -1,    -1,    36,    -1,    -1,    43,    70,
      45,    72,    43,    -1,    45,    -1,    77,    -1,    79,    80,
      -1,    70,    -1,    72,    36,     8,     9,    -1,    77,     8,
       9,    80,    -1,    -1,    -1,    70,    -1,    72,    -1,    70,
      -1,    72,    77,     8,     9,    80,    77,     8,     9,    80,
      -1,    -1,    -1,    36,     8,     9,    -1,    36,    70,    71,
      72,    -1,    -1,    -1,    -1,    77,    -1,    -1,    80,    -1,
      -1,    36,     8,     9,    -1,    36,    -1,    -1,    -1,    -1,
      -1,    -1,    36,    -1,    -1,    -1,    -1,    70,    71,    72,
      -1,    70,    71,    72,    77,    -1,    -1,    80,    77,    -1,
      36,    80,    -1,    -1,    -1,    70,    71,    72,    -1,    70,
      -1,    72,    77,    -1,    -1,    80,    77,    -1,    72,    80,
      -1,    -1,    -1,    77,    -1,    79,    80,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    72,    -1,    -1,    -1,
      -1,    77,    -1,    -1,    80
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    82,     0,     1,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    29,
      30,    32,    33,    34,    35,    56,    57,    58,    59,    60,
      61,    62,    65,    66,    67,    70,    71,    72,    75,    83,
      86,    75,     8,     9,    36,    41,    43,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    54,    55,    70,
      72,    77,    79,    80,    88,    89,    90,    91,    92,    93,
      94,    95,    96,    98,    99,   102,   104,   105,   106,   107,
     108,   112,    88,   106,   107,    88,    88,    79,    88,   104,
      88,   104,    88,    77,   100,   112,    46,    70,    71,    76,
      77,    87,    99,   107,   112,    79,    98,   103,   106,   107,
      98,    98,    98,    98,    76,    88,   104,   109,    76,    88,
      98,   109,   109,   102,   104,   108,   112,   108,   109,    75,
      77,   106,    88,   106,    88,   106,    88,   104,    88,   107,
     104,    88,    98,   106,    73,    74,    73,    74,    75,   112,
     112,    77,    77,    77,    77,    77,    77,     6,     8,     9,
     110,   105,   112,   113,    69,   107,   112,   112,    76,    76,
      76,    76,    76,    76,    76,    76,    76,    76,    76,    76,
      77,    76,    76,    76,    76,    76,   112,    76,    76,    76,
      76,    76,    76,   110,   110,    77,    87,   107,   112,    89,
      76,    76,    77,     9,    68,    76,    76,    76,    76,    76,
      76,    76,    76,    76,   109,    76,    88,    98,   109,   109,
     109,   109,    76,    77,    11,    76,    76,    76,    76,    76,
      76,    76,    76,    76,    76,    76,    76,    85,   113,    84,
     113,   112,   112,   112,   112,   105,   112,   105,   112,     7,
     112,   112,    77,     8,    78,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    78,    88,    89,    91,    94,
      95,    99,   104,   106,   107,    88,    88,    88,    99,    88,
      98,    99,    96,   104,   106,   107,    94,    99,    88,    48,
      88,    97,    88,    88,    37,    38,    39,   105,   111,    88,
     106,   107,    88,    88,    88,    88,   105,    88,   105,    88,
     105,    88,   104,   105,    88,   105,    88,   104,   100,   112,
      89,    78,    77,    87,   107,    77,    87,   107,   112,    40,
      68,    98,   106,   107,    98,    98,    98,    98,    98,    98,
      98,   105,   105,   104,   112,   112,    88,   104,   104,   106,
     106,    88,    88,    88,   107,    88,   112,   105,   106,    98,
      83,    75,    83,    75,    78,    78,    78,    78,    78,    78,
      78,    78,   110,   111,   105,   113,   113,   113,     6,     7,
     113,   113,   113,   113,   113,    76,    76,    77,    78,    78,
      76,    76,    76,    76,    76,    76,    76,    76,    76,    78,
      89,    89,    76,    78,    76,    76,    76,    73,    76,    76,
      76,    76,    76,    76,    76,    77,    78,    78,   113,   113,
      46,    91,    96,   112,    88,    88,    88,    88,    88,    88,
      99,    99,   100,    78,    78,    77,    87,   107,    99,    98,
      98,   104,   104,    79,   102,   103,   104,    88,   106,   101,
     104,   112,   101,   104,    88,    38,    78,    89,    76,    73,
       9,    76,    76,    76,    76,    76,    78,    78,    98,   104,
      88,    88,   112,    88,    88
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    81,    82,    82,    84,    83,    85,    83,    83,    83,
      83,    83,    83,    83,    86,    86,    86,    86,    86,    86,
      86,    86,    86,    86,    86,    86,    86,    86,    86,    86,
      86,    86,    86,    86,    86,    86,    86,    86,    86,    86,
      86,    86,    86,    86,    86,    86,    86,    86,    86,    86,
      86,    86,    86,    86,    86,    86,    86,    86,    86,    86,
      86,    86,    86,    86,    86,    86,    86,    86,    86,    86,
      86,    86,    86,    86,    86,    86,    86,    86,    86,    86,
      86,    86,    86,    86,    86,    86,    86,    86,    86,    86,
      86,    86,    86,    86,    86,    86,    86,    86,    86,    86,
      86,    86,    86,    86,    86,    86,    86,    86,    86,    86,
      86,    86,    86,    86,    86,    86,    86,    86,    86,    86,
      86,    86,    86,    86,    86,    86,    86,    86,    86,    87,
      87,    87,    88,    89,    89,    90,    91,    92,    93,    94,
      94,    94,    94,    94,    95,    95,    96,    97,    98,    98,
      99,    99,   100,   101,   102,   102,   103,   103,   104,   105,
     105,   106,   106,   107,   107,   108,   108,   108,   109,   109,
     110,   110,   110,   111,   111,   111,   112,   112,   112,   112,
     112,   112,   113,   113,   113,   113,   113,   113,   113,   113,
     113,   113,   113
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     0,     4,     0,     4,     4,     4,
       2,     1,     2,     2,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     6,     4,     4,     4,
       6,     4,     4,     6,     6,     6,     4,     4,     6,     4,
       6,     4,     6,     4,     4,     2,     6,     4,     4,     4,
       6,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     2,     2,     4,     3,     3,     5,     4,     4,     6,
       4,     4,     6,     6,     6,     8,     4,     4,     3,     2,
       4,     4,     6,     8,     4,     6,     4,     4,     6,     6,
       8,     8,     8,     8,     4,     4,     4,     6,     4,     6,
       4,     4,     2,     2,     3,     3,     3,     3,     3,     3,
       2,     4,     6,     6,     8,     6,     6,     6,     2,     4,
       2,     2,     1,     1,     1,     1,     1,     1,     1,     1,
       4,     4,     4,     1,     4,     4,     1,     4,     1,     4,
       1,     4,     1,     3,     2,     2,     2,     3,     2,     1,
       4,     3,     5,     1,     4,     4,     5,     7,     0,     1,
       0,     2,     2,     1,     1,     1,     1,     1,     2,     2,
       2,     3,     1,     3,     3,     3,     3,     3,     4,     4,
       3,     3,     3
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
        case 4:
#line 38 "a.y" /* yacc.c:1646  */
    {
		if((yyvsp[-1].sym)->value != pc)
			yyerror("redeclaration of %s", (yyvsp[-1].sym)->name);
		(yyvsp[-1].sym)->value = pc;
	}
#line 1669 "a.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 45 "a.y" /* yacc.c:1646  */
    {
		(yyvsp[-1].sym)->type = LLAB;
		(yyvsp[-1].sym)->value = pc;
	}
#line 1678 "a.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 51 "a.y" /* yacc.c:1646  */
    {
		(yyvsp[-3].sym)->type = LVAR;
		(yyvsp[-3].sym)->value = (yyvsp[-1].lval);
	}
#line 1687 "a.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 56 "a.y" /* yacc.c:1646  */
    {
		if((yyvsp[-3].sym)->value != (yyvsp[-1].lval))
			yyerror("redeclaration of %s", (yyvsp[-3].sym)->name);
		(yyvsp[-3].sym)->value = (yyvsp[-1].lval);
	}
#line 1697 "a.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 62 "a.y" /* yacc.c:1646  */
    {
		nosched = (yyvsp[-1].lval);
	}
#line 1705 "a.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 74 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1713 "a.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 78 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1721 "a.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 82 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1729 "a.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 86 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1737 "a.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 90 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1745 "a.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 94 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1753 "a.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 101 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1761 "a.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 105 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1769 "a.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 109 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1777 "a.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 113 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1785 "a.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 117 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1793 "a.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 121 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1801 "a.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 128 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1809 "a.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 132 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1817 "a.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 139 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1825 "a.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 143 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1833 "a.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 147 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1841 "a.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 151 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1849 "a.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 158 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1857 "a.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 162 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1865 "a.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 169 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1873 "a.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 173 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1881 "a.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 177 "a.y" /* yacc.c:1646  */
    {
		outgcode((yyvsp[-5].lval), &(yyvsp[-4].gen), NREG, &(yyvsp[-2].gen), &(yyvsp[0].gen));
	}
#line 1889 "a.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 181 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1897 "a.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 185 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1905 "a.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 189 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), (yyvsp[0].lval), &nullgen);
	}
#line 1913 "a.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 196 "a.y" /* yacc.c:1646  */
    {
		outgcode((yyvsp[-5].lval), &(yyvsp[-4].gen), NREG, &(yyvsp[-2].gen), &(yyvsp[0].gen));
	}
#line 1921 "a.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 200 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1929 "a.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 204 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1937 "a.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 214 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-5].lval), &(yyvsp[-4].gen), (yyvsp[-2].lval), &(yyvsp[0].gen));
	}
#line 1945 "a.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 218 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-5].lval), &(yyvsp[-4].gen), (yyvsp[-2].lval), &(yyvsp[0].gen));
	}
#line 1953 "a.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 222 "a.y" /* yacc.c:1646  */
    {
		outgcode((yyvsp[-5].lval), &(yyvsp[-4].gen), NREG, &(yyvsp[-2].gen), &(yyvsp[0].gen));
	}
#line 1961 "a.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 226 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1969 "a.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 230 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1977 "a.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 234 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-5].lval), &(yyvsp[-4].gen), (yyvsp[-2].lval), &(yyvsp[0].gen));
	}
#line 1985 "a.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 238 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1993 "a.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 242 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-5].lval), &(yyvsp[-4].gen), (yyvsp[-2].lval), &(yyvsp[0].gen));
	}
#line 2001 "a.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 246 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 2009 "a.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 250 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-5].lval), &(yyvsp[-4].gen), (yyvsp[-2].lval), &(yyvsp[0].gen));
	}
#line 2017 "a.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 254 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 2025 "a.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 258 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 2033 "a.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 262 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-1].lval), &(yyvsp[0].gen), NREG, &(yyvsp[0].gen));
	}
#line 2041 "a.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 269 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-5].lval), &(yyvsp[-4].gen), (yyvsp[-2].lval), &(yyvsp[0].gen));
	}
#line 2049 "a.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 276 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 2057 "a.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 280 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 2065 "a.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 287 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), (yyvsp[0].gen).reg, &(yyvsp[0].gen));
	}
#line 2073 "a.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 291 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-5].lval), &(yyvsp[-4].gen), (yyvsp[-2].lval), &(yyvsp[0].gen));
	}
#line 2081 "a.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 299 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 2089 "a.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 303 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 2097 "a.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 307 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 2105 "a.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 311 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 2113 "a.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 315 "a.y" /* yacc.c:1646  */
    {
		int r;
		r = (yyvsp[-2].gen).offset;
		(yyvsp[-2].gen).offset = 0;
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), r, &(yyvsp[0].gen));
	}
#line 2124 "a.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 322 "a.y" /* yacc.c:1646  */
    {
		int r;
		r = (yyvsp[0].gen).offset;
		(yyvsp[0].gen).offset = 0;
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), r, &(yyvsp[0].gen));
	}
#line 2135 "a.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 329 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 2143 "a.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 333 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 2151 "a.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 337 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 2159 "a.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 341 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 2167 "a.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 350 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-1].lval), &nullgen, NREG, &(yyvsp[0].gen));
	}
#line 2175 "a.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 354 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-1].lval), &nullgen, NREG, &(yyvsp[0].gen));
	}
#line 2183 "a.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 358 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &nullgen, NREG, &(yyvsp[-1].gen));
	}
#line 2191 "a.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 362 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-2].lval), &nullgen, NREG, &(yyvsp[0].gen));
	}
#line 2199 "a.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 366 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-2].lval), &nullgen, NREG, &(yyvsp[0].gen));
	}
#line 2207 "a.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 370 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-4].lval), &nullgen, NREG, &(yyvsp[-1].gen));
	}
#line 2215 "a.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 374 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 2223 "a.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 378 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 2231 "a.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 382 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-5].lval), &(yyvsp[-4].gen), NREG, &(yyvsp[-1].gen));
	}
#line 2239 "a.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 386 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &nullgen, (yyvsp[-2].lval), &(yyvsp[0].gen));
	}
#line 2247 "a.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 390 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &nullgen, (yyvsp[-2].lval), &(yyvsp[0].gen));
	}
#line 2255 "a.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 394 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-5].lval), &nullgen, (yyvsp[-4].lval), &(yyvsp[-1].gen));
	}
#line 2263 "a.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 398 "a.y" /* yacc.c:1646  */
    {
		Gen g;
		g = nullgen;
		g.type = D_CONST;
		g.offset = (yyvsp[-4].lval);
		outcode((yyvsp[-5].lval), &g, (yyvsp[-2].lval), &(yyvsp[0].gen));
	}
#line 2275 "a.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 406 "a.y" /* yacc.c:1646  */
    {
		Gen g;
		g = nullgen;
		g.type = D_CONST;
		g.offset = (yyvsp[-4].lval);
		outcode((yyvsp[-5].lval), &g, (yyvsp[-2].lval), &(yyvsp[0].gen));
	}
#line 2287 "a.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 414 "a.y" /* yacc.c:1646  */
    {
		Gen g;
		g = nullgen;
		g.type = D_CONST;
		g.offset = (yyvsp[-6].lval);
		outcode((yyvsp[-7].lval), &g, (yyvsp[-4].lval), &(yyvsp[-1].gen));
	}
#line 2299 "a.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 425 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), (yyvsp[0].lval), &nullgen);
	}
#line 2307 "a.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 429 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), (yyvsp[0].lval), &nullgen);
	}
#line 2315 "a.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 433 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-2].lval), &(yyvsp[-1].gen), NREG, &nullgen);
	}
#line 2323 "a.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 437 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-1].lval), &nullgen, NREG, &nullgen);
	}
#line 2331 "a.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 444 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 2339 "a.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 448 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 2347 "a.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 452 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-5].lval), &(yyvsp[-4].gen), (yyvsp[-2].gen).reg, &(yyvsp[0].gen));
	}
#line 2355 "a.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 456 "a.y" /* yacc.c:1646  */
    {
		outgcode((yyvsp[-7].lval), &(yyvsp[-6].gen), (yyvsp[-4].gen).reg, &(yyvsp[-2].gen), &(yyvsp[0].gen));
	}
#line 2363 "a.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 460 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 2371 "a.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 464 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-5].lval), &(yyvsp[-4].gen), (yyvsp[0].gen).reg, &(yyvsp[-2].gen));
	}
#line 2379 "a.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 471 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 2387 "a.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 475 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 2395 "a.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 479 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-5].lval), &(yyvsp[-4].gen), (yyvsp[0].gen).reg, &(yyvsp[-2].gen));
	}
#line 2403 "a.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 483 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-5].lval), &(yyvsp[-4].gen), (yyvsp[0].gen).reg, &(yyvsp[-2].gen));
	}
#line 2411 "a.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 490 "a.y" /* yacc.c:1646  */
    {
		outgcode((yyvsp[-7].lval), &(yyvsp[-6].gen), (yyvsp[-4].gen).reg, &(yyvsp[-2].gen), &(yyvsp[0].gen));
	}
#line 2419 "a.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 494 "a.y" /* yacc.c:1646  */
    {
		outgcode((yyvsp[-7].lval), &(yyvsp[-6].gen), (yyvsp[-4].gen).reg, &(yyvsp[-2].gen), &(yyvsp[0].gen));
	}
#line 2427 "a.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 498 "a.y" /* yacc.c:1646  */
    {
		outgcode((yyvsp[-7].lval), &(yyvsp[-6].gen), (yyvsp[-4].gen).reg, &(yyvsp[-2].gen), &(yyvsp[0].gen));
	}
#line 2435 "a.tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 502 "a.y" /* yacc.c:1646  */
    {
		outgcode((yyvsp[-7].lval), &(yyvsp[-6].gen), (yyvsp[-4].gen).reg, &(yyvsp[-2].gen), &(yyvsp[0].gen));
	}
#line 2443 "a.tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 509 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 2451 "a.tab.c" /* yacc.c:1646  */
    break;

  case 105:
#line 513 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 2459 "a.tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 521 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 2467 "a.tab.c" /* yacc.c:1646  */
    break;

  case 107:
#line 525 "a.y" /* yacc.c:1646  */
    {
		outgcode((yyvsp[-5].lval), &(yyvsp[-4].gen), NREG, &(yyvsp[-2].gen), &(yyvsp[0].gen));
	}
#line 2475 "a.tab.c" /* yacc.c:1646  */
    break;

  case 108:
#line 529 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 2483 "a.tab.c" /* yacc.c:1646  */
    break;

  case 109:
#line 533 "a.y" /* yacc.c:1646  */
    {
		outgcode((yyvsp[-5].lval), &(yyvsp[-4].gen), NREG, &(yyvsp[-2].gen), &(yyvsp[0].gen));
	}
#line 2491 "a.tab.c" /* yacc.c:1646  */
    break;

  case 110:
#line 537 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 2499 "a.tab.c" /* yacc.c:1646  */
    break;

  case 111:
#line 541 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 2507 "a.tab.c" /* yacc.c:1646  */
    break;

  case 112:
#line 545 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-1].lval), &(yyvsp[0].gen), NREG, &nullgen);
	}
#line 2515 "a.tab.c" /* yacc.c:1646  */
    break;

  case 113:
#line 552 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-1].lval), &nullgen, NREG, &nullgen);
	}
#line 2523 "a.tab.c" /* yacc.c:1646  */
    break;

  case 114:
#line 556 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-2].lval), &(yyvsp[-1].gen), NREG, &nullgen);
	}
#line 2531 "a.tab.c" /* yacc.c:1646  */
    break;

  case 115:
#line 560 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-2].lval), &(yyvsp[-1].gen), NREG, &nullgen);
	}
#line 2539 "a.tab.c" /* yacc.c:1646  */
    break;

  case 116:
#line 564 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-2].lval), &nullgen, NREG, &(yyvsp[0].gen));
	}
#line 2547 "a.tab.c" /* yacc.c:1646  */
    break;

  case 117:
#line 568 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-2].lval), &nullgen, NREG, &(yyvsp[0].gen));
	}
#line 2555 "a.tab.c" /* yacc.c:1646  */
    break;

  case 118:
#line 575 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-2].lval), &(yyvsp[-1].gen), NREG, &nullgen);
	}
#line 2563 "a.tab.c" /* yacc.c:1646  */
    break;

  case 119:
#line 579 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-2].lval), &(yyvsp[-1].gen), NREG, &nullgen);
	}
#line 2571 "a.tab.c" /* yacc.c:1646  */
    break;

  case 120:
#line 586 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-1].lval), &nullgen, NREG, &nullgen);
	}
#line 2579 "a.tab.c" /* yacc.c:1646  */
    break;

  case 121:
#line 593 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 2587 "a.tab.c" /* yacc.c:1646  */
    break;

  case 122:
#line 597 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-5].lval), &(yyvsp[-4].gen), (yyvsp[-2].lval), &(yyvsp[0].gen));
	}
#line 2595 "a.tab.c" /* yacc.c:1646  */
    break;

  case 123:
#line 601 "a.y" /* yacc.c:1646  */
    {
		outgcode((yyvsp[-5].lval), &(yyvsp[-4].gen), NREG, &(yyvsp[0].gen), &(yyvsp[-2].gen));
	}
#line 2603 "a.tab.c" /* yacc.c:1646  */
    break;

  case 124:
#line 605 "a.y" /* yacc.c:1646  */
    {
		outgcode((yyvsp[-7].lval), &(yyvsp[-6].gen), (yyvsp[-4].lval), &(yyvsp[0].gen), &(yyvsp[-2].gen));
	}
#line 2611 "a.tab.c" /* yacc.c:1646  */
    break;

  case 125:
#line 612 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-5].lval), &(yyvsp[-4].gen), (yyvsp[-2].lval), &(yyvsp[0].gen));
	}
#line 2619 "a.tab.c" /* yacc.c:1646  */
    break;

  case 126:
#line 616 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-5].lval), &(yyvsp[-4].gen), (yyvsp[-2].lval), &(yyvsp[0].gen));
	}
#line 2627 "a.tab.c" /* yacc.c:1646  */
    break;

  case 127:
#line 620 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-5].lval), &(yyvsp[-4].gen), (yyvsp[-2].lval), &(yyvsp[0].gen));
	}
#line 2635 "a.tab.c" /* yacc.c:1646  */
    break;

  case 128:
#line 627 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-1].lval), &nullgen, NREG, &nullgen);
	}
#line 2643 "a.tab.c" /* yacc.c:1646  */
    break;

  case 129:
#line 633 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_BRANCH;
		(yyval.gen).offset = (yyvsp[-3].lval) + pc;
	}
#line 2653 "a.tab.c" /* yacc.c:1646  */
    break;

  case 130:
#line 639 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		if(pass == 2)
			yyerror("undefined label: %s", (yyvsp[-1].sym)->name);
		(yyval.gen).type = D_BRANCH;
		(yyval.gen).sym = (yyvsp[-1].sym);
		(yyval.gen).offset = (yyvsp[0].lval);
	}
#line 2666 "a.tab.c" /* yacc.c:1646  */
    break;

  case 131:
#line 648 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_BRANCH;
		(yyval.gen).sym = (yyvsp[-1].sym);
		(yyval.gen).offset = (yyvsp[-1].sym)->value + (yyvsp[0].lval);
	}
#line 2677 "a.tab.c" /* yacc.c:1646  */
    break;

  case 132:
#line 657 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_REG;
		(yyval.gen).reg = (yyvsp[0].lval);
	}
#line 2687 "a.tab.c" /* yacc.c:1646  */
    break;

  case 135:
#line 669 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_SPR;
		(yyval.gen).offset = (yyvsp[0].lval);
	}
#line 2697 "a.tab.c" /* yacc.c:1646  */
    break;

  case 136:
#line 677 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_CREG;
		(yyval.gen).reg = NREG;	/* whole register */
	}
#line 2707 "a.tab.c" /* yacc.c:1646  */
    break;

  case 137:
#line 685 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_SPR;
		(yyval.gen).offset = (yyvsp[0].lval);
	}
#line 2717 "a.tab.c" /* yacc.c:1646  */
    break;

  case 138:
#line 693 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_MSR;
	}
#line 2726 "a.tab.c" /* yacc.c:1646  */
    break;

  case 139:
#line 700 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_SPR;
		(yyval.gen).offset = (yyvsp[0].lval);
	}
#line 2736 "a.tab.c" /* yacc.c:1646  */
    break;

  case 140:
#line 706 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = (yyvsp[-3].lval);
		(yyval.gen).offset = (yyvsp[-1].lval);
	}
#line 2746 "a.tab.c" /* yacc.c:1646  */
    break;

  case 141:
#line 712 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = (yyvsp[-3].lval);
		(yyval.gen).offset = (yyvsp[-1].lval);
	}
#line 2756 "a.tab.c" /* yacc.c:1646  */
    break;

  case 142:
#line 718 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = (yyvsp[-3].lval);
		(yyval.gen).reg = (yyvsp[-1].lval);
		(yyval.gen).offset = 0;
	}
#line 2767 "a.tab.c" /* yacc.c:1646  */
    break;

  case 144:
#line 728 "a.y" /* yacc.c:1646  */
    {
		if((yyvsp[-1].lval) < 0 || (yyvsp[-1].lval) > 15)
			yyerror("segment register number out of range");
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_SREG;
		(yyval.gen).reg = (yyvsp[-1].lval);
		(yyval.gen).offset = NREG;
	}
#line 2780 "a.tab.c" /* yacc.c:1646  */
    break;

  case 145:
#line 737 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_SREG;
		(yyval.gen).reg = NREG;
		(yyval.gen).offset = (yyvsp[-1].lval);
	}
#line 2791 "a.tab.c" /* yacc.c:1646  */
    break;

  case 146:
#line 746 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_FPSCR;
		(yyval.gen).reg = NREG;
	}
#line 2801 "a.tab.c" /* yacc.c:1646  */
    break;

  case 147:
#line 754 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_FPSCR;
		(yyval.gen).reg = (yyvsp[-1].lval);
	}
#line 2811 "a.tab.c" /* yacc.c:1646  */
    break;

  case 148:
#line 762 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_FREG;
		(yyval.gen).reg = (yyvsp[0].lval);
	}
#line 2821 "a.tab.c" /* yacc.c:1646  */
    break;

  case 149:
#line 768 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_FREG;
		(yyval.gen).reg = (yyvsp[-1].lval);
	}
#line 2831 "a.tab.c" /* yacc.c:1646  */
    break;

  case 150:
#line 776 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_CREG;
		(yyval.gen).reg = (yyvsp[0].lval);
	}
#line 2841 "a.tab.c" /* yacc.c:1646  */
    break;

  case 151:
#line 782 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_CREG;
		(yyval.gen).reg = (yyvsp[-1].lval);
	}
#line 2851 "a.tab.c" /* yacc.c:1646  */
    break;

  case 152:
#line 790 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_REG;
		(yyval.gen).reg = (yyvsp[0].lval);
	}
#line 2861 "a.tab.c" /* yacc.c:1646  */
    break;

  case 153:
#line 798 "a.y" /* yacc.c:1646  */
    {
		int mb, me;
		ulong v;

		(yyval.gen) = nullgen;
		(yyval.gen).type = D_CONST;
		mb = (yyvsp[-2].lval);
		me = (yyvsp[0].lval);
		if(mb < 0 || mb > 31 || me < 0 || me > 31){
			yyerror("illegal mask start/end value(s)");
			mb = me = 0;
		}
		if(mb <= me)
			v = ((ulong)~0L>>mb) & (~0L<<(31-me));
		else
			v = ~(((ulong)~0L>>(me+1)) & (~0L<<(31-(mb-1))));
		(yyval.gen).offset = v;
	}
#line 2884 "a.tab.c" /* yacc.c:1646  */
    break;

  case 154:
#line 819 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = (yyvsp[0].gen);
		(yyval.gen).type = D_CONST;
	}
#line 2893 "a.tab.c" /* yacc.c:1646  */
    break;

  case 155:
#line 824 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_SCONST;
		memcpy((yyval.gen).sval, (yyvsp[0].sval), sizeof((yyval.gen).sval));
	}
#line 2903 "a.tab.c" /* yacc.c:1646  */
    break;

  case 156:
#line 832 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_FCONST;
		(yyval.gen).dval = (yyvsp[0].dval);
	}
#line 2913 "a.tab.c" /* yacc.c:1646  */
    break;

  case 157:
#line 838 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_FCONST;
		(yyval.gen).dval = -(yyvsp[0].dval);
	}
#line 2923 "a.tab.c" /* yacc.c:1646  */
    break;

  case 158:
#line 845 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_CONST;
		(yyval.gen).offset = (yyvsp[0].lval);
	}
#line 2933 "a.tab.c" /* yacc.c:1646  */
    break;

  case 160:
#line 854 "a.y" /* yacc.c:1646  */
    {
		if((yyval.lval) < 0 || (yyval.lval) >= NREG)
			print("register value out of range\n");
		(yyval.lval) = (yyvsp[-1].lval);
	}
#line 2943 "a.tab.c" /* yacc.c:1646  */
    break;

  case 161:
#line 862 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_OREG;
		(yyval.gen).reg = (yyvsp[-1].lval);
		(yyval.gen).offset = 0;
	}
#line 2954 "a.tab.c" /* yacc.c:1646  */
    break;

  case 162:
#line 869 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_OREG;
		(yyval.gen).reg = (yyvsp[-3].lval);
		(yyval.gen).xreg = (yyvsp[-1].lval);
		(yyval.gen).offset = 0;
	}
#line 2966 "a.tab.c" /* yacc.c:1646  */
    break;

  case 164:
#line 880 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_OREG;
		(yyval.gen).reg = (yyvsp[-1].lval);
		(yyval.gen).offset = (yyvsp[-3].lval);
	}
#line 2977 "a.tab.c" /* yacc.c:1646  */
    break;

  case 165:
#line 889 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_OREG;
		(yyval.gen).name = (yyvsp[-1].lval);
		(yyval.gen).sym = S;
		(yyval.gen).offset = (yyvsp[-3].lval);
	}
#line 2989 "a.tab.c" /* yacc.c:1646  */
    break;

  case 166:
#line 897 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_OREG;
		(yyval.gen).name = (yyvsp[-1].lval);
		(yyval.gen).sym = (yyvsp[-4].sym);
		(yyval.gen).offset = (yyvsp[-3].lval);
	}
#line 3001 "a.tab.c" /* yacc.c:1646  */
    break;

  case 167:
#line 905 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_OREG;
		(yyval.gen).name = D_STATIC;
		(yyval.gen).sym = (yyvsp[-6].sym);
		(yyval.gen).offset = (yyvsp[-3].lval);
	}
#line 3013 "a.tab.c" /* yacc.c:1646  */
    break;

  case 170:
#line 917 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = 0;
	}
#line 3021 "a.tab.c" /* yacc.c:1646  */
    break;

  case 171:
#line 921 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[0].lval);
	}
#line 3029 "a.tab.c" /* yacc.c:1646  */
    break;

  case 172:
#line 925 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = -(yyvsp[0].lval);
	}
#line 3037 "a.tab.c" /* yacc.c:1646  */
    break;

  case 177:
#line 937 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[0].sym)->value;
	}
#line 3045 "a.tab.c" /* yacc.c:1646  */
    break;

  case 178:
#line 941 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = -(yyvsp[0].lval);
	}
#line 3053 "a.tab.c" /* yacc.c:1646  */
    break;

  case 179:
#line 945 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[0].lval);
	}
#line 3061 "a.tab.c" /* yacc.c:1646  */
    break;

  case 180:
#line 949 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = ~(yyvsp[0].lval);
	}
#line 3069 "a.tab.c" /* yacc.c:1646  */
    break;

  case 181:
#line 953 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-1].lval);
	}
#line 3077 "a.tab.c" /* yacc.c:1646  */
    break;

  case 183:
#line 960 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) + (yyvsp[0].lval);
	}
#line 3085 "a.tab.c" /* yacc.c:1646  */
    break;

  case 184:
#line 964 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) - (yyvsp[0].lval);
	}
#line 3093 "a.tab.c" /* yacc.c:1646  */
    break;

  case 185:
#line 968 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) * (yyvsp[0].lval);
	}
#line 3101 "a.tab.c" /* yacc.c:1646  */
    break;

  case 186:
#line 972 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) / (yyvsp[0].lval);
	}
#line 3109 "a.tab.c" /* yacc.c:1646  */
    break;

  case 187:
#line 976 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) % (yyvsp[0].lval);
	}
#line 3117 "a.tab.c" /* yacc.c:1646  */
    break;

  case 188:
#line 980 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-3].lval) << (yyvsp[0].lval);
	}
#line 3125 "a.tab.c" /* yacc.c:1646  */
    break;

  case 189:
#line 984 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-3].lval) >> (yyvsp[0].lval);
	}
#line 3133 "a.tab.c" /* yacc.c:1646  */
    break;

  case 190:
#line 988 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) & (yyvsp[0].lval);
	}
#line 3141 "a.tab.c" /* yacc.c:1646  */
    break;

  case 191:
#line 992 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) ^ (yyvsp[0].lval);
	}
#line 3149 "a.tab.c" /* yacc.c:1646  */
    break;

  case 192:
#line 996 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) | (yyvsp[0].lval);
	}
#line 3157 "a.tab.c" /* yacc.c:1646  */
    break;


#line 3161 "a.tab.c" /* yacc.c:1646  */
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
