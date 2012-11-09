% arc = hyp_draw_arc(CSX, s, clockwise)
% Approximate an arc using linear segments.
% Returns array of (x,y) coordinates.
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

function arc = hyp_draw_arc(CSX, s, clockwise)

  x1 = s.x1;
  y1 = s.y1;
  x2 = s.x2;
  y2 = s.y2;
  xc = s.xc;
  yc = s.yc;
  r = s.r;

  [alpha, r1] = cart2pol(x1-xc, y1-yc);
  [beta, r2] = cart2pol(x2-xc, y2-yc);

  if (clockwise)
    % draw arc clockwise from (x1,y1) to (x2,y2)
    if (alpha < beta)
      alpha = alpha + 2 * pi;
    end
  else
    % draw arc counterclockwise from (x1,y1) to (x2,y2)
    if (beta < alpha)
      beta = beta + 2 * pi;
    end
    % draw a circle if starting and end points are the same
    if ((x1 == x2) && (y1 == y2)) 
      beta = alpha + 2 * pi;
    end
  end

  % calculate number of segments
  % a full circle is drawn using CSX.arc_segments segments;
  % a 90 degree arc using CSX.arc_segments/4

  segments = round(CSX.arc_segments*abs(beta-alpha)/(2*pi)) + 1;

  % sanity checks.
  if (segments > CSX.arc_segments+1)
    segments = CSX.arc_segments+1;
  end

  if (segments < 2)
    segments = 2;
  end

  % arc from (x1, y1) to (x2, y2);
  angle = linspace(alpha, beta, segments);
  [x, y] = pol2cart(angle, r);
  x = x + xc;
  y = y + yc;

  arc = [x.' , y.'].';

end
% not truncated
