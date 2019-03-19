function flag = isSerializable(~)
%ISSERIALIZABLE Is data source serializable.
%  Returns false if this data source should not be serialized
%  into a data store, whether it is a file or a recent source list.
%  Generally, sources that do not have a text-string that can be
%  used to refer to the actual data repository are non-serializable,
%  otherwise the actual data itself must be recorded into the
%  repository leading to storage and efficiency issues.

% Copyright 2012 The MathWorks, Inc.

flag = false;
% [EOF]
