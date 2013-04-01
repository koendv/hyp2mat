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
   Class PCB defines a generic pcb board as a stack of dielectric and copper.
   The class is intended as the greatest common denominator of formats.
   The board outline is defined as a polygon, possibly with polygonal holes.
   For each layer, the copper is defined as a list of polygons, each polygon possibly with polygonal holes.
   Vias provide connections between the different copper layers.
   All dimensions are in meter.

   The following methods are defined:
   ReadHyperLynx reads a hyperlynx file.
   WritePDF prints a PCB to a pdf file.
   WriteCSXCAD converts a PCB to an OpenEMS matlab script.

 */ 
   

#ifndef PCB_H
#define PCB_H

#include <string>
#include <vector>

namespace Hyp2Mat {

  std::string version();

  /* a two-dimensional point */

  class Point {
    public:
      double x;
      double y;
      Point(double x_val = 0, double y_val = 0): x(x_val), y(y_val) {};
      bool operator==(const Point &right) const { return ((x == right.x) && ( y == right.y)); };
      bool operator!=(const Point &right) const { return ((x != right.x) || ( y != right.y)); };
  };
    
  typedef std::vector<Point> PointList;

 /* a polygon edge. Can be an outside edge (border) or an inside edge (hole) */

  class Edge {
    public:
      Edge();
      PointList vertex; /* represents a closed polygon */
      double width; /* edge width of polygon */
      bool is_hole;
      bool is_clockwise();
      void reverse(); /* reverse vertex order */
      int nesting_level; /* set by PCB::Simplify() */
    };
  
  typedef std::vector<Edge> Polygon;

  typedef std::vector<Polygon> PolygonList;

  class Pin {
    public:
      Pin();
      std::string ref; /* reference, e.g. U1.1 */
      double x; /* pad center coordinates */
      double y;
      double z0; /* bottom of copper layer */
      double z1; /* top of copper layer */
      std::string layer_name;
      Edge metal; /* pad outline */
    };
  
  typedef std::vector<Pin> PinList;

  /* a polygon on a signal layer is presence of copper. 
     a plane layer is drawn negative: a polygon on a plane layer is absence of copper.  */

  enum layer_enum { LAYER_SIGNAL, LAYER_DIELECTRIC, LAYER_PLANE };
  
  class Layer {
    public:
      std::string layer_name; /* informational only */
      std::string material_name; /* informational only */
      layer_enum layer_type;    
      PolygonList metal;
      double thickness(); /* layer thickness */
      double z0; /* vertical position of bottom of layer */
      double z1; /* vertical position of top of layer */
      /* dielectric */
      double epsilon_r;
      double loss_tangent;
      /* copper on signal and plane layers */
      double bulk_resistivity;
      double resistivity_temp_coeff; /* temperature coefficient of resistivity */
    };
  
  typedef std::vector<Layer> LayerList;
  
  class Via {
    public:
      double x;
      double y;
      double z0; /* bottom */
      double z1; /* top */
      double radius;
    };
  
  typedef std::vector<Via> ViaList;

  enum device_value_enum { DEVICE_VALUE_FLOAT, DEVICE_VALUE_STRING, DEVICE_VALUE_NONE };

  class Device {
    public:
      Device();
      std::string name;
      std::string ref; /* reference, e.g. R1, C2, L3, IC4 */
      device_value_enum value_type;
      double value_float; /* in ohms, farad, henry */
      std::string value_string;
      std::string layer_name;
    };
  
  typedef std::vector<Device> DeviceList;

  struct Bounds {
    double x_min;
    double x_max;
    double y_min;
    double y_max;
    double z_min;
    double z_max;
    };

  /* 
   * class PCB respresents a pcb board.
   *
   * PCB::Simplify does polygon arithmetic. Simplify() joins overlapping and adjoining copper, 
   * and sets nesting level: 0 for outer edge; 1 for inner edge; 2 for outer edge of embedded hole, ...
   * If desired, the board is also cropped to a 3d region.
   */

  class PCB {
    public:
      PCB();
      PolygonList board; /* board outline */
      LayerList stackup;
      ViaList via;
      double via_plating_thickness;
      DeviceList device;
      PinList pin;

      void ReadHyperLynx(std::string filename, std::vector<std::string> nets = std::vector<std::string>(), double arc_precision = 0);
      void SetEpsilonR(double epsilon_r); /* set dielectric epsilon r. overrides value in Hyperlynx file. */
      Bounds GetBounds(); /* gets board extension in x,y and z */
      void Simplify(double grid); /* join adjacent and overlapping copper */
      void Simplify(double grid, Bounds bounds); /* Simplifies and crops board */
      void PrintSummary();
      void WritePDF(std::string filename, double hue = -1, double saturation = -1, double brightness = -1);
      void WriteCSXCAD(std::string filename);
      unsigned int debug; /* setting debug to 0 switches debugging off */
  };

}

#endif

/* not truncated */
