%error-verbose
%debug
%defines "parse.h"

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

%union {
    int boolval;
    double floatval;
    char* strval;
}

%{
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>

/* error message output */
void yyerror(const char *s);

/* YYPRINT and yyprint print values of the tokens when debugging is switched on */
static void yyprint(FILE*, int, YYSTYPE);
#define YYPRINT(file, type, value) yyprint (file, type, value)

/* functions for matlab output */
void m_func(const char *funcname);                  /* emit function name */
void m_flt(const char *varname);                    /* emit floating point variable */
void m_str(const char *varname);                    /* emit string */
void m_bool(const char *varname);                   /* emit boolean */
void m_val(const char* varname, const char* value); /* emit name/value pair */
void m_end();                                       /* emit end of file */

%}

/*
 * Hyperlynx keywords
 */

 /* Punctuation: { } ( ) = , */

 /* Sections */

%token BOARD_FILE VERSION DATA_MODE UNITS PLANE_SEP
%token BOARD STACKUP DEVICES SUPPLIES
%token PAD PADSTACK NET NET_CLASS END KEY

 /* Keywords */

%token A ARC COPPER CURVE DETAILED DIELECTRIC ENGLISH LENGTH
%token LINE METRIC N OPTIONS PERIMETER_ARC PERIMETER_SEGMENT PIN
%token PLANE POLYGON POLYLINE POLYVOID POUR S SEG SIGNAL
%token SIMPLIFIED SIM_BOTH SIM_IN SIM_OUT USEG VIA WEIGHT

 /* Assignments */

%token A1 A2 BR C C_QM CO_QM D ER F ID
%token L L1 L2 LPS LT M NAME
%token P PKG PR_QM PS R REF SX SY S1 S1X S1Y S2 S2X S2Y T TC
%token USE_DIE_FOR_METAL V V_QM VAL W X X1 X2
%token XC Y Y1 Y2 YC Z ZL ZLEN ZW
 
 /* Booleans */

%token YES NO

%token <boolval> BOOL
%token <floatval> FLOAT
%token <strval> STRING

%start hyp_file

%%

/*
 * Note:
 * Use left recursion when parsing board perimeter and nets. 
 * When using left recursion cpu time is linear with board size. 
 * When using right recursion we run out of memory on large boards. 
 * (Typical error message: line xxx: memory exhausted at 'yyy' )
 */

 /* 
    hyperlynx file sections: 

    board_file
    version 
    data_mode* 
    units 
    plane_sep* 
    board* 
    stackup* 
    devices 
    supplies* 
    padstack* 
    net 
    net_class* 
    end

    * = optional section

  */

hyp_file
  : hyp_file hyp_section
  | hyp_section ;

hyp_section
  : board_file
  | version 
  | data_mode 
  | units
  | plane_sep 
  | board 
  | stackup 
  | devices 
  | supplies 
  | padstack 
  | net 
  | netclass 
  | end 
  | key 
  | '{' error '}' ;

  /* board_file */

board_file
  : '{' BOARD_FILE { m_func("board_file"); } '}' ;

  /* version */

version
  : '{' VERSION { m_func("version"); } '=' FLOAT { m_flt("vers"); } '}' ;

  /* data_mode */

data_mode
  : '{' DATA_MODE { m_func("data_mode"); } '=' mode '}' ;

mode
  : SIMPLIFIED { m_val("mode", "simplified"); }
  | DETAILED   { m_val("mode", "detailed"); } ;

  /* units */

units
  : '{' UNITS { m_func("units"); } '=' unit_system metal_thickness_unit '}' ;

unit_system
  : ENGLISH { m_val("units", "english"); }
  | METRIC  { m_val("units", "metric"); };

metal_thickness_unit
  : WEIGHT  { m_val("metal", "weight"); }
  | LENGTH  { m_val("metal", "length"); } ;

  /* plane_sep */
plane_sep
  : '{' PLANE_SEP { m_func("plane_sep"); } '=' FLOAT { m_flt("ps"); } '}' ;

  /* board */

board
  : '{' BOARD board_paramlist '}' ;
 
board_paramlist
  : board_paramlist board_param
  | board_param ; 

board_param
  : perimeter_segment
  | perimeter_arc
  | attribute 
  | '(' error ')' ;

perimeter_segment
  : '(' PERIMETER_SEGMENT { m_func("perimeter_segment"); } coord_line  ')'

perimeter_arc
  : '(' PERIMETER_ARC { m_func("perimeter_arc"); } coord_arc ')'

  /* stackup */

stackup
  : '{' STACKUP { m_func("stackup"); } stackup_paramlist '}' ;

stackup_paramlist
  : stackup_paramlist stackup_param
  | stackup_param ;

stackup_param
  : options
  | signal
  | dielectric
  | plane
  | '(' error ')' ;

options
  : '(' OPTIONS { m_func("options"); } options_params ;

options_params
  : USE_DIE_FOR_METAL '=' BOOL { m_bool("use_die_for_metal"); } ')'
  | ')'
  ;

signal
  : '(' SIGNAL { m_func("signal"); } signal_paramlist ')' ;

signal_paramlist
  : signal_paramlist signal_param
  | signal_param ;

signal_param
  : T '=' FLOAT  { m_flt("t"); }
  | P '=' FLOAT  { m_flt("p"); }
  | C '=' FLOAT  { m_flt("c"); }
  | BR '=' FLOAT { m_flt("br"); }
  | TC '=' FLOAT { m_flt("tc"); }
  | ER '=' FLOAT { m_flt("er"); }
  | LT '=' FLOAT { m_flt("lt"); }
  | L '=' STRING { m_str("l"); }
  | M '=' STRING { m_str("m"); }
  | PS '=' FLOAT { m_flt("ps"); }
  ;

dielectric
  : '(' DIELECTRIC { m_func("dielectric"); } dielectric_paramlist ')' ;

dielectric_paramlist
  : dielectric_paramlist dielectric_param
  | dielectric_param ;

dielectric_param
  : T '=' FLOAT    { m_flt("t"); }
  | C '=' FLOAT    { m_flt("c"); }
  | ER '=' FLOAT   { m_flt("er"); }
  | LT '=' FLOAT   { m_flt("lt"); }
  | CO_QM '=' BOOL { m_bool("co"); }
  | PR_QM '=' BOOL { m_bool("pr"); }
  | L '=' STRING   { m_str("l"); }
  | M '=' STRING   { m_str("m"); }
  ;

plane
  : '(' PLANE { m_func("plane"); } plane_paramlist ')' ;

plane_paramlist
  : plane_paramlist plane_param
  | plane_param ;

plane_param
  : T '=' FLOAT  { m_flt("t"); }
  | BR '=' FLOAT { m_flt("br"); }
  | TC '=' FLOAT { m_flt("tc"); }
  | C '=' FLOAT  { m_flt("c"); }
  | ER '=' FLOAT { m_flt("er"); }
  | LT '=' FLOAT { m_flt("lt"); }
  | L '=' STRING { m_str("l"); }
  | M '=' STRING { m_str("m"); }
  | PS '=' FLOAT { m_flt("ps"); }
  ;

  /* devices */

devices
  : '{' DEVICES device_list '}'
  | '{' DEVICES '}' ;

device_list
  : device_list device 
  | device ;

device
  : '(' { m_func("device"); } STRING { m_str("type"); } device_paramlist ')'
  | '(' error ')' ;

device_paramlist
  : device_paramlist device_param
  | device_param ;
 
device_param
  : REF '=' STRING  { m_str("ref"); }
  | NAME '=' STRING { m_str("name"); }
  | VAL '=' FLOAT   { m_flt("val"); }
  | VAL '=' STRING  { m_str("val"); }
  | L '=' STRING    { m_str("l"); }
  | PKG '=' STRING  { m_str("pkg"); }
  ;

  /* supplies */

supplies
  : '{' SUPPLIES supply_list '}' ;

supply_list
  : supply_list supply
  | supply ;

supply
  : '(' S          { m_func("supply"); } 
    NAME '=' STRING { m_str("name"); } 
    VAL '=' FLOAT   { m_flt("val"); } 
    V_QM '=' BOOL   { m_bool("v"); }
    C_QM '=' BOOL   { m_bool("c"); }
    ')' 
  | '(' error ')' ;

  /* padstack */

padstack
  : '{' PADSTACK { m_func("padstack"); } '=' STRING { m_str("padstack_name"); } drill_size '}' ;

drill_size 
  : ',' FLOAT { m_flt("drill_size"); } padstack_list
  | ',' padstack_list ;
  | padstack_list ;

padstack_list
  : padstack_list padstack_def 
  | padstack_def ;

padstack_def
  : '(' { m_func("padstack_element"); } STRING { m_str("layer_name"); } ',' pad_shape ')'
  | '(' error ')' ;

pad_shape
  : FLOAT { m_flt("pad_shape"); } ',' pad_coord
  | ',' { yylval.floatval = -1; m_flt("pad_shape"); } pad_coord /* Workaround: Altium sometimes prints an empty pad shape */
  ;

pad_coord
  : FLOAT { m_flt("pad_sx"); } ',' FLOAT { m_flt("pad_sy"); } ',' FLOAT { m_flt("pad_angle"); } pad_type 

pad_type
  : /* empty */ 
  | ',' M     { m_val("pad_type", "m"); } 
  | ',' A     { m_val("pad_type", "a"); } 
  | ',' FLOAT { m_flt("thermal_clear_shape"); } 
    ',' FLOAT { m_flt("thermal_clear_sx"); } 
    ',' FLOAT { m_flt("thermal_clear_sy"); } 
    ',' FLOAT { m_flt("thermal_clear_angle"); } 
    ',' T     { m_val("pad_type", "t"); } 
  ;

  /* net */

net
  : '{' NET { m_func("net"); } '=' STRING { m_str("name"); } net_def '}' ;

net_def
  : PS '=' FLOAT { m_flt("ps"); } net_subrecord_list 
  | net_subrecord_list ;

net_subrecord_list
  : net_subrecord_list net_subrecord
  | net_subrecord ;

net_subrecord
  : seg 
  | arc
  | via 
  | pin 
  | pad 
  | useg 
  | polygon 
  | polyvoid 
  | polyline 
  | attribute 
  | '(' error ')'
  | '{' error '}'
  ;

seg
  : '(' SEG { m_func("seg"); } seg_param_list ')' ;

seg_param_list
  : seg_param_list seg_param
  | seg_param ;

seg_param
  : coord_line 
  | W '=' FLOAT    { m_flt("w"); }
  | L '=' STRING   { m_str("l"); }
  | PS '=' FLOAT   { m_flt("ps"); }
  | LPS '=' FLOAT  { m_flt("lps"); }
  ;

arc
  : '(' ARC { m_func("arc"); } arc_param_list ')' ;

arc_param_list
  : arc_param_list arc_param
  | arc_param ;

arc_param
  : coord_arc 
  | W '=' FLOAT    { m_flt("w"); }
  | L '=' STRING   { m_str("l"); }
  | PS '=' FLOAT   { m_flt("ps"); }
  | LPS '=' FLOAT  { m_flt("lps"); }
  ;

via
  : '(' VIA { m_func("via"); } via_param_list ')' ; 

via_param_list
  : via_param_list via_param
  | via_param ;

via_param
  : coord_point 
  | P '=' STRING  { m_str("p"); } /* hyperlynx v2.x VIA format */
  | D '=' FLOAT   { m_flt("d"); } /* deprecated hyperlynx v1.x VIA format */
  | L1 '=' STRING { m_str("l1"); } 
  | L2 '=' STRING { m_str("l2"); } 
  | S1 '=' STRING { m_str("s1"); } 
  | S1X '=' FLOAT { m_flt("s1x"); }
  | S1Y '=' FLOAT { m_flt("s1y"); }
  | A1 '=' FLOAT  { m_flt("a1"); }
  | S2 '=' STRING { m_str("s2"); } 
  | S2X '=' FLOAT { m_flt("s2x"); }
  | S2Y '=' FLOAT { m_flt("s2y"); }
  | A2 '=' FLOAT  { m_flt("a2"); }
  ;

pin
  : '(' PIN { m_func("pin"); } pin_param_list ')' ;

pin_param_list
  : pin_param_list pin_param
  | pin_param ;

pin_param
  : coord_point
  | R '=' STRING  { m_str("r"); }
  | P '=' STRING  { m_str("p"); }
  | F '=' pin_function 
  ;

pin_function
  : SIM_OUT  { m_val("f", "sim_out"); }
  | SIM_IN   { m_val("f", "sim_in"); }
  | SIM_BOTH { m_val("f", "sim_both"); }
  ;

pad
  : '(' PAD { m_func("pad"); } pad_param_list ')' ; /* deprecated hyperlynx v1.x only */

pad_param_list
  : pad_param_list pad_param
  | pad_param ;

pad_param
  : coord_point
  | L '=' STRING  { m_str("l"); }
  | S '=' STRING  { m_str("s"); }
  | SX '=' FLOAT  { m_flt("sx"); }
  | SY '=' FLOAT  { m_flt("sy"); }
  | A '=' FLOAT   { m_flt("a"); }
  ;

useg
  : '(' USEG { m_func("useg"); } useg_param_list ')' ; 

useg_param_list
  : useg_param_list useg_param
  | useg_param ;

useg_param
  : coord_point1
  | L1 '=' STRING  { m_str("l1"); }
  | coord_point2 
  | L2 '=' STRING  { m_str("l2"); } 
  | ZL '=' STRING  { m_str("zl"); } 
  | ZW '=' FLOAT   { m_flt("zw"); }
  | ZLEN '=' FLOAT { m_flt("zlen"); }
  | Z '=' FLOAT    { m_flt("z"); }
  | D '=' FLOAT    { m_flt("d"); }
  | R '=' FLOAT    { m_flt("r"); }
  ;

polygon
  : '{' POLYGON { m_func("polygon"); } polygon_param_list '}' ;

polygon_param_list
  : polygon_param_list polygon_param
  | polygon_param ;

polygon_param
  : L '=' STRING  { m_str("l"); }
  | poly_type 
  | W '=' FLOAT   { m_flt("w"); }
  | ID '=' STRING { m_str("id"); }
  | coord_point 
  | line 
  | curve 
  | '(' error ')'
  ;

poly_type
  : T '=' POUR    { m_val("t", "pour"); }
  | T '=' PLANE   { m_val("t", "plane"); }
  | T '=' COPPER  { m_val("t", "copper"); }
  ;

polyvoid
  : '{' POLYVOID { m_func("polyvoid"); } polyvoid_param_list '}' ;

polyvoid_param_list
  : polyvoid_param_list polyvoid_param
  | polyvoid_param ;

polyvoid_param
  : ID '=' STRING  { m_str("id"); }
  | coord_point 
  | line 
  | curve 
  ;

polyline
  : '{' POLYLINE { m_func("polyline"); } polygon_param_list '}' ;

line
  : '(' LINE { m_func("line"); } coord_point ')' ;

curve
  : '(' CURVE { m_func("curve"); } coord_arc ')' ;

  /* net class */

netclass
  : '{' NET_CLASS { m_func("net_class"); } '=' STRING { m_str("class_name"); } netclass_subrecords ;

netclass_subrecords
  : netclass_paramlist '}'
  | '}' 
  ;

netclass_paramlist
  : netclass_paramlist netclass_param
  | netclass_param 
  ;

netclass_param
  : attribute 
  | net_name 
  | '(' error ')'
  ;

net_name
  : '(' N { m_func("net_class_element"); } N '=' STRING { m_str("net_name"); } ')' ;

  /* end */

end
  : '{' END { m_func("end"); } '}' { m_end(); } ;

  /* key */

key
  : '{' KEY { m_func("key"); } '=' STRING { m_str("value"); } '}' { m_end(); };

  /* coordinates */

coord_point
  : X '=' FLOAT { m_flt("x"); } Y '=' FLOAT { m_flt("y"); } ;

coord_point1
  : X1 '=' FLOAT { m_flt("x1"); } Y1 '=' FLOAT { m_flt("y1"); } ;

coord_point2
  : X2 '=' FLOAT { m_flt("x2"); } Y2 '=' FLOAT { m_flt("y2"); } ;

coord_line
  : coord_point1 coord_point2 ;

coord_arc
  : coord_line XC '=' FLOAT { m_flt("xc"); } YC '=' FLOAT { m_flt("yc"); } R '=' FLOAT { m_flt("r"); } ;

  /* attribute */

attribute
  : '(' A { m_func("attribute"); } N '=' STRING { m_str("n"); } V '=' STRING { m_str("v"); } ')';

%%

/*
 * Supporting C routines
 */

/* print values of the tokens when debugging is switched on. See YYPRINT at beginning of file. */
static void
yyprint (file, type, value)
  FILE *file;
  int type;
  YYSTYPE value;
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
 * Matlab output routines
 */

extern FILE* out;
extern int yylineno;
int in_func = 0;

/* emit function name */

void m_func(const char *funcname)
{
  if (in_func) fprintf(out, "));\n");
  in_func = 1;
  fprintf(out, "CSX = hyp_%s(CSX, struct('lineno', %i", funcname, yylineno);
}

/* emit floating point variable */

void m_flt(const char *varname)
{
  fprintf(out, ", '%s', %.8g", varname, yylval.floatval);
}

/* emit string */

void m_str(const char *varname)
{
  fprintf(out, ", '%s', '", varname);
  char *ch;
  for (ch = yylval.strval; *ch != 0; ++ch) {
    if (*ch == '\'') fprintf(out, "'");
    fprintf(out, "%c", *ch);
    }
  fprintf(out, "'");
}

/* emit boolean */

void m_bool(const char *varname)
{
  fprintf(out, ", '%s', %s", varname, yylval.boolval ? "true" : "false" );
}

/* emit name, value pair */

void m_val(const char *varname, const char* value)
{
  fprintf(out, ", '%s', '%s'", varname, value);
}

/* emit end of file */

void m_end()
{
  if (in_func) fprintf(out, "));\n");
  in_func = 0;
}

/* not truncated */
