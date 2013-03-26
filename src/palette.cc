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
 * Generate a palette of colors, given hue, saturation and value (brightness).
 * The number of colors needed may be specified, but does not need to be known in advance.
 * After Martin Ankerl * "How to Generate Random Colors Programmatically"
 */

#include <cmath>
#include "palette.h"

Palette::Palette()
{
  /* default values */
  palette_hue = 0.69; /* blue */
  palette_saturation = 0.99;
  palette_value = 0.99;
  step = (sqrt(5) - 1)/2; /* golden ratio conjugate */ 
}

void Palette::SetPalette(double hue, double saturation, double value)
{
  /* set user values */
  if ((0 <= hue) && (hue <= 1)) palette_hue = hue;
  if ((0 <= saturation) && (saturation <= 1)) palette_saturation = saturation;
  if ((0 <= value) && (value <= 1)) palette_value = value;
  return;
}

void Palette::SetNumberOfColors(unsigned int number_of_colors)
{
  if (number_of_colors > 0) step = 1 / (double) number_of_colors;
  else step = (sqrt(5) - 1)/2; /* golden ratio conjugate */;
  return;
}

PaletteColor Palette::Color(int color)
{

  /* hue, saturation and value (brightness) are double, in range 0..1. (HSV color model) */  

  /* calculate hue of color */
  double integer_part; 
  double h = modf(palette_hue + color * step, &integer_part);

  /* convert hsv color model to rgb */
  double hi;
  double f = modf(h * 6, &hi);
  double v = palette_value;
  double p = v * (1 - palette_saturation);
  double q = v * (1 - f * palette_saturation);
  double t = v * (1 - (1 - f) * palette_saturation);

  double red, green, blue;
  switch(static_cast<int>(hi + 0.5)) {
    case 0: 
      red = v;
      green = t;
      blue = p;
      break;
    case 1: 
      red = q;
      green = v;
      blue = p;
      break;
    case 2: 
      red = p;
      green = v;
      blue = t;
      break;
    case 3: 
      red = p;
      green = q;
      blue = v;
      break;
    case 4: 
      red = t;
      green = p;
      blue = v;
      break;
    case 5: 
      red = v;
      green = p;
      blue = q;
      break;
    default:
      red = 0;
      green = 0;
      blue = 0;
      break;
    }

  PaletteColor clr;
  clr.red = red;
  clr.green = green;
  clr.blue = blue;

  return clr;
  
}

/* not truncated */
