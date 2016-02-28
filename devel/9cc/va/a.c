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
    LCONST = 278,
    LSP = 279,
    LSB = 280,
    LFP = 281,
    LPC = 282,
    LHI = 283,
    LLO = 284,
    LMREG = 285,
    LTYPEX = 286,
    LREG = 287,
    LFREG = 288,
    LFCREG = 289,
    LR = 290,
    LM = 291,
    LF = 292,
    LFCR = 293,
    LSCHED = 294,
    LFCONST = 295,
    LSCONST = 296,
    LNAME = 297,
    LLAB = 298,
    LVAR = 299
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE YYSTYPE;
union YYSTYPE
{
#line 5 "a.y" /* yacc.c:355  */

	Sym	*sym;
	vlong	lval;
	double	dval;
	char	sval[8];
	Gen	gen;

#line 160 "a.tab.c" /* yacc.c:355  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);



/* Copy the second part of user declarations.  */

#line 175 "a.tab.c" /* yacc.c:358  */

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
#define YYLAST   598

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  63
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  29
/* YYNRULES -- Number of rules.  */
#define YYNRULES  119
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  266

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   299

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,    61,    12,     5,     2,
      59,    60,    10,     8,    58,     9,     2,    11,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    55,    57,
       6,    56,     7,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     4,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     3,     2,    62,     2,     2,     2,
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
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    32,    32,    33,    37,    36,    44,    43,    49,    54,
      60,    64,    65,    66,    72,    76,    83,    87,    94,   103,
     110,   119,   123,   130,   134,   138,   142,   146,   153,   159,
     168,   177,   181,   188,   195,   199,   203,   207,   214,   221,
     228,   232,   236,   243,   247,   252,   253,   256,   262,   271,
     280,   281,   282,   283,   284,   289,   296,   297,   298,   299,
     300,   305,   312,   313,   316,   319,   325,   333,   339,   347,
     353,   360,   366,   371,   376,   382,   388,   396,   397,   403,
     411,   420,   421,   427,   430,   431,   437,   444,   453,   454,
     456,   464,   472,   473,   481,   489,   497,   507,   510,   514,
     520,   521,   522,   525,   526,   530,   534,   538,   542,   548,
     549,   553,   557,   561,   565,   569,   573,   577,   581,   585
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
  "LTYPEJ", "LTYPEK", "LCONST", "LSP", "LSB", "LFP", "LPC", "LHI", "LLO",
  "LMREG", "LTYPEX", "LREG", "LFREG", "LFCREG", "LR", "LM", "LF", "LFCR",
  "LSCHED", "LFCONST", "LSCONST", "LNAME", "LLAB", "LVAR", "':'", "'='",
  "';'", "','", "'('", "')'", "'$'", "'~'", "$accept", "prog", "line",
  "$@1", "$@2", "inst", "comma", "rel", "vlgen", "vgen", "lgen", "fgen",
  "mreg", "fcreg", "freg", "ximm", "nireg", "ireg", "gen", "oreg", "imr",
  "imm", "reg", "sreg", "name", "offset", "pointer", "con", "expr", YY_NULLPTR
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
     295,   296,   297,   298,   299,    58,    61,    59,    44,    40,
      41,    36,   126
};
# endif

#define YYPACT_NINF -134

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-134)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -134,   173,  -134,   -37,   -13,   -13,   396,   -27,   200,    33,
     -27,    22,   438,   438,   469,   469,   -22,   -22,   -22,   -27,
     -27,   300,   327,    -8,     5,   -12,    12,  -134,  -134,    13,
    -134,  -134,    20,   521,    31,  -134,  -134,  -134,    36,   521,
     521,  -134,    84,  -134,    29,   402,   521,    37,  -134,  -134,
    -134,  -134,    38,    48,  -134,  -134,  -134,  -134,  -134,  -134,
    -134,    51,    59,    69,    73,  -134,  -134,  -134,  -134,  -134,
      74,   451,   451,    75,    80,    82,   521,    92,    96,    95,
     101,   111,   112,   465,   116,  -134,  -134,   355,  -134,   -27,
    -134,  -134,  -134,  -134,  -134,   114,  -134,  -134,   521,  -134,
     521,  -134,   521,  -134,    33,   -13,  -134,  -134,   172,   521,
     521,   122,   124,  -134,   263,   121,   144,  -134,  -134,  -134,
      48,  -134,   438,    33,    66,   521,   521,   521,   355,    33,
      84,    47,    29,  -134,  -134,  -134,  -134,   123,  -134,  -134,
     499,   434,   465,   503,    91,   521,   -22,    10,   142,    47,
    -134,   147,  -134,  -134,  -134,   355,   275,   274,   275,   415,
     150,  -134,   149,  -134,   153,    47,  -134,  -134,    91,  -134,
     521,   521,   521,   206,   208,   521,   521,   521,   521,   521,
    -134,  -134,   517,  -134,   156,  -134,  -134,  -134,   158,   160,
     163,   164,   171,  -134,   174,   122,  -134,   175,   276,  -134,
    -134,   178,  -134,   176,  -134,  -134,   191,   193,  -134,   195,
    -134,   -27,  -134,   218,  -134,  -134,  -134,  -134,  -134,  -134,
      33,   -13,   198,   203,   533,   579,   586,   521,   521,   135,
     135,  -134,  -134,  -134,  -134,    48,  -134,  -134,  -134,  -134,
    -134,  -134,    33,  -134,  -134,   204,    66,   465,   197,   116,
     -22,  -134,  -134,  -134,   225,  -134,   422,   422,  -134,  -134,
    -134,  -134,  -134,  -134,   227,  -134
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,     0,     0,     0,     0,    45,     0,     0,
      45,    45,     0,     0,     0,     0,     0,     0,     0,    45,
      45,    45,    45,     0,     0,     0,     0,    11,     3,     0,
      13,    92,     0,     0,     0,    89,    88,    91,     0,     0,
       0,   103,    97,   104,     0,     0,     0,     0,    63,    62,
      83,    81,    84,    82,    46,    19,    54,    55,    65,    69,
      67,     0,     0,     0,     0,    50,    51,    52,    53,    64,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    60,    61,    46,    40,    45,
      57,    58,    59,    56,    43,     0,    10,     6,     0,     4,
       0,    12,     0,    90,     0,     0,   106,   105,     0,     0,
       0,     0,     0,   109,     0,     0,     0,    75,    74,    72,
      71,   107,     0,     0,     0,     0,     0,     0,     0,     0,
      97,    97,     0,    23,    24,    77,    79,     0,    25,    26,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    97,
      38,     0,    39,    41,    42,     0,     0,     0,     0,     0,
       0,    15,    91,    17,    91,    97,    98,    99,     0,    86,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
     108,    76,     0,    18,     0,   101,   100,   102,     0,     0,
       0,     0,     0,    20,    45,    48,    49,     0,     0,    78,
      27,     0,    28,     0,    30,    31,     0,     0,    34,    69,
      35,    45,    48,     0,    44,     7,     8,     5,     9,    93,
       0,     0,     0,     0,   119,   118,   117,     0,     0,   110,
     111,   112,   113,   114,    73,     0,    85,    87,    94,    66,
      70,    68,    46,    21,    80,     0,     0,     0,     0,     0,
       0,    37,    14,    16,     0,    95,   115,   116,    22,    47,
      29,    32,    33,    36,     0,    96
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -134,  -134,  -130,  -134,  -134,  -134,    25,   -69,  -134,   -11,
     299,   306,   307,   308,     6,   -83,   -53,  -119,    46,   -41,
       0,  -133,     8,    19,    27,   -80,   151,    -6,   -14
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    28,   158,   156,    29,    55,   133,    64,    89,
      47,    90,    91,    92,    69,    48,   134,   135,    93,    50,
      34,    35,    51,    37,    52,   111,   189,   113,   114
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint16 yytable[] =
{
      53,   152,    53,   138,   119,    38,    53,    53,    78,    78,
     205,    95,    36,    36,   150,    53,    53,    70,   199,   139,
      30,    59,    80,    81,    82,    62,   215,   103,   217,    31,
      73,    54,    32,   106,   107,    71,    72,    39,    40,   120,
     121,    77,    79,    99,    83,    84,    88,    94,    33,    96,
     195,   196,    49,   209,    49,   109,   110,    62,    74,    75,
      97,    98,    41,   112,    31,   137,   137,    32,   100,   212,
     101,    31,   202,   204,    32,    31,   153,   151,    32,   102,
      54,    53,   199,    43,   157,   222,   159,   200,    76,   104,
     108,    46,   109,   110,   105,   122,   160,   123,   136,   136,
     185,   186,   187,   166,   167,   163,   145,   124,    31,   107,
     125,    32,   161,    36,   154,   261,    53,   193,   126,   190,
     191,   192,    53,   162,   164,   185,   186,   187,   127,    39,
      40,   128,   129,   140,   201,   151,   151,   206,   141,   207,
     142,   234,   184,   188,   214,   177,   178,   179,   194,    53,
     143,   197,   208,   210,    41,   144,   224,   225,   226,   146,
     203,   229,   230,   231,   232,   233,   262,   136,   183,   147,
     148,   181,   155,     2,     3,    43,   235,    45,   260,   165,
      76,   168,   198,    46,   169,   211,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,   182,   213,   220,    39,    40,
     219,   221,   227,   256,   257,   228,   236,   197,   237,   243,
     238,   253,    23,   239,   240,    24,    25,    26,   252,    36,
      27,   241,   242,    41,   247,   244,   251,   246,    56,    57,
      58,   151,    31,    59,    60,    32,    61,    62,    63,   248,
     258,   249,    42,   250,    43,   245,   263,   254,    33,    44,
     264,    45,    46,   255,   259,   197,   170,   171,   172,   173,
     174,   175,   176,   177,   178,   179,     3,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   265,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    13,    14,    15,
      16,    17,    18,    19,    20,    21,    22,    65,    39,    40,
     185,   186,   187,   245,    66,    67,    68,     0,    31,   223,
       0,    32,     0,   180,    23,     0,     0,    24,    25,    26,
       0,   216,    27,    41,     0,    39,    40,     0,    85,    86,
      58,     0,    31,    59,    60,    32,    61,    62,    63,     0,
       0,     0,    42,     0,    43,     0,     0,     0,    87,    44,
      41,     0,    46,    39,    40,    85,    86,    58,     0,    31,
      59,    60,    32,    61,    62,    63,     0,     0,     0,    42,
       0,    43,     0,     0,     0,    54,    44,     0,    41,    46,
       0,     0,     0,    85,    86,    58,     0,    31,    59,    60,
      32,    61,    62,    63,    39,    40,     0,    42,     0,    43,
      39,   115,   116,     0,    44,     0,     0,    46,   170,   171,
     172,   173,   174,   175,   176,   177,   178,   179,     0,    41,
     175,   176,   177,   178,   179,    41,     0,     0,    31,     0,
       0,    32,    39,    40,     0,     0,    39,    40,    42,     0,
      43,     0,   117,   118,    42,    44,    43,    45,    46,    39,
      40,    44,     0,     0,    46,     0,     0,    41,     0,     0,
       0,    41,   218,    39,    40,     0,    31,    39,    40,    32,
      31,     0,     0,    32,    41,     0,   149,   131,    43,     0,
      42,     0,    43,    76,     0,     0,    46,    44,    41,     0,
      46,     0,    41,   130,   131,    43,     0,    39,    40,     0,
     132,    39,    40,    46,     0,     0,     0,   149,   131,    43,
       0,    42,     0,    43,    76,    39,    40,    46,    76,    39,
      40,    46,    41,     0,     0,     0,    41,   171,   172,   173,
     174,   175,   176,   177,   178,   179,     0,     0,     0,     0,
      41,    42,     0,    43,    41,     0,     0,    43,   132,     0,
       0,    46,    76,     0,    33,    46,     0,     0,     0,    42,
       0,    43,     0,     0,     0,    43,    44,     0,     0,    46,
      76,     0,     0,    46,   172,   173,   174,   175,   176,   177,
     178,   179,   173,   174,   175,   176,   177,   178,   179
};

static const yytype_int16 yycheck[] =
{
       6,    84,     8,    72,    45,     5,    12,    13,    14,    15,
     143,    22,     4,     5,    83,    21,    22,     9,   137,    72,
      57,    43,    16,    17,    18,    47,   156,    33,   158,    42,
      11,    58,    45,    39,    40,    10,    11,     8,     9,    45,
      46,    14,    15,    55,    19,    20,    21,    22,    61,    57,
     130,   131,     6,    43,     8,     8,     9,    47,    12,    13,
      55,    56,    33,    44,    42,    71,    72,    45,    56,   149,
      57,    42,   141,   142,    45,    42,    87,    83,    45,    59,
      58,    87,   201,    54,    98,   165,   100,   140,    59,    58,
       6,    62,     8,     9,    58,    58,   102,    59,    71,    72,
      34,    35,    36,   109,   110,   105,    11,    59,    42,   115,
      59,    45,   104,   105,    89,   248,   122,   128,    59,   125,
     126,   127,   128,   104,   105,    34,    35,    36,    59,     8,
       9,    58,    58,    58,   140,   141,   142,   143,    58,   145,
      58,   182,   123,   124,   155,    10,    11,    12,   129,   155,
      58,   132,   146,   147,    33,    59,   170,   171,   172,    58,
     141,   175,   176,   177,   178,   179,   249,   140,   122,    58,
      58,    50,    58,     0,     1,    54,   182,    61,   247,     7,
      59,    59,    59,    62,    60,    43,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    61,    59,    58,     8,     9,
      60,    58,     6,   227,   228,     7,    60,   198,    60,   194,
      60,   221,    49,    60,    60,    52,    53,    54,   220,   221,
      57,    60,    58,    33,    58,    60,   211,    59,    38,    39,
      40,   247,    42,    43,    44,    45,    46,    47,    48,    58,
     242,    58,    52,    58,    54,    37,   250,    59,    61,    59,
      35,    61,    62,    60,    60,   246,     3,     4,     5,     6,
       7,     8,     9,    10,    11,    12,     1,     3,     4,     5,
       6,     7,     8,     9,    10,    11,    12,    60,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,     8,     8,     9,
      34,    35,    36,    37,     8,     8,     8,    -1,    42,   168,
      -1,    45,    -1,    60,    49,    -1,    -1,    52,    53,    54,
      -1,    57,    57,    33,    -1,     8,     9,    -1,    38,    39,
      40,    -1,    42,    43,    44,    45,    46,    47,    48,    -1,
      -1,    -1,    52,    -1,    54,    -1,    -1,    -1,    58,    59,
      33,    -1,    62,     8,     9,    38,    39,    40,    -1,    42,
      43,    44,    45,    46,    47,    48,    -1,    -1,    -1,    52,
      -1,    54,    -1,    -1,    -1,    58,    59,    -1,    33,    62,
      -1,    -1,    -1,    38,    39,    40,    -1,    42,    43,    44,
      45,    46,    47,    48,     8,     9,    -1,    52,    -1,    54,
       8,     9,    10,    -1,    59,    -1,    -1,    62,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    -1,    33,
       8,     9,    10,    11,    12,    33,    -1,    -1,    42,    -1,
      -1,    45,     8,     9,    -1,    -1,     8,     9,    52,    -1,
      54,    -1,    50,    51,    52,    59,    54,    61,    62,     8,
       9,    59,    -1,    -1,    62,    -1,    -1,    33,    -1,    -1,
      -1,    33,    57,     8,     9,    -1,    42,     8,     9,    45,
      42,    -1,    -1,    45,    33,    -1,    52,    53,    54,    -1,
      52,    -1,    54,    59,    -1,    -1,    62,    59,    33,    -1,
      62,    -1,    33,    52,    53,    54,    -1,     8,     9,    -1,
      59,     8,     9,    62,    -1,    -1,    -1,    52,    53,    54,
      -1,    52,    -1,    54,    59,     8,     9,    62,    59,     8,
       9,    62,    33,    -1,    -1,    -1,    33,     4,     5,     6,
       7,     8,     9,    10,    11,    12,    -1,    -1,    -1,    -1,
      33,    52,    -1,    54,    33,    -1,    -1,    54,    59,    -1,
      -1,    62,    59,    -1,    61,    62,    -1,    -1,    -1,    52,
      -1,    54,    -1,    -1,    -1,    54,    59,    -1,    -1,    62,
      59,    -1,    -1,    62,     5,     6,     7,     8,     9,    10,
      11,    12,     6,     7,     8,     9,    10,    11,    12
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    64,     0,     1,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    49,    52,    53,    54,    57,    65,    68,
      57,    42,    45,    61,    83,    84,    85,    86,    83,     8,
       9,    33,    52,    54,    59,    61,    62,    73,    78,    81,
      82,    85,    87,    90,    58,    69,    38,    39,    40,    43,
      44,    46,    47,    48,    71,    73,    74,    75,    76,    77,
      85,    69,    69,    86,    81,    81,    59,    87,    90,    87,
      77,    77,    77,    69,    69,    38,    39,    58,    69,    72,
      74,    75,    76,    81,    69,    72,    57,    55,    56,    55,
      56,    57,    59,    90,    58,    58,    90,    90,     6,     8,
       9,    88,    86,    90,    91,     9,    10,    50,    51,    82,
      90,    90,    58,    59,    59,    59,    59,    59,    58,    58,
      52,    53,    59,    70,    79,    80,    87,    90,    70,    79,
      58,    58,    58,    58,    59,    11,    58,    58,    58,    52,
      70,    90,    78,    72,    69,    58,    67,    91,    66,    91,
      90,    85,    86,    83,    86,     7,    90,    90,    59,    60,
       3,     4,     5,     6,     7,     8,     9,    10,    11,    12,
      60,    50,    61,    81,    86,    34,    35,    36,    86,    89,
      90,    90,    90,    72,    86,    88,    88,    86,    59,    80,
      79,    90,    70,    86,    70,    84,    90,    90,    77,    43,
      77,    43,    88,    59,    72,    65,    57,    65,    57,    60,
      58,    58,    88,    89,    91,    91,    91,     6,     7,    91,
      91,    91,    91,    91,    82,    90,    60,    60,    60,    60,
      60,    60,    58,    69,    60,    37,    59,    58,    58,    58,
      58,    69,    85,    83,    59,    60,    91,    91,    85,    60,
      70,    84,    78,    77,    35,    60
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    63,    64,    64,    66,    65,    67,    65,    65,    65,
      65,    65,    65,    65,    68,    68,    68,    68,    68,    68,
      68,    68,    68,    68,    68,    68,    68,    68,    68,    68,
      68,    68,    68,    68,    68,    68,    68,    68,    68,    68,
      68,    68,    68,    68,    68,    69,    69,    70,    70,    70,
      71,    71,    71,    71,    71,    71,    72,    72,    72,    72,
      72,    72,    73,    73,    74,    75,    75,    76,    76,    77,
      77,    78,    78,    78,    78,    78,    78,    79,    79,    79,
      80,    81,    81,    81,    82,    82,    82,    82,    83,    83,
      84,    85,    86,    86,    87,    87,    87,    88,    88,    88,
      89,    89,    89,    90,    90,    90,    90,    90,    90,    91,
      91,    91,    91,    91,    91,    91,    91,    91,    91,    91
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     0,     4,     0,     4,     4,     4,
       2,     1,     2,     2,     6,     4,     6,     4,     4,     2,
       4,     5,     6,     3,     3,     3,     3,     4,     4,     6,
       4,     4,     6,     6,     4,     4,     6,     5,     3,     3,
       2,     3,     3,     2,     4,     0,     1,     4,     2,     2,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     4,     1,     4,     1,
       4,     2,     2,     4,     2,     2,     3,     1,     2,     1,
       3,     1,     1,     1,     1,     4,     3,     4,     1,     1,
       2,     1,     1,     4,     4,     5,     7,     0,     2,     2,
       1,     1,     1,     1,     1,     2,     2,     2,     3,     1,
       3,     3,     3,     3,     3,     4,     4,     3,     3,     3
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
#line 37 "a.y" /* yacc.c:1646  */
    {
		if((yyvsp[-1].sym)->value != pc)
			yyerror("redeclaration of %s", (yyvsp[-1].sym)->name);
		(yyvsp[-1].sym)->value = pc;
	}
#line 1496 "a.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 44 "a.y" /* yacc.c:1646  */
    {
		(yyvsp[-1].sym)->type = LLAB;
		(yyvsp[-1].sym)->value = pc;
	}
#line 1505 "a.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 50 "a.y" /* yacc.c:1646  */
    {
		(yyvsp[-3].sym)->type = LVAR;
		(yyvsp[-3].sym)->value = (yyvsp[-1].lval);
	}
#line 1514 "a.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 55 "a.y" /* yacc.c:1646  */
    {
		if((yyvsp[-3].sym)->value != (yyvsp[-1].lval))
			yyerror("redeclaration of %s", (yyvsp[-3].sym)->name);
		(yyvsp[-3].sym)->value = (yyvsp[-1].lval);
	}
#line 1524 "a.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 61 "a.y" /* yacc.c:1646  */
    {
		nosched = (yyvsp[-1].lval);
	}
#line 1532 "a.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 73 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-5].lval), &(yyvsp[-4].gen), (yyvsp[-2].lval), &(yyvsp[0].gen));
	}
#line 1540 "a.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 77 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1548 "a.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 84 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-5].lval), &(yyvsp[-4].gen), (yyvsp[-2].lval), &(yyvsp[0].gen));
	}
#line 1556 "a.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 88 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1564 "a.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 95 "a.y" /* yacc.c:1646  */
    {
		if(!isreg(&(yyvsp[-2].gen)) && !isreg(&(yyvsp[0].gen)))
			print("one side must be register\n");
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1574 "a.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 104 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-1].lval), &nullgen, NREG, &nullgen);
	}
#line 1582 "a.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 111 "a.y" /* yacc.c:1646  */
    {
		if(!isreg(&(yyvsp[-2].gen)) && !isreg(&(yyvsp[0].gen)))
			print("one side must be register\n");
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1592 "a.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 120 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-4].lval), &(yyvsp[-3].gen), (yyvsp[-1].lval), &nullgen);
	}
#line 1600 "a.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 124 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-5].lval), &(yyvsp[-4].gen), (yyvsp[-2].lval), &(yyvsp[0].gen));
	}
#line 1608 "a.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 131 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-2].lval), &nullgen, NREG, &(yyvsp[0].gen));
	}
#line 1616 "a.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 135 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-2].lval), &nullgen, NREG, &(yyvsp[0].gen));
	}
#line 1624 "a.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 139 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-2].lval), &nullgen, NREG, &(yyvsp[0].gen));
	}
#line 1632 "a.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 143 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-2].lval), &nullgen, NREG, &(yyvsp[0].gen));
	}
#line 1640 "a.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 147 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &nullgen, (yyvsp[-2].lval), &(yyvsp[0].gen));
	}
#line 1648 "a.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 154 "a.y" /* yacc.c:1646  */
    {
		if(!isreg(&(yyvsp[-2].gen)))
			print("left side must be register\n");
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1658 "a.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 160 "a.y" /* yacc.c:1646  */
    {
		if(!isreg(&(yyvsp[-4].gen)))
			print("left side must be register\n");
		outcode((yyvsp[-5].lval), &(yyvsp[-4].gen), (yyvsp[-2].lval), &(yyvsp[0].gen));
	}
#line 1668 "a.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 169 "a.y" /* yacc.c:1646  */
    {
		if(!isreg(&(yyvsp[-2].gen)))
			print("left side must be register\n");
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1678 "a.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 178 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1686 "a.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 182 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-5].lval), &(yyvsp[-4].gen), (yyvsp[-2].lval), &(yyvsp[0].gen));
	}
#line 1694 "a.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 189 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-5].lval), &(yyvsp[-4].gen), (yyvsp[-2].lval), &(yyvsp[0].gen));
	}
#line 1702 "a.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 196 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1710 "a.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 200 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1718 "a.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 204 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-5].lval), &(yyvsp[-4].gen), (yyvsp[-2].lval), &(yyvsp[0].gen));
	}
#line 1726 "a.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 208 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-4].lval), &(yyvsp[-3].gen), (yyvsp[-1].lval), &nullgen);
	}
#line 1734 "a.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 215 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-2].lval), &nullgen, NREG, &(yyvsp[0].gen));
	}
#line 1742 "a.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 222 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-2].lval), &nullgen, NREG, &(yyvsp[0].gen));
	}
#line 1750 "a.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 229 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-1].lval), &nullgen, NREG, &nullgen);
	}
#line 1758 "a.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 233 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-2].lval), &nullgen, NREG, &(yyvsp[0].gen));
	}
#line 1766 "a.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 237 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-2].lval), &(yyvsp[-1].gen), NREG, &nullgen);
	}
#line 1774 "a.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 244 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-1].lval), &nullgen, NREG, &nullgen);
	}
#line 1782 "a.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 248 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1790 "a.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 257 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_BRANCH;
		(yyval.gen).offset = (yyvsp[-3].lval) + pc;
	}
#line 1800 "a.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 263 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		if(pass == 2)
			yyerror("undefined label: %s", (yyvsp[-1].sym)->name);
		(yyval.gen).type = D_BRANCH;
		(yyval.gen).sym = (yyvsp[-1].sym);
		(yyval.gen).offset = (yyvsp[0].lval);
	}
#line 1813 "a.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 272 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_BRANCH;
		(yyval.gen).sym = (yyvsp[-1].sym);
		(yyval.gen).offset = (yyvsp[-1].sym)->value + (yyvsp[0].lval);
	}
#line 1824 "a.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 285 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_HI;
	}
#line 1833 "a.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 290 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_LO;
	}
#line 1842 "a.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 301 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_HI;
	}
#line 1851 "a.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 306 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_LO;
	}
#line 1860 "a.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 320 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_MREG;
		(yyval.gen).reg = (yyvsp[0].lval);
	}
#line 1870 "a.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 326 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_MREG;
		(yyval.gen).reg = (yyvsp[-1].lval);
	}
#line 1880 "a.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 334 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_FCREG;
		(yyval.gen).reg = (yyvsp[0].lval);
	}
#line 1890 "a.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 340 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_FCREG;
		(yyval.gen).reg = (yyvsp[-1].lval);
	}
#line 1900 "a.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 348 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_FREG;
		(yyval.gen).reg = (yyvsp[0].lval);
	}
#line 1910 "a.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 354 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_FREG;
		(yyval.gen).reg = (yyvsp[-1].lval);
	}
#line 1920 "a.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 361 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_CONST;
		(yyval.gen).offset = (yyvsp[0].lval);
	}
#line 1930 "a.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 367 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = (yyvsp[0].gen);
		(yyval.gen).type = D_CONST;
	}
#line 1939 "a.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 372 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = (yyvsp[0].gen);
		(yyval.gen).type = D_OCONST;
	}
#line 1948 "a.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 377 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_SCONST;
		memcpy((yyval.gen).sval, (yyvsp[0].sval), sizeof((yyval.gen).sval));
	}
#line 1958 "a.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 383 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_FCONST;
		(yyval.gen).dval = (yyvsp[0].dval);
	}
#line 1968 "a.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 389 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_FCONST;
		(yyval.gen).dval = -(yyvsp[0].dval);
	}
#line 1978 "a.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 398 "a.y" /* yacc.c:1646  */
    {
		if((yyvsp[-1].lval) != 0)
			yyerror("offset must be zero");
		(yyval.gen) = (yyvsp[0].gen);
	}
#line 1988 "a.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 404 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = (yyvsp[0].gen);
		if((yyvsp[0].gen).name != D_EXTERN && (yyvsp[0].gen).name != D_STATIC) {
		}
	}
#line 1998 "a.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 412 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_OREG;
		(yyval.gen).reg = (yyvsp[-1].lval);
		(yyval.gen).offset = 0;
	}
#line 2009 "a.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 422 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_OREG;
		(yyval.gen).offset = (yyvsp[0].lval);
	}
#line 2019 "a.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 432 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = (yyvsp[-3].gen);
		(yyval.gen).type = D_OREG;
		(yyval.gen).reg = (yyvsp[-1].lval);
	}
#line 2029 "a.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 438 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_OREG;
		(yyval.gen).reg = (yyvsp[-1].lval);
		(yyval.gen).offset = 0;
	}
#line 2040 "a.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 445 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_OREG;
		(yyval.gen).reg = (yyvsp[-1].lval);
		(yyval.gen).offset = (yyvsp[-3].lval);
	}
#line 2051 "a.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 457 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_CONST;
		(yyval.gen).offset = (yyvsp[0].lval);
	}
#line 2061 "a.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 465 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_REG;
		(yyval.gen).reg = (yyvsp[0].lval);
	}
#line 2071 "a.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 474 "a.y" /* yacc.c:1646  */
    {
		if((yyval.lval) < 0 || (yyval.lval) >= NREG)
			print("register value out of range\n");
		(yyval.lval) = (yyvsp[-1].lval);
	}
#line 2081 "a.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 482 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_OREG;
		(yyval.gen).name = (yyvsp[-1].lval);
		(yyval.gen).sym = S;
		(yyval.gen).offset = (yyvsp[-3].lval);
	}
#line 2093 "a.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 490 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_OREG;
		(yyval.gen).name = (yyvsp[-1].lval);
		(yyval.gen).sym = (yyvsp[-4].sym);
		(yyval.gen).offset = (yyvsp[-3].lval);
	}
#line 2105 "a.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 498 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_OREG;
		(yyval.gen).name = D_STATIC;
		(yyval.gen).sym = (yyvsp[-6].sym);
		(yyval.gen).offset = (yyvsp[-3].lval);
	}
#line 2117 "a.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 507 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = 0;
	}
#line 2125 "a.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 511 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[0].lval);
	}
#line 2133 "a.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 515 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = -(yyvsp[0].lval);
	}
#line 2141 "a.tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 527 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[0].sym)->value;
	}
#line 2149 "a.tab.c" /* yacc.c:1646  */
    break;

  case 105:
#line 531 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = -(yyvsp[0].lval);
	}
#line 2157 "a.tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 535 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[0].lval);
	}
#line 2165 "a.tab.c" /* yacc.c:1646  */
    break;

  case 107:
#line 539 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = ~(yyvsp[0].lval);
	}
#line 2173 "a.tab.c" /* yacc.c:1646  */
    break;

  case 108:
#line 543 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-1].lval);
	}
#line 2181 "a.tab.c" /* yacc.c:1646  */
    break;

  case 110:
#line 550 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) + (yyvsp[0].lval);
	}
#line 2189 "a.tab.c" /* yacc.c:1646  */
    break;

  case 111:
#line 554 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) - (yyvsp[0].lval);
	}
#line 2197 "a.tab.c" /* yacc.c:1646  */
    break;

  case 112:
#line 558 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) * (yyvsp[0].lval);
	}
#line 2205 "a.tab.c" /* yacc.c:1646  */
    break;

  case 113:
#line 562 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) / (yyvsp[0].lval);
	}
#line 2213 "a.tab.c" /* yacc.c:1646  */
    break;

  case 114:
#line 566 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) % (yyvsp[0].lval);
	}
#line 2221 "a.tab.c" /* yacc.c:1646  */
    break;

  case 115:
#line 570 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-3].lval) << (yyvsp[0].lval);
	}
#line 2229 "a.tab.c" /* yacc.c:1646  */
    break;

  case 116:
#line 574 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-3].lval) >> (yyvsp[0].lval);
	}
#line 2237 "a.tab.c" /* yacc.c:1646  */
    break;

  case 117:
#line 578 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) & (yyvsp[0].lval);
	}
#line 2245 "a.tab.c" /* yacc.c:1646  */
    break;

  case 118:
#line 582 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) ^ (yyvsp[0].lval);
	}
#line 2253 "a.tab.c" /* yacc.c:1646  */
    break;

  case 119:
#line 586 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) | (yyvsp[0].lval);
	}
#line 2261 "a.tab.c" /* yacc.c:1646  */
    break;


#line 2265 "a.tab.c" /* yacc.c:1646  */
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
