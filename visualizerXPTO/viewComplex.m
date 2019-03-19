function [ ] = viewComplex( varargin )
%VIEWCOMPLEX represents the real and imaginary parts of the current
%selected signals.
%   VIEWCOMPLEX just represents the real and imaginary parts.

%% Close complex figures
closeComplexFigures;

%% Open "Signal Browser"
signalBrowser = findobj('Tag', 'SignalBrowser');
flag = 0;
if ~isempty(varargin)
    if varargin{1} == 1
        flag = 1;
    end
end
if isempty(signalBrowser)
    defaultVerb(1, 1);
    changeTimeWindow;
    signalBrowser = findobj('Tag', 'SignalBrowser');
else
    xlim = get(findobj('Tag', 'DisplayAxes1_RealMag'), 'XLim');
    if xlim(1) < 0 || flag == 1
        defaultVerb(1, 1);
        changeTimeWindow;
    end
end

%% Plots
signals = sptool('Signals');
activeSignals = get(findobj('Tag', 'list1'), 'Value');
N = length(activeSignals);
currentAxes = findobj('Tag', 'DisplayAxes1_RealMag');
yli = get(currentAxes, 'YLabel');
f = figure;
set(f, 'Tag', 'figCompRI', 'numbertitle', 'off', 'Name', 'Signal Browser');
h = pan(findobj('Tag', 'figCompRI'));
set(h, 'ActionPostCallback', @updatePan);
lw = 1.5;
for k = 1:N    
    obj1 = findobj('Tag', sprintf('DisplayLine%u', k));
    obj2 = findobj('Tag', sprintf('DisplayLine%u_Imag',k));
    % Real part
    subplot(2, 1, 1); hold(gca, 'on');
    plot(get(obj1, 'XData'), get(obj1, 'YData'), 'Color', get(obj1, 'Color'), 'Linewidth', lw, 'Tag', 'pr1'); 
    % Imaginary part
    subplot(2, 1, 2); hold(gca, 'on');
    plot(get(obj2, 'XData'), get(obj2, 'YData'), 'Color', get(obj2, 'Color'), 'Linewidth', lw, 'Tag', 'pi1');  
end 

%% Axes
axesReal = subplot(2, 1, 1); set(axesReal, 'Tag', 'axesReal');
set(axesReal, 'XLim', get(currentAxes, 'XLim'), 'box', 'on');
set(axesReal, 'YLim', get(currentAxes, 'YLim'), 'YTick', get(currentAxes,'YTick'), 'YTickLabel', get(currentAxes, 'YTickLabel'));
xl = get(axesReal, 'XLabel'); set(xl, 'String', 'Time (s)');
yl = get(axesReal, 'YLabel'); set(yl, 'String', get(yli,'String'));
t = title('Real part'); set(t, 'Fontweight', 'bold', 'fontsize', 12);
grid(axesReal, 'on');
axesImag = subplot(2, 1, 2); set(axesImag, 'Tag', 'axesImag');
set(axesImag, 'XLim', get(currentAxes, 'XLim'), 'box', 'on');
set(axesImag, 'YLim', get(currentAxes, 'YLim'), 'YTick', get(currentAxes, 'YTick'), 'YTickLabel', get(currentAxes, 'YTickLabel'));
xl = get(axesImag, 'XLabel'); set(xl, 'String', 'Time (s)');
yl = get(axesImag, 'YLabel'); set(yl, 'String', get(yli,'String'));
t = title('Imaginary part'); set(t, 'Fontweight', 'bold', 'fontsize', 12);
grid(axesImag, 'on');

%% Legends
subplot(2, 1, 1);
str = cell(1, N);
for k = 1:N
    arg = {'real (' , signals(activeSignals(k)).label, ')'};
    str(k) = {strjoin(arg, '')};
end
str = str';
legend(str);
subplot(2, 1, 2);
str2 = cell(1, N);
for k = 1:N
    arg = {'imag (' , signals(activeSignals(k)).label, ')'};
    str2(k) = {strjoin(arg, '')};
end
str2 = str2';
legend(str2);

%% Omit native "Signal Browser"
set(signalBrowser, 'Visible', 'off');

%% Set global variable "choice"
setGlobalchoice(0);
%%