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
 * Hyperlynx 'DEVICE' record. 
 * Specifies pcb components: resistors, capacitors, ICs, ...
 * Required record.
 */

bool HypFile::Hyp::exec_devices(parse_param& h)
{
  if (trace_hyp) {
    cerr << "device: ";
    cerr << "device_type = " << h.device_type;
    cerr << " ref = " << h.ref ;
    if (h.name_set) cerr << " name = " << h.name;
    if (h.value_float_set) cerr << " value_float = " << h.value_float;
    if (h.value_string_set) cerr << " value_string = " << h.value_string;
    if (h.layer_name_set) cerr << " layer_name = " << h.layer_name;
    if (h.package_set) cerr << " package = " << h.package;
    cerr << endl;
    }

  Device d;
 
  d.device_type = h.device_type; 

  d.ref = h.ref;

  if (h.name_set)
    d.name = h.name;
  else
    d.name = "";

  if (h.value_float_set)
    d.value_float = h.value_float;
  else 
    d.value_float = 0;
   
  d.value_float_set = h.value_float_set;

  if (h.value_string_set)
    d.value_string = h.value_string;
  else
    d.value_string = "";
 
  d.value_string_set = h.value_string_set;

  if (h.layer_name_set)
    d.layer_name = h.layer_name;
  else {
    error ("device layer not set");
    return true;
    }

  if (h.package_set)
    d.package = h.package;
  else
    d.package = "";

  device.push_back(d);

  return false;
}

/* not truncated */
