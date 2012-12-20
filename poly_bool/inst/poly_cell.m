function pout = poly_cell(pin);
%function pout = poly_cell(pin);
%
% Convert polygon from nx2 array of vertices to cell array. 
%
% pin :   nx2 array of polygon vertices or cell array of polygons.
% pout :  cell array of polygons.

  pout = pin;

  % split nx2 array of [NaN, Nan] separated polygons in cell array
  if ~iscell(pin)
     pin = [ NaN NaN ; pin ; NaN NaN ];
     idx = find(sum(isnan(pin), 2) > 0 );
     pout = {};
     for i = 1:length(idx)-1;
       n = idx(i)+1;
       m = idx(i+1)-1;
       if (m >= n)
         pout{i} = pin(n:m, :);
       end
     end
  end

return
% not truncated
