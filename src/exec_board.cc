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
#include <cstdio>
#include <cmath>
#include "hypfile.h"

using namespace std;
using namespace HypFile;

/* 
 * Hyperlynx 'BOARD_FILE' section. 
 * Hyperlynx file header.
 */

bool HypFile::Hyp::exec_board_file(parse_param& h)
{
  if (trace_hyp) cerr << "board_file" << endl;

  return false;
}

/*
 * Hyperlynx 'VERSION' record.
 * Specifies version number.
 * Required record; must be first record of the file. 
 */

bool HypFile::Hyp::exec_version(parse_param& h)
{
  if (trace_hyp) cerr << "version: vers = " << h.vers << endl;

  if (h.vers < 2.0) error("warning: version 1.x deprecated");

  return false;
}

/*
 * Hyperlynx 'DATA_MODE' record.
 * Specifies whether data is sufficient for power simulation.
 */

bool HypFile::Hyp::exec_data_mode(parse_param& h)
{
  if (trace_hyp) cerr << "data_mode: detailed = " << h.detailed << endl;

  return false;
}

/*
 * Hyperlynx 'UNITS' record.
 * Specifies measurement system (english/metric) for the rest of the file.
 */

bool HypFile::Hyp::exec_units(parse_param& h)
{
  if (trace_hyp) cerr << "units: unit_system_english = " << h.unit_system_english << " metal_thickness_weight = " << h.metal_thickness_weight << endl;
  // convert everything to meter

  if (h.unit_system_english) {
    unit = inches;                                // lengths in inches. 1 in = 2.54 cm = 0.0254 m
    if (h.metal_thickness_weight)
      metal_thickness_unit = copper_imperial_weight * unit;        // metal thickness in ounces/ft2. 1 oz/ft2 copper = 1.341 mil
    else
      metal_thickness_unit = unit;                // metal thickness in inches
    }
  else {
    unit = 0.01;                                  // lengths in centimeters. 1 cm = 0.01 m
    if (h.metal_thickness_weight)
      metal_thickness_unit = copper_metric_weight * unit;       // metal thickness in grams/cm2. 1 gr/cm2 copper = 0.1116 cm
    else
      metal_thickness_unit = unit;                // metal thickness in centimeters
    }

  return false;
}

/*
 * Hyperlynx 'PLANE_SEP' record.
 * Defines default trace to plane separation
 */

bool HypFile::Hyp::exec_plane_sep(parse_param& h)
{
  if (trace_hyp) cerr << "plane_sep: default_plane_separation = " << h.default_plane_separation << endl;

  board.plane_separation = h.default_plane_separation * unit;

  return false;
}

/*
 * Hyperlynx 'PERIMETER_SEGMENT' subrecord of 'BOARD' record. 
 * Draws linear board outline segment.
 */

bool HypFile::Hyp::exec_perimeter_segment(parse_param& h)
{
  if (trace_hyp) cerr << "perimeter_segment: x1 = " << h.x1 << " y1 = " << h.y1 << " x2 = " << h.x2 << " y2 = " << h.y2 << endl;

  Polygon poly;

  h.x1 *= unit;
  h.y1 *= unit;
  h.x2 *= unit;
  h.y2 *= unit;

  poly.width = 0;
  poly.positive = true;
  poly.vertex.push_back(Point(h.x1, h.y1));
  poly.vertex.push_back(Point(h.x2, h.y2));

  // Add segment to board outline
  add_perimeter_polygon(poly);

  return false;
}

/*
 * Hyperlynx 'PERIMETER_ARC' subrecord of 'BOARD' record. 
 * Draws arc segment of board outline.
 */

bool HypFile::Hyp::exec_perimeter_arc(parse_param& h)
{
  if (trace_hyp) cerr << "perimeter_arc: x1 = " << h.x1 << " y1 = " << h.y1 << " x2 = " << h.x2 << " y2 = " << h.y2 << " xc = " << h.xc << " yc = " << h.yc << " r = " << h.r << endl;

  Polygon poly;

  h.x1 *= unit;
  h.y1 *= unit;
  h.x2 *= unit;
  h.y2 *= unit;
  h.xc *= unit;
  h.yc *= unit;
  h.r *= unit;

  poly = arc2poly(h.x1, h.y1, h.x2, h.y2, h.xc, h.yc, h.r, false); // 'PERIMETER_ARC' draws arc counterclockwise

  // Add arc to board outline
  add_perimeter_polygon(poly); 

  return false;
}

/*
 * Hyperlynx 'A' attribute subrecord of 'BOARD' record.
 * Defines board attributes as name/value pairs.
 */

bool HypFile::Hyp::exec_board_attribute(parse_param& h)
{
  if (trace_hyp) cerr << "board_attribute: name = " << h.name  << " value = " << h.value << endl;

  Attribute a;
  a.name = h.name;
  a.value = h.value;

  board.attribute.push_back(a);

  return false;
}

/*
 * Add a polygon to the board outline
 */

void HypFile::Hyp::add_perimeter_polygon(Polygon new_segment)
{
  // Note first polygon of board perimeter is the board outline, and positive. 
  // Subsequent polygons are board cutouts, and negative.

  // safety check
  if (new_segment.vertex.empty()) 
    return;

  if (new_segment.vertex.size() == 1) {
    error("runt perimeter segment"); 
    return;
    }

  // if new segment is closed, append new segment as perimeter polygon.
  if (new_segment.vertex.front() == new_segment.vertex.back()) {
    new_segment.positive = false;
    board.edge.push_back(new_segment);
    // only first polygon of board perimeter is positive, rest are holes.
    board.edge.front().positive = true;
    return;
    }

  // loop over list of perimeter polygons, see if new segment is good match
  for (PolygonList::iterator i = board.edge.begin(); i != board.edge.end(); ++i) {

    // perimeter polygon is closed. 
    // don't append.
    if (i->vertex.front() == i->vertex.back()) 
      continue;

    // first point of new segment is last point of perimeter polygon. 
    // append new segment to perimeter polygon
    if (i->vertex.back() == new_segment.vertex.front()) {
      i->vertex.insert(i->vertex.end(), ++new_segment.vertex.begin(), new_segment.vertex.end());
      return;
      }

    // last point of new segment is last point of perimeter polygon. 
    // append new segment back-to-front to perimeter polygon
    if (i->vertex.back() == new_segment.vertex.back()) {
      // append new segment to back of last polygon
      i->vertex.insert(i->vertex.end(), ++new_segment.vertex.rbegin(), new_segment.vertex.rend());
      return;
      }

    // first point of new segment is first point of perimeter polygon. 
    // insert new segment at beginning of perimeter polygon
    if (i->vertex.front() == new_segment.vertex.front()) {
      i->vertex.insert(i->vertex.begin(), new_segment.vertex.rbegin(), --new_segment.vertex.rend());
      return;
      }

    // last point of new segment is first point of perimeter polygon. 
    // insert new segment back-to-front at beginning of perimeter polygon
    if (i->vertex.front() == new_segment.vertex.back()) {
      // append new segment to back of last polygon
      i->vertex.insert(i->vertex.begin(), new_segment.vertex.begin(), --new_segment.vertex.end());
      return;
      }

    }

  // no match found. append new segment as new perimeter polygon.
  new_segment.positive = false;
  board.edge.push_back(new_segment);
  // only first polygon of board perimeter is positive, rest are holes.
  board.edge.front().positive = true; 
  return;

}

/* not truncated */
