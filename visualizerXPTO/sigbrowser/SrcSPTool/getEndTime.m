function endTime = getEndTime(this)
%GETENDTIME Get the last sample instant of all signals present

%   Copyright 2012 The MathWorks, Inc.

endTime = 0;

for i=1:numel(this.Signals)
  endTime = max(size(this.Signal(i).Data,1) ./ this.Signal(i).Rate, endTime); 
end

if isequal(endTime,0)
  % safeguard against empty/null signals
  % TimeDomainVisual uses 10 sec. as a default 
  endTime = 10;
end
% [EOF]
