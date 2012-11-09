% CSX = hyp_draw_polygon(CSX, s)
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

function CSX = hyp_draw_polygon(CSX, s)

  % Check whether to import current net 

  if (~hyp_draw_net(CSX))
    % clear polygon data
    CSX.poly = {};
    return;
  end

  % draw polygon

  if (~isfield(CSX, 'poly') || ~isfield(CSX.poly, 'l'))
    return; % nothing to do
  end

  % CSX.poly.polytype = 'polyline'; % force drawing polygon outlines for debugging

  % draw polyline segments 
  if (strcmp(CSX.poly.polytype, 'polyline'))
    CSX = hyp_draw_polyline(CSX, s);
    return
  end

  % remove duplicate vertices
  pts = [];
  x0 = NaN;
  y0 = NaN;
  for i = 1:length(CSX.poly.points)
    x1 = CSX.poly.points(1, i);
    y1 = CSX.poly.points(2, i);
    if ((x0 ~= x1) || (y0 ~= y1))
      pts(1, end + 1) = x1;
      pts(2, end) = y1;
      x0 = x1;
      y0 = y1;
    end
  end

  % don't repeat the last point
  if ((length(pts) >= 2) && (pts(1,1) == pts(1,end)) && (pts(2,1) == pts(2,end)))
    pts = pts(:,1:end-1);
  end

  % sanity check
  if (length(pts) < 3)
    disp([ 'at line ' num2str(s.lineno) ': hyp_draw_polygon: only ' num2str(length(pts)) ' vertices (was: ' num2str(length(CSX.poly.points)) ' )' ]);
    CSX.poly = {}; % XXX ? Or draw a line if number of points = 2 ?
    return;
  end

  CSX.poly.points = pts;

  % loop through layers
  layer = CSX.poly.l;
  for j = 1:length(CSX.stackup)
    % either this layer exactly, or MDEF for all metal layers.
    if (strcmp(CSX.stackup{j}.l, layer) || (strcmp('MDEF', layer) && ~strcmp(CSX.stackup{j}.type, 'dielectric')))

      % position of layer
      layer_z = CSX.stackup{j}.z;

      % determine material

      % polygon is positive on signal layers, negative on power and ground planes.

      if (strcmp(CSX.poly.polytype, 'polygon'))
        material = CSX.stackup{j}.material;
        prio = CSX.prio_material;
      end

      % polyvoid is negative on signal layers, positive on power and ground planes.

      if (strcmp(CSX.poly.polytype, 'polyvoid'))
        material = CSX.stackup{j}.cutout;
        prio = CSX.prio_cutout;
      end

      % polygon has line width 'CSX.poly.w'
      % XXX we need to expand the polygon by CSX.poly.w/2
      % XXX not done yet. 

      % draw polygon
      CSX = AddPolygon(CSX, material, prio, 2, layer_z, CSX.poly.points);

    end
  end

  % clear polygon data
  CSX.poly = {};

  % example of manually tuning priorities of a single net
  % if (strcmp(CSX.net, 'GND'))
  %   % nested polygons and polyvoids
  %   CSX.prio_material = CSX.prio_cutout + 1;
  %   CSX.prio_cutout = CSX.prio_material + 1;
  % end

end

% not truncated

