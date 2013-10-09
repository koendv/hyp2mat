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

/* 
 * Uses the Clipper library from Angus Johnson to do polygon arithmetic
 */

#include <iostream>
#include <cstdio>
#include <cmath>
#include "hypfile.h"

using namespace std;
using namespace HypFile;

/*
 * Polygon constructor
 */

Polygon::Polygon()
{
  vertex.clear();
  polygon_type = POLYGON_TYPE_COPPER;
  id = -1;
  positive = true; /* is a polygon, not a hole */
  width = 0.0;
  plane_separation = -1.0; /* negative if not set */
  left_plane_separation = -1.0; /* negative if not set */
  net_name.clear();
  layer_name.clear();
}

/*
 * Draw an arc from (x1, y1) to (x2, y2) with center (xc, yc) and radius r using a polygonal approximation.
 * Arc may be clockwise or counterclockwise. 
 */

Polygon HypFile::Hyp::arc2poly(double x1, double y1, double x2, double y2, double xc, double yc, double r, bool clockwise)
{

  Polygon arc_segment;
  arc_segment.width = 0;
  arc_segment.positive = true;
  arc_segment.layer_name = "";

  int poly_points = 0;

  double alpha = atan2(y1-yc, x1-xc);
  double beta = atan2(y2-yc, x2-xc);


  if (clockwise) {
    // draw arc clockwise from (x1,y1) to (x2,y2)
    if (alpha < beta)
      alpha = alpha + 2 * pi;
    }
  else {
    // draw arc counterclockwise from (x1,y1) to (x2,y2)
    if (beta < alpha)
      beta = beta + 2 * pi;
    // draw a circle if starting and end points are the same
    if ((x1 == x2) && (y1 == y2)) 
      beta = alpha + 2 * pi;
    }

  // Calculate number of segments needed for a full circle.
  int segments = min_circle_segments;

  if (arc_precision > 0) {
    // Increase number of segments until difference between circular arc 
    // and polygonal approximation is less than max_arc_error

    double arc_error;
    do { 
      arc_error = r * (1 - cos(pi/segments));
      if (arc_error > arc_precision) segments += 4;
      }
    while (arc_error > arc_precision);

    }
  else if (arc_precision < 0)
    error("error: negative arc precision");

  // A full circle is drawn using 'segments' segments; a 90 degree arc using segments/4.
  poly_points = round(segments*abs(beta-alpha)/(2*pi));
 
  // Sanity checks
  if (poly_points < 1) poly_points = 1;
  
  // first point
  arc_segment.vertex.push_back(Point(x1, y1));

  // intermediate points
  for (int i = 1; i < poly_points ; i++) {
    double angle = alpha + (beta - alpha) * i / poly_points;
    double x = xc + r * cos(angle);     
    double y = yc + r * sin(angle);     
    arc_segment.vertex.push_back(Point(x, y));
    }

  // last point
  arc_segment.vertex.push_back(Point(x2, y2));

  return arc_segment;
}

/*
 * Draws straight metal trace segment as a polygon.
 */

Polygon HypFile::Hyp::segment2poly(double x1, double y1, double x2, double y2, double width)
{
/*
 * Note a hyperlynx line segment is drawn, ending in two half-circles:
 *              -------------------------------         ^
 *             /                               \        |
 *            |                                 |     width
 *             \                               /        |
 *              -------------------------------         V
 *
 *              <----------- length ---------->
 */

  Polygon line_segment; 
  line_segment.width = 0;
  line_segment.positive = true;
  line_segment.layer_name = "";

  /* Sanity check */
  double dx;
  double dy;
  if (( x1 == x2) && (y2 == y1)) {
    dx = width/2;
    dy = 0;
    }
  else {
    double length = sqrt((x2-x1)*(x2-x1)+(y2-y1)*(y2-y1));
    dx = width / 2 * (y2 - y1) / length;
    dy = width / 2 * (x2 - x1) / length;
    }

  Polygon arc2 = arc2poly(x2 + dx, y2 - dy, x2 - dx, y2 + dy, x2, y2, width/2, false);
  Polygon arc1 = arc2poly(x1 - dx, y1 + dy, x1 + dx, y1 - dy, x1, y1, width/2, false);

  line_segment.vertex = arc2.vertex;
  line_segment.vertex.insert(line_segment.vertex.end(), arc1.vertex.begin(), arc1.vertex.end());

  return line_segment;
}

/*
 * Convert a pad to a polygon
 */

Polygon HypFile::Hyp::pad2poly(double pad_x, double pad_y, Pad pad)
{
  Polygon pad_poly;
  pad_poly.width = 0;
  pad_poly.layer_name = pad.layer_name;
  pad_poly.positive = true;
  if (pad.pad_type == PAD_TYPE_ANTIPAD) pad_poly.polygon_type = POLYGON_TYPE_ANTIPAD;
  else pad_poly.polygon_type = POLYGON_TYPE_PAD;

  double sx = pad.pad_sx;
  double sy = pad.pad_sy;
  double angle = pad.pad_angle;

  if (fmod(angle, 180) == 0) angle = 0;
  else if (fmod(angle, 90) == 0) {
    double t;
    t = sx;
    sx = sy;
    sy = t;
    angle = 0;
    }

  /* 
   * Circular and oval pads
   */

  if (pad.pad_shape == PAD_SHAPE_OVAL) {
    /* calculate number of segments needed */
    int segments = min_circle_segments;
    if (arc_precision > 0) {
      /* Increase number of segments until difference between oval
         and polygonal approximation is less than max_arc_error */
      double arc_error_x;
      double arc_error_y;
      do {
        arc_error_x = sx / 2 * (1 - cos(pi/segments));
        arc_error_y = sy / 2 * (1 - cos(pi/segments));
        if ((arc_error_x > arc_precision) || (arc_error_y > arc_precision)) segments += 4;
        }
      while ((arc_error_x > arc_precision) || (arc_error_y > arc_precision));
      }
    /* calculate points */
    for (int i = 0; i < segments; i++) {
      double alpha = 2 * pi * i / segments;
      double x = sx / 2 / cos(pi / segments) * cos(alpha);
      double y = sy / 2 / cos(pi / segments) * sin(alpha);
      pad_poly.vertex.push_back(Point(x, y));
      }
    }

  /* 
   * Square and rectangular pads
   */

  else if (pad.pad_shape == PAD_SHAPE_RECTANGULAR) {
    pad_poly.vertex.push_back(Point( sx/2,  sy/2));
    pad_poly.vertex.push_back(Point( sx/2, -sy/2));
    pad_poly.vertex.push_back(Point(-sx/2, -sy/2));
    pad_poly.vertex.push_back(Point(-sx/2,  sy/2));
    }

  /* 
   * Oblong pads
   */

  else if (pad.pad_shape == PAD_SHAPE_OBLONG) {
    if (sx > sy) {
      /* horizontal oblong pad */
      double x1 = (sx - sy)/2;
      double y1 = -sy/2;
      double x2 = x1;
      double y2 = -y1;
      double xc = (sx - sy)/2;
      double yc = 0;
      double r  = sy/2;
      Polygon arc_right = arc2poly(x1, y1, x2, y2, xc, yc, r, false);
      Polygon arc_left  = arc2poly(-x1, -y1, -x2, -y2, -xc, -yc, r, false);
      pad_poly.vertex = arc_right.vertex;
      pad_poly.vertex.insert(pad_poly.vertex.end(), arc_left.vertex.begin(), arc_left.vertex.end());
      }
    else {
      /* vertical oblong pad */
      double x1 = sx/2;
      double y1 = (sy - sx)/2;
      double x2 = -x1;
      double y2 = y1;
      double xc = 0;
      double yc = (sy-sx)/2;
      double r  = sx/2;
      Polygon arc_top     = arc2poly(x1, y1, x2, y2, xc, yc, r, false);
      Polygon arc_bottom  = arc2poly(-x1, -y1, -x2, -y2, -xc, -yc, r, false);
      pad_poly.vertex = arc_top.vertex;
      pad_poly.vertex.insert(pad_poly.vertex.end(), arc_bottom.vertex.begin(), arc_bottom.vertex.end());
      }
    }

  /*
   * Unknown pad shape 
   */

  else error("unknown pad shape");

  /* Rotate polygon over pad rotation angle */
  if (angle != 0) {
    double cosa = cos(angle * pi / 180);
    double sina = sin(angle * pi / 180);
    for (PointList::iterator i = pad_poly.vertex.begin(); i != pad_poly.vertex.end(); ++i) {
      double x_rot = cosa * i->x - sina * i->y ; 
      double y_rot = sina * i->x + cosa * i->y ; 
      i->x = x_rot;
      i->y = y_rot;
      }
    }

  /* Translate polygon to pad center */
  for (PointList::iterator i = pad_poly.vertex.begin(); i != pad_poly.vertex.end(); ++i) {
    i->x += pad_x;
    i->y += pad_y;
    }

  /* return pad converted to polygon */
  return pad_poly;
}

/* not truncated */
