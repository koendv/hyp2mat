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
 * Plane separation can be specified for the board, for a layer, for a net or for a polygon.
 *
 * Copper polygons of type "PLANE" are calculated as follows:
 *  - all polygons of all other nets on the same layer are computed, and explanded by the plane separation. This produces the mask.
 *  - The mask is subtracted from the "PLANE" type polygon.
 *
 * If mask polygon and plane polygon have a different value for plane separation the largest value is used.
 *
 * Note there is no plane separation between polygons of the same net.
 */

/*
 * copy layer stackup
 */

void HyperLynx::CopyStackUp(Hyp2Mat::PCB& pcb)
{
  /* iterate over all layers */
  for (HypFile::LayerList::iterator l = hyp_file.stackup.begin(); l != hyp_file.stackup.end(); ++l) {

    /* check if we're interested in this layer. if no layers are specified, copy all layers */
    bool layer_wanted = layers.empty() || (std::find(layers.begin(), layers.end(), l->layer_name) != layers.end());

    /* copy this layer if needed */
    if (layer_wanted) CopyLayer(pcb, *l);
    }

  return;
}

/*
 * copy layer
 */

void HyperLynx::CopyLayer(Hyp2Mat::PCB& pcb, HypFile::Layer& hyp_layer)
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
  layer.thickness = hyp_layer.thickness;
  layer.epsilon_r = hyp_layer.epsilon_r;
  layer.loss_tangent = hyp_layer.loss_tangent;
  layer.bulk_resistivity = hyp_layer.bulk_resistivity;
  layer.resistivity_temp_coeff = hyp_layer.temperature_coefficient;

  /* if the layer is metallic, copy the metal */
  if (layer.layer_type != Hyp2Mat::LAYER_DIELECTRIC) {

    /* Use default plane separation if layer plane separation is not set. layer plane separation is -1.0 if not set */
    layer_plane_separation = hyp_file.board.plane_separation;

    /* board-level override from SetClearance method / --clearance command-line parameter */
    if (HyperLynx::clearance >= 0) layer_plane_separation = HyperLynx::clearance;

    if (hyp_layer.plane_separation >= 0) layer_plane_separation = hyp_layer.plane_separation;

    /* copy layer copper */
    Hyp2Mat::FloatPolygons raw_polygons;
    Hyp2Mat::Polygon layer_copper = CopyCopper(layer, raw_polygons);
   
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

Hyp2Mat::Polygon HyperLynx::CopyCopper(Hyp2Mat::Layer layer, Hyp2Mat::FloatPolygons& raw_polygons)
{

  /* vectors of different polygon types. element [i] belongs to net hyp_file.net[i].net_name */
  std::vector<Hyp2Mat::Polygon> net_pour;      /* POLYGON T=POUR  and ordinary line and arc segments */
  std::vector<Hyp2Mat::Polygon> net_plane;     /* POLYGON T=PLANE */
  std::vector<Hyp2Mat::Polygon> net_copper;    /* POLYGON T=COPPER */
  std::vector<Hyp2Mat::Polygon> net_pads;      /* pads */
  std::vector<Hyp2Mat::Polygon> net_antipads;  /* anti-pads */
  std::vector<bool> net_wanted;                /* true if net needs to be included in the output */

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
  for (unsigned int i = 0; i < hyp_file.net.size(); ++i) {
    /* check if we're interested in this net. if no nets are specified, copy all nets */
    net_wanted[i] = nets.empty() || (std::find(nets.begin(), nets.end(), hyp_file.net[i].net_name) != nets.end());
    }

  /* calculate all copper. */
  for (unsigned int i = 0; i < hyp_file.net.size(); ++i) {

    /* skip unwanted nets */
    if (!net_wanted[i]) continue;

    /* copy anti-metal of this net */
    net_pour[i] = CopyNet(layer, hyp_file.net[i], POLYGON_TYPE_POUR, raw_polygons); 
    net_copper[i] = CopyNet(layer, hyp_file.net[i], POLYGON_TYPE_COPPER, raw_polygons); 
    net_plane[i] = CopyNet(layer, hyp_file.net[i], POLYGON_TYPE_PLANE, raw_polygons); 
    net_pads[i] = CopyNet(layer, hyp_file.net[i], POLYGON_TYPE_PAD, raw_polygons); 
    net_antipads[i] = CopyNet(layer, hyp_file.net[i], POLYGON_TYPE_ANTIPAD, raw_polygons); 
    }

  /*
   * sum all copper on this layer 
   */ 

  Hyp2Mat::Polygon layer_copper;

  /* add plane and pour polygons */
  for (unsigned int i = 0; i < hyp_file.net.size(); ++i) {

    /* skip unwanted nets */
    if (!net_wanted[i]) continue;

    layer_copper.Union(net_pour[i]);
    layer_copper.Union(net_plane[i]);
    }

  /* subtract antipads from pour and plane polygons. */
  for (unsigned int i = 0; i < hyp_file.net.size(); ++i) {

    /* skip unwanted nets */
    if (!net_wanted[i]) continue;

    layer_copper.Difference(net_antipads[i]);
    }

  /* add copper and pad polygons */
  for (unsigned int i = 0; i < hyp_file.net.size(); ++i) {

    /* skip unwanted nets */
    if (!net_wanted[i]) continue;

    layer_copper.Union(net_copper[i]);
    layer_copper.Union(net_pads[i]);
    }

  /*
   * Crop to board size 
   */

  if (!board.IsEmpty()) {
    layer_copper.Intersection(board);
    layer_copper = CropPolygon(layer_copper, bounds);
    }

  return layer_copper;
}

/*
 * copy net
 * copy all polygons of a net.
 * if anti is true, copy anti-pads
 * if plane is true, copy plane polygons (and subtract other_nets)
 */

Hyp2Mat::Polygon HyperLynx::CopyNet(Hyp2Mat::Layer layer, HypFile::Net& hyp_net, polygon_type_enum poly_type, Hyp2Mat::FloatPolygons& raw_polygons)
{
  Hyp2Mat::Polygon net_copper;

  /* calculate net-specific plane separation */
  net_plane_separation = layer_plane_separation;
  if (hyp_net.plane_separation >= 0) net_plane_separation = hyp_net.plane_separation;

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
    Hyp2Mat::Polygon poly_copper = CopyPolygon(j->second, raw_polygons);
    net_copper.Union(poly_copper);
    }

  return net_copper;
}

/*
 * copy polygon
 * polygon is copied twice; once to the list of "raw" polygons for --raw output; 
 * once added to this layers' copper for normal output.
 */

Hyp2Mat::Polygon HyperLynx::CopyPolygon(HypFile::PolygonList metal, Hyp2Mat::FloatPolygons& raw_polygons)
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

  if (metal.front().polygon_type == POLYGON_TYPE_PLANE) {
    /* calculate polygon-specific plane separation */
    polygon_plane_separation = net_plane_separation;
    if (metal.front().plane_separation >= 0) polygon_plane_separation = metal.front().plane_separation;

    /* calculate mask needed to get clearance with other nets */
    Hyp2Mat::Polygon mask;
    mask = PlaneSeparation(metal.front().layer_name, metal.front().net_name);

    /* subtract mask from polygon */
    poly.Difference(mask);
    }

  return poly;
}

/*
 * Creates the mask by which to trim a plane polygon, taking into account plane separation.
 *
 * Algorithm: sum all polygons on layer 'layer_name' belonging to nets other than net 'net_name', 
 * expanded by the biggest polygon_plane separation.
 */

Hyp2Mat::Polygon HyperLynx::PlaneSeparation(std::string layer_name, std::string net_name)
{
  Hyp2Mat::Polygon mask;

  if (polygon_plane_separation < 0) return mask; // XXX correct?

  /* iterate over all nets */
  for (HypFile::NetList::iterator i = hyp_file.net.begin(); i != hyp_file.net.end(); ++i) {

    /* all nets except net 'net_name' */
    if (i->net_name == net_name) continue;

    /* wanted nets only */
    bool net_wanted = nets.empty() || (std::find(nets.begin(), nets.end(), net_name) != nets.end());
    if (!net_wanted) continue;

    /* iterate over all Hyperlynx polygon id's */
    for (HypFile::PolygonMap::iterator j =  i->metal.begin(); j != i->metal.end(); ++j) {
      Hyp2Mat::Polygon poly;

      if (j->second.empty()) continue; /* ought never to happen */

      /* polygons on layer 'layer_name' only */
      if (j->second.begin()->layer_name != layer_name) continue;

      /* iterate over all edges */
      for (HypFile::PolygonList::iterator k = j->second.begin(); k != j->second.end(); ++k) {

        /* iterate over all vertices */
        Hyp2Mat::FloatPolygon edge;
        for (HypFile::PointList::iterator v = k->vertex.begin(); v != k->vertex.end(); ++v)
          edge.push_back(Hyp2Mat::FloatPoint(v->x, v->y));

        /* add edge to polygon */
        if (k->positive) poly.AddEdge(edge);
        else poly.AddHole(edge);
        }

      double width = j->second.begin()->width;

      /* calculate plane separation of the masking polygon */
      double mask_plane_separation;
      mask_plane_separation = layer_plane_separation; /* layer */
      if (i->plane_separation >= 0) mask_plane_separation = i->plane_separation; /* net */
      if (j->second.begin()->plane_separation >= 0) mask_plane_separation = j->second.begin()->plane_separation; /* polygon */

      /* compare plane separation of polygon and masking polygon; choose biggest plane separation. */
      double biggest_plane_separation;
      if (polygon_plane_separation > mask_plane_separation) biggest_plane_separation = polygon_plane_separation;
      else biggest_plane_separation = mask_plane_separation;

      /* grow masking polygon by plane separation */
      poly.Offset(width/2 + biggest_plane_separation);

      /* add to mask */
      mask.Union(poly);
      
      }

    }

  return mask;  
}

/* not truncated */
