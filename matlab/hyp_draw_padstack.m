% CSX = hyp_draw_padstack(CSX, s)
% Support function.
% Draws via and pin padstack.
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

function CSX = hyp_draw_padstack(CSX, s)

  x = s.x;
  y = s.y;
  padstack_name = s.p;

  drill_size = 0;
  stack = {};
 
  % look up padstack
  for i = 1:length(CSX.padstack)
    if (strcmp(CSX.padstack{i}.padstack_name, padstack_name))
      drill_size = CSX.padstack{i}.drill_size;
      stack = CSX.padstack{i}.stack;
    end
  end

  % if there is a via, add to list of vias
  if (drill_size ~= 0)
    via = {};
    via.z1 = CSX.stackup{1}.z;
    via.z2 = CSX.stackup{end}.z;
    via.x = x;
    via.y = y;
    via.radius = drill_size/2;

    % add via to list
    CSX.via_list{end+1} = via;
  end

  % loop through padstack
  for i = 1:length(stack)
    layer = stack{i}.layer_name;
    pad_shape = stack{i}.pad_shape;
    pad_sx = stack{i}.pad_sx;
    pad_sy = stack{i}.pad_sy;
    pad_angle = stack{i}.pad_angle;
    pad_type = stack{i}.pad_type;
    % look up layer
    for j = 1:length(CSX.stackup)
      % either this layer exactly, or MDEF for all metal layers.
      if (strcmp(CSX.stackup{j}.l, layer) || (strcmp('MDEF', layer) && ~strcmp(CSX.stackup{j}.type, 'dielectric')))

        pad_layer = CSX.stackup{j}.l;
        pad_z = CSX.stackup{j}.z;
        pad_t = CSX.stackup{j}.t;

        % if there is a pin reference, add to list of pins.
        % we'll use this to connect ports later.
        if (isfield(s,'r'))
          port.x = x;                      % x coordinate
          port.y = y;                      % y coordinate
          port.z = pad_z;                  % z coordinate
          port.t = pad_t;                  % copper thickness
          port.sx = pad_sx;                % pad width
          port.sy = pad_sy;                % pad height
          port.angle = pad_angle;          % pad rotation
          port.shape = pad_shape;          % pad shape. 0 = circular, 1 = rectangular, 2 = oblong
          port.ref = s.r;                  % reference
          port.net = CSX.current_net;      % net name
          port.layer = pad_layer;          % layer

          if (pad_z == CSX.stackup{1}.z)   % side of board
            port.side = 'top';
          elseif (pad_z == CSX.stackup{end}.z)
            port.side = 'bottom';
          else
            port.side = '';
          end
         
          CSX.HyperLynxPort{end+1}=port;   % add to list
        end

        CSX = hyp_draw_pad(CSX, pad_layer, pad_shape, x, y, pad_sx, pad_sy, pad_angle);

      end
    end
  end

end

% not truncated

