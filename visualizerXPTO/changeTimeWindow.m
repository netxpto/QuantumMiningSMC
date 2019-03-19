function [ ] = changeTimeWindow( ~ )
%CHANGETIMEWINDOW updates the "Signal Browser" time window
%from "visualizer".
%   CHANGETIMEWINDOW just updates the "Signal Browser" time window.

%% Get global variable "nSymbols"
nSymbolsr = getGlobalnSymbols;

%% Types
tb = 'Binary';

%% Change time window
signals = sptool('Signals');
activeSignals = get(findobj('Tag', 'list1'), 'Value');
currentAxes = findobj('Tag', 'DisplayAxes1_RealMag'); 
if strcmp(signals(activeSignals(1)).type, tb) 
    factor = 200;
    set(currentAxes, 'XLim', [0 ((nSymbolsr*factor) - 1)*(1/signals(activeSignals(1)).Fs)]); 
else
    samplesPerSymbol = double(signals(activeSignals(1)).SPTIdentifier.version);
    set(currentAxes, 'XLim', [0 ((nSymbolsr*samplesPerSymbol) - 1)*(1/signals(activeSignals(1)).Fs)]);
end
xl = get(currentAxes, 'XLabel');
if strcmp('Time (ns)', get(xl, 'String'))
    set(currentAxes, 'XTickLabel' ,get(currentAxes, 'XTick')*1e9);
end
if strcmp('Time (ps)', get(xl, 'String'))
    set(currentAxes, 'XTickLabel' ,get(currentAxes, 'XTick')*1e12);
end