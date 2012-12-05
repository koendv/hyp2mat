% CSX = hyp_draw_padstack(CSX, s)
% Support function.
% Draws via or pin padstack.
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

  % XXX Needs work

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

  % drill hole
  if (drill_size ~= 0)
    z1 = CSX.stackup{1}.z;
    z2 = CSX.stackup{end}.z;
    p1 = [ x y z1 ];
    p2 = [ x y z2 ];
    radius = drill_size/2;

    % copper in via
    CSX = AddCylinder(CSX, 'via', CSX.prio_via, p1, p2, radius); % via copper
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

        % choose material
        material = CSX.stackup{j}.material;
        prio = CSX.prio_material;

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
          port.material = material;        % material
          CSX.HyperLynxPort{end+1}=port;   % add to list
        end

        % pad rotation
        if (rem(pad_angle, 180) == 0)
          % do nothing
          pad_angle = 0;
        elseif (rem(pad_angle, 90) == 0)
          t = pad_sx;
          pad_sx = pad_sy;
          pad_sy = t;
          pad_angle = 0;
        else
          warning(['pad angle ' num2str(pad_angle) ' not a multiple of 90']);
        end

        % draw pad
        if (pad_shape == 0)
          if (pad_sx == pad_sy) 

            % circular pads
            p1 = [ x y pad_z];
            CSX = hyp_draw_circle(CSX, material, prio, p1, pad_sx/2);
          else

            % elliptical pads 
            alpha = linspace(0, 2*pi, CSX.arc_segments);
            ellipse_x = pad_sx / 2 * cos(alpha);
            ellipse_y = pad_sy / 2 * sin(alpha);
            ellipse = [ellipse_x.' ellipse_y.'].';
            p1 = [ x y 0];
            if (pad_angle == 0)
              CSX = AddPolygon(CSX, material, prio, 2, pad_z, ellipse, 'Transform', {'Translate', p1});
            else
              CSX = AddPolygon(CSX, material, prio, 2, pad_z, ellipse, 'Transform', {'Rotate_Z', pad_angle*pi/180, 'Translate', p1});
            end
          end
        end
        if (pad_shape == 1)

          % rectangular pads
          p1 = [ -pad_sx/2, -pad_sy/2, pad_z];
          p2 = [  pad_sx/2,  pad_sy/2, pad_z];
          p3 = [ x, y, 0 ];
          if (pad_angle == 0)
            CSX = AddBox(CSX, material, prio, p1, p2, 'Transform', {'Translate', p3});
          else
            CSX = AddBox(CSX, material, prio, p1, p2, 'Transform', {'Rotate_Z', pad_angle*pi/180, 'Translate', p3});
          end

        end
        if (pad_shape == 2)
          % oblong pads

          if (pad_sx > pad_sy)
            % horizontal oblong pad
            s = {};
            s.x1 = (pad_sx - pad_sy)/2;
            s.y1 = -pad_sy/2;
            s.x2 = (pad_sx - pad_sy)/2;
            s.y2 = pad_sy/2;
            s.xc = (pad_sx - pad_sy)/2;
            s.yc = 0;
            s.r  = pad_sy/2;
            arc_right = hyp_draw_arc(CSX, s, 0);
            s.x1 = -(pad_sx - pad_sy)/2;
            s.y1 = pad_sy/2;
            s.z1 = 0;
            s.x2 = -(pad_sx - pad_sy)/2;
            s.y2 = -pad_sy/2;
            s.z2 = 0;
            s.xc = -(pad_sx - pad_sy)/2;
            s.yc = 0;
            s.r  = pad_sy/2;
            arc_left = hyp_draw_arc(CSX, s, 0);
            oblong = [ arc_right arc_left ];
          else
            % vertical oblong pad
            s = {};
            s.x1 = pad_sx/2;
            s.y1 = (pad_sy - pad_sx)/2;
            s.z1 = 0;
            s.x2 = -pad_sx/2;
            s.y2 = (pad_sy - pad_sx)/2;
            s.z2 = 0;
            s.xc = 0;
            s.yc = (pad_sy - pad_sx)/2;
            s.r  = pad_sx/2;
            arc_top = hyp_draw_arc(CSX, s, 0);
            s.x1 = -pad_sx/2;
            s.y1 = -(pad_sy - pad_sx)/2;
            s.z1 = 0;
            s.x2 = pad_sx/2;
            s.y2 = -(pad_sy - pad_sx)/2;
            s.z2 = 0;
            s.xc = 0;
            s.yc = -(pad_sy - pad_sx)/2;
            s.r  = pad_sx/2;
            arc_bottom = hyp_draw_arc(CSX, s, 0);
            oblong = [ arc_top arc_bottom ];
          end
          % create oblong pad
          p1 = [ x y 0];
          if (pad_angle == 0)
            CSX = AddPolygon(CSX, material, prio, 2, pad_z, oblong, 'Transform', {'Translate', p1});
          else
            CSX = AddPolygon(CSX, material, prio, 2, pad_z, oblong, 'Transform', {'Rotate_Z', pad_angle*pi/180, 'Translate', p1});
          end
        end
      end
    end
  end

end

% not truncated

