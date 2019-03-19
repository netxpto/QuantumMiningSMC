function openAutoscaleSplit(this, ~, ~)
%openAutoscaleSplit open the autoscale split

%   Copyright 2013-2014 The MathWorks, Inc.

b = this.AutoscaleButtons;

if isempty(b)
    
    hGUI = this.Application.getGUI;
    bSplit = hGUI.findwidget('Base/Toolbars/Main/Tools/ZoomAndAutoscale/Autoscale');
    
    b.X = uipushtool('Parent', bSplit, ...
        'Tag', 'uimgr.uipushtool_AutoscaleX', ...
        'CData', uimgr.getappdata(hGUI, 'autoscaleX'), ...
        'TooltipString', getString(message('Spcuilib:scopes:ScaleXAxisLimitsTooltip')), ...
        'ClickedCallback', uiservices.makeCallback(@autoscaleCallback, this, 'X'));
    b.Y = uipushtool('Parent', bSplit, ...
        'Tag', 'uimgr.uipushtool_AutoscaleY', ...
        'CData', uimgr.getappdata(hGUI, 'autoscaleY'), ...
        'TooltipString', getString(message('Spcuilib:scopes:ScaleYAxisLimitsTooltip')), ...
        'ClickedCallback', uiservices.makeCallback(@autoscaleCallback, this, 'Y'));
    b.XY = uipushtool('Parent', bSplit, ...
        'Tag', 'uimgr.uipushtool_AutoscaleXY', ...
        'CData', uimgr.getappdata(hGUI, 'autoscaleXY'), ...
        'TooltipString', getString(message('Spcuilib:scopes:ScaleXYAxesLimitsTooltip')), ...
        'ClickedCallback', uiservices.makeCallback(@autoscaleCallback, this, 'XY'));
    b.C = uipushtool('Parent', bSplit, ...
        'Tag', 'uimgr.uipushtool_AutoscaleC', ...
        'CData', uimgr.getappdata(hGUI, 'autoscaleC'), ...
        'TooltipString', getString(message('Spcuilib:scopes:ScaleCAxisLimitsTooltip')), ...
        'ClickedCallback', uiservices.makeCallback(@autoscaleCallback, this, 'C'));
    this.AutoscaleButtons = b;
end

% Get all the available autoscales
[p,s] = getPlotNavigationAxes(this.Application.Visual);
all = sort([p s]);
not = setdiff('XYC', all);
% Make them all visible
for a = all
    set(b.(a), 'Visible', 'On');
end
for a = not
    set(b.(a), 'Visible', 'Off');
end
% Make xy visible if both are there.
if ~isempty(strfind(all, 'X')) && ~isempty(strfind(all, 'Y'))
    set(b.XY, 'Visible', 'on');
end
% Make the previous autoscale visible off since its visible
% on the split itself.
set(b.(getPropertyValue(this, 'PreviousAutoscale')), 'Visible', 'off');

% [EOF]
