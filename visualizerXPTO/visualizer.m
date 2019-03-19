function [ ] = visualizer( ~ )
%VISUALIZER Initializes "visualizer", a graphical tool for optical communications.
%   VISUALIZER just opens "visualizer", opening and configuring "sptool" MATLAB
%   application.

%   Carlos Ferreira Ribeiro
%   Prof. Armando Nolasco Pinto
%   January, 2016
%   IT, Aveiro.

%% Clear command window

warning('off');
clc;

s1 = [ pwd '\set'];
s2 = [ pwd '\get'];
s3 = [ pwd '\fd'];
addpath(s1, s2, s3);

sptool;

%% List of "sptool" modifications
% Modify application name to "Netxpto Visualizer v1.0"
set(findobj('Tag', 'sptool'), 'Name', 'Netxpto Visualizer v1.0');
% Modify "File" menu option "Open Session..." to "Open directory..."
set(findobj('Tag', 'loadmenu'), 'label', '&Open directory...');
% Modify "File" menu option "Import..." to "Open file(s)..."
set(findobj('Tag', 'importmenu'), 'label', '&Open file(s)...');
% Modify "File" menu option "Export..." to "Reload"
set(findobj('Tag', 'exportmenu'), 'label', '&Reload');
% Modify "File" menu option "Save Session" to "Configurations"
set(findobj('Tag', 'savemenu'), 'label', '&Configurations');
% Omit "File" menu options "Save Session As..." and "Preferences..."
set(findobj('Tag', 'saveasmenu'), 'visible', 'off');
set(findobj('Tag', 'prefmenu'), 'visible', 'off');
% Omit "Edit" menu options "Duplicate", "Name..." and "Sampling Frequency..."
%set(findobj('Tag', 'dupmenu'), 'visible', 'off');
%set(findobj('Tag', 'newnamemenu'), 'visible', 'off');
%set(findobj('Tag', 'freqmenu'), 'visible', 'off');
% Omit "Windows" menu
%set(findobj('Tag', 'winmenu'),'Visible', 'off');
% Omit "Help" all options except "SPTool Help"
set(findobj('Tag', 'helpoverview'), 'Visible', 'on'); 
set(findobj('Tag', 'helptoolbox'), 'Visible','off'); 
set(findobj('Tag', 'helpmouse'), 'Visible', 'off'); 
set(findobj('Tag', 'helpdemos'), 'Visible', 'off'); 
set(findobj('Tag', 'helpabout'), 'Visible', 'off'); 
% Change name of "View" pushbutton from "Signals" component to "Time
% Domain"
viewSignals = findobj('Tag', 'sigbrowser.sigbrowseAdapter:view');
set(viewSignals, 'String', '<html> Time Domain </html>');
% Change name of "New" pushbutton from "Filters" component to "Frequency
% Response"
newFilters = findobj('Tag', 'filtdes:create');
set(newFilters, 'String', 'Frequency Response');
% Omit "Edit" and "Apply" pushbuttons from "Filters" component
set(findobj('Tag', 'filtdes:change'), 'visible', 'off');
set(findobj('Tag', 'applyfilt:apply'), 'visible', 'off')
% Omit "Create" and "Update" pushbuttons from "Spectra" component
set(findobj('Tag', 'spectview:create'), 'Visible', 'off');
set(findobj('Tag', 'spectview:update'), 'Visible', 'off');
%
 








