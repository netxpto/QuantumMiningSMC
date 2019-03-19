function minorSampleTimes = getMinorSampleTimes(this, ~)
%GETMINORSAMPLETIMES Get the minorSampleTimes.

%   Copyright 2012 The MathWorks, Inc.

minorSampleTimes = 0;
if nargin < 2
    minorSampleTimes = repmat(minorSampleTimes, getNumInputs(this), 1);
end

% [EOF]
