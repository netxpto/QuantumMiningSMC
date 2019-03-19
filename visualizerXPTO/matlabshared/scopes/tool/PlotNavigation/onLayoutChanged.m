function onLayoutChanged(this, ~, ~)
%ONLAYOUTCHANGED Respond to the layout finishing changing.

%   Copyright 2014 The MathWorks, Inc.

listeners = this.AxesLimitListeners;
for indx = 1:numel(listeners)
    listeners(indx).Enabled = true;
end

% [EOF]
