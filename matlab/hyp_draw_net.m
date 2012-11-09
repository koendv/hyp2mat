% ans = hyp_draw_net(CSX)
% Check whether the current NET needs to be included in the output or skipped.
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

function ans = hyp_draw_net(CSX)

  %
  % - if the list of nets to be drawn is empty, draw all nets.
  % - if the current net is member of the list of nets to be imported, draw the current net
  %

  ans = isempty(CSX.simulated_nets) || any(strcmp(CSX.current_net, CSX.simulated_nets));

end

% not truncated

