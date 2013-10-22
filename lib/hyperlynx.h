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
 * loads a Hyperlynx file in a Hyp2Mat class
 */

#ifndef HYPERLYNX_H
#define HYPERLYNX_H

#include "hyp2mat.h"
#include "hypfile.h"
#include "polygon.h"

class HyperLynx {
public:
  HyperLynx();
  void Read (std::string filename, Hyp2Mat::PCB& pcb);

  /* parameters */
  std::vector<std::string> layers; 
  std::vector<std::string> nets; 
  bool raw;
  double arc_precision;
  double clearance;
  std::vector<std::string> flood_layers;
  Hyp2Mat::Bounds bounds;

private:
  HypFile::Hyp hyp_file;
  Hyp2Mat::Polygon board; /* polygon of board outline */
  Hyp2Mat::Bounds saved_bounds; /* copy of bounds */

  double layer_plane_separation; /* plane separation for this layer */
  double net_plane_separation; /* plane separation for this net on this layer */
  double polygon_plane_separation; /* plane separation for this polygon on this net on this layer */

  bool LoadHypFile(Hyp2Mat::PCB& pcb, std::string filename, double arc_precision);
  void CopyBoard(Hyp2Mat::PCB& pcb); /* copy board outline */

  void CopyStackUp(Hyp2Mat::PCB& pcb); /* copy all layers, both metal and dielectric */
  void CopyLayer(Hyp2Mat::PCB& pcb, HypFile::Layer& hyp_layer); /* copy a single layer */
  Hyp2Mat::Polygon CopyCopper(Hyp2Mat::Layer layer, Hyp2Mat::FloatPolygons& raw_polygons);  /* copy all the copper of a layer */
  Hyp2Mat::Polygon CopyNet(Hyp2Mat::Layer layer, HypFile::Net& hyp_net, polygon_type_enum poly_type, Hyp2Mat::FloatPolygons& raw_polygons); /* copy a single net on a layer */

  Hyp2Mat::Polygon CopyPolygon(HypFile::PolygonList metal, Hyp2Mat::FloatPolygons& raw_polygons); /* copy a single polygon of a net on a layer */
  Hyp2Mat::Polygon PlaneSeparation(std::string layer_name, std::string net_name);

  void CopyVias(Hyp2Mat::PCB& pcb);
  void CopyViaPlating(Hyp2Mat::PCB& pcb);

  void CopyDevices(Hyp2Mat::PCB& pcb);

  void CopyPins(Hyp2Mat::PCB& pcb); /* copy all pins to pcb */
  void CopyPin(Hyp2Mat::PCB& pcb, HypFile::Pin& pin, HypFile::Padstack& padstack); /* copy a single pin to pcb */

  void PrintNets();
  };

#endif

/* not truncated */
