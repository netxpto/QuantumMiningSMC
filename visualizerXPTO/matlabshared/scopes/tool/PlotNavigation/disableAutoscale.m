function disableAutoscale(this,~,~)
setPropertyValue( this, 'OnceAtStop', false, 'AutoscaleMode', 'Manual' );
propertyChanged( this, 'OnceAtStop' );
end
