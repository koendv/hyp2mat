% q = polygon_extend(p, width, arc_segments);
%
% Extends (buffers, expands) a polygon p by a given width.
% Returns the extended polygon.
% circles are drawn using 'arc_segments' arcs.
%
% Requires polybool extension from Ulf Griessman
% https://sites.google.com/site/ulfgri/numerical/polybool
%
% Example:
%   p = [ 0 0;  1 0  ; 1 1; 0 1 ; 0 0 ];
%   q= polygon_extend(p, 0.5);
%   drawPolygon(q);
%
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

function q = polygon_extend(p, width, arc_segments);

  % trivial case
  if (width == 0)
    q = p;
    return;
  end

  if (isempty(p))
    q = p;
    return;
  end

  % We extend the polygon by iterating over the vertices, 
  % putting a circle on top of every vertex, 
  % and putting a rectangle on top of every edge.

  % calculate circle
  alpha = linspace(0, 2*pi, arc_segments+1) + pi/arc_segments;
  circle_x = width / cos(pi/arc_segments) * cos(alpha);
  circle_y = width / cos(pi/arc_segments) * sin(alpha);
  circle = [ circle_x; circle_y].';

  q = p;
  % add circles on top of every vertex
  for j = 1:length(p) 
    xc  = p(j,1);
    yc  = p(j,2);
    overlay_circle = [xc+circle_x; yc+circle_y].';
    % add to polygon
    q = polygon_arith(q, '+', polycw(overlay_circle));
  end

  % add rectangles on top of every edge
  p(end+1,:) = p(1,:); % close polygon
  for j = 1:length(p)-1;
    % determine edge points
    p1 = p(j,:);
    p2 = p(j+1,:);
    if (p1 == p2)
      continue;
    end

    % create rectangle
    x1 = p1(1);
    y1 = p1(2);
    x2 = p2(1);
    y2 = p2(2);

    d = sqrt((x2-x1)^2+(y2-y1)^2);
    dx = (y2-y1)*width/d;
    dy = (x2-x1)*width/d;

    overlay_rectangle = [ x1+dx y1-dy; x2+dx y2-dy; x2-dx y2+dy; x1-dx y1+dy];
    % add to polygon
    q = polygon_arith(q, '+', polycw(overlay_rectangle));
    
  end

  return;
  
end 
% not truncated

