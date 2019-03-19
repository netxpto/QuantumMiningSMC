function data = getData(this, ~, ~, inputIndex)
%GETDATA  Get the data.
%   We return all data as we will only have one timestamp.

%   Copyright 2012 The MathWorks, Inc.

if nargin < 4
    n = numel(this.Signals);
    inputIndex = 1:n;
else
    n = 1;
end
data = repmat(struct('values',[],'time',0),1,n);
for i = 1:numel(inputIndex)
  data(i).values     = this.Signal(inputIndex(i)).Data.';
  data(i).time       = (0:(size(this.Signal(inputIndex(i)).Data,1)-1)) ./ this.Signal(inputIndex(i)).Rate;
end

% [EOF]
