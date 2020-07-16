/*
 * hyp2mat - convert hyperlynx files to QucsStudio PCB format
 * Copyright 2018 Michael Margraf.
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

#include <fstream>
#include <iostream>

#include "qucsstudio.h"

// define the version of output file
#define PACKAGE_VERSION "3.3.1"

// define the layout file identifier
#define QUCSLAYOUT_ID "<QucsStudio Layout "

using namespace Hyp2Mat;

QucsStudio::QucsStudio()
{
}

// ------------------------------------------------------------------
// export the board properties
void QucsStudio::export_board(Hyp2Mat::PCB& pcb)
{
  double thickness = pcb.stackup.begin()->thickness;
  Bounds bounds = pcb.GetBounds();
  double dx = 0.5*(bounds.x_max - bounds.x_min);
  double dy = 0.5*(bounds.y_max - bounds.y_min);

  std::cout << "<Properties>" << std::endl
            << "DataSet=*.dat" << std::endl
            << "DataDisplay=*.dpl" << std::endl
            << "OpenDisplay=1" << std::endl
            << "Grid=0.1 mm" << std::endl
            << "Mesh=20" << std::endl
            << "Excite=2|0.0001|0" << std::endl
            << "Frequency=1 GHz|9 GHz|401" << std::endl
            << "Boundary=" << dx << "|" << dy << "|2" << std::endl;

  bool no_substrate_found = true;

  for (FloatPolygons::iterator i = pcb.board.begin(); i != pcb.board.end(); ++i) {
    if (i->is_hole)
      continue;  // don't output holes

    for (LayerList::reverse_iterator l = pcb.stackup.rbegin(); l != pcb.stackup.rend(); ++l) {
      if (l->layer_type != LAYER_DIELECTRIC)
        continue;   // only output dielectrics

      if(no_substrate_found) {
        no_substrate_found = false;

        // output material parameters
        std::cout << "Substrate=" << l->layer_name
                  << "|" << l->epsilon_r
                  << "|" << l->z1 - l->z0
                  << "|" << l->loss_tangent
                  << "|0|" << thickness << std::endl;
      }
      else
        std::cerr << "Warning: More than one substrate found. Ignoring it." << std::endl;
    }
  }

  if(no_substrate_found)
    std::cout << "Substrate=Subst1|1.0|1 mm|0|0|0" << std::endl;

  std::cout << "</Properties>" << std::endl;
}

// ------------------------------------------------------------------
// export the metallization layer
void QucsStudio::export_layer(Hyp2Mat::PCB& pcb, Hyp2Mat::Layer& layer)
{
  int n;
  char buf[32];
  double cx, cy, dx, dy;
  FloatPolygon::iterator j;
  FloatPolygons::iterator i;
  Hyp2Mat::FloatPolygon edge;
 
  n = 0;
  for (i = layer.metal.begin(); i != layer.metal.end(); ++i) {
    if (i->is_hole)
      continue;

    sprintf(buf, "metal%d ", n++);
    edge = i->poly;
    j = edge.begin();
    cx = j->x;
    cy = j->y;

    if (edge.size() == 4) {  // check if it's a rectangle
      if (edge.at(0).x == edge.at(1).x) {
        if (edge.at(1).y == edge.at(2).y)
          if (edge.at(2).x == edge.at(3).x)
            if (edge.at(3).y == edge.at(0).y) {
              dx = edge.at(2).x - edge.at(1).x;
              if (dx < 0.0) {
                dx = -dx;
                cx = edge.at(2).x;
              }
              dy = edge.at(1).y - edge.at(0).y;
              if (dy < 0.0) {
                dy = -dy;
                cy = edge.at(1).y;
              }
              std::cout << "Rectangle " << buf << cx << " " << cy
                        << " 0|0|" << dx << "|" << dy << std::endl;
              continue;
            }
      }
      else if (edge.at(0).y == edge.at(1).y) {
        if (edge.at(1).x == edge.at(2).x)
          if (edge.at(2).y == edge.at(3).y)
            if (edge.at(3).x == edge.at(0).x) {
              dx = edge.at(1).x - edge.at(0).x;
              if (dx < 0.0) {
                dx = -dx;
                cx = edge.at(1).x;
              }
              dy = edge.at(2).y - edge.at(1).y;
              if (dy < 0.0) {
                dy = -dy;
                cy = edge.at(2).y;
              }
              std::cout << "Rectangle " << buf << cx << " " << cy
                        << " 0|0|" << dx << "|" << dy << std::endl;
              continue;
            }
      }
    }

    std::cout << "Polygon " << buf << cx << " " << cy << " ";
    for (; j != edge.end(); ++j)
      std::cout << "|" << (j->x - cx) << "|" << (j->y - cy);
    std::cout << std::endl;
  }
}

// ------------------------------------------------------------------
void QucsStudio::export_vias(Hyp2Mat::PCB& pcb)
{
  for (ViaList::iterator i = pcb.via.begin(); i != pcb.via.end(); ++i)
    std::cout << "Via X " << i->x << " " << i->y << " "
              <<  i->radius << std::endl;
}
    
// ------------------------------------------------------------------
void QucsStudio::export_devices(Hyp2Mat::PCB& pcb)
{
  for (DeviceList::iterator i = pcb.device.begin(); i != pcb.device.end(); ++i) {
    std::cerr << "Device: name=" << i->name << ", ref=" << i->ref;

    // output device value if available
    if (i->value_type == DEVICE_VALUE_FLOAT) 
      std::cerr << ", value=" << i->value_float;
    else if (i->value_type == DEVICE_VALUE_STRING) 
       std::cerr << ", value=" << i->value_string;

    std::cerr << ", layer_name=" << i->layer_name;
    std::cerr << std::endl; 
  }
}

// ------------------------------------------------------------------
// PCB ports are the connection point of wire lines. So usually every wire
// owns a port, but these are not exciting sources for an EM simulation.
void QucsStudio::export_ports(Hyp2Mat::PCB& pcb)
{
  int no = 0;
  for (PinList::iterator i = pcb.pin.begin(); i != pcb.pin.end(); ++i) {
    no++;

    #if 0
    double x_max = 0, y_max = 0, x_min = 0, y_min = 0;
    bool first = true;
    for (FloatPolygon::iterator j = i->metal.begin(); j != i->metal.end(); ++j) {
      if ((j->x > x_max) || first) x_max = j->x;
      if ((j->y > y_max) || first) y_max = j->y;
      if ((j->x < x_min) || first) x_min = j->x;
      if ((j->y < y_min) || first) y_min = j->y;
      first = false;
    }

    // determine whether port is on top or bottom layer of pcb
    double dbottom = std::abs(i->z0 - pcb.stackup.back().z0);
    double dtop = std::abs(i->z1 - pcb.stackup.front().z1);
    if (dtop > dbottom)  // port on bottom?
      continue;

    x_max -= x_min;
    y_max -= y_min;
    if (x_max >= y_max)
      y_max = 0.0;
    else
      x_max = 0.0;

    std::cout << "Lumped " << i->ref << " " << i->x << " " << i->y << " "
              << no << " 2 50 0 " << x_max << " " << y_max << std::endl;
    #else
    std::cerr << "Port: ref=" << i->ref << ", x=" << i->x << ", y=" << i->y  << std::endl;
    #endif
  }
}

// ------------------------------------------------------------------
// write PCB to file in QucsStudio format 
bool QucsStudio::Write(const std::string& filename, Hyp2Mat::PCB pcb)
{
  if ((filename != "-") && (freopen(filename.c_str(), "w", stdout) == NULL)) {
    std::cerr << "ERROR: Cannot open '" << filename << "' for writing.";
    return false;
  }

  std::cout << QUCSLAYOUT_ID PACKAGE_VERSION ">" << std::endl;

  export_board(pcb);

  // export metalization
  std::cout << "<Items>" << std::endl;
  export_layer(pcb, *pcb.stackup.begin());
  if (pcb.stackup.size() > 1)
    std::cerr << "Warning: More than one layer found. Ignoring it." << std::endl;

  export_vias(pcb);

  export_devices(pcb);

  export_ports(pcb);

  std::cout << "</Items>" << std::endl;
  fclose(stdout);
  return true;
}

/* not truncated */
