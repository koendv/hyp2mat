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

/* 
 * Loads a hyperlynx file from disk.
 */

#ifndef HYPFILE_H
#define HYPFILE_H

#include <string>
#include <vector>
#include <map>
#include "parse_param.h"

namespace HypFile {

  class Point {
  public:
    double x;
    double y;
    Point(double x_val = 0, double y_val = 0): x(x_val), y(y_val) {};
    bool operator==(const Point &right) const { return ((x == right.x) && ( y == right.y)); };
  };
    
  typedef std::vector<Point> PointList;
  
  class Polygon {
  public:
    Polygon();
    PointList vertex;
    polygon_type_enum polygon_type;
    std::string net_name;
    int id;
    bool positive; /* false if hole */
    double width; /* edge width of polygon */
    /* if polygon_type == POLYGON_TYPE_PLANE keep plane_separation distance from other nets */
    double plane_separation;  /* -1.0 if not set */
    double left_plane_separation;  /* -1.0 if not set */
    std::string layer_name;
    };
  
  typedef std::vector<Polygon> PolygonList;
  
  struct Via {
    double x;
    double y;
    std::string layer0_name; /* bottom */
    std::string layer1_name; /* top */
    double z0;
    double z1;
    double radius;
    };
  
  typedef std::vector<Via> ViaList;
  
  struct Pin {
    double x;
    double y;
    std::string pin_reference;
    std::string padstack_name;
    pin_function_enum pin_function;
    };
  
  typedef std::vector<Pin> PinList;
  
  struct UnroutedSegment {
    double x1;
    double y1;
    std::string layer1_name;
    double x2;
    double y2;
    std::string layer2_name;
    bool zlayer_name_set;
    std::string zlayer_name;
    double width;
    bool impedance_set;
    double impedance;
    double delay;
    double resistance;
    };
  
  typedef std::vector<UnroutedSegment> UnroutedSegmentList;
  
  enum layer_enum { LAYER_SIGNAL, LAYER_DIELECTRIC, LAYER_PLANE };
  
  struct Layer {
    layer_enum layer_type;    
    double bulk_resistivity;
    bool conformal;
    double epsilon_r;
    std::string layer_name;
    double loss_tangent;
    std::string material_name;
    double plane_separation; /* trace to plane separation */
    double plating_thickness;
    bool prepreg;
    double temperature_coefficient; /* temperature coefficient of resistivity */
    double thickness; /* layer thickness */
    double z0; /* vertical position of bottom of layer */
    double z1; /* vertical position of top of layer */
    };
  
  typedef std::vector<Layer> LayerList;
  
  struct Device {
    std::string device_type;
    std::string ref;
    std::string name;
    double value_float;
    bool value_float_set;
    std::string value_string;
    bool value_string_set;
    std::string layer_name;
    std::string package;
    };
  
  typedef std::vector<Device> DeviceList;
  
  struct Supply {
    std::string name;
    double value_float;
    bool voltage_specified;
    bool conversion;
    };
  
  typedef std::vector<Supply> SupplyList;
  
  enum pad_shape_enum { PAD_SHAPE_OVAL, PAD_SHAPE_RECTANGULAR, PAD_SHAPE_OBLONG };
  
  struct Pad {
    std::string layer_name;
    pad_type_enum pad_type;
    pad_shape_enum pad_shape;
    double pad_sx;
    double pad_sy;
    double pad_angle;
    pad_shape_enum thermal_clear_shape;
    double thermal_clear_sx;
    double thermal_clear_sy;
    double thermal_clear_angle;
    };
  
  typedef std::vector<Pad> PadList;
  
  struct Padstack {
    std::string padstack_name;
    double drill_size;
    PadList pads;
    };
  
  typedef std::vector<Padstack> PadstackList;
  
  struct Attribute {
    std::string name;
    std::string value;
    };
  
  typedef std::vector<Attribute> AttributeList;
  
  typedef std::vector<std::string> StringList;
  
  struct NetClass {
    std::string net_class_name;
    StringList net_class_element;
    AttributeList attribute;
    };

  /* PolygonByID orders polygons according to polygon id. PolygonList is a list of polygons with the same polygon id */

  typedef std::map<int, PolygonList> PolygonMap; 
  
  struct Net {
    std::string net_name;
    PolygonMap metal;
    double plane_separation;
    ViaList via;
    UnroutedSegmentList unrouted_segment;
    PinList pin;
    AttributeList attribute;
    };
  
  typedef std::vector<Net> NetList;
  
  struct Board {
    PolygonList edge;
    PolygonList segments;
    AttributeList attribute;
    double plane_separation;
    };
  
  typedef std::vector<NetClass> NetClassList;
  
    class Hyp
    {
    public:
      Hyp();
      bool trace_scanner;     // flex scanner debugging
      bool trace_parser;      // bison parser debugging
      bool trace_hyp;         // hyperlynx debugging
      bool load (const std::string& filename);         // load Hyperlynx file
      bool save (const std::string& filename); // write to file
      
      void error(const std::string& m);
  
      Board board;
      LayerList stackup; 
      DeviceList device; 
      SupplyList supply; 
      PadstackList padstack;
      NetClassList net_class;
      NetList net;
  
      /* Physical constants */
      double pi;
      double inches;                 /* inches to m */
      double copper_imperial_weight; /* metal thickness in ounces/ft2 */
      double copper_metric_weight;   /* metal thickness in grams/cm2 */
      double copper_bulk_resistivity;
      double copper_temperature_coefficient; /* temperature coefficient of bulk resistivity */
      double fr4_epsilon_r;          /* dielectric constant of substrate */
      double fr4_loss_tangent;       /* loss tangent of substrate */
      double conformal_epsilon_r;    /* dielectric constant of conformal coating */
  
      int min_circle_segments;       /* minimum number of polygon segments to draw a full circle; needs to be a multiple of 4 */
      double arc_precision;          /* maximum difference between perfect circle arc and polygonal approximation */
      StringList flood_layers;       /* layers to be flooded with copper */
  
      /* Hyperlynx UNIT and OPTIONS */
      double unit;                   /* conversion factor: pcb length units to meters */
      double metal_thickness_unit;   /* conversion factor: metal thickness to meters */
      bool use_die_for_metal;        /* use dielectric constant and loss tangent of dielectric for metal layers */
  
      Polygon current_polygon;       /* polygon currently being parsed */
  
      /* exec_* routines are called by parser to interpret hyperlynx file */ 
      bool exec_board_file(parse_param& h);
      bool exec_version(parse_param& h);
      bool exec_data_mode(parse_param& h);
      bool exec_units(parse_param& h);
      bool exec_plane_sep(parse_param& h);
      bool exec_perimeter_segment(parse_param& h);
      bool exec_perimeter_arc(parse_param& h);
      bool exec_board_attribute(parse_param& h);
  
      bool exec_options(parse_param& h);
      bool exec_signal(parse_param& h);
      bool exec_dielectric(parse_param& h);
      bool exec_plane(parse_param& h);
      bool trace_layer(parse_param& h);
      bool add_metal_layer(parse_param& h);
      bool add_dielectric_layer(parse_param& h);
      bool calc_layer_position();
      bool calc_layer_epsilon();
  
      bool exec_devices(parse_param& h);
  
      bool exec_supplies(parse_param& h);
  
      bool exec_padstack_element(parse_param& h);
      bool exec_padstack_end(parse_param& h);
  
      bool exec_net(parse_param& h);
      bool exec_net_plane_separation(parse_param& h);
      bool exec_net_attribute(parse_param& h);
      bool exec_seg(parse_param& h);
      bool exec_arc(parse_param& h);
      bool exec_via(parse_param& h);
      bool exec_via_v1(parse_param& h); /* Old style via format */ 
      bool exec_pin(parse_param& h);
      bool exec_pad(parse_param& h);
      bool exec_useg(parse_param& h);
  
      bool exec_polygon_begin(parse_param& h);
      bool exec_polygon_end(parse_param& h);
      bool exec_polyvoid_begin(parse_param& h);
      bool exec_polyvoid_end(parse_param& h);
      bool exec_polyline_begin(parse_param& h);
      bool exec_polyline_end(parse_param& h);
      bool exec_line(parse_param& h);
      bool exec_curve(parse_param& h);
  
      bool exec_net_class(parse_param& h);
      bool exec_net_class_element(parse_param& h);
      bool exec_net_class_attribute(parse_param& h);
  
      bool exec_end(parse_param& h);
      bool exec_key(parse_param& h);
  
      /* convert a circular arc to a polygon */
      Polygon arc2poly(double x1, double y1, double x2, double y2, double xc, double yc, double r, bool clockwise);
      /* convert a line segment to a polygon */
      Polygon segment2poly(double x1, double y1, double x2, double y2, double width);
      /* convert a pad to a polygon */
      Polygon pad2poly(double pad_x, double pad_y, Pad pad);
      /* Add a polygon to the board outline */
      void add_perimeter_polygon(Polygon poly); 
      /* Add a polygon to the current net */
      void add_polygon(Polygon poly); 
      /* Add a polygon to a current net */
      void add_polygon_to_net(Net& pnet, Polygon poly); 
      /* Add a pad at coordinates x, y  */
      void add_pad(double x, double y, Pad pad); 
      /* Add a via at coordinates x, y  */
      void add_via(double x, double y, std::string layer0_name, std::string layer1_name, double radius); 
      /* Flood specified layers with copper */
      void flood_layers_with_copper();
  
    };

}


#endif

/* not truncated */
