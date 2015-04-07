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

#ifndef POLYGON_H
#define POLYGON_H

#include "clipper.hpp"
#include "hyp2mat.h"

namespace Hyp2Mat {

  bool IsClockwise (FloatPolygon& poly);
  void Reverse (FloatPolygon& poly);

  class Polygon {
    public:
      Polygon();
      static void SetGrid(double new_grid);
      static double AlignToGrid(double x);
      static void SetArcPrecision(double new_arc_precision);
      void AddEdge(FloatPolygon edge);
      void AddHole(FloatPolygon hole);
      void Offset(double delta);
      void Simplify();
      void Intersection(Polygon clip);
      void Union(Polygon clip);
      void Difference(Polygon clip);
      void Xor(Polygon clip);
      bool IsEmpty();
      FloatPolygons Result();
    private:
      /* convert Hyp2Mat to Clipper */
      void _AddEdge(FloatPolygon edge, bool orientation);
      ClipperLib::IntPoint _convert(FloatPoint p);
      ClipperLib::Polygon _convert(FloatPolygon edge);
      ClipperLib::Polygon _unique(ClipperLib::Polygon p);
  
      /* Clipper polygon operations */
      void _Execute(ClipperLib::ClipType op, Polygon clip);
      ClipperLib::PolyFillType _filltype;
  
      /* convert Clipper to Hyp2Mat */
      void _AddToPolygonList(ClipperLib::PolyNode& polynode, FloatPolygons& polygons, int level);
      FloatPolygon _convert (ClipperLib::Polygon clip_poly);
      FloatPoint _convert (ClipperLib::IntPoint p);

      /* data */
      ClipperLib::Polygons _subject;
      static double _grid; /* resolution of x and y coordinates, identical for all polygons. */
      static double _arc_precision; /* maximum difference between perfect circle arc and polygonal approximation */
    };
  
};

#endif

/* not truncated */
