% CSX = ImportHyperLynx(CSX, filename, simulated_nets)
% load Hyperlynx file 'filename' into CSX .
% The optional argument simulated_nets is a string or a cell array of strings.
% Examples:
% CSX = ImportHyperLynx(CSX, 'board.hyp' )
% will import the complete Hyperlynx file 'board.hyp' into the struct CSX.
%
% CSX = ImportHyperLynx(CSX, 'board.hyp', 'CLK_P' )
% will import only the CLK_P net from the Hyperlynx file 'board.hyp' into the struct CSX.
%
% CSX = ImportHyperLynx(CSX, 'board.hyp', {'GND', 'CLK_P', 'CLK_N'} )
% will import only the GND, CLK_P and CLK_N nets.
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

function CSX = ImportHyperLynx(CSX, filename, simulated_nets)

  % determine nets to import
  if ((nargin ~= 2) && (nargin ~= 3))
    error('Wrong number of arguments');
  end

  if (~isstruct(CSX))
    error ('expecting CSX struct as first argument');
  end

  if (~ischar(filename))
    error ('expecting filename string as second argument');
  end

  if (nargin == 2)
    % two arguments: import all nets
    CSX.simulated_nets = {};
  elseif (ischar(simulated_nets))
    % third argument is a string: import a single net
    CSX.simulated_nets = { };
    CSX.simulated_nets = { simulated_nets };
  elseif (iscellstr(simulated_nets))
    % third argument is a cell array of strings: import a list of nets
    CSX.simulated_nets = simulated_nets;
  else
    % default: import all nets
    warning('importing all nets');
    CSX.simulated_nets = {};
  end

  % conversion
  cmd = [ '../hyp2mat -o pcb.m -f pcb ''' filename '''' ];
  % convert .hyp to .m
  status = system(cmd); % security implications?
  if (status == 0) 
    % run generated pcb.m
    CSX = pcb(CSX); 
    delete 'pcb.m';
  else
    error([ 'error executing ' cmd ]); 
  end

end

% not truncated

