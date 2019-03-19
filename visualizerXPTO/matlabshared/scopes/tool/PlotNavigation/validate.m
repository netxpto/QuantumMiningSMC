function varargout = validate(this,hdlg)
%VALIDATE Returns true if this object is valid
success = true;
exception = MException.empty;
suffix = this.Application.Specification.getHelpArgs( 'PlotNavigationSuffix' );
if isempty( suffix )
    suffix = '';
end
[p,s] = getPlotNavigationAxes( this.Application.Visual );
if hdlg.getWidgetValue( [ 'Plot NavigationAutoscaleSecondaryAxes', suffix ] )
    p = [ p, s ];
end
for axis = p
    ydisplay = hdlg.getWidgetValue( [ 'Plot Navigation', axis, 'DataDisplay', suffix ] );
    try
        ydisplay = evaluateVariable( this.Application, ydisplay );
        if isNotValid( ydisplay )
            success = false;
            [msg,id] = uiscopes.message( [ 'Invalid', axis, 'Display', suffix ] );
            exception = MException( id, msg );
        end
    catch exception
        success = false;
    end
end
nUpdates = hdlg.getWidgetValue( [ 'Plot NavigationUpdatesBeforeAutoscale', suffix ] );
try
    nUpdates = evaluateVariable( this.Application, nUpdates );
    if ~isscalar( nUpdates ) || ~isnumeric( nUpdates ) || isnan( nUpdates ) || nUpdates<1 || rem( nUpdates, 1 )~=0
        success = false;
        [msg,id] = uiscopes.message( 'InvalidUpdatesBeforeAutoscale' );
        exception = MException( id, msg );
    end
catch exception
    success = false;
end
if nargout
    varargout = { success, exception };
elseif ~success
    throw( exception );
end
end
