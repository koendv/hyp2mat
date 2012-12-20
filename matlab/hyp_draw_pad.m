% CSX = hyp_draw_pad(CSX, layer, shape, x, y, sx, sy, angle)
% Support function.
% Draws pad.
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

function CSX = hyp_draw_pad(CSX, layer, shape, x, y, sx, sy, angle)

  % pad rotation
  if (rem(angle, 180) == 0)
    % do nothing
    angle = 0;
  elseif (rem(angle, 90) == 0)
    t = sx;
    sx = sy;
    sy = t;
    angle = 0;
  end

  % draw pad

  if (shape == 0)
    % circular and elliptical pads 
    alpha = linspace(0, 2*pi, CSX.arc_segments+1) + pi/CSX.arc_segments;
    ellipse_x = sx / 2 / cos(pi/CSX.arc_segments) * cos(alpha);
    ellipse_y = sy / 2 / cos(pi/CSX.arc_segments) * sin(alpha);
    pad_poly = [ellipse_x; ellipse_y].';
  elseif (shape == 1)
    % rectangular pads
    corner1 = [ sx/2, sy/2];
    corner2 = [ sx/2, -sy/2];
    corner3 = -corner1;
    corner4 = -corner2;
    pad_poly = [corner1; corner2; corner3; corner4];
  elseif (shape == 2)
    % oblong pads
    if (sx > sy)
      % horizontal oblong pad
      p1 = [ (sx - sy)/2, -sy/2];
      p2 = [ (sx - sy)/2,  sy/2];
      center = [ (sx - sy)/2, 0];
      radius = sy/2;
      arc_right = hyp_arc2poly(CSX, center, p1, p2, radius, 0);
      arc_left  = hyp_arc2poly(CSX, -center, -p1, -p2, radius, 0);
      pad_poly = [ arc_right; arc_left ];
    else
      % vertical oblong pad
      p1 = [  sx/2, (sy - sx)/2];
      p2 = [ -sx/2, (sy - sx)/2];
      center = [ 0, (sy - sx)/2];
      radius = sx/2;
      arc_top    = hyp_arc2poly(CSX, center, p1, p2, radius, 0);
      arc_bottom = hyp_arc2poly(CSX, -center, -p1, -p2, radius, 0);
      pad_poly = [ arc_top; arc_bottom ];
     end
  else 
    warning ([ 'unknown pad shape' num2str(shape) ]);
    return;
  end

  % rotate over angle and translate to [x, y]
  pad_x = pad_poly(:,1);
  pad_y = pad_poly(:,2);

  if (angle ~= 0)
    cot = cos(angle*pi/180);
    sit = sin(angle*pi/180);

    poly_x = x + cot .* pad_x - sit .* pad_y;
    poly_y = y + sit .* pad_x + cot .* pad_y;
  else
    poly_x = x + pad_x;
    poly_y = y + pad_y;
  end

  poly = [ poly_x, poly_y];

  % draw pad
  CSX = hyp_draw_poly(CSX, '+', layer, 0, poly);

end

% not truncated
