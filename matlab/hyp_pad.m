% CSX = hyp_pad(CSX, s)
% Hyperlynx 'PAD' subrecord of 'NET' record. 
% Draws component pad.
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

function CSX = hyp_pad(CSX, s)

%
% The PAD subrecord only occurs in Hyperlynx v1.x files (obsolete).
% Syntax: 
% (PAD X=0.0 Y=0.0 L=layer S=shape SX=0.0 SY=0.0 A=0.0)
%

  error('Hyperlynx version 1.x pad');

end

% not truncated

