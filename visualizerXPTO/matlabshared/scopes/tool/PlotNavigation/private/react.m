function react(this)
%REACT React to current zoom state
% Install cursor and functions

%   Copyright 2013-2014 The MathWorks, Inc.

hFig = this.Application.Parent;

panState = 'off';
captureState = true;
switch lower(this.ZoomMode)
    case 'zoomin'
        zoom(hFig, 'on');
    case 'zoomx'
        zoom(hFig, 'xon');
    case 'zoomy'
        zoom(hFig, 'yon');
    case 'pan'
        
        % No need to check if hPan is here.  It can't be enabled if there
        % is no widget for it.
        pan(hFig, 'on');
        panState = 'on';
    otherwise
        captureState = false;
        zoom(hFig, 'off');
        pan(hFig, 'off');
end
if this.IsZoomCompact
    updateZoomSplit(this)
end
if captureState
    if this.ZoomPanNeverEnabled
        hFigure = this.Application.Parent;
        hZoomState = zoom(hFigure);
        hZoomState.ActionPostCallback = @this.onFigureZoomCB;
        hPanState = pan(hFigure);
        hPanState.ActionPreCallback = @this.onPanEnterCB;
        hPanState.ActionPostCallback = @this.onPanReleaseCB;
        this.ZoomPanNeverEnabled = false;
    end
    hAxesC = this.Application.Visual.getAxesContainers;
    for indx = 1:numel(hAxesC)
        hAxes = hAxesC{ indx }.Axes;
        for jndx = 1:numel(hAxes)
            if isempty(resetplotview(hAxes(jndx), 'GetStoredViewStruct'))
                resetplotview(hAxes(jndx), 'SaveCurrentView');
            end
        end
    end
end
setappdata(hFig, 'PanOnState', panState);

% [EOF]
