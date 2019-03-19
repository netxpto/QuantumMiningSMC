function varargout = sptimport(varargin)
%SPTIMPORT Import Dialog box for importing data into the SPTool.
%   [componentNum,struc,importSettings,wd] = ...
%                  sptimport(components,labelList,importSettings,wd)
%     puts up a dialog box allowing a user to create a SPTool structure
%     from MATLAB variables, or to select an exported SPTool structure.
%     Inputs:
%        components - data structure created by sptregistry functions,
%          contains information about the components of this SPTool session.
%        labelList - list of strings - current variable labels in the SPTool.
%          sptimport will prompt user if he specifies a label which already
%          exists in this list
%        importSettings - if [], the default settings are chosen
%          otherwise uses importSettings from last call to this function.
%          this is a way to remember the settings the user last typed into
%          this dialog box.
%        wd - working directory - location to begin browsing for files
%     Outputs:
%        componentNum - 0 if user hit cancel, 1..N if success; value indicates
%         which component this structure belongs to
%        struc - new SPTool structure (may have same name as existing object)
%        importSettings - use this to call this dialog box next time.
%        wd - working directory - end location for file browsing

%   Copyright 1988-2012 The MathWorks, Inc.

if nargin == 0
    action = get(gcbo,'Tag');
elseif isstr(varargin{1})
    action = varargin{1};
else
    action = 'init';
end

switch action
  case 'init'  
    if isempty(varargin{3})|~ishandle(varargin{3}.fig)
      % Open figure and create ui objects
      initImport
      ud = get(gcf,'UserData');
      ud.fullFileName = '';
      ud.fileNameString = '';
      ud.FsString = '1';
      set(ud.h.editFs,'String',ud.FsString);
      ud.fig = gcf;
        ud.labelList = varargin{2};
      ud.struc = [];
      
      set([ud.h.filename ud.h.filenameLabel ud.h.browse],'Enable','off')
      minwidth([ud.h.SourceLabel ud.h.ContentsLabel ud.h.labelLabel])

      % More initialization code here

      components = varargin{1};
      ud.wd = varargin{4};  % working directory
      numComponents = length(components);
      importAsString = cell(1,numComponents);
      for i=1:numComponents
        [popupString,fields,FsFlag,defaultLabel] = ...
            feval(components(i).importFcn,'fields');
        ud.FsFlag(i) = FsFlag;
        importAsString{i} = popupString;
        ud.fields{i} = fields;
        ud.importFcn{i} = components(i).importFcn;
        ud.defaultLabel{i} = uniqlabel(ud.labelList,defaultLabel);
        ud.formValue(i) = 1;
        for j = 1:length(fields)
          ud.fieldStrings{i}{j} = cell(1,length(fields(j).fields));
          for k = 1:length(fields(j).fields)
              ud.fieldStrings{i}{j}{k} = '';
          end
        end
      end
      set(ud.h.importas,'String',importAsString,'Value',1)
      set(gcf,'UserData',ud)
      changeComponent
    
      getContentsOfWorkspace

    else
      ud = varargin{3};
      figure(ud.fig)
      ud.labelList = varargin{2};
      components = varargin{1};
      ud.wd = varargin{4};  % working directory
      numComponents = length(components);
      for i = 1:numComponents
          [popupString,fields,FsFlag,defaultLabel] = ...
                      feval(components(i).importFcn,'fields');
          ud.defaultLabel{i} = uniqlabel(ud.labelList,defaultLabel);
      end
      currentComponent = get(ud.h.importas,'Value');
      set(ud.h.editLabel,'String',ud.defaultLabel{currentComponent})
      set(ud.fig,'UserData',ud)
      if ispc
      % on UNIX, already modal
          set(ud.fig,'WindowStyle','modal')
      end
      set(ud.fig,'Visible','on')
      if get(ud.h.radio1,'Value')==1
          getContentsOfWorkspace
      else
          getContentsOfFile
      end
    end

    % Initialization done ... now wait for OK or Cancel buttons:
    set(ud.h.OKButton,'UserData','')
    waitfor(ud.h.OKButton, 'UserData')
    
    ud = get(ud.fig,'UserData');
    
    switch get(ud.h.OKButton,'UserData')
    case 'OK'
       struc = ud.struc;
       componentNum = get(ud.h.importas,'Value');
    case 'Cancel'
       componentNum = 0;
       struc = [];
    end
    
    set(ud.fig,'Visible','off')
    if ispc
    % prevent modal focus on invisible window when not on UNIX
        set(ud.fig,'WindowStyle','normal')
    end
    
    % delete help objects if they are there:
    delete(findobj(ud.fig,'tag','importhelp'))
    
    varargout = {componentNum,struc,ud,ud.wd};
    
  case 'radio1'
    % Callback code for radiobutton with Tag "radio1"
    % Source: from Workspace
    ud = get(gcf,'UserData');
    val = get(ud.h.radio1,'Value');
    if val==0   % User has clicked on this radio even though it was
                % already on - so leave it on and exit
        set(ud.h.radio1,'Value',1)
    else
        set(ud.h.radio2,'Value',0)
        set([ud.h.filename ud.h.filenameLabel ud.h.browse],'Enable','off')
        set(ud.h.ContentsLabel,'String',getString(message('signal:sptoolgui:WorkspaceContents')))
        minwidth(ud.h.ContentsLabel)
        getContentsOfWorkspace
    end
    
  case 'radio2'
    % Callback code for radiobutton with Tag "radio2"
    % Source: from Disk
    ud = get(gcf,'UserData');
    val = get(ud.h.radio2,'Value');
    if val==0   % User has clicked on this radio even though it was
                % already on - so leave it on and exit
        set(ud.h.radio2,'Value',1)
    else
        set(ud.h.radio1,'Value',0)
        set([ud.h.filename ud.h.filenameLabel ud.h.browse],'Enable','on')
        set(ud.h.ContentsLabel,'String',getString(message('signal:sptoolgui:FileContents')))
        minwidth(ud.h.ContentsLabel)
        getContentsOfFile
    end

  case 'filename'
    % Callback code for edit with Tag "filename"
    ud = get(gcf,'UserData');
    filename = get(ud.h.filename,'String');
    if isempty(find(filename=='.')) 
        if (length(filename)<4) | ~strcmp(lower(filename(end-3:end)),'.mat')  
            filename = [filename '.mat'];
            set(ud.h.filename,'String',filename)
        end
    end
    fullFileName = which(filename);
    if ~isempty(fullFileName)
        ud.fullFileName = fullFileName;
        ud.fileNameString = get(ud.h.filename,'String');
        set(gcf,'UserData',ud)
        getContentsOfFile
    else
        if (length(filename)<4) | ~strcmp(lower(filename(end-3:end)),'.mat'),
            errstr = getString(message('signal:sptoolgui:TheMATfilenamemustendMat'));
        else
            errstr = getString(message('signal:sptoolgui:SorryIcantfindthefile',filename));
        end
        waitfor(msgbox(errstr,getString(message('signal:sptoolgui:FileError')),'error','modal'))
        set(ud.h.filename,'String',ud.fileNameString)
    end
    
  case 'browse'
    % Callback code for pushbutton with Tag "browse"
    ud = get(gcf,'UserData');
    matlab_wd = pwd;
    cd(ud.wd)
    [f,p]=uigetfile('*.mat');
    cd(matlab_wd)
    if ~isequal(f,0)
        ud.fullFileName = fullfile(p,f);
        ud.fileNameString = f;
        ud.wd = p;
        set(ud.h.filename,'String',ud.fileNameString)
        set(gcf,'UserData',ud)
        getContentsOfFile
    end
  
  case 'listbox'
    % Callback code for listbox with Tag "listbox"
    ud = get(gcf,'UserData');
    val = get(ud.h.listbox,'Value');
    str = get(ud.h.listbox,'String');
    types = get(ud.h.listbox,'UserData');
    currentComponent = get(ud.h.importas,'Value');
    currentForm = get(ud.h.formPopup,'Value');
    hands = [ud.h.arrow1 ud.h.edit1 
             ud.h.arrow2 ud.h.edit2 
             ud.h.arrow3 ud.h.edit3 
             ud.h.arrow4 ud.h.edit4 
             ud.h.arrow5 ud.h.editFs];

    switch types(val)
    case -1   % selection not a SPT data type and not "transferable" to the
              % edit boxes by using the arrows
        set(hands(:,1),'Enable','off')
        set(hands(:,2),'Enable','on')
        % set edit strings
        strings = ud.fieldStrings{currentComponent}{currentForm}';
        set(hands(1:length(strings),2),{'String'},strings)
        set(ud.h.editFs,'String',ud.FsString)
        
    case 0    % selection is transferable
        set(hands(:,1:2),'Enable','on')
        % set edit strings
        strings = ud.fieldStrings{currentComponent}{currentForm}';
        set(hands(1:length(strings),2),{'String'},strings)
        set(ud.h.editFs,'String',ud.FsString)
             
    otherwise % selection is a valid SPT data type
              %  - can't transfer by arrows
              %  - might need to change component
              %  - set edit boxes to 'Enable','off' with special strings
        if get(ud.h.importas,'Value')~=types(val)
            set(ud.h.importas,'Value',types(val))
            changeComponent
            set(ud.h.listbox,'Value',val)
        end
        varName = str{val};
        ind = find(varName=='[');
        varName(ind-1:end)=[];
        workspaceFlag = get(ud.h.radio1,'Value');
        if ud.FsFlag(types(val))
           if workspaceFlag
               Fs = getStructureField(varName,'Fs');
           else
               Fs = getStructureField(varName,'Fs',ud.fullFileName);
           end
           set(ud.h.editFs,'String',sprintf('%.9g',Fs))
        end
        if workspaceFlag
            label = getStructureField(varName,'label');
        else
            label = getStructureField(varName,'label',ud.fullFileName);
        end
        set(ud.h.editLabel,'String',label)
        set(hands(:,1),'Enable','off')
        set(hands(1:4,2),'Enable','off','String',['<' label '>'])
    end
    
  case 'Help'
    % Callback code for pushbutton with Tag "Help"
    fig = gcf;
    uiList = findobj(fig,'type','uicontrol');
    saveVis = get(uiList,'Visible');
    if ispc
        set(uiList,'Visible','off')
    end
    ud = get(fig,'UserData');
    fp = get(fig,'Position');
    sz = sptsizes;
    f = uicontrol('Style','frame',...
           'Position',[sz.fus sz.fus fp(3)-2*sz.fus fp(4)-sz.fus-1],...
           'Tag','importhelp');
    tp = [2*sz.fus 4*sz.fus+sz.uh fp(3)-4*sz.fus fp(4)-(6*sz.fus+sz.uh)];
       % text position
    [fontname,fontsize]=fixedfont;
    t = uicontrol('Style','listbox','Position',tp,'String',importHelpStr,'Max',2,...
         'Tag','importhelp','HorizontalAlignment','left',...
         'BackgroundColor','w','FontName',fontname,'FontSize',fontsize);
    % bp = [2*sz.fus 2*sz.fus sz.bw sz.uh];  % button position
    bp = [27 16 60 20];  %-- use exact same pos as 'Help' button
    b = uicontrol('Style','pushbutton','Position',bp,...
         'Tag','importhelp','String','OK',...
         'Callback','delete(findobj(gcf,''tag'',''importhelp''))');
    waitfor(b)
    if all(ishandle(uiList))
        if ispc
            set(uiList,{'Visible'},saveVis)
        end
    end
    
  case 'Cancel'
    % Callback code for pushbutton with Tag "Cancel"
    ud = get(gcf,'UserData');
    % handle close request when in "Help" mode:
    b = findobj(gcf,'tag','importhelp');
    if ~isempty(b)
        delete(b)  % triggers waitfor in 'Help' callback
    end

    set(ud.h.OKButton,'UserData','Cancel')  % triggers waitfor in initial
                                            % function call
      
  case 'OKButton'
    % Callback code for pushbutton with Tag "OK"
    ud = get(gcf,'UserData');


    str = get(ud.h.listbox,'String');
    val = get(ud.h.listbox,'Value');
    types = get(ud.h.listbox,'UserData');
    currentComponent = get(ud.h.importas,'Value');
    currentForm = get(ud.h.formPopup,'Value');
    errstr = ''; % error string
    
    componentNames = get(ud.h.importas,'String');
    if types(val)>0  % import previously exported object
        varName = str{val};
        ind = find(varName=='[');
        varName(ind-1:end)=[];
        workspaceFlag = get(ud.h.radio1,'Value');
        if workspaceFlag
            ud.struc = getVariable(varName);
        else
            ud.struc = getVariable(varName,ud.fullFileName);
        end

        err = 0;        
        if ud.FsFlag(types(val))
           if workspaceFlag
               [ud.struc.Fs,err] = getVariable(get(ud.h.editFs,'String'));
           else
               [ud.struc.Fs,err] = getVariable(get(ud.h.editFs,'String'),...
                                                ud.fullFileName);
           end
           if err,
               errstr = getString(message('signal:sptoolgui:SamplingFrequencycannotbeevaluated'));
           end
        end
        
        if ~err
            [valid,ud.struc] = feval(ud.importFcn{currentComponent},...
                  'valid',ud.struc);
            if ~valid,
                errstr = getString(message('signal:sptoolgui:yourselectionisnotvalid',componentNames{currentComponent}));
            end
            err = ~valid;
        end
    else % make a new object
        % make a vector of handles
        strings = ud.fields{currentComponent}(currentForm).fields;
        hands = [ ud.h.edit1 
                  ud.h.edit2 
                  ud.h.edit3 
                  ud.h.edit4 ];
        hands = hands(1:length(strings));        
        if ud.FsFlag(currentComponent)
            hands = [hands; ud.h.editFs];
            strings{end+1} = get(ud.h.labelFs,'String');
        end
        params = cell(1,length(hands)+1);
        params{1} = get(ud.h.formPopup,'Value');
        if get(ud.h.radio2,'Value')
            getVariableParams{2} = ud.fullFileName;
        end
        for i=1:length(hands)
            getVariableParams{1} = get(hands(i),'String');
            [params{i+1},err] = getVariable(getVariableParams{:});
            if err,
               switch err
               case 1
                   errstr = getString(message('signal:sptoolgui:entrycouldnotbeevaluated',strings{i}));
               case 2
                 errstr = getString(message('signal:sptoolgui:entersomethinginthefield',...
                                            getTranslatedString('sptoolgui/private/sptimport',strings{i})));
               end
               break
            end
        end
        if ~err
            [err,errstr,ud.struc] = ...
                   feval(ud.importFcn{currentComponent},'make',params);
        end
    end

    if ~err
        label = get(ud.h.editLabel,'String');
        err = ~isvalidvar(label);
        if ~err
            if ~isempty(findcstr(ud.labelList,label))
                % prompt for over-write
                str2display = [getString(message('signal:sptoolgui:importingoverwrite1',label,label)) ...
                               getString(message('signal:sptoolgui:importingoverwrite2',label))];
                switch questdlg(...
                        str2display,...
                        getString(message('signal:sptoolgui:NameConflict')),getString(message('signal:sptoolgui:Yes')),getString(message('signal:sptoolgui:No')),getString(message('signal:sptoolgui:No')));
                    case getString(message('signal:sptoolgui:Yes'))
                        ud.struc.label = label;
                    case getString(message('signal:sptoolgui:No'))
                        return
                end
            else
                ud.struc.label = label;
            end
        else
            errstr = {getString(message('signal:sptoolgui:invalidentry1'))
                      getString(message('signal:sptoolgui:invalidentry2'))};
        end
    end
    
    set(gcf,'UserData',ud)

    if isempty(errstr)
        % now send signal that we are done to waitfor:
        set(ud.h.OKButton,'UserData','OK')
    else
        % put up error dialog box
        msgtitile = getString(message('signal:sptoolgui:ImportError'));
        h=msgbox(errstr,msgtitile,'error','modal');
        waitfor(h)
    end

  case 'importas'
    % Callback code for popupmenu with Tag "importas"
    changeComponent
    
  case 'formPopup'
    % Callback code for popupmenu with Tag "formPopup"
    ud = get(gcf,'UserData');
    currentComponent = get(ud.h.importas,'Value');
    currentForm = get(ud.h.formPopup,'Value');
    ud.formValue(currentComponent) = currentForm;
    set(gcf,'UserData',ud)
    changeComponent(1)
    
  case 'arrow1'
    % Callback code for pushbutton with Tag "arrow1"
    ud = get(gcf,'UserData');
    str = get(ud.h.listbox,'String');
    val = get(ud.h.listbox,'Value');
    set(ud.h.edit1,'String',str{val})
    editStringChange(1)
    
  case 'arrow2'
    % Callback code for pushbutton with Tag "arrow2"
    ud = get(gcf,'UserData');
    str = get(ud.h.listbox,'String');
    val = get(ud.h.listbox,'Value');
    set(ud.h.edit2,'String',str{val})
    editStringChange(2)

  case 'arrow3'
    % Callback code for pushbutton with Tag "arrow3"
    ud = get(gcf,'UserData');
    str = get(ud.h.listbox,'String');
    val = get(ud.h.listbox,'Value');
    set(ud.h.edit3,'String',str{val})
    editStringChange(3)

  case 'arrow4'
    % Callback code for pushbutton with Tag "arrow4"
    ud = get(gcf,'UserData');
    str = get(ud.h.listbox,'String');
    val = get(ud.h.listbox,'Value');
    set(ud.h.edit4,'String',str{val})
    editStringChange(4)

  case 'arrow5'
    % Callback code for pushbutton with Tag "arrow5"
    ud = get(gcf,'UserData');
    str = get(ud.h.listbox,'String');
    val = get(ud.h.listbox,'Value');
    set(ud.h.editFs,'String',str{val})
    ud.FsString = get(ud.h.editFs,'String');
    set(gcf,'UserData',ud)
  
  case 'edit1'
    % Callback code for edit with Tag "edit1"
    editStringChange(1)
    
  case 'edit2'
    % Callback code for edit with Tag "edit2"
    editStringChange(2)
    
  case 'edit3'
    % Callback code for edit with Tag "edit3"
    editStringChange(3)
    
  case 'edit4'
    % Callback code for edit with Tag "edit4"
    editStringChange(4)
    
  case 'editFs'
    % Callback code for edit with Tag "editFs"
    ud = get(gcf,'UserData');
    ud.FsString = get(ud.h.editFs,'String');
    set(gcf,'UserData',ud)
    
end

function editStringChange(i)
%editStringChange  sets userdata structure which saves the strings
%                  entered into the import dialog box
%     i is between 1 and 4
%     userdata is changed
    ud = get(gcf,'UserData');
    currentComponent = get(ud.h.importas,'Value');
    currentForm = get(ud.h.formPopup,'Value');
    eval(['ud.fieldStrings{currentComponent}{currentForm}{i} = ' ...
          'get(ud.h.edit' num2str(i) ',''string'');']);
    set(gcf,'UserData',ud)

function getContentsOfFile
  % Called by 'browse' button and callback of filename edit box,
  % also when 'From Disk' radio button is clicked

  ud = get(gcf,'UserData');
  if isempty(ud.fullFileName)
    set(ud.h.listbox,'String',{'<no file selected>'},'Value',1,'UserData',-1)
  else
    whosString = ['w=whos(''-file'',''' ud.fullFileName ''');'];
    err=0;
    eval(whosString,'err=1;')
    if err
        set(ud.h.listbox,'String',{'<file not found>'},'Value',1,'UserData',-1)
    else
        if length(w) == 0
            set(ud.h.listbox,'String',{'<file empty>'},'Value',1,'UserData',-1)
        else
            listString = {'<no selection>' w.name};
            [type,listString] = componentMarkup(listString,w,ud.fullFileName);
            set(ud.h.listbox,'String',listString,'Value',1,'UserData',type)
        end
    end
  end
  selectNothing
  
function getContentsOfWorkspace
  % Called when 'From Workspace' radio button is clicked
  % and (possibly) at initialization time
  
  ud = get(gcf,'UserData');
  w = evalin('base','whos');
  if length(w) == 0
      set(ud.h.listbox,'String',{'<no variables>'},'Value',1,'UserData',-1)
  else
      listString = {'<no selection>' w.name};
      [type,listString] = componentMarkup(listString,w);
      set(ud.h.listbox,'String',listString,'Value',1,'UserData',type)
  end
  selectNothing
  

function selectNothing
  ud = get(gcf,'UserData');
  currentComponent = get(ud.h.importas,'Value');
  currentForm = get(ud.h.formPopup,'Value');
  hands = [ud.h.arrow1 ud.h.edit1 
             ud.h.arrow2 ud.h.edit2 
             ud.h.arrow3 ud.h.edit3 
             ud.h.arrow4 ud.h.edit4 
             ud.h.arrow5 ud.h.editFs];
  set(hands(:,1),'Enable','off')
  set(hands(:,2),'Enable','on')
  % set edit strings
  strings = ud.fieldStrings{currentComponent}{currentForm}';
  set(hands(1:length(strings),2),{'String'},strings)
  set(ud.h.editFs,'String',ud.FsString)


function [type,listString] = componentMarkup(listString,w,fname)
%componentMarkup - identify types of objects in workspace or MAT-file
%  Inputs:
%      listString - cell array - the first element is ignored, the remaining
%        elements are string variable names in the workspace or file
%      w - whos structure from workspace or MAT-file
%      fname - if present, specifies full MAT-file name (directory & filename)
%              if not present, componentMarkup expects to find the variables in 
%                the WORKSPACE.
%  Outputs:
%      type - vector of integers, 1 element for each string in listString
%                 -1  --> do not allow transfer with the arrow buttons
%                         (unknown SPT object or the first element in the list)
%                  0  --> normal MATLAB vector; allow transfer with arrow buttons
%                  1..n (where n = number of components) -->
%                         SPT object identified, number corresponds to position
%                         in string of ud.h.importas
%      listString - list of strings for ud.h.listbox, edited to indicate
%                  various SPT objects

ud = get(gcf,'UserData');
if nargin == 2    % FROM WORKSPACE
    workspaceFlag = 1;
else
    workspaceFlag = 0;
end

type = zeros(1,length(listString));
type(1) = -1;
dataNames = get(ud.h.importas,'String');
%versions = cell(length(dataNames));
%for i = 1:length(ud.importFcn)
%    versions{i} = feval(ud.importFcn{i},'version');
%end
for i = 2:length(listString)
    if strcmp(w(i-1).class,'struct') & isequal(w(i-1).size,[1 1])
       if workspaceFlag
           SPTIdent = getStructureField(listString{i},'SPTIdentifier');
       else
           SPTIdent = getStructureField(listString{i},'SPTIdentifier',fname);
       end
       if ~isempty(SPTIdent)
           ind = find(strcmp(dataNames,SPTIdent.type));
           if isempty(ind)
               type(i) = -1;
           else
               type(i) = ind;
           end
       end
       switch type(i)
       case -1
           listString{i} = [listString{i} ' [Unknown]'];
       case 0
           % do nothing
       otherwise
           listString{i} = ...
                 [listString{i} ' [' dataNames{ind} ']'];
       end
    end
end

function varargout = getStructureField(varargin)
%getStructureField
% field = getStructureField(varName,fieldName,fname)
% Returns the field 'fieldName' of the structure 'varName' in the
% MAT-file 'fname'.  fname is optional; if you don't specify it, this
% function looks in the workspace instead.

if nargin == 2  % workspace
    varargout{1} = evalin('base',[varargin{1} '.' varargin{2}],'[]');
else  % MAT-file
    load(varargin{3},varargin{1})
    varargout{1} = eval([varargin{1} '.' varargin{2}],'[]');
end


function varargout = getVariable(varargin)
%getVariable
% [var,err] = getVariable(varName,fname)
% Returns the var 'varName' in the MAT-file 'fname'.  fname is optional; 
% if you don't specify it, this function looks in the workspace instead.
% If 'varName' is not found in fname, the string is evaluated in this function's
% workspace;
% err = 2 if the string is empty,
%       1 if there is an error in evaluating the string, 
%       0 if OK
GETVARIABLE_ERROR = 0;  % this name  needs to be long and ugly to minimize 
                           % chances of a (still possible) name clash
if isempty(varargin{1})
    varargout{1} = [];
    varargout{2} = 2;
    return
end
if nargin == 1  % workspace
    varargout{1} = evalin('base',varargin{1},'''ARBITRARY_STRING''');
    if isequal(varargout{1},'ARBITRARY_STRING')
        GETVARIABLE_ERROR = 1;
    end
else  % MAT-file
    w = warning('off');
    eval(['load(''' varargin{2} ''',''' varargin{1} ''')'],'GETVARIABLE_ERROR=1;')
    eval(['varargout{1}=' varargin{1} ';'],'GETVARIABLE_ERROR=1;');
    warning(w);
end
if GETVARIABLE_ERROR
    varargout{1} = [];
end
varargout{2}=GETVARIABLE_ERROR;


function changeComponent(formFlag)
%changeComponent - set Import As: area of dialog according to components
% if formFlag is present, the 'form' popup has changed (not the
% 'Import As' popup).

    ud = get(gcf,'UserData');
    currentComponent = get(ud.h.importas,'Value');
    currentForm = get(ud.h.formPopup,'Value');
    if currentForm ~= ud.formValue(currentComponent)
        currentForm = ud.formValue(currentComponent);
        set(ud.h.formPopup,'Value',currentForm)
    end
    set(ud.h.formPopup,'Value',ud.formValue(currentComponent))
    if length(ud.fields{currentComponent}) == 1
        set([ud.h.Form ud.h.formPopup],'Visible','off')
    else
        set([ud.h.Form ud.h.formPopup],'Visible','on')
    end
    set(ud.h.formPopup,'String',{ud.fields{currentComponent}.form})
    hands = [ud.h.arrow1 ud.h.edit1 ud.h.label1
             ud.h.arrow2 ud.h.edit2 ud.h.label2
             ud.h.arrow3 ud.h.edit3 ud.h.label3
             ud.h.arrow4 ud.h.edit4 ud.h.label4
             ud.h.arrow5 ud.h.editFs ud.h.labelFs];

    currentLabels = ...
         ud.fields{currentComponent}(currentForm).fields;
    set(hands(1:length(currentLabels),:),'Visible','on')
    set(hands(length(currentLabels)+1:4,:),'Visible','off')
    
    % set labels:
    set(hands(1:length(currentLabels),3),{'String'},currentLabels')
 
    types = get(ud.h.listbox,'UserData');
    
    if nargin<1
       % if an SPT Object is selected in the listbox, deselect it
       % in the process of changing Components
       if types(get(ud.h.listbox,'Value')) > 0
           set(ud.h.listbox,'Value',1)  % no selection!
           set(hands(:,1),'Enable','off')
           set(hands(:,2),'Enable','on')
       end
       set(ud.h.editLabel,'String',ud.defaultLabel{currentComponent})
       if ~ud.FsFlag(currentComponent)
           set(hands(5,:),'Visible','off')
       else
           set(hands(5,:),'Visible','on')
       end
    end
    
    if types(get(ud.h.listbox,'Value')) <= 0
        % if previously exported structure is not selected, update edit strings
        set(hands(1:length(currentLabels),2),{'String'},...
            ud.fieldStrings{currentComponent}{currentForm}')
    end
    
function initImport
% This is the [HAND EDITED] machine-generated representation of a 
% MATLAB object
% and its children.  Note that handle values may change when these
% objects are re-created. This may cause problems with some callbacks.
% The command syntax may be supported in the future, but is currently 
% incomplete and subject to change.
%
% To re-open this system, just type the name of the MATLAB file at the MATLAB
% prompt. The MATLAB file and its associated MAT-file must be on your path.
ud = [];
ss = get(0,'ScreenSize');
h = 308;
w = 587;  % height and width in pixels
% Place window's upper left corner at [40,60] offset from upper left
fp = [40 ss(4)-h-60 w h];
a = figure('Color',get(0,'DefaultUicontrolBackgroundColor'), ...
   'Position',fp, ...
   'DockControls','off',...
   'IntegerHandle','off',...
   'Name',getString(message('signal:sptoolgui:ImportToSPTool')),...
   'NumberTitle','off',...
   'Resize','off',...
   'CloseRequestFcn','sbswitch(''sptimport'',''Cancel'')',...
   'Tag','Fig1',...
   'WindowStyle','modal',...
   'HandleVisibility','Callback',...
   'Units','pixels',...
   'MenuBar','none');
b = uicontrol('Parent',a, ...
   'Units','pixels',...
     'Position',[6 5 324 39], ...
   'Style','frame', ...
   'Tag','Frame6');
b = uicontrol('Parent',a, ...
   'Units','pixels',...
   'Position',[353 5 228 38], ...
   'Style','frame', ...
   'Tag','Frame5');
b = uicontrol('Parent',a, ...
   'Units','pixels',...
   'Position',[354 53 228 244], ...
   'Style','frame', ...
   'Tag','Frame4');
b = uicontrol('Parent',a, ...
   'Units','pixels',...
   'Position',[159 53 147 245], ...
   'Style','frame', ...
   'Tag','Frame3');
b = uicontrol('Parent',a, ...
   'Units','pixels',...
   'Position',[7 53 141 245], ...
   'Style','frame', ...
   'Tag','Frame2');
b = uicontrol('Parent',a, ...
   'Units','pixels',...
   'Position',[11 151 133 101], ...
   'Style','frame', ...
   'Visible','off',...
   'Tag','Frame1');
b = uicontrol('Parent',a, ...
   'Units','pixels',...
   'Callback','sbswitch(''sptimport'')', ...
   'Position',[12 259 125 20], ...
   'String',getString(message('signal:sptoolgui:FromWorkspace')), ...
   'Style','radiobutton', ...
   'Tag','radio1', ...
   'Value',1);
ud = addToUserData(ud,b);
b = uicontrol('Parent',a, ...
   'Units','pixels',...
   'Callback','sbswitch(''sptimport'')', ...
   'Position',[12 238 107 20], ...
   'String',getString(message('signal:sptoolgui:FromDisk')), ...
   'Style','radiobutton', ...
   'Tag','radio2', ...
   'Value',0);
ud = addToUserData(ud,b);
if ispc
    pos=[27 219 106 20];
else 
    pos=[27 215 106 20];
end
b = uicontrol('Parent',a, ...
   'Units','pixels',...
   'HorizontalAlignment','left', ...
   'Position',pos, ...
   'String',getString(message('signal:sptoolgui:MATfileName')), ...
   'Style','text', ...
   'Tag','filenameLabel');
ud = addToUserData(ud,b);
b = uicontrol('Parent',a, ...
   'Units','pixels',...
   'BackgroundColor',[1 1 1], ...
   'Callback','sbswitch(''sptimport'')', ...
   'HorizontalAlignment','left',...
   'Position',[28 195 110 24], ...
   'Style','edit', ...
   'Tag','filename');
ud = addToUserData(ud,b);
b = uicontrol('Parent',a, ...
   'Units','pixels',...
   'Callback','sbswitch(''sptimport'')', ...
   'Position',[30 170 106 20], ...
   'String',getString(message('signal:sptoolgui:Browse')), ...
   'Tag','browse');
ud = addToUserData(ud,b);
b = uicontrol('Parent',a, ...
   'Units','pixels',...
   'Position',[18 286 116 20], ...
   'String',getString(message('signal:sptoolgui:Source')), ...
   'Style','text', ...
   'Tag','SourceLabel');
ud = addToUserData(ud,b);
b = uicontrol('Parent',a, ...
   'Units','pixels',...
   'Position',[166 285 136 20], ...
   'String',getString(message('signal:sptoolgui:WorkspaceContents')), ...
   'Style','text', ...
   'Tag','ContentsLabel');
ud = addToUserData(ud,b);
b = uicontrol('Parent',a, ...
   'Units','pixels',...
   'BackgroundColor',[1 1 1], ...
   'Callback','sbswitch(''sptimport'')', ...
   'Position',[165 58 136 225], ...
   'Style','listbox', ...
   'Tag','listbox', ...
   'UserData',-1,...
   'Value',1);
ud = addToUserData(ud,b);
b = uicontrol('Parent',a, ...
   'Units','pixels',...
   'Callback','sbswitch(''sptimport'')', ...
   'Position',[247 16 60 20], ...
   'String',getString(message('signal:sptoolgui:Help')), ...
   'Tag','Help');
b = uicontrol('Parent',a, ...
   'Units','pixels',...
   'Callback','sbswitch(''sptimport'')', ...
   'Position',[137 16 60 20], ...
   'String',getString(message('signal:sptoolgui:Cancel')), ...
   'Tag','Cancel');
ud = addToUserData(ud,b);
b = uicontrol('Parent',a, ...
   'Units','pixels',...
   'Callback','sbswitch(''sptimport'')', ...
   'Position',[27 16 60 20], ...
   'String',getString(message('signal:sptoolgui:OK')), ...
   'Tag','OKButton');
ud = addToUserData(ud,b);
b = uicontrol('Parent',a, ...
   'Units','pixels',...
   'Position',[358 283 74 20], ...
   'String',getString(message('signal:sptoolgui:ImportAs')), ...
   'Style','text', ...
   'Tag','StaticText2');
b = uicontrol('Parent',a, ...
   'Units','pixels',...
   'Callback','sbswitch(''sptimport'')', ...
   'Position',[431 283 117 20], ...
   'String',' ', ...
   'Style','popupmenu', ...
   'BackgroundColor','white',...
   'Tag','importas', ...
   'Value',1);
ud = addToUserData(ud,b);
b = uicontrol('Parent',a, ...
   'Units','pixels',...
   'Position',[370 258 60 20], ...
   'String',getString(message('signal:sptoolgui:Form')), ...
   'Style','text', ...
   'Tag','Form');
ud = addToUserData(ud,b);
b = uicontrol('Parent',a, ...
   'Units','pixels',...
   'Callback','sbswitch(''sptimport'')', ...
   'Position',[420 259 145 20], ...
   'String',' ', ...
   'Style','popupmenu', ...
   'BackgroundColor','white',...
   'Tag','formPopup', ...
   'Value',1);
ud = addToUserData(ud,b);
b = uicontrol('Parent',a, ...
   'Units','pixels',...
   'BackgroundColor',[1 1 1], ...
   'HorizontalAlignment','left', ...
   'Position',[363 228 110 21], ...
   'Style','edit', ...
   'Callback','sbswitch(''sptimport'')', ...
   'Tag','edit1');
ud = addToUserData(ud,b);
b = uicontrol('Parent',a, ...
   'Units','pixels',...
   'HorizontalAlignment','left', ...
   'Position',[478 228 100 20], ...
   'String','Label1', ...
   'Style','text', ...
   'Tag','label1');
ud = addToUserData(ud,b);
b = uicontrol('Parent',a, ...
   'Units','pixels',...
   'Callback','sbswitch(''sptimport'')', ...
   'Position',[312 228 36 20], ...
   'String','-->', ...
   'Tag','arrow1');
ud = addToUserData(ud,b);
b = uicontrol('Parent',a, ...
   'Units','pixels',...
   'Callback','sbswitch(''sptimport'')', ...
   'Position',[312 195 36 20], ...
   'String','-->', ...
   'Tag','arrow2');
ud = addToUserData(ud,b);
b = uicontrol('Parent',a, ...
   'Units','pixels',...
   'HorizontalAlignment','left', ...
   'Position',[478 195 100 20], ...
   'String','Label1', ...
   'Style','text', ...
   'Tag','label2');
ud = addToUserData(ud,b);
b = uicontrol('Parent',a, ...
   'Units','pixels',...
   'BackgroundColor',[1 1 1], ...
   'HorizontalAlignment','left', ...
   'Position',[363 195 110 21], ...
   'Style','edit', ...
   'Callback','sbswitch(''sptimport'')', ...
   'Tag','edit2');
ud = addToUserData(ud,b);
b = uicontrol('Parent',a, ...
   'Units','pixels',...
   'BackgroundColor',[1 1 1], ...
   'HorizontalAlignment','left', ...
   'Position',[363 162 110 21], ...
   'Callback','sbswitch(''sptimport'')', ...
   'Style','edit', ...
   'Tag','edit3');
ud = addToUserData(ud,b);
b = uicontrol('Parent',a, ...
   'Units','pixels',...
   'HorizontalAlignment','left', ...
   'Position',[478 162 100 20], ...
   'String','Label1', ...
   'Style','text', ...
   'Tag','label3');
ud = addToUserData(ud,b);
b = uicontrol('Parent',a, ...
   'Units','pixels',...
   'Callback','sbswitch(''sptimport'')', ...
   'Position',[312 162 36 20], ...
   'String','-->', ...
   'Tag','arrow3');
ud = addToUserData(ud,b);
b = uicontrol('Parent',a, ...
   'Units','pixels',...
   'BackgroundColor',[1 1 1], ...
   'HorizontalAlignment','left', ...
   'Position',[363 128 110 21], ...
   'Callback','sbswitch(''sptimport'')', ...
   'Style','edit', ...
   'Tag','edit4');
ud = addToUserData(ud,b);
b = uicontrol('Parent',a, ...
   'Units','pixels',...
   'HorizontalAlignment','left', ...
   'Position',[478 128 100 20], ...
   'String','Label1', ...
   'Style','text', ...
   'Tag','label4');
ud = addToUserData(ud,b);
b = uicontrol('Parent',a, ...
   'Units','pixels',...
   'Callback','sbswitch(''sptimport'')', ...
   'Position',[312 128 36 20], ...
   'String','-->', ...
   'Tag','arrow4');
ud = addToUserData(ud,b);
b = uicontrol('Parent',a, ...
   'Units','pixels',...
   'BackgroundColor',[1 1 1], ...
   'Callback','sbswitch(''sptimport'')', ...
   'HorizontalAlignment','left', ...
   'Position',[363 63 210 21], ...
   'Style','edit', ...
   'Tag','editFs');
ud = addToUserData(ud,b);
b = uicontrol('Parent',a, ...
   'Units','pixels',...
   'HorizontalAlignment','left', ...
   'Position',[365 86 128 16], ...
   'String',getString(message('signal:sptoolgui:SamplingFrequency')), ...
   'Style','text', ...
   'Tag','labelFs');
ud = addToUserData(ud,b);
b = uicontrol('Parent',a, ...
   'Units','pixels',...
   'Callback','sbswitch(''sptimport'')', ...
   'Position',[312 63 36 20], ...
   'String','-->', ...
   'Tag','arrow5');
ud = addToUserData(ud,b);
b = uicontrol('Parent',a, ...
   'Units','pixels',...
   'BackgroundColor',[1 1 1], ...
   'HorizontalAlignment','left', ...
   'Position',[363 12 209 21], ...
   'Style','edit', ...
   'Tag','editLabel');
ud = addToUserData(ud,b);
b = uicontrol('Parent',a, ...
   'Units','pixels',...
   'HorizontalAlignment','left', ...
   'Position',[364 34 50 16], ...
   'String',getString(message('signal:sptoolgui:Name')), ...
   'Style','text', ...
   'Tag','labelLabel');
ud = addToUserData(ud,b);
set(a,'UserData',ud)


function udOutput = addToUserData(udInput,b)
%addToUserData - adds the handle b to input structure with the
%  field name given by b's tag
    udOutput = udInput;
    eval(['udOutput.h.' get(b,'Tag') ' = b;'])
    
function minwidth(h,n)
%MINWIDTH Minimize width of centered text object to be just wide
% enough for extent.
% optional second argument specifies additional pixels on either side
% of text, defaults to 2

if nargin == 1
    n = 2;
end
for i=1:length(h)
    ex = get(h(i),'Extent');
    pos = get(h(i),'Position');
    switch get(h(i),'HorizontalAlignment')
    case 'center'
       set(h(i),'Position',[pos(1)+pos(3)/2-ex(3)/2-n pos(2) ex(3)+2*n pos(4)])
    case 'left'
       set(h(i),'Position',[pos(1)+n pos(2) ex(3)+2*n pos(4)])
       set(h(i),'HorizontalAlignment','center')
    end
end

function s = importHelpStr
% return cell array of strings which describes the import dialog
%ADDCATALOG
s = {
'IMPORTING DATA TO SPTOOL'
' '
'SOURCE   In this frame, click on "From Workspace" to import'
'data from the MATLAB workspace.  Click on "From Disk" to import'
'data from a MAT-file saved on disk.  With "From Disk" selected,'
'you can type a MAT-file name and hit enter, or click Browse to'
'look for a file on your computer.'
' '
'CONTENTS   If "From Workspace" is selected, this is a list'
'of the variables in the MATLAB workspace.  If "From Disk"'
'is selected, this is a list of the variables saved in the'
'MAT-file you entered.  To import data that has been previously'
'exported from the SPTool, just click on it here.'
' '
'ARROWS   Use an arrow button "-->" to move the selected'
'variable into the variable field to the right of the arrow.'
'You can also type variable names directly into these fields.'
'When "From Workspace" is selected, you can type in expressions'
'as well to create data or filters on the fly.'
' '
'IMPORT AS...   Select from this menu the type of data that'
'you wish to import. The fields underneath will change according'
'to the information needed to import the object.  For example, a'
'"Signal" object requires a Data field which is the signal data'
'vector or matrix, and a Sampling Frequency which is a scalar.'
' '
'NAME   Enter a name for your imported data here.  The SPTool'
'uses this name to keep track of your data.  The name must be'
'a legal MATLAB variable name, that is, it must begin with a'
'letter and then consist only of letters, numerals (0 through '
'9), and underscores "_".'
};
   
   
   
   
