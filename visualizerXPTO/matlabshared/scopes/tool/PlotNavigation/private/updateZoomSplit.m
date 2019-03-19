function updateZoomSplit(this)
%updateZoomSplit update the zoom split attributes

%   Copyright 2013-2014 The MathWorks, Inc.

openZoomSplit(this);

previousZoom = getPropertyValue( this, 'PreviousZoomMode' );
switch lower( previousZoom )
    case 'zoomin'
        icon = 'toggle_zoom_in';
        tooltip = getString(message('Spcuilib:scopes:ZoomInTooltip'));
    case 'zoomx'
        icon = 'toggle_zoom_x';
        tooltip = getString(message('Spcuilib:scopes:ZoomXTooltip'));
    case 'zoomy'
        icon = 'toggle_zoom_y';
        tooltip = getString(message('Spcuilib:scopes:ZoomYTooltip'));
    case 'pan'
        icon = 'toggle_pan';
        tooltip = getString(message('Spcuilib:scopes:PanTooltip'));
end
if strcmpi( this.ZoomMode, 'off' )
    splitState = 'off';
else
    splitState = 'on';
end
bZoomMode = this.Application.getGUI.findchild( 'Base/Toolbars/Main/Tools/ZoomAndAutoscale/Zoom' );
bZoomMode.setWidgetProperty( 'State', splitState, 'TooltipString', tooltip );
bZoomMode.IconAppData = icon;
updateIcon( bZoomMode );

% [EOF]
