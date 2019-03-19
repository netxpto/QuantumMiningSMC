function sampleTimes = getSampleTimes(this, ~)
%GETSAMPLETIMES Get the sampleTimes.

%   Copyright 2012 The MathWorks, Inc.

if ~isempty(this.Signal)
  sampleTimes = 1./cell2mat({this.Signal.Rate});
else
  sampleTimes = [];
end

for i=1:numel(sampleTimes)
  sampleTimes(i) = sampleTimes(i) * size(this.Signal(i).Data, 1);
end

% [EOF]
