%pkg load geometry
% p1 = [ 0 0;  1 0  ; 1 1; 0 1 ; 0 0 ];
% p2 = 2 * p1;
% polygon_arith(p1, '+', p2);

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

  % non-trivial cases: use polybool in matlab, oc_polybool in octave
  if exist('OCTAVE_VERSION', 'builtin')

    % octave
    if strcmp(operand, '+')
      action = 'AND';
    elseif strcmp(operand, '-') 
      action = 'AB';
    else
      error ([ 'unknown operand ' operand]);
    end

    [qx, qy, npol, nint, npert] = oc_polybool(p1, p2, action);
    q = [qx, qy];
  
  else
  
    % matlab
    p1x = p1(:,1).';
    p1y = p1(:,2).';
  
    p2x = p2(:,1).';
    p2y = p2(:,2).';
  
    if (~strcmp(operand, '+') && ~strcmp(operand, '-'))
      error ([ 'unknown operand ' operand]);
    end
  
    [qx, qy] = polybool (operand, p1x, p1y, p2x, p2y);
  
    q = [qx.', qy.'];
  end
 
end 
% not truncated

