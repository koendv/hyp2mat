% CSX = hyp_draw_board(CSX, s)
% Support function. 
% Draws pcb outline.
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

function CSX = hyp_draw_board(CSX, s)

  % XXX For now, limit ourselves to homogeneous rectangular boards.
  % XXX (no circular boards, cutouts, or boards with layers of different dielectrics yet)

  % first, determine the maximum dimensions of the board

  % board horizontal size: loop through all board segments and arcs.
  xmin =  min(CSX.board_outline(1, 1:end));
  xmax =  max(CSX.board_outline(1, 1:end));
  ymin =  min(CSX.board_outline(2, 1:end));
  ymax =  max(CSX.board_outline(2, 1:end));

  % loop through stackup, assigning heights.
  % CSX.stackup{i}.z is layer vertical position
  % CSX.stackup{i}.t is layer height
  z = 0;
  for i = length(CSX.stackup):-1:1
    CSX.stackup{i}.z = z;
    z = z + CSX.stackup{i}.t;
  end

  % determine dielectric height
  zmin = inf;
  zmax = -inf;
  for i = 1:length(CSX.stackup)
    z = CSX.stackup{i}.z;
    if (z < zmin)
      zmin = z;
    end
    if (z > zmax)
      zmax = z;
    end
    if (strcmp(CSX.stackup{i}.type, 'dielectric'))
      z = z + CSX.stackup{i}.t; % add dielectric thickness
      if (z > zmax)
        zmax = z;
      end 
    end
  end

  % create minimal mesh
  mesh.x = [ xmin xmax ];
  mesh.y = [ ymin ymax ];
  mesh.z = [ zmin zmax ]; 

  % add mesh
  CSX = DefineRectGrid(CSX, CSX.units, mesh);

  % create materials
  % XXX Ought to create the dielectrics from the stackup
  physical_constants;
  substrate_epr = CSX.substrate_epr;
  CSX = AddMaterial( CSX, 'FR4');
  CSX = SetMaterialProperty( CSX, 'FR4', 'Epsilon', substrate_epr, 'Mue', 1);
  CSX = AddMetal( CSX, 'via' ); % via metal

  % create a different metal/cutout for every layer
  % allows us to inspect inner layers
  for i = 1:length(CSX.stackup)
 
    % signal layers are drawn positive:
    % material is copper, cutout is dielectric.
    if (strcmp(CSX.stackup{i}.type, 'signal'))

      material = [ CSX.stackup{i}.l '_copper' ];
      CSX.stackup{i}.material = material;
      thickness = CSX.stackup{i}.t * CSX.units / CSX.zscale; % thickness in m
      conductivity = CSX.conductivity;
      CSX = AddConductingSheet(CSX, material, conductivity, thickness);

      cutout = [ CSX.stackup{i}.l '_cutout'];
      CSX.stackup{i}.cutout = cutout;
      CSX = AddMaterial( CSX, cutout);
      CSX = SetMaterialProperty( CSX, cutout, 'Epsilon', substrate_epr, 'Mue', 1); 

    end

    % plane layers are drawn negative:
    % cutout is copper, material is dielectric.
    if (strcmp(CSX.stackup{i}.type, 'plane'))

      cutout = [ CSX.stackup{i}.l '_copper'];
      CSX.stackup{i}.cutout = cutout;
      thickness = CSX.stackup{i}.t * CSX.units / CSX.zscale; % thickness in m
      conductivity = CSX.conductivity;
      CSX = AddConductingSheet(CSX, cutout, conductivity, thickness);

      material = [ CSX.stackup{i}.l '_cutout'];
      CSX.stackup{i}.material = material;
      CSX = AddMaterial( CSX, material);
      CSX = SetMaterialProperty( CSX, material, 'Epsilon', substrate_epr, 'Mue', 1);

    end

    % dielectric layers have no metal

  end

  % create board 
  p1 = [xmin ymin zmin];
  p2 = [xmax ymax zmax];
  CSX = AddBox (CSX, 'FR4', CSX.prio_dielectric, p1, p2);

  % XXX if board is not rectangular
  % CSX = AddLinPoly(CSX, 'FR4', CSX.prio_dielectric, 2, zmin, CSX.board_outline, zmax - zmin);

  units_mm = CSX.units * 1000;
  disp(['board: x = ', num2str(xmin*units_mm), ' : ', num2str(xmax*units_mm), ' mm']);
  disp(['       y = ', num2str(ymin*units_mm), ' : ', num2str(ymax*units_mm), ' mm']);
  disp(['       z = ', num2str(zmin*units_mm), ' : ', num2str(zmax*units_mm), ' mm']);
  disp(['epsilonr = ', num2str(substrate_epr)]);

  % create power and ground planes
  for i = 1:length(CSX.stackup)
    if (strcmp(CSX.stackup{i}.type, 'plane'))
      z1 = CSX.stackup{i}.z;
      z2 = CSX.stackup{i}.z+CSX.stackup{i}.t;
      p1 = [xmin ymin z1];
      p2 = [xmax ymax z1];
      material = CSX.stackup{i}.cutout;
      CSX = AddBox (CSX, material, CSX.prio_plane, p1, p2);
      disp(['plane: z = ', num2str(z1), ' : ', num2str(z2)]);
    end
  end

end

% not truncated

