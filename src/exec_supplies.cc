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
 * Hyperlynx 'S' subrecord of 'SUPPLIES' record. 
 * Specifies a power supply net.
 */

bool HypFile::Hyp::exec_supplies(parse_param& h)
{
  if (trace_hyp) {
    cerr << "supplies: name = " << h.name;
    cerr << " value_float = " << h.value_float;
    cerr << " voltage_specified = " << h.voltage_specified;
    cerr << " conversion = " << h.conversion;
    cerr << endl;
    }

  Supply s;

  s.name = h.name;

  s.value_float = h.value_float;

  s.voltage_specified = h.voltage_specified;

  s.conversion = h.conversion;

  supply.push_back(s);

  return false;
}

/* not truncated */
