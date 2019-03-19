function propertyChanged(this,propName)
%PROPERTYCHANGED React to property value change events.
if ~ischar( propName )
    propName = propName.AffectedObject.Name;
end
switch propName
    case { 'YDataDisplay', 'XDataDisplay', 'ZDataDisplay', 'CDataDisplay' }
        this.Autoscaler.(propName) = getPropertyValue( this, propName )/100;
        % If we are in 'Auto' scale mode and the request range or anchor
        % changes, we want to force an autoscale.  This will cause the
        % property value to be immediately reflected in the display.
        if strcmpi( getPropertyValue( this, 'AutoscaleMode' ), 'Auto' )
            performAutoscale( this, false, true );
        end
    case { 'AutoscaleXAnchor', 'AutoscaleYAnchor', 'AutoscaleZAnchor', 'AutoscaleCAnchor' }
        propValue = lower( getPropertyValue( this, propName ) );
        propName = strrep( propName, 'Autoscale', '' );
        this.Autoscaler.(propName) = propValue;
        % If we are in 'Auto' scale mode and the request range or anchor
        % changes, we want to force an autoscale.  This will cause the
        % property value to be immediately reflected in the display.
        if strcmpi( getPropertyValue( this, 'AutoscaleMode' ), 'Auto' )
            performAutoscale( this, false, true );
        end
    case { 'OnceAtStop', 'AutoscaleMode' }
        updateAutoscaleModes( this );
    case { 'ExpandOnly', 'AutoscaleSecondaryAxes' }
        this.Autoscaler.(propName) = getPropertyValue( this, propName );
    case 'UpdatesBeforeAutoscale'
        this.UpdatesBeforeAutoscale = evalPropertyValue( this, 'UpdatesBeforeAutoscale' );
        prefix = 'Base/Menus/Tools/ZoomAndAutoscale/Autoscale/AutoscaleModes/';
        hUpdatesMenu = this.Application.getGUI.findwidget( [ prefix, 'EnableAfterUpdates' ] );
        hUpdatesMenu.Labels = { uiscopes.message( 'ScaleAxesLimitsUpdates', num2str( this.UpdatesBeforeAutoscale ) ) };
    case 'PreviousAutoscale'
        if this.IsAutoscaleCompact
            % Find the split.
            openAutoscaleSplit(this);
            b = this.AutoscaleButtons;
            bSplit = this.Application.getGUI.findchild('Base/Toolbars/Main/Tools/ZoomAndAutoscale/Autoscale');
            % Find the button for the previous autoscale.
            bPrev = b.(getPropertyValue(this, 'PreviousAutoscale'));
            % Set the tooltip and icon of the split to match the
            % previous autoscale action.
            bSplit.setWidgetProperty('TooltipString', bPrev.TooltipString);
            bSplit.IconAppData = ['autoscale' getPropertyValue(this, 'PreviousAutoscale')];
            updateIcon(bSplit);
        end
    case 'PreviousZoomMode'
        if this.IsZoomCompact
            updateZoomSplit( this );
        end
end
end
