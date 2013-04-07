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
 * HypFile is concerned with parsing the hyperlynx file, and contains many variables and methods 
 * which are only of interest during parsing.
 *
 * Hyp2Mat is interested in presenting an uncluttered interface.
 * 
 * This is where data is passed on from HypFile to Hyp2Mat, from the obscure to the obvious.
 */

#include <iostream>
#include <algorithm>

#include "hyperlynx.h"

/*
 * Read a pcb in hyperlynx format
 */

void HyperLynx::Read (std::string filename, Hyp2Mat::PCB& pcb, std::vector<std::string> nets, double arc_precision)
{
  HypFile::Hyp hyp_file;

  /*
   * load hyperlynx file in hyp_file 
   */

  bool success = LoadHypFile(pcb, hyp_file, filename, arc_precision);
  if (!success)
    return;

  /*
   * Copy hyperlynx file data from hyp_file to pcb
   */

  CopyBoard(pcb, hyp_file); /* copy board outline */
  CopyStackUp(pcb, hyp_file); /* copy layer stackup */
  CopyPolygons(pcb, hyp_file, nets); /* copy polygons */
  CopyVias(pcb, hyp_file, nets); /* copy layer stackup */
  CopyDevices(pcb, hyp_file); /* copy device info */
  CopyPins(pcb, hyp_file, nets); /* copy pin references */

  /* 
   * Check whether to print all net names 
   */

  /* if one of the nets is "?", print list of all nets */
  if (std::find(nets.begin(), nets.end(), "?") != nets.end()) 
    PrintNets(hyp_file);

  return;
}

/*
 * Load Hyperlynx file in hyp_file
 */

bool HyperLynx::LoadHypFile(Hyp2Mat::PCB& pcb, HypFile::Hyp& hyp_file, std::string filename, double arc_precision)
{
  hyp_file.trace_scanner = (pcb.debug == 4);
  hyp_file.trace_parser = (pcb.debug == 3);
  hyp_file.trace_hyp = (pcb.debug == 2);

  if (arc_precision < 0) {
    std::cerr << "arc precision negative" << std::endl;
    arc_precision = 0;
    }
  hyp_file.arc_precision = arc_precision;

  bool success = hyp_file.load(filename);

  if (pcb.debug == 1) {
    std::cerr << "writing hyp_file dump to hyp_debug.txt" << std::endl;
    hyp_file.save ("hyp_debug.txt");
    }

  return success;
}

/*
 * copy board outline 
 */

void HyperLynx::CopyBoard(Hyp2Mat::PCB& pcb, HypFile::Hyp& hyp_file)
{
  bool outer_edge = true;
  Hyp2Mat::Polygon board_polygon;
  for (HypFile::PolygonList::iterator i = hyp_file.board.edge.begin(); i != hyp_file.board.edge.end(); ++i) {
    Hyp2Mat::Edge edge;
    edge.width = 0;
    edge.is_hole = !outer_edge;
    for (HypFile::PointList::iterator j = i->vertex.begin(); j != i->vertex.end(); ++j)
      edge.vertex.push_back(Hyp2Mat::Point(j->x, j->y));
    
    board_polygon.push_back(edge);
    outer_edge = false;
  }
  pcb.board.push_back(board_polygon);
  return;
}

/*
 * copy layer stackup
 */

void HyperLynx::CopyStackUp(Hyp2Mat::PCB& pcb, HypFile::Hyp& hyp_file)
{
  for (HypFile::LayerList::iterator l = hyp_file.stackup.begin(); l != hyp_file.stackup.end(); ++l) {
    Hyp2Mat::Layer layer;
    layer.layer_name = l->layer_name;
    layer.material_name = l->material_name;
    switch (l->layer_type) {
      case HypFile::LAYER_SIGNAL :     layer.layer_type = Hyp2Mat::LAYER_SIGNAL;
        break;
      case HypFile::LAYER_DIELECTRIC : layer.layer_type = Hyp2Mat::LAYER_DIELECTRIC;
        break;
      case HypFile::LAYER_PLANE :      layer.layer_type = Hyp2Mat::LAYER_PLANE;
        break;
      default:                         layer.layer_type = Hyp2Mat::LAYER_SIGNAL;
        break;
      }
    layer.z0 = l->z0;
    layer.z1 = l->z1;
    layer.epsilon_r = l->epsilon_r;
    layer.loss_tangent = l->loss_tangent;
    layer.bulk_resistivity = l->bulk_resistivity;
    layer.resistivity_temp_coeff = l->temperature_coefficient;
    pcb.stackup.push_back(layer);
    }
  return;
}

/*
 * copy polygons
 */

void HyperLynx::CopyPolygons(Hyp2Mat::PCB& pcb, HypFile::Hyp& hyp_file, std::vector<std::string> net_names)
{
  /* iterate over all layers */
  for (Hyp2Mat::LayerList::iterator l = pcb.stackup.begin(); l != pcb.stackup.end(); ++l) {

    if (l->layer_type == Hyp2Mat::LAYER_DIELECTRIC) continue;

    /* iterate over all nets */
    for (HypFile::NetList::iterator i = hyp_file.net.begin(); i != hyp_file.net.end(); ++i) {

      /* check if we're interested in this net. if no nets are specified, copy all nets */
      bool net_wanted = net_names.empty() || (std::find(net_names.begin(), net_names.end(), i->net_name) != net_names.end());
      if (!net_wanted) continue;

      /* iterate over all Hyperlynx polygon id's */
      for (HypFile::PolygonMap::iterator j =  i->metal.begin(); j != i->metal.end(); ++j) {

        /* The Hyperlynx poligon id is non-negative.
           If polygon_id is positive or zero, join all Hyperlynx polygons with this id in a single Hyp2Mat polygon 
           If polygon id is negative (e.g. -1) emit a Hyp2Mat polygon for every Hyperlynx polygon */

        Hyp2Mat::Polygon poly;

        for (HypFile::PolygonList::iterator k = j->second.begin(); k != j->second.end(); ++k) {

          /* this polygon is on our layer if 
           * - the polygon's layer is the current layer exactly,
           * - or the polygon's layer is MDEF for all metal layers,
           * - or the polygon's layer is ADEF for all plane layers. 
           */
          bool on_this_layer = (k->layer_name == l->layer_name)
            ||((k->layer_name == "MDEF") && ((l->layer_type == Hyp2Mat::LAYER_SIGNAL) || (l->layer_type == Hyp2Mat::LAYER_PLANE)))
            ||((k->layer_name == "ADEF") && (l->layer_type == Hyp2Mat::LAYER_PLANE));
          if (!on_this_layer) continue;

          /* copy vertices */
          Hyp2Mat::Edge edge;
          for (HypFile::PointList::iterator v = k->vertex.begin(); v != k->vertex.end(); ++v)
            edge.vertex.push_back(Hyp2Mat::Point(v->x, v->y));
          edge.is_hole = !k->positive;
          edge.width = k->width;

          if (edge.vertex.empty()) 
            continue;

          /* if polygon_id is positive or zero, join all Hyperlynx polygons with this id in a single Hyp2Mat polygon */
          if (j->first >= 0)
            poly.push_back(edge);
          else {
            /* negative polygon id: emit a Hyp2Mat polygon */
            Hyp2Mat::Polygon freestanding_poly;
            freestanding_poly.push_back(edge);
            l->metal.push_back(freestanding_poly);
            }

          }

        /* add the Hyp2Mat polygon to the copper on this layer */
        if (!poly.empty())
          l->metal.push_back(poly);
        }
      }
    }
  return;
}

/*
 * copy vias
 */

void HyperLynx::CopyVias(Hyp2Mat::PCB& pcb, HypFile::Hyp& hyp_file, std::vector<std::string> nets)
{

  for (HypFile::NetList::iterator i = hyp_file.net.begin(); i != hyp_file.net.end(); ++i) {

    /* check if we're interested in this net. if no nets are specified, copy all nets */
    bool net_wanted = nets.empty() || (std::find(nets.begin(), nets.end(), i->net_name) != nets.end());
    if (!net_wanted) continue;

    /* copy vias of current net */
    for (HypFile::ViaList::iterator j = i->via.begin(); j != i->via.end(); ++j) {
      Hyp2Mat::Via v;
      v.x = j->x;
      v.y = j->y;
      v.z0 = j->z0;
      v.z1 = j->z1;
      v.radius = j->radius;
      pcb.via.push_back(v);
      } 
  }
  return;
}

/* 
 *  copy via plating
 */

void CopyViaPlating(Hyp2Mat::PCB& pcb, HypFile::Hyp& hyp_file)
{
  /* assume via plating thickness is identical to outer plating thickness */
  for (HypFile::LayerList::iterator l = hyp_file.stackup.begin(); l != hyp_file.stackup.end(); ++l) 
    if (l->layer_type != HypFile::LAYER_DIELECTRIC) {
      /* first metal layer */
      pcb.via_plating_thickness = l->plating_thickness; 
      break;
      }

  return;
}

/*
 * copy devices
 */

void HyperLynx::CopyDevices(Hyp2Mat::PCB& pcb, HypFile::Hyp& hyp_file)
{
  for (HypFile::DeviceList::iterator i = hyp_file.device.begin(); i != hyp_file.device.end(); ++i) {
    /* set up device */
    Hyp2Mat::Device dev;
    dev.ref = i->ref;
    dev.name = i->name;
    dev.layer_name = i->layer_name;

    if (i->value_float_set) {
      dev.value_float = i->value_float;
      dev.value_type = Hyp2Mat::DEVICE_VALUE_FLOAT;
      }
    else if (i->value_string_set) {
      dev.value_string = i->value_string;
      dev.value_type = Hyp2Mat::DEVICE_VALUE_STRING;
      }
    else dev.value_type = Hyp2Mat::DEVICE_VALUE_NONE;

    /* add device to board */
    pcb.device.push_back(dev);
    }
  return;
}

/*
 * copy pins
 */

void HyperLynx::CopyPins(Hyp2Mat::PCB& pcb, HypFile::Hyp& hyp_file, std::vector<std::string> nets)
{
  /* iterate over all pins */
  for (HypFile::NetList::iterator i = hyp_file.net.begin(); i != hyp_file.net.end(); ++i) {

    /* check if we're interested in this net. if no nets are specified, copy all nets */
    bool net_wanted = nets.empty() || (std::find(nets.begin(), nets.end(), i->net_name) != nets.end());
    if (!net_wanted) continue;

    for (HypFile::PinList::iterator j = i->pin.begin(); j != i->pin.end(); ++j)
      /* find pin padstack */  
      for (HypFile::PadstackList::iterator p = hyp_file.padstack.begin(); p != hyp_file.padstack.end(); ++p)
        if (p->padstack_name == j->padstack_name)
          /* copy this pin */
          CopyPin(pcb, hyp_file, *j, *p);
    }

  return;
}

/*
 * copy a single pin
 */

void HyperLynx::CopyPin(Hyp2Mat::PCB& pcb, HypFile::Hyp& hyp_file, HypFile::Pin& pin, HypFile::Padstack& padstack)
{
  Hyp2Mat::Pin new_pin;
  new_pin.x = pin.x;
  new_pin.y = pin.y;
  new_pin.ref = pin.pin_reference;

  HypFile::Pad pad;

  std::string layer_name;

  /* now find pad */

  /* how many layers does the padstack have? */
  if (padstack.pads.size() == 1) {
    /* only a single layer: that's easy */
    pad = padstack.pads.front();
    }
  else {
    /* determine pin device name */
    size_t found = pin.pin_reference.find('.');
    if ((found == std::string::npos) || (found == 0)) return; /* device not found */
    std::string device_name = pin.pin_reference.substr(0, found);

    /* find device pin belongs to */
    bool device_found = false;
    for (Hyp2Mat::DeviceList::iterator i = pcb.device.begin(); i != pcb.device.end(); ++i) 
      if (device_name == i->ref) {
        device_found = true;
        layer_name = i->layer_name; /* device layer */
        }
    if (!device_found) return;
  
    /* we now know the device layer. Find this layer in the pin padstack. */
    bool pad_found = false;
    for (HypFile::PadList::iterator p = padstack.pads.begin(); p != padstack.pads.end(); ++p)
      if (p->layer_name == layer_name) {
        pad_found = true;
        pad = *p;
        }
    if (!pad_found) return;
    }

  /* find layer vertical position */
  for (Hyp2Mat::LayerList::iterator l = pcb.stackup.begin(); l != pcb.stackup.end(); ++l) 
    if (pad.layer_name == l->layer_name) {
      new_pin.layer_name = l->layer_name;
      new_pin.z0 = l->z0;
      new_pin.z1 = l->z1;
      }

  /* convert pad to polygon, and copy pad shape */
  HypFile::Polygon pad_poly = hyp_file.pad2poly(new_pin.x, new_pin.y, pad);
  new_pin.metal.width = 0;
  new_pin.metal.is_hole = false;
  new_pin.metal.vertex.clear();
  for (HypFile::PointList::iterator j = pad_poly.vertex.begin(); j != pad_poly.vertex.end(); ++j)
      new_pin.metal.vertex.push_back(Hyp2Mat::Point(j->x, j->y));
  
  /* add pin to pcb */
  pcb.pin.push_back(new_pin);
    
  return;
}

/*
 * Print all available nets
 */

void HyperLynx::PrintNets(HypFile::Hyp& hyp_file)
{
  std::vector<std::string> net_names;

  for (HypFile::NetList::iterator i = hyp_file.net.begin(); i != hyp_file.net.end(); ++i)
    net_names.push_back(i->net_name);
 
  sort(net_names.begin(), net_names.end()); 
  unique(net_names.begin(), net_names.end());

  std::cout << "nets:";
  for (std::vector<std::string>::iterator i = net_names.begin(); i != net_names.end(); ++i)
    std::cout << " " << *i;
  std::cout << std::endl;
    
  return;
}

/* not truncated */
