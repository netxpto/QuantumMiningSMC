function flag = justzoom(fig)
%JUSTZOOM Determines whether the current buttondownfcn follows a 
%   zoom windowbuttondownfcn for the client (eg, the signal browser,
%   the filter viewer or the spectrum viewer) in figure fig.
%   Meant to be called by any buttondownfcn (eg, pickfcn, ruldown, 
%   pandown) that might be called AFTER the windowbuttondownfcn.
%   Writes userdata and clears flag if so.
 
%   Copyright 1988-2010 The MathWorks, Inc.

if nargin < 1
    fig = gcf;
end

ud = get(fig,'UserData');

if isequal(ud.justzoom,get(fig,'CurrentPoint'))
    flag = 1;
    ud.justzoom = [0 0];
    set(fig,'UserData',ud)
else
    flag = 0;
end

