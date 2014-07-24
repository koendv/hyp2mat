/*
 * hyp2mat - convert hyperlynx files to matlab scripts
 * Copyright 2014 Koen De Vleeschauwer.
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
 * loads Gerber,Excellon and Centroid files in a Hyp2Mat class
 */

#ifndef GERBER_H
#define GERBER_H

#include "hyp2mat.h"
#include "gerbv.h"

class Gerber {
public:
  Gerber();
  ~Gerber();
  void Read (std::vector<std::string> gerber_filenames, std::string outline_filename, std::string tools_filename, std::string drill_filename, std::string pickandplace_filename, Hyp2Mat::PCB& pcb); 

private:
  gerbv_project_t *gerbv_project;
  bool debug;

  void LoadTools(std::string tools_filename);
  void LoadDrill(std::string drill_filename);
  void LoadGerber(std::string gerber_filename);
  void LoadPickAndPlace(std::string pickandplace_filename);
  void LoadFile(std::string filename, gerbv_layertype_t layertype);

  };

#endif

/* not truncated */
