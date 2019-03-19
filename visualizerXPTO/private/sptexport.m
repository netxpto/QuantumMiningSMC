function varargout = sptexport(varargin)
%SPTEXPORT  Export Dialog box for SPTool.

%   Copyright 1988-2012 The MathWorks, Inc.

if nargin == 0 | ~isstr(varargin{1})
    action = 'init';
else
    action = varargin{1};
end

switch action
%-----------------------------------------------------------------------------
% [prefs,componentSelect,fname,wd] = ...
%    sptexport(prefs,components,session,{selectionInd},wd,componentSelect,fname)
% Inputs:
%   prefs           - preferences as stored in SPTool
%   selectionInd    - is a cell array with one element for each column in the SPTool.
%                     Each element of the cell array contains an index
%                     vector which indicates which items in that column are currently selected.
%   componentSelect - optional, default to 1 
%   fname           - optional, default to 1; fname is a string or 0
%   wd              - working directory; directory string for exporting files
%
% Outputs:
%   prefs           - preferences structure containing new choices user has made.
%   componentSelect - integer;  1==>All components
%                               2-N ==>export component componentSelect-1
%                     this is the value of the popup menu for masking certain types
%                     of objects
%   fname           - string; name of the file to which the objects are exported
%   wd              - working directory; updated directory string for exporting files
%
case 'init'
    prefs = varargin{1};
    components = varargin{2};
    session = varargin{3};
    selectionInd = varargin{4};
    wd = varargin{5};
    if nargin > 5
        componentSelect = varargin{6};
    else
        componentSelect = 1;
    end
    if nargin > 6
        fname = varargin{7};
    else
        fname = 0;
    end
    figname = getString(message('signal:sptoolgui:ExportfromSPTool'));
    cancelstring = getString(message('signal:sptoolgui:Cancel'));
    fus = 5;     % frame / uicontrol spacing
    ffs = 8; 
    uh  = 20;    % uicontrol height
    bw  = 180;   % button width
    lfs = 5;     %label / frame spacing
    lbs = 3;     % label / box spacing
    lh  = 16;    % label height
    listh = 300; % listbox height
    pw  = 120;   % popup width   
   
    % Code added to increase the width of figure in case of languages other
    % than english. This particular string may be bigger than the width of
    % figure if the dimensions defined above are used.
    if is_ctrltbx_avail,
        a = largestuiwidth({getString(message('signal:sptoolgui:ExportfiltersasTFobjects'))});
        if (a > 180)
            bw = a + 25;
        end
    end    
            
    fp = get(0,'DefaultFigurePosition');
    w  = 3*ffs+2*fus+2*bw;
    h  = 2*fus+listh+uh+lh/2+ffs+ffs;
    fp = [fp(1) fp(2)+fp(4)-h w h];  % keep upper left corner fixed

    fig_props = { ...
       'Name'                   figname ...
       'Units'                  'pixels' ...
       'Resize'                 'off' ...
       'NumberTitle'            'off' ...
       'MenuBar'                'none' ...
       'WindowStyle'            'modal' ...
       'CreateFcn'              ''    ...
       'Position'               fp   ...
       'CloseRequestFcn'        'sbswitch(''sptexport'',''cancel'')' ...
       'DockControls',          'off',...
       'Color'                  get(0,'DefaultUicontrolBackgroundColor') ...
        };

    fig = figure(fig_props{:});
    
    exportFramePos = [ffs ffs 2*fus+bw 2*fus+lh/2+uh+listh];
    ud.exportFrame = uicontrol('Units','pixels', 'Tag', 'exportFrame', ...
              'Style','frame','Position',exportFramePos);
    ud.exportLabel = framelab(ud.exportFrame, getString(message('signal:sptoolgui:ExportList')),lfs,lh);

    % Tweak position of label & popup: [horz_pos ver_pos width height]
    if ispc
       slTweak   = [0 -2 0 0];
       spupTweak = [0  2 0 0];
    else % isunix
       slTweak   = [0 -3 0 0];
       spupTweak = [0  0 0 0];    
    end

    ud.showLabel = uicontrol('Style','text',...
                    'Units','pixels',...
                    'String',[getString(message('signal:sptoolgui:Show')) ':'],...
                    'HorizontalAlignment','right',...
                    'Tag', 'ExportShowLabel', ...
                    'Position',[exportFramePos(1)+fus ...
                        exportFramePos(2)+fus...
                        exportFramePos(3)-2*fus-pw-lbs uh]+slTweak );
    
    ud.showPopup = uicontrol('Style','popupmenu',...
                    'BackgroundColor','white',...
                    'Units','pixels',...
                    'String',getTranslatedStringcell('signal:sptoolgui',{'All' components.name}),...
                    'Tag', 'ExportShowPopup', ...
                    'Value',componentSelect,...
                    'Position',[exportFramePos(1)+exportFramePos(3)-fus-pw ...
                        exportFramePos(2)+fus pw uh]+spupTweak,...
                    'Callback','sbswitch(''sptexport'',''popup'')');
        
    [fontname,fontsize] = fixedfont;
    ud.exportList = uicontrol('Style','listbox','BackgroundColor','w',...
                        'Units','pixels',...
                        'Position',exportFramePos+[fus 2*fus+uh ...
                                 -2*fus  -(2*fus+uh+lh/2)],...
                        'FontName',fontname,...
                        'FontSize',fontsize,...
                        'Max',2,...
                        'Tag', 'exportList', ...
                        'Callback','sbswitch(''sptexport'',''list'')');
          
    button1Pos = [2*ffs+2*fus+bw fp(4)-ffs-uh bw uh];
    nui=1;
    
    % Set uicontrol visible iif the Control Toolbox is installed
    visFlag='off';
    allPanels = {prefs.panelName};
    exportInd = findcstr(allPanels,'export');
    if ~isempty(exportInd),
        tfObjFlag = prefs(exportInd).currentValue{1};
    else
        tfObjFlag = 0;
    end
    
    if is_ctrltbx_avail,
        visFlag = 'on';
        nui=nui+2;     % Leave some space
    end    
    ud.tfButton = uicontrol('Style','Checkbox',...
        'Units','pixels',...
        'Visible',visFlag,...
        'Value',tfObjFlag,...
        'String',getString(message('signal:sptoolgui:ExportfiltersasTFobjects')),...
        'Position',button1Pos-1*[0 uh+fus 0 0],...
        'Tag', 'ExportTFButton', ...
        'Callback','sbswitch(''sptexport'',''tfObjButton_cb'')');
       
    ud.diskButton = uicontrol('Style','pushbutton',...
      'String',getString(message('signal:sptoolgui:Exporttodisk')),...
      'Units','pixels',...
      'Position',button1Pos-nui*[0 uh+fus 0 0],...
      'Tag', 'ExportDiskButton', ...
      'Callback','sbswitch(''sptexport'',''disk'')');
    nui=nui+1;
          
    ud.workspaceButton = uicontrol('Style','pushbutton',...
      'Units','pixels',...
      'String',getString(message('signal:sptoolgui:Exporttoworkspace')),...
      'Position',button1Pos-nui*[0 uh+fus 0 0],...
      'Tag', 'ExportWorkspaceButton', ...
      'Callback','sbswitch(''sptexport'',''workspace'')');
    nui=nui+1;
  
    ud.spacer = uicontrol('Style','frame',...
      'Units','pixels',...
      'Tag', 'ExportSpacer', ...
      'Position',button1Pos-nui*[0 uh+fus 0 0]+[0 uh/2-1 0 -uh+2]);
    nui=nui+1;
  
    ud.cancelButton = uicontrol('Style','pushbutton',...
      'Units','pixels',...
      'String',cancelstring,...
      'Position',button1Pos-nui*[0 uh+fus 0 0],...
      'Tag', 'ExportCancelButton', ...
      'Callback','sbswitch(''sptexport'',''cancel'')');
    nui=nui+1;
  
    ud.helpButton = uicontrol('Style','pushbutton',...
      'Units','pixels',...
      'String',getString(message('signal:sptoolgui:Help')),...
      'Position',button1Pos-nui*[0 uh+fus 0 0],...
      'Tag', 'ExportHelpButton', ...
      'Callback','sbswitch(''sptexport'',''help'')');
    nui=nui+1;
  
    ud.flag = '';
    ud.session = session;
    ud.components = components;
    ud.selectionInd = selectionInd;
    for i = 1:length(ud.selectionInd)
        if isempty(ud.session{i})
            ud.selectionInd{i} = [];
        end
    end
    
    set(fig,'UserData',ud)
    
    sptexport('popup')  % sets listbox string and value
    
    done = 0;
    while ~done
        waitfor(fig,'userdata')

        ud = get(fig,'UserData');
        
        switch ud.flag
        case 'help'
        case 'disk'
            if isequal(fname,0) | isempty(fname)
                fname = 'export.mat';
            end
            matlab_wd = pwd;
            cd(wd)
            [fname,p]=uiputfile(fname,getString(message('signal:sptoolgui:ExportToDisk')));
            cd(matlab_wd)
            if ~isequal(fname,0)
                sessionIndex = get(ud.exportList,'UserData');
                ind = get(ud.exportList,'Value');
                i = sessionIndex(ind(1),1);
                j = sessionIndex(ind(1),2);
                fname = fullfile(p,fname);
                
                component = ud.session{i}(j);                
                % Check if Control Toolbox exists and if the "Export as TF Object" check box is selected.
                % Also make sure that it's a filter, then set the flag to create an LTI TF Object.
                if is_ctrltbx_avail & get(ud.tfButton,'Value')==1 & ...
                        strcmp(component.SPTIdentifier.type,'Filter');                
                    component = createLTIobj(ud.session{i}(j).label,component);                    
                end
                assigninMat(fname,ud.session{i}(j).label,component);
                
                for k = 2:length(ind)
                    i = sessionIndex(ind(k),1);
                    j = sessionIndex(ind(k),2);                    
                    
                    component = ud.session{i}(j);
                    % Check if Control Toolbox exists and if the "Export as TF Object" check box is selected.
                    % Also make sure that it's a filter, then set the flag to create an LTI TF Object.
                    if is_ctrltbx_avail & get(ud.tfButton,'Value')==1 & ...
                            strcmp(component.SPTIdentifier.type,'Filter'),
                        component = createLTIobj(ud.session{i}(j).label,component);
                    end
                    assigninMat(fname,ud.session{i}(j).label,component,'-append');
                end
                wd = p;
                done = 1;
            end
            
               
        case 'workspace'           
            w = evalin('base','whos');
            wname = {w.name};
            sessionIndex = get(ud.exportList,'UserData');
            ind = get(ud.exportList,'Value');
            
            overwrite = 0;
            for k=1:length(ind)
                i = sessionIndex(ind(k),1);
                j = sessionIndex(ind(k),2);
                if findcstr(wname,ud.session{i}(j).label)
                    overwrite = 1;
                    break
                end
            end
            if overwrite
                str2display = getString(message('signal:sptoolgui:AtleastOneOfTheItems'));
                switch questdlg(...
                        str2display,...
                        getString(message('signal:sptoolgui:VariableNameConflict')),getString(message('signal:sptoolgui:Yes')),getString(message('signal:sptoolgui:No')),getString(message('signal:sptoolgui:No')))
                    case getString(message('signal:sptoolgui:Yes'))
                   overwriteOK = 1;
                case getString(message('signal:sptoolgui:No'))
                   overwriteOK = 0;
                end
            else
                overwriteOK = 1;
            end
            
            if overwriteOK 
                for k = 1:length(ind)
                    i = sessionIndex(ind(k),1);
                    j = sessionIndex(ind(k),2);
                    
                    component = ud.session{i}(j);                    
                    % Check if Control Toolbox exists and if the "Export as TF Object" check box is selected.
                    % Also make sure that it's a filter, then set the flag to create an LTI TF Object.
                    if is_ctrltbx_avail & get(ud.tfButton,'Value')==1 & ...
                            strcmp(component.SPTIdentifier.type,'Filter'),
                        component = createLTIobj(ud.session{i}(j).label,component);
                    end
                    assignin('base',ud.session{i}(j).label,component);
                end
                done = 1;
            end
            
        case 'cancel'
            %   do nothing and return
            done = 1;
        end

        if ~done
            ud.flag = [];
            set(fig,'UserData',ud)
        end
    end
    
    varargout{1} = prefs;
    varargout{2} = get(ud.showPopup,'Value');
    varargout{3} = fname;
    varargout{4} = wd;
    
    delete(fig)
    
%------------------------------------------------------------------------
% sptexport('popup')
case 'popup'
    fig = gcf;
    ud = get(fig,'UserData');
    
    val = get(ud.showPopup,'Value');
    
    if val>1
        session = ud.session(val-1);
        components = ud.components(val-1);
        selectionInd = ud.selectionInd(val-1);
    else
        session = ud.session;
        components = ud.components;
        selectionInd = ud.selectionInd;
    end
    
    listStr = {};
    sessionIndex = [];
    for i=1:length(session)
        for j=1:length(session{i})
            str = [components(i).structName ': ' session{i}(j).label ' [' ...
                   session{i}(j).type ']'];
            listStr = {listStr{:} str};
            if val>1
                sessionIndex = [sessionIndex; val-1 j];
            else
                sessionIndex = [sessionIndex; i j];
            end
        end
    end

    if val == 1
        listVal = selectionInd{1}(:);
        cumulativeTotal = 0;
        for i=2:length(selectionInd)
            cumulativeTotal = cumulativeTotal + length(ud.session{i-1});
            listVal = [listVal(:); selectionInd{i}(:)+cumulativeTotal];
        end
    else
        listVal = selectionInd{1};
    end
    set(ud.exportList,'String',listStr,'Value',listVal,'UserData',sessionIndex);
    sptexport('list')
    
case 'list'
% callback of listbox
    fig = gcf;
    ud = get(fig,'UserData');
    val = get(ud.exportList,'Value');
    if isempty(val)
        set([ud.diskButton ud.workspaceButton],'Enable','off')
    else
        set([ud.diskButton ud.workspaceButton],'Enable','on')
    end
    
case 'help'
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
           'Tag','exporthelp');
    tp = [2*sz.fus 4*sz.fus+sz.uh fp(3)-4*sz.fus fp(4)-(6*sz.fus+sz.uh)];
       % text position
    [fontname,fontsize]=fixedfont;
    t = uicontrol('Style','listbox','Position',tp,'String',exportHelpStr,'Max',2,...
         'Tag','exporthelp','HorizontalAlignment','left',...
         'BackgroundColor','w','FontName',fontname,'FontSize',fontsize);
    bp = [fp(3)/2-sz.fus-sz.bw/2 2*sz.fus sz.bw sz.uh];  % button position
    b = uicontrol('Style','pushbutton','Position',bp,...
         'Tag','exporthelp','String','OK',...
         'Callback','delete(findobj(gcf,''tag'',''exporthelp''))');
    waitfor(b)
    if all(ishandle(uiList))
        if ispc
            set(uiList,{'Visible'},saveVis)
        end
    end
    
case 'tfObjButton_cb'
    % Callback code for the "Export as TF Object" checkbox
    fig = findobj(0,'Tag','sptool');
    ud = get(fig,'UserData');
    allPanels = {ud.prefs.panelName};
    exportInd = findcstr(allPanels,'export');
    if ~isempty(exportInd),    
        ud.prefs(exportInd).currentValue{1} = get(gcbo,'Value');
        set(fig,'UserData',ud);
    end
    
otherwise
    fig = gcf;
    ud = get(fig,'UserData');
    ud.flag = action;
    set(fig,'UserData',ud)

    delete(findobj(fig,'tag','exporthelp'))

end


function assigninMat(varargin)
% assigninMat - assign variable names in Mat file
% assigninMat(fname,varname,var) overwrites fname with the variable named
%   varname with value var
% assigninMat(fname,varname,var,'-append') appends var to MAT file fname

eval([varargin{2} '= varargin{3};'])
if nargin<=3
    save(varargin{1},varargin{2})
else
    save(varargin{1},varargin{2},'-append')
end
%ADDCATALOG 
function s = exportHelpStr
% return cell array of strings which describes the export dialog
%ADDCATALOG
s = {
'EXPORTING DATA FROM SPTOOL'
' '
'EXPORT LIST   This is a list of objects in the'
'SPTool. Click on an unselected object to select'
'that object alone.  To add or remove a single '
'object to or from the selection, Control click'
'(Command click on the Mac) on that object.  To'
'add or remove a range of objects to the selection,'
'Shift click.'
' '
'EXPORT BUTTONS (TO DISK and TO WORKSPACE)'
'By clicking one of these buttons you will export the'
'selected objects.  Each selected object will be'
'saved as a 1-by-1 structure variable.  You can '
'use and modify the fields of these structures, but'
'be careful since the SPTool and its clients might'
'not be able to recognize an object once you''ve'
'modified it.'
'   TO DISK - asks you for a file name and saves'
'    the variables in a MAT-file.'
'   TO WORKSPACE - saves the variables in the '
'    base MATLAB workspace.'
' '
'SHOW POPUP MENU  Select the type of objects you would'
'like to choose from in this menu.  Only objects of'
'the selected type will appear in the ''Export'
'List''. '
};

function ctrltbxAvail = is_ctrltbx_avail
% IS_CTRLTBX_AVAIL Determines if the Control Toolbox is available
%                  in SPTool's Export facility.
%
% Input:
%    none
% Output:
%    ctrltbxAvail - status indicating whether or not the Control Toolbox
%                   is available.
%     0 = don't render "Export as TF Object" uicontrol
%     1 = available, render the "Export as TF Object" uicontrol

% Determine if Control Toolbox is available by finding the TF method.
ctrltbxAvail = ~isempty(which('tf/tf'));
return

function SYS = createLTIobj(label,filt)
% CREATELTIOBJ Returns the filter transfer function coefficients and sampling frequency
%              as an LTI TF Object (to be used by the Controls toolbox)

SYS = tf(filt.tf.num,filt.tf.den,'variable','z^-1','Ts',1/filt.Fs);
%SYS = SS(A,B,C,D,Ts); 
%SYS = ZPK(Z,P,K,Ts);
return
