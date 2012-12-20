function [pc, hf] = poly_buffer(pa, op, ra, ud);
%function [pc, hf] = poly_buffer(pa, op, ra, ud);
%
% poly_buffer : a function to perform buffering on planar polygons
% 
% pa : EITHER a nx2 matrix of vertices describing a polygon 
%      OR a cell array with polygons, each of which is a nx2 matrix
%      of vertices (one vertex per row)
%
% op : type of buffering operation:
%       'miter'  : difference - points in polygon pa and not in polygon pb
%       'square' : intersection - points in polygon pa and in polygon pb
%       'round'  : exclusive or - points either in polygon pa or in polygon pb
%
% ra : buffering radius.
%
% ud : conversion factor for conversion from user
%      coordinates to database coordinates
%        
% pc : cell array with the result(s) of the boolean set operation
%      of polygons pa and pb (can be more than one polygon)
% hf : hole flags for each of the output polygons. If > 0, the
%      polygon is an interior boundary of a polygon with a hole.

% This function is based on the Clipper library by Angus Johnson (www.angusj.com).
%
% The interface function 'poly_boolmex' must be compiled before
% using this function.
%
% Matlab: 
% at the Matlab prompt type the command
%    makemex-matlab
%
% Octave:
%    cd src; make
%

% check arguments
if nargin < 4
   error('poly_buffer :  expecting 4 arguments.');
end

if (~strcmp(op, 'miter') && ~strcmp(op, 'square') && ~strcmp(op, 'round'))
   error('poly_buffer :  operation should be one of miter, square, round');
end

% prepare arguments
pa = poly_cell(pa);

% call polygon clipper
[pc, hf] = poly_boolmex(pa, pa, op, ra, ud);

return
