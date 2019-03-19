function rawData = getRawData(this, ~)
%GETRAWDATA Get the rawData.

%   Copyright 2012 The MathWorks, Inc.

if nargin < 2
    rawData = {this.Signals.Data};
end

% [EOF]
