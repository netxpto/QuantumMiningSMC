function b = isDataEmpty(this)
%ISDATAEMPTY True if the object has empty data.

%   Copyright 2012 The MathWorks, Inc.

b = isempty(this.Signals);

% [EOF]
