function [ ] = initializeGlobalVariables( ~ )
%INITIALIZEGLOBALVARIABLES Initializes global variables from "visualizer".
%   INITIALIZEGLOBALVARIABLES just initializes all used global variables.

%% Set global variable "choice"
setGlobalchoice(0);

%% Set global variable "choice2"
setGlobalchoice2(0);

%% Set global variable "choice3"
setGlobalchoice3(0);

%% Set global variable "t_binary"
setGlobalt_binary('int');

%% Set global variable "t_real"
setGlobalt_real('double');

%% Set global variable "t_complex"
setGlobalt_complex('double');

%% Set global variable "nRead"
setGlobalnRead(256);

%% Set global variable "nSymbols"
setGlobalnSymbols(32);

%% Set global variable "previousPath"
setGlobalPreviousPath('');

%% Set global variable "start"
setGlobalstart(0);

%% Set global variable "phaseOption"
setGlobalphaseOption(0);



