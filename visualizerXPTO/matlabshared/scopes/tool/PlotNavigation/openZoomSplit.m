function openZoomSplit(this, ~, ~)
%OPENZOOMSPLIT Open the zoom splittool

%   Copyright 2014 The MathWorks, Inc.

b = this.ZoomButtons;

if isempty(b)
    
    zoomInTooltip = getString(message('Spcuilib:scopes:ZoomInTooltip'));
    zoomXTooltip  = getString(message('Spcuilib:scopes:ZoomXTooltip'));
    zoomYTooltip  = getString(message('Spcuilib:scopes:ZoomYTooltip'));
    panTooltip    = getString(message('Spcuilib:scopes:PanTooltip'));
    hGUI = this.Application.getGUI;
    h = hGUI.findwidget('Base/Toolbars/Main/Tools/ZoomAndAutoscale/Zoom');
    b.ZoomIn = uitoggletool('Parent', h, ...
        'CData', uimgr.getappdata(hGUI, 'toggle_zoom_in'), ...
        'TooltipString', zoomInTooltip, ...
        'ClickedCallback', @this.toggleZoomIn, ...
        'Tag', 'Exploration.ZoomIn');
    b.ZoomX = uitoggletool('Parent', h, ...
        'CData', uimgr.getappdata(hGUI, 'toggle_zoom_x'), ...
    	'TooltipString', zoomXTooltip, ...
        'ClickedCallback', @this.toggleZoomX, ...
        'Tag', 'Exploration.ZoomX');
    b.ZoomY = uitoggletool('Parent', h, ...
        'CData', uimgr.getappdata(hGUI, 'toggle_zoom_y'), ...
        'TooltipString', zoomYTooltip, ...
        'ClickedCallback', @this.toggleZoomY, ...
        'Tag', 'Exploration.ZoomY');

    if getPropertyValue(this, 'ShowPanningAction')
        b.Pan = uitoggletool('Parent', h, ...
            'CData', uimgr.getappdata(hGUI, 'toggle_pan'), ...
            'TooltipString', panTooltip, ...
            'ClickedCallback', @this.togglePan, ...
            'Tag', 'Exploration.Pan');
    else
        b.Pan = [];
    end
    this.ZoomButtons = b;
end

c = struct2cell(b);
c = [c{:}];

set(c, 'Visible', 'on', 'State', 'off');

previousAutoscale = getPropertyValue(this, 'PreviousZoomMode');
set(b.(previousAutoscale), 'Visible', 'off');

set(b.Pan, 'Enable', uiservices.logicalToOnOff(this.PanEnabled));

% [EOF]
