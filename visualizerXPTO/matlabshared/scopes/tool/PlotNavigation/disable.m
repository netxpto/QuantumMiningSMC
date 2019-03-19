function disable(this)
%DISABLE  Disable the extension.
% Do not leave the zoom on when disabling the extension.

%   Copyright 2014 The MathWorks, Inc.

this.ToolsMenuOpeningListener = [];

m = this.Menus;
if ~isempty(m)
    % If we are showing the autoscale, the we have rendered the autoscale
    % via uimgr.  If not, we have rendered the zoomin via uimgr.
    if getPropertyValue(this, 'ShowAutoscaleAction')
        m = rmfield(m, {'PerformAutoscale'});
    else
        m = rmfield(m, {'ZoomIn'});
    end
    f = fieldnames(m);
    for indx = 1:numel(f)
        if ishghandle(m.(f{indx}))
            delete(m.(f{indx}));
        end
    end
end

this.ZoomMode = 'off';

% [EOF]
