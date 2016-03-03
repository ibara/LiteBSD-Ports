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
    LCONST = 269,
    LSP = 270,
    LSB = 271,
    LFP = 272,
    LPC = 273,
    LTOS = 274,
    LAREG = 275,
    LDREG = 276,
    LFREG = 277,
    LWID = 278,
    LFCONST = 279,
    LSCONST = 280,
    LNAME = 281,
    LLAB = 282,
    LVAR = 283
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
	double	dval;
	char	sval[8];
	Addr	addr;
	Gen	gen;
	Gen2	gen2;

#line 149 "a.tab.c" /* yacc.c:355  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_A_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 164 "a.tab.c" /* yacc.c:358  */

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
#define YYLAST   399

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  47
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  27
/* YYNRULES -- Number of rules.  */
#define YYNRULES  98
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  199

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   283

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,    45,    12,     5,     2,
      43,    44,    10,     8,    42,     9,     2,    11,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    39,    40,
       6,    41,     7,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     4,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     3,     2,    46,     2,     2,     2,
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
      35,    36,    37,    38
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    30,    30,    31,    35,    34,    42,    41,    47,    48,
      49,    52,    57,    63,    64,    65,    66,    67,    68,    69,
      70,    71,    72,    73,    74,    75,    78,    82,    89,    94,
     101,   106,   113,   120,   125,   132,   139,   147,   156,   157,
     165,   171,   180,   189,   194,   200,   213,   219,   225,   231,
     237,   243,   249,   254,   259,   274,   286,   299,   312,   325,
     338,   351,   366,   367,   380,   388,   389,   390,   393,   418,
     424,   430,   436,   442,   445,   451,   459,   462,   466,   472,
     473,   474,   477,   478,   482,   486,   490,   494,   500,   501,
     505,   509,   513,   517,   521,   525,   529,   533,   537
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
  "LCONST", "LSP", "LSB", "LFP", "LPC", "LTOS", "LAREG", "LDREG", "LFREG",
  "LWID", "LFCONST", "LSCONST", "LNAME", "LLAB", "LVAR", "':'", "';'",
  "'='", "','", "'('", "')'", "'$'", "'~'", "$accept", "prog", "line",
  "$@1", "$@2", "inst", "noaddr", "srcgen", "dstgen", "gengen", "dstrel",
  "genrel", "spec1", "spec2", "spec3", "rel", "gen", "type", "xreg", "reg",
  "scale", "areg", "name", "offset", "pointer", "con", "expr", YY_NULLPTR
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
     275,   276,   277,   278,   279,   280,   281,   282,   283,    58,
      59,    61,    44,    40,    41,    36,   126
};
# endif

#define YYPACT_NINF -87

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-87)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     -87,   116,   -87,   -19,   242,     1,   192,   242,   242,    43,
     242,   192,   217,   242,   242,    14,    19,    20,   -87,   -87,
      24,   -87,    11,   161,   -87,     9,   -87,   -87,   -87,   173,
     -87,   259,   282,    11,   -87,    27,   -87,   -87,    35,   -87,
      41,   -87,   -87,   242,   -87,   -87,   -87,    81,   -87,    76,
      11,    38,    38,   302,    11,   -87,   -87,   112,   -87,   138,
     -87,   -87,    27,   242,   -87,   -87,   -87,   141,   -87,   -87,
     142,   -87,    11,   -87,    11,   -87,   -87,   333,   -87,    11,
      11,   150,    11,    11,   143,   158,   322,   -87,    32,   160,
      22,   335,   -87,   -87,   -87,   -87,   -87,   242,     7,   -87,
     291,   -87,    11,   -87,   -87,   -87,   -87,   -87,   177,   302,
     242,   242,   175,   379,   175,   379,   164,   -87,   -87,    38,
     -87,   -87,   115,   198,   165,    35,   166,    12,    11,    11,
      11,   208,   210,    11,    11,    11,    11,    11,   -87,   -87,
     -87,   -87,   185,   176,   -87,    44,   160,   187,   183,   -87,
     189,    72,   -87,   -87,   -87,   190,   -87,   -87,   -87,   188,
     -87,   -87,   -87,   -87,   156,   387,   139,    11,    11,   162,
     162,   -87,   -87,   -87,   209,   -87,    35,   195,   242,   -87,
      11,   242,   214,   -87,    87,    87,    11,   199,   -87,   -87,
     -87,   194,   -87,   200,   -87,   -87,    11,   -87,   -87
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,     0,     0,    26,     0,     0,     0,     0,
       0,     0,    26,     0,     0,     0,     0,     0,     8,     3,
       0,    10,     0,     0,    82,    67,    65,    66,    63,    76,
      83,     0,     0,     0,    13,     0,    43,    62,    54,    73,
      51,    27,    14,     0,    15,    30,    16,     0,    17,    28,
       0,    76,    76,     0,     0,    18,    33,     0,    19,     0,
      20,    21,    30,    27,    22,    23,    24,     0,    38,    25,
       0,     6,     0,     4,     0,     9,    85,     0,    84,     0,
       0,     0,     0,     0,     0,     0,     0,    71,     0,    88,
       0,     0,    47,    46,    45,    44,    86,     0,     0,    55,
       0,    31,     0,    29,    41,    42,    34,    88,     0,     0,
       0,     0,     0,    11,     0,    12,     0,    49,    50,    76,
      77,    78,     0,    69,     0,    58,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    87,    48,
      32,    67,     0,     0,    72,     0,     0,     0,     0,    35,
       0,    51,     7,     5,    52,     0,    80,    79,    81,     0,
      53,    69,    56,    60,    98,    97,    96,     0,     0,    89,
      90,    91,    92,    93,     0,    70,    59,     0,     0,    40,
       0,     0,     0,    74,    94,    95,     0,     0,    57,    61,
      36,     0,    39,     0,    68,    64,     0,    75,    37
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -87,   -87,   -11,   -87,   -87,   -87,   230,   -87,   111,    -2,
     -87,   -87,   -87,   -87,   -87,   -47,    -3,   -87,   -86,   147,
     -87,   -28,   220,   -36,   -87,    -9,   -24
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    19,   114,   112,    20,    42,    48,    44,    34,
      55,    58,    46,    66,    69,    56,    45,    36,    99,    37,
     187,    38,    39,    84,   159,    40,    90
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      57,    35,   126,    88,    47,    49,   106,    59,    62,    61,
      67,    70,    68,    76,    78,   104,   105,    79,    80,    22,
      50,    21,    89,    95,    96,   128,   129,   130,   131,   132,
     133,   134,   135,   136,   137,    24,   141,    26,    27,   162,
     101,    78,   143,    41,    57,   107,    82,    83,   113,    30,
     115,    22,    50,    71,    54,    72,   144,    33,    73,   177,
     101,    74,   149,   107,    75,   107,   138,    24,   107,    97,
     117,   118,   145,   120,   121,    98,   125,   107,    98,    51,
      52,    30,    78,   155,   100,    53,    54,    98,   176,    33,
     188,   146,   102,   147,   140,   133,   134,   135,   136,   137,
      57,   152,   151,   153,   164,   165,   166,   150,   140,   169,
     170,   171,   172,   173,   181,   100,     2,     3,   103,   107,
     107,   107,    60,    65,   107,   107,   107,   107,   107,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
     156,   157,   158,   184,   185,   131,   132,   133,   134,   135,
     136,   137,    15,    16,    17,   108,    18,   119,   107,   107,
     129,   130,   131,   132,   133,   134,   135,   136,   137,    22,
      50,   191,   135,   136,   137,   190,     3,   194,   192,    81,
     109,    82,    83,   110,   111,    24,   122,   198,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    30,
      22,    23,   123,   127,    77,   148,   160,    33,   154,   161,
     163,    15,    16,    17,   167,    18,    24,   168,   174,   186,
     175,    25,    26,    27,    28,    22,    23,   179,    29,   178,
      30,   180,   183,   182,    43,    31,   196,    32,    33,   189,
     193,    24,    64,   195,   197,   142,    25,    26,    27,    28,
      22,    23,    94,    29,     0,    30,     0,     0,     0,    63,
      31,     0,    32,    33,     0,     0,    24,    22,    50,     0,
       0,    25,    26,    27,    28,     0,     0,     0,    29,     0,
      30,     0,     0,    24,     0,    31,     0,    32,    33,    85,
      22,    91,     0,     0,     0,    29,     0,    30,     0,    22,
      50,     0,    86,    87,     0,    33,    24,     0,     0,     0,
      22,    50,     0,     0,     0,    24,    92,    93,    29,     0,
      30,   143,     0,     0,     0,    54,    24,    29,    33,    30,
      22,    50,     0,     0,    86,   144,     0,    33,    51,    52,
      30,    22,    50,    22,    50,    54,    24,     0,    33,     0,
       0,     0,   124,     0,     0,     0,     0,    24,     0,    24,
      30,     0,     0,   116,     0,    54,    87,     0,    33,   139,
       0,    30,     0,    30,     0,     0,    54,     0,    54,    33,
       0,    33,   128,   129,   130,   131,   132,   133,   134,   135,
     136,   137,   130,   131,   132,   133,   134,   135,   136,   137
};

static const yytype_int16 yycheck[] =
{
       9,     4,    88,    31,     7,     8,    53,    10,    11,    11,
      13,    14,    14,    22,    23,    51,    52,     8,     9,     8,
       9,    40,    31,    32,    33,     3,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    24,    29,    30,    31,   125,
      43,    50,    30,    42,    53,    54,     8,     9,    72,    38,
      74,     8,     9,    39,    43,    41,    44,    46,    39,   145,
      63,    41,   109,    72,    40,    74,    44,    24,    77,    42,
      79,    80,   100,    82,    83,    43,    44,    86,    43,    36,
      37,    38,    91,   119,    43,    42,    43,    43,    44,    46,
     176,   100,    11,   102,    97,     8,     9,    10,    11,    12,
     109,   112,   111,   114,   128,   129,   130,   110,   111,   133,
     134,   135,   136,   137,    42,    43,     0,     1,    42,   128,
     129,   130,    11,    12,   133,   134,   135,   136,   137,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      25,    26,    27,   167,   168,     6,     7,     8,     9,    10,
      11,    12,    36,    37,    38,    43,    40,     7,   167,   168,
       4,     5,     6,     7,     8,     9,    10,    11,    12,     8,
       9,   180,    10,    11,    12,   178,     1,   186,   181,     6,
      42,     8,     9,    42,    42,    24,    43,   196,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    38,
       8,     9,    44,    43,    43,    28,     8,    46,    44,    44,
      44,    36,    37,    38,     6,    40,    24,     7,    33,    10,
      44,    29,    30,    31,    32,     8,     9,    44,    36,    42,
      38,    42,    44,    43,    42,    43,    42,    45,    46,    44,
      26,    24,    12,    44,    44,    98,    29,    30,    31,    32,
       8,     9,    32,    36,    -1,    38,    -1,    -1,    -1,    42,
      43,    -1,    45,    46,    -1,    -1,    24,     8,     9,    -1,
      -1,    29,    30,    31,    32,    -1,    -1,    -1,    36,    -1,
      38,    -1,    -1,    24,    -1,    43,    -1,    45,    46,    30,
       8,     9,    -1,    -1,    -1,    36,    -1,    38,    -1,     8,
       9,    -1,    43,    44,    -1,    46,    24,    -1,    -1,    -1,
       8,     9,    -1,    -1,    -1,    24,    34,    35,    36,    -1,
      38,    30,    -1,    -1,    -1,    43,    24,    36,    46,    38,
       8,     9,    -1,    -1,    43,    44,    -1,    46,    36,    37,
      38,     8,     9,     8,     9,    43,    24,    -1,    46,    -1,
      -1,    -1,    30,    -1,    -1,    -1,    -1,    24,    -1,    24,
      38,    -1,    -1,    30,    -1,    43,    44,    -1,    46,    34,
      -1,    38,    -1,    38,    -1,    -1,    43,    -1,    43,    46,
      -1,    46,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,     5,     6,     7,     8,     9,    10,    11,    12
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    48,     0,     1,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    36,    37,    38,    40,    49,
      52,    40,     8,     9,    24,    29,    30,    31,    32,    36,
      38,    43,    45,    46,    56,    63,    64,    66,    68,    69,
      72,    42,    53,    42,    55,    63,    59,    63,    54,    63,
       9,    36,    37,    42,    43,    57,    62,    72,    58,    63,
      55,    56,    63,    42,    53,    55,    60,    63,    56,    61,
      63,    39,    41,    39,    41,    40,    72,    43,    72,     8,
       9,     6,     8,     9,    70,    30,    43,    44,    68,    72,
      73,     9,    34,    35,    69,    72,    72,    42,    43,    65,
      43,    63,    11,    42,    70,    70,    62,    72,    43,    42,
      42,    42,    51,    73,    50,    73,    30,    72,    72,     7,
      72,    72,    43,    44,    30,    44,    65,    43,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    44,    34,
      63,    29,    66,    30,    44,    68,    72,    72,    28,    62,
      63,    72,    49,    49,    44,    70,    25,    26,    27,    71,
       8,    44,    65,    44,    73,    73,    73,     6,     7,    73,
      73,    73,    73,    73,    33,    44,    44,    65,    42,    44,
      42,    42,    43,    44,    73,    73,    10,    67,    65,    44,
      63,    72,    63,    26,    72,    44,    42,    44,    72
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    47,    48,    48,    50,    49,    51,    49,    49,    49,
      49,    52,    52,    52,    52,    52,    52,    52,    52,    52,
      52,    52,    52,    52,    52,    52,    53,    53,    54,    54,
      55,    55,    56,    57,    57,    58,    59,    60,    61,    61,
      62,    62,    62,    63,    63,    63,    63,    63,    63,    63,
      63,    63,    63,    63,    63,    63,    63,    63,    63,    63,
      63,    63,    64,    64,    65,    66,    66,    66,    67,    68,
      68,    68,    68,    68,    69,    69,    70,    70,    70,    71,
      71,    71,    72,    72,    72,    72,    72,    72,    73,    73,
      73,    73,    73,    73,    73,    73,    73,    73,    73
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     0,     4,     0,     4,     1,     2,
       2,     3,     3,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     0,     1,     1,     2,
       1,     2,     3,     1,     2,     3,     5,     7,     1,     5,
       4,     2,     2,     1,     2,     2,     2,     2,     3,     3,
       3,     1,     4,     4,     1,     2,     4,     5,     3,     4,
       4,     5,     1,     1,     5,     1,     1,     1,     2,     3,
       4,     2,     3,     1,     5,     7,     0,     2,     2,     1,
       1,     1,     1,     1,     2,     2,     2,     3,     1,     3,
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
#line 35 "a.y" /* yacc.c:1646  */
    {
		if((yyvsp[-1].sym)->value != pc)
			yyerror("redeclaration of %s", (yyvsp[-1].sym)->name);
		(yyvsp[-1].sym)->value = pc;
	}
#line 1413 "a.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 42 "a.y" /* yacc.c:1646  */
    {
		(yyvsp[-1].sym)->type = LLAB;
		(yyvsp[-1].sym)->value = pc;
	}
#line 1422 "a.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 53 "a.y" /* yacc.c:1646  */
    {
		(yyvsp[-2].sym)->type = LVAR;
		(yyvsp[-2].sym)->value = (yyvsp[0].lval);
	}
#line 1431 "a.tab.c" /* yacc.c:1646  */
    break;

  case 12:
#line 58 "a.y" /* yacc.c:1646  */
    {
		if((yyvsp[-2].sym)->value != (yyvsp[0].lval))
			yyerror("redeclaration of %s", (yyvsp[-2].sym)->name);
		(yyvsp[-2].sym)->value = (yyvsp[0].lval);
	}
#line 1441 "a.tab.c" /* yacc.c:1646  */
    break;

  case 13:
#line 63 "a.y" /* yacc.c:1646  */
    { outcode((yyvsp[-1].lval), &(yyvsp[0].gen2)); }
#line 1447 "a.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 64 "a.y" /* yacc.c:1646  */
    { outcode((yyvsp[-1].lval), &(yyvsp[0].gen2)); }
#line 1453 "a.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 65 "a.y" /* yacc.c:1646  */
    { outcode((yyvsp[-1].lval), &(yyvsp[0].gen2)); }
#line 1459 "a.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 66 "a.y" /* yacc.c:1646  */
    { outcode((yyvsp[-1].lval), &(yyvsp[0].gen2)); }
#line 1465 "a.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 67 "a.y" /* yacc.c:1646  */
    { outcode((yyvsp[-1].lval), &(yyvsp[0].gen2)); }
#line 1471 "a.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 68 "a.y" /* yacc.c:1646  */
    { outcode((yyvsp[-1].lval), &(yyvsp[0].gen2)); }
#line 1477 "a.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 69 "a.y" /* yacc.c:1646  */
    { outcode((yyvsp[-1].lval), &(yyvsp[0].gen2)); }
#line 1483 "a.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 70 "a.y" /* yacc.c:1646  */
    { outcode((yyvsp[-1].lval), &(yyvsp[0].gen2)); }
#line 1489 "a.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 71 "a.y" /* yacc.c:1646  */
    { outcode((yyvsp[-1].lval), &(yyvsp[0].gen2)); }
#line 1495 "a.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 72 "a.y" /* yacc.c:1646  */
    { outcode((yyvsp[-1].lval), &(yyvsp[0].gen2)); }
#line 1501 "a.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 73 "a.y" /* yacc.c:1646  */
    { outcode((yyvsp[-1].lval), &(yyvsp[0].gen2)); }
#line 1507 "a.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 74 "a.y" /* yacc.c:1646  */
    { outcode((yyvsp[-1].lval), &(yyvsp[0].gen2)); }
#line 1513 "a.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 75 "a.y" /* yacc.c:1646  */
    { outcode((yyvsp[-1].lval), &(yyvsp[0].gen2)); }
#line 1519 "a.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 78 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = nullgen;
		(yyval.gen2).to = nullgen;
	}
#line 1528 "a.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 83 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = nullgen;
		(yyval.gen2).to = nullgen;
	}
#line 1537 "a.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 90 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = (yyvsp[0].gen);
		(yyval.gen2).to = nullgen;
	}
#line 1546 "a.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 95 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = (yyvsp[-1].gen);
		(yyval.gen2).to = nullgen;
	}
#line 1555 "a.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 102 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = nullgen;
		(yyval.gen2).to = (yyvsp[0].gen);
	}
#line 1564 "a.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 107 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = nullgen;
		(yyval.gen2).to = (yyvsp[0].gen);
	}
#line 1573 "a.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 114 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = (yyvsp[-2].gen);
		(yyval.gen2).to = (yyvsp[0].gen);
	}
#line 1582 "a.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 121 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = nullgen;
		(yyval.gen2).to = (yyvsp[0].gen);
	}
#line 1591 "a.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 126 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = nullgen;
		(yyval.gen2).to = (yyvsp[0].gen);
	}
#line 1600 "a.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 133 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen2).from = (yyvsp[-2].gen);
		(yyval.gen2).to = (yyvsp[0].gen);
	}
#line 1609 "a.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 140 "a.y" /* yacc.c:1646  */
    {
		(yyvsp[-4].gen).displace = (yyvsp[-2].lval);
		(yyval.gen2).from = (yyvsp[-4].gen);
		(yyval.gen2).to = (yyvsp[0].gen);
	}
#line 1619 "a.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 148 "a.y" /* yacc.c:1646  */
    {
		(yyvsp[-6].gen).field = (yyvsp[0].lval);
		(yyvsp[-4].gen).field = (yyvsp[-2].lval);
		(yyval.gen2).from = (yyvsp[-6].gen);
		(yyval.gen2).to = (yyvsp[-4].gen);
	}
#line 1630 "a.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 158 "a.y" /* yacc.c:1646  */
    {
		(yyvsp[-4].gen).displace = (yyvsp[-2].lval);
		(yyval.gen2).from = (yyvsp[-4].gen);
		(yyval.gen2).to = (yyvsp[0].gen);
	}
#line 1640 "a.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 166 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_BRANCH;
		(yyval.gen).s0.offset = (yyvsp[-3].lval) + pc;
	}
#line 1650 "a.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 172 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		if(pass == 2)
			yyerror("undefined label: %s", (yyvsp[-1].sym)->name);
		(yyval.gen).type = D_BRANCH;
		(yyval.gen).s0.sym = (yyvsp[-1].sym);
		(yyval.gen).s0.offset = (yyvsp[0].lval);
	}
#line 1663 "a.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 181 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_BRANCH;
		(yyval.gen).s0.sym = (yyvsp[-1].sym);
		(yyval.gen).s0.offset = (yyvsp[-1].sym)->value + (yyvsp[0].lval);
	}
#line 1674 "a.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 190 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = (yyvsp[0].lval);
	}
#line 1683 "a.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 195 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_CONST;
		(yyval.gen).s0.offset = (yyvsp[0].lval);
	}
#line 1693 "a.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 201 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		{
			Addr *a;
			a = &(yyval.gen).s0;
			*a = (yyvsp[0].addr);
		}
		if((yyvsp[0].addr).type == D_AUTO || (yyvsp[0].addr).type == D_PARAM)
			yyerror("constant cannot be automatic: %s",
				(yyvsp[0].addr).sym->name);
		(yyval.gen).type = (yyvsp[0].addr).type | I_ADDR;
	}
#line 1710 "a.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 214 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_SCONST;
		memcpy((yyval.gen).sval, (yyvsp[0].sval), sizeof((yyval.gen).sval));
	}
#line 1720 "a.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 220 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_FCONST;
		(yyval.gen).dval = (yyvsp[0].dval);
	}
#line 1730 "a.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 226 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_FCONST;
		(yyval.gen).dval = -(yyvsp[0].dval);
	}
#line 1740 "a.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 232 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_STACK;
		(yyval.gen).s0.offset = (yyvsp[0].lval);
	}
#line 1750 "a.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 238 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_STACK;
		(yyval.gen).s0.offset = -(yyvsp[0].lval);
	}
#line 1760 "a.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 244 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_CONST | I_INDIR;
		(yyval.gen).s0.offset = (yyvsp[0].lval);
	}
#line 1770 "a.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 250 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = (yyvsp[-1].lval) | I_INDDEC;
	}
#line 1779 "a.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 255 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = (yyvsp[-2].lval) | I_INDINC;
	}
#line 1788 "a.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 260 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = (yyvsp[0].addr).type;
		{
			Addr *a;
			a = &(yyval.gen).s0;
			*a = (yyvsp[0].addr);
		}
		if(((yyval.gen).type & D_MASK) == D_NONE) {
			(yyval.gen).index = D_NONE | I_INDEX1;
			(yyval.gen).scale = 0;
			(yyval.gen).displace = 0;
		}
	}
#line 1807 "a.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 275 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = (yyvsp[-1].addr).type;
		{
			Addr *a;
			a = &(yyval.gen).s0;
			*a = (yyvsp[-1].addr);
		}
		(yyval.gen).index = (yyvsp[0].addr).type | I_INDEX1;
		(yyval.gen).scale = (yyvsp[0].addr).offset;
	}
#line 1823 "a.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 287 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = (yyvsp[-2].addr).type;
		{
			Addr *a;
			a = &(yyval.gen).s0;
			*a = (yyvsp[-2].addr);
		}
		(yyval.gen).index = (yyvsp[0].addr).type | I_INDEX2;
		(yyval.gen).scale = (yyvsp[0].addr).offset;
		(yyval.gen).displace = 0;
	}
#line 1840 "a.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 300 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = (yyvsp[-2].addr).type;
		{
			Addr *a;
			a = &(yyval.gen).s0;
			*a = (yyvsp[-2].addr);
		}
		(yyval.gen).index = (yyvsp[0].addr).type | I_INDEX2;
		(yyval.gen).scale = (yyvsp[0].addr).offset;
		(yyval.gen).displace = (yyvsp[-4].lval);
	}
#line 1857 "a.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 313 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = (yyvsp[-1].addr).type;
		{
			Addr *a;
			a = &(yyval.gen).s0;
			*a = (yyvsp[-1].addr);
		}
		(yyval.gen).index = D_NONE | I_INDEX3;
		(yyval.gen).scale = 0;
		(yyval.gen).displace = 0;
	}
#line 1874 "a.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 326 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = (yyvsp[-1].addr).type;
		{
			Addr *a;
			a = &(yyval.gen).s0;
			*a = (yyvsp[-1].addr);
		}
		(yyval.gen).index = D_NONE | I_INDEX3;
		(yyval.gen).scale = 0;
		(yyval.gen).displace = (yyvsp[-3].lval);
	}
#line 1891 "a.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 339 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = (yyvsp[-2].addr).type;
		{
			Addr *a;
			a = &(yyval.gen).s0;
			*a = (yyvsp[-2].addr);
		}
		(yyval.gen).index = (yyvsp[-1].addr).type | I_INDEX3;
		(yyval.gen).scale = (yyvsp[-1].addr).offset;
		(yyval.gen).displace = 0;
	}
#line 1908 "a.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 352 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = (yyvsp[-2].addr).type;
		{
			Addr *a;
			a = &(yyval.gen).s0;
			*a = (yyvsp[-2].addr);
		}
		(yyval.gen).index = (yyvsp[-1].addr).type | I_INDEX3;
		(yyval.gen).scale = (yyvsp[-1].addr).offset;
		(yyval.gen).displace = (yyvsp[-4].lval);
	}
#line 1925 "a.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 381 "a.y" /* yacc.c:1646  */
    {
		(yyval.addr).type = (yyvsp[-3].lval);
		(yyval.addr).offset = (yyvsp[-2].lval)+(yyvsp[-1].lval);
		(yyval.addr).sym = S;
	}
#line 1935 "a.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 394 "a.y" /* yacc.c:1646  */
    {
		switch((yyvsp[0].lval)) {
		case 1:
			(yyval.lval) = 0;
			break;

		case 2:
			(yyval.lval) = 1;
			break;

		default:
			yyerror("bad scale: %ld", (yyvsp[0].lval));

		case 4:
			(yyval.lval) = 2;
			break;

		case 8:
			(yyval.lval) = 3;
			break;
		}
	}
#line 1962 "a.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 419 "a.y" /* yacc.c:1646  */
    {
		(yyval.addr).type = (yyvsp[-1].lval) | I_INDIR;
		(yyval.addr).sym = S;
		(yyval.addr).offset = 0;
	}
#line 1972 "a.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 425 "a.y" /* yacc.c:1646  */
    {
		(yyval.addr).type = (yyvsp[-1].lval) | I_INDIR;
		(yyval.addr).sym = S;
		(yyval.addr).offset = (yyvsp[-3].lval);
	}
#line 1982 "a.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 431 "a.y" /* yacc.c:1646  */
    {
		(yyval.addr).type = D_NONE | I_INDIR;
		(yyval.addr).sym = S;
		(yyval.addr).offset = 0;
	}
#line 1992 "a.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 437 "a.y" /* yacc.c:1646  */
    {
		(yyval.addr).type = D_NONE | I_INDIR;
		(yyval.addr).sym = S;
		(yyval.addr).offset = (yyvsp[-2].lval);
	}
#line 2002 "a.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 446 "a.y" /* yacc.c:1646  */
    {
		(yyval.addr).type = (yyvsp[-1].lval);
		(yyval.addr).sym = (yyvsp[-4].sym);
		(yyval.addr).offset = (yyvsp[-3].lval);
	}
#line 2012 "a.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 452 "a.y" /* yacc.c:1646  */
    {
		(yyval.addr).type = D_STATIC;
		(yyval.addr).sym = (yyvsp[-6].sym);
		(yyval.addr).offset = (yyvsp[-3].lval);
	}
#line 2022 "a.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 459 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = 0;
	}
#line 2030 "a.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 463 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[0].lval);
	}
#line 2038 "a.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 467 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = -(yyvsp[0].lval);
	}
#line 2046 "a.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 479 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[0].sym)->value;
	}
#line 2054 "a.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 483 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = -(yyvsp[0].lval);
	}
#line 2062 "a.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 487 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[0].lval);
	}
#line 2070 "a.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 491 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = ~(yyvsp[0].lval);
	}
#line 2078 "a.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 495 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-1].lval);
	}
#line 2086 "a.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 502 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) + (yyvsp[0].lval);
	}
#line 2094 "a.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 506 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) - (yyvsp[0].lval);
	}
#line 2102 "a.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 510 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) * (yyvsp[0].lval);
	}
#line 2110 "a.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 514 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) / (yyvsp[0].lval);
	}
#line 2118 "a.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 518 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) % (yyvsp[0].lval);
	}
#line 2126 "a.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 522 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-3].lval) << (yyvsp[0].lval);
	}
#line 2134 "a.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 526 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-3].lval) >> (yyvsp[0].lval);
	}
#line 2142 "a.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 530 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) & (yyvsp[0].lval);
	}
#line 2150 "a.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 534 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) ^ (yyvsp[0].lval);
	}
#line 2158 "a.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 538 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) | (yyvsp[0].lval);
	}
#line 2166 "a.tab.c" /* yacc.c:1646  */
    break;


#line 2170 "a.tab.c" /* yacc.c:1646  */
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
