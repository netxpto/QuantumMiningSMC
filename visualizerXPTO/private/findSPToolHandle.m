function hFig = findSPToolHandle
%FINDSPTOOLHANDLE 
%   OUT = FINDSPTOOLHANDLE(ARGS) find handle of sptool figure

%   Copyright 2010 The MathWorks, Inc.

hFig = findobj('type', 'figure', 'tag', 'sptool');

% [EOF]
