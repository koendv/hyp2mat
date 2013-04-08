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

#include "geometry.h"

Geometry::Geometry()
{
  grid = 1E-6;
}

void Geometry::Simplify(Hyp2Mat::PCB& pcb, double grid_size, double arc_precision, Hyp2Mat::Bounds bounds)
{
  if (grid_size > 0) grid = grid_size;
  else grid = 1E-6;

  if (arc_precision > 0) limit = arc_precision / grid_size;
  else limit = 0;

  /* Crop copper */
   
  ClipperLib::Polygons clipper_board = convert(pcb.board);

  for (Hyp2Mat::LayerList::iterator l = pcb.stackup.begin(); l != pcb.stackup.end(); ++l) {
    if (l->layer_type == Hyp2Mat::LAYER_DIELECTRIC) continue;

    ClipperLib::Polygons metal_layer = convert(l->metal);
    
    /* invert plane layers */
    if (l->layer_type == Hyp2Mat::LAYER_PLANE) metal_layer = invert(metal_layer, clipper_board);

    /* remove any copper outside bounds */
    l->metal = convert(crop(metal_layer, bounds.x_min, bounds.x_max, bounds.y_min, bounds.y_max));
    }

  /* crop board */
  pcb.board = convert(crop(convert(pcb.board), bounds.x_min, bounds.x_max, bounds.y_min, bounds.y_max));

  /* remove vias outside bounds */
  CropVias(pcb, bounds);

  /* remove layers above or below bounds */
  CropLayers(pcb, bounds);

  /* align pins with grid */
  SnapPinsToGrid(pcb);

  return;
}

/* 
 * Convert from Hyp2Mat to Clipper 
 */

ClipperLib::IntPoint Geometry::convert (Hyp2Mat::Point p)
{
   return ClipperLib::IntPoint(floor(p.x / grid + 0.5), floor(p.y / grid + 0.5));
}

ClipperLib::Polygons Geometry::convert (Hyp2Mat::Edge edge)
{

  /* convert from Hyp2Mat floating point polygon to Clipper long64 polygon */
  ClipperLib::Polygon poly;
  for (Hyp2Mat::PointList::iterator i = edge.vertex.begin(); i != edge.vertex.end(); ++i)
    poly.push_back(convert(*i));

  /* remove repeated vertices */
  poly = unique(poly);
  
  /* Convert to non-self-intersecting polygons */
  ClipperLib::Polygons polys;
  ClipperLib::SimplifyPolygon(poly, polys, ClipperLib::pftEvenOdd);

  /* Set polygons to positive orientation */
  for (ClipperLib::Polygons::iterator i = polys.begin(); i != polys.end(); ++i) {
    if (!Orientation(*i)) ClipperLib::ReversePolygon(*i);
    /* remove repeated vertices */
    *i = unique(*i); 
    }

  /* Offset by half line width */
  double delta = edge.width / 2 / grid;
  if (edge.is_hole) delta = -delta;
  ClipperLib::Polygons offset_polys;

  if (limit == 0)
    /* square corners */
    OffsetPolygons(polys, offset_polys, delta, ClipperLib::jtSquare);
  else
    /* round corners */
    OffsetPolygons(polys, offset_polys, delta, ClipperLib::jtRound, limit);

  /* Set polygons to same orientation as initial*/
  for (ClipperLib::Polygons::iterator i = offset_polys.begin(); i != offset_polys.end(); ++i)
    if (Orientation(*i) == edge.is_hole) ClipperLib::ReversePolygon(*i);

  return offset_polys;
}

ClipperLib::Polygons Geometry::convert (Hyp2Mat::Polygon poly)
{

  ClipperLib::Polygons result;
  ClipperLib::Clipper clipper;

  for (Hyp2Mat::Polygon::iterator i = poly.begin(); i != poly.end(); ++i)
    clipper.AddPolygons(convert(*i), ClipperLib::ptSubject);

  clipper.Execute(ClipperLib::ctUnion, result);
  return result;

}

ClipperLib::Polygons Geometry::convert (Hyp2Mat::PolygonList plist)
{
  ClipperLib::Polygons result;
  ClipperLib::Clipper clipper;

  /*join polygons */
  for (Hyp2Mat::PolygonList::iterator i = plist.begin(); i != plist.end(); ++i) {
    clipper.AddPolygons(convert(*i), ClipperLib::ptClip);
    }

  clipper.Execute(ClipperLib::ctUnion, result, ClipperLib::pftNonZero, ClipperLib::pftNonZero);
  return result;
}

/*
 * Remove repeated vertices from a polygon
 */

ClipperLib::Polygon Geometry::unique(ClipperLib::Polygon p) {
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
 * Crop 
 */

ClipperLib::Polygons Geometry::crop (ClipperLib::Polygons polys, double x_min, double x_max, double y_min, double y_max)
{
  ClipperLib::Clipper clipper;
  ClipperLib::Polygons result;
  ClipperLib::Polygon bounding_box;
  ClipperLib::Polygons subject;

  bounding_box.push_back(convert(Hyp2Mat::Point(x_min, y_min)));
  bounding_box.push_back(convert(Hyp2Mat::Point(x_max, y_min)));
  bounding_box.push_back(convert(Hyp2Mat::Point(x_max, y_max)));
  bounding_box.push_back(convert(Hyp2Mat::Point(x_min, y_max)));

  subject.push_back(bounding_box);

  clipper.AddPolygons(subject, ClipperLib::ptSubject);
  clipper.AddPolygons(polys, ClipperLib::ptClip);
  
  clipper.Execute(ClipperLib::ctIntersection, result, ClipperLib::pftNonZero, ClipperLib::pftNonZero);

  return result;
}

ClipperLib::Polygons Geometry::invert (ClipperLib::Polygons poly, ClipperLib::Polygons board)
{
  ClipperLib::Clipper clipper;
  ClipperLib::Polygons result;

  clipper.AddPolygons(poly, ClipperLib::ptClip);
  clipper.AddPolygons(board, ClipperLib::ptSubject);
  clipper.Execute(ClipperLib::ctXor, result);
  return result;
}

Hyp2Mat::PolygonList Geometry::convert (ClipperLib::Polygons clip_polys)
{
  ClipperLib::Clipper clipper;
  ClipperLib::PolyTree polytree;

  ClipperLib::Polygons polys;

  /* convert to PolyTree */
  clipper.AddPolygons(clip_polys, ClipperLib::ptSubject);
  clipper.Execute(ClipperLib::ctUnion, polytree, ClipperLib::pftNonZero, ClipperLib::pftNonZero);

  /* recursively descend polytree, converting nodes */
  Hyp2Mat::PolygonList poly_list;
  for (int i = 0; i < polytree.ChildCount(); ++i)
    AddToPolyList(*polytree.Childs[i], poly_list, 0);

  return poly_list;
}

void Geometry::AddToPolyList(ClipperLib::PolyNode& polynode, Hyp2Mat::PolygonList& poly_list, int level)
{
  Hyp2Mat::Polygon poly;
  Hyp2Mat::Edge outer_edge;
  
  outer_edge = convert(polynode.Contour);
  outer_edge.nesting_level = level;
  poly.push_back(outer_edge);

  for (int i = 0; i < polynode.ChildCount(); ++i) {
    Hyp2Mat::Edge inner_edge;
    inner_edge = convert(polynode.Childs[i]->Contour);
    inner_edge.nesting_level = level + 1;
    poly.push_back(inner_edge);
    }

  poly_list.push_back(poly);
  
  /* Add outer polygons nested within holes */
  for (int i = 0; i < polynode.ChildCount(); ++i)
    for (int j = 0; j < polynode.Childs[i]->ChildCount(); ++j)
      AddToPolyList(*polynode.Childs[i]->Childs[j], poly_list, level + 2);

  return; 
}

 /*
  * Convert edge back from ClipperLib to Hyp2Mat
  */

Hyp2Mat::Point Geometry::convert (ClipperLib::IntPoint p)
{
  return Hyp2Mat::Point(p.X * grid, p.Y * grid);
}

Hyp2Mat::Edge Geometry::convert (ClipperLib::Polygon clip_poly)
{
  Hyp2Mat::Edge edge;

  for (ClipperLib::Polygon::iterator i = clip_poly.begin(); i != clip_poly.end(); ++i) 
    edge.vertex.push_back(convert(*i));
  edge.width = 0;
  edge.is_hole = !Orientation(clip_poly);
  if (edge.is_clockwise() != Orientation(clip_poly)) edge.reverse();

  return edge;
}

/*
 * Remove all vias which are out of bounds.
 */

void Geometry::CropVias(Hyp2Mat::PCB& pcb, Hyp2Mat::Bounds bounds)
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

void Geometry::CropLayers(Hyp2Mat::PCB& pcb, Hyp2Mat::Bounds bounds)
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

/*
 * Snap pins to grid
 */

void Geometry::SnapPinsToGrid(Hyp2Mat::PCB& pcb)
{
  for (Hyp2Mat::PinList::iterator i = pcb.pin.begin(); i != pcb.pin.end(); ++i)
    for (Hyp2Mat::PointList::iterator j = i->metal.vertex.begin(); j != i->metal.vertex.end(); ++j)
      *j = convert(convert(*j));
  return;
}

/*
 * for debugging - prints polygons
 */

void PrintPcb(Hyp2Mat::PCB& pcb) {
  std::cerr << "pcb"<< std::endl;
  for (Hyp2Mat::LayerList::iterator l = pcb.stackup.begin(); l != pcb.stackup.end(); ++l) {
    std::cerr << " layer " << l->layer_name << std::endl;
    for (Hyp2Mat::PolygonList::iterator i = l->metal.begin(); i != l->metal.end(); ++i) {
      std::cerr << "  polygon" << std::endl;
      for (Hyp2Mat::Polygon::iterator j = i->begin(); j != i->end(); ++j) {
        std::cerr << "   edge"<< std::endl;
        std::cerr << "    is_hole: " << j->is_hole << std::endl;
        std::cerr << "    width: " << j->width << std::endl;
        for (Hyp2Mat::PointList::iterator k = j->vertex.begin(); k != j->vertex.end(); ++k)
          std::cerr << "    x, y: " << k->x << " " << k->y << std::endl;
        }
      }
    }
  }

/* not truncated */
