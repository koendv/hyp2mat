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
 * Generate color palette given hue, saturation and value (brightness). 
 * After Martin Ankerl * "How to Generate Random Colors Programmatically"
 */

void PDF::set_color(cairo_t* cr, int color_idx)
{
  PaletteColor rgb = Color(color_idx);

  cairo_set_source_rgb (cr, rgb.red, rgb.green, rgb.blue);
}

/*
 * Set stroke and fill color to black
 */

void PDF::set_color_black(cairo_t* cr)
{
  cairo_set_source_rgb (cr, 0.0, 0.0, 0.0);
}


/*
 * fill all polygons 
 */

void PDF::page_fill(cairo_t* cr)
{
  cairo_fill(cr);
  return;
}

/*
 * draw all polygon outlines
 */

void PDF::page_stroke(cairo_t* cr)
{
  cairo_stroke(cr);
  return;
}

/*
 * Write layer name on top of page
 */

void PDF::draw_caption(cairo_t* cr, std::string txt)
{
  cairo_save(cr);
  cairo_move_to(cr, margin, y_max - y_min + margin + text_height); /* page top left */
  cairo_identity_matrix(cr); /* else text is upside-down */
  cairo_show_text(cr, txt.c_str());
  cairo_restore(cr);
}

/*
 * Export copper polygon as pdf
 */

void PDF::draw(cairo_t* cr, FloatPolygon& poly)
{
  double old_px, old_py;

  /* pdf requires outer edge of polygon to be clockwise; holes to be counterclockwise */
    
  bool firstpoint = true;
  for (FloatPolygon::iterator i = poly.begin(); i != poly.end(); ++i) {

    double px = i->x * m_to_points - x_min + margin;
    double py = i->y * m_to_points - y_min + margin;
    if (firstpoint) {
      cairo_move_to(cr, px, py);
      firstpoint = false;
      }
    else {
      if ((px != old_px) || (py != old_py))
        cairo_line_to(cr, px, py);
      }
    old_px = px; old_py = py;
    }
  cairo_close_path (cr);
  return;
}

/*
 * Export all polygons of a copper layer as pdf. May contain holes.
 */

void PDF::draw(cairo_t* cr, FloatPolygons& polygons)
{
  for (FloatPolygons::iterator i = polygons.begin(); i != polygons.end(); ++i)
    draw(cr, i->poly);
}

/*
 * Draws a via as a circle.
 */

void PDF::draw(cairo_t* cr, Hyp2Mat::Via& via)
{
  double x = via.x * m_to_points - x_min + margin;
  double y = via.y * m_to_points - y_min + margin;
  double r = via.radius * m_to_points;
  cairo_move_to(cr, x, y);
  cairo_arc(cr, x, y, r, 0.0, 2 * M_PI);
}

void PDF::draw_composite_view(cairo_t* cr, Hyp2Mat::PCB& pcb)
{
  /* new page */
  draw_caption(cr, "Composite");

  /* draw board */
  set_color_black(cr);
  draw(cr, pcb.board);
  page_stroke (cr);

  /* draw layers */
  int current_color = 0;
  for (LayerList::reverse_iterator l = pcb.stackup.rbegin(); l != pcb.stackup.rend(); ++l) {
    if (l->layer_type == LAYER_DIELECTRIC) continue;
    set_color(cr, current_color++);
    draw(cr, l->metal);
    page_stroke (cr);
  }

  /* draw vias */
  set_color_black(cr);
  for (ViaList::iterator i = pcb.via.begin(); i != pcb.via.end(); ++i)
    PDF::draw(cr, *i);
  page_fill (cr);

  /* end of page */
  cairo_show_page(cr);

  return;
}

void PDF::draw_layer_view(cairo_t* cr, Hyp2Mat::PCB& pcb)
{

  int current_color = 0;
  for (LayerList::reverse_iterator l = pcb.stackup.rbegin(); l != pcb.stackup.rend(); ++l) {
    if (l->layer_type == LAYER_DIELECTRIC) continue;

    /* draw caption */
    draw_caption(cr, l->layer_name);

    /* draw board */
    set_color_black(cr);
    draw(cr, pcb.board);
    page_stroke (cr);

    /* draw layer */
    set_color(cr, current_color++);
    draw(cr, l->metal);
    page_fill (cr);

    /* draw vias */
    set_color_black(cr);
    for (ViaList::iterator i = pcb.via.begin(); i != pcb.via.end(); ++i) {
      /* blind vias: only draw via if via goes through current layer */
      if ((i->z0 <= l->z0) && (i->z1 >= l->z1)) {
        draw(cr, *i);
        }
      }
    page_fill (cr);

    /* end of page */
    cairo_show_page(cr);

    }

  return;
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

  try {
    /* get pcb size */
    Bounds bounds = pcb.GetBounds();
    x_max = bounds.x_max * m_to_points;
    y_max = bounds.y_max * m_to_points;
    x_min = bounds.x_min * m_to_points;
    y_min = bounds.y_min * m_to_points;

    /* calculate page size */
    double width = x_max - x_min + 2 * margin;
    double height = y_max - y_min + 2 * margin + text_height; /* add margin for caption */

    if (width < 3) width = 3;
    if (height < 3) height = 3;

    cairo_surface_t* surface =  cairo_pdf_surface_create (filename.c_str(), width, height);
    cairo_t* cr = cairo_create (surface);

    /* point y axis up */
    cairo_translate(cr, 0.0, height);
    cairo_scale(cr, 1.0, -1.0);

    /* global settings */
    cairo_set_font_size (cr, text_height);
    cairo_set_line_width(cr, 0.1);

    /* Draw composite page */
    draw_composite_view(cr, pcb);

    /* Draw individual layers' pages */
    draw_layer_view(cr, pcb);

    cairo_destroy (cr);
    cairo_surface_destroy (surface);
    } 
  catch (...) {
    return;
    }

  return;
}
/* not truncated */
