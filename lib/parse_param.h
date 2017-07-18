
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

#ifndef PARSE_PARAM_H
#define PARSE_PARAM_H

#include <string>

  /* 
   * Parameters passed on by the parser.
   * All variables added here are initialized in new_record().
   */

  enum pad_type_enum { PAD_TYPE_METAL, PAD_TYPE_ANTIPAD, PAD_TYPE_THERMAL_RELIEF };

  enum pin_function_enum { PIN_SIM_IN, PIN_SIM_OUT, PIN_SIM_BOTH };

  enum polygon_type_enum { POLYGON_TYPE_POUR, POLYGON_TYPE_PLANE, POLYGON_TYPE_COPPER, POLYGON_TYPE_PAD, POLYGON_TYPE_ANTIPAD };

  struct parse_param {
    double vers;   /* version of the hyp file format */
    bool detailed; /* data detailed enough for power integrity */
    bool unit_system_english; /* english or metric units */
    bool metal_thickness_weight; /* copper by weight or by length */
    double default_plane_separation; /* trace to plane separation */
    
    /* stackup record */
    bool use_die_for_metal; /* dielectric constant and loss tangent of dielectric for metal layers */
    double bulk_resistivity;
    bool conformal;
    double epsilon_r;
    std::string layer_name;
    double loss_tangent;
    std::string material_name;
    double plane_separation;
    double plating_thickness;
    bool prepreg;
    double temperature_coefficient; /* temperature coefficient of resistivity */
    double thickness; /* layer thickness */

    /* stackup record flags */
    bool bulk_resistivity_set;
    bool conformal_set;
    bool epsilon_r_set;
    bool layer_name_set;
    bool loss_tangent_set;
    bool material_name_set;
    bool plane_separation_set;
    bool plating_thickness_set;
    bool prepreg_set;
    bool temperature_coefficient_set;
    bool thickness_set;
    
    /* device record */
    std::string device_type;
    std::string ref;
    double value_float;
    std::string value_string;
    std::string package;

    /* device record flags */
    bool name_set;
    bool value_float_set;
    bool value_string_set;
    bool package_set;

    /* supplies record */
    bool voltage_specified;
    bool conversion;

    /* padstack record */
    std::string padstack_name;
    double drill_size;
    double pad_shape;
    double pad_sx;
    double pad_sy;
    double pad_angle;
    double thermal_clear_shape;
    double thermal_clear_sx;
    double thermal_clear_sy;
    double thermal_clear_angle;
    pad_type_enum pad_type;

    /* padstack record flags */
    bool padstack_name_set;
    bool drill_size_set;
    bool pad_type_set;

    /* net record */
    double width;
    double left_plane_separation;
    bool width_set;
    bool left_plane_separation_set;

    /* via subrecord of net */
    std::string layer1_name;
    bool layer1_name_set;
    std::string layer2_name;
    bool layer2_name_set;
    double via_pad_angle;
    bool via_pad_angle_set;
    std::string via_pad_shape;
    bool via_pad_shape_set;
    double via_pad_sx;
    bool via_pad_sx_set;
    double via_pad_sy;
    bool via_pad_sy_set;
    double via_pad1_angle;
    bool via_pad1_angle_set;
    std::string via_pad1_shape;
    bool via_pad1_shape_set;
    double via_pad1_sx;
    bool via_pad1_sx_set;
    double via_pad1_sy;
    bool via_pad1_sy_set;
    double via_pad2_angle;
    bool via_pad2_angle_set;
    std::string via_pad2_shape;
    bool via_pad2_shape_set;
    double via_pad2_sx;
    bool via_pad2_sx_set;
    double via_pad2_sy;
    bool via_pad2_sy_set;

    /* pin subrecord of net */
    std::string pin_reference;
    bool pin_reference_set;
    pin_function_enum pin_function;
    bool pin_function_set;

    /* useg subrecord of net */
    std::string zlayer_name;
    bool zlayer_name_set;
    double length;
    double impedance;
    bool impedance_set;
    double delay;
    double resistance;
    bool resistance_set;

    /* polygon subrecord of net */
    int id;
    bool id_set;
    polygon_type_enum polygon_type;
    bool polygon_type_set;

    /* net class record */
    std::string net_class_name;
    std::string net_name;

    /* key record */
    std::string key;

    /* Attributes */
    std::string name; /* attribute name */
    std::string value; /* attribute value */
    
    /* point, line and arc coordinates */
    double x; /* coordinates point */
    double y; /* coordinates point */
    double x1; /* coordinates point 1 */
    double y1; /* coordinates point 1 */
    double x2; /* coordinates point 2 */
    double y2; /* coordinates point 2 */
    double xc; /* coordinates arc */
    double yc; /* coordinates arc */
    double r; /* coordinates arc */
  };

#endif 
