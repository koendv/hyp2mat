% CSX = hyp_seg(CSX, s)
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

function CSX = hyp_seg(CSX, s)

  % Check whether to import current net 
  if (~hyp_draw_net(CSX))
    return;
  end

  layer = s.l;

  x1 = s.x1;
  y1 = s.y1;

  x2 = s.x2;
  y2 = s.y2;

  width = s.w;

  % draw copper trace
  CSX = hyp_draw_trace(CSX, '+', layer, width, [x1 y1], [x2 y2]);
  
end

% not truncated
