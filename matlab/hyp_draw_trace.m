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
 
  % create line segment
  line = [p1; p2; p1]; % polygon contains at least three vertices
  
  % draw line
  CSX = hyp_draw_poly(CSX, action, layer, width/2, line);
  
end

% not truncated
