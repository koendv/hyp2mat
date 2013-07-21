function CSX = AddHyperLynxComponent(CSX, name, prio, port1, port2, varargin)
% function CSX = AddHyperLynxComponent(CSX, name, prio, port1, port2, varargin)
%
% Add a resistor, capacitor or inductance to a CSX model imported from HyperLynx.
% Component layout is limited to x or y direction, and top or bottom layers of pcb.
% 
% arguments:
%   name:       component name.
%   prio:       component priority.
%   port1:      component pad name. 
%   port2:      component pad name. 
% 
% optional arguments:
%   'R', 'C', 'L':  definition of the lumped element properties
%
% examples:
% 50R resistor between pads R1.1 and R1.2 with 1pF parasitic capacitance:
% CSX = AddHyperLynxComponent( CSX, 'R1', 'R1.1', 'R1.2', 'R', 50, 'C', 1e-12 );
%
% See also GetHyperLynxPort, AddLumpedElement
%

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

% Check CSX contains HyperLynx port list.

  if (~isfield(CSX, 'HyperLynxPort'))
    error('CSX not created by ImportHyperLynx()');
  end

% Get pad coordinates pad_start and pad_stop. pad position is either 'top' or 'bottom'. 

[position1, pad1_start, pad1_stop] = GetHyperLynxPort(CSX, port1);
[position2, pad2_start, pad2_stop] = GetHyperLynxPort(CSX, port2);

% Determine component orientation
pad1c=(pad1_start+pad1_stop)/2;
pad2c=(pad2_start+pad2_stop)/2;
v=pad2c-pad1c;
theta=atan2(v(2), v(1)); % theta between -pi and pi.
delta=pi/12; % allow 15 degree play 
vertical=abs((abs(theta) - pi/2)) < delta % true if component has vertical orientation 
horizontal=(abs(theta) < delta) || ((pi - abs(theta)) < delta) % true if component has horizontal orientation 

% layout component
if (vertical)
  % layout component in y-direction
  comp_height = abs(pad1_stop(2) - pad1_start(2));
  comp_start  = [pad1_start(1), (pad1_start(2)+pad1_stop(2))/2, pad1_start(3)];
  comp_stop   = [pad2_stop(1), (pad2_start(2)+pad2_stop(2))/2, pad2_stop(3)];
  direction = 1;
elseif (horizontal)
  % layout component in x-direction
  comp_height = abs(pad1_stop(1) - pad1_start(1));
  comp_start  = [(pad1_start(1)+pad1_stop(1))/2, pad1_start(2), pad1_start(3)];
  comp_stop   = [(pad2_start(1)+pad2_stop(1))/2, pad2_stop(2), pad2_stop(3)];
  direction = 0;
else
  % arbitrary angles not implemented yet
  error ('component orientation has to be 0 or 90 degrees');
end

% Component position has to be 'top' or 'bottom'. 
% Check both pads are on the same side of the board
if (~strcmp(position1, position2))
  error('component pads not on same side of board');
end

if (strcmp(position1, 'top'))
  % component mounted on top of board
  comp_stop = comp_stop + [ 0, 0, comp_height];
elseif (strcmp(position1, 'bottom'))
  comp_stop = comp_stop - [ 0, 0, comp_height];
else
  error('component pads not on top or bottom side of board');
end

% Add component material to CSX model
CSX = AddLumpedElement( CSX, name, direction, 'Caps', 1, varargin{:});

% Add component to CSX model
CSX = AddBox( CSX, name, prio, comp_start, comp_stop );

% not truncated
