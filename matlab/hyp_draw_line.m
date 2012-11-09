% CSX = hyp_draw_line(CSX, propName, prio, p1, p2, z, width)
% Add a hyperlynx line segment to  CSX and assign to a property with name <propName>.
% The line segment is parallel with the xy plane.
%
%  p1   :   begin point
%  p2   :   end point
%  z    :   height above xy plane
%  width:   line width
%  prio :   primitive priority
%
%  Example:
%       CSX = AddMetal(CSX,'metal'); %create PEC with propName 'metal'
%       CSX = hyp_draw_line(CSX,'metal', 10, [-2 -2], [2 2], 50, 0.5);
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

function CSX = hyp_draw_line(CSX, propName, prio, p1, p2, z, width)

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
  % 

  % determine angle and length
  x1 = p1(1);
  y1 = p1(2);
  x2 = p2(1);
  y2 = p2(2);
  [theta, length] = cart2pol(x2-x1, y2-y1);

%
% two methods for drawing a line segment: 
% as a polygon or as a box and two cylinders.
% 2do: Choose optimal solution.
%

if 0
  % draw line segment as LinPoly
  [dy, dx] = pol2cart(theta, width);

  % draw line horizontally
  s = {};
  s.x1 = x2 + dx;
  s.y1 = y2 - dy;
  s.x2 = x2 - dx;
  s.y2 = y2 + dy;
  s.xc = x2;
  s.yc = y2;
  s.r  = width/2;
  arc_right = hyp_draw_arc(CSX, s, 0);
  s.x1 = x1 - dx;
  s.y1 = y1 + dy;
  s.x2 = x1 + dx;
  s.y2 = y1 - dy;
  s.xc = x1;
  s.yc = y1;
  s.r  = width/2;
  arc_left = hyp_draw_arc(CSX, s, 0);
  line = [ arc_right arc_left ];

  % draw line
  CSX = AddPolygon(CSX, propName, prio, 2, z, line);

else

  % draw line segment as one Box and two Cylinders

  % draw trace segment
  if (length ~= 0)
    p1 = [ 0, width/2, z ];
    p2 = [ length, -width/2, z];
    p3 = [ x1, y1, 0 ];

    CSX = AddBox(CSX, propName, prio, p1, p2, 'Transform', {'Rotate_Z', theta, 'Translate', p3});
  end
    
  % draw endpoint 1
  p1 = [ x1 y1 z ];
  CSX = hyp_draw_circle(CSX, propName, prio, p1, width/2 );
    
  % draw endpoint 2
  p2 = [ x2 y2 z ];
  CSX = hyp_draw_circle(CSX, propName, prio, p2, width/2 );
end

end
% not truncated
