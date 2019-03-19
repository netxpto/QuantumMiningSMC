function onSourceStop(this,~,~)
hSrc = this.Application.DataSource;
if ~isempty( hSrc ) && ~hSrc.isSnapshotMode
    performAutoscale( this, true, true );
end
end
