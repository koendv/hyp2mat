/* A Bison parser, made by GNU Bison 2.5.  */

/* Bison interface for Yacc-like parsers in C
   
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

/* "%code requires" blocks.  */

/* Line 2068 of yacc.c  */
#line 21 "parse.yy"

#include "hypfile.h"



/* Line 2068 of yacc.c  */
#line 44 "parse.h"

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

/* Line 2068 of yacc.c  */
#line 30 "parse.yy"

    int boolval;
    int intval;
    double floatval;
    char* strval;



/* Line 2068 of yacc.c  */
#line 276 "parse.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


