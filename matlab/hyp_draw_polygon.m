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

  % XXX CSX.poly.polytype = 'polyline'; % force drawing polygon outlines for debugging

  % draw polyline segments 
  if (strcmp(CSX.poly.polytype, 'polyline'))
    CSX = hyp_draw_polyline(CSX, s);
    return
  end

  poly = CSX.poly.points;
  layer = CSX.poly.l;
  width = CSX.poly.w;

  % remove polyvoids, add polygons.
  if (strcmp(CSX.poly.polytype, 'polyvoid'))
    action = '+';
  else
    action ='-';
  end;

  CSX = hyp_draw_poly(CSX, action, layer, width, poly);

  % clear polygon data
  CSX.poly = {};

end

% not truncated

