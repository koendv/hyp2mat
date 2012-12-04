% epsilon = GetEpsilon(CSX)
%
% Obtain dielectric constant of CSX model
%
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


function epsilon = GetEpsilon(CSX)

  if (~isstruct(CSX))
    error ('CSX struct expected');
  end

  if (~isfield(CSX, 'RectilinearGrid'))
    error('CSX does not have a rectilinear grid');
  end

  %
  % Iterate over CSX and return maximum epsilon.
  % Maximum epsilon determines smallest resolution.
  %
  epsilon = 1;
  if (~isfield(CSX, 'Properties'))
    error ('CSX struct expected');
  end
  if (isstruct(CSX.Properties))
    prop_fn = fieldnames(CSX.Properties);
    for p = 1:length(prop_fn)
      property = CSX.Properties.(prop_fn{p});
      for m = 1:length(property)
        material=property{m};
        if (isfield(material, 'Property') && isfield(material.Property, 'ATTRIBUTE') && isfield(material.Property.ATTRIBUTE, 'Epsilon'))
          if (material.Property.ATTRIBUTE.Epsilon > epsilon)
            epsilon = material.Property.ATTRIBUTE.Epsilon;
          end
        end
      end
    end
  end
 
end
%not truncated
