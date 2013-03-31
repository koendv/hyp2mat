% demo for hyp2mat - simulation of a notch filter.
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
unit = 1; % this pcb has units in meter. 
f_max = 7e9;
substrate_epr = 4.8;
resolution = c0 / f_max / sqrt(substrate_epr) / unit / 25;
hypfilename = 'notchfilter.HYP';

AirBox = c0/f_max/unit/4;

% set up FTDT
FDTD  = InitFDTD();
FDTD = SetGaussExcite( FDTD, f_max/2, f_max/2 );
BC   = {'PML_8' 'PML_8' 'PML_8' 'PML_8' 'PML_8' 'PML_8'};
FDTD = SetBoundaryCond( FDTD, BC );

% create 3d model
CSX = InitCSX();
disp([ 'loading ' hypfilename ]);
CSX = ImportHyperLynx(CSX, hypfilename);

mesh = DetectEdges(CSX);

% get the coordinates of the ports.
% 'TP1.TP' is pin 'TP' of test point 'TP1'. (See schematic)
[port1_material, port1_start, port1_stop] = GetHyperLynxPort(CSX, 'TP1.TP');
[port2_material, port2_start, port2_stop] = GetHyperLynxPort(CSX, 'TP3.TP');

port1_stop(3)=mesh.z(1); % port goes all the way to the ground plane
[CSX, port{1}] = AddLumpedPort( CSX, 999, 1, 50 , port1_start, port1_stop, [0 0 -1], true);

port2_stop(3)=mesh.z(1); % port goes all the way to the ground plane
[CSX, port{2}] = AddLumpedPort( CSX, 999, 2, 50, port2_start, port2_stop, [0 0 -1]);

% re-run DetectEdges to add include the lumped ports in the mesh.
mesh = DetectEdges(CSX);

% add air-box around the imported structure
mesh.x = [min(mesh.x)-AirBox max(mesh.x)+AirBox mesh.x];
mesh.y = [min(mesh.y)-AirBox max(mesh.y)+AirBox mesh.y];
mesh.z = [min(mesh.z)-AirBox max(mesh.z)+2*AirBox mesh.z];

mesh = SmoothMesh(mesh, resolution);

% add 8 additional cells for the pml
mesh = AddPML(mesh, 8);

CSX = DefineRectGrid(CSX, unit, mesh);

% write/show/run the openEMS compatible xml-file
  
Sim_Path = 'tmp';
Sim_CSX = 'msl.xml';
  
[status, message, messageid] = rmdir( Sim_Path, 's' ); % clear previous directory
[status, message, messageid] = mkdir( Sim_Path ); % create empty simulation folder

disp([ 'Estimated simulation runtime: 2500 timesteps' ]); % inform user this may take a while...
 
WriteOpenEMS( [Sim_Path '/' Sim_CSX], FDTD, CSX );
CSXGeomPlot( [Sim_Path '/' Sim_CSX] );
RunOpenEMS( Sim_Path, Sim_CSX );

%% post-processing
close all
f = linspace( 1e6, f_max, 1601 );
port = calcPort( port, Sim_Path, f, 'RefImpedance', 50);

s11 = port{1}.uf.ref./ port{1}.uf.inc;
s21 = port{2}.uf.ref./ port{1}.uf.inc;

plot(f/1e9,20*log10(abs(s11)),'k-','LineWidth',2);
hold on;
grid on;
plot(f/1e9,20*log10(abs(s21)),'r--','LineWidth',2);
legend('S_{11}','S_{21}');
ylabel('S-Parameter (dB)','FontSize',12);
xlabel('frequency (GHz) \rightarrow','FontSize',12);
ylim([-40 2]);
print ('notchfilter_simulation.png', '-dpng');

% not truncated
