% CSX = hyp_units(CSX, s)
% Hyperlynx 'UNITS' record. 
% Specifies measurement system (english/metric) for the rest of the file.
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

function CSX = hyp_units(CSX, s)

  % convert everything to m
  if strcmp(s.units, 'english')
    CSX.units = 0.0254;          % lengths in inches. 1 in = 2.54 cm = 0.0254 m
    if strcmp(s.metal, 'weight')
      CSX.zscale = 1.341;        % metal thickness in ounces/ft2. 1 oz = 1.341 mil
    else
      CSX.zscale = 1;            % metal thickness in inches
    end
  else
    CSX.units = 0.01;            % lengths in centimeters. 1 cm = 0.01 m
    if strcmp(s.metal, 'weight')
      CSX.zscale = 0.1116;       % metal thickness in grams/cm2. 1 gr/cm2 = 0.1116 cm
    else
      CSX.zscale = 1;            % metal thickness in centimeters
    end
  end

end

% not truncated

