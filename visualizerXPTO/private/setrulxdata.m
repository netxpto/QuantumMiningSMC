function setrulxdata(h,xdata)
%SETRULXDATA Checks to see if ruler lines are on top of each other.
%   If they are, sets thickness of line 1 to 3, otherwise sets
%   thickness of line 1 to the default.
%   If markers are visible, sets marker size of line 1 to twice that
%   of marker size of line 2.

%   Copyright 1988-2010 The MathWorks, Inc.

    ax = get(h,'Parent');
    fig = get(ax,'Parent');
    
    ud = get(fig,'UserData');
    R = find(h==ud.ruler.lines(1:2));
    R1 = 1 + (R==1);
    
    xdata1 = get(ud.ruler.lines(R1),'XData');
    
    if xdata1(1) ~= xdata(1)
        set(ud.ruler.lines(1),'LineWidth',get(ud.ruler.lines(2),'LineWidth'))
        set(ud.ruler.markers(1),'MarkerSize',...
                    get(ud.ruler.markers(2),'MarkerSize'))
    else
        set(ud.ruler.lines(1),'LineWidth',3)
        set(ud.ruler.markers(1),'MarkerSize',...
                    1.5*get(ud.ruler.markers(2),'MarkerSize'))
    end
    set(ud.ruler.lines(R),'XData',xdata)