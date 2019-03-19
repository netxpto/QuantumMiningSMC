function [ ] = closeComplexFigures( ~ )
%CLOSECOMPLEXFIGURES closes all complex representations from "visualizer".
%   CLOSECOMPLEXFIGURES just closes complex figures.

%% Close figures
close(findobj('Tag', 'figCompRI'));
close(findobj('Tag', 'figCompModPhase'));
close(findobj('Tag', 'figCompMod2Phase'));
%%