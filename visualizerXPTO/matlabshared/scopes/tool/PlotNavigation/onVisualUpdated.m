function onVisualUpdated(this,~,~)
%Listener to the VisualUpdated event.
% This should only fire one time after a new source has been added.
uiservices.setListenerEnable( this.FirstVisualUpdatedListener, false );
% Reset the zoom state to whatever the new visual/data combination
% indicates.  Only perform this action if the zoom is storing old values.
hVisual = this.Application.Visual;
axesContainers = hVisual.getAxesContainers;
for containerInd = 1:length( axesContainers )
    hAxes = axesContainers{ containerInd }.Axes;
    for idx = 1:length( hAxes )
        if ishghandle( hAxes( idx ) ) && ~isempty( resetplotview( hAxes( idx ), 'GetStoredViewStruct' ) )
            zoom( hAxes( idx ), 'reset' );
        end
    end
end
end
