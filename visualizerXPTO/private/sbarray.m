function varargout = sbarray(varargin)
%SBARRAY  Array Signals Display Manager for Signal Browser.
%  [ind,columns] = sbarray(sigs)
%  Inputs:
%     sigs - structure array of array signals
%  Outputs:
%     ind - integer index of the signal in sigs whose column index was changed
%           [] if cancel was hit
%     columns - (non-empty) index vector for sigs(ind)

%   Copyright 1988-2010 The MathWorks, Inc.

if nargin == 0 || ~ischar(varargin{1})
    action = 'init';
else
    action = varargin{1};
end

switch action
%------------------------------------------------------------------------
%  [ind,columns] = sbarray(sigs)
%  Inputs:
%     sigs - structure array of array signals
%  Outputs:
%     ind - integer index of the signal in sigs whose column index was changed
%           [] if cancel was hit
%     columns - (non-empty) index vector for sigs(ind)
case 'init'
    sigs = varargin{1};
    
    figname = getString(message('signal:sptoolgui:ColumnSelectionForArraySignals'));
    okstring = 'OK';
    cancelstring = 'Cancel';
    fus = 5;  % frame / uicontrol spacing
    ffs = 8;
    uh = 24;  % uicontrol height
    lw = 300; % list width
    bw = 100; % button width
    lfs = 5; %label / frame spacing
    lbs = 3; % label / box spacing
    lh = 16; % label height
    
    fp = get(0,'DefaultFigurePosition');
    w = 2*ffs+2*fus+lw;
    h = 6*fus+2*lh+5*uh;
    fp = [fp(1) fp(2)+fp(4)-h w h];  % keep upper left corner fixed

    fig_props = { ...
       'Name'                   figname ...
       'Resize'                 'off' ...
       'NumberTitle'            'off' ...
       'WindowStyle'            'modal' ...
       'Units'                  'pixels' ...
       'CreateFcn'              ''    ...
       'Position'               fp   ...
       'CloseRequestFcn'        'sbswitch(''sbarray'',''cancel'')'  ...
       'DockControls',          'off',...
       'Color'                  get(0,'DefaultUicontrolBackgroundColor') ...
       };

    fig = figure(fig_props{:});
    
    ok_btn = uicontrol('Style','pushbutton',...
      'Units','pixels',...
      'String',getTranslatedString('signal:sptoolgui',okstring),...
      'Tag', 'ArraySigOk', ...
      'Position',[fp(3)/2-ffs/2-bw fus bw uh],...
      'Callback','sbswitch(''sbarray'',''ok'')');
    cancel_btn = uicontrol('Style','pushbutton',...
      'Units','pixels',...
      'String',getTranslatedString('signal:sptoolgui',cancelstring),...
      'Tag', 'ArraySigCancel', ...
      'Position',[fp(3)/2+ffs/2 fus bw uh],...
      'Callback','sbswitch(''sbarray'',''cancel'')');
    
    listFramePos = [ffs fp(4)-fus-lh-3*uh-fus lw+2*fus fus+3*uh+lh/2];
    indexFramePos = [ffs 2*fus+uh lw+2*fus fus+uh+lh/2];
    
    ud.listFrame = uicontrol('Units','pixels','Style','frame','Position', ...
        listFramePos,'Tag','arrayListFrame');
    ud.indexFrame = uicontrol('Units','pixels','Style','frame','Position', ...
        indexFramePos, 'Tag', 'arrayIndexFrame');
    
    ud.listFrameLabel = framelab(ud.listFrame,'Array Signals',lfs,lh);
    ud.indexFrameLabel = framelab(ud.indexFrame,...
                 'Column Index Vector for 1234567890',lfs,lh);
    
    indexLabel(ud.indexFrameLabel,sigs(1).label)
    
    maxLabelLength = 0;
    for i=1:length(sigs)
        maxLabelLength = max(maxLabelLength,length(sigs(i).label));
    end
    formatStr = sprintf('%%%gs   %%s',maxLabelLength);
    ud.listStr = {};
    for i=1:length(sigs)
           [m,n]=size(sigs(i).data);
           if isreal(sigs(i).data)
               complexStr = 'real';
           else
               complexStr = 'complex';
           end
           N = size(sigs(i).data,2);
          % siginfo1Str = sprintf('%g-by-%g %s',m,n,complexStr);
           siginfo1Str = sprintf('%g columns',N);
           newline = sprintf(formatStr,sigs(i).label,siginfo1Str);
           ud.listStr = {ud.listStr{:} newline};
    end
    [fontname,fontsize] = fixedfont;
    ud.list = uicontrol('Units','pixels',...
                        'Style','listbox','BackgroundColor','w',...
                        'Position',listFramePos+[fus fus -2*fus  -fus-lh/2],...
                        'FontName',fontname,...
                        'FontSize',fontsize,...
                        'String',ud.listStr,...
                        'Value',1,...
                        'Tag', 'arrayList', ...
                        'Callback','sbswitch(''sbarray'',''list'')');
    
    ud.indexEdit = uicontrol('Style','edit',...
           'Units','pixels',...
           'BackgroundColor','w',...
           'HorizontalAlignment','left',...
           'String',index2str(sigs(1).lineinfo.columns),...
           'Tag', 'arrayIndexEdit', ...
           'Position',indexFramePos+[fus fus -2*fus  -fus-lh/2]);
    
    ud.sigs = sigs;
    ud.flag = '';    
    set(fig,'UserData',ud)
    
    done = 0;
    while ~done
        waitfor(fig,'userdata')

        ud = get(fig,'UserData');
        err = 0;
        
        switch ud.flag
        case 'ok'
           val = get(ud.list,'Value');
           colStr = get(ud.indexEdit,'String');
           arb_obj = {'arbitrary' 'object'};
           columns = evalin('base',colStr,'arb_obj');
           if isequal(columns,arb_obj)
               errstr = {getString(message('signal:sptoolgui:SorryTheColumnIndexVectorYouEntered'))};
               msgbox(errstr,getString(message('signal:sptoolgui:Error')),'error','modal')
               err = 1;
           end
           N = size(ud.sigs(val).data,2);
           if ~err && (~all(round(abs(columns))==columns) || any(columns<=0))
               errstr = getString(message('signal:sptoolgui:TheColumnIndexVectorMustContainOnlyPositiveIntegers'));
               msgbox(errstr,getString(message('signal:sptoolgui:Error')),'error','modal')
               err = 1;
           end
           if ~err && (max(columns) > N)
               errstr = sprintf('%s',getString(message('signal:sptoolgui:TheSelectedSignalArrayHasOnly0numberintegerColumns', N)));
               msgbox(errstr,getString(message('signal:sptoolgui:Error')),'error','modal')
               err = 1;
           end
           if ~err
               varargout{1} = val;
               varargout{2} = columns;
           end
           
        case 'cancel'
           % do nothing and return
           varargout{1} = [];
           varargout{2} = [];
           
        end
    
        done = ~err;
        ud.flag = [];
        set(fig,'UserData',ud)
    end
    
    delete(fig)
    
case 'ok'
    fig = gcf;
    ud = get(fig,'UserData');
    ud.flag = 'ok';
    set(fig,'UserData',ud)
    
case 'cancel'
    fig = gcf;
    ud = get(fig,'UserData');
    ud.flag = 'cancel';
    set(fig,'UserData',ud)
    
case 'list'
    fig = gcf;
    ud = get(fig,'UserData');

    val = get(ud.list,'Value');
    columns = ud.sigs(val).lineinfo.columns;
    
    indexStr = index2str(columns);
    
    set(ud.indexEdit,'String',indexStr)
    
    indexLabel(ud.indexFrameLabel,ud.sigs(val).label)
end

function indexStr = index2str(columns)

    cDiff = diff(columns);
    if max(cDiff)>1
        indexStr = ['[' num2str(columns) ']'];
    else
        if length(columns)==1
            indexStr = num2str(columns);
        else
            indexStr = [num2str(columns(1)) ':' num2str(columns(end))];
        end
    end



function indexLabel(h,label)
    set(h,'String',sprintf('%s',getString(message('signal:sptoolgui:ColumnIndexVectorFor',label))))
    
    ex = get(h,'Extent');
    ex(3) = ex(3) + 6;
    pos = get(h,'Position');
    set(h,'Position',[pos(1) pos(2) ex(3) pos(4)])
