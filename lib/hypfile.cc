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
#include <iomanip> 
#include <sstream> 
#include <cstdio>
#include <cmath>
#include "hypfile.h"
#include "parser.h"

using namespace std;
using namespace HypFile;

HypFile::Hyp::Hyp()
  : trace_scanner(false), trace_parser(false), trace_hyp(false)
{
  unit = 1;
  metal_thickness_unit = 1;
  use_die_for_metal = false;

  min_circle_segments = 8; // 8 seems minimal, 16 seems more than sufficient.
  arc_precision = 0;
  pi = M_PI;
  inches  = 0.0254; /* inches to m */
  copper_imperial_weight = 1.341; /* metal thickness in ounces/ft2. 1 oz/ft2 copper = 1.341 mil */
  copper_metric_weight = 0.1116;  /* metal thickness in grams/cm2. 1 gr/cm2 copper = 0.1116 cm */
  copper_bulk_resistivity = 1.724e-8;
  copper_temperature_coefficient = 0.00393;
  fr4_epsilon_r = 4.3;
  fr4_loss_tangent = 0.020;
  conformal_epsilon_r = 3.3; /* dielectric constant of conformal layer */
  board.plane_separation = -1.0; /* distance between PLANE polygon and copper of different nets; -1 if not set */

}

/*
 * parse a Hyperlynx file
 */

bool HypFile::Hyp::load(const std::string &filename)
{
  if (trace_hyp) cerr << "parsing " << filename << endl;

  /* open input file */
  if (filename != "-") {
    yyin = fopen (filename.c_str(),"r");
    if (!yyin) {
      cerr << "Can't open input file " << filename << endl;
      return false;
    }
  }

  /* parse input file */
  yy_flex_debug = trace_scanner;
  yydebug = trace_parser;
  int retval = yyparse(this);

  /* close input file */
  fclose(yyin);

  bool success = retval == 0;  
  return success;
}

/*
 * Error output
 */

void HypFile::Hyp::error(const std::string& msg)
{
  cerr << msg << endl;
}

/*
 * Dumps internal structures for debugging.
 */

bool HypFile::Hyp::save(const std::string& filename)
{
  /*
   * Open file for output
   */

  if ((filename != "-") && (freopen(filename.c_str(), "w", stdout) == NULL)) {
    std::ostringstream err_msg;
    err_msg << "could not open '" << filename << "' for writing";
    error(err_msg.str());
    return true;
    }
   
  /*
   * Variables
   */

  cout << "unit = " << unit << endl;
  cout << "metal_thickness_unit = " << metal_thickness_unit << endl;
  cout << "use_die_for_metal = " << use_die_for_metal << endl;
  cout << "min_circle_segments = " << min_circle_segments << endl;
  cout << "arc_precision = " << arc_precision << endl;
  cout << "pi = " << pi << endl;
  cout << "inches = " << inches << endl;
  cout << "copper_imperial_weight = " << copper_imperial_weight << endl;
  cout << "copper_metric_weight = " << copper_metric_weight << endl;
  cout << "copper_bulk_resistivity = " << copper_bulk_resistivity << endl;
  cout << "copper_temperature_coefficient = " << copper_temperature_coefficient << endl;
  cout << "fr4_epsilon_r = " << fr4_epsilon_r << endl;
  cout << "fr4_loss_tangent = " << fr4_loss_tangent << endl;
  cout << "conformal_epsilon_r = " << conformal_epsilon_r << endl;

  /*
   * Export board
   */

  cout << "board" << endl;
  for (PolygonList::iterator i = board.edge.begin(); i != board.edge.end(); ++i) {
    cout << " polygon" << endl;
    cout << "  positive " << i->positive << endl;
    for (PointList::iterator j = i->vertex.begin(); j != i->vertex.end(); ++j) {
      cout << "  x = " << j->x << endl;
      cout << "  y = " << j->y << endl;
    }
  }

  cout << " plane_separation = " << board.plane_separation << endl;
  cout << " board_attribute" << endl;
  for (AttributeList::iterator j = board.attribute.begin(); j != board.attribute.end(); ++j) {
    cout << "  name = '" << j->name << "'" << endl;
    cout << "  value = '" << j->value << "'" << endl;
    }

  /*
   * Export devices
   */

  cout << "device" << endl;
  for (DeviceList::iterator dev_it = device.begin(); dev_it != device.end(); ++dev_it) {
    cout << " device" << endl;
    cout << "  device_type = '" << dev_it->device_type << "'" << endl;
    cout << "  ref = '" << dev_it->ref << "'" << endl;
    cout << "  name = '" << dev_it->name << "'" << endl;
    cout << "  value_float = " << dev_it->value_float << endl;
    cout << "  value_float_set = " << dev_it->value_float_set << endl;
    cout << "  value_string = '" << dev_it->value_string << "'" << endl;
    cout << "  value_string_set = " << dev_it->value_string_set << endl;
    cout << "  layer_name = '" << dev_it->layer_name << "'" << endl;
    cout << "  package = '" << dev_it->package << "'" << endl;
    }

  /*
   * Export supplies
   */

  cout << "supplies" << endl;
  for (SupplyList::iterator supp_it = supply.begin(); supp_it != supply.end(); ++supp_it) {
    cout << " supply" << endl;
    cout << "  name = '" << supp_it->name << "'" << endl;
    cout << "  value_float = " << supp_it->value_float << endl;
    cout << "  voltage_specified = " << supp_it->voltage_specified << endl;
    cout << "  conversion = " << supp_it->conversion << endl;
  }
 
  /*
   * Export stackup
   */

  cout << "layers" << endl;
  for (LayerList::iterator it = stackup.begin(); it != stackup.end(); ++it) {
    cout << " layer" << endl;
    cout << "  layer_name = '" << it->layer_name << "'" << endl;
    cout << "  layer_type = " << it->layer_type << " ";
    switch (it->layer_type) {
      case LAYER_SIGNAL:     cout << "LAYER_SIGNAL"; break; 
      case LAYER_DIELECTRIC: cout << "LAYER_DIELECTRIC"; break; 
      case LAYER_PLANE:      cout << "LAYER_PLANE"; break; 
      default:               cout << "Error"; break;
      }
    cout << endl;

    cout << "  bulk_resistivity = " << it->bulk_resistivity << endl;
    cout << "  conformal = " << it->conformal << endl;
    cout << "  epsilon_r = " << it->epsilon_r << endl;
    cout << "  layer_name = " << it->layer_name << endl;
    cout << "  loss_tangent = " << it->loss_tangent << endl;
    cout << "  material_name = " << it->material_name << endl;
    cout << "  plane_separation = " << it->plane_separation << endl;
    cout << "  plating_thickness = " << it->plating_thickness << endl;
    cout << "  prepreg = " << it->prepreg << endl;
    cout << "  temperature_coefficient = " << it->temperature_coefficient << endl;
    cout << "  thickness = " << it->thickness << endl;
    cout << "  z0 = " << it->z0 << endl;
    cout << "  z1 = " << it->z1 << endl;
    }

  /*
   * Padstack
   */

  cout << "padstacks" << endl;
  for (PadstackList::iterator it = padstack.begin(); it != padstack.end(); ++it) {
    cout << " padstack" << endl;
    cout << "  padstack_name = '" << it->padstack_name << "'" << endl;
    cout << "  drill_size = " << it->drill_size << endl;
    cout << "  pads" << endl;
    for (PadList::iterator p = it->pads.begin(); p != it->pads.end(); ++p) {
      cout << "   pad" << endl;
      cout << "    layer_name = '" << p->layer_name << "'" << endl;
      cout << "    pad_type = " << p->pad_type << " ";
      switch (p->pad_type) {
        case PAD_TYPE_METAL:          cout << "PAD_TYPE_METAL"; break; 
        case PAD_TYPE_ANTIPAD:        cout << "PAD_TYPE_ANTIPAD"; break; 
        case PAD_TYPE_THERMAL_RELIEF: cout << "PAD_TYPE_THERMAL_RELIEF"; break; 
        default:                      cout << "Error"; break;
      }
      cout << endl;
      cout << "    pad_shape = " << p->pad_shape << " ";
      switch (p->pad_shape) {
        case PAD_SHAPE_OVAL:          cout << "PAD_SHAPE_OVAL"; break; 
        case PAD_SHAPE_RECTANGULAR:   cout << "PAD_SHAPE_RECTANGULAR"; break; 
        case PAD_SHAPE_OBLONG:        cout << "PAD_SHAPE_OBLONG"; break; 
        default:                      cout << "Error"; break;
      }
      cout << endl;
      cout << "    pad_sx = " << p->pad_sx << endl;
      cout << "    pad_sy = " << p->pad_sy << endl;
      cout << "    pad_angle = " << p->pad_angle << endl;
      cout << "    thermal_clear_shape = " << p->thermal_clear_shape << " ";
      switch (p->thermal_clear_shape) {
        case PAD_SHAPE_OVAL:          cout << "PAD_SHAPE_OVAL"; break; 
        case PAD_SHAPE_RECTANGULAR:   cout << "PAD_SHAPE_RECTANGULAR"; break; 
        case PAD_SHAPE_OBLONG:        cout << "PAD_SHAPE_OBLONG"; break; 
        default:                      cout << "Error"; break;
      }
      cout << endl;
      cout << "    thermal_clear_sx = " << p->thermal_clear_sx << endl;
      cout << "    thermal_clear_sy = " << p->thermal_clear_sy << endl;
      cout << "    thermal_clear_angle = " << p->thermal_clear_angle << endl;
      }
    }

  /*
   * Net
   */

  cout << "net" << endl;
  for (NetList::iterator i = net.begin(); i != net.end(); ++i) {
    cout << " net" << endl;
    cout << "  net_name = '" << i->net_name << "'" << endl;

    /* 
     * Net Polygons 
     */
    for (PolygonMap::iterator j =  i->metal.begin(); j != i->metal.end(); ++j) {
      cout << "  polygon id " << j->first << endl;
      for (PolygonList::iterator k = j->second.begin(); k != j->second.end(); ++k) {
        cout << "   polygon" << endl;
        cout << "    positive " << k->positive << endl;
        cout << "    width " << k->width << endl;
        cout << "    layer_name " << k->layer_name << endl;
        for (PointList::iterator l = k->vertex.begin(); l != k->vertex.end(); ++l) {
          cout << "    x = " << l->x << endl;
          cout << "    y = " << l->y << endl;
        }
      }
    }
  
    /* 
     * Net Vias 
     */
  
    cout << "  vias" << endl;
    for (ViaList::iterator j = i->via.begin(); j != i->via.end(); ++j) {
      cout << "   via" << endl;
      cout << "    x = " << j->x << endl;
      cout << "    y = " << j->y << endl;
      cout << "    layer0_name = " << j->layer0_name << endl;
      cout << "    layer1_name = " << j->layer1_name << endl;
      cout << "    radius = " << j->radius << endl;
      }
  
    /* 
     * Net Pins 
     */
  
    cout << "  pins" << endl;
    for (PinList::iterator j = i->pin.begin(); j != i->pin.end(); ++j) {
      cout << "   pin" << endl;
      cout << "    x = " << j->x << endl;
      cout << "    y = " << j->y << endl;
      cout << "    pin_reference = " << j->pin_reference << endl;
      cout << "    padstack_name = " << j->padstack_name << endl;
      cout << "    pin_function = " << j->pin_function << " ";
        switch (j->pin_function) {
          case PIN_SIM_IN:          cout << "PIN_SIM_IN"; break; 
          case PIN_SIM_OUT:         cout << "PIN_SIM_OUT"; break; 
          case PIN_SIM_BOTH:        cout << "PIN_SIM_BOTH"; break; 
          default:                  cout << "Error"; break;
        }
      cout << endl;
      }
  
    /* 
     * Net Unrouted segments 
     */
  
    cout << "  unrouted_segments" << endl;
    for (UnroutedSegmentList::iterator j = i->unrouted_segment.begin(); j != i->unrouted_segment.end(); ++j) {
      cout << "   unrouted_segment" << endl;
      cout << "    x1 = " << j->x1 << endl;
      cout << "    y1 = " << j->y1 << endl;
      cout << "    layer1_name = " << j->layer1_name << endl;
      cout << "    x2 = " << j->x2 << endl;
      cout << "    y2 = " << j->y2 << endl;
      cout << "    layer2_name = " << j->layer2_name << endl;
      cout << "    zlayer_name_set = " << j->zlayer_name_set << endl;
      cout << "    zlayer_name = " << j->zlayer_name << endl;
      cout << "    width = " << j->width << endl;
      cout << "    impedance_set = " << j->impedance_set << endl;
      cout << "    impedance = " << j->impedance << endl;
      cout << "    delay = " << j->delay << endl;
      cout << "    resistance = " << j->resistance << endl;
      }
    
    /* 
     * Net Attributes
     */

    if (!i->attribute.empty()) cout << "  attribute" << endl;
    for (AttributeList::iterator j = i->attribute.begin(); j != i->attribute.end(); ++j) {
      cout << "   name = '" << j->name << "'" << endl;
      cout << "   value = '" << j->value << "'" << endl;
      }
    }

  /*
   * Net Class
   */

  cout << "net_class" << endl;
  for (NetClassList::iterator i = net_class.begin(); i != net_class.end(); ++i) {
    cout << " net_class" << endl;
    cout << "  net_class_name = '" << i->net_class_name << "'" << endl;
    cout << "  net_class_element" << endl;
    for (StringList::iterator j = i->net_class_element.begin(); j != i->net_class_element.end(); ++j) {
      cout << "   net_name = '" << *j << "'" << endl;
      }
    if (!i->attribute.empty()) cout << "  attribute" << endl;
    for (AttributeList::iterator j = i->attribute.begin(); j != i->attribute.end(); ++j) {
      cout << "   name = '" << j->name << "'" << endl;
      cout << "   value = '" << j->value << "'" << endl;
      }
    }

  cout << "end" << endl;

  fclose(stdout);

  return true;
}

/* not truncated */
