function [ ] = setGlobalPreviousPath ( val )
%SETGLOBALPREVIOUSPATH Sets global variable "previousPath".
%   SETGLOBALPREVIOUSPATH(val) just sets and save the value of "previousPath" global variable to "val".

global previousPath;
if isempty(val)
    load previousPath.mat;
end    
previousPath = val;
save previousPath.mat;