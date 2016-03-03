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
    LTYPE0 = 258,
    LTYPE1 = 259,
    LTYPE2 = 260,
    LTYPE3 = 261,
    LTYPE4 = 262,
    LTYPEC = 263,
    LTYPED = 264,
    LTYPEN = 265,
    LTYPER = 266,
    LTYPET = 267,
    LTYPES = 268,
    LTYPEM = 269,
    LTYPEI = 270,
    LTYPEG = 271,
    LCONST = 272,
    LFP = 273,
    LPC = 274,
    LSB = 275,
    LBREG = 276,
    LLREG = 277,
    LSREG = 278,
    LFREG = 279,
    LFCONST = 280,
    LSCONST = 281,
    LSP = 282,
    LNAME = 283,
    LLAB = 284,
    LVAR = 285
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 4 "a.y" /* yacc.c:355  */

	Sym	*sym;
	long	lval;
	struct {
		long v1;
		long v2;
	} con2;
	double	dval;
	char	sval[8];
	Gen	gen;
	Gen2	gen2;

#line 154 "a.tab.c" /* yacc.c:355  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_A_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 169 "a.tab.c" /* yacc.c:358  */

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
#define YYLAST   500

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  49
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  34
/* YYNRULES -- Number of rules.  */
#define YYNRULES  116
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  229

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   285

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,    47,    12,     5,     2,
      45,    46,    10,     8,    44,     9,     2,    11,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    41,    42,
       6,    43,     7,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     4,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     3,     2,    48,     2,     2,     2,
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
      35,    36,    37,    38,    39,    40
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    35,    35,    36,    40,    39,    47,    46,    52,    53,
      54,    57,    62,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    84,    88,    95,
     102,   109,   114,   121,   126,   133,   138,   145,   153,   158,
     166,   171,   178,   179,   182,   187,   197,   202,   212,   217,
     222,   229,   234,   242,   243,   246,   247,   248,   252,   256,
     257,   258,   261,   262,   265,   271,   280,   289,   294,   299,
     304,   309,   316,   322,   333,   339,   345,   351,   390,   391,
     394,   400,   406,   412,   421,   430,   435,   440,   446,   454,
     464,   468,   477,   484,   493,   496,   500,   506,   507,   511,
     514,   515,   519,   523,   527,   531,   537,   538,   542,   546,
     550,   554,   558,   562,   566,   570,   574
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "'|'", "'^'", "'&'", "'<'", "'>'", "'+'",
  "'-'", "'*'", "'/'", "'%'", "LTYPE0", "LTYPE1", "LTYPE2", "LTYPE3",
  "LTYPE4", "LTYPEC", "LTYPED", "LTYPEN", "LTYPER", "LTYPET", "LTYPES",
  "LTYPEM", "LTYPEI", "LTYPEG", "LCONST", "LFP", "LPC", "LSB", "LBREG",
  "LLREG", "LSREG", "LFREG", "LFCONST", "LSCONST", "LSP", "LNAME", "LLAB",
  "LVAR", "':'", "';'", "'='", "','", "'('", "')'", "'$'", "'~'",
  "$accept", "prog", "line", "$@1", "$@2", "inst", "nonnon", "rimrem",
  "remrim", "rimnon", "nonrem", "nonrel", "spec1", "spec2", "spec3",
  "spec4", "spec5", "spec6", "spec7", "spec8", "rem", "rom", "rim", "rel",
  "reg", "imm", "mem", "omem", "nmem", "nam", "offset", "pointer", "con",
  "expr", YY_NULLPTR
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
     285,    58,    59,    61,    44,    40,    41,    36,   126
};
# endif

#define YYPACT_NINF -91

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-91)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -91,   147,   -91,   -22,    15,   283,   166,   166,   330,   223,
      50,   303,   344,   406,   166,   166,   166,   406,    51,    24,
      44,   -91,   -91,    67,   -91,   -91,   -91,   428,   428,   -91,
     -91,   -91,   -91,   -91,   -91,    71,   -91,   330,   371,   428,
     -91,   -91,   -91,   -91,   -91,   -91,    63,    65,   364,   -91,
     -91,    68,   -91,   -91,    69,   -91,    74,    66,    71,    27,
     256,   -91,   -91,   -91,   -91,   -91,   -91,   -91,    70,   -91,
     108,   330,   -91,   -91,   -91,    27,   386,   428,   -91,   -91,
      77,   -91,    79,   -91,    83,   -91,    84,   -91,    85,   -91,
      90,   -91,   428,   -91,   428,   -91,   -91,   -91,   128,   428,
     428,    94,   -91,     7,   100,   -91,    38,   -91,   127,    -5,
     412,   -91,   -91,   421,   -91,   -91,   -91,   330,   166,   -91,
     -91,    94,   -91,   -91,    49,   428,   -91,   -91,   111,   232,
     330,   330,   330,   232,   204,   146,   204,   146,    27,   -91,
     -91,     3,   428,   143,   -91,   428,   428,   428,   184,   187,
     428,   428,   428,   428,   428,   -91,   186,    56,   155,   156,
     -91,   161,   -91,   -91,   162,   165,   -91,   168,   174,   193,
     -91,   -91,   191,   -91,   -91,   192,   -91,   -91,   -91,   190,
     199,   206,   473,   481,   488,   428,   428,     2,     2,   -91,
     -91,   -91,   428,   428,   194,   -91,   -91,   -91,   -91,   200,
     200,   216,   218,   200,   219,   -91,   -91,   242,   170,   170,
     207,   212,   237,   -91,   -91,   -91,   -91,   -91,   227,   428,
     -91,   -91,   264,   -91,   229,   428,   -91,   230,   -91
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,     0,    27,     0,     0,     0,     0,     0,
       0,    27,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     8,     3,     0,    10,    28,    13,     0,     0,   100,
      67,    69,    71,    68,    70,    94,   101,     0,     0,     0,
      14,    34,    53,    54,    78,    79,    90,    80,     0,    15,
      62,    32,    63,    16,     0,    17,     0,     0,    94,    94,
       0,    21,    41,    55,    59,    61,    60,    56,    80,    19,
       0,    28,    42,    43,    22,    94,     0,     0,    18,    36,
       0,    20,     0,    23,     0,    24,     0,    25,    49,    26,
       0,     6,     0,     4,     0,     9,   103,   102,     0,     0,
       0,     0,    33,     0,     0,   106,     0,   104,     0,     0,
       0,    75,    74,     0,    73,    72,    31,     0,     0,    57,
      58,    65,    66,    40,     0,     0,    65,    35,     0,     0,
       0,     0,    48,     0,     0,    11,     0,    12,    94,    95,
      96,     0,     0,    85,    86,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   105,     0,     0,     0,     0,
      77,     0,    29,    30,     0,     0,    38,     0,    44,    46,
      50,    51,     0,     7,     5,     0,    99,    97,    98,     0,
       0,     0,   116,   115,   114,     0,     0,   107,   108,   109,
     110,   111,     0,     0,    81,    87,    82,    76,    64,     0,
       0,     0,     0,     0,     0,    92,    88,     0,   112,   113,
       0,     0,     0,    37,    39,    45,    47,    52,     0,     0,
      91,    83,     0,    93,     0,     0,    89,     0,    84
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -91,   -91,   -29,   -91,   -91,   -91,   267,   -91,   -91,   -91,
     270,   -91,   -91,   -91,   -91,   -91,   -91,   -91,   -91,   -91,
       0,   222,     8,    -6,    12,    -8,    21,    16,    -2,     6,
     -49,   -91,    -9,   -90
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    22,   136,   134,    23,    26,    53,    55,    49,
      40,    78,    69,    81,    61,    74,    83,    85,    87,    89,
      50,    62,    51,    63,    42,    52,    43,    44,    45,    46,
     101,   179,    47,   106
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      68,    65,   135,    80,   137,    41,    79,    67,    56,   121,
     122,    41,   152,   153,   154,    54,    70,   142,    96,    97,
      24,    64,    84,    86,    88,    66,   126,   157,   158,   105,
     107,   176,   159,   177,    82,    99,   100,   102,    90,   115,
     178,   145,   146,   147,   148,   149,   150,   151,   152,   153,
     154,    68,    65,   143,   114,   182,   183,   184,    67,    25,
     187,   188,   189,   190,   191,    93,   193,    80,   105,   119,
     127,   102,    64,   120,    27,    28,    66,    98,   164,    99,
     100,   157,   158,   105,   155,   105,   159,    94,    35,   175,
     139,   140,    91,    29,    92,   208,   209,    30,    31,    32,
      33,    97,   194,    34,   105,   173,    36,   174,   108,    95,
     109,    38,   116,   117,    39,   124,   165,   162,   118,   125,
     167,   166,   128,   129,   172,   171,   163,   130,   131,   132,
     168,   169,   170,   180,   133,   138,   105,   105,   105,   141,
     164,   105,   105,   105,   105,   105,   144,     2,     3,   145,
     146,   147,   148,   149,   150,   151,   152,   153,   154,   156,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    13,
      14,    15,    16,    17,    27,    28,   105,   105,   150,   151,
     152,   153,   154,   210,   211,    18,    19,    20,   181,    21,
     185,   213,   214,    29,   186,   217,   192,    30,    31,    32,
      33,   195,   196,    34,    35,     3,    36,   197,   198,   199,
     224,    38,   200,    48,    39,   201,   227,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    13,    14,    15,    16,
      17,    27,    28,    57,   202,   203,   205,   204,   207,   212,
      27,    28,    18,    19,    20,   206,    21,    48,   215,   218,
      29,   216,   219,   220,    30,    31,    32,    33,   221,    29,
      34,    58,    59,    36,    27,    28,    57,    60,    38,   222,
      48,    39,    36,   223,   225,   226,   228,    77,    72,    48,
      39,    73,   123,    29,     0,     0,     0,    30,    31,    32,
      33,    27,    28,    34,    58,    59,    36,     0,     0,     0,
       0,    38,     0,    48,    39,     0,     0,     0,     0,     0,
      29,    27,    28,     0,    30,    31,    32,    33,     0,     0,
      34,    35,     0,    36,     0,     0,     0,    37,    38,     0,
      29,    39,     0,     0,    30,    31,    32,    33,    27,    28,
      34,    35,     0,    36,     0,     0,     0,    71,    38,     0,
       0,    39,    27,    28,     0,     0,     0,    29,     0,     0,
       0,    30,    31,    32,    33,     0,     0,    34,    35,     0,
      36,    29,    27,   110,     0,    38,     0,     0,    39,    27,
      28,     0,    75,    59,    36,     0,     0,     0,    76,    77,
       0,    29,    39,     0,    27,    28,     0,     0,    29,   111,
     112,     0,    35,   103,    36,     0,     0,     0,   104,   113,
       0,    36,    39,    29,    27,    28,    77,     0,     0,    39,
      27,    28,     0,     0,    75,    59,    36,     0,     0,    27,
      28,    77,     0,    29,    39,     0,    27,    28,     0,    29,
       0,     0,     0,     0,    35,     0,    36,   160,    29,     0,
       0,    38,    36,     0,    39,    29,   161,    77,     0,     0,
      39,    36,     0,     0,     0,     0,    77,     0,    36,    39,
       0,     0,     0,    77,     0,     0,    39,   146,   147,   148,
     149,   150,   151,   152,   153,   154,   147,   148,   149,   150,
     151,   152,   153,   154,   148,   149,   150,   151,   152,   153,
     154
};

static const yytype_int16 yycheck[] =
{
       9,     9,    92,    12,    94,     5,    12,     9,     8,    58,
      59,    11,    10,    11,    12,     7,    10,    10,    27,    28,
      42,     9,    14,    15,    16,     9,    75,    32,    33,    38,
      39,    28,    37,    30,    13,     8,     9,    37,    17,    48,
      37,     3,     4,     5,     6,     7,     8,     9,    10,    11,
      12,    60,    60,    46,    48,   145,   146,   147,    60,    44,
     150,   151,   152,   153,   154,    41,    10,    76,    77,    57,
      76,    71,    60,    57,     8,     9,    60,     6,    29,     8,
       9,    32,    33,    92,    46,    94,    37,    43,    38,   138,
      99,   100,    41,    27,    43,   185,   186,    31,    32,    33,
      34,   110,    46,    37,   113,   134,    40,   136,    45,    42,
      45,    45,    44,    44,    48,    45,   125,   117,    44,    11,
     129,   129,    45,    44,   133,   133,   118,    44,    44,    44,
     130,   131,   132,   142,    44,     7,   145,   146,   147,    45,
      29,   150,   151,   152,   153,   154,    46,     0,     1,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    12,    32,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    22,
      23,    24,    25,    26,     8,     9,   185,   186,     8,     9,
      10,    11,    12,   192,   193,    38,    39,    40,    45,    42,
       6,   199,   200,    27,     7,   203,    10,    31,    32,    33,
      34,    46,    46,    37,    38,     1,    40,    46,    46,    44,
     219,    45,    44,    47,    48,    41,   225,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    23,    24,    25,
      26,     8,     9,    10,    41,    44,    46,    45,    32,    45,
       8,     9,    38,    39,    40,    46,    42,    47,    32,    30,
      27,    33,    10,    46,    31,    32,    33,    34,    46,    27,
      37,    38,    39,    40,     8,     9,    10,    44,    45,    32,
      47,    48,    40,    46,    10,    46,    46,    45,    11,    47,
      48,    11,    60,    27,    -1,    -1,    -1,    31,    32,    33,
      34,     8,     9,    37,    38,    39,    40,    -1,    -1,    -1,
      -1,    45,    -1,    47,    48,    -1,    -1,    -1,    -1,    -1,
      27,     8,     9,    -1,    31,    32,    33,    34,    -1,    -1,
      37,    38,    -1,    40,    -1,    -1,    -1,    44,    45,    -1,
      27,    48,    -1,    -1,    31,    32,    33,    34,     8,     9,
      37,    38,    -1,    40,    -1,    -1,    -1,    44,    45,    -1,
      -1,    48,     8,     9,    -1,    -1,    -1,    27,    -1,    -1,
      -1,    31,    32,    33,    34,    -1,    -1,    37,    38,    -1,
      40,    27,     8,     9,    -1,    45,    -1,    -1,    48,     8,
       9,    -1,    38,    39,    40,    -1,    -1,    -1,    44,    45,
      -1,    27,    48,    -1,     8,     9,    -1,    -1,    27,    35,
      36,    -1,    38,    32,    40,    -1,    -1,    -1,    37,    45,
      -1,    40,    48,    27,     8,     9,    45,    -1,    -1,    48,
       8,     9,    -1,    -1,    38,    39,    40,    -1,    -1,     8,
       9,    45,    -1,    27,    48,    -1,     8,     9,    -1,    27,
      -1,    -1,    -1,    -1,    38,    -1,    40,    35,    27,    -1,
      -1,    45,    40,    -1,    48,    27,    35,    45,    -1,    -1,
      48,    40,    -1,    -1,    -1,    -1,    45,    -1,    40,    48,
      -1,    -1,    -1,    45,    -1,    -1,    48,     4,     5,     6,
       7,     8,     9,    10,    11,    12,     5,     6,     7,     8,
       9,    10,    11,    12,     6,     7,     8,     9,    10,    11,
      12
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    50,     0,     1,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    38,    39,
      40,    42,    51,    54,    42,    44,    55,     8,     9,    27,
      31,    32,    33,    34,    37,    38,    40,    44,    45,    48,
      59,    69,    73,    75,    76,    77,    78,    81,    47,    58,
      69,    71,    74,    56,    71,    57,    69,    10,    38,    39,
      44,    63,    70,    72,    73,    74,    76,    77,    81,    61,
      78,    44,    55,    59,    64,    38,    44,    45,    60,    72,
      81,    62,    75,    65,    71,    66,    71,    67,    71,    68,
      75,    41,    43,    41,    43,    42,    81,    81,     6,     8,
       9,    79,    69,    32,    37,    81,    82,    81,    45,    45,
       9,    35,    36,    45,    78,    81,    44,    44,    44,    73,
      76,    79,    79,    70,    45,    11,    79,    72,    45,    44,
      44,    44,    44,    44,    53,    82,    52,    82,     7,    81,
      81,    45,    10,    46,    46,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    46,    32,    32,    33,    37,
      35,    35,    69,    71,    29,    81,    74,    81,    69,    69,
      69,    74,    81,    51,    51,    79,    28,    30,    37,    80,
      81,    45,    82,    82,    82,     6,     7,    82,    82,    82,
      82,    82,    10,    10,    46,    46,    46,    46,    46,    44,
      44,    41,    41,    44,    45,    46,    46,    32,    82,    82,
      81,    81,    45,    74,    74,    32,    33,    74,    30,    10,
      46,    46,    32,    46,    81,    10,    46,    81,    46
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    49,    50,    50,    52,    51,    53,    51,    51,    51,
      51,    54,    54,    54,    54,    54,    54,    54,    54,    54,
      54,    54,    54,    54,    54,    54,    54,    55,    55,    56,
      57,    58,    58,    59,    59,    60,    60,    61,    62,    62,
      63,    63,    64,    64,    65,    65,    66,    66,    67,    67,
      67,    68,    68,    69,    69,    70,    70,    70,    70,    70,
      70,    70,    71,    71,    72,    72,    72,    73,    73,    73,
      73,    73,    74,    74,    74,    74,    74,    74,    75,    75,
      76,    76,    76,    76,    76,    76,    76,    76,    76,    76,
      77,    77,    78,    78,    79,    79,    79,    80,    80,    80,
      81,    81,    81,    81,    81,    81,    82,    82,    82,    82,
      82,    82,    82,    82,    82,    82,    82
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     0,     4,     0,     4,     1,     2,
       2,     3,     3,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     0,     1,     3,
       3,     2,     1,     2,     1,     2,     1,     5,     3,     5,
       2,     1,     1,     1,     3,     5,     3,     5,     2,     1,
       3,     3,     5,     1,     1,     1,     1,     2,     2,     1,
       1,     1,     1,     1,     4,     2,     2,     1,     1,     1,
       1,     1,     2,     2,     2,     2,     4,     3,     1,     1,
       1,     4,     4,     6,     9,     3,     3,     4,     5,     8,
       1,     6,     5,     7,     0,     2,     2,     1,     1,     1,
       1,     1,     2,     2,     2,     3,     1,     3,     3,     3,
       3,     3,     4,     4,     3,     3,     3
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
#line 40 "a.y" /* yacc.c:1646  */
    {
		if((yyvsp[-1].sym)->value != pc)
			yyerror("redeclaration of %s", (yyvsp[-1].sym)->name);
		(yyvsp[-1].sym)->value = pc;
	}
#line 1459 "a.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 47 "a.y" /* yacc.c:1646  */
    {
		(yyvsp[-1].sym)->type = LLAB;
		(yyvsp[-1].sym)->value = pc;
	}
#line 1468 "a.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 58 "a.y" /* yacc.c:1646  */
    {
		(yyvsp[-2].sym)->type = LVAR;
		(yyvsp[-2].sym)->value = (yyvsp[0].lval);
	}
#line 1477 "a.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 63 "a.y" /* yacc.c:1646  */
    {
		if((yyvsp[-2].sym)->value != (yyvsp[0].lval))
			yyerror("redeclaration of %s", (yyvsp[-2].sym)->name);
		(yyvsp[-2].sym)->value = (yyvsp[0].lval);
	}
#line 1487 "a.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 68 "a.y" /* yacc.c:1646  */
    { outcode((yyvsp[-1].lval), &(yyvsp[0].gen2)); }
#line 1493 "a.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 69 "a.y" /* yacc.c:1646  */
    { outcode((yyvsp[-1].lval), &(yyvsp[0].gen2)); }
#line 1499 "a.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 70 "a.y" /* yacc.c:1646  */
    { outcode((yyvsp[-1].lval), &(yyvsp[0].gen2)); }
#line 1505 "a.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 71 "a.y" /* yacc.c:1646  */
    { outcode((yyvsp[-1].lval), &(yyvsp[0].gen2)); }
#line 1511 "a.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 72 "a.y" /* yacc.c:1646  */
    { outcode((yyvsp[-1].lval), &(yyvsp[0].gen2)); }
#line 1517 "a.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 73 "a.y" /* yacc.c:1646  */
    { outcode((yyvsp[-1].lval), &(yyvsp[0].gen2)); }
#line 1523 "a.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 74 "a.y" /* yacc.c:1646  */
    { outcode((yyvsp[-1].lval), &(yyvsp[0].gen2)); }
#line 1529 "a.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 75 "a.y" /* yacc.c:1646  */
    { outcode((yyvsp[-1].lval), &(yyvsp[0].gen2)); }
#line 1535 "a.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 76 "a.y" /* yacc.c:1646  */
    { outcode((yyvsp[-1].lval), &(yyvsp[0].gen2)); }
#line 1541 "a.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 77 "a.y" /* yacc.c:1646  */
    { outcode((yyvsp[-1].lval), &(yyvsp[0].gen2)); }
#line 1547 "a.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 78 "a.y" /* yacc.c:1646  */
    { outcode((yyvsp[-1].lval), &(yyvsp[0].gen2)); }
#line 1553 "a.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 79 "a.y" /* yacc.c:1646  */
    { outcode((yyvsp[-1].lval), &(yyvsp[0].gen2)); }
#line 1559 "a.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 80 "a.y" /* yacc.c:1646  */
    { outcode((yyvsp[-1].lval), &(yyvsp[0].gen2)); }
#line 1565 "a.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 81 "a.y" /* yacc.c:1646  */
    { outcode((yyvsp[-1].lval), &(yyvsp[0].gen2)); }
#line 1571 "a.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 84 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = nullgen;
		(yyval.gen2).to = nullgen;
	}
#line 1580 "a.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 89 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = nullgen;
		(yyval.gen2).to = nullgen;
	}
#line 1589 "a.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 96 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = (yyvsp[-2].gen);
		(yyval.gen2).to = (yyvsp[0].gen);
	}
#line 1598 "a.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 103 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = (yyvsp[-2].gen);
		(yyval.gen2).to = (yyvsp[0].gen);
	}
#line 1607 "a.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 110 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = (yyvsp[-1].gen);
		(yyval.gen2).to = nullgen;
	}
#line 1616 "a.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 115 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = (yyvsp[0].gen);
		(yyval.gen2).to = nullgen;
	}
#line 1625 "a.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 122 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = nullgen;
		(yyval.gen2).to = (yyvsp[0].gen);
	}
#line 1634 "a.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 127 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = nullgen;
		(yyval.gen2).to = (yyvsp[0].gen);
	}
#line 1643 "a.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 134 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = nullgen;
		(yyval.gen2).to = (yyvsp[0].gen);
	}
#line 1652 "a.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 139 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = nullgen;
		(yyval.gen2).to = (yyvsp[0].gen);
	}
#line 1661 "a.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 146 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = (yyvsp[-4].gen);
		(yyval.gen2).from.scale = (yyvsp[-2].lval);
		(yyval.gen2).to = (yyvsp[0].gen);
	}
#line 1671 "a.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 154 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = (yyvsp[-2].gen);
		(yyval.gen2).to = (yyvsp[0].gen);
	}
#line 1680 "a.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 159 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = (yyvsp[-4].gen);
		(yyval.gen2).from.scale = (yyvsp[-2].lval);
		(yyval.gen2).to = (yyvsp[0].gen);
	}
#line 1690 "a.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 167 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = nullgen;
		(yyval.gen2).to = (yyvsp[0].gen);
	}
#line 1699 "a.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 172 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = nullgen;
		(yyval.gen2).to = (yyvsp[0].gen);
	}
#line 1708 "a.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 183 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = (yyvsp[-2].gen);
		(yyval.gen2).to = (yyvsp[0].gen);
	}
#line 1717 "a.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 188 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = (yyvsp[-4].gen);
		(yyval.gen2).to = (yyvsp[-2].gen);
		if((yyval.gen2).from.index != D_NONE)
			yyerror("dp shift with lhs index");
		(yyval.gen2).from.index = (yyvsp[0].lval);
	}
#line 1729 "a.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 198 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = (yyvsp[-2].gen);
		(yyval.gen2).to = (yyvsp[0].gen);
	}
#line 1738 "a.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 203 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = (yyvsp[-4].gen);
		(yyval.gen2).to = (yyvsp[-2].gen);
		if((yyval.gen2).to.index != D_NONE)
			yyerror("dp move with lhs index");
		(yyval.gen2).to.index = (yyvsp[0].lval);
	}
#line 1750 "a.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 213 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = (yyvsp[-1].gen);
		(yyval.gen2).to = nullgen;
	}
#line 1759 "a.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 218 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = (yyvsp[0].gen);
		(yyval.gen2).to = nullgen;
	}
#line 1768 "a.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 223 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = (yyvsp[-2].gen);
		(yyval.gen2).to = (yyvsp[0].gen);
	}
#line 1777 "a.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 230 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = (yyvsp[-2].gen);
		(yyval.gen2).to = (yyvsp[0].gen);
	}
#line 1786 "a.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 235 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = (yyvsp[-4].gen);
		(yyval.gen2).from.scale = (yyvsp[-2].lval);
		(yyval.gen2).to = (yyvsp[0].gen);
	}
#line 1796 "a.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 249 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = (yyvsp[0].gen);
	}
#line 1804 "a.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 253 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = (yyvsp[0].gen);
	}
#line 1812 "a.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 266 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_BRANCH;
		(yyval.gen).offset = (yyvsp[-3].lval) + pc;
	}
#line 1822 "a.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 272 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		if(pass == 2)
			yyerror("undefined label: %s", (yyvsp[-1].sym)->name);
		(yyval.gen).type = D_BRANCH;
		(yyval.gen).sym = (yyvsp[-1].sym);
		(yyval.gen).offset = (yyvsp[0].lval);
	}
#line 1835 "a.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 281 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_BRANCH;
		(yyval.gen).sym = (yyvsp[-1].sym);
		(yyval.gen).offset = (yyvsp[-1].sym)->value + (yyvsp[0].lval);
	}
#line 1846 "a.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 290 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = (yyvsp[0].lval);
	}
#line 1855 "a.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 295 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = (yyvsp[0].lval);
	}
#line 1864 "a.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 300 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = (yyvsp[0].lval);
	}
#line 1873 "a.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 305 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_SP;
	}
#line 1882 "a.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 310 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = (yyvsp[0].lval);
	}
#line 1891 "a.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 317 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_CONST;
		(yyval.gen).offset = (yyvsp[0].lval);
	}
#line 1901 "a.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 323 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = (yyvsp[0].gen);
		(yyval.gen).index = (yyvsp[0].gen).type;
		(yyval.gen).type = D_ADDR;
		/*
		if($2.type == D_AUTO || $2.type == D_PARAM)
			yyerror("constant cannot be automatic: %s",
				$2.sym->name);
		 */
	}
#line 1916 "a.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 334 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_SCONST;
		memcpy((yyval.gen).sval, (yyvsp[0].sval), sizeof((yyval.gen).sval));
	}
#line 1926 "a.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 340 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_FCONST;
		(yyval.gen).dval = (yyvsp[0].dval);
	}
#line 1936 "a.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 346 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_FCONST;
		(yyval.gen).dval = (yyvsp[-1].dval);
	}
#line 1946 "a.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 352 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_FCONST;
		(yyval.gen).dval = -(yyvsp[0].dval);
	}
#line 1956 "a.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 395 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_INDIR+D_NONE;
		(yyval.gen).offset = (yyvsp[0].lval);
	}
#line 1966 "a.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 401 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_INDIR+(yyvsp[-1].lval);
		(yyval.gen).offset = (yyvsp[-3].lval);
	}
#line 1976 "a.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 407 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_INDIR+D_SP;
		(yyval.gen).offset = (yyvsp[-3].lval);
	}
#line 1986 "a.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 413 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_INDIR+D_NONE;
		(yyval.gen).offset = (yyvsp[-5].lval);
		(yyval.gen).index = (yyvsp[-3].lval);
		(yyval.gen).scale = (yyvsp[-1].lval);
		checkscale((yyval.gen).scale);
	}
#line 1999 "a.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 422 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_INDIR+(yyvsp[-6].lval);
		(yyval.gen).offset = (yyvsp[-8].lval);
		(yyval.gen).index = (yyvsp[-3].lval);
		(yyval.gen).scale = (yyvsp[-1].lval);
		checkscale((yyval.gen).scale);
	}
#line 2012 "a.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 431 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_INDIR+(yyvsp[-1].lval);
	}
#line 2021 "a.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 436 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_INDIR+D_SP;
	}
#line 2030 "a.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 441 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_INDIR+(yyvsp[-1].lval);
		(yyval.gen).offset = (yyvsp[-3].lval);
	}
#line 2040 "a.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 447 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_INDIR+D_NONE;
		(yyval.gen).index = (yyvsp[-3].lval);
		(yyval.gen).scale = (yyvsp[-1].lval);
		checkscale((yyval.gen).scale);
	}
#line 2052 "a.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 455 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_INDIR+(yyvsp[-6].lval);
		(yyval.gen).index = (yyvsp[-3].lval);
		(yyval.gen).scale = (yyvsp[-1].lval);
		checkscale((yyval.gen).scale);
	}
#line 2064 "a.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 465 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = (yyvsp[0].gen);
	}
#line 2072 "a.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 469 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = (yyvsp[-5].gen);
		(yyval.gen).index = (yyvsp[-3].lval);
		(yyval.gen).scale = (yyvsp[-1].lval);
		checkscale((yyval.gen).scale);
	}
#line 2083 "a.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 478 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = (yyvsp[-1].lval);
		(yyval.gen).sym = (yyvsp[-4].sym);
		(yyval.gen).offset = (yyvsp[-3].lval);
	}
#line 2094 "a.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 485 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_STATIC;
		(yyval.gen).sym = (yyvsp[-6].sym);
		(yyval.gen).offset = (yyvsp[-3].lval);
	}
#line 2105 "a.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 493 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = 0;
	}
#line 2113 "a.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 497 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[0].lval);
	}
#line 2121 "a.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 501 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = -(yyvsp[0].lval);
	}
#line 2129 "a.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 508 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = D_AUTO;
	}
#line 2137 "a.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 516 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[0].sym)->value;
	}
#line 2145 "a.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 520 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = -(yyvsp[0].lval);
	}
#line 2153 "a.tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 524 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[0].lval);
	}
#line 2161 "a.tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 528 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = ~(yyvsp[0].lval);
	}
#line 2169 "a.tab.c" /* yacc.c:1646  */
    break;

  case 105:
#line 532 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-1].lval);
	}
#line 2177 "a.tab.c" /* yacc.c:1646  */
    break;

  case 107:
#line 539 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) + (yyvsp[0].lval);
	}
#line 2185 "a.tab.c" /* yacc.c:1646  */
    break;

  case 108:
#line 543 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) - (yyvsp[0].lval);
	}
#line 2193 "a.tab.c" /* yacc.c:1646  */
    break;

  case 109:
#line 547 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) * (yyvsp[0].lval);
	}
#line 2201 "a.tab.c" /* yacc.c:1646  */
    break;

  case 110:
#line 551 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) / (yyvsp[0].lval);
	}
#line 2209 "a.tab.c" /* yacc.c:1646  */
    break;

  case 111:
#line 555 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) % (yyvsp[0].lval);
	}
#line 2217 "a.tab.c" /* yacc.c:1646  */
    break;

  case 112:
#line 559 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-3].lval) << (yyvsp[0].lval);
	}
#line 2225 "a.tab.c" /* yacc.c:1646  */
    break;

  case 113:
#line 563 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-3].lval) >> (yyvsp[0].lval);
	}
#line 2233 "a.tab.c" /* yacc.c:1646  */
    break;

  case 114:
#line 567 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) & (yyvsp[0].lval);
	}
#line 2241 "a.tab.c" /* yacc.c:1646  */
    break;

  case 115:
#line 571 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) ^ (yyvsp[0].lval);
	}
#line 2249 "a.tab.c" /* yacc.c:1646  */
    break;

  case 116:
#line 575 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) | (yyvsp[0].lval);
	}
#line 2257 "a.tab.c" /* yacc.c:1646  */
    break;


#line 2261 "a.tab.c" /* yacc.c:1646  */
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
