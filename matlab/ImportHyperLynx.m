% CSX = ImportHyperLynx(CSX, filename, varargin)
% load Hyperlynx file 'filename' into CSX .
%
% Other optional arguments:
% epsilonr       Set dielectric epsilon r. Overrides value in Hyperlynx file.
% net            Import net. Repeat to import several nets. Default is importing all nets.
% layer          Import layer. Repeat to import several layers. Default is importing all layers.
% epsilonr       Set dielectric epsilon r.
% xmin           Crop pcb. Set lower bound of x coordinate.
% xmax           Crop pcb. Set higher bound of x coordinate.
% ymin           Crop pcb. Set lower bound of y coordinate.
% ymax           Crop pcb. Set higher bound of y coordinate.
% zmin           Crop pcb. Set lower bound of z coordinate.
% zmax           Crop pcb. Set higher bound of z coordinate.
% grid           Set output grid size.  (default=`10e-6' = 1 um)
% arc-precision  Set maximum difference between perfect arc and polygonal approximation.
%
% ImportHyperLynx needs read and write access to the current directory.
%
% Examples:
% CSX = ImportHyperLynx(CSX, 'board.hyp' )
% will import the complete Hyperlynx file 'board.hyp' into the struct CSX.
%
% CSX = ImportHyperLynx(CSX, 'board.hyp', 'net', 'CLK_P' )
% will import only the CLK_P net from the Hyperlynx file 'board.hyp' into the struct CSX.
%
% CSX = ImportHyperLynx(CSX, 'board.hyp', 'net', '?' )
% lists all available nets.
%
% CSX = ImportHyperLynx(CSX, 'board.hyp', 'net', 'GND', 'net', 'CLK_P', 'net', 'CLK_N' )
% will import only the GND, CLK_P and CLK_N nets.
%
% CSX = ImportHyperLynx(CSX, 'board.hyp', 'xmin', 0.02, 'xmax', 0.03, 'ymin', 0.04, 'ymax', 0.05 )
% Imports all nets, and crops the board to the region 2 cm < x < 3 cm, 4 cm < y < 5 cm.
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

function CSX = ImportHyperLynx(CSX, filename, varargin)

  % determine nets to import
  if (nargin < 2)
    error('Wrong number of arguments');
  end

  if (~isstruct(CSX))
    error ('expecting CSX struct as first argument');
  end

  if (~ischar(filename))
    error ('expecting filename string as second argument');
  end

  % build command line
  cmdargs = '';

  % parse optional arguments 
  if (nargin < 3)
    varargin = {};
  end

  vn = 1;
  while (vn <= numel(varargin)) 
    if (strcmpi(varargin{vn}, 'net'))
      vn = vn + 1;
      cmdargs = [ cmdargs ' --net ''' varargin{vn} '''' ];
    end
    if (strcmpi(varargin{vn}, 'layer'))
      vn = vn + 1;
      cmdargs = [ cmdargs ' --layer ''' varargin{vn} '''' ];
    end
    if (strcmpi(varargin{vn}, 'epsilonr'))
      vn = vn + 1;
      cmdargs = [ cmdargs ' --epsilonr ' num2str(varargin{vn}) ];
    end
    if (strcmpi(varargin{vn}, 'xmin'))
      vn = vn + 1;
      cmdargs = [ cmdargs ' --xmin ' num2str(varargin{vn}) ];
    end
    if (strcmpi(varargin{vn}, 'xmax'))
      vn = vn + 1;
      cmdargs = [ cmdargs ' --xmax ' num2str(varargin{vn}) ];
    end
    if (strcmpi(varargin{vn}, 'ymin'))
      vn = vn + 1;
      cmdargs = [ cmdargs ' --ymin ' num2str(varargin{vn}) ];
    end
    if (strcmpi(varargin{vn}, 'ymax'))
      vn = vn + 1;
      cmdargs = [ cmdargs ' --ymax ' num2str(varargin{vn}) ];
    end
    if (strcmpi(varargin{vn}, 'zmin'))
      vn = vn + 1;
      cmdargs = [ cmdargs ' --zmin ' num2str(varargin{vn}) ];
    end
    if (strcmpi(varargin{vn}, 'zmax'))
      vn = vn + 1;
      cmdargs = [ cmdargs ' --zmax ' num2str(varargin{vn}) ];
    end
    if (strcmpi(varargin{vn}, 'grid'))
      vn = vn + 1;
      cmdargs = [ cmdargs ' --grid ' num2str(varargin{vn}) ];
    end
    if (strcmpi(varargin{vn}, 'arc-precision'))
      vn = vn + 1;
      cmdargs = [ cmdargs ' --arc-precision ' num2str(varargin{vn}) ];
    end
    vn = vn + 1;
  end

  % conversion
  if isunix
    cmd = [ 'export LD_LIBRARY_PATH=; hyp2mat --verbose --output-format csxcad --output pcb.m ' cmdargs ' ''' filename '''' ];
  elseif ispc
    m_filename = mfilename('fullpath');
    dir = fileparts( m_filename );
    cmd = [ '"' dir '\..\hyp2mat.exe" --verbose --output-format csxcad --output pcb.m ' cmdargs ' "' filename '"' ];
  else
    error('hyp2mat:ImportHyperLynx','unknown/unsupported operating system...');
  end

  % convert .hyp to .m
  disp (['command: ' cmd ]); 
  if isOctave() 
    fflush(stdout);
  else
    drawnow('update');
  end

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

