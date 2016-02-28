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
#line 1 "cc.y" /* yacc.c:339  */

#include "cc.h"

#line 70 "cc.tab.c" /* yacc.c:339  */

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
   by #include "cc.tab.h".  */
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
#line 4 "cc.y" /* yacc.c:355  */

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

#line 210 "cc.tab.c" /* yacc.c:355  */
};
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_CC_TAB_H_INCLUDED  */

/* Copy the second part of user declarations.  */

#line 225 "cc.tab.c" /* yacc.c:358  */

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
#define YYLAST   1202

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  98
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  75
/* YYNRULES -- Number of rules.  */
#define YYNRULES  246
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  412

/* YYTRANSLATE[YYX] -- Symbol number corresponding to YYX as returned
   by yylex, with out-of-bounds checking.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   328

#define YYTRANSLATE(YYX)                                                \
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, without out-of-bounds checking.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    96,     2,     2,     2,    35,    22,     2,
      41,    92,    33,    31,     4,    32,    39,    34,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    17,     3,
      25,     5,    26,    16,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    40,     2,    93,    21,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    94,    20,    95,    97,     2,     2,     2,
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
       2,     2,     2,     2,     2,     2,     1,     2,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    18,    19,
      23,    24,    27,    28,    29,    30,    36,    37,    38,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91
};

#if YYDEBUG
  /* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,    69,    69,    70,    76,    80,    82,    96,    81,   111,
     116,   115,   123,   126,   127,   134,   135,   139,   143,   152,
     156,   162,   168,   167,   179,   192,   193,   196,   200,   207,
     206,   212,   211,   218,   222,   225,   229,   232,   237,   241,
     250,   253,   256,   261,   266,   269,   270,   274,   280,   284,
     288,   294,   295,   301,   305,   310,   313,   314,   318,   319,
     325,   326,   327,   333,   336,   343,   344,   349,   354,   358,
     364,   374,   377,   381,   387,   388,   394,   398,   402,   408,
     412,   413,   419,   420,   426,   427,   427,   438,   444,   452,
     452,   463,   467,   471,   476,   490,   494,   498,   502,   506,
     512,   515,   518,   521,   524,   531,   532,   538,   539,   543,
     547,   551,   555,   559,   563,   567,   571,   575,   579,   583,
     587,   591,   595,   599,   603,   607,   611,   615,   619,   623,
     627,   631,   635,   639,   643,   647,   651,   655,   661,   662,
     669,   677,   678,   682,   686,   690,   694,   698,   702,   706,
     710,   714,   720,   724,   730,   736,   744,   748,   753,   758,
     762,   766,   767,   774,   781,   788,   795,   802,   809,   816,
     823,   824,   827,   837,   855,   865,   883,   886,   889,   890,
     897,   896,   919,   923,   926,   931,   936,   942,   950,   956,
     962,   968,   976,   984,   991,   997,   996,  1008,  1016,  1022,
    1021,  1033,  1041,  1050,  1054,  1049,  1071,  1070,  1079,  1085,
    1086,  1092,  1095,  1101,  1102,  1103,  1106,  1107,  1113,  1114,
    1117,  1121,  1125,  1126,  1129,  1130,  1131,  1132,  1133,  1134,
    1135,  1136,  1137,  1140,  1141,  1142,  1143,  1144,  1145,  1146,
    1149,  1150,  1151,  1154,  1169,  1181,  1182
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || 0
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "';'", "','", "'='", "LPE", "LME",
  "LMLE", "LDVE", "LMDE", "LRSHE", "LLSHE", "LANDE", "LXORE", "LORE",
  "'?'", "':'", "LOROR", "LANDAND", "'|'", "'^'", "'&'", "LEQ", "LNE",
  "'<'", "'>'", "LLE", "LGE", "LLSH", "LRSH", "'+'", "'-'", "'*'", "'/'",
  "'%'", "LMM", "LPP", "LMG", "'.'", "'['", "'('", "LNAME", "LTYPE",
  "LFCONST", "LDCONST", "LCONST", "LLCONST", "LUCONST", "LULCONST",
  "LVLCONST", "LUVLCONST", "LSTRING", "LLSTRING", "LAUTO", "LBREAK",
  "LCASE", "LCHAR", "LCONTINUE", "LDEFAULT", "LDO", "LDOUBLE", "LELSE",
  "LEXTERN", "LFLOAT", "LFOR", "LGOTO", "LIF", "LINT", "LLONG",
  "LREGISTER", "LRETURN", "LSHORT", "LSIZEOF", "LUSED", "LSTATIC",
  "LSTRUCT", "LSWITCH", "LTYPEDEF", "LTYPESTR", "LUNION", "LUNSIGNED",
  "LWHILE", "LVOID", "LENUM", "LSIGNED", "LCONSTNT", "LVOLATILE", "LSET",
  "LSIGNOF", "LRESTRICT", "LINLINE", "')'", "']'", "'{'", "'}'", "'!'",
  "'~'", "$accept", "prog", "xdecl", "$@1", "$@2", "xdlist", "$@3",
  "xdecor", "xdecor2", "adecl", "adlist", "$@4", "pdecl", "pdlist",
  "edecl", "$@5", "$@6", "zedlist", "edlist", "edecor", "abdecor",
  "abdecor1", "abdecor2", "abdecor3", "init", "qual", "qlist", "ilist",
  "zarglist", "arglist", "block", "slist", "labels", "label", "stmnt",
  "forexpr", "ulstmnt", "$@7", "$@8", "zcexpr", "zexpr", "lexpr", "cexpr",
  "expr", "xuexpr", "uexpr", "pexpr", "string", "lstring", "zelist",
  "elist", "sbody", "@9", "zctlist", "types", "tlist", "ctlist", "complex",
  "$@10", "$@11", "$@12", "$@13", "$@14", "gctnlist", "zgnlist", "gctname",
  "gcnlist", "gcname", "enum", "tname", "cname", "gname", "name", "tag",
  "ltag", YY_NULLPTR
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[NUM] -- (External) token number corresponding to the
   (internal) symbol number NUM (which must be that of a token).  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,    59,    44,    61,   258,   259,   260,   261,
     262,   263,   264,   265,   266,   267,    63,    58,   268,   269,
     124,    94,    38,   270,   271,    60,    62,   272,   273,   274,
     275,    43,    45,    42,    47,    37,   276,   277,   278,    46,
      91,    40,   279,   280,   281,   282,   283,   284,   285,   286,
     287,   288,   289,   290,   291,   292,   293,   294,   295,   296,
     297,   298,   299,   300,   301,   302,   303,   304,   305,   306,
     307,   308,   309,   310,   311,   312,   313,   314,   315,   316,
     317,   318,   319,   320,   321,   322,   323,   324,   325,   326,
     327,   328,    41,    93,   123,   125,    33,   126
};
# endif

#define YYPACT_NINF -337

#define yypact_value_is_default(Yystate) \
  (!!((Yystate) == (-337)))

#define YYTABLE_NINF -204

#define yytable_value_is_error(Yytable_value) \
  0

  /* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
     STATE-NUM.  */
static const yytype_int16 yypact[] =
{
    -337,   541,  -337,  -337,  -337,  -337,  -337,  -337,  -337,  -337,
    -337,  -337,  -337,  -337,   -19,  -337,  -337,   -19,  -337,  -337,
      29,  -337,  -337,  -337,  -337,  -337,  -337,   294,  -337,  -337,
     963,   928,  -337,   963,  -337,  -337,  -337,  -337,  -337,  -337,
     -77,  -337,   -54,  -337,   -28,  -337,  -337,   127,    45,   280,
      36,  -337,  -337,   963,  -337,  -337,  -337,  -337,  -337,  -337,
     963,   963,   928,    -8,    -8,    54,    61,   236,    67,  -337,
     127,  -337,   158,   745,   850,  -337,     2,   963,   889,  -337,
    -337,  -337,  -337,   166,     3,  -337,  -337,  -337,  -337,  -337,
     193,   928,   168,   745,   745,   745,   745,   745,   745,   607,
    -337,  -337,  -337,  -337,  -337,  -337,  -337,  -337,  -337,  -337,
    -337,   778,   813,   745,   745,    68,  -337,  1081,  -337,  -337,
     697,   121,   178,  -337,   172,   141,   231,   124,  -337,  -337,
    -337,   308,   745,    54,  -337,    54,   143,   127,   677,  -337,
    1081,  -337,  -337,  -337,  -337,  -337,  -337,    52,  1081,   156,
     607,  -337,   607,  -337,  -337,  -337,  -337,   745,   745,   745,
     745,   745,   745,   745,   745,   745,   745,   745,   745,   745,
     745,   745,   745,   745,   745,   745,   745,   745,   745,   745,
     745,   745,   745,   745,   745,   745,   745,  -337,  -337,   145,
     145,   745,   745,  -337,  -337,   205,  -337,   850,  -337,   745,
     142,  -337,  -337,  -337,   151,  -337,   308,   745,  -337,   235,
     242,  -337,   237,  1081,  -337,    10,  -337,  -337,  -337,   199,
     145,   745,   244,   250,   677,   163,   745,  -337,  -337,   -14,
     169,   156,   156,  1081,  1081,  1081,  1081,  1081,  1081,  1081,
    1081,  1081,  1081,  1081,    42,  1098,  1114,  1129,  1143,  1156,
    1167,  1167,   428,   428,   428,   428,   194,   194,   285,   285,
    -337,  -337,  -337,  -337,  -337,    11,  1081,   171,   262,  -337,
    -337,   209,   167,  -337,   175,   745,   850,   269,  -337,  -337,
     308,   745,  -337,   337,  -337,   127,  -337,   180,  -337,  -337,
     272,   250,  -337,  -337,   153,   710,   188,   189,   745,  -337,
    -337,   745,  -337,  -337,  -337,   195,   200,  -337,  -337,  -337,
     288,   277,   299,   745,   300,   289,   434,   145,   266,   745,
     271,   274,   283,   290,  -337,  -337,   503,  -337,  -337,  -337,
     143,   248,   327,   328,   301,  -337,  -337,  -337,   677,  -337,
    -337,  -337,   420,  -337,  -337,  -337,  -337,  -337,  -337,  1050,
    -337,  -337,   257,   343,   745,   345,   745,   745,   745,   745,
    -337,  -337,  -337,   311,  -337,  -337,   348,   203,   259,  -337,
     315,  -337,    57,  -337,   265,    60,    64,   270,   607,   353,
    -337,   127,  -337,   745,   434,   357,   434,   434,   358,   360,
    -337,   127,   168,  -337,    66,   302,  -337,  -337,  -337,  -337,
     745,   362,  -337,   364,   434,   368,  -337,  -337,   745,   284,
     434,  -337
};

  /* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
     Performed when YYTABLE does not specify something else to do.  Zero
     means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,   182,     1,   208,   233,   224,   231,   235,   230,   226,
     227,   238,   225,   234,     0,   236,   237,     0,   229,   232,
       0,   228,   240,   241,   242,   239,     3,     0,   193,   183,
     184,   186,   216,   185,   219,   218,   245,   246,   180,   197,
     194,   201,   198,   206,   202,     4,   211,     0,     0,     6,
      13,    15,   244,   187,   209,   213,   215,   214,   211,   217,
     190,   188,     0,     0,     0,     0,     0,     0,     0,     5,
       0,    25,     0,   102,    63,   210,   189,   191,     0,   192,
      29,   196,   200,   220,     0,   204,    14,   212,    16,    12,
       9,     7,     0,     0,     0,     0,     0,     0,     0,     0,
     243,   167,   166,   162,   163,   164,   165,   168,   169,   172,
     174,     0,     0,     0,     0,     0,   103,   104,   107,   138,
     141,   170,   171,   161,     0,     0,    64,    40,    65,   181,
      31,    33,     0,   222,   207,     0,     0,     0,     0,    11,
      51,   143,   144,   145,   142,   149,   148,     0,   105,    40,
       0,   150,     0,   151,   146,   147,    18,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   160,   159,     0,
       0,     0,   176,   173,   175,     0,    17,     0,   211,   102,
       0,    67,    66,    41,    44,    45,    33,     0,    37,     0,
      34,    35,    15,   221,   223,     0,    71,     8,    27,     0,
       0,     0,    61,    58,    60,     0,     0,   152,   211,     0,
       0,    40,    40,   127,   128,   129,   130,   131,   132,   134,
     133,   135,   136,   137,     0,   125,   124,   123,   122,   121,
     119,   120,   115,   116,   117,   118,   114,   113,   111,   112,
     108,   109,   110,   157,   158,     0,   178,     0,   177,    68,
      69,    42,     0,    48,     0,   102,    63,     0,    39,    30,
       0,     0,   205,     0,    26,     0,    54,     0,    56,    55,
      62,    59,    52,   106,    42,     0,     0,     0,     0,   156,
     155,     0,    43,    49,    50,     0,     0,    32,    36,    38,
       0,   243,     0,     0,     0,     0,     0,     0,     0,   100,
       0,     0,     0,     0,    70,    72,    85,    74,    73,    80,
       0,     0,     0,   101,     0,    28,    53,    57,     0,   139,
     153,   154,   126,   179,    47,    46,    79,    78,    95,     0,
      96,    77,     0,     0,     0,     0,   176,     0,     0,   176,
      75,    81,    86,     0,    84,    19,    21,     0,     0,    76,
       0,    97,     0,    93,     0,     0,     0,     0,   100,     0,
      20,     0,   140,     0,     0,     0,     0,     0,     0,     0,
      82,     0,     0,    24,     0,    87,    98,    94,    91,    99,
     100,    83,    23,     0,     0,     0,    92,    88,   100,     0,
       0,    90
};

  /* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -337,  -337,  -337,  -337,  -337,   305,  -337,   -26,  -337,  -337,
    -336,  -337,  -337,    87,  -337,  -337,  -337,   223,   119,  -337,
    -129,  -187,  -337,  -337,   -82,   206,  -337,   126,   192,   275,
     139,  -337,  -337,   147,  -304,  -337,   148,  -337,  -337,  -228,
    -181,  -182,   -83,   -45,   -63,   111,  -337,  -337,  -337,  -302,
     176,    46,  -337,  -337,    -1,    -4,   -88,   457,  -337,  -337,
    -337,  -337,  -337,     5,   -47,    20,  -337,   460,   -73,   256,
     268,   -24,   -52,  -127,   -12
};

  /* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    26,    71,   136,    48,    72,   208,    50,   325,
     367,   379,    91,   219,    78,   131,   206,   209,   210,   211,
     202,   203,   204,   205,   222,   223,   224,   225,   125,   126,
     217,   283,   326,   327,   328,   389,   329,   330,   331,   332,
     115,   116,   333,   148,   118,   119,   120,   121,   122,   267,
     268,    39,    62,    27,    79,   127,    29,    30,    63,    64,
      66,   135,    65,    53,    67,    54,    31,    32,    84,    33,
      34,    35,   123,    51,    52
};

  /* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
     positive, shift that token.  If negative, reduce the rule whose
     number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      28,    49,    40,   137,   212,    42,    57,   133,    44,    57,
     139,    76,   352,   274,   133,   226,   147,  -195,   272,   228,
     230,    68,   128,    36,    37,   278,   199,   229,   117,    57,
     141,   142,   143,   144,   145,   146,    57,    57,    61,   287,
    -199,    86,   274,    87,    90,   393,   226,   140,    69,    70,
     154,   155,    87,    57,   374,   401,   226,   377,    80,   298,
     214,   226,   215,    41,   226,    77,  -203,   147,   226,   147,
     226,    36,    37,    75,   130,    38,    73,    74,   273,   212,
     395,    75,   397,   398,   302,   244,    38,   213,    22,    23,
      28,   355,    24,   140,   305,   149,    83,    75,   134,   309,
     407,   201,   296,   297,   299,   282,   411,   302,   265,    81,
      82,   218,   233,   234,   235,   236,   237,   238,   239,   240,
     241,   242,   243,    43,   245,   246,   247,   248,   249,   250,
     251,   252,   253,   254,   255,   256,   257,   258,   259,   260,
     261,   262,   290,   293,   227,   128,   231,   266,   232,   384,
     390,   271,   386,   212,   117,    85,   387,   198,   403,    88,
      46,   156,   117,    92,   199,   200,    36,    37,    47,    36,
      37,   132,   405,   193,    68,   198,   117,   263,   264,   140,
     409,   294,   199,   200,    36,    37,   228,    36,    37,   228,
      93,   275,   276,   199,   229,   334,   199,   229,   -10,    94,
      95,    96,   284,   285,    97,    98,   380,   381,   286,    99,
     100,   195,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,   151,   153,   128,   182,   183,   184,   185,   186,
     117,   194,   339,   196,   273,   197,   117,   216,   279,    22,
      23,   111,   198,    24,   269,    86,   280,    87,   288,   199,
     200,    36,    37,   342,   281,   289,   266,   112,   292,   218,
     303,   295,   138,   300,   113,   114,   301,   304,   349,    46,
      87,   372,   307,   336,   375,   376,   337,    47,    36,    37,
     340,   341,    28,    -9,    -9,   -10,    55,    60,   344,    55,
     391,   346,   345,   140,   347,    22,    23,    45,    56,    24,
     394,    56,   348,   350,   365,   353,   351,   354,   366,    55,
     402,   266,   356,   363,   266,   357,    55,    55,   184,   185,
     186,    56,    22,    23,   358,   207,    24,    46,    56,    56,
     364,   359,   226,    55,    46,    47,    36,    37,   310,   370,
    -100,    46,    47,    36,    37,    56,   371,   140,   373,    47,
      36,    37,   378,   -22,   382,   366,   383,   385,   392,    93,
     396,   399,   388,   400,   404,   366,   381,   406,    94,    95,
      96,   408,   335,    97,    98,    89,   410,    28,    99,   311,
       3,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,     4,   312,   313,     5,   314,   315,   316,     6,   308,
       7,     8,   -89,   317,   318,     9,    10,    11,   319,    12,
     111,   320,    13,    14,   321,    15,    16,    17,    18,   322,
      19,    20,    21,    22,    23,   323,   112,    24,    25,   277,
     291,   -85,   324,   113,   114,   310,   168,  -100,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,    93,   180,   181,   182,
     183,   184,   185,   186,   368,    94,    95,    96,   306,   362,
      97,    98,   270,   360,   361,    99,   311,   343,   101,   102,
     103,   104,   105,   106,   107,   108,   109,   110,    58,   312,
     313,    59,   314,   315,   316,     0,     0,     0,     0,   -89,
     317,   318,     0,     0,     0,   319,  -100,   111,   320,     0,
       0,   321,     0,     0,     0,     0,   322,     0,     0,     0,
       0,     0,   323,   112,     0,    93,     0,     0,   -85,     0,
     113,   114,     0,     0,    94,    95,    96,     0,     0,    97,
      98,     2,     0,     0,    99,   311,     0,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,     0,   312,   313,
       0,   314,   315,   316,     0,     0,     0,     0,   -89,   317,
     318,     0,     0,     0,   319,     0,   111,   320,     0,     0,
     321,     0,     0,     0,     3,   322,     0,     0,     0,     0,
       0,   323,   112,     0,     0,     4,     0,     0,     5,   113,
     114,     0,     6,     0,     7,     8,     0,     0,     0,     9,
      10,    11,     0,    12,     0,     0,    13,    14,     0,    15,
      16,    17,    18,     0,    19,    20,    21,    22,    23,    93,
       0,    24,    25,     0,     0,     0,     0,     0,    94,    95,
      96,     0,     0,    97,    98,     0,     0,     0,    99,   100,
       3,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,     4,     0,     0,     5,     0,     0,     0,     6,     0,
       7,     8,     0,     0,     0,     9,    10,    11,     0,    12,
     111,     0,    13,    14,     0,    15,    16,    17,    18,     0,
      19,    20,    21,    22,    23,     0,   112,    24,    25,    93,
       0,     0,     0,   113,   114,     0,     0,     0,    94,    95,
      96,     0,     0,    97,    98,     0,   220,   221,    99,   100,
       0,   101,   102,   103,   104,   105,   106,   107,   108,   109,
     110,     0,    93,   187,   188,   189,   190,   191,   192,     0,
       0,    94,    95,    96,     0,     0,    97,    98,     0,     0,
     111,    99,   100,     0,   101,   102,   103,   104,   105,   106,
     107,   108,   109,   110,     0,     0,   112,    93,     0,     0,
       0,   138,     0,   113,   114,     0,    94,    95,    96,     0,
       0,    97,    98,   111,     0,     0,    99,   100,     0,   101,
     102,   103,   104,   105,   106,   107,   108,   109,   110,   112,
      93,     0,     0,     0,   338,     0,   113,   114,     0,    94,
      95,    96,     0,     0,    97,    98,     0,     0,   111,   150,
     100,     0,   101,   102,   103,   104,   105,   106,   107,   108,
     109,   110,     0,     0,   112,    93,     0,     0,     0,     0,
       0,   113,   114,     0,    94,    95,    96,     0,     0,    97,
      98,   111,     0,     0,   152,   100,     0,   101,   102,   103,
     104,   105,   106,   107,   108,   109,   110,   112,     0,     0,
       0,     0,     0,     0,   113,   114,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,   111,     0,     0,   124,
       0,     0,   100,     3,     0,     0,     0,     0,     0,     0,
       0,     0,   112,     0,     4,     0,     0,     5,     0,   113,
     114,     6,     0,     7,     8,     0,     0,     0,     9,    10,
      11,     0,    12,     0,     0,    13,    14,     0,    15,    16,
      17,    18,     3,    19,    20,    21,    22,    23,     0,     0,
      24,    25,     0,     4,     0,     0,     5,     0,     0,     0,
       6,     0,     7,     8,     0,     0,     0,     9,    10,    11,
       0,    12,     0,     0,    13,    14,     0,    15,    16,    17,
      18,     3,    19,    20,    21,    22,    23,     0,     0,    24,
      25,     0,     4,     0,   129,     5,     0,     0,     0,     6,
       0,     7,     8,     0,     0,     0,     9,    10,    11,     0,
      12,     0,     0,    13,    14,     0,    15,    16,    17,    18,
       0,    19,    20,    21,    22,    23,     0,     4,    24,    25,
       5,     0,     0,     0,     6,     0,     7,     8,     0,     0,
       0,     9,    10,    11,     0,    12,     0,     0,    13,     0,
       0,    15,    16,     0,    18,     0,    19,     0,    21,    22,
      23,     0,     0,    24,    25,   157,   158,   159,   160,   161,
     162,   163,   164,   165,   166,   167,   168,   369,   169,   170,
     171,   172,   173,   174,   175,   176,   177,   178,   179,   180,
     181,   182,   183,   184,   185,   186,   157,   158,   159,   160,
     161,   162,   163,   164,   165,   166,   167,   168,     0,   169,
     170,   171,   172,   173,   174,   175,   176,   177,   178,   179,
     180,   181,   182,   183,   184,   185,   186,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     183,   184,   185,   186,   171,   172,   173,   174,   175,   176,
     177,   178,   179,   180,   181,   182,   183,   184,   185,   186,
     172,   173,   174,   175,   176,   177,   178,   179,   180,   181,
     182,   183,   184,   185,   186,   173,   174,   175,   176,   177,
     178,   179,   180,   181,   182,   183,   184,   185,   186,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   176,   177,   178,   179,   180,   181,   182,   183,
     184,   185,   186
};

static const yytype_int16 yycheck[] =
{
       1,    27,    14,    91,   131,    17,    30,     4,    20,    33,
      92,    58,   316,   200,     4,     4,    99,    94,   199,    33,
     149,    47,    74,    42,    43,   207,    40,    41,    73,    53,
      93,    94,    95,    96,    97,    98,    60,    61,    33,   221,
      94,    67,   229,    67,    70,   381,     4,    92,     3,     4,
     113,   114,    76,    77,   356,   391,     4,   359,    62,    17,
     133,     4,   135,    17,     4,    60,    94,   150,     4,   152,
       4,    42,    43,    53,    78,    94,    40,    41,    92,   206,
     384,    61,   386,   387,   271,   168,    94,   132,    86,    87,
      91,   319,    90,   138,   275,    99,    42,    77,    95,   281,
     404,   127,   231,   232,    93,    95,   410,   294,   191,    63,
      64,   137,   157,   158,   159,   160,   161,   162,   163,   164,
     165,   166,   167,    94,   169,   170,   171,   172,   173,   174,
     175,   176,   177,   178,   179,   180,   181,   182,   183,   184,
     185,   186,   224,   226,    92,   197,   150,   192,   152,    92,
     378,   198,    92,   280,   199,    94,    92,    33,    92,    92,
      33,    93,   207,     5,    40,    41,    42,    43,    41,    42,
      43,     5,   400,    52,   200,    33,   221,   189,   190,   224,
     408,   228,    40,    41,    42,    43,    33,    42,    43,    33,
      22,    40,    41,    40,    41,   283,    40,    41,     5,    31,
      32,    33,     3,     4,    36,    37,     3,     4,   220,    41,
      42,    39,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,   111,   112,   276,    31,    32,    33,    34,    35,
     275,    53,   295,    92,    92,     4,   281,    94,     3,    86,
      87,    73,    33,    90,    39,   271,     4,   271,     4,    40,
      41,    42,    43,   298,    17,     5,   301,    89,    95,   285,
      93,    92,    94,    92,    96,    97,     4,    92,   313,    33,
     294,   354,     3,    93,   357,   358,     4,    41,    42,    43,
      92,    92,   283,     3,     4,     5,    30,    31,    93,    33,
     378,     3,    92,   338,    17,    86,    87,     3,    30,    90,
     383,    33,     3,     3,     3,   317,    17,    41,   334,    53,
     392,   356,    41,    65,   359,    41,    60,    61,    33,    34,
      35,    53,    86,    87,    41,    17,    90,    33,    60,    61,
       3,    41,     4,    77,    33,    41,    42,    43,     1,    82,
       3,    33,    41,    42,    43,    77,     3,   392,     3,    41,
      42,    43,    41,     5,    95,   381,    41,    92,     5,    22,
       3,     3,    92,     3,    62,   391,     4,     3,    31,    32,
      33,     3,   285,    36,    37,    70,    92,   378,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    55,    56,    57,    58,    59,    60,    61,   280,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,   206,
     224,    94,    95,    96,    97,     1,    16,     3,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,    22,    29,    30,    31,
      32,    33,    34,    35,   338,    31,    32,    33,   276,   330,
      36,    37,   197,   326,   326,    41,    42,   301,    44,    45,
      46,    47,    48,    49,    50,    51,    52,    53,    31,    55,
      56,    31,    58,    59,    60,    -1,    -1,    -1,    -1,    65,
      66,    67,    -1,    -1,    -1,    71,     3,    73,    74,    -1,
      -1,    77,    -1,    -1,    -1,    -1,    82,    -1,    -1,    -1,
      -1,    -1,    88,    89,    -1,    22,    -1,    -1,    94,    -1,
      96,    97,    -1,    -1,    31,    32,    33,    -1,    -1,    36,
      37,     0,    -1,    -1,    41,    42,    -1,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    -1,    55,    56,
      -1,    58,    59,    60,    -1,    -1,    -1,    -1,    65,    66,
      67,    -1,    -1,    -1,    71,    -1,    73,    74,    -1,    -1,
      77,    -1,    -1,    -1,    43,    82,    -1,    -1,    -1,    -1,
      -1,    88,    89,    -1,    -1,    54,    -1,    -1,    57,    96,
      97,    -1,    61,    -1,    63,    64,    -1,    -1,    -1,    68,
      69,    70,    -1,    72,    -1,    -1,    75,    76,    -1,    78,
      79,    80,    81,    -1,    83,    84,    85,    86,    87,    22,
      -1,    90,    91,    -1,    -1,    -1,    -1,    -1,    31,    32,
      33,    -1,    -1,    36,    37,    -1,    -1,    -1,    41,    42,
      43,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    54,    -1,    -1,    57,    -1,    -1,    -1,    61,    -1,
      63,    64,    -1,    -1,    -1,    68,    69,    70,    -1,    72,
      73,    -1,    75,    76,    -1,    78,    79,    80,    81,    -1,
      83,    84,    85,    86,    87,    -1,    89,    90,    91,    22,
      -1,    -1,    -1,    96,    97,    -1,    -1,    -1,    31,    32,
      33,    -1,    -1,    36,    37,    -1,    39,    40,    41,    42,
      -1,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    -1,    22,    36,    37,    38,    39,    40,    41,    -1,
      -1,    31,    32,    33,    -1,    -1,    36,    37,    -1,    -1,
      73,    41,    42,    -1,    44,    45,    46,    47,    48,    49,
      50,    51,    52,    53,    -1,    -1,    89,    22,    -1,    -1,
      -1,    94,    -1,    96,    97,    -1,    31,    32,    33,    -1,
      -1,    36,    37,    73,    -1,    -1,    41,    42,    -1,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    89,
      22,    -1,    -1,    -1,    94,    -1,    96,    97,    -1,    31,
      32,    33,    -1,    -1,    36,    37,    -1,    -1,    73,    41,
      42,    -1,    44,    45,    46,    47,    48,    49,    50,    51,
      52,    53,    -1,    -1,    89,    22,    -1,    -1,    -1,    -1,
      -1,    96,    97,    -1,    31,    32,    33,    -1,    -1,    36,
      37,    73,    -1,    -1,    41,    42,    -1,    44,    45,    46,
      47,    48,    49,    50,    51,    52,    53,    89,    -1,    -1,
      -1,    -1,    -1,    -1,    96,    97,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    73,    -1,    -1,    39,
      -1,    -1,    42,    43,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    89,    -1,    54,    -1,    -1,    57,    -1,    96,
      97,    61,    -1,    63,    64,    -1,    -1,    -1,    68,    69,
      70,    -1,    72,    -1,    -1,    75,    76,    -1,    78,    79,
      80,    81,    43,    83,    84,    85,    86,    87,    -1,    -1,
      90,    91,    -1,    54,    -1,    -1,    57,    -1,    -1,    -1,
      61,    -1,    63,    64,    -1,    -1,    -1,    68,    69,    70,
      -1,    72,    -1,    -1,    75,    76,    -1,    78,    79,    80,
      81,    43,    83,    84,    85,    86,    87,    -1,    -1,    90,
      91,    -1,    54,    -1,    95,    57,    -1,    -1,    -1,    61,
      -1,    63,    64,    -1,    -1,    -1,    68,    69,    70,    -1,
      72,    -1,    -1,    75,    76,    -1,    78,    79,    80,    81,
      -1,    83,    84,    85,    86,    87,    -1,    54,    90,    91,
      57,    -1,    -1,    -1,    61,    -1,    63,    64,    -1,    -1,
      -1,    68,    69,    70,    -1,    72,    -1,    -1,    75,    -1,
      -1,    78,    79,    -1,    81,    -1,    83,    -1,    85,    86,
      87,    -1,    -1,    90,    91,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,    18,    19,
      20,    21,    22,    23,    24,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    35,     5,     6,     7,     8,
       9,    10,    11,    12,    13,    14,    15,    16,    -1,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    19,    20,    21,
      22,    23,    24,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    20,    21,    22,    23,    24,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    35,
      21,    22,    23,    24,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    35,    22,    23,    24,    25,    26,
      27,    28,    29,    30,    31,    32,    33,    34,    35,    23,
      24,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    35,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35
};

  /* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
     symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,    99,     0,    43,    54,    57,    61,    63,    64,    68,
      69,    70,    72,    75,    76,    78,    79,    80,    81,    83,
      84,    85,    86,    87,    90,    91,   100,   151,   152,   154,
     155,   164,   165,   167,   168,   169,    42,    43,    94,   149,
     172,   149,   172,    94,   172,     3,    33,    41,   103,   105,
     106,   171,   172,   161,   163,   167,   168,   169,   155,   165,
     167,   161,   150,   156,   157,   160,   158,   162,   105,     3,
       4,   101,   104,    40,    41,   163,   162,   161,   112,   152,
     153,   149,   149,    42,   166,    94,   105,   169,    92,   103,
     105,   110,     5,    22,    31,    32,    33,    36,    37,    41,
      42,    44,    45,    46,    47,    48,    49,    50,    51,    52,
      53,    73,    89,    96,    97,   138,   139,   141,   142,   143,
     144,   145,   146,   170,    39,   126,   127,   153,   170,    95,
     153,   113,     5,     4,    95,   159,   102,   154,    94,   122,
     141,   142,   142,   142,   142,   142,   142,   140,   141,   153,
      41,   143,    41,   143,   142,   142,    93,     5,     6,     7,
       8,     9,    10,    11,    12,    13,    14,    15,    16,    18,
      19,    20,    21,    22,    23,    24,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    35,    36,    37,    38,
      39,    40,    41,    52,    53,    39,    92,     4,    33,    40,
      41,   105,   118,   119,   120,   121,   114,    17,   105,   115,
     116,   117,   171,   141,   166,   166,    94,   128,   105,   111,
      39,    40,   122,   123,   124,   125,     4,    92,    33,    41,
     118,   153,   153,   141,   141,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   140,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   141,   141,
     141,   141,   141,   172,   172,   140,   141,   147,   148,    39,
     127,   162,   138,    92,   119,    40,    41,   115,   139,     3,
       4,    17,    95,   129,     3,     4,   172,   139,     4,     5,
     122,   123,    95,   140,   162,    92,   118,   118,    17,    93,
      92,     4,   119,    93,    92,   138,   126,     3,   116,   139,
       1,    42,    55,    56,    58,    59,    60,    66,    67,    71,
      74,    77,    82,    88,    95,   107,   130,   131,   132,   134,
     135,   136,   137,   140,   154,   111,    93,     4,    94,   142,
      92,    92,   141,   148,    93,    92,     3,    17,     3,   141,
       3,    17,   132,   172,    41,   137,    41,    41,    41,    41,
     131,   134,   128,    65,     3,     3,   105,   108,   125,    17,
      82,     3,   140,     3,   147,   140,   140,   147,    41,   109,
       3,     4,    95,    41,    92,    92,    92,    92,    92,   133,
     137,   154,     5,   108,   140,   132,     3,   132,   132,     3,
       3,   108,   122,    92,    62,   137,     3,   132,     3,   137,
      92,   132
};

  /* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    98,    99,    99,   100,   100,   101,   102,   100,   103,
     104,   103,   103,   105,   105,   106,   106,   106,   106,   107,
     107,   108,   109,   108,   108,   110,   110,   111,   111,   113,
     112,   114,   112,   115,   115,   116,   116,   117,   117,   117,
     118,   118,   119,   119,   119,   120,   120,   120,   121,   121,
     121,   122,   122,   123,   123,   123,   124,   124,   124,   124,
     125,   125,   125,   126,   126,   127,   127,   127,   127,   127,
     128,   129,   129,   129,   130,   130,   131,   131,   131,   132,
     132,   132,   133,   133,   134,   135,   134,   134,   134,   136,
     134,   134,   134,   134,   134,   134,   134,   134,   134,   134,
     137,   137,   138,   138,   139,   140,   140,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   141,   141,
     141,   141,   141,   141,   141,   141,   141,   141,   142,   142,
     142,   143,   143,   143,   143,   143,   143,   143,   143,   143,
     143,   143,   144,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   144,   144,   144,   144,   144,   144,   144,   144,
     144,   144,   145,   145,   146,   146,   147,   147,   148,   148,
     150,   149,   151,   151,   152,   152,   152,   152,   152,   152,
     152,   152,   153,   154,   155,   156,   155,   155,   155,   157,
     155,   155,   155,   158,   159,   155,   160,   155,   155,   161,
     161,   162,   162,   163,   163,   163,   164,   164,   165,   165,
     166,   166,   166,   166,   167,   167,   167,   167,   167,   167,
     167,   167,   167,   168,   168,   168,   168,   168,   168,   168,
     169,   169,   169,   170,   171,   172,   172
};

  /* YYR2[YYN] -- Number of symbols on the right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     2,     3,     0,     0,     6,     1,
       0,     4,     3,     1,     3,     1,     3,     4,     4,     2,
       3,     1,     0,     4,     3,     0,     4,     1,     3,     0,
       4,     0,     5,     0,     1,     1,     3,     1,     3,     2,
       0,     1,     2,     3,     1,     1,     4,     4,     2,     3,
       3,     1,     3,     3,     2,     2,     2,     3,     1,     2,
       1,     1,     2,     0,     1,     1,     2,     2,     3,     3,
       3,     0,     2,     2,     1,     2,     3,     2,     2,     2,
       1,     2,     1,     2,     2,     0,     2,     5,     7,     0,
      10,     5,     7,     3,     5,     2,     2,     3,     5,     5,
       0,     1,     0,     1,     1,     1,     3,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     5,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     1,     5,
       7,     1,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     3,     5,     5,     4,     4,     3,     3,     2,
       2,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     2,     1,     2,     0,     1,     1,     3,
       0,     4,     0,     1,     1,     1,     1,     2,     2,     3,
       2,     3,     1,     1,     2,     0,     4,     2,     2,     0,
       4,     2,     2,     0,     0,     7,     0,     5,     1,     1,
       2,     0,     2,     1,     1,     1,     1,     2,     1,     1,
       1,     3,     2,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1
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
#line 77 "cc.y" /* yacc.c:1646  */
    {
		dodecl(xdecl, lastclass, lasttype, Z);
	}
#line 1779 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 6:
#line 82 "cc.y" /* yacc.c:1646  */
    {
		lastdcl = T;
		firstarg = S;
		dodecl(xdecl, lastclass, lasttype, (yyvsp[0].node));
		if(lastdcl == T || lastdcl->etype != TFUNC) {
			diag((yyvsp[0].node), "not a function");
			lastdcl = types[TFUNC];
		}
		thisfn = lastdcl;
		markdcl();
		firstdcl = dclstack;
		argmark((yyvsp[0].node), 0);
	}
#line 1797 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 7:
#line 96 "cc.y" /* yacc.c:1646  */
    {
		argmark((yyvsp[-2].node), 1);
	}
#line 1805 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 8:
#line 100 "cc.y" /* yacc.c:1646  */
    {
		Node *n;

		n = revertdcl();
		if(n)
			(yyvsp[0].node) = new(OLIST, n, (yyvsp[0].node));
		if(!debug['a'] && !debug['Z'])
			codgen((yyvsp[0].node), (yyvsp[-4].node));
	}
#line 1819 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 9:
#line 112 "cc.y" /* yacc.c:1646  */
    {
		dodecl(xdecl, lastclass, lasttype, (yyvsp[0].node));
	}
#line 1827 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 10:
#line 116 "cc.y" /* yacc.c:1646  */
    {
		(yyvsp[0].node) = dodecl(xdecl, lastclass, lasttype, (yyvsp[0].node));
	}
#line 1835 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 11:
#line 120 "cc.y" /* yacc.c:1646  */
    {
		doinit((yyvsp[-3].node)->sym, (yyvsp[-3].node)->type, 0L, (yyvsp[0].node));
	}
#line 1843 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 14:
#line 128 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OIND, (yyvsp[0].node), Z);
		(yyval.node)->garb = simpleg((yyvsp[-1].lval));
	}
#line 1852 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 16:
#line 136 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = (yyvsp[-1].node);
	}
#line 1860 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 17:
#line 140 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OFUNC, (yyvsp[-3].node), (yyvsp[-1].node));
	}
#line 1868 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 18:
#line 144 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OARRAY, (yyvsp[-3].node), (yyvsp[-1].node));
	}
#line 1876 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 19:
#line 153 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = dodecl(adecl, lastclass, lasttype, Z);
	}
#line 1884 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 20:
#line 157 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = (yyvsp[-1].node);
	}
#line 1892 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 21:
#line 163 "cc.y" /* yacc.c:1646  */
    {
		dodecl(adecl, lastclass, lasttype, (yyvsp[0].node));
		(yyval.node) = Z;
	}
#line 1901 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 22:
#line 168 "cc.y" /* yacc.c:1646  */
    {
		(yyvsp[0].node) = dodecl(adecl, lastclass, lasttype, (yyvsp[0].node));
	}
#line 1909 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 23:
#line 172 "cc.y" /* yacc.c:1646  */
    {
		long w;

		w = (yyvsp[-3].node)->sym->type->width;
		(yyval.node) = doinit((yyvsp[-3].node)->sym, (yyvsp[-3].node)->type, 0L, (yyvsp[0].node));
		(yyval.node) = contig((yyvsp[-3].node)->sym, (yyval.node), w);
	}
#line 1921 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 24:
#line 180 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = (yyvsp[-2].node);
		if((yyvsp[0].node) != Z) {
			(yyval.node) = (yyvsp[0].node);
			if((yyvsp[-2].node) != Z)
				(yyval.node) = new(OLIST, (yyvsp[-2].node), (yyvsp[0].node));
		}
	}
#line 1934 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 27:
#line 197 "cc.y" /* yacc.c:1646  */
    {
		dodecl(pdecl, lastclass, lasttype, (yyvsp[0].node));
	}
#line 1942 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 29:
#line 207 "cc.y" /* yacc.c:1646  */
    {
		lasttype = (yyvsp[0].type);
	}
#line 1950 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 31:
#line 212 "cc.y" /* yacc.c:1646  */
    {
		lasttype = (yyvsp[0].type);
	}
#line 1958 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 33:
#line 218 "cc.y" /* yacc.c:1646  */
    {
		lastfield = 0;
		edecl(CXXX, lasttype, S);
	}
#line 1967 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 35:
#line 226 "cc.y" /* yacc.c:1646  */
    {
		dodecl(edecl, CXXX, lasttype, (yyvsp[0].node));
	}
#line 1975 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 37:
#line 233 "cc.y" /* yacc.c:1646  */
    {
		lastbit = 0;
		firstbit = 1;
	}
#line 1984 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 38:
#line 238 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OBIT, (yyvsp[-2].node), (yyvsp[0].node));
	}
#line 1992 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 39:
#line 242 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OBIT, Z, (yyvsp[0].node));
	}
#line 2000 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 40:
#line 250 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = (Z);
	}
#line 2008 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 42:
#line 257 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OIND, (Z), Z);
		(yyval.node)->garb = simpleg((yyvsp[0].lval));
	}
#line 2017 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 43:
#line 262 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OIND, (yyvsp[0].node), Z);
		(yyval.node)->garb = simpleg((yyvsp[-1].lval));
	}
#line 2026 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 46:
#line 271 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OFUNC, (yyvsp[-3].node), (yyvsp[-1].node));
	}
#line 2034 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 47:
#line 275 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OARRAY, (yyvsp[-3].node), (yyvsp[-1].node));
	}
#line 2042 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 48:
#line 281 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OFUNC, (Z), Z);
	}
#line 2050 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 49:
#line 285 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OARRAY, (Z), (yyvsp[-1].node));
	}
#line 2058 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 50:
#line 289 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = (yyvsp[-1].node);
	}
#line 2066 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 52:
#line 296 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OINIT, invert((yyvsp[-1].node)), Z);
	}
#line 2074 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 53:
#line 302 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OARRAY, (yyvsp[-1].node), Z);
	}
#line 2082 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 54:
#line 306 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OELEM, Z, Z);
		(yyval.node)->sym = (yyvsp[0].sym);
	}
#line 2091 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 57:
#line 315 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OLIST, (yyvsp[-2].node), (yyvsp[-1].node));
	}
#line 2099 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 59:
#line 320 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OLIST, (yyvsp[-1].node), (yyvsp[0].node));
	}
#line 2107 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 62:
#line 328 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OLIST, (yyvsp[-1].node), (yyvsp[0].node));
	}
#line 2115 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 63:
#line 333 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = Z;
	}
#line 2123 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 64:
#line 337 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = invert((yyvsp[0].node));
	}
#line 2131 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 66:
#line 345 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OPROTO, (yyvsp[0].node), Z);
		(yyval.node)->type = (yyvsp[-1].type);
	}
#line 2140 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 67:
#line 350 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OPROTO, (yyvsp[0].node), Z);
		(yyval.node)->type = (yyvsp[-1].type);
	}
#line 2149 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 68:
#line 355 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(ODOTDOT, Z, Z);
	}
#line 2157 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 69:
#line 359 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OLIST, (yyvsp[-2].node), (yyvsp[0].node));
	}
#line 2165 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 70:
#line 365 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = invert((yyvsp[-1].node));
	//	if($2 != Z)
	//		$$ = new(OLIST, $2, $$);
		if((yyval.node) == Z)
			(yyval.node) = new(OLIST, Z, Z);
	}
#line 2177 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 71:
#line 374 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = Z;
	}
#line 2185 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 72:
#line 378 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OLIST, (yyvsp[-1].node), (yyvsp[0].node));
	}
#line 2193 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 73:
#line 382 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OLIST, (yyvsp[-1].node), (yyvsp[0].node));
	}
#line 2201 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 75:
#line 389 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OLIST, (yyvsp[-1].node), (yyvsp[0].node));
	}
#line 2209 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 76:
#line 395 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OCASE, (yyvsp[-1].node), Z);
	}
#line 2217 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 77:
#line 399 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OCASE, Z, Z);
	}
#line 2225 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 78:
#line 403 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OLABEL, dcllabel((yyvsp[-1].sym), 1), Z);
	}
#line 2233 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 79:
#line 409 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = Z;
	}
#line 2241 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 81:
#line 414 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OLIST, (yyvsp[-1].node), (yyvsp[0].node));
	}
#line 2249 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 83:
#line 421 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = (yyvsp[0].node);
	}
#line 2257 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 85:
#line 427 "cc.y" /* yacc.c:1646  */
    {
		markdcl();
	}
#line 2265 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 86:
#line 431 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = revertdcl();
		if((yyval.node))
			(yyval.node) = new(OLIST, (yyval.node), (yyvsp[0].node));
		else
			(yyval.node) = (yyvsp[0].node);
	}
#line 2277 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 87:
#line 439 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OIF, (yyvsp[-2].node), new(OLIST, (yyvsp[0].node), Z));
		if((yyvsp[0].node) == Z)
			warn((yyvsp[-2].node), "empty if body");
	}
#line 2287 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 88:
#line 445 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OIF, (yyvsp[-4].node), new(OLIST, (yyvsp[-2].node), (yyvsp[0].node)));
		if((yyvsp[-2].node) == Z)
			warn((yyvsp[-4].node), "empty if body");
		if((yyvsp[0].node) == Z)
			warn((yyvsp[-4].node), "empty else body");
	}
#line 2299 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 89:
#line 452 "cc.y" /* yacc.c:1646  */
    { markdcl(); }
#line 2305 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 90:
#line 453 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = revertdcl();
		if((yyval.node)){
			if((yyvsp[-6].node))
				(yyvsp[-6].node) = new(OLIST, (yyval.node), (yyvsp[-6].node));
			else
				(yyvsp[-6].node) = (yyval.node);
		}
		(yyval.node) = new(OFOR, new(OLIST, (yyvsp[-4].node), new(OLIST, (yyvsp[-6].node), (yyvsp[-2].node))), (yyvsp[0].node));
	}
#line 2320 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 91:
#line 464 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OWHILE, (yyvsp[-2].node), (yyvsp[0].node));
	}
#line 2328 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 92:
#line 468 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(ODWHILE, (yyvsp[-2].node), (yyvsp[-5].node));
	}
#line 2336 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 93:
#line 472 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(ORETURN, (yyvsp[-1].node), Z);
		(yyval.node)->type = thisfn->link;
	}
#line 2345 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 94:
#line 477 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OCONST, Z, Z);
		(yyval.node)->vconst = 0;
		(yyval.node)->type = types[TINT];
		(yyvsp[-2].node) = new(OSUB, (yyval.node), (yyvsp[-2].node));

		(yyval.node) = new(OCONST, Z, Z);
		(yyval.node)->vconst = 0;
		(yyval.node)->type = types[TINT];
		(yyvsp[-2].node) = new(OSUB, (yyval.node), (yyvsp[-2].node));

		(yyval.node) = new(OSWITCH, (yyvsp[-2].node), (yyvsp[0].node));
	}
#line 2363 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 95:
#line 491 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OBREAK, Z, Z);
	}
#line 2371 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 96:
#line 495 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OCONTINUE, Z, Z);
	}
#line 2379 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 97:
#line 499 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OGOTO, dcllabel((yyvsp[-1].sym), 0), Z);
	}
#line 2387 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 98:
#line 503 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OUSED, (yyvsp[-2].node), Z);
	}
#line 2395 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 99:
#line 507 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OSET, (yyvsp[-2].node), Z);
	}
#line 2403 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 100:
#line 512 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = Z;
	}
#line 2411 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 102:
#line 518 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = Z;
	}
#line 2419 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 104:
#line 525 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OCAST, (yyvsp[0].node), Z);
		(yyval.node)->type = types[TLONG];
	}
#line 2428 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 106:
#line 533 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OCOMMA, (yyvsp[-2].node), (yyvsp[0].node));
	}
#line 2436 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 108:
#line 540 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OMUL, (yyvsp[-2].node), (yyvsp[0].node));
	}
#line 2444 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 109:
#line 544 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(ODIV, (yyvsp[-2].node), (yyvsp[0].node));
	}
#line 2452 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 110:
#line 548 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OMOD, (yyvsp[-2].node), (yyvsp[0].node));
	}
#line 2460 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 111:
#line 552 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OADD, (yyvsp[-2].node), (yyvsp[0].node));
	}
#line 2468 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 112:
#line 556 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OSUB, (yyvsp[-2].node), (yyvsp[0].node));
	}
#line 2476 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 113:
#line 560 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OASHR, (yyvsp[-2].node), (yyvsp[0].node));
	}
#line 2484 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 114:
#line 564 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OASHL, (yyvsp[-2].node), (yyvsp[0].node));
	}
#line 2492 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 115:
#line 568 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OLT, (yyvsp[-2].node), (yyvsp[0].node));
	}
#line 2500 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 116:
#line 572 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OGT, (yyvsp[-2].node), (yyvsp[0].node));
	}
#line 2508 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 117:
#line 576 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OLE, (yyvsp[-2].node), (yyvsp[0].node));
	}
#line 2516 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 118:
#line 580 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OGE, (yyvsp[-2].node), (yyvsp[0].node));
	}
#line 2524 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 119:
#line 584 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OEQ, (yyvsp[-2].node), (yyvsp[0].node));
	}
#line 2532 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 120:
#line 588 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(ONE, (yyvsp[-2].node), (yyvsp[0].node));
	}
#line 2540 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 121:
#line 592 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OAND, (yyvsp[-2].node), (yyvsp[0].node));
	}
#line 2548 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 122:
#line 596 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OXOR, (yyvsp[-2].node), (yyvsp[0].node));
	}
#line 2556 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 123:
#line 600 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OOR, (yyvsp[-2].node), (yyvsp[0].node));
	}
#line 2564 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 124:
#line 604 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OANDAND, (yyvsp[-2].node), (yyvsp[0].node));
	}
#line 2572 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 125:
#line 608 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OOROR, (yyvsp[-2].node), (yyvsp[0].node));
	}
#line 2580 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 126:
#line 612 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OCOND, (yyvsp[-4].node), new(OLIST, (yyvsp[-2].node), (yyvsp[0].node)));
	}
#line 2588 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 127:
#line 616 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OAS, (yyvsp[-2].node), (yyvsp[0].node));
	}
#line 2596 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 128:
#line 620 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OASADD, (yyvsp[-2].node), (yyvsp[0].node));
	}
#line 2604 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 129:
#line 624 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OASSUB, (yyvsp[-2].node), (yyvsp[0].node));
	}
#line 2612 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 130:
#line 628 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OASMUL, (yyvsp[-2].node), (yyvsp[0].node));
	}
#line 2620 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 131:
#line 632 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OASDIV, (yyvsp[-2].node), (yyvsp[0].node));
	}
#line 2628 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 132:
#line 636 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OASMOD, (yyvsp[-2].node), (yyvsp[0].node));
	}
#line 2636 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 133:
#line 640 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OASASHL, (yyvsp[-2].node), (yyvsp[0].node));
	}
#line 2644 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 134:
#line 644 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OASASHR, (yyvsp[-2].node), (yyvsp[0].node));
	}
#line 2652 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 135:
#line 648 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OASAND, (yyvsp[-2].node), (yyvsp[0].node));
	}
#line 2660 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 136:
#line 652 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OASXOR, (yyvsp[-2].node), (yyvsp[0].node));
	}
#line 2668 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 137:
#line 656 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OASOR, (yyvsp[-2].node), (yyvsp[0].node));
	}
#line 2676 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 139:
#line 663 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OCAST, (yyvsp[0].node), Z);
		dodecl(NODECL, CXXX, (yyvsp[-3].type), (yyvsp[-2].node));
		(yyval.node)->type = lastdcl;
		(yyval.node)->xcast = 1;
	}
#line 2687 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 140:
#line 670 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OSTRUCT, (yyvsp[-1].node), Z);
		dodecl(NODECL, CXXX, (yyvsp[-5].type), (yyvsp[-4].node));
		(yyval.node)->type = lastdcl;
	}
#line 2697 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 142:
#line 679 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OIND, (yyvsp[0].node), Z);
	}
#line 2705 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 143:
#line 683 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OADDR, (yyvsp[0].node), Z);
	}
#line 2713 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 144:
#line 687 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OPOS, (yyvsp[0].node), Z);
	}
#line 2721 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 145:
#line 691 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(ONEG, (yyvsp[0].node), Z);
	}
#line 2729 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 146:
#line 695 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(ONOT, (yyvsp[0].node), Z);
	}
#line 2737 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 147:
#line 699 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OCOM, (yyvsp[0].node), Z);
	}
#line 2745 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 148:
#line 703 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OPREINC, (yyvsp[0].node), Z);
	}
#line 2753 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 149:
#line 707 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OPREDEC, (yyvsp[0].node), Z);
	}
#line 2761 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 150:
#line 711 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OSIZE, (yyvsp[0].node), Z);
	}
#line 2769 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 151:
#line 715 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OSIGN, (yyvsp[0].node), Z);
	}
#line 2777 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 152:
#line 721 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = (yyvsp[-1].node);
	}
#line 2785 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 153:
#line 725 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OSIZE, Z, Z);
		dodecl(NODECL, CXXX, (yyvsp[-2].type), (yyvsp[-1].node));
		(yyval.node)->type = lastdcl;
	}
#line 2795 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 154:
#line 731 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OSIGN, Z, Z);
		dodecl(NODECL, CXXX, (yyvsp[-2].type), (yyvsp[-1].node));
		(yyval.node)->type = lastdcl;
	}
#line 2805 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 155:
#line 737 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OFUNC, (yyvsp[-3].node), Z);
		if((yyvsp[-3].node)->op == ONAME)
		if((yyvsp[-3].node)->type == T)
			dodecl(xdecl, CXXX, types[TINT], (yyval.node));
		(yyval.node)->right = invert((yyvsp[-1].node));
	}
#line 2817 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 156:
#line 745 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OIND, new(OADD, (yyvsp[-3].node), (yyvsp[-1].node)), Z);
	}
#line 2825 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 157:
#line 749 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(ODOT, new(OIND, (yyvsp[-2].node), Z), Z);
		(yyval.node)->sym = (yyvsp[0].sym);
	}
#line 2834 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 158:
#line 754 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(ODOT, (yyvsp[-2].node), Z);
		(yyval.node)->sym = (yyvsp[0].sym);
	}
#line 2843 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 159:
#line 759 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OPOSTINC, (yyvsp[-1].node), Z);
	}
#line 2851 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 160:
#line 763 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OPOSTDEC, (yyvsp[-1].node), Z);
	}
#line 2859 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 162:
#line 768 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OCONST, Z, Z);
		(yyval.node)->type = types[TINT];
		(yyval.node)->vconst = (yyvsp[0].vval);
		(yyval.node)->cstring = strdup(symb);
	}
#line 2870 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 163:
#line 775 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OCONST, Z, Z);
		(yyval.node)->type = types[TLONG];
		(yyval.node)->vconst = (yyvsp[0].vval);
		(yyval.node)->cstring = strdup(symb);
	}
#line 2881 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 164:
#line 782 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OCONST, Z, Z);
		(yyval.node)->type = types[TUINT];
		(yyval.node)->vconst = (yyvsp[0].vval);
		(yyval.node)->cstring = strdup(symb);
	}
#line 2892 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 165:
#line 789 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OCONST, Z, Z);
		(yyval.node)->type = types[TULONG];
		(yyval.node)->vconst = (yyvsp[0].vval);
		(yyval.node)->cstring = strdup(symb);
	}
#line 2903 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 166:
#line 796 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OCONST, Z, Z);
		(yyval.node)->type = types[TDOUBLE];
		(yyval.node)->fconst = (yyvsp[0].dval);
		(yyval.node)->cstring = strdup(symb);
	}
#line 2914 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 167:
#line 803 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OCONST, Z, Z);
		(yyval.node)->type = types[TFLOAT];
		(yyval.node)->fconst = (yyvsp[0].dval);
		(yyval.node)->cstring = strdup(symb);
	}
#line 2925 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 168:
#line 810 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OCONST, Z, Z);
		(yyval.node)->type = types[TVLONG];
		(yyval.node)->vconst = (yyvsp[0].vval);
		(yyval.node)->cstring = strdup(symb);
	}
#line 2936 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 169:
#line 817 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OCONST, Z, Z);
		(yyval.node)->type = types[TUVLONG];
		(yyval.node)->vconst = (yyvsp[0].vval);
		(yyval.node)->cstring = strdup(symb);
	}
#line 2947 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 172:
#line 828 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OSTRING, Z, Z);
		(yyval.node)->type = typ(TARRAY, types[TCHAR]);
		(yyval.node)->type->width = (yyvsp[0].sval).l + 1;
		(yyval.node)->cstring = (yyvsp[0].sval).s;
		(yyval.node)->sym = symstring;
		(yyval.node)->etype = TARRAY;
		(yyval.node)->class = CSTATIC;
	}
#line 2961 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 173:
#line 838 "cc.y" /* yacc.c:1646  */
    {
		char *s;
		int n;

		n = (yyvsp[-1].node)->type->width - 1;
		s = alloc(n+(yyvsp[0].sval).l+MAXALIGN);

		memcpy(s, (yyvsp[-1].node)->cstring, n);
		memcpy(s+n, (yyvsp[0].sval).s, (yyvsp[0].sval).l);
		s[n+(yyvsp[0].sval).l] = 0;

		(yyval.node) = (yyvsp[-1].node);
		(yyval.node)->type->width += (yyvsp[0].sval).l;
		(yyval.node)->cstring = s;
	}
#line 2981 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 174:
#line 856 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OLSTRING, Z, Z);
		(yyval.node)->type = typ(TARRAY, types[TRUNE]);
		(yyval.node)->type->width = (yyvsp[0].sval).l + sizeof(TRune);
		(yyval.node)->rstring = (TRune*)(yyvsp[0].sval).s;
		(yyval.node)->sym = symstring;
		(yyval.node)->etype = TARRAY;
		(yyval.node)->class = CSTATIC;
	}
#line 2995 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 175:
#line 866 "cc.y" /* yacc.c:1646  */
    {
		char *s;
		int n;

		n = (yyvsp[-1].node)->type->width - sizeof(TRune);
		s = alloc(n+(yyvsp[0].sval).l+MAXALIGN);

		memcpy(s, (yyvsp[-1].node)->rstring, n);
		memcpy(s+n, (yyvsp[0].sval).s, (yyvsp[0].sval).l);
		*(TRune*)(s+n+(yyvsp[0].sval).l) = 0;

		(yyval.node) = (yyvsp[-1].node);
		(yyval.node)->type->width += (yyvsp[0].sval).l;
		(yyval.node)->rstring = (TRune*)s;
	}
#line 3015 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 176:
#line 883 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = Z;
	}
#line 3023 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 179:
#line 891 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(OLIST, (yyvsp[-2].node), (yyvsp[0].node));
	}
#line 3031 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 180:
#line 897 "cc.y" /* yacc.c:1646  */
    {
		(yyval.tyty).t1 = strf;
		(yyval.tyty).t2 = strl;
		(yyval.tyty).t3 = lasttype;
		(yyval.tyty).c = lastclass;
		strf = T;
		strl = T;
		lastbit = 0;
		firstbit = 1;
		lastclass = CXXX;
		lasttype = T;
	}
#line 3048 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 181:
#line 910 "cc.y" /* yacc.c:1646  */
    {
		(yyval.type) = strf;
		strf = (yyvsp[-2].tyty).t1;
		strl = (yyvsp[-2].tyty).t2;
		lasttype = (yyvsp[-2].tyty).t3;
		lastclass = (yyvsp[-2].tyty).c;
	}
#line 3060 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 182:
#line 919 "cc.y" /* yacc.c:1646  */
    {
		lastclass = CXXX;
		lasttype = types[TINT];
	}
#line 3069 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 184:
#line 927 "cc.y" /* yacc.c:1646  */
    {
		(yyval.tycl).t = (yyvsp[0].type);
		(yyval.tycl).c = CXXX;
	}
#line 3078 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 185:
#line 932 "cc.y" /* yacc.c:1646  */
    {
		(yyval.tycl).t = simplet((yyvsp[0].lval));
		(yyval.tycl).c = CXXX;
	}
#line 3087 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 186:
#line 937 "cc.y" /* yacc.c:1646  */
    {
		(yyval.tycl).t = simplet((yyvsp[0].lval));
		(yyval.tycl).c = simplec((yyvsp[0].lval));
		(yyval.tycl).t = garbt((yyval.tycl).t, (yyvsp[0].lval));
	}
#line 3097 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 187:
#line 943 "cc.y" /* yacc.c:1646  */
    {
		(yyval.tycl).t = (yyvsp[-1].type);
		(yyval.tycl).c = simplec((yyvsp[0].lval));
		(yyval.tycl).t = garbt((yyval.tycl).t, (yyvsp[0].lval));
		if((yyvsp[0].lval) & ~BCLASS & ~BGARB)
			diag(Z, "duplicate types given: %T and %Q", (yyvsp[-1].type), (yyvsp[0].lval));
	}
#line 3109 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 188:
#line 951 "cc.y" /* yacc.c:1646  */
    {
		(yyval.tycl).t = simplet(typebitor((yyvsp[-1].lval), (yyvsp[0].lval)));
		(yyval.tycl).c = simplec((yyvsp[0].lval));
		(yyval.tycl).t = garbt((yyval.tycl).t, (yyvsp[0].lval));
	}
#line 3119 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 189:
#line 957 "cc.y" /* yacc.c:1646  */
    {
		(yyval.tycl).t = (yyvsp[-1].type);
		(yyval.tycl).c = simplec((yyvsp[-2].lval));
		(yyval.tycl).t = garbt((yyval.tycl).t, (yyvsp[-2].lval)|(yyvsp[0].lval));
	}
#line 3129 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 190:
#line 963 "cc.y" /* yacc.c:1646  */
    {
		(yyval.tycl).t = simplet((yyvsp[0].lval));
		(yyval.tycl).c = simplec((yyvsp[-1].lval));
		(yyval.tycl).t = garbt((yyval.tycl).t, (yyvsp[-1].lval));
	}
#line 3139 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 191:
#line 969 "cc.y" /* yacc.c:1646  */
    {
		(yyval.tycl).t = simplet(typebitor((yyvsp[-1].lval), (yyvsp[0].lval)));
		(yyval.tycl).c = simplec((yyvsp[-2].lval)|(yyvsp[0].lval));
		(yyval.tycl).t = garbt((yyval.tycl).t, (yyvsp[-2].lval)|(yyvsp[0].lval));
	}
#line 3149 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 192:
#line 977 "cc.y" /* yacc.c:1646  */
    {
		(yyval.type) = (yyvsp[0].tycl).t;
		if((yyvsp[0].tycl).c != CXXX)
			diag(Z, "illegal combination of class 4: %s", cnames[(yyvsp[0].tycl).c]);
	}
#line 3159 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 193:
#line 985 "cc.y" /* yacc.c:1646  */
    {
		lasttype = (yyvsp[0].tycl).t;
		lastclass = (yyvsp[0].tycl).c;
	}
#line 3168 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 194:
#line 992 "cc.y" /* yacc.c:1646  */
    {
		dotag((yyvsp[0].sym), TSTRUCT, 0);
		(yyval.type) = (yyvsp[0].sym)->suetag;
	}
#line 3177 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 195:
#line 997 "cc.y" /* yacc.c:1646  */
    {
		dotag((yyvsp[0].sym), TSTRUCT, autobn);
	}
#line 3185 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 196:
#line 1001 "cc.y" /* yacc.c:1646  */
    {
		(yyval.type) = (yyvsp[-2].sym)->suetag;
		if((yyval.type)->link != T)
			diag(Z, "redeclare tag: %s", (yyvsp[-2].sym)->name);
		(yyval.type)->link = (yyvsp[0].type);
		sualign((yyval.type));
	}
#line 3197 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 197:
#line 1009 "cc.y" /* yacc.c:1646  */
    {
		taggen++;
		sprint(symb, "_%d_", taggen);
		(yyval.type) = dotag(lookup(), TSTRUCT, autobn);
		(yyval.type)->link = (yyvsp[0].type);
		sualign((yyval.type));
	}
#line 3209 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 198:
#line 1017 "cc.y" /* yacc.c:1646  */
    {
		dotag((yyvsp[0].sym), TUNION, 0);
		(yyval.type) = (yyvsp[0].sym)->suetag;
	}
#line 3218 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 199:
#line 1022 "cc.y" /* yacc.c:1646  */
    {
		dotag((yyvsp[0].sym), TUNION, autobn);
	}
#line 3226 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 200:
#line 1026 "cc.y" /* yacc.c:1646  */
    {
		(yyval.type) = (yyvsp[-2].sym)->suetag;
		if((yyval.type)->link != T)
			diag(Z, "redeclare tag: %s", (yyvsp[-2].sym)->name);
		(yyval.type)->link = (yyvsp[0].type);
		sualign((yyval.type));
	}
#line 3238 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 201:
#line 1034 "cc.y" /* yacc.c:1646  */
    {
		taggen++;
		sprint(symb, "_%d_", taggen);
		(yyval.type) = dotag(lookup(), TUNION, autobn);
		(yyval.type)->link = (yyvsp[0].type);
		sualign((yyval.type));
	}
#line 3250 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 202:
#line 1042 "cc.y" /* yacc.c:1646  */
    {
		dotag((yyvsp[0].sym), TENUM, 0);
		(yyval.type) = (yyvsp[0].sym)->suetag;
		if((yyval.type)->link == T)
			(yyval.type)->link = types[TINT];
		(yyval.type) = (yyval.type)->link;
	}
#line 3262 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 203:
#line 1050 "cc.y" /* yacc.c:1646  */
    {
		dotag((yyvsp[0].sym), TENUM, autobn);
	}
#line 3270 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 204:
#line 1054 "cc.y" /* yacc.c:1646  */
    {
		en.tenum = T;
		en.cenum = T;
	}
#line 3279 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 205:
#line 1059 "cc.y" /* yacc.c:1646  */
    {
		(yyval.type) = (yyvsp[-5].sym)->suetag;
		if((yyval.type)->link != T)
			diag(Z, "redeclare tag: %s", (yyvsp[-5].sym)->name);
		if(en.tenum == T) {
			diag(Z, "enum type ambiguous: %s", (yyvsp[-5].sym)->name);
			en.tenum = types[TINT];
		}
		(yyval.type)->link = en.tenum;
		(yyval.type) = en.tenum;
	}
#line 3295 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 206:
#line 1071 "cc.y" /* yacc.c:1646  */
    {
		en.tenum = T;
		en.cenum = T;
	}
#line 3304 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 207:
#line 1076 "cc.y" /* yacc.c:1646  */
    {
		(yyval.type) = en.tenum;
	}
#line 3312 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 208:
#line 1080 "cc.y" /* yacc.c:1646  */
    {
		(yyval.type) = tcopy((yyvsp[0].sym)->type);
	}
#line 3320 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 210:
#line 1087 "cc.y" /* yacc.c:1646  */
    {
		(yyval.lval) = typebitor((yyvsp[-1].lval), (yyvsp[0].lval));
	}
#line 3328 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 211:
#line 1092 "cc.y" /* yacc.c:1646  */
    {
		(yyval.lval) = 0;
	}
#line 3336 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 212:
#line 1096 "cc.y" /* yacc.c:1646  */
    {
		(yyval.lval) = typebitor((yyvsp[-1].lval), (yyvsp[0].lval));
	}
#line 3344 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 217:
#line 1108 "cc.y" /* yacc.c:1646  */
    {
		(yyval.lval) = typebitor((yyvsp[-1].lval), (yyvsp[0].lval));
	}
#line 3352 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 220:
#line 1118 "cc.y" /* yacc.c:1646  */
    {
		doenum((yyvsp[0].sym), Z);
	}
#line 3360 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 221:
#line 1122 "cc.y" /* yacc.c:1646  */
    {
		doenum((yyvsp[-2].sym), (yyvsp[0].node));
	}
#line 3368 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 224:
#line 1129 "cc.y" /* yacc.c:1646  */
    { (yyval.lval) = BCHAR; }
#line 3374 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 225:
#line 1130 "cc.y" /* yacc.c:1646  */
    { (yyval.lval) = BSHORT; }
#line 3380 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 226:
#line 1131 "cc.y" /* yacc.c:1646  */
    { (yyval.lval) = BINT; }
#line 3386 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 227:
#line 1132 "cc.y" /* yacc.c:1646  */
    { (yyval.lval) = BLONG; }
#line 3392 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 228:
#line 1133 "cc.y" /* yacc.c:1646  */
    { (yyval.lval) = BSIGNED; }
#line 3398 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 229:
#line 1134 "cc.y" /* yacc.c:1646  */
    { (yyval.lval) = BUNSIGNED; }
#line 3404 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 230:
#line 1135 "cc.y" /* yacc.c:1646  */
    { (yyval.lval) = BFLOAT; }
#line 3410 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 231:
#line 1136 "cc.y" /* yacc.c:1646  */
    { (yyval.lval) = BDOUBLE; }
#line 3416 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 232:
#line 1137 "cc.y" /* yacc.c:1646  */
    { (yyval.lval) = BVOID; }
#line 3422 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 233:
#line 1140 "cc.y" /* yacc.c:1646  */
    { (yyval.lval) = BAUTO; }
#line 3428 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 234:
#line 1141 "cc.y" /* yacc.c:1646  */
    { (yyval.lval) = BSTATIC; }
#line 3434 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 235:
#line 1142 "cc.y" /* yacc.c:1646  */
    { (yyval.lval) = BEXTERN; }
#line 3440 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 236:
#line 1143 "cc.y" /* yacc.c:1646  */
    { (yyval.lval) = BTYPEDEF; }
#line 3446 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 237:
#line 1144 "cc.y" /* yacc.c:1646  */
    { (yyval.lval) = BTYPESTR; }
#line 3452 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 238:
#line 1145 "cc.y" /* yacc.c:1646  */
    { (yyval.lval) = BREGISTER; }
#line 3458 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 239:
#line 1146 "cc.y" /* yacc.c:1646  */
    { (yyval.lval) = 0; }
#line 3464 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 240:
#line 1149 "cc.y" /* yacc.c:1646  */
    { (yyval.lval) = BCONSTNT; }
#line 3470 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 241:
#line 1150 "cc.y" /* yacc.c:1646  */
    { (yyval.lval) = BVOLATILE; }
#line 3476 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 242:
#line 1151 "cc.y" /* yacc.c:1646  */
    { (yyval.lval) = 0; }
#line 3482 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 243:
#line 1155 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(ONAME, Z, Z);
		if((yyvsp[0].sym)->class == CLOCAL)
			(yyvsp[0].sym) = mkstatic((yyvsp[0].sym));
		(yyval.node)->sym = (yyvsp[0].sym);
		(yyval.node)->type = (yyvsp[0].sym)->type;
		(yyval.node)->etype = TVOID;
		if((yyval.node)->type != T)
			(yyval.node)->etype = (yyval.node)->type->etype;
		(yyval.node)->xoffset = (yyvsp[0].sym)->offset;
		(yyval.node)->class = (yyvsp[0].sym)->class;
		(yyvsp[0].sym)->aused = 1;
	}
#line 3500 "cc.tab.c" /* yacc.c:1646  */
    break;

  case 244:
#line 1170 "cc.y" /* yacc.c:1646  */
    {
		(yyval.node) = new(ONAME, Z, Z);
		(yyval.node)->sym = (yyvsp[0].sym);
		(yyval.node)->type = (yyvsp[0].sym)->type;
		(yyval.node)->etype = TVOID;
		if((yyval.node)->type != T)
			(yyval.node)->etype = (yyval.node)->type->etype;
		(yyval.node)->xoffset = (yyvsp[0].sym)->offset;
		(yyval.node)->class = (yyvsp[0].sym)->class;
	}
#line 3515 "cc.tab.c" /* yacc.c:1646  */
    break;


#line 3519 "cc.tab.c" /* yacc.c:1646  */
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
#line 1183 "cc.y" /* yacc.c:1906  */

