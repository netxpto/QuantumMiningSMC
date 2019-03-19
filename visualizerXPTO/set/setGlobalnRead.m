function [ ] = setGlobalnRead( val )
%SETGLOBALNREAD Sets global variable "nRead".
%   SETGLOBALNREAD(val) just sets and save the value of "nRead" global variable to "val".

global nRead;
nRead = val;
save nRead.mat;
