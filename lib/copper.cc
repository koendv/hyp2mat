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

#include "hyperlynx.h"
#include "crop.h"
#include <iostream>
#include <algorithm>

using namespace Hyp2Mat;

/*
 * These routines calculate the copper layers.
 * The routines operate in one of two modes: raw or normal.
 *
 * In raw mode, polygons are simply copied from input to output. No processing is done. Raw mode is mainly useful for debugging purposes.
 *
 * In normal mode, all copper on a single layer is calculated as a set of non-overlapping polygons.
 * Two kinds of copper polygons exist: normal and "PLANE". 
 *
 * Normal copper polygons are simply added to the copper of the plane.
 * Copper polygons of type "PLANE" have a clearance from other nets. This clearance is the "plane separation".
 * Copper polygons of type "PLANE" are calculated in two steps:
 *  We need a clearance "plane separation" between other nets and our polygon.
 *  - first, all copper polygons which are not of type "PLANE" are computed.
 *  - second, all copper polygon of type "PLANE" are computed. 
 *  - finally, the clearance is computed as follows:
 *      We expand all non-PLANE polygons of other nets by a distance "plane separation" and subtract them from the "PLANE" polygon.
 *
 * Two kinds of copper layers exist: "SIGNAL" and "PLANE". A "PLANE" layer is drawn in negative. 
 * On a "SIGNAL" layer the default is dielectric, and copper is drawn. On a "PLANE" layer, the default is copper, and dielectric is drawn.
 * The name of the PLANE layer is the name of the net of the default copper. 
 * E.g. a layer called VCC has a default of being 100% copper, connected to VCC.
 * "PLANE" layers may have a "plane separation" clearance between the default copper and other nets.
 */

/*
 * copy layer
 */

void HyperLynx::CopyLayer(Hyp2Mat::PCB& pcb, HypFile::Hyp& hyp_file, Hyp2Mat::Bounds bounds, Hyp2Mat::Polygon& board, HypFile::Layer& hyp_layer, std::vector<std::string> net_names, bool raw)
{

  Hyp2Mat::Layer layer;
  layer.layer_name = hyp_layer.layer_name;
  layer.material_name = hyp_layer.material_name;
  switch (hyp_layer.layer_type) {
    case HypFile::LAYER_SIGNAL :     layer.layer_type = Hyp2Mat::LAYER_SIGNAL;
      break;
    case HypFile::LAYER_DIELECTRIC : layer.layer_type = Hyp2Mat::LAYER_DIELECTRIC;
      break;
    case HypFile::LAYER_PLANE :      layer.layer_type = Hyp2Mat::LAYER_PLANE;
      break;
    default:                         layer.layer_type = Hyp2Mat::LAYER_SIGNAL;
      break;
    }
  layer.z0 = hyp_layer.z0;
  layer.z1 = hyp_layer.z1;
  layer.epsilon_r = hyp_layer.epsilon_r;
  layer.loss_tangent = hyp_layer.loss_tangent;
  layer.bulk_resistivity = hyp_layer.bulk_resistivity;
  layer.resistivity_temp_coeff = hyp_layer.temperature_coefficient;

  /* if the layer is metallic, copy the metal */
  if (layer.layer_type != Hyp2Mat::LAYER_DIELECTRIC) {

    /* Use default plane separation if layer plane separation is not set. layer plane separation is -1.0 if not set */
    double plane_separation = hyp_file.board.plane_separation;
    if (hyp_layer.plane_separation >= 0) plane_separation = hyp_layer.plane_separation;

    /* copy layer copper */
    Hyp2Mat::FloatPolygons raw_polygons;
    Hyp2Mat::Polygon layer_copper = CopyCopper(pcb, hyp_file, bounds, board, layer, hyp_layer, net_names, plane_separation, raw_polygons);
   
    if (raw)
      layer.metal = raw_polygons;
    else 
      layer.metal = layer_copper.Result(); /* calculate layer copper */
    }

  /* add to stackup */
  pcb.stackup.push_back(layer);

  return;
}

/*
 * copy copper
 */

Hyp2Mat::Polygon HyperLynx::CopyCopper(Hyp2Mat::PCB& pcb, HypFile::Hyp& hyp_file, Hyp2Mat::Bounds bounds, Hyp2Mat::Polygon& board, Hyp2Mat::Layer layer, HypFile::Layer& hyp_layer, std::vector<std::string> net_names, double plane_separation, Hyp2Mat::FloatPolygons& raw_polygons)
{
  Hyp2Mat::Polygon layer_copper;

  std::vector<Hyp2Mat::Polygon> net_poly;
  std::vector<Hyp2Mat::Polygon> net_plane_poly;
  std::vector<bool> net_wanted;

  HypFile::NetList::size_type net_size =  hyp_file.net.size();
  net_poly.resize(net_size);
  net_plane_poly.resize(net_size);
  net_wanted.resize(net_size);

  /* iterate over all nets twice.
     The first time we calculate all the copper except "plane" polygons. 
     The second time we calculate plane polygon pours. Plane polygons have a clearance from copper of other nets */

  /* Determine which nets we need to include */
  for (int i = 0; i < hyp_file.net.size(); ++i) {
    /* check if we're interested in this net. if no nets are specified, copy all nets */
    net_wanted[i] = net_names.empty() || (std::find(net_names.begin(), net_names.end(), hyp_file.net[i].net_name) != net_names.end());
    }
    
  /* first iteration. calculate all copper except "plane" polygons. */
  for (int i = 0; i < hyp_file.net.size(); ++i) {

    /* skip unwanted nets */
    if (!net_wanted[i]) continue;

    /* copy non-plane polygons of this net */
    Hyp2Mat::Polygon empty_poly;
    net_poly[i] = CopyNet(pcb, hyp_file, layer, hyp_file.net[i], plane_separation, false, empty_poly, raw_polygons); 
    }

  /* second iteration. calculate all plane polygons. */
  for (int i = 0; i < hyp_file.net.size(); ++i) {

    /* skip unwanted nets */
    if (!net_wanted[i]) continue;

    /* optimisation: skip nets without plane polygons. This avoids spending time calculating an empty polygon. Improves overall speed 10x */
    bool has_plane_poly = false;
    /* iterate over all Hyperlynx polygon id's */
    for (HypFile::PolygonMap::iterator j =  hyp_file.net[i].metal.begin(); j != hyp_file.net[i].metal.end(); ++j) {
      if (j->second.empty()) continue;
      bool this_polygon_plane = j->second.begin()->polygon_type == POLYGON_TYPE_PLANE;
      has_plane_poly = has_plane_poly || this_polygon_plane;
      }
    if (!has_plane_poly) continue;

    /* calculate other nets' copper.
       We need to calculate other net's copper because plane polygons have to keep a clearance 'plane_separation' from other nets. */
    Hyp2Mat::Polygon other_nets;
    for (int j = 0; j < hyp_file.net.size(); ++j)
      if (j != i) other_nets.Union(net_poly[j]);

    /* calculate plane polygons of this net */
    net_plane_poly[i] = CopyNet(pcb, hyp_file, layer, hyp_file.net[i], plane_separation, true, other_nets, raw_polygons); 
    }
    
  /* sum all copper */
  for (int i = 0; i < hyp_file.net.size(); ++i) {
    if (!net_wanted[i]) continue;
    layer_copper.Union(net_poly[i]);
    layer_copper.Union(net_plane_poly[i]);
  }

  /* plane layers: invert copper */ // XXX fixme
  if (layer.layer_type == Hyp2Mat::LAYER_PLANE) {
    Hyp2Mat::Polygon inverted_copper;
    inverted_copper.Union(board); /* default is 100% copper */
    inverted_copper.Difference(layer_copper); /* invert */
    layer_copper = inverted_copper;
    }

  /*
   * Crop to board size 
   */

  layer_copper = CropPolygon(layer_copper, bounds);

  return layer_copper;
}

/*
 * copy net
 */

Hyp2Mat::Polygon HyperLynx::CopyNet(Hyp2Mat::PCB& pcb, HypFile::Hyp& hyp_file, Hyp2Mat::Layer layer, HypFile::Net& hyp_net, double plane_separation, bool plane, Hyp2Mat::Polygon other_nets, Hyp2Mat::FloatPolygons& raw_polygons)
{
  Hyp2Mat::Polygon net_copper;

  /* Use net plane separation if set. net plane separation is -1 if not set. */
  if (hyp_net.plane_separation >= 0) plane_separation = hyp_net.plane_separation;

  /* iterate over all Hyperlynx polygon id's */
  for (HypFile::PolygonMap::iterator j =  hyp_net.metal.begin(); j != hyp_net.metal.end(); ++j) {

    if (j->second.empty()) continue;

    /* polygon layer */
    std::string poly_layer_name = j->second.begin()->layer_name;
  
    /* this polygon is on our layer if the polygon's layer name is the current layer's name.*/

    bool on_this_layer = (poly_layer_name == layer.layer_name);
 
    bool this_polygon_plane = j->second.begin()->polygon_type == POLYGON_TYPE_PLANE;

    /* Join all Hyperlynx polygons/polyvoids with the same id in a single Hyp2Mat polygon */
    if (on_this_layer && (this_polygon_plane == plane)) {
      Hyp2Mat::Polygon poly_copper = CopyPolygon(j->second, plane_separation, plane, other_nets, raw_polygons);
      net_copper.Union(poly_copper);
      }
    }

  return net_copper;
}

/*
 * copy polygon
 * polygon is copied twice; once to the list of "raw" polygons for --raw output; 
 * once added to this layers' copper for normal output.
 */

Hyp2Mat::Polygon HyperLynx::CopyPolygon(HypFile::PolygonList metal, double plane_separation, bool plane, Hyp2Mat::Polygon other_nets, Hyp2Mat::FloatPolygons& raw_polygons)
{
  Hyp2Mat::Polygon poly;

  if (metal.empty()) return poly; /* ought never to happen */

  /* polygon plane separation */
  if (metal.begin()->plane_separation >= 0) plane_separation = metal.begin()->plane_separation;

  /* Join all Hyperlynx polygons/polyvoids with the same id in a single Hyp2Mat polygon */
  for (HypFile::PolygonList::iterator k = metal.begin(); k != metal.end(); ++k) {

    /* copy vertices */
    Hyp2Mat::FloatPolygon edge;
    for (HypFile::PointList::iterator v = k->vertex.begin(); v != k->vertex.end(); ++v)
      edge.push_back(Hyp2Mat::FloatPoint(v->x, v->y));

    /* fix orientation */
    if (IsClockwise(edge) != k->positive) Reverse(edge);

    /* add to raw polygons */
    Hyp2Mat::FloatPoly raw_poly;
    raw_poly.poly = edge;
    raw_poly.is_hole = !k->positive;
    raw_poly.nesting_level = 0;
    raw_polygons.push_back(raw_poly);

    /* add to cooked polygons */
    if (k->positive) poly.AddEdge(edge);
    else poly.AddHole(edge);
    }

  /* take line width of polygon into account */
  double width = metal.begin()->width;
  poly.Simplify();
  poly.Offset(width/2);

  /* take plane separation into account */
  if (plane && (plane_separation >= 0.0)) {
    Hyp2Mat::Polygon clearance = other_nets;
    clearance.Offset(plane_separation);
    poly.Difference(clearance);
    }

  return poly;
}

/* not truncated */
