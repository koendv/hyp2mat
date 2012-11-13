% demo for hyp2mat - simulation of a 1.440 GHz hairpin filter.
%
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

% initialize
physical_constants;
unit = 0.0254; % this pcb has units in inches 

fc = 1.440e9; % center frequency
f0 = 1.3e9;   % est. 20 dB corner frequency
substrate_epr = 4.3;
resolution = c0 / (f0+fc) / sqrt(substrate_epr) / unit / 25;
hypfilename = '../eagle/hairpinfilter/hairpinfilter.HYP';

% set up FTDT
clear FDTD;
FDTD  = InitFDTD();
FDTD = SetGaussExcite( FDTD, f0, fc );
BC   = {'PML_8' 'PML_8' 'MUR' 'MUR' 'PEC' 'MUR'};
FDTD = SetBoundaryCond( FDTD, BC );

% create 3d model
clear CSX;
CSX = InitCSX();
disp([ 'loading ' hypfilename ]);
% There's no need to import the ground plane; the z-axis 'PEC' boundary condition is our ground plane
CSX = ImportHyperLynx(CSX, hypfilename, { 'V1', 'V2', 'V3', 'V4', 'V5' });

% get the coordinates of the ports.
% 'TP1.TP' is pin 'TP' of test point 'TP1'. (See schematic)
[port1_material, port1_start, port1_stop] = GetHyperLynxPort(CSX, 'TP1.TP');
[port2_material, port2_start, port2_stop] = GetHyperLynxPort(CSX, 'TP5.TP');

% Determine size of the pcb.
mesh.x = [ ];
mesh.y = [ ];
mesh.z = [ ];
mesh = DetectEdges(CSX, mesh);
x_max = max(mesh.x);
x_min = min(mesh.x);
y_max = max(mesh.y);
y_min = min(mesh.y);
z_max = max(mesh.z);
z_min = min(mesh.z);

% initialize the mesh with the "air-box" dimensions and port coordinates 
SimBox.x = unique(sort([ x_min port1_start(1) port2_start(1) port1_stop(1) port2_stop(1) x_max ]));
SimBox.y = unique(sort([ y_min port1_start(2) port2_start(2) port1_stop(2) port2_stop(2) y_max ]));
SimBox.z = unique(sort([ z_min port1_start(3) port2_start(3) port1_stop(3) port2_stop(3) z_max linspace(z_min, z_max, 5) z_max+4*resolution ])); 

% find trace/pcb edges 
edges = DetectEdges(CSX, SimBox);

% generate a smooth mesh with max. cell size = resolution
smoothedmesh = SmoothMesh(edges, resolution);

% If you want to see how the mesh is refined from SimBox -> edges -> smoothedmesh
% uncomment the following three lines in turn.
%CSX = DefineRectGrid(CSX, unit, SimBox);
%CSX = DefineRectGrid(CSX, unit, edges);
CSX = DefineRectGrid(CSX, unit, smoothedmesh);

% Add the two ports to the filter.
CSX = AddMaterial(CSX, 'PEC');

port1_stop(3)=smoothedmesh.z(1); % port goes all the way to the ground plane
[CSX,portstruct{1}] = AddMSLPort( CSX, 999, 1, 'PEC', port1_start, port1_stop, 0, [0 0 -1], 'ExcitePort', 'excite');

port2_stop(3)=smoothedmesh.z(1); % port goes all the way to the ground plane
[CSX,portstruct{2}] = AddMSLPort( CSX, 999, 2, 'PEC', port2_start, port2_stop, 0, [0 0 -1]);

% write/show/run the openEMS compatible xml-file
  
Sim_Path = 'tmp';
Sim_CSX = 'msl.xml';
  
[status, message, messageid] = rmdir( Sim_Path, 's' ); % clear previous directory
[status, message, messageid] = mkdir( Sim_Path ); % create empty simulation folder
 
WriteOpenEMS( [Sim_Path '/' Sim_CSX], FDTD, CSX );
CSXGeomPlot( [Sim_Path '/' Sim_CSX] );
RunOpenEMS( Sim_Path, Sim_CSX );

