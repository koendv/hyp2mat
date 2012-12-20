%
% script to make .mex files
%

disp('Compiling functions for polygon operations');

if exist('OCTAVE_VERSION')
  error('This script is for Matlab. For Octave use pkg install');
end

cd src
mex -O poly_boolmex.cpp clipper.cpp
mex -O poly_iscwmex.c
cd ..

disp('add src and inst to your matlab path');
% not truncated
