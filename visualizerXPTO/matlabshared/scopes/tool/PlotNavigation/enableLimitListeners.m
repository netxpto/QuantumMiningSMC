function oldState = enableLimitListeners(this,newState)
%ENABLELIMITLISTENERS
limitListeners = this.AxesLimitListeners;
if ~isempty( limitListeners )
    oldState = uiservices.getListenerEnable( limitListeners );
    uiservices.setListenerEnable( limitListeners, newState );
else
    oldState = true;
end
end
