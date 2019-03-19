function varargout = sptprefs(varargin)
%SPTPREFS  Preferences dialog box for SPTool.
%  prefs = sptprefs(prefs)

%   Copyright 1988-2012 The MathWorks, Inc.

if ~isstr(varargin{1})
    action = 'init';
    prefs = varargin{1};
else
    action = varargin{1};
end

switch action
case 'init'

    SPTfig = gcf;

    if nargin > 1
       panelInd = varargin{2};
    else
       panelInd = 1;
    end
    
    figname = getString(message('signal:sptoolgui:PreferencesForSPTool'));
    okstring = getString(message('signal:sptoolgui:OK'));
    cancelstring = getString(message('signal:sptoolgui:Cancel'));
    fus = 5;  % frame / uicontrol spacing
    ffs = 8;
    uh = 24;  % uicontrol height
    listw = 175;  % list box width
    lw = 170; % label width
    bw = 110; % button width
    bw1 = 90; % button width for Cancel and OK
    uw = 250; % uicontrol width
    lfs = 5;  % label / frame spacing
    lbs = 3;  % label / box spacing
    lh = 16;  % label height
    
    fp = get(0,'DefaultFigurePosition');
    w = 2*ffs + listw + fus + 2*fus + lbs + lw + uw;
    h = 8*(fus+uh) + lh + 2*fus + uh;
    fp = [15 fp(2)+fp(4)-h w h];  % keep top of window fixed

    fig_props = { ...
       'Name'                   figname ...
       'Resize'                 'off' ...
       'NumberTitle'            'off' ...
       'WindowStyle'            'modal' ... 
       'CreateFcn'              ''    ...
       'Position'               fp   ...
       'CloseRequestFcn'        'sbswitch(''sptprefs'',''cancel'')' ...
       'Color'                  get(0,'DefaultUicontrolBackgroundColor') ...
       'Units'                  'pixels' ...
       'DockControls',          'off',...
       'HandleVisibility'       'callback'};

    fig = figure(fig_props{:});
    
    cancel_btn = uicontrol('Style','pushbutton',...
      'Units','pixels',...
      'String',cancelstring,...
      'Position',[fp(3)-ffs-bw1 ffs bw1 uh],...
      'Tag','cancelButton',...
      'Callback','sbswitch(''sptprefs'',''cancel'')');
    ud.ok_btn = uicontrol('Style','pushbutton',...
      'Units','pixels',...
      'String',okstring,...
      'Enable','off',...
      'Tag','okButton',...
      'Position',[fp(3)-ffs-bw1-fus-bw1 ffs bw1 uh],...
      'Callback','applyfilt(''ok'')');
    
    ud.list = uicontrol('Style','listbox',...
      'Units','pixels',...
      'String',{prefs.panelDescription},...
      'Value',panelInd,...
      'Tag','listbox',...
      'Position',[ffs ffs+fus+uh listw fp(4)-2*ffs-fus-uh],...
      'Callback','sbswitch(''sptprefs'',''list'')',...
      'BackgroundColor','w');
      
    ud.helpButton = uicontrol('Style','pushbutton',...
      'Units','pixels',...
      'Tag','helpButton',...
      'String',[getString(message('signal:sptoolgui:Help')) '...'],...
      'Position',[ffs ffs listw uh],...
      'Callback','sbswitch(''sptprefs'',''help'')');
    
    ud.prefs = prefs;
    
    ud.factory = uicontrol('Style','pushbutton',...
      'Units','pixels',...
      'String',getString(message('signal:sptoolgui:FactorySettings')),...
      'Tag','factoryButton',...
      'Position',[ffs+fus+listw ffs bw uh],...
      'Callback','sbswitch(''sptprefs'',''factory'')');
    ud.revert = uicontrol('Style','pushbutton',...
      'Units','pixels',...
      'String',getString(message('signal:sptoolgui:RevertPanel')),...
      'Tag','revertButton',...
      'Position',[ffs+fus+listw+fus+bw ffs bw uh],...
      'Callback','sbswitch(''sptprefs'',''revert'')');
          
   
    pf_pos = [ffs+fus+listw 2*fus+uh 2*fus+lbs+lw+uw 8*(uh+fus)+lh/2];
    ud.panelFrame = uicontrol('Style','frame',...
      'Units','pixels',...
      'Tag','panelFrame',...
      'Position',pf_pos);
    ud.panelLabel = uicontrol('Style','text',...
      'Units','pixels',...
      'Tag','panelLabel',...
      'Position',[pf_pos(1)+10 pf_pos(2)+pf_pos(4)-lh/2 lw lh],...
      'String',ud.prefs(panelInd).panelDescription);
    minwidthLeft(ud.panelLabel)
    
    ud.panelControls = cell(length(prefs),1);
    
    ud.panelControls{panelInd} = createPanel(prefs(panelInd),pf_pos,...
         fus,lbs,lw,lh,uw,uh);
    
    ud.whichControl = [];
    ud.changedFlags = zeros(length(ud.prefs),1);
    ud.flag = '';
    set(fig,'UserData',ud)
    setEnable(panelInd,ud.prefs,ud.panelControls,ud.factory,ud.revert)        
%   Added for using FDATool
%   In the case that filter  panel is opened in front, enable/disable accordingly       
    if isequal(panelInd, 5) 
        if nargin == 3 && varargin{3} == 1
            filterPanelEnable(panelInd, ud);
        else
            set([ud.panelControls{panelInd}{:}], 'Enable', 'off');
        end
    end
%   Added for using SignalBrowser
    if isequal(panelInd, 3) 
        if nargin == 3 && varargin{3} == 1
            sigbrowsePanelEnable(panelInd, ud);
        else
            set([ud.panelControls{panelInd}{:}], 'Enable', 'off');
        end
    end          
    
    setEnable(panelInd,ud.prefs,ud.panelControls,ud.factory,ud.revert);
%     jsun - also disable factory button if filter designer/fdatool window is
%     open. Otherwise, factory may change to using different filter
%     designer
    if isequal(panelInd, 5) 
        if nargin == 3 && varargin{3} == 0            
            set(ud.factory, 'Enable', 'off');
        end
    end      
    
    done = 0;
    while ~done
        waitfor(fig,'userdata')

        ud = get(fig,'UserData');
       
        switch ud.flag
        case 'list'
            oldPanelInd = panelInd;
            panelInd = get(ud.list,'Value');
           
            if panelInd ~= oldPanelInd
               % change label
               set(ud.panelLabel,'String',ud.prefs(panelInd).panelDescription);
               minwidthLeft(ud.panelLabel)

               % hide old uicontrols
               set([ud.panelControls{oldPanelInd}{:}],'Visible','off')
               
               % show or create uicontrols for this panel
               if isempty(ud.panelControls{panelInd})
                   ud.panelControls{panelInd} = createPanel(prefs(panelInd),...
                       pf_pos,fus,lbs,lw,lh,uw,uh);
               else
                    set([ud.panelControls{panelInd}{:}],'Visible','on')
               end
%             Added for using FDATool               
%             disable the controls related to Filter Designer is FDATool is
%             selected. Not the best way but a way!               
               if isequal(panelInd, 5) 
                   if nargin == 3 && varargin{3} == 1
                        filterPanelEnable(panelInd, ud);
                   else
                       set([ud.panelControls{panelInd}{:}], 'Enable', 'off');
                   end
               end
%             Added for using SignalBrowser
               if isequal(panelInd, 3) 
                   if nargin == 3 && varargin{3} == 1
                        sigbrowsePanelEnable(panelInd, ud);
                   else
                       set([ud.panelControls{panelInd}{:}], 'Enable', 'off');
                   end
               end                     
               setEnable(panelInd,ud.prefs,ud.panelControls,ud.factory,ud.revert);
            %     jsun - also disable factory button if filter designer/fdatool window is
            %     open. Otherwise, factory may change to using different filter
            %     designer
               if isequal(panelInd, 5) 
                   if nargin == 3 && varargin{3} == 0
                       set(ud.factory, 'Enable', 'off');
                   end
               end      
               
            end
        case 'change'            
            % if this is a radio button, set values of all visible 
            % radiobuttons with the same tag.
            if strcmp('radiobutton',...
                 get(ud.panelControls{panelInd}{ud.whichControl}(1),'Style'));
                radioTag = ud.prefs(panelInd).controls{ud.whichControl,4};
                if ~isempty(radioTag)
                    u = findobj('style','radiobutton','userdata',radioTag,...
                        'visible','on');
                    set(u,'Value',0)
                    set(ud.panelControls{panelInd}{ud.whichControl},'Value',1)
                end
            end
%           Added for using FDATool            
%           disable the controls related to Filter Designer is FDATool is
%           selected. Not the best way but a way!
            if isequal(panelInd, 5)
                filterPanelEnable(panelInd, ud);                      
            end
%           Added for using SignalBrowser
            if isequal(panelInd, 3)
                sigbrowsePanelEnable(panelInd, ud);                      
            end            
            % check to see if current values are factory or original,
            %  and set enable property of Factory Settings and Revert Panel
            %  buttons appropriately.
            
            setEnable(panelInd,ud.prefs,ud.panelControls,ud.factory,ud.revert)

        case 'revert'
        % revert panel button pushed
            setPanel(ud.panelControls{panelInd},ud.prefs(panelInd).currentValue)
%             setEnable(panelInd,ud.prefs,ud.panelControls,ud.factory,ud.revert)
%             Added for using FDATool               
%             disable the controls related to Filter Designer is FDATool is
%             selected. Not the best way but a way!               
               if isequal(panelInd, 5) 
                   if nargin == 3 && varargin{3} == 1
                        filterPanelEnable(panelInd, ud);
                   else
                       set([ud.panelControls{panelInd}{:}], 'Enable', 'off');
                   end
               end      
               setEnable(panelInd,ud.prefs,ud.panelControls,ud.factory,ud.revert);
            %     jsun - also disable factory button if filter designer/fdatool window is
            %     open. Otherwise, factory may change to using different filter
            %     designer
               if isequal(panelInd, 5) 
                   if nargin == 3 && varargin{3} == 0
                       set(ud.factory, 'Enable', 'off');
                   end
               end               
        case 'factory'
        % factory button pushed
            setPanel(ud.panelControls{panelInd},ud.prefs(panelInd).controls(:,7))
%             setEnable(panelInd,ud.prefs,ud.panelControls,ud.factory,ud.revert)
            
%             Added for using FDATool               
%             disable the controls related to Filter Designer is FDATool is
%             selected. Not the best way but a way!               
               if isequal(panelInd, 5) 
                   if nargin == 3 && varargin{3} == 1
                        filterPanelEnable(panelInd, ud);
                   else
                       set([ud.panelControls{panelInd}{:}], 'Enable', 'off');
                   end
               end   
%              Added for using SignalBrowser
               if isequal(panelInd, 3) 
                   if nargin == 3 && varargin{3} == 1
                        sigbrowsePanelEnable(panelInd, ud);
                   else
                       set([ud.panelControls{panelInd}{:}], 'Enable', 'off');
                   end
               end                                    
               setEnable(panelInd,ud.prefs,ud.panelControls,ud.factory,ud.revert);
            %     jsun - also disable factory button if filter designer/fdatool window is
            %     open. Otherwise, factory may change to using different filter
            %     designer
               if isequal(panelInd, 5) 
                   if nargin == 3 && varargin{3} == 0
                       set(ud.factory, 'Enable', 'off');
                   end
               end                  
        case 'ok'
            newprefs = ud.prefs;
            err = 0;
            for i=1:length(ud.prefs)
                if ~isempty(ud.panelControls{i})
                    newprefs(i).currentValue = getPanel(ud.panelControls{i});
                end
                
                if ~isequal(newprefs(i).currentValue,ud.prefs(i).currentValue)
                    p = cell2struct(newprefs(i).currentValue,...
                                    ud.prefs(i).controls(:,1));
                    for j=1:length(ud.prefs(i).clientList)
                        errstr = feval(ud.prefs(i).clientList{j},'setprefs',...
                           ud.prefs(i).panelName,p);
                        err = length(errstr)>0;
                        if err
                            break
                        end
                    end
                    if err
                        break
                    end
                    ud.prefs(i).currentValue = newprefs(i).currentValue;
                    sptool('setprefs',ud.prefs(i).panelName,p,SPTfig)
                end
            end
            if err
                done = 0;
                msgbox(errstr,getString(message('signal:sptoolgui:Error')),'error','modal')
            else
                done = 1;
            end
        case 'cancel'
            % do nothing and return
            done = 1;
            err = 1;
        case 'help'
            sptprefsHelp
        end
    
        if ~done
            if strcmp(get(ud.revert,'Enable'),'on')
                ud.changedFlags(panelInd) = 1;
            else
                ud.changedFlags(panelInd) = 0;
            end
            if any(ud.changedFlags)
                set(ud.ok_btn,'Enable','on')
            else
                set(ud.ok_btn,'Enable','off')
            end
            ud.flag = [];
            set(fig,'UserData',ud)
        end
    end   
    
    delete(fig)
    varargout{1} = ud.prefs;
    varargout{2} = panelInd;
        
% ------
%  action == 'ok' or 'cancel' or anything else
otherwise
    fig = gcf;
    ud = get(fig,'UserData');
    ud.flag = action;
    if nargin > 1
        ud.whichControl = varargin{2};
    end
    set(fig,'UserData',ud)
        
end

function minwidthLeft(h,n)
%MINWIDTH Minimize width of left justified text object to be just wide
% enough for extent.
% optional second argument specifies additional pixels on either side
% of text, defaults to 2

if nargin == 1
    n = 2;
end
for i=1:length(h)
    ex = get(h(i),'Extent');
    pos = get(h(i),'Position');
    style = get(h(i),'Style');
    if strcmp(style,'checkbox') | strcmp(style,'radiobutton')
        % add to width to account for radio or check box
        set(h(i),'HorizontalAlignment','center',...
           'Position',[pos(1) pos(2) ex(3)+2*n+30 pos(4)])
   else
       set(h(i),'HorizontalAlignment','center',...
           'Position',[pos(1) pos(2) ex(3)+2*n pos(4)]);
    end
end


function panelControls = createPanel(prefs,pf_pos,fus,lbs,lw,lh,uw,uh);
%createPanel - create uicontrols for Panel
% Inputs:
%    prefs - 1-by-1 struct with fields
%       .panelName, .panelDescription, .controls, .currentValue
%    pf_pos - position in pixels of panel frame
%    fus,lbs,lw,lh,uw,uh - spacing and positioning information
% Outputs:
%    panelControls - cell array of handle vectors, one entry for each row
%       of prefs.controls

labelPos = [pf_pos(1)+fus pf_pos(2)+pf_pos(4)-lh/2+fus lw uh];
controlPos = [pf_pos(1)+fus+lbs+lw ...
       pf_pos(2)+pf_pos(4)-lh/2+fus uw uh];

N = size(prefs.controls,1);
% prefs.controls is a table containing the following columns:
%  Name  Description  type  radiogroup  number_of_lines popup_string ...
%      factory_value  help_string

panelControls = cell(1,N);
for i=1:N
    if i == 1
        labelPos = labelPos - [0 uh+fus 0 0];
    else
        labelPos = labelPos - [0 prefs.controls{i-1,5}*uh+fus 0 0];
    end
    controlPos = controlPos - [0 prefs.controls{i,5}*uh+fus 0 0];
    controlPos(4) = prefs.controls{i,5}*uh;
    switch prefs.controls{i,3}
    case 'edit'
        panelControls{i} = uicontrol('Style','text',...
            'Units','pixels',...
            'Tag',['control' num2str(i)],...
            'Position',labelPos,...
            'String',getTranslatedString('signal:sptoolgui',prefs.controls{i,2}),...
            'HorizontalAlignment','right');
        panelControls{i} = [ panelControls{i}  uicontrol('Style','edit',...
            'Units','pixels',...
            'Tag',['control' num2str(i)],...
            'BackgroundColor','w','String',getTranslatedString('signal:sptoolgui',prefs.currentValue{i}),...
            'Callback',['sbswitch(''sptprefs'',''change'',' num2str(i) ')'],...
            'UserData',prefs.controls{i,4},...
            'Position',controlPos,...
            'HorizontalAlignment','left',...
            'Max',prefs.controls{i,5})   ];    
    case 'popupmenu'
        panelControls{i} = uicontrol('Style','text',...
            'Units','pixels',...
            'Tag',['control' num2str(i)],...
            'Position',labelPos,...
            'String',getTranslatedString('signal:sptoolgui',prefs.controls{i,2}),...
            'HorizontalAlignment','right');
        panelControls{i} = [ panelControls{i}  uicontrol('Style','popupmenu',...
            'BackgroundColor','white',...
            'Units','pixels',...
            'Tag',['control' num2str(i)],...
            'String',getTranslatedStringcell('signal:sptoolgui',prefs.controls{i,6}),...
            'Value',prefs.currentValue{i},...
            'Callback',['sbswitch(''sptprefs'',''change'',' num2str(i) ')'],...
            'UserData',prefs.controls{i,4},...
            'Position',controlPos)   ];    
    case 'radiobutton'
        panelControls{i} = uicontrol('Style','radiobutton',...
            'Units','pixels',...
            'Tag',['control' num2str(i)],...
            'Position',labelPos,...
            'String',getTranslatedString('signal:sptoolgui',prefs.controls{i,2}),...
            'Value',prefs.currentValue{i},...
            'Callback',['sbswitch(''sptprefs'',''change'',' num2str(i) ')'],...
            'UserData',prefs.controls{i,4});
        minwidthLeft(panelControls{i})

    case 'checkbox'
        panelControls{i} = uicontrol('Style','checkbox',...
            'Units','pixels',...
            'Tag',['control' num2str(i)],...
            'Position',labelPos,...
            'String',getTranslatedString('signal:sptoolgui',prefs.controls{i,2}),...
            'Value',prefs.currentValue{i},...
            'Callback',['sbswitch(''sptprefs'',''change'',' num2str(i) ')'],...
            'UserData',prefs.controls{i,4});
        minwidthLeft(panelControls{i})
    end

end

function setPanel(panelControls,value)
%setPanel - set values of uicontrols
% Inputs:
%    panelControls - cell array of handle vectors for the current panel
%    value - cell array of values for each entry of panelControls

N = length(panelControls);
for i=1:N
    if length(panelControls{i}) == 1
        style = get(panelControls{i}(1),'Style');
    else
        style = get(panelControls{i}(2),'Style');
    end
    switch style
    case 'edit'
        set(panelControls{i}(2),'String',getTranslatedString('signal:sptoolgui',value{i}))
    case 'popupmenu'
        set(panelControls{i}(2),'Value',value{i})
    case {'radiobutton', 'checkbox'}
        set(panelControls{i}(1),'Value',value{i})
    end
end
% test
for i=1:N
    out = panelControls{i};
end

function values = getPanel(panelControls)
%getPanel - get values from uicontrols
% Inputs:
%    panelControls - cell array of handle vectors for the current panel
% Outputs:
%    value - cell array (column) of values for each entry of panelControls
   values = {};
   for i = 1:length(panelControls)
       if length(panelControls{i}) == 1
           style = get(panelControls{i}(1),'Style');
       else
           style = get(panelControls{i}(2),'Style');
       end
       switch style
       case 'edit'
           values = {values{:} ...
                get(panelControls{i}(2),'String')};
       case 'popupmenu'
           values = {values{:} ...
                get(panelControls{i}(2),'Value')};
       case {'radiobutton','checkbox'}
           values = {values{:} ...
                get(panelControls{i}(1),'Value')};
       end
   end
   values = values(:);

function setEnable(panelInd,prefs,panelControls,factory,revert)
% setEnable - sets enable property of factory and revert uicontrols

   values = getPanel(panelControls{panelInd});
   if isequal(values,prefs(panelInd).currentValue)
       set(revert,'Enable','off')
   else
       set(revert,'Enable','on')
   end
   if isequal(values,prefs(panelInd).controls(:,7))
       set(factory,'Enable','off')
   else
       set(factory,'Enable','on')
   end
   
function sptprefsHelp
% local function to go into help mode
    fig = gcf;
    ud = get(fig,'UserData');
    set(ud.helpButton,'String',getString(message('signal:sptprefhelp:ClickForHelp')))
    setptr(fig,'help')

    panelInd = get(ud.list,'Value');
    
    uilist = findobj(fig,'type','uicontrol');
    saveenable = get(uilist,'Enable');
    set(uilist,'Enable','inactive',...
       'ButtonDownFcn',...
         'set(findobj(gcf,''tag'',''helpButton''),''userdata'',get(gcbo,''tag''))')

    set(ud.helpButton,'UserData',0)
    waitfor(ud.helpButton,'userdata')
    tag = get(ud.helpButton,'UserData');
    switch tag
    case 'helpButton'
        s = {
        getString(message('signal:sptprefhelp:PREFERENCESDIALOGBOX'))
        ' '
        getString(message('signal:sptprefhelp:ThisWindowAllowsYouToChangeCertainSettingsOfTheSPToolAn'))
        ' '
        getString(message('signal:sptprefhelp:ToGetHelpOnAnyPreferenceOrOnHowToUseTheButtonsClickHelp'))
        };
    case 'revertButton'
        s = {
        getString(message('signal:sptprefhelp:REVERTPANELBUTTON'))
        ' '
        getString(message('signal:sptprefhelp:PressThisButtonToRevertThePreferencesInTheCurrentPanelT'))
        ' '
        getString(message('signal:sptprefhelp:ThisButtonIsEnabledOnlyIfYouHaveMadeAChangeToThePrefere')) 
        };
    case 'factoryButton'
        s = {
        getString(message('signal:sptprefhelp:FACTORYSETTINGSBUTTON'))
        ' '
        getString(message('signal:sptprefhelp:PressThisButtonToRestoreThePreferencesInTheCurrentPanel'))
        ' '
        getString(message('signal:sptprefhelp:ThisButtonIsEnabledOnlyWhenThePreferencesInTheCurrentPa')) 
        };
    case 'okButton'
        s = {
        getString(message('signal:sptprefhelp:OKBUTTON'))
        ' '
        getString(message('signal:sptprefhelp:PressThisButtonToApplyYourChangesToThePreferencesAndClo'))
        ' '
        getString(message('signal:sptprefhelp:ThisButtonIsOnlyEnabledWhenYouHaveMadeSomeChangeToAPref'))
        ' '
        getString(message('signal:sptprefhelp:IfTheFilesigprefsmatIsNotFoundADialogBoxWillAppearAskin'))
        };
    case 'cancelButton'
        s = {
        getString(message('signal:sptprefhelp:CANCELBUTTON'))
        ' '
        getString(message('signal:sptprefhelp:PressThisButtonToCloseThePreferencesWindowIgnoringAnyCh'))
        };
    case 'listbox'
        s = {
        getString(message('signal:sptprefhelp:PREFERENCECATEGORYLIST'))
        ' '
        getString(message('signal:sptprefhelp:ThisIsAListOfAllTheCategoriesOfPreferencesThatYouCanCha'))        
        ' '
        getString(message('signal:sptprefhelp:ByClickingOnACategoryThePanelOnTheRightChangesToReflect'))
        ' '
        getString(message('signal:sptprefhelp:ToGetHelpOnACategoryChangeToThatCategoryFirstAndThenCli'))
        };
    case {'panelLabel','panelFrame'}
        s = {
        [getString(message('signal:sptprefhelp:PreferencesFor')) ' "' ud.prefs(panelInd).panelDescription '"'] ...
        ' ' ...
        ud.prefs(panelInd).panelHelp{:} ...
        };
    otherwise
        ind = findstr('control',tag);
        if ~isempty(ind)
            tag(1:7)=[];
            i = str2double(tag);
            s = {[getString(message('signal:sptprefhelp:YouHaveClickedOnPreference')) ' "' ...
                     getTranslatedString('signal:sptprefhelp',ud.prefs(panelInd).controls{i,2}) '"' ... 
                     getString(message('signal:sptprefhelp:InTheCategory')) ' "' ...
                  getTranslatedString('signal:sptprefhelp',ud.prefs(panelInd).panelDescription) '".'] ...
                 ' ' ...
                 ud.prefs(panelInd).controls{i,8}{:} ...
                 };
        else
            s = {[getString(message('signal:sptprefhelp:ThisObjectHasTag')) tag]};
        end
    end
    
    set(uilist,{'Enable'},saveenable,'ButtonDownFcn','')
    set(ud.helpButton,'String',getString(message('signal:sptprefhelp:Help')))
    setptr(fig,'arrow')
    
    fp = get(fig,'Position');
    sz = sptsizes;
    helpButtonPos = get(ud.helpButton,'Position');
    sz.uh = helpButtonPos(4);

    saveVis = get(uilist,'Visible');
    if ispc
        set(uilist,'Visible','off')
    end

    f = uicontrol('Style','frame',...
           'Position',[sz.fus sz.fus fp(3)-2*sz.fus fp(4)-sz.fus-1],...
           'Tag','prefhelp');
    tp = [2*sz.fus 4*sz.fus+sz.uh fp(3)-4*sz.fus fp(4)-(6*sz.fus+sz.uh)];
       % text position
    [fontname,fontsize]=fixedfont;
    t = uicontrol('Style','text','Position',tp,'String',s,'Max',2,...
         'Tag','prefhelp','HorizontalAlignment','left',...
         'BackgroundColor','w','FontName',fontname,'FontSize',fontsize);
    bp = [fp(3)/2-sz.bw/2 2*sz.fus sz.bw sz.uh];
    okCbStr = 'delete(findobj(gcf,''tag'',''prefhelp''))';
    b = uicontrol('Style','pushbutton','Position',bp,...
         'Tag','prefhelp','String',getString(message('signal:sptprefhelp:OK')),...
         'Callback',okCbStr);
      
    % Temporarily set the CloseRequestFcn property of fig to be the same
    % as the callback string of the OK button.  This will allow the user
    % to close the figure by clicking on the close box ('X') of the fig.
    closeReqFcn = get(fig,'CloseRequestFcn');
    set(fig,'CloseRequestFcn',okCbStr);
    waitfor(b)
    set(fig,'CloseRequestFcn',closeReqFcn);
    
    if all(ishandle(uilist))
        set(uilist,{'Visible'},saveVis)
    end

function filterPanelEnable(panelInd, ud)
    values = getPanel(ud.panelControls{panelInd});
    FDAFlag = values(2);
    if FDAFlag{1} == 1
        set(ud.panelControls{panelInd}{4}, 'Enable', 'off');
        set(ud.panelControls{panelInd}{5}, 'Enable', 'off');
        set(ud.panelControls{panelInd}{6}, 'Enable', 'off');
    else
        set(ud.panelControls{panelInd}{4}, 'Enable', 'on');
        set(ud.panelControls{panelInd}{5}, 'Enable', 'on');
        set(ud.panelControls{panelInd}{6}, 'Enable', 'on');
    end                              
    
function sigbrowsePanelEnable(panelInd, ud)
    values = getPanel(ud.panelControls{panelInd});
    LegacyFlag = values(1);
    if LegacyFlag{1} == 0
      enabState = 'off';
    else
      enabState = 'on';
    end
    set([ud.panelControls{panelInd}{2:end}], 'Enable', enabState);
