% [material, start, stop] = GetHyperLynxPort(CSX, port_ref)
% Obtain coordinates and material of HyperLynx pin or pad.
%
% CSX:          CSX-object created by ImportHyperLynx()
% port_ref:     port reference from HyperLynx file.
% If an invalid port reference is given, all valid port references are printed.
%
% material:     port material, created using AddMetal()
% start:        3D port start coordinates
% stop:         3D port stop coordinates

% Example:
% place an excitation at pin 1 of component 'CON1':
% CSX = AddMaterial(CSX, 'PEC');
% [port_material, port_start, port_stop] = GetHyperLynxPort(CSX, 'CON1.1');
% [CSX,portstruct] = AddMSLPort( CSX, 999, 1, 'PEC', port_start, port_stop, 0, [0 0 -1], 'ExcitePort', 'excite');
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

function [material, start, stop] = GetHyperLynxPort(CSX, port_ref)

  % the openEMS AddMSLPort, AddCurvePort functions require a port to be 
  % a rectangular prism with sides parallel to the axis,
  % which is what this function returns.

  % Check CSX contains HyperLynx port list.
  if (~isfield(CSX, 'HyperLynxPort'))
    error('CSX not created by ImportHyperLynx()');
  end

  % find port
  port_found = 0;
  for i = 1:length(CSX.HyperLynxPort)
    if (~port_found && strcmp(port_ref, CSX.HyperLynxPort{i}.ref))
      port_found = 1;
      port = CSX.HyperLynxPort{i};
      port_index = i;
    end
  end

  % exit if port not found
  if (~port_found)
    disp(['Valid ports:' ]);
    for i = 1:length(CSX.HyperLynxPort)
      net = CSX.HyperLynxPort{i}.net;
      ref = CSX.HyperLynxPort{i}.ref;
      disp(['Net: ' net ' Port: ' ref ]);
    end
    error(['port not found: ' port_ref ]);
  end

  % pad dimensions
  x = port.x;
  y = port.y;
  z = port.z;
  dx = port.sx/2;
  dy = port.sy/2;
  alpha = port.angle;

  % pad rotation
  if (port.shape ~= 0) % don't rotate circles/ellipses
    % rotate rectangle/oblong pads
    if (rem(alpha, 180) == 0)
      % do nothing
    elseif (rem(alpha, 90) == 0)
      t = dx;
      dx = dy;
      dy = t;
    else
      % XXX  please extend for arbitrary angles
      error('angle not a multiple of 90');
    end
  end

  % calculate pad corners
  dx = abs(dx);
  dy = abs(dy);
  x1 = x - dx;
  x2 = x + dx;
  y1 = y - dy;
  y2 = y + dy;

  % return values
  material = port.material;
  start = [ x1 y1 z];
  stop  = [ x2 y2 z ];
  
end

% not truncated
