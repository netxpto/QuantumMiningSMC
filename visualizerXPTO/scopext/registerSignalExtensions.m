function registerSignalExtensions(ext)
%REGISTERSIGNALEXTENSIONS Define the REGISTERSIGNALEXTENSIONS class.

%   Copyright 2013 The MathWorks, Inc.

r = ext.add(...
    'Sources', 'SPTool', 'sigbrowser.SPToolSource', ...
    getString(message('signal:sigbrowse:SrcSPTool')));
r.Visible = false;

uiscopes.registerMeasurement(ext, 'signalstats', 'matlabshared.scopes.measurements.SignalStatistics', ...
    'MISignalStatistics', 'TTSignalStatistics');
uiscopes.registerMeasurement(ext, 'bilevel', 'matlabshared.scopes.measurements.BilevelMeasurements', ...
    'MIBilevelMeasurements', 'TTBilevelMeasurements');
uiscopes.registerMeasurement(ext, 'peaks', 'matlabshared.scopes.measurements.PeakFinder', ...
    'MIPeakFinder', 'TTPeakFinder');

% [EOF]
