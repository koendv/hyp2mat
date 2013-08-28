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
  void Read (std::string filename, Hyp2Mat::PCB& pcb, std::vector<std::string> layers, std::vector<std::string> nets, bool raw, double arc_precision, Hyp2Mat::Bounds bounds);

private:
  bool LoadHypFile(Hyp2Mat::PCB& pcb, HypFile::Hyp& hyp_file, std::string filename, double arc_precision);
  void CopyBoard(Hyp2Mat::PCB& pcb, HypFile::Hyp& hyp_file, Hyp2Mat::Polygon& board, Hyp2Mat::Bounds& bounds);
  void CopyStackUp(Hyp2Mat::PCB& pcb, HypFile::Hyp& hyp_file, Hyp2Mat::Bounds bounds, Hyp2Mat::Polygon& board, std::vector<std::string> layer_names, std::vector<std::string> net_names, bool raw);
  void CopyLayer(Hyp2Mat::PCB& pcb, HypFile::Hyp& hyp_file, Hyp2Mat::Bounds bounds, Hyp2Mat::Polygon& board, HypFile::Layer& hyp_layer, std::vector<std::string> net_names, bool raw);
  Hyp2Mat::Polygon CopyCopper(Hyp2Mat::PCB& pcb, HypFile::Hyp& hyp_file, Hyp2Mat::Bounds bounds, Hyp2Mat::Polygon& board, Hyp2Mat::Layer layer, HypFile::Layer& hyp_layer, std::vector<std::string> net_names, double plane_separation, Hyp2Mat::FloatPolygons& raw_polygons);
  Hyp2Mat::Polygon CopyNet(Hyp2Mat::PCB& pcb, HypFile::Hyp& hyp_file, Hyp2Mat::Layer layer, HypFile::Net& hyp_net, double plane_separation, bool plane, Hyp2Mat::Polygon other_nets, Hyp2Mat::FloatPolygons& raw_polygons);
  Hyp2Mat::Polygon CopyPolygon(HypFile::PolygonList metal, double plane_separation, bool plane, Hyp2Mat::Polygon other_nets, Hyp2Mat::FloatPolygons& raw_polygons);

  void CopyVias(Hyp2Mat::PCB& pcb, HypFile::Hyp& hyp_file, Hyp2Mat::Bounds bounds, std::vector<std::string> nets);
  void CopyViaPlating(Hyp2Mat::PCB& pcb, HypFile::Hyp& hyp_file);
  void CopyDevices(Hyp2Mat::PCB& pcb, HypFile::Hyp& hyp_file);
  void CopyPins(Hyp2Mat::PCB& pcb, HypFile::Hyp& hyp_file, std::vector<std::string> nets);
  void CopyPin(Hyp2Mat::PCB& pcb, HypFile::Hyp& hyp_file, HypFile::Pin& pin, HypFile::Padstack& padstack);
  void PrintNets(HypFile::Hyp& hyp_file);
  };

#endif

/* not truncated */
