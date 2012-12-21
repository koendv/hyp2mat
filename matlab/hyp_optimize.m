% CSX = hyp_optimize(CSX)
% Buffers and joins polygons.
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

function CSX = hyp_optimize(CSX)

  % check if polygon arithmetic functions available
  if ~exist('poly_bool')
    disp('poly_bool not installed, not optimizing');
    return;
  end

  scale = 2^32; % needed for poly_bool. XXX

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
      new_layout = {};
 
      % iterate over polygons
      for j = 1:length(layout)

        poly = layout{j};

        current_polygon = poly.polygon;

        if (isempty(current_polygon))
          continue;
        end
        current_polygon = poly_cw(current_polygon);

        if (poly.width ~= 0)
          current_polygon = poly_buffer(current_polygon, 'miter', poly.width/2, scale);
        end

        if poly.add
          % copper: clockwise (positive) polygon
          action = 'or';
        else
          % cutouts in copper: counterclockwise (negative) polygon
          action = 'notb';
        end
 
        if isempty(new_layout)
          if poly.add
            new_layout = current_polygon;
          end
        else 
          [new_layout, hole_flag] = poly_bool(new_layout, current_polygon, action, scale);
% Superfluous? See poly_boolmex source: creation of hole flags.
%          % make polygons clockwise if positive, counterclockwise if hole.
%          for j = 1:length(new_layout)
%            if hole_flag(j)
%              new_layout{j} = poly_ccw(new_layout{j});
%            else
%              new_layout{j} = poly_cw(new_layout{j});
%            end
%          end
        end
      end

      % copy new_layout to layout
      layout = {};
      for j = 1:length(new_layout)
        s = {};
        s.polygon = new_layout{j};
        s.add = poly_iscw(s.polygon);
        s.width = 0;
        layout(end+1) = s;
      end

      CSX.stackup{i}.layout = layout;
     
    end 
  end

end

% not truncated

