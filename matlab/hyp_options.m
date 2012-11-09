% CSX = hyp_options(CSX, s)
% Hyperlynx 'OPTIONS' record. 
% Defines dielectric constant of metal layers.
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

function CSX = hyp_options(CSX, s)

  % not used yet; we assume homogeneous dielectric. 
  % defaults
  if (isfield(s, 'use_die_for_metal')
    CSX.use_die_for_metal = s.use_die_for_metal;
  else
    CSX.use_die_for_metal = false;
  end

end

% not truncated

