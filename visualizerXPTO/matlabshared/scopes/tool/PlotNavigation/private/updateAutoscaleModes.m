% Local functions called from getPropertySet()
%
function updateAutoscaleModes(this)
prefix = 'Base/Menus/Tools/ZoomAndAutoscale/Autoscale/';
% When 'Auto' is turn on, enable the listener so that we autoscale
% at each time step.  Perform a quick autoscale regardless of the
% over/under.
% Fix the menu and button.
hMgr = this.Application.getGUI;
hAutoMenu = hMgr.findwidget( [ prefix, 'AutoscaleModes/EnableAutoscale' ] );
hUpdatesMenu = hMgr.findwidget( [ prefix, 'AutoscaleModes/EnableAfterUpdates' ] );
hStopMenu = this.Application.getGUI.findwidget( [ prefix, 'Actions/EnableOnceAtStop' ] );
set( hAutoMenu, 'Checked', 'off' );
set( hUpdatesMenu, 'Checked', 'off' );
visualListenerEnable = false;
limitsListenerEnable = false;
this.AutoscaleAtUpdate = false;
switch getPropertyValue( this, 'AutoscaleMode' )
    case 'Once at stop'
        setPropertyValue( this, 'OnceAtStop', true, 'AutoscaleMode', 'Manual', false );
    case 'Auto'
        set( hAutoMenu, 'Checked', 'on' );
        performAutoscale( this, false, true );
        visualListenerEnable = true;
        % When we are in "auto" mode, we also want to force an
        % update at the end.  It can look out of sync if we don't.
        limitsListenerEnable = true;
    case 'Updates'
        set( hUpdatesMenu, 'Checked', 'on' );
        visualListenerEnable = true;
        limitsListenerEnable = true;
        this.AutoscaleAtUpdate = true;
        this.UpdateCount = 0;
end
onceAtStop = getPropertyValue( this, 'OnceAtStop' );
if onceAtStop
    limitsListenerEnable = true;
    set( hStopMenu, 'Checked', 'on' );
else
    set( hStopMenu, 'Checked', 'off' );
end
uiservices.setListenerEnable( this.VisualUpdatedListener, visualListenerEnable );
uiservices.setListenerEnable( this.AxesLimitListeners, limitsListenerEnable );
uiservices.setListenerEnable( this.SourceStoppedListener, onceAtStop );
hStopMenu.Checked = uiservices.logicalToOnOff( onceAtStop );

end
