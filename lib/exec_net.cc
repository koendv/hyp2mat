
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
#include <cmath>
#include "hypfile.h"

using namespace std;
using namespace HypFile;

/*
 * Hyperlynx 'NET' record. 
 * Specifies a net.
 */

bool HypFile::Hyp::exec_net(parse_param& h)
{
  if (trace_hyp) cerr << "net: net_name = " << h.net_name << endl;

  Net n;
  n.net_name = h.net_name;
  n.plane_separation = -1.0;
  net.push_back(n);

  return false;
}

/*
 * Hyperlynx 'PS' subrecord of 'NET' record. 
 * Specifies plane separation of a net.
 */

bool HypFile::Hyp::exec_net_plane_separation(parse_param& h)
{
  if (trace_hyp) cerr << "net_plane_separation: plane_separation = " << h.plane_separation << endl;
  h.plane_separation *= unit;

  net.back().plane_separation = h.plane_separation;

  return false;
}

/*
 * Hyperlynx 'SEG' subrecord of 'NET' record. 
 * Draws straight metal trace segment.
 */

bool HypFile::Hyp::exec_seg(parse_param& h)
{
  if (trace_hyp) {
    cerr << "seg: x1 = " << h.x1 << " y1 = " << h.y1 << " x2 = " << h.x2 << " y2 = " << h.y2;
    cerr << " width = " << h.width << " layer_name = " << h.layer_name;
    if (h.plane_separation_set) cerr << " plane_separation = " << h.plane_separation; 
    if (h.left_plane_separation_set) cerr << " left_plane_separation = " << h.left_plane_separation; 
    cerr << endl;
    }

  Polygon p;

  h.x1 *= unit;
  h.y1 *= unit;
  h.x2 *= unit;
  h.y2 *= unit;
  h.width *= unit;
  h.plane_separation *= unit;
  if (!h.plane_separation_set) h.plane_separation = -1.0;
  h.left_plane_separation *= unit;
  if (!h.left_plane_separation_set) h.left_plane_separation = -1.0;

  p = segment2poly(h.x1 , h.y1, h.x2, h.y2, h.width);
  p.layer_name = h.layer_name;
  p.width = 0;
  p.positive = true;
  p.plane_separation = h.plane_separation; /* distance to other copper; -1 if not set */
  p.left_plane_separation = h.left_plane_separation;

  add_polygon(p);

  return false;
}

/*
 * Hyperlynx 'ARC' subrecord of 'NET' record. 
 * Draws arc metal trace segment.
 */

bool HypFile::Hyp::exec_arc(parse_param& h)
{
  if (trace_hyp) {
    cerr << "arc: x1 = " << h.x1 << " y1 = " << h.y1 << " x2 = " << h.x2 << " y2 = " << h.y2;
    cerr << " xc = " << h.xc << " yc = " << h.yc << " r = " << h.r;
    cerr << " width = " << h.width << " layer_name = " << h.layer_name;
    if (h.plane_separation_set) cerr << " plane_separation = " << h.plane_separation; 
    if (h.left_plane_separation_set) cerr << " left_plane_separation = " << h.left_plane_separation; 
    cerr << endl;
    }

  h.x1 *= unit;
  h.y1 *= unit;
  h.x2 *= unit;
  h.y2 *= unit;
  h.xc *= unit;
  h.yc *= unit;
  h.r *= unit;
  h.width *= unit;
  h.plane_separation *= unit;
  if (!h.plane_separation_set) h.plane_separation = -1.0;
  h.left_plane_separation *= unit;
  if (!h.left_plane_separation_set) h.left_plane_separation = -1.0;

  /* 'ARC' draws arc clockwise */
  Polygon arc = arc2poly(h.x1, h.y1, h.x2, h.y2, h.xc, h.yc, h.r, true);

  /* draw arc segments */
  for (PointList::iterator i = arc.vertex.begin(); (i != arc.vertex.end()) && (i != --arc.vertex.end()); ++i) {
    Polygon line_segment;
    PointList::iterator j = i + 1;
    line_segment = segment2poly(i->x, i->y, j->x, j->y, h.width);
    line_segment.positive = true;
    line_segment.layer_name = h.layer_name;
    line_segment.width = 0;
    line_segment.plane_separation = h.plane_separation;
    line_segment.left_plane_separation = h.left_plane_separation;
    add_polygon(line_segment);
    }

  return false;
}

/*
 * Hyperlynx 'VIA' subrecord of 'NET' record. 
 * Draws via as defined in padstack.
 */

bool HypFile::Hyp::exec_via(parse_param& h)
{
  /* detect old-style v1.0 via */
  if (!h.padstack_name_set)
    return exec_via_v1(h);

  if (trace_hyp) {
    cerr << "via: x = " << h.x << " y = " << h.y;
    if (h.layer1_name_set) cerr << " layer1_name = " << h.layer1_name; 
    if (h.layer2_name_set) cerr << " layer2_name = " << h.layer2_name; 
    cerr << " padstack_name = " << h.padstack_name;
    cerr << endl;
    }

  h.x *= unit;
  h.y *= unit;

  /* lookup padstack */
  for (PadstackList::iterator i = padstack.begin(); i != padstack.end(); ++i)
    if (i->padstack_name == h.padstack_name) {

      /* set via begin layer */
      if (!h.layer1_name_set) {
        if (!i->pads.empty())
          h.layer1_name = i->pads.front().layer_name; 
        else
          h.layer1_name = stackup.front().layer_name; /* top */
        }

      /* set via end layer */
      if (!h.layer2_name_set) {
        if (!i->pads.empty())
          h.layer2_name = i->pads.back().layer_name; 
        else
          h.layer2_name = stackup.back().layer_name; /* bottom */
        }

      /* add to list of via holes */
      add_via(h.x, h.y, h.layer2_name, h.layer1_name, i->drill_size/2);

      /* loop over padstack and generate pads */
      for (PadList::iterator j = i->pads.begin(); j != i->pads.end(); ++j)
        add_pad(h.x, h.y, *j);

      return false;
      }

  /* padstack not found */

  return false;
}

/*
 * Hyperlynx 'VIA' subrecord of 'NET' record. 
 * Draws deprecated v1.x via.
 */

bool HypFile::Hyp::exec_via_v1(parse_param& h)
{
  if (trace_hyp) {
    cerr << "via: x = " << h.x << " y = " << h.y;
    cerr << " drill_size = " << h.drill_size;
    if (h.layer1_name_set) cerr << " layer1_name = " << h.layer1_name; 
    if (h.layer2_name_set) cerr << " layer2_name = " << h.layer2_name; 
    cerr << " via_pad1_shape = " << h.via_pad1_shape << " via_pad1_sx = " << h.via_pad1_sx << " via_pad1_sy = " << h.via_pad1_sy << " via_pad1_angle = " << h.via_pad1_angle;
    cerr << " via_pad2_shape = " << h.via_pad2_shape << " via_pad2_sx = " << h.via_pad2_sx << " via_pad2_sy = " << h.via_pad2_sy << " via_pad2_angle = " << h.via_pad2_angle;
    cerr << endl;
    }

  h.drill_size *= unit;
  h.x *= unit;
  h.y *= unit;
  h.via_pad1_sx *= unit;
  h.via_pad1_sy *= unit;
  h.via_pad2_sx *= unit;
  h.via_pad2_sy *= unit;

  if (!h.layer1_name_set) h.layer1_name = stackup.front().layer_name; /* top */
  if (!h.layer2_name_set) h.layer2_name = stackup.back().layer_name; /* bottom */

  /* add to list of via holes */
  add_via(h.x, h.y, h.layer2_name, h.layer1_name, h.drill_size/2);

	if (!h.via_pad1_shape_set || !h.via_pad1_sx_set || !h.via_pad1_sy_set || !h.via_pad1_angle_set ||
	    !h.via_pad2_shape_set || !h.via_pad2_sx_set || !h.via_pad2_sy_set || !h.via_pad2_angle_set)
    return false;

  /* add top pad */
  Pad pad;
  pad.layer_name = h.layer1_name;
  if (h.via_pad1_shape == "OVAL") pad.pad_shape = PAD_SHAPE_OVAL;
  else if (h.via_pad1_shape == "RECT") pad.pad_shape = PAD_SHAPE_RECTANGULAR;
  else if (h.via_pad1_shape == "OBLONG") pad.pad_shape = PAD_SHAPE_OBLONG;
  else pad.pad_shape = PAD_SHAPE_OVAL;
  pad.pad_type = PAD_TYPE_METAL;
  pad.pad_sx = h.via_pad1_sx;
  pad.pad_sy = h.via_pad1_sy;
  pad.pad_angle = h.via_pad1_angle;
  pad.thermal_clear_shape = PAD_SHAPE_OVAL;
  pad.thermal_clear_sx = 0;
  pad.thermal_clear_sy= 0;
  pad.thermal_clear_angle= 0;
  add_pad(h.x, h.y, pad);

  /* add bottom pad */
  pad.layer_name = h.layer2_name;
  if (h.via_pad2_shape == "OVAL") pad.pad_shape = PAD_SHAPE_OVAL;
  else if (h.via_pad2_shape == "RECT") pad.pad_shape = PAD_SHAPE_RECTANGULAR;
  else if (h.via_pad2_shape == "OBLONG") pad.pad_shape = PAD_SHAPE_OBLONG;
  else pad.pad_shape = PAD_SHAPE_OVAL;
  pad.pad_type = PAD_TYPE_METAL;
  pad.pad_sx = h.via_pad2_sx;
  pad.pad_sy = h.via_pad2_sy;
  pad.pad_angle = h.via_pad2_angle;
  add_pad(h.x, h.y, pad);

  return false;
}

/*
 * Hyperlynx 'PIN' subrecord of 'NET' record. 
 * Draws PIN as defined in padstack.
 */

bool HypFile::Hyp::exec_pin(parse_param& h)
{
  if (trace_hyp) {
    cerr << "pin: x = " << h.x << " y = " << h.y;
    cerr << " pin_reference = " << h.pin_reference;
    cerr << " padstack_name = " << h.padstack_name;
    if (h.pin_function_set) cerr << " pin_function = " << h.pin_function;
    cerr << endl;
    }

  h.x *= unit;
  h.y *= unit;

  /* add to list of pins */
  Pin p;
  p.x = h.x;
  p.y = h.y;
  p.pin_reference = h.pin_reference;
  p.padstack_name = h.padstack_name;
  p.pin_function = PIN_SIM_BOTH;
  if (h.pin_function_set) p.pin_function = h.pin_function;
  net.back().pin.push_back(p);

  /* lookup padstack */
  if (h.padstack_name_set)
    for (PadstackList::iterator i = padstack.begin(); i != padstack.end(); ++i)
      if (i->padstack_name == h.padstack_name) {
        /* add to list of via holes */
        add_via(h.x, h.y, stackup.back().layer_name /* bottom */, stackup.front().layer_name /* top */, i->drill_size/2);
  
        /* loop over padstack and generate pads */
        for (PadList::iterator j = i->pads.begin(); j != i->pads.end(); ++j)
          add_pad(h.x, h.y, *j);
        return false;
        }

  /* padstack not found or not set */

  return false;
}

/*
 * Hyperlynx 'PAD' subrecord of 'NET' record. 
 * Draws deprecated v1.x pad.
 */

bool HypFile::Hyp::exec_pad(parse_param& h)
{
  if (trace_hyp) {
    cerr << "pad: x = " << h.x << " y = " << h.y;
    if (h.layer_name_set) cerr << " layer_name = " << h.layer_name; 
    cerr << " via_pad1_shape = " << h.via_pad1_shape << " via_pad1_sx = " << h.via_pad1_sx << " via_pad1_sy = " << h.via_pad1_sy << " via_pad1_angle = " << h.via_pad1_angle;
    cerr << endl;
    }

  h.x *= unit;
  h.y *= unit;
  h.via_pad1_sx *= unit;
  h.via_pad1_sy *= unit;

  /* add pad */
  Pad pad;
  pad.layer_name = h.layer_name;
  if (h.via_pad1_shape == "OVAL") pad.pad_shape = PAD_SHAPE_OVAL;
  else if (h.via_pad1_shape == "RECT") pad.pad_shape = PAD_SHAPE_RECTANGULAR;
  else if (h.via_pad1_shape == "OBLONG") pad.pad_shape = PAD_SHAPE_OBLONG;
  else pad.pad_shape = PAD_SHAPE_OVAL;
  pad.pad_type = PAD_TYPE_METAL;
  pad.pad_sx = h.via_pad1_sx;
  pad.pad_sy = h.via_pad1_sy;
  pad.pad_angle = h.via_pad1_angle;
  pad.thermal_clear_shape = PAD_SHAPE_OVAL;
  pad.thermal_clear_sx = 0;
  pad.thermal_clear_sy= 0;
  pad.thermal_clear_angle= 0;
  add_pad(h.x, h.y, pad);

  return false;
}

/*
 * Hyperlynx 'USEG' subrecord of 'NET' record. 
 * Designates unrouted metal trace segment
 */

bool HypFile::Hyp::exec_useg(parse_param& h)
{
  if (trace_hyp) {
    cerr << "useg: x1 = " << h.x1 << " y1 = " << h.y1 << " layer1_name = " << h.layer1_name;
    cerr << " x2 = " << h.x2 << " y2 = " << h.y2 << " layer2_name = " << h.layer2_name;
    if (h.zlayer_name_set) cerr << " zlayer_name = " << h.zlayer_name << " width = " << h.width << " length = " << h.length;
    if (h.impedance_set) cerr << " impedance = " << h.impedance << " delay = " << h.delay;
    if (h.resistance_set) cerr << " resistance = " << h.resistance;
    cerr << endl;
    }

  h.x1 *= unit;
  h.y1 *= unit;
  h.x2 *= unit;
  h.y2 *= unit;
  h.width *= unit;
  h.length *= unit;
  if (!h.resistance_set) h.resistance = 0;

  UnroutedSegment u;
  u.x1 = h.x1;
  u.y1 = h.y1;
  u.layer1_name = h.layer1_name;
  u.x2 = h.x2;
  u.y2 = h.y2;
  u.layer2_name = h.layer2_name;
  u.zlayer_name = h.zlayer_name;
  u.zlayer_name_set = h.zlayer_name_set;
  u.width = h.width;
  u.impedance = h.impedance;
  u.impedance_set = h.impedance_set;
  u.delay = h.delay;
  u.resistance = h.resistance;
  net.back().unrouted_segment.push_back(u);

  return false;
}

/*
 * Hyperlynx 'A' attribute subrecord of 'NET' record.
 * Defines net attributes as name/value pairs.
 */

bool HypFile::Hyp::exec_net_attribute(parse_param& h)
{
  if (trace_hyp) cerr << "net_attribute: name = " << h.name  << " value = " << h.value << endl;

  Attribute a;
  a.name = h.name;
  a.value = h.value;

  net.back().attribute.push_back(a);

  return false;
}

/*
 * Add a polygon to the current net
 */

void HypFile::Hyp::add_polygon(Polygon poly)
{
  if (poly.vertex.empty()) return;
  add_polygon_to_net(net.back(), poly);
}

/* 
 * Add polygon poly to net pnet 
 */

void HypFile::Hyp::add_polygon_to_net(Net& pnet, Polygon poly) {

  /* set net name of polygon to net */
  poly.net_name = pnet.net_name;

  /* The polygon id is a positive number if the polygon has been assigned a polygon id. 
   * If the polygon has not been assigned a polygon id, the id is -1. 
   * An polygon which has not been assigned a polygon id has a single, outer edge, no holes. 
   * (the polygon does not have an id a POLYVOID could refer to).
   */

  if (poly.id < 0) {
    /*
     * If a polygon has id -1, assign the polygon an arbitrary negative id, different from -1.  
     * Make sure all polygons of the same net do not clash/have different ids.
     */
    int new_id = 0;
    if (!pnet.metal.empty())
      new_id = pnet.metal.begin()->first - 1; /* new id is one less than the smallest existing id */
    if (new_id >= -1) new_id = -2; /* does not clash with an assigned id (positive) or the default (-1) */
    poly.id = new_id;
    }

  /* check if polygons with this id already exist */
  PolygonMap::iterator i = pnet.metal.find(poly.id);
  if (i == pnet.metal.end()) {
    PolygonList empty_list;
    pnet.metal[poly.id] = empty_list;
    }

  /* add polygon to net */
  pnet.metal[poly.id].push_back(poly);

  return;
}

/*
 * Add a pad at coordinates (x, y)
 */

void HypFile::Hyp::add_pad(double pad_x, double pad_y, Pad pad)
{
  /* convert to polygon */
  Polygon pad_poly = pad2poly(pad_x, pad_y, pad);
 
  /* add pad to copper */
  add_polygon(pad_poly);

  return;
}

/*
 * Add via to list of via holes
 */

void HypFile::Hyp::add_via(double x, double y, std::string layer0_name, std::string layer1_name, double radius)
{
  if (radius != 0.0) {

    /* lookup drill vertical begin and end */
    double z0 = stackup.back().z0; /* defaults to bottom */
    double z1 = stackup.front().z1; /* defaults to top */

    for (LayerList::iterator l = stackup.begin(); l != stackup.end(); ++l)
      if (layer0_name == l->layer_name) {
        z0 = l->z0;
        z1 = l->z1;
        }

    for (LayerList::iterator l = stackup.begin(); l != stackup.end(); ++l)
      if (layer1_name == l->layer_name) {
        if (l->z1 > z1) z1 = l->z1;
        if (l->z0 < z0) z0 = l->z0;
        }

    Via v;
    v.x = x;
    v.y = y;
    v.layer0_name = layer0_name;
    v.layer1_name = layer1_name;
    v.z0 = z0;
    v.z1 = z1;
    v.radius = radius;
    net.back().via.push_back(v);
    }

  return;
}

/* not truncated */
