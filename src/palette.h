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
 * After Martin Ankerl "How to Generate Random Colors Programmatically"
 */

#ifndef PALETTE_H
#define PALETTE_H

struct PaletteColor {
  double red;
  double green;
  double blue;
  };

class Palette {
public:
  Palette();
  /* hue, saturation and value (brightness) are double, in range 0..1. (HSV color model) */
  void SetPalette(double hue, double saturation, double value); /* HSV model */
  /* Set the number of colors in the palette. If the number of colors is not known in advance, choose 0 */
  void SetNumberOfColors(unsigned int number_of_colors);
  /* Return palette color */
  PaletteColor Color(int color);

private:
  double palette_hue;
  double palette_saturation;
  double palette_value;
  double step;
  };

#endif

/* not truncated */
