function [ out ] = getGlobalPreviousPath( ~ )
%GETGLOBALCHOICE2 Gets global variable "choice2".
%   choice2r = GETGLOBALCHOICE2 just returns the value of "choice2" global variable ("choice2r").

global previousPath;
if ~ischar(previousPath)
    previousPath = pwd;
end
out = previousPath;