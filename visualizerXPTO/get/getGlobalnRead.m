function [ nReadr ] = getGlobalnRead( ~ )
%GETGLOBALNREAD Gets global variable "nRead".
%   nReadr = GETGLOBALNREAD just returns the value of "nRead" global variable ("nReadr").

global nRead;
nReadr = nRead;