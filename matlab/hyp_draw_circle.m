% CSX = hyp_draw_arc(CSX, propName, prio, center, rad)
% Add a circular disc to CSX and assign to a property with name <propName>.
% The disc is parallel with the xy plane.
%
%  center:  circle center coordinates
%  rad  :   circle radius
%  prio :   primitive priority
%
%  Example:
%       CSX = AddMetal(CSX,'metal'); %create PEC with propName 'metal'
%       CSX = hyp_draw_circle(CSX,'metal',10,[0 0 0],50);
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

function CSX = hyp_draw_circle(CSX, propName, prio, center, rad)

  % We add a cylinder of height 0 at the axis origin, 
  % rotate the resulting disc so it's parallel to the xy plane, 
  % and translate the disc to the circle center.

  CSX = AddCylinder(CSX, propName, prio, [ 0 0 0 ], [ 0 0 0 ], rad, 'Transform', {'Rotate_X', pi/2, 'Translate', center});

end
% not truncated
