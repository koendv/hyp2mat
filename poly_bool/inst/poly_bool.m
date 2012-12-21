function [pc, hf] = poly_bool(pa, pb, op, ud);
%function [pc, hf] = poly_bool(pa, pb, op, ud);
%
% polybool : a function to perform boolean set operations on 
%            planar polygons
% 
% pa : EITHER a nx2 matrix of vertices describing a polygon 
%      OR a cell array with polygons, each of which is a nx2 matrix
%      of vertices (one vertex per row)
% pb : EITHER a nx2 matrix of vertices describing a polygon 
%      OR a cell array with polygons, each of which is a nx2 matrix
%      of vertices (one vertex per row)
% op : type of algebraic operation:
%       'notb' : difference - points in polygon pa and not in polygon pb
%       'and'  : intersection - points in polygon pa and in polygon pb
%       'xor'  : exclusive or - points either in polygon pa or in polygon pb
%       'or'   : union - points in polygons pa or pb
% ud : conversion factor for conversion from user
%      coordinates to database coordinates
%        
% pc : cell array with the result(s) of the boolean set operation
%      of polygons pa and pb (can be more than one polygon)
% hf : hole flags for each of the output polygons. If > 0, the
%      polygon is an interior boundary of a polygon with a hole.
%
% This function is based on the Clipper library by Angus Johnson (www.angusj.com).
% The Clipper library uses integer arithmetic. Polygons are scaled up by a scale factor 'ud'
% before clipping, and scaled down after processing.
% 
% The Clipper library uses polygon orientation - clockwise or counterclockwise - to distinguish between 
% positive polygons and negative 'holes'. It assumes the first, outer, polygon has a positive 
% orientation. Inner 'hole' polygons have an opposite orientation.
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

% Ulf Griesmann, NIST, November 2012
% Koen De Vleeschauwer, December 2012

% check arguments
if nargin < 4
   error('poly_bool :  expecting 4 arguments.');
end

if (~strcmp(op, 'notb') && ~strcmp(op, 'and') && ~strcmp(op, 'xor') && ~strcmp(op, 'or'))
   error('poly_bool :  operation should be one of notb, and, xor, or');
end

% prepare arguments
pa = poly_cell(pa);
pb = poly_cell(pb);

% call polygon clipper
[pc, hf] = poly_boolmex(pa, pb, op, 0, ud);

return
%not truncated
