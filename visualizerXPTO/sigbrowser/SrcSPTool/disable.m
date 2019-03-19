function disable(this)
%DISABLE Called when extension is disabled, overloaded for SrcSPTool.

% Copyright 2012 The MathWorks, Inc.

if this.ActiveSource
    % Stop the controls from sending more information up to the
    % source/visual combo.
    clearDisplay(this);
    releaseData(this.Application);
end

% [EOF]
