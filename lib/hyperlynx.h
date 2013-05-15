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
  void CopyBoard(Hyp2Mat::PCB& pcb, HypFile::Hyp& hyp_file, Hyp2Mat::Bounds bounds);
  void CopyStackUp(Hyp2Mat::PCB& pcb, HypFile::Hyp& hyp_file, Hyp2Mat::Bounds bounds);
  void CopyRawPolygons(Hyp2Mat::PCB& pcb, HypFile::Hyp& hyp_file, std::vector<std::string> layers, std::vector<std::string> nets);
  void CopyPolygons(Hyp2Mat::PCB& pcb, HypFile::Hyp& hyp_file, std::vector<std::string> layers, std::vector<std::string> nets, Hyp2Mat::Bounds bounds);
  HypFile::PolygonList PlaneSeparation(HypFile::Hyp& hyp_file, HypFile::Layer& layer, HypFile::Net& net, HypFile::Polygon& poly);
  void CopyVias(Hyp2Mat::PCB& pcb, HypFile::Hyp& hyp_file, std::vector<std::string> nets, Hyp2Mat::Bounds bounds);
  void CopyViaPlating(Hyp2Mat::PCB& pcb, HypFile::Hyp& hyp_file);
  void CopyDevices(Hyp2Mat::PCB& pcb, HypFile::Hyp& hyp_file);
  void CopyPins(Hyp2Mat::PCB& pcb, HypFile::Hyp& hyp_file, std::vector<std::string> nets);
  void CopyPin(Hyp2Mat::PCB& pcb, HypFile::Hyp& hyp_file, HypFile::Pin& pin, HypFile::Padstack& padstack);
  void PrintNets(HypFile::Hyp& hyp_file);
  };

#endif

/* not truncated */
