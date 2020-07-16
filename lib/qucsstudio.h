/*
 * hyp2mat - convert hyperlynx files to matlab scripts
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

#ifndef QUCSSTUDIO_H
#define QUCSSTUDIO_H

#include "hyp2mat.h"

class QucsStudio {
public:
  QucsStudio();
  bool Write(const std::string& filename, Hyp2Mat::PCB pcb); /* save in QucsStudio format */

private:
  void export_layer(Hyp2Mat::PCB& pcb, Hyp2Mat::Layer& layer); /* output a copper layer */
  void export_board(Hyp2Mat::PCB& pcb); /* output the dielectric */
  void export_vias(Hyp2Mat::PCB& pcb);
  void export_devices(Hyp2Mat::PCB& pcb);
  void export_ports(Hyp2Mat::PCB& pcb);
};

#endif

/* not truncated */
