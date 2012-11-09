% CSX = hyp_end(CSX, s)
% Hyperlynx 'END' record. 
% Defines end of board description.
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

function CSX = hyp_end(CSX, s)

  % draw last polygon, if any
  CSX = hyp_draw_polygon(CSX, s);

  % Check all simulated nets have been imported
  net_not_found = {};
  for n = 1:length(CSX.simulated_nets)
    net_name = CSX.simulated_nets{n};
    if (~strcmp(net_name, CSX.all_nets))
      net_not_found{end+1} = net_name;
    end
  end
  if (~isempty(CSX.simulated_nets) && ~isempty(net_not_found))
    warning ('nets not found ');
    disp (['nets not found: ' net_not_found ]);
    disp (['valid nets : ' unique(sort(CSX.all_nets)) ]);
  end 

  % Clear variables
  CSX.all_nets = {};
  CSX.simulated_nets = {};

  % remove temporary fields if not debugging
  if (~CSX.debug)

    for fld = { 'debug' 'all_nets' 'board_outline' 'conductivity' 'current_net' 'draw_board' 'f_max' 'padstack' 'plane_sep' 'poly' 'poly_list' 'stackup' 'net' 'prio_dielectric' 'prio_plane' 'prio_material' 'prio_cutout' 'prio_via' 'prio_drill' 'simulated_nets' 'substrate_epr' 'arc_segments' 'unnamed_layer' 'units' 'use_die_for_metal' 'zscale' }
       if (isfield(CSX, fld))
         CSX = rmfield(CSX, fld);
       end
     end

  end 

end

% not truncated

