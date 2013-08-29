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

#ifndef CROP_H
#define CROP_H

#include "hyp2mat.h"
#include "polygon.h"

namespace Hyp2Mat {
  Hyp2Mat::Bounds AdjustBounds(Hyp2Mat::PCB& pcb, Bounds bounds);
  Hyp2Mat::Polygon CropPolygon (Hyp2Mat::Polygon poly, Hyp2Mat::Bounds bounds);
  void CropVias(Hyp2Mat::PCB& pcb, Hyp2Mat::Bounds bounds);
  void CropLayers(Hyp2Mat::PCB& pcb, Hyp2Mat::Bounds& bounds);
  }

#endif

/* not truncated */
