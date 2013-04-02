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

#ifndef GEOM_H
#define GEOM_H

#include "hyp2mat.h"
#include "clipper.hpp"

class Geometry {
  public:
    Geometry();
    void Simplify(Hyp2Mat::PCB& pcb, double grid, Hyp2Mat::Bounds bounds);
    void CropVias(Hyp2Mat::PCB& pcb, Hyp2Mat::Bounds bounds);
    void CropLayers(Hyp2Mat::PCB& pcb, Hyp2Mat::Bounds bounds);
    void SnapPinsToGrid(Hyp2Mat::PCB& pcb);

    /* convert from Hyp2Mat to ClipperLib */
    ClipperLib::IntPoint convert (Hyp2Mat::Point p);
    ClipperLib::Polygons convert (Hyp2Mat::Edge edge);
    ClipperLib::Polygons convert (Hyp2Mat::Polygon poly);
    ClipperLib::Polygons convert (Hyp2Mat::PolygonList poly_list);

    /* remove repeated vertices */
    ClipperLib::Polygon unique (ClipperLib::Polygon p);
    /* crop polygon to bounding box */
    ClipperLib::Polygons crop (ClipperLib::Polygons poly, double x_min, double x_max, double y_min, double y_max);
    ClipperLib::Polygons invert (ClipperLib::Polygons poly, ClipperLib::Polygons board);

    /* convert from ClipperLib to Hyp2Mat */
    Hyp2Mat::Point convert (ClipperLib::IntPoint p);
    Hyp2Mat::Edge convert (ClipperLib::Polygon poly);
    Hyp2Mat::PolygonList convert (ClipperLib::Polygons poly);

  private:
    void AddToPolyList(ClipperLib::PolyNode& polynode, Hyp2Mat::PolygonList& poly_list, int level);
    void PrintPcb(Hyp2Mat::PCB& pcb);
    double grid;
  };

#endif

/* not truncated */
