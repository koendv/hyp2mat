function CSX = AddHyperLynxDielectric(CSX, name, epsilon, loss_tangent)
% function CSX = AddHyperLynxDielectric(CSX, name, epsilon, loss_tangent)
%
% Add a dielectric with the given name to a CSX model imported from HyperLynx.
% 
% arguments:
%   name:         material name.
%   epsilon:      relative electric permittivity
%   loss_tangent: loss tangent
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
% For a discussion of the parameters, see http://openems.de/index.php/Dispersive_Material_Property 

% Default substrate is a lossless dielectric.
CSX = AddMaterial( CSX, name);
CSX = SetMaterialProperty( CSX, name, 'Epsilon', epsilon, 'Mue', 1 );
%CSX = SetMaterialProperty( CSX, name, 'Kappa', kappa); % optional conductivity

% Drude material
%CSX = AddLorentzMaterial(CSX, name);
%CSX = SetMaterialProperty(CSX, name, 'Epsilon', epsilon, 'Kappa', kappa);
%CSX = SetMaterialProperty(CSX, name, 'EpsilonPlasmaFrequency', 5e9, 'EpsilonRelaxTime', 5e-9);

% not truncated
