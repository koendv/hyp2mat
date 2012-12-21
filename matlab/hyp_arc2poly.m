% arc = hyp_arc2poly(CSX, center, p1, p2, radius, clockwise)
% Approximate an arc using linear segments. 
% p1 and p2 are the endpoints of the arc.
% Returns a polygon.
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

function arc = hyp_arc2poly(CSX, center, p1, p2, radius, clockwise)

  x1 = p1(1);
  y1 = p1(2);
  x2 = p2(1);
  y2 = p2(2);
  xc = center(1);
  yc = center(2);

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

  % calculate number of segments to have an acceptable error

  max_err = CSX.arc_maxerr / CSX.units; % maximum error between perfect circular arc and polygonal approximation
  segments = CSX.arc_segments;
  err = inf;
  while (err > max_err)
    err = radius * (1-cos(pi/segments));
    if (err > max_err)
      segments = segments + 4;
    end
  end

  % a full circle is drawn using 'segments' segments;
  % a 90 degree arc using segments/4
  segments = round(segments*abs(beta-alpha)/(2*pi)) + 1;

  % sanity checks.
  % if (segments > CSX.arc_segments+1)
  %   segments = CSX.arc_segments+1;
  % end

  if (segments < 2)
    segments = 2;
  end

  % arc from (x1, y1) to (x2, y2);
  angle = linspace(alpha, beta, segments);
  [x, y] = pol2cart(angle, radius);
  x = x + xc;
  y = y + yc;

  % clean up
  x(1) = x1;
  y(1) = y1;
  x(end) = x2;
  y(end) = y2;

  % output
  arc = [x; y].';

end
% not truncated
