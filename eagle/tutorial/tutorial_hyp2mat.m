% tutorial for hyp2mat - capacitor in a microstrip.
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
f_max = 10e9;

% Importing printed circuit board
CSX = InitCSX();
CSX = ImportHyperLynx(CSX, 'msl.HYP');

% Adding a component
[pad1_material, pad1_start, pad1_stop] = GetHyperLynxPort(CSX, 'C1.1');
[pad2_material, pad2_start, pad2_stop] = GetHyperLynxPort(CSX, 'C1.2');

c1_height = pad1_stop - pad1_start;
c1_start = [(pad1_start(1)+pad1_stop(1))/2, pad1_start(2), pad1_start(3)];
c1_stop  = [(pad2_start(1)+pad2_stop(1))/2, pad2_stop(2), pad2_stop(3)+c1_height];

CSX = AddLumpedElement( CSX, 'Capacitor', 0, 'Caps', 1, 'C', 100e-9);
CSX = AddBox( CSX, 'Capacitor', 0, c1_start, c1_stop );

% Adding excitation and load
[port1_material, port1_start, port1_stop] = GetHyperLynxPort(CSX, 'TP1.TP');
[gnd1_material, gnd1_start, gnd1_stop]    = GetHyperLynxPort(CSX, 'TP3.TP');
[port2_material, port2_start, port2_stop] = GetHyperLynxPort(CSX, 'TP2.TP');
[gnd2_material, gnd2_start, gnd2_stop]    = GetHyperLynxPort(CSX, 'TP4.TP');

[CSX, port{1}] = AddLumpedPort( CSX, 999, 1, 50, gnd1_start, port1_stop, [0 0 -1], true);
[CSX, port{2}] = AddLumpedPort( CSX, 999, 2, 50, gnd2_start, port2_stop, [0 0 -1]);

% Setting up a mesh
unit = GetUnits(CSX);
substrate_epr = GetEpsilon(CSX);
resolution = c0 / f_max / sqrt(substrate_epr) / unit / 25;
AirBox = c0 / f_max / unit / 25;

z_top = port1_start(3);
z_bottom = gnd1_start(3);
z_middle = (z_top+z_bottom)/2;

mesh.x = [];
mesh.y = [];
mesh.z = [ z_middle ];

mesh = DetectEdges(CSX, mesh);

mesh.x = [min(mesh.x)-AirBox max(mesh.x)+AirBox mesh.x];
mesh.y = [min(mesh.y)-AirBox max(mesh.y)+AirBox mesh.y];
mesh.z = [min(mesh.z)-AirBox max(mesh.z)+2*AirBox mesh.z];

%mesh = SmoothMesh(mesh, resolution);
mesh = SmoothMesh(mesh, resolution, 'algorithm', [ 1 ]);

% Setting boundary conditions

FDTD = InitFDTD();
FDTD = SetGaussExcite(FDTD, f_max/2, f_max/2);

BC   = {'PML_8' 'PML_8' 'PML_8' 'PML_8' 'PML_8' 'PML_8'};
FDTD = SetBoundaryCond(FDTD, BC );

mesh = AddPML(mesh, 8);

CSX = DefineRectGrid(CSX, unit, mesh);

% Simulation

Sim_Path = 'tmp';
Sim_CSX = 'msl.xml';

[status, message, messageid] = rmdir(Sim_Path, 's'); % clear previous directory
[status, message, messageid] = mkdir(Sim_Path ); % create empty simulation folder

disp([ 'Estimated simulation runtime: 6500 timesteps' ]); % inform user this may take a while... 

WriteOpenEMS([Sim_Path '/' Sim_CSX], FDTD, CSX);
CSXGeomPlot([Sim_Path '/' Sim_CSX]);
RunOpenEMS(Sim_Path, Sim_CSX);

% Calculating the s-parameters

close all
f = linspace( 1e6, f_max, 1601 );
port = calcPort( port, Sim_Path, f, 'RefImpedance', 50);

s11 = port{1}.uf.ref./ port{1}.uf.inc;
s21 = port{2}.uf.ref./ port{1}.uf.inc;

semilogx(f/1e9,20*log10(abs(s11)),'k-','LineWidth',2);
hold on;
grid on;
semilogx(f/1e9,20*log10(abs(s21)),'r--','LineWidth',2);
legend('S_{11}','S_{21}');
ylabel('S-Parameter (dB)','FontSize',12);
xlabel('frequency (GHz) \rightarrow','FontSize',12);
ylim([-80 10]);
print('sparam.png', '-dpng');

% not truncated
