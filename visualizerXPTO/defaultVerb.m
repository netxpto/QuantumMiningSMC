function [ ] = defaultVerb( arg1, arg2 )
%DEFAULTVERB runs default 'verb' action from "visualizer".
%   DEFAULTVERB(arg1, arg2) just runs default sptool('verb') action.

%% 'Verb'
sptoolfig = findobj(0, 'Tag', 'sptool');
setptr(sptoolfig, 'watch')
drawnow
ud = get(sptoolfig, 'UserData');
i = arg1;
j = arg2;
feval(ud.components(i).verbs(j).owningClient, 'action', ...
ud.components(i).verbs(j).action);
if ishghandle(sptoolfig)
    setptr(sptoolfig, 'arrow')
end
if i == 1 && j == 1
    % For MATLAB R2013a   
    if  strcmp(version, '8.1.0.604 (R2013a)')
        h = pan(findobj('Tag', 'SignalBrowser'));
        set(h, 'ActionPostCallback', @updatePan);
    end
end