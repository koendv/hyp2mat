% q = polygon_arith(p1, operand, p2);
%
% Adds or subtracts two polygons.
% Returns the resulting polygon.
% Requires polybool extension from Ulf Griessman
% https://sites.google.com/site/ulfgri/numerical/polybool
%
% Example:
%   p1 = [ 0 0;  1 0  ; 1 1; 0 1 ; 0 0 ];
%   p2 = 2 * p1;
%   polygon_arith(p1, '+', p2);
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


function q = polygon_arith(p1, operand, p2);

  % trivial cases 
  if isempty(p1)
    if strcmp(operand, '+')
      q = p2;
    else
      q = [];
    end;
    return;
  end;

  if isempty(p2)
    q = p1;
    return;
  end;

  % non-trivial cases: use polybool in matlab, gpc polybool in octave

  if exist('polyboolmex', 'file')
    % use general polygon clipper polybool (matlab and octave)
    if strcmp(operand, '+')
      action = 'or';
    elseif strcmp(operand, '-') 
      action = 'notb';
    else
      error ([ 'unknown operand ' operand]);
    end

    [q, hf] = polybool(p1, p2, action);
%XXX    if (sum(hf) ~= 0)
%XXX    disp('here');
%XXX    hf
%XXX    iscw(q)
%XXX    end

  %elseif exist('polybool', 'file')
  % XXX: 2do: implement using mapping toolbox polybool (matlab only)
  else 
    error('install gpc polybool');
  end

end 
% not truncated

