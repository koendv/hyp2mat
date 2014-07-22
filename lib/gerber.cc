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
#include "gerbv.h"

/*
 * Read a pcb in gerber format
 */

void Gerber::Read (std::vector<std::string> gerber_filenames, std::string tools_filename, std::string drill_filename, std::string outline_filename, std::string pickandplace_filename, Hyp2Mat::PCB& pcb)
{

  /* XXX Work in progress */

  /* create a top level gerber structure */
  gerbv_project_t *gerbv_project = gerbv_create_project();

  /* load Excellon tools file, if available */
  if (!tools_filename.empty()) {
    std::cout << "tools:" << tools_filename << std::endl;
    if (!gerbv_process_tools_file(optarg))
      std::cout << "error processing tools" << std::endl;
    }

  /* parse gerber and excellon files */
  for(std::vector<std::string>::iterator it = gerber_filenames.begin(); it != gerber_filenames.end(); ++it) {
    std::cout << "gerber:" << *it << std::endl;
    /* load gerber file */
    gchar* fname = g_strdup(it->c_str());
    gerbv_open_layer_from_filename (gerbv_project, fname);

    /* get index of file in project */
    gint idx_loaded = gerbv_project->last_loaded;

    /* check file parsed ok */
    if (gerbv_project->file[idx_loaded] == NULL)
      std::cout << "error processing gerber" << std::endl;

    /* print file type */
    switch( gerbv_project->file[idx_loaded]->image->layertype) {
      case GERBV_LAYERTYPE_RS274X:
        std::cout << "rs274x" << std::endl;
        break;
      case GERBV_LAYERTYPE_DRILL:
        std::cout << "drill" << std::endl;
        break;
      case GERBV_LAYERTYPE_PICKANDPLACE:
        std::cout << "pickandplace" << std::endl;
        break;
      default:
        std::cout << "??" << std::endl;
      }
    }
  
  /* destroy the top-level gerber structure, free memory */
  gerbv_destroy_project (gerbv_project);

  return;
}

/* not truncated */
