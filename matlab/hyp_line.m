% CSX = hyp_line(CSX, s)
% Hyperlynx 'LINE' subrecord of 'POLYGON' and 'POLYVOID' record. 
% Draws straight segment of polygon.
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

function CSX = hyp_line(CSX, s)

  CSX.poly.points(1, end+1) = s.x;
  CSX.poly.points(2, end) = s.y;

end

% not truncated

