% CSX = hyp_version(CSX, s)
% Hyperlynx 'VERSION' record. 
% Specifies version number
% required record; must be first record of the file.
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

function CSX = hyp_version(CSX, s)

  % check version
  if (s.vers < 2.0)
    % Hyperlynx version 1.x VIA and PAD records differ from v2.x
    error('Hyperlynx version 1.x not supported')
  end

  % initalize variables
  CSX.debug = false;         % set to false to suppress messages
  CSX.debug = true;         % set to true for debug messages
  CSX.board_outline = [];    % coordinates of pcb border
  CSX.draw_board = true;     % flag
  CSX.unnamed_layer = 0;     % number of unnamed layers
  CSX.padstack = {};         % list of padstacks in 'PADSTACK' record
  CSX.plane_sep = 0;         % plane separation in 'PLANE_SEP' record
  CSX.poly = {};             % current polygon in 'POLYGON' record.
  CSX.poly_list = {};        % list of polygon id's and parameters
  CSX.stackup = {};          % list of pcb layers in 'STACKUP' record.
  CSX.via_list = {};         % list of vias
  CSX.current_net = '';      % name of current net in 'NET' record.
  CSX.all_nets = {};         % list of all net names
  CSX.HyperLynxPort = {};    % list of all i/o pins/pads

  % parameters
  CSX.substrate_epr   = 4.3;  % FR4 dielectric
  CSX.conductivity    = 56e6; % copper conductivity

  % arc segments
  CSX.arc_segments    = 8;   % number of linear segments per arc; needs to be a multiple of 4

end

% not truncated

