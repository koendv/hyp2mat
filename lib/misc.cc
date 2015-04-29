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

#include <limits>

#include "config.h"
#include "hyp2mat.h"

using namespace Hyp2Mat;

std::string Hyp2Mat::version()
{
  return VERSION;
}

Device::Device()
{
  name.clear();
  ref.clear();
  value_type = DEVICE_VALUE_NONE;
  value_float = -1;
  value_string.clear();
}

Pin::Pin()
{
  ref.clear();
  x = 0;
  y = 0;
  z0 = 0;
  z1 = 0;
  layer_name.clear();
  metal.clear();
}

Bounds::Bounds()
{
  /* bounds default to infinity */
  double dbl_max = std::numeric_limits<double>::max();
  x_min = - dbl_max;
  x_max =   dbl_max;
  y_min = - dbl_max;
  y_max =   dbl_max;
  z_min = - dbl_max;
  z_max =   dbl_max;
}

/* not truncated */
