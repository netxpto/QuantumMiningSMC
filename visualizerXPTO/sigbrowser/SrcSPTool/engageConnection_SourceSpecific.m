function engageConnection_SourceSpecific(this)
%engageConnection_SourceSpecific Called by Source::enable method when a source is enabled.
%   Overload for SrcSPTool.

% Copyright 2012 The MathWorks, Inc.
this.ErrorStatus = 'success';
this.ErrorMsg = '';