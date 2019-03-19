function varargout = applyfilt(varargin)
%APPLYFILT  Apply Filter utility for SPTool.

%   Copyright 1988-2010 The MathWorks, Inc.
switch varargin{1}

%------------------------------------------------------------------------
% enable = applyfilt('selection',verb.action,msg,SPTfig)
%  respond to selection change in SPTool
% possible actions are
%    'apply'
%  apply Button is enabled when there is exactly one signal selected
%  and one filter selected
case 'selection'
    SPTfig = varargin{4};
    f = sptool('Filters',0,SPTfig);
    s = sptool('Signals',0,SPTfig);
    if length(f)==1  &  length(s)==1
        enable = 'on';
    else
        enable = 'off';
    end
        
    varargout{1} = enable;
    
%------------------------------------------------------------------------
% applyfilt('action','apply')
%  respond to button push in SPTool
case 'action'
    SPTfig = findSPToolHandle;
    f = sptool('Filters',0,SPTfig);
    s = sptool('Signals',0,SPTfig);
    
    figname = 'Apply Filter';
    okstring = 'OK';
    cancelstring = 'Cancel';
    fus = 5;  % frame / uicontrol spacing
    ffs = 8;
    uh = 24;  % uicontrol height
    lw = 100; % label width
    bw = 100; % button width
    uw = 225; % uicontrol width
    lfs = 5; %label / frame spacing
    lbs = 3; % label / box spacing
    lh = 16; % label height
    
    fp = get(0,'DefaultFigurePosition');
    w = 2*ffs+lw+lbs+uw;
    h = 5*uh+3*fus+3*ffs;
    fp = [fp(1) fp(2)+fp(4)-h w h];  % keep upper left corner fixed

    fig_props = { ...
       'Name'                   getTranslatedString('signal:sptoolgui',figname) ...
       'Resize'                 'off' ...
       'NumberTitle'            'off' ...
       'WindowStyle'            'modal' ...
       'CreateFcn'              ''    ...
       'Position'               fp   ...
       'CloseRequestFcn'        'sbswitch(''applyfilt'',''cancel'')' ...
       'Color'                  get(0,'DefaultUicontrolBackgroundColor') ...
       'DockControls',          'off',...
       'Tag',                   'applyfiltdialog', ...    
       'Units'                  'pixels'
       };

    fig = figure(fig_props{:});
    
    cancel_btn = uicontrol('Style','pushbutton',...
      'Units','pixels',...
      'Tag','cancel_btn', ...
      'String',getTranslatedString('signal:sptoolgui',cancelstring),...
      'Position',[fp(3)/2+ffs/2 ffs bw uh],...
      'Callback','sbswitch(''applyfilt'',''cancel'')');
    ok_btn = uicontrol('Style','pushbutton',...
      'Units','pixels',...
      'Tag','ok_btn', ...
      'String',getTranslatedString('signal:sptoolgui',okstring),...
      'Position',[fp(3)/2-ffs/2-bw ffs bw uh],...
      'Callback','sbswitch(''applyfilt'',''ok'')');
    
    bottomLabelPos = [ffs-10 ffs+uh+ffs lw uh];
    bottomUIControlPos = [ffs+lw+lbs ffs+uh+ffs uw uh];
    
    ud.inputLabel = uicontrol('Style','text','String',getString(message('signal:sptoolgui:InputSignal')),...
          'Units','pixels',...
          'HorizontalAlignment','right',...
          'Position',bottomLabelPos+[0 3*(uh+fus) 0 0]);
    ud.filterLabel = uicontrol('Style','text','String',getString(message('signal:sptoolgui:Filter')),...
          'Units','pixels',...
          'HorizontalAlignment','right',...
          'Position',bottomLabelPos+[0 2*(uh+fus) 0 0]);
    ud.algorithmLabel = uicontrol('Style','text','String',getString(message('signal:sptoolgui:Algorithm')),...
          'Units','pixels',...
          'HorizontalAlignment','right',...          
          'Position',bottomLabelPos+[0 1*(uh+fus) 0 0]);
    ud.outputLabel = uicontrol('Style','text','String',getString(message('signal:sptoolgui:OutputSignal')),...
          'Units','pixels',...
          'HorizontalAlignment','right',...
          'Position',bottomLabelPos+[0 0*(uh+fus) 0 0]);
      
    if isfield(f, 'FDAspecs') && isfield(f.FDAspecs, 'current_filt')
%         FDATool filter - use the exact filter object to filter data as default, plus
%         a constructed filter selection
        if ~isfir(f.FDAspecs.current_filt)
            popupString = {f.FDAspecs.current_filt.FilterStructure  getString(message('signal:sptoolgui:ZeroPhaseIIRfiltfilt'))};
        else
            popupString = {f.FDAspecs.current_filt.FilterStructure  getString(message('signal:sptoolgui:FFTBasedFIRfftfilt'))};
        end
    else
%         Filter Designer filter - make a selection of constructed filters
        popupString = {getString(message('signal:sptoolgui:DirectFormIITransposedfilter')) ...
                       getString(message('signal:sptoolgui:ZeroPhaseIIRfiltfilt'))};
        isFIR = (length(f.tf.den)==1);
        if isFIR
            popupString = {popupString{:} getString(message('signal:sptoolgui:FFTBasedFIRfftfilt'))};
        end
    end
    
    ud.algorithmPopup = uicontrol('Style','popupmenu',...
          'Units','pixels',...
          'String',popupString,...
          'Value',1,...
          'Position',bottomUIControlPos+[0 1*(uh+fus) 0 0],...
          'BackgroundColor', 'w');

    ud.inputText = uicontrol('Style','edit','String',s.label,...
          'Units','pixels',...
          'Enable','inactive',...
          'HorizontalAlignment','left',...
          'Position',bottomUIControlPos+[0 3*(uh+fus) 0 0]);
          
    ud.filterText = uicontrol('Style','edit','String',f.label,...
          'Units','pixels',...
          'Enable','inactive',...
          'HorizontalAlignment','left',...
          'Position',bottomUIControlPos+[0 2*(uh+fus) 0 0]);

    labelList = sptool('labelList',SPTfig);
    [ps,fields,FsFlag,defaultLabel] = importsig('fields');
    defaultLabel = uniqlabel(labelList,defaultLabel);

    ud.outputEdit = uicontrol('Style','edit',...
           'Units','pixels',...
           'BackgroundColor','w',...
           'HorizontalAlignment','left',...
           'String',defaultLabel,...
           'Position',bottomUIControlPos+[0 0*(uh+fus) 0 0]);
    
    ud.flag = '';    
    set(fig,'UserData',ud)
    
    done = 0;
    while ~done
        waitfor(fig,'userdata')

        ud = get(fig,'UserData');
        err = 0;
        
        switch ud.flag
        case 'ok'
            label = get(ud.outputEdit,'String');
            mergeFlag = 0;
            if isempty(label)
                errstr = {getString(message('signal:sptoolgui:SorryYouMustEnterANameForTheOutputSignal'))};
                msgbox(errstr,getString(message('signal:sptoolgui:Error')),'error','modal')
                err = 1;
            elseif ~isvalidvar(label)
                errstr = {'Sorry, the output signal name you entered is not a valid name.  It must be a valid MATLAB variable name.'};
                msgbox(errstr,getString(message('signal:sptoolgui:Error')),'error','modal')
                err = 1;
            elseif ~isempty(findcstr(labelList,label))
                str2display = sprintf('%s',getString(message('signal:sptoolgui:ByNamingTheOutputSignalYouAreReplacingAnAlreadyExisting',...
                    label,label,label)));
                switch questdlg(...
                        str2display,...
                        getString(message('signal:sptoolgui:LabelConflict')),'Yes','No','No');
                    case 'Yes'
                        % OK, good
                   mergeFlag = 1;
                case 'No'
                   err = 1;
                end
            end
            if ~err && isempty(f.Fs) && ~isequal(s.Fs,f.Fs) 
                %Do not compare if Fs is empty which means it's imported
                %filter.
                promptStr = getString(message('signal:sptoolgui:TheSignalAndFilterYouHaveSelectedHaveDifferentSamplingFre'));
                switch questdlg(promptStr,getString(message('signal:sptoolgui:SamplingFrequencyConflict')),...
                          'Yes','No','Yes')
                case 'Yes'
                        % OK, good
                case 'No'
                    err = 1;
                end
            end
            if ~err % LET'S FILTER!!!!
                [err,errstr,newSig] = importsig('make',{1,s.data,s.Fs});
                newSig.label = label;
                switch get(ud.algorithmPopup,'Value')
                case 1  % filter
                    if isfield(f, 'FDAspecs') && isfield(f.FDAspecs, 'current_filt')
%                   in the case of fdatool filter, use filter object to filter the data
                        for i=1:size(s.data,2)  % loop over columns
                            evalStr = ['newSig.data(:,i) = '...
                                   'filter(f.FDAspecs.current_filt,s.data(:,i));'];
                            try, eval(evalStr);catch ME, err = 1;end  %#ok  
                        end
                    else
                        for i=1:size(s.data,2)  % loop over columns
                            evalStr = ['newSig.data(:,i) = '...
                                   'filter(f.tf.num,f.tf.den,s.data(:,i));'];
                            try, eval(evalStr);catch ME, err = 1;end  %#ok  
                        end
                    end
                case 2  
%                   Added to allow a constructed filter for FDATool filters
                    if isfield(f, 'FDAspecs') && isfield(f.FDAspecs, 'current_filt')
                        for i=1:size(s.data,2)  % filtfilt
                            if ~isfir(f.FDAspecs.current_filt)
                                evalStr = ['newSig.data(:,i) = '...
                                    'filtfilt(f.tf.num,f.tf.den,s.data(:,i));'];
                            else
                                evalStr = ['newSig.data(:,i) = '...
                                    'fftfilt(f.tf.num,s.data(:,i))/f.tf.den;'];
                            end
                            try, eval(evalStr);catch ME, err = 1;end  %#ok  
                        end
                    else % filtfilt
                        for i=1:size(s.data,2)  % loop over columns
                            evalStr = ['newSig.data(:,i) = '...
                                    'filtfilt(f.tf.num,f.tf.den,s.data(:,i));'];
                            try, eval(evalStr);catch ME, err = 1;end  %#ok  
                        end
                    end
                case 3  % fftfilt
                    for i=1:size(s.data,2)  % loop over columns
                        evalStr = ['newSig.data(:,i) = '...
                                 'fftfilt(f.tf.num,s.data(:,i))/f.tf.den;'];
                        try, eval(evalStr);catch ME, err = 1;end  %#ok  
                    end
                end
                if err
                    errstr = {getString(message('signal:sptoolgui:SorryAnErrorOccurredDuringFiltering'))
                              getString(message('signal:sptoolgui:ErrorMessage'))
                              ME.message};
                    msgbox(errstr,getString(message('signal:sptoolgui:Error')),'error','modal')
                else 
                    if mergeFlag
                        sigs = sptool('Signals');
                        sigLabels = {sigs.label};
                        ind = findcstr(sigLabels,newSig.label);
                        newSig = importsig('merge',sigs(ind),newSig);
                    end
                    sptool('import',newSig,1,SPTfig,1)
                end
            end
                        
        case 'cancel'
           % do nothing and return
           
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
    
end

% [EOF] applyfilt.m

% LocalWords:  Tfig Aspecs IIR popupmenu userdata Fs
