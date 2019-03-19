% Local functions called from getPropertySet()
%
function toggleZoomSplit(this,varargin)
toggle( this, getPropertyValue( this, 'PreviousZoomMode' ) );

end
