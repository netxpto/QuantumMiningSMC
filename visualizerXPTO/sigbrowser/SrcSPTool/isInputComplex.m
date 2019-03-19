function b = isInputComplex(this, inputIndex)
%ISINPUTCOMPLEX True if the input is complex

%   Copyright 2012 The MathWorks, Inc.

if nargin == 1
  %return a column vector containing the complexity of each signal
  n = numel(this.Signals);
  b = false(n,1);
  for i=1:n
    if ~isreal(this.Signals(i).Data)
      b(i) = true;
    end
  end
else
  %return the complexity of the specified signal
  b = ~isreal(this.Signals(inputIndex).Data);
end

% [EOF]
