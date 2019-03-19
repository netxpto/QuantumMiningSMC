function varargout = fvinit(varargin)
%FVINIT Initialize filter viewer.

%   Copyright 1988-2010 The MathWorks, Inc.
 

    save_shh = get(0,'ShowHiddenHandles');
    set(0,'ShowHiddenHandles','on')

    figname = getString(message('signal:sptoolgui:FilterViewer'));

    toolnum = 1;  % which instance of the filter viewer

    visFrame = 'off';   % set frame around single popups invisible 
    if strcmp(visFrame,'off')
       indentPop = 15; 
    else
       indentPop = 0;
    end
    
    % ====================================================================
    % set defaults and initialize userdata structure
    filtview1Prefs = sptool('getprefs','filtview1');
    filtview2Prefs = sptool('getprefs','filtview2');
    rulerPrefs = sptool('getprefs','ruler');
    colorPrefs = sptool('getprefs','color');

    ud.prefs.tool.zoompersist = filtview1Prefs.zoomFlag;

    ud.prefs.colororder = colorPrefs.colorOrder;
    ud.prefs.linestyleorder = colorPrefs.linestyleOrder;

    % Ruler preferences
    ud.prefs.tool.ruler = filtview1Prefs.rulerEnable;    % rulersenabled 
    
    markerStr = { '+' 'o' '*' '.' 'x' ...
         'square' 'diamond' 'v' '^' '>' '<' 'pentagram' 'hexagram' }';
    typeStr = {'vertical' 'horizontal' 'track' 'slope'}';

    ud.prefs.ruler.color = rulerPrefs.rulerColor;
    ud.prefs.ruler.marker = markerStr{rulerPrefs.rulerMarker};
    ud.prefs.ruler.markersize = rulerPrefs.markerSize;
    ud.prefs.ruler.type = typeStr{rulerPrefs.initialType};
    
    ud.linecache.h = [];  % no line objects defined yet

    ud.lines = [];
    ud.SPToolIndices = [];
    ud.focusline = [];
    ud.focusIndex = 1;
    ud.colorCount = 0;  % number of colors allocated thus far
    ud.colororder = num2cell(evalin('base',ud.prefs.colororder),2);
    ud.linestyleorder = num2cell(evalin('base',ud.prefs.linestyleorder),2);

    for i = 1:6  % number of subplots
      ud.limits(i).xlim = [0 1];
      ud.limits(i).ylim = [0 1];
    end

    ud.prefs.nfft = evalin('base',filtview1Prefs.nfft);
    ud.prefs.nimp = evalin('base',filtview1Prefs.nimp);
            % Length of impulse / step response ([] ==> auto)
                                    
    ud.prefs.magmode = {'linear' 'log' 'decibels'};
    ud.prefs.magmode = ud.prefs.magmode{filtview1Prefs.magscale};
    ud.prefs.phasemode = {'degrees' 'radians'};
    ud.prefs.phasemode = ud.prefs.phasemode{filtview1Prefs.phaseunits};
    ud.prefs.freqscale = {'linear' 'log'};
    ud.prefs.freqscale = ud.prefs.freqscale{filtview1Prefs.freqscale};
    ud.prefs.freqrange = filtview1Prefs.freqrange;
            % 1==[0 Fs/2], 2==[0 Fs], 3==[-Fs/2 Fs/2]
                        
    if filtview2Prefs.mode1
        ud.prefs.tilemode = [2 3];
    elseif filtview2Prefs.mode2
        ud.prefs.tilemode = [3 2];
    elseif filtview2Prefs.mode3
        ud.prefs.tilemode = [6 1];
    elseif filtview2Prefs.mode4
        ud.prefs.tilemode = [1 6];
    end
            
    ud.prefs.plots = [1 1 0 0 0 0]';  
    realFilterFlag = 1; 
    ud.prefs.plottitles = filtview('plotTitles',realFilterFlag);
    ud.prefs.plotspacing = [40 35 10 20]; 
        % spacing in pixels from [left bottom right top] 
        % lots of room: [40 40 20 25]
    scalefactor = (get(0,'ScreenPixelsPerInch')/72)^.5;
    ud.prefs.plotspacing = ud.prefs.plotspacing*scalefactor;
    
    co = get(0,'DefaultAxesColorOrder');
    ud.prefs.linecolor = co(1,:);

    ud.sz = sptsizes;
    ud.sz.bw = ud.sz.bw+10;
    
    ud.justzoom = [ -1 -1 ] ;  % used for mode switching (between zoom and pan)
             % justzoom is set to the current point after a zoom, and is reset
             % by a call to justzoom.m to [0 0]
             % Set to [-1 -1] now since currentpoint is [0 0] for an invisible
             % figure
    filtDefined = 0; 
    
    %
    % Initialize ud.filt 
    %
    switch nargin
    case 0
        makeParams = {1 1 1};
    case 1
        for i = 1:length(varargin{1})  % Loop through all selected filters
            if isstruct(varargin{1}(i))
                [valid,ud.filt(i)] = importfilt('valid',varargin{1}(i));
                if ~valid
                    error(message('signal:fvinit:InvalidParam'))
                end
                filtDefined = 1;
            else
                makeParams = {varargin{1} 1 1};
            end
        end
    case 2
            makeParams = {varargin{1:2} 1};
    case 3
            makeParams = varargin;
    end
    if ~filtDefined
        [err,~,ud.filt,errObj] = importfilt('make',{1 makeParams{:}});
        if err
            error(errObj);
        end
    end
    
    ud.prefs.Fs = sprintf('%.9g',max([ud.filt.Fs]));

    ud.prefs.rulerPrefsPanelName = 'filtview1'; %ruler needs to know this so
       % it can update the SPTOOL preferences when it is turned on/off

    ud.tilefig = [];  % handle to tile dialog box figure
    ud.loadfig = [];  % handle to Load dialog box figure
    ud.tabfig = [];  % settings figure handle
    ud.toolnum = toolnum;
    
    ud.pointer = 0;  % pointer mode ...  == -1 watch, 0 arrow/drag indicators, 
                     %     1 zoom, 2 help
    sz = ud.sz;

    screensize = get(0,'ScreenSize');
    
    % minimum width, height of figure window;
    ud.resize.minsize = [3*sz.bw+9*sz.fus+ud.sz.rw+60 ...
                         sz.ih+16*sz.fus+11*sz.uh+4*sz.lh];   
       % actual values on PCWIN [595  419];
       % Fontsize (therefore label height, sz.lh) on UNIX is smaller 
       % therefore actual values on UNIX [595  403]
       
    ud.resize.leftwidth = sz.bw+6*sz.fus;
    ud.left_width = ud.resize.leftwidth;  % used by ruler.m for positioning
    ud.resize.topheight = 0; %sz.ih;
    
    fp = get(0,'DefaultFigurePosition');
    
    % Give UNIX figures a little more height so that the ruler panel fits
    % By default figures on UNIX platforms are smaller
    if isunix
        fp = fp + [0 -20 0 20];    % Arbitrarily add (enough) 20 to the height
    end
    
    w = max(ud.resize.minsize(1),fp(3))+ud.sz.rw;
    h = max(ud.resize.minsize(2),fp(4));
    fp = [fp(1) fp(2)+fp(4)-h w h];  % keep upper-left corner stationary
    
    % ====================================================================
    % Save figure position for use in resizefcn:   
    ud.resize.figpos = fp;
    
    % CREATE FIGURE
    fig = figure('CreateFcn','',...
            'CloseRequestFcn','filtview(''SPTclose'')',...
            'Tag','filtview',...
            'NumberTitle','off',...
            'DockControls','off',...
            'IntegerHandle','off',...
            'UserData',ud,...
            'Units','pixels',...
            'Position',fp,...
            'MenuBar','none',...
            'InvertHardcopy','off',...
            'PaperPositionMode','auto',...
            'Visible','off',...
            'Name',figname,...
            'ResizeFcn','sbswitch(''resizedispatch'')%');


    uibgcolor = get(0,'DefaultUicontrolBackgroundColor');
    uifgcolor = get(0,'DefaultUicontrolForegroundColor');

    % ====================================================================
    % MENUs
    %  create cell array with {menu label, callback, tag}

 %  MENU LABEL                     CALLBACK                      TAG
mc={ 
 getString(message('signal:sptoolgui:File'))                              ' '                        'filemenu'
 ['>' 'Pa&ge Setup' '...']                   'printcomp(''pgepos'')'    'pagepos'
 ['>' 'Print Pre&view' '...']                'printcomp(''prev'')'      'printprev'
 ['>&' getString(message('signal:sptoolgui:Print')) '...']                        'printcomp(''prnt'')'      'prnt'
 '>---'                              ' '                        ' '
 ['>&' getString(message('signal:sptoolgui:Close')) '^w']                         'close'                    'closemenu'
 ['&' getString(message('signal:sptoolgui:Window'))]                           winmenu('callback')        'winmenu'    };
 
% 'Options'                           ' '                        'optionsmenu'
% '>&Tile Axes...'                    'sbswitch(''fvtile'')'     'tilemenu'

    % Build the cell array string for the Help menu.
    mh = filtview_helpmenu;
	mc = [mc;mh];
    mc = ruler('makemenu',mc);  % add Marker menu

    menu_handles = makemenu(fig, char(mc(:,1)), ...
                            char(mc(:,2)), char(mc(:,3)));
    winmenu(fig)
    
 %   set(menu_handles,'hiddenhandle','on')
 %   ud.ht.reloadmenu = menu_handles(3);
 %   set(menu_handles(3),'enable','off')
    
    % ====================================================================
    % Frames

    f1 = [1 0 sz.bw+6*sz.ffs-1 fp(4)-sz.ih];
    ud.ht.frame1 = uicontrol('Units','pixels',...
              'Style','frame','Position',f1,'Tag','mainframe');

    f2 = [1 fp(4)-sz.ih-1 sz.bw+6*sz.ffs-1 sz.ih+1];
    ud.ht.frame2 = uicontrol('Units','pixels',...
              'Style','frame','Position',f2,'Tag','filtlabelframe');

    pf_height = 8*sz.fus+8.5*sz.uh;
    pf =  [  f1(1)+sz.fus      f1(2)+f1(4)-(sz.lh/2+sz.fus+pf_height) ...
             f1(3)-2*sz.fus    pf_height  ];
    ud.ht.plotsframe = uicontrol('Units','pixels',...
              'Style','frame','Position',pf,'Tag','plotframe');
    freqframeHt = 4*sz.fus+2*sz.uh+2.5*sz.lh;
    ff = [sz.fus pf(2)-freqframeHt-sz.fus-sz.lh/2 4*sz.fus+sz.bw freqframeHt];
    ud.ht.freqframe = uicontrol('Units','pixels',...
              'Style','frame','Tag','freqframe',...
              'Position',ff);

    mf = [pf(1)+sz.fus pf(2)+pf(4)-(sz.lh/2+sz.fus+2*sz.uh+sz.fus)+4 ...
             pf(3)-2*sz.fus  sz.fus+1.5*sz.uh];
    ud.ht.magframe = uicontrol('Units','pixels','Visible',visFrame,...
              'Style','frame','Position',mf,'Tag','magframe');
    phf = [pf(1)+sz.fus pf(2)+pf(4)-(sz.lh/2+2*sz.fus+4*sz.uh+2*sz.fus)+5 ...
             pf(3)-2*sz.fus  sz.fus+1.5*sz.uh];
    ud.ht.phaseframe = uicontrol('Units','pixels','Visible',visFrame,...
              'Style','frame','Position',phf,'Tag','phaseframe');

    fsf = [ff(1)+sz.fus ff(2)+ff(4)-(sz.fus+sz.uh+1.5*sz.lh) ...
           ff(3)-2*sz.fus sz.fus+sz.uh+.5*sz.lh];
    ud.ht.fscaleframe = uicontrol('Units','pixels','Visible',visFrame,...
              'Style','frame','Tag','freqsframe',...
              'Position',fsf);

    frf = [ff(1)+sz.fus ff(2)+1.5*sz.fus ...
           ff(3)-2*sz.fus sz.fus+sz.uh+.5*sz.lh];
    ud.ht.frangeframe = uicontrol('Units','pixels','Visible',visFrame,...
              'Style','frame','Tag','freqrframe',...
              'Position',frf);

   % ud.ht.Fsframe = uicontrol('Units','pixels',...
   %           'Style','frame','Tag','fsframe','Position',...
   %             [sz.fus sz.fus ...
   %              4*sz.fus+sz.bw sz.fus+sz.uh+.5*sz.lh]);

    % ====================================================================
    % Labels

    ud.ht.plotslabel = framelab(ud.ht.plotsframe,'Plots',sz.lfs,sz.lh,'tag','plottext');
    ud.ht.freqlabel = framelab(ud.ht.freqframe,'Frequency Axis',sz.lfs,sz.lh,'tag','freqtext');
    ud.ht.fscalelabel = framelab(ud.ht.fscaleframe,'Scale',sz.lfs,sz.lh,'tag','freqstext');
    ud.ht.frangelabel = framelab(ud.ht.frangeframe,'Range',sz.lfs,sz.lh,'tag','freqrtext');
    
    ud.ht.filterLabel = uicontrol('Style','text',...
                'HorizontalAlignment','left',...
                'Tag','filterLabel',...
                'Position',[sz.fus fp(4)-2-19 sz.bw 19]);
    ud.ht.Fsedit = uicontrol('Style','text',...
                'HorizontalAlignment','left',...
                'Tag','fsbox',...
                'Position',[sz.fus fp(4)-4-2*19 sz.bw 19]);

    [FsStr filtLabelStr] = filtview('filtFsLabelStrs', ...
        ud.prefs,ud.ht,ud.filt);
    set(ud.ht.filterLabel, 'String',filtLabelStr)
    set( ud.ht.Fsedit,'String',FsStr)
    
    % framelab(ud.ht.Fsframe,'Sampling Freq.',sz.lfs,sz.lh,'tag','fstext');

    % ====================================================================
    % Checkboxes
    cb_props = {'Units','pixels',...
              'Style','checkbox','HorizontalAlignment','left'};
 
    checkbox_width = 15; % The checkbox part of the uicontrol

    cb1_pos = [mf(1)+sz.fus mf(2)+sz.uh+sz.fus sz.bw sz.uh];
    ud.ht.cb(1) = uicontrol(cb_props{:},...
        'String',getString(message('signal:sptoolgui:Magnitude')),...
        'Tag','magcheck',...
        'Value',ud.prefs.plots(1),...
        'Callback','filtview(''cb'',1)',...
	'Position',cb1_pos);
    if ~isunix
       label_ext = get(ud.ht.cb(1),'Extent');
       cb1_pos(3) = label_ext(3)+checkbox_width;
       set(ud.ht.cb(1),'Position', cb1_pos);
    end
    
    cb2_pos = [phf(1)+sz.fus phf(2)+sz.uh+sz.fus sz.bw sz.uh];
    ud.ht.cb(2) = uicontrol(cb_props{:},...
        'String',getString(message('signal:sptoolgui:Phase')),...
        'Tag','phasecheck',...
        'Value',ud.prefs.plots(2),...
	'Callback','filtview(''cb'',2)',...
	'Position',cb2_pos);
    if ~isunix
       label_ext = get(ud.ht.cb(2),'Extent');
       cb2_pos(3) = label_ext(3)+checkbox_width;
       set(ud.ht.cb(2),'Position', cb2_pos);
    end    

    ud.ht.cb(3) = uicontrol(cb_props{:},...
       'String',getString(message('signal:sptoolgui:GroupDelay')),...
       'Tag','groupdelay',...
       'Value',ud.prefs.plots(3),...
       'Callback','filtview(''cb'',3)',...
       'Position',[pf(1)+2*sz.fus pf(2)+4*sz.fus+3*sz.uh sz.bw sz.uh]);
    ud.ht.cb(4) = uicontrol(cb_props{:},...
       'String',[getString(message('signal:sptoolgui:Zeros')) ' and ' getString(message('signal:sptoolgui:Poles'))],...
       'Tag','polezero',...
       'Value',ud.prefs.plots(4),...
       'Callback','filtview(''cb'',4)',...
       'Position',[pf(1)+2*sz.fus pf(2)+3*sz.fus+2*sz.uh sz.bw sz.uh]);
    ud.ht.cb(5) = uicontrol(cb_props{:},...
       'String',getString(message('signal:sptoolgui:ImpulseResponse')),...
       'Tag','impresp',...
       'Value',ud.prefs.plots(5),...
       'Callback','filtview(''cb'',5)',...
       'Position',[pf(1)+2*sz.fus pf(2)+2*sz.fus+sz.uh sz.bw sz.uh]);
    ud.ht.cb(6) = uicontrol(cb_props{:},...
       'String',getString(message('signal:sptoolgui:StepResponse')),...
       'Tag','stepresp',...
       'Value',ud.prefs.plots(6),...
       'Callback','filtview(''cb'',6)',...
       'Position',[pf(1)+2*sz.fus pf(2)+sz.fus sz.bw sz.uh]);

    % ====================================================================
    % Popups
    pop_props = {'Units','pixels','BackgroundColor','white',...
              'Style','popup','HorizontalAlignment','left'};

    % Tweak position & size of popups: [horz_pos ver_pos width height]
    if ispc
       popTweak = [0  0 0 0];
    else % isunix
       popTweak = [0 -2 0 0];
    end
  
    switch ud.prefs.magmode
    case 'linear'
       magpopvalue = 1;
    case 'log'
       magpopvalue = 2;
    case 'decibels'
       magpopvalue = 3;
    end
    
    ud.ht.magpop = uicontrol(pop_props{:},...
       'String',{'linear'; 'log'; 'decibels'},...
       'Tag','maglist',...
       'Callback','filtview(''magpop'')',...
       'Value',magpopvalue,...
       'Position',[mf(1:2)+sz.fus+[indentPop 0] sz.bw-indentPop sz.uh]+popTweak);

    switch ud.prefs.phasemode
    case 'degrees'
       phasepopvalue = 1;
    case 'radians'
       phasepopvalue = 2;
    end

    ud.ht.phasepop = uicontrol(pop_props{:},...
       'String',{'degrees'; 'radians'},...
       'Tag','phaselist',...
       'Callback','filtview(''phasepop'')',...
       'Value',phasepopvalue,...
       'Position',[phf(1:2)+sz.fus+[indentPop 0] sz.bw-indentPop sz.uh]+popTweak);
 
    switch ud.prefs.freqscale
    case 'linear'
        fscalevalue = 1;
    case 'log'
        fscalevalue = 2;
    end
    ud.ht.fscalepop = uicontrol(pop_props{:},...
       'String',{'linear'; 'log'},...
       'Tag','freqscale',...
       'Callback','filtview(''fscalepop'')',...
       'Value',fscalevalue,...
       'Position',[fsf(1:2)+sz.fus+[indentPop 0] sz.bw-indentPop sz.uh]+popTweak);

    ud.ht.frangepop = uicontrol(pop_props{:},...
       'String',{'[0,Fs/2]'; '[0,Fs]'; '[-Fs/2,Fs/2]'},...
       'Tag','freqrange',...
       'Callback','filtview(''frangepop'')',...
       'Value',ud.prefs.freqrange,...
       'Position',[frf(1:2)+sz.fus+[indentPop 0] sz.bw-indentPop sz.uh]+popTweak);

    % ====================================================================
    % Create axes:

    ax_props = {
         'Units','pixels',...
         'Box','on',...
         'Parent',fig};

    ud.ht.highlightAxes = axes(ax_props{:},'XTick',[],'YTick',[],'Visible','off');
    % create axes:
    for i=1:6
        ud.ht.a(i) = axes(ax_props{:});
        set(ud.ht.a(i),'ButtonDownFcn',['filtview(''mainaxes_down'',' num2str(i) ')']);
        if i == 4   % Zero-Pole plot
            w = linspace(0,2*pi,201);
            zgrid = line(cos(w),sin(w), ...
                 'Color',get(ud.ht.a(4),'XColor'),...
                'LineStyle',':',...
                'Tag','unitcircle',...
                'Parent',ud.ht.a(4));
        end
    end

    ud.titles = {   getString(message('signal:sptoolgui:Magnitude')) 
                    { getString(message('signal:sptoolgui:Phasedegrees')) getString(message('signal:sptoolgui:Phaseradians')) } 
                    getString(message('signal:sptoolgui:GroupDelay')) 
                    [getString(message('signal:sptoolgui:Zeros')) ' ' '&' ' '  getString(message('signal:sptoolgui:Poles'))] 
                    getString(message('signal:sptoolgui:ImpulseResponse'))
                    getString(message('signal:sptoolgui:StepResponse'))};

    ud.tags =    { 'magaxes'
                   'phaseaxes'
                   'delayaxes'
                   'pzaxes'
                   'impaxes'
                   'stepaxes' };

    ud.xlabels = {  getString(message('signal:sptoolgui:Frequency'))
                    getString(message('signal:sptoolgui:Frequency'))
                    getString(message('signal:sptoolgui:Frequency'))
                    getString(message('signal:sptoolgui:Real'))
                    getString(message('signal:sptoolgui:Time'))
                    getString(message('signal:sptoolgui:Time')) };
  
    ud.ylabels = {  ''
                    ''
                    ''
                    getString(message('signal:sptoolgui:Imaginary'))
                    ''
                    ''};

    th = get(ud.ht.a,'Title');
    set([th{[1 3:6]}],{'String'},ud.titles([1 3:6]))
    switch ud.prefs.phasemode
    case 'degrees'
        set(th{2},'String',ud.titles{2}(1))
    case 'radians'
        set(th{2},'String',ud.titles{2}(2))
    end
    if fscalevalue == 2
        set(ud.ht.a([1 2 3]),'XScale','log')
    end
    if magpopvalue == 2
        set(ud.ht.a(1),'YScale','log')
    end

    set([th{:}],{'Tag'},ud.tags)
    set(ud.ht.a,{'Tag'},ud.tags)
    xh = get(ud.ht.a,'XLabel');
    set([xh{:}],{'String'},ud.xlabels,{'Tag'},ud.tags)
    yh = get(ud.ht.a,'YLabel');
    set([yh{:}],{'String'},ud.ylabels,{'Tag'},ud.tags)

    % ==================================================================
    % Identify Main axes - axes where rulers are focused

    % Let Magnitude plot be the default mainaxes
    mainaxes = ud.ht.a(1);
    set(mainaxes,'HandleVisibility','callback');
    ud.mainaxes = mainaxes;

    % ====================================================================
    % initialize lines to []
    for i = 1:length(ud.filt)
        if ~isstruct(ud.lines)
            ud.lines = filtview('emptyLinesStruct');
        else
            ud.lines(i) = filtview('emptyLinesStruct');
        end
    end

    % ====================================================================
    % Create context menu for lines in plot axes:  TPK 6/5/99
    ud.contextMenu.u = uicontextmenu('Parent',fig);
    ud.contextMenu.pickMenu = uimenu(ud.contextMenu.u);  
    ud.contextMenu.changeName = uimenu(ud.contextMenu.pickMenu);
    ud.contextMenu.Fs = uimenu(ud.contextMenu.pickMenu);
    ud.contextMenu.lineprop = uimenu(ud.contextMenu.pickMenu);

    set(ud.ht.a,'UIContextMenu',ud.contextMenu.u)
    
    % ====================================================================
    % Save userdata structure
    set(fig,'UserData',ud)
    
    % ====================================================================
    % now add toolbar for filter viewer
    % NEW toolbar, using uitoolbar functionality, available as of 5.3:
    ud = create_toolbar(fig);
    set(fig,'UserData',ud)

    set(fig,'ResizeFcn',...
            appstr(get(fig,'ResizeFcn'),'sbswitch(''fvresize'')'))
    set(fig,'WindowButtonMotionFcn',...
          ['sbswitch(''fvmotion'',' num2str(ud.toolnum) ')'])
    set(fig,'HandleVisibility','callback')

    % ====================================================================
    % create legend - changes userdata
    sptlegend(fig,'filtview(''changefocus'')','filtview',1)
    ud = get(fig,'UserData');
    
    % ====================================================================    
    % Do the plots - changes userdata
    filtview('plots',ud.prefs.plots)
    ud = get(fig,'UserData');
  
    % ====================================================================
    % display the rulers on the default plot (magnitude);
    if 1   % ud.prefs.tool.ruler    ALWAYS initialize rulers, turn off after
                                   % if not enabled
        saveRulerFlag = ud.prefs.tool.ruler;
        plotPopVal = 1;       % mag plot is default
        realFilterFlag = (isreal(ud.filt(ud.focusIndex).tf.num) &...
            isreal(ud.filt(ud.focusIndex).tf.den));
        ud.prefs.plottitles = filtview('plotTitles',realFilterFlag);
        set(fig,'UserData',ud)

        popupCallback = 'filtview(''rulerpopup'')';	
        ruler('init',fig,ud.prefs.plottitles,plotPopVal,popupCallback,...
            ud.ht.a,[1 2 3 5 6]);
        ud = get(fig,'UserData');    
    else
        %set([ud.toolbar.zoominx ud.toolbar.zoominy ...
        %     ud.toolbar.zoomoutx ud.toolbar.zoomouty],'enable','off')
    end
    
    ud.focusline = ud.lines(ud.focusIndex).mag; % mag plot is default 
    set(fig,'UserData',ud)

    % ====================================================================
    % Initialize legend; set color and line style of all filters
    ud = get(fig,'UserData');

    for i = 1:length(ud.filt)
        if isempty(ud.filt(i).lineinfo)
            % assign line color and style to each filter
            [ud.filt(i).lineinfo,ud.colorCount] = ...
                nextcolor(ud.colororder,ud.linestyleorder,ud.colorCount);
            % poke back into SPTool
            sptool('import',ud.filt(i))            
        end

        % Set line color & style for the rest of the lines for each filter
        handleCell = struct2cell(ud.lines(i));
        h = [handleCell{:}];
        set(h,'Color',ud.filt(i).lineinfo.color, ...
            'LineStyle',ud.filt(i).lineinfo.linestyle)
    end
    set(fig,'UserData',ud)
    
    sptlegend('setstring',{ud.filt.label},{},fig,0)
    sptlegend('setvalue',ud.focusline,ud.focusIndex,1,fig)
    %set(ud.legend.legendline,'linestyle', ...
    %    ud.filt(ud.focusIndex).lineinfo.linestyle)

    % ====================================================================
    % position axes:
    fvresize(1,fig)
    ruler('resize',1,fig)

    ut = get(ud.toolbar.whatsthis,'Parent');
    tbfillblanks(ut,ud.toolbar.lineprop,ud.toolbar)

    % set ud.limits of 2 visible plots (the default plots, phase and mag)
    ud = get(fig,'UserData');
    ud = filtview('setudlimits',ud,ud.ht.a,[1 2]); 
    set(fig,'UserData',ud)

    % Now highlight the main axes:
    moveRulerFlag = 0;

    filtview('mainaxes_down',1,moveRulerFlag)

    if saveRulerFlag
        ruler('newlimits',fig)
        ruler('newsig',fig)
        ruler('showlines',fig)
    else
        ud=get(fig,'UserData');
        ud.prefs.tool.ruler = 0;
        set(fig,'UserData',ud)
        ruler('close',fig)
    end

    set(fig,'Visible','on')
    set(0,'ShowHiddenHandles',save_shh)

%-------------------------------------------------------------------------------
function ud = create_toolbar(fig)
% CREATE_TOOLBAR Creates the toolbar for the Filter Viewer.
%
ud = get(fig,'UserData');
load siggui_icons
ut=uitoolbar(fig);
ud.toolbar.print = ...
    uipushtool('CData',bmp.print,...
                 'Parent',ut,...
                 'ClickedCallback','printcomp(''prnt'')' ,...
                 'TooltipString',[getString(message('signal:sptoolgui:Print')) '...']);
ud.toolbar.printpreview = ...
    uipushtool('CData',bmp.printpreview,...
                 'Parent',ut,...
                 'ClickedCallback','printcomp(''prev'')' ,...
                 'TooltipString',[getString(message('signal:sptoolgui:PrintPreview')) '...']);
ud.toolbar.mousezoom = ...
    uitoggletool('CData',bmp.mousezoom,...
                 'Separator','on',...
                 'Parent',ut,...
                 'ClickedCallback','sbswitch(''fvzoom'',''mousezoom'')',...
                 'TooltipString',getString(message('signal:sptoolgui:MouseZoom')));
ud.toolbar.fullviewall = ...
    uipushtool('CData',bmp.fullviewall,...
                 'Parent',ut,...
                 'ClickedCallback','sbswitch(''fvzoom'',''zoomout'',''all'')',...
                 'TooltipString',getString(message('signal:sptoolgui:FullViewAllzoomsOutBothAxesAllPlots'))); 
ud.toolbar.fullview = ...
    uipushtool('CData',bmp.fullview,...
                 'Parent',ut,...
                 'ClickedCallback','sbswitch(''fvzoom'',''zoomout'')',...
                 'TooltipString',getString(message('signal:sptoolgui:FullViewzoomsOutBothAxesSelectedPlot'))); 
ud.toolbar.zoominy = ...
    uipushtool('CData',bmp.zoominy,...
                 'Parent',ut,...
                 'ClickedCallback','sbswitch(''fvzoom'',''zoominy'')',...
                 'TooltipString',getString(message('signal:sptoolgui:ZoomInYverticalAxis')));
ud.toolbar.zoomouty = ...
    uipushtool('CData',bmp.zoomouty,...
                 'Parent',ut,...
                 'ClickedCallback','sbswitch(''fvzoom'',''zoomouty'')',...
                 'TooltipString',getString(message('signal:sptoolgui:ZoomOutYverticalAxis')));
ud.toolbar.zoominx = ...
    uipushtool('CData',bmp.zoominx,...
                 'Parent',ut,...
                 'ClickedCallback','sbswitch(''fvzoom'',''zoominx'')',...
                 'TooltipString',getString(message('signal:sptoolgui:ZoomInXhorizontalAxis')));
ud.toolbar.zoomoutx = ...
    uipushtool('CData',bmp.zoomoutx,...
                 'Parent',ut,...
                 'ClickedCallback','sbswitch(''fvzoom'',''zoomoutx'')',...
                 'TooltipString',getString(message('signal:sptoolgui:ZoomOutXhorizontalAxis')));
ud.toolbar.select = ...
    uipushtool('CData',bmp.select,...
                 'Parent',ut,...
                 'Separator','on',...
                 'ClickedCallback','sbswitch(''sptlegend'',''popup'')' ,...
                 'TooltipString',[getString(message('signal:sptoolgui:SelectSignal')) '...']);
ud.toolbar.lineprop = ...
    uipushtool('CData',bmp.lineprop,...
                 'Parent',ut,...
                 'ClickedCallback','sbswitch(''sptlegend'',''button'')' ,...
                 'TooltipString',[getString(message('signal:sptoolgui:LineProperties')) '...']);
ud.toolbar = ruler('toolbarbuttons',ud.toolbar,ut,bmp);

ud.toolbar.whatsthis = ...
    uitoggletool('CData',bmp.whatsthis,...
                 'Parent',ut,...
                 'Separator','on',...
                 'ClickedCallback','sbswitch(''filtview'',''help'')' ,...
                 'TooltipString',getString(message('signal:sptoolgui:WhatsThisHelp')));

%------------------------------------------------------------------------------
function mh = filtview_helpmenu
% Set up a string cell array that can be passed to makemenu to create the Help
% menu for the Filter Viewer.
  
% Define specifics for the Help menu in Filter Viewer.
toolname      = getString(message('signal:sptoolgui:FilterViewer'));
toolhelp_cb   = 'filtview(''help'',''topics'')';
toolhelp_tag  = 'helptopicsmenu';
whatsthis_cb  = 'filtview(''help'',''whatsthis'')';
whatsthis_tag = 'whatsthismenu';

% Add other Help menu choices that are common to all SPTool clients.
mh=sptool_helpmenu(toolname,toolhelp_cb,toolhelp_tag,whatsthis_cb,whatsthis_tag);

% [EOF] fvinit.m
