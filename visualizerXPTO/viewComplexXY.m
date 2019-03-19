function [ ] = viewComplexXY( varargin )
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
    data = signals(activeSignals(k)).data;
    data_x = data(1:4:end) + 1i.*data(2:4:end);
    data_y = data(3:4:end) + 1i.*data(4:4:end);
    data_x = real(data_x)' + imag(data_x)'.*1i;
    data_y = real(data_y)' + imag(data_y)'.*1i;
    
    obj1 = findobj('Tag', sprintf('DisplayLine%u', k));
    
    % Real part X mode
    obj1.XData = obj1.XData(1:length(data_x));
    obj1.YData = real(data_x);

    subplot(4, 1, 1); hold(gca, 'on');
    plot(get(obj1, 'XData'), get(obj1, 'YData'), 'Color', get(obj1, 'Color'),'Linewidth', lw, 'Tag', 'pr1');
     
    % Imaginary part Y mode
    obj1.YData = imag(data_x);
    
    subplot(4, 1, 2); hold(gca, 'on');
    plot(get(obj1, 'XData'), get(obj1, 'YData'), 'Color', get(obj1, 'Color'),'Linewidth', lw, 'Tag', 'pr1');
   
    % Real part Y mode
    obj1.YData = real(data_y);
      
    subplot(4, 1, 3); hold(gca, 'on');
    plot(get(obj1, 'XData'), get(obj1, 'YData'), 'Color', get(obj1, 'Color'),'Linewidth', lw, 'Tag', 'pr1');

    % Imag part Y mode
    obj1.YData = imag(data_y);
      
    subplot(4, 1, 4); hold(gca, 'on');
    plot(get(obj1, 'XData'), get(obj1, 'YData'), 'Color', get(obj1, 'Color'),'Linewidth', lw, 'Tag', 'pr1');

    
    
end 

%% Axes
axesReal = subplot(4, 1, 1); set(axesReal, 'Tag', 'axesReal');
set(axesReal, 'XLim', get(currentAxes, 'XLim'), 'box', 'on');
set(axesReal, 'YLim', get(currentAxes, 'YLim'), 'YTick', get(currentAxes,'YTick'), 'YTickLabel', get(currentAxes, 'YTickLabel'));
xl = get(axesReal, 'XLabel'); set(xl, 'String', 'Time (s)');
yl = get(axesReal, 'YLabel'); set(yl, 'String', get(yli,'String'));
t = title('Real part - X mode'); set(t, 'Fontweight', 'bold', 'fontsize', 12);
grid(axesReal, 'on');
axesImag = subplot(4, 1, 2); set(axesImag, 'Tag', 'axesImag');
set(axesImag, 'XLim', get(currentAxes, 'XLim'), 'box', 'on');
set(axesImag, 'YLim', get(currentAxes, 'YLim'), 'YTick', get(currentAxes, 'YTick'), 'YTickLabel', get(currentAxes, 'YTickLabel'));
xl = get(axesImag, 'XLabel'); set(xl, 'String', 'Time (s)');
yl = get(axesImag, 'YLabel'); set(yl, 'String', get(yli,'String'));
t = title('Imaginary part - X mode'); set(t, 'Fontweight', 'bold', 'fontsize', 12);
grid(axesImag, 'on');
axesReal = subplot(4, 1, 3); set(axesReal, 'Tag', 'axesReal');
set(axesReal, 'XLim', get(currentAxes, 'XLim'), 'box', 'on');
set(axesReal, 'YLim', get(currentAxes, 'YLim'), 'YTick', get(currentAxes,'YTick'), 'YTickLabel', get(currentAxes, 'YTickLabel'));
xl = get(axesReal, 'XLabel'); set(xl, 'String', 'Time (s)');
yl = get(axesReal, 'YLabel'); set(yl, 'String', get(yli,'String'));
t = title('Real part - Y mode'); set(t, 'Fontweight', 'bold', 'fontsize', 12);
grid(axesReal, 'on');
axesImag = subplot(4, 1, 4); set(axesImag, 'Tag', 'axesImag');
set(axesImag, 'XLim', get(currentAxes, 'XLim'), 'box', 'on');
set(axesImag, 'YLim', get(currentAxes, 'YLim'), 'YTick', get(currentAxes, 'YTick'), 'YTickLabel', get(currentAxes, 'YTickLabel'));
xl = get(axesImag, 'XLabel'); set(xl, 'String', 'Time (s)');
yl = get(axesImag, 'YLabel'); set(yl, 'String', get(yli,'String'));
t = title('Imaginary part - Y mode'); set(t, 'Fontweight', 'bold', 'fontsize', 12);
grid(axesImag, 'on');
%% Legends
% subplot(4, 1, 1);
% str = cell(1, N);
% for k = 1:N
%     arg = {'real (' , signals(activeSignals(k)).label, ')'};
%     str(k) = {strjoin(arg, '')};
% end
% str = str';
% legend(str);
% subplot(4, 1, 2);
% str2 = cell(1, N);
% for k = 1:N
%     arg = {'imag (' , signals(activeSignals(k)).label, ')'};
%     str2(k) = {strjoin(arg, '')};
% end
% str2 = str2';
% legend(str2);

%% Omit native "Signal Browser"
set(signalBrowser, 'Visible', 'off');

%% Set global variable "choice"
setGlobalchoice(0);
%%