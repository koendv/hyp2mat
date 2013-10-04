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

#include <cmath>
#include <iostream>
#include <algorithm>

#include "polygon.h"

using namespace Hyp2Mat;

double Polygon::_grid = 10E-6; /* 10 micron */
double Polygon::_arc_precision = 0.0;

Polygon::Polygon()
{
  _filltype = ClipperLib::pftNonZero; // Use edge orientation to determine whether or not polygon edge is a hole.
}

void Polygon::SetGrid(double new_grid)
{
  _grid = new_grid;
}

void Polygon::SetArcPrecision(double new_arc_precision)
{
  _arc_precision = new_arc_precision;
}

/*
 * Align coordinates to grid
 */

double Polygon::AlignToGrid(double x)
{
  return floor(x / _grid + 0.5) * _grid;
}

/*
 * Return true if polygon is empty 
 */

bool Polygon::IsEmpty()
{
  return _subject.empty();
}

/*
 * Add an outer edge to the polygon
 */

void Polygon::AddEdge(FloatPolygon edge)
{
  _AddEdge(edge, true);
}

/*
 * Add an inner edge to the polygon
 */

void Polygon::AddHole(FloatPolygon hole)
{
  _AddEdge(hole, false);
}

/*
 * Add an edge - positive or negative - to the polygon
 */

void Polygon::_AddEdge(FloatPolygon edge, bool orientation)
{

  /* trivial case */
  if (edge.empty()) return;

  /* convert edge to Clipper polygon */
  ClipperLib::Polygon poly = _convert(edge);

  /* orient edge clockwise */
  if (!Orientation(poly) == orientation) ClipperLib::ReversePolygon(poly);

  /* Add to polygon */
  _subject.push_back(poly);
 
  return;
}

/* 
 * convert from Hyp2Mat floating point point to Clipper long64 point 
 */

ClipperLib::IntPoint Polygon::_convert(FloatPoint p)
{
  return ClipperLib::IntPoint(floor(p.x / _grid + 0.5), floor(p.y / _grid + 0.5));
}

/* 
 * convert from Hyp2Mat floating point polygon to Clipper long64 polygon 
 */

ClipperLib::Polygon Polygon::_convert(FloatPolygon edge)
{
  ClipperLib::Polygon poly;
  for (FloatPolygon::iterator i = edge.begin(); i != edge.end(); ++i)
    poly.push_back(_convert(*i));

  /* remove repeated vertices */
  poly = _unique(poly);

  return poly;
}

/*
 * Remove repeated vertices from a polygon
 */

ClipperLib::Polygon Polygon::_unique(ClipperLib::Polygon p) 
{
  ClipperLib::Polygon q = p;
  q.clear();
 
  if (p.empty()) return q; 

  /* initial condition: add first vertex */
  q.push_back(p.front());

  /* create list of non-repeating vertices */
  for (ClipperLib::Polygon::iterator i = p.begin(); i != p.end(); ++i)
    if ((i->X != q.back().X) || (i->Y != q.back().Y)) q.push_back(*i);
  
  /* Remove last vertex if first and last vertex same */ 
  while ((q.size() > 1) && ((q.front().X == q.back().X) && (q.front().Y == q.back().Y)))
    q.pop_back();
    
  return q; 
}

/*
 * Expand the polygon by a distance
 */

void Polygon::Offset(double delta)
{
  ClipperLib::Polygons result;
  double d = delta / _grid;
  double limit = _arc_precision / _grid;

  if (delta == 0.0) return;

  if (_arc_precision == 0)
    /* square corners */
    OffsetPolygons(_subject, result, d, ClipperLib::jtSquare);
  else
    /* round corners */
    OffsetPolygons(_subject, result, d, ClipperLib::jtRound, limit);

  _subject = result;
  
  return;
}

/*
 * Remove self-intersections
 */

void Polygon::Simplify()
{
  ClipperLib::Polygons result;
  ClipperLib::SimplifyPolygons(_subject, result, _filltype);
  _subject = result;

}

/*
 * Basic polygon operations
 */

void Polygon::Intersection(Polygon clip)
{
  _Execute(ClipperLib::ctIntersection, clip);
}

void Polygon::Union(Polygon clip)
{
  _Execute(ClipperLib::ctUnion, clip);
}

void Polygon::Difference(Polygon clip)
{
  _Execute(ClipperLib::ctDifference, clip);
}

void Polygon::Xor(Polygon clip)
{
  _Execute(ClipperLib::ctXor, clip);
}

void Polygon::_Execute(ClipperLib::ClipType op, Polygon clip)
{
  ClipperLib::Polygons result;
  ClipperLib::Clipper clipper;
 
  clipper.AddPolygons(_subject, ClipperLib::ptSubject); 
  clipper.AddPolygons(clip._subject, ClipperLib::ptClip); 

  clipper.Execute(op, result, _filltype, _filltype);
  _subject = result;

  return;
}

/*
 * Convert from Clipper long64 polygon to Hyp2Mat floating point polygon 
 */

FloatPolygons Polygon::Result()
{
  ClipperLib::Clipper clipper;
  ClipperLib::PolyTree polytree;

  /* convert to PolyTree */
  clipper.AddPolygons(_subject, ClipperLib::ptSubject);
  clipper.Execute(ClipperLib::ctUnion, polytree, _filltype, _filltype);

  /* recursively descend polytree, converting nodes */
  FloatPolygons result;
  for (int i = 0; i < polytree.ChildCount(); ++i)
    _AddToPolygonList(*polytree.Childs[i], result, 0);

  return result;
}

void Polygon::_AddToPolygonList(ClipperLib::PolyNode& polynode, FloatPolygons& polygons, int level)
{
  FloatPoly outer_edge;
  
  outer_edge.poly = _convert(polynode.Contour);
  outer_edge.is_hole = !ClipperLib::Orientation(polynode.Contour);
  outer_edge.nesting_level = level;
  /* make outer edges of polygon clockwise; holes counterclockwise. XXX is this superfluous? */
  if (outer_edge.is_hole == IsClockwise(outer_edge.poly)) Reverse(outer_edge.poly);

  polygons.push_back(outer_edge);

  for (int i = 0; i < polynode.ChildCount(); ++i) {
    FloatPoly inner_edge;
    inner_edge.poly = _convert(polynode.Childs[i]->Contour);
    inner_edge.is_hole = !ClipperLib::Orientation(polynode.Childs[i]->Contour);
    inner_edge.nesting_level = level + 1;
    /* make outer edges of polygon clockwise; holes counterclockwise. XXX is this superfluous? */
    if (inner_edge.is_hole == IsClockwise(inner_edge.poly)) Reverse(inner_edge.poly);
    polygons.push_back(inner_edge);
    }

  /* Add outer polygons nested within holes */
  for (int i = 0; i < polynode.ChildCount(); ++i)
    for (int j = 0; j < polynode.Childs[i]->ChildCount(); ++j)
      _AddToPolygonList(*polynode.Childs[i]->Childs[j], polygons, level + 2);

  return; 
}

 /*
  * Convert edge back from ClipperLib to Hyp2Mat
  */

FloatPolygon Polygon::_convert (ClipperLib::Polygon clip_poly)
{
  FloatPolygon edge;

  for (ClipperLib::Polygon::iterator i = clip_poly.begin(); i != clip_poly.end(); ++i) 
    edge.push_back(_convert(*i));

  return edge;
}

 /*
  * Convert point back from ClipperLib to Hyp2Mat
  */

FloatPoint Polygon::_convert (ClipperLib::IntPoint p)
{
  return FloatPoint(p.X * _grid, p.Y * _grid);
}

/*
 * Calculate if a polygon is clockwise or not.
 */

bool Hyp2Mat::IsClockwise(FloatPolygon& poly) {

  /* calculate area of polygon */
  double area = 0;
  for (FloatPolygon::iterator i = poly.begin(); i != poly.end(); ++i) {
    /* i points to current element; j points to next element */
    FloatPolygon::iterator j = i;
    ++j;
    /* polygon is closed */
    if (j == poly.end()) j = poly.begin();
    area += i->x * j->y - j->x * i->y;
    }

  /* positive area is clockwise. Arbitrarily choose clockwise if area 0 */
  return area >= 0; 
  
}

/*
 *  Reverse order of vertices.
 */

void Hyp2Mat::Reverse(FloatPolygon& poly)
{
  std::reverse(poly.begin(), poly.end());
}


/* not truncated */
