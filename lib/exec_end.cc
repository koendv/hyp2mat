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
 * Hyperlynx 'END' record. 
 * Marks end of board description.
 */

bool HypFile::Hyp::exec_end(parse_param& h)
{
  if (trace_hyp) cerr << "end" << endl;

  if (warn_plane_layers) error("Board contains PLANE layers. Use SIGNAL layers for better accuracy.");
  if (warn_plane_polygons) error("Board contains PLANE polygons. Use POUR or COPPER polygons for better accuracy.");

  return false;
}

/*
 * Hyperlynx 'KEY' record. 
 */

bool HypFile::Hyp::exec_key(parse_param& h)
{
  if (trace_hyp) cerr << "key: key = " << h.key << endl;

  return false;
}

/* not truncated */
