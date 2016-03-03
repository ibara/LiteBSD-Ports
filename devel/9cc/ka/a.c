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
    LMOVD = 259,
    LMOVB = 260,
    LSWAP = 261,
    LADDW = 262,
    LCMP = 263,
    LBRA = 264,
    LFMOV = 265,
    LFCONV = 266,
    LFADD = 267,
    LCPOP = 268,
    LTRAP = 269,
    LJMPL = 270,
    LXORW = 271,
    LNOP = 272,
    LEND = 273,
    LRETT = 274,
    LUNIMP = 275,
    LTEXT = 276,
    LDATA = 277,
    LRETRN = 278,
    LCONST = 279,
    LSP = 280,
    LSB = 281,
    LFP = 282,
    LPC = 283,
    LCREG = 284,
    LFLUSH = 285,
    LREG = 286,
    LFREG = 287,
    LR = 288,
    LC = 289,
    LF = 290,
    LFSR = 291,
    LFPQ = 292,
    LPSR = 293,
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
	long	lval;
	double	dval;
	char	sval[8];
	Gen	gen;

#line 163 "a.tab.c" /* yacc.c:355  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_A_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 178 "a.tab.c" /* yacc.c:358  */

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
#define YYLAST   682

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  63
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  24
/* YYNRULES -- Number of rules.  */
#define YYNRULES  139
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  331

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
       0,    31,    31,    32,    36,    35,    43,    42,    48,    53,
      59,    63,    64,    65,    71,    75,    79,    83,    87,    95,
      99,   103,   107,   111,   115,   123,   127,   134,   138,   144,
     148,   152,   162,   166,   170,   174,   182,   186,   194,   208,
     212,   216,   220,   224,   228,   232,   236,   244,   248,   252,
     256,   265,   273,   277,   281,   285,   292,   299,   303,   310,
     314,   318,   322,   329,   336,   340,   344,   348,   355,   359,
     366,   370,   374,   381,   385,   392,   396,   403,   407,   411,
     415,   419,   426,   433,   437,   444,   448,   452,   459,   465,
     471,   480,   489,   497,   505,   513,   521,   527,   535,   541,
     549,   554,   562,   568,   575,   583,   584,   592,   599,   606,
     614,   615,   624,   632,   640,   649,   650,   653,   656,   660,
     666,   667,   668,   671,   672,   676,   680,   684,   688,   694,
     695,   699,   703,   707,   711,   715,   719,   723,   727,   731
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "'|'", "'^'", "'&'", "'<'", "'>'", "'+'",
  "'-'", "'*'", "'/'", "'%'", "LMOVW", "LMOVD", "LMOVB", "LSWAP", "LADDW",
  "LCMP", "LBRA", "LFMOV", "LFCONV", "LFADD", "LCPOP", "LTRAP", "LJMPL",
  "LXORW", "LNOP", "LEND", "LRETT", "LUNIMP", "LTEXT", "LDATA", "LRETRN",
  "LCONST", "LSP", "LSB", "LFP", "LPC", "LCREG", "LFLUSH", "LREG", "LFREG",
  "LR", "LC", "LF", "LFSR", "LFPQ", "LPSR", "LSCHED", "LFCONST", "LSCONST",
  "LNAME", "LLAB", "LVAR", "':'", "'='", "';'", "','", "'('", "')'", "'$'",
  "'~'", "$accept", "prog", "line", "$@1", "$@2", "inst", "rel", "rreg",
  "fsr", "fpq", "psr", "creg", "freg", "ximm", "fimm", "imm", "sreg",
  "addr", "name", "comma", "offset", "pointer", "con", "expr", YY_NULLPTR
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

#define YYPACT_NINF -171

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-171)))

#define YYTABLE_NINF -1

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -171,   373,  -171,   -51,   399,   423,   440,   263,   -12,   -11,
     -43,   446,    24,    24,   -21,    -4,   -43,   -12,    10,   -43,
     -11,    14,   599,   599,   -43,   543,   -19,    78,   -15,   -20,
    -171,  -171,    -7,  -171,   613,   613,  -171,  -171,  -171,  -171,
     -14,     6,    17,  -171,  -171,    55,  -171,   557,   572,   613,
      23,    28,    33,    44,    88,    98,   104,  -171,   106,  -171,
      35,  -171,   111,   113,   114,   138,   150,   155,   182,   613,
     183,   184,   189,   192,   199,   201,   202,  -171,   576,     8,
     207,   215,   216,   217,   218,   234,   236,   238,  -171,   463,
     240,   243,    87,   -43,   -43,  -171,  -171,   245,   -43,  -171,
     613,   250,    64,   167,  -171,    55,   130,   -43,   -43,   125,
    -171,  -171,   613,  -171,   613,  -171,  -171,  -171,   613,   613,
     613,   271,   613,   613,   251,     2,  -171,   100,  -171,  -171,
      35,  -171,   478,   263,   -11,   263,   263,   -11,   478,   124,
      52,   263,   263,   263,   263,   -11,   -11,   290,  -171,   501,
     263,   -11,   -11,   -11,   -11,   -12,   130,  -171,   252,   259,
    -171,   505,    24,    24,    24,    24,   -21,   -11,  -171,   -11,
    -171,   254,  -171,   -11,   -11,  -171,  -171,  -171,  -171,   -11,
    -171,   578,   139,   613,   251,  -171,  -171,  -171,   112,   326,
     220,   326,   279,   253,   258,   261,   130,  -171,  -171,   139,
     -11,   613,  -171,   613,   613,   613,   313,   353,   613,   613,
     613,   613,   613,  -171,  -171,  -171,  -171,  -171,  -171,  -171,
    -171,  -171,  -171,  -171,  -171,  -171,  -171,  -171,  -171,  -171,
    -171,   301,   302,  -171,  -171,  -171,  -171,  -171,  -171,  -171,
    -171,  -171,  -171,  -171,  -171,  -171,  -171,  -171,   307,  -171,
     309,  -171,  -171,  -171,   330,  -171,  -171,  -171,  -171,  -171,
    -171,   314,   318,  -171,  -171,   543,  -171,   319,  -171,   323,
    -171,  -171,   324,   351,   311,  -171,  -171,  -171,  -171,  -171,
    -171,  -171,   325,   350,   352,   363,   109,   409,   670,   613,
     613,   195,   195,  -171,  -171,  -171,  -171,  -171,   -11,   -11,
      24,   -21,  -171,  -171,    36,    36,   367,   368,  -171,   375,
    -171,  -171,  -171,   225,   225,  -171,  -171,  -171,  -171,  -171,
    -171,  -171,  -171,  -171,   520,  -171,  -171,  -171,   364,   609,
    -171
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,     0,     0,     0,     0,     0,     0,     0,
     115,     0,     0,     0,     0,   115,   115,     0,   115,   115,
       0,   115,     0,     0,   115,     0,     0,     0,     0,     0,
      11,     3,     0,    13,     0,     0,   123,    96,   105,    98,
       0,     0,     0,    93,    95,   117,   124,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    92,     0,   110,
       0,    94,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   116,     0,     0,
       0,     0,     0,     0,     0,     0,   115,     0,    62,     0,
       0,     0,   116,   115,   115,    77,    82,     0,   115,    68,
       0,     0,     0,     0,    88,   117,   117,   115,   115,     0,
      10,     6,     0,     4,     0,    12,   126,   125,     0,     0,
       0,     0,     0,     0,     0,     0,   129,     0,   101,   100,
     104,   127,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,   104,     0,
       0,     0,     0,     0,     0,     0,   117,    51,     0,     0,
     102,     0,     0,     0,     0,     0,     0,   116,    61,     0,
      52,     0,    53,     0,     0,    80,    81,    78,    79,     0,
      69,     0,     0,     0,    90,    91,    57,    58,     0,     0,
       0,     0,     0,     0,     0,     0,   117,   118,   119,     0,
       0,     0,   107,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,   128,    14,    64,    27,    34,    63,    36,
      32,    49,    47,    65,    28,    15,    24,    25,   121,   120,
     122,     0,     0,    29,    35,    37,    33,    50,    48,    16,
      26,    19,    17,    30,    31,    18,    38,    41,    92,    42,
      92,    75,    76,    90,     0,   103,    22,    23,    21,    20,
      70,    71,    73,    59,    60,     0,    45,    92,    46,    92,
      56,    83,     0,     0,     0,     7,     8,     5,     9,   106,
      97,    99,     0,     0,     0,     0,   139,   138,   137,     0,
       0,   130,   131,   132,   133,   134,   111,   112,     0,     0,
       0,     0,    54,    55,     0,     0,     0,     0,    89,     0,
     113,   109,   108,   135,   136,    39,    40,    72,    74,    43,
      66,    44,    67,    84,     0,    86,    87,    85,     0,     0,
     114
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -171,  -171,  -170,  -171,  -171,  -171,   -67,    65,   295,  -171,
    -104,    19,    30,    -3,   128,    -1,   126,   120,   129,   230,
     -97,   237,   -22,   -68
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    31,   191,   189,    32,   107,    50,    51,    63,
      52,    53,    54,    55,    81,    56,    57,   129,    59,    78,
     124,   232,    60,   127
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint16 yytable[] =
{
     102,   102,    66,   109,    67,    71,    33,    75,   184,   185,
     200,   157,   116,   117,    87,    77,    91,   159,    37,   275,
      98,   277,   170,    41,    64,   126,   130,   131,   215,    38,
      38,    40,    40,    85,   223,    65,   114,    38,   110,    40,
     113,    80,    83,    84,   190,   118,   192,   148,    94,    69,
     115,    38,    39,    40,    77,    42,   158,    69,   160,   253,
     201,   121,   202,   122,   123,   119,    39,   109,    92,    42,
      62,    70,    77,    74,    76,    69,   120,    38,   126,    40,
      86,   132,    90,    93,    44,    97,   133,   228,   229,   230,
     126,   134,   126,    38,   140,    40,   193,   194,   195,   282,
     197,   198,   135,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   204,   205,   206,   207,   208,   209,   210,
     211,   212,   176,   182,    58,    68,    72,    73,    38,    39,
      40,    82,    42,   111,   112,   286,   287,   288,   122,   123,
     291,   292,   293,   294,   295,   108,   136,   228,   229,   230,
     274,   101,   103,    38,   252,    40,   137,   175,   227,   272,
     213,   273,   138,    37,   139,    38,   240,    40,    41,   141,
      43,   142,   143,   125,   228,   229,   230,   241,   183,   285,
     271,   126,   126,   126,   188,   262,   126,   126,   126,   126,
     126,   256,   258,   259,   260,   261,   144,   214,   302,   218,
     320,   322,   221,   222,   225,   210,   211,   212,   145,   172,
     237,   238,   239,   146,   242,   171,   245,   246,   247,   249,
     251,   313,   314,   203,   204,   205,   206,   207,   208,   209,
     210,   211,   212,   208,   209,   210,   211,   212,   266,   268,
     147,   149,   150,   109,   270,    88,    89,   151,    95,    96,
     152,    99,   216,   217,   104,   219,   220,   153,   224,   154,
     155,   233,   234,   235,   236,   161,   231,   126,   126,   243,
     244,    34,    35,   162,   163,   164,   165,   276,   196,   248,
     250,   257,   203,   204,   205,   206,   207,   208,   209,   210,
     211,   212,   166,   263,   167,   264,   169,    36,   173,   267,
     269,   174,   130,   179,   325,   323,   327,   117,   181,   255,
     199,   254,   265,   279,   231,    45,   168,    46,   280,   289,
     318,   281,    47,   177,   178,    49,   284,     3,   180,    37,
     317,    38,    39,    40,    41,    42,   278,   186,   187,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
     290,   296,   297,   315,   316,   298,    25,   299,   274,   319,
     321,   308,   300,     2,     3,    26,   301,   304,    27,    28,
      29,   305,   306,    30,   309,   303,     4,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    17,
      18,    19,    20,    21,    22,    23,    24,    34,    35,   307,
     310,   328,   311,    25,   205,   206,   207,   208,   209,   210,
     211,   212,    26,   312,   330,    27,    28,    29,    69,   324,
      30,    34,    35,    36,   226,   326,   283,     0,    37,     0,
      38,    39,    40,    41,    42,    43,     0,    44,    34,    35,
       0,    45,     0,    46,    34,    35,     0,    36,    47,     0,
      48,    49,    37,     0,    38,    39,    40,    41,    42,     0,
      61,    34,    35,     0,    36,    45,     0,    46,     0,     0,
      36,    38,    47,    40,    48,    49,    34,    35,    39,     0,
       0,    42,    45,     0,    46,     0,     0,    36,    45,    47,
      46,    69,    49,     0,    38,    47,    40,    79,    49,    34,
      35,     0,    36,    34,    35,   105,   106,    46,     0,    38,
       0,    40,    47,     0,     0,    49,    44,     0,    34,   329,
      45,     0,    46,     0,     0,    36,     0,    47,     0,    36,
      49,     0,    38,     0,    40,     0,     0,    39,     0,     0,
      42,    34,    35,    45,    36,    46,     0,    45,     0,    46,
      47,     0,     0,    49,    47,    34,    35,    49,     0,     0,
     160,   128,    45,     0,    46,     0,     0,    36,     0,    47,
      34,    35,    49,     0,    34,    35,    34,    35,     0,     0,
       0,    36,     0,     0,     0,   105,   106,    46,    38,     0,
      40,     0,    47,     0,     0,    49,    36,    34,    35,     0,
      36,    46,    36,     0,     0,     0,   100,    34,    35,    49,
       0,    34,    35,   128,    45,     0,    46,     0,   156,   106,
      46,    47,    46,    36,    49,   100,     0,   100,    49,    69,
      49,     0,     0,    36,     0,     0,     0,    36,     0,     0,
       0,    45,     0,    46,     0,     0,     0,     0,   100,   255,
       0,    49,     0,    46,     0,     0,     0,    46,   100,     0,
       0,    49,   100,     0,     0,    49,   206,   207,   208,   209,
     210,   211,   212
};

static const yytype_int16 yycheck[] =
{
      22,    23,     5,    25,     5,     6,    57,     8,   105,   106,
       8,    78,    34,    35,    15,    58,    17,     9,    39,   189,
      21,   191,    89,    44,     5,    47,    48,    49,   132,    41,
      41,    43,    43,    14,   138,     5,    56,    41,    57,    43,
      55,    11,    12,    13,   112,    59,   114,    69,    18,    61,
      57,    41,    42,    43,    58,    45,    78,    61,    50,   156,
      58,     6,    60,     8,     9,    59,    42,    89,    58,    45,
       5,     6,    58,     8,     9,    61,    59,    41,   100,    43,
      15,    58,    17,    18,    48,    20,    58,    35,    36,    37,
     112,    58,   114,    41,    59,    43,   118,   119,   120,   196,
     122,   123,    58,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    92,    59,     4,     5,     6,     7,    41,    42,
      43,    11,    45,    55,    56,   203,   204,   205,     8,     9,
     208,   209,   210,   211,   212,    25,    58,    35,    36,    37,
      38,    22,    23,    41,   155,    43,    58,    92,   139,   181,
      60,   183,    58,    39,    58,    41,   147,    43,    44,    58,
      46,    58,    58,    47,    35,    36,    37,   147,    11,   201,
     181,   203,   204,   205,    59,   166,   208,   209,   210,   211,
     212,   161,   162,   163,   164,   165,    58,   132,   265,   134,
     304,   305,   137,   138,   139,    10,    11,    12,    58,    89,
     145,   146,   147,    58,   149,    89,   151,   152,   153,   154,
     155,   289,   290,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,     8,     9,    10,    11,    12,   173,   174,
      58,    58,    58,   265,   179,    15,    16,    58,    18,    19,
      58,    21,   132,   133,    24,   135,   136,    58,   138,    58,
      58,   141,   142,   143,   144,    58,   140,   289,   290,   149,
     150,     8,     9,    58,    58,    58,    58,    57,     7,   153,
     154,   161,     3,     4,     5,     6,     7,     8,     9,    10,
      11,    12,    58,   167,    58,   169,    58,    34,    58,   173,
     174,    58,   324,    58,   307,   306,   307,   329,    58,    50,
      59,    59,    58,    60,   188,    52,    86,    54,    60,     6,
     301,    60,    59,    93,    94,    62,   200,     1,    98,    39,
     300,    41,    42,    43,    44,    45,    57,   107,   108,    13,
      14,    15,    16,    17,    18,    19,    20,    21,    22,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
       7,    60,    60,   298,   299,    58,    40,    58,    38,   304,
     305,    60,    58,     0,     1,    49,    58,    58,    52,    53,
      54,    58,    58,    57,    59,   265,    13,    14,    15,    16,
      17,    18,    19,    20,    21,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,     8,     9,    58,
      60,    36,    60,    40,     5,     6,     7,     8,     9,    10,
      11,    12,    49,    60,    60,    52,    53,    54,    61,    61,
      57,     8,     9,    34,   139,   307,   199,    -1,    39,    -1,
      41,    42,    43,    44,    45,    46,    -1,    48,     8,     9,
      -1,    52,    -1,    54,     8,     9,    -1,    34,    59,    -1,
      61,    62,    39,    -1,    41,    42,    43,    44,    45,    -1,
      47,     8,     9,    -1,    34,    52,    -1,    54,    -1,    -1,
      34,    41,    59,    43,    61,    62,     8,     9,    42,    -1,
      -1,    45,    52,    -1,    54,    -1,    -1,    34,    52,    59,
      54,    61,    62,    -1,    41,    59,    43,    61,    62,     8,
       9,    -1,    34,     8,     9,    52,    53,    54,    -1,    41,
      -1,    43,    59,    -1,    -1,    62,    48,    -1,     8,     9,
      52,    -1,    54,    -1,    -1,    34,    -1,    59,    -1,    34,
      62,    -1,    41,    -1,    43,    -1,    -1,    42,    -1,    -1,
      45,     8,     9,    52,    34,    54,    -1,    52,    -1,    54,
      59,    -1,    -1,    62,    59,     8,     9,    62,    -1,    -1,
      50,    51,    52,    -1,    54,    -1,    -1,    34,    -1,    59,
       8,     9,    62,    -1,     8,     9,     8,     9,    -1,    -1,
      -1,    34,    -1,    -1,    -1,    52,    53,    54,    41,    -1,
      43,    -1,    59,    -1,    -1,    62,    34,     8,     9,    -1,
      34,    54,    34,    -1,    -1,    -1,    59,     8,     9,    62,
      -1,     8,     9,    51,    52,    -1,    54,    -1,    52,    53,
      54,    59,    54,    34,    62,    59,    -1,    59,    62,    61,
      62,    -1,    -1,    34,    -1,    -1,    -1,    34,    -1,    -1,
      -1,    52,    -1,    54,    -1,    -1,    -1,    -1,    59,    50,
      -1,    62,    -1,    54,    -1,    -1,    -1,    54,    59,    -1,
      -1,    62,    59,    -1,    -1,    62,     6,     7,     8,     9,
      10,    11,    12
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    64,     0,     1,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    40,    49,    52,    53,    54,
      57,    65,    68,    57,     8,     9,    34,    39,    41,    42,
      43,    44,    45,    46,    48,    52,    54,    59,    61,    62,
      70,    71,    73,    74,    75,    76,    78,    79,    80,    81,
      85,    47,    70,    72,    74,    75,    76,    78,    80,    61,
      70,    78,    80,    80,    70,    78,    70,    58,    82,    61,
      75,    77,    80,    75,    75,    74,    70,    78,    82,    82,
      70,    78,    58,    70,    75,    82,    82,    70,    78,    82,
      59,    81,    85,    81,    82,    52,    53,    69,    80,    85,
      57,    55,    56,    55,    56,    57,    85,    85,    59,    59,
      59,     6,     8,     9,    83,    79,    85,    86,    51,    80,
      85,    85,    58,    58,    58,    58,    58,    58,    58,    58,
      59,    58,    58,    58,    58,    58,    58,    58,    85,    58,
      58,    58,    58,    58,    58,    58,    52,    69,    85,     9,
      50,    58,    58,    58,    58,    58,    58,    58,    82,    58,
      69,    79,    80,    58,    58,    70,    75,    82,    82,    58,
      82,    58,    59,    11,    83,    83,    82,    82,    59,    67,
      86,    66,    86,    85,    85,    85,     7,    85,    85,    59,
       8,    58,    60,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    60,    70,    73,    80,    80,    70,    80,
      80,    70,    70,    73,    80,    70,    71,    74,    35,    36,
      37,    79,    84,    80,    80,    80,    80,    70,    70,    70,
      74,    75,    70,    80,    80,    70,    70,    70,    79,    70,
      79,    70,    78,    83,    59,    50,    75,    80,    75,    75,
      75,    75,    74,    79,    79,    58,    70,    79,    70,    79,
      70,    78,    85,    85,    38,    65,    57,    65,    57,    60,
      60,    60,    83,    84,    79,    85,    86,    86,    86,     6,
       7,    86,    86,    86,    86,    86,    60,    60,    58,    58,
      58,    58,    69,    80,    58,    58,    58,    58,    60,    59,
      60,    60,    60,    86,    86,    70,    70,    75,    74,    70,
      73,    70,    73,    78,    61,    76,    77,    78,    36,     9,
      60
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    63,    64,    64,    66,    65,    67,    65,    65,    65,
      65,    65,    65,    65,    68,    68,    68,    68,    68,    68,
      68,    68,    68,    68,    68,    68,    68,    68,    68,    68,
      68,    68,    68,    68,    68,    68,    68,    68,    68,    68,
      68,    68,    68,    68,    68,    68,    68,    68,    68,    68,
      68,    68,    68,    68,    68,    68,    68,    68,    68,    68,
      68,    68,    68,    68,    68,    68,    68,    68,    68,    68,
      68,    68,    68,    68,    68,    68,    68,    68,    68,    68,
      68,    68,    68,    68,    68,    68,    68,    68,    68,    69,
      69,    69,    70,    71,    72,    73,    74,    74,    75,    75,
      76,    76,    77,    77,    78,    79,    79,    80,    80,    80,
      80,    80,    81,    81,    81,    82,    82,    83,    83,    83,
      84,    84,    84,    85,    85,    85,    85,    85,    85,    86,
      86,    86,    86,    86,    86,    86,    86,    86,    86,    86
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     0,     4,     0,     4,     4,     4,
       2,     1,     2,     2,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     4,     4,     6,
       6,     4,     4,     6,     6,     4,     4,     4,     4,     4,
       4,     3,     3,     3,     5,     5,     4,     3,     3,     4,
       4,     3,     2,     4,     4,     4,     6,     6,     2,     3,
       4,     4,     6,     4,     6,     4,     4,     2,     3,     3,
       3,     3,     2,     4,     6,     6,     6,     6,     2,     4,
       2,     2,     1,     1,     1,     1,     1,     4,     1,     4,
       2,     2,     2,     3,     2,     1,     4,     3,     5,     5,
       1,     4,     4,     5,     7,     0,     1,     0,     2,     2,
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
#line 36 "a.y" /* yacc.c:1646  */
    {
		if((yyvsp[-1].sym)->value != pc)
			yyerror("redeclaration of %s", (yyvsp[-1].sym)->name);
		(yyvsp[-1].sym)->value = pc;
	}
#line 1543 "a.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 43 "a.y" /* yacc.c:1646  */
    {
		(yyvsp[-1].sym)->type = LLAB;
		(yyvsp[-1].sym)->value = pc;
	}
#line 1552 "a.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 49 "a.y" /* yacc.c:1646  */
    {
		(yyvsp[-3].sym)->type = LVAR;
		(yyvsp[-3].sym)->value = (yyvsp[-1].lval);
	}
#line 1561 "a.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 54 "a.y" /* yacc.c:1646  */
    {
		if((yyvsp[-3].sym)->value != (yyvsp[-1].lval))
			yyerror("redeclaration of %s", (yyvsp[-3].sym)->name);
		(yyvsp[-3].sym)->value = (yyvsp[-1].lval);
	}
#line 1571 "a.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 60 "a.y" /* yacc.c:1646  */
    {
		nosched = (yyvsp[-1].lval);
	}
#line 1579 "a.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 72 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1587 "a.tab.c" /* yacc.c:1646  */
    break;

  case 15:
#line 76 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1595 "a.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 80 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1603 "a.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 84 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1611 "a.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 88 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1619 "a.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 96 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1627 "a.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 100 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1635 "a.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 104 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1643 "a.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 108 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1651 "a.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 112 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1659 "a.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 116 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1667 "a.tab.c" /* yacc.c:1646  */
    break;

  case 25:
#line 124 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1675 "a.tab.c" /* yacc.c:1646  */
    break;

  case 26:
#line 128 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1683 "a.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 135 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1691 "a.tab.c" /* yacc.c:1646  */
    break;

  case 28:
#line 139 "a.y" /* yacc.c:1646  */
    {
		if((yyvsp[-2].gen).offset != 0)
			yyerror("constant must be zero");
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1701 "a.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 145 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1709 "a.tab.c" /* yacc.c:1646  */
    break;

  case 30:
#line 149 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1717 "a.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 153 "a.y" /* yacc.c:1646  */
    {
		if((yyvsp[-2].gen).offset != 0)
			yyerror("constant must be zero");
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1727 "a.tab.c" /* yacc.c:1646  */
    break;

  case 32:
#line 163 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1735 "a.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 167 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1743 "a.tab.c" /* yacc.c:1646  */
    break;

  case 34:
#line 171 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1751 "a.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 175 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1759 "a.tab.c" /* yacc.c:1646  */
    break;

  case 36:
#line 183 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1767 "a.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 187 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1775 "a.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 195 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1783 "a.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 209 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-5].lval), &(yyvsp[-4].gen), (yyvsp[-2].lval), &(yyvsp[0].gen));
	}
#line 1791 "a.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 213 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-5].lval), &(yyvsp[-4].gen), (yyvsp[-2].lval), &(yyvsp[0].gen));
	}
#line 1799 "a.tab.c" /* yacc.c:1646  */
    break;

  case 41:
#line 217 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1807 "a.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 221 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1815 "a.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 225 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-5].lval), &(yyvsp[-4].gen), (yyvsp[-2].lval), &(yyvsp[0].gen));
	}
#line 1823 "a.tab.c" /* yacc.c:1646  */
    break;

  case 44:
#line 229 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-5].lval), &(yyvsp[-4].gen), (yyvsp[-2].lval), &(yyvsp[0].gen));
	}
#line 1831 "a.tab.c" /* yacc.c:1646  */
    break;

  case 45:
#line 233 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1839 "a.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 237 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1847 "a.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 245 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1855 "a.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 249 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1863 "a.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 253 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1871 "a.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 257 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1879 "a.tab.c" /* yacc.c:1646  */
    break;

  case 51:
#line 266 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-2].lval), &nullgen, NREG, &(yyvsp[0].gen));
	}
#line 1887 "a.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 274 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-2].lval), &nullgen, NREG, &(yyvsp[0].gen));
	}
#line 1895 "a.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 278 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-2].lval), &nullgen, NREG, &(yyvsp[0].gen));
	}
#line 1903 "a.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 282 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-4].lval), &nullgen, (yyvsp[-2].lval), &(yyvsp[0].gen));
	}
#line 1911 "a.tab.c" /* yacc.c:1646  */
    break;

  case 55:
#line 286 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-4].lval), &nullgen, (yyvsp[-2].lval), &(yyvsp[0].gen));
	}
#line 1919 "a.tab.c" /* yacc.c:1646  */
    break;

  case 56:
#line 293 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1927 "a.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 300 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-2].lval), &(yyvsp[-1].gen), NREG, &nullgen);
	}
#line 1935 "a.tab.c" /* yacc.c:1646  */
    break;

  case 58:
#line 304 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-2].lval), &(yyvsp[-1].gen), NREG, &nullgen);
	}
#line 1943 "a.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 311 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), (yyvsp[0].lval), &nullgen);
	}
#line 1951 "a.tab.c" /* yacc.c:1646  */
    break;

  case 60:
#line 315 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), (yyvsp[0].lval), &nullgen);
	}
#line 1959 "a.tab.c" /* yacc.c:1646  */
    break;

  case 61:
#line 319 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-2].lval), &(yyvsp[-1].gen), NREG, &nullgen);
	}
#line 1967 "a.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 323 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-1].lval), &nullgen, NREG, &nullgen);
	}
#line 1975 "a.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 330 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1983 "a.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 337 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1991 "a.tab.c" /* yacc.c:1646  */
    break;

  case 65:
#line 341 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 1999 "a.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 345 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-5].lval), &(yyvsp[-4].gen), (yyvsp[-2].lval), &(yyvsp[0].gen));
	}
#line 2007 "a.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 349 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-5].lval), &(yyvsp[-4].gen), (yyvsp[-2].lval), &(yyvsp[0].gen));
	}
#line 2015 "a.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 356 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-1].lval), &nullgen, NREG, &nullgen);
	}
#line 2023 "a.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 360 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-2].lval), &(yyvsp[-1].gen), NREG, &nullgen);
	}
#line 2031 "a.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 367 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 2039 "a.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 371 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 2047 "a.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 375 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-5].lval), &(yyvsp[-4].gen), (yyvsp[-2].gen).reg, &(yyvsp[0].gen));
	}
#line 2055 "a.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 382 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 2063 "a.tab.c" /* yacc.c:1646  */
    break;

  case 74:
#line 386 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-5].lval), &(yyvsp[-4].gen), (yyvsp[-2].gen).reg, &(yyvsp[0].gen));
	}
#line 2071 "a.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 393 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 2079 "a.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 397 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 2087 "a.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 404 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-1].lval), &nullgen, NREG, &nullgen);
	}
#line 2095 "a.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 408 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-2].lval), &(yyvsp[-1].gen), NREG, &nullgen);
	}
#line 2103 "a.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 412 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-2].lval), &(yyvsp[-1].gen), NREG, &nullgen);
	}
#line 2111 "a.tab.c" /* yacc.c:1646  */
    break;

  case 80:
#line 416 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-2].lval), &nullgen, NREG, &(yyvsp[0].gen));
	}
#line 2119 "a.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 420 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-2].lval), &nullgen, NREG, &(yyvsp[0].gen));
	}
#line 2127 "a.tab.c" /* yacc.c:1646  */
    break;

  case 82:
#line 427 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-1].lval), &nullgen, NREG, &nullgen);
	}
#line 2135 "a.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 434 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-3].lval), &(yyvsp[-2].gen), NREG, &(yyvsp[0].gen));
	}
#line 2143 "a.tab.c" /* yacc.c:1646  */
    break;

  case 84:
#line 438 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-5].lval), &(yyvsp[-4].gen), (yyvsp[-2].lval), &(yyvsp[0].gen));
	}
#line 2151 "a.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 445 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-5].lval), &(yyvsp[-4].gen), (yyvsp[-2].lval), &(yyvsp[0].gen));
	}
#line 2159 "a.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 449 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-5].lval), &(yyvsp[-4].gen), (yyvsp[-2].lval), &(yyvsp[0].gen));
	}
#line 2167 "a.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 453 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-5].lval), &(yyvsp[-4].gen), (yyvsp[-2].lval), &(yyvsp[0].gen));
	}
#line 2175 "a.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 460 "a.y" /* yacc.c:1646  */
    {
		outcode((yyvsp[-1].lval), &nullgen, NREG, &nullgen);
	}
#line 2183 "a.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 466 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_BRANCH;
		(yyval.gen).offset = (yyvsp[-3].lval) + pc;
	}
#line 2193 "a.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 472 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		if(pass == 2)
			yyerror("undefined label: %s", (yyvsp[-1].sym)->name);
		(yyval.gen).type = D_BRANCH;
		(yyval.gen).sym = (yyvsp[-1].sym);
		(yyval.gen).offset = (yyvsp[0].lval);
	}
#line 2206 "a.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 481 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_BRANCH;
		(yyval.gen).sym = (yyvsp[-1].sym);
		(yyval.gen).offset = (yyvsp[-1].sym)->value + (yyvsp[0].lval);
	}
#line 2217 "a.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 490 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_REG;
		(yyval.gen).reg = (yyvsp[0].lval);
	}
#line 2227 "a.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 498 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_PREG;
		(yyval.gen).reg = (yyvsp[0].lval);
	}
#line 2237 "a.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 506 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_PREG;
		(yyval.gen).reg = (yyvsp[0].lval);
	}
#line 2247 "a.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 514 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_PREG;
		(yyval.gen).reg = (yyvsp[0].lval);
	}
#line 2257 "a.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 522 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_CREG;
		(yyval.gen).reg = (yyvsp[0].lval);
	}
#line 2267 "a.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 528 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_CREG;
		(yyval.gen).reg = (yyvsp[-1].lval);
	}
#line 2277 "a.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 536 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_FREG;
		(yyval.gen).reg = (yyvsp[0].lval);
	}
#line 2287 "a.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 542 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_FREG;
		(yyval.gen).reg = (yyvsp[-1].lval);
	}
#line 2297 "a.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 550 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = (yyvsp[0].gen);
		(yyval.gen).type = D_CONST;
	}
#line 2306 "a.tab.c" /* yacc.c:1646  */
    break;

  case 101:
#line 555 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_SCONST;
		memcpy((yyval.gen).sval, (yyvsp[0].sval), sizeof((yyval.gen).sval));
	}
#line 2316 "a.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 563 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_FCONST;
		(yyval.gen).dval = (yyvsp[0].dval);
	}
#line 2326 "a.tab.c" /* yacc.c:1646  */
    break;

  case 103:
#line 569 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_FCONST;
		(yyval.gen).dval = -(yyvsp[0].dval);
	}
#line 2336 "a.tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 576 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_CONST;
		(yyval.gen).offset = (yyvsp[0].lval);
	}
#line 2346 "a.tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 585 "a.y" /* yacc.c:1646  */
    {
		if((yyval.lval) < 0 || (yyval.lval) >= NREG)
			print("register value out of range\n");
		(yyval.lval) = (yyvsp[-1].lval);
	}
#line 2356 "a.tab.c" /* yacc.c:1646  */
    break;

  case 107:
#line 593 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_OREG;
		(yyval.gen).reg = (yyvsp[-1].lval);
		(yyval.gen).offset = 0;
	}
#line 2367 "a.tab.c" /* yacc.c:1646  */
    break;

  case 108:
#line 600 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_ASI;
		(yyval.gen).reg = (yyvsp[-3].lval);
		(yyval.gen).offset = (yyvsp[-1].lval);
	}
#line 2378 "a.tab.c" /* yacc.c:1646  */
    break;

  case 109:
#line 607 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_OREG;
		(yyval.gen).reg = (yyvsp[-3].lval);
		(yyval.gen).xreg = (yyvsp[-1].lval);
		(yyval.gen).offset = 0;
	}
#line 2390 "a.tab.c" /* yacc.c:1646  */
    break;

  case 111:
#line 616 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_OREG;
		(yyval.gen).reg = (yyvsp[-1].lval);
		(yyval.gen).offset = (yyvsp[-3].lval);
	}
#line 2401 "a.tab.c" /* yacc.c:1646  */
    break;

  case 112:
#line 625 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_OREG;
		(yyval.gen).name = (yyvsp[-1].lval);
		(yyval.gen).sym = S;
		(yyval.gen).offset = (yyvsp[-3].lval);
	}
#line 2413 "a.tab.c" /* yacc.c:1646  */
    break;

  case 113:
#line 633 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_OREG;
		(yyval.gen).name = (yyvsp[-1].lval);
		(yyval.gen).sym = (yyvsp[-4].sym);
		(yyval.gen).offset = (yyvsp[-3].lval);
	}
#line 2425 "a.tab.c" /* yacc.c:1646  */
    break;

  case 114:
#line 641 "a.y" /* yacc.c:1646  */
    {
		(yyval.gen) = nullgen;
		(yyval.gen).type = D_OREG;
		(yyval.gen).name = D_STATIC;
		(yyval.gen).sym = (yyvsp[-6].sym);
		(yyval.gen).offset = (yyvsp[-3].lval);
	}
#line 2437 "a.tab.c" /* yacc.c:1646  */
    break;

  case 117:
#line 653 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = 0;
	}
#line 2445 "a.tab.c" /* yacc.c:1646  */
    break;

  case 118:
#line 657 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[0].lval);
	}
#line 2453 "a.tab.c" /* yacc.c:1646  */
    break;

  case 119:
#line 661 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = -(yyvsp[0].lval);
	}
#line 2461 "a.tab.c" /* yacc.c:1646  */
    break;

  case 124:
#line 673 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[0].sym)->value;
	}
#line 2469 "a.tab.c" /* yacc.c:1646  */
    break;

  case 125:
#line 677 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = -(yyvsp[0].lval);
	}
#line 2477 "a.tab.c" /* yacc.c:1646  */
    break;

  case 126:
#line 681 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[0].lval);
	}
#line 2485 "a.tab.c" /* yacc.c:1646  */
    break;

  case 127:
#line 685 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = ~(yyvsp[0].lval);
	}
#line 2493 "a.tab.c" /* yacc.c:1646  */
    break;

  case 128:
#line 689 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-1].lval);
	}
#line 2501 "a.tab.c" /* yacc.c:1646  */
    break;

  case 130:
#line 696 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) + (yyvsp[0].lval);
	}
#line 2509 "a.tab.c" /* yacc.c:1646  */
    break;

  case 131:
#line 700 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) - (yyvsp[0].lval);
	}
#line 2517 "a.tab.c" /* yacc.c:1646  */
    break;

  case 132:
#line 704 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) * (yyvsp[0].lval);
	}
#line 2525 "a.tab.c" /* yacc.c:1646  */
    break;

  case 133:
#line 708 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) / (yyvsp[0].lval);
	}
#line 2533 "a.tab.c" /* yacc.c:1646  */
    break;

  case 134:
#line 712 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) % (yyvsp[0].lval);
	}
#line 2541 "a.tab.c" /* yacc.c:1646  */
    break;

  case 135:
#line 716 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-3].lval) << (yyvsp[0].lval);
	}
#line 2549 "a.tab.c" /* yacc.c:1646  */
    break;

  case 136:
#line 720 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-3].lval) >> (yyvsp[0].lval);
	}
#line 2557 "a.tab.c" /* yacc.c:1646  */
    break;

  case 137:
#line 724 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) & (yyvsp[0].lval);
	}
#line 2565 "a.tab.c" /* yacc.c:1646  */
    break;

  case 138:
#line 728 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) ^ (yyvsp[0].lval);
	}
#line 2573 "a.tab.c" /* yacc.c:1646  */
    break;

  case 139:
#line 732 "a.y" /* yacc.c:1646  */
    {
		(yyval.lval) = (yyvsp[-2].lval) | (yyvsp[0].lval);
	}
#line 2581 "a.tab.c" /* yacc.c:1646  */
    break;


#line 2585 "a.tab.c" /* yacc.c:1646  */
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
