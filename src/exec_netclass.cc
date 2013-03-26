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
 * Hyperlynx 'NET_CLASS' record. 
 * Defines a class of nets.
 */

bool HypFile::Hyp::exec_net_class(parse_param& h)
{
  if (trace_hyp) cerr << "net_class: net_class_name = " << h.net_class_name << endl;

  NetClass nc;
  nc.net_class_name = h.net_class_name;
  net_class.push_back(nc);

  return false;
}

/*
 * Hyperlynx 'A' attribute subrecord of 'NET_CLASS' record.
 * Defines net class attributes as name/value pairs.
 */

bool HypFile::Hyp::exec_net_class_attribute(parse_param& h)
{
  if (trace_hyp) cerr << "netclass_attribute: name = " << h.name  << " value = " << h.value << endl;

  Attribute a;
  a.name = h.name;
  a.value = h.value;

  net_class.back().attribute.push_back(a);

  return false;
}

/*
 * Hyperlynx 'N' subrecord of 'NET_CLASS' record. 
 * Defines membership of a net class.
 */

bool HypFile::Hyp::exec_net_class_element(parse_param& h)
{
  if (trace_hyp) cerr << "net_class_element: net_name = " << h.net_name << endl;

  if (!net_class.empty())
    net_class.back().net_class_element.push_back(h.net_name); 

  return false;
}

/* not truncated */
