function CSX = AddHyperLynxMetal3D(CSX, name, resistivity, thickness)
% function CSX = AddHyperLynxMetal3D(CSX, name, resistivity, thickness)
%
% Add a metal as a 3D object to a CSX model imported from HyperLynx.
% 
% arguments:
%   name:         material name.
%   resistivity:  metal resistivity
%   thickness:    thickness of the metal layer, in m.
% 
% See also AddMaterial, AddDebyeMaterial, AddLorentzMaterial
%

% Copyright 2014 Koen De Vleeschauwer.
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

% Copy this script to your project directory to customize. 

disp('Using 3D metal: slow convergence.');

if (resistivity > 0.0)
  conductivity = 1.0 / resistivity;
  CSX = AddMaterial( CSX, name);
  CSX = SetMaterialProperty( CSX, name, 'Kappa', conductivity); % lossy conductor
else
  CSX = AddMetal(CSX, name); % perfect conductor
end

% not truncated
