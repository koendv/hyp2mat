% CSX = hyp_draw_polyline(CSX, s)
% Support function.
% Draws polyline.
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

function CSX = hyp_draw_polyline(CSX, s)

  % draw polyline

  if (~isfield(CSX, 'poly') || ~isfield(CSX.poly, 'l'))
    return; % nothing to do
  end

  % draw polyline segments 
  if (strcmp(CSX.poly.polytype, 'polyline'))
    for k = 1:(length(CSX.poly.points)-1)
      layer = CSX.poly.l;
      width = CSX.poly.w;
      x1 = CSX.poly.points(1, k);
      y1 = CSX.poly.points(2, k);
      x2 = CSX.poly.points(1, k+1);
      y2 = CSX.poly.points(2, k+1);
      CSX = hyp_draw_trace(CSX, '+', layer, width, [x1 y1], [x2 y2]); % draw the segment
    end
    CSX.poly = {}; % clear polygon data
  end

end

% not truncated

