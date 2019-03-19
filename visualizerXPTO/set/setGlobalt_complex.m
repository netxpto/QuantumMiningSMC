function [ ] = setGlobalt_complex ( val )
%SETGLOBALT_COMPLEX Sets global variable "t_complex".
%   SETGLOBALT_COMPLEX(val) just sets the value of "t_complex" global variable to "val".

global t_complex;
t_complex = val;
save t_complex.mat;

