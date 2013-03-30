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

 /*
  * adjust_z calculates the position of a layer, assuming metal layers have thickness 0.
  * This improve simulation speed.
  */

double CSXCAD::adjust_z(Hyp2Mat::PCB& pcb, double z)
{

  /* look up z in the layer list */
  for (LayerList::reverse_iterator l = pcb.stackup.rbegin(); l != pcb.stackup.rend(); ++l) {
    if (l->z0 == z) return l->z0;
    if (l->z1 == z) {
      /* enforce zero thickness on signal and plane layers */
      if  ((l->layer_type == LAYER_SIGNAL) || (l->layer_type == LAYER_PLANE)) return l->z0;
      /* dielectric layers are ok */
      else return l->z1;
      }
    }
  /* default is unchanged. Ought never to get here. */
  return z;
}

void CSXCAD::export_edge(Edge& edge)
{
  std::cout << "pgon = [];" << std::endl;
  for (PointList::iterator i = edge.vertex.begin(); i != edge.vertex.end(); ++i) {
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

bool CSXCAD::contains_polygon(Hyp2Mat::PolygonList& polylist)
{
  bool result = false;

  for (PolygonList::iterator i = polylist.begin(); i != polylist.end(); ++i) {
    for (Polygon::iterator j = i->begin(); j != i->end(); ++j) {
      result = !j->is_hole;
      if (result) break;
      };
    if (result) break;
    }

  return result;
}

/* true if polygon list contains at least one hole */

bool CSXCAD::contains_hole(Hyp2Mat::PolygonList& polylist)
{
  bool result = false;

  for (PolygonList::iterator i = polylist.begin(); i != polylist.end(); ++i) {
    for (Polygon::iterator j = i->begin(); j != i->end(); ++j) {
      result = j->is_hole;
      if (result) break;
      };
    if (result) break;
    }

  return result;
}

  /*
   * Export dielectric
   */
 
void CSXCAD::export_board(Hyp2Mat::PCB& pcb)
{
  double fr4_epsilon_r = 4.3; // fixme XXX handle case where board layers have different dielectric constant
  for (LayerList::iterator l = pcb.stackup.begin(); l != pcb.stackup.end(); ++l)
    if (l->layer_type == LAYER_DIELECTRIC) fr4_epsilon_r = l->epsilon_r;

  /* CSXCAD coordinate grid definition */
  Bounds bounds = pcb.GetBounds();
  double z_min = adjust_z(pcb, bounds.z_min);
  double z_max = adjust_z(pcb, bounds.z_max);

  std::cout << "function CSX = pcb(CSX)" << std::endl;
  std::cout << "% matlab script created by hyp2mat" << std::endl;
  std::cout << "% create minimal mesh" << std::endl;
  std::cout << "mesh = {};" << std::endl;
  std::cout << "mesh.x = [" << bounds.x_min << " " << bounds.x_max << "];" << std::endl;
  std::cout << "mesh.y = [" << bounds.y_min << " " << bounds.y_max << "];" << std::endl;
  std::cout << "mesh.z = [" << z_min << " " << z_max << "];" << std::endl;
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
    std::vector<double> dielectrics;

    for (PolygonList::iterator i = pcb.board.begin(); i != pcb.board.end(); ++i)
      for (Polygon::iterator j = i->begin(); j != i->end(); ++j) {
        /* only output board material, not holes */
        if (j->is_hole) continue; 
        for (LayerList::reverse_iterator l = pcb.stackup.rbegin(); l != pcb.stackup.rend(); ++l) {
          /* only output dielectrics */
          if (l->layer_type != LAYER_DIELECTRIC) continue;

          /* find material corresponding to dielectric constant */
          std::vector<double>::iterator it = std::find(dielectrics.begin(), dielectrics.end(), l->epsilon_r);
          if (it == dielectrics.end()) {
            /* not found, create material */
            /* add dielectric to list */ 
            dielectrics.push_back(l->epsilon_r);
            it = dielectrics.end(); --it;
            /* create material */
            int index = std::distance(dielectrics.begin(), it);
            std::cout << "% create board material" << std::endl;
            std::cout << "CSX = AddMaterial( CSX, 'Dielectric" << index << "');" << std::endl;
            std::cout << "CSX = SetMaterialProperty( CSX, 'Dielectric" << index << "', 'Epsilon', " << *it << ", 'Mue', 1);" << std::endl;
            }

          /* output CSXCAD polygon */
          int index = std::distance(dielectrics.begin(), it);
          std::cout << "% board outline, layer " << l->layer_name << std::endl;
          export_edge(*j);
          int priority = prio_dielectric + j->nesting_level;
          double z0 = adjust_z(pcb, l->z0);
          double z1 = adjust_z(pcb, l->z1);
          std::cout << "CSX = AddLinPoly(CSX, 'Dielectric" << index << "', " << priority << ", 2, " << z0 << ", pgon, " << z1 - z0 << ");" << std::endl;

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

  if (contains_hole(pcb.board)) {
    std::cout << "% create board cutout material" << std::endl;
    std::cout << "CSX = AddMaterial( CSX, 'Drill');" << std::endl;
    std::cout << "CSX = SetMaterialProperty( CSX, 'Drill', 'Epsilon', 1, 'Mue', 1);" << std::endl;

    for (PolygonList::iterator i = pcb.board.begin(); i != pcb.board.end(); ++i)
      for (Polygon::iterator j = i->begin(); j != i->end(); ++j) {
        /* output CSXCAD polygon */
        if (!j->is_hole) continue;
        std::cout << "% board cutout" << std::endl;
        export_edge(*j);
        int priority = prio_dielectric + j->nesting_level;
        std::cout << "CSX = AddLinPoly(CSX, 'Drill', " << priority << ", 2, " << z_min << ", pgon, " << z_max - z_min << ");" << std::endl;
      }
    };

  return;
}

  /*
   * Export copper
   */
 
void CSXCAD::export_layer(Hyp2Mat::PCB& pcb, Hyp2Mat::Layer& layer)
{
  std::string layer_material = layer.layer_name + "_copper";
  std::string layer_cutout = layer.layer_name + "_cutout";
  
  // create layer material if at least one positive polygon present
  if (contains_polygon(layer.metal)) {
      double copper_conductivity = 1 / layer.bulk_resistivity;
      std::cout << "% create layer " << layer.layer_name << " material" << std::endl;
      std::cout << "CSX = AddConductingSheet(CSX, '" << layer_material << "', " << copper_conductivity << ", " << layer.thickness() << ");" << std::endl;
      };

  // create layer cutout material if at least one hole present
  if (contains_hole(layer.metal)) {
      std::cout << "% create layer " << layer.layer_name << " cutout" << std::endl;
      std::cout << "CSX = AddConductingSheet(CSX, '" << layer_cutout << "', " << 0 << ", " << layer.thickness() << ");" << std::endl;
      };

  /*
   * Export the layer. 
   */

  std::string material;
 
  for (PolygonList::iterator i = layer.metal.begin(); i != layer.metal.end(); ++i)
    for (Polygon::iterator j = i->begin(); j != i->end(); ++j) {
      /* output CSXCAD polygon */
      if (!j->is_hole) {
        std::cout << "% copper" << std::endl;
        material = layer_material;
        }
      else {
        std::cout << "% cutout" << std::endl;
        material = layer_cutout;
        };

      export_edge(*j);
      int priority = prio_material + j->nesting_level;
      double z0 = adjust_z(pcb, layer.z0);
      std::cout << "CSX = AddPolygon(CSX, '" << material << "', " << priority << ", 2, " << z0 << ", pgon);" << std::endl;
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
      double z0 = adjust_z(pcb, i->z0);
      double z1 = adjust_z(pcb, i->z1);
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
    for (PointList::iterator j = i->metal.vertex.begin(); j != i->metal.vertex.end(); j++) {
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
    double z0 = adjust_z(pcb, i->z0);

    std::cout << "CSX.HyperLynxPort{end+1} = struct('ref', " << string2matlab(i->ref);
    std::cout << ", 'xc', " << i->x  << ", 'yc', " << i->y << ", 'z', " << z0; 
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

void CSXCAD::Write(const std::string& filename, Hyp2Mat::PCB pcb)
{
  /* open file for output */

  if ((filename != "-") && (freopen(filename.c_str(), "w", stdout) == NULL)) {
    std::cerr << "could not open '" << filename << "' for writing";
    return;
    }

  /* Export dielectric */
  export_board(pcb);

  /* Export copper */
  std::cout << "% copper" << std::endl;
  for (LayerList::iterator l = pcb.stackup.begin(); l != pcb.stackup.end(); ++l)
    export_layer(pcb, *l);

  /* Export vias */
  export_vias(pcb);

  export_devices(pcb);

  export_ports(pcb);

  std::cout << "%not truncated" << std::endl;

  fclose(stdout);

  return;
}

/* not truncated */
