% CSX = hyp_draw_trace(CSX, action, layer, width, p1, p2)
% Hyperlynx 'SEG' subrecord of 'NET' record. 
% Draws straight metal trace segment.
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

function CSX = hyp_draw_trace(CSX, action, layer, width, p1, p2)

  % A hyperlynx line segment is drawn, ending in two half-circles:
  %
  %              -------------------------------         ^
  %             /                               \        |
  %            |                                 |     width
  %             \                               /        |
  %              -------------------------------         V
  %
  %              <----------- length ---------->
  %
 
  % begin and end coordinates 
  x1 = p1(1);
  y1 = p1(2);

  x2 = p2(1);
  y2 = p2(2);

  % determine angle and length
  [theta, length] = cart2pol(x2-x1, y2-y1);
  
  % draw line segment as LinPoly
  [dy, dx] = pol2cart(theta, width/2); % XXX Check: width or width /2?
  
  p3 = [ x2 + dx, y2 - dy];
  p4 = [ x2 - dx, y2 + dy];
  center = [ x2, y2 ];
  radius = width/2;
  
  arc_right = hyp_arc2poly(CSX, center, p3, p4, radius, 0);
  
  p5 = [ x1 - dx, y1 + dy];
  p6 = [ x1 + dx, y1 - dy];
  center = [ x1, y1 ];
  radius = width/2;
  
  arc_left = hyp_arc2poly(CSX, center, p5, p6, radius, 0);
  
  line = [ arc_right; arc_left ];
  
  % draw line
  CSX = hyp_draw_poly(CSX, action, layer, 0, line);
  
end

% not truncated
