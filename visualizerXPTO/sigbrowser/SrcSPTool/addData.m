function addData(this, name, sampleRate, data)
%ADDDATA add data to the existing source
%
% name       is a string containing the name of the signal
% sampleRate is the sample rate for the data
% data       a 2D matrix containing real/complex samples along the columns

%   Copyright 2012 The MathWorks, Inc.

if isempty(this.Signals)
  % if no signals are present, just assign to Signals
   this.Signals = struct( ...
  'Name',name, ...
  'Data',data, ...
  'Rate',sampleRate);
else
  % otherwise, add to the end of the existing list
  n = numel(this.Signals);
  this.Signals(n+1) = struct( ...
    'Name',name, ...
    'Data',data, ...
    'Rate',sampleRate);
end
