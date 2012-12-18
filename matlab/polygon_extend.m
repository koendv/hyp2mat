%pkg load geometry
%p = [ 0 0;  1 0  ; 1 1; 0 1 ; 0 0 ];
% polygon_extend(p, 0.5);

function q = polygon_extend(p, width);

  % trivial case
  if (width == 0)
    q = p;
    return;
  end

  if (isempty(p))
    q = p;
    return;
  end

  q = p;
  return;

  % experimental code below...

  % use bufferm2 in matlab, expandPolygon in octave
  if exist('OCTAVE_VERSION', 'builtin')
    % octave
    if ~exist('expandPolygon')
      pkg load geometry;
    end
    save('-ascii','mf', 'p');
 
    q = expandPolygon(p, width); % XXX not 100% correct: behavior on corners is different.

  else
    % matlab
    px = p(:,1).';
    py = p(:,2).';
    [px, py] = poly2cw(px, py);

    [qx,  qy] = bufferm2('xy', px, py, width, 'out', 4, 'vector'); 
    q = [qx, qy];
    
  end
 
end 
% not truncated

