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

%code requires {
#include "hypfile.h"
}

%parse-param {class HypFile::Hyp *hyp }
%error-verbose
%debug
%defines

%union {
    int boolval;
    int intval;
    double floatval;
    char* strval;
}

%{
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
%token <intval> POSINT
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
  | error '}' ;

  /* board_file */

board_file
  : '{' BOARD_FILE { if (hyp->exec_board_file(h)) YYERROR; } '}' ;

  /* version */

version
  : '{' VERSION '=' FLOAT { h.vers = yylval.floatval; } '}' { if (hyp->exec_version(h)) YYERROR; } ;

  /* data_mode */

data_mode
  : '{' DATA_MODE '=' mode '}' { if (hyp->exec_data_mode(h)) YYERROR; };

mode
  : SIMPLIFIED { h.detailed = false; }
  | DETAILED   { h.detailed = true; } ;

  /* units */

units
  : '{' UNITS '=' unit_system metal_thickness_unit '}' { if (hyp->exec_units(h)) YYERROR; } ;

unit_system
  : ENGLISH { h.unit_system_english = true; }
  | METRIC  { h.unit_system_english = false; };

metal_thickness_unit
  : WEIGHT  { h.metal_thickness_weight = true; }
  | LENGTH  { h.metal_thickness_weight = false; } ;

  /* plane_sep */
plane_sep
  : '{' PLANE_SEP '=' FLOAT { h.default_plane_separation = yylval.floatval; } '}' { if (hyp->exec_plane_sep(h)) YYERROR; } ;

  /* board */

board
  : '{' BOARD board_param_list '}'
  | '{' BOARD '}' ;
 
board_param_list
  : board_param_list board_param_list_item
  | board_param_list_item ;

board_param_list_item
  : '(' board_param ')'
  | '(' board_param { yyerror(hyp, "warning: missing ')'"); }
  | error ')' ;
  ;

board_param
  : perimeter_segment
  | perimeter_arc
  | board_attribute ;

perimeter_segment
  : PERIMETER_SEGMENT coord_line { if (hyp->exec_perimeter_segment(h)) YYERROR; } ;

perimeter_arc
  : PERIMETER_ARC coord_arc { if (hyp->exec_perimeter_arc(h)) YYERROR; } ;

board_attribute
  : A N '=' STRING { h.name = yylval.strval; } V '=' STRING { h.value = yylval.strval; } { if (hyp->exec_board_attribute(h)) YYERROR; } ;

  /* stackup */

stackup
  : '{' STACKUP stackup_paramlist '}' ;

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
  : '(' OPTIONS options_params { if (hyp->exec_options(h)) YYERROR; } ;

options_params
  : USE_DIE_FOR_METAL '=' BOOL { h.use_die_for_metal = yylval.boolval; } ')'
  | ')'
  ;

signal
  : '(' SIGNAL { new_record(); } signal_paramlist ')' { if (hyp->exec_signal(h)) YYERROR; } ;

signal_paramlist
  : signal_paramlist signal_param
  | signal_param ;

signal_param
  : thickness
  | plating_thickness
  | C  '=' FLOAT { h.bulk_resistivity = yylval.floatval; h.bulk_resistivity_set = true; }
  | bulk_resistivity
  | temperature_coefficient
  | epsilon_r
  | loss_tangent
  | layer_name
  | material_name
  | plane_separation ;

dielectric
  : '(' DIELECTRIC { new_record(); } dielectric_paramlist ')' { if (hyp->exec_dielectric(h)) YYERROR; } ;

dielectric_paramlist
  : dielectric_paramlist dielectric_param
  | dielectric_param ;

dielectric_param
  : thickness
  | C '=' FLOAT { h.epsilon_r = yylval.floatval; h.epsilon_r_set = true; }
  | epsilon_r
  | loss_tangent
  | conformal
  | prepreg
  | layer_name
  | material_name
  ;

plane
  : '(' PLANE { new_record(); } plane_paramlist ')' { if (hyp->exec_plane(h)) YYERROR; } ;

plane_paramlist
  : plane_paramlist plane_param
  | plane_param ;

plane_param
  : thickness
  | C  '=' FLOAT { h.bulk_resistivity = yylval.floatval; h.bulk_resistivity_set = true; }
  | bulk_resistivity
  | temperature_coefficient
  | epsilon_r
  | loss_tangent
  | layer_name
  | material_name
  | plane_separation ;

thickness
  : T  '=' FLOAT  { h.thickness = yylval.floatval; h.thickness_set = true; }

plating_thickness
  : P  '=' FLOAT  { h.plating_thickness = yylval.floatval; h.plating_thickness_set = true; }

bulk_resistivity
  : BR '=' FLOAT  { h.bulk_resistivity = yylval.floatval; h.bulk_resistivity_set = true; }

temperature_coefficient
  : TC '=' FLOAT  { h.temperature_coefficient = yylval.floatval; h.temperature_coefficient_set = true; }

epsilon_r
  : ER '=' FLOAT  { h.epsilon_r = yylval.floatval; h.epsilon_r_set = true; }

loss_tangent
  : LT '=' FLOAT  { h.loss_tangent = yylval.floatval; h.loss_tangent_set = true; }

layer_name
  : L '=' STRING { h.layer_name = yylval.strval; h.layer_name_set = true; }

material_name
  : M  '=' STRING { h.material_name = yylval.strval; h.material_name_set = true; }

plane_separation
  : PS '=' FLOAT  { h.plane_separation = yylval.floatval; h.plane_separation_set = true; }

conformal
  : CO_QM '=' BOOL { h.conformal = yylval.boolval; h.conformal_set = true; }

prepreg
  : PR_QM '=' BOOL { h.prepreg = yylval.boolval; h.prepreg_set = true; }

  /* devices */

devices
  : '{' DEVICES device_list '}'
  | '{' DEVICES '}' ;

device_list
  : device_list device 
  | device ;

device
  : '(' { new_record(); } STRING { h.device_type = yylval.strval; } REF '=' STRING  { h.ref = yylval.strval; } device_paramlist ')' { if (hyp->exec_devices(h)) YYERROR; } 
  | '(' error ')' ;

device_paramlist
  : name device_value
  | device_value 
  ;
 
device_value
  : value device_layer
  | device_layer 
  ;

device_layer
  : layer_name package 
  | layer_name
  ;

name
  : NAME '=' STRING { h.name = yylval.strval; h.name_set = true; } ;

value
  : value_float
  | value_string
  ;

value_float
  : VAL '=' FLOAT   { h.value_float = yylval.floatval; h.value_float_set = true; } ;

value_string
  : VAL '=' STRING  { h.value_string = yylval.strval; h.value_string_set = true; } ;

package
  : PKG '=' STRING  { h.package = yylval.strval; h.package_set = true; } ;

  /* supplies */

supplies
  : '{' SUPPLIES supply_list '}' ;

supply_list
  : supply_list supply
  | supply ;

supply
  : '(' S name value_float voltage_spec conversion ')'  { if (hyp->exec_supplies(h)) YYERROR; } 
  | '(' error ')' ;

voltage_spec
  : V_QM '=' BOOL { h.voltage_specified = yylval.boolval; } ;

conversion
  : C_QM '=' BOOL { h.conversion = yylval.boolval; }

  /* padstack */

padstack
  : '{' PADSTACK { new_record(); } '=' STRING { h.padstack_name = yylval.strval; h.padstack_name_set = true; } drill_size '}' { if (hyp->exec_padstack_end(h)) YYERROR; } ;

drill_size 
  : ',' FLOAT { h.drill_size = yylval.floatval; h.drill_size_set = true; } padstack_list
  | ',' padstack_list ;
  | padstack_list ;

padstack_list
  : padstack_list padstack_def 
  | padstack_def ;

padstack_def
  : '(' STRING { h.layer_name = yylval.strval; h.layer_name_set = true; } ',' pad_shape pad_coord pad_type { if (hyp->exec_padstack_element(h)) YYERROR; new_record(); }
  | '(' error ')' ;

pad_shape
  : FLOAT { h.pad_shape = yylval.floatval; } ','
  | ',' { h.pad_shape = -1; } /* Workaround: Altium sometimes prints an empty pad shape */
  ;

pad_coord
  : FLOAT { h.pad_sx = yylval.floatval; } ',' FLOAT { h.pad_sy = yylval.floatval; } ',' FLOAT { h.pad_angle = yylval.floatval; } 

pad_type
  : ')' 
  | ',' M ')' { h.pad_type = PAD_TYPE_METAL; h.pad_type_set = true; } 
  | ',' A ')' { h.pad_type = PAD_TYPE_ANTIPAD; h.pad_type_set = true; } 
  | ',' FLOAT { h.thermal_clear_shape = yylval.floatval; } 
    ',' FLOAT { h.thermal_clear_sx = yylval.floatval; } 
    ',' FLOAT { h.thermal_clear_sy = yylval.floatval; } 
    ',' FLOAT { h.thermal_clear_angle = yylval.floatval; } 
    ',' T ')' { h.pad_type = PAD_TYPE_THERMAL_RELIEF; h.pad_type_set = true; } 
  ;

  /* net */

net
  : '{' NET '=' STRING { h.net_name = yylval.strval; if (hyp->exec_net(h)) YYERROR; } net_separation ;

net_separation
  : plane_separation { if (hyp->exec_net_plane_separation(h)) YYERROR; } net_copper
  | net_copper
  ;

net_copper
  : net_subrecord_list '}'
  | { yyerror(hyp, "warning: empty net"); } '}'
  ;

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
  | net_attribute 
  | '(' error ')'
  | '{' error '}'
  ;

seg
  : '(' SEG { new_record(); } coord_line width layer_name ps_lps_param { if (hyp->exec_seg(h)) YYERROR; } ;

arc
  : '(' ARC { new_record(); } coord_arc width layer_name ps_lps_param { if (hyp->exec_arc(h)) YYERROR; } ;

ps_lps_param
  : plane_separation lps_param
  | lps_param
  ;

lps_param
  : left_plane_separation ')'
  | ')'
  ;

width
  : W '=' FLOAT    { h.width = yylval.floatval; h.width_set = true; } ;
  
left_plane_separation
  : LPS '=' FLOAT { h.left_plane_separation = yylval.floatval; h.left_plane_separation_set = true; } ;

via
  : '(' VIA { new_record(); } coord_point via_param_list ')' { if (hyp->exec_via(h)) YYERROR; } ; 
  ;

via_param_list
  : via_param_list via_param
  | via_param
  ;

via_param
  : padstack_name
  /* parameters below are for deprecated v1.0 via format */
  | D '=' FLOAT   { h.drill_size = yylval.floatval; h.drill_size_set = true; } 
  | layer1_name
  | layer2_name
  | S '=' STRING  { h.via_pad_shape = yylval.strval; h.via_pad_shape_set = true; }
  | SX '=' FLOAT  { h.via_pad_sx = yylval.floatval; h.via_pad_sx_set = true; }
  | SY '=' FLOAT  { h.via_pad_sy = yylval.floatval; h.via_pad_sy_set = true; }
  | A '=' FLOAT   { h.via_pad_angle = yylval.floatval; h.via_pad_angle_set = true; }
  | S1 '=' STRING { h.via_pad1_shape = yylval.strval; h.via_pad1_shape_set = true; } 
  | S1X '=' FLOAT { h.via_pad1_sx = yylval.floatval; h.via_pad1_sx_set = true; }
  | S1Y '=' FLOAT { h.via_pad1_sy = yylval.floatval; h.via_pad1_sy_set = true; }
  | A1 '=' FLOAT  { h.via_pad1_angle = yylval.floatval; h.via_pad1_angle_set = true; }
  | S2 '=' STRING { h.via_pad2_shape = yylval.strval; h.via_pad2_shape_set = true; } 
  | S2X '=' FLOAT { h.via_pad2_sx = yylval.floatval; h.via_pad2_sx_set = true; }
  | S2Y '=' FLOAT { h.via_pad2_sy = yylval.floatval; h.via_pad2_sy_set = true; }
  | A2 '=' FLOAT  { h.via_pad2_angle  = yylval.floatval; h.via_pad2_angle_set = true; }
  ;

padstack_name
  : P '=' STRING  { h.padstack_name = yylval.strval; h.padstack_name_set = true; } ;

layer1_name
  : L1 '=' STRING { h.layer1_name = yylval.strval; h.layer1_name_set = true; } ;

layer2_name
  : L2 '=' STRING { h.layer2_name = yylval.strval; h.layer2_name_set = true; } ;

pin
  : '(' PIN { new_record(); } coord_point pin_reference pin_param { if (hyp->exec_pin(h)) YYERROR; } ;

pin_param
  : padstack_name pin_function_param
  | pin_function_param
  ;

pin_function_param
  : pin_function ')'
  | ')'
  ;

pin_reference
  : R '=' STRING  { h.pin_reference = yylval.strval; h.pin_reference_set = true; } ;

pin_function
  : F '=' SIM_OUT  { h.pin_function = PIN_SIM_OUT; h.pin_function_set = true; }
  | F '=' SIM_IN   { h.pin_function = PIN_SIM_IN; h.pin_function_set = true; }
  | F '=' SIM_BOTH { h.pin_function = PIN_SIM_BOTH; h.pin_function_set = true; }
  | F '=' STRING   { h.pin_function = PIN_SIM_BOTH; h.pin_function_set = true; yyerror(hyp, "warning: SIM_BOTH assumed"); }
  ;

pad /* deprecated hyperlynx v1.x only */
  : '(' PAD { new_record(); } coord_point pad_param_list ')' { if (hyp->exec_pad(h)) YYERROR; } ;

pad_param_list
  : pad_param_list pad_param
  | pad_param
  ;

pad_param
  : layer_name
  | S '=' STRING  { h.via_pad_shape = yylval.strval; h.via_pad_shape_set = true; }
  | SX '=' FLOAT  { h.via_pad_sx = yylval.floatval; h.via_pad_sx_set = true; }
  | SY '=' FLOAT  { h.via_pad_sy = yylval.floatval; h.via_pad_sy_set = true; }
  | A '=' FLOAT   { h.via_pad_angle = yylval.floatval; h.via_pad_angle_set = true; }
  ;

useg
  : '(' USEG { new_record(); } coord_point1 layer1_name coord_point2 layer2_name useg_param { if (hyp->exec_useg(h)) YYERROR; } ; 

useg_param
  : useg_stackup
  | useg_impedance
  ;

useg_stackup
  : ZL '=' STRING  { h.zlayer_name = yylval.strval; h.zlayer_name_set = true; } 
    ZW '=' FLOAT   { h.width = yylval.floatval; }
    ZLEN '=' FLOAT { h.length = yylval.floatval; }
    ')'
  ;

useg_impedance
  : Z '=' FLOAT    { h.impedance = yylval.floatval; h.impedance_set = true; }
    D '=' FLOAT    { h.delay = yylval.floatval; }
    useg_resistance;

useg_resistance 
  : R '=' FLOAT    { h.resistance = yylval.floatval; h.resistance_set = true;}
    ')'
  | ')'
  ;

polygon
  : '{' POLYGON { new_record(); } polygon_param_list coord_point { if (hyp->exec_polygon_begin(h)) YYERROR; }
    lines_and_curves '}' { if (hyp->exec_polygon_end(h)) YYERROR; } ;

polygon_param_list
  : polygon_param_list polygon_param
  | polygon_param
  ;

polygon_param
  : layer_name
  | width
  | polygon_type
  | polygon_id
  ;

polygon_id
  : ID '=' POSINT { h.id = yylval.intval; h.id_set = true; } /* polygon id is a non-negative integer */
  ;

polygon_type
  : T '=' POUR    { h.polygon_type = POLYGON_TYPE_POUR; h.polygon_type_set = true; }
  | T '=' PLANE   { h.polygon_type = POLYGON_TYPE_PLANE; h.polygon_type_set = true; }
  | T '=' COPPER  { h.polygon_type = POLYGON_TYPE_COPPER; h.polygon_type_set = true; }
  ;

polyvoid
  : '{' POLYVOID { new_record(); } polygon_id coord_point { if (hyp->exec_polyvoid_begin(h)) YYERROR; }
    lines_and_curves '}' { if (hyp->exec_polyvoid_end(h)) YYERROR; } ;

polyline
  : '{' POLYLINE { new_record(); } polygon_param_list coord_point { if (hyp->exec_polyline_begin(h)) YYERROR; }
    lines_and_curves '}' { if (hyp->exec_polyline_end(h)) YYERROR; } ;

lines_and_curves
  : lines_and_curves line_or_curve
  | line_or_curve
  ;

line_or_curve
  : line
  | curve
  | ')' { yyerror(hyp, "warning: unexpected ')'"); }
  | '(' error ')'
  ;

line
  : '(' LINE { new_record(); } coord_point ')' { if (hyp->exec_line(h)) YYERROR; } ;

curve
  : '(' CURVE { new_record(); } coord_arc ')' { if (hyp->exec_curve(h)) YYERROR; } ;

net_attribute
  : '(' A N '=' STRING { h.name = yylval.strval; } V '=' STRING { h.value = yylval.strval; } ')' { if (hyp->exec_net_attribute(h)) YYERROR; } ;

  /* net class */

netclass
  : '{' NET_CLASS '=' STRING { h.net_class_name = yylval.strval; if (hyp->exec_net_class(h)) YYERROR; } netclass_subrecords ;

netclass_subrecords
  : netclass_paramlist '}'
  | '}' 
  ;

netclass_paramlist
  : netclass_paramlist netclass_param
  | netclass_param 
  ;

netclass_param
  : netclass_attribute 
  | net_name 
  | '(' error ')'
  ;

net_name
  : '(' N N '=' STRING { h.net_name = yylval.strval; } ')' { if (hyp->exec_net_class_element(h)) YYERROR; } ;

netclass_attribute
  : '(' A N '=' STRING { h.name = yylval.strval; } V '=' STRING { h.value = yylval.strval; } ')' { if (hyp->exec_net_class_attribute(h)) YYERROR; } ;

  /* end */

end
  : '{' END '}' { if (hyp->exec_end(h)) YYERROR; } ;

  /* key */

key
  : '{' KEY '=' STRING { h.key = yylval.strval; } '}' { if (hyp->exec_key(h)) YYERROR; } ;

  /* coordinates */

coord_point
  : X '=' FLOAT { h.x = yylval.floatval; } Y '=' FLOAT { h.y = yylval.floatval; } ;

coord_point1
  : X1 '=' FLOAT { h.x1 = yylval.floatval; } Y1 '=' FLOAT { h.y1 = yylval.floatval; } ;

coord_point2
  : X2 '=' FLOAT { h.x2 = yylval.floatval; } Y2 '=' FLOAT { h.y2 = yylval.floatval; } ;

coord_line
  : coord_point1 coord_point2 ;

coord_arc
  : coord_line XC '=' FLOAT { h.xc = yylval.floatval; } YC '=' FLOAT { h.yc = yylval.floatval; } R '=' FLOAT { h.r = yylval.floatval; } ;

%%

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
  h.via_pad_angle  = 0;
  h.via_pad_angle_set = false;
  h.via_pad_shape.clear();
  h.via_pad_shape_set = false;
  h.via_pad_sx  = 0;
  h.via_pad_sx_set = false;
  h.via_pad_sy  = 0;
  h.via_pad_sy_set = false;
  h.via_pad1_angle  = 0;
  h.via_pad1_angle_set = false;
  h.via_pad1_shape.clear();
  h.via_pad1_shape_set = false;
  h.via_pad1_sx  = 0;
  h.via_pad1_sx_set = false;
  h.via_pad1_sy  = 0;
  h.via_pad1_sy_set = false;
  h.via_pad2_angle  = 0;
  h.via_pad2_angle_set = false;
  h.via_pad2_shape.clear();
  h.via_pad2_shape_set = false;
  h.via_pad2_sx  = 0;
  h.via_pad2_sx_set = false;
  h.via_pad2_sy  = 0;
  h.via_pad2_sy_set = false;

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
