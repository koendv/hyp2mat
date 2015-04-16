% CSX = ImportHyperLynx(CSX, filename, varargin)
% load Hyperlynx file 'filename' into CSX .
%
% Other optional arguments:
% epsilonr       float.  Set dielectric epsilon r. Overrides value in Hyperlynx file.
% net            string. Import net. Repeat to import several nets. Default is importing all nets.
% layer          string. Import layer. Repeat to import several layers. Default is importing all layers.
% epsilonr       float.  Set dielectric epsilon r.
% xmin           float.  Crop pcb. Set lower bound of x coordinate.
% xmax           float.  Crop pcb. Set higher bound of x coordinate.
% ymin           float.  Crop pcb. Set lower bound of y coordinate.
% ymax           float.  Crop pcb. Set higher bound of y coordinate.
% zmin           float.  Crop pcb. Set lower bound of z coordinate.
% zmax           float.  Crop pcb. Set higher bound of z coordinate.
% grid           float.  Set output grid size.  (default = 10e-6 = 1 um)
% arc-precision  float.  Set maximum difference between perfect arc and polygonal approximation.
% clearance      float.  Set trace-to-plane clearance. (default = 0.0002)
% flood          string. Flood layer with copper. Repeat to flood several layers. The value "plane_layers" floods all plane layers.
% pcb-outline    none.   Detailed board outline.
% lossy-copper   none.   Model copper as lossy. Default is modeling copper as a perfect conductor.
% metal-3d       none.   Model copper as a 3D object. Default is modeling copper as a 2D sheet.
% version        none.   Print hyp2mat version number.
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
    if (strcmpi(varargin{vn}, 'clearance'))
      vn = vn + 1;
      cmdargs = [ cmdargs ' --clearance ' num2str(varargin{vn}) ];
    end
    if (strcmpi(varargin{vn}, 'flood'))
      vn = vn + 1;
      cmdargs = [ cmdargs ' --flood ''' varargin{vn} '''' ];
    end
    if (strcmpi(varargin{vn}, 'epsilonr'))
      vn = vn + 1;
      cmdargs = [ cmdargs ' --epsilonr ' num2str(varargin{vn}) ];
    end
    if (strcmpi(varargin{vn}, 'bulk-resistivity'))
      vn = vn + 1;
      cmdargs = [ cmdargs ' --bulk-resistivity ' num2str(varargin{vn}) ];
    end
    if (strcmpi(varargin{vn}, 'loss-tangent'))
      vn = vn + 1;
      cmdargs = [ cmdargs ' --loss-tangent ' num2str(varargin{vn}) ];
    end
    if (strcmpi(varargin{vn}, 'pcb-outline'))
      cmdargs = [ cmdargs ' --pcb-outline ' ];
    end
    if (strcmpi(varargin{vn}, 'lossy-copper'))
      cmdargs = [ cmdargs ' --lossy-copper ' ];
    end
    if (strcmpi(varargin{vn}, 'metal-3d'))
      cmdargs = [ cmdargs ' --metal-3d ' ];
    end
    if (strcmpi(varargin{vn}, 'version'))
      cmdargs = [ cmdargs ' --version ' ];
    end
    vn = vn + 1;
  end

  % conversion
  m_filename = mfilename('fullpath');
  dir = fileparts( m_filename );
  if isunix
    hyp2mat_binary = searchBinary('hyp2mat', ...
        {[dir filesep '..' filesep 'src' filesep], ...  % try development path
         [dir filesep '..' filesep '..' filesep '..' filesep 'bin' filesep]}); % try default install path
    cmd = [ 'export LD_LIBRARY_PATH=; ' hyp2mat_binary ' --verbose --output-format csxcad --output pcb.m ' cmdargs ' ''' filename '''' ];
  elseif ispc
    hyp2mat_binary = searchBinary('hyp2mat.exe', {[dir filesep '..' filesep]});
    cmd = [ '"' hyp2mat_binary '" --verbose --output-format csxcad --output pcb.m ' cmdargs ' "' filename '"' ];
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

