/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison implementation for Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2011 Free Software Foundation, Inc.
   
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
#define YYBISON_VERSION "2.5"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */


/* Line 268 of yacc.c  */
#line 71 "parse.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

/* "%code requires" blocks.  */

/* Line 288 of yacc.c  */
#line 21 "parse.y"

#include "hypfile.h"



/* Line 288 of yacc.c  */
#line 101 "parse.c"

/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     BOARD_FILE = 258,
     VERSION = 259,
     DATA_MODE = 260,
     UNITS = 261,
     PLANE_SEP = 262,
     BOARD = 263,
     STACKUP = 264,
     DEVICES = 265,
     SUPPLIES = 266,
     PAD = 267,
     PADSTACK = 268,
     NET = 269,
     NET_CLASS = 270,
     END = 271,
     KEY = 272,
     A = 273,
     ARC = 274,
     COPPER = 275,
     CURVE = 276,
     DETAILED = 277,
     DIELECTRIC = 278,
     ENGLISH = 279,
     LENGTH = 280,
     LINE = 281,
     METRIC = 282,
     N = 283,
     OPTIONS = 284,
     PERIMETER_ARC = 285,
     PERIMETER_SEGMENT = 286,
     PIN = 287,
     PLANE = 288,
     POLYGON = 289,
     POLYLINE = 290,
     POLYVOID = 291,
     POUR = 292,
     S = 293,
     SEG = 294,
     SIGNAL = 295,
     SIMPLIFIED = 296,
     SIM_BOTH = 297,
     SIM_IN = 298,
     SIM_OUT = 299,
     USEG = 300,
     VIA = 301,
     WEIGHT = 302,
     A1 = 303,
     A2 = 304,
     BR = 305,
     C = 306,
     C_QM = 307,
     CO_QM = 308,
     D = 309,
     ER = 310,
     F = 311,
     ID = 312,
     L = 313,
     L1 = 314,
     L2 = 315,
     LPS = 316,
     LT = 317,
     M = 318,
     NAME = 319,
     P = 320,
     PKG = 321,
     PR_QM = 322,
     PS = 323,
     R = 324,
     REF = 325,
     SX = 326,
     SY = 327,
     S1 = 328,
     S1X = 329,
     S1Y = 330,
     S2 = 331,
     S2X = 332,
     S2Y = 333,
     T = 334,
     TC = 335,
     USE_DIE_FOR_METAL = 336,
     V = 337,
     V_QM = 338,
     VAL = 339,
     W = 340,
     X = 341,
     X1 = 342,
     X2 = 343,
     XC = 344,
     Y = 345,
     Y1 = 346,
     Y2 = 347,
     YC = 348,
     Z = 349,
     ZL = 350,
     ZLEN = 351,
     ZW = 352,
     YES = 353,
     NO = 354,
     BOOL = 355,
     POSINT = 356,
     FLOAT = 357,
     STRING = 358
   };
#endif
/* Tokens.  */
#define BOARD_FILE 258
#define VERSION 259
#define DATA_MODE 260
#define UNITS 261
#define PLANE_SEP 262
#define BOARD 263
#define STACKUP 264
#define DEVICES 265
#define SUPPLIES 266
#define PAD 267
#define PADSTACK 268
#define NET 269
#define NET_CLASS 270
#define END 271
#define KEY 272
#define A 273
#define ARC 274
#define COPPER 275
#define CURVE 276
#define DETAILED 277
#define DIELECTRIC 278
#define ENGLISH 279
#define LENGTH 280
#define LINE 281
#define METRIC 282
#define N 283
#define OPTIONS 284
#define PERIMETER_ARC 285
#define PERIMETER_SEGMENT 286
#define PIN 287
#define PLANE 288
#define POLYGON 289
#define POLYLINE 290
#define POLYVOID 291
#define POUR 292
#define S 293
#define SEG 294
#define SIGNAL 295
#define SIMPLIFIED 296
#define SIM_BOTH 297
#define SIM_IN 298
#define SIM_OUT 299
#define USEG 300
#define VIA 301
#define WEIGHT 302
#define A1 303
#define A2 304
#define BR 305
#define C 306
#define C_QM 307
#define CO_QM 308
#define D 309
#define ER 310
#define F 311
#define ID 312
#define L 313
#define L1 314
#define L2 315
#define LPS 316
#define LT 317
#define M 318
#define NAME 319
#define P 320
#define PKG 321
#define PR_QM 322
#define PS 323
#define R 324
#define REF 325
#define SX 326
#define SY 327
#define S1 328
#define S1X 329
#define S1Y 330
#define S2 331
#define S2X 332
#define S2Y 333
#define T 334
#define TC 335
#define USE_DIE_FOR_METAL 336
#define V 337
#define V_QM 338
#define VAL 339
#define W 340
#define X 341
#define X1 342
#define X2 343
#define XC 344
#define Y 345
#define Y1 346
#define Y2 347
#define YC 348
#define Z 349
#define ZL 350
#define ZLEN 351
#define ZW 352
#define YES 353
#define NO 354
#define BOOL 355
#define POSINT 356
#define FLOAT 357
#define STRING 358




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 293 of yacc.c  */
#line 30 "parse.y"

    int boolval;
    int intval;
    double floatval;
    char* strval;



/* Line 293 of yacc.c  */
#line 333 "parse.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */

/* Line 343 of yacc.c  */
#line 37 "parse.y"

#include <cstdlib>
#include <cstdarg>
#include <cstdio>
#include <cstring>
#include <string>
#include <sstream>
#include "parser.h"

void yyerror(HypFile::Hyp *, const char *);

/* YYPRINT and yyprint print values of the tokens when debugging is switched on */
void yyprint(FILE *, int, YYSTYPE);
#define YYPRINT(file, type, value) yyprint (file, type, value)

/* clear parse_param struct at beginning of new record */
void new_record();

/* struct to pass to calling class */
parse_param h;



/* Line 343 of yacc.c  */
#line 369 "parse.c"

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
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
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
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#   if ! defined malloc && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
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
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  33
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   727

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  110
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  191
/* YYNRULES -- Number of rules.  */
#define YYNRULES  300
/* YYNRULES -- Number of states.  */
#define YYNSTATES  621

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   358

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
     107,   108,     2,     2,   109,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   106,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,   104,     2,   105,     2,     2,     2,     2,
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
      95,    96,    97,    98,    99,   100,   101,   102,   103
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     6,     8,    10,    12,    14,    16,    18,
      20,    22,    24,    26,    28,    30,    32,    34,    36,    40,
      41,    46,    47,    54,    60,    62,    64,    71,    73,    75,
      77,    79,    80,    87,    92,    95,    97,    99,   101,   103,
     107,   112,   117,   118,   119,   131,   136,   139,   141,   143,
     145,   147,   149,   153,   157,   158,   164,   166,   167,   173,
     176,   178,   180,   182,   186,   188,   190,   192,   194,   196,
     198,   200,   201,   207,   210,   212,   214,   218,   220,   222,
     224,   226,   228,   230,   231,   237,   240,   242,   244,   248,
     250,   252,   254,   256,   258,   260,   262,   266,   270,   274,
     278,   282,   286,   290,   294,   298,   302,   306,   311,   315,
     318,   320,   321,   322,   323,   334,   338,   341,   343,   346,
     348,   351,   353,   357,   359,   361,   365,   369,   373,   378,
     381,   383,   391,   395,   399,   403,   404,   405,   414,   415,
     420,   423,   425,   428,   430,   431,   439,   443,   444,   448,
     450,   451,   452,   460,   462,   466,   470,   471,   472,   473,
     474,   490,   491,   499,   500,   504,   506,   509,   511,   513,
     515,   517,   519,   521,   523,   525,   527,   529,   531,   535,
     539,   540,   548,   549,   557,   560,   562,   565,   567,   571,
     575,   576,   582,   584,   586,   588,   591,   593,   596,   598,
     601,   605,   609,   613,   614,   615,   616,   617,   618,   619,
     620,   621,   622,   662,   663,   670,   673,   675,   678,   680,
     684,   688,   692,   696,   697,   698,   699,   700,   701,   724,
     725,   734,   736,   738,   739,   740,   741,   755,   756,   757,
     767,   768,   774,   776,   777,   778,   787,   790,   792,   794,
     796,   798,   800,   804,   808,   812,   816,   817,   818,   827,
     828,   829,   838,   841,   843,   845,   847,   851,   852,   858,
     859,   865,   866,   867,   879,   880,   887,   890,   892,   895,
     897,   899,   901,   905,   906,   914,   915,   916,   928,   932,
     933,   940,   941,   949,   950,   958,   959,   967,   970,   971,
     972
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     111,     0,    -1,   111,   112,    -1,   112,    -1,   113,    -1,
     115,    -1,   117,    -1,   119,    -1,   122,    -1,   124,    -1,
     132,    -1,   161,    -1,   175,    -1,   180,    -1,   198,    -1,
     278,    -1,   288,    -1,   289,    -1,   104,     1,   105,    -1,
      -1,   104,     3,   114,   105,    -1,    -1,   104,     4,   106,
     102,   116,   105,    -1,   104,     5,   106,   118,   105,    -1,
      41,    -1,    22,    -1,   104,     6,   106,   120,   121,   105,
      -1,    24,    -1,    27,    -1,    47,    -1,    25,    -1,    -1,
     104,     7,   106,   102,   123,   105,    -1,   104,     8,   125,
     105,    -1,   125,   126,    -1,   126,    -1,   127,    -1,   128,
      -1,   129,    -1,   107,     1,   108,    -1,   107,    31,   297,
     108,    -1,   107,    30,   298,   108,    -1,    -1,    -1,   107,
      18,    28,   106,   103,   130,    82,   106,   103,   131,   108,
      -1,   104,     9,   133,   105,    -1,   133,   134,    -1,   134,
      -1,   135,    -1,   138,    -1,   142,    -1,   146,    -1,   107,
       1,   108,    -1,   107,    29,   136,    -1,    -1,    81,   106,
     100,   137,   108,    -1,   108,    -1,    -1,   107,    40,   139,
     140,   108,    -1,   140,   141,    -1,   141,    -1,   150,    -1,
     151,    -1,    51,   106,   102,    -1,   152,    -1,   153,    -1,
     154,    -1,   155,    -1,   156,    -1,   157,    -1,   158,    -1,
      -1,   107,    23,   143,   144,   108,    -1,   144,   145,    -1,
     145,    -1,   150,    -1,    51,   106,   102,    -1,   154,    -1,
     155,    -1,   159,    -1,   160,    -1,   156,    -1,   157,    -1,
      -1,   107,    33,   147,   148,   108,    -1,   148,   149,    -1,
     149,    -1,   150,    -1,    51,   106,   102,    -1,   152,    -1,
     153,    -1,   154,    -1,   155,    -1,   156,    -1,   157,    -1,
     158,    -1,    79,   106,   102,    -1,    65,   106,   102,    -1,
      50,   106,   102,    -1,    80,   106,   102,    -1,    55,   106,
     102,    -1,    62,   106,   102,    -1,    58,   106,   103,    -1,
      63,   106,   103,    -1,    68,   106,   102,    -1,    53,   106,
     100,    -1,    67,   106,   100,    -1,   104,    10,   162,   105,
      -1,   104,    10,   105,    -1,   162,   163,    -1,   163,    -1,
      -1,    -1,    -1,   107,   164,   103,   165,    70,   106,   103,
     166,   167,   108,    -1,   107,     1,   108,    -1,   170,   168,
      -1,   168,    -1,   171,   169,    -1,   169,    -1,   156,   174,
      -1,   156,    -1,    64,   106,   103,    -1,   172,    -1,   173,
      -1,    84,   106,   102,    -1,    84,   106,   103,    -1,    66,
     106,   103,    -1,   104,    11,   176,   105,    -1,   176,   177,
      -1,   177,    -1,   107,    38,   170,   172,   178,   179,   108,
      -1,   107,     1,   108,    -1,    83,   106,   100,    -1,    52,
     106,   100,    -1,    -1,    -1,   104,    13,   181,   106,   103,
     182,   183,   105,    -1,    -1,   109,   102,   184,   185,    -1,
     109,   185,    -1,   185,    -1,   185,   186,    -1,   186,    -1,
      -1,   107,   103,   187,   109,   188,   190,   193,    -1,   107,
       1,   108,    -1,    -1,   102,   189,   109,    -1,   109,    -1,
      -1,    -1,   102,   191,   109,   102,   192,   109,   102,    -1,
     108,    -1,   109,    63,   108,    -1,   109,    18,   108,    -1,
      -1,    -1,    -1,    -1,   109,   102,   194,   109,   102,   195,
     109,   102,   196,   109,   102,   197,   109,    79,   108,    -1,
      -1,   104,    14,   106,   103,   199,   200,   105,    -1,    -1,
     158,   201,   202,    -1,   202,    -1,   202,   203,    -1,   203,
      -1,   204,    -1,   206,    -1,   212,    -1,   232,    -1,   238,
      -1,   244,    -1,   256,    -1,   263,    -1,   266,    -1,   275,
      -1,   107,     1,   108,    -1,   104,     1,   105,    -1,    -1,
     107,    39,   205,   297,   210,   156,   208,    -1,    -1,   107,
      19,   207,   298,   210,   156,   208,    -1,   158,   209,    -1,
     209,    -1,   211,   108,    -1,   108,    -1,    85,   106,   102,
      -1,    61,   106,   102,    -1,    -1,   107,    46,   213,   291,
     214,    -1,   215,    -1,   222,    -1,   216,    -1,   220,   217,
      -1,   217,    -1,   221,   218,    -1,   218,    -1,   219,   108,
      -1,    65,   106,   103,    -1,    59,   106,   103,    -1,    60,
     106,   103,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    54,   106,   102,   223,   220,   221,    73,   106,
     103,   224,    74,   106,   102,   225,    75,   106,   102,   226,
      48,   106,   102,   227,    76,   106,   103,   228,    77,   106,
     102,   229,    78,   106,   102,   230,    49,   106,   102,   231,
     108,    -1,    -1,   107,    32,   233,   291,   236,   234,    -1,
     219,   235,    -1,   235,    -1,   237,   108,    -1,   108,    -1,
      69,   106,   103,    -1,    56,   106,    44,    -1,    56,   106,
      43,    -1,    56,   106,    42,    -1,    -1,    -1,    -1,    -1,
      -1,   107,    12,   239,   291,   156,    38,   106,   103,   240,
      71,   106,   102,   241,    72,   106,   102,   242,    18,   106,
     102,   243,   108,    -1,    -1,   107,    45,   245,   293,   220,
     295,   221,   246,    -1,   247,    -1,   251,    -1,    -1,    -1,
      -1,    95,   106,   103,   248,    97,   106,   102,   249,    96,
     106,   102,   250,   108,    -1,    -1,    -1,    94,   106,   102,
     252,    54,   106,   102,   253,   254,    -1,    -1,    69,   106,
     102,   255,   108,    -1,   108,    -1,    -1,    -1,   104,    34,
     257,   259,   291,   258,   269,   105,    -1,   259,   260,    -1,
     260,    -1,   156,    -1,   210,    -1,   262,    -1,   261,    -1,
      57,   106,   101,    -1,    79,   106,    37,    -1,    79,   106,
      33,    -1,    79,   106,    20,    -1,    -1,    -1,   104,    36,
     264,   261,   291,   265,   269,   105,    -1,    -1,    -1,   104,
      35,   267,   259,   291,   268,   269,   105,    -1,   269,   270,
      -1,   270,    -1,   271,    -1,   273,    -1,   107,     1,   108,
      -1,    -1,   107,    26,   272,   291,   108,    -1,    -1,   107,
      21,   274,   298,   108,    -1,    -1,    -1,   107,    18,    28,
     106,   103,   276,    82,   106,   103,   277,   108,    -1,    -1,
     104,    15,   106,   103,   279,   280,    -1,   281,   105,    -1,
     105,    -1,   281,   282,    -1,   282,    -1,   285,    -1,   283,
      -1,   107,     1,   108,    -1,    -1,   107,    28,    28,   106,
     103,   284,   108,    -1,    -1,    -1,   107,    18,    28,   106,
     103,   286,    82,   106,   103,   287,   108,    -1,   104,    16,
     105,    -1,    -1,   104,    17,   106,   103,   290,   105,    -1,
      -1,    86,   106,   102,   292,    90,   106,   102,    -1,    -1,
      87,   106,   102,   294,    91,   106,   102,    -1,    -1,    88,
     106,   102,   296,    92,   106,   102,    -1,   293,   295,    -1,
      -1,    -1,   297,    89,   106,   102,   299,    93,   106,   102,
     300,    69,   106,   102,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   130,   130,   131,   134,   135,   136,   137,   138,   139,
     140,   141,   142,   143,   144,   145,   146,   147,   148,   153,
     153,   158,   158,   163,   166,   167,   172,   175,   176,   179,
     180,   184,   184,   189,   192,   193,   196,   197,   198,   199,
     202,   205,   208,   208,   208,   213,   216,   217,   220,   221,
     222,   223,   224,   227,   230,   230,   231,   235,   235,   238,
     239,   242,   243,   244,   245,   246,   247,   248,   249,   250,
     251,   254,   254,   257,   258,   261,   262,   263,   264,   265,
     266,   267,   268,   272,   272,   275,   276,   279,   280,   281,
     282,   283,   284,   285,   286,   287,   290,   293,   296,   299,
     302,   305,   308,   311,   314,   317,   320,   325,   326,   329,
     330,   333,   333,   333,   333,   334,   337,   338,   342,   343,
     347,   348,   352,   355,   356,   360,   363,   366,   371,   374,
     375,   378,   379,   382,   385,   390,   390,   390,   393,   393,
     394,   395,   398,   399,   402,   402,   403,   406,   406,   407,
     411,   411,   411,   414,   415,   416,   417,   418,   419,   420,
     417,   427,   427,   430,   430,   431,   434,   435,   438,   439,
     440,   441,   442,   443,   444,   445,   446,   447,   448,   449,
     453,   453,   456,   456,   459,   460,   464,   465,   469,   472,
     475,   475,   479,   480,   484,   487,   488,   492,   493,   497,
     501,   504,   507,   510,   513,   514,   515,   516,   517,   518,
     519,   520,   510,   525,   525,   528,   529,   533,   534,   538,
     541,   542,   543,   547,   550,   551,   552,   553,   547,   558,
     558,   561,   562,   566,   567,   568,   566,   573,   574,   573,
     578,   578,   580,   584,   584,   584,   588,   589,   593,   594,
     595,   596,   600,   604,   605,   606,   610,   610,   610,   614,
     614,   614,   618,   619,   623,   624,   625,   629,   629,   632,
     632,   635,   635,   635,   640,   640,   643,   644,   648,   649,
     653,   654,   655,   659,   659,   662,   662,   662,   667,   672,
     672,   677,   677,   680,   680,   683,   683,   686,   689,   689,
     689
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "BOARD_FILE", "VERSION", "DATA_MODE",
  "UNITS", "PLANE_SEP", "BOARD", "STACKUP", "DEVICES", "SUPPLIES", "PAD",
  "PADSTACK", "NET", "NET_CLASS", "END", "KEY", "A", "ARC", "COPPER",
  "CURVE", "DETAILED", "DIELECTRIC", "ENGLISH", "LENGTH", "LINE", "METRIC",
  "N", "OPTIONS", "PERIMETER_ARC", "PERIMETER_SEGMENT", "PIN", "PLANE",
  "POLYGON", "POLYLINE", "POLYVOID", "POUR", "S", "SEG", "SIGNAL",
  "SIMPLIFIED", "SIM_BOTH", "SIM_IN", "SIM_OUT", "USEG", "VIA", "WEIGHT",
  "A1", "A2", "BR", "C", "C_QM", "CO_QM", "D", "ER", "F", "ID", "L", "L1",
  "L2", "LPS", "LT", "M", "NAME", "P", "PKG", "PR_QM", "PS", "R", "REF",
  "SX", "SY", "S1", "S1X", "S1Y", "S2", "S2X", "S2Y", "T", "TC",
  "USE_DIE_FOR_METAL", "V", "V_QM", "VAL", "W", "X", "X1", "X2", "XC", "Y",
  "Y1", "Y2", "YC", "Z", "ZL", "ZLEN", "ZW", "YES", "NO", "BOOL", "POSINT",
  "FLOAT", "STRING", "'{'", "'}'", "'='", "'('", "')'", "','", "$accept",
  "hyp_file", "hyp_section", "board_file", "$@1", "version", "$@2",
  "data_mode", "mode", "units", "unit_system", "metal_thickness_unit",
  "plane_sep", "$@3", "board", "board_paramlist", "board_param",
  "perimeter_segment", "perimeter_arc", "board_attribute", "$@4", "$@5",
  "stackup", "stackup_paramlist", "stackup_param", "options",
  "options_params", "$@6", "signal", "$@7", "signal_paramlist",
  "signal_param", "dielectric", "$@8", "dielectric_paramlist",
  "dielectric_param", "plane", "$@9", "plane_paramlist", "plane_param",
  "thickness", "plating_thickness", "bulk_resistivity",
  "temperature_coefficient", "epsilon_r", "loss_tangent", "layer_name",
  "material_name", "plane_separation", "conformal", "prepreg", "devices",
  "device_list", "device", "$@10", "$@11", "$@12", "device_paramlist",
  "device_value", "device_layer", "name", "value", "value_float",
  "value_string", "package", "supplies", "supply_list", "supply",
  "voltage_spec", "conversion", "padstack", "$@13", "$@14", "drill_size",
  "$@15", "padstack_list", "padstack_def", "$@16", "pad_shape", "$@17",
  "pad_coord", "$@18", "$@19", "pad_type", "$@20", "$@21", "$@22", "$@23",
  "net", "$@24", "net_def", "$@25", "net_subrecord_list", "net_subrecord",
  "seg", "$@26", "arc", "$@27", "ps_lps_param", "lps_param", "width",
  "left_plane_separation", "via", "$@28", "via_new_or_old_style",
  "via_new_style", "via_new_style_l1_param", "via_new_style_l2_param",
  "via_new_style_padstack_param", "padstack_name", "layer1_name",
  "layer2_name", "via_old_style", "$@29", "$@30", "$@31", "$@32", "$@33",
  "$@34", "$@35", "$@36", "$@37", "pin", "$@38", "pin_param",
  "pin_function_param", "pin_reference", "pin_function", "pad", "$@39",
  "$@40", "$@41", "$@42", "$@43", "useg", "$@44", "useg_param",
  "useg_stackup", "$@45", "$@46", "$@47", "useg_impedance", "$@48", "$@49",
  "useg_resistance", "$@50", "polygon", "$@51", "$@52",
  "polygon_param_list", "polygon_param", "polygon_id", "polygon_type",
  "polyvoid", "$@53", "$@54", "polyline", "$@55", "$@56",
  "lines_and_curves", "line_or_curve", "line", "$@57", "curve", "$@58",
  "net_attribute", "$@59", "$@60", "netclass", "$@61",
  "netclass_subrecords", "netclass_paramlist", "netclass_param",
  "net_name", "$@62", "netclass_attribute", "$@63", "$@64", "end", "key",
  "$@65", "coord_point", "$@66", "coord_point1", "$@67", "coord_point2",
  "$@68", "coord_line", "coord_arc", "$@69", "$@70", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
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
     355,   356,   357,   358,   123,   125,    61,    40,    41,    44
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint16 yyr1[] =
{
       0,   110,   111,   111,   112,   112,   112,   112,   112,   112,
     112,   112,   112,   112,   112,   112,   112,   112,   112,   114,
     113,   116,   115,   117,   118,   118,   119,   120,   120,   121,
     121,   123,   122,   124,   125,   125,   126,   126,   126,   126,
     127,   128,   130,   131,   129,   132,   133,   133,   134,   134,
     134,   134,   134,   135,   137,   136,   136,   139,   138,   140,
     140,   141,   141,   141,   141,   141,   141,   141,   141,   141,
     141,   143,   142,   144,   144,   145,   145,   145,   145,   145,
     145,   145,   145,   147,   146,   148,   148,   149,   149,   149,
     149,   149,   149,   149,   149,   149,   150,   151,   152,   153,
     154,   155,   156,   157,   158,   159,   160,   161,   161,   162,
     162,   164,   165,   166,   163,   163,   167,   167,   168,   168,
     169,   169,   170,   171,   171,   172,   173,   174,   175,   176,
     176,   177,   177,   178,   179,   181,   182,   180,   184,   183,
     183,   183,   185,   185,   187,   186,   186,   189,   188,   188,
     191,   192,   190,   193,   193,   193,   194,   195,   196,   197,
     193,   199,   198,   201,   200,   200,   202,   202,   203,   203,
     203,   203,   203,   203,   203,   203,   203,   203,   203,   203,
     205,   204,   207,   206,   208,   208,   209,   209,   210,   211,
     213,   212,   214,   214,   215,   216,   216,   217,   217,   218,
     219,   220,   221,   223,   224,   225,   226,   227,   228,   229,
     230,   231,   222,   233,   232,   234,   234,   235,   235,   236,
     237,   237,   237,   239,   240,   241,   242,   243,   238,   245,
     244,   246,   246,   248,   249,   250,   247,   252,   253,   251,
     255,   254,   254,   257,   258,   256,   259,   259,   260,   260,
     260,   260,   261,   262,   262,   262,   264,   265,   263,   267,
     268,   266,   269,   269,   270,   270,   270,   272,   271,   274,
     273,   276,   277,   275,   279,   278,   280,   280,   281,   281,
     282,   282,   282,   284,   283,   286,   287,   285,   288,   290,
     289,   292,   291,   294,   293,   296,   295,   297,   299,   300,
     298
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     0,
       4,     0,     6,     5,     1,     1,     6,     1,     1,     1,
       1,     0,     6,     4,     2,     1,     1,     1,     1,     3,
       4,     4,     0,     0,    11,     4,     2,     1,     1,     1,
       1,     1,     3,     3,     0,     5,     1,     0,     5,     2,
       1,     1,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     0,     5,     2,     1,     1,     3,     1,     1,     1,
       1,     1,     1,     0,     5,     2,     1,     1,     3,     1,
       1,     1,     1,     1,     1,     1,     3,     3,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     4,     3,     2,
       1,     0,     0,     0,    10,     3,     2,     1,     2,     1,
       2,     1,     3,     1,     1,     3,     3,     3,     4,     2,
       1,     7,     3,     3,     3,     0,     0,     8,     0,     4,
       2,     1,     2,     1,     0,     7,     3,     0,     3,     1,
       0,     0,     7,     1,     3,     3,     0,     0,     0,     0,
      15,     0,     7,     0,     3,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     3,     3,
       0,     7,     0,     7,     2,     1,     2,     1,     3,     3,
       0,     5,     1,     1,     1,     2,     1,     2,     1,     2,
       3,     3,     3,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    39,     0,     6,     2,     1,     2,     1,     3,
       3,     3,     3,     0,     0,     0,     0,     0,    22,     0,
       8,     1,     1,     0,     0,     0,    13,     0,     0,     9,
       0,     5,     1,     0,     0,     8,     2,     1,     1,     1,
       1,     1,     3,     3,     3,     3,     0,     0,     8,     0,
       0,     8,     2,     1,     1,     1,     3,     0,     5,     0,
       5,     0,     0,    11,     0,     6,     2,     1,     2,     1,
       1,     1,     3,     0,     7,     0,     0,    11,     3,     0,
       6,     0,     7,     0,     7,     0,     7,     2,     0,     0,
      12
};

/* YYDEFACT[STATE-NAME] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint16 yydefact[] =
{
       0,     0,     0,     3,     4,     5,     6,     7,     8,     9,
      10,    11,    12,    13,    14,    15,    16,    17,     0,    19,
       0,     0,     0,     0,     0,     0,     0,     0,   135,     0,
       0,     0,     0,     1,     2,    18,     0,     0,     0,     0,
       0,     0,     0,    35,    36,    37,    38,     0,     0,    47,
      48,    49,    50,    51,   108,     0,     0,   110,     0,     0,
     130,     0,     0,     0,   288,     0,    20,    21,    25,    24,
       0,    27,    28,     0,    31,     0,     0,     0,     0,    33,
      34,     0,    71,     0,    83,    57,    45,    46,     0,     0,
     107,   109,     0,     0,   128,   129,     0,   161,   274,   289,
       0,    23,    30,    29,     0,     0,    39,     0,     0,     0,
       0,     0,     0,    52,     0,     0,    56,    53,     0,     0,
     115,   112,   132,     0,     0,   136,     0,     0,     0,    22,
      26,    32,     0,     0,     0,   297,     0,    41,    40,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    74,    75,
      77,    78,    81,    82,    79,    80,     0,     0,     0,     0,
       0,     0,    86,    87,    89,    90,    91,    92,    93,    94,
      95,     0,     0,     0,    60,    61,    62,    64,    65,    66,
      67,    68,    69,    70,     0,     0,     0,     0,     0,     0,
       0,   163,     0,   165,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   277,     0,   275,     0,   279,
     281,   280,   290,    42,   293,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    72,    73,    54,     0,     0,
       0,     0,    84,    85,     0,     0,    58,    59,     0,   122,
       0,     0,     0,     0,     0,     0,   141,   143,     0,   243,
     259,   256,     0,   223,     0,   182,   213,   180,   229,   190,
       0,   162,   166,     0,     0,     0,   276,   278,     0,     0,
     295,   298,    76,   105,   100,   102,   101,   103,   106,    96,
       0,    98,    88,   104,    99,    63,    97,     0,   125,     0,
       0,     0,     0,   144,   138,   140,   137,   142,   179,     0,
       0,     0,   178,     0,     0,     0,     0,     0,     0,     0,
     164,   282,     0,     0,     0,     0,     0,     0,    55,   113,
     133,     0,   131,   146,     0,     0,     0,     0,     0,   248,
     249,     0,   247,   251,   250,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,   134,     0,   139,     0,     0,     0,   246,   244,
     260,   257,     0,     0,   271,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   191,   192,   194,   196,   198,     0,
       0,     0,   193,   285,   283,    43,   294,     0,     0,     0,
     121,     0,   117,   119,     0,     0,   123,   124,   147,   149,
       0,   252,   255,   254,   253,   188,     0,     0,     0,   291,
       0,     0,     0,     0,     0,   218,     0,   214,   216,     0,
       0,     0,     0,     0,     0,     0,   199,   195,   197,     0,
       0,     0,   296,   299,     0,     0,   120,   114,   116,   118,
       0,   150,     0,     0,     0,   263,   264,   265,     0,     0,
       0,     0,     0,     0,   187,     0,   183,   185,     0,   219,
       0,   215,   217,   181,   201,     0,   203,   202,   200,     0,
     284,    44,     0,   126,     0,   148,     0,   153,     0,   145,
       0,   269,   267,   245,   262,   261,   258,     0,   224,     0,
       0,   184,   186,   222,   221,   220,     0,     0,   230,   231,
     232,     0,     0,     0,   127,     0,     0,     0,   156,   266,
       0,     0,     0,     0,   272,   189,     0,     0,     0,   286,
       0,   151,   155,   154,     0,     0,     0,   292,     0,     0,
     237,   233,     0,     0,   300,     0,     0,   270,   268,     0,
     273,     0,     0,     0,   287,     0,   157,   225,     0,     0,
       0,   152,     0,     0,     0,     0,   204,     0,     0,   238,
     234,     0,   158,     0,     0,     0,     0,     0,   226,     0,
     242,   239,     0,     0,     0,     0,     0,     0,   205,   159,
       0,   240,   235,     0,     0,     0,     0,     0,     0,     0,
     227,   241,   236,     0,     0,     0,   206,   160,   228,     0,
       0,     0,   207,     0,     0,     0,   208,     0,     0,     0,
     209,     0,     0,     0,   210,     0,     0,     0,   211,     0,
     212
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     2,     3,     4,    36,     5,   100,     6,    70,     7,
      73,   104,     8,   105,     9,    42,    43,    44,    45,    46,
     268,   431,    10,    48,    49,    50,   117,   280,    51,   119,
     173,   174,    52,   114,   147,   148,    53,   118,   161,   162,
     149,   176,   164,   165,   150,   151,   329,   153,   170,   154,
     155,    11,    56,    57,    89,   184,   351,   391,   392,   393,
     124,   395,   396,   397,   436,    12,    59,    60,   242,   291,
      13,    61,   188,   245,   325,   246,   247,   324,   400,   440,
     442,   476,   535,   479,   524,   552,   567,   584,    14,   126,
     192,   260,   193,   194,   195,   307,   196,   305,   456,   457,
     330,   458,   197,   309,   374,   375,   376,   377,   378,   379,
     370,   381,   382,   501,   561,   583,   599,   603,   607,   611,
     615,   619,   198,   306,   417,   418,   367,   419,   199,   303,
     513,   553,   575,   595,   200,   308,   498,   499,   542,   565,
     587,   500,   541,   564,   571,   586,   201,   299,   406,   331,
     332,   333,   334,   202,   301,   408,   203,   300,   407,   444,
     445,   446,   511,   447,   510,   204,   411,   529,    15,   127,
     207,   208,   209,   210,   430,   211,   429,   533,    16,    17,
     128,   338,   450,   109,   269,   135,   316,   110,   111,   317,
     472
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -410
static const yytype_int16 yypact[] =
{
     -73,   253,    12,  -410,  -410,  -410,  -410,  -410,  -410,  -410,
    -410,  -410,  -410,  -410,  -410,  -410,  -410,  -410,   -56,  -410,
      23,    28,    48,    59,    -1,    15,   112,    80,  -410,    67,
      98,   111,   115,  -410,  -410,  -410,   144,   131,    49,    96,
     137,   132,   121,  -410,  -410,  -410,  -410,    74,   122,  -410,
    -410,  -410,  -410,  -410,  -410,    14,   125,  -410,    83,   129,
    -410,   145,   150,   162,  -410,   172,  -410,  -410,  -410,  -410,
     171,  -410,  -410,    66,  -410,   169,   250,   192,   192,  -410,
    -410,   175,  -410,    43,  -410,  -410,  -410,  -410,   176,   182,
    -410,  -410,   178,   216,  -410,  -410,   184,  -410,  -410,  -410,
     183,  -410,  -410,  -410,   185,   186,  -410,   188,   189,   201,
     203,   190,   191,  -410,   128,   194,  -410,  -410,   109,    90,
    -410,  -410,  -410,   196,   212,  -410,    24,   130,   198,  -410,
    -410,  -410,   202,   195,   200,  -410,   204,  -410,  -410,   205,
     206,   207,   208,   209,   210,   211,   213,    19,  -410,  -410,
    -410,  -410,  -410,  -410,  -410,  -410,   218,   214,   215,   217,
     219,     0,  -410,  -410,  -410,  -410,  -410,  -410,  -410,  -410,
    -410,   220,   221,   -23,  -410,  -410,  -410,  -410,  -410,  -410,
    -410,  -410,  -410,  -410,   234,   225,   223,   224,    87,   101,
      93,  -410,   226,    78,  -410,  -410,  -410,  -410,  -410,  -410,
    -410,  -410,  -410,  -410,  -410,  -410,   148,  -410,   135,  -410,
    -410,  -410,  -410,  -410,  -410,   222,   228,   230,   233,   232,
     235,   237,   238,   236,   240,  -410,  -410,  -410,   241,   242,
     243,   244,  -410,  -410,   245,   246,  -410,  -410,   229,  -410,
     247,   231,   256,    16,    -3,   248,   249,  -410,   252,  -410,
    -410,  -410,   251,  -410,   281,  -410,  -410,  -410,  -410,  -410,
      78,  -410,  -410,   254,   294,   312,  -410,  -410,   268,   260,
    -410,  -410,  -410,  -410,  -410,  -410,  -410,  -410,  -410,  -410,
     255,  -410,  -410,  -410,  -410,  -410,  -410,   257,  -410,   258,
     259,   261,   262,  -410,  -410,   249,  -410,  -410,  -410,    99,
      99,   295,  -410,   269,   265,   192,   269,   192,   192,   269,
      78,  -410,   266,   267,   270,   271,   272,   273,  -410,  -410,
    -410,   274,  -410,  -410,   275,   249,   276,   277,   279,  -410,
    -410,    89,  -410,  -410,  -410,    89,   269,   280,   296,   264,
     283,   292,   283,   316,   155,   278,   284,   285,   287,   286,
     288,   134,  -410,   -66,   249,   289,   173,   291,  -410,  -410,
    -410,  -410,   293,   340,  -410,   296,   290,   -12,   296,   297,
     201,   298,   299,   300,  -410,  -410,  -410,  -410,  -410,   301,
      50,   314,  -410,  -410,  -410,  -410,  -410,   305,   306,   304,
     325,   303,  -410,  -410,   -32,   296,  -410,  -410,  -410,  -410,
     310,  -410,  -410,  -410,  -410,  -410,   307,   307,   307,  -410,
     309,   315,   -43,   313,   311,  -410,   -15,  -410,  -410,   317,
     -43,   318,   320,   321,   319,   323,  -410,  -410,  -410,   331,
     322,   324,  -410,  -410,    41,   327,  -410,  -410,  -410,  -410,
     326,  -410,   103,   179,   136,  -410,  -410,  -410,   139,   140,
     308,   328,   330,   332,  -410,   -41,  -410,  -410,   329,  -410,
     180,  -410,  -410,  -410,  -410,   177,  -410,  -410,  -410,   333,
    -410,  -410,   349,  -410,   337,  -410,   334,  -410,    -2,  -410,
     336,  -410,  -410,  -410,  -410,  -410,  -410,   335,  -410,   339,
     343,  -410,  -410,  -410,  -410,  -410,   341,   342,  -410,  -410,
    -410,   316,   346,   344,  -410,   350,   338,   345,  -410,  -410,
     192,   269,   352,   348,  -410,  -410,   353,   354,   320,  -410,
     356,  -410,  -410,  -410,   347,   351,   355,  -410,   358,   357,
    -410,  -410,   361,   359,  -410,   360,   364,  -410,  -410,   366,
    -410,   370,   302,   365,  -410,   368,  -410,  -410,   367,   369,
     371,  -410,   363,   379,   374,   375,  -410,   376,   373,  -410,
    -410,   386,  -410,   378,   -39,   385,   377,   380,  -410,   381,
    -410,  -410,   382,   383,   384,   402,   388,   389,  -410,  -410,
     387,  -410,  -410,   407,   390,   392,   393,   394,   391,   405,
    -410,  -410,  -410,   396,   395,   397,  -410,  -410,  -410,   413,
     398,   404,  -410,   416,   401,   406,  -410,   418,   408,   409,
    -410,   422,   410,   411,  -410,   447,   412,   415,  -410,   400,
    -410
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -410,  -410,   399,  -410,  -410,  -410,  -410,  -410,  -410,  -410,
    -410,  -410,  -410,  -410,  -410,  -410,   420,  -410,  -410,  -410,
    -410,  -410,  -410,  -410,   462,  -410,  -410,  -410,  -410,  -410,
    -410,   227,  -410,  -410,  -410,   282,  -410,  -410,  -410,   362,
     -97,  -410,  -100,   -90,   -95,   -72,  -113,   -31,  -119,  -410,
    -410,  -410,  -410,   372,  -410,  -410,  -410,  -410,     8,    32,
     161,  -410,   403,  -410,  -410,  -410,  -410,   456,  -410,  -410,
    -410,  -410,  -410,  -410,  -410,  -230,  -236,  -410,  -410,  -410,
    -410,  -410,  -410,  -410,  -410,  -410,  -410,  -410,  -410,  -410,
    -410,  -410,   414,  -184,  -410,  -410,  -410,  -410,   100,    64,
     -92,  -410,  -410,  -410,  -410,  -410,  -410,   141,   143,   158,
    -340,  -409,  -410,  -410,  -410,  -410,  -410,  -410,  -410,  -410,
    -410,  -410,  -410,  -410,  -410,   106,  -410,  -410,  -410,  -410,
    -410,  -410,  -410,  -410,  -410,  -410,  -410,  -410,  -410,  -410,
    -410,  -410,  -410,  -410,  -410,  -410,  -410,  -410,  -410,   239,
    -132,   263,  -410,  -410,  -410,  -410,  -410,  -410,  -410,  -134,
    -247,  -410,  -410,  -410,  -410,  -410,  -410,  -410,  -410,  -410,
    -410,  -410,   417,  -410,  -410,  -410,  -410,  -410,  -410,  -410,
    -410,  -298,  -410,   419,  -410,   156,  -410,   -76,  -302,  -410,
    -410
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -112
static const yytype_int16 yytable[] =
{
     183,   152,   112,   340,   380,   168,   181,   191,   341,   262,
     297,   344,    33,   465,   295,    88,   506,   292,   453,   177,
     453,   163,   175,   166,   179,   159,   142,   157,   171,   178,
     569,     1,   141,   359,   152,   142,   398,   360,   361,   143,
     144,   414,   172,   399,   414,   159,   167,   180,   168,    35,
     157,   158,   389,   373,   183,   141,   146,   160,   142,   297,
     181,   507,   143,   144,   163,   454,   166,   454,   159,   570,
     139,    68,   140,   177,   141,    81,   175,   142,   179,   146,
     160,   143,   144,   178,    92,   236,   145,   169,   182,   167,
      69,   102,   159,   415,   252,   354,   415,    82,   146,   294,
     508,   180,   248,    83,   243,   253,    41,    84,   232,   532,
     372,   254,   255,   103,    85,   373,     1,  -111,   297,   293,
      71,    93,    47,    72,   115,   256,   262,   225,   189,    37,
     169,   190,   257,    75,    38,   249,   250,   251,   258,   259,
     157,   171,   182,   288,   473,   141,   326,   142,   142,   263,
      76,   116,   143,   144,    39,   172,   326,   142,   159,   157,
     158,   518,    77,    78,   141,    40,   264,   142,   327,   146,
     160,   143,   144,    62,   328,   337,   265,   159,   327,   139,
     480,   140,   189,   141,   328,   190,   142,    58,   146,   160,
     143,   144,   142,   402,   243,   145,   244,   484,   123,   358,
     481,   484,   484,   358,    63,   482,   403,   146,   525,   371,
     404,   477,   478,   526,   369,   372,    64,    54,   389,    55,
     373,    65,   493,   494,   495,   363,    79,    86,    41,    47,
      90,   342,    55,    67,    94,   205,    58,   206,   390,    74,
     266,   483,   206,   443,   485,   486,   443,   443,   365,    66,
     368,    96,   412,    97,    18,   420,    19,    20,    21,    22,
      23,    24,    25,    26,    27,    98,    28,    29,    30,    31,
      32,   496,   497,   448,   449,    99,   101,   106,   107,   108,
     123,   390,   390,   113,   120,   121,   122,   125,   129,   134,
     130,   131,   136,   455,   132,   133,   186,   214,   137,   138,
     156,   455,   185,   212,   238,   213,   215,   241,   290,   304,
     216,   217,   218,   219,   220,   221,   222,   223,   227,   224,
     228,   229,   312,   230,   270,   231,   234,   235,   239,   240,
     271,   261,   272,   273,   274,   287,   278,   289,   275,   276,
     313,   277,   279,   281,   282,   283,   284,   285,   286,   288,
     314,   315,   326,   296,   142,   337,   243,   298,   320,   302,
     319,   366,   311,   318,   349,   321,   350,   364,   328,   322,
     323,   339,   345,   346,   352,   369,   347,   348,   410,   373,
     372,   383,   355,   356,   353,   357,   362,   384,   385,   386,
     401,   435,   387,   405,   388,   409,   413,   452,   487,   549,
     237,    34,   438,   421,   423,   424,   425,   432,   433,   426,
     434,   437,   441,   469,   443,   451,   459,   460,   503,   528,
     580,   464,   467,   466,   548,   462,   468,   439,    91,   226,
     470,   488,   471,   474,   543,   475,   489,   492,   490,   502,
     504,   512,   514,   505,   509,   515,   522,   516,   517,   519,
     520,   558,   521,   523,   527,   530,   536,   531,   534,   537,
     566,   600,    80,   538,   539,   540,   546,   544,   547,   545,
     551,   550,   557,   554,   556,   555,   559,   560,   562,   563,
     568,   572,   588,   573,   594,   578,   579,   576,   577,   574,
     581,   582,   604,   585,   590,   608,   616,   593,   596,   589,
     612,   591,   592,   597,   601,   598,   602,   605,   620,   606,
      87,   610,   394,   614,   609,    95,   613,   618,   617,   491,
     463,   427,   461,   233,   428,   416,   422,   187,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,   335,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   336,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   267,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,   310,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,   343
};

#define yypact_value_is_default(yystate) \
  ((yystate) == (-410))

#define yytable_value_is_error(yytable_value) \
  YYID (0)

static const yytype_int16 yycheck[] =
{
     119,   114,    78,   305,   344,   118,   119,   126,   306,   193,
     246,   309,     0,   422,   244,     1,    18,     1,    61,   119,
      61,   118,   119,   118,   119,    68,    58,    50,    51,   119,
      69,   104,    55,   331,   147,    58,   102,   335,   336,    62,
      63,    56,    65,   109,    56,    68,   118,   119,   161,   105,
      50,    51,    84,    65,   173,    55,    79,    80,    58,   295,
     173,    63,    62,    63,   161,   108,   161,   108,    68,   108,
      51,    22,    53,   173,    55,     1,   173,    58,   173,    79,
      80,    62,    63,   173,     1,   108,    67,   118,   119,   161,
      41,    25,    68,   108,     1,   325,   108,    23,    79,   102,
     102,   173,     1,    29,   107,    12,   107,    33,   108,   518,
      60,    18,    19,    47,    40,    65,   104,   103,   354,   103,
      24,    38,   107,    27,    81,    32,   310,   108,   104,   106,
     161,   107,    39,     1,   106,    34,    35,    36,    45,    46,
      50,    51,   173,   102,   103,    55,    57,    58,    58,     1,
      18,   108,    62,    63,   106,    65,    57,    58,    68,    50,
      51,   501,    30,    31,    55,   106,    18,    58,    79,    79,
      80,    62,    63,   106,    85,    86,    28,    68,    79,    51,
       1,    53,   104,    55,    85,   107,    58,   107,    79,    80,
      62,    63,    58,    20,   107,    67,   109,   444,    64,   331,
      21,   448,   449,   335,   106,    26,    33,    79,   510,    54,
      37,   108,   109,   511,    59,    60,   105,   105,    84,   107,
      65,   106,    42,    43,    44,   338,   105,   105,   107,   107,
     105,   307,   107,   102,   105,   105,   107,   107,   351,   102,
     105,   105,   107,   107,   105,   105,   107,   107,   340,   105,
     342,   106,   365,   103,     1,   368,     3,     4,     5,     6,
       7,     8,     9,    10,    11,   103,    13,    14,    15,    16,
      17,    94,    95,   407,   408,   103,   105,   108,    28,    87,
      64,   394,   395,   108,   108,   103,   108,   103,   105,    88,
     105,   105,    89,   412,   106,   106,    84,   102,   108,   108,
     106,   420,   106,   105,    70,   103,   106,    83,    52,    28,
     106,   106,   106,   106,   106,   106,   106,   106,   100,   106,
     106,   106,    28,   106,   102,   106,   106,   106,   103,   106,
     102,   105,   102,   100,   102,   106,   100,   106,   103,   102,
      28,   103,   102,   102,   102,   102,   102,   102,   102,   102,
      82,    91,    57,   105,    58,    86,   107,   105,   100,   108,
     103,    69,   108,   108,    92,   106,    93,   103,    85,   108,
     108,   106,   106,   106,   100,    59,   106,   106,    38,    65,
      60,   103,   106,   106,   109,   106,   106,   103,   103,   102,
     101,    66,   106,   102,   106,   102,   106,    82,    90,    97,
     173,     2,   394,   106,   106,   106,   106,   102,   102,   108,
     106,   108,   102,    82,   107,   106,   103,   106,    69,    71,
      18,   103,   103,   102,    54,   108,   103,   395,    56,   147,
     108,   103,   108,   106,    73,   109,   106,   108,   106,   106,
     103,   106,   103,   109,   108,   102,   108,   106,   106,   103,
     106,    72,   102,   108,   102,   102,   109,   103,   102,   108,
      74,    48,    42,   108,   106,   108,   102,   108,   102,   109,
     102,   106,   109,   106,   103,   106,   102,   102,   102,   106,
     102,    96,    75,   106,    79,   102,   102,   106,   106,   109,
     102,   102,    76,   106,   102,    77,    49,   106,   102,   109,
      78,   108,   108,   108,   106,   108,   102,   106,   108,   103,
      48,   102,   351,   102,   106,    59,   106,   102,   106,   455,
     420,   380,   416,   161,   381,   367,   370,   124,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,   300,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   301,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   208,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,   260,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,   308
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint16 yystos[] =
{
       0,   104,   111,   112,   113,   115,   117,   119,   122,   124,
     132,   161,   175,   180,   198,   278,   288,   289,     1,     3,
       4,     5,     6,     7,     8,     9,    10,    11,    13,    14,
      15,    16,    17,     0,   112,   105,   114,   106,   106,   106,
     106,   107,   125,   126,   127,   128,   129,   107,   133,   134,
     135,   138,   142,   146,   105,   107,   162,   163,   107,   176,
     177,   181,   106,   106,   105,   106,   105,   102,    22,    41,
     118,    24,    27,   120,   102,     1,    18,    30,    31,   105,
     126,     1,    23,    29,    33,    40,   105,   134,     1,   164,
     105,   163,     1,    38,   105,   177,   106,   103,   103,   103,
     116,   105,    25,    47,   121,   123,   108,    28,    87,   293,
     297,   298,   297,   108,   143,    81,   108,   136,   147,   139,
     108,   103,   108,    64,   170,   103,   199,   279,   290,   105,
     105,   105,   106,   106,    88,   295,    89,   108,   108,    51,
      53,    55,    58,    62,    63,    67,    79,   144,   145,   150,
     154,   155,   156,   157,   159,   160,   106,    50,    51,    68,
      80,   148,   149,   150,   152,   153,   154,   155,   156,   157,
     158,    51,    65,   140,   141,   150,   151,   152,   153,   154,
     155,   156,   157,   158,   165,   106,    84,   172,   182,   104,
     107,   158,   200,   202,   203,   204,   206,   212,   232,   238,
     244,   256,   263,   266,   275,   105,   107,   280,   281,   282,
     283,   285,   105,   103,   102,   106,   106,   106,   106,   106,
     106,   106,   106,   106,   106,   108,   145,   100,   106,   106,
     106,   106,   108,   149,   106,   106,   108,   141,    70,   103,
     106,    83,   178,   107,   109,   183,   185,   186,     1,    34,
      35,    36,     1,    12,    18,    19,    32,    39,    45,    46,
     201,   105,   203,     1,    18,    28,   105,   282,   130,   294,
     102,   102,   102,   100,   102,   103,   102,   103,   100,   102,
     137,   102,   102,   102,   102,   102,   102,   106,   102,   106,
      52,   179,     1,   103,   102,   185,   105,   186,   105,   257,
     267,   264,   108,   239,    28,   207,   233,   205,   245,   213,
     202,   108,    28,    28,    82,    91,   296,   299,   108,   103,
     100,   106,   108,   108,   187,   184,    57,    79,    85,   156,
     210,   259,   260,   261,   262,   259,   261,    86,   291,   106,
     298,   291,   297,   293,   291,   106,   106,   106,   106,    92,
      93,   166,   100,   109,   185,   106,   106,   106,   260,   291,
     291,   291,   106,   156,   103,   210,    69,   236,   210,    59,
     220,    54,    60,    65,   214,   215,   216,   217,   218,   219,
     220,   221,   222,   103,   103,   103,   102,   106,   106,    84,
     156,   167,   168,   169,   170,   171,   172,   173,   102,   109,
     188,   101,    20,    33,    37,   102,   258,   268,   265,   102,
      38,   276,   156,   106,    56,   108,   219,   234,   235,   237,
     156,   106,   295,   106,   106,   106,   108,   217,   218,   286,
     284,   131,   102,   102,   106,    66,   174,   108,   168,   169,
     189,   102,   190,   107,   269,   270,   271,   273,   269,   269,
     292,   106,    82,    61,   108,   158,   208,   209,   211,   103,
     106,   235,   108,   208,   103,   221,   102,   103,   103,    82,
     108,   108,   300,   103,   106,   109,   191,   108,   109,   193,
       1,    21,    26,   105,   270,   105,   105,    90,   103,   106,
     106,   209,   108,    42,    43,    44,    94,    95,   246,   247,
     251,   223,   106,    69,   103,   109,    18,    63,   102,   108,
     274,   272,   106,   240,   103,   102,   106,   106,   220,   103,
     106,   102,   108,   108,   194,   298,   291,   102,    71,   277,
     102,   103,   221,   287,   102,   192,   109,   108,   108,   106,
     108,   252,   248,    73,   108,   109,   102,   102,    54,    97,
     106,   102,   195,   241,   106,   106,   103,   109,    72,   102,
     102,   224,   102,   106,   253,   249,    74,   196,   102,    69,
     108,   254,    96,   106,   109,   242,   106,   106,   102,   102,
      18,   102,   102,   225,   197,   106,   255,   250,    75,   109,
     102,   108,   108,   106,    79,   243,   102,   108,   108,   226,
      48,   106,   102,   227,    76,   106,   103,   228,    77,   106,
     102,   229,    78,   106,   102,   230,    49,   106,   102,   231,
     108
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (hyp, YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* This macro is provided for backward compatibility. */

#ifndef YY_LOCATION_PRINT
# define YY_LOCATION_PRINT(File, Loc) ((void) 0)
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value, hyp); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, class HypFile::Hyp *hyp)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep, hyp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    class HypFile::Hyp *hyp;
#endif
{
  if (!yyvaluep)
    return;
  YYUSE (hyp);
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep, class HypFile::Hyp *hyp)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep, hyp)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
    class HypFile::Hyp *hyp;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep, hyp);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule, class HypFile::Hyp *hyp)
#else
static void
yy_reduce_print (yyvsp, yyrule, hyp)
    YYSTYPE *yyvsp;
    int yyrule;
    class HypFile::Hyp *hyp;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       , hyp);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule, hyp); \
} while (YYID (0))

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
#ifndef	YYINITDEPTH
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
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
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
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
  YYSIZE_T yysize0 = yytnamerr (0, yytname[yytoken]);
  YYSIZE_T yysize = yysize0;
  YYSIZE_T yysize1;
  enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
  /* Internationalized format string. */
  const char *yyformat = 0;
  /* Arguments of yyformat. */
  char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
  /* Number of reported tokens (one for the "unexpected", one per
     "expected"). */
  int yycount = 0;

  /* There are many possibilities here to consider:
     - Assume YYFAIL is not used.  It's too flawed to consider.  See
       <http://lists.gnu.org/archive/html/bison-patches/2009-12/msg00024.html>
       for details.  YYERROR is fine as it does not invoke this
       function.
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
                yysize1 = yysize + yytnamerr (0, yytname[yyx]);
                if (! (yysize <= yysize1
                       && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
                  return 2;
                yysize = yysize1;
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

  yysize1 = yysize + yystrlen (yyformat);
  if (! (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM))
    return 2;
  yysize = yysize1;

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

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep, class HypFile::Hyp *hyp)
#else
static void
yydestruct (yymsg, yytype, yyvaluep, hyp)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
    class HypFile::Hyp *hyp;
#endif
{
  YYUSE (yyvaluep);
  YYUSE (hyp);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (class HypFile::Hyp *hyp);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;


/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (class HypFile::Hyp *hyp)
#else
int
yyparse (hyp)
    class HypFile::Hyp *hyp;
#endif
#endif
{
    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
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
  int yytoken;
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

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

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
      yychar = YYLEX;
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
  *++yyvsp = yylval;

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
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 19:

/* Line 1806 of yacc.c  */
#line 153 "parse.y"
    { if (hyp->exec_board_file(h)) YYERROR; }
    break;

  case 21:

/* Line 1806 of yacc.c  */
#line 158 "parse.y"
    { h.vers = yylval.floatval; }
    break;

  case 22:

/* Line 1806 of yacc.c  */
#line 158 "parse.y"
    { if (hyp->exec_version(h)) YYERROR; }
    break;

  case 23:

/* Line 1806 of yacc.c  */
#line 163 "parse.y"
    { if (hyp->exec_data_mode(h)) YYERROR; }
    break;

  case 24:

/* Line 1806 of yacc.c  */
#line 166 "parse.y"
    { h.detailed = false; }
    break;

  case 25:

/* Line 1806 of yacc.c  */
#line 167 "parse.y"
    { h.detailed = true; }
    break;

  case 26:

/* Line 1806 of yacc.c  */
#line 172 "parse.y"
    { if (hyp->exec_units(h)) YYERROR; }
    break;

  case 27:

/* Line 1806 of yacc.c  */
#line 175 "parse.y"
    { h.unit_system_english = true; }
    break;

  case 28:

/* Line 1806 of yacc.c  */
#line 176 "parse.y"
    { h.unit_system_english = false; }
    break;

  case 29:

/* Line 1806 of yacc.c  */
#line 179 "parse.y"
    { h.metal_thickness_weight = true; }
    break;

  case 30:

/* Line 1806 of yacc.c  */
#line 180 "parse.y"
    { h.metal_thickness_weight = false; }
    break;

  case 31:

/* Line 1806 of yacc.c  */
#line 184 "parse.y"
    { h.default_plane_separation = yylval.floatval; }
    break;

  case 32:

/* Line 1806 of yacc.c  */
#line 184 "parse.y"
    { if (hyp->exec_plane_sep(h)) YYERROR; }
    break;

  case 40:

/* Line 1806 of yacc.c  */
#line 202 "parse.y"
    { if (hyp->exec_perimeter_segment(h)) YYERROR; }
    break;

  case 41:

/* Line 1806 of yacc.c  */
#line 205 "parse.y"
    { if (hyp->exec_perimeter_arc(h)) YYERROR; }
    break;

  case 42:

/* Line 1806 of yacc.c  */
#line 208 "parse.y"
    { h.name = yylval.strval; }
    break;

  case 43:

/* Line 1806 of yacc.c  */
#line 208 "parse.y"
    { h.value = yylval.strval; }
    break;

  case 44:

/* Line 1806 of yacc.c  */
#line 208 "parse.y"
    { if (hyp->exec_board_attribute(h)) YYERROR; }
    break;

  case 53:

/* Line 1806 of yacc.c  */
#line 227 "parse.y"
    { if (hyp->exec_options(h)) YYERROR; }
    break;

  case 54:

/* Line 1806 of yacc.c  */
#line 230 "parse.y"
    { h.use_die_for_metal = yylval.boolval; }
    break;

  case 57:

/* Line 1806 of yacc.c  */
#line 235 "parse.y"
    { new_record(); }
    break;

  case 58:

/* Line 1806 of yacc.c  */
#line 235 "parse.y"
    { if (hyp->exec_signal(h)) YYERROR; }
    break;

  case 63:

/* Line 1806 of yacc.c  */
#line 244 "parse.y"
    { h.bulk_resistivity = yylval.floatval; h.bulk_resistivity_set = true; }
    break;

  case 71:

/* Line 1806 of yacc.c  */
#line 254 "parse.y"
    { new_record(); }
    break;

  case 72:

/* Line 1806 of yacc.c  */
#line 254 "parse.y"
    { if (hyp->exec_dielectric(h)) YYERROR; }
    break;

  case 76:

/* Line 1806 of yacc.c  */
#line 262 "parse.y"
    { h.epsilon_r = yylval.floatval; h.epsilon_r_set = true; }
    break;

  case 83:

/* Line 1806 of yacc.c  */
#line 272 "parse.y"
    { new_record(); }
    break;

  case 84:

/* Line 1806 of yacc.c  */
#line 272 "parse.y"
    { if (hyp->exec_plane(h)) YYERROR; }
    break;

  case 88:

/* Line 1806 of yacc.c  */
#line 280 "parse.y"
    { h.bulk_resistivity = yylval.floatval; h.bulk_resistivity_set = true; }
    break;

  case 96:

/* Line 1806 of yacc.c  */
#line 290 "parse.y"
    { h.thickness = yylval.floatval; h.thickness_set = true; }
    break;

  case 97:

/* Line 1806 of yacc.c  */
#line 293 "parse.y"
    { h.plating_thickness = yylval.floatval; h.plating_thickness_set = true; }
    break;

  case 98:

/* Line 1806 of yacc.c  */
#line 296 "parse.y"
    { h.bulk_resistivity = yylval.floatval; h.bulk_resistivity_set = true; }
    break;

  case 99:

/* Line 1806 of yacc.c  */
#line 299 "parse.y"
    { h.temperature_coefficient = yylval.floatval; h.temperature_coefficient_set = true; }
    break;

  case 100:

/* Line 1806 of yacc.c  */
#line 302 "parse.y"
    { h.epsilon_r = yylval.floatval; h.epsilon_r_set = true; }
    break;

  case 101:

/* Line 1806 of yacc.c  */
#line 305 "parse.y"
    { h.loss_tangent = yylval.floatval; h.loss_tangent_set = true; }
    break;

  case 102:

/* Line 1806 of yacc.c  */
#line 308 "parse.y"
    { h.layer_name = yylval.strval; h.layer_name_set = true; }
    break;

  case 103:

/* Line 1806 of yacc.c  */
#line 311 "parse.y"
    { h.material_name = yylval.strval; h.material_name_set = true; }
    break;

  case 104:

/* Line 1806 of yacc.c  */
#line 314 "parse.y"
    { h.plane_separation = yylval.floatval; h.plane_separation_set = true; }
    break;

  case 105:

/* Line 1806 of yacc.c  */
#line 317 "parse.y"
    { h.conformal = yylval.boolval; h.conformal_set = true; }
    break;

  case 106:

/* Line 1806 of yacc.c  */
#line 320 "parse.y"
    { h.prepreg = yylval.boolval; h.prepreg_set = true; }
    break;

  case 111:

/* Line 1806 of yacc.c  */
#line 333 "parse.y"
    { new_record(); }
    break;

  case 112:

/* Line 1806 of yacc.c  */
#line 333 "parse.y"
    { h.device_type = yylval.strval; }
    break;

  case 113:

/* Line 1806 of yacc.c  */
#line 333 "parse.y"
    { h.ref = yylval.strval; }
    break;

  case 114:

/* Line 1806 of yacc.c  */
#line 333 "parse.y"
    { if (hyp->exec_devices(h)) YYERROR; }
    break;

  case 122:

/* Line 1806 of yacc.c  */
#line 352 "parse.y"
    { h.name = yylval.strval; h.name_set = true; }
    break;

  case 125:

/* Line 1806 of yacc.c  */
#line 360 "parse.y"
    { h.value_float = yylval.floatval; h.value_float_set = true; }
    break;

  case 126:

/* Line 1806 of yacc.c  */
#line 363 "parse.y"
    { h.value_string = yylval.strval; h.value_string_set = true; }
    break;

  case 127:

/* Line 1806 of yacc.c  */
#line 366 "parse.y"
    { h.package = yylval.strval; h.package_set = true; }
    break;

  case 131:

/* Line 1806 of yacc.c  */
#line 378 "parse.y"
    { if (hyp->exec_supplies(h)) YYERROR; }
    break;

  case 133:

/* Line 1806 of yacc.c  */
#line 382 "parse.y"
    { h.voltage_specified = yylval.boolval; }
    break;

  case 134:

/* Line 1806 of yacc.c  */
#line 385 "parse.y"
    { h.conversion = yylval.boolval; }
    break;

  case 135:

/* Line 1806 of yacc.c  */
#line 390 "parse.y"
    { new_record(); }
    break;

  case 136:

/* Line 1806 of yacc.c  */
#line 390 "parse.y"
    { h.padstack_name = yylval.strval; h.padstack_name_set = true; }
    break;

  case 138:

/* Line 1806 of yacc.c  */
#line 393 "parse.y"
    { h.drill_size = yylval.floatval; h.drill_size_set = true; }
    break;

  case 144:

/* Line 1806 of yacc.c  */
#line 402 "parse.y"
    { h.layer_name = yylval.strval; h.layer_name_set = true; }
    break;

  case 145:

/* Line 1806 of yacc.c  */
#line 402 "parse.y"
    { if (hyp->exec_padstack_element(h)) YYERROR; new_record(); }
    break;

  case 147:

/* Line 1806 of yacc.c  */
#line 406 "parse.y"
    { h.pad_shape = yylval.floatval; }
    break;

  case 149:

/* Line 1806 of yacc.c  */
#line 407 "parse.y"
    { h.pad_shape = -1; }
    break;

  case 150:

/* Line 1806 of yacc.c  */
#line 411 "parse.y"
    { h.pad_sx = yylval.floatval; }
    break;

  case 151:

/* Line 1806 of yacc.c  */
#line 411 "parse.y"
    { h.pad_sy = yylval.floatval; }
    break;

  case 152:

/* Line 1806 of yacc.c  */
#line 411 "parse.y"
    { h.pad_angle = yylval.floatval; }
    break;

  case 154:

/* Line 1806 of yacc.c  */
#line 415 "parse.y"
    { h.pad_type = PAD_TYPE_METAL; h.pad_type_set = true; }
    break;

  case 155:

/* Line 1806 of yacc.c  */
#line 416 "parse.y"
    { h.pad_type = PAD_TYPE_ANTIPAD; h.pad_type_set = true; }
    break;

  case 156:

/* Line 1806 of yacc.c  */
#line 417 "parse.y"
    { h.thermal_clear_shape = yylval.floatval; }
    break;

  case 157:

/* Line 1806 of yacc.c  */
#line 418 "parse.y"
    { h.thermal_clear_sx = yylval.floatval; }
    break;

  case 158:

/* Line 1806 of yacc.c  */
#line 419 "parse.y"
    { h.thermal_clear_sy = yylval.floatval; }
    break;

  case 159:

/* Line 1806 of yacc.c  */
#line 420 "parse.y"
    { h.thermal_clear_angle = yylval.floatval; }
    break;

  case 160:

/* Line 1806 of yacc.c  */
#line 421 "parse.y"
    { h.pad_type = PAD_TYPE_THERMAL_RELIEF; h.pad_type_set = true; }
    break;

  case 161:

/* Line 1806 of yacc.c  */
#line 427 "parse.y"
    { h.net_name = yylval.strval; if (hyp->exec_net(h)) YYERROR; }
    break;

  case 163:

/* Line 1806 of yacc.c  */
#line 430 "parse.y"
    { if (hyp->exec_net_plane_separation(h)) YYERROR; }
    break;

  case 180:

/* Line 1806 of yacc.c  */
#line 453 "parse.y"
    { new_record(); }
    break;

  case 181:

/* Line 1806 of yacc.c  */
#line 453 "parse.y"
    { if (hyp->exec_seg(h)) YYERROR; }
    break;

  case 182:

/* Line 1806 of yacc.c  */
#line 456 "parse.y"
    { new_record(); }
    break;

  case 183:

/* Line 1806 of yacc.c  */
#line 456 "parse.y"
    { if (hyp->exec_arc(h)) YYERROR; }
    break;

  case 188:

/* Line 1806 of yacc.c  */
#line 469 "parse.y"
    { h.width = yylval.floatval; h.width_set = true; }
    break;

  case 189:

/* Line 1806 of yacc.c  */
#line 472 "parse.y"
    { h.left_plane_separation = yylval.floatval; h.left_plane_separation_set = true; }
    break;

  case 190:

/* Line 1806 of yacc.c  */
#line 475 "parse.y"
    { new_record(); }
    break;

  case 194:

/* Line 1806 of yacc.c  */
#line 484 "parse.y"
    { if (hyp->exec_via(h)) YYERROR; }
    break;

  case 200:

/* Line 1806 of yacc.c  */
#line 501 "parse.y"
    { h.padstack_name = yylval.strval; h.padstack_name_set = true; }
    break;

  case 201:

/* Line 1806 of yacc.c  */
#line 504 "parse.y"
    { h.layer1_name = yylval.strval; h.layer1_name_set = true; }
    break;

  case 202:

/* Line 1806 of yacc.c  */
#line 507 "parse.y"
    { h.layer2_name = yylval.strval; h.layer2_name_set = true; }
    break;

  case 203:

/* Line 1806 of yacc.c  */
#line 510 "parse.y"
    { h.drill_size = yylval.floatval; }
    break;

  case 204:

/* Line 1806 of yacc.c  */
#line 513 "parse.y"
    { h.pad1_shape = yylval.strval; }
    break;

  case 205:

/* Line 1806 of yacc.c  */
#line 514 "parse.y"
    { h.pad1_sx = yylval.floatval; }
    break;

  case 206:

/* Line 1806 of yacc.c  */
#line 515 "parse.y"
    { h.pad1_sy = yylval.floatval; }
    break;

  case 207:

/* Line 1806 of yacc.c  */
#line 516 "parse.y"
    { h.pad1_angle = yylval.floatval; }
    break;

  case 208:

/* Line 1806 of yacc.c  */
#line 517 "parse.y"
    { h.pad2_shape = yylval.strval; }
    break;

  case 209:

/* Line 1806 of yacc.c  */
#line 518 "parse.y"
    { h.pad2_sx = yylval.floatval; }
    break;

  case 210:

/* Line 1806 of yacc.c  */
#line 519 "parse.y"
    { h.pad2_sy = yylval.floatval; }
    break;

  case 211:

/* Line 1806 of yacc.c  */
#line 520 "parse.y"
    { h.pad2_angle  = yylval.floatval; }
    break;

  case 212:

/* Line 1806 of yacc.c  */
#line 521 "parse.y"
    { if (hyp->exec_via_v1(h)) YYERROR; }
    break;

  case 213:

/* Line 1806 of yacc.c  */
#line 525 "parse.y"
    { new_record(); }
    break;

  case 214:

/* Line 1806 of yacc.c  */
#line 525 "parse.y"
    { if (hyp->exec_pin(h)) YYERROR; }
    break;

  case 219:

/* Line 1806 of yacc.c  */
#line 538 "parse.y"
    { h.pin_reference = yylval.strval; h.pin_reference_set = true; }
    break;

  case 220:

/* Line 1806 of yacc.c  */
#line 541 "parse.y"
    { h.pin_function = PIN_SIM_OUT; h.pin_function_set = true; }
    break;

  case 221:

/* Line 1806 of yacc.c  */
#line 542 "parse.y"
    { h.pin_function = PIN_SIM_IN; h.pin_function_set = true; }
    break;

  case 222:

/* Line 1806 of yacc.c  */
#line 543 "parse.y"
    { h.pin_function = PIN_SIM_BOTH; h.pin_function_set = true; }
    break;

  case 223:

/* Line 1806 of yacc.c  */
#line 547 "parse.y"
    { new_record(); }
    break;

  case 224:

/* Line 1806 of yacc.c  */
#line 550 "parse.y"
    { h.pad1_shape = yylval.strval; }
    break;

  case 225:

/* Line 1806 of yacc.c  */
#line 551 "parse.y"
    { h.pad1_sx = yylval.floatval; }
    break;

  case 226:

/* Line 1806 of yacc.c  */
#line 552 "parse.y"
    { h.pad1_sy = yylval.floatval; }
    break;

  case 227:

/* Line 1806 of yacc.c  */
#line 553 "parse.y"
    { h.pad1_angle = yylval.floatval; }
    break;

  case 228:

/* Line 1806 of yacc.c  */
#line 554 "parse.y"
    { if (hyp->exec_pad(h)) YYERROR; }
    break;

  case 229:

/* Line 1806 of yacc.c  */
#line 558 "parse.y"
    { new_record(); }
    break;

  case 230:

/* Line 1806 of yacc.c  */
#line 558 "parse.y"
    { if (hyp->exec_useg(h)) YYERROR; }
    break;

  case 233:

/* Line 1806 of yacc.c  */
#line 566 "parse.y"
    { h.zlayer_name = yylval.strval; h.zlayer_name_set = true; }
    break;

  case 234:

/* Line 1806 of yacc.c  */
#line 567 "parse.y"
    { h.width = yylval.floatval; }
    break;

  case 235:

/* Line 1806 of yacc.c  */
#line 568 "parse.y"
    { h.length = yylval.floatval; }
    break;

  case 237:

/* Line 1806 of yacc.c  */
#line 573 "parse.y"
    { h.impedance = yylval.floatval; h.impedance_set = true; }
    break;

  case 238:

/* Line 1806 of yacc.c  */
#line 574 "parse.y"
    { h.delay = yylval.floatval; }
    break;

  case 240:

/* Line 1806 of yacc.c  */
#line 578 "parse.y"
    { h.resistance = yylval.floatval; h.resistance_set = true;}
    break;

  case 243:

/* Line 1806 of yacc.c  */
#line 584 "parse.y"
    { new_record(); }
    break;

  case 244:

/* Line 1806 of yacc.c  */
#line 584 "parse.y"
    { if (hyp->exec_polygon_begin(h)) YYERROR; }
    break;

  case 245:

/* Line 1806 of yacc.c  */
#line 585 "parse.y"
    { if (hyp->exec_polygon_end(h)) YYERROR; }
    break;

  case 252:

/* Line 1806 of yacc.c  */
#line 600 "parse.y"
    { h.id = yylval.intval; h.id_set = true; }
    break;

  case 253:

/* Line 1806 of yacc.c  */
#line 604 "parse.y"
    { h.polygon_type = POLYGON_TYPE_POUR; h.polygon_type_set = true; }
    break;

  case 254:

/* Line 1806 of yacc.c  */
#line 605 "parse.y"
    { h.polygon_type = POLYGON_TYPE_PLANE; h.polygon_type_set = true; }
    break;

  case 255:

/* Line 1806 of yacc.c  */
#line 606 "parse.y"
    { h.polygon_type = POLYGON_TYPE_COPPER; h.polygon_type_set = true; }
    break;

  case 256:

/* Line 1806 of yacc.c  */
#line 610 "parse.y"
    { new_record(); }
    break;

  case 257:

/* Line 1806 of yacc.c  */
#line 610 "parse.y"
    { if (hyp->exec_polyvoid_begin(h)) YYERROR; }
    break;

  case 258:

/* Line 1806 of yacc.c  */
#line 611 "parse.y"
    { if (hyp->exec_polyvoid_end(h)) YYERROR; }
    break;

  case 259:

/* Line 1806 of yacc.c  */
#line 614 "parse.y"
    { new_record(); }
    break;

  case 260:

/* Line 1806 of yacc.c  */
#line 614 "parse.y"
    { if (hyp->exec_polyline_begin(h)) YYERROR; }
    break;

  case 261:

/* Line 1806 of yacc.c  */
#line 615 "parse.y"
    { if (hyp->exec_polyline_end(h)) YYERROR; }
    break;

  case 267:

/* Line 1806 of yacc.c  */
#line 629 "parse.y"
    { new_record(); }
    break;

  case 268:

/* Line 1806 of yacc.c  */
#line 629 "parse.y"
    { if (hyp->exec_line(h)) YYERROR; }
    break;

  case 269:

/* Line 1806 of yacc.c  */
#line 632 "parse.y"
    { new_record(); }
    break;

  case 270:

/* Line 1806 of yacc.c  */
#line 632 "parse.y"
    { if (hyp->exec_curve(h)) YYERROR; }
    break;

  case 271:

/* Line 1806 of yacc.c  */
#line 635 "parse.y"
    { h.name = yylval.strval; }
    break;

  case 272:

/* Line 1806 of yacc.c  */
#line 635 "parse.y"
    { h.value = yylval.strval; }
    break;

  case 273:

/* Line 1806 of yacc.c  */
#line 635 "parse.y"
    { if (hyp->exec_net_attribute(h)) YYERROR; }
    break;

  case 274:

/* Line 1806 of yacc.c  */
#line 640 "parse.y"
    { h.net_class_name = yylval.strval; if (hyp->exec_net_class(h)) YYERROR; }
    break;

  case 283:

/* Line 1806 of yacc.c  */
#line 659 "parse.y"
    { h.net_name = yylval.strval; }
    break;

  case 284:

/* Line 1806 of yacc.c  */
#line 659 "parse.y"
    { if (hyp->exec_net_class_element(h)) YYERROR; }
    break;

  case 285:

/* Line 1806 of yacc.c  */
#line 662 "parse.y"
    { h.name = yylval.strval; }
    break;

  case 286:

/* Line 1806 of yacc.c  */
#line 662 "parse.y"
    { h.value = yylval.strval; }
    break;

  case 287:

/* Line 1806 of yacc.c  */
#line 662 "parse.y"
    { if (hyp->exec_net_class_attribute(h)) YYERROR; }
    break;

  case 288:

/* Line 1806 of yacc.c  */
#line 667 "parse.y"
    { if (hyp->exec_end(h)) YYERROR; }
    break;

  case 289:

/* Line 1806 of yacc.c  */
#line 672 "parse.y"
    { h.key = yylval.strval; }
    break;

  case 290:

/* Line 1806 of yacc.c  */
#line 672 "parse.y"
    { if (hyp->exec_key(h)) YYERROR; }
    break;

  case 291:

/* Line 1806 of yacc.c  */
#line 677 "parse.y"
    { h.x = yylval.floatval; }
    break;

  case 292:

/* Line 1806 of yacc.c  */
#line 677 "parse.y"
    { h.y = yylval.floatval; }
    break;

  case 293:

/* Line 1806 of yacc.c  */
#line 680 "parse.y"
    { h.x1 = yylval.floatval; }
    break;

  case 294:

/* Line 1806 of yacc.c  */
#line 680 "parse.y"
    { h.y1 = yylval.floatval; }
    break;

  case 295:

/* Line 1806 of yacc.c  */
#line 683 "parse.y"
    { h.x2 = yylval.floatval; }
    break;

  case 296:

/* Line 1806 of yacc.c  */
#line 683 "parse.y"
    { h.y2 = yylval.floatval; }
    break;

  case 298:

/* Line 1806 of yacc.c  */
#line 689 "parse.y"
    { h.xc = yylval.floatval; }
    break;

  case 299:

/* Line 1806 of yacc.c  */
#line 689 "parse.y"
    { h.yc = yylval.floatval; }
    break;

  case 300:

/* Line 1806 of yacc.c  */
#line 689 "parse.y"
    { h.r = yylval.floatval; }
    break;



/* Line 1806 of yacc.c  */
#line 3280 "parse.c"
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

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYEMPTY : YYTRANSLATE (yychar);

  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (hyp, YY_("syntax error"));
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
        yyerror (hyp, yymsgp);
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
		      yytoken, &yylval, hyp);
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

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

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
		  yystos[yystate], yyvsp, hyp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


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

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (hyp, YY_("memory exhausted"));
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
                  yytoken, &yylval, hyp);
    }
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp, hyp);
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
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 2067 of yacc.c  */
#line 691 "parse.y"


/*
 * Supporting C routines
 */

void yyerror(HypFile::Hyp *hyp, const char *msg)
{
  std::ostringstream err_msg;

  err_msg << "line " << yylineno << ": " << msg << " at '" << yytext << "'";
  hyp->error(err_msg.str());
}

void yyprint(FILE *file, int type, YYSTYPE value)
{
  if (type == STRING)
    fprintf (file, "%s", value.strval);
   else if (type == FLOAT)
    fprintf (file, "%g", value.floatval);
   else if (type == BOOL)
    fprintf (file, "%i", value.boolval);
  return;
}

/*
 * reset parse_param struct at beginning of record
 */

void new_record()
{
  h.vers = 0;
  h.detailed = false;
  h.unit_system_english = false;
  h.metal_thickness_weight = false;
  h.default_plane_separation = 0;
  h.use_die_for_metal = false;
  h.bulk_resistivity = 0;
  h.conformal = false;
  h.epsilon_r = 0;
  h.layer_name.clear();
  h.loss_tangent = 0;
  h.material_name.clear();
  h.plane_separation = 0;
  h.plating_thickness = 0;
  h.prepreg = false;
  h.temperature_coefficient = 0;
  h.thickness = 0;
  h.bulk_resistivity_set = false;
  h.conformal_set = false;
  h.epsilon_r_set = false;
  h.layer_name_set = false;
  h.loss_tangent_set = false;
  h.material_name_set = false;
  h.plane_separation_set = false;
  h.plating_thickness_set = false;
  h.prepreg_set = false;
  h.temperature_coefficient_set = false;
  h.thickness_set = false;
  h.device_type.clear();
  h.ref.clear();
  h.value_float = 0;
  h.value_string.clear();
  h.package.clear();
  h.name_set = false;
  h.value_float_set = false;
  h.value_string_set = false;
  h.package_set = false;
  h.voltage_specified = false;
  h.conversion = false;
  h.padstack_name.clear();
  h.drill_size = 0;
  h.pad_shape = 0;
  h.pad_sx = 0;
  h.pad_sy = 0;
  h.pad_angle = 0;
  h.thermal_clear_shape = 0;
  h.thermal_clear_sx = 0;
  h.thermal_clear_sy = 0;
  h.thermal_clear_angle = 0;
  h.pad_type = PAD_TYPE_METAL;
  h.padstack_name_set = false;
  h.drill_size_set = false;
  h.pad_type_set = false;
  h.width = 0;
  h.left_plane_separation = 0;
  h.width_set = false;
  h.left_plane_separation_set = false;
  h.layer1_name.clear();
  h.layer1_name_set = false;
  h.layer2_name.clear();
  h.layer2_name_set = false;
  h.pad1_shape.clear();
  h.pad1_sx = 0;
  h.pad1_sy = 0;
  h.pad1_angle = 0;
  h.pad2_shape.clear();
  h.pad2_sx = 0;
  h.pad2_sy = 0;
  h.pad2_angle = 0;
  h.pin_reference.clear();
  h.pin_reference_set = false;
  h.pin_function = PIN_SIM_BOTH;
  h.pin_function_set = false;
  h.zlayer_name.clear();
  h.zlayer_name_set = false;
  h.length = 0;
  h.impedance = 0;
  h.impedance_set = false;
  h.delay = 0;
  h.resistance = 0;
  h.resistance_set = false;
  h.id = -1;
  h.id_set = false;
  h.polygon_type = POLYGON_TYPE_PLANE;
  h.polygon_type_set = false;
  h.net_class_name.clear();
  h.net_name.clear();
  h.key.clear();
  h.name.clear();
  h.value.clear();
  h.x = 0;
  h.y = 0;
  h.x1 = 0;
  h.y1 = 0;
  h.x2 = 0;
  h.y2 = 0;
  h.xc = 0;
  h.yc = 0;
  h.r = 0;

  return;
}

/* not truncated */

