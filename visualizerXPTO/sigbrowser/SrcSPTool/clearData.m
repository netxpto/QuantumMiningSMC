function clearData(this, inputIndex)
%CLEARDATA clears all data from the source
%
%   If inputIndex is specified, clear the corresponding signal
%   and re-install the data source for immediate viewing

%   Copyright 2012 The MathWorks, Inc.

if nargin == 1
  this.Signals = [];
else  
  this.Signals(inputIndex) = [];
  installDataSource(this.Application, this);
end
