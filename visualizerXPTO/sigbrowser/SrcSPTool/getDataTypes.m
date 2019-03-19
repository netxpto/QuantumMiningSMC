function dataTypes = getDataTypes(this, inputIndex)
%GETDATATYPES Get the dataTypes for the inputs.

%   Copyright 2012 The MathWorks, Inc.

if nargin < 2
  dataTypes = cell(numel(this.Signals),1);
  for i=1:numel(dataTypes)
    dataTypes{i} = class(this.Signals(i).Data);
  end
else
  dataTypes = class(this.Signals(inputIndex).Data);
end

% [EOF]
