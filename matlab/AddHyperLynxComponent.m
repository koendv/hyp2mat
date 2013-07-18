function CSX = AddHyperLynxComponent(CSX, name, prio, port1, port2, direction, varargin)
% function CSX = AddHyperLynxComponent(CSX, name, prio, port1, port2, direction, varargin)
%
% Add a two-port lumped element to the CSX model.
% 
% arguments:
%   name:       component name, e.g. 'R1'
%   prio:       component priority
%   port1:      component pad name. 
%   port2:      component pad name. 
%   direction:  0 for x-orienation, 1 for y-orientation of the lumped element
% 
% optional arguments:
%   'R', 'C', 'L':  definition of the lumped element properties
%
% examples:
% lumped element capacitor in x-direction with 1pF
% CSX = AddHyperLynxComponent( CSX, 'C1', 0, 'C1.1', 'C1.2', 'C', 1e-12 );
%
% See also AddLumpedElement, AddBox
%

% Get pad coordinates pad_start and pad_stop. pad position is either 'top' or 'bottom'. 

[position1, pad1_start, pad1_stop] = GetHyperLynxPort(CSX, port1);
[position2, pad2_start, pad2_stop] = GetHyperLynxPort(CSX, port2);

% Layout component
if (direction == 0)
  % layout component in x-direction
  comp_height = abs(pad1_stop(1) - pad1_start(1));
  comp_start  = [(pad1_start(1)+pad1_stop(1))/2, pad1_start(2), pad1_start(3)];
  comp_stop   = [(pad2_start(1)+pad2_stop(1))/2, pad2_stop(2), pad2_stop(3)];
elseif (direction == 1)
  comp_height = abs(pad1_stop(2) - pad1_start(2));
  comp_start  = [pad1_start(1), (pad1_start(2)+pad1_stop(2))/2, pad1_start(3)];
  comp_stop   = [pad2_stop(1), (pad2_start(2)+pad2_stop(2))/2, pad2_stop(3)];
else
  error('component direction not 0 or 1');
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
