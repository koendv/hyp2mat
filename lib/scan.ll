
/*
 * hyp2mat - convert hyperlynx files to matlab scripts
 * Copyright 2012 Koen De Vleeschauwer.
 *
 * This file is part of hyp2mat.
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

%option noyywrap nodefault yylineno debug

%{
#include <stdlib.h>
#include "parse.h"
#include "parser.h"

/* copy a string between quotes */
char *strunquote(const char *);

/* remember hyperlynx file section we're in */
int section = -1;

%}

 /*
  * The scanner knows five states: INITIAL, STATE_STRING, STATE_POSINT, STATE_FLOAT and STATE_COMMENT
  *
  * In state INITIAL the scanner recognizes:
  * - punctuation (such as {}()=, )
  * - keywords (such as VERSION, BOARD, NET), 
  * - strings (both between double quotes " and unquoted),
  * - and floating point numbers (with optional suffix, eg. 10 uF), 
  *
  * In STATE_STRING the scanner recognizes punctuation and strings only.
  * This avoids unquoted strings being interpreted as numbers, or keywords.
  *
  * In STATE_POSINT the scanner recognizes a positive integer.
  *
  * In STATE_FLOAT the scanner recognizes a floating point number (without suffix).
  *
  * In STATE_COMMENT the scanner discards all text up to the next 
  * right brace } , and then continues in state INITIAL.
  *
  */

%x STATE_STRING STATE_POSINT STATE_FLOAT STATE_COMMENT STATE_COMMENT_EOL

 /* whitespace: space, tab, vertical tab, form feed */
WS                  [ \t\v\f]

 /* accept windows and unix newlines */
NEWLINE             [\r\n]+

 /* 
  * lines with an asterisk in the first column are comments 
  */
COMMENT             ^\*[^\n\r]*[\n\r]+

 /* Left-hand side of an assignment: check whether next token is equals sign */
LHS                 [ \t\v\f]*"="

 /*
  * Positive integer
  */

POSINT              [0-9]+

 /*
  * Floating point numbers
  */

 /* ordinary floating point numbers */
SIMPLE_FLOAT        [-+]?([0-9]+"."[0-9]*|"."?[0-9]+)([Ee][-+]?[0-9]+)?

 /* floating point numbers with suffix, e,g. pF, nH */
FLOAT_SUFFIX        [A-Za-z]*{WS}+
FLOAT_YOTTA         {SIMPLE_FLOAT}{WS}*"Y"{FLOAT_SUFFIX}
FLOAT_ZETA          {SIMPLE_FLOAT}{WS}*"Z"{FLOAT_SUFFIX}
FLOAT_EXA           {SIMPLE_FLOAT}{WS}*"E"{FLOAT_SUFFIX}
FLOAT_PETA          {SIMPLE_FLOAT}{WS}*"P"{FLOAT_SUFFIX}
FLOAT_TERA          {SIMPLE_FLOAT}{WS}*"T"{FLOAT_SUFFIX}
FLOAT_GIGA          {SIMPLE_FLOAT}{WS}*"G"{FLOAT_SUFFIX}
FLOAT_MEGA          {SIMPLE_FLOAT}{WS}*"M"{FLOAT_SUFFIX}
FLOAT_KILO          {SIMPLE_FLOAT}{WS}*[Kk]{FLOAT_SUFFIX}
FLOAT_MILLI         {SIMPLE_FLOAT}{WS}*"m"{FLOAT_SUFFIX}
FLOAT_MICRO         {SIMPLE_FLOAT}{WS}*[uU]{FLOAT_SUFFIX}
FLOAT_NANO          {SIMPLE_FLOAT}{WS}*[nN]{FLOAT_SUFFIX}
FLOAT_PICO          {SIMPLE_FLOAT}{WS}*[pP]{FLOAT_SUFFIX}
FLOAT_FEMTO         {SIMPLE_FLOAT}{WS}*[fF]{FLOAT_SUFFIX}
FLOAT_ATTO          {SIMPLE_FLOAT}{WS}*"a"{FLOAT_SUFFIX}
FLOAT_ZEPTO         {SIMPLE_FLOAT}{WS}*"z"{FLOAT_SUFFIX}
FLOAT_YOCTO         {SIMPLE_FLOAT}{WS}*"y"{FLOAT_SUFFIX}
  
 /* 
  * Strings
  */

 /* an unquoted string */
 /* a sequence of characters, excluding whitespace and reserved characters.
  * also allow strings such as "(Net0)" .
  */
STRING              [^ \t\v\f\r\n\{\}\(\)=\"]+|"("[[:alnum:]]+")"

 /* a string enclosed in double quotes " */
QUOTED_STRING       \"([^\"\n]|\"\")*\"

/* an unquoted string with spaces */
CHARS_AND_SPACES       [^\t\v\f\r\n\{\}\(\)=\"]+

/* all variables used in assignments */
VARIABLE            ("A"|"A1"|"A2"|"BR"|"C"|"C\?"|"CO\?"|"D"|"ER"|"F"|"ID"|"L"|"L1"|"L2"|"LPS"|"LT"|"M"|"N"|"NAME"|"P"|"PKG"|"PR\?"|"PS"|"R"|"REF"|"S"|"SX"|"SY"|"S1"|"S1X"|"S1Y"|"S2"|"S2X"|"S2Y"|"T"|"TC"|"USE_DIE_FOR_METAL"|"V"|"V\?"|"VAL"|"W"|"X"|"X1"|"X2"|"XC"|"Y"|"Y1"|"Y2"|"YC"|"Z"|"ZL"|"ZLEN"|"ZW")

/* an unquoted string with spaces is terminated by the next assignment or the end of line */
STRING_W_SPACES     {CHARS_AND_SPACES}(" "{VARIABLE}"=")?

/* an empty string is terminated by the next assignment, a ')' or a '}' */
EMPTY_STRING        ({WS}{VARIABLE}"="|")"|"}")

%%

 /* When in STATE_COMMENT skip all comment until next right brace */
<STATE_COMMENT>{
[^\}]*              { BEGIN INITIAL; /* skip all comment until next right brace */ }
}

 /* When in STATE_COMMENT_EOL skip all comment until end-of-line */
<STATE_COMMENT_EOL>{
[^\r\n]*{NEWLINE}+ { BEGIN INITIAL; /* skip all comment until next end-of-line */ }
}


 /* skip comments and whitespace */
<*>{

  {COMMENT}           { /* skip comments */ }

  {WS}+               { /* skip whitespace */ }

  {NEWLINE}+          { /* skip newlines */ }

}

 /*
  * Hyperlynx keywords 
  */

 /* Sections */

"BOARD_FILE"        {section = BOARD_FILE; return BOARD_FILE;}
"VERSION"           {section = VERSION; return VERSION;}
"DATA_MODE"         {section = DATA_MODE; return DATA_MODE;}
"UNITS"             {section = UNITS; return UNITS;}
"PLANE_SEP"         {section = PLANE_SEP; return PLANE_SEP;}
"BOARD"             {section = BOARD; BEGIN STATE_COMMENT_EOL; return BOARD;}
"STACKUP"           {section = STACKUP; BEGIN STATE_COMMENT_EOL; return STACKUP;}
"DEVICES"           {section = DEVICES; BEGIN STATE_COMMENT_EOL; return DEVICES;}
"SUPPLIES"          {section = SUPPLIES; BEGIN STATE_COMMENT_EOL; return SUPPLIES;}
"PADSTACK"          {section = PADSTACK; BEGIN STATE_STRING; return PADSTACK;}
"NET"               {section = NET; BEGIN STATE_STRING; return NET;}
"NET_CLASS"         {section = NET_CLASS; return NET_CLASS;}
"END"               {section = END; return END;}
"KEY"               {section = KEY; return KEY;}

 /* Keywords */

"A"                 {return A;}
"ARC"               {return ARC;}
"COPPER"            {return COPPER;}
"CURVE"             {return CURVE;}
"DETAILED"          {if (section == DATA_MODE) BEGIN STATE_COMMENT; return DETAILED;}
"DIELECTRIC"        {return DIELECTRIC;}
"ENGLISH"           {return ENGLISH;}
"LENGTH"            {if (section == UNITS) BEGIN STATE_COMMENT; return LENGTH;}
"LINE"              {return LINE;}
"METRIC"            {return METRIC;}
"M"                 {return M;}
"N"                 {return N;}
"OPTIONS"           {return OPTIONS;}
"PAD"               {return PAD;}
"PERIMETER_ARC"     {return PERIMETER_ARC;}
"PERIMETER_SEGMENT" {return PERIMETER_SEGMENT;}
"PIN"               {return PIN;}
"PLANE"             {return PLANE;}
"POLYGON"           {return POLYGON;}
"POLYLINE"          {return POLYLINE;}
"POLYVOID"          {return POLYVOID;}
"POUR"              {return POUR;}
"S"                 {return S;}
"T"                 {return T;}
"SEG"               {return SEG;}
"SIGNAL"            {return SIGNAL;}
"SIMPLIFIED"        {if (section == DATA_MODE) BEGIN STATE_COMMENT; return SIMPLIFIED; }
"SIM_BOTH"          {return SIM_BOTH;}
"SIM_IN"            {return SIM_IN;}
"SIM_OUT"           {return SIM_OUT;}
"USEG"              {return USEG;}
"VIA"               {return VIA;}
"WEIGHT"            {if (section == UNITS) BEGIN STATE_COMMENT; return WEIGHT;}

 /* Assignments */

"A"/{LHS}           {return A;}
"A1"/{LHS}          {return A1;}
"A2"/{LHS}          {return A2;}
"BR"/{LHS}          {return BR;}
"C"/{LHS}           {return C;}
"C?"/{LHS}          {return C_QM;}
"CO?"/{LHS}         {return CO_QM;}
"D"/{LHS}           {return D;}
"ER"/{LHS}          {return ER;}
"F"/{LHS}           {return F;}
"ID"/{LHS}          {BEGIN STATE_POSINT; return ID;}
"L"/{LHS}           {BEGIN STATE_STRING; return L;}
"L1"/{LHS}          {BEGIN STATE_STRING; return L1;}
"L2"/{LHS}          {BEGIN STATE_STRING; return L2;}
"LPS"/{LHS}         {return LPS;}
"LT"/{LHS}          {return LT;}
"M"/{LHS}           {BEGIN STATE_STRING; return M;}
"N"/{LHS}           {BEGIN STATE_STRING; return N;}
"NAME"/{LHS}        {BEGIN STATE_STRING; return NAME;}
 /* P is used as "plating thickness" in "stackup/signal" and as "padstack" in "net/via" */
"P"/{LHS}           {if (section == NET) BEGIN STATE_STRING; return P;}
"PKG"/{LHS}         {BEGIN STATE_STRING; return PKG;}
"PR?"/{LHS}         {return PR_QM;}
"PS"/{LHS}          {return PS;}
"R"/{LHS}           {return R;}
"REF"/{LHS}         {BEGIN STATE_STRING; return REF;}
"S"/{LHS}           {BEGIN STATE_STRING; return S;}
"SX"/{LHS}          {return SX;}
"SY"/{LHS}          {return SY;}
"S1"/{LHS}          {BEGIN STATE_STRING; return S1;}
"S1X"/{LHS}         {return S1X;}
"S1Y"/{LHS}         {return S1Y;}
"S2"/{LHS}          {BEGIN STATE_STRING; return S2;}
"S2X"/{LHS}         {return S2X;}
"S2Y"/{LHS}         {return S2Y;}
"T"/{LHS}           {return T;}
"TC"/{LHS}          {return TC;}
"USE_DIE_FOR_METAL"/{LHS}           {return USE_DIE_FOR_METAL;}
"V"/{LHS}           {BEGIN STATE_STRING; return V;}
"V?"/{LHS}          {return V_QM;}
"VAL"/{LHS}         {if (section == DEVICES) BEGIN STATE_STRING; return VAL;}
"W"/{LHS}           {return W;}
"X"/{LHS}           {return X;}
"X1"/{LHS}          {return X1;}
"X2"/{LHS}          {return X2;}
"XC"/{LHS}          {return XC;}
"Y"/{LHS}           {return Y;}
"Y1"/{LHS}          {return Y1;}
"Y2"/{LHS}          {return Y2;}
"YC"/{LHS}          {return YC;}
"Z"/{LHS}           {return Z;}
"ZL"/{LHS}          {BEGIN STATE_STRING; return ZL;}
"ZLEN"/{LHS}        {return ZLEN;}
"ZW"/{LHS}          {return ZW;}
 
 /* Booleans */

"YES"|"yes"         {yylval.boolval = 1; return BOOL; }
"NO"|"no"           {yylval.boolval = 0; return BOOL; }

 /* Floats */

                    /* ordinary floating point numbers */
{SIMPLE_FLOAT}      {yylval.floatval = strtod(yytext, NULL); return FLOAT;}

                    /* floating point numbers with suffix, e,g. pF, nH */
{FLOAT_YOTTA}       {yylval.floatval = strtod(yytext, NULL) * 1e24; return FLOAT;}
{FLOAT_ZETA}        {yylval.floatval = strtod(yytext, NULL) * 1e21; return FLOAT;}
{FLOAT_EXA}         {yylval.floatval = strtod(yytext, NULL) * 1e18; return FLOAT;}
{FLOAT_PETA}        {yylval.floatval = strtod(yytext, NULL) * 1e15; return FLOAT;}
{FLOAT_TERA}        {yylval.floatval = strtod(yytext, NULL) * 1e12; return FLOAT;}
{FLOAT_GIGA}        {yylval.floatval = strtod(yytext, NULL) * 1e9; return FLOAT;}
{FLOAT_MEGA}        {yylval.floatval = strtod(yytext, NULL) * 1e6; return FLOAT;}
{FLOAT_KILO}        {yylval.floatval = strtod(yytext, NULL) * 1e3; return FLOAT;}
{FLOAT_MILLI}       {yylval.floatval = strtod(yytext, NULL) * 1e-3; return FLOAT;}
{FLOAT_MICRO}       {yylval.floatval = strtod(yytext, NULL) * 1e-6; return FLOAT;}
{FLOAT_NANO}        {yylval.floatval = strtod(yytext, NULL) * 1e-9; return FLOAT;}
{FLOAT_PICO}        {yylval.floatval = strtod(yytext, NULL) * 1e-12; return FLOAT;}
{FLOAT_FEMTO}       {yylval.floatval = strtod(yytext, NULL) * 1e-15; return FLOAT;}
{FLOAT_ATTO}        {yylval.floatval = strtod(yytext, NULL) * 1e-18; return FLOAT;}
{FLOAT_ZEPTO}       {yylval.floatval = strtod(yytext, NULL) * 1e-21; return FLOAT;}
{FLOAT_YOCTO}       {yylval.floatval = strtod(yytext, NULL) * 1e-24; return FLOAT;}

                    /* floating point numbers in VERSION and PLANE_SEP have no suffix and are followed by optional comments */
<STATE_FLOAT>{
{SIMPLE_FLOAT}      {yylval.floatval = strtod(yytext, NULL); BEGIN STATE_COMMENT; return FLOAT;}
}

                    /* A positive integer is used only in polygon/polyline/polyvoid "ID = posint" */
<STATE_POSINT>{
{POSINT}            { BEGIN INITIAL; yylval.intval = atoi(yytext); return POSINT; }
}

 
                    /* 
                     * This is a workaround for syntactically incorrect .hyp files.
                     * We accept the following constructs as representing an empty string:
                     * NAME= L1=somelayer
                     * NAME= )
                     * NAME= }
                     * and we accept the following constructs as representing "a string with spaces"
                     * NAME=a string with spaces L1=somelayer
                     */
<STATE_STRING>{

{EMPTY_STRING}      { yyless(0); BEGIN INITIAL; yylval.strval = strdup(""); return STRING; } /* emit empty string and reprocess */

{STRING_W_SPACES}   {
                      /*
                       * Commas are not allowed in strings in the padstack section unless the string is enclosed in double quotes (").
                       */

                      if ((section == PADSTACK) && strchr(yytext, ','))
                        REJECT 
                      else 
                      { 
                        char *s = strdup(yytext);
  
                        BEGIN INITIAL; 
  
                        /* strip final ' VAR=' */
                        if ((strlen(s) != 0) && (s[strlen(s)-1] == '=')) {
                          char* space = strrchr(s, ' ');
                          /* strip trailing spaces */
                          if (space != NULL)
                            while ((space >= s) && (*space == ' ')) *space-- = '\0';
                          yyless(strlen(s));
                          }
  
                        yylval.strval = s;
  
                        return STRING;
                      }
                    }
}

<*>{

  "{"                 {return '{';}

  "}"                 {BEGIN STATE_COMMENT_EOL; return '}';}

  "("                 {if (section == PADSTACK) BEGIN STATE_STRING; return '(';}

                      /* allow for comment after the closing bracket ) */
  ")"                 {BEGIN STATE_COMMENT_EOL; return ')';}

  ","                 {return ',';}

  "="                 {if ((section == VERSION) || (section == PLANE_SEP)) BEGIN STATE_FLOAT; return '=';}

                      /* string */
  {STRING}            { 
                        /*
                         * Commas are not allowed in strings in the padstack section unless the string is enclosed in double quotes (").
                         */

                        if ((section == PADSTACK) && strchr(yytext, ','))
                          REJECT 
                        else 
                        { 
                          BEGIN INITIAL; 
                          yylval.strval = strdup(yytext); 
                          return STRING;
                        }
                      }

                      /* string in double quotes */
  {QUOTED_STRING}     {BEGIN INITIAL; yylval.strval = strunquote(yytext); return STRING;}

  <<EOF>>             {yyterminate();}

                      /* have bison catch all unrecognized characters with parse errors */
  .                   {return yytext[0];}

}

%%

 /* 
  * copy a quoted string.
  * e.g. "data 0" -> data 0
  * a double quote inside the string can be escaped by writing two consecutive double quotes
  * e.g. "net ""hi""" -> net "hi"
  */

 char *strunquote(const char *src) 
 {
   char* dst;
   size_t len = strlen(src) + 1;
   dst = (char *)malloc(len);
   if (dst != NULL) 
   {
     char* p = (char *)src + 1; /* first char after initial quote */
     char* q = dst;
     do 
       if (*p == '"') p++;
     while ((*q++ = *p++) != '\0');
   }
   return dst;
 }

 /* not truncated */
