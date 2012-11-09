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
      t = {};
      t.lineno = CSX.poly.lineno;
      t.l = CSX.poly.l;
      t.x1 = CSX.poly.points(1, k);
      t.y1 = CSX.poly.points(2, k);
      t.x2 = CSX.poly.points(1, k+1);
      t.y2 = CSX.poly.points(2, k+1);
      t.w = CSX.poly.w;
      CSX = hyp_seg(CSX, t); % draw the segment
    end
    CSX.poly = {}; % clear polygon data
  end

end

% not truncated

