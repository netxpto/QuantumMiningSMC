function spinit(SPTfig)
%SPINIT Initialization for SPECTVIEW (Spectrum Viewer Client)

%   Copyright 1988-2012 The MathWorks, Inc.
%   Carlos Ferreira Ribeiro
%   Prof. Armando Nolasco Pinto
%   January, 2016
%   IT, Aveiro.

shh = get(0,'ShowHiddenHandles');
set(0,'ShowHiddenHandles','on');

if nargin < 2
    SPTfig = findobj(0,'tag','sptool');
end

% ====================================================================
% set defaults and initialize userdata structure

    rulerPrefs = sptool('getprefs','ruler',SPTfig);
    colorPrefs = sptool('getprefs','color',SPTfig);
    spectviewPrefs = sptool('getprefs','spectview',SPTfig);

    ud.prefs.tool.ruler = spectviewPrefs.rulerEnable;    % rulers enabled
    ud.prefs.tool.zoompersist = spectviewPrefs.zoomFlag; % is zoom mode persistent or
       % does it go away when you zoom once?

    ud.prefs.minsize = [150 378]; 
      % minsize(1)   - minimum width of main axes in pixels
      % minsize(2)   - minimum height of figure in pixels

    ud.prefs.colororder = colorPrefs.colorOrder;
    ud.prefs.linestyleorder = colorPrefs.linestyleOrder;
    
    ud.prefs.xaxis.label = getString(message('signal:sptoolgui:Frequency'));
    ud.prefs.xaxis.grid = 1;
    ud.prefs.yaxis.label = '';
    ud.prefs.yaxis.grid = 1;

    ud.prefs.title.mode = 'auto';  % can be 'auto' or 'manual'
    ud.prefs.title.manualstring = '';  % title string in case mode is manual

    markerStr = { '+' 'o' '*' '.' 'x' ...
         'square' 'diamond' 'v' '^' '>' '<' 'pentagram' 'hexagram' }';
    typeStr = {'vertical' 'horizontal' 'track' 'slope'}';
    
    ud.prefs.ruler.color = rulerPrefs.rulerColor;
    ud.prefs.ruler.marker = markerStr{rulerPrefs.rulerMarker};
    ud.prefs.ruler.markersize = rulerPrefs.markerSize;
    ud.prefs.ruler.type = typeStr{rulerPrefs.initialType};
    ud.prefs.rulerPrefsPanelName = 'spectview'; %ruler needs to know this so
       % it can update the SPTOOL preferences when it is turned on/off

    ud.sz = sptsizes;

    ud.spect = [];  % structure array of currently visible spectra
    ud.lines = [];
    ud.patches = {};
    ud.linecache.h = [];  % no line objects defined yet
    ud.SPToolIndices = [];
    ud.focusIndex = [];
    ud.focusline = [];
    ud.colorCount = 0;  % number of colors allocated thus far
    ud.colororder = num2cell(evalin('base',ud.prefs.colororder),2);
    ud.linestyleorder = num2cell(evalin('base',ud.prefs.linestyleorder),2);

    ud.pointer = 0;  % == -1 watch, 0 arrow/drag indicators, 1 zoom,
                     %     2 help

    ud.tabfig = [];  % handle to settings dialog box figure

    ud.limits.xlim = [0 1];
    ud.limits.ylim = [0 1];

    ud.justzoom = [ 0 0 ] ;
    
    switch computer
    case 'MAC2'
        ud.maxPopupEntries = 300;
    otherwise
        % most platforms don't scroll beyond screen
        ud.maxPopupEntries = 24;
    end
    ud.inheritList = [];  % see local fcn newInheritString (spectview) for
                          % explanation
    
    screensize = get(0,'ScreenSize');
    fp = get(0,'DefaultFigurePosition');
    ud.left_width = 225;
    
    %fw = fp(3)+ud.prefs.tool.ruler*ud.sz.rw+ud.left_width; % figure width
    fw = 800;  
    fw = min(fw,screensize(3)-50);
    fh = ud.prefs.minsize(2);
    fp = [fp(1)-(fw-fp(3))/2 fp(2)+fp(4)-fh fw fh];
% CREATE FIGURE

figname = getString(message('signal:sptoolgui:SpectrumViewer'));
fig = figure('CreateFcn','',...
         'CloseRequestFcn','sbswitch(''spectview'',''SPTclose'')',...
         'WindowStyle','normal',...
         'DockControls','off',...
         'Tag','spectview',...
         'NumberTitle','off',...
         'IntegerHandle','off',...
         'HandleVisibility','callback',...
         'MenuBar','none',...
         'Position',fp,...
         'UserData',ud,...
         'InvertHardcopy','off',...
         'PaperPositionMode','auto',...
         'Units','pixels',...
         'Visible','off',...
         'Name',figname,...
         'ResizeFcn','sbswitch(''resizedispatch'')%');
         
set(fig,'Renderer','painters') %make sure this is manually set to prevent
                               % going into zbuffer when creating confidence
                               % patches
                               
    % ====================================================================
    % MENUs
    %  create cell array with {menu label, callback, tag}

 %  MENU LABEL                     CALLBACK                      TAG
mc={ 
 getString(message('signal:sptoolgui:File'))                              ' '                     'filemenu'
 ['>' getString(message('signal:sptoolgui:PrintSetup')) '...']                   'printcomp(''prntsetup'')' 'prntsetup'
 ['>' getString(message('signal:sptoolgui:PrintPreview2')) '...']                'printcomp(''prev'')'   'printprev'
 ['>' getString(message('signal:sptoolgui:Print')) '...']                        'printcomp(''prnt'')'   'prnt'
 '>---'                              ' '                     ' '
 ['>' getString(message('signal:sptoolgui:Close')) '^w']                         'spectview(''SPTclose'')'  'closemenu'
 getString(message('signal:sptoolgui:Options'))                           ' '                        'optionsmenu'                
 ['>' getString(message('signal:sptoolgui:MagnitudeScale'))]                 ' '                        'magscale'
 ['>>' getString(message('signal:sptoolgui:decibels'))]                       'spectview(''magscale'',''db'')'   'dbMag'
 ['>>' getString(message('signal:sptoolgui:Linear'))]                         'spectview(''magscale'',''lin'')' 'linearMag'
 ['>' getString(message('signal:sptoolgui:FrequencyRange'))]                 ' '                        'freqrange'
 '>>&[0, Fs/2]'                      'spectview(''freqrange'',''half'')' 'half'
 '>>&[0, Fs]'                        'spectview(''freqrange'',''whole'')' 'whole'
 '>>&[-Fs/2, Fs/2]'                  'spectview(''freqrange'',''neg'')' 'negative'
 ['>' getString(message('signal:sptoolgui:FrequencyScale'))]                 ' '                        'freqscale'
 ['>>' getString(message('signal:sptoolgui:Linear'))]                         'spectview(''freqscale'',''lin'')' 'linearFreq'
 ['>>' getString(message('signal:sptoolgui:Log'))]                            'spectview(''freqscale'',''log'')' 'logFreq'};

    mc = ruler('makemenu',mc);  % add Marker menu
 
    menu_handles = makemenu(fig, char(mc(:,1)), ...
                            char(mc(:,2)), char(mc(:,3)));
   
    matlab.ui.internal.createWinMenu(fig);
                            
    ud.hand.magscaleMenu = menu_handles(8:9);    % Numbers denote which menu items
    ud.hand.freqrangeMenu = menu_handles(11:13); % can be checked.
    ud.hand.freqscaleMenu = menu_handles(15:16);

    set(ud.hand.magscaleMenu(spectviewPrefs.magscale),'Checked','on')
    set(ud.hand.freqscaleMenu(spectviewPrefs.freqscale),'Checked','on')
    set(ud.hand.freqrangeMenu(spectviewPrefs.freqrange),'Checked','on')
    
    % Help menu.
    mh = spectview_helpmenu;
    menu_handles = [menu_handles;makemenu(fig, char(mh(:,1)), ...
                            char(mh(:,2)), char(mh(:,3)))];

    set(menu_handles,'HandleVisibility','callback')
    
    % ====================================================================
    % Create Main axes
    mainaxes = axes('Units','pixels',...
         'Box','on',...
         'HandleVisibility','callback', ...
         'Tag','mainaxes',...
         'ButtonDownFcn','spectview(''mainaxes_down'')');
    % create a copy that will be underneath the main axes, and
    % will be used as a border during panning operations to prevent
    % background erasemode from clobbering the main axes plot box.
    temp = copyobj(mainaxes,fig);
    mainaxes_border = mainaxes;
    mainaxes = temp;
    
    % copyobj does not copy callbacks.
    set(mainaxes,'ButtonDownFcn','spectview(''mainaxes_down'')');

    set(mainaxes_border,'XTick',[],'YTick',[],'Visible','off',...
          'Tag','mainaxes_border')

    set(get(mainaxes,'Title'),'FontAngle',  get(mainaxes, 'FontAngle'), ...
        'FontName',   get(mainaxes, 'FontName'), ...
        'FontSize',   get(mainaxes, 'FontSize'), ...
        'FontWeight', get(mainaxes, 'FontWeight'), ...
        'Color',get(mainaxes,'XColor'),...
        'Tag','mainaxestitle',...
        'Interpreter','none')
    set(get(mainaxes,'XLabel'),'String',ud.prefs.xaxis.label,...
         'Tag','mainaxesxlabel')
    set(get(mainaxes,'YLabel'),'String',ud.prefs.yaxis.label,...
         'Tag','mainaxesylabel')
    if (spectviewPrefs.freqscale == 2)
        set(mainaxes,'XScale','log')
    end     
    ud.mainaxes = mainaxes;
    ud.mainaxes_border = mainaxes_border;
    
    % ====================================================================
    % Create frames and uicontrols for property editor
    frame_props = {'Units','pixels','Style','frame'};
    
    ud.hand.propFrame = uicontrol(frame_props{:},'Tag','propFrame');
    ud.hand.signalFrame = uicontrol(frame_props{:},'Tag','signalFrame');
    ud.hand.paramFrame = uicontrol(frame_props{:},'Tag','paramFrame');

    text_props = {'Units','pixels','Style','text'};
    % ud.hand.propLabel = uicontrol(text_props{:},'String','',...
    %     'fontsize',12,'Tag','propLabel');
    ud.hand.propLabel = get(ud.mainaxes,'Title');
    ud.hand.signalLabel = uicontrol(text_props{:},'String',getString(message('signal:sptoolgui:Signal')),...
          'Tag','signalLabel');
    ud.hand.siginfo1Label = uicontrol(text_props{:},'String','<None>',...
          'HorizontalAlignment','left','Tag','siginfo1Label');
    ud.hand.siginfo2Label = uicontrol(text_props{:},'String','',...
          'HorizontalAlignment','left','Tag','siginfo2Label');
    ud.hand.paramLabel = uicontrol(text_props{:},'String',getString(message('signal:sptoolgui:Parameters')),...
          'Tag','paramLabel');
    
    label_props = {text_props{:}, 'horizontalalignment','right' };
    
    ud.hand.revertButton = uicontrol('Style','pushbutton',...
            'Units','pixels',...
            'String',getString(message('signal:sptoolgui:Revert')),'Callback','spectview(''revert'')',...
            'Enable','off','Tag','revertButton');
    ud.hand.applyButton = uicontrol('Style','pushbutton',...
            'Units','pixels',...
            'String',getString(message('signal:sptoolgui:Apply')),'Callback','spectview(''apply'')',...
            'Enable','off','Tag','applyButton');

    % Get methods structure.
    ud.methods = [];
    ud.methods = spmethp(ud.methods); % calls one in signal/private
    % now call each one found on path:
    p = sptool('getprefs','plugins',SPTfig);
    if p.plugFlag
        ud.methods = sptool('callall','spmeth',ud.methods);
    end
    
    ud.hand.methodLabel = uicontrol(label_props{:},'String',getString(message('signal:sptoolgui:Method')),...
           'Tag','methodLabel');         
    %MethodNamelbls = getTranslatedString('signal:sigtools:siggui',{ud.methods.methodName});   
    ud.hand.methodPopup = uicontrol('Style','popupmenu',...
            'BackgroundColor','white',...
            'Units','pixels',...
            'String',{ud.methods.methodName},... % 'String',MethodNamelbls,...
            'Tag','methodPopup',...
            'Callback','spectview(''changeMethod'')');
            
    edit_props = {'style','edit', 'horizontalalignment','left',...
                  'Units','pixels',...
                  'backgroundcolor','white' };
    
    ud.hand.confidenceCheckbox = uicontrol('Style','checkbox',...
            'Units','pixels',...
            'Value',0,'String',getString(message('signal:sptoolgui:ConfInt')),...
            'Callback','spectview(''confidence'',''check'')',...
            'Tag','confidenceCheckbox');
    ud.hand.confidenceEdit = uicontrol(edit_props{:},...
            'Units','pixels',...
            'String','.95',...
            'Tag','confidenceEdit',...
            'Callback','spectview(''confidence'',''edit'')');
            
    ud.hand.inheritPopup = uicontrol('Style','popupmenu',...
            'BackgroundColor','white',...
            'Units','pixels',...
            'String',{getString(message('signal:sptoolgui:InheritFrom'))},...
            'Tag','inheritPopup',...
            'Callback','spectview(''inherit'')');
    
    N = 9;  % maximum number of parameters
    for i=1:N
        ud.hand.label(i) = uicontrol(label_props{:},...
            'Tag',['label' num2str(i)]);
        ud.hand.uicontrol(i) = uicontrol(edit_props{:},...
            'Callback',['spectview(''paramChange'',' sprintf('%.9g',i) ')'],...
            'Tag',['uicontrol' num2str(i)]);
    end

    % ====================================================================
    % Create context menu for lines in main axes:  TPK 6/26/99
    ud.contextMenu.u = uicontextmenu('Parent',fig,'Tag','spContextMenu');
    ud.contextMenu.pickMenu = uimenu(ud.contextMenu.u);  
    ud.contextMenu.changeName = uimenu(ud.contextMenu.pickMenu);
    ud.contextMenu.Fs = uimenu(ud.contextMenu.pickMenu);
    ud.contextMenu.lineprop = uimenu(ud.contextMenu.pickMenu);
    set(ud.mainaxes,'UIContextMenu',ud.contextMenu.u)
    % save ud before calling toolbar since fillparams accesses it
    set(fig,'UserData',ud)
    
    confid.enable = get(ud.hand.confidenceCheckbox,'Value');
    confid.level = get(ud.hand.confidenceCheckbox,'String');
    
    % Make 'Welch' default PSD method
    %choice = 'welch';  % (see "case 'linedown'" in spectview.m)
    % Make '2-sided Welch' default PSD method
    choice = '2-sided welch'; % (see "case 'linedown'" in spectview.m)
    
    % Select choice if it is available:
    defMethNum = find(strcmp(choice,lower({ud.methods.methodName})));
    if isempty(defMethNum)
        defMethNum = 1;  % Use first method listed in spmethop.m
    end

    spectview('fillParams',fig,ud.methods(defMethNum).methodName,...
               ud.methods(defMethNum).default,confid)
    
    % ====================================================================
    % now add toolbar
    % NEW toolbar, using uitoolbar functionality, available as of 5.3:  
    %    - T. Krauss, 6/26/99
    ut=uitoolbar(fig);
    load siggui_icons
    ud.toolbar.print = ...
        uipushtool('CData',bmp.print,...
                     'Parent',ut,...
                     'ClickedCallback','printcomp(''prnt'')' ,...
                     'Tag', 'spPrint', ...
                     'TooltipString',[getString(message('signal:sptoolgui:Print')) '...']);
    ud.toolbar.printpreview = ...
        uipushtool('CData',bmp.printpreview,...
                     'Parent',ut,...
                     'ClickedCallback','printcomp(''prev'')' ,...
                     'Tag', 'spPrintPreview', ...
                     'TooltipString',[getString(message('signal:sptoolgui:PrintPreview')) '...']);
    ud.toolbar.mousezoom = ...
        uitoggletool('CData',bmp.mousezoom,...
                     'Separator','on',...
                     'Parent',ut,...
                     'ClickedCallback','sbswitch(''spzoom'',''mousezoom'')',...
                     'Tag', 'spMouseZoom', ...
                     'TooltipString',getString(message('signal:sptoolgui:MouseZoom')));
    ud.toolbar.fullview = ...
        uipushtool('CData',bmp.fullview,...
                     'Parent',ut,...
                     'ClickedCallback','sbswitch(''spzoom'',''zoomout'')',...
                     'Tag', 'spFullView', ...
                     'TooltipString',getString(message('signal:sptoolgui:FullViewzoomsOutBothAxes'))); 
    ud.toolbar.zoominy = ...
        uipushtool('CData',bmp.zoominy,...
                     'Parent',ut,...
                     'ClickedCallback','sbswitch(''spzoom'',''zoominy'')',...
                     'Tag', 'spZoomInY', ...
                     'TooltipString',getString(message('signal:sptoolgui:ZoomInYverticalAxis')));
    ud.toolbar.zoomouty = ...
        uipushtool('CData',bmp.zoomouty,...
                     'Parent',ut,...
                     'ClickedCallback','sbswitch(''spzoom'',''zoomouty'')',...
                     'Tag', 'spZoomOutY', ...
                     'TooltipString',getString(message('signal:sptoolgui:ZoomOutYverticalAxis')));
    ud.toolbar.zoominx = ...
        uipushtool('CData',bmp.zoominx,...
                     'Parent',ut,...
                     'ClickedCallback','sbswitch(''spzoom'',''zoominx'')',...
                     'Tag', 'spZoomInX', ...
                     'TooltipString',getString(message('signal:sptoolgui:ZoomInXhorizontalAxis')));
    ud.toolbar.zoomoutx = ...
        uipushtool('CData',bmp.zoomoutx,...
                     'Parent',ut,...
                     'ClickedCallback','sbswitch(''spzoom'',''zoomoutx'')',...
                     'Tag', 'spZoomOutX', ...
                     'TooltipString',getString(message('signal:sptoolgui:ZoomOutXhorizontalAxis')));
    ud.toolbar.select = ...
        uipushtool('CData',bmp.select,...
                     'Parent',ut,...
                     'Separator','on',...
                     'ClickedCallback','sbswitch(''sptlegend'',''popup'')' ,...
                     'Tag', 'spSelect', ...
                     'TooltipString',[getString(message('signal:sptoolgui:SelectSignal')) '...']);
    ud.toolbar.lineprop = ...
        uipushtool('CData',bmp.lineprop,...
                     'Parent',ut,...
                     'ClickedCallback','sbswitch(''sptlegend'',''button'')' ,...
                     'Tag', 'spLineProp', ...
                     'TooltipString',[getString(message('signal:sptoolgui:LineProperties')) '...']);
 
    ud.toolbar = ruler('toolbarbuttons',ud.toolbar,ut,bmp);
 
    ud.toolbar.whatsthis = ...
        uitoggletool('CData',bmp.whatsthis,...
                     'Parent',ut,...
                     'Separator','on',...
                     'ClickedCallback','sbswitch(''spectview'',''help'')' ,...
                     'Tag', 'spWhatsThis', ...
                     'TooltipString',getString(message('signal:sptoolgui:WhatsThisHelp')));

    set(fig,'UserData',ud)
            
    % create legend - changes userdata
    sptlegend(fig,'spectview(''changefocus'')','spectview')

    set(fig,'ResizeFcn',...
            appstr(get(fig,'ResizeFcn'),'sbswitch(''spresize'')'))
            
    set(fig,'WindowButtonMotionFcn','sbswitch(''spmotion'')')

    spresize(0,fig)
    
    if ud.prefs.tool.ruler
       ruler
       ud = get(fig,'UserData');
       ud.ruler.evenlySpaced = 0;
       set(fig,'UserData',ud)
    end

    set(fig,'Visible','on')
    set(0,'ShowHiddenHandles',shh);

	%------------------------------------------------------------------------------
function mh = spectview_helpmenu
% Set up a string cell array that can be passed to makemenu to create the Help
% menu for the Spectrum Viewer.
  
% Define specifics for the Help menu in Spectrum Viewer.
toolname      = getString(message('signal:sptoolgui:SpectrumViewer'));
toolhelp_cb   = 'spectview(''help'',''topics'')';
toolhelp_tag  = 'helptopicsmenu';
whatsthis_cb  = 'spectview(''help'',''whatsthis'')';
whatsthis_tag = 'whatsthismenu';

% Add other Help menu choices that are common to all SPTool clients.
mh=sptool_helpmenu(toolname,toolhelp_cb,toolhelp_tag,whatsthis_cb,whatsthis_tag);

% [EOF] spinit.m
