% [side, start, stop, centre ] = GetHyperLynxPort(CSX, port_ref)
% Obtain coordinates of HyperLynx pin or pad.
%
% CSX:          CSX-object created by ImportHyperLynx()
% port_ref:     port reference from HyperLynx file.
% If an invalid port reference is given, all valid port references are printed.
%
% side:         board side, 'top', 'bottom' or ''.
% start:        3D port start coordinates
% stop:         3D port stop coordinates
% centre:       3D port centre coordinates

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

function [side, start, stop, centre] = GetHyperLynxPort(CSX, port_ref)

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
    % display valid port names
    portnames = {};
    for i = 1:length(CSX.HyperLynxPort)
      portnames(end+1) = CSX.HyperLynxPort{i}.ref;
    end
    portnames = sort(portnames);
    fprintf('Valid ports:');
    for i = 1:length(portnames)
      fprintf(' %s', portnames{i});
    end
    fprintf('\n');
    % exit with error message
    error(['port not found: ' port_ref ]);
  end

  % return values
  side = port.position;
  start = [ port.x1 port.y1 port.z ];
  stop  = [ port.x2 port.y2 port.z ];
  centre  = [ port.xc port.yc port.z ];
  
end

% not truncated
