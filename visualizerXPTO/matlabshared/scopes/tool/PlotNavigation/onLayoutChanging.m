function onLayoutChanging(this, ~, ~)
%ONLAYOUTCHANGING Respond to the layout changing.

%   Copyright 2014 The MathWorks, Inc.

listeners = this.AxesLimitListeners;
for indx = 1:numel(listeners)
    listeners(indx).Enabled = false;
end

% [EOF]
