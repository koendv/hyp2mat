% demo for hyp2mat
% run from openems matlab command prompt
% See hyp2mat(1) - convert hyperlynx files to matlab scripts.

% (C) 2011,2012 Thorsten Liebig <thorsten.liebig@gmx.de>
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

close all
clear
clc

disp('hyp2mat demo');
disp('AppCSXCAD will show warnings "No primitives found in property XXX". This is normal.');

hypfiles = { '../eagle/notchfilter/notchfilter.HYP' '../eagle/hairpinfilter/hairpinfilter.HYP' };
%hypfiles = { '../eagle/ArduinoNano/Arduino Nano2.HYP' };

for idx = 1:length(hypfiles)

  hypfilename = hypfiles{idx};

  % initialize
  clear CSX;
  CSX = InitCSX();

  % create 3d model
  disp([ 'loading ' hypfilename ]);
  CSX = ImportHyperLynx(CSX, hypfilename);

  % set up FTDT
  clear FDTD;
  FDTD  = InitFDTD();
  
  % Add the two ports to the filter.
  % 'TP1.TP' is pin 'TP' of test point 'TP1'.
  % (See schematic)
  %[CSX,portstruct{1}] = AddHyperLynxPort(CSX, 'TP1.TP', 'driver');
  %[CSX,portstruct{2}] = AddHyperLynxPort(CSX, 'TP2.TP', 'load');
  
  % write/show/run the openEMS compatible xml-file
  
  Sim_Path = 'tmp';
  Sim_CSX = 'msl.xml';
  
  [status, message, messageid] = rmdir( Sim_Path, 's' ); % clear previous directory
  [status, message, messageid] = mkdir( Sim_Path ); % create empty simulation folder
  
  
  WriteOpenEMS( [Sim_Path '/' Sim_CSX], FDTD, CSX );
  CSXGeomPlot( [Sim_Path '/' Sim_CSX] );

end
