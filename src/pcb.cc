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

#include <iostream>
#include <iomanip>

#include "config.h"
#include "hyp2mat.h"

#include "geometry.h"
#include "hyperlynx.h"
#include "csxcad.h"
#include "pdf.h"

using namespace Hyp2Mat;

PCB::PCB()
{
  debug = 0;
  via_plating_thickness = 0;
}

void PCB::Simplify(double grid)
{
  Simplify(grid, GetBounds());
}

void PCB::Simplify(double grid, Bounds bounds)
{
  Geometry geom;
  geom.Simplify(*this, grid, bounds);
}

/*
 * Read a pcb in hyperlynx format 
 */

void PCB::ReadHyperLynx (std::string filename, std::vector<std::string> nets, double arc_precision)
{
  HyperLynx hyperlynx;
  hyperlynx.Read(filename, *this, nets, arc_precision);
}

/*
 * Write a pcb in csxcad format
 */

void PCB::WriteCSXCAD (std::string filename)
{
  CSXCAD csxcad;
  csxcad.Write(filename, *this);
}

/*
 * Write a pcb in pdf format
 */

void PCB::WritePDF (std::string filename, double hue, double saturation, double brightness)
{
  PDF pdf;
  pdf.SetPalette(hue, saturation, brightness);
  pdf.Write(filename, *this);
}

/* 
 * Set dielectric epsilon r. Overrides value in Hyperlynx file. 
 */

void PCB::SetEpsilonR(double epsilon_r) 
{

  /* set all layers to epsilon_r, except outer copper layers, which we assume are in air (er = 1) */

  /* iterate over all layers, setting epsilon */
  for (LayerList::iterator i = stackup.begin(); i != stackup.end(); ++i) {
    i->epsilon_r = epsilon_r;

    /* check for outer copper layer */ 
    if (i->layer_type == LAYER_DIELECTRIC)
      continue;
    
    /* find dielectric layer above current metal layer */
    bool upper_layer_found = false;
    for (LayerList::iterator j = stackup.begin(); j != i; ++j)
      if (j->layer_type == LAYER_DIELECTRIC) upper_layer_found = true;

    /* find dielectric layer below current metal layer */
    bool lower_layer_found = false;
    for (LayerList::iterator j = i; j != stackup.end(); ++j)
      if (j->layer_type == LAYER_DIELECTRIC) lower_layer_found = true;

    /* assume outer copper layers are in air */
    if (!upper_layer_found || !lower_layer_found) {
      i->epsilon_r = 1.0;
      i->loss_tangent = 0.0;
      }
    }
  return;
}

/*
 * Calculate the bounding rectangle of a pcb.
 */

Bounds PCB::GetBounds() {
  Bounds bounds;
  bounds.x_min = 0;
  bounds.x_max = 0;
  bounds.y_min = 0;
  bounds.y_max = 0;
  bounds.z_min = 0;
  bounds.z_max = 0;

  /* iterate over layers; find z_max and z_min */
  if (stackup.empty())
    std::cerr << "warning: no layers" << std::endl;
  else {
    bounds.z_min = stackup.back().z0;
    bounds.z_max = stackup.front().z1;
    for (LayerList::iterator l = stackup.begin(); l != stackup.end(); ++l) {
      if (l->z0 < bounds.z_min) bounds.z_min = l->z0;
      if (l->z1 > bounds.z_max) bounds.z_max = l->z1;
      }
    }

  /* iterate over board edge; find x_min, x_max, y_min, y_max */
  bool first_point = true;

  for (PolygonList::iterator i = board.begin(); i != board.end(); ++i)
    for (Polygon::iterator j = i->begin(); j != i->end(); ++j)
      for (PointList::iterator k = j->vertex.begin(); k != j->vertex.end(); ++k) {
        if ((k->x > bounds.x_max) || first_point) bounds.x_max = k->x;
        if ((k->y > bounds.y_max) || first_point) bounds.y_max = k->y;
        if ((k->x < bounds.x_min) || first_point) bounds.x_min = k->x;
        if ((k->y < bounds.y_min) || first_point) bounds.y_min = k->y;
        first_point = false;
        }

  return bounds;
  }

void PCB::PrintSummary() {
  Bounds bounds = GetBounds();

  /* save stream flags and precision */
  std::ios::fmtflags saved_flgs = std::cerr.flags();
  std::streamsize saved_prec = std::cerr.precision();
  std::cerr.flags(std::ios::fixed);
  std::cerr.precision(2);

  double m_to_mm = 1000;
  std::cerr << "board size:" << std::endl;
  std::cerr << " x = " << std::setw(8) << bounds.x_min*m_to_mm << " : " << std::setw(8) << bounds.x_max*m_to_mm << " mm" << std::endl;
  std::cerr << " y = " << std::setw(8) << bounds.y_min*m_to_mm << " : " << std::setw(8) << bounds.y_max*m_to_mm << " mm" << std::endl;
  std::cerr << " z = " << std::setw(8) << bounds.z_min*m_to_mm << " : " << std::setw(8) << bounds.z_max*m_to_mm << " mm" << std::endl;

  std::cerr << "layers:" << std::endl;
  if (stackup.empty()) std::cerr << "(none)";
  for (LayerList::iterator it = stackup.begin(); it != stackup.end(); ++it) {
    std::cerr << " ";
    switch (it->layer_type) {
      case LAYER_SIGNAL:     std::cerr << "signal     " <<  std::setw(8) << it->thickness() * 1E6 << " um" ; break;
      case LAYER_DIELECTRIC: std::cerr << "dielectric " <<  std::setw(8) << it->thickness() * 1E3 << " mm" ; break;
      case LAYER_PLANE:      std::cerr << "plane      " <<  std::setw(8) << it->thickness() * 1E6 << " um" ; break;
      default:               std::cerr << "***        " <<  std::setw(8) << it->thickness() * 1E3 << " mm" ; break;
      }
    std::cerr << " eps_r " << it->epsilon_r << " '" << it->layer_name << "'" << std::endl;
    }

  /* reset flags and precision */
  std::cerr.flags(saved_flgs);
  std::cerr.precision(saved_prec);

  return;
}

/* not truncated */
