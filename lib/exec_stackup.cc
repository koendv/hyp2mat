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

#include <iterator>
#include <iostream>
#include <algorithm>
#include "hypfile.h"

using namespace std;
using namespace HypFile;

/*
 * Hyperlynx 'STACKUP' record. 
 * Defines pcb layers.
 */

/*
 * Hyperlynx 'OPTIONS' subrecord of STACKUP record. 
 * Defines dielectric constant and loss tangent of metal layers. 
 */

bool HypFile::Hyp::exec_options(parse_param& h)
{
  if (trace_hyp) cerr << "options: use_die_for_metal = " << h.use_die_for_metal << endl;
  use_die_for_metal = h.use_die_for_metal;

  return false;
}

/*
 * Hyperlynx 'SIGNAL' subrecord of 'STACKUP' record. 
 * Defines a pcb signal layer.
 */

bool HypFile::Hyp::exec_signal(parse_param& h)
{
  if (trace_hyp)
    cerr << "signal:";
  trace_layer(h);

  add_metal_layer(h);

  /* 
   *The difference between signal and plane layers is 
   * - signal layers are drawn positive, plane layers are drawn negative
   * - signal layers need a name
   */

  stackup.back().layer_type = LAYER_SIGNAL;

  if (!h.layer_name_set)
    error("missing signal layer name" );

  calc_layer_position();
  calc_layer_epsilon();

  return false;
}

/*
 * Hyperlynx 'DIELECTRIC' subrecord of 'STACKUP' record. 
 * Defines a pcb dielectric layer.
 */

bool HypFile::Hyp::exec_dielectric(parse_param& h)
{
  if (trace_hyp)
    cerr << "dielectric:";
  trace_layer(h);

  add_dielectric_layer(h);

  calc_layer_position();
  calc_layer_epsilon();

  return false;
}

/*
 * Hyperlynx 'PLANE' subrecord of 'STACKUP' record. 
 * Defines a pcb power or signal plane. 
 */

bool HypFile::Hyp::exec_plane(parse_param& h)
{
  if (trace_hyp)
    cerr << "plane:";
  trace_layer(h);

  add_metal_layer(h);

  stackup.back().layer_type = LAYER_PLANE;

  calc_layer_position();
  calc_layer_epsilon();

  return false;
}

/*
 * Debug output for layers
 */

bool HypFile::Hyp::trace_layer(parse_param& h)
{
  if (trace_hyp) {
    if (h.thickness_set) cerr << " thickness = " << h.thickness;
    if (h.plating_thickness_set) cerr << " plating_thickness = " << h.plating_thickness;
    if (h.bulk_resistivity_set) cerr << " bulk_resistivity = " << h.bulk_resistivity;
    if (h.temperature_coefficient_set) cerr << " temperature_coefficient = " << h.temperature_coefficient;
    if (h.epsilon_r_set) cerr << " epsilon_r = " << h.epsilon_r;
    if (h.loss_tangent_set) cerr << " loss_tangent = " << h.loss_tangent;
    if (h.conformal_set) cerr << " conformal = " << h.conformal;
    if (h.prepreg_set) cerr << " prepreg = " << h.prepreg;
    if (h.layer_name_set) cerr << " layer_name = " << h.layer_name;
    if (h.material_name_set) cerr << " material_name = " << h.material_name;
    if (h.plane_separation_set) cerr << " plane_separation = " << h.plane_separation;
    cerr << endl;
    }

  return true;
}

/*
 * Add a signal or plane layer.
 */

bool HypFile::Hyp::add_metal_layer(parse_param& h)
{
  Layer l;

  l.layer_type = LAYER_SIGNAL;

  if (h.thickness_set)
    l.thickness = h.thickness * metal_thickness_unit;
  else {
    error ("thickness missing");
    return true;
    }
 
  if (h.plating_thickness_set)
    l.plating_thickness = h.plating_thickness * metal_thickness_unit;
  else
    l.plating_thickness = 0;

  if (h.bulk_resistivity_set)
    l.bulk_resistivity = h.bulk_resistivity;
  else 
    l.bulk_resistivity = copper_bulk_resistivity;

  if (h.temperature_coefficient_set)
    l.temperature_coefficient = h.temperature_coefficient;
  else
    l.temperature_coefficient = copper_temperature_coefficient;

  if (h.conformal_set) {
    error("only dielectric layers conformal");
    return true;
    }
  else
    l.conformal = false;

  if (h.epsilon_r_set)
    l.epsilon_r = h.epsilon_r;
  else
    l.epsilon_r = fr4_epsilon_r;

  if (h.loss_tangent_set)
    l.loss_tangent = h.loss_tangent;
  else
    l.loss_tangent = fr4_loss_tangent;

  // use use_die_for_metal ? XXX

  if (h.prepreg_set) {
    error("only dielectric layers prepreg");
    return true;
    }
  else
    l.prepreg = false;

  if (h.layer_name_set)
    l.layer_name = h.layer_name;
  else {
    error("missing signal layer name" );
    l.layer_name = ""; // Continue anyhow
    }

  if (h.material_name_set)
    l.material_name = h.material_name;
  else
    l.material_name = "";

  if (h.plane_separation_set)
    l.plane_separation = h.plane_separation * unit;
  else
    l.plane_separation = board.plane_separation;

  // Add layer to stackup
  stackup.push_back(l);

  return false;
}

/*
 * add a substrate layer
 */

bool HypFile::Hyp::add_dielectric_layer(parse_param& h)
{

  Layer l;

  l.layer_type = LAYER_DIELECTRIC;

  if (h.thickness_set)
    l.thickness = h.thickness * unit;
  else {
    error ("thickness missing");
    return true;
    }
 
  if (h.plating_thickness_set) {
    error ("dielectric has no plating thickness");
    return true;
    }
  else
    l.plating_thickness = 0;

  if (h.bulk_resistivity_set) {
    error ("dielectric has no bulk resistivity");
    return true;
    }
  else 
    l.bulk_resistivity = 0;

  if (h.temperature_coefficient_set) {
    error ("dielectric has no bulk resistivity temperature coefficient");
    return true;
    }
  else
    l.temperature_coefficient = 0;

  if (h.conformal_set)
    l.conformal = h.conformal;
  else
    l.conformal = false;

  if (h.epsilon_r_set)
    l.epsilon_r = h.epsilon_r;
  else if (l.conformal)
    l.epsilon_r = conformal_epsilon_r;
  else
    l.epsilon_r = fr4_epsilon_r;

  if (h.loss_tangent_set)
    l.loss_tangent = h.loss_tangent;
  else
    l.loss_tangent = fr4_loss_tangent;

  if (h.prepreg_set)
    l.prepreg = h.prepreg;
  else
    l.prepreg = true;

  if (h.layer_name_set)
    l.layer_name = h.layer_name;
  else
    l.layer_name = "";

  if (h.material_name_set)
    l.material_name = h.material_name;
  else
    l.material_name = "";

  if (h.plane_separation_set) {
    error ("dielectric has no plane separation");
    return true;
    }
  else
    l.plane_separation = 0;

  // Add layer to stackup
  stackup.push_back(l);

  return false;
}

 /*
  * Recalculate vertical position of all layers.
  * Layers are ordered top first, bottom last.
  * Called when a new layer is added.
  */

bool HypFile::Hyp::calc_layer_position()
{
  // calculate vertical position, from bottom to top
  double z_top = 0; // current vertical position
  for (LayerList::reverse_iterator it = stackup.rbegin(); it != stackup.rend(); ++it) {
    if (it->layer_type == LAYER_DIELECTRIC) {
      /* substrate and prepreg */
      it->z0 = z_top;
      it->z1 = z_top + it->thickness;
      z_top = it->z1;
      }
    else {
      /* metal layers */
      it->z0 = z_top;
      it->z1 = z_top;
      }
    }

  return true;
}

bool HypFile::Hyp::calc_layer_epsilon()
{

  /* no calculation needed if dielectric constant have been specified */
  if (use_die_for_metal)
    return true;

  /* calculate dielectric constants */
  for (LayerList::iterator i = stackup.begin(); i != stackup.end(); ++i) {

    /* skip dielectric layers */
    if (i->layer_type == LAYER_DIELECTRIC)
      continue;

    /* find first dielectric layer above current metal layer */
    bool upper_layer_found = false;
    LayerList::iterator upper_layer;
    for (LayerList::iterator j = stackup.begin(); j != i; ++j)
      if (j->layer_type == LAYER_DIELECTRIC) {
        upper_layer_found = true;
        upper_layer = j;
        };

    /* find first dielectric layer below current metal layer */
    bool lower_layer_found = false;
    LayerList::iterator lower_layer;
    for (LayerList::iterator j = i; j != stackup.end(); ++j)
      if (j->layer_type == LAYER_DIELECTRIC) {
        lower_layer_found = true;
        lower_layer = j;
        break;
        };

    if (!upper_layer_found || !lower_layer_found) {
      /* assume outer copper layers are in air */
      i->epsilon_r = 1.0;
      i->loss_tangent = 0.0;
      }
    else if (upper_layer->prepreg && !lower_layer->prepreg) {
      /* upper layer is prepreg. use values of upper layer */
      i->epsilon_r = upper_layer->epsilon_r;
      i->loss_tangent = upper_layer->loss_tangent;
      }
    else if (!upper_layer->prepreg && lower_layer->prepreg) {
      /* lower layer is prepreg. use values of lower layer */
      i->epsilon_r = lower_layer->epsilon_r;
      i->loss_tangent = lower_layer->loss_tangent;
      } 
    else {
      /* use average of upper and lower layer */
      i->epsilon_r = (upper_layer->epsilon_r + lower_layer->epsilon_r)/2;
      i->loss_tangent = (upper_layer->loss_tangent + lower_layer->loss_tangent) /2;
      }
    }

  return false;
}

/*
 * Flood specified layers with copper 
 */

void HypFile::Hyp::flood_layers_with_copper()
{

  /* check if we have to flood all plane layers */
  bool flood_plane_layers = std::find(flood_layers.begin(), flood_layers.end(), "plane_layers") != flood_layers.end(); /* look for special value "plane_layers" */

  for (LayerList::iterator l = stackup.begin(); l != stackup.end(); ++l) {
   
    /* metallic layers only */
    if (l->layer_type == LAYER_DIELECTRIC) continue;

    /* check if we have to flood this layer */
    bool flood_this_layer = std::find(flood_layers.begin(), flood_layers.end(), l->layer_name) != flood_layers.end();

    /* flood with copper if we have to flood this layer, or if this is a plane layer and we have to flood all plane layers */
    if (!(flood_this_layer || (flood_plane_layers && (l->layer_type == LAYER_PLANE)))) continue;

    /* create default copper on plane layer */
    Polygon default_copper;
    default_copper.vertex = board.edge.front().vertex; /* polygon is board outline */
    default_copper.polygon_type = POLYGON_TYPE_PLANE;
    default_copper.net_name = l->layer_name; /* flood layer VCC with copper belonging to net VCC, layer GND with copper belonging to net GND */
    default_copper.id = -1;
    default_copper.positive = true;
    default_copper.width = 0.0;
    default_copper.plane_separation = -1.0;
    default_copper.left_plane_separation = -1.0;
    default_copper.layer_name = l->layer_name;

    /* Look up net with same name as layer */
    NetList::iterator n;
    for (n = net.begin(); n != net.end(); ++n)
      if (n->net_name == l->layer_name) {
        add_polygon_to_net(*n, default_copper);
        break;
        }

    /* Net with same name as layer not found. Create net */
    if (n == net.end()) {
      Net plane_net;
      plane_net.net_name = l->layer_name;
      plane_net.plane_separation = -1.0;
      net.push_back(plane_net);
      add_polygon_to_net(net.back(), default_copper);
      }

    }

  return;
}

/* not truncated */
