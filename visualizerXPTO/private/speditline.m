function varargout = speditline(varargin)
%SPEDITLINE Edit line's color and style for Signal GUI.
%[lineColor,lineStyle,ok] = speditline(h,label,lineColorOrder,bgColor)
% Inputs:
%    h - handle to line
%    label - string; label for line
%    lineColorOrder - cell array of colors
%    bgColor - background color on which line will be drawn
% Outputs:
%    lineColor - color (either [rgb] triple or string e.g. 'r')
%    lineStyle - string; '-' '--' ':' '-.'
%    ok - ==1 if user hit OK, ==0 if user hit cancel

%   Copyright 1988-2010 The MathWorks, Inc.

if ~isstr(varargin{1})
    action = 'init';
else
    action = varargin{1};
end

switch action
case 'init'
    lineHandle = varargin{1};
    label = varargin{2};
    lineColorOrder = varargin{3};
    bgColor = varargin{4};

    lineStyleOrder = {[getString(message('signal:sptoolgui:solid')) ' (-)'] ... 
                      [getString(message('signal:sptoolgui:dashed')) ' (--)'] ... 
                      [getString(message('signal:sptoolgui:dotted')) ' (:)'] ... 
                      [getString(message('signal:sptoolgui:dashdot')) ' (-.)']};
    
    
    figname = getString(message('signal:sptoolgui:EditLine'));
    okstring = getString(message('signal:sptoolgui:OK'));
    cancelstring = getString(message('signal:sptoolgui:Cancel'));
    fus = 8;
    ffs = 8;
    uh = 20;
    uw = 150;
    lfs = 5; %label / frame spacing
    lh = 16; % label height

    maxColorChoices = 7;
    numChoices = min(maxColorChoices,length(lineColorOrder));
    lineColorOrder = lineColorOrder(1:numChoices);
    
    fp = get(0,'DefaultFigurePosition');
    w = 2*(fus+ffs)+uw;
    h = (numChoices+3)*(fus+uh)+lh+2*fus+uh+lh+fus+uh+lh+uh+fus;
    fp = [fp(1) fp(2)+fp(4)-h w h];  % keep upper left corner fixed

    fig_props = { ...
       'Name'                   figname ...
       'Resize'                 'off' ...
       'NumberTitle'            'off' ...
       'WindowStyle'            'modal' ...
       'CreateFcn'              ''    ...
       'Position'               fp   ...
       'CloseRequestFcn'        'sbswitch(''speditline'',''cancel'')' ...
       'Color'                  get(0,'DefaultUicontrolBackgroundColor') ...
       'Units'                  'pixels' ...
       'DockControls',          'off',...
       'Visible'                'off' ...
       };

    fig = figure(fig_props{:});
    
    btn_wid = (fp(3)-2*(fus)-fus)/2;
    ok_btn = uicontrol('Style','pushbutton',...
      'Units','pixels',...
      'String',okstring,...
      'Position',[fus fus btn_wid uh],...
      'Tag', 'EditLineOK', ...
      'Callback','sbswitch(''speditline'',''ok'')');
    cancel_btn = uicontrol('Style','pushbutton',...
      'Units','pixels',...
      'String',cancelstring,...
      'Position',[2*fus+btn_wid fus btn_wid uh],...
      'Tag', 'EditLineCancel', ...
      'Callback','sbswitch(''speditline'',''cancel'')');
    
    lfpos = [ffs fp(4)-ffs-lh/2-fus-uh fp(3)-2*ffs uh+fus+lh/2];
    labelFrame = uicontrol('Style','frame',...
         'Units','pixels',...
         'Tag', 'EditLineFrame', ...
         'Position',lfpos);
    labelLabel = framelab(labelFrame,getString(message('signal:sptoolgui:Name')),lfs,lh);
    labelText = uicontrol('Units','pixels',...
              'Style','text','String',label,...
          'HorizontalAlignment','left',...
          'Tag', 'EditLineText', ...
          'Position',[lfpos(1)+fus lfpos(2)+fus uw uh]);

    lspos = [ffs lfpos(2)-lh-uh-fus lfpos([3 4]) ];
    lineStyleFrame = uicontrol('Units','pixels',...
                               'Style','frame',...
              'Tag', 'EditLineStyleFrame', ...
              'Position',lspos);

    lineStyleLabel = framelab(lineStyleFrame,getString(message('signal:sptoolgui:LineStyle')),lfs,lh);
    
    switch get(lineHandle,'LineStyle')
    case '-'
        lineStylePopupVal = 1;
    case '--'
        lineStylePopupVal = 2;
    case ':'
        lineStylePopupVal = 3;
    case '-.'
        lineStylePopupVal = 4;
    otherwise
        % In case line handle has no line style -this should never be execute
        lineStylePopupVal = 1; 
    end

    lineStylePopup = uicontrol('Units','pixels',...
        'Style','popupmenu',...
        'BackgroundColor','white',...
	    'String',lineStyleOrder,...
 	    'Value',lineStylePopupVal,...
        'Tag', 'EditLineStylePopup', ...
  	    'Position',[lspos(1)+fus lspos(2)+fus uw uh]);

    lcpos = [ffs 2*fus+uh fp(3)-2*ffs lh/2+(numChoices+3)*(uh+fus)-fus];
    lineColorFrame = uicontrol('Units','pixels',...
              'Style','frame',...
              'Tag', 'EditLineColorFrame', ...
         'Position',lcpos);
    lineColorLabel = framelab(lineColorFrame,getString(message('signal:sptoolgui:LineColor')),lfs,lh);

    for i = 1:numChoices
        radio(i) = uicontrol('Units','pixels',...
              'Style','radiobutton',...
           'Position',[lcpos(1)+fus lcpos(2)+lcpos(4)-lh/2-i*(fus+uh)+fus uw uh],...
           'Value',0,...
           'BackgroundColor',bgColor,...
           'ForegroundColor',lineColorOrder{i},...
           'String',colorString(lineColorOrder{i}),...
           'Tag', ['EditLineRadioButton' num2str(i)], ...
           'Callback','sbswitch(''speditline'',''radio'')');
    end
    
    radio(numChoices+1) = uicontrol('Units','pixels',...
              'Style','radiobutton',...
           'Position',[lcpos(1)+fus ...
                lcpos(2)+lcpos(4)-lh/2-(numChoices+1)*(fus+uh)+fus uw uh],...
           'Value',0,...
           'BackgroundColor',bgColor,...
           'ForegroundColor',[1 0 0],...
           'String',getString(message('signal:sptoolgui:Other')),...
           'Tag', ['EditLineRadioButton' num2str(numChoices+1)], ...
           'Callback','sbswitch(''speditline'',''radio'')');
           
    indent = 15; % in pixels
    customLabel = uicontrol('Units','pixels',...
              'Style','text',...
           'Position',[lcpos(1)+fus+indent ...
                lcpos(2)+fus+uh uw-indent uh],...
           'HorizontalAlignment','left',...
           'Tag', 'EditLineColorspec', ...
           'String',getString(message('signal:sptoolgui:EnterColorspec')));
           
    edit = uicontrol('Units','pixels',...
              'Style','edit',...
           'Position',[lcpos(1)+fus+indent ...
                lcpos(2)+fus-1 uw-indent uh+2],...
           'Value',0,...
           'BackgroundColor','w',...
           'HorizontalAlignment','left',...
           'String','[1 0 0]',...
           'Tag', 'EditLineColor', ...
           'Callback','sbswitch(''speditline'',''edit'')');
           
    ind = whichColor(lineColorOrder,get(lineHandle,'Color'));
    if isempty(ind)
        set(radio(end),'Value',1,'ForegroundColor',get(lineHandle,'Color'))
        [str,c,str1] = colorString(get(lineHandle,'Color'));
        set(edit,'String',str1)
        set(edit,'Enable','on')
        set(customLabel,'Enable','on')
    else
        set(radio(ind),'Value',1)
        set(edit,'Enable','off')
        set(customLabel,'Enable','off')
    end
    
    ud.radio = radio;
    ud.edit = edit;
    ud.customLabel = customLabel;
    ud.flag = [];
    
    set(fig,'UserData',ud,'Visible','on')
    
    done = 0;
    while ~done
        waitfor(fig,'userdata')

        ud = get(fig,'UserData');
        switch ud.flag
   
        case 'ok'
            value = 1;
            val = get(ud.radio,'Value');
            ind = find([val{:}]);
        case 'cancel'
            value = 0;
            ind = 1;
        end
      
        if ind ~= numChoices+1
            err = 0;
            varargout{1} = lineColorOrder{ind};
        else            
            % custom color
            err = 0;
            eval(['c = ' get(ud.edit,'String') ';'],'err=1;')
            if ~err
                u = uicontrol('Visible','off');
                eval('set(u,''BackgroundColor'',c)','err=1;')
                delete(u)
            end
            if err
               errstr = {getString(message('signal:sptoolgui:CannotEvaluateColorspecTryEitherAThree'))
                         getString(message('signal:sptoolgui:HelpPlotForCompleteList'))};
               msgbox(errstr,getString(message('signal:sptoolgui:Error')),'error','modal')
            else
                varargout{1} = c;
            end
        end
    
        done = ~err;
        ud.flag = [];
        set(fig,'UserData',ud)
    end
    
    switch get(lineStylePopup,'Value')
    case 1
        varargout{2} = '-';
    case 2
        varargout{2} = '--';
    case 3
        varargout{2} = ':';
    case 4
        varargout{2} = '-.';
    end
    varargout{3} = value;
    
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
    
case 'radio'
    fig = gcf;
    ud = get(fig,'UserData');
    set(ud.radio,'Value',0)
    set(gcbo,'Value',1)
    if gcbo == ud.radio(end)
        set(ud.edit,'Enable','on')
        set(ud.customLabel,'Enable','on')
    else
        set(ud.edit,'Enable','off')
        set(ud.customLabel,'Enable','off')
    end
    
case 'edit'
    fig = gcf;
    ud = get(fig,'UserData');

    err = 0;
    eval(['c = ' get(ud.edit,'String') ';'],'err=1;')
    if ~err
        u = uicontrol('Visible','off');
        eval('set(u,''BackgroundColor'',c)','err=1;')
        delete(u)
    end
    if err 
        errstr = {'Cannot evaluate colorspec.  Try either a three'
                  'element vector (e.g., [1 0 1]), or a color'
                  'string (e.g., ''r''; see "help plot" for'
                  'a complete list).'};
        msgbox(errstr,getString(message('signal:sptoolgui:Error')),'error','modal')
    else
        set(ud.radio(end),'ForegroundColor',c)
    end

end

function [str,c,str1] = colorString(c)
% Returns a string indicating the color of the input
% colorspec... c may have the following forms:
%   'r','g', etc  
%   [1 0 1]  (rgb triple)
    if isstr(c)
    %  convert c to RGB triple
        c = rgbcolor(c);
    end
    
    if isequal(c,[0 0 0])
        c1 = c;
    else
        c1 = c/max(c);
    end
    str = [ '[' num2str(c(1)) ' ' num2str(c(2)) ' ' num2str(c(3)) ']' ];
    str1 = [ '[' num2str(c1(1)) ' ' num2str(c1(2)) ' ' num2str(c1(3)) ']' ];
    
    switch str1
    case '[1 0 0]'
        str = [getString(message('signal:sptoolgui:Red')) str];
    case '[0 1 0]'
        str = [getString(message('signal:sptoolgui:Green')) str];
    case '[0 0 1]'
        str = [getString(message('signal:sptoolgui:Blue')) str];
    case '[1 1 0]'
        str = [getString(message('signal:sptoolgui:Yellow')) str];
    case '[1 0 1]'
        str = [getString(message('signal:sptoolgui:Magenta')) str];
    case '[0 1 1]'
        str = [getString(message('signal:sptoolgui:Cyan')) str];
    case '[1 1 1]'
        if c(1)==1
            str = [getString(message('signal:sptoolgui:White')) str];
        else
            str = [getString(message('signal:sptoolgui:Gray')) str];
        end
    case '[0 0 0]'
        str = [getString(message('signal:sptoolgui:Black')) str];
    end    

function ind = whichColor(lineColorOrder,c)
    
    for ind=1:length(lineColorOrder)
        c1 = rgbcolor(lineColorOrder{ind});
        if isequal(c,c1)
            break
        end
    end

    if ~isequal(c,c1)
        ind = [];
    end
    
function c = rgbcolor(c)
%  converts string 'r','g' etc to rgb triple

    if ~isstr(c)
        return
    end
    
    switch c
    case 'y'
        c = [1 1 0];
    case 'm'
        c = [1 0 1];
    case 'c'
        c = [0 1 1];
    case 'r'
        c = [1 0 0];
    case 'g'
        c = [0 1 0];
    case 'w'
        c = [1 1 1];
    case 'k'
        c = [0 0 0];
    end

