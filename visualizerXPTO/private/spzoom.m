function spzoom(action)
%SPZOOM  Spectrum Viewer zoom function.
%  Contains callbacks for Zoom button group of spectview.
%    mousezoom
%    zoomout
%    zoominx
%    zoomoutx
%    zoominy
%    zoomouty

%   Copyright 1988-2010 The MathWorks, Inc.
 
fig = gcbf;
ud = get(fig,'UserData');

if ud.pointer==2   % help mode
    if strcmp(action,'mousezoom')
        state = strcmp(get(ud.toolbar.mousezoom,'State'),'on');
        % toggle button state back to the way it was:
        if state
              set(ud.toolbar.mousezoom,'State','off')
        else
              set(ud.toolbar.mousezoom,'State','on')
        end
    end
    spthelp('exit','spzoom',action)
    return
end

switch action

case 'mousezoom'
    state = strcmp(get(ud.toolbar.mousezoom,'State'),'on');
    if state == 1   % button is currently down
        set(fig,'WindowButtonDownFcn','sbswitch(''spzoom'',''mousedown'')')
        ud.pointer = 1;  
        set(fig,'UserData',ud)
        set(ud.mainaxes,'UIContextMenu',[])
        setptr(fig,'arrow')
    else   % button is currently up - turn off zoom mode
        set(fig,'WindowButtonDownFcn','')
        ud.pointer = 0;  
        set(fig,'UserData',ud)
        set(ud.mainaxes,'UIContextMenu',ud.contextMenu.u)
        setptr(fig,'arrow')
    end

case 'zoomout'
    set(ud.mainaxes,'XLim',ud.limits.xlim,'YLim',ud.limits.ylim)
    
    if ud.prefs.tool.ruler
        ruler('newlimits',fig)
    end
    
case 'zoominx'
    xlim = get(ud.mainaxes,'XLim');
    newxlim = .25*[3 1]*xlim' + [0 diff(xlim)/2];
    if diff(newxlim) > 1e-13;
       set(ud.mainaxes,'XLim',newxlim)
       if ud.prefs.tool.ruler
           ruler('newlimits')
       end
    end

case 'zoomoutx'
    xlim = get(ud.mainaxes,'XLim');
    xlim = .5*[3 -1]*xlim' + [0 diff(xlim)*2];
    xlim = [max(xlim(1),ud.limits.xlim(1)) min(xlim(2),ud.limits.xlim(2))];
    set(ud.mainaxes,'XLim',xlim)
    if ud.prefs.tool.ruler
        ruler('newlimits')
    end

case 'zoominy'
    ylim = get(ud.mainaxes,'YLim');
    newylim = .25*[3 1]*ylim' + [0 diff(ylim)/2];
    if diff(newylim) > 0;
       set(ud.mainaxes,'YLim',newylim)
       if ud.prefs.tool.ruler
           ruler('newlimits')
       end
    end

case 'zoomouty'
    ylim = get(ud.mainaxes,'YLim');
    ylim = .5*[3 -1]*ylim' + [0 diff(ylim)*2];
    ylim = [max(ylim(1),ud.limits.ylim(1)) min(ylim(2),ud.limits.ylim(2))];
    set(ud.mainaxes,'YLim',ylim)
    if ud.prefs.tool.ruler
        ruler('newlimits')
    end

%-------------- these are self callbacks:
case 'mousedown'
    ud.justzoom = get(fig,'CurrentPoint'); 
    set(fig,'UserData',ud)

    pstart = get(fig,'CurrentPoint');

    % don't do anything if click is outside mainaxes_port
    fp = get(fig,'Position');   % in pixels already
    sz = ud.sz;
    toolbar_ht = 0; %sz.ih;
    left_width = ud.left_width;
    mp = [left_width 0  fp(3)-(left_width)  fp(4)-(toolbar_ht)];
    %click is outside of main panel:
    if ~pinrect(pstart,[mp(1) mp(1)+mp(3) mp(2) mp(2)+mp(4)])
        return
    end
    
    r=rbbox([pstart 0 0],pstart);

    oldxlim = get(ud.mainaxes,'XLim');
    oldylim = get(ud.mainaxes,'YLim');

    if all(r([3 4])==0)
        % just a click - zoom about that point
        p1 = get(ud.mainaxes,'CurrentPoint');

        switch get(fig,'SelectionType')
        case 'normal'     % zoom in
            xlim = p1(1,1) + [-.25 .25]*diff(oldxlim);
            ylim = p1(1,2) + [-.25 .25]*diff(oldylim);
        otherwise    % zoom out
            xlim = p1(1,1) + [-1 1]*diff(oldxlim);
            ylim = p1(1,2) + [-1 1]*diff(oldylim);
        end

    elseif any(r([3 4])==0)  
        % zero width or height - stay in zoom mode and 
        % try again
        return

    else 
        % zoom to the rectangle dragged
        set(fig,'CurrentPoint',[r(1) r(2)])
        p1 = get(ud.mainaxes,'CurrentPoint');
        set(fig,'CurrentPoint',[r(1)+r(3) r(2)+r(4)])
        p2 = get(ud.mainaxes,'CurrentPoint');
        
        xlim = [p1(1,1) p2(1,1)];
        ylim = [p1(1,2) p2(1,2)];
    end

    newxlim = inbounds(xlim,ud.limits.xlim);
    newylim = inbounds(ylim,ud.limits.ylim);
    if diff(newxlim) > 1e-13 
       set(ud.mainaxes,'XLim',newxlim)
    else
       newxlim = oldxlim;
    end
    if diff(newylim) > 0
       set(ud.mainaxes,'YLim',newylim)
    end
    if ud.prefs.tool.ruler
        ruler('newlimits')
    end
    if ~ud.prefs.tool.zoompersist
    % LEAVE ZOOM MODE
        setptr(fig,'arrow')
        set(fig,'WindowButtonDownFcn','')
        set(ud.toolbar.mousezoom,'State','off')
        set(ud.mainaxes,'UIContextMenu',ud.contextMenu.u)
        ud.pointer = 0;  
    end
    set(fig,'UserData',ud)
    set(fig,'CurrentPoint',ud.justzoom)

end

        


