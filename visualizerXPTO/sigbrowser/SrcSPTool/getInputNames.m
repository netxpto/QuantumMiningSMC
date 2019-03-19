function inputNames = getInputNames(this, inputIndex)
%GETINPUTNAMES Get the inputNames.

%   Copyright 2012 The MathWorks, Inc.

if nargin > 1
  inputNames = this.Signals(inputIndex).Name;
else
  inputNames = {this.Signals.Name};
end

% [EOF]
