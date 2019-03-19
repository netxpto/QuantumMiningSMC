function mh = sptool_helpmenu(toolname,toolhelp_cb,toolhelp_tag,whatsthis_cb,whatsthis_tag)
% SPTOOL_HELPMENU Creates the string to be passed to makemenu to create the Help menu
%                 for SPTool clients, i.e., Signal Browser, Spectrum Viewer, 
%                 Filter Designer, and Filter Viewer.

%   Copyright 1988-2002 The MathWorks, Inc.
 

mh={getString(message('signal:sptoolgui:Help'))                             ' '                                'helpmenu'
    sprintf(['>&%s ' getString(message('signal:sptoolgui:Help'))],toolname)            toolhelp_cb                        toolhelp_tag
	sprintf(['>' getString(message('signal:sptoolgui:SignalProcessingToolboxHelp'))]) 'sptool(''help'',''helptoolbox'')' 'helptoolbox'
    '>------'                          ' '                                ' '
    ['>' getString(message('signal:sptoolgui:WhatsThis'))]                   whatsthis_cb                       whatsthis_tag
    '>------'                          ' '                                ' '
    sprintf(['>' getString(message('signal:sptoolgui:Examples'))])                           'sptool(''help'',''helpdemos'')'   'helpdemos'
    '>------'                          ' '                                ' '
    sprintf(['>' getString(message('signal:sptoolgui:AboutSignalProcessingToolbox'))]) 'sptool(''help'',''helpabout'')'   'helpabout'
	 
};

% [EOF] sptool_helpmenu.m
