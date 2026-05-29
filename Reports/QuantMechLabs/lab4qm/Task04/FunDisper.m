function y = FunDisper(z)
% dispersion equation FunDisper(z) = 0 

global b;
global c;
global nLevel;

y = pi*nLevel - atan(z/sqrt(b^2 - z^2)) - atan(z/sqrt(c^2 - z^2)) - z;
