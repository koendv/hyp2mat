% CSX = hyp_dielectric(CSX, s)
% Hyperlynx 'DIELECTRIC' subrecord of 'STACKUP' record. 
% Defines a pcb dielectric layer.
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

function CSX = hyp_dielectric(CSX, s)

  % default values

  if (~isfield(s,'l')) % layer name
    CSX.unnamed_layer = CSX.unnamed_layer + 1;
    s.l = [ 'No Name ' num2str(CSX.unnamed_layer) ];
  end

  if (~isfield(s,'co')) % conformal dielectric
    s.co = false; % default is planar
  end

  if (~isfield(s,'pr')) % prepreg or core
    s.pr = true; % default is prepreg
  end

  if (~isfield(s,'er')) % dielectric constant
    if (isfield(s,'c')) % compatibility with older versions
      s.er = s.c;
    else 
      if (s.co) % conformal dielectric
        s.er = 3.3;
      else
        s.er = 4.3; % typical FR4
      end
  end

  if (~isfield(s,'lt')) % loss tangent
    s.lt = 0.020; % FR4
  end

  % vertical scaling
  s.t = s.t * CSX.zscale;

  % add to stackup 
  s.type = 'dielectric';
  CSX.stackup{end + 1} = s;

end

% not truncated

