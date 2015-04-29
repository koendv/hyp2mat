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

#ifndef CSXCAD_H
#define CSXCAD_H

#include "hyp2mat.h"

class CSXCAD {
public:
  CSXCAD();
  void Write(const std::string& filename, Hyp2Mat::PCB pcb, bool pcb_outline, bool lossy_copper, bool metal_3d); /* save in CSXcad format */

private:
  int prio_dielectric;  // FR4 dielectric
  int prio_material;    // copper
  int prio_via;         // via metal
  int prio_drill;       // hole

  double adjust_z(Hyp2Mat::PCB& pcb, double z);

  void export_edge(Hyp2Mat::FloatPolygon& edge); /* output a polygon edge */
  void export_layer(Hyp2Mat::PCB& pcb, Hyp2Mat::Layer& layer, bool lossy_copper, bool metal_3d, bool odd); /* output a copper layer */
  void export_board(Hyp2Mat::PCB& pcb, bool pcb_outline); /* output the dielectric */
  void export_vias(Hyp2Mat::PCB& pcb);
  void export_devices(Hyp2Mat::PCB& pcb);
  void export_ports(Hyp2Mat::PCB& pcb);
  bool contains_polygon(Hyp2Mat::FloatPolygons& polygons); /* true if polygon list contains at least one (positive) polygon */
  bool contains_hole(Hyp2Mat::FloatPolygons& polygons); /* true if polygon list contains at least one hole */
  std::string string2matlab(std::string str); /* quote a string using matlab conventions */

  };

#endif

/* not truncated */
