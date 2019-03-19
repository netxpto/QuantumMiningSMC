function timeOfDisplayData = getTimeOfDisplayData(this)
%GETTIMEOFDISPLAYDATA Get the timeOfDisplayData.

%   Copyright 2012 The MathWorks, Inc.

timeOfDisplayData = 0;
sigs = this.Signals;
for indx = 1:numel(sigs)
    nSamples = size(sigs(indx).Data, 1);
    timeOfDisplayData = max(timeOfDisplayData, (nSamples-1)/sigs(indx).Rate);
end

% [EOF]
