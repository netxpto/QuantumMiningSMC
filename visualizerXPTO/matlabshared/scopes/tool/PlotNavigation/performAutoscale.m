function performAutoscale(this,auto,force)
%PERFORMAUTOSCALE Perform the autoscale action.
%   performAutoscale(h, AUTO, FORCE) perform an autoscale operation.
%   AUTO is passed to the onAutoscale method to inform visuals that the
%   scaling operation was automatic.  It is true for Automatic and
%   OnceAtStop AutoscalingModes and false for Manual.  When FORCE is true
%   the scaling operation will always happen (true for once at stop and
%   when the user presses the menu or button).

%   Copyright 2013-2014 The MathWorks, Inc.

if nargin<3
    force = true;
end
if ~ischar(force) && ~force && ...
        this.AutoscaleAtUpdate && this.Application.DataSource.isRunning
    % Check if enough updates have passed before we perform an update.
    this.UpdateCount = this.UpdateCount + 1;
    if this.UpdateCount<this.UpdatesBeforeAutoscale
        return
    end
    force = true;
    uiservices.setListenerEnable( this.VisualUpdatedListener, false );
end
if ischar( force )
    setPropertyValue( this, 'PreviousAutoscale', force );
end
% Disable the axes limit listener.
oldEnabState = enableLimitListeners( this, false );
hVisual = this.Application.Visual;
onPreAutoscale( hVisual );
% Get all of the axes from the visual.
axesContainers = getAxesContainers( hVisual );
hAutoscaler = this.Autoscaler;
hasAutoscaled = false;
for containerInd = 1:length( axesContainers )
    [indAutoscaled,axesScaled] = autoscale( hAutoscaler, axesContainers{ containerInd }, force );
    hasAutoscaled = hasAutoscaled || indAutoscaled;
end
% Re-enable the axes limit listener.
enableLimitListeners( this, oldEnabState );
% Notify visual that autoscale has been performed
if hasAutoscaled
    try
        onAutoscale( this.Application.Visual, auto, axesScaled );
    catch ME
        if strcmp( ME.identifier, 'MATLAB:maxrhs' ) || ...
           strcmp( ME.identifier, 'MATLAB:TooManyInputs' )
            onAutoscale( this.Application.Visual, auto );
        else
            rethrow( ME );
        end
    end
end
onPostAutoscale( hVisual );
if hasAutoscaled
    for containerInd = 1:numel( axesContainers )
        hAxes = axesContainers{ containerInd }.Axes;
        for indx = 1:numel( hAxes )
            zoom( hAxes( indx ), 'reset' );
        end
    end
end
end
