classdef PlotNavigation < matlabshared.scopes.tool.Tool
    %PlotNavigation PlotNavigation   Define the PlotNavigation class.
    %    PlotNavigation   Define the PlotNavigation class.
    
    %   Copyright 2010-2014 The MathWorks, Inc.
    
    properties
        ZoomMode = 'Off'
        PanEnabled = true
    end
    
    properties (Access=protected)
        Autoscaler
        VisualUpdatedListener
        FirstVisualUpdatedListener
        SourceStoppedListener
        SourceChangedListener
        VisualChangedListener
        AxesLimitListeners
        ToolsMenuOpeningListener
        LayoutDimensionsListeners
        AutoscaleSecondaryAxes
        ExpandOnly
        AutoscaleAtUpdate = false
        UpdateCount = 0
        UpdatesBeforeAutoscale = 0
        ZoomPanNeverEnabled = true
        IsZoomCompact = false
        IsAutoscaleCompact = false
        AxesLimitsCache = {}
        ZoomButtons
        AutoscaleButtons
        Menus
    end
    
    properties (Constant,Hidden)
        AxesScaling = { 'Manual', 'Auto', 'Updates', 'Once at stop' }
        YAnchors = { 'Top', 'Center', 'Bottom' }
        XAnchors = { 'Left', 'Center', 'Right' }
        ZoomModes = { 'Off', 'ZoomIn', 'ZoomX', 'ZoomY', 'Pan' }
    end
    
    methods
        function this = PlotNavigation(varargin)
            %PlotNavigation   Construct the PlotNavigation class.
%             mlock;
            this@matlabshared.scopes.tool.Tool( varargin{ : } );
            hAutoscaler = matlabshared.scopes.tool.Autoscaler;
            % Update all the properties to match those passed in the Cfg object.
            hAutoscaler.AutoscaleSecondaryAxes = getPropertyValue( this, 'AutoscaleSecondaryAxes' );
            hAutoscaler.ExpandOnly = getPropertyValue( this, 'ExpandOnly' );
            hAutoscaler.XAnchor = lower( getPropertyValue( this, 'AutoscaleXAnchor' ) );
            hAutoscaler.YAnchor = lower( getPropertyValue( this, 'AutoscaleYAnchor' ) );
            hAutoscaler.XDataDisplay = getPropertyValue( this, 'XDataDisplay' )/100;
            hAutoscaler.YDataDisplay = getPropertyValue( this, 'YDataDisplay' )/100;
            this.Autoscaler = hAutoscaler;
            % Add a listener to the VisualUpdated event so that we can perform the
            % autoscale when AlwaysAutoscale is true.  For performance, this listener
            % will be disabled whenever AlwaysAutoscale is false.
            hScope = this.Application;
            this.VisualUpdatedListener = addlistener(hScope, 'VisualUpdated', @this.visualUpdatedCallback);
            uiservices.setListenerEnable(this.VisualUpdatedListener, false);
            this.SourceStoppedListener = addlistener(hScope, 'SourceStop', @this.onSourceStop);
            uiservices.setListenerEnable(this.SourceStoppedListener, false);
            % Listener to reset the zoom state
            this.SourceChangedListener = addlistener(hScope, 'SourceRun', @this.onSourceChanged);
            this.FirstVisualUpdatedListener = addlistener(hScope, 'VisualUpdated', @this.onVisualUpdated);
            uiservices.setListenerEnable(this.FirstVisualUpdatedListener, false);
            this.VisualChangedListener = addlistener(hScope, 'VisualChanged', @this.onVisualChanged);
            this.LayoutDimensionsListeners = [...
                addlistener(hScope, 'LayoutDimensionsChanging', @this.onLayoutChanging); ...
                addlistener(hScope, 'LayoutDimensionsChanged', @this.onLayoutChanged)];
            this.ToolsMenuOpeningListener = addlistener(hScope, 'ToolsMenuOpening', @this.onToolsMenuOpening);
            % Update the listeners' enable states.
            propertyChanged( this, 'AutoscaleMode' );
            this.UpdatesBeforeAutoscale = evalPropertyValue( this, 'UpdatesBeforeAutoscale' );
        end
        
        function set.ZoomMode(this,zmode)
            if strcmpi( zmode, 'off' )
                owner = '';
            else
                owner = zmode;
                setPropertyValue( this, 'PreviousZoomMode', zmode );
            end
            this.Application.MouseMotionOwner = owner;
            this.ZoomMode = zmode;
            react( this );
        end
        
        function set.PanEnabled(this,state)
            this.PanEnabled = state;
            if state
                enable = 'on';
            else
                enable = 'off';
                % Disable pan mode if enabled when disabling pan button
                if strcmpi( this.ZoomMode, 'Pan' )  %#ok<*MCSUP>
                    this.ZoomMode = 'Off';
                end
                if strcmpi( getPropertyValue( this, 'PreviousZoomMode' ), 'Pan' )
                    setPropertyValue( this, 'PreviousZoomMode', 'ZoomIn' );
                end
            end
            m = this.Menus;
            if isfield(m, 'Pan')
                set(m.Pan, 'Enable', enable);
            end
            b = this.ZoomButtons;
            if isfield(b, 'Pan')
                set(b.Pan, 'Enable', enable);
            end
        end
    end
    
    methods (Hidden)
        oldState = enableLimitListeners(this,newState)
        
        enablePan(this,state)
        
        optionsDialogTitle = getOptionsDialogTitle(~,~)
        
        [propsSchema,mode] = getPropsSchema(this,hDlg)
        
        onVisualChanged(this,~,~)
        
        onVisualUpdated(this,~,~)
        
        propertyChanged(this,propName)
        
        varargout = validate(this,hdlg)
    end
    
    methods (Access=protected)
        onFigureZoomCB(this,~,e)
        
        onPanEnterCB(this,~,e)
        
        onPanReleaseCB(this,~,e)
                
        visualUpdatedCallback(this,~,~)
        
        onSourceChanged(this,~,~)
        
        onSourceStop(this,~,~)
        
        plugInGUI = createGUI(this)
        
        disable(this)
        
        disableAutoscale(this,~,~)
        
        enableGUI(this,enabState)
        
        toggleZoomIn(this,~,~)
        
        toggleZoomX(this,~,~)
        
        toggleZoomY(this,~,~)
        
        togglePan(this,~,~)
        
        autoscaleSplitClickedCallback(this)
        
        autoscaleCallback(this,varargin)
        
        toggleAutoscaleAuto(this,~,~)
        
        toggleAutoscaleUpdates(this,~,~)
        
        enableOnceAtStopCallback(this,~,~)
        
        editOptionsCallback(this,~,~)
    end
    
    methods (Static)
        hPropDb = getPropertySet(varargin)
    end
end

% [EOF]
