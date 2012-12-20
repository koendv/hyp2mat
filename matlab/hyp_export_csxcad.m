% CSX = hyp_export_csxcad(CSX)
% Support function. 
% See hyp2mat(1) - convert hyperlynx files to matlab scripts.

% Copyright 2012 Koen De Vleeschauwer.
%
% This file is part of hyp2mat.
%
% This program is free software: you can redistribute it and/or modify
% it under the terms of the GNU General Public License as published by
% the Free Software Foundation, either version 3 of the License, or
% (at your option) any later version.
%
% This program is distributed in the hope that it will be useful,
% but WITHOUT ANY WARRANTY; without even the implied warranty of
% MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
% GNU General Public License for more details.
%
% You should have received a copy of the GNU General Public License
% along with this program.  If not, see <http://www.gnu.org/licenses/>.

function CSX = hyp_export_csxcad(CSX)

  % priorities

  prio_dielectric = 100;  % FR4 dielectric
  prio_plane      = 200;  % power and ground planes
  prio_material   = 300;  % copper
  prio_via        = 500;  % via metal

  % determine board size
  xmin =  min(CSX.board_outline(:, 1));
  xmax =  max(CSX.board_outline(:, 1));
  ymin =  min(CSX.board_outline(:, 2));
  ymax =  max(CSX.board_outline(:, 2));
  zmin =  CSX.stackup{end}.z;
  zmax =  CSX.stackup{1}.z + CSX.stackup{1}.t;

  % create minimal mesh
  mesh.x = [ xmin xmax ];
  mesh.y = [ ymin ymax ];
  mesh.z = [ zmin zmax ];

  % add mesh
  CSX = DefineRectGrid(CSX, CSX.units, mesh);

  % determine dielectric height
  pcb_zmin = inf;
  pcb_zmax = -inf;
  for i = 1:length(CSX.stackup)
    if (strcmp(CSX.stackup{i}.type, 'dielectric'))
      z = CSX.stackup{i}.z;
      pcb_zmin = min([ z pcb_zmin ]);
      pcb_zmax = max([ z pcb_zmax ]);
      z = z + CSX.stackup{i}.t; % add dielectric thickness
      pcb_zmax = max([ z pcb_zmax ]);
    end
  end

  % create board
  % XXX Ought to create the dielectrics from the stackup for non-homogeneous dielectrics
  physical_constants;
  substrate_epr = CSX.substrate_epr;

  p1 = [xmin ymin pcb_zmin];
  p2 = [xmax ymax pcb_zmax];

  CSX = AddMaterial( CSX, 'FR4');
  CSX = SetMaterialProperty( CSX, 'FR4', 'Epsilon', substrate_epr, 'Mue', 1);
  CSX = AddBox (CSX, 'FR4', prio_dielectric, p1, p2);

  % create vias

  if ~isempty(CSX.via_list)
    % create via metal
    CSX = AddMetal( CSX, 'via' ); 
    for i = 1:length(CSX.via_list)
      via = CSX.via_list{i};
    
      p1 = [ via.x via.y via.z1 ];
      p2 = [ via.x via.y via.z2 ];

      % copper in via
      CSX = AddCylinder(CSX, 'via', prio_via, p1, p2, via.radius); % via copper

  end

  % create polygons

  for i = 1:length(CSX.stackup)
    if (strcmp(CSX.stackup{i}.type, 'signal') || strcmp(CSX.stackup{i}.type, 'plane'))
      %
      % Each layer has a layout, which is a cell array of polygons
      % each polygon has three fields:
      % polygon: list of vertices
      % width: polygon edge width
      % add: true if polygon is positive, false if polygon is hole.
      %
      layout = CSX.stackup{i}.layout;
 
      % copper and cutout not created yet
      copper = '';
      cutout = '';
      % initial priority
      current_priority = prio_material;
      % initial material
      current_material = '';

      % iterate over polygons
      for j = 1:length(layout)

        poly = layout{j};

        if (isempty(poly.polygon))
          continue;
        end

        if poly.add
          % copper: create material if necessary
          if isempty(copper)
            % create copper material
            copper = [ CSX.stackup{i}.l '_copper' ];
            thickness = CSX.stackup{i}.t * CSX.units / CSX.zscale; % thickness in m
            conductivity = CSX.conductivity;
            CSX = AddConductingSheet(CSX, copper, conductivity, thickness);
          end
          new_material = copper;
        else
          % cutouts in copper: create material if necessary
          if isempty(cutout)
            % create cutout material
            cutout = [ CSX.stackup{i}.l '_cutout' ];
            CSX = AddMaterial(CSX, cutout);
            CSX = SetMaterialProperty(CSX, cutout, 'Epsilon', substrate_epr, 'Mue', 1);
          end
          new_material = cutout;
       end

        % increase priority if necessary
        if ~strcmp(current_material, new_material)
          current_priority = current_priority + 1;
        end
 
        % polygon material
        current_material = new_material;

        % polygon vertical position
        layer_z = CSX.stackup{i}.z;

        % polygon vertices
        current_polygon = poly.polygon;

        % add to CSX model
        CSX = AddPolygon(CSX, current_material, current_priority, 2, layer_z, current_polygon.');
      end
    end 
  end

  % display helpful messages
  units_um = CSX.units / CSX.zscale * 1e6; % thickness in um
  for i = 1:length(CSX.stackup)
    if (strcmp(CSX.stackup{i}.type, 'signal') || strcmp(CSX.stackup{i}.type, 'plane'))
      disp(['copper : ', CSX.stackup{i}.l, ' = ', num2str(CSX.stackup{i}.t*units_um), ' um']);
    end
  end

  disp(['epsilonr = ', num2str(CSX.substrate_epr)]);

  units_mm = CSX.units * 1000; %  size in mm
  disp(['board: x = ', num2str(xmin*units_mm), ' : ', num2str(xmax*units_mm), ' mm']);
  disp(['       y = ', num2str(ymin*units_mm), ' : ', num2str(ymax*units_mm), ' mm']);
  disp(['       z = ', num2str(zmin*units_mm), ' : ', num2str(zmax*units_mm), ' mm']);

end

% not truncated

