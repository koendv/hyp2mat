% CSX = hyp_perimeter_arc(CSX, s)
% Hyperlynx 'PERIMETER_ARC' subrecord of 'BOARD' record. 
% Draws arc segment of board outline.
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

function CSX = hyp_perimeter_arc(CSX, s)

  p1 = [ s.x1 s.y1 ];
  p2 = [ s.x2 s.y2 ];
  center = [ s.xc s.yc ];
  radius = s.r;

  peri_arc = hyp_arc2poly(CSX, center, p1, p2, radius, false); % 'PERIMETER_ARC' draws arc counterclockwise

  % Add arc to board outline
  separator = [ NaN NaN ];
  CSX.board_outline = [ CSX.board_outline ; separator ; peri_arc];

% not truncated

