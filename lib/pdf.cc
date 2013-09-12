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
 * Uses libharu by Takeshi Kanno to generate pdf documents.
 */

#include <iostream>
#include <cstdio>
#include <algorithm>
#include "pdf.h"
#include <hpdf_types.h>

using namespace Hyp2Mat;

PDF::PDF()
{
  text_height = 10; /* caption size in points */
  margin = text_height; /* page margin */
  m_to_points = 1 / (2.54 / 100 / 72); /* convert m to points (1/72 of an inch) */
  x_max = x_min = y_max = y_min = 0;
}

/*
 * PDF error handler
 */

void error_handler (HPDF_STATUS error_no, HPDF_STATUS detail_no, void *user_data)
{
    printf ("ERROR: error_no=%04X, detail_no=%d\n", (unsigned int) error_no, (int) detail_no); 
    throw std::exception (); /* throw exception on error */
}

/*
 * set page size to size of pcb board 
 */

void PDF::set_page_size(HPDF_Page page)
{
  double width = x_max - x_min + 2 * margin;
  double height = y_max - y_min + 2 * margin + text_height; /* add margin for caption */

  if (width < 3) width = 3;
  if (height < 3) height = 3;

  HPDF_Page_SetWidth(page, width);
  HPDF_Page_SetHeight(page, height);
}

/*
 * Generate color palette given hue, saturation and value (brightness). 
 * After Martin Ankerl * "How to Generate Random Colors Programmatically"
 */

void PDF::set_color(HPDF_Page page, int color_idx)
{
  PaletteColor rgb = Color(color_idx);

  HPDF_Page_SetRGBStroke(page, rgb.red, rgb.green, rgb.blue);
  HPDF_Page_SetRGBFill(page, rgb.red, rgb.green, rgb.blue);
}

/*
 * Set stroke and fill color to black
 */

void PDF::set_color_black(HPDF_Page page)
{
  HPDF_Page_SetRGBStroke(page, 0, 0, 0);
  HPDF_Page_SetRGBFill(page, 0, 0, 0);
}

/*
 * fill all polygons 
 */

void PDF::page_fill(HPDF_Page page)
{
  if (HPDF_Page_GetGMode(page) == HPDF_GMODE_PATH_OBJECT) HPDF_Page_Fill (page);
  return;
}

/*
 * draw all polygon outlines
 */

void PDF::page_stroke(HPDF_Page page)
{
  if (HPDF_Page_GetGMode(page) == HPDF_GMODE_PATH_OBJECT) HPDF_Page_Stroke (page);
  return;
}

/*
 * Write layer name on top of page
 */

void PDF::draw_caption(HPDF_Page page, HPDF_Font font, std::string txt)
{
  HPDF_Page_SetFontAndSize(page, font, text_height);
  HPDF_Page_BeginText(page);
  HPDF_Page_MoveTextPos(page, margin, y_max - y_min + margin + text_height); /* page top left */
  HPDF_Page_ShowText(page, txt.c_str());
  HPDF_Page_EndText(page);
}

/*
 * Export copper polygon as pdf
 */

void PDF::draw(HPDF_Page page, FloatPolygon& poly)
{
  double old_px, old_py;

  /* pdf requires outer edge of polygon to be clockwise; holes to be counterclockwise */
    
  bool firstpoint = true;
  for (FloatPolygon::iterator i = poly.begin(); i != poly.end(); ++i) {

    double px = i->x * m_to_points - x_min + margin;
    double py = i->y * m_to_points - y_min + margin;
    if (firstpoint) {
      HPDF_Page_MoveTo(page, px, py);
      firstpoint = false;
      }
    else {
      if ((px != old_px) || (py != old_py))
        HPDF_Page_LineTo(page, px, py);
      }
    old_px = px; old_py = py;
    }
  HPDF_Page_ClosePath (page);
  return;
}

/*
 * Export all polygons of a copper layer as pdf. May contain holes.
 */

void PDF::draw(HPDF_Page page, FloatPolygons& polygons)
{
  for (FloatPolygons::iterator i = polygons.begin(); i != polygons.end(); ++i)
    draw(page, i->poly);
}

/*
 * Draws a via as a circle.
 */

void PDF::draw(HPDF_Page page, Hyp2Mat::Via& via)
{
  double x = via.x * m_to_points - x_min + margin;
  double y = via.y * m_to_points - y_min + margin;
  double r = via.radius * m_to_points;
  HPDF_Page_Circle(page, x, y, r);
}

void PDF::draw_composite_view(HPDF_Doc pdf, HPDF_Font font, Hyp2Mat::PCB& pcb)
{
  /* new page */
  HPDF_Page page = HPDF_AddPage(pdf);
  set_page_size(page);
  HPDF_Page_SetLineWidth(page, 0);
  draw_caption(page, font, "Composite");

  /* draw board */
  set_color_black(page);
  draw(page, pcb.board);
  page_stroke (page);

  /* draw layers */
  int current_color = 0;
  for (LayerList::reverse_iterator l = pcb.stackup.rbegin(); l != pcb.stackup.rend(); ++l) {
    if (l->layer_type == LAYER_DIELECTRIC) continue;
    set_color(page, current_color++);
    draw(page, l->metal);
    page_stroke (page);
  }

  /* draw vias */
  set_color_black(page);
  for (ViaList::iterator i = pcb.via.begin(); i != pcb.via.end(); ++i)
    PDF::draw(page, *i);
  page_fill (page);

  return;
}

void PDF::draw_layer_view(HPDF_Doc pdf, HPDF_Font font, Hyp2Mat::PCB& pcb)
{

  int current_color = 0;
  for (LayerList::reverse_iterator l = pcb.stackup.rbegin(); l != pcb.stackup.rend(); ++l) {
    if (l->layer_type == LAYER_DIELECTRIC) continue;

    HPDF_Page page = HPDF_AddPage(pdf);
    set_page_size(page);
    HPDF_Page_SetLineWidth(page, 0);

    /* draw caption */
    draw_caption(page, font, l->layer_name);

    /* draw board */
    set_color_black(page);
    draw(page, pcb.board);
    page_stroke (page);

    /* draw layer */
    set_color(page, current_color++);
    draw(page, l->metal);
    page_fill (page);

    /* draw vias */
    set_color_black(page);
    for (ViaList::iterator i = pcb.via.begin(); i != pcb.via.end(); ++i) {
      /* blind vias: only draw via if via goes through current layer */
      if ((i->z0 <= l->z0) && (i->z1 >= l->z1)) {
        draw(page, *i);
        }
      }
    page_fill (page);
    }
}

/*
 * Save current board as pdf. 
 * First page is the composite view of all signal layers,
 * subsequent pages are one layer each.
 */

void PDF::Write(const std::string& filename, Hyp2Mat::PCB& pcb)
{
  /* choose a color palette. Change HSV values to what suits your taste */
  int number_of_colors = 0;
  for (LayerList::reverse_iterator l = pcb.stackup.rbegin(); l != pcb.stackup.rend(); ++l)
    if (l->layer_type != LAYER_DIELECTRIC) number_of_colors++; /* count layers to determine number of colors needed */
  
  SetNumberOfColors(number_of_colors);

  HPDF_Doc pdf = HPDF_New (error_handler, NULL); /* set error-handler */
  if (!pdf) {
    std::cerr << "cannot create pdf object" << std::endl;
    return;
    }

  try {
    /* get pcb size */
    Bounds bounds = pcb.GetBounds();
    x_max = bounds.x_max * m_to_points;
    y_max = bounds.y_max * m_to_points;
    x_min = bounds.x_min * m_to_points;
    y_min = bounds.y_min * m_to_points;

    /* Document settings */
    HPDF_Font font = HPDF_GetFont(pdf, "Helvetica", NULL);
    HPDF_SetInfoAttr(pdf, HPDF_INFO_CREATOR, "hyp2mat");
    HPDF_SetCompressionMode (pdf, HPDF_COMP_ALL);
    
    /* Draw composite page */
    draw_composite_view(pdf, font, pcb);

    /* Draw individual layers' pages */
    draw_layer_view(pdf, font, pcb);

    HPDF_SaveToFile (pdf, filename.c_str());
    } 
  catch (...) {
    HPDF_Free (pdf);
    return;
    }

  HPDF_Free (pdf);

  return;
}
/* not truncated */
