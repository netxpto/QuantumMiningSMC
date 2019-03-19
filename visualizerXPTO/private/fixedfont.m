function [fontname,fontsize] = fixedfont
%FIXEDFONT Returns name and size of a fixed width font for this system.
%   Example usage:
%     [fontname,fontsize] = fixedfont;

%   Copyright 1988-2010 The MathWorks, Inc.

fontname = get(0,'FixedWidthFontName');
fontsize = get(0,'DefaultUicontrolFontSize');
