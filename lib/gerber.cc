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
#include <algorithm>

#include "config.h"
#include "gerber.h"

/*
 * Creation and destruction 
 */

Gerber::Gerber()
{
  /* create a top level libgerbv structure */
  gerbv_project = gerbv_create_project();
  debug = false;
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

void Gerber::Read (std::vector<std::string> gerber_filenames, std::string outline_filename, std::string tools_filename, std::string drill_filename, std::string pickandplace_filename, Hyp2Mat::PCB& pcb)
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

  /* parse excellon files */
  LoadTools(tools_filename);
  LoadDrill(drill_filename);

  /* parse gerber files */
  for(std::vector<std::string>::iterator it = gerber_filenames.begin(); it != gerber_filenames.end(); ++it)
    LoadGerber(*it);

  /* parse board outline */
  LoadGerber(outline_filename);

  /* parse pick-and-place file */
  LoadPickAndPlace(pickandplace_filename);
  
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
  if (!gerbv_process_tools_file(fname))
    std::cerr << "error processing tools " << tools_filename << std::endl;
}

void Gerber::LoadDrill(std::string drill_filename)
{
  if (debug) std::cerr << "drill:" << drill_filename << std::endl;

  LoadFile(drill_filename, GERBV_LAYERTYPE_DRILL);
}

void Gerber::LoadGerber(std::string gerber_filename)
{
  if (debug) std::cerr << "gerber:" <<  gerber_filename << std::endl;

  LoadFile(gerber_filename, GERBV_LAYERTYPE_RS274X);
}

void Gerber::LoadPickAndPlace(std::string pickandplace_filename)
{
  if (debug) std::cerr << "pickandplace:" <<  pickandplace_filename << std::endl;

  LoadFile(pickandplace_filename, GERBV_LAYERTYPE_PICKANDPLACE);
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
  if (gerbv_project->file[idx_loaded] == NULL)
    std::cerr << "error processing " << filename << std::endl;

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
    }
}

/* not truncated */
