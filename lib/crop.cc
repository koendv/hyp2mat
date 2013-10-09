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

#include "crop.h"

using namespace Hyp2Mat;

/*
 * Adjust bounds to the size of the board 
 */

Hyp2Mat::Bounds Hyp2Mat::AdjustBounds(Hyp2Mat::PCB& pcb, Hyp2Mat::Bounds bounds) 
{
  Bounds new_bounds = bounds;
  Bounds size = pcb.GetBounds();

  if (new_bounds.x_min < size.x_min) new_bounds.x_min = size.x_min;
  if (new_bounds.x_max > size.x_max) new_bounds.x_max = size.x_max;
  if (new_bounds.y_min < size.y_min) new_bounds.y_min = size.y_min;
  if (new_bounds.y_max > size.y_max) new_bounds.y_max = size.y_max;
  if (new_bounds.z_min < size.z_min) new_bounds.z_min = size.z_min;
  if (new_bounds.z_max > size.z_max) new_bounds.z_max = size.z_max;

  return new_bounds;
}

/*
 * Crop polygon
 */

Hyp2Mat::Polygon Hyp2Mat::CropPolygon (Hyp2Mat::Polygon poly, Hyp2Mat::Bounds bounds)
{
  Hyp2Mat::Polygon result;
  FloatPolygon bounding_poly;

  bounding_poly.push_back(FloatPoint(bounds.x_min, bounds.y_min));
  bounding_poly.push_back(FloatPoint(bounds.x_max, bounds.y_min));
  bounding_poly.push_back(FloatPoint(bounds.x_max, bounds.y_max));
  bounding_poly.push_back(FloatPoint(bounds.x_min, bounds.y_max));
  result.AddEdge(bounding_poly);

  result.Intersection(poly);

  return result;
}

/*
 * Remove all vias which are out of bounds.
 * XXX no longer needed?
 */

void Hyp2Mat::CropVias(Hyp2Mat::PCB& pcb, Hyp2Mat::Bounds bounds)
{
  Hyp2Mat::Via new_via;
  Hyp2Mat::ViaList new_vialist;

  for (Hyp2Mat::ViaList::iterator i = pcb.via.begin(); i != pcb.via.end(); ++i) {
    if ((i->x > bounds.x_max) || (i->x < bounds.x_min) || (i->y > bounds.y_max) || (i->y < bounds.y_min)) continue;
    if ((i->z0 > bounds.z_max) && (i->z1 > bounds.z_max)) continue;
    if ((i->z0 < bounds.z_min) && (i->z1 < bounds.z_min)) continue;
    new_via = *i;
    if (new_via.z1 > bounds.z_max) new_via.z1 = bounds.z_max;
    if (new_via.z0 < bounds.z_min) new_via.z0 = bounds.z_min;
    new_vialist.push_back(new_via);
    }
  pcb.via = new_vialist;
  return;
}

/*
 * Remove all layers which are out of bounds.
 */

void Hyp2Mat::CropLayers(Hyp2Mat::PCB& pcb, Hyp2Mat::Bounds& bounds)
{
  Hyp2Mat::Layer new_layer;
  Hyp2Mat::LayerList new_stackup;

  for (Hyp2Mat::LayerList::iterator l = pcb.stackup.begin(); l != pcb.stackup.end(); ++l) {
    if ((l->z0 > bounds.z_max) && (l->z1 > bounds.z_max)) continue;
    if ((l->z0 < bounds.z_min) && (l->z1 < bounds.z_min)) continue;
    new_layer = *l;
    if (new_layer.z1 > bounds.z_max) new_layer.z1 = bounds.z_max;
    if (new_layer.z0 < bounds.z_min) new_layer.z0 = bounds.z_min;
    new_stackup.push_back(new_layer);
    }
  pcb.stackup = new_stackup;
  return;
}

/* not truncated */
