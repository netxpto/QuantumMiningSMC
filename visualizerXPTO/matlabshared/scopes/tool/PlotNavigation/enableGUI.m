function enableGUI(this,enabState)
%ENABLEGUI Enable the GUI when there is data.
% If we do not have a visual, or if the visual we do have does have the
% axes field, or there are not valid objects to autoscale against, make
% sure that we are always disabled.
if isempty( this.Application.Visual ) || isempty( getAxesContainers( this.Application.Visual ) )
    enabState = 'off';
end
hUI = this.Application.getGUI;
set( hUI.findchild( 'Base/Menus/Tools/PerformAutoscale' ), 'Enable', enabState );
set( hUI.findchild( 'Base/Toolbars/Main/Tools/ZoomAndAutoscale/Autoscale' ), 'Enable', enabState );
onVisualChanged( this );
end
