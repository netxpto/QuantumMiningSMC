function [ ] = setGlobalt_binary( val )
%SETGLOBALT_BINARY Sets global variable "t_binary".
%   SETGLOBALT_BINARY(val) just sets the value of "t_binary" global variable to "val".

global t_binary;
t_binary = val;
save t_binary.mat;