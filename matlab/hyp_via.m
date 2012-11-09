% CSX = hyp_via(CSX, s)
% Hyperlynx 'VIA' subrecord of 'NET' record. 
% Draws via.
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

function CSX = hyp_via(CSX, s)

  % The difference between the 'VIA' and the 'PIN' subrecord is that 
  % the 'PIN' is associated with a component pin and the via is not.

  % The syntax of the VIA subrecords depends upon Hyperlynx file version.
  %
  % In Hyperlynx v1.x files (obsolete):
  % Syntax: (VIA X=0.0 Y=0.0 D=0.0 L1=layer L2=layer S1=shape S1X=0.0 S1Y=0.0 A1=0.0 S2=shape S2X=0.0 S2Y=0.0 A2=0.0)
  %

  if (~isfield(s,'p'))
    error('Hyperlynx version 1.x via');
  end

  %
  % In Hyperlynx v2.0 and later:
  % Syntax: (VIA X=0.0 Y=0.0 P=padstack)
  %

  % Check whether to import current net
  if (~hyp_draw_net(CSX))
    return;
  end

  CSX = hyp_draw_padstack(CSX, s);

end

% not truncated

