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

#include "config.h"
#include "hyperlynx.h"
#include "polygon.h"
#include "crop.h"

HyperLynx::HyperLynx () 
{
  raw = false;
  arc_precision = 0;
  clearance = 0.0002;
}


/*
 * Read a pcb in hyperlynx format
 */

void HyperLynx::Read (std::string filename, Hyp2Mat::PCB& pcb)
{
  /*
   * load hyperlynx file in hyp_file 
   */

  bool success = LoadHypFile(pcb, filename, arc_precision);
  if (!success) throw ("load hyp file error");

  /*
   * Copy hyperlynx file data from hyp_file to pcb
   */
  saved_bounds = bounds;
  CopyBoard(pcb); /* copy board outline. crop board in x, y axis */

  CopyStackUp(pcb); /* copy layer stackup */
  bounds = AdjustBounds(pcb, saved_bounds);
  CopyVias(pcb); /* copy vias */
  CopyViaPlating(pcb); /* copy via plating thickness */
  CopyDevices(pcb); /* copy device info */
  CopyPins(pcb); /* copy pin references */

  bounds = AdjustBounds(pcb, saved_bounds);
  CropLayers(pcb, bounds); /* crop board in z axis */

  if (bounds.z_min == bounds.z_max) std::cerr << "warning: zero height board" << std::endl;

  /* 
   * Check whether to print all net names 
   */

  /* if one of the nets is "?", print list of all nets */
  if (std::find(nets.begin(), nets.end(), "?") != nets.end()) 
    PrintNets();

  return;
}

/*
 * Load Hyperlynx file in hyp_file
 */

bool HyperLynx::LoadHypFile(Hyp2Mat::PCB& pcb, std::string filename, double arc_precision)
{
  hyp_file.trace_scanner = (pcb.debug == 4);
  hyp_file.trace_parser = (pcb.debug == 3);
  hyp_file.trace_hyp = (pcb.debug == 2);

  if (pcb.debug != 0) {
    std::cerr << "hyp2mat " << VERSION << " using ClipperLib " << CLIPPER_VERSION << std::endl;
    }

  if (arc_precision < 0) {
    std::cerr << "arc precision negative" << std::endl;
    arc_precision = 0;
    }
  hyp_file.arc_precision = arc_precision;

  hyp_file.flood_layers = flood_layers;

  bool success = hyp_file.load(filename);

  if (pcb.debug == 1) {
    std::cerr << "writing hyp_file dump to hyp_debug.txt" << std::endl;
    hyp_file.save ("hyp_debug.txt");
    }

  return success;
}

/*
 * copy board outline into polygon "board" and crop according to "bounds".
 */

void HyperLynx::CopyBoard(Hyp2Mat::PCB& pcb)
{
  bool outer_edge = true;

  for (HypFile::PolygonList::iterator i = hyp_file.board.edge.begin(); i != hyp_file.board.edge.end(); ++i) {
    Hyp2Mat::Polygon poly;
    Hyp2Mat::FloatPolygon edge;
    for (HypFile::PointList::iterator j = i->vertex.begin(); j != i->vertex.end(); ++j)
      edge.push_back(Hyp2Mat::FloatPoint(j->x, j->y));
    poly.AddEdge(edge);
    if (outer_edge)
      board.Union(poly);
    else 
      board.Difference(poly);
    outer_edge = false;
  }

  /*
   * Cropping - Remove board outside bounds
   */

  pcb.board = board.Result();         /* store intermediate result */
  bounds = AdjustBounds(pcb, bounds); /* tighten bounds around board */
  board = CropPolygon(board, bounds); /* crop board. store board as integer polygon */
  pcb.board = board.Result();         /* board as floating point polygon */

  return;
}

/*
 * copy vias
 */

void HyperLynx::CopyVias(Hyp2Mat::PCB& pcb)
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

      /* cropping */
      if (!board.IsEmpty() && ((v.x > bounds.x_max) || (v.x < bounds.x_min) || (v.y > bounds.y_max) || (v.y < bounds.y_min))) continue; // Only crop in x and y if board outline is defined
      if ((v.z0 > bounds.z_max) && (v.z1 > bounds.z_max)) continue;
      if ((v.z0 < bounds.z_min) && (v.z1 < bounds.z_min)) continue;
      if (v.z1 > bounds.z_max) v.z1 = bounds.z_max;
      if (v.z0 < bounds.z_min) v.z0 = bounds.z_min;

      /* add to list of vias */
      pcb.via.push_back(v);
      } 
    }

  return;
}

/* 
 *  copy via plating
 */

void HyperLynx::CopyViaPlating(Hyp2Mat::PCB& pcb)
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

void HyperLynx::CopyDevices(Hyp2Mat::PCB& pcb)
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

void HyperLynx::CopyPins(Hyp2Mat::PCB& pcb)
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
          CopyPin(pcb, *j, *p);
    }

  return;
}

/*
 * copy a single pin
 */

void HyperLynx::CopyPin(Hyp2Mat::PCB& pcb, HypFile::Pin& pin, HypFile::Padstack& padstack)
{
  Hyp2Mat::Pin new_pin;
  new_pin.x = Hyp2Mat::Polygon::AlignToGrid(pin.x);
  new_pin.y = Hyp2Mat::Polygon::AlignToGrid(pin.y);
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
  new_pin.metal.clear();
  for (HypFile::PointList::iterator j = pad_poly.vertex.begin(); j != pad_poly.vertex.end(); ++j)
      new_pin.metal.push_back(Hyp2Mat::FloatPoint(Hyp2Mat::Polygon::AlignToGrid(j->x), Hyp2Mat::Polygon::AlignToGrid(j->y)));
  
  /* add pin to pcb */
  pcb.pin.push_back(new_pin);
    
  return;
}

/*
 * Print all available nets
 */

void HyperLynx::PrintNets()
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
