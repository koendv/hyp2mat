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

  /* polygon types */
  
  struct FloatPoint {
    double x;
    double y;
    FloatPoint(double x_val = 0, double y_val = 0): x(x_val), y(y_val) {};
    bool operator==(const FloatPoint &right) const { return ((x == right.x) && ( y == right.y)); };
    bool operator!=(const FloatPoint &right) const { return ((x != right.x) || ( y != right.y)); };
    };
  
  typedef std::vector<FloatPoint> FloatPolygon;
  
  struct FloatPoly {
    FloatPolygon poly;
    bool is_hole;
    int nesting_level; /* hole within polygon within hole within ... */
    };
  
  typedef std::vector<FloatPoly> FloatPolygons;
  
  /*
   * a pin has a reference and an outline 
   */ 

  class Pin {
    public:
      Pin();
      std::string ref; /* reference, e.g. U1.1 */
      double x; /* pad center coordinates */
      double y;
      double z0; /* bottom of copper layer */
      double z1; /* top of copper layer */
      std::string layer_name;
      FloatPolygon metal; /* pad outline */
    };
  
  typedef std::vector<Pin> PinList;

  /*
   * Layers 
   */

  enum layer_enum { LAYER_SIGNAL, LAYER_DIELECTRIC, LAYER_PLANE };
  
  class Layer {
    public:
      std::string layer_name; /* informational only */
      std::string material_name; /* informational only */
      layer_enum layer_type;    
      FloatPolygons metal;
      double thickness; /* layer thickness */
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

  /*
   * a via is a cylindrical, conductive hole. 
   */
  
  class Via {
    public:
      double x;
      double y;
      double z0; /* bottom */
      double z1; /* top */
      double radius;
    };
  
  typedef std::vector<Via> ViaList;

  /*
   * Devices
   */

  enum device_value_enum { DEVICE_VALUE_FLOAT, DEVICE_VALUE_STRING, DEVICE_VALUE_NONE };

  class Device {
    public:
      Device();
      std::string name;
      std::string ref; /* reference, e.g. R1, C2, L3, IC4 */
      device_value_enum value_type; /* whether to use value_float or value_string */
      double value_float; /* in ohms, farad, henry */
      std::string value_string;
      std::string layer_name;
    };
  
  typedef std::vector<Device> DeviceList;

  /* board dimensions */
  class Bounds {
    public:
      Bounds();
      double x_min;
      double x_max;
      double y_min;
      double y_max;
      double z_min;
      double z_max;
    };

  /* 
   * class PCB represents a pcb board.
   */

  class PCB {
    public:
      PCB();
      FloatPolygons board; /* board outline */
      LayerList stackup;
      ViaList via;
      double via_plating_thickness;
      DeviceList device;
      PinList pin;

      /*
       * ReadHyperLynx loads the Hyperlynx file "filename". 
       * "layers" is the names of the layers to import. Default is importing all layers.
       * "nets" is the names of the nets to import. Default is importing all nets.
       */

      void ReadHyperLynx(std::string filename, std::vector<std::string> layers = std::vector<std::string>(), std::vector<std::string> nets = std::vector<std::string>());
      void SetEpsilonR(double epsilon_r); /* set dielectric epsilon r. Overrides value in Hyperlynx file. */
      void SetBulkResistance(double bulk_resistivity); /* set dielectric bulk resistance. Overrides value in Hyperlynx file. */
      void SetLossTangent(double loss_tangent); /* set dielectric loss tangent. Overrides value in Hyperlynx file. */
      void SetGrid(double new_grid); /* set resolution of x and y coordinates */
      void SetArcPrecision(double new_arc_precision); /* set maximum difference between perfect circle arc and polygonal approximation */
      void SetClearance(double new_clearance); /* set trace-to-plane clearance */
      Bounds GetBounds(); /* gets board extension in x,y and z */
      void SetBounds(Bounds new_bounds); /* crops board in x,y and z */

      void PrintSummary(); /* print layer summary */

      /*
       * WritePDF exports to the pdf file "filename".
       * "hue", "saturation" and "brightness" are color options with a value between 0.0 and 1.0.
       */

      void WritePDF(std::string filename, double hue = -1, double saturation = -1, double brightness = -1);

      /* 
       * WriteCSXCAD exports csxcad matlab code to file "filename".
       * If "pcb_outline" is true, the detailed pcb shape is exported, including pcb cutouts. 
       * If "pcb_outline" is false, a simple rectangular bounding box is output."
       */

      void WriteCSXCAD(std::string filename, bool pcb_outline = false, bool lossy_copper = false, bool metal_3d = false);

      /* 
       * WriteQucsStudio exports QucsStudio PCB format to file "filename".
       */

      bool WriteQucsStudio(std::string filename);

      unsigned int debug; /* setting debug to 0 switches debugging off */
      std::vector<std::string> flood_layers; /* names of layers to be flooded with copper */
      bool raw; /* set raw processing */
    private:
      void _UserOverrides();
      void _CheckBoardOutline();
      Bounds _bounds;
      double _arc_precision;
      double _clearance;
      double _epsilon_r;
      bool   _epsilon_r_override;
      double _bulk_resistivity;
      bool   _bulk_resistivity_override;
      double _loss_tangent;
      bool   _loss_tangent_override;
  };

}

#endif

/* not truncated */
