function maxDimensions = getMaxDimensions(this, inputIndex)
%GETMAXDIMENSIONS Get the maxDimensions.

%   Copyright 2012 The MathWorks, Inc.

if nargin == 1
  maxDimensions = zeros(numel(this.Signals),2);
  for i=1:numel(this.Signals)
    maxDimensions(i,:) = [1 size(this.Signals(i).Data, 2)];
  end
else
  maxDimensions = [1 size(this.Signals(inputIndex).Data, 2)];
end
% [EOF]
