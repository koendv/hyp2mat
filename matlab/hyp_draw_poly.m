% CSX = hyp_draw_poly(CSX, action, layer, width, poly)
% Support function.
% Draws polygon, polyvoid, polyline.
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

function CSX = hyp_draw_poly(CSX, action, layer, width, poly)

  % look up layer
  for j = 1:length(CSX.stackup)
    % either this layer exactly, or MDEF for all metal layers.
    if (strcmp(CSX.stackup{j}.l, layer) || (strcmp('MDEF', layer) && ~strcmp(CSX.stackup{j}.type, 'dielectric')))

      % found layer. Add or subtract?
      add_poly = strcmp(action, '+');

      % plane layers are drawn negative
      if (strcmp(CSX.stackup{j}.type, 'plane'))
        add_poly = ~add_poly;
      end
      
      if (add_poly)
        operation = '+';
      else
        operation = '-';
      end

      % polygon has line width 'width'. expand the polygon by width/2
      extended_poly = polygon_extend(poly, width/2);
      extended_poly = poly;

      % add to already existing polygons
      current_layout = CSX.stackup{j}.layout;
      new_layout = polygon_arith(current_layout, operation, extended_poly);
      CSX.stackup{j}.layout = new_layout;

    end
  end
end

% not truncated

