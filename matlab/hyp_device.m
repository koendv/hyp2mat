% CSX = hyp_device(CSX, s)
% Hyperlynx 'DEVICE' record. 
% Specifies pcb components: resistors, capacitors, ICs, ...
% Required record.
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

function CSX = hyp_device(CSX, s)
  
  % set up the pcb layers
  if (CSX.draw_board)
    CSX.draw_board = false;    % draw board only once
    CSX = hyp_draw_board(CSX); % draw the pcb
  end

  % device data
  % mandatory fields are reference and layer.
  device = {};

  if (isfield(s,'ref')) % reference
    % remove non-alpha characters, or WriteOpenEMS may crash if characters such as '%' are present
    device.reference = regexprep(s.ref, '[^a-zA-Z0-9]', '');
  else
    device.reference = '';
  end

  if (isfield(s,'name')) % name
    device.name = regexprep(s.name, '[^a-zA-Z0-9]', '');
  end

  if (isfield(s,'val')) % value
    device.value = s.val;
  end

  if (isfield(s,'l')) % layer
    device.layer = s.l;
  else
    device.layer = '';
  end

  if (isfield(s,'pkg')) % package
    device.package = s.pkg;
  end

  CSX.HyperLynxDevice{end+1} = device;

end

% not truncated

