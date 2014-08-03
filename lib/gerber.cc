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
 * This is where we use libgerbv to read Gerber and Excellon files.
 */

#include <iostream>
#include <sstream>
#include <algorithm>

#include "config.h"
#include "gerber.h"
#include "hypfile.h"

/*
 * Creation and destruction 
 */

Gerber::Gerber()
{
  /* create a top level libgerbv structure */
  gerbv_project = gerbv_create_project();
  debug = false;
  scale=0.0254; /* inches to m */
  /* scale = 1.0; /* XXX for debugging only */
  copper_thickness = 35e-6; /* 35um in m */
  dielectric_thickness = 1.5e-3; /* 1.5mm in m */
  pcb_thickness = 0.0; /* running count of pcb thickness */
}

Gerber::~Gerber()
{
  /* destroy the top-level libgerbv structure and free all memory */
  gerbv_destroy_project (gerbv_project);
  gerbv_project = NULL;
}

/*
 * Read a pcb in gerber format
 */

void Gerber::Read (std::vector<std::string> gerber_filenames, std::string outline_filename, std::string tools_filename, std::vector<std::string> drill_filenames, std::vector<std::string> pickandplace_filenames, Hyp2Mat::PCB& pcb)
{

  debug = pcb.debug != 0;

  /* set g_log logging level */
  int loglevel;
  switch (pcb.debug) {
    case 0: loglevel = G_LOG_LEVEL_ERROR;
      break;
    case 1: loglevel = G_LOG_LEVEL_CRITICAL;
      break;
    case 2: loglevel = G_LOG_LEVEL_WARNING;
      break;
    case 3: loglevel = G_LOG_LEVEL_MESSAGE;
      break;
    case 4: loglevel = G_LOG_LEVEL_INFO;
      break;
    case 5: loglevel = G_LOG_LEVEL_DEBUG;
      break;
    default: loglevel = G_LOG_LEVEL_MASK;
    }
  g_log_set_handler(NULL, (GLogLevelFlags)loglevel, g_log_default_handler, NULL);

  /* parse gerber files */
  for(std::vector<std::string>::iterator it = gerber_filenames.begin(); it != gerber_filenames.end(); ++it)
    LoadGerber(*it, pcb);

  /* parse excellon drill files */
  LoadTools(tools_filename);
  for(std::vector<std::string>::iterator it = drill_filenames.begin(); it != drill_filenames.end(); ++it)
    LoadDrill(*it, pcb);

  /* parse pick-and-place file */
  for(std::vector<std::string>::iterator it = pickandplace_filenames.begin(); it != pickandplace_filenames.end(); ++it)
    LoadPickAndPlace(*it, pcb);
  
  /* parse board outline */
  LoadOutline(outline_filename, pcb);

  return;
}

/*
 * File IO
 */
 

void Gerber::LoadTools(std::string tools_filename)
{
  if (tools_filename.empty()) return;

  if (debug) std::cerr << "tools:" << tools_filename << std::endl;

  gchar* fname = g_strdup(tools_filename.c_str());
  if (!gerbv_process_tools_file(fname)) {
      std::cerr << "error processing tools " << tools_filename << std::endl;
      throw std::exception ();
    }
}

void Gerber::LoadDrill(std::string drill_filename, Hyp2Mat::PCB& pcb)
{
  if (debug) std::cerr << "drill:" << drill_filename << std::endl;

  LoadFile(drill_filename, GERBV_LAYERTYPE_DRILL);

  /* duplicate and clean current image */
  gerbv_image_t *image = gerbv_image_duplicate_image (gerbv_project->file[gerbv_project->last_loaded]->image, NULL);

  /* determine board height */
  Hyp2Mat::Bounds bounds = pcb.GetBounds();

  /* loop over nets, find drills */
  gerbv_net_t *currentNet;
  for (currentNet = image->netlist; currentNet; currentNet = currentNet->next){
      if (currentNet->aperture_state != GERBV_APERTURE_STATE_FLASH) continue;
      if (!image->aperture[currentNet->aperture]) continue;
      if (image->aperture[currentNet->aperture]->type !=  GERBV_APTYPE_CIRCLE) continue;

      /* we have a drill hole */
      double xVal,yVal, dia;
      xVal = currentNet->stop_x;
      yVal = currentNet->stop_y;
      dia = image->aperture[currentNet->aperture]->parameter[0];

      if (debug) std::cout << "drill at (" << xVal << ", " << yVal << ") dia: " << dia << std::endl;

      /* copy drill hole */
      Hyp2Mat::Via via;
      via.x = xVal * scale;
      via.y = yVal * scale;
      via.z0 = 0.0;
      via.z1 = pcb_thickness;
      via.radius = dia / 2 * scale;
      pcb.via.push_back(via);
    }

  /* free image */
  gerbv_destroy_image (image);
  
}

void Gerber::LoadGerber(std::string gerber_filename, Hyp2Mat::PCB& pcb)
{
  if (debug) std::cerr << "gerber:" <<  gerber_filename << std::endl;

  LoadFile(gerber_filename, GERBV_LAYERTYPE_RS274X);

  /*
   * if the first copper layer, add copper and dielectric.
   * if the second copper layer, simply add copper. the dielectric is altready there 
   * if third or later copper layers, add dielectric and copper
   */

  if (pcb.stackup.empty()) {
    /* first copper layer */
    AddCopperLayer(pcb);
    AddDielectricLayer(pcb);
    }
  else {
    /* second or later copper layer */
    if (pcb.stackup.back().layer_type == Hyp2Mat::LAYER_SIGNAL) /* last layer was copper */
      AddDielectricLayer(pcb);
    AddCopperLayer(pcb);
    }

}

void Gerber::AddCopperLayer(Hyp2Mat::PCB& pcb)
{
  /* create copper layer */
  HypFile::Hyp constants;
  Hyp2Mat::Layer copper;
  copper.layer_type = Hyp2Mat::LAYER_SIGNAL;
  copper.layer_name.clear();
  copper.material_name = "copper";
  copper.metal = LayerToPolygon();
  copper.epsilon_r = 0.0;
  copper.loss_tangent = 0.0;
  copper.bulk_resistivity = constants.copper_bulk_resistivity;
  copper.resistivity_temp_coeff = constants.copper_temperature_coefficient;

  /* count copper layers */
  int copper_layers = 0;
  for (Hyp2Mat::LayerList::iterator l = pcb.stackup.begin(); l != pcb.stackup.end(); ++l)
    if (l->layer_type == Hyp2Mat::LAYER_SIGNAL) copper_layers++;
 
  /* set layer name */
  copper_layers++;
  std::ostringstream name;
  name << "Metal " << copper_layers;
  copper.layer_name = name.str();

  /* set vertical position */
  copper.z0 = pcb_thickness;
  pcb_thickness += copper_thickness;
  copper.z1 = pcb_thickness;

  /* add copper layer */
  pcb.stackup.push_back(copper);

}

void Gerber::AddDielectricLayer(Hyp2Mat::PCB& pcb)
{
  /* create dielectric layer */
  HypFile::Hyp constants;
  Hyp2Mat::Layer dielectric;
  dielectric.layer_type = Hyp2Mat::LAYER_DIELECTRIC;
  dielectric.layer_name.clear();
  dielectric.material_name = "FR4";
  dielectric.epsilon_r = constants.fr4_epsilon_r;
  dielectric.loss_tangent = constants.fr4_loss_tangent;
  dielectric.bulk_resistivity = 0.0;
  dielectric.resistivity_temp_coeff = 0.0;

  /* count dielectric layers */
  int dielectric_layers = 0;
  for (Hyp2Mat::LayerList::iterator l = pcb.stackup.begin(); l != pcb.stackup.end(); ++l)
    if (l->layer_type == Hyp2Mat::LAYER_DIELECTRIC) dielectric_layers++;
 
  /* set layer name */
  dielectric_layers++;
  std::ostringstream name;
  name << "Dielectric " << dielectric_layers; 
  dielectric.layer_name = name.str();

  /* set vertical position */
  dielectric.z0 = pcb_thickness;
  pcb_thickness += dielectric_thickness;
  dielectric.z1 = pcb_thickness;
    
  /* add copper layer */
  pcb.stackup.push_back(dielectric);
}

void Gerber::LoadOutline(std::string outline_filename, Hyp2Mat::PCB& pcb)
{
  if (debug) std::cerr << "outline:" <<  outline_filename << std::endl;

  if (!outline_filename.empty()) {
    /* Convert Gerber board outline to Hyp2Mat polygon */ 
    LoadGerber(outline_filename, pcb);
    pcb.board = LayerToPolygon();
    }
  else {
    /* create default rectangular outline */

    /* loop over layers, determine bounds */

    double x_min = 0.0;
    double y_min = 0.0;
    double x_max = 0.0;
    double y_max = 0.0;
  
    /* loop over layers */
    bool first = true;
    for (int idx = 0; idx <= gerbv_project->last_loaded; idx++) {
      if (gerbv_project->file[idx] && gerbv_project->file[idx]->image && gerbv_project->file[idx]->image->info) {
        gerbv_image_info_t *info = gerbv_project->file[idx]->image->info;
        if (first || (x_min > info->min_x)) x_min = info->min_x;
        if (first || (x_max < info->max_x)) x_max = info->max_x;
        if (first || (y_min > info->min_y)) y_min = info->min_y;
        if (first || (y_max < info->max_y)) y_max = info->max_y;
        first = false;
        }
      }

    if ((x_min != x_max) && (y_min != y_max)) {

      /* add rectangular outline to pcb */

      Hyp2Mat::FloatPoly outline;
      outline.poly.push_back(Hyp2Mat::FloatPoint(x_min * scale, y_min * scale));
      outline.poly.push_back(Hyp2Mat::FloatPoint(x_min * scale, y_max * scale));
      outline.poly.push_back(Hyp2Mat::FloatPoint(x_max * scale, y_max * scale));
      outline.poly.push_back(Hyp2Mat::FloatPoint(x_max * scale, y_min * scale));
      outline.is_hole = false;
      outline.nesting_level = 0;
  
      pcb.board.push_back(outline);
      }
    }

}

void Gerber::LoadPickAndPlace(std::string pickandplace_filename, Hyp2Mat::PCB& pcb)
{
  if (debug) std::cerr << "pickandplace:" <<  pickandplace_filename << std::endl;

  LoadFile(pickandplace_filename, GERBV_LAYERTYPE_PICKANDPLACE);

  /* duplicate and clean current image */
  gerbv_image_t *image = gerbv_image_duplicate_image (gerbv_project->file[gerbv_project->last_loaded]->image, NULL);

  /* loop over nets */
  gerbv_net_t *currentNet;
  for (currentNet = image->netlist; currentNet; currentNet = currentNet->next){
    // XXX 
    std::cerr << std::endl;
//    GString glbl = currentNet->label;
//    std::cerr << glbl->str << std::endl;
    std::cerr << currentNet->start_x << std::endl;
    std::cerr << currentNet->start_y << std::endl;
    std::cerr << currentNet->stop_x << std::endl;
    std::cerr << currentNet->stop_y << std::endl;
    // Insert code which actually does something here. XXX
    }

  /* free image */
  gerbv_destroy_image (image);
  
}

void Gerber::LoadFile(std::string filename, gerbv_layertype_t layertype)
{
  if (filename.empty()) return;

  /* load Gerber, Excellon or Centroid file */
  gchar* fname = g_strdup(filename.c_str());
  gerbv_open_layer_from_filename (gerbv_project, fname);

  /* get index of file in project */
  gint idx_loaded = gerbv_project->last_loaded;

  /* check file parsed ok */
  if (gerbv_project->file[idx_loaded] == NULL) {
      std::cerr << "error processing " << filename << std::endl;
      throw std::exception ();
    }

  /* check file type */
  if (gerbv_project->file[idx_loaded]->image->layertype != layertype) {
      std::cerr << "error processing " << filename << ": file type is ";
      switch( gerbv_project->file[idx_loaded]->image->layertype) {
        case GERBV_LAYERTYPE_RS274X:
          std::cerr << "rs274x";
          break;
        case GERBV_LAYERTYPE_DRILL:
          std::cerr << "drill";
          break;
        case GERBV_LAYERTYPE_PICKANDPLACE:
          std::cerr << "pickandplace";
          break;
        default:
          std::cerr << "unknown";
        };
        std::cerr << std::endl;
      throw std::exception ();
    }
}

/*
 * Gerber to Polygon conversion
 */

Hyp2Mat::FloatPolygons Gerber::LayerToPolygon()
{
  Hyp2Mat::FloatPolygons poly;

  /* get index of file in project */
  gint idx_loaded = gerbv_project->last_loaded;

  /* check file parsed ok */
  if (gerbv_project->file[idx_loaded] == NULL)
    std::cerr << "error: file did not load" << std::endl;

  /* check file type */
  if (gerbv_project->file[idx_loaded]->image->layertype != GERBV_LAYERTYPE_RS274X)
    std::cerr << "error: layer not a gerber layer" << std::endl;

  /* export to cairo */
 
  
  return poly;

}

/* not truncated */
