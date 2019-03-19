function [ ] = setGlobalt_real( val )
%SETGLOBALT_REAL Sets global variable "t_real".
%   SETGLOBALT_REAL(val) just sets the value of "t_real" global variable to "val".

global t_real;
t_real = val;
save t_real.mat;
