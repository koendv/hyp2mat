% CSX = hyp_plane(CSX, s)
% Hyperlynx 'PLANE' subrecord of 'STACKUP' record. 
% Defines a pcb power or signal plane. 
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

function CSX = hyp_plane(CSX, s)

  % default values

  if (~isfield(s,'l')) % layer name
    CSX.unnamed_layer = CSX.unnamed_layer + 1;
    s.l = [ 'No Name ' num2str(CSX.unnamed_layer) ];
  end

  if (~isfield(s,'br')) % bulk resistivity
    if (isfield(s,'c'))
      s.br = s.c;
    else
      s.br = 1.724e-8; % copper typical
  end

  if (~isfield(s,'er')) % dielectric constant
    s.er = 4.3; % FR4 typical
  end

  if (~isfield(s,'lt')) % loss tangent
    s.lt = 0.020; % FR4 typical
  end

  if (~isfield(s,'tc')) % temperature coefficient
    s.tc = 0.00393; % copper typical
  end

  if (~isfield(s,'ps')) % plane separation
    s.ps = CSX.plane_sep; % default plane separation
  end

  % vertical scaling
  s.t = s.t * CSX.zscale;
  s.ps = s.ps * CSX.zscale;

  % add to stackup
  s.type = 'plane';
  CSX.stackup{end + 1} = s;

end

% not truncated

