function onSourceChanged(this,~,~)
%Listener to the SourceChanged Event.
% Enable the FirstVisualUpdatedListener.  We want to respond to the first
% VisualUpdated event from the application after the source is added.
uiservices.setListenerEnable( this.FirstVisualUpdatedListener, true );
if this.AutoscaleAtUpdate
    uiservices.setListenerEnable( this.VisualUpdatedListener, true );
    this.UpdateCount = 0;
end
end
