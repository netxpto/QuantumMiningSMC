function varargout = sptool(varargin)
%SPTOOL  Signal Processing Tool - Graphical User Interface.
%   SPTOOL opens the SPTool window which allows you to import,
%   analyze, and manipulate signals, filters, and spectra.
%
%   Exporting Component Structures from SPTool via the Command Line
%   ---------------------------------------------------------------
%   The following commands export component structures from the
%   currently open SPTool.
%
%   s = sptool('Signals') returns a structure array of all the signals.
%   f = sptool('Filters') returns a structure array of all the filters.
%   s = sptool('Spectra') returns a structure array of all the spectra.
%
%   [s,ind] = sptool(...) returns an index vector indicating which
%   of the elements of s are currently selected in SPTool.
%
%   s = sptool(...,0) returns only the currently selected objects.
%
%   Creating and Loading Component Structures via the Command Line
%   --------------------------------------------------------------
%   struc = sptool('create',PARAMLIST) creates a component
%   structure, struc, (defined by PARAMLIST) in the workspace.
%
%   sptool('load',struc) loads struc into SPTool; opens SPTool if
%   necessary.
%
%   struc = sptool('load',PARAMLIST) loads the component structure
%   defined by PARAMLIST into SPTool; if an optional output argument is
%   specified, a component structure, struc, is created in the workspace.
%
%   COMPONENT           PARAMLIST
%   ~~~~~~~~~           ~~~~~~~~~
%   SIGNALS:   COMPONENT_NAME,DATA,FS,LABEL
%   FILTERS:   COMPONENT_NAME,FORM,FILTER_PARAMS,FS,LABEL
%   SPECTRA:   COMPONENT_NAME,DATA,F,LABEL
%
%   PARAMETER DEFINITIONS
%   ~~~~~~~~~~~~~~~~~~~~~
%   COMPONENT_NAME - 'Signal', 'Filter', or 'Spectrum'; if omitted
%             COMPONENT_NAME defaults to 'Signal'.
%   FORM    - the form (or structure, e.g.,'tf','ss','sos', or 'zpk')
%             of the filter.
%   DATA    - a vector of doubles representing a signal or a spectrum.
%   FILTER_PARAMS - NUM, DEN when FORM is 'tf',
%                   SOS Matrix when FORM is 'sos',
%                   Z,P,K when FORM is 'zpk', and
%                   A,B,C,D when FORM is 'ss'.
%   FS      - sampling frequency (OPTIONAL), defaults to 1.
%   F       - frequency vector; applies to spectrum components only.
%   LABEL   - a string specifying the variable name of the component
%             as it will appear in SPTool (OPTIONAL); defaults to one
%             of the following: 'sig', 'filt', or 'spec'.
%
%   % Example:
%   %   Launch Signal Processing Tool Graphical User Interface.
%   
%   sptool;     % Lanches sptool

%  API for calling other tools -
%     Components and clients are defined by 'components' data
%     structure in the file signal/private/sptcompp.m.  See this
%     file for details.
%     Verb Button Pressed - calls
%         feval(verb.owningClient,'action',verb.action)
%     Selection has changed - calls for ALL verbs
%         enable = feval(verb.owningClient,'selection',verb.action,msg)
%         and sets verb's enable to result
%     Closing SPTool - calls
%         feval(defaultClient,'SPTclose')

%   Authors: T. Krauss and B. Jones
%   Copyright 1988-2013 The MathWorks, Inc.
%
%   Modified by
%   Carlos Ferreira Ribeiro
%   Armando Nolasco Pinto
%   January, 2016
%   IT, Aveiro.

if nargin == 0
    action = 'init';
    shh = get(0,'ShowHiddenHandles');           % 0 identifies the graphics root object (screen), armazena o valor de ShowHiddenHandles
    set(0,'ShowHiddenHandles','on')             % set ShowHiddenHandles = 'on'
    sptoolfig = findobj(0,'Tag','sptool');      % verifies if an object with a tag='spool' exists
    set(0,'ShowHiddenHandles',shh);             % reset ShowHiddenHandles with the original value
    if ~isempty(sptoolfig)                      % if an object with tag='sptool' exists 
        figure(sptoolfig)                       % puts the focus on it
        return                                  % leaves the sptool function
    end
    
    % if an object with tag='sptool' do not exists
    set(0, 'ShowHiddenHandles', 'on')
    
    % Initialize global variables
    initializeGlobalVariables;
 
    %   Name         Size Bytes Class   Attributes  Value
    %   choice       1x1  8     double  global      0
    %   choice2      1x1  8     double  global      0
    %   choice3      1x1  8     double  global      0
    %   nRead        1x1  8     double  global      256
    %   nSymbols     1x1  8     double  global      32
    %   phaseOption  1x1  8     double  global      0
    %   previousPath 1x37 74    char    global      from previousPath.mat
    %   start        1x1  8     double  global      0
    %   t_binary     1x3  6     char    global      'int'
    %   t_complex    1x6  12    char    global      'double'
    %   t_real       1x6  12    char    global      'double'
    
else
    %     convert2FDA flag is used to force FDATool format conversion without
    %     dialog. Only used for internal test
    if isequal(varargin{1}, 'convert2FDA')
        action = 'init';
        shh = get(0,'ShowHiddenHandles');
        set(0,'ShowHiddenHandles','on')
        sptoolfig = findobj(0,'Tag','sptool');
        set(0,'ShowHiddenHandles',shh);
        if ~isempty(sptoolfig)
            figure(sptoolfig)
            return
        end
    else
        action = varargin{1};
    end
end


switch action    
    case 'init'                % initialization        
        % Initialize SPTool userdata (ud)
        ud = init_sptool_data_settings;        
        % create sptool manager gui
        [fig,ud] = create_sptool_gui(ud);       
        set(fig,'UserData',ud,'ResizeFcn','sptool(''resize'')')        
        allPanels = {ud.prefs.panelName};
        defsesInd = findcstr(allPanels,'defsession');
        fileExist = ~isempty(which('startup.spt'));
        if ud.prefs(defsesInd).currentValue{1}(1) & fileExist,
            % Load default SPTool session file
            if ~isempty(varargin) && isequal(varargin{1}, 'convert2FDA')
                sptool('open','startup.spt', 1);
            else
                sptool('open','startup.spt');
            end
        end
        ud = get(fig,'UserData');
        if ud.fdwarnflag == 1
            hFdw = siggui.dontshowagaindlg;
            hFdw.PrefTag='sptoolfilterdesignerwarn';
            if hFdw.need2show
                hFdw.text = {getString(message('signal:sptoolgui:SPToolLoadsPreferencesFromTheSigprefsmatFileFilterDesig'))
                    ''
                    getString(message('signal:sptoolgui:NoteFilterDesignerWillBeRemovedInAFutureRelease'))
                    };
                hFdw.name=getString(message('signal:sptoolgui:FilterDesigner'));
                hFdw.NoHelpButton = true;
                hFdw.Icon='warn';
                hFdw.render;
                set(hFdw,'Visible','on');
                waitfor(hFdw.FigureHandle);
            end
            ud.fdwarnflag = 0;
            set(fig,'UserData',ud);
        end      
        sptool('resize')
        selectionChanged(fig,'new')
        set(fig,'HandleVisibility','callback','Visible','on')        
        cacheclasses;
        
        %% Create new pushbuttons
        createNewPushbuttons;
        
        %% Change callbacks
        changeCallbacks;
        
        %% Set global variable "start"
        setGlobalstart(1);
        %%
        
        %------------------------------------------------------------------------
        % structArray = sptool('callall',fname,structArray)
        % searches for all fname.m on path
        % and calls all found with structArray = feval(fname,structArray)
    case 'callall'
        fname = varargin{2};
        structArray = varargin{3};
        
        w = which('-all',fname);
        % make sure each entry is unique
        for i=length(w):-1:1
            ind = findcstr(w(1:i-1),w{i});
            if ~isempty(ind)
                w(i) = [];
            end
        end
        if ~isempty(w)
            origPath=pwd;
        end
        for i=1:length(w)
            thispath=char(w(i));
            pathsp=find(thispath==filesep);
            pathname=thispath(1:pathsp(end));
            cd(pathname)
            structArray = feval(fname,structArray);
        end
        if ~isempty(w)
            cd(origPath)
        end
        varargout{1} = structArray;
        
        %------------------------------------------------------------------------
        % p = sptool('getprefs',panelName)
        % p = sptool('getprefs',panelName,fig)
        %   Return preference structure for panel with panelName
        %   Inputs:
        %     panelName - string
        %     fig (optional) - figure of SPTool; uses findobj if not given
        %   Outputs:
        %     p - value structure for this panel
        %     p_defaults - default values structure (from sigprefs.mat) for this panel
    case 'getprefs'
        % set showhiddenhandles since this might be called from the command line:
        shh = get(0,'ShowHiddenHandles');
        set(0,'ShowHiddenHandles','on');
        panelName = varargin{2};
        if nargin > 2
            fig = varargin{3};
        else
            fig = findobj(0,'Tag','sptool');
        end
        ud = get(fig,'UserData');
        allPanels = {ud.prefs.panelName};
        i = findcstr(allPanels,panelName);
        if isempty(i)
            error(message('signal:sptool:GUIErrPanel', panelName, sprintf( '\n   ''%s'' ', allPanels{ : } )));
        end
        %p = prefstruct(ud.prefs(i));
        p = cell2struct(ud.prefs(i).currentValue,ud.prefs(i).controls(:,1));
        varargout{1} = p;
        if nargout > 1
            % Provide the default value to compare against a possible new preference
            fileName='sigprefs.mat';
            if ~isempty(which(fileName)),
                % Load MAT-file containing saved preferences to compare
                % against current preferences.
                load(fileName);
                
                % Return a structure with containing the preference names and values.
                varargout{2} = eval(['SIGPREFS.',ud.prefs(i).panelName]);
            else
                % Compare against default (factory) settings stored in the figure's userdata.
                varargout{2} = cell2struct(ud.prefs(i).controls(:,7),...
                    ud.prefs(i).controls(:,1));
            end
            
        end
        set(0,'ShowHiddenHandles',shh);
        
        
        %------------------------------------------------------------------------
        % sptool('setprefs',panelName,p)
        % sptool('setprefs',panelName,p,fig)
        %   Set preference structure for panel with panelName
        %   Inputs:
        %     panelName - string
        %     p - value structure for this panel
        %     fig (optional) - figure of SPTool; uses findobj if not given
    case 'setprefs'
        panelName = varargin{2};
        p = varargin{3};
        if nargin > 3
            fig = varargin{4};
        else
            fig = findobj(0,'Tag','sptool');
        end
        ud = get(fig,'UserData');
        allPanels = {ud.prefs.panelName};
        i = findcstr(allPanels,panelName);
        if isempty(i)
            error(message('signal:sptool:GUIErrPanel', panelName, sprintf( '\n   ''%s'' ', allPanels{ : } )));
        end
        ud.prefs(i).currentValue = struct2cell(p);
        setsigpref(ud.prefs(i).panelName,p);
        set(fig,'UserData',ud)
        
    case 'resize'
        
        sptoolfig = findall(0,'Tag','sptool');
        ud = get(sptoolfig,'UserData');
        numComponents = length(ud.components);
        fp = get(sptoolfig,'Position');
        upperlefty = fp(2)+fp(4);
        
        % NOTE: Only set the figure position when the figure gets too small.
        % Changing the figure's position using SET clears the 'Maximized' state
        % of the figure, hence, disabling the 'Minimize' button on the figure.
        if fp(3) < 208 && fp(4) < 193,
            fp(2) = upperlefty - 193;
            fp(3) = 208;
            fp(4) = 193;
            set(sptoolfig,'Position',fp);
            % warndlg('Restoring SPTool figure to its minimum height and width.','Too Small');
        elseif fp(3) < 208,
            fp(3) = 208;
            set(sptoolfig,'Position',fp);
            % warndlg('Restoring SPTool figure to its minimum width.','Too Thin');
        elseif fp(4) < 193.
            fp(2) = upperlefty - 193;
            fp(4) = 193;
            set(sptoolfig,'Position',fp);
            % warndlg('Restoring SPTool figure to its minimum height.','Too Short');
        end
        
        d1 = 3;
        d2 = 3;
        uw = (fp(3)-(numComponents+1)*d1)/numComponents;
        uh = 20;  % height of buttons and labels
        lb = d2+(d2+uh)*(ud.maxVerbs);  % list bottom

        %% Resizing "visualizer" pushbuttons 
        startr = getGlobalstart; % Get global variable "start" 
        for i = 1:numComponents % 3 Different Components ("Signals", "Filters" and "Spectra")
            set(ud.list(i),'Position',[d1+(d1+uw)*(i-1) lb uw fp(4)-lb-2*d2-uh])
            set(ud.label(i),'Position',[d1+(d1+uw)*(i-1) fp(4)-d2-uh uw uh])
            if i == 1
                if startr == 0
                    nButtons = 1; % Starting value
                else
                    nButtons = 5; % 5 buttons from "Signals" component
                end
            end
            if i == 2
                nButtons = 4; % 4 buttons from "Filters" component (2 visible + 2 invisible)
            end
            if i == 3
                nButtons = 3; % 3 buttons from "Spectra" component (1 visible + 2 invisible)
            end 
            %for j=1:length(ud.components(i).verbs) 
            for j = 1:nButtons
                if i > 1 || (i == 1 && j <= 3)
                set(ud.buttonHandles(i).h(j),'Position',...
                   [d1+(d1+uw)*(i-1) lb-j*(d2+uh) uw uh]);
                end
                if i == 1 && j >= 4
                    if j == 4
                        set(ud.buttonHandles(i).h(j),'Position',...
                        [(d1+(d1+uw)*(i-1)) lb-j*(d2+uh) 0.45*uw uh]);
                    else
                        pEyeDiag = get(findobj('Tag', 'buttonEyeDiagram'), 'position');
                        pModPhase = get(findobj('Tag', 'buttonModPhase'), 'position');
                        set(findobj('Tag', 'buttonMod2Phase'), 'position', [pEyeDiag(1)+pEyeDiag(3)-pModPhase(3) pModPhase(2) pModPhase(3) pModPhase(4)]);
                    end
                end       
            end
        end
        %%
        
    case 'open'
        %  sptool('open')   <-- uses uigetfile to get filename and path
        %  sptool('open',f) <-- uses file with filename f on the MATLAB path
        %  sptool('open',f,p) <-- uses file with filename f and path p
        sptoolfig = findall(0,'Tag','sptool');
        ud = get(sptoolfig,'UserData');
        if ud.unchangedFlag == 0
            if ~saveChangesPrompt(ud.sessionName,'opening')
                return
            end
        end
        forceFDAformat = 0;
        
        switch nargin
            case 1
                matlab_wd = pwd;
                cd(ud.wd)
                [f,ud.sessionPath]=uigetfile('*.spt',getString(message('signal:sptoolgui:OpenSession')));
                cd(matlab_wd)
                
                if ~isequal(f,0),
                    loadName = fullfile(ud.sessionPath,f);
                    ud.wd = ud.sessionPath;
                    set(sptoolfig,'UserData',ud)
                else
                    return
                end
                
            case 2
                f = varargin{2};
                loadName = which(f);
                if isempty(loadName)
                    error(message('signal:sptool:GUIErrFileNotFound', f))
                end
                ud.sessionPath = '';
            case 3
                f = varargin{2};
                if isequal(varargin{3}, 1)
                    loadName = which(f);
                    if isempty(loadName)
                        error(message('signal:sptool:GUIErrFileNotFound', f))
                    end
                    ud.sessionPath = '';
                    forceFDAformat = 1;
                else
                    ud.sessionPath = varargin{3};
                    loadName = fullfile(ud.sessionPath,f);
                    if exist(loadName, 'file')~=2
                        error(message('signal:sptool:GUIErrFileNotFound', loadName))
                    end
                end
        end
        
        if ~isequal(f,0)
            load(loadName,'-mat')
            if ~exist('session','var')  % variable
                waitfor(msgbox(getString(message('signal:sptoolgui:ThisFileIsNotAValidSPTFile')),...
                    getString(message('signal:sptoolgui:MissingSessionInfo')),'error','modal'))
                return
            end
            
            ud = get(sptoolfig,'UserData');
            ud.sessionName = f;
            %-------------------------------------------------------------------------
            % Code added for FDATool - check for pref setting to determine what to use
            % for filter design
            %-------------------------------------------------------------------------
            filtdesPrefs = sptool('getprefs','filtdes',sptoolfig);
            ud.session = struct2cell(session); %#ok 'session' is loaded from mat-file
            set(sptoolfig,'UserData', ud);
            filters = ud.session{2};
            checkFilterWindow(ud);
            if ~isempty(filters) %has filter
                if isequal(filtdesPrefs.fdatoolFlag, 1) && hasSPTfilter(ud.session) && ~isfield(filters(1), 'FDAspecs')
                    %           prompt the user for fdatool use and perform the spec translation if needed.
                    promptForFDA(ud, filtdesPrefs, forceFDAformat);
                elseif isequal(filtdesPrefs.fdatoolFlag, 0) &&  isfield(filters(1), 'FDAspecs')
                    promptForRemoveFilters(filtdesPrefs);
                    % avoid showing the warning dialog
                    ud=get(sptoolfig,'UserData');
                    ud.fdwarnflag = 0;
                    set(sptoolfig,'UserData', ud);
                end
            end
            filtdesPrefs = sptool('getprefs','filtdes',sptoolfig);
            ud = get(sptoolfig,'UserData');
            msgstr = '';
            if isequal(filtdesPrefs.fdatoolFlag, 0)
                [ud.session,msgstr] = sptvalid(ud.session,ud.components);
            end
            %-----
            % end
            %-----
            figname = sprintf('%s: %s%s',getString(message('signal:sptoolgui:SPTool')),ud.sessionPath,ud.sessionName);
            set(sptoolfig,'Name',figname)
            set(sptoolfig,'UserData',ud)
            for k = 1:length(ud.session)
                if ~isempty(ud.session{k})
                    set(ud.list(k),'Value',1);
                end
            end
            
            updateLists;
            selectionChanged(sptoolfig,'new');
            for idx = [4 6]
                set(ud.filemenu_handles(idx),'Enable','on');
            end
            set(ud.filemenu_handles(5),'Enable','off');
            ud = get(sptoolfig,'UserData');
            %-----------------------
            % Code added for FDATool
            %-----------------------
            %        check for the flags and see if conversion has caused them
            %        to set
            if isequal(ud.savedFlag, 1) && isequal(ud.unchangedFlag, 1)
                if isempty(msgstr)
                    ud.savedFlag = 1;
                    ud.unchangedFlag = 1;
                else
                    ud.savedFlag = 0;
                    ud.unchangedFlag = 0;
                end
            end
            %-----
            % end
            %-----
            ud.changedStruc = [];
        end
        set(sptoolfig,'UserData',ud);
        
        %----------------------------------------------------------------------
        %err = sptool('save')
        %    save session, using known file name
        %    If the session has never been saved, calls sptool('saveas')
        %  CAUTION: saves userdata on exit (to save ud.unchangedFlag)
        %  Outputs:
        %    err - ==1 if cancelled, 0 if save was successful.
    case 'save'
        sptoolfig = findall(0,'Tag','sptool');
        ud = get(sptoolfig,'UserData');
        if ~ud.savedFlag
            err = sptool('saveas');
            ud = get(sptoolfig,'UserData');
        else
            session = cell2struct(ud.session,{ud.components.name}); %#ok session is saved in mat-file
            save(fullfile(ud.sessionPath,ud.sessionName),'session')
            err = 0;
        end
        ud.unchangedFlag = ~err;
        if ud.unchangedFlag
            set(ud.filemenu_handles(5),'Enable','off')
        end
        set(sptoolfig,'UserData',ud)
        varargout{1} = err;
        %----------------------------------------------------------------------
        %err = sptool('saveas')
        %    save session, prompting for file name.
        %  CAUTION: saves userdata on exit (to save ud.unchangedFlag)
        %  Outputs:
        %    err - ==1 if cancelled, 0 if save was successful.
    case 'saveas'
        sptoolfig = findall(0,'Tag','sptool');
        ud = get(sptoolfig,'UserData');
        matlab_wd = pwd;
        cd(ud.wd)
        [f,p] = sptuiputfile(ud.sessionName,'Save Session');
        
        cd(matlab_wd)
        if ~isequal(f,0)
            if (length(f)<4) || ~isequal(f(end-3:end),'.spt')
                f = [f '.spt'];
            end
            session = cell2struct(ud.session,{ud.components.name}); %#ok session is saved in mat-file
            ud.sessionName = f;
            ud.sessionPath = p;
            save(fullfile(p,f),'session', '-v6')
            
            ud.sessionName = f;
            set(sptoolfig,'UserData',ud)
            
            figname = sprintf('%s: %s',getString(message('signal:sptoolgui:SPTool')),ud.sessionName);
            set(sptoolfig,'Name',figname)
            ud.unchangedFlag = 1;
            ud.wd = p;
        else
            % ud.unchangedFlag = ud.unchangedFlag;  % value doesn't change
        end
        if ud.unchangedFlag
            set(ud.filemenu_handles(5),'Enable','off')
        end
        ud.savedFlag = ud.savedFlag | ud.unchangedFlag;
        set(sptoolfig,'UserData',ud)
        varargout{1} = ~ud.unchangedFlag;
        
    case 'pref'
        sptoolfig = findall(0,'Tag','sptool');
        ud = get(sptoolfig,'UserData');
        org_prefs = cell2struct(ud.prefs(5).currentValue, ud.prefs(5).controls(:,1));
        %     if FDATool window is open, disable the filter designer controls
        filtdesFig = findobj(0,'Tag','filtdes');
        if (isfield(ud, 'hFDA') && ishandle(ud.hFDA)) || ~isempty(filtdesFig)
            [ud.prefs ud.panelInd] = sptprefs(ud.prefs,ud.panelInd, 0);
        else
            [ud.prefs ud.panelInd] = sptprefs(ud.prefs,ud.panelInd, 1);
        end
        %-----------------------------------------------------------------
        % Code added to obsolete legacy signal browser
        %-----------------------------------------------------------------
        sigbrowsePrefs = sptool('getprefs','sigbrowse',sptoolfig);
        if sigbrowsePrefs.legacyBrowserEnable
          newOwner = 'sigbrowse';
        else
          newOwner = 'sigbrowser.sigbrowseAdapter';
        end
        ud.prefs(3).clientList = {newOwner};
        componentNames = {ud.components.name};
        i = findcstr(componentNames,'Signals');
        ud.components(i).verbs.owningClient = newOwner;                          
        %-----------------------------------------------------------------
        % Code added for FDATool - pref changed, prompt the user if needed
        %-----------------------------------------------------------------
        set(sptoolfig,'UserData',ud);
        filtdesPrefs = sptool('getprefs','filtdes',sptoolfig);
        cur_prefs = cell2struct(ud.prefs(5).currentValue, ud.prefs(5).controls(:,1));
        filters = ud.session{2};
        if ~isempty(filters)
            if isequal(cur_prefs.fdatoolFlag, 1) && ~isequal(org_prefs.fdatoolFlag, cur_prefs.fdatoolFlag) && ~isfield(filters(1), 'FDAspecs')
                %         switch from FDesigner to FDATool
                promptForFDA(ud, filtdesPrefs, 0);
            elseif isequal(cur_prefs.fdatoolFlag, 0) && ~isequal(org_prefs.fdatoolFlag, cur_prefs.fdatoolFlag)
                %         switch from FDATool to FDesigner
                promptForRemoveFilters(filtdesPrefs);
            end
        end
        
    case 'close'
        
        sptoolfig = findall(0, 'Tag', 'sptool');
        
        %{
        ud = get(sptoolfig,'UserData');
        
        p = cell(1, length(ud.prefs));
        p_default = p;
        for i = 1:length(ud.prefs)
            [p{i},p_default{i}] = sptool('getprefs',...
                ud.prefs(i).panelName,sptoolfig);
        end
        if ~isequal(p,p_default)
            setsigpref({ud.prefs.panelName},p,1);
        end
        
        if ud.unchangedFlag == 0
            if ~saveChangesPrompt(ud.sessionName,'closing')
                return
            end
        end
        
        for i=1:length(ud.components)
            for j=1:length(ud.components(i).verbs)
                feval(ud.components(i).verbs(j).owningClient,'SPTclose',...
                    ud.components(i).verbs(j).action);
            end
        end
        if ~isempty(ud.importSettings)
            if ishandle(ud.importSettings.fig)
                delete(ud.importSettings.fig)
            end
        end
        %}
        
        delete(sptoolfig)
        clear global SIGPREFS;

        %% Close all figures when "visualizer" is turn off
        closeAllFigures;
 
        %% Set global variable "nRead" and save
        setGlobalnRead(256);
        
        %% Set global variable "previousPath" and save
        setGlobalPreviousPath(getGlobalPreviousPath);
        
        %% Set global variable "t_binary" and save
        setGlobalt_binary('int');
        
        %% Set global variable "t_real" and save
        setGlobalt_real('double');
        
        %% Set global variable "t_complex" and save
        setGlobalt_complex('double');
        %%

    case 'create'
        % Create Signal, Filter or Spectrum structure from the command line:
        % sptool('create',...)
        %
        
        narginchk(2,9)
        shh = get(0,'ShowHiddenHandles');
        set(0,'ShowHiddenHandles','on')
        sptoolfig = findobj(0,'Tag','sptool');
        set(0,'ShowHiddenHandles',shh);
        
        if isempty(sptoolfig)  % SPTool is not open - only get required info
            ud.components = [];
            ud.components = sptcompp(ud.components); % calls one in signal/private
            ud.prefs = sptprefp;
            allPanels = {ud.prefs.panelName};
            plugInd = findcstr(allPanels,'plugins');
            plugPrefs = cell2struct(ud.prefs(plugInd).currentValue,...
                ud.prefs(plugInd).controls(:,1));
            if plugPrefs.plugFlag
                % now call each one found on path:
                ud.components = sptool('callall','sptcomp',ud.components);
            end
        else                   % SPTool is open; get the info from SPTool
            ud = get(sptoolfig,'UserData');
        end
        compNames = {ud.components.structName};
        
        if ~ischar(varargin{2}) && ~isnumeric(varargin{2})
            set(0,'ShowHiddenHandles',shh)
            error(message('signal:sptool:InvalidParamSecondArgMustBeString'))
        end
        
        compIndx = [];
        if isnumeric(varargin{2})
            % No component name specified; use default component (1st one)
            compIndx = 1;
            varargin{end+1} = varargin{end}; % Make room to insert comp name
            mvIndx = 2:nargin;
            varargin(mvIndx+1) = varargin(mvIndx);
            varargin{2} = compNames{compIndx}; % Insert component name
        else
            % Since the 2nd arg is not data for the default (1st) component
            % then it must be a string containing the component name; check it!
            for i = 1:length(compNames)
                if strcmpi(varargin{2}, compNames{i})
                    % Index into compNames which matches compnt name entered
                    compIndx = i;
                    break
                end
            end
            if isempty(compIndx)
                % Component name was specified incorrectly
                set(0,'ShowHiddenHandles',shh)
                error(message('signal:sptool:GUIErrComponent', varargin{ 2 }, sprintf( '      ''%s''\n', compNames{ : } )));
            end
        end
        
        [~,fields,FsFlag,defaultLabel] = ...
            feval(ud.components(compIndx).importFcn,'fields');
        
        [formIndx, formTags] = formIndex(fields,varargin{3});
        if isempty(formIndx)
            set(0,'ShowHiddenHandles',shh)
            error(message('signal:sptool:GUIErrForm', formTags))
        elseif length(fields) ~= 1  % A valid 'form' was specified
            varargin(3) = [];       % Remove 'form' from input argument list
        end
        
        if ischar(varargin{end})
            compCell = varargin(3:end-1); % Cell array containing the component
        else
            compCell = varargin(3:end);
        end
        numCompFields = length(fields(formIndx).fields);
        
        if length(compCell) < numCompFields
            % Padd with []s in case some default arguments were left out
            compCell{numCompFields} = [];
        elseif length(compCell) > numCompFields+1
            errObj = message('signal:sptool:TooManyInputArgs',compNames{compIndx});
              
            if ~ischar(varargin{end})
                errstr = getString(errObj);
                errObj = message('signal:sptool:LastArgMustBeString', errstr);
            end
            set(0,'ShowHiddenHandles',shh)
            error(errObj)
        end
        
        % If Fs required and no Fs was entered; use default Fs=1
        if FsFlag & (length(compCell) < numCompFields+1)
            Fs = 1;
            compCell = {compCell{:} Fs};
        end
        
        % Complete cell array to be passed to the import function
        paramsCell = {formIndx compCell{:}};
        [err,errstr,struc] = feval(ud.components(compIndx).importFcn,...
            'make',paramsCell);
        if err
            set(0,'ShowHiddenHandles',shh)
            error(message('signal:sptool:GUIErr', errstr))
        end
        
        if ischar(varargin{end})
            if ~isvalidvar(varargin{end})
                set(0,'ShowHiddenHandles',shh)
                error(message('signal:sptool:InvalidParam'))
            else
                % Component label
                struc.label = varargin{end};
            end
        else
            struc.label = defaultLabel;
        end
        
        if nargout == 1
            varargout{1} = struc;
        end
        
    case 'load'
        % Importing Signals, Filters or Spectra from the command line:
        % sptool('load',struc) checks for validity of struc and imports it
        %   if it is valid.
        % The following syntax creates new structure using 'make' facility of
        % importFcn, and imports it into SPTool.
        
        error(nargchk(2,9,nargin,'struct'))
        shh = get(0,'ShowHiddenHandles');
        set(0,'ShowHiddenHandles','on')
        sptoolfig = findobj(0,'Tag','sptool');
        if isempty(sptoolfig)           % SPTool is closed - open it!
            sptool('init')
            sptoolfig = findobj(0,'Tag','sptool');
        end
        
        ud = get(sptoolfig,'UserData');
        compNames = {ud.components.structName};
        
        if isstruct(varargin{2})  % Second input is a structure
            struc = varargin{2};
            if nargin > 2
                set(0,'ShowHiddenHandles',shh)
                error(message('signal:sptool:Nargchk'))
            end
            
            errObj = isvalidstruc(struc,ud.components,compNames);
            if ~isempty(errObj)
                set(0,'ShowHiddenHandles',shh)
                error(errObj)
            end
        else                      % Second input is not a structure
            if ~ischar(varargin{2}) && ~isnumeric(varargin{2})
                set(0,'ShowHiddenHandles',shh)                
                error(message('signal:sptool:InvalidParamSecondArgMustBeStruct'))
            end
            % Create and load at the same time? - 'create' will parse the input
            struc =  sptool('create',varargin{2:end});
        end
        
        if nargout > 0
            varargout{1} = struc;
        end
        index = sptool('import',struc,1,sptoolfig);
        set(0,'ShowHiddenHandles',shh)
        
        if nargout>1,
            % Return load-index into list of objects for component
            varargout{2} = index;
        end
        
    case 'import'
        % sptool('import')  import structure using dialog box from sptimport
        % sptool('import',struc)  import given structure
        % sptool('import',struc,selectFlag)  import given structure and change
        %   selection in appropriate column to the imported struc if
        %     selectFlag == 1 (selectFlag defaults to 0)
        % sptool('import',struc,selectFlag,sptoolfig)
        %    uses sptoolfig as figure handle of sptool; if omitted, uses findobj
        % sptool('import',struc,selectFlag,sptoolfig,updateChangedStruc)
        %   if updateChangedStruc == 1, changedStruc is updated in the case of
        %    a replacement of struc (changedStruc is ALWAYS cleared when struc
        %    is new).
        %   if updateChangedStruc == 0, changedStruc is untouched (default)
        
        if nargin < 4
            sptoolfig = findobj(0,'Tag','sptool');
        else
            sptoolfig = varargin{4};
        end
        ud = get(sptoolfig,'UserData');
        
        if nargin < 5
            updateChangedStruc = 0;
        else
            updateChangedStruc = varargin{5};
        end
        
        if nargin < 3
            selectFlag = 0;
        else
            selectFlag = varargin{3};
        end
        
        if isempty(sptoolfig)
            error(message('signal:sptool:GUIErrSptoolNotOpen', 'SPTool'))
        end
        labelList = sptool('labelList',sptoolfig);
        if nargin == 1
            [componentNum,struc,ud.importSettings,ud.importwd] = ...
                sptimport(ud.components,labelList,ud.importSettings,ud.importwd);
            figure(sptoolfig)
            
            if componentNum<1  % user cancelled - so save userdata and bail
                set(sptoolfig,'UserData',ud)
                return
            end
        else
            struc = varargin{2};
        end
        
        componentNum = findcstr({ud.components.structName},...
            struc.SPTIdentifier.type);
        
        %---------------------------------------
        % Code added for FDATool - import filter
        %---------------------------------------
        %     some fields need to be added in order to use the fdatool
        filtdesPrefs = sptool('getprefs','filtdes',sptoolfig);
        if isequal(struc.SPTIdentifier.type, 'Filter') && isequal(filtdesPrefs.fdatoolFlag, 1)
            if ~isfield(struc ,'FDAspecs')
                struc.FDAspecs = [];
                if ~isfield(struc.specs ,'currentModule')
                    struc.specs.currentModule = 'fdpzedit';
                end
                % When import a filter designer format object,
                % the field struc.FDAspecs.sidebar.design cannot be empty.
                struc.FDAspecs.sidebar.design = getfdaformat(struc);
            end
        end
        if ~isempty(findcstr(labelList,struc.label))
            % replace old structure with this label -----------------------------
            
            % first find column number of old structure:
            for i=1:length(ud.components)
                ind = findStructWithLabel(ud.session{i},struc.label);
                if ~isempty(ind)
                    oldComponentNum = i;
                    break
                end
            end
            
            if (nargin==1) || updateChangedStruc
                ud.changedStruc = ud.session{oldComponentNum}(ind);
            end
            if componentNum == oldComponentNum  % replace in same column
                oldComponent = ud.session{oldComponentNum}(ind);
                if (nargin==1) % | strcmp(oldComponent.label,struc.label)
                    
                    % The following doesn't work because filtdes.m calls
                    % sptool('import') when the filter is already imported
                    %if strcmp(oldComponent.label,struc.label)
                    %    s1 = sprintf('Warning: Component structure %s ',struc.label);
                    %    s2 = sprintf('already exists in SPTool; replacing %s.',struc.label);
                    %    disp([s1 s2])
                    %end
                    
                    % obtained by import dialog so retain lineinfo field
                    ud.session{oldComponentNum}(ind) = ...
                        feval(ud.components(oldComponentNum).importFcn,'merge',...
                        oldComponent,struc);
                    
                else
                    ud.session{oldComponentNum}(ind) = struc;
                end
            else  % overwrite object of different type (column)
                ud.session{oldComponentNum}(ind) = [];
                if isempty(ud.session{componentNum})
                    ud.session{componentNum} = struc;
                else
                    ud.session{componentNum}(end+1) = struc;
                end
            end
            
            set(sptoolfig,'UserData',ud)
            updateLists(sptoolfig,componentNum)
            if componentNum ~= oldComponentNum
                updateLists(sptoolfig,oldComponentNum)
                set(ud.list(componentNum),'Value',length(ud.session{componentNum}))
            end
            if selectFlag || (nargin==1) || updateChangedStruc
                selectionChanged(sptoolfig,'new')
            end
        else
            % append structure to appropriate structure array -------------------
            ud.changedStruc = [];
            if isempty(ud.session{componentNum})
                ud.session{componentNum} = struc;
                ind=1;
            else
                ud.session{componentNum}(end+1) = struc;
                ind=length(ud.session{componentNum});
            end
            set(sptoolfig,'UserData',ud)
            updateLists(sptoolfig)
            set(ud.list(componentNum),'Value',length(ud.session{componentNum}))
            selectionChanged(sptoolfig,'new')
        end
        for idx = 4:6
            set(ud.filemenu_handles(idx),'Enable','on');
        end
        ud = get(sptoolfig,'UserData');  % need to get this again since
        % selectionChanged call might have
        % changed userdata
        ud.unchangedFlag = 0;
        set(ud.filemenu_handles(5),'Enable','on')
        set(sptoolfig,'UserData',ud);
        
        if nargout>0,
            % Return object-index to newly added object in the component
            % (ex, it was the 4th signal in the Signals list)
            varargout{1} = ind;
        end
                
        % sptool('export')
        %   Export objects from SPTool.
    case 'export'
        sptoolfig = findobj(0,'Tag','sptool');
        ud = get(sptoolfig,'UserData');
        [newprefs,componentSelect,fname,pathname] = ...
            sptexport(ud.prefs,ud.components,ud.session,get(ud.list,'Value'),ud.exportwd);
        
        if ~isequal(ud.prefs,newprefs)
            ud.prefs = newprefs;
        end
        
        if ~isequal(pathname,ud.exportwd)
            ud.exportwd = pathname;
            set(sptoolfig,'UserData',ud)
        end
        
        %------------------------------------------------------------------------
        % labelList = sptool('labelList',SPTfig)
        %   returns a cell array of strings containing all of the
        %   object labels currently in the SPTool.
    case 'labelList'
        fig = varargin{2};
        labelList = {};
        ud = get(fig,'UserData');
        for i=1:length(ud.components)
            if ~isempty(ud.session{i})
                labelList = {labelList{:} ud.session{i}.label};
            end
        end
        varargout{1} = labelList;
        
        %------------------------------------------------------------------------
        % sptool('edit')
        % callback of Edit menu
    case 'edit'
        sptoolfig = findobj(0,'Tag','sptool');
        ud = get(sptoolfig,'UserData');
        mh2 = ud.editmenu_handles(2);
        mh3 = ud.editmenu_handles(3);
        mh4 = ud.editmenu_handles(4);
        mh5 = ud.editmenu_handles(5);
        str = cell(0);
        for idx = 1:length(ud.components)
            tmp = get(ud.list(idx),'Value');
            if tmp == 1 & isempty(get(ud.list(idx),'String')) %#ok<AND2>
                tmp = [];
            end
            if idx == 1,
                selobj = tmp(:);
                compidx = ones(length(tmp),1);
            else
                selobj = [selobj; tmp(:)]; %#ok<AGROW>
                compidx = [compidx; idx*ones(length(tmp),1)]; %#ok<AGROW>
            end
            
            ud.compidx = [compidx selobj];
            tmpstr = get(ud.list(idx),'String');
            if ~isempty(tmpstr)
                tmpstr = tmpstr(tmp);
                if isempty(str)
                    str = tmpstr;
                else
                    if ~isempty(tmpstr) % Only if "tmpstr" is not empty
                        str = cat(1, str, tmpstr);
                    end
                end
            end
        end
        
        if ~isempty(selobj)
            % first initialize FsFlag array (determines if you can edit the
            % sampling frequency of a component or not)
            for i = 1:length(ud.components)
                [popupString,fields,FsFlag(i),defaultLabel] = ...
                    feval(ud.components(i).importFcn,'fields');
            end
            
            set(mh2,'Enable','on');
            set(mh3,'Enable','on');
            set(mh4,'Enable','on');
            set(mh5,'Enable','on');
            
            % remove extra menu items:
            delete(ud.dupsub(length(selobj)+1:end))
            ud.dupsub(length(selobj)+1:end)=[];
            delete(ud.clearsub(length(selobj)+1:end))
            ud.clearsub(length(selobj)+1:end)=[];
            delete(ud.namesub(length(selobj)+1:end))
            ud.namesub(length(selobj)+1:end)=[];
            ind = length(selobj)+1:length(ud.freqsub);
            delete(ud.freqsub(ind( find(ishandle(ud.freqsub(ind))) )))
            ud.freqsub(length(selobj)+1:end)=[];
            
            for idx1 = 1:length(selobj)
                if idx1 > length(ud.dupsub)
                    % create a new uimenu
                    ud.dupsub(idx1) = uimenu(mh2,'Label',str{idx1},'Tag',['dupmenu' int2str(idx1)],...
                        'Callback',['sptool(''duplicate'',',int2str(idx1),')']);
                    ud.clearsub(idx1) = uimenu(mh3,'Label',str{idx1},'Tag',['clearmenu' int2str(idx1)],...
                        'Callback',['sptool(''clear'',',int2str(idx1),')']);
                    ud.namesub(idx1) = uimenu(mh4,'Label',str{idx1},...
                        'Tag',['newnamemenu' int2str(idx1)],...
                        'Callback',['sptool(''newname'',',int2str(idx1),')']);
                    if FsFlag(compidx(idx1))
                        ud.freqsub(idx1) = uimenu(mh5,'Label',str{idx1},...
                            'Tag',['freqmenu' int2str(idx1)],'Callback',['sptool(''freq'',',int2str(idx1),')']);
                    else
                        % just put place holder here - don't create menu item since
                        % we can't edit the Sampling frequency for this component
                        %if matlab.graphics.internal.isGraphicsVersion1 
                        % For MATLAB R2013a 
                        if  strcmp(version, '8.1.0.604 (R2013a)')
                            ud.freqsub(idx1) = -1;
                        % For MATLAB R2014b and R2015a    
                        else                            
                            ud.freqsub(idx1) = gobjects;                            
                        end
                    end
                end
                if idx1 <= length(selobj)
                    % change label and ensure visibility of existing uimenu
                    set(ud.dupsub(idx1),'Visible','on','Label',str{idx1},'Tag',['dupmenu' int2str(idx1)]);
                    set(ud.clearsub(idx1),'Visible','on','Label',str{idx1},'Tag',['clearmenu' int2str(idx1)]);
                    set(ud.namesub(idx1),'Visible','on','Label',str{idx1},'Tag',['newnamemenu' int2str(idx1)]);
                    if FsFlag(compidx(idx1))
                        if ishandle(ud.freqsub(idx1))
                            set(ud.freqsub(idx1),'Visible','on','Label',str{idx1},'Tag',['freqmenu' int2str(idx1)]);
                        else
                            ud.freqsub(idx1) = uimenu(mh5,'Label',str{idx1},...
                                'Tag',['freqmenu' int2str(idx1)], ...
                                'Callback',['sptool(''freq'',',int2str(idx1),')']);
                        end
                    elseif ishandle(ud.freqsub(idx1))
                        set(ud.freqsub(idx1),'Visible','off')
                    end
                end
                
            end
        else
            set(mh2,'Enable','off');
            set(mh3,'Enable','off');
            set(mh4,'Enable','off');
            set(mh5,'Enable','off');
        end
        drawnow;
        set(sptoolfig,'UserData',ud);
        
        %------------------------------------------------------------------------
        % sptool('duplicate')
        % callback of duplicate submenu
    case 'duplicate'
        sptoolfig = findobj(0,'Tag','sptool');
        ud = get(sptoolfig,'UserData');
        lab = get(ud.dupsub(varargin{2}),'Label');
        bracket = findstr(lab,'[');
        lab1 = [lab(1:bracket-2),'copy'];
        lab2 = lab(bracket-1:end);
        idx = ud.compidx(varargin{2},:);
        
        % make sure new label is unique:
        labelList = {ud.session{idx(1)}.label};
        numCopies = 1;
        while ~isempty(findcstr(labelList,lab1))
            lab1 = [lab(1:bracket-2),'copy',num2str(numCopies)];
            numCopies = numCopies + 1;
        end
        
        ud.changedStruc = [];
        ud.session{idx(1)}(end+1)=ud.session{idx(1)}(idx(2));
        ud.session{idx(1)}(end).label = lab1;
        n = length(get(ud.list(idx(1)),'String'));
        set(ud.list(idx(1)),'Value',n+1);
        ud.unchangedFlag = 0;
        set(ud.filemenu_handles(5),'Enable','on')
        set(sptoolfig,'UserData',ud);
        updateLists(sptoolfig)
        selectionChanged(sptoolfig,'dup')
        
        %------------------------------------------------------------------------
        % sptool('clear')
        % callback of clear submenu
    case 'clear'
        sptoolfig = findobj(0,'Tag','sptool');
        ud = get(sptoolfig,'UserData');
        if strcmpi(varargin{2}, 'clear all');
            %----------------------------------------------------------
            % Code added for FDATool - changing back to Filter Designer
            %       remove all filters from sessions and update the UI
            %----------------------------------------------------------
            set(ud.list(2),'Value',[],'String',[]);
            ud.session{2} = [];
            ud.unchangedFlag = 0;
            set(sptoolfig,'UserData',ud);
            selectionChanged(sptoolfig,'clear');
        else 
            lab = get(ud.dupsub(varargin{2}),'Label');
            idx = ud.compidx(varargin{2},:);
            
            ud.changedStruc = ud.session{idx(1)}(idx(2));
            ud.session{idx(1)}(idx(2)) = [];
            
            if ud.components(idx(1)).multipleSelection
                % just remove item from selection
                listVal = get(ud.list(idx(1)),'Value');
                listValInd = find(listVal==idx(2));
                listVal(listValInd+1:end) = listVal(listValInd+1:end)-1;
                listVal(listValInd) = [];
            else
                listVal = 1;
            end
            str = get(ud.list(idx(1)),'String');
            str(idx(2)) = [];
            set(ud.list(idx(1)),'Value',listVal,'String',str);
            
            ud.unchangedFlag = 0;
            set(ud.filemenu_handles(5),'Enable','on')
            set(sptoolfig,'UserData',ud)
            
            %% Close all figures 
            closeAllFigures;
                        
            %% Selection changed
            selectionChanged(sptoolfig, 'clear');
            %%
        end
        
        %% Label of clear choice
        clearChoice = strsplit(lab, ' [');
        clearChoice = char(clearChoice(1));
        
        if length(clearChoice) >= 7
            if strcmp(clearChoice(1:6), 'spect_') % Spectrum was deleted
                flag = 0;
            else % Signal or Filter was deleted
                flag = 1;
            end
        else % Signal or Filter was deleted
            flag = 1;
        end
                
        %% Clear the associated spectrum (if there)
        if flag == 1 % Signal or Filter was deleted
            sptoolfig = findobj(0, 'Tag', 'sptool');
            ud = get(sptoolfig, 'UserData');
            list3 = get(ud.list(3), 'String');
            inds = 0; % Index of associated spectrum
            for k = 1:length(list3)
                str = strsplit(char(list3(k)), {' ', '[', '_', ']'});
                if length(str) > 4
                    str = str(2:end-2);
                    str = strjoin(str, '_'); 
                else
                    str = str(2);
                end
                if strcmp(clearChoice, str)
                    inds = k;
                end
            end
            if inds ~= 0
                % Clear the associated spectrum with selected Signal or
                % Filter
                idx = [3 inds]; % [Component List-index]
                ud.changedStruc = ud.session{idx(1)}(idx(2));
                ud.session{idx(1)}(idx(2)) = [];
                % Just remove item from selection
                set(ud.list(idx(1)), 'Value', idx(2));
                listVal = get(ud.list(idx(1)), 'Value');
                listValInd = find(listVal == idx(2));
                listVal(listValInd + 1:end) = listVal(listValInd + 1:end) - 1;
                listVal(listValInd) = [];
                str = get(ud.list(idx(1)), 'String');
                str(idx(2)) = [];
                set(ud.list(idx(1)), 'Value', listVal, 'String', str);
                % Save
                ud.unchangedFlag = 0;
                set(ud.filemenu_handles(5), 'Enable', 'on');
                set(sptoolfig, 'UserData', ud);
                % Selection changed
                selectionChanged(sptoolfig, 'clear');
            end
        end
                
        %% Disable list selection
        disableListSelection;
        
        %% Set global variable "choice"
        setGlobalchoice(0);
        
        %% Set global variable "choice2"
        setGlobalchoice2(0);
        
        %% Set global variable "choice3"
        setGlobalchoice3(0);
        
        %% Disable all pushbuttons
        disableAllPushbuttons;
        %%
        
        %------------------------------------------------------------------------
        % sptool('newname',idx)
        % callback of Name... submenu; idx is the index into the name
        %  submenu items (an integer >= 1).
        % sptool('newname',lab)
        %  if the second input arg is a string with a particular label,
        %  then the object with that name is changed.
        %  This second syntax provides the sptool clients with a way
        %  to change the names of their selected objects.
        %
        %  - lab syntax added 6/19/99 by TPK
    case 'newname'
        sptoolfig = findobj(0,'Tag','sptool');
        ud = get(sptoolfig,'UserData');
        if ~ischar(varargin{2})
            lab = get(ud.dupsub(varargin{2}),'Label');
            % don't need to set up ud.compidx here since
            % we are assuming this is the callback of the uimenu and
            % hence sptool('edit') has just been called.
            idx = ud.compidx(varargin{2},:);
        else
            lab = varargin{2};
            % search for label within session structure, since
            % we need the 'label [type]' format for lab
            for i=1:length(ud.session)
                labels = {ud.session{i}.label}';
                j = find(strcmp(labels,lab));
                if ~isempty(j),
                    break
                end
            end
            listStr = get(ud.list(i),'String');
            lab = listStr{j};
            idx = [i j];
            sptool('edit')  % <-- need to call this to set up ud.compidx
            ud = get(sptoolfig,'UserData');
        end
        bracket = findstr(lab,'[');
        lab1 = lab(1:bracket-2);
        lab2 = lab(bracket-1:end);
        prompt={getString(message('signal:sptoolgui:VariableName'))};
        def = {lab1};
        title = getString(message('signal:sptoolgui:NameChange'));
        lineNo = 1;
        lab1 =inputdlg(prompt,title,lineNo,def);
        if isempty(lab1)
            return
        end
        err = ~isvalidvar(lab1{:});
        if ~err
            currentlabels = get(ud.list(idx(1)),'String');
            labelList = sptool('labelList',sptoolfig);
            strInd = findcstr(labelList,deblank(lab1{:}));
            if ~isempty(strInd)
                if ~isequal(lab1{1},ud.session{idx(1)}(idx(2)).label)
                    % error prompt if name is already taken by another variable
                    errstr = {getString(message('signal:sptoolgui:ThereIsAlreadyAnObjectInTheSPTool'))};
                    errordlg(errstr,getString(message('signal:sptoolgui:NonuniqueName')),'replace');
                else
                    % no op (user entered the same name)
                end
                return
            end
        else
            errstr = {getString(message('signal:sptoolgui:SorryTheNameYouEnteredIsNotValid'))
                getString(message('signal:sptoolgui:PleaseUseALegalMATLABVariableName'))};
            errordlg(errstr,getString(message('signal:sptoolgui:BadVariableName')),'replace');
            return
        end
        
        if isequal(ud.session{idx(1)}(idx(2)).label,lab1{:})
            % new label is the same as the old one - do nothing!
            return
        end
        ud.changedStruc = ud.session{idx(1)}(idx(2));
        ud.session{idx(1)}(idx(2)).label = lab1{:};
        ud.unchangedFlag = 0;
        set(ud.filemenu_handles(5),'Enable','on')
        set(sptoolfig,'UserData',ud);
        listStr = get(ud.list(idx(1)),'String');
        listStr{idx(2)} = [deblank(lab1{:}) lab2];
        set(ud.list(idx(1)),'String',listStr)
        selectionChanged(sptoolfig,'label')
        
        %------------------------------------------------------------------------
        % sptool('freq')
        % callback of Sampling Frequency... submenu
    case 'freq'
        sptoolfig = findobj(0,'Tag','sptool');
        ud = get(sptoolfig,'UserData');
        lab = get(ud.dupsub(varargin{2}),'Label');
        idx = ud.compidx(varargin{2},:);
        prompt={[getString(message('signal:sptoolgui:SamplingFrequency')) ':']};
        def = {sprintf('%.9g',ud.session{idx(1)}(idx(2)).Fs)};
        title = getString(message('signal:sptoolgui:SpecifySamplingFrequency'));
        lineNo = 1;
        if isfield(ud.session{idx(1)}, 'FDAspecs')
            str = sprintf('%s %s', prompt{1}, def{1});
            msgbox(str, prompt{1});
            Fs = def;
        else
            Fs=inputdlg(prompt,title,lineNo,def);
            if isempty(Fs)
                return
            end
        end
        [Fs,err] = validarg(Fs{:},[0 Inf],[1 1],getString(message('signal:sptoolgui:SamplingFrequencyExpression')));
        if err ~= 0
            return
        end
        if ud.session{idx(1)}(idx(2)).Fs == Fs
            % new Fs is the same as the old one - do nothing!
            return
        end
        
        ud.changedStruc = ud.session{idx(1)}(idx(2));
        ud.session{idx(1)}(idx(2)) = feval(ud.components(idx(1)).importFcn,'changeFs',...
            ud.session{idx(1)}(idx(2)),Fs);
        ud.unchangedFlag = 0;
        set(ud.filemenu_handles(5),'Enable','on')
        set(sptoolfig,'UserData',ud);
        selectionChanged(sptoolfig,'Fs')
        
        %------------------------------------------------------------------------
        % sptool('changeFs',struc)
        % external interface to allow clients to change ONLY THE SAMPLING
        % FREQUENCY of the passed in object.
        %  Added 5/31/99, TPK
        % sptool('changeFs',lab,Fs)
        %  if the second input arg is a string with a particular label,
        %  then the object with that name is changed to given Fs.
        %  This second syntax provides the sptool clients with a way
        %  to change the sampling frequencies of selected objects.
        %
        %  - lab syntax added 6/26/99 by TPK
    case 'changeFs'
        sptoolfig = findobj(0,'Tag','sptool');
        ud = get(sptoolfig,'UserData');
        
        if ~ischar(varargin{2})
            struc = varargin{2};
            Fs = struc.Fs;
        else
            lab = varargin{2};
            % search for label within session structure
            for i=1:length(ud.session)
                labels = {ud.session{i}.label}';
                j = find(strcmp(labels,lab));
                if ~isempty(j),
                    break
                end
            end
            struc = ud.session{i}(j);
            Fs = varargin{3};
        end
        
        inType = struc.SPTIdentifier.type;
        idx(1) = find(strcmp(inType,{ud.components.structName}));
        idx(2) = find(strcmp(struc.label,{ud.session{idx(1)}.label}));
        
        ud.changedStruc = ud.session{idx(1)}(idx(2));
        ud.session{idx(1)}(idx(2)) = feval(ud.components(idx(1)).importFcn,'changeFs',...
            ud.session{idx(1)}(idx(2)),Fs);
        ud.unchangedFlag = 0;
        set(ud.filemenu_handles(5),'Enable','on')
        set(sptoolfig,'UserData',ud);
        selectionChanged(sptoolfig,'Fs')
        
        %------------------------------------------------------------------------
        % sptool('help','overview')    <-- Help Tool (SPTool)
        % sptool('help','helptoolbox') <-- Help Toolbox
        % sptool('help','helpdemos')   <-- Demos
        % sptool('help','helpabout')   <-- About Signal Toolbox
        % sptool('help') <-- context sensitive help
    case 'help'
        sptoolfig = findobj(0,'Tag','sptool');
        ud = get(sptoolfig,'UserData');
        whichHelp = varargin{2};
        titleStr = 'SPTOOL Help';
        helpFcn = 'spthelpstr';
        
        switch whichHelp,
            case 'overview',
                helpview(fullfile(docroot,'toolbox','signal', 'signal.map'),'sptool');
                % 		spthelp('tag',sptoolfig,titleStr,helpFcn,'overview');
                
            case 'helptoolbox', % Used by all SPTool clients
                doc signal/
                
            case 'helpdemos',   % Used by all SPTool clients
                demo toolbox signal
                
            case 'helpabout',   % Used by all SPTool clients
                aboutsignaltbx;
                
            otherwise
                % Context Sensitive Help.
                saveEnableControls = [];
                if ud.pointer ~= 2   % if not in help mode
                    numComponents = length(ud.components);
                    % enter help mode
                    controlNumber = 0;
                    for idx1 = 1:numComponents
                        controlNumber = controlNumber + 1;
                        saveEnableControls(controlNumber) = ud.list(idx1);
                        for idx2 = 1:length(ud.components(idx1).verbs)
                            controlNumber = controlNumber + 1;
                            saveEnableControls(controlNumber) = ud.buttonHandles(idx1).h(idx2);
                        end
                    end
                    spthelp('enter',sptoolfig,saveEnableControls,[],titleStr,helpFcn)
                else
                    spthelp('exit')
                end
        end
        
        %------------------------------------------------------------------------
        %  sptool('verb',i,j)  - i component #, j verb #
    case 'verb'
        
        %% "Time Domain" action from "Signals" component 
        if varargin{2} == 1 && varargin{3} == 1
            % Open "Signal Browser"
            signals = sptool('Signals');
            activeSignals = get(findobj('Tag', 'list1'), 'Value');
            tc1 = 'TimeContinuousAmplitudeDiscreteComplex';
            tc2 = 'TimeContinuousAmplitudeContinuousComplex';
            tc5 = 'BandpassSignal';
            toxy = 'OpticalSignalXY';
            signalBrowser = findobj('Tag', 'SignalBrowser');
            if isempty(signalBrowser)
                if (strcmp(signals(activeSignals(1)).type, tc1) ||...
                    strcmp(signals(activeSignals(1)).type, tc2) ||...
                    strcmp(signals(activeSignals(1)).type, tc5) ||...
                    strcmp(signals(activeSignals(1)).type, toxy)||...
                    strcmp(signals(activeSignals(1)).type, 'PhotonStreamXY'))
                    if (strcmp(signals(activeSignals(1)).type, toxy) ||...
                        strcmp(signals(activeSignals(1)).type, 'PhotonStreamXY'))    
                       viewComplexXY; 
                    else
                       viewComplex;
                    end
                else
                    defaultVerb(varargin{2}, varargin{3});
                    changeLinewidth;
                    changeTimeWindow;
                    % Set global variable "choice"
                    setGlobalchoice(0);
                end
            else
                if strcmp(signals(activeSignals(1)).type, tc1) || strcmp(signals(activeSignals(1)).type, tc2) || strcmp(signals(activeSignals(1)).type, tc5)
                    viewComplex;
                end
            end           
        end
        %%
        
        %% "View" action (impulse response) from "Filters" component 
        if varargin{2} == 2 && varargin{3} == 1
            % Create figure
            f = findobj('Tag', 'figImp');
            if isempty(f) % New figure
                f = figure; set(f, 'Tag', 'figImp', 'numbertitle', 'off', 'Name', 'Filter Browser'); 
                axis(gca, [-1 1 -1 1]); set(gca, 'box', 'on'); currAxes = get(f, 'CurrentAxes');
            else % Clear axes
                currAxes = get(f, 'CurrentAxes');
                cla(currAxes); legend(currAxes, '');
            end
            % Plots
            filters = sptool('Filters');
            activeFilters = get(findobj('Tag', 'list2'), 'Value');
            matrixColors = [0 0 0; 0 0 1; 0 1 0; 1 0 0; 0.5 0 0.5; 0.8 0.8 0; 0 1 1; 0.8 0 0.8; 1 0.5 1; 0.42 0.42 0.42; 0.44 0.7 0.15; 1 0.65 0; 0.1 0.1 0.44; 0.34 0.42 0.19; 0 0.77 0.80; 0.65 0.17 0.17]; 
            lw = 1.5;
            tMinVect = zeros(1, length(activeFilters));
            tMaxVect = tMinVect;
            ampMinVect = zeros(1, length(activeFilters));
            ampMaxVect = ampMinVect;
            for k = 1:length(activeFilters)
                tVect = filters(activeFilters(k)).t;
                impVect = filters(activeFilters(k)).imp;
                tMinVect(k) = min(tVect);
                tMaxVect(k) = max(tVect);
                ampMinVect(k) = min(impVect);
                ampMaxVect(k) = max(impVect);
                hold(currAxes, 'on');
                %randColor = [rand(1, 1) rand(1, 1) rand(1, 1)];
                filterLabel = filters(activeFilters(k)).label;
                filterInd = findcstr({filters.label}, filterLabel); % Filter index
                plot(currAxes, tVect, impVect, 'Color', matrixColors(filterInd, :), 'Linewidth', lw);
                %plot(currAxes, tVect, impVect, 'Color', randColor, 'Linewidth', lw);
            end 
            % Axes
            sum = (max(ampMaxVect) - min(ampMinVect))/10;
            axis(currAxes, [min(tMinVect) max(tMaxVect) min(ampMinVect)-sum max(ampMaxVect)+sum]);
            % Grid
            grid(currAxes, 'on');
            % Labels
            fz = 12;
            xlabel(currAxes, 't (s)', 'FontSize', fz);
            ylabel(currAxes, 'h (t)', 'FontSize', fz);
            % Legends
            str = cell(1, length(activeFilters));
            for k = 1:length(activeFilters)
                str(k) = {strjoin(strsplit(filters(activeFilters(k)).label, '_'), ' ')};
            end
            legend(currAxes, str);
            % Set global variable "choice2"
            setGlobalchoice2(0);   
        end
        %%
        
        %% "Frequency Response" action from "Filters" component  
        if varargin{2} == 2 && varargin{3} == 2
            % Create figure
            f = findobj('Tag', 'figFreq');
            if isempty(f) % New figure
                f = figure; set(f, 'Tag', 'figFreq', 'numbertitle', 'off', 'Name', 'Filter Browser (frequency domain)'); 
                sub1 = subplot(2,1,1); set(sub1, 'Tag', 'axesModH', 'box', 'on'); 
                sub2 = subplot(2,1,2); set(sub2, 'Tag', 'axesPhaseH', 'box', 'on');
            else % Clear axes
                sub1 = findobj('Tag', 'axesModH');
                sub2 = findobj('Tag', 'axesPhaseH');
                cla(sub1); legend(sub1, '');
                cla(sub2); legend(sub2, '');
            end
            % Plots
            format long;
            filters = sptool('Filters');
            activeFilters = get(findobj('Tag', 'list2'), 'Value');
            matrixColors = [0 0 0; 0 0 1; 0 1 0; 1 0 0; 0.5 0 0.5; 0.8 0.8 0; 0 1 1; 0.8 0 0.8; 1 0.5 1; 0.42 0.42 0.42; 0.44 0.7 0.15; 1 0.65 0; 0.1 0.1 0.44; 0.34 0.42 0.19; 0 0.77 0.80; 0.65 0.17 0.17]; 
            phaseOptionr = getGlobalphaseOption;
            lw = 1.5;
            fMinVect = zeros(1, length(activeFilters));
            fMaxVect = fMinVect;
            ampMinVect = zeros(1, length(activeFilters));
            ampMinVect2 = zeros(1, length(activeFilters));
            ampMaxVect = ampMinVect; 
            ampMaxVect2 = ampMinVect2;
            for k = 1:length(activeFilters)
                N = length(filters(activeFilters(k)).imp);
                nSecondHalf = N - find(filters(activeFilters(k)).imp == max(filters(activeFilters(k)).imp));
                nFirstHalf = N - nSecondHalf - 1;
                dt = 1/filters(activeFilters(k)).Fs;
                df = 1/(N*dt);
                fVect = [-(nFirstHalf:-1:1) 0 (1:1:nSecondHalf)].*df;
                Hdes = fftshift(fft(filters(activeFilters(k)).imp))/N; 
                H = Hdes/max(abs(Hdes));
                Hlog = 20*log10(abs(H));
                %phaseH = angle(H);
                phaseH = calculatePhase(phaseOptionr, H);
                fMinVect(k) = min(fVect);
                fMaxVect(k) = max(fVect);
                ampMinVect(k) = min(Hlog);
                ampMinVect2(k) = min(phaseH);
                ampMaxVect(k) = max(Hlog);
                ampMaxVect2(k) = max(phaseH);
                % Magnitude
                hold(sub1, 'on');
                %randColor = [rand(1, 1) rand(1, 1) rand(1, 1)];
                filterLabel = filters(activeFilters(k)).label;
                filterInd = findcstr({filters.label}, filterLabel); % Filter index
                plot(sub1, fVect, Hlog, 'Color', matrixColors(filterInd, :), 'Linewidth', lw);
                %plot(sub1, fVect, Hlog, 'Color', randColor, 'Linewidth', lw);
                % Phase
                hold(sub2, 'on'); 
                %randColor = [rand(1, 1) rand(1, 1) rand(1, 1)];
                plot(sub2, fVect, phaseH, 'Color', matrixColors(end - filterInd + 1, :), 'Linewidth', lw); 
                %plot(sub2, fVect, phaseH, 'Color', randColor, 'Linewidth', lw); 
            end 
            % Axes
            sum = (max(ampMaxVect) - min(ampMinVect))/10;
            sum2 = (max(ampMaxVect2) - min(ampMinVect2))/10;
            axis(sub1, [min(fMinVect) max(fMaxVect) min(ampMinVect)-sum max(ampMaxVect)+sum]);
            axis(sub2, [min(fMinVect) max(fMaxVect) min(ampMinVect2)-sum2 max(ampMaxVect2)+sum2]);
            % Grid
            grid(sub1, 'on');
            grid(sub2, 'on');
            % Labels
            fz = 12;
            xlabel(sub1, 'f (Hz)', 'FontSize', fz);
            ylabel(sub1, '|H (f)| (dB)', 'FontSize', fz);
            xlabel(sub2, 'f (Hz)', 'FontSize', fz);
            ylabel(sub2, 'Phase [H (f)] (rad)', 'FontSize', fz);
            % Legends
            str = cell(1, length(activeFilters));
            for k = 1:length(activeFilters)
                str(k) = {strjoin(strsplit(filters(activeFilters(k)).label, '_'), ' ')};
            end
            legend(sub1, str);
            legend(sub2, str); 
        end
        %%
        
        %% "View" action from "Spectra" component   
        if varargin{2} == 3 && varargin{3} == 1
            % flagSp
            if isempty(findobj('Tag', 'spectview'))
                flagSp = 0; % "Spectrum Viewer" window is close
            else
                flagSp = 1; % "Spectrum Viewer" window is open
            end
            % Default representation - open "Spectrum Viewer"
            defaultVerb(varargin{2}, varargin{3});
            % Change the default spectrum method
            set(findobj('Tag', 'methodPopup'), 'Value', 9, 'Enable', 'off');
            % Apply action 
            activeSpectra = get(findobj('Tag', 'list3'), 'Value');
            N = length(activeSpectra);
            for k = 1:N
                spectview('apply', activeSpectra(k)); 
            end
            % "Spectrum Viewer" modifications
            if flagSp == 0
                modifyWindow;
            end
            % "Spectra" handles modifications
            modifySpectraHandles;
            % Set global variable "choice3"
            setGlobalchoice3(0);
        end 
        %%
        
        %------------------------------------------------------------------------
        %  sptool('list',i)  - i component #     LISTBOX CALLBACK
    case 'list'
        
        %% List from "Signals" component
        if varargin{2} == 1
            signalBrowser = findobj('Tag', 'SignalBrowser');
            if ~isempty(signalBrowser)
                if strcmp(get(signalBrowser, 'Visible'), 'off') && isempty(findobj('Tag', 'figCompRI')) && isempty(findobj('Tag', 'figCompModPhase')) && isempty(findobj('Tag', 'figCompMod2Phase'))
                    close(signalBrowser);
                    signalBrowser = findobj('Tag', 'SignalBrowser');
                end
            end
            % Close old figures
            closeComplexFigures;
            close(findobj('Tag', 'Eye Diagram'));
            % Get global variable "choice"
            choicer = getGlobalchoice;
            % Activate "list1"
            activateList1;
            signals = sptool('Signals');
            activeSignals = get(findobj('Tag', 'list1'), 'Value');
            tc1 = 'TimeContinuousAmplitudeDiscreteComplex';
            tc2 = 'TimeContinuousAmplitudeContinuousComplex';
            tc5 = 'BandpassSignal';
            if isempty(signalBrowser) 
                if length(getGlobalchoice) == length(choicer)
                    if getGlobalchoice == choicer
                        sptool('verb', 1, 1);
                    end
                end
            else
                % Delete old "stems"
                delete(findobj('Tag', 'st1'));
                % View
                if strcmp(signals(activeSignals(1)).type, tc1) || strcmp(signals(activeSignals(1)).type, tc2) || strcmp(signals(activeSignals(1)).type, tc5) 
                    viewComplex(1);
                else
                    sptoolfig = findobj(0, 'Tag', 'sptool');
                    ud = get(sptoolfig, 'UserData');
                    idx = varargin{2};
                    if 0  % short cut (double click) is disabled for now
                        % if strcmp(get(sptoolfig,'SelectionType'),'open')
                        dc = ud.components(idx).defaultClient;
                        whichClient = 0;
                        k = 0;
                        while ~whichClient
                            k = k + 1;
                            if strcmp(dc,ud.components(idx).verbs(k).owningClient)
                                whichClient = k;
                            end
                        end
                        sptool('verb', idx,whichClient);
                    else
                        selectionChanged(sptoolfig, 'value')
                    end
                    % Change linewidth/linestyle and time window
                    changeLinewidth;
                    changeTimeWindow;
                end
            end
            if isempty(get(findobj('Tag', 'list2'), 'Value')) || get(findobj('Tag', 'list2'), 'Value') == 0
                set(findobj('Tag', 'filtdes:create'), 'Enable', 'off');
            end
        end
        %%
        
        %% List from "Filters" component
        if varargin{2} == 2
            set(findobj('Tag', 'filtview:view'), 'Enable', 'on');
            set(findobj('Tag', 'filtdes:create'), 'Enable', 'on');
            % Get global variable "choice2"
            choice2r = getGlobalchoice2;
            % Set global variable "choice2"
            setGlobalchoice2(get(findobj('Tag', 'list2'), 'Value'));
            if isempty(findobj('Tag', 'figImp')) 
                if length(getGlobalchoice2) == length(choice2r)
                    if getGlobalchoice2 == choice2r
                        sptool('verb', 2, 1);
                    end
                end
            else
                sptool('verb', 2, 1);
            end 
            if ~isempty(findobj('Tag', 'figFreq'))
                sptool('verb', 2, 2);
            end
            % If list2 is empty
            if isempty(get(findobj('Tag', 'list2'), 'Value'))
                set(findobj('Tag', 'filtview:view'), 'Enable', 'off');
                set(findobj('Tag', 'filtdes:create'), 'Enable', 'off'); 
            end
        end
        %%
        
        %% List from "Spectra" component
        if varargin{2} == 3
            set(findobj('Tag', 'spectview:view'), 'Enable', 'on');
            % Get global variable "choice3"
            choice3r = getGlobalchoice3;
            % Set global variable "choice3"
            setGlobalchoice3(get(findobj('Tag', 'list3'), 'Value'));
            if isempty(findobj('Tag', 'spectview')) 
                if length(getGlobalchoice3) == length(choice3r)
                    if getGlobalchoice3 == choice3r
                        sptool('verb', 3, 1);
                    end
                end
            else
                sptool('verb', 3, 1);
            end 
            % If list3 is empty
            if isempty(get(findobj('Tag', 'list3'), 'Value'))
                 set(findobj('Tag', 'spectview:view'), 'Enable', 'off');
            end
        end 
        %%
        
        %----------------------------------------------------------------------------
        % struct = sptool('changedStruc',sptoolfig) - return recently
        %  changed structure (removed, imported, name or Fs changed)
        %  sptoolfig - optional, found with findobj if not present
    case 'changedStruc'
        if nargin < 2
            shh = get(0,'ShowHiddenHandles');
            set(0,'ShowHiddenHandles','on')
            sptoolfig = findobj(0,'Tag','sptool');
            set(0,'ShowHiddenHandles',shh);
        else
            sptoolfig = varargin{2};
        end
        ud = get(sptoolfig,'UserData');
        varargout{1} = ud.changedStruc;
        
        %----------------------------------------------------------------------------
        % Fs  = sptool('commonFs)
    case 'commonFs'
        % For sampling frequency, if there are filters in the SPTool,
        % use their common sampling frequency (or the last one if
        % they don't have one).  If there no filters, do the same
        % for Signals.  If no signals either, use Fs = 1.
        ftemp = sptool('Filters');
        if ~isempty(ftemp)
            Fs = ftemp(end).Fs;
            if isempty(Fs)
                Fs = 1;
            end
        else
            stemp = sptool('Signals');
            if ~isempty(stemp)
                Fs = stemp(end).Fs;
            else
                Fs = 1;
            end
        end
        varargout{1} = Fs;
        
        %----------------------------------------------------------------------------
        %  Client API
        %  can ask for the currently selected objects
        % [s,ind] = sptool(componentName,allFlag)
        % [s,ind] = sptool(componentName,allFlag,fig)
        % Returns a structure array of the current data items in the SPTool
        %   Inputs:
        %      componentName - string; name of component e.g. 'Filters' or 'Signals'
        %      allFlag - 1 ==> return all objects of the requested type in s
        %                0 ==> return only the currently selected objects in s
        %                allFlag is optional; it defaults to 1
        %      fig - figure handle of SPTool; optional - if omitted, will be found with findobj
        %   Outputs:
        %       s - structure array
        %       ind - optional output; indices of s which are currently selected
        %             in SPTool
    otherwise
        if nargin < 3
            shh = get(0,'ShowHiddenHandles');
            set(0,'ShowHiddenHandles','on')
            sptoolfig = findobj(0,'Tag','sptool');
            set(0,'ShowHiddenHandles',shh);
        else
            if isempty(varargin{3}) || ishandle(varargin{3})
                sptoolfig = varargin{3};
            else
                error(message('signal:sptool:GUIErrInvalidInput', '''help sptool'''))
            end
        end
        
        if isempty(sptoolfig)
            error(message('signal:sptool:GUIErrSptoolNotOpen', 'SPTool'))
        end
        
        ud = get(sptoolfig,'UserData');
        l = {ud.components.name};
        whichComponent = findcstr(l,action);
        if isempty(whichComponent)
            error(message('signal:sptool:GUIErrNoComponentAvailable', action, sprintf( '''%s'' ', l{ : } )));
        end
        if nargin > 1
            allFlag = varargin{2};
        else
            allFlag = 1;
        end
        
        varargout{1} = ud.session{whichComponent};
        ind = get(ud.list(whichComponent),'Value');
        if allFlag
            varargout{2} = ind;
        else
            if isempty(varargout{1})
                varargout = { [] [] };
            else
                varargout = {varargout{1}(ind) 1:length(ind)};
            end
        end
        
end


%-------------------------------------------------------------------------
function ud = init_sptool_data_settings
% INIT_SPTOOL_DATA_SETTINGS Build the preference structure to create
%                           uicontrols for the preference dialog window.
%                           Also create the "factory default" settings in
%                           the .controls field.

ud.prefs = sptprefp;        % SPTool Preference Registry - Private directory

% Loop through each panel of preferences
ud.fdwarnflag = 0;
for i=1:length(ud.prefs)
    p = getsigpref(ud.prefs(i).panelName);
    if ~isempty(p)
        ud.prefs(i).currentValue = struct2cell(p);
        % check to make sure currentValue has the correct number of
        % elements.  If it doesn't then the preferences must be out
        % of date, so set the current value to the factory setting.
        if length(ud.prefs(i).currentValue) ~= length(ud.prefs(i).controls(:,1))
            ud.prefs(i).currentValue = ud.prefs(i).controls(:,7);
            warning(message('signal:sptool:InvalidPreferences', ud.prefs( i ).panelDescription));
        end
        if isfield(p,'filterDesignerFlag')
            if p.filterDesignerFlag==1
                ud.fdwarnflag = 1;
            end
        end
    end
end
allPanels = {ud.prefs.panelName};
plugInd = findcstr(allPanels,'plugins');
plugPrefs = cell2struct(ud.prefs(plugInd).currentValue,ud.prefs(plugInd).controls(:,1));
if plugPrefs.plugFlag
    % add any additional preferences
    ud.prefs = sptool('callall','sptpref',ud.prefs);
end

ud.panelInd = 1;
ud.components = [];
ud.components = sptcompp(ud.components); % calls one in signal/private
if plugPrefs.plugFlag
    % now call each one found on path:
    ud.components = sptool('callall','sptcomp',ud.components);
end

ud.sessionName = 'untitled.spt'; 
ud.wd = pwd;           % working directory for Opening, Saving, etc.
ud.importwd = pwd;           % working directory for importing file data
ud.exportwd = pwd;           % working directory for exporting data to disk
ud.savedFlag = 0;            % flag indicating if session has ever been saved
ud.unchangedFlag = 1;        % indicates if sess. is unchanged since last save

% the 'changeStruc' field will contain the structure which has been
% altered by the most recent operation, as it was before the operation.
% operations include: clear, name change, import over (such as
% applying a filter), sampling frequency change
ud.changedStruc = [];
ud.importSettings = [];
ud.sessionPath = '';
ud.pointer = 1;


%-------------------------------------------------------------------------
function [fig,ud] = create_sptool_gui(ud)
% CREATE_SPTOOL_GUI Creates the GUI for SPTool - the data manager

numComponents = length(ud.components);
maxVerbs = 1;
for i=1:numComponents
    maxVerbs = max(maxVerbs,length(ud.components(i).verbs));
end
ud.maxVerbs = maxVerbs;

% Determine figure position
figHeight   = ud.maxVerbs*20+240;
screenRect  = get(0,'ScreenSize');
fp = [18 screenRect(4)-figHeight-50 140*numComponents figHeight];

figname = sprintf('%s: %s',getString(message('signal:sptoolgui:SPTool')),ud.sessionName);

uibgcolor = get(0,'DefaultUicontrolBackgroundColor');

% CREATE FIGURE
fig = figure('CreateFcn','',...
    'CloseRequestFcn','sptool(''close'')',...
    'WindowStyle','normal',...
    'Tag','sptool',...
    'NumberTitle','off',...
    'IntegerHandle','off',...
    'Units','pixels',...
    'Position',fp,...
    'MenuBar','none',...
    'Color',uibgcolor,...
    'InvertHardcopy','off',...
    'PaperPositionMode','auto',...
    'Visible','off',...
    'Name',figname);

for i=1:numComponents
    maxVerbs = max(maxVerbs,length(ud.components(i).verbs));
    ud.list(i) = uicontrol('Style','listbox','BackgroundColor','w',...
        'Units','pixels',...
        'Interruptible','off',...
        'Callback',['sptool(''list'',' num2str(i) ')'],...
        'Value',[],'Tag',['list' num2str(i)]);
    if ud.components(i).multipleSelection
        set(ud.list(i),'Max',2)
    end
    ud.label(i) = uicontrol('Style','text','String', ...
        getTranslatedString('signal:sptoolgui',ud.components(i).name),...
        'Tag',['list' num2str(i) '_txt'],...
        'Units','pixels',...
        'FontWeight','bold');
    for j=1:length(ud.components(i).verbs)
        ud.buttonHandles(i).h(j) = uicontrol('String',...
            ud.components(i).verbs(j).buttonLabel,...
            'Units','pixels',...
            'Interruptible', 'off',...
            'BusyAction', 'cancel',...
            'Callback',['sptool(''verb'',' num2str(i) ',' num2str(j) ')'],...
            'Tag',[ud.components(i).verbs(j).owningClient ':' ...
            ud.components(i).verbs(j).action]);
    end
end

% ====================================================================
% MENUs
%  create cell array with {menu label, callback, tag}

%  MENU LABEL                     CALLBACK                      TAG
fm={
    getString(message('signal:sptoolgui:File'))                              ' '                        'filemenu'
    ['>' getString(message('signal:sptoolgui:OpenSession')) '...' '^o']               'sptool(''open'')'         'loadmenu'
    '>------'                           ' '                        ' '
    ['>' getString(message('signal:sptoolgui:Import')) '...' '^i']                     'sptool(''import'')'       'importmenu'
    ['>' getString(message('signal:sptoolgui:Export')) '...' '^e']                     'sptool(''export'')'       'exportmenu'
    '>------'                           ' '                        ' '
    ['>' getString(message('signal:sptoolgui:SaveSessions')) '^s']                  'sptool(''save'');'        'savemenu'
    ['>' getString(message('signal:sptoolgui:SaveSessionAs')) '...']               'sptool(''saveas'');'       'saveasmenu'
    '>------'                           ' '                        ' '
    ['>' getString(message('signal:sptoolgui:Preferences')) '...']                   'sptool(''pref'') '        'prefmenu'
    ['>' getString(message('signal:sptoolgui:CloseMenu')) '^w']                         'sptool(''close'')'        'closemenu'};

ud.filemenu_handles = makemenu(fig, char(fm(:,1)),char(fm(:,2)), char(fm(:,3)));
for idx = 4:6
    set(ud.filemenu_handles(idx),'Enable','off');
end


%  MENU LABEL                     CALLBACK                      TAG
em={
    getString(message('signal:sptoolgui:Edit'))                             'sptool(''edit'')'           'editmenu'
    ['>' getString(message('signal:sptoolgui:Duplicate'))]                       'sptool(''edit'')'           'dupmenu'
    ['>' getString(message('signal:sptoolgui:Clear'))]                           'sptool(''edit'')'           'clearmenu'
    '>------'                          ' '                          ' '
    ['>' getString(message('signal:sptoolgui:Name')) '...']                         'sptool(''edit'')'           'newnamemenu'
    ['>' getString(message('signal:sptoolgui:SamplingFrequency')) '...']           'sptool(''edit'')'           'freqmenu'};

ud.editmenu_handles = makemenu(fig, char(em(:,1)),char(em(:,2)), char(em(:,3)));
ud.dupsub(1) = uimenu(ud.editmenu_handles(2),'Label',' ',...
    'Tag','dupmenu','Callback',['sptool(''duplicate'',',int2str(1),')']);
ud.clearsub(1) = uimenu(ud.editmenu_handles(3),'Label',' ',...
    'Tag','clearmenu','Callback',['sptool(''clear'',',int2str(1),')']);
ud.namesub(1) = uimenu(ud.editmenu_handles(4),'Label',' ',...
    'Tag','newnamemenu','Callback',['sptool(''newname'',',int2str(1),')']);
ud.freqsub(1) = uimenu(ud.editmenu_handles(5),'Label',' ',...
    'Tag','freqmenu','Callback',['sptool(''freq'',',int2str(1),')']);
for idx = 2:5
    set(ud.editmenu_handles(idx),'Enable','off');
end

matlab.ui.internal.createWinMenu(fig);

% Build the cell array string for the Help menu
hm=sptooldatamanager_help;

ud.helpmenu_handles = makemenu(fig, char(hm(:,1)),char(hm(:,2)), char(hm(:,3)));

ud.session = cell(numComponents,1);
for i=1:numComponents
    ud.session{i} = [];
end

%-------------------------------------------------------------------------
function mh = sptooldatamanager_help
% Set up a string cell array that can be passed to makemenu to create the Help
% menu for SPTool's data manager.

% Define specifics for the Help menu in SPTool's data manager.
toolname      = getString(message('signal:sptoolgui:SPTool'));
toolhelp_cb   = 'sptool(''help'',''overview'')';
toolhelp_tag  = 'helpoverview';
whatsthis_cb  = 'sptool(''help'',''mouse'')';
whatsthis_tag = 'helpmouse';

% Add other Help menu choices that are common to all SPTool clients.
mh=sptool_helpmenu(toolname,toolhelp_cb,toolhelp_tag,whatsthis_cb,whatsthis_tag);

%-------------------------------------------------------------------------
function updateLists(fig,componentNum)
%updateLists  - creates listbox strings for all components
%   based on ud.session

if nargin<1
    fig = findobj(0,'Tag','sptool');
end
ud = get(fig,'UserData');
if nargin<2
    componentNum = 1:length(ud.components);
end

for i=componentNum
    listStr = cell(1,length(ud.session{i}));
    for j=1:length(ud.session{i})
        listStr{j} = ...
            [ud.session{i}(j).label ' [' getTranslatedString('signal:sptoolgui',ud.session{i}(j).type) ']'];
    end
    if isempty(listStr)
        set(ud.list(i),'Value',[])
    elseif length(listStr)<max(get(ud.list(i),'Value'))
        set(ud.list(i),'Value',1)
    end
    set(ud.list(i),'String',listStr)
end


%-------------------------------------------------------------------------
function ind = findStructWithLabel(structArray,label)
% ind = findStructWithLabel(structArray,label)
% returns the index of the (unique) structure element in structArray with
% field .label equal to the string argument label.

if isempty(structArray)
    ind = [];
else
    l = {structArray.label};
    ind = findcstr(l,label);
end


%-------------------------------------------------------------------------
function selectionChanged(fig,msg)
%selectionChanged
%   enables / disables all verb buttons based on listbox values;
%   in the process the client tools have a chance to update themselves
%   based on the selection
%   msg - string; will be passed to the clients as well
%     possibilities: 'new' - new entries to list, or change in current entries - default
%                    'value' - new listbox value, no other change
%                    'Fs' - sampling freq of object in current selection changed
%                    'label' - label of an object in the current selection changed
%                    'dup' - an object has been duplicated
%                    'clear' - an object has been deleted

if nargin<1
    fig = findobj(0,'Tag','sptool');
end
if nargin<2
    msg = 'new';
end
ud = get(fig,'UserData');
for i=1:length(ud.components)
    for j=1:length(ud.components(i).verbs)
        enable = feval(ud.components(i).verbs(j).owningClient,'selection',...
            ud.components(i).verbs(j).action,msg,fig);
        set(ud.buttonHandles(i).h(j),'Enable',enable)
    end
end


%-------------------------------------------------------------------------
function continuevar = saveChangesPrompt(sessionName,operation)
% continuevar = saveChangesPrompt(sessionName,operation)
%    Informs user via a dialog box that the SPTool session
%    with name sessionName has been changed.
%    The user then has the choice of
%      1) saving the session
%      2) not saving the session
%      3) cancelling the operation
%    continuevar will be true (1) if the session was saved successfully
%    or the Don't save button was pressed (option 2 above).
% operation should be a string indicating the operation being
% performed.  It should be either 'closing' or 'opening'.  This
% string will actually appear in the dialog box.
%
%    Note: userdata will be changed if session is saved.

continuevar = 1;
% Compose the string with the operation variable with [ ] instead of
% sprintf to make it easier to translate. g424968
switch operation
    case 'opening'
        str = getString(message('signal:sptoolgui:SaveChangesToSPToolSessionOpening',sessionName));
    case 'closing'
        str = getString(message('signal:sptoolgui:SaveChangesToSPToolSessionClosing',sessionName));
end

ButtonName = questdlg(str,...
    getString(message('signal:sptoolgui:UnsavedSession')), ...
    getString(message('signal:sptoolgui:Save')), ...
    getString(message('signal:sptoolgui:DontSave')), ...
    getString(message('signal:sptoolgui:Cancel')), ...
    getString(message('signal:sptoolgui:Save')));
switch ButtonName
    case getString(message('signal:sptoolgui:Save'))
        if sptool('save')
            continuevar = 0;
        end
    case getString(message('signal:sptoolgui:Cancel'))
        continuevar = 0;
end


%-------------------------------------------------------------------------
function errObj = isvalidstruc(struc,ImportFcns,compNames)
% ISVALIDSTRUC returns an empty string if all the fields in the given
% structure are valid, otherwise it returns a string containing an error
% message.
% Inputs:
%   struc - component structure
%   ImportFcs - structure containing the import functions
%   compNames - component names
% Outputs:
%   errstr - error message if structure is invalid

errObj = [];
if ~isfield(struc,'label') || ~isstr(struc.label) || isempty(struc.label)
    errObj = message('signal:sptool:EmptyLabel');
    
elseif ~isvalidvar(struc.label)
    errObj = message('signal:sptool:InvalidVarName');
    
elseif isfield(struc,'SPTIdentifier') && (length(struc) == 1) && ...
        isfield(struc.SPTIdentifier,'type')
    i = find(strcmp(struc.SPTIdentifier.type,compNames));
    if ~isempty(i)
        [valid,struc] = feval(ImportFcns(i).importFcn,'valid',struc);
        if ~valid
            errObj = message('signal:sptool:InvalidStruct');
        end
    else
      compName = struc.SPTIdentifier.type;
      errObj = message('signal:sptool:InvalidComponent',compName,sprintf('      ''%s''\n',compNames{:}));
    end
end


%-------------------------------------------------------------------------
function [session_out,msgstr] = sptvalid(session_in,components)
% [session,msgstr] = sptvalid(session,components)
%   "Validates" a session by updating all structs, and removing any
%   that are invalid.
%   This function will ensure that the elements in the input session
%   cell array match the components in the input components array, if
%   necessary by reordering, removing, or adding empty elements to
%   the session struct.
% Inputs:
%   session - cell array, each element is a vector of SPTool structs
%   components - structure array, one element for each element in
%      session, specifies the component of the corresponding element in session
% Outputs:
%   session - updated session cell array
%   msgstr - empty if session has not been changed, otherwise contains a
%            message saying that the input session file was not up-to-date
%            and will need to be saved.
%            If not empty, this message will be displayed in a dialog box

invalidList = {};
updatedList = {};

session_out = session_in;

for i=length(session_out):-1:1
    % removed bad structure arrays:
    if ~isempty(session_out{i}) && ~isfield(session_out{i},'SPTIdentifier')
        session_out(i) = [];
    elseif ~isempty(session_out{i}) && ...
            ~isfield(session_out{i}(1).SPTIdentifier,'type')
        session_out(i) = [];
    end
end

sessionTypes = cell(1, length(session_out));
for i=1:length(session_out)
    if ~isempty(session_out{i})
        sessionTypes{i} = session_out{i}(1).SPTIdentifier.type;
    else
        sessionTypes{i} = '';
    end
end
componentTypes = {components.structName};

% find and remove sessions not in components:
[C,i] = setdiff(sessionTypes,componentTypes);
if ~isempty(i),
    for ii=sort(-i)
        invalidList = addinvalid(invalidList,session_out{-ii});
    end
end

session_out(i) = [];
sessionTypes(i) = [];

% find components not in sessions, and add empty lists for those components
session_temp = session_out;
session_out = cell(length(components),1);
[C,ia,ib] = intersect(char(sessionTypes),char(componentTypes),'rows');
session_out(ib) = session_temp(ia);

% now use importfcn of each component to validate / update each
% structure
for i=1:length(session_out)
    objArray = [];
    for j=1:length(session_out{i})
        [valid,s] = feval(components(i).importFcn,'valid',session_out{i}(j));
        if valid
            if isempty(objArray)
                objArray = s;
            else
                objArray(end+1) = s;
            end
            if ~isequal(s,session_out{i}(j))
                updatedList = addinvalid(updatedList,session_out{i}(j));
            end
        else
            invalidList = addinvalid(invalidList,session_out{i}(j));
        end
        % disp(session_out{i}(j).label)
    end
    session_out{i} = objArray;
end

% for each cell
for i=1:length(session_out)
    
    % if the cell is not empty, then compare individual cells of session_in & session_out
    if ~isempty(session_out{i}) && ~isequal(session_in{i},session_out{i})
        if isempty(invalidList)
            invalidList = {'<none>'};
        end
        if isempty(updatedList)
            updatedList = {'<none>'};
        end
        msgstr = sprintf(...
            [getString(message('signal:sptoolgui:ThisSessionFileWasOutofdateOrContainedInvalid')) '\n' ...
            getString(message('signal:sptoolgui:SPToolStructures')) '\n' ...
            '\n' ...
            getString(message('signal:sptoolgui:UpdatedStructures')) ': %s\n'...
            getString(message('signal:sptoolgui:InvalidStructures')) ': %s\n' ...
            '\n'...
            getString(message('signal:sptoolgui:TheSessionWillNeedToBeSaved'))],...
            sprintf('%s ',updatedList{:}),sprintf('%s ',invalidList{:}));
        
        waitfor(msgbox(msgstr,...
            getString(message('signal:sptoolgui:SessionChanges')),'error','modal'))
    else
        msgstr = '';
    end
    
end

%-------------------------------------------------------------------------
function invalidList = addinvalid(invalidList,struc)
if ~isempty(struc)
    invalidList{end+1} = [struc.label '(' struc.SPTIdentifier.type ')'];
else
    invalidList{end+1} = '';
end


%-------------------------------------------------------------------------
function [formIndx, formTags] = formIndex(fields,form)
% FORMINDEX determine what the 'form' is for a given component.  Some
% components have different forms. For example filters can be entered as a
% transfer function (tf), state space (ss), second-order sections (sos), or
% zero-pole gain (zpk).
% Inputs:
%   fields - the component structure fields
%   form - the third input argument to sptool which, depending on the
%          component, could be the 'form' which describes how the component
%          was entered eg. for filters it's: 'tf','ss','sos', or 'zpk'
% Outputs:
%   formIndx - a scalar indicating which form was used when entering the
%              component
%   formTags - a list of strings which are possible forms

formTags = [];
if length(fields) == 1       % Component only has one form
    formIndx = 1;
else                         % Component has multiple forms
    for i=1:length(fields)
        formTag = fields(i).formTag;
        formTags = [formTags '''' formTag '''' ' '];
    end
    
    formIndx = [];
    if ~isempty(form)  % Form was specified
        if ischar(form)
            formIndx = find(strcmp(form,{fields.formTag}));
        elseif length(form) == 1 && find(form==[1:length(fields)])
            % Using numbers to specify form
            formIndx = form;
        end
    end
end


%-------------------------------------------------------------------------
function [f,p] = sptuiputfile(sessionName,dlgboxTitle)
% SPTUIPUTFILE The following is a workaround for uiputfile which works
%              differently on the PC, UNIX and MAC.
% PCWIN: uiputfile adds the extension as specified in 'Save as Type'
%        and checks if the file exists.
% UNIX:  uiputfile DOES NOT add the extension as specified in the
%        'Filter', however it DOES check if the file, with the
%        extension specified in the 'Filter', exists.
% MAC:   uiputfile checks if the file name, as entered, exists
%
% Inputs:
%   sessionName - string containing the default session name
%   dlgboxTitle - string containing the title of the dialog box
% Outputs:
%   f - string containing the file name with extension .spt
%   p - string containing the path to file

[f,p] = uiputfile(sessionName,dlgboxTitle);

if ~isequal(f,0) && ~strcmp(computer,'PCWIN') && ...
        ~isequal(f(end-3:end),'.spt')
    
    % UNIX and MAC's uiputfile DOES NOT automatically add
    % extensions to file names
    if isempty(findstr(f,'.'))
        % no '.' extension, so add '.spt' to file name
        f = [f '.spt'];
    end
    
    % On the MAC only... after adding the extension to the file name we
    % must check again if the file exists since the MAC's uiputfile
    % only checks the file name as entered (eg without the extension)
    tYes = getString(message('signal:sptoolgui:Yes')); % Translating string 'Yes'
    tNo = getString(message('signal:sptoolgui:No')); % Translating string 'No'
    ButtonName = tNo;
    while ~isequal(f,0) && strcmp(computer,'MAC2') && strcmp(ButtonName,tNo)
        if exist(f,'file') == 2
            question=[p,f,getString(message('signal:sptoolgui:ThisFileAlreadyExistsReplaceExistingFile'))];
            ButtonName = questdlg(question,...
                getString(message('signal:sptoolgui:SaveSessions')),...
                tYes,tNo,tNo);  % Default is 'No'
        else
            ButtonName = tYes;
        end
        if strcmp(ButtonName,tNo)
            [f,p]=uiputfile(f,dlgboxTitle);
            if ~isequal(f,0) && isempty(findstr(f,'.'))
                % no '.' extension, so add '.spt' to file name
                f = [f '.spt'];
            else
                %file name with extension has been entered; exit while-loop
                ButtonName = tYes;
            end
        end
    end   % while-loop
end   % if not PCWIN and no extension specified


% %-------------------------------------------------------------------------
function cacheclasses
% Cache some of the classes required to launch FVTool into memory. This
% will reduce the launch for FVTool (while increasing it for SPTool).

stpk = findpackage('sigtools');
findclass(stpk,'fvtool');

sgpk = findpackage('siggui');
findclass(sgpk,'fvtool');

dpk = findpackage('dfilt');
findclass(dpk,'df1');
findclass(dpk,'dfiltwfs');

%-------------------------------------------------------------------
% Added for FDATool - prompt to switch to FDATool and convert format
%-------------------------------------------------------------------
function udnew = promptForFDA(ud, filtdesPrefs, forceFDAformat)

str = { getString(message('signal:sptoolgui:FiltersInThisSessionWereCreatedWithFilterDesignerAndAre'))
    ''
    getString(message('signal:sptoolgui:SelectConvertIfYouWantToSetFDAToolAsThePreferredFilterD'))
    ''
    getString(message('signal:sptoolgui:NoteConvertingToAnFDAToolFormatCannotBeUndone'))
    ''
    getString(message('signal:sptoolgui:SelectDoNotUseFDAToolToContinueUsingFilterDesigner'))
    ''
    getString(message('signal:sptoolgui:NoteFilterDesignerWillBeRemovedInAFutureRelease'))
    ''};

sptoolfig = findobj(0,'Tag','sptool');
tConvert = getString(message('signal:sptoolgui:Convert'));
tDonot = getString(message('signal:sptoolgui:DoNotUseFDATool'));

if isequal(forceFDAformat, 1)
    answer = tConvert;
else
    answer = questdlg(str,getString(message('signal:sptoolgui:ConvertFiltersToUseFDATool')),tConvert,tDonot,tConvert);
end

switch answer
    case tConvert
        %       OK, convert filters to FDA format
        filters = ud.session{2};
        for idx = 1:length(filters)
            if ~isfield(filters(idx),'FDAspecs') || isempty(filters(idx).FDAspecs)
                filters(idx).FDAspecs.sidebar.design = getfdaformat(filters(idx));
                currentModule = filters(idx).specs.currentModule;
                %remove the spec area with only currentModule remained.
                filters(idx).specs = [];
                filters(idx).specs.currentModule = currentModule;
            end
        end
        ud.session{2} = filters;
        ud.unchangedFlag = 0;
        ud.savedFlag = 0;
        set(sptoolfig,'UserData', ud);
    case tDonot
        %       User wants to stay with old filter designer and we
        %       set the preference accordingly
        filtdesPrefs.fdatoolFlag = false;
        filtdesPrefs.filterDesignerFlag = true;
        sptool('setprefs','filtdes',filtdesPrefs, sptoolfig);
    otherwise
        %         force to make selection if user clicks x to try to quit.
        promptForFDA(ud, filtdesPrefs, 0);
end
udnew = ud;

%-------------------------------------------------------------------
% Added for FDATool - prompt to switch to Filter Designer and remove
% filters
%-------------------------------------------------------------------
function promptForRemoveFilters(filtdesPrefs)
str = { getString(message('signal:sptoolgui:FiltersInThisSessionWereCreatedWithFDAToolAndAreNotComp'))
    ''
    getString(message('signal:sptoolgui:DoYouWantToUseFilterDesignerAndDeleteYourCurrentFilters'))
    ''
    getString(message('signal:sptoolgui:NoteFilterDesignerWillBeRemovedInAFutureRelease'))
    ''};

switch questdlg(str,getString(message('signal:sptoolgui:DesignFiltersUsingFilterDesigner')),'Yes','No','Yes');
    case 'Yes'
        %        OK, remove all filters from list and the session
        sptool('clear' ,'clear all');
    case 'No'
        %       do nothing but change the setting back to FDAtool
        sptoolfig = findobj(0,'Tag','sptool');
        filtdesPrefs.fdatoolFlag = true;
        filtdesPrefs.filterDesignerFlag = false;
        sptool('setprefs','filtdes',filtdesPrefs, sptoolfig);
    otherwise
        %         force to make selection if user clicks x to try to quit.
        promptForRemoveFilters(filtdesPrefs);
end

%----------------------------------------------
% FDATool - find if session contains old filter
%----------------------------------------------
function out = hasSPTfilter(session)
filters = session{2};
out = 0;
for idx = 1:length(filters)
    if ~isfield(filters(idx),'FDAspecs')
        out = 1;
        return
    end
end

%--------------------------------------------------------------
% FDATool - Check if FDATool or Filter Designer is open. If so,
% close before loading session. <g292547>
%--------------------------------------------------------------
function checkFilterWindow(ud)
if (isfield(ud, 'hFDA') && ishandle(ud.hFDA))
    %     str = 'FDATool is now open and needs to be closed to load a session.';
    %     waitfor(msgbox(str, 'SPTool'));
    close(ud.hFDA, 'force');
    return;
end

filtdesFig = findobj(0,'Tag','filtdes');
if ~isempty(filtdesFig)
    %     str = 'The Filter Designer is now open and needs to be closed to load a session.';
    %     waitfor(msgbox(str, 'SPTool'));
    delete(filtdesFig);
    return;
end

function [ ] = activateList1( ~ )
%ACTIVATELIST1 updates "list1" pushbuttons and selected signals from "visualizer".
%   ACTIVATELIST1 just updates "list1" pushbuttons and selected signals.

%% Update "Time Domain" pushbutton
set(findobj('Tag', 'sigbrowser.sigbrowseAdapter:view'), 'Enable', 'on');

%% Update "list1" and complex pushbuttons
tc1 = 'TimeContinuousAmplitudeDiscreteComplex';
tc2 = 'TimeContinuousAmplitudeContinuousComplex'; 
tc5 = 'BandpassSignal';
toxy = 'OpticalSignalXY';
activeSignals = get(findobj('Tag', 'list1'), 'Value');
N = length(activeSignals);
signals = sptool('Signals');  
% Complex index
inc = 1;
indComplex = zeros(1, N);
for k = 1:N
    if strcmp(signals(activeSignals(k)).type, tc1) || strcmp(signals(activeSignals(k)).type, tc2) || strcmp(signals(activeSignals(k)).type, tc5)
        indComplex(inc) = activeSignals(k);
        inc = inc + 1;
    end
end
indComplex = indComplex(indComplex ~= 0);
if isempty(indComplex) 
    indComplex = [];
end
% Change active signals
if ~isempty(indComplex)     
    if length(indComplex) == N       
        set(findobj('Tag', 'buttonConstellation'), 'Enable', 'on');
        set(findobj('Tag', 'buttonModPhase'), 'Enable', 'on');
        set(findobj('Tag', 'buttonMod2Phase'), 'Enable','on');        
    else   
        set(findobj('Tag', 'buttonConstellation'), 'Enable', 'off');
        set(findobj('Tag', 'buttonModPhase'), 'Enable', 'off');
        set(findobj('Tag', 'buttonMod2Phase'), 'Enable', 'off'); 
        inc = 1;
        ind = zeros(1, N);
        for k = 1:N
            flag = 0;            
            for counter = 1:length(indComplex)
                if activeSignals(k) == indComplex(counter)
                    flag = 1;
                end
            end            
            if flag == 0
                ind(inc) = activeSignals(k);
                inc = inc + 1;
            end
        end
        ind = ind(ind ~= 0);
        if isempty(ind) 
            ind = [];
        end
        set(findobj('Tag', 'list1'), 'Value', ind);
        activeSignals = get(findobj('Tag', 'list1'), 'Value');
        N = length(activeSignals);
    end           
else
    set(findobj('Tag', 'buttonConstellation'),'Enable','off');
    set(findobj('Tag', 'buttonModPhase'),'Enable','off');
    set(findobj('Tag', 'buttonMod2Phase'),'Enable','off');           
end

%% Update "Eye Diagram" pushbutton
sum = 0;
for k = 1:N   
    if strcmp(signals(activeSignals(k)).type, 'TimeContinuousAmplitudeContinuousReal')
        sum = sum + 1;
    end
end
if sum == N
    set(findobj('Tag', 'buttonEyeDiagram'), 'Enable', 'on');
else
    if ~isempty(indComplex)     
        if length(indComplex) == N
            set(findobj('Tag', 'buttonEyeDiagram'), 'Enable', 'on');
        else
            set(findobj('Tag', 'buttonEyeDiagram'), 'Enable', 'off');
        end
    else
        set(findobj('Tag', 'buttonEyeDiagram'), 'Enable', 'off');
    end
end

%% Set global variable "choice"
setGlobalchoice(get(findobj('Tag', 'list1'), 'Value'));

%% If list1 is empty
if isempty(get(findobj('Tag', 'list1'), 'Value'))
    set(findobj('Tag', 'sigbrowser.sigbrowseAdapter:view'), 'Enable', 'off');
    set(findobj('Tag', 'buttonEyeDiagram'), 'Enable', 'off');
    set(findobj('Tag', 'buttonConstellation'),'Enable','off');
    set(findobj('Tag', 'buttonModPhase'),'Enable','off');
    set(findobj('Tag', 'buttonMod2Phase'),'Enable','off');  
end
%%

function [ ] = changeLinewidth( ~ )
%CHANGELINEWIDTH updates the "Signal Browser" linewidth/linestyle
%from "visualizer".
%   CHANGELINEWIDTH just updates the "Signal Browser" linewidth/linestyle.

%% Types
td1 = 'TimeDiscreteAmplitudeDiscreteReal';
td2 = 'TimeDiscreteAmplitudeContinuousReal';

%% Change linewidth/linestyle
signals = sptool('Signals');
activeSignals = get(findobj('Tag', 'list1'), 'Value');
N = length(activeSignals);
currentAxes = findobj('Tag', 'DisplayAxes1_RealMag');
lw = 1.5;
%for k = 1:N
for k = activeSignals
    %obj1 = findobj('Tag', sprintf('DisplayLine%u', k)); set(obj1, 'Linewidth', lw, 'Linestyle', '-', 'Marker', 'none');
    obj1 = findobj('DisplayName', signals(k).label); set(obj1, 'Linewidth', lw, 'Linestyle', '-', 'Marker', 'none');
    %obj2 = findobj('Tag', sprintf('DisplayLine%u_Imag', k)); set(obj2, 'Linewidth', lw, 'Linestyle', '-', 'Marker', 'none');
    obj2 = findobj('DisplayName', signals(k).label); set(obj2, 'Linewidth', lw, 'Linestyle', '-', 'Marker', 'none');  
    if strcmp(signals(k).type, td1) || strcmp(signals(k).type, td2)
        set(obj1, 'Visible', 'off');
        st1 = stem(currentAxes, get(obj1, 'XData'), get(obj1, 'YData')); hold(currentAxes, 'on');
        set(st1, 'Color', get(obj1, 'Color'), 'Tag', 'st1', 'linewidth', lw);
    end
end
%%

% function [ ] = changeTimeWindow( ~ )
% %CHANGETIMEWINDOW updates the "Signal Browser" time window
% %from "visualizer".
% %   CHANGETIMEWINDOW just updates the "Signal Browser" time window.
% 
% %% Get global variable "nSymbols"
% nSymbolsr = getGlobalnSymbols;
% 
% %% Types
% tb = 'Binary';
% 
% %% Change time window
% signals = sptool('Signals');
% activeSignals = get(findobj('Tag', 'list1'), 'Value');
% currentAxes = findobj('Tag', 'DisplayAxes1_RealMag'); 
% if strcmp(signals(activeSignals(1)).type, tb) 
%     factor = 200;
%     set(currentAxes, 'XLim', [0 ((nSymbolsr*factor) - 1)*(1/signals(activeSignals(1)).Fs)]); 
% else
%     samplesPerSymbol = double(signals(activeSignals(1)).SPTIdentifier.version);
%     set(currentAxes, 'XLim', [0 ((nSymbolsr*samplesPerSymbol) - 1)*(1/signals(activeSignals(1)).Fs)]);
% end
% xl = get(currentAxes, 'XLabel');
% if strcmp('Time (ns)', get(xl, 'String'))
%     set(currentAxes, 'XTickLabel' ,get(currentAxes, 'XTick')*1e9);
% end
% if strcmp('Time (ps)', get(xl, 'String'))
%     set(currentAxes, 'XTickLabel' ,get(currentAxes, 'XTick')*1e12);
% end
%%

% function [ ] = viewComplex( varargin )
% %VIEWCOMPLEX represents the real and imaginary parts of the current
% %selected signals.
% %   VIEWCOMPLEX just represents the real and imaginary parts.
% 
% %% Close complex figures
% closeComplexFigures;
% 
% %% Open "Signal Browser"
% signalBrowser = findobj('Tag', 'SignalBrowser');
% flag = 0;
% if ~isempty(varargin)
%     if varargin{1} == 1
%         flag = 1;
%     end
% end
% if isempty(signalBrowser)
%     defaultVerb(1, 1);
%     changeTimeWindow;
%     signalBrowser = findobj('Tag', 'SignalBrowser');
% else
%     xlim = get(findobj('Tag', 'DisplayAxes1_RealMag'), 'XLim');
%     if xlim(1) < 0 || flag == 1
%         defaultVerb(1, 1);
%         changeTimeWindow;
%     end
% end
% 
% %% Plots
% signals = sptool('Signals');
% activeSignals = get(findobj('Tag', 'list1'), 'Value');
% N = length(activeSignals);
% currentAxes = findobj('Tag', 'DisplayAxes1_RealMag');
% yli = get(currentAxes, 'YLabel');
% f = figure;
% set(f, 'Tag', 'figCompRI', 'numbertitle', 'off', 'Name', 'Signal Browser');
% h = pan(findobj('Tag', 'figCompRI'));
% set(h, 'ActionPostCallback', @updatePan);
% lw = 1.5;
% for k = 1:N    
%     obj1 = findobj('Tag', sprintf('DisplayLine%u', k));
%     obj2 = findobj('Tag', sprintf('DisplayLine%u_Imag',k));
%     % Real part
%     subplot(2, 1, 1); hold(gca, 'on');
%     plot(get(obj1, 'XData'), get(obj1, 'YData'), 'Color', get(obj1, 'Color'), 'Linewidth', lw, 'Tag', 'pr1'); 
%     % Imaginary part
%     subplot(2, 1, 2); hold(gca, 'on');
%     plot(get(obj2, 'XData'), get(obj2, 'YData'), 'Color', get(obj2, 'Color'), 'Linewidth', lw, 'Tag', 'pi1');  
% end 
% 
% %% Axes
% axesReal = subplot(2, 1, 1); set(axesReal, 'Tag', 'axesReal');
% set(axesReal, 'XLim', get(currentAxes, 'XLim'), 'box', 'on');
% set(axesReal, 'YLim', get(currentAxes, 'YLim'), 'YTick', get(currentAxes,'YTick'), 'YTickLabel', get(currentAxes, 'YTickLabel'));
% xl = get(axesReal, 'XLabel'); set(xl, 'String', 'Time (s)');
% yl = get(axesReal, 'YLabel'); set(yl, 'String', get(yli,'String'));
% t = title('Real part'); set(t, 'Fontweight', 'bold', 'fontsize', 12);
% grid(axesReal, 'on');
% axesImag = subplot(2, 1, 2); set(axesImag, 'Tag', 'axesImag');
% set(axesImag, 'XLim', get(currentAxes, 'XLim'), 'box', 'on');
% set(axesImag, 'YLim', get(currentAxes, 'YLim'), 'YTick', get(currentAxes, 'YTick'), 'YTickLabel', get(currentAxes, 'YTickLabel'));
% xl = get(axesImag, 'XLabel'); set(xl, 'String', 'Time (s)');
% yl = get(axesImag, 'YLabel'); set(yl, 'String', get(yli,'String'));
% t = title('Imaginary part'); set(t, 'Fontweight', 'bold', 'fontsize', 12);
% grid(axesImag, 'on');
% 
% %% Legends
% subplot(2, 1, 1);
% str = cell(1, N);
% for k = 1:N
%     arg = {'real (' , signals(activeSignals(k)).label, ')'};
%     str(k) = {strjoin(arg, '')};
% end
% str = str';
% legend(str);
% subplot(2, 1, 2);
% str2 = cell(1, N);
% for k = 1:N
%     arg = {'imag (' , signals(activeSignals(k)).label, ')'};
%     str2(k) = {strjoin(arg, '')};
% end
% str2 = str2';
% legend(str2);
% 
% %% Omit native "Signal Browser"
% set(signalBrowser, 'Visible', 'off');
% 
% %% Set global variable "choice"
% setGlobalchoice(0);
% %%

% function [ ] = viewComplexXY( varargin )
% %VIEWCOMPLEX represents the real and imaginary parts of the current
% %selected signals.
% %   VIEWCOMPLEX just represents the real and imaginary parts.
% 
% %% Close complex figures
% closeComplexFigures;
% 
% %% Open "Signal Browser"
% signalBrowser = findobj('Tag', 'SignalBrowser');
% flag = 0;
% if ~isempty(varargin)
%     if varargin{1} == 1
%         flag = 1;
%     end
% end
% if isempty(signalBrowser)
%     defaultVerb(1, 1);
%     changeTimeWindow;
%     signalBrowser = findobj('Tag', 'SignalBrowser');
% else
%     xlim = get(findobj('Tag', 'DisplayAxes1_RealMag'), 'XLim');
%     if xlim(1) < 0 || flag == 1
%         defaultVerb(1, 1);
%         changeTimeWindow;
%     end
% end
% 
% %% Plots
% signals = sptool('Signals');
% activeSignals = get(findobj('Tag', 'list1'), 'Value');
% N = length(activeSignals);
% currentAxes = findobj('Tag', 'DisplayAxes1_RealMag');
% yli = get(currentAxes, 'YLabel');
% f = figure;
% set(f, 'Tag', 'figCompRI', 'numbertitle', 'off', 'Name', 'Signal Browser');
% h = pan(findobj('Tag', 'figCompRI'));
% set(h, 'ActionPostCallback', @updatePan);
% lw = 1.5;
% for k = 1:N 
%     data = signals(activeSignals(k)).data;
%     data_x = data(1:4:end) + 1i.*data(2:4:end);
%     data_y = data(3:4:end) + 1i.*data(4:4:end);
%     data_x = real(data_x)' + imag(data_x)'.*1i;
%     data_y = real(data_y)' + imag(data_y)'.*1i;
%     
%     obj1 = findobj('Tag', sprintf('DisplayLine%u', k));
%     
%     % Real part X mode
%     obj1.XData = obj1.XData(1:length(data_x));
%     obj1.YData = real(data_x);
% 
%     subplot(4, 1, 1); hold(gca, 'on');
%     plot(get(obj1, 'XData'), get(obj1, 'YData'), 'Color', get(obj1, 'Color'),'Linewidth', lw, 'Tag', 'pr1');
%      
%     % Imaginary part Y mode
%     obj1.YData = imag(data_x);
%     
%     subplot(4, 1, 2); hold(gca, 'on');
%     plot(get(obj1, 'XData'), get(obj1, 'YData'), 'Color', get(obj1, 'Color'),'Linewidth', lw, 'Tag', 'pr1');
%    
%     % Real part Y mode
%     obj1.YData = real(data_y);
%       
%     subplot(4, 1, 3); hold(gca, 'on');
%     plot(get(obj1, 'XData'), get(obj1, 'YData'), 'Color', get(obj1, 'Color'),'Linewidth', lw, 'Tag', 'pr1');
% 
%     % Imag part Y mode
%     obj1.YData = imag(data_y);
%       
%     subplot(4, 1, 4); hold(gca, 'on');
%     plot(get(obj1, 'XData'), get(obj1, 'YData'), 'Color', get(obj1, 'Color'),'Linewidth', lw, 'Tag', 'pr1');
% 
%     
%     
% end 
% 
% %% Axes
% % axesReal = subplot(4, 1, 1); set(axesReal, 'Tag', 'axesReal');
% % set(axesReal, 'XLim', get(currentAxes, 'XLim'), 'box', 'on');
% % set(axesReal, 'YLim', get(currentAxes, 'YLim'), 'YTick', get(currentAxes,'YTick'), 'YTickLabel', get(currentAxes, 'YTickLabel'));
% % xl = get(axesReal, 'XLabel'); set(xl, 'String', 'Time (s)');
% % yl = get(axesReal, 'YLabel'); set(yl, 'String', get(yli,'String'));
% % t = title('Real part'); set(t, 'Fontweight', 'bold', 'fontsize', 12);
% % grid(axesReal, 'on');
% % axesImag = subplot(4, 1, 2); set(axesImag, 'Tag', 'axesImag');
% % set(axesImag, 'XLim', get(currentAxes, 'XLim'), 'box', 'on');
% % set(axesImag, 'YLim', get(currentAxes, 'YLim'), 'YTick', get(currentAxes, 'YTick'), 'YTickLabel', get(currentAxes, 'YTickLabel'));
% % xl = get(axesImag, 'XLabel'); set(xl, 'String', 'Time (s)');
% % yl = get(axesImag, 'YLabel'); set(yl, 'String', get(yli,'String'));
% % t = title('Imaginary part'); set(t, 'Fontweight', 'bold', 'fontsize', 12);
% % grid(axesImag, 'on');
% 
% %% Legends
% % subplot(4, 1, 1);
% % str = cell(1, N);
% % for k = 1:N
% %     arg = {'real (' , signals(activeSignals(k)).label, ')'};
% %     str(k) = {strjoin(arg, '')};
% % end
% % str = str';
% % legend(str);
% % subplot(4, 1, 2);
% % str2 = cell(1, N);
% % for k = 1:N
% %     arg = {'imag (' , signals(activeSignals(k)).label, ')'};
% %     str2(k) = {strjoin(arg, '')};
% % end
% % str2 = str2';
% % legend(str2);
% 
% %% Omit native "Signal Browser"
% set(signalBrowser, 'Visible', 'off');
% 
% %% Set global variable "choice"
% setGlobalchoice(0);
% %%



function viewEyeDiagram( hObject, callbackdata )
%VIEWEYEDIAGRAM represents eye diagrams of the current
%selected signals.
%   VIEWEYEDIAGRAM just represents one or more eye diagrams.
                                   
%% Eye Diagram
delete(findobj('Tag', 'Eye Diagram')); % Delete old objects
activeSignals = get(findobj('Tag', 'list1'), 'Value');
N = length(activeSignals);
signals = sptool('Signals');
tc1 = 'TimeContinuousAmplitudeDiscreteComplex';
tc2 = 'TimeContinuousAmplitudeContinuousComplex';
tc5 = 'BandpassSignal';
for k = 1:N
    samplesPerSymbol = signals(activeSignals(k)).SPTIdentifier.version;
    x = signals(activeSignals(k)).data;
    n = 2*samplesPerSymbol; % Number of samples in each trace
    period = 2; % Number of represented periods
    offset = 0; % Diagram offset
    eyediagram(x, n, period, offset); 
    if strcmp(signals(activeSignals(1)).type, tc1) || strcmp(signals(activeSignals(1)).type, tc2) || strcmp(signals(activeSignals(1)).type, tc5) 
        subplot(2, 1, 1);
        title(strjoin({'Eye Diagram for In-Phase', signals(activeSignals(k)).label}, ' '));
        xlabel('t / T');
        grid(gca, 'on');
        subplot(2, 1, 2);
        title(strjoin({'Eye Diagram for Quadrature', signals(activeSignals(k)).label}, ' '));
        xlabel('t / T');
        grid(gca, 'on');
    else
        title(strjoin({'Eye Diagram for', signals(activeSignals(k)).label}, ' '));
        xlabel('t / T');
        grid('on');
    end
end
%%

function viewConstellation( hObject, callbackdata )
%VIEWCONSTELLATION represents the constellation diagrams of the current 
%selected signals.
%   VIEWCONSTELLATION just represents the constellation diagrams.

%% Close complex figures
closeComplexFigures;

%% Open "Signal Browser"
signalBrowser = findobj('Tag', 'SignalBrowser');
if isempty(signalBrowser)
    defaultVerb(1, 1);
else
    set(signalBrowser, 'Visible', 'on');
end

%% Constellation
signals = sptool('Signals'); 
activeSignals = get(findobj('Tag', 'list1'), 'Value');
N = length(activeSignals);
lw = 1;
for k = 1:N   
    obj1 = findobj('Tag', sprintf('DisplayLine%u', k));
    obj2 = findobj('Tag', sprintf('DisplayLine%u_Imag', k));
    color = get(obj1, 'Color');
    samplesPerSymbol = signals(activeSignals(k)).SPTIdentifier.version;
    set(obj1, 'XData', real(signals(activeSignals(k)).data(1:samplesPerSymbol:end)), 'YData', imag(signals(activeSignals(k)).data(1:samplesPerSymbol:end)), 'LineStyle', 'none', 'Marker', 'o','MarkerSize', 2, 'MarkerFaceColor', color, 'Linewidth', lw);
    %set(obj1 ,'XData' , real(signals(activeSignals(k)).data), 'YData', imag(signals(activeSignals(k)).data), 'LineStyle', 'none', 'Marker', 'o', 'MarkerSize', 2, 'MarkerFaceColor', color, 'Linewidth', lw);
    set(obj2,'Visible','off');        
end

%% Axes
currentAxes = findobj('Tag', 'DisplayAxes1_RealMag'); 
top = 2; li = linspace(-top, top, ((2*top)/0.5) + 1);
set(currentAxes, 'XLim', [li(1) li(end)], 'YLim', [li(1) li(end)], 'XTickLabel', li); 
grid(currentAxes, 'on');

%% Labels
xlabel = get(currentAxes, 'XLabel'); set(xlabel, 'String', 'Real');
ylabel = get(currentAxes, 'YLabel'); set(ylabel,'String', 'Imaginary');

%% Legend
str = cell(1, N);
for k = 1:N
    str(k) = {signals(activeSignals(k)).label};
end
leg = findobj('Tag', 'legend');
set(leg, 'String', str);
%%
  
function viewModPhase( hObject, callbackdata )
%VIEWMODPHASE represents the magnitude (module) and phase of the current 
%selected signals.
%   VIEWMODPHASE just represents the magnitude and phase.

%% Close complex figures
closeComplexFigures;

%% Open "Signal Browser"
signalBrowser = findobj('Tag', 'SignalBrowser');
if isempty(signalBrowser)
    defaultVerb(1, 1);
    changeTimeWindow;
    signalBrowser = findobj('Tag', 'SignalBrowser');
else
    xlim = get(findobj('Tag', 'DisplayAxes1_RealMag'), 'XLim');
    if xlim(1) < 0
        defaultVerb(1, 1);
        changeTimeWindow;
    end
end

%% Plots
signals = sptool('Signals');
activeSignals = get(findobj('Tag', 'list1'), 'Value');
N = length(activeSignals);
ampMinVect = zeros(1, N);
ampMinVect2 = zeros(1, N);
ampMaxVect = ampMinVect; 
ampMaxVect2 = ampMinVect2; 
currentAxes = findobj('Tag', 'DisplayAxes1_RealMag');
f = figure;
set(f, 'Tag', 'figCompModPhase', 'numbertitle', 'off', 'Name', 'Signal Browser');
phaseOptionr = getGlobalphaseOption;
lw = 1.5;
for k = 1:N   
    absSig = abs(signals(activeSignals(k)).data);
    phaseSig = calculatePhase(phaseOptionr, signals(activeSignals(k)).data);
    ampMinVect(k) = min(absSig);
    ampMinVect2(k) = min(phaseSig);
    ampMaxVect(k) = max(absSig); 
    ampMaxVect2(k) = max(phaseSig); 
    obj1 = findobj('Tag', sprintf('DisplayLine%u', k));
    obj2 = findobj('Tag', sprintf('DisplayLine%u_Imag',k));
    % Magnitude
    subplot(2, 1, 1); hold(gca, 'on');
    plot(get(obj1, 'XData'), absSig, 'Color', get(obj1, 'Color'), 'Linewidth', lw); 
    % Phase
    subplot(2, 1, 2); hold(gca, 'on');
    plot(get(obj2, 'XData'), phaseSig, 'Color', get(obj2, 'Color'), 'Linewidth', lw);  
end 

%% Axes
sum = (max(ampMaxVect) - min(ampMinVect))/10;
sum2 = (max(ampMaxVect2) - min(ampMinVect2))/10;
axesMod = subplot(2, 1, 1); set(axesMod, 'Tag', 'axesMod');
set(axesMod, 'XLim', get(currentAxes, 'XLim'), 'box', 'on');
set(axesMod, 'YLim', [min(ampMinVect)-sum max(ampMaxVect)+sum]);
xl = get(axesMod, 'XLabel'); set(xl, 'String', 'Time (s)');
yl = get(axesMod, 'YLabel'); set(yl, 'String', '|Amplitude|');
t = title('Magnitude'); set(t, 'Fontweight', 'bold', 'fontsize', 12);
grid(axesMod, 'on');
axesPhase = subplot(2, 1, 2); set(axesPhase, 'Tag', 'axesPhase');
set(axesPhase, 'XLim', get(currentAxes, 'XLim'), 'box', 'on');
set(axesPhase, 'YLim', [min(ampMinVect2)-sum2 max(ampMaxVect2)+sum2]);
xl = get(axesPhase, 'XLabel'); set(xl, 'String', 'Time (s)');
yl = get(axesPhase, 'YLabel'); set(yl, 'String', 'Phase (rad)');
t = title('Phase'); set(t, 'Fontweight', 'bold', 'fontsize', 12);
grid(axesPhase, 'on');

%% Legends
subplot(2, 1, 1);
str = cell(1, N);
for k = 1:N
    str(k) = {['|Amp (', signals(activeSignals(k)).label, ')|']};
end
legend(str);
subplot(2, 1, 2);
str2 = cell(1, N);
for k = 1:N
    str2(k) = {['Phase (', signals(activeSignals(k)).label, ')']};
end
legend(str2);

%% Omit native "Signal Browser"
set(signalBrowser, 'Visible', 'off');

%% Set global variable "choice"
setGlobalchoice(0);
%%

function viewMod2Phase( hObject, callbackdata )
%VIEWMOD2PHASE represents the squared magnitude (module) and phase of the current 
%selected signals.
%   VIEWMOD2PHASE just represents the squared magnitude and phase.

%% Close complex figures
closeComplexFigures;

%% Open "Signal Browser"
signalBrowser = findobj('Tag', 'SignalBrowser');
if isempty(signalBrowser)
    defaultVerb(1, 1);
    changeTimeWindow;
    signalBrowser = findobj('Tag', 'SignalBrowser');
else
    xlim = get(findobj('Tag', 'DisplayAxes1_RealMag'), 'XLim');
    if xlim(1) < 0
        defaultVerb(1, 1);
        changeTimeWindow;
    end
end

%% Plots
signals = sptool('Signals');
activeSignals = get(findobj('Tag', 'list1'), 'Value');
N = length(activeSignals);
ampMinVect = zeros(1, N);
ampMinVect2 = zeros(1, N);
ampMaxVect = ampMinVect; 
ampMaxVect2 = ampMinVect2; 
currentAxes = findobj('Tag', 'DisplayAxes1_RealMag');
f = figure;
set(f, 'Tag', 'figCompMod2Phase', 'numbertitle', 'off', 'Name', 'Signal Browser');
phaseOptionr = getGlobalphaseOption;
lw = 1.5;
for k = 1:N   
    absSig2 = abs(signals(activeSignals(k)).data).^2;
    phaseSig = calculatePhase(phaseOptionr, signals(activeSignals(k)).data);
    ampMinVect(k) = min(absSig2);
    ampMinVect2(k) = min(phaseSig);
    ampMaxVect(k) = max(absSig2); 
    ampMaxVect2(k) = max(phaseSig); 
    obj1 = findobj('Tag', sprintf('DisplayLine%u', k));
    obj2 = findobj('Tag', sprintf('DisplayLine%u_Imag',k));
    % Amplitude
    subplot(2, 1, 1); hold(gca, 'on');
    plot(get(obj1, 'XData'), absSig2, 'Color', get(obj1, 'Color'), 'Linewidth', lw); 
    % Phase
    subplot(2, 1, 2); hold(gca, 'on');
    plot(get(obj2, 'XData'), phaseSig, 'Color', get(obj2, 'Color'), 'Linewidth', lw);  
end 

%% Axes
sum = (max(ampMaxVect) - min(ampMinVect))/10;
sum2 = (max(ampMaxVect2) - min(ampMinVect2))/10;
axesMod2 = subplot(2, 1, 1); set(axesMod2, 'Tag', 'axesMod2');
set(axesMod2, 'XLim', get(currentAxes, 'XLim'), 'box', 'on');
set(axesMod2, 'YLim', [min(ampMinVect)-sum max(ampMaxVect)+sum]);
xl = get(axesMod2, 'XLabel'); set(xl, 'String', 'Time (s)');
yl = get(axesMod2, 'YLabel'); set(yl, 'String', '|Amplitude| ^2');
t = title('Magnitude ^2'); set(t, 'Fontweight', 'bold', 'fontsize', 12);
grid(axesMod2, 'on');
axesPhase2 = subplot(2, 1, 2); set(axesPhase2, 'Tag', 'axesPhase2');
set(axesPhase2, 'XLim', get(currentAxes, 'XLim'), 'box', 'on');
set(axesPhase2, 'YLim', [min(ampMinVect2)-sum2 max(ampMaxVect2)+sum2]);
xl = get(axesPhase2, 'XLabel'); set(xl, 'String', 'Time (s)');
yl = get(axesPhase2, 'YLabel'); set(yl, 'String', 'Phase (rad)');
t = title('Phase'); set(t, 'Fontweight', 'bold', 'fontsize', 12);
grid(axesPhase2, 'on');

%% Legends
subplot(2, 1, 1);
str = cell(1, N);
for k = 1:N
    str(k) = {['|Amp^2 (', signals(activeSignals(k)).label, ')|']};
end
legend(str);
subplot(2, 1, 2);
str2 = cell(1, N);
for k = 1:N
    str2(k) = {['Phase (', signals(activeSignals(k)).label, ')']};
end
legend(str2);

%% Omit native "Signal Browser"
set(signalBrowser, 'Visible', 'off');

%% Set global variable "choice"
setGlobalchoice(0);
%%

function openFiles( hObject, callbackdata )
%OPENFILES Opens files to "visualizer".
%   OPENFILES just opens data files.

%% Open file browser with last user directory path
% Get global variable "previousPath"
previousPathr = getGlobalpreviousPath;
nValidFiles = 0; % Number of valid read files
% uigetfile: f - name(s) with ext and p - directory path
[f, p] = uigetfile(fullfile(previousPathr, '*.sgn;*.imp'), 'MultiSelect', 'on', 'Select File(s) to open'); % Browse to only ".sgn" and ".imp" files

%% Valid directory
if ischar(p)
    previousPathr = p(1:end - 1); 
end

%% Get the number of selected files
if iscell(f)
    nFiles = length(f);
else
    arg = {p, f};
    if iscellstr(arg)
        nFiles = 1;
    else
        nFiles = 0;
    end
end

%% Analyse selected files 
if nFiles > 0
    for n = 1:nFiles
        
        if nFiles > 1
            arg = {p, char(f(n))};
        end
        fileName = strjoin(arg, ''); % fileName with fullpath
        [pathstr, name, ext] = fileparts(fileName);
        
        % Open file
        fid = fopen(fileName);
        
        if strcmp(ext, '.sgn') % Only to ".sgn" files 
            % Read header
            [type, symbolPeriod, samplingPeriod, flagT] = readSignalHeader(fid); 
            % Read and load data            
            if flagT == 1
                nValidFiles = nValidFiles + 1;
                % Number of samples per period
                samplesPerSymbol = int64(symbolPeriod/samplingPeriod);
                % Read data
                [data, samplingFrequency] = readSignalData(fid, type, symbolPeriod, samplingPeriod);
                % Load signal file information to sptool
                loadSignal(data, samplingFrequency, type, samplesPerSymbol, name);                
            else
                % Error message
                fprintf('Error: "%s" without terminator!\n', fileName);
            end
        end
        
        if strcmp(ext, '.imp') % Only to ".imp" files  
            % Read header
            flagT = readImpHeader(fid);
            % Read and load data
            if flagT == 1
                nValidFiles = nValidFiles + 1;
                % Signal type
                type = 'ImpulseResponse';
                % Read data
                [time, data, samplingFrequency] = readImpData(fid);
                % Load impulse response file information to sptool 
                loadImp(time, data, samplingFrequency, type, name);                  
            else
                % Error message
                fprintf('Error: "%s" without terminator!\n', fileName);
            end
        end
        
        if ~strcmp(ext, '.sgn') && ~strcmp(ext, '.imp')
            % Error message
            fprintf('Error: "%s" has incorrect file extension!\n', fileName);
        end
        
        % Close file
        fclose(fid);
            
    end
end

%% Conclude openFiles
if nValidFiles > 0
    % Close all figures
    closeAllFigures;
    % Disable list selection
    disableListSelection;
    % Set global variable "choice"
    setGlobalchoice(0);
    % Set global variable "choice2"
    setGlobalchoice2(0);      
    % Set global variable "choice3"
    setGlobalchoice3(0);
    % Disable all pushbuttons
    disableAllPushbuttons;
    % Set global variable "previousPath"
    setGlobalpreviousPath(previousPathr);
end
%%

function openDirectory( hObject, callbackdata )
%OPENDIRECTORY Opens all data files from a given directory to "visualizer".
%   OPENDIRECTORY just opens data files.

%% Open file browser with last user directory path
% Get global variable "previousPath"
previousPathr = getGlobalPreviousPath;
nValidFiles = 0; % Number of valid read files
% uigetdir: pathName - Directory path
pathName = uigetdir(previousPathr);
setGlobalPreviousPath(pathName);

%% Get the number of directory files
nFiles = 0;
if ischar(pathName)
    files = dir(pathName);
    N = length(files);
    ind = zeros(1, N);
    for k = 1:N
        if ~files(k).isdir
            nFiles = nFiles + 1;
            ind(nFiles) = k;
        end
    end
    if nFiles > 0
        ind = ind(ind ~= 0);
        files = files(ind);
    else
        % Error message
        fprintf('Error: empty directory!\n');
    end
end
if nFiles > 0
    % f - name(s) with ext
    f = cell(1, nFiles);
    for k = 1:nFiles
        f(k) = cellstr(files(k).name);
    end
end

%% Analyse directory files 
if nFiles > 0
    for n = 1:nFiles
        
        arg = {pathName, '\', char(f(n))};
        fileName = strjoin(arg, ''); % fileName with fullpath
        [pathstr, name, ext] = fileparts(fileName);
        
        % Open file
        fid = fopen(fileName);
        
        if strcmp(ext, '.sgn') % Only to ".sgn" files 
            % Read header
            [type, symbolPeriod, samplingPeriod, flagT] = readSignalHeader(fid); 
            % Read and load data            
            if flagT == 1
                nValidFiles = nValidFiles + 1;
                % Number of samples per period
                samplesPerSymbol = int64(symbolPeriod/samplingPeriod);
                % Read data
                [data, samplingFrequency] = readSignalData(fid, type, symbolPeriod, samplingPeriod);
                % Load signal file information to sptool
                loadSignal(data, samplingFrequency, type, samplesPerSymbol, name);                
            else
                % Error message
                fprintf('Error: "%s" without terminator!\n', fileName);
            end
        end
        
        if strcmp(ext, '.imp') % Only to ".imp" files  
            % Read header
            flagT = readImpHeader(fid);
            % Read and load data
            if flagT == 1
                nValidFiles = nValidFiles + 1;
                % Signal type
                type = 'ImpulseResponse';
                % Read data
                [time, data, samplingFrequency] = readImpData(fid);
                % Load impulse response file information to sptool 
                loadImp(time, data, samplingFrequency, type, name);                  
            else
                % Error message
                fprintf('Error: "%s" without terminator!\n', fileName);
            end
        end
        
        if ~strcmp(ext, '.sgn') && ~strcmp(ext, '.imp')
            % Error message
            fprintf('Error: "%s" has incorrect file extension!\n', fileName);
        end
        
        % Close file
        fclose(fid);
            
    end
end

%% Conclude openDirectory 
if nValidFiles > 0
    % Close all figures
    closeAllFigures;
    % Disable list selection
    disableListSelection;
    % Set global variable "choice"
    setGlobalchoice(0);
    % Set global variable "choice2"
    setGlobalchoice2(0);      
    % Set global variable "choice3"
    setGlobalchoice3(0);
    % Disable all pushbuttons
    disableAllPushbuttons;
    % Set global variable "previousPath"
    setGlobalPreviousPath(pathName);
end
%%

function reload( hObject, callbackdata )
%RELOAD Reloads the current signals/filters/spectra from the 
%last user directory path to "visualizer".
%   RELOAD just reloads data files.

%% Directory path
% Get global variable "previousPath"
previousPathr = getGlobalPreviousPath;
nValidFiles = 0; % Number of valid read files
% pathName - Directory path
pathName = previousPathr; 

%% Current signals and filters labels
signals = sptool('Signals');
filters = sptool('Filters');
N1 = length(signals);
N2 = length(filters);
if N1 >0 || N2 >0
    sigLabels = cell(1, N1);
    filtLabels = cell(1, N2);
    for k = 1:N1
        sigLabels(k) = cellstr(signals(k).label);
    end
    for k = 1:N2
        filtLabels(k) = cellstr(filters(k).label);
    end
end
if N1 == 0 && N2 == 0
    fprintf('Error: Reload action is not possible!\n');
end

%% Get the number of directory files
nFiles = 0;
if N1 >0 || N2 >0
    if ischar(pathName)
        files = dir(pathName);
        N = length(files);
        ind = zeros(1, N);
        for k = 1:N
            if ~files(k).isdir
                nFiles = nFiles + 1;
                ind(nFiles) = k;
            end
        end
        if nFiles > 0
            ind = ind(ind ~= 0);
            files = files(ind);
        else
            % Error message
            fprintf('Error: empty directory!\n');
        end
    end
    if nFiles > 0
        % f - name(s) with ext
        f = cell(1, nFiles);
        for k = 1:nFiles
            f(k) = cellstr(files(k).name);
        end
    end
end

%% Analyse directory files 
if nFiles > 0
    flag = 0;
    for n = 1:nFiles
        
        arg = {pathName, '\', char(f(n))};
        fileName = strjoin(arg, ''); % fileName with fullpath
        [pathstr, name, ext] = fileparts(fileName);
        
        % Open file
        fid = fopen(fileName);
        
        if strcmp(ext, '.sgn') && sum(strcmp(sigLabels, name)) == 1 % Only to ".sgn" files 
            flag = 1;
            % Read header
            [type, symbolPeriod, samplingPeriod, flagT] = readSignalHeader(fid); 
            % Read and load data            
            if flagT == 1
                nValidFiles = nValidFiles + 1;
                % Number of samples per period
                samplesPerSymbol = int64(symbolPeriod/samplingPeriod);
                % Read data
                [data, samplingFrequency] = readSignalData(fid, type, symbolPeriod, samplingPeriod);
                % Load signal file information to sptool
                loadSignal(data, samplingFrequency, type, samplesPerSymbol, name);                
            else
                % Error message
                fprintf('Error: "%s" without terminator!\n', fileName);
            end
        end
        
        if strcmp(ext, '.imp') && sum(strcmp(filtLabels, name)) == 1 % Only to ".imp" files  
            flag = 1;
            % Read header
            flagT = readImpHeader(fid);
            % Read and load data
            if flagT == 1
                nValidFiles = nValidFiles + 1;
                % Signal type
                type = 'ImpulseResponse';
                % Read data
                [time, data, samplingFrequency] = readImpData(fid);
                % Load impulse response file information to sptool 
                loadImp(time, data, samplingFrequency, type, name);                  
            else
                % Error message
                fprintf('Error: "%s" without terminator!\n', fileName);
            end
        end
        
        % Close file
        fclose(fid);
            
    end
end

%% Conclude reload
if nValidFiles > 0
    % Close all figures
    closeAllFigures;
    % Disable list selection
    disableListSelection;
    % Set global variable "choice"
    setGlobalchoice(0);
    % Set global variable "choice2"
    setGlobalchoice2(0);      
    % Set global variable "choice3"
    setGlobalchoice3(0);
    % Disable all pushbuttons
    disableAllPushbuttons;
else
    if nFiles > 0 && flag == 0
        fprintf('Error: Files not found!\n');
    end
end
%%

function setConfigurations( hObject, callbackdata )
%SETCONFIGURATIONS sets "visualizer" confifurations.
%   SETCONFIGURATIONS(hObject, callbackdata) just sets all the configurations.

%% Read new configurations
flagBr = 0;
par1 = 1;
par2 = 2;
par3 = 3;
prompt = {'Enter the number of symbols to read:', 'Enter the number of symbols to view:', 'Enter the mode to view the phase:'};
dlg_title = 'Configurations';
num_lines = 1;
defaultans = {num2str(getGlobalnRead), num2str(getGlobalnSymbols), num2str(getGlobalphaseOption)};
while(par1 < par2 || par1 <= 0 || par2 <= 0 || par3 < 0 || par3 > 2 || isnan(par1) || isnan(par2) || isnan(par3))
    answer = inputdlg(prompt, dlg_title, num_lines, defaultans);
    if length(answer) == 3
        par1 = str2double(char(answer(1))); 
        par2 = str2double(char(answer(2))); 
        par3 = str2double(char(answer(3)));
    else
        flagBr = 1;
        break;
    end
end

%% Set new configurations
if flagBr == 0
    setGlobalnRead(par1);
    setGlobalnSymbols(par2);
    setGlobalphaseOption(round(par3));
end
%%

% function updatePan( hObject, callbackdata )
% %UPDATEPAN updates "pan" objects from "visualizer".
% %   UPDATEPAN just updates "pan" objects.
% 
% %% Get global variables
% previousPathr = getGlobalpreviousPath;
% t_binaryr = getGlobalt_binary;
% t_realr = getGlobalt_real;
% t_complexr = getGlobalt_complex;
% 
% %% Update pan
% activeSignals = get(findobj('Tag', 'list1'), 'Value');
% N = length(activeSignals);
% signals = sptool('Signals');
% tb = 'Binary';
% td1 = 'TimeDiscreteAmplitudeDiscreteReal';
% td2 = 'TimeDiscreteAmplitudeContinuousReal';
% tc1 = 'TimeContinuousAmplitudeDiscreteComplex';
% tc2 = 'TimeContinuousAmplitudeContinuousComplex';
% tc5 = 'BandpassSignal';
% if N == 1 
%     if ~strcmp(signals(activeSignals(1)).type, tc1) && ~strcmp(signals(activeSignals(1)).type, tc2) && ~strcmp(signals(activeSignals(1)).type, tc5)
%         xlim = get(findobj('Tag', 'DisplayAxes1_RealMag'), 'XLim');
%         xPan = xlim(2);
%         if strcmp(signals(activeSignals(1)).type, td1) || strcmp(signals(activeSignals(1)).type, td2)
%             curve = findobj('Tag', 'st1');
%         else
%             curve = findobj('Tag', 'DisplayLine1'); 
%         end
%         xdata = get(curve, 'XData');
%         xend = xdata(end);
%         if xPan > xend
%             ydata = get(curve, 'YData');
%             path = previousPathr;
%             arg = {path, '\', signals(activeSignals(1)).label, '.sgn'};
%             fileName = strjoin(arg, ''); % fileName with fullpath
%             % Open file
%             fid = fopen(fileName);
%             if fid == -1
%                 % Error message
%                 fprintf('Error: File not found!\n');
%             else
%                 % Read header
%                 [~, ~, ~, flagT] = readSignalHeader(fid);            
%                 if flagT == 1
%                     ts = 1/signals(activeSignals(1)).Fs;
%                     if strcmp(signals(activeSignals(1)).type, tb)
%                         factor = 200;
%                         nShownBits = ((xend/ts) + 1)/factor;
%                         for k = 1:nShownBits
%                             fread(fid, 1, t_binaryr);
%                         end
%                         % Read data
%                         [ynew, ~] = readSignalData(fid, tb, ts*factor, ts*factor);
%                     else
%                         samplesPerSymbol = signals(activeSignals(1)).SPTIdentifier.version;
%                         nShownSymbols = ((xend/ts) + 1)/samplesPerSymbol;
%                         for k = 1:nShownSymbols
%                             fread(fid, double(samplesPerSymbol)*1, t_realr);
%                         end
%                         % Read data
%                         [ynew, ~] = readSignalData(fid, signals(activeSignals(1)).type, ts*double(samplesPerSymbol), ts);
%                     end
%                     % Update
%                     xnew = xend + ts:ts:xend + ts*length(ynew);
%                     set(curve, 'XData', [xdata xnew], 'YData', [ydata ynew]);
%                 else
%                     % Error message
%                     fprintf('Error: "%s" without terminator!\n', fileName);
%                 end
%             end
%             % Close file
%             fclose(fid);
%         end 
%     else
%         axesReal = findobj('Tag', 'axesReal');
%         axesImag = findobj('Tag', 'axesImag');
%         curveReal = findobj('Tag', 'pr1');
%         curveImag = findobj('Tag', 'pi1');
%         xlimr = get(axesReal, 'XLim');
%         xPanr = xlimr(2);
%         xdatar = get(curveReal, 'XData');
%         xendr = xdatar(end); 
%         xlimi = get(axesImag, 'XLim');        
%         xPani = xlimi(2);
%         xdatai = get(curveImag, 'XData');
%         xendi = xdatai(end);
%         if xPanr > xendr || xPani > xendi
%             path = previousPathr;
%             arg = {path, '\', signals(activeSignals(1)).label, '.sgn'};
%             fileName = strjoin(arg, ''); % fileName with fullpath
%             % Open file
%             fid = fopen(fileName);
%             if fid == -1
%                 % Error message
%                 fprintf('Error: File not found!\n');
%             else
%                 % Read header
%                 [~, ~, ~, flagT] = readSignalHeader(fid); 
%                 if flagT == 1
%                     ts = 1/signals(activeSignals(1)).Fs;
%                     samplesPerSymbol = signals(activeSignals(1)).SPTIdentifier.version;
%                 else
%                     % Error message
%                     fprintf('Error: "%s" without terminator!\n', fileName);
%                 end
%             end
%         end
%         if xPanr > xendr
%             if flagT == 1 && fid ~= -1
%                 ydatar = get(curveReal, 'YData');
%                 nShownSymbols = ((xendr/ts) + 1)/samplesPerSymbol;
%             end
%         end
%         if xPani > xendi
%             if flagT == 1 && fid ~= -1
%                 ydatai = get(curveImag, 'YData');
%                 nShownSymbols = ((xendi/ts) + 1)/samplesPerSymbol;
%             end
%         end
%         if xPanr > xendr || xPani > xendi
%             if flagT == 1 && fid ~= -1
%                 for k = 1:nShownSymbols
%                     fread(fid, 2*double(samplesPerSymbol)*1, t_complexr);
%                 end
%                 % Read data
%                 [ynew, ~] = readSignalData(fid, signals(activeSignals(1)).type, ts*double(samplesPerSymbol), ts);
%             end
%         end
%         if xPanr > xendr
%             if flagT == 1 && fid ~= -1
%                 % Update
%                 ynewr = real(ynew);
%                 xnewr = xendr + ts:ts:xendr + ts*length(ynewr);
%                 set(curveReal, 'XData', [xdatar xnewr], 'YData', [ydatar ynewr]);
%             end
%         end
%         if xPani > xendi
%             if flagT == 1 && fid ~= -1
%                 % Update
%                 ynewi = imag(ynew);
%                 xnewi = xendi + ts:ts:xendi + ts*length(ynewi);
%                 set(curveImag, 'XData', [xdatai xnewi], 'YData', [ydatai ynewi]);
%             end
%         end
%         if xPanr > xendr || xPani > xendi
%             % Close file
%             fclose(fid);
%         end 
%     end
% end
% %%


function [ ] = createNewPushbuttons( ~ )
%CREATENEWPUSHBUTTONS Creates additional pushbuttons from "visualizer".
%   CREATENEWPUSHBUTTONS just creates all new pushbuttons.

%% Buttons struct
sptoolfig = findall(0, 'Tag', 'sptool');
ud = get(sptoolfig, 'UserData');

%% Pushbuttons "View" from "Signals" component and "New"; "Edit", and "Apply" from "Filters" component
viewSignals = findobj('Tag', 'sigbrowser.sigbrowseAdapter:view');        
newFilters = findobj('Tag', 'filtdes:create'); set(newFilters, 'Enable', 'off');
editFilters = findobj('Tag', 'filtdes:change');
applyFilters = findobj('Tag', 'applyfilt:apply');

%% Position of pushbuttons "View" from "Signals" component and "New"; "Edit", and "Apply" from "Filters" component
pViewSig = get(viewSignals, 'Position');
pNewFilt = get(newFilters, 'Position');
pEditFilt = get(editFilters, 'Position');
pApplyFilt = get(applyFilters, 'Position');

%% Create "Eye Diagram" pushbutton        
buttonEyeDiagram = uicontrol('Tag', 'buttonEyeDiagram', 'Style', 'pushbutton', 'String', '<html> Eye Diagram </html>',...
'Position', [pViewSig(1) pNewFilt(2) pViewSig(3:4)],...
'Callback', @viewEyeDiagram);  
set(buttonEyeDiagram, 'Enable', 'off');
ud.buttonHandles(1).h(2) = buttonEyeDiagram;

%% Create "Constellation" pushbutton          
buttonConstellation = uicontrol('Tag', 'buttonConstellation', 'Style', 'pushbutton', 'String', '<html> Constellation </html>',...
'Position', [pViewSig(1) pEditFilt(2) pViewSig(3:4)],...
'Callback', @viewConstellation);  
set(buttonConstellation, 'Enable', 'off');
ud.buttonHandles(1).h(3) = buttonConstellation;

%% Create Magnitude and Phase pushbutton  
buttonModPhase = uicontrol('Tag', 'buttonModPhase', 'Style', 'pushbutton', 'String', '<html> |A| &ang&empty </html>',...
'Position', [pViewSig(1) pApplyFilt(2) 0.45*pViewSig(3) pViewSig(4)],...
'Callback', @viewModPhase);  
set(buttonModPhase, 'Enable', 'off');
ud.buttonHandles(1).h(4) = buttonModPhase;

%% Create Squared Magnitude and Phase pushbutton          
buttonMod2Phase = uicontrol('Tag','buttonMod2Phase', 'Style', 'pushbutton', 'String', '<html> |A|<sup>2</sup> &ang&empty </html>',...
'Position', [0.57*pViewSig(3) pApplyFilt(2) 0.45*pViewSig(3) pViewSig(4)],...
'Callback', @viewMod2Phase);  
set(buttonMod2Phase, 'Enable', 'off');
ud.buttonHandles(1).h(5) = buttonMod2Phase;

%% Save 
set(gcf, 'UserData', ud);  
%%

function [ ] = changeCallbacks( ~ )
%CHANGECALLBACKS Changes callbacks from "visualizer".
%   CHANGECALLBACKS just changes some default callbacks.

%% Change callback of "File" menu option "Open session..."/"Open directory..."
set(findobj('Tag', 'loadmenu'), 'callback', @openDirectory); 

%% Change callback of "File" menu option "Import..."/"Open file(s)..."
set(findobj('Tag', 'importmenu'), 'callback', @openFiles); 

%% Change callback of "File" menu option "Export..."/"Reload"
set(findobj('Tag', 'exportmenu'), 'callback', @reload);  

%% Change callback of "File" menu option "Save Session"/"Configurations"
set(findobj('Tag', 'savemenu'), 'callback', @setConfigurations, 'Enable', 'on');  
%%



function [ startr ] = getGlobalstart( ~ )
%GETGLOBALSTART Gets global variable "start".
%   startr = GETGLOBALSTART just returns the value of "start" global variable ("startr").

global start;
startr = start;



% function [ nSymbolsr ] = getGlobalnSymbols( ~ )
% %GETGLOBALNSYMBOLS Gets global variable "nSymbols".
% %   nSymbolsr = GETGLOBALNSYMBOLS just returns the value of "nSymbols" global variable ("nSymbolsr").
% 
% global nSymbols;
% nSymbolsr = nSymbols;


function [ previousPathr ] = getGlobalpreviousPath( ~ )
%GETGLOBALPREVIOUSPATH Gets global variable "previousPath".
%   previousPathr = GETGLOBALPREVIOUSPATH just returns the value of "previousPath" global variable ("previousPathr").

global previousPath;
previousPathr = previousPath;



function [ choicer ] = getGlobalchoice( ~ )
%GETGLOBALCHOICE Gets global variable "choice".
%   choicer = GETGLOBALCHOICE just returns the value of "choice" global variable ("choicer").

global choice;
choicer = choice;



function [ choice3r ] = getGlobalchoice3( ~ )
%GETGLOBALCHOICE3 Gets global variable "choice3".
%   choice3r = GETGLOBALCHOICE3 just returns the value of "choice3" global variable ("choice3r").

global choice3;
choice3r = choice3;






function [ phaseOptionr ] = getGlobalphaseOption( ~ )
%GETGLOBALPHASEOPTION Gets global variable "phaseOption".
%   phaseOptionr = GETGLOBALPHASEOPTION just returns the value of "phaseOption" global variable ("phaseOptionr").

global phaseOption;
phaseOptionr = phaseOption;

function [ ] = closeAllFigures( ~ )
%CLOSEALLFIGURES close all figures from "visualizer".
%   CLOSEALLFIGURES just closes all generated figures/graphics.

%% Close "Signals" representations
close(findobj('Tag', 'SignalBrowser'));
close(findobj('Tag', 'Eye Diagram')); 
close(findobj('Tag', 'figCompRI'));
close(findobj('Tag', 'figCompModPhase'));
close(findobj('Tag', 'figCompMod2Phase'));
  
%% Close "Filters" representations
close(findobj('Tag', 'figImp'));
close(findobj('Tag', 'figFreq'));

%% Close "Spectra" representations
close(findobj('Tag', 'spectview'));
%%

function [ ] = disableAllPushbuttons( ~ )
%DISABLEALLPUSHBUTTONS disables all pushbuttons from "visualizer".
%   DISABLEALLPUSHBUTTONS just disables all visible pushbuttons.

%% Disable pushbuttons from "Signals" component
set(findobj('Tag', 'sigbrowser.sigbrowseAdapter:view'), 'Enable', 'off');
set(findobj('Tag', 'buttonEyeDiagram'), 'Enable', 'off');      
set(findobj('Tag', 'buttonConstellation'), 'Enable', 'off');
set(findobj('Tag', 'buttonModPhase'), 'Enable', 'off');
set(findobj('Tag', 'buttonMod2Phase'), 'Enable', 'off'); 

%% Disable pushbuttons from "Filters" component
set(findobj('Tag', 'filtview:view'), 'Enable', 'off');
set(findobj('Tag', 'filtdes:create'), 'Enable', 'off');

%% Disable pushbuttons from "Spectra" component
set(findobj('Tag', 'spectview:view'), 'Enable', 'off');
%%
   
function [ ] = disableListSelection( ~ )
%DISABLELISTSELECTION disables list selection from "visualizer".
%   DISABLELISTSELECTION just disables list selection.

%% List 1
set(findobj('Tag', 'list1'), 'Value', []);

%% List 2
set(findobj('Tag', 'list2'), 'Value', []);

%% List 3
set(findobj('Tag', 'list3'), 'Value', []);
%%

% function [ ] = defaultVerb( arg1, arg2 )
% %DEFAULTVERB runs default 'verb' action from "visualizer".
% %   DEFAULTVERB(arg1, arg2) just runs default sptool('verb') action.
% 
% %% 'Verb'
% sptoolfig = findobj(0, 'Tag', 'sptool');
% setptr(sptoolfig, 'watch')
% drawnow
% ud = get(sptoolfig, 'UserData');
% i = arg1;
% j = arg2;
% feval(ud.components(i).verbs(j).owningClient, 'action', ...
% ud.components(i).verbs(j).action);
% if ishghandle(sptoolfig)
%     setptr(sptoolfig, 'arrow')
% end
% if i == 1 && j == 1
%     % For MATLAB R2013a   
%     if  strcmp(version, '8.1.0.604 (R2013a)')
%         h = pan(findobj('Tag', 'SignalBrowser'));
%         set(h, 'ActionPostCallback', @updatePan);
%     end
% end
% %%
  
function [ ] = modifyWindow( ~ )
%MODIFYWINDOW Modifies "Spectrum Viewer" window from "visualizer".
%   MODIFYWINDOW just modifies "Spectrum Viewer".

%% Occult the box over parameters
set(findobj('Tag', 'propFrame'), 'Visible', 'off');

%% Change labels 
m = findobj('Tag', 'mainaxes');
xl = get(m, 'XLabel'); set(xl, 'String', 'Frequency (Hz)');
yl = get(m, 'YLabel'); set(yl, 'String', 'Magnitude (dB)');

%% Omit some options
set(findobj('Tag', 'inheritPopup'), 'Visible', 'off');
set(findobj('Tag', 'revertButton'), 'Visible', 'off');
set(findobj('Tag', 'applyButton'), 'Visible', 'off');
%%

function [ ] = modifySpectraHandles( ~ )
%MODIFYSPECTRAHANDLES Modifies spectra handles from "Spectrum Viewer".
%   MODIFYSPECTRAHANDLES just modifies spectra handles.

%% Convert from Unilateral Spectra to Bilateral Spectra (considering baseband signals)
fig = gcf;
ud = get(fig, 'UserData');
spectra = sptool('Spectra'); 
activeSpectra = get(findobj('Tag', 'list3'), 'Value');
N = length(activeSpectra);
% Change xdata and ydata
for k = 1:N
    spectrumCurve = findobj('Tag', spectra(activeSpectra(k)).label);            
    xdata = get(spectrumCurve, 'XData');        
    ydata = get(spectrumCurve, 'YData');
    xdata = xdata(abs(xdata) <= 0.5*spectra(activeSpectra(k)).Fs);
    ydata = ydata(1:length(xdata));
    if xdata(1) >= 0
        newXdata = zeros(1, 2*length(xdata) - 1); 
        newXdata(1:length(xdata) - 1) = -1*xdata(length(xdata):-1:2); 
        newXdata(length(xdata):length(newXdata)) = xdata;
        newYdata = zeros(1, 2*length(ydata) - 1);
        newYdata(1:length(ydata) - 1) = ydata(length(ydata):-1:2); 
        newYdata(length(ydata):length(newYdata)) = ydata;
        set(spectrumCurve, 'XData', newXdata, 'YData', newYdata);
    end
end
% Change xlim, colors and linewidth 
FsVect = zeros(1, length(activeSpectra));
matrixColors = [0 0 0; 0 0 1; 0 1 0; 1 0 0; 0.5 0 0.5; 0.8 0.8 0; 0 1 1; 0.8 0 0.8; 1 0.5 1; 0.42 0.42 0.42; 0.44 0.7 0.15; 1 0.65 0; 0.1 0.1 0.44; 0.34 0.42 0.19; 0 0.77 0.80; 0.65 0.17 0.17];
lw = 1.5;
for k = 1:N
    FsVect(k) = spectra(activeSpectra(k)).Fs;
    spectrumCurve = findobj('Tag', spectra(activeSpectra(k)).label);
    %randColor = [rand(1, 1) rand(1, 1) rand(1, 1)];
    spectrumLabel = spectra(activeSpectra(k)).label;
    spectrumInd = findcstr({spectra.label}, spectrumLabel); % Spectrum index
    set(spectrumCurve, 'Color', matrixColors(spectrumInd, :), 'Linewidth', lw);
    %set(spectrumCurve, 'Color', randColor, 'Linewidth', lw);
end
set(findobj('Tag', 'mainaxes'), 'XLim', [-0.5*max(FsVect) 0.5*max(FsVect)]);
  
%% Change title
set(findobj('Tag', 'mainaxestitle'), 'String', 'Power Spectral Density');

%% Spectra legends
str = cell(1, N);
h = zeros(N, 1);
for k = 1:N
    str(k) = {strjoin(strsplit(spectra(activeSpectra(k)).label, 'spect_'), '')};
    str(k) = {strjoin(strsplit(char(str(k)), '_'), ' ')};
    h(k) = findobj('Tag', spectra(activeSpectra(k)).label);
end
legend(flipud(h'), str);

%% Change initial printf markers formats 
eFormat = '%5.3e';
fFormat = '%5.3f';
rb1 = findobj('Tag', 'rulerbox1'); set(rb1, 'String', sprintf(eFormat, get(rb1, 'UserData')));  
rb2 = findobj('Tag', 'rulerbox2'); set(rb2,'String', sprintf(eFormat, get(rb2, 'UserData')));    
y1t = findobj('Tag', 'y1text'); set(y1t, 'String', sprintf(fFormat, str2double(get(y1t,'String'))));
y2t = findobj('Tag','y2text'); set(y2t, 'String', sprintf(fFormat, str2double(get(y2t, 'String'))));    
dxt = findobj('Tag', 'dxtext'); set(dxt, 'String', sprintf(eFormat, str2double(get(dxt,'String'))));
dyt = findobj('Tag', 'dytext'); set(dyt, 'String', sprintf(fFormat, str2double(get(dyt, 'String')))); 

%% Change rulerlines and rulermarkers linewidth
lw = 1.5;
rl1 = findobj('Tag', 'ruler1line'); set(rl1, 'Linewidth', lw);
rl2 = findobj('Tag', 'ruler2line'); set(rl2, 'Linewidth', lw);
rm1 = findobj('Tag', 'ruler1marker'); set(rm1, 'Linewidth', lw);
rm2 = findobj('Tag', 'ruler2marker'); set(rm2, 'Linewidth', lw);

%% Update rulerlines and rulermarkers
sbswitch('ruldown', 1)
sbswitch('ruldown', 2);
sbswitch('spmotion');

%% Grid 'on'
grid(ud.mainaxes, 'on');
%%

function [ type, symbolPeriod, samplingPeriod, flagT ] = readSignalHeader( fid )
%READSIGNALHEADER Reads a signal header file to "visualizer".
%   [ type, symbolPeriod, samplingPeriod, flagT ] = READSIGNALHEADER( fid )
%   just reads a header file ("fid")
%   returning the data parameters ("type", "symbolPeriod" and "samplingPeriod"). 
%   If flagT == 1, was found a terminator. 

%% Signal type
type = strsplit(fgetl(fid), {' ', ':'});
type = char(type(end));

%% Symbol period
symbolPeriod = strsplit(fgetl(fid), {' ', ':'});
symbolPeriod = str2double(char(symbolPeriod(end)));

%% Sampling period
samplingPeriod = strsplit(fgetl(fid), {' ', ':'});
samplingPeriod = str2double(char(samplingPeriod(end)));

%% Terminator and flagT
str = '';
terminator = '// ### HEADER TERMINATOR ###';
counterT = 0; flagT = 1;
while ~strcmp(str, terminator)
    str = fgetl(fid);
    counterT = counterT + 1;
    if counterT > 100
        flagT = 0;
        break;
    end
end
%%
   
%%

function [ ] = loadSignal( data, samplingFrequency, type, samplesPerSymbol, name )
%LOADSIGNAL Loads a signal to "sptool"/"visualizer".
%   LOADSIGNAL(data, samplingFrequency, type, samplesPerSymbol, name)
%   just loads a signal to simulator knowing the struct parameters
%   ("data"; "samplingFrequency"; "type"; "samplesPerSymbol" and name).

%% Some Standard types
tb = 'Binary';
td1 = 'TimeDiscreteAmplitudeDiscreteReal';
td2 = 'TimeDiscreteAmplitudeDiscreteComplex';
td3 = 'TimeDiscreteAmplitudeContinuousReal';
td4 = 'TimeDiscreteAmplitudeContinuousComplex';

%% Load
struct = sptool('create', 'Signal', data, samplingFrequency, name);
%struct.data;
%struct.Fs;
%struct.type = 'vector';
struct.type = type;
%struct.lineinfo.color = [1 0 0];
%struct.lineinfo.linestyle = '-';
%struct.lineinfo.columns = 1;
%struct.SPTIdentifier.type = 'Signal';
%struct.SPTIdentifier.version = '1.0';
struct.SPTIdentifier.version = samplesPerSymbol;
%struct.label;
sptool('load', struct);

%% Create spectrum
if ~strcmp(type, tb) && ~strcmp(type, td1) && ~strcmp(type, td2) && ~strcmp(type, td3) && ~strcmp(type, td4) % Time continuous signals
    arg = {'spect_', name};
    struct = sptool('create', 'Spectrum', [], [], strjoin(arg, ''));
    %struct.P;
    %struct.f;
    %struct.confid.enable = 0;
    %struct.confid.level = '.95';
    %struct.confid.Pc = [];
    %struct.specs.valueArrays = {{7x1 cell} {1x1 cell}};
    %struct.specs.methodName = {'Welch' 'FFT'};
    %struct.specs.methodNum = 2;
    struct.signal = [length(data) 1 0];
    struct.signalLabel = name;
    struct.Fs = samplingFrequency;
    %struct.type = 'auto';
    %struct.lineinfo.color = [1 0 0];
    %struct.lineinfo.linestyle = '-';
    %struct.lineinfo.columns = 1;
    %struct.SPTIdentifier.type = 'Spectrum';
    %struct.SPTIdentifier.version = '1.1';
    %struct.label;
    sptool('load', struct);
end
%%

function [ flagT ] = readImpHeader( fid )
%READIMPHEADER Reads a impulse response header file to "visualizer".
%   [ flagT ] = READIMPHEADER( fid ) just reads a header file ("fid").
%   If flagT == 1, was found a terminator. 

%% Terminator and flagT
str = '';
terminator = '// ### HEADER TERMINATOR ###';
counterT = 0; flagT = 1;
while ~strcmp(str, terminator)
    str = fgetl(fid);
    counterT = counterT + 1;
    if counterT > 100
        flagT = 0;
        break;
    end
end
%%

function [ time, data, samplingFrequency ] = readImpData( fid )
%READIMPDATA Reads impulse response data to "visualizer".
%   [ time, data, samplingFrequency ] = READIMPDATA(fid)
%   just reads data ("data") from a file ("fid")
%   returning the time vector ("time") and the sampling frequency ("samplingFrequency").

%% Read data
str = fgetl(fid); % First line
nMax = 10000;
time = ones(1, nMax).*(-100);
data = ones(1, nMax).*(-100);
inc = 1;
while str ~= -1
    str2 = strsplit(str, ' ');
    time(inc) = str2double(char(str2(1)));
    data(inc) = str2double(char(str2(2)));
    str = fgetl(fid);
    inc = inc + 1;
end
time = time(time ~= -100);
data = data(data ~= -100);

%% Sampling period
if time(1) < 0
    samplingPeriod = abs(time(1)) - abs(time(2));
else
    samplingPeriod = time(2) - time(1);
end

%% Sampling frequency 
samplingFrequency = 1/samplingPeriod;
%%

function [ ] = loadImp( time, data, samplingFrequency, type, name )
%LOADIMP Loads a impulse response to "sptool"/"visualizer".
%   LOADIMP(time, data, samplingFrequency, type, name)
%   just loads a impulse response to simulator knowing the struct parameters
%   ("time"; "data"; "samplingFrequency"; "type" and name).

%% Load 
struct = sptool('create', 'Filter', 'tf', 1, 1, samplingFrequency, name);
%struct.tf;
%struct.ss = [];
%struct.zpk = [];
%struct.sos = [];
struct.imp = data;
%struct.step = [];
struct.t = time;
%struct.H;
%struct.G = [];
%struct.f;
%struct.specs = 'currentModule: 'fdfirls'';
%struct.Fs;
%struct.type = 'design';
struct.type = type;
%struct.lineinfo.color = [1 0 0];
%struct.lineinfo.linestyle = '-';
%struct.SPTIdentifier.type = 'Filter';
%struct.SPTIdentifier.version = '2.0';
%struct.label;
%struct.FDAspecs = ...;
sptool('load', struct);

%% Create spectrum
arg = {'spect_', name};
struct = sptool('create', 'Spectrum', [], [], strjoin(arg, ''));
%struct.P;
%struct.f;
%struct.confid.enable = 0;
%struct.confid.level = '.95';
%struct.confid.Pc = [];
%struct.specs.valueArrays = {{7x1 cell} {1x1 cell}};
%struct.specs.methodName = {'Welch' 'FFT'};
%struct.specs.methodNum = 2;
struct.signal = [length(data) 1 0];
%struct.signalLabel = name;
struct.Fs = samplingFrequency;
%struct.type = 'auto';
%struct.lineinfo.color = [1 0 0];
%struct.lineinfo.linestyle = '-';
%struct.lineinfo.columns = 1;
%struct.SPTIdentifier.type = 'Spectrum';
%struct.SPTIdentifier.version = '1.1';
%struct.label;
sptool('load', struct);
%%
 
function [ phaser ] = calculatePhase( interval, complexVector )
%CALCULATEPHASE calculates the phase of a complex vector.
%   phaser = CALCULATEPHASE(interval, complexVector)
%   just computes the phase ("phaser") of a complex vector
%   ("complexVector") between [-pi pi] if interval = 0, between [0, 2*pi]
%   if interval = 1 and between ]-inf inf[ if interval = 2.

%% Calculate phase
N = length(complexVector);
phaser = zeros(1, N);
for k = 1:N
    
    %% Real and imaginary parts
    re = real(complexVector(k));
    im = imag(complexVector(k));
    
    result = abs(im)/abs(re);
    
    %% If interval is "0"
    if interval == 0
        if re > 0 && im > 0
            phaser(k) = atan(result);
        end
        if re > 0 && im < 0
            phaser(k) = -atan(result);
        end
        if re < 0 && im > 0
            phaser(k) = atan(1/result) + pi/2;
        end
        if re < 0 && im < 0
            phaser(k) = -atan(1/result) - pi/2;
        end
        if re == 0 && im == 0
            phaser(k) = 0;
        end
        if re == 0 && im > 0
            phaser(k) = pi/2;
        end
        if re == 0 && im < 0
            phaser(k) = -pi/2;
        end
        if re > 0 && im == 0
            phaser(k) = 0;
        end
        if re < 0 && im == 0
            phaser(k) = pi;
        end
    end
    
    %% If interval is "1" 
    if interval == 1 
        if re > 0 && im > 0
            phaser(k) = atan(result);
        end
        if re > 0 && im < 0
            phaser(k) = 2*pi - atan(result);
        end
        if re < 0 && im > 0
            phaser(k) = atan(1/result) + pi/2;
        end
        if re < 0 && im < 0
            phaser(k) = 3*(pi/2) - atan(1/result);
        end
        if re == 0 && im == 0
            phaser(k) = 0;
        end
        if re == 0 && im > 0
            phaser(k) = pi/2;
        end
        if re == 0 && im < 0
            phaser(k) = 3*(pi/2);
        end
        if re > 0 && im == 0
            phaser(k) = 0;
        end
        if re < 0 && im == 0
            phaser(k) = pi;
        end
    end

end

%% If interval is "2"
if interval == 2
    phaser = unwrap(angle(complexVector));
end    
%%

% function [ ] = closeComplexFigures( ~ )
% %CLOSECOMPLEXFIGURES closes all complex representations from "visualizer".
% %   CLOSECOMPLEXFIGURES just closes complex figures.
% 
% %% Close figures
% close(findobj('Tag', 'figCompRI'));
% close(findobj('Tag', 'figCompModPhase'));
% close(findobj('Tag', 'figCompMod2Phase'));
% %%

% [EOF] sptool.m




