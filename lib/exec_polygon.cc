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
#include "hypfile.h"

using namespace std;
using namespace HypFile;

/*
 * Hyperlynx 'POLYGON' subrecord of 'NET' record. 
 * Draws polygonal metal area.
 */

bool HypFile::Hyp::exec_polygon_begin(parse_param& h)
{
  if (trace_hyp) {
    cerr << "polygon begin:";
    if (h.layer_name_set) cerr << " layer_name = " << h.layer_name;
    if (h.width_set) cerr << " width = " << h.width;
    if (h.polygon_type_set) { 
      cerr << " polygon_type = " << h.polygon_type << " ";
      switch (h.polygon_type) {
        case POLYGON_TYPE_PLANE:     cerr << "POLYGON_TYPE_PLANE"; break;
        case POLYGON_TYPE_POUR:      cerr << "POLYGON_TYPE_POUR"; break;
        case POLYGON_TYPE_COPPER:    cerr << "POLYGON_TYPE_COPPER"; break;
        default:                     cerr << "Error"; break;
        }
      }
    if (h.id_set) cerr << " id = " << h.id;
    cerr << " x = " << h.x << " y = " << h.y << endl;
    }

  if (!h.layer_name_set) {
    error("expected polygon layer L = ");
    return true;
    }

  if (!h.width_set) h.width = 0;

  if (!h.polygon_type_set) h.polygon_type = POLYGON_TYPE_PLANE;

  if (!h.id_set) {
    error("expected polygon id ID = ");
    return true;
    }

  h.width *= unit;
  h.x *= unit;
  h.y *= unit;

  current_polygon.id = h.id;
  current_polygon.polygon_type = h.polygon_type;
  current_polygon.width = h.width;
  current_polygon.positive = true;
  current_polygon.layer_name = h.layer_name;
  current_polygon.vertex.clear();
  current_polygon.vertex.push_back(Point(h.x, h.y));

  return false;
}

bool HypFile::Hyp::exec_polygon_end(parse_param& h)
{
  if (trace_hyp) cerr << "polygon end" << endl;

  add_polygon(current_polygon);

  return false;
}

/*
 * Hyperlynx 'POLYVOID' subrecord of 'NET' record. 
 * Creates polygonal hole in metal area.
 */

bool HypFile::Hyp::exec_polyvoid_begin(parse_param& h)
{
  if (trace_hyp) {
    cerr << "polyvoid begin:";
    if (h.id_set) cerr << " id = " << h.id;
    cerr << " x = " << h.x << " y = " << h.y << endl;
    }

  if (!h.id_set) {
    error("expected polygon id ID = ");
    return true;
    }

  h.x *= unit;
  h.y *= unit;

  current_polygon.id = h.id;
  current_polygon.positive = false;
  current_polygon.vertex.clear();
  current_polygon.vertex.push_back(Point(h.x, h.y));

  /* inherit layer_name and width from parent */
  for (PolygonList::iterator i = net.back().metal[h.id].begin(); i != net.back().metal[h.id].end(); ++i) {
    if (i->positive) { 
      current_polygon.polygon_type = i->polygon_type;
      current_polygon.width = i->width;
      current_polygon.layer_name = i->layer_name;
      return false;
      }
    }

  /* no positive polygon/polyline found with same id */
  error("polyvoid does not have parent polygon or polyline with same id");
  return true;
}

bool HypFile::Hyp::exec_polyvoid_end(parse_param& h)
{
  if (trace_hyp) cerr << "polyvoid end" << endl;

  add_polygon(current_polygon);

  return false;
}

/*
 * Hyperlynx 'POLYLINE' subrecord of 'NET' record. 
 * Draws metal trace.
 */

bool HypFile::Hyp::exec_polyline_begin(parse_param& h)
{
  if (trace_hyp) {
    cerr << "polyline begin:";
    if (h.layer_name_set) cerr << " layer_name = " << h.layer_name;
    if (h.width_set) cerr << " width = " << h.width;
    if (h.polygon_type_set) { 
      cerr << " polygon_type = " << h.polygon_type << " ";
      switch (h.polygon_type) {
        case POLYGON_TYPE_PLANE:     cerr << "POLYGON_TYPE_PLANE"; break;
        case POLYGON_TYPE_POUR:      cerr << "POLYGON_TYPE_POUR"; break;
        case POLYGON_TYPE_COPPER:    cerr << "POLYGON_TYPE_COPPER"; break;
        default:                     cerr << "Error"; break;
        }
      }
    if (h.id_set) cerr << " id = " << h.id;
    cerr << " x = " << h.x << " y = " << h.y << endl;
    }

  if (!h.layer_name_set) {
    error("expected polygon layer L = ");
    return true;
    }

  if (!h.width_set) {
    error("expected polygon width W = ");
    return true;
    }

  if (!h.polygon_type_set) h.polygon_type = POLYGON_TYPE_PLANE;

  if (!h.id_set) {
    error("expected polygon id ID = ");
    return true;
    }

  h.width *= unit;
  h.x *= unit;
  h.y *= unit;

  current_polygon.id = h.id;
  current_polygon.polygon_type = h.polygon_type;
  current_polygon.width = h.width;
  current_polygon.positive = true;
  current_polygon.layer_name = h.layer_name;
  current_polygon.vertex.clear();
  current_polygon.vertex.push_back(Point(h.x, h.y));

  return false;
}

bool HypFile::Hyp::exec_polyline_end(parse_param& h)
{
  if (trace_hyp) cerr << "polyline end" << endl;

  // Draw polyline as sequence of line segments
  for (PointList::iterator i = current_polygon.vertex.begin(); (i != current_polygon.vertex.end()) && (i != --current_polygon.vertex.end()); ++i) {
    Polygon line_segment;
    PointList::iterator j = i + 1;
    line_segment = segment2poly(i->x, i->y, j->x, j->y, current_polygon.width);
    line_segment.id = current_polygon.id;
    line_segment.polygon_type = current_polygon.polygon_type;
    line_segment.positive = true;
    line_segment.layer_name = current_polygon.layer_name;
    line_segment.width = 0;
    add_polygon(line_segment);
    }

  return false;
}

/*
 * Hyperlynx 'LINE' subrecord of 'NET' record. 
 * Draws straight metal trace.
 */

bool HypFile::Hyp::exec_line(parse_param& h)
{
  if (trace_hyp) cerr << "line: x = " << h.x << " y = " << h.y << endl;

  h.x *= unit;
  h.y *= unit;

  /* add point to current polygon */
  current_polygon.vertex.push_back(Point(h.x, h.y));

  return false;
}

/*
 * Hyperlynx 'CURVE' subrecord of 'NET' record. 
 * Draws curved metal trace.
 */

bool HypFile::Hyp::exec_curve(parse_param& h)
{
  if (trace_hyp) {
    cerr << "curve: x1 = " << h.x1 << " y1 = " << h.y1 << " x2 = " << h.x2 << " y2 = " << h.y2;
    cerr << " xc = " << h.xc << " yc = " << h.yc << " r = " << h.r << endl;
    }

  h.x1 *= unit;
  h.y1 *= unit;
  h.x2 *= unit;
  h.y2 *= unit;
  h.xc *= unit;
  h.yc *= unit;
  h.r *= unit;
  h.width *= unit;

  /* calculate arc */
  /* 'CURVE' draws arc counterclockwise */
  Polygon arc = arc2poly(h.x1, h.y1, h.x2, h.y2, h.xc, h.yc, h.r, false); 

  /* calculate distance between polygon end vertex and arc begin and arc end */
  double endvertex_x = current_polygon.vertex.back().x;
  double endvertex_y = current_polygon.vertex.back().y;
  double dist1 =  (endvertex_x - h.x1) * (endvertex_x - h.x1) + (endvertex_y - h.y1) * (endvertex_y - h.y1);
  double dist2 =  (endvertex_x - h.x2) * (endvertex_x - h.x2) + (endvertex_y - h.y2) * (endvertex_y - h.y2);

  /* add arc to current polygon. begin with point closest to end vertex. */
  if (dist1 <= dist2)
    /* add arc from (x1, y1) to (x2, y2) */
    current_polygon.vertex.insert(current_polygon.vertex.end(), arc.vertex.begin(), arc.vertex.end());
  else
    /* add arc from (x2, y2) to (x1, y1) */
    current_polygon.vertex.insert(current_polygon.vertex.end(), arc.vertex.rbegin(), arc.vertex.rend());

  return false;
}

/* not truncated */
