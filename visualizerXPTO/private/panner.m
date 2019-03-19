function varargout = panner(varargin)
%PANNER Panner management function.
 
%   Copyright 1988-2010 The MathWorks, Inc.

if nargin < 1
    action = 'init';
else
    action = varargin{1};
end

switch lower(action)

%-----------------------------------------------------------------
% curs = panner('motion',fig)
%  returns 1 if currentpoint is over panner patch, 0 else
%
  case 'motion'
    fig = varargin{2};
    ud = get(fig,'UserData');
    panaxes = ud.panner.panaxes;
    p = get(panaxes,'CurrentPoint');
    panpatch = ud.panner.panpatch;
    xd = get(panpatch,'XData');
    yd = get(panpatch,'YData');
    varargout{1} = pinrect(p(1,1:2),[xd([1 2]) yd([1 3])]);
    
%-----------------------------------------------------------------
% panner  -or-  panner('init',fig)
%  startup code - adds panner to fig
%
  case 'init'

    if nargin<2
       fig = gcf;
    else
       fig = varargin{2};
    end

    ud = get(fig,'UserData');

    % create panner axes and patch
    panaxes = axes( ...
        'Parent',fig,...
        'Tag', 'panaxes', ...
        'Box', 'off', ...
        'Units','pixels',...
        'XLim', ud.limits.xlim,...
        'YLim', ud.limits.ylim,...
        'XTick', [], ...
        'YTick', [] );

    edgecolor = get(panaxes,'XColor');

    pc = get(panaxes,'Color');
    if ~isstr(pc)  % might be 'none', in which case don't set x,ycolor of axes
        set(panaxes,'XColor',pc)
        set(panaxes,'YColor',pc)
    else
        fc = get(fig,'Color');
        set(panaxes,'XColor',fc)
        set(panaxes,'YColor',fc)
    end
    
    set(get(panaxes,'YLabel'),'String',getString(message('signal:sptoolgui:Panner')),'Tag','pannerxlabel',...
            'Color',edgecolor,'FontSize',8)
    
    ud.panner.panaxes = panaxes;

    xlim = get(ud.mainaxes,'XLim');
    ylim = get(ud.mainaxes,'YLim');
    xd = xlim([1 2 2 1 1]);
    yd = ylim([1 1 2 2 1]);
    panpatch = patch( ...
        xd, ...
        yd, ...
        [1 1 1],...
        'FaceColor',pc, ...
        'Parent',panaxes,...
        'Tag','panpatch',...
        'EdgeColor',edgecolor,...
        'ButtonDownFcn', 'sbswitch(''pandown'',0)');
    % Use erasemode property if HG1. Deprecate erasemode in HG2.
    if ~feature('hgusingmatlabclasses')
        set(panpatch,'EraseMode','xor');
    elseif ~graphicsversion(panpatch,'handlegraphics')
        set(panpatch,'PickableParts','all')
    end

    set(panpatch,'FaceColor','none')
    ud.panner.panpatch = panpatch;

    ud.prefs.tool.panner = 1;

    set(fig,'UserData',ud)

    panner('resize',1,fig)

    set(fig,'ResizeFcn',appstr(get(fig,'ResizeFcn'),...
                'sbswitch(''panner'',''resize'')'))

%-----------------------------------------------------------------
% panner('close',fig)
%  shutdown code - removes panner from browser
%   Inputs:
%     fig - figure handle of browser
%
  case 'close'
    fig = varargin{2};
    ud = get(fig,'UserData');
    ud.prefs.tool.panner = 0;
    delete(findobj(fig,'tag','panaxes')) % delete panner axes (and all children)
    % clear the line cache:
    ud.linecache.ph = [];
    ud.linecache.phh = [];

    for i=1:length(ud.lines)
        ud.lines.ph = [];
    end
    
    set(fig,'ResizeFcn',remstr(get(fig,'ResizeFcn'),...
          'sbswitch(''panner'',''resize'')'))
    set(fig,'UserData',ud)

%-----------------------------------------------------------------
% panner('resize',create_flag,fig)
%  ResizeFcn for panner
%
  case 'resize'
    if nargin >= 2
        create_flag = varargin{2};
    else
        create_flag = 0;
    end
    if nargin >= 3
        fig = varargin{3};
    else
        fig = gcbf;
    end

    ud = get(fig,'UserData');
    sz = ud.sz;

    fp = get(fig,'Position');   % in pixels already

    toolbar_ht = 0;
    
    ruler_port = [0 0 fp(3) sz.rh*ud.prefs.tool.ruler];
    panner_port = [0 ruler_port(4) ...
                    fp(3) sz.ph*ud.prefs.tool.panner];

    % fancy way:
    % xl = get(ud.panner.panaxes,'xlabel');
    % xlUnits = get(xl,'units');
    % set(xl,'units','points')
    % ex = get(xl,'extent');
    % set(xl,'units',xlUnits)

    % simple way:
    %ex = [0 0 0 16];   % guess height of xlabel in pixels
    ex = [0 0 0 0];  %<-- don't leave room for xlabel; moved to ylabel; TPK 6/21/99
    if ispc
        tweak = 5;
    else
        tweak = 0;
    end    
    pan_pos = panner_port + [sz.ffs sz.ffs+ex(4)+tweak -2*sz.ffs -sz.ffs-ex(4)-tweak];

    % Make panner width same as main axes: TPK 6/21/99
    ax_pos = get(ud.mainaxes,'Position');
    pan_pos(1) = ax_pos(1); pan_pos(3) = ax_pos(3);

    set(ud.panner.panaxes,'Position',pan_pos)

%-----------------------------------------------------------------
% panner('update',fig)
%   assume xlimits of panaxes are correctly set to full view
%   reset patch to limits of mainaxes
%
  case 'update'
     if nargin >= 2
         fig = varargin{2};
     else
         fig = gcf;
     end
     ud = get(fig,'UserData');
     panaxes = ud.panner.panaxes;
     xlim = get(ud.mainaxes,'XLim');
     ylim = get(ud.mainaxes,'YLim');
     panpatch = ud.panner.panpatch;
     setpdata(panpatch,xlim,ylim)

% ----------------------------------------------------------------------
% panner('zoom',xlim,ylim)
%   set patch limits based on limits input
  case 'zoom'
     xlim = varargin{2};
     ylim = varargin{3};
     if nargin > 3
         fig = varargin{4};
     else
         fig = gcf;
     end
     ud = get(fig,'UserData');
     panpatch = ud.panner.panpatch;
     setpdata(panpatch,xlim,ylim)

end

function setpdata(panpatch,xlim,ylim)
%setpdata - set x and ydata of patch object to rectangle specified by
% xlim and ylim input
 
     set(panpatch,'XData',[xlim(1) xlim(2) xlim(2) xlim(1) xlim(1)], ...
              'YData',[ylim(1) ylim(1) ylim(2) ylim(2) ylim(1)]) % thumb patch
    

