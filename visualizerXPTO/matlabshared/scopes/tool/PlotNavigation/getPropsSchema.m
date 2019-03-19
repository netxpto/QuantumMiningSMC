function [propsSchema,mode] = getPropsSchema(this,hDlg)
%GETPROPSSCHEMA Get the propsSchema.
hCfg = this.Config;
% Add the autoscale mode combobox.
if ~getPropertyValue( this, 'ShowAutoscaleAction' )
    propsSchema = [  ];
    mode = [  ];
    return
end
suffix = this.Application.Specification.getHelpArgs( 'PlotNavigationSuffix' );
if isempty( suffix )
    suffix = '';
end
[mode.Label,mode.Widget] = uiscopes.getWidgetSchema( hCfg, 'AutoscaleMode', 'combobox', 1, 1 );
mode.Label.ColSpan = [ 1, 2 ];
mode.Widget.ColSpan = [ 3, 4 ];
mode.Widget.DialogRefresh = true;
mode.Widget.Tag = [ mode.Widget.Tag, suffix ];
mode.Widget.Entries = { getString( message( 'Spcuilib:scopes:AutoscaleModeManual' ) ), getString( message( 'Spcuilib:scopes:AutoscaleModeAuto' ) ), getString( message( 'Spcuilib:scopes:AutoscaleModeUpdates' ) ) };
row = uiservices.Counter( 0 );
if nargout<2
    row.next;
end
% Add the always expand checkbox.
opt_row = row.next;
expandonly = uiscopes.getWidgetSchema( hCfg, 'ExpandOnly', 'checkbox', opt_row, 2 );
[updates_lbl,updates] = uiscopes.getWidgetSchema( hCfg, 'UpdatesBeforeAutoscale', 'edit', opt_row, 2 );
if nargout>1
    expandonly.ColSpan = [ 1, 5 ];
    updates_lbl.ColSpan = [ 1, 2 ];
    updates.ColSpan = [ 3, 3 ];
else
    expandonly.ColSpan = [ 2, 5 ];
end
% Determine if we should disable the expand checkbox.
val = uiservices.getWidgetValue( mode.Widget, hDlg );
if ischar( val )
    val = find( strcmpi( val, { 'Manual', 'Auto', 'Updates' } ) ) - 1;
end
expandonly.Visible = false;
updates_lbl.Visible = false;
updates.Visible = false;
switch val
    case 1
        expandonly.Visible = true;
    case 2
        updates_lbl.Visible = true;
        updates.Visible = true;
end
[pAxes,sAxes] = getPlotNavigationAxes( this.Application.Visual );
onceAtStop = uiscopes.getWidgetSchema( hCfg, 'OnceAtStop', 'checkbox', row.next, 1 );
onceAtStop.ColSpan = [ 1, 5 ];
items = {  };
spacer.Type = 'text';
spacer.Tag = 'Spacer';
spacer.Name = '  ';
spacer.ColSpan = [ 1, 1 ];
for aName = pAxes
    axis.Type = 'text';
    axis.Tag = [ aName, 'AxisLabel' ];
    axis.Name = getString( message( [ 'Spcuilib:scopes:Text', aName, 'Axis' ] ) );
    axis.ColSpan = [ 1, 5 ];
    axis.RowSpan = row.next( 2 );
    % Add the y-axis data range editbox.
    [rangeLabel,range] = uiscopes.getWidgetSchema( hCfg, [ aName, 'DataDisplay' ], 'edit', row.next, 2 );
    range.Tag = [ range.Tag, suffix ];
    % Add the yrange anchor "orientation" combobox.
    [anchorLabel,anchor] = extmgr.getWidgetSchema( hCfg, [ 'Autoscale', aName, 'Anchor' ], getString( message( 'Spcuilib:scopes:AutoscaleAnchorLabel' ) ), 'combobox', row.Value, 4 );
    anchor.Entries = { getString( message( [ 'Spcuilib:scopes:Autoscale', aName, 'Anchor1' ] ) ), getString( message( [ 'Spcuilib:scopes:Autoscale', aName, 'Anchor2' ] ) ), getString( message( [ 'Spcuilib:scopes:Autoscale', aName, 'Anchor3' ] ) ) };
    spacer.RowSpan = range.RowSpan;
    anchor.Tag = [ anchor.Tag, suffix ];
    items = [ items, { axis, spacer, rangeLabel, range, anchorLabel, anchor } ];  %#ok<AGROW>
end
if ~isempty( sAxes )
    % Get the autoscale x-axis checkbox.
    sAxis = extmgr.getWidgetSchema( hCfg, 'AutoscaleSecondaryAxes', getString( message( 'Spcuilib:scopes:AutoscaleXAxisLabel' ) ), 'checkbox', row.next, 1 );
    sAxis.ColSpan = [ 1, 5 ];
    sAxis.DialogRefresh = true;
    sAxis.Tag = [sAxis.Tag suffix];
    items = [ items, { sAxis } ];
end
for aName = sAxes
    % Add the x-axis data range editbox.
    [rangeLabel,range] = uiscopes.getWidgetSchema( hCfg, [ aName, 'DataDisplay' ], 'edit', row.next, 2 );
    range.Tag = [ range.Tag, suffix ];
    % Add the xrange anchor "orientation" combobox.
    [anchorLabel,anchor] = extmgr.getWidgetSchema( hCfg, [ 'Autoscale', aName, 'Anchor' ], getString( message( 'Spcuilib:scopes:AutoscaleAnchorLabel' ) ), 'combobox', row.Value, 4 );
    anchor.Entries = { getString( message( [ 'Spcuilib:scopes:Autoscale', aName, 'Anchor1' ] ) ), getString( message( [ 'Spcuilib:scopes:Autoscale', aName, 'Anchor2' ] ) ), getString( message( [ 'Spcuilib:scopes:Autoscale', aName, 'Anchor3' ] ) ) };
    anchor.Tag = [ anchor.Tag, suffix ];
    spacer.RowSpan = range.RowSpan;
    items = [ items, { spacer, rangeLabel, range, anchorLabel, anchor } ];  %#ok<AGROW>
end
% Special case a few of the labels if color is the primary axes
if strcmp( pAxes, 'C' )
    mode.Label.Name = uiscopes.message( 'AutoscaleModeColorLabel' );
    expandonly.Name = uiscopes.message( 'ExpandOnlyColorLabel' );
    onceAtStop.Name = uiscopes.message( 'OnceAtStopColorLabel' );
end
expandonly.Tag = [ expandonly.Tag, suffix ];
onceAtStop.Tag = [ onceAtStop.Tag, suffix ];
updates.Tag = [ updates.Tag, suffix ];
% Define all the items that always exist.
items = [ { expandonly, updates_lbl, updates, onceAtStop }, items ];
% If we have 2 outputs, return the mode widgets in a separate output.  If
% not, add them to the list of items.
if nargout<2
    items = [ { mode.Label, mode.Widget }, items ];
end
propsSchema.Name = uiscopes.message( 'TextParameters' );
propsSchema.Type = 'group';
propsSchema.LayoutGrid = [ row.next, 5 ];
propsSchema.RowStretch = [ zeros( 1, row.Value - 1 ), 1 ];
propsSchema.ColStretch = [ 0, 1, 2, 1, 2 ];
propsSchema.Items = items;
propsSchema.Tag = 'AutoscalingPropertiesGroup';
end
