function cw = poly_iscw(pin);
%function cw = poly_iscw(pin);
%
% iscw :  returns a vector with a flag for each
%         polgon. The flag is 1 if the polygon has 
%         clockwise orientation, 0 otherwise.
%
% pin :   Nx2 array of polygon vertices or cell array of polygons.
% cw   :  array with flags indicating the orientation of the
%         polygons.
%
% The interface function 'poly_iscwmex' must be compiled
% before using this function.

% Initial version, Ulf Griesmann, NIST, November 2012
% Koen De Vleeschauwer, December 2012

% convert to cell array
pin = poly_cell(pin);

% call mex function
cw = poly_iscwmex(pin);

return
%not truncated
