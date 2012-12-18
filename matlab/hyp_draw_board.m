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

  % determine the maximum dimensions of the board

  % board horizontal size: loop through all board segments and arcs.
  xmin =  min(CSX.board_outline(:, 1));
  xmax =  max(CSX.board_outline(:, 1));
  ymin =  min(CSX.board_outline(:, 2));
  ymax =  max(CSX.board_outline(:, 2));

  % board verical size: loop through stackup, assigning heights.
  % CSX.stackup{i}.z is layer vertical position
  % CSX.stackup{i}.t is layer height
  z = 0;
  for i = length(CSX.stackup):-1:1
    CSX.stackup{i}.z = z;
    z = z + CSX.stackup{i}.t;
  end

  % initialize layers
  for i = 1:length(CSX.stackup)
 
    % signal layers are drawn positive:
    % material is copper, cutout is dielectric.
    if (strcmp(CSX.stackup{i}.type, 'signal'))
      CSX.stackup{i}.layout = [];

    % plane layers are drawn negative:
    % cutout is copper, material is dielectric.
    elseif (strcmp(CSX.stackup{i}.type, 'plane'))
      corner1 = [ xmin ymin ];
      corner2 = [ xmax ymin ];
      corner3 = [ xmax ymax ];
      corner4 = [ xmin ymax ];
      CSX.stackup{i}.layout = [corner1 corner2 corner2 corner4];

    end
    % dielectric layers have no metal

  end
end
% not truncated

