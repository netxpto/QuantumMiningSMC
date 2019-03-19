function onVisualChanged(this,~,~)
%ONVISUALCHANGED Respond to visual changes.
% Remove the old listeners.
this.AxesLimitListeners = [  ];
newVisual = this.Application.Visual;
% The visual can be set to [].  When this happens, we want to return early
% because there will be no axes to add a listener to.
if isempty( newVisual )
    return
end
hScaler = this.Autoscaler;
% Ask the visual what limits should the plot navigation be listening to.
oldLimitName = hScaler.PrimaryAxes;
[newLimitName,sAxes] = getPlotNavigationAxes( newVisual );
hScaler.PrimaryAxes = upper( newLimitName );
hScaler.SecondaryAxes = upper( sAxes );
hAxesC = getAxesContainers( newVisual );
if isempty( strfind( sort( [ newLimitName, sAxes ] ), getPropertyValue( this, 'PreviousAutoscale' ) ) )
    setPropertyValue( this, 'PreviousAutoscale', hScaler.PrimaryAxes( 1 ) );
end
listeners = repmat( { [  ] }, 1, numel( hAxesC ) );
for indx = 1:numel( hAxesC )
    % Create listeners against all the axes' limitName properties to set the
    % autoscale mdoe back to manual.
    if ishghandle( hAxesC{ indx }.Axes )
        listeners{ indx } = uiservices.addlistener( hAxesC{ indx }.Axes, [ newLimitName, 'Lim' ], 'PostSet', @this.disableAutoscale );
        % If the zoom is enabled, make sure that we have cached
        % the current view
        if ~strcmpi( this.ZoomMode, 'off' )
            for jndx = 1:numel( hAxesC{ indx }.Axes )
                if isempty( resetplotview( hAxesC{ indx }.Axes( jndx ), 'GetStoredViewStruct' ) )
                    resetplotview( hAxesC{ indx }.Axes( jndx ), 'SaveCurrentView' );
                end
            end
        end
    end
end

% If the autoscale is compact, the button states will be updated in the
% openAutoscaleSplit method called when clicking the dropdown split button.
if ~this.IsAutoscaleCompact
    % Update which buttons are visible.
    hFig = this.Application.Parent;
    hX  = findobj(hFig, 'Tag', 'uimgr.uipushtool_AutoscaleX');
    hY  = findobj(hFig, 'Tag', 'uimgr.uipushtool_AutoscaleY');
    hXY = findobj(hFig, 'Tag', 'uimgr.uipushtool_AutoscaleXY');
    hC  = findobj(hFig, 'Tag', 'uimgr.uipushtool_AutoscaleC');
    
    allAxes = [newLimitName sAxes];
    hasX = ~isempty(strfind(allAxes, 'X'));
    hasY = ~isempty(strfind(allAxes, 'Y'));
    hasC = ~isempty(strfind(allAxes, 'C'));
    
    % Get the visible states.  Determine new separator states based on the
    % visible.  if we used to have X&Y and now we only have C, the separator
    % should be before C.
    xVis  = uiservices.logicalToOnOff(hasX);
    xSep  = xVis;
    yVis  = uiservices.logicalToOnOff(hasY);
    ySep  = uiservices.logicalToOnOff(hasY && ~hasX);
    xyVis = uiservices.logicalToOnOff(hasX && hasY);
    cVis  = uiservices.logicalToOnOff(hasC);
    cSep  = uiservices.logicalToOnOff(~(hasX || hasY));
    set(hX,  'Visible', xVis, 'Separator', xSep);
    set(hY,  'Visible', yVis, 'Separator', ySep);
    set(hXY, 'Visible', xyVis); % xy will never be first, it will never have sep.
    set(hC,  'Visible', cVis, 'Separator', cSep);
end

this.AxesLimitListeners = [ listeners{ : } ];
% Updates when the limit name changes.
if ~strcmp( newLimitName, oldLimitName )
    % Update the dialog if its open.
    editOptions( this, false );
end
end
