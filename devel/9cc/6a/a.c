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
    LTYPEXC = 272,
    LTYPEX = 273,
    LTYPEY = 274,
    LTYPERT = 275,
    LCONST = 276,
    LFP = 277,
    LPC = 278,
    LSB = 279,
    LBREG = 280,
    LLREG = 281,
    LSREG = 282,
    LFREG = 283,
    LMREG = 284,
    LXREG = 285,
    LYREG = 286,
    LFCONST = 287,
    LSCONST = 288,
    LSP = 289,
    LNAME = 290,
    LLAB = 291,
    LVAR = 292
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 4 "a.y" /* yacc.c:355  */

	Sym	*sym;
	vlong	lval;
	double	dval;
	char	sval[8];
	Gen	gen;
	Gen2	gen2;

#line 157 "a.tab.c" /* yacc.c:355  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_A_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 172 "a.tab.c" /* yacc.c:358  */

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
#define YYLAST   571

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  56
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  38
/* YYNRULES -- Number of rules.  */
#define YYNRULES  131
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  264

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   292

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,    54,    12,     5,     2,
      52,    53,    10,     8,    51,     9,     2,    11,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    48,    49,
       6,    50,     7,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     4,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     3,     2,    55,     2,     2,     2,
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
      45,    46,    47
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    30,    30,    31,    35,    34,    42,    41,    47,    48,
      49,    52,    57,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    76,    77,    78,    79,
      80,    83,    87,    94,   101,   108,   113,   120,   125,   132,
     137,   144,   152,   157,   165,   170,   177,   178,   181,   186,
     196,   201,   211,   216,   221,   228,   234,   245,   253,   266,
     271,   279,   280,   294,   298,   305,   306,   309,   310,   311,
     315,   319,   320,   321,   324,   325,   328,   334,   343,   352,
     357,   362,   367,   372,   377,   382,   387,   394,   400,   411,
     417,   423,   429,   437,   438,   441,   447,   453,   459,   468,
     477,   482,   487,   493,   501,   511,   515,   524,   531,   540,
     543,   547,   553,   554,   558,   561,   562,   566,   570,   574,
     578,   584,   585,   589,   593,   597,   601,   605,   609,   613,
     617,   621
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
  "LTYPEM", "LTYPEI", "LTYPEG", "LTYPEXC", "LTYPEX", "LTYPEY", "LTYPERT",
  "LCONST", "LFP", "LPC", "LSB", "LBREG", "LLREG", "LSREG", "LFREG",
  "LMREG", "LXREG", "LYREG", "LFCONST", "LSCONST", "LSP", "LNAME", "LLAB",
  "LVAR", "':'", "';'", "'='", "','", "'('", "')'", "'$'", "'~'",
  "$accept", "prog", "line", "$@1", "$@2", "inst", "nonnon", "rimrem",
  "remrim", "rimnon", "nonrem", "nonrel", "spec1", "spec2", "spec3",
  "spec4", "spec5", "spec6", "spec7", "spec8", "spec9", "spec10", "spec11",
  "spec12", "rem", "rom", "rim", "rel", "reg", "imm", "mem", "omem",
  "nmem", "nam", "offset", "pointer", "con", "expr", YY_NULLPTR
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
     285,   286,   287,   288,   289,   290,   291,   292,    58,    59,
      61,    44,    40,    41,    36,   126
};
# endif

#define YYPACT_NINF -106

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-106)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -106,   238,  -106,   -34,     1,   363,   388,   388,   438,   298,
     -25,   413,    45,   185,   388,   388,   388,   185,   234,     8,
     388,     8,   -18,     7,    29,  -106,  -106,    34,  -106,  -106,
    -106,   497,   497,  -106,  -106,  -106,  -106,  -106,  -106,  -106,
    -106,  -106,    60,  -106,   438,   181,   497,  -106,  -106,  -106,
    -106,  -106,  -106,    32,    41,    96,  -106,  -106,    44,  -106,
    -106,    51,  -106,    52,   463,    60,    69,   338,  -106,  -106,
    -106,  -106,  -106,  -106,  -106,    59,  -106,   101,   438,  -106,
    -106,  -106,    69,    63,   497,  -106,  -106,    61,  -106,    65,
    -106,    68,  -106,    70,  -106,    71,  -106,    72,  -106,    77,
    -106,    78,  -106,  -106,    79,  -106,  -106,  -106,   497,  -106,
     497,  -106,  -106,  -106,   107,   497,   497,    81,  -106,     4,
      82,  -106,   171,  -106,    98,     3,   309,  -106,  -106,   478,
    -106,  -106,  -106,   438,   388,  -106,  -106,    81,  -106,  -106,
       5,   497,  -106,  -106,   104,   480,   438,   438,   438,   480,
     438,   438,   438,   275,   550,   275,   550,    69,  -106,  -106,
     -26,   497,    94,  -106,   497,   497,   497,   141,   143,   497,
     497,   497,   497,   497,  -106,   144,    17,   105,   106,  -106,
     112,  -106,  -106,   116,   122,  -106,   140,   147,   148,  -106,
    -106,   146,   150,   151,   152,   153,  -106,  -106,   156,  -106,
    -106,  -106,   139,   145,   169,   559,   304,   530,   497,   497,
      33,    33,  -106,  -106,  -106,   497,   497,   157,  -106,  -106,
    -106,  -106,     8,     8,   174,   177,     8,   497,   438,   234,
     438,   179,  -106,  -106,   201,   176,   176,   162,   166,   184,
    -106,  -106,  -106,  -106,  -106,  -106,   178,   180,  -106,   182,
     497,  -106,  -106,   217,   497,   234,  -106,   189,   497,  -106,
    -106,  -106,   190,  -106
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,     0,    31,     0,     0,     0,     0,     0,
       0,    31,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    63,     0,     0,     0,     8,     3,     0,    10,    32,
      13,     0,     0,   115,    79,    81,    84,    80,    82,    85,
      86,    83,   109,   116,     0,     0,     0,    14,    38,    65,
      66,    93,    94,   105,    95,     0,    15,    74,    36,    75,
      16,     0,    17,     0,     0,   109,   109,     0,    21,    45,
      67,    71,    73,    72,    68,    95,    19,     0,    32,    46,
      47,    22,   109,     0,     0,    18,    40,     0,    20,     0,
      23,     0,    24,     0,    25,    53,    28,     0,    26,     0,
      27,     0,    61,    29,     0,    30,    64,     6,     0,     4,
       0,     9,   118,   117,     0,     0,     0,     0,    37,     0,
       0,   121,     0,   119,     0,     0,     0,    90,    89,     0,
      88,    87,    35,     0,     0,    69,    70,    77,    78,    44,
       0,     0,    77,    39,     0,     0,     0,     0,    52,     0,
       0,     0,     0,     0,    11,     0,    12,   109,   110,   111,
       0,     0,   100,   101,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   120,     0,     0,     0,     0,    92,
       0,    33,    34,     0,     0,    42,     0,    48,    50,    54,
      59,     0,     0,     0,     0,    65,     7,     5,     0,   114,
     112,   113,     0,     0,     0,   131,   130,   129,     0,     0,
     122,   123,   124,   125,   126,     0,     0,    96,   102,    97,
      91,    76,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   107,   103,     0,   127,   128,     0,     0,     0,
      41,    43,    49,    51,    60,    55,     0,    57,    62,     0,
       0,   106,    98,     0,     0,     0,   108,     0,     0,    56,
      58,   104,     0,    99
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -106,  -106,  -105,  -106,  -106,  -106,   223,   224,  -106,  -106,
     235,  -106,  -106,  -106,  -106,  -106,  -106,  -106,  -106,  -106,
    -106,  -106,  -106,  -106,    20,   210,    -3,   -10,    -8,     0,
      16,    -4,    -2,     6,   -31,  -106,    -9,   -84
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    26,   155,   153,    27,    30,    60,    62,    56,
      47,    85,    76,    88,    68,    81,    90,    92,    94,    98,
     100,    96,   103,   105,    57,    69,    58,    70,    49,    59,
      50,    51,    52,    53,   117,   202,    54,   122
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint16 yytable[] =
{
      75,    71,    86,    87,    61,    73,   199,    74,   200,    72,
      99,    91,    93,    95,   161,    28,    77,   104,   201,   101,
      42,   106,   112,   113,   154,    48,   156,   216,    63,    89,
     107,    48,   108,    97,   137,   138,   121,   123,   183,   176,
     177,   176,   177,   171,   172,   173,   131,   178,   196,   178,
     197,   142,    29,    31,    32,   109,   135,   162,    75,    71,
     136,   130,    55,    73,   118,    74,   114,    72,   115,   116,
     217,    31,    32,   143,    87,   121,    33,   115,   116,   110,
     205,   206,   207,   111,   124,   210,   211,   212,   213,   214,
      82,    66,    43,   125,    33,   132,    83,    84,   118,   121,
      46,   121,   133,   134,    31,   126,   158,   159,    82,    66,
      43,   140,   141,   144,   157,    84,   145,   113,    46,   146,
     121,   147,   148,   149,   235,   236,   198,    33,   150,   151,
     152,   182,   184,   160,   175,   163,   186,   183,   127,   128,
     191,    42,   193,    43,   195,   185,   204,   208,   129,   190,
     209,    46,   203,   181,   215,   121,   121,   121,   218,   219,
     121,   121,   121,   121,   121,   220,   187,   188,   189,   221,
     192,   194,   181,   222,   164,   165,   166,   167,   168,   169,
     170,   171,   172,   173,   169,   170,   171,   172,   173,    31,
      32,   223,   232,    31,    32,   224,   225,   226,   233,   121,
     121,   227,   228,   229,   230,   234,   237,   238,   231,   239,
     242,   250,    33,   249,   243,   251,    33,   119,   245,   252,
     253,   247,   240,   241,   174,   120,   244,   258,    43,   254,
      42,   255,    43,    84,    79,   256,    46,    45,     2,     3,
      46,   257,   261,   263,   102,   259,    80,   260,   246,   262,
     248,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    34,
      35,    36,    37,    38,    39,    40,     3,   139,    41,     0,
       0,     0,     0,    22,    23,    24,     0,    25,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    31,    32,    64,   166,
     167,   168,   169,   170,   171,   172,   173,    31,    32,     0,
      22,    23,    24,     0,    25,     0,     0,     0,     0,    33,
       0,     0,     0,    34,    35,    36,    37,    38,    39,    40,
      33,     0,    41,    65,    66,    43,    31,    32,    64,    67,
      45,   179,    55,    46,     0,     0,    43,     0,     0,     0,
       0,    84,     0,     0,    46,     0,     0,     0,     0,    33,
       0,    31,    32,    34,    35,    36,    37,    38,    39,    40,
       0,     0,    41,    65,    66,    43,     0,     0,     0,     0,
      45,     0,    55,    46,    33,     0,    31,    32,    34,    35,
      36,    37,    38,    39,    40,     0,     0,    41,    42,     0,
      43,     0,     0,     0,    44,    45,     0,     0,    46,    33,
       0,    31,    32,    34,    35,    36,    37,    38,    39,    40,
       0,     0,    41,    42,     0,    43,     0,     0,     0,     0,
      45,     0,    55,    46,    33,     0,    31,    32,    34,    35,
      36,    37,    38,    39,    40,     0,     0,    41,    42,     0,
      43,     0,     0,     0,    78,    45,     0,     0,    46,    33,
       0,    31,    32,    34,    35,    36,    37,    38,    39,    40,
       0,     0,    41,    42,     0,    43,    31,    32,    31,    32,
      45,     0,     0,    46,    33,     0,     0,     0,    34,    35,
      36,    37,    38,    39,    40,    31,    32,    41,     0,    33,
      43,    33,     0,     0,     0,    45,     0,     0,    46,     0,
     180,     0,     0,     0,     0,    43,     0,    43,    33,     0,
      84,     0,    84,    46,    55,    46,   167,   168,   169,   170,
     171,   172,   173,     0,    43,     0,     0,     0,     0,    84,
       0,     0,    46,   164,   165,   166,   167,   168,   169,   170,
     171,   172,   173,   165,   166,   167,   168,   169,   170,   171,
     172,   173
};

static const yytype_int16 yycheck[] =
{
       9,     9,    12,    12,     7,     9,    32,     9,    34,     9,
      18,    14,    15,    16,    10,    49,    10,    20,    44,    19,
      45,    21,    31,    32,   108,     5,   110,    10,     8,    13,
      48,    11,    50,    17,    65,    66,    45,    46,    33,    36,
      37,    36,    37,    10,    11,    12,    55,    44,   153,    44,
     155,    82,    51,     8,     9,    48,    64,    53,    67,    67,
      64,    55,    54,    67,    44,    67,     6,    67,     8,     9,
      53,     8,     9,    83,    83,    84,    31,     8,     9,    50,
     164,   165,   166,    49,    52,   169,   170,   171,   172,   173,
      45,    46,    47,    52,    31,    51,    51,    52,    78,   108,
      55,   110,    51,    51,     8,     9,   115,   116,    45,    46,
      47,    52,    11,    52,     7,    52,    51,   126,    55,    51,
     129,    51,    51,    51,   208,   209,   157,    31,    51,    51,
      51,   134,   141,    52,    36,    53,   145,    33,    42,    43,
     149,    45,   150,    47,   152,   145,    52,     6,    52,   149,
       7,    55,   161,   133,    10,   164,   165,   166,    53,    53,
     169,   170,   171,   172,   173,    53,   146,   147,   148,    53,
     150,   151,   152,    51,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,     8,     9,    10,    11,    12,     8,
       9,    51,    53,     8,     9,    48,    48,    51,    53,   208,
     209,    51,    51,    51,    51,    36,   215,   216,    52,    52,
      36,    10,    31,    34,    37,    53,    31,    36,   227,    53,
      36,   229,   222,   223,    53,    44,   226,    10,    47,    51,
      45,    51,    47,    52,    11,    53,    55,    52,     0,     1,
      55,   250,    53,    53,    20,   254,    11,   255,   228,   258,
     230,    13,    14,    15,    16,    17,    18,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    35,
      36,    37,    38,    39,    40,    41,     1,    67,    44,    -1,
      -1,    -1,    -1,    45,    46,    47,    -1,    49,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,     8,     9,    10,     5,
       6,     7,     8,     9,    10,    11,    12,     8,     9,    -1,
      45,    46,    47,    -1,    49,    -1,    -1,    -1,    -1,    31,
      -1,    -1,    -1,    35,    36,    37,    38,    39,    40,    41,
      31,    -1,    44,    45,    46,    47,     8,     9,    10,    51,
      52,    42,    54,    55,    -1,    -1,    47,    -1,    -1,    -1,
      -1,    52,    -1,    -1,    55,    -1,    -1,    -1,    -1,    31,
      -1,     8,     9,    35,    36,    37,    38,    39,    40,    41,
      -1,    -1,    44,    45,    46,    47,    -1,    -1,    -1,    -1,
      52,    -1,    54,    55,    31,    -1,     8,     9,    35,    36,
      37,    38,    39,    40,    41,    -1,    -1,    44,    45,    -1,
      47,    -1,    -1,    -1,    51,    52,    -1,    -1,    55,    31,
      -1,     8,     9,    35,    36,    37,    38,    39,    40,    41,
      -1,    -1,    44,    45,    -1,    47,    -1,    -1,    -1,    -1,
      52,    -1,    54,    55,    31,    -1,     8,     9,    35,    36,
      37,    38,    39,    40,    41,    -1,    -1,    44,    45,    -1,
      47,    -1,    -1,    -1,    51,    52,    -1,    -1,    55,    31,
      -1,     8,     9,    35,    36,    37,    38,    39,    40,    41,
      -1,    -1,    44,    45,    -1,    47,     8,     9,     8,     9,
      52,    -1,    -1,    55,    31,    -1,    -1,    -1,    35,    36,
      37,    38,    39,    40,    41,     8,     9,    44,    -1,    31,
      47,    31,    -1,    -1,    -1,    52,    -1,    -1,    55,    -1,
      42,    -1,    -1,    -1,    -1,    47,    -1,    47,    31,    -1,
      52,    -1,    52,    55,    54,    55,     6,     7,     8,     9,
      10,    11,    12,    -1,    47,    -1,    -1,    -1,    -1,    52,
      -1,    -1,    55,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,     4,     5,     6,     7,     8,     9,    10,
      11,    12
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    57,     0,     1,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    45,    46,    47,    49,    58,    61,    49,    51,
      62,     8,     9,    31,    35,    36,    37,    38,    39,    40,
      41,    44,    45,    47,    51,    52,    55,    66,    80,    84,
      86,    87,    88,    89,    92,    54,    65,    80,    82,    85,
      63,    82,    64,    80,    10,    45,    46,    51,    70,    81,
      83,    84,    85,    87,    88,    92,    68,    89,    51,    62,
      66,    71,    45,    51,    52,    67,    83,    92,    69,    86,
      72,    82,    73,    82,    74,    82,    77,    86,    75,    84,
      76,    85,    63,    78,    82,    79,    85,    48,    50,    48,
      50,    49,    92,    92,     6,     8,     9,    90,    80,    36,
      44,    92,    93,    92,    52,    52,     9,    42,    43,    52,
      89,    92,    51,    51,    51,    84,    87,    90,    90,    81,
      52,    11,    90,    83,    52,    51,    51,    51,    51,    51,
      51,    51,    51,    60,    93,    59,    93,     7,    92,    92,
      52,    10,    53,    53,     3,     4,     5,     6,     7,     8,
       9,    10,    11,    12,    53,    36,    36,    37,    44,    42,
      42,    80,    82,    33,    92,    85,    92,    80,    80,    80,
      85,    92,    80,    84,    80,    84,    58,    58,    90,    32,
      34,    44,    91,    92,    52,    93,    93,    93,     6,     7,
      93,    93,    93,    93,    93,    10,    10,    53,    53,    53,
      53,    53,    51,    51,    48,    48,    51,    51,    51,    51,
      51,    52,    53,    53,    36,    93,    93,    92,    92,    52,
      85,    85,    36,    37,    85,    92,    80,    84,    80,    34,
      10,    53,    53,    36,    51,    51,    53,    92,    10,    92,
      84,    53,    92,    53
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    56,    57,    57,    59,    58,    60,    58,    58,    58,
      58,    61,    61,    61,    61,    61,    61,    61,    61,    61,
      61,    61,    61,    61,    61,    61,    61,    61,    61,    61,
      61,    62,    62,    63,    64,    65,    65,    66,    66,    67,
      67,    68,    69,    69,    70,    70,    71,    71,    72,    72,
      73,    73,    74,    74,    74,    75,    75,    76,    76,    77,
      77,    78,    78,    79,    79,    80,    80,    81,    81,    81,
      81,    81,    81,    81,    82,    82,    83,    83,    83,    84,
      84,    84,    84,    84,    84,    84,    84,    85,    85,    85,
      85,    85,    85,    86,    86,    87,    87,    87,    87,    87,
      87,    87,    87,    87,    87,    88,    88,    89,    89,    90,
      90,    90,    91,    91,    91,    92,    92,    92,    92,    92,
      92,    93,    93,    93,    93,    93,    93,    93,    93,    93,
      93,    93
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     0,     4,     0,     4,     1,     2,
       2,     3,     3,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     0,     1,     3,     3,     2,     1,     2,     1,     2,
       1,     5,     3,     5,     2,     1,     1,     1,     3,     5,
       3,     5,     2,     1,     3,     5,     7,     5,     7,     3,
       5,     1,     5,     0,     1,     1,     1,     1,     1,     2,
       2,     1,     1,     1,     1,     1,     4,     2,     2,     1,
       1,     1,     1,     1,     1,     1,     1,     2,     2,     2,
       2,     4,     3,     1,     1,     1,     4,     4,     6,     9,
       3,     3,     4,     5,     8,     1,     6,     5,     7,     0,
       2,     2,     1,     1,     1,     1,     1,     2,     2,     2,
       3,     1,     3,     3,     3,     3,     3,     4,     4,     3,
       3,     3
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
#line 35 "a.y" /* yacc.c:1646  */
    {
		if((yyvsp[-1].sym)->value != pc)
			yyerror("redeclaration of %s", (yyvsp[-1].sym)->name);
		(yyvsp[-1].sym)->value = pc;
	}
#line 1497 "a.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 42 "a.y" /* yacc.c:1646  */
    {
		(yyvsp[-1].sym)->type = LLAB;
		(yyvsp[-1].sym)->value = pc;
	}
#line 1506 "a.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 53 "a.y" /* yacc.c:1646  */
    {
		(yyvsp[-2].sym)->type = LVAR;
		(yyvsp[-2].sym)->value = (yyvsp[0].lval);
	}
#line 1515 "a.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 58 "a.y" /* yacc.c:1646  */
    {
		if((yyvsp[-2].sym)->value != (yyvsp[0].lval))
			yyerror("redeclaration of %s", (yyvsp[-2].sym)->name);
		(yyvsp[-2].sym)->value = (yyvsp[0].lval);
	}
#line 1525 "a.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 63 "a.y" /* yacc.c:1646  */
    { outcode((yyvsp[-1].lval), &(yyvsp[0].gen2)); }
#line 1531 "a.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 64 "a.y" /* yacc.c:1646  */
    { outcode((yyvsp[-1].lval), &(yyvsp[0].gen2)); }
#line 1537 "a.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 65 "a.y" /* yacc.c:1646  */
    { outcode((yyvsp[-1].lval), &(yyvsp[0].gen2)); }
#line 1543 "a.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 66 "a.y" /* yacc.c:1646  */
    { outcode((yyvsp[-1].lval), &(yyvsp[0].gen2)); }
#line 1549 "a.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 67 "a.y" /* yacc.c:1646  */
    { outcode((yyvsp[-1].lval), &(yyvsp[0].gen2)); }
#line 1555 "a.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 68 "a.y" /* yacc.c:1646  */
    { outcode((yyvsp[-1].lval), &(yyvsp[0].gen2)); }
#line 1561 "a.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 69 "a.y" /* yacc.c:1646  */
    { outcode((yyvsp[-1].lval), &(yyvsp[0].gen2)); }
#line 1567 "a.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 70 "a.y" /* yacc.c:1646  */
    { outcode((yyvsp[-1].lval), &(yyvsp[0].gen2)); }
#line 1573 "a.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 71 "a.y" /* yacc.c:1646  */
    { outcode((yyvsp[-1].lval), &(yyvsp[0].gen2)); }
#line 1579 "a.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 72 "a.y" /* yacc.c:1646  */
    { outcode((yyvsp[-1].lval), &(yyvsp[0].gen2)); }
#line 1585 "a.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 73 "a.y" /* yacc.c:1646  */
    { outcode((yyvsp[-1].lval), &(yyvsp[0].gen2)); }
#line 1591 "a.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 74 "a.y" /* yacc.c:1646  */
    { outcode((yyvsp[-1].lval), &(yyvsp[0].gen2)); }
#line 1597 "a.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 75 "a.y" /* yacc.c:1646  */
    { outcode((yyvsp[-1].lval), &(yyvsp[0].gen2)); }
#line 1603 "a.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 76 "a.y" /* yacc.c:1646  */
    { outcode((yyvsp[-1].lval), &(yyvsp[0].gen2)); }
#line 1609 "a.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 77 "a.y" /* yacc.c:1646  */
    { outcode((yyvsp[-1].lval), &(yyvsp[0].gen2)); }
#line 1615 "a.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 78 "a.y" /* yacc.c:1646  */
    { outcode((yyvsp[-1].lval), &(yyvsp[0].gen2)); }
#line 1621 "a.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 79 "a.y" /* yacc.c:1646  */
    { outcode((yyvsp[-1].lval), &(yyvsp[0].gen2)); }
#line 1627 "a.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 80 "a.y" /* yacc.c:1646  */
    { outcode((yyvsp[-1].lval), &(yyvsp[0].gen2)); }
#line 1633 "a.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 83 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = nullgen;
		(yyval.gen2).to = nullgen;
	}
#line 1642 "a.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 88 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = nullgen;
		(yyval.gen2).to = nullgen;
	}
#line 1651 "a.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 95 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = (yyvsp[-2].gen);
		(yyval.gen2).to = (yyvsp[0].gen);
	}
#line 1660 "a.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 102 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = (yyvsp[-2].gen);
		(yyval.gen2).to = (yyvsp[0].gen);
	}
#line 1669 "a.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 109 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = (yyvsp[-1].gen);
		(yyval.gen2).to = nullgen;
	}
#line 1678 "a.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 114 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = (yyvsp[0].gen);
		(yyval.gen2).to = nullgen;
	}
#line 1687 "a.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 121 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = nullgen;
		(yyval.gen2).to = (yyvsp[0].gen);
	}
#line 1696 "a.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 126 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = nullgen;
		(yyval.gen2).to = (yyvsp[0].gen);
	}
#line 1705 "a.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 133 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = nullgen;
		(yyval.gen2).to = (yyvsp[0].gen);
	}
#line 1714 "a.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 138 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = nullgen;
		(yyval.gen2).to = (yyvsp[0].gen);
	}
#line 1723 "a.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 145 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = (yyvsp[-4].gen);
		(yyval.gen2).from.scale = (yyvsp[-2].lval);
		(yyval.gen2).to = (yyvsp[0].gen);
	}
#line 1733 "a.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 153 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = (yyvsp[-2].gen);
		(yyval.gen2).to = (yyvsp[0].gen);
	}
#line 1742 "a.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 158 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = (yyvsp[-4].gen);
		(yyval.gen2).from.scale = (yyvsp[-2].lval);
		(yyval.gen2).to = (yyvsp[0].gen);
	}
#line 1752 "a.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 166 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = nullgen;
		(yyval.gen2).to = (yyvsp[0].gen);
	}
#line 1761 "a.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 171 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = nullgen;
		(yyval.gen2).to = (yyvsp[0].gen);
	}
#line 1770 "a.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 182 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = (yyvsp[-2].gen);
		(yyval.gen2).to = (yyvsp[0].gen);
	}
#line 1779 "a.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 187 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = (yyvsp[-4].gen);
		(yyval.gen2).to = (yyvsp[-2].gen);
		if((yyval.gen2).from.index != D_NONE)
			yyerror("dp shift with lhs index");
		(yyval.gen2).from.index = (yyvsp[0].lval);
	}
#line 1791 "a.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 197 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = (yyvsp[-2].gen);
		(yyval.gen2).to = (yyvsp[0].gen);
	}
#line 1800 "a.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 202 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = (yyvsp[-4].gen);
		(yyval.gen2).to = (yyvsp[-2].gen);
		if((yyval.gen2).to.index != D_NONE)
			yyerror("dp move with lhs index");
		(yyval.gen2).to.index = (yyvsp[0].lval);
	}
#line 1812 "a.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 212 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = (yyvsp[-1].gen);
		(yyval.gen2).to = nullgen;
	}
#line 1821 "a.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 217 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = (yyvsp[0].gen);
		(yyval.gen2).to = nullgen;
	}
#line 1830 "a.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 222 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = (yyvsp[-2].gen);
		(yyval.gen2).to = (yyvsp[0].gen);
	}
#line 1839 "a.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 229 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = (yyvsp[-4].gen);
		(yyval.gen2).to = (yyvsp[-2].gen);
		(yyval.gen2).from.offset = (yyvsp[0].lval);
	}
#line 1849 "a.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 235 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = (yyvsp[-6].gen);
		if(!isxyreg((yyvsp[-4].gen).type))
			yyerror("second source operand must be X/Y register");
		(yyval.gen2).from.index = (yyvsp[-4].gen).type;
		(yyval.gen2).to = (yyvsp[-2].gen);
		(yyval.gen2).from.offset = (yyvsp[0].lval);
	}
#line 1862 "a.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 246 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = (yyvsp[-2].gen);
		(yyval.gen2).to = (yyvsp[0].gen);
		if((yyvsp[-4].gen).type != D_CONST)
			yyerror("illegal constant");
		(yyval.gen2).to.offset = (yyvsp[-4].gen).offset;
	}
#line 1874 "a.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 254 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = (yyvsp[-4].gen);
		(yyval.gen2).to = (yyvsp[0].gen);
		if((yyvsp[-6].gen).type != D_CONST)
			yyerror("illegal constant");
		(yyval.gen2).to.offset = (yyvsp[-6].gen).offset;
		if(!isxyreg((yyvsp[-2].gen).type))
			yyerror("second source operand must be X/Y register");
		(yyval.gen2).to.index = (yyvsp[-2].gen).type;
	}
#line 1889 "a.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 267 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = (yyvsp[-2].gen);
		(yyval.gen2).to = (yyvsp[0].gen);
	}
#line 1898 "a.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 272 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = (yyvsp[-4].gen);
		(yyval.gen2).from.scale = (yyvsp[-2].lval);
		(yyval.gen2).to = (yyvsp[0].gen);
	}
#line 1908 "a.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 281 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = (yyvsp[-4].gen);
		(yyval.gen2).to = (yyvsp[0].gen);
		if(isxyreg((yyvsp[-2].gen).type)) {
			if(isxyreg((yyvsp[-4].gen).type))
				(yyval.gen2).from.index = (yyvsp[-2].gen).type;
			else if(isxyreg((yyvsp[0].gen).type))
				(yyval.gen2).to.index = (yyvsp[-2].gen).type;
		} else
			yyerror("second source operand must be X or Y register");
	}
#line 1924 "a.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 294 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = nullgen;
		(yyval.gen2).to = nullgen;
	}
#line 1933 "a.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 299 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = (yyvsp[0].gen);
		(yyval.gen2).to = nullgen;
	}
#line 1942 "a.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 312 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = (yyvsp[0].gen);
	}
#line 1950 "a.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 316 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = (yyvsp[0].gen);
	}
#line 1958 "a.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 329 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_BRANCH;
		(yyval.gen).offset = (yyvsp[-3].lval) + pc;
	}
#line 1968 "a.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 335 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		if(pass == 2)
			yyerror("undefined label: %s", (yyvsp[-1].sym)->name);
		(yyval.gen).type = D_BRANCH;
		(yyval.gen).sym = (yyvsp[-1].sym);
		(yyval.gen).offset = (yyvsp[0].lval);
	}
#line 1981 "a.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 344 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_BRANCH;
		(yyval.gen).sym = (yyvsp[-1].sym);
		(yyval.gen).offset = (yyvsp[-1].sym)->value + (yyvsp[0].lval);
	}
#line 1992 "a.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 353 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = (yyvsp[0].lval);
	}
#line 2001 "a.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 358 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = (yyvsp[0].lval);
	}
#line 2010 "a.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 363 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = (yyvsp[0].lval);
	}
#line 2019 "a.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 368 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = (yyvsp[0].lval);
	}
#line 2028 "a.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 373 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_SP;
	}
#line 2037 "a.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 378 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = (yyvsp[0].lval);
	}
#line 2046 "a.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 383 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = (yyvsp[0].lval);
	}
#line 2055 "a.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 388 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = (yyvsp[0].lval);
	}
#line 2064 "a.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 395 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_CONST;
		(yyval.gen).offset = (yyvsp[0].lval);
	}
#line 2074 "a.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 401 "a.y" /* yacc.c:1646  */
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
#line 2089 "a.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 412 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_SCONST;
		memcpy((yyval.gen).sval, (yyvsp[0].sval), sizeof((yyval.gen).sval));
	}
#line 2099 "a.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 418 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_FCONST;
		(yyval.gen).dval = (yyvsp[0].dval);
	}
#line 2109 "a.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 424 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_FCONST;
		(yyval.gen).dval = (yyvsp[-1].dval);
	}
#line 2119 "a.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 430 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_FCONST;
		(yyval.gen).dval = -(yyvsp[0].dval);
	}
#line 2129 "a.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 442 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_INDIR+D_NONE;
		(yyval.gen).offset = (yyvsp[0].lval);
	}
#line 2139 "a.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 448 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_INDIR+(yyvsp[-1].lval);
		(yyval.gen).offset = (yyvsp[-3].lval);
	}
#line 2149 "a.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 454 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_INDIR+D_SP;
		(yyval.gen).offset = (yyvsp[-3].lval);
	}
#line 2159 "a.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 460 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_INDIR+D_NONE;
		(yyval.gen).offset = (yyvsp[-5].lval);
		(yyval.gen).index = (yyvsp[-3].lval);
		(yyval.gen).scale = (yyvsp[-1].lval);
		checkscale((yyval.gen).scale);
	}
#line 2172 "a.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 469 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_INDIR+(yyvsp[-6].lval);
		(yyval.gen).offset = (yyvsp[-8].lval);
		(yyval.gen).index = (yyvsp[-3].lval);
		(yyval.gen).scale = (yyvsp[-1].lval);
		checkscale((yyval.gen).scale);
	}
#line 2185 "a.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 478 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_INDIR+(yyvsp[-1].lval);
	}
#line 2194 "a.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 483 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_INDIR+D_SP;
	}
#line 2203 "a.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 488 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_INDIR+(yyvsp[-1].lval);
		(yyval.gen).offset = (yyvsp[-3].lval);
	}
#line 2213 "a.tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 494 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_INDIR+D_NONE;
		(yyval.gen).index = (yyvsp[-3].lval);
		(yyval.gen).scale = (yyvsp[-1].lval);
		checkscale((yyval.gen).scale);
	}
#line 2225 "a.tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 502 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_INDIR+(yyvsp[-6].lval);
		(yyval.gen).index = (yyvsp[-3].lval);
		(yyval.gen).scale = (yyvsp[-1].lval);
		checkscale((yyval.gen).scale);
	}
#line 2237 "a.tab.c" /* yacc.c:1646  */
    break;

  case 105:
#line 512 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = (yyvsp[0].gen);
	}
#line 2245 "a.tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 516 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = (yyvsp[-5].gen);
		(yyval.gen).index = (yyvsp[-3].lval);
		(yyval.gen).scale = (yyvsp[-1].lval);
		checkscale((yyval.gen).scale);
	}
#line 2256 "a.tab.c" /* yacc.c:1646  */
    break;

  case 107:
#line 525 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = (yyvsp[-1].lval);
		(yyval.gen).sym = (yyvsp[-4].sym);
		(yyval.gen).offset = (yyvsp[-3].lval);
	}
#line 2267 "a.tab.c" /* yacc.c:1646  */
    break;

  case 108:
#line 532 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_STATIC;
		(yyval.gen).sym = (yyvsp[-6].sym);
		(yyval.gen).offset = (yyvsp[-3].lval);
	}
#line 2278 "a.tab.c" /* yacc.c:1646  */
    break;

  case 109:
#line 540 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = 0;
	}
#line 2286 "a.tab.c" /* yacc.c:1646  */
    break;

  case 110:
#line 544 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[0].lval);
	}
#line 2294 "a.tab.c" /* yacc.c:1646  */
    break;

  case 111:
#line 548 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = -(yyvsp[0].lval);
	}
#line 2302 "a.tab.c" /* yacc.c:1646  */
    break;

  case 113:
#line 555 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = D_AUTO;
	}
#line 2310 "a.tab.c" /* yacc.c:1646  */
    break;

  case 116:
#line 563 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[0].sym)->value;
	}
#line 2318 "a.tab.c" /* yacc.c:1646  */
    break;

  case 117:
#line 567 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = -(yyvsp[0].lval);
	}
#line 2326 "a.tab.c" /* yacc.c:1646  */
    break;

  case 118:
#line 571 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[0].lval);
	}
#line 2334 "a.tab.c" /* yacc.c:1646  */
    break;

  case 119:
#line 575 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = ~(yyvsp[0].lval);
	}
#line 2342 "a.tab.c" /* yacc.c:1646  */
    break;

  case 120:
#line 579 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-1].lval);
	}
#line 2350 "a.tab.c" /* yacc.c:1646  */
    break;

  case 122:
#line 586 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) + (yyvsp[0].lval);
	}
#line 2358 "a.tab.c" /* yacc.c:1646  */
    break;

  case 123:
#line 590 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) - (yyvsp[0].lval);
	}
#line 2366 "a.tab.c" /* yacc.c:1646  */
    break;

  case 124:
#line 594 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) * (yyvsp[0].lval);
	}
#line 2374 "a.tab.c" /* yacc.c:1646  */
    break;

  case 125:
#line 598 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) / (yyvsp[0].lval);
	}
#line 2382 "a.tab.c" /* yacc.c:1646  */
    break;

  case 126:
#line 602 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) % (yyvsp[0].lval);
	}
#line 2390 "a.tab.c" /* yacc.c:1646  */
    break;

  case 127:
#line 606 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-3].lval) << (yyvsp[0].lval);
	}
#line 2398 "a.tab.c" /* yacc.c:1646  */
    break;

  case 128:
#line 610 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-3].lval) >> (yyvsp[0].lval);
	}
#line 2406 "a.tab.c" /* yacc.c:1646  */
    break;

  case 129:
#line 614 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) & (yyvsp[0].lval);
	}
#line 2414 "a.tab.c" /* yacc.c:1646  */
    break;

  case 130:
#line 618 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) ^ (yyvsp[0].lval);
	}
#line 2422 "a.tab.c" /* yacc.c:1646  */
    break;

  case 131:
#line 622 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) | (yyvsp[0].lval);
	}
#line 2430 "a.tab.c" /* yacc.c:1646  */
    break;


#line 2434 "a.tab.c" /* yacc.c:1646  */
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
