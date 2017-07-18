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
 * Hyperlynx 'PADSTACK' record. 
 * Defines a padstack.
 */

/*
 * Hyperlynx padstack-element subrecord of 'PADSTACK' record. 
 * Defines an individual pad in a padstack.
 */

bool HypFile::Hyp::exec_padstack_element(parse_param& h)
{
  if (trace_hyp) {
    cerr << "padstack_element:";
    if (h.padstack_name_set) cerr << " padstack_name = " << h.padstack_name;
    if (h.drill_size_set) cerr << " drill_size = " << h.drill_size;
    cerr << " layer_name = " << h.layer_name;
    cerr << " pad_shape = " << h.pad_shape;
    cerr << " pad_sx = " << h.pad_sx;
    cerr << " pad_sy = " << h.pad_sy;
    cerr << " pad_angle = " << h.pad_angle;
    if (h.pad_type_set & (h.pad_type == PAD_TYPE_THERMAL_RELIEF)) {
      cerr << " thermal_clear_shape = " << h.thermal_clear_shape;
      cerr << " thermal_clear_sx = " << h.thermal_clear_sx;
      cerr << " thermal_clear_sy = " << h.thermal_clear_sy;
      cerr << " thermal_clear_angle = " << h.thermal_clear_angle;
      }
    if (h.pad_type_set) cerr << " pad_type = " << h.pad_type;
    cerr << endl;
    }

  h.drill_size *= unit;
  h.pad_sx *= unit;
  h.pad_sy *= unit;
  h.thermal_clear_sx *= unit;
  h.thermal_clear_sy *= unit;

  /* new padstack */
  if (h.padstack_name_set) {
    Padstack pdstack;
    pdstack.padstack_name = h.padstack_name;
    if (h.drill_size_set) 
      pdstack.drill_size = h.drill_size;
    else
      pdstack.drill_size = 0;

    /* add new padstack to list of padstacks */
    padstack.push_back(pdstack);
    }

  /* new pad */
  Pad p;

  p.layer_name = h.layer_name;

  /* pad type */
  if (h.pad_type_set) p.pad_type = h.pad_type;
  else if (p.layer_name == "ADEF") p.pad_type = PAD_TYPE_ANTIPAD;
  else p.pad_type = PAD_TYPE_METAL;

  if (h.pad_shape == 0) 
    p.pad_shape = PAD_SHAPE_OVAL;
  else if (h.pad_shape == 1) 
    p.pad_shape = PAD_SHAPE_RECTANGULAR;
  else if (h.pad_shape == 2) 
    p.pad_shape = PAD_SHAPE_OBLONG;
  else if (h.pad_shape == -1) 
    p.pad_shape = PAD_SHAPE_OVAL; /* workaround for Altium bug */
  else {
    error("unknown pad shape");
    return true;
    }

  p.pad_sx = h.pad_sx;
  p.pad_sy = h.pad_sy;
  p.pad_angle = h.pad_angle;

  if (p.pad_type == PAD_TYPE_THERMAL_RELIEF) {
    if (h.thermal_clear_shape == 0) 
      p.thermal_clear_shape = PAD_SHAPE_OVAL;
    else if (h.thermal_clear_shape == 1) 
      p.thermal_clear_shape = PAD_SHAPE_RECTANGULAR;
    else if (h.thermal_clear_shape == 2) 
      p.thermal_clear_shape = PAD_SHAPE_OBLONG;
    else {
      error("unknown thermal clear shape");
      return true;
      }
  
    p.thermal_clear_sx = h.thermal_clear_sx;
    p.thermal_clear_sy = h.thermal_clear_sy;
    p.thermal_clear_angle = h.thermal_clear_angle;
  }
  else {
    p.thermal_clear_shape = PAD_SHAPE_OVAL;
    p.thermal_clear_sx = 0;
    p.thermal_clear_sy = 0;
    p.thermal_clear_angle = 0;
  }

  /* add new pad to list of pads of current padstack */
  padstack.back().pads.push_back(p);

  return false;
}

/*
 * Note: 
 * MDEF and ADEF layer names have special meaning.
 * MDEF specified the default pad for signal or plane layers. 
 * If a signal or plane layer has no metal pad, and an MDEF default pad is specified, the MDEF pad is applied to the layer.
 * ADEF specifies the default anti-pad (non-conducting hole in the copper) for plane layers.
 * If a plane layer has no anti-pad, and an ADEF default pad is specified, the ADEF pad is applied to the layer.
 */

bool HypFile::Hyp::exec_padstack_end(parse_param& h)
{
  if (trace_hyp) {
    cerr << "padstack_end:";
    cerr << endl;
    };

  PadList new_pads;

  /* 
   * Loop through stackup. 
   * Calculate new padstack, where all MDEF and ADEF pads have been expanded into one or more metal layers.
   */

  for (LayerList::iterator l = stackup.begin(); l != stackup.end(); ++l) {
    /* Loop through pad list */
    bool layer_has_pad = false;
    bool layer_has_antipad = false;
    for (PadList::iterator p = padstack.back().pads.begin(); p != padstack.back().pads.end(); ++p) {
      if (l->layer_name == p->layer_name) {
        new_pads.push_back(*p);
        layer_has_pad = layer_has_pad || (p->pad_type == PAD_TYPE_METAL);
        layer_has_antipad = layer_has_antipad || (p->pad_type == PAD_TYPE_ANTIPAD);
        }
      }
      
    /* If a plane layer has no antipad, add ADEF pad if specified */
    if (!layer_has_antipad && (l->layer_type == LAYER_PLANE)) 
      for (PadList::iterator p = padstack.back().pads.begin(); p != padstack.back().pads.end(); ++p)
        if (p->layer_name == "ADEF") {
          Pad new_pad = *p;
          new_pad.layer_name = l->layer_name; /* change ADEF into current layer name */
          new_pads.push_back(new_pad);
          }

    /* If a signal or plane layer has no pad, add MDEF pad if specified */
    if (!layer_has_pad && ((l->layer_type == LAYER_SIGNAL) || (l->layer_type == LAYER_PLANE)) )
      for (PadList::iterator p = padstack.back().pads.begin(); p != padstack.back().pads.end(); ++p)
        if (p->layer_name == "MDEF") {
          Pad new_pad = *p;
          new_pad.layer_name = l->layer_name; /* change MDEF into current layer name */
          new_pads.push_back(new_pad);
          layer_has_pad = true;
          }

    /* 
     * Determine begin and end layers of via
     */

    /* via is from top to bottom if at least one pad is metal MDEF */
    bool is_through_hole_via = false;
    for (PadList::iterator p = padstack.back().pads.begin(); p != padstack.back().pads.end(); ++p)
      if ((p->layer_name == "MDEF") && (p->pad_type == PAD_TYPE_METAL)) is_through_hole_via = true;

    /* determine via top and bottom layers */
    std::string pad_top_layer;
    std::string pad_bottom_layer;
   
    if (is_through_hole_via) {
      pad_top_layer = stackup.front().layer_name; /* via begins at top layer */
      pad_bottom_layer = stackup.back().layer_name; /* via ends at bottom layer */
      }
    else {
      /* via begins at highest layer with metal pad, and ends at lowest layer with a metal pad */
      bool layer_found = false;
      for (LayerList::iterator l = stackup.begin(); l != stackup.end(); ++l)
        for (PadList::iterator p = padstack.back().pads.begin(); p != padstack.back().pads.end(); ++p)
          if ((l->layer_name == p->layer_name) && (p->pad_type == PAD_TYPE_METAL)) {
            if (!layer_found) {
              pad_top_layer = l->layer_name;
              layer_found = true;
              }
            pad_bottom_layer = l->layer_name;
            };

      if (!layer_found) {
        std::cerr << "warning: padstack without metal: " << padstack.back().padstack_name << std::endl;
        pad_top_layer = stackup.front().layer_name; /* via begins at top layer */
        pad_bottom_layer = stackup.back().layer_name; /* via ends at bottom layer */
        }
      }

    /*
     * Fill in missing via pads
     */

    // XXX Fixme
    // XXX Check if loops need to begin at pad_top_layer / end at pad_bottom_layer
    // XXX Check if pad_top_layer / pad_bottom_layer need visibility
    // XXX Check if need mechanism to remove pads?

    /* If a signal or plane layer has no pad, and no MDEF pad, copy an existing pad */
    bool is_through_hole_pad = is_through_hole_via && (padstack.back().drill_size > 0);
    if (!layer_has_pad && is_through_hole_pad && ((l->layer_type == LAYER_SIGNAL) || (l->layer_type == LAYER_PLANE)) ) {
      Pad new_pad;
      bool pad_found = false;
      for (PadList::iterator p = padstack.back().pads.begin(); p != padstack.back().pads.end(); ++p) {
        if (p->pad_type != PAD_TYPE_METAL) continue;
        if (!pad_found) {
          /* if all padstack pads are equal, create a pad on this layer which is like the others */
          new_pad = *p;
          new_pad.layer_name = l->layer_name;
          pad_found = true;
          }
        else if ((p->pad_shape != new_pad.pad_shape) || (p->pad_sx != new_pad.pad_sx) || (p->pad_sy != new_pad.pad_sy) || (p->pad_angle != new_pad.pad_angle)) {
          /* if padstack pads are different, create a circular pad with a diameter equal to the smallest pad dimension */
          double radius = new_pad.pad_sx;
          if (radius > new_pad.pad_sy) radius = new_pad.pad_sy;
          if (radius > p->pad_sx) radius = p->pad_sx;
          if (radius > p->pad_sy) radius = p->pad_sy;
          new_pad.pad_shape = PAD_SHAPE_OVAL;
          new_pad.pad_sx = radius;
          new_pad.pad_sy = radius;
          new_pad.pad_angle = 0;
          }
        }
      /* add newly created pad to padstack */
      if (pad_found) new_pads.push_back(new_pad);
      }

    }

  /* put new padstack in place. */
  
  padstack.back().pads = new_pads;

  return false;
 
}

/* not truncated */
