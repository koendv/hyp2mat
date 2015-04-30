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

#include <algorithm>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cmath>

#include "csxcad.h"

using namespace Hyp2Mat;

CSXCAD::CSXCAD()
{
  /* CSXcad priorities */

  prio_dielectric = 100;  // FR4 dielectric
  prio_material   = 200;  // copper
  prio_via        = 300;  // via metal
  prio_drill      = 400;  // hole

  return;
}

void CSXCAD::export_edge(FloatPolygon& edge)
{
  std::cout << "pgon = [];" << std::endl;
  for (FloatPolygon::iterator i = edge.begin(); i != edge.end(); ++i) {
    std::cout << "pgon(:, end+1) = [" << i->x << ";" << i->y << "];" << std::endl;
    }
  return;
}

/*
 * quote a string using matlab conventions.
 */

std::string CSXCAD::string2matlab(std::string str)
{
   std::ostringstream ostring;

  // escape non-alpha characters, or WriteOpenEMS (in matlab) may crash on characters such as '%' in strings

   ostring << "'";
   for (unsigned int i = 0; i < str.size(); i++) {
     if (str[i] == '\'') ostring << '\'';
     if (str[i] == '%') ostring << '%';
     ostring << str[i];
     }
   ostring << "'";

   return ostring.str();
}

/* true if polygon list contains at least one (positive) polygon */

bool CSXCAD::contains_polygon(Hyp2Mat::FloatPolygons& polygons)
{
  bool result = false;

  for (FloatPolygons::iterator i = polygons.begin(); i != polygons.end(); ++i) {
    result = !i->is_hole;
    if (result) break;
    };

  return result;
}

/* true if polygon list contains at least one hole */

bool CSXCAD::contains_hole(Hyp2Mat::FloatPolygons& polygons)
{
  bool result = false;

  for (FloatPolygons::iterator i = polygons.begin(); i != polygons.end(); ++i) {
    result = i->is_hole;
    if (result) break;
    };

  return result;
}

  /*
   * Export dielectric
   * If pcb_outline is true, export exact board shape, including holes. 
   * If pcb_outline is false, export bounding box.
   */
 
void CSXCAD::export_board(Hyp2Mat::PCB& pcb, bool pcb_outline)
{
  /* CSXCAD coordinate grid definition */
  Bounds bounds = pcb.GetBounds();

  std::cout << "function CSX = pcb(CSX)" << std::endl;
  std::cout << "% matlab script created by hyp2mat" << std::endl;
  std::cout << "% create minimal mesh" << std::endl;
  std::cout << "mesh = {};" << std::endl;
  std::cout << "mesh.x = [" << bounds.x_min << " " << bounds.x_max << "];" << std::endl;
  std::cout << "mesh.y = [" << bounds.y_min << " " << bounds.y_max << "];" << std::endl;
  std::cout << "mesh.z = [" << bounds.z_min << " " << bounds.z_max << "];" << std::endl;
  std::cout << "% add mesh" << std::endl;
  std::cout << "CSX = DefineRectGrid(CSX, 1, mesh);" << std::endl;

  /*
   * Export the board. The board outline is positive; 
   */

  /*
   * create board material if at least one positive polygon present
   * We output one polygon for each dielectric layer, as each layer may have 
   * a different epsilon_r.
   */

  if (contains_polygon(pcb.board)) {

    for (FloatPolygons::iterator i = pcb.board.begin(); i != pcb.board.end(); ++i) {
      /* only output board material, not holes */
      if (i->is_hole) continue; 
      for (LayerList::reverse_iterator l = pcb.stackup.rbegin(); l != pcb.stackup.rend(); ++l) {
        /* only output dielectrics */
        if (l->layer_type != LAYER_DIELECTRIC) continue;

        /* Output command to create material */
        std::cout << "CSX = AddHyperLynxDielectric(CSX, 'Dielectric_" << l->layer_name << "', " << l->epsilon_r <<  ", " << l->loss_tangent << ");" << std::endl;

        /* output CSXCAD polygon */
        std::cout << "% board outline, layer " << l->layer_name << std::endl;
        int priority = prio_dielectric + i->nesting_level;
        if (pcb_outline) {
           /* Use AddLinPoly */
           export_edge(i->poly);
           std::cout << "CSX = AddLinPoly(CSX, 'Dielectric_" << l->layer_name << "', " << priority << ", 2, " << l->z0 << ", pgon, " << l->z1 - l->z0 << ");" << std::endl;
          }
        else {
          /* Use AddBox */
          std::cout << "CSX = AddBox(CSX, 'Dielectric_" << l->layer_name << "', " << priority << ", [ ";
          std::cout << bounds.x_min << ", " << bounds.y_min << ", " << l->z0 << "], [ ";
          std::cout << bounds.x_max << ", " << bounds.y_max << ", " << l->z1 << "]  );" << std::endl;
          }
        }
      }
    };


  /*
   * Export board cutouts
   */

  /* 
   * create board cutout material if at least one negative polygon present
   * For each cutout we create a single polygon which goes through all layers.
   */

  if (pcb_outline && contains_hole(pcb.board)) {
    std::cout << "% create board cutout material" << std::endl;
    std::cout << "CSX = AddMaterial( CSX, 'Drill');" << std::endl;
    std::cout << "CSX = SetMaterialProperty( CSX, 'Drill', 'Epsilon', 1, 'Mue', 1);" << std::endl;

    for (FloatPolygons::iterator i = pcb.board.begin(); i != pcb.board.end(); ++i) {
      /* output CSXCAD polygon */
      if (!i->is_hole) continue;
      std::cout << "% board cutout" << std::endl;
      export_edge(i->poly);
      int priority = prio_dielectric + i->nesting_level;
      std::cout << "CSX = AddLinPoly(CSX, 'Drill', " << priority << ", 2, " << bounds.z_min << ", pgon, " << bounds.z_max - bounds.z_min << ");" << std::endl;
      }
    };

  return;
}

  /*
   * Export copper.
   * If lossy_copper is true, take copper conductivity into account. 
   * If lossy_copper is false, assume copper is perfect conductor.
   */
 
void CSXCAD::export_layer(Hyp2Mat::PCB& pcb, Hyp2Mat::Layer& layer, bool lossy_copper, bool metal_3d, bool odd)
{
  std::string layer_material = layer.layer_name + "_copper";
  std::string layer_cutout = layer.layer_name + "_cutout";
  
  // create layer material if at least one positive polygon present
  if (contains_polygon(layer.metal)) {
      std::cout << "% create layer " << layer.layer_name << " material" << std::endl;

      double copper_resistivity = 0.0;
      if (lossy_copper && (layer.bulk_resistivity > 0)) copper_resistivity = layer.bulk_resistivity;
      
      if (metal_3d) 
        std::cout << "CSX = AddHyperLynxMetal3D(CSX, '" << layer_material << "', " << copper_resistivity <<  ", " <<  layer.thickness << ");" << std::endl;
      else
        std::cout << "CSX = AddHyperLynxMetal2D(CSX, '" << layer_material << "', " << copper_resistivity <<  ", " <<  layer.thickness << ");" << std::endl;
      };

  // create layer cutout material if at least one hole present
  if (contains_hole(layer.metal)) {
      std::cout << "% create layer " << layer.layer_name << " cutout" << std::endl;
      std::cout << "CSX = AddMaterial( CSX, '" << layer_cutout << "');" << std::endl;
      // outer and inner copper layers have different epsilonr
      std::cout << "CSX = SetMaterialProperty( CSX, '" << layer_cutout << "', 'Epsilon', " << layer.epsilon_r << ", 'Mue', 1);" << std::endl;
      };

  /*
   * Export the layer. 
   */

  std::string material;
 
  for (FloatPolygons::iterator i = layer.metal.begin(); i != layer.metal.end(); ++i) {
    /* output CSXCAD polygon */
    if (!i->is_hole) {
      std::cout << "% copper" << std::endl;
      material = layer_material;
      }
    else {
      std::cout << "% cutout" << std::endl;
      material = layer_cutout;
      };

    export_edge(i->poly);
    int priority = prio_material + i->nesting_level;
    /* Default is exporting copper as a 2D conducting sheet */
    if (metal_3d) {
      // We assume alternating layers of substrate and prepreg,and that metal layers are pushed into the prepreg.
      // This means that the top metal layer is pointing up, the second metal layer is pointing down, and so on.
      double thickness = layer.thickness;
      if (odd) thickness = -thickness;
      std::cout << "CSX = AddLinPoly(CSX, '" << material << "', " << priority << ", 2, " << layer.z0 << ", pgon, " << thickness << ");" << std::endl;
      }
    else
      std::cout << "CSX = AddPolygon(CSX, '" << material << "', " << priority << ", 2, " << layer.z0 << ", pgon);" << std::endl;
    }

  return;
}

/*
 * Export vias
 */

void CSXCAD::export_vias(Hyp2Mat::PCB& pcb)
{
  if (!pcb.via.empty()) {
    /* create via material */
    std::cout << "% via copper" << std::endl;
    std::cout << "CSX = AddMetal( CSX, 'via' );" << std::endl;
    for (ViaList::iterator i = pcb.via.begin(); i != pcb.via.end(); ++i) {
      double z0 = i->z0;
      double z1 = i->z1;
      std::cout << "CSX = AddCylinder(CSX, 'via', " << prio_via ;
      std::cout << ", [ " << i->x << " , " << i->y << " , " << z0;
      std::cout << " ], [ " << i->x << " , " << i->y << " , " << z1;
      std::cout << " ], " <<  i->radius << ");" << std::endl;
      }
    }
  return;
}
    
/*
 * Export devices
 */

void CSXCAD::export_devices(Hyp2Mat::PCB& pcb)
{
  std::cout << "% devices" << std::endl;
  std::cout << "CSX.HyperLynxDevice = {};" << std::endl;
  for (DeviceList::iterator i = pcb.device.begin(); i != pcb.device.end(); ++i) {
    std::cout << "CSX.HyperLynxDevice{end+1} = struct('name', " << string2matlab(i->name) << ", 'ref', " << string2matlab(i->ref);

    /* output device value if available */
    if (i->value_type == DEVICE_VALUE_FLOAT) 
      std::cout << ", 'value', " << i->value_float;
    else if (i->value_type == DEVICE_VALUE_STRING) 
       std::cout << ", 'value', " << string2matlab(i->value_string);

    std::cout << ", 'layer_name', " << string2matlab(i->layer_name);
    std::cout << ");" << std::endl; 
    }
}

/*
 * Export port
 */

void CSXCAD::export_ports(Hyp2Mat::PCB& pcb)
{
  std::cout << "% ports" << std::endl;
  std::cout << "CSX.HyperLynxPort = {};" << std::endl;
  for (PinList::iterator i = pcb.pin.begin(); i != pcb.pin.end(); ++i) {
    /* csxcad requires rectangular ports, axis aligned. Use the bounding box. XXX fixme ? Use largest inscribed rectangle instead */
    double x_max = 0, y_max = 0, x_min = 0, y_min = 0;
    bool first = true;
    for (FloatPolygon::iterator j = i->metal.begin(); j != i->metal.end(); ++j) {
      if ((j->x > x_max) || first) x_max = j->x;
      if ((j->y > y_max) || first) y_max = j->y;
      if ((j->x < x_min) || first) x_min = j->x;
      if ((j->y < y_min) || first) y_min = j->y;
      first = false;
      }

    /* determine whether port is on top or bottom layer of pcb */
    double dbottom = std::abs(i->z0 - pcb.stackup.back().z0);
    double dtop = std::abs(i->z1 - pcb.stackup.front().z1);
    bool on_top = dtop <= dbottom;

    std::cout << "CSX.HyperLynxPort{end+1} = struct('ref', " << string2matlab(i->ref);
    std::cout << ", 'xc', " << i->x  << ", 'yc', " << i->y << ", 'z', " << i->z0; 
    std::cout << ", 'x1', " << x_min  << ", 'y1', " << y_min ; 
    std::cout << ", 'x2', " << x_max  << ", 'y2', " << y_max ; 
    std::cout << ", 'position', " << (on_top ? "'top'" : "'bottom'"); 
    std::cout << ", 'layer_name', " <<  string2matlab(i->layer_name) << ");" << std::endl;
    }
  return;
}

/*
 * Write pcb to file in CSXCAD format 
 */

void CSXCAD::Write(const std::string& filename, Hyp2Mat::PCB pcb, bool pcb_outline, bool lossy_copper, bool metal_3d)
{

  /* open file for output */

  if ((filename != "-") && (freopen(filename.c_str(), "w", stdout) == NULL)) {
    std::cerr << "could not open '" << filename << "' for writing";
    return;
    }

  /* Export dielectric. If pcb_outline is true, export exact board shape, including holes. 
     If pcb_outline is false, export bounding box. */
  export_board(pcb, pcb_outline);

  /* Export copper */
  std::cout << "% copper" << std::endl;
  bool odd = false;
  for (LayerList::iterator l = pcb.stackup.begin(); l != pcb.stackup.end(); ++l) {
    if (l->layer_type == LAYER_DIELECTRIC) odd = !odd;
    export_layer(pcb, *l, lossy_copper, metal_3d, odd);
    }

  /* Export vias */
  export_vias(pcb);

  export_devices(pcb);

  export_ports(pcb);

  std::cout << "%not truncated" << std::endl;

  fclose(stdout);

  return;
}

/* not truncated */
