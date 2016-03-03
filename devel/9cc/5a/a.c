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
    LTYPE1 = 258,
    LTYPE2 = 259,
    LTYPE3 = 260,
    LTYPE4 = 261,
    LTYPE5 = 262,
    LTYPE6 = 263,
    LTYPE7 = 264,
    LTYPE8 = 265,
    LTYPE9 = 266,
    LTYPEA = 267,
    LTYPEB = 268,
    LTYPEC = 269,
    LTYPED = 270,
    LTYPEE = 271,
    LTYPEF = 272,
    LTYPEG = 273,
    LTYPEH = 274,
    LTYPEI = 275,
    LTYPEJ = 276,
    LTYPEK = 277,
    LTYPEL = 278,
    LTYPEM = 279,
    LTYPEN = 280,
    LTYPEBX = 281,
    LCONST = 282,
    LSP = 283,
    LSB = 284,
    LFP = 285,
    LPC = 286,
    LTYPEX = 287,
    LR = 288,
    LREG = 289,
    LF = 290,
    LFREG = 291,
    LC = 292,
    LCREG = 293,
    LPSR = 294,
    LFCR = 295,
    LCOND = 296,
    LS = 297,
    LAT = 298,
    LFCONST = 299,
    LSCONST = 300,
    LNAME = 301,
    LLAB = 302,
    LVAR = 303
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

#line 167 "a.tab.c" /* yacc.c:355  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_A_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 182 "a.tab.c" /* yacc.c:358  */

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
#define YYLAST   671

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  69
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  34
/* YYNRULES -- Number of rules.  */
#define YYNRULES  128
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  326

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   303

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,    67,    12,     5,     2,
      65,    66,    10,     8,    62,     9,     2,    11,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    59,    61,
       6,    60,     7,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    63,     2,    64,     4,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     3,     2,    68,     2,     2,     2,
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
      55,    56,    57,    58
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    34,    34,    35,    39,    38,    46,    45,    51,    56,
      62,    63,    64,    70,    74,    78,    85,    92,    99,   103,
     110,   117,   124,   131,   138,   147,   159,   163,   167,   174,
     181,   185,   192,   199,   206,   213,   217,   221,   225,   232,
     254,   261,   270,   276,   279,   283,   288,   289,   292,   298,
     307,   315,   321,   326,   331,   337,   340,   346,   354,   358,
     367,   373,   374,   375,   376,   381,   387,   393,   399,   400,
     403,   404,   412,   421,   422,   431,   432,   438,   441,   442,
     443,   445,   453,   461,   470,   476,   482,   488,   496,   502,
     510,   511,   515,   523,   524,   530,   531,   539,   540,   543,
     549,   557,   565,   573,   583,   586,   590,   596,   597,   598,
     601,   602,   606,   610,   614,   618,   624,   627,   633,   634,
     638,   642,   646,   650,   654,   658,   662,   666,   670
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "'|'", "'^'", "'&'", "'<'", "'>'", "'+'",
  "'-'", "'*'", "'/'", "'%'", "LTYPE1", "LTYPE2", "LTYPE3", "LTYPE4",
  "LTYPE5", "LTYPE6", "LTYPE7", "LTYPE8", "LTYPE9", "LTYPEA", "LTYPEB",
  "LTYPEC", "LTYPED", "LTYPEE", "LTYPEF", "LTYPEG", "LTYPEH", "LTYPEI",
  "LTYPEJ", "LTYPEK", "LTYPEL", "LTYPEM", "LTYPEN", "LTYPEBX", "LCONST",
  "LSP", "LSB", "LFP", "LPC", "LTYPEX", "LR", "LREG", "LF", "LFREG", "LC",
  "LCREG", "LPSR", "LFCR", "LCOND", "LS", "LAT", "LFCONST", "LSCONST",
  "LNAME", "LLAB", "LVAR", "':'", "'='", "';'", "','", "'['", "']'", "'('",
  "')'", "'$'", "'~'", "$accept", "prog", "line", "$@1", "$@2", "inst",
  "cond", "comma", "rel", "ximm", "fcon", "reglist", "gen", "nireg",
  "ireg", "ioreg", "oreg", "imsr", "imm", "reg", "regreg", "shift", "rcon",
  "sreg", "spreg", "creg", "frcon", "freg", "name", "offset", "pointer",
  "con", "oexpr", "expr", YY_NULLPTR
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
     295,   296,   297,   298,   299,   300,   301,   302,   303,    58,
      61,    59,    44,    91,    93,    40,    41,    36,   126
};
# endif

#define YYPACT_NINF -146

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-146)))

#define YYTABLE_NINF -59

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -146,   306,  -146,   -42,  -146,  -146,  -146,  -146,   -40,  -146,
    -146,  -146,  -146,  -146,   498,   498,  -146,   -40,   -40,  -146,
    -146,  -146,  -146,  -146,  -146,   -40,    -8,   -35,   -31,  -146,
    -146,     6,  -146,   102,   102,   384,   -25,   -40,   466,   -25,
     102,   447,    48,   -25,   538,   538,  -146,    95,  -146,   538,
     538,   -30,    18,    61,   213,  -146,    28,   174,   474,   132,
     174,   213,   213,    20,  -146,   538,  -146,   538,  -146,  -146,
    -146,    32,  -146,  -146,  -146,   538,    76,  -146,  -146,  -146,
    -146,    49,    93,   105,  -146,  -146,  -146,   205,   389,  -146,
    -146,   113,  -146,  -146,  -146,  -146,   124,  -146,   125,   128,
     492,  -146,    98,    98,  -146,   131,   423,   117,    36,   144,
     147,    20,   153,  -146,  -146,  -146,  -146,   231,   538,   538,
     175,  -146,   114,  -146,   500,    21,   538,   -40,  -146,   177,
     182,    12,  -146,   183,  -146,   185,   188,   191,    36,  -146,
     355,   305,   355,   610,   538,  -146,    36,   249,   251,   253,
     254,    36,   538,   196,   532,   199,  -146,  -146,  -146,   128,
     423,    36,   108,   539,    95,  -146,  -146,  -146,  -146,   202,
    -146,  -146,   227,  -146,    36,   210,     7,   212,   108,   214,
      20,    98,  -146,  -146,    21,   538,   538,   538,   272,   275,
     538,   538,   538,   538,   538,  -146,  -146,   221,  -146,  -146,
    -146,   220,   226,  -146,    66,   538,   235,   136,    66,    36,
      36,  -146,  -146,  -146,  -146,   225,  -146,   228,   205,   205,
     205,   205,  -146,   252,  -146,  -146,   536,  -146,   267,   268,
     287,   175,   379,   288,   -40,   230,    36,    36,    36,    36,
     293,   294,   291,   649,   398,   656,   538,   538,   190,   190,
    -146,  -146,  -146,   298,  -146,    28,  -146,   340,   299,  -146,
     -40,   320,   321,  -146,    36,  -146,  -146,  -146,  -146,  -146,
    -146,  -146,  -146,   128,  -146,  -146,  -146,  -146,  -146,   544,
    -146,  -146,   229,  -146,   130,  -146,   356,  -146,   121,   121,
    -146,  -146,    36,    66,  -146,   295,    36,  -146,  -146,  -146,
    -146,   328,   334,  -146,    36,  -146,   338,  -146,   112,   339,
      36,   349,  -146,   353,    36,  -146,   538,   112,   357,   292,
     362,  -146,  -146,   538,  -146,   640
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,     0,    43,    43,    43,    43,    46,    43,
      43,    43,    43,    43,     0,     0,    43,    46,    46,    43,
      43,    43,    43,    43,    43,    46,     0,     0,     0,    10,
       3,     0,    12,     0,     0,     0,    46,    46,     0,    46,
       0,     0,    46,    46,     0,     0,   110,   104,   111,     0,
       0,     0,     0,     0,     0,    42,     0,     0,     0,     0,
       0,     0,     0,     0,     6,     0,     4,     0,    11,    94,
      91,     0,    90,    44,    45,     0,     0,    79,    78,    80,
      93,    82,     0,     0,    99,    65,    66,     0,     0,    62,
      55,     0,    73,    77,    68,    61,    63,    69,    75,    67,
       0,    47,   104,   104,    21,     0,     0,     0,     0,     0,
       0,     0,     0,    82,    29,   113,   112,     0,     0,     0,
       0,   118,     0,   114,     0,     0,     0,    46,    34,     0,
       0,     0,    98,     0,    97,     0,     0,     0,     0,    20,
       0,     0,     0,     0,     0,    81,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    56,    54,    52,    51,
       0,     0,     0,     0,   104,    18,    19,    70,    71,     0,
      49,    50,     0,    22,     0,     0,    46,     0,     0,     0,
       0,   104,   105,   106,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   115,    30,     0,   108,   107,
     109,     0,     0,    33,     0,     0,     0,     0,     0,     0,
       0,     7,     8,     5,     9,     0,    15,    82,     0,     0,
       0,     0,    16,     0,    72,    57,     0,    17,     0,     0,
       0,    49,     0,     0,    46,     0,     0,     0,     0,     0,
      46,     0,     0,   128,   127,   126,     0,     0,   119,   120,
     121,   122,   123,     0,   101,     0,    35,     0,    99,    36,
      46,     0,     0,    92,    14,    84,    88,    89,    85,    86,
      87,   100,    53,     0,    64,    76,    74,    48,    23,     0,
      59,    60,     0,    28,    46,    27,     0,   102,   124,   125,
      31,    32,     0,     0,    38,     0,     0,    13,    25,    24,
      26,     0,     0,    37,     0,    40,     0,   103,     0,     0,
       0,     0,    95,     0,     0,    41,     0,     0,     0,     0,
     116,    83,    96,     0,    39,   117
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -146,  -146,   -99,  -146,  -146,  -146,   618,    45,   337,   -48,
     380,   -71,   -96,  -146,   -43,   -38,   -79,     5,  -118,   -12,
    -146,   -24,     3,  -145,   -33,   129,  -146,   -13,    -2,   -88,
     257,   -10,  -146,   -29
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    30,   142,   140,    31,    33,   101,   104,    89,
      90,   175,    91,   166,    92,    93,    94,    76,    77,    78,
     305,    79,   265,    80,   113,   313,   133,    97,    98,   120,
     201,   121,   324,   122
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      81,    81,    81,   109,    52,    52,   196,    81,   128,   158,
     173,    96,    51,    53,   170,   171,   236,   229,   230,    32,
     139,   206,    37,    95,    66,    99,    73,    74,   105,    67,
     112,   110,   124,   230,   115,   116,   141,    37,   143,    82,
     123,   211,   127,   213,   129,   107,   134,   135,   130,   136,
     137,    64,    65,    38,   153,   147,   148,   167,   149,   198,
     199,   200,    55,    56,   227,   145,   156,    68,   179,    37,
      63,   -58,   126,    81,    69,   176,   231,    70,   159,    71,
      72,   100,    96,   125,   106,   138,    69,   111,   114,    70,
     169,    71,    72,   241,    95,    88,    99,   144,   168,    73,
      74,   117,   150,   118,   119,   153,   118,   119,   182,   183,
      37,    83,    84,   217,   197,   215,   202,   185,   186,   187,
     188,   189,   190,   191,   192,   193,   194,    81,   228,   190,
     191,   192,   193,   194,   216,   290,    96,   240,   146,   222,
      69,   234,   223,    70,   116,    71,    72,   272,    95,    70,
      99,    71,    72,    73,    74,   151,   243,   244,   245,   311,
     312,   248,   249,   250,   251,   252,   281,   282,    69,    75,
     152,    70,   203,    71,    72,   160,   257,    83,    84,   174,
     195,    83,   258,    73,    74,   266,   266,   266,   266,   161,
     162,   256,    37,   163,   259,   260,   172,   261,   262,   131,
     192,   193,   194,   280,   176,   176,   177,   291,   267,   267,
     267,   267,   178,    44,    45,   180,   273,   288,   289,    83,
      84,   237,   268,   269,   270,    73,    74,   283,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   181,   204,
     184,   298,    46,    69,   205,   207,    70,   208,    71,    72,
     209,    69,   297,   210,    70,   218,    71,    72,   219,   302,
     220,   221,   224,    48,    73,    74,   226,   232,   233,   110,
      49,   309,   300,    50,   235,   238,   239,   315,   246,   278,
     303,   318,   247,   253,   306,   285,   254,   319,   255,   225,
     264,   263,   279,   299,   325,   185,   186,   187,   188,   189,
     190,   191,   192,   193,   194,   294,     2,     3,   185,   186,
     187,   188,   189,   190,   191,   192,   193,   194,   271,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,   274,   275,    18,    19,    20,    21,    22,
      23,    24,    25,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   276,   277,   284,     3,   287,   322,   286,
     304,   293,    26,    27,    28,    75,   212,    29,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,   295,   296,    18,    19,    20,    21,    22,    23,
      24,    25,    44,    45,   307,   301,   308,    44,   154,   155,
     310,   314,   292,   187,   188,   189,   190,   191,   192,   193,
     194,    26,    27,    28,   316,   317,    29,   198,   199,   200,
     233,    46,    69,   321,   323,    70,    46,    71,    72,    83,
      84,    44,    45,    85,    86,    73,    74,   165,     0,   132,
      47,   242,    48,   156,   157,    47,   320,    48,     0,    87,
       0,    88,    50,     0,    87,    44,    45,    50,     0,     0,
      46,    69,     0,     0,    70,     0,    71,    72,    83,    84,
       0,     0,    85,    86,    44,    45,     0,     0,     0,    47,
       0,    48,    44,    45,    46,     0,     0,     0,    87,     0,
      88,    50,     0,     0,     0,     0,     0,     0,    73,    74,
      44,    45,     0,    46,     0,    48,    44,    45,    44,    45,
     108,    46,    87,     0,     0,    50,     0,     0,     0,     0,
       0,     0,   102,   103,    48,    73,    74,     0,     0,    46,
       0,    49,    48,     0,    50,    46,     0,    46,     0,    49,
      44,    45,    50,     0,    44,    45,    44,    45,   164,   103,
      48,     0,    44,    45,    47,     0,    48,    87,    48,     0,
      50,     0,     0,    49,     0,    49,    50,    75,    50,    46,
       0,     0,     0,    46,     0,    46,     0,   198,   199,   200,
      70,    46,    71,    72,     0,     0,   225,     0,     0,     0,
      48,     0,    47,     0,    48,     0,    48,    49,     0,     0,
      50,    87,    48,    49,    50,     0,    50,     0,     0,    87,
       0,     0,    50,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,    34,    35,    36,     0,    39,    40,    41,
      42,    43,     0,     0,    54,     0,     0,    57,    58,    59,
      60,    61,    62,   185,   186,   187,   188,   189,   190,   191,
     192,   193,   194,   186,   187,   188,   189,   190,   191,   192,
     193,   194,   188,   189,   190,   191,   192,   193,   194,     0,
       0,   214
};

static const yytype_int16 yycheck[] =
{
      33,    34,    35,    41,    14,    15,   124,    40,    56,    88,
     106,    35,    14,    15,   102,   103,     9,   162,   163,    61,
      63,     9,    62,    35,    59,    35,    51,    52,    38,    60,
      42,    41,    62,   178,    44,    45,    65,    62,    67,    34,
      50,   140,    54,   142,    57,    40,    59,    60,    58,    61,
      62,    59,    60,     8,    87,     6,     7,   100,     9,    38,
      39,    40,    17,    18,   160,    75,    54,    61,   111,    62,
      25,    64,    11,   106,    38,   108,   164,    41,    88,    43,
      44,    36,   106,    65,    39,    65,    38,    42,    43,    41,
     100,    43,    44,   181,   106,    67,   106,    65,   100,    51,
      52,     6,    53,     8,     9,   138,     8,     9,   118,   119,
      62,    45,    46,   146,   124,   144,   126,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,   160,   161,     8,
       9,    10,    11,    12,   146,   253,   160,   180,    62,   151,
      38,   174,   152,    41,   154,    43,    44,   226,   160,    41,
     160,    43,    44,    51,    52,    62,   185,   186,   187,    47,
      48,   190,   191,   192,   193,   194,   237,   238,    38,    67,
      65,    41,   127,    43,    44,    62,   205,    45,    46,    62,
      66,    45,    46,    51,    52,   218,   219,   220,   221,    65,
      65,   204,    62,    65,   207,   208,    65,   209,   210,    67,
      10,    11,    12,   236,   237,   238,    62,   255,   218,   219,
     220,   221,    65,     8,     9,    62,   226,   246,   247,    45,
      46,   176,   219,   220,   221,    51,    52,   239,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,     7,    62,
      65,   279,    37,    38,    62,    62,    41,    62,    43,    44,
      62,    38,   264,    62,    41,     6,    43,    44,     7,   292,
       7,     7,    66,    58,    51,    52,    67,    65,    41,   279,
      65,   304,   284,    68,    64,    63,    62,   310,     6,   234,
     293,   314,     7,    62,   296,   240,    66,   316,    62,    54,
      62,    66,    62,    64,   323,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,   260,     0,     1,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    66,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    66,    66,    29,    30,    31,    32,    33,
      34,    35,    36,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    66,    66,    62,     1,    66,    66,    65,
      65,    62,    56,    57,    58,    67,    61,    61,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    62,    62,    29,    30,    31,    32,    33,    34,
      35,    36,     8,     9,    66,    39,    62,     8,     9,    10,
      62,    62,    62,     5,     6,     7,     8,     9,    10,    11,
      12,    56,    57,    58,    65,    62,    61,    38,    39,    40,
      41,    37,    38,    66,    62,    41,    37,    43,    44,    45,
      46,     8,     9,    49,    50,    51,    52,   100,    -1,    59,
      56,   184,    58,    54,    55,    56,   317,    58,    -1,    65,
      -1,    67,    68,    -1,    65,     8,     9,    68,    -1,    -1,
      37,    38,    -1,    -1,    41,    -1,    43,    44,    45,    46,
      -1,    -1,    49,    50,     8,     9,    -1,    -1,    -1,    56,
      -1,    58,     8,     9,    37,    -1,    -1,    -1,    65,    -1,
      67,    68,    -1,    -1,    -1,    -1,    -1,    -1,    51,    52,
       8,     9,    -1,    37,    -1,    58,     8,     9,     8,     9,
      63,    37,    65,    -1,    -1,    68,    -1,    -1,    -1,    -1,
      -1,    -1,    56,    57,    58,    51,    52,    -1,    -1,    37,
      -1,    65,    58,    -1,    68,    37,    -1,    37,    -1,    65,
       8,     9,    68,    -1,     8,     9,     8,     9,    56,    57,
      58,    -1,     8,     9,    56,    -1,    58,    65,    58,    -1,
      68,    -1,    -1,    65,    -1,    65,    68,    67,    68,    37,
      -1,    -1,    -1,    37,    -1,    37,    -1,    38,    39,    40,
      41,    37,    43,    44,    -1,    -1,    54,    -1,    -1,    -1,
      58,    -1,    56,    -1,    58,    -1,    58,    65,    -1,    -1,
      68,    65,    58,    65,    68,    -1,    68,    -1,    -1,    65,
      -1,    -1,    68,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,     5,     6,     7,    -1,     9,    10,    11,
      12,    13,    -1,    -1,    16,    -1,    -1,    19,    20,    21,
      22,    23,    24,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,     4,     5,     6,     7,     8,     9,    10,
      11,    12,     6,     7,     8,     9,    10,    11,    12,    -1,
      -1,    61
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    70,     0,     1,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    29,    30,
      31,    32,    33,    34,    35,    36,    56,    57,    58,    61,
      71,    74,    61,    75,    75,    75,    75,    62,    76,    75,
      75,    75,    75,    75,     8,     9,    37,    56,    58,    65,
      68,    97,   100,    97,    75,    76,    76,    75,    75,    75,
      75,    75,    75,    76,    59,    60,    59,    60,    61,    38,
      41,    43,    44,    51,    52,    67,    86,    87,    88,    90,
      92,    93,    86,    45,    46,    49,    50,    65,    67,    78,
      79,    81,    83,    84,    85,    88,    90,    96,    97,   100,
      76,    76,    56,    57,    77,   100,    76,    86,    63,    84,
     100,    76,    88,    93,    76,   100,   100,     6,     8,     9,
      98,   100,   102,   100,    62,    65,    11,    88,    78,    96,
     100,    67,    79,    95,    96,    96,    88,    88,    65,    83,
      73,   102,    72,   102,    65,   100,    62,     6,     7,     9,
      53,    62,    65,    93,     9,    10,    54,    55,    85,   100,
      62,    65,    65,    65,    56,    77,    82,    83,    97,   100,
      98,    98,    65,    81,    62,    80,    93,    62,    65,    83,
      62,     7,   100,   100,    65,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    66,    87,   100,    38,    39,
      40,    99,   100,    76,    62,    62,     9,    62,    62,    62,
      62,    71,    61,    71,    61,   102,    88,    93,     6,     7,
       7,     7,    88,   100,    66,    54,    67,    81,    93,    92,
      92,    98,    65,    41,    93,    64,     9,    76,    63,    62,
      83,    98,    99,   102,   102,   102,     6,     7,   102,   102,
     102,   102,   102,    62,    66,    62,    96,   102,    46,    96,
      96,    88,    88,    66,    62,    91,    93,   100,    91,    91,
      91,    66,    85,   100,    66,    66,    66,    66,    76,    62,
      93,    80,    80,    88,    62,    76,    65,    66,   102,   102,
      87,    78,    62,    62,    76,    62,    62,    88,    84,    64,
      88,    39,    93,    96,    65,    89,    88,    66,    62,    93,
      62,    47,    48,    94,    62,    93,    65,    62,    93,   102,
      94,    66,    66,    62,   101,   102
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    69,    70,    70,    72,    71,    73,    71,    71,    71,
      71,    71,    71,    74,    74,    74,    74,    74,    74,    74,
      74,    74,    74,    74,    74,    74,    74,    74,    74,    74,
      74,    74,    74,    74,    74,    74,    74,    74,    74,    74,
      74,    74,    74,    75,    75,    75,    76,    76,    77,    77,
      77,    78,    78,    78,    78,    78,    79,    79,    80,    80,
      80,    81,    81,    81,    81,    81,    81,    81,    81,    81,
      82,    82,    83,    84,    84,    85,    85,    85,    86,    86,
      86,    87,    88,    89,    90,    90,    90,    90,    91,    91,
      92,    92,    92,    93,    93,    94,    94,    95,    95,    96,
      96,    97,    97,    97,    98,    98,    98,    99,    99,    99,
     100,   100,   100,   100,   100,   100,   101,   101,   102,   102,
     102,   102,   102,   102,   102,   102,   102,   102,   102
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     0,     4,     0,     4,     4,     4,
       1,     2,     2,     7,     6,     5,     5,     5,     4,     4,
       3,     3,     4,     6,     7,     7,     7,     6,     6,     3,
       4,     6,     6,     4,     3,     5,     5,     7,     6,    12,
       7,     9,     2,     0,     2,     2,     0,     2,     4,     2,
       2,     2,     2,     4,     2,     1,     2,     3,     1,     3,
       3,     1,     1,     1,     4,     1,     1,     1,     1,     1,
       1,     1,     3,     1,     4,     1,     4,     1,     1,     1,
       1,     2,     1,     5,     4,     4,     4,     4,     1,     1,
       1,     1,     4,     1,     1,     1,     4,     1,     1,     1,
       4,     4,     5,     7,     0,     2,     2,     1,     1,     1,
       1,     1,     2,     2,     2,     3,     0,     2,     1,     3,
       3,     3,     3,     3,     4,     4,     3,     3,     3
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
#line 39 "a.y" /* yacc.c:1646  */
    {
		if((yyvsp[-1].sym)->value != pc)
			yyerror("redeclaration of %s", (yyvsp[-1].sym)->name);
		(yyvsp[-1].sym)->value = pc;
	}
#line 1545 "a.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 46 "a.y" /* yacc.c:1646  */
    {
		(yyvsp[-1].sym)->type = LLAB;
		(yyvsp[-1].sym)->value = pc;
	}
#line 1554 "a.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 52 "a.y" /* yacc.c:1646  */
    {
		(yyvsp[-3].sym)->type = LVAR;
		(yyvsp[-3].sym)->value = (yyvsp[-1].lval);
	}
#line 1563 "a.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 57 "a.y" /* yacc.c:1646  */
    {
		if((yyvsp[-3].sym)->value != (yyvsp[-1].lval))
			yyerror("redeclaration of %s", (yyvsp[-3].sym)->name);
		(yyvsp[-3].sym)->value = (yyvsp[-1].lval);
	}
#line 1573 "a.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 71 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-6].lval), (yyvsp[-5].lval), &(yyvsp[-4].gen), (yyvsp[-2].lval), &(yyvsp[0].gen));
	}
#line 1581 "a.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 75 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-5].lval), (yyvsp[-4].lval), &(yyvsp[-3].gen), (yyvsp[-1].lval), &nullgen);
	}
#line 1589 "a.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 79 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-4].lval), (yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1597 "a.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 86 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-4].lval), (yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1605 "a.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 93 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-4].lval), (yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1613 "a.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 100 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), (yyvsp[-2].lval), &nullgen, NREG, &(yyvsp[0].gen));
	}
#line 1621 "a.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 104 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), (yyvsp[-2].lval), &nullgen, NREG, &(yyvsp[0].gen));
	}
#line 1629 "a.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 111 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-2].lval), Always, &nullgen, NREG, &(yyvsp[0].gen));
	}
#line 1637 "a.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 118 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-2].lval), Always, &nullgen, NREG, &(yyvsp[0].gen));
	}
#line 1645 "a.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 125 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), (yyvsp[-2].lval), &nullgen, NREG, &(yyvsp[0].gen));
	}
#line 1653 "a.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 132 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-5].lval), (yyvsp[-4].lval), &(yyvsp[-3].gen), (yyvsp[-1].lval), &nullgen);
	}
#line 1661 "a.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 139 "a.y" /* yacc.c:1646  */
    {
		Gen g;

		g = nullgen;
		g.type = D_CONST;
		g.offset = (yyvsp[-1].lval);
		outcode((yyvsp[-6].lval), (yyvsp[-5].lval), &(yyvsp[-4].gen), NREG, &g);
	}
#line 1674 "a.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 148 "a.y" /* yacc.c:1646  */
    {
		Gen g;

		g = nullgen;
		g.type = D_CONST;
		g.offset = (yyvsp[-3].lval);
		outcode((yyvsp[-6].lval), (yyvsp[-5].lval), &g, NREG, &(yyvsp[0].gen));
	}
#line 1687 "a.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 160 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-6].lval), (yyvsp[-5].lval), &(yyvsp[-2].gen), (yyvsp[-4].gen).reg, &(yyvsp[0].gen));
	}
#line 1695 "a.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 164 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-5].lval), (yyvsp[-4].lval), &(yyvsp[-1].gen), (yyvsp[-3].gen).reg, &(yyvsp[-3].gen));
	}
#line 1703 "a.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 168 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-5].lval), (yyvsp[-4].lval), &(yyvsp[-2].gen), (yyvsp[0].gen).reg, &(yyvsp[0].gen));
	}
#line 1711 "a.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 175 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-2].lval), (yyvsp[-1].lval), &nullgen, NREG, &nullgen);
	}
#line 1719 "a.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 182 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), Always, &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1727 "a.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 186 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-5].lval), Always, &(yyvsp[-4].gen), (yyvsp[-2].lval), &(yyvsp[0].gen));
	}
#line 1735 "a.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 193 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-5].lval), Always, &(yyvsp[-4].gen), (yyvsp[-2].lval), &(yyvsp[0].gen));
	}
#line 1743 "a.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 200 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), (yyvsp[-2].lval), &(yyvsp[-1].gen), NREG, &nullgen);
	}
#line 1751 "a.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 207 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-2].lval), Always, &nullgen, NREG, &(yyvsp[0].gen));
	}
#line 1759 "a.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 214 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-4].lval), (yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1767 "a.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 218 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-4].lval), (yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1775 "a.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 222 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-6].lval), (yyvsp[-5].lval), &(yyvsp[-4].gen), (yyvsp[-2].lval), &(yyvsp[0].gen));
	}
#line 1783 "a.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 226 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-5].lval), (yyvsp[-4].lval), &(yyvsp[-3].gen), (yyvsp[-1].gen).reg, &nullgen);
	}
#line 1791 "a.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 233 "a.y" /* yacc.c:1646  */
    {
		Gen g;

		g = nullgen;
		g.type = D_CONST;
		g.offset =
			(0xe << 24) |		/* opcode */
			((yyvsp[-11].lval) << 20) |		/* MCR/MRC */
			((yyvsp[-10].lval) << 28) |		/* scond */
			(((yyvsp[-9].lval) & 15) << 8) |	/* coprocessor number */
			(((yyvsp[-7].lval) & 7) << 21) |	/* coprocessor operation */
			(((yyvsp[-5].lval) & 15) << 12) |	/* arm register */
			(((yyvsp[-3].lval) & 15) << 16) |	/* Crn */
			(((yyvsp[-1].lval) & 15) << 0) |	/* Crm */
			(((yyvsp[0].lval) & 7) << 5) |	/* coprocessor information */
			(1<<4);			/* must be set */
		outcode(AWORD, Always, &nullgen, NREG, &g);
	}
#line 1814 "a.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 255 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-6].lval), (yyvsp[-5].lval), &(yyvsp[-4].gen), (yyvsp[-2].gen).reg, &(yyvsp[0].gen));
	}
#line 1822 "a.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 262 "a.y" /* yacc.c:1646  */
    {
		(yyvsp[-2].gen).type = D_REGREG;
		(yyvsp[-2].gen).offset = (yyvsp[0].lval);
		outcode((yyvsp[-8].lval), (yyvsp[-7].lval), &(yyvsp[-6].gen), (yyvsp[-4].gen).reg, &(yyvsp[-2].gen));
	}
#line 1832 "a.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 271 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-1].lval), Always, &nullgen, NREG, &nullgen);
	}
#line 1840 "a.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 276 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = Always;
	}
#line 1848 "a.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 280 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = ((yyvsp[-1].lval) & ~C_SCOND) | (yyvsp[0].lval);
	}
#line 1856 "a.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 284 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-1].lval) | (yyvsp[0].lval);
	}
#line 1864 "a.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 293 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_BRANCH;
		(yyval.gen).offset = (yyvsp[-3].lval) + pc;
	}
#line 1874 "a.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 299 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		if(pass == 2)
			yyerror("undefined label: %s", (yyvsp[-1].sym)->name);
		(yyval.gen).type = D_BRANCH;
		(yyval.gen).sym = (yyvsp[-1].sym);
		(yyval.gen).offset = (yyvsp[0].lval);
	}
#line 1887 "a.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 308 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_BRANCH;
		(yyval.gen).sym = (yyvsp[-1].sym);
		(yyval.gen).offset = (yyvsp[-1].sym)->value + (yyvsp[0].lval);
	}
#line 1898 "a.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 316 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_CONST;
		(yyval.gen).offset = (yyvsp[0].lval);
	}
#line 1908 "a.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 322 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = (yyvsp[0].gen);
		(yyval.gen).type = D_CONST;
	}
#line 1917 "a.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 327 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = (yyvsp[0].gen);
		(yyval.gen).type = D_OCONST;
	}
#line 1926 "a.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 332 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_SCONST;
		memcpy((yyval.gen).sval, (yyvsp[0].sval), sizeof((yyval.gen).sval));
	}
#line 1936 "a.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 341 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_FCONST;
		(yyval.gen).dval = (yyvsp[0].dval);
	}
#line 1946 "a.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 347 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_FCONST;
		(yyval.gen).dval = -(yyvsp[0].dval);
	}
#line 1956 "a.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 355 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = 1 << (yyvsp[0].lval);
	}
#line 1964 "a.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 359 "a.y" /* yacc.c:1646  */
    {
		int i;
		(yyval.lval)=0;
		for(i=(yyvsp[-2].lval); i<=(yyvsp[0].lval); i++)
			(yyval.lval) |= 1<<i;
		for(i=(yyvsp[0].lval); i<=(yyvsp[-2].lval); i++)
			(yyval.lval) |= 1<<i;
	}
#line 1977 "a.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 368 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (1<<(yyvsp[-2].lval)) | (yyvsp[0].lval);
	}
#line 1985 "a.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 377 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = (yyvsp[-3].gen);
		(yyval.gen).reg = (yyvsp[-1].lval);
	}
#line 1994 "a.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 382 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_PSR;
		(yyval.gen).reg = (yyvsp[0].lval);
	}
#line 2004 "a.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 388 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_FPCR;
		(yyval.gen).reg = (yyvsp[0].lval);
	}
#line 2014 "a.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 394 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_OREG;
		(yyval.gen).offset = (yyvsp[0].lval);
	}
#line 2024 "a.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 405 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = (yyvsp[0].gen);
		if((yyvsp[0].gen).name != D_EXTERN && (yyvsp[0].gen).name != D_STATIC) {
		}
	}
#line 2034 "a.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 413 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_OREG;
		(yyval.gen).reg = (yyvsp[-1].lval);
		(yyval.gen).offset = 0;
	}
#line 2045 "a.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 423 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_OREG;
		(yyval.gen).reg = (yyvsp[-1].lval);
		(yyval.gen).offset = (yyvsp[-3].lval);
	}
#line 2056 "a.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 433 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = (yyvsp[-3].gen);
		(yyval.gen).type = D_OREG;
		(yyval.gen).reg = (yyvsp[-1].lval);
	}
#line 2066 "a.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 446 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_CONST;
		(yyval.gen).offset = (yyvsp[0].lval);
	}
#line 2076 "a.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 454 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_REG;
		(yyval.gen).reg = (yyvsp[0].lval);
	}
#line 2086 "a.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 462 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_REGREG;
		(yyval.gen).reg = (yyvsp[-3].lval);
		(yyval.gen).offset = (yyvsp[-1].lval);
	}
#line 2097 "a.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 471 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_SHIFT;
		(yyval.gen).offset = (yyvsp[-3].lval) | (yyvsp[0].lval) | (0 << 5);
	}
#line 2107 "a.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 477 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_SHIFT;
		(yyval.gen).offset = (yyvsp[-3].lval) | (yyvsp[0].lval) | (1 << 5);
	}
#line 2117 "a.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 483 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_SHIFT;
		(yyval.gen).offset = (yyvsp[-3].lval) | (yyvsp[0].lval) | (2 << 5);
	}
#line 2127 "a.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 489 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_SHIFT;
		(yyval.gen).offset = (yyvsp[-3].lval) | (yyvsp[0].lval) | (3 << 5);
	}
#line 2137 "a.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 497 "a.y" /* yacc.c:1646  */
    {
		if((yyval.lval) < 0 || (yyval.lval) >= 16)
			print("register value out of range\n");
		(yyval.lval) = (((yyvsp[0].lval)&15) << 8) | (1 << 4);
	}
#line 2147 "a.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 503 "a.y" /* yacc.c:1646  */
    {
		if((yyval.lval) < 0 || (yyval.lval) >= 32)
			print("shift value out of range\n");
		(yyval.lval) = ((yyvsp[0].lval)&31) << 7;
	}
#line 2157 "a.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 512 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = REGPC;
	}
#line 2165 "a.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 516 "a.y" /* yacc.c:1646  */
    {
		if((yyvsp[-1].lval) < 0 || (yyvsp[-1].lval) >= NREG)
			print("register value out of range\n");
		(yyval.lval) = (yyvsp[-1].lval);
	}
#line 2175 "a.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 525 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = REGSP;
	}
#line 2183 "a.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 532 "a.y" /* yacc.c:1646  */
    {
		if((yyvsp[-1].lval) < 0 || (yyvsp[-1].lval) >= NREG)
			print("register value out of range\n");
		(yyval.lval) = (yyvsp[-1].lval);
	}
#line 2193 "a.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 544 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_FREG;
		(yyval.gen).reg = (yyvsp[0].lval);
	}
#line 2203 "a.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 550 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_FREG;
		(yyval.gen).reg = (yyvsp[-1].lval);
	}
#line 2213 "a.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 558 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_OREG;
		(yyval.gen).name = (yyvsp[-1].lval);
		(yyval.gen).sym = S;
		(yyval.gen).offset = (yyvsp[-3].lval);
	}
#line 2225 "a.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 566 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_OREG;
		(yyval.gen).name = (yyvsp[-1].lval);
		(yyval.gen).sym = (yyvsp[-4].sym);
		(yyval.gen).offset = (yyvsp[-3].lval);
	}
#line 2237 "a.tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 574 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_OREG;
		(yyval.gen).name = D_STATIC;
		(yyval.gen).sym = (yyvsp[-6].sym);
		(yyval.gen).offset = (yyvsp[-3].lval);
	}
#line 2249 "a.tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 583 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = 0;
	}
#line 2257 "a.tab.c" /* yacc.c:1646  */
    break;

  case 105:
#line 587 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[0].lval);
	}
#line 2265 "a.tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 591 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = -(yyvsp[0].lval);
	}
#line 2273 "a.tab.c" /* yacc.c:1646  */
    break;

  case 111:
#line 603 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[0].sym)->value;
	}
#line 2281 "a.tab.c" /* yacc.c:1646  */
    break;

  case 112:
#line 607 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = -(yyvsp[0].lval);
	}
#line 2289 "a.tab.c" /* yacc.c:1646  */
    break;

  case 113:
#line 611 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[0].lval);
	}
#line 2297 "a.tab.c" /* yacc.c:1646  */
    break;

  case 114:
#line 615 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = ~(yyvsp[0].lval);
	}
#line 2305 "a.tab.c" /* yacc.c:1646  */
    break;

  case 115:
#line 619 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-1].lval);
	}
#line 2313 "a.tab.c" /* yacc.c:1646  */
    break;

  case 116:
#line 624 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = 0;
	}
#line 2321 "a.tab.c" /* yacc.c:1646  */
    break;

  case 117:
#line 628 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[0].lval);
	}
#line 2329 "a.tab.c" /* yacc.c:1646  */
    break;

  case 119:
#line 635 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) + (yyvsp[0].lval);
	}
#line 2337 "a.tab.c" /* yacc.c:1646  */
    break;

  case 120:
#line 639 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) - (yyvsp[0].lval);
	}
#line 2345 "a.tab.c" /* yacc.c:1646  */
    break;

  case 121:
#line 643 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) * (yyvsp[0].lval);
	}
#line 2353 "a.tab.c" /* yacc.c:1646  */
    break;

  case 122:
#line 647 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) / (yyvsp[0].lval);
	}
#line 2361 "a.tab.c" /* yacc.c:1646  */
    break;

  case 123:
#line 651 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) % (yyvsp[0].lval);
	}
#line 2369 "a.tab.c" /* yacc.c:1646  */
    break;

  case 124:
#line 655 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-3].lval) << (yyvsp[0].lval);
	}
#line 2377 "a.tab.c" /* yacc.c:1646  */
    break;

  case 125:
#line 659 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-3].lval) >> (yyvsp[0].lval);
	}
#line 2385 "a.tab.c" /* yacc.c:1646  */
    break;

  case 126:
#line 663 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) & (yyvsp[0].lval);
	}
#line 2393 "a.tab.c" /* yacc.c:1646  */
    break;

  case 127:
#line 667 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) ^ (yyvsp[0].lval);
	}
#line 2401 "a.tab.c" /* yacc.c:1646  */
    break;

  case 128:
#line 671 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) | (yyvsp[0].lval);
	}
#line 2409 "a.tab.c" /* yacc.c:1646  */
    break;


#line 2413 "a.tab.c" /* yacc.c:1646  */
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
