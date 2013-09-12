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

    /* board-level override from SetClearance method / --clearance command-line parameter */
    if (HyperLynx::_clearance >= 0) plane_separation = HyperLynx::_clearance;

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

  /* vectors of different polygon types. element [i] belongs to net hyp_file.net[i].net_name */
  std::vector<Hyp2Mat::Polygon> net_pour;      /* POLYGON T=POUR  and ordinary line and arc segments */
  std::vector<Hyp2Mat::Polygon> net_plane;     /* POLYGON T=PLANE */
  std::vector<Hyp2Mat::Polygon> net_copper;    /* POLYGON T=COPPER */
  std::vector<Hyp2Mat::Polygon> net_pads;      /* pads */
  std::vector<Hyp2Mat::Polygon> net_antipads;  /* anti-pads */
  std::vector<bool> net_wanted;

  HypFile::NetList::size_type net_size =  hyp_file.net.size();
  net_pour.resize(net_size);
  net_plane.resize(net_size);
  net_copper.resize(net_size);
  net_pads.resize(net_size);
  net_antipads.resize(net_size);
  net_wanted.resize(net_size);

  /* iterate over all nets twice.
     The first time we calculate all the copper except "plane" polygons. 
     The second time we calculate plane polygon pours. Plane polygons have a clearance from copper of other nets */

  /* Determine which nets we need to include */
  for (int i = 0; i < hyp_file.net.size(); ++i) {
    /* check if we're interested in this net. if no nets are specified, copy all nets */
    net_wanted[i] = net_names.empty() || (std::find(net_names.begin(), net_names.end(), hyp_file.net[i].net_name) != net_names.end());
    }

  /* first iteration. calculate all copper, but don't take plane separation into account yet. */
  for (int i = 0; i < hyp_file.net.size(); ++i) {

    /* skip unwanted nets */
    if (!net_wanted[i]) continue;

    /* copy anti-metal of this net */
    Hyp2Mat::Polygon empty_poly;
    net_pour[i] = CopyNet(pcb, hyp_file, board, layer, hyp_file.net[i], POLYGON_TYPE_POUR, -1.0, empty_poly, raw_polygons); 
    net_copper[i] = CopyNet(pcb, hyp_file, board, layer, hyp_file.net[i], POLYGON_TYPE_COPPER, -1.0, empty_poly, raw_polygons); 
    net_plane[i] = CopyNet(pcb, hyp_file, board, layer, hyp_file.net[i], POLYGON_TYPE_PLANE, -1.0, empty_poly, raw_polygons); 
    net_pads[i] = CopyNet(pcb, hyp_file, board, layer, hyp_file.net[i], POLYGON_TYPE_PAD, -1.0, empty_poly, raw_polygons); 
    net_antipads[i] = CopyNet(pcb, hyp_file, board, layer, hyp_file.net[i], POLYGON_TYPE_ANTIPAD, -1.0, empty_poly, raw_polygons); 
    }

  /* second iteration. calculate all plane polygons, taking plane separation into account. */
  for (int i = 0; i < hyp_file.net.size(); ++i) {

    /* skip unwanted nets */
    if (!net_wanted[i]) continue;

    /* optimisation: skip nets without plane polygons. This avoids spending time calculating an empty polygon. Improves overall speed 10x */
    if (!NetHasPlanePoly(hyp_file.net[i])) continue;

    /* calculate other nets' copper.
       We need to calculate other net's copper because plane polygons have to keep a clearance 'plane_separation' from other nets. */

    Hyp2Mat::Polygon other_copper;
    for (int j = 0; j < hyp_file.net.size(); ++j)
      if (j != i) {
        other_copper.Union(net_pour[j]);
        other_copper.Union(net_plane[j]);
        other_copper.Union(net_copper[j]);
        other_copper.Union(net_pads[j]);
        }

    /* calculate other nets' antipads. plane polygons have holes where antipads are. */

    Hyp2Mat::Polygon other_antipads;
    for (int j = 0; j < hyp_file.net.size(); ++j)
      if (j != i) {
        other_antipads.Union(net_antipads[j]);
        }

    /* calculate plane polygons of this net */
    Hyp2Mat::FloatPolygons dummy;
    net_plane[i] = CopyNet(pcb, hyp_file, board, layer, hyp_file.net[i], POLYGON_TYPE_PLANE, plane_separation, other_copper, dummy); 

    /* Subtract other nets' antipads */
    net_plane[i].Difference(other_antipads);
    }
   
  /* sum all copper on this layer */ 
  Hyp2Mat::Polygon layer_copper;

  for (int i = 0; i < hyp_file.net.size(); ++i) {

    /* skip unwanted nets */
    if (!net_wanted[i]) continue;

    Hyp2Mat::Polygon this_net_copper;
    if (layer.layer_type != Hyp2Mat::LAYER_PLANE) {
      /* on plane layers, these are the small copper segments used to split planes */
      this_net_copper.Union(net_copper[i]);
      }
    this_net_copper.Union(net_pour[i]);
    this_net_copper.Union(net_plane[i]);
    this_net_copper.Union(net_pads[i]);

    layer_copper.Union(this_net_copper);
    }

  /*
   * Crop to board size 
   */

  layer_copper.Intersection(board);
  layer_copper = CropPolygon(layer_copper, bounds);

  return layer_copper;
}

bool HyperLynx::NetHasPlanePoly(HypFile::Net& hyp_net)
{
  bool has_plane_poly = false;

  /* iterate over all Hyperlynx polygon id's */
  for (HypFile::PolygonMap::iterator j =  hyp_net.metal.begin(); j != hyp_net.metal.end(); ++j) {
    if (j->second.empty()) continue;
    bool this_polygon_plane = j->second.begin()->polygon_type == POLYGON_TYPE_PLANE;
    has_plane_poly = has_plane_poly || this_polygon_plane;
    }

  return has_plane_poly;
}

/*
 * copy net
 * copy all polygons of a net.
 * if anti is true, copy anti-pads
 * if plane is true, copy plane polygons (and subtract other_nets)
 */

Hyp2Mat::Polygon HyperLynx::CopyNet(Hyp2Mat::PCB& pcb, HypFile::Hyp& hyp_file, Hyp2Mat::Polygon& board, Hyp2Mat::Layer layer, HypFile::Net& hyp_net, polygon_type_enum poly_type, double plane_separation, Hyp2Mat::Polygon other_nets, Hyp2Mat::FloatPolygons& raw_polygons)
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
    if (poly_layer_name != layer.layer_name) continue;
 
    /* Check polygon is the correct type (pour, plane, copper, pad or antipad) */
    if (j->second.begin()->polygon_type != poly_type) continue;

    /* Join all Hyperlynx polygons/polyvoids with the same id in a single Hyp2Mat polygon */
    Hyp2Mat::Polygon poly_copper = CopyPolygon(j->second, plane_separation, other_nets, raw_polygons);
    net_copper.Union(poly_copper);
    }

  return net_copper;
}

/*
 * copy polygon
 * polygon is copied twice; once to the list of "raw" polygons for --raw output; 
 * once added to this layers' copper for normal output.
 */

Hyp2Mat::Polygon HyperLynx::CopyPolygon(HypFile::PolygonList metal, double plane_separation, Hyp2Mat::Polygon other_nets, Hyp2Mat::FloatPolygons& raw_polygons)
{
  Hyp2Mat::Polygon poly;

  if (metal.empty()) return poly; /* ought never to happen */

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

  /* polygon plane separation */
  if (metal.begin()->plane_separation >= 0) plane_separation = metal.begin()->plane_separation;
  /* apply plane separation */
  if (metal.begin()->polygon_type == POLYGON_TYPE_PLANE) PlaneSeparation(poly, other_nets, plane_separation);

  return poly;
}

/*
 * clip net_metal until the distance to other_net_metal is equal to or greater than plane_separation
 */

void HyperLynx::PlaneSeparation(Hyp2Mat::Polygon& net_metal, Hyp2Mat::Polygon other_net_metal, double plane_separation)
{
  /* take plane separation into account */
  if (plane_separation >= 0.0) {
    Hyp2Mat::Polygon clearance = other_net_metal;
    clearance.Offset(plane_separation);
    net_metal.Difference(clearance);
    }
  return;  
}

/* not truncated */
