function hPropDb = getPropertySet(varargin)
%getPropertySet Get the PropertySet.
% Define the different autoscale modes.
if isempty( findtype( 'AxesScaling' ) )
    schema.EnumType( 'AxesScaling', matlabshared.scopes.tool.PlotNavigation.AxesScaling );
end
% Define the Y-axis anchors
if isempty( findtype( 'AutoscaleYAnchors' ) )
    schema.EnumType( 'AutoscaleYAnchors', matlabshared.scopes.tool.PlotNavigation.YAnchors );
end
% Define the X-axis anchors
if isempty( findtype( 'AutoscaleXAnchors' ) )
    schema.EnumType( 'AutoscaleXAnchors', matlabshared.scopes.tool.PlotNavigation.XAnchors );
end
hPropDb = extmgr.PropertySet( 'OnceAtStop', 'bool', true, 'YDataDisplay', 'mxArray', 80, 'XDataDisplay', 'mxArray', 100, 'CDataDisplay', 'mxArray', 100, 'AutoscaleMode', 'AxesScaling', 'Manual', 'ExpandOnly', 'bool', true, 'AutoscaleYAnchor', 'AutoscaleYAnchors', 'Center', 'AutoscaleXAnchor', 'AutoscaleXAnchors', 'Center', 'AutoscaleCAnchor', 'AutoscaleYAnchors', 'Center', 'AutoscaleSecondaryAxes', 'bool', false, 'ShowStreamingMenuItems', 'bool', true, 'ShowPanningAction', 'bool', true, 'ShowAutoscaleAction', 'bool', true, 'PreviousZoomMode', 'string', 'ZoomIn', 'PreviousAutoscale', 'string', 'Y', 'UpdatesBeforeAutoscale', 'string', '10', varargin{ : } );
end
