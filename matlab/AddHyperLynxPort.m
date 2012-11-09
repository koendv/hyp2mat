% [CSX,port] = AddHyperLynxPort(CSX, pin_ref, pin_mode)
% Add port to pin 'pin_ref'.
%
% CSX:          CSX-object created by ImportHyperLynx()
% pin_ref:      pin reference from HyperLynx file.
% pin_mode:     One of 'driver' or 'load'.
% If an invalid pin reference is given, all valid pin references are printed.
%
% Examples:
% [CSX,port1] = AddHyperLynxPort(CSX, 'CON1.1', 'driver')
% places an excitation at pin 1 of component 'CON1'
% [CSX,port1] = AddHyperLynxPort(CSX, 'U1.2', 'load')
% places a load at pin 2 of component 'U1'
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

function [CSX,port] = AddHyperLynxPort(CSX, pin_ref, pin_mode)

  % Check CSX contains HyperLynx pin list.
  if (~isfield(CSX, 'pin_list'))
    error('CSX not created by ImportHyperLynx()');
  end

  % find pin
  pin_found = 0;
  for i = 1:length(CSX.pin_list)
    if (~pin_found && strcmp(pin_ref, CSX.pin_list{i}.ref))
      pin_found = 1;
      pin = CSX.pin_list{i};
      pin_index = i;
    end
  end

  % exit if pin not found
  if (~pin_found)
    disp(['Valid pins:' ]);
    for i = 1:length(CSX.pin_list)
      net = CSX.pin_list{i}.net;
      ref = CSX.pin_list{i}.ref;
      disp(['Net: ' net ' Pin: ' ref ]);
    end
    error(['pin not found: ' pin_ref ]);
  end
 
  % info
  disp([ sprintf('%-7s', pin_mode), pin_ref, ' (', pin.material , ')' ] ); 
  
  % Create port
  if (strcmp(pin_mode, 'load'))
    [CSX, port] = AddMSLPort(CSX, 999, pin_index, pin.material, pin.start, pin.stop, 0, [0 0 -1]); 
  elseif (strcmp(pin_mode, 'driver'))
    [CSX, port] = AddMSLPort(CSX, 999, pin_index, pin.material, pin.start, pin.stop, 0, [0 0 -1], 'ExcitePort', pin.ref); 
  else 
    error('pin_mode should be driver or load')
  end
  
end

% not truncated

