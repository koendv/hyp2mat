% CSX = hyp_arc(CSX, s)
% Hyperlynx 'ARC' subrecord of 'NET' record. 
% Draws arc metal trace segment.
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

function CSX = hyp_arc(CSX, s)

  % Check whether to import current net 
  if (~hyp_draw_net(CSX))
    return;
  end

  % calculate arc
  p1 = [ s.x1 s.y1 ];
  p2 = [ s.x2 s.y2 ];
  center = [ s.xc s.yc ];
  radius = s.r;
  arc = hyp_arc2poly(CSX, center, p1, p2, radius, true); % 'ARC' draws arc clockwise

  % draw segments
  for k = 1:(length(arc)-1)
    t = {};
    t.lineno = s.lineno;
    t.l = s.l;
    t.x1 = arc(1, k);
    t.y1 = arc(2, k);
    t.x2 = arc(1, k+1);
    t.y2 = arc(2, k+1);
    t.w = s.w;
    CSX = hyp_seg(CSX, t);
    CSX = hyp_draw_trace(CSX, '+', layer, width, [x1 y1], [x2 y2]);
  end

end
% not truncated
