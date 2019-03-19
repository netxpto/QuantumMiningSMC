function plugInGUI = createGUI(this)
%CREATEGUI Create the GUI components

%   Copyright 2013-2014 The MathWorks, Inc.

zoomInTooltip = getString(message('Spcuilib:scopes:ZoomInTooltip'));
zoomXTooltip  = getString(message('Spcuilib:scopes:ZoomXTooltip'));
zoomYTooltip  = getString(message('Spcuilib:scopes:ZoomYTooltip'));
panTooltip    = getString(message('Spcuilib:scopes:PanTooltip'));

if isToolbarCompact(this.Application.Specification, 'zoom')
    bZoom = uimgr.uitogglesplittool('Zoom');
    z = getPropertyValue(this, 'PreviousZoomMode');
    switch lower(z)
        case 'zoomin'
            icon = 'toggle_zoom_in';
            tooltip = zoomInTooltip;
        case 'zoomx'
            icon = 'toggle_zoom_x';
            tooltip = zoomXTooltip;
        case 'zoomy'
            icon = 'toggle_zoom_y';
            tooltip = zoomYTooltip;
        case 'pan'
            icon = 'toggle_pan';
            tooltip = panTooltip;
    end
    bZoom.IconAppData = icon;
    bZoom.setWidgetPropertyDefault('TooltipString', tooltip, ...
        'Callback', @this.openZoomSplit, ...
        'ClickedCallback', uiservices.makeCallback(@toggleZoomSplit, this));
    this.IsZoomCompact = true;
    buttons = {bZoom};
else
    bZoomIn = uimgr.uitoggletool('ZoomIn');
    bZoomIn.IconAppData = 'toggle_zoom_in';
    bZoomIn.setWidgetPropertyDefault('TooltipString', zoomInTooltip, ...
        'ClickedCallback', @this.toggleZoomIn, 'Tag', 'Exploration.ZoomIn');
    bZoomX = uimgr.uitoggletool('ZoomX');
    bZoomX.IconAppData = 'toggle_zoom_x';
    bZoomX.setWidgetPropertyDefault('TooltipString', zoomXTooltip, ...
        'ClickedCallback', @this.toggleZoomX, 'Tag', 'Exploration.ZoomX');
    bZoomY = uimgr.uitoggletool('ZoomY');
    bZoomY.IconAppData = 'toggle_zoom_y';
    bZoomY.setWidgetPropertyDefault('TooltipString', zoomYTooltip,...
        'ClickedCallback', @this.toggleZoomY, 'Tag', 'Exploration.ZoomY');
    buttons = {bZoomIn, bZoomX, bZoomY};
end

if ~this.IsZoomCompact
    if getPropertyValue(this, 'ShowPanningAction')
        bPan = uimgr.uitoggletool('Pan');
        bPan.IconAppData = 'toggle_pan';
        bPan.setWidgetPropertyDefault('TooltipString', panTooltip, ...
            'ClickedCallback', @this.togglePan, 'Tag', 'Exploration.Pan');
        buttons = [ buttons, {bPan} ];
    end
    bZoom = uimgr.uibuttongroup('Zoom', buttons{:});
end
bAll = {bZoom};
if getPropertyValue(this, 'ShowAutoscaleAction')
    menu = uimgr.uimenu('PerformAutoscale', uiscopes.message('ScaleAxesLimits'));
    menu.setWidgetPropertyDefault('Accelerator', 'A', ...
        'Callback', uiservices.makeCallback(@autoscaleCallback, this));
    
    [p,s] = getPlotNavigationAxes(this.Application.Visual);
    aa = sort([ p, s ]);
    autoscaleTooltip.X  = getString(message('Spcuilib:scopes:ScaleXAxisLimitsTooltip'));
    autoscaleTooltip.Y  = getString(message('Spcuilib:scopes:ScaleYAxisLimitsTooltip'));
    autoscaleTooltip.XY = getString(message('Spcuilib:scopes:ScaleXYAxesLimitsTooltip'));
    autoscaleTooltip.C  = getString(message('Spcuilib:scopes:ScaleCAxisLimitsTooltip'));
    if isToolbarCompact(this.Application.Specification, 'autoscale')
        bAutoscale = uimgr.uisplittool('Autoscale');
        previousAutoscale = getPropertyValue(this, 'PreviousAutoscale');
        bAutoscale.setWidgetPropertyDefault('TooltipString', autoscaleTooltip.(previousAutoscale), ...
            'Callback', @this.openAutoscaleSplit, ...
            'ClickedCallback', uiservices.makeCallback(@autoscaleSplitClickedCallback, this));
        bAutoscale.IconAppData = [ 'autoscale', previousAutoscale ];
        this.IsAutoscaleCompact = true;
    else
        bAutoscaleX = uimgr.uipushtool('AutoscaleX');
        bAutoscaleX.IconAppData = 'autoscaleX';
        bAutoscaleX.setWidgetPropertyDefault('TooltipString', autoscaleTooltip.X, ...
            'ClickedCallback', uiservices.makeCallback(@autoscaleCallback, this, 'X'));
        bAutoscaleX.Visible = uiservices.logicalToOnOff(~isempty(strfind(aa, 'X')));
        bAutoscaleY = uimgr.uipushtool('AutoscaleY');
        bAutoscaleY.IconAppData = 'autoscaleY';
        bAutoscaleY.setWidgetPropertyDefault('TooltipString', autoscaleTooltip.Y, ...
            'ClickedCallback', uiservices.makeCallback(@autoscaleCallback, this, 'Y'));
        bAutoscaleY.Visible = uiservices.logicalToOnOff(~isempty(strfind(aa, 'Y')));
        bAutoscaleXY = uimgr.uipushtool('AutoscaleXY');
        bAutoscaleXY.IconAppData = 'autoscaleXY';
        bAutoscaleXY.setWidgetPropertyDefault('TooltipString', autoscaleTooltip.XY, ...
            'ClickedCallback', uiservices.makeCallback(@autoscaleCallback, this, 'XY'));
        bAutoscaleXY.Visible = uiservices.logicalToOnOff(~isempty(strfind(aa, 'XY')));
        %                 bAutoscaleZ = uimgr.uipushtool('AutoscaleZ');
        %                 bAutoscaleZ.IconAppData = 'autoscaleZ';
        %                 bAutoscaleZ.setWidgetPropertyDefault(...
        %                     'TooltipString', uiscopes.message(['ScaleZAxisLimitsTooltip'), ...
        %                     'ClickedCallback', cb);
        bAutoscaleC = uimgr.uipushtool('AutoscaleC');
        bAutoscaleC.IconAppData = 'autoscaleC';
        bAutoscaleC.setWidgetPropertyDefault('TooltipString', autoscaleTooltip.C, ...
            'ClickedCallback', uiservices.makeCallback(@autoscaleCallback, this, 'C'));
        bAutoscaleC.Visible = uiservices.logicalToOnOff(~isempty(strfind(aa, 'C')));
        buttons = {bAutoscaleX, bAutoscaleY, bAutoscaleXY, bAutoscaleC};
        bAutoscale = uimgr.uibuttongroup('Autoscale', buttons{:});
        this.IsAutoscaleCompact = false;
    end
    bAll = [ bAll, {bAutoscale} ];
else
    menu = uimgr.uimenu('ZoomIn', uiscopes.message('ZoomIn'));
    menu.setWidgetPropertyDefault('Callback', @this.toggleZoomIn);
end
menu.Placement = -10;
% Add the zoom and autoscale items together into groups so that they will
% stay next to each other when rendered.
bZoomAuto = uimgr.uibuttongroup('ZoomAndAutoscale', 1, bAll{:});
plugInGUI = uimgr.Installer({bZoomAuto, 'Base/Toolbars/Main/Tools'; 
    menu, 'Base/Menus/Tools'});

% [EOF]
