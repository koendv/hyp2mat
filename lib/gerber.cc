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

#include "gerb_file.h"
#include "pick-and-place.h"

#include "cairo-pdf.h" // XXX

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
  copper.metal = GerberToPolygons();
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
    LoadFile(outline_filename, GERBV_LAYERTYPE_RS274X);
    pcb.board = GerberToPolygons();
    }

}

void Gerber::LoadPickAndPlace(std::string pickandplace_filename, Hyp2Mat::PCB& pcb)
{
  if (debug) std::cerr << "pickandplace:" <<  pickandplace_filename << std::endl;

  /* load pick and place */
  gchar* fname = g_strdup(pickandplace_filename.c_str());
  GArray *parsedPickAndPlaceData = gerbv_parse_pick_and_place (fname);
  if (parsedPickAndPlaceData == NULL) {
    std::cerr << "error: pick and place load error" << std::endl;
    throw std::exception ();
    }
    
  /* loop over components */
  for (int i = 0; i < parsedPickAndPlaceData->len; i++) {
    PnpPartData partData = g_array_index(parsedPickAndPlaceData, PnpPartData, i);

    /* check component is on top or bottom layer */
    bool bottom_layer= ((partData.layer[0]=='b') || (partData.layer[0]=='B'));
    bool top_layer= ((partData.layer[0]=='t') || (partData.layer[0]=='T'));
    if (!bottom_layer && !top_layer) continue;

    /* fill in device values */
    Hyp2Mat::Device dev;
    std::ostringstream name;
    name << "D" << i;
    dev.name = name.str();
    dev.ref = partData.designator;
    dev.value_type = Hyp2Mat::DEVICE_VALUE_NONE;
    if (!pcb.stackup.empty() && bottom_layer) dev.layer_name = pcb.stackup.front().layer_name;
    if (!pcb.stackup.empty() && top_layer) dev.layer_name = pcb.stackup.back().layer_name;
    
    pcb.device.push_back(dev); 

    /* fill in pin values */
    Hyp2Mat::Pin dev_pin;
    dev_pin.ref = partData.designator;
    dev_pin.x = partData.mid_x;
    dev_pin.y = partData.mid_y;

    // XXX check not mixed up front & back

    if (!pcb.stackup.empty() && bottom_layer) {
      dev_pin.z0 = pcb.stackup.front().z0;
      dev_pin.z1 = pcb.stackup.front().z1;
      dev_pin.layer_name = pcb.stackup.front().layer_name;
      }

    if (!pcb.stackup.empty() && top_layer) {
      dev_pin.z0 = pcb.stackup.back().z0;
      dev_pin.z1 = pcb.stackup.back().z1;
      dev_pin.layer_name = pcb.stackup.back().layer_name;
      }

     double half_width = partData.width / 2;
     double half_length = partData.length / 2;

     double rot = partData.rotation;
     if (((rot > 45) && (rot < 135)) || ((rot > 225) && (rot < 315)) ||
         ((rot < -45) && (rot > -135)) || ((rot < -225) && (rot > -315))) {
       double half_length = partData.width / 2;
       double half_width = partData.length / 2;
       }
     else {
       double half_width = partData.width / 2;
       double half_length = partData.length / 2;
       }
     dev_pin.metal.push_back(Hyp2Mat::FloatPoint(dev_pin.x - half_length, dev_pin.y - half_width));
     dev_pin.metal.push_back(Hyp2Mat::FloatPoint(dev_pin.x - half_length, dev_pin.y + half_width));
     dev_pin.metal.push_back(Hyp2Mat::FloatPoint(dev_pin.x + half_length, dev_pin.y + half_width));
     dev_pin.metal.push_back(Hyp2Mat::FloatPoint(dev_pin.x + half_length, dev_pin.y - half_width));
      
     pcb.pin.push_back(dev_pin);

    }

  /* free pick and place data */
  g_array_free (parsedPickAndPlaceData, TRUE);

  return;
  
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

Hyp2Mat::FloatPolygons Gerber::GerberToPolygons()
{
  /* Sanity checks */

  /* get index of file in project */
  gint idx_loaded = gerbv_project->last_loaded;

  /* check file parsed ok */
  gerbv_fileinfo_t *fd = gerbv_project->file[idx_loaded];
  if (fd == NULL){
    std::cerr << "error: file not loaded" << std::endl;
    throw std::exception();
    }

  /* convert to cairo paths */
  GArray *gerbv_path_array = gerbv_export_paths_from_layer(fd, 72);
  if (gerbv_path_array == NULL) {
      std::cerr << "error: gerber to polygon conversion error" << std::endl;
      throw std::exception ();
    }

  /* convert cairo paths to Hyp2Mat polygons */
  Hyp2Mat::Polygon layer;

  /* loop over all paths */
  for (int i=0; i<gerbv_path_array->len; i++) {
    gerbv_path_t gerbv_path = g_array_index(gerbv_path_array, gerbv_path_t, i);
    /* loop over path */
    cairo_path_t *cairo_path = gerbv_path.path;
    cairo_path_data_t *data;
    Hyp2Mat::FloatPolygon poly;

    /* loop over single path */
    double x0 = 0, y0 = 0;
    bool x0y0_set = FALSE;
    for (int j=0; j < cairo_path->num_data; j += cairo_path->data[j].header.length) {
        data = &cairo_path->data[j];
        double x1 = data[1].point.x;
        double y1 = data[1].point.y;
        switch (data->header.type) {
        case CAIRO_PATH_MOVE_TO:
            x0 = x1;
            y0 = y1;
            x0y0_set = TRUE;
            if (debug) std::cerr << "cairo: move_to(" << x1 << ", " << y1 << ")" << std::endl;
            poly.clear();
            poly.push_back(Hyp2Mat::FloatPoint(x1, y1));
            break;
        case CAIRO_PATH_LINE_TO:
            if (debug) std::cerr << "cairo: line_to(" << x1 << ", " << y1 << ")" << std::endl;
            poly.push_back(Hyp2Mat::FloatPoint(x1, y1));
            break;
        case CAIRO_PATH_CURVE_TO:
            std::cerr << "error: unexpected cairo path_curve_to" << std::endl;
            break;
        case CAIRO_PATH_CLOSE_PATH:
            if (debug) std::cerr << "cairo: close_path" << std::endl;
            if (x0y0_set) poly.push_back(Hyp2Mat::FloatPoint(x0, y0));
            layer.AddEdge(poly); //XXX Check
            poly.clear();
            break;
        }
      }

    /* XXX stroke and fill */
    if (debug) {
        if (gerbv_path.fill)
          std::cerr<<"cairo: fill";
        else
          std::cerr<<"cairo: stroke"; // XXX
        std::cerr << " line_width " << gerbv_path.line_width << std::endl;
      }

    /* XXX expand polygon by half line width */

    }

  /* XXX release memory */

  /* return polygons */
  return layer.Result();

}

/* not truncated */
