function p = sptprefp
% SPTPREFP  SPTool Preference Registry - Private directory.
% To register alternative preferences for the sptool, create
% a function named 'sptpref' which will be on the path, which
% takes and returns a preference structure array (where the
% structure fields are defined as in this function).

%   Copyright 1988-2012 The MathWorks, Inc.

%   Rulers  ---------------------------------------------------------------
rulerPrefs.panelName = 'ruler';
rulerPrefs.panelDescription = getString(message('signal:sptprefhelp:Markers'));
rulerPrefs.clientList = {'sigbrowse' 'spectview'};
rulerPrefs.panelHelp = {
    getString(message('signal:sptprefhelp:ThesePreferencesApplyToAllClientsThatUseMarkersNamelyTh'))
    };

% controls cell array.  Columns are:
%  Name  Description  type  radiogroup  number_of_lines popup_string ...
%      factory_value  help_string
c = get(0,'DefaultAxesColor');
if isstr(c)  % might be 'none'
    c = get(0,'DefaultFigureColor');
end
if c*[1 1 1]' > 1.5  %  light background
    factoryRulerColor = '[1 0 1]';
else  % dark background
    factoryRulerColor = '[1 0 0]';
end
rulerPrefs.controls = {
    'rulerColor' 'Marker Color' 'edit' '' 1 ...
    {} ...
    factoryRulerColor ...
    {getString(message('signal:sptprefhelp:ThisIsTheColorUsedForTheMarkers'))
    getString(message('signal:sptprefhelp:TheColorCanBeAnRgbTripleEg101OrAnyOfTheFollowingColorSp'))
    ['       ''y''     ' getString(message('signal:sptprefhelp:yellow'))]
    ['       ''m''     ' getString(message('signal:sptprefhelp:magenta'))]
    ['       ''c''     ' getString(message('signal:sptprefhelp:cyan'))]
    ['       ''r''     ' getString(message('signal:sptprefhelp:red'))]
    ['       ''g''     ' getString(message('signal:sptprefhelp:green'))]
    ['       ''b''     ' getString(message('signal:sptprefhelp:blue'))]
    ['       ''w''     ' getString(message('signal:sptprefhelp:white'))]
    ['       ''k''     ' getString(message('signal:sptprefhelp:black'))]
    }
    'rulerMarker' 'Marker Style' 'popupmenu' '' 1 ...
    { '+' 'o' '*' '.' 'x' ...
    'square' 'diamond' 'v' '^' '>' '<' 'pentagram' 'hexagram'}' ...
    2 ...
    {getString(message('signal:sptprefhelp:ThisIsTheMarkerstyleUsedForTheMarkersInTrackAndSlopeMod'))
    ['           +     ' getString(message('signal:sptprefhelp:plus'))]
    ['           o     ' getString(message('signal:sptprefhelp:circle'))]
    ['           *     ' getString(message('signal:sptprefhelp:star'))]
    ['           .     ' getString(message('signal:sptprefhelp:point'))]
    ['           x     ' getString(message('signal:sptprefhelp:xmark'))]
    ['                 ' getString(message('signal:sptprefhelp:square'))]
    ['                 ' getString(message('signal:sptprefhelp:diamond'))]
    ['           v     ' getString(message('signal:sptprefhelp:triangledown'))]
    ['           ^     ' getString(message('signal:sptprefhelp:triangleup'))]
    ['           >     ' getString(message('signal:sptprefhelp:triangleright'))]
    ['           <     ' getString(message('signal:sptprefhelp:triangleleft'))]
    ['                 ' getString(message('signal:sptprefhelp:pentagram'))]
    ['                 ' getString(message('signal:sptprefhelp:hexagram'))]
    }
    'markerSize' 'Marker Size' 'edit' '' 1 ...
    {} ...
    '8' ...
    {getString(message('signal:sptprefhelp:ThisIsTheMarkersizeUsedForTheMarkerShapeInTrackAndSlope'))
    }
    'initialType' 'Initial Type' 'popupmenu' '' 1 ...
    {'vertical' 'horizontal' 'track' 'slope'}' ...
    3 ...
    {getString(message('signal:sptprefhelp:WhenAToolSuchAsTheSignalBrowserOrSpectrumViewerIsOpened'))}
    };

rulerPrefs.currentValue = rulerPrefs.controls(:,7);


%   Colors ---------------------------------------------------------------
colorPrefs.panelName = 'color';
colorPrefs.panelDescription = getString(message('signal:sptprefhelp:Colors'));
colorPrefs.clientList = {'sigbrowse' 'spectview'};
colorPrefs.panelHelp = {
    getString(message('signal:sptprefhelp:ThesePreferencesApplyToAllWindowsThatDisplayMultipleDat'))
    ' '
    getString(message('signal:sptprefhelp:ColorsAndLinestylesAreAssignedToNewDataAccordingToThese'))
    getString(message('signal:sptprefhelp:IfYouCloseAToolAndReopenItItWillStartAtTheTopOfTheColor'))
    };
% controls cell array.  Columns are:
%  Name  Description  type  radiogroup  number_of_lines popup_string ...
%      factory_value  help_string
colorPrefs.controls = {
    'colorOrder' 'Color Order' 'edit' '' 3 ...
    {} ...
    'get(0,''defaultaxescolororder'')' ...
    {getString(message('signal:sptprefhelp:EnterInAnExpressionHereWhichDefinesTheLineColorOrder'))
    getString(message('signal:sptprefhelp:TheStringYouEnterShouldEvaluateToEither'))
    ['   - ' getString(message('signal:sptprefhelp:AColorDesignationSuchAs')) ' ''r'',']
    ['   - ' getString(message('signal:sptprefhelp:ANby3MatrixOfNColorsSuchAs')) ' [1 0 1] or jet(16), or']
    ['   - ' getString(message('signal:sptprefhelp:ACellArrayColumnOfSuchObjectsSuchAs'))]
    '        {get(0,''defaultaxescolororder''); ''k''; [0 1 0]; jet(16)}'
    }
    'linestyleOrder' 'Line Style Order' 'edit' '' 3 ...
    {} ...
    'get(0,''defaultaxeslinestyleorder'')' ...
    {
    getString(message('signal:sptprefhelp:TheStringYouEnterShouldEvaluateToEither'))
    ['   - ' getString(message('signal:sptprefhelp:ALineStyleDesignationStringSuchAs')) ' '':'', ''-'', ''--'', or ''-.'',']
    ['   - ' getString(message('signal:sptprefhelp:AStringMatrixOfTheAboveOr'))]
    ['   - ' getString(message('signal:sptprefhelp:ACellArrayColumnOfSuchObjectsSuchAs'))]
    '           {get(0,''defaultaxeslinestyleorder''); ''--''}'
    }
    };

colorPrefs.currentValue = colorPrefs.controls(:,7);


%   Signal Browser --------------------------------------------------------
sigbrowsePrefs.panelName = 'sigbrowse';
sigbrowsePrefs.panelDescription = getString(message('signal:sptprefhelp:SignalBrowser'));
sigbrowsePrefs.clientList = {'sigbrowse','sigbrowser.sigbrowseAdapter'};
sigbrowsePrefs.panelHelp = {
    getString(message('signal:sptprefhelp:TheseAreThePreferencesForTheSignalBrowser'))
    };
% controls cell array.  Columns are:
%  Name  Description  type  radiogroup  number_of_lines popup_string ...
%      factory_value  help_string
sigbrowsePrefs.controls = {
    'legacyBrowserEnable' 'Use Legacy Browser (to be removed)' 'checkbox' 'UseLegacyTag' 1 {} 0 {'This uses the legacy signal browser'}
    'xlabel' 'X Label' 'edit' '' 1 ...
    {} ...
    getString(message('signal:sptoolgui:Time')) ...
    {getString(message('signal:sptprefhelp:ThisIsTheStringUsedToLabelTheXaxesusuallyTimeOfTheSignalBrowser'))}
    'ylabel' 'Y Label' 'edit' '' 1 ...
    {} ...
    '' ...
    {getString(message('signal:sptprefhelp:ThisIsTheStringUsedToLabelTheYaxesOfTheSignalBrowser'))}
    'rulerEnable' 'Markers' 'checkbox' '' 1 ...
    {} ...
    1 ...
    {getString(message('signal:sptprefhelp:CheckThisBoxToUseTheMarkersInTheSignalBrowser'))
    ' '
    getString(message('signal:sptprefhelp:IfThisBoxIsNotCheckedTheMarkersWillBeAbsent'))}
    'pannerEnable' 'Panner' 'checkbox' '' 1 ...
    {} ...
    1 ...
    {getString(message('signal:sptprefhelp:CheckThisBoxToUseThePannerInTheSignalBrowser'))
    ' '
    getString(message('signal:sptprefhelp:IfThisBoxIsNotCheckedThePannerWillBeAbsent'))}
    'zoomFlag' getString(message('signal:sptprefhelp:StayInZoomModeAfterZoom')) 'checkbox' '' 1 ...
    {} ...
    0 ...
    {getString(message('signal:sptprefhelp:ThisCheckboxDeterminesWhetherYouStayInZoomModeAfterTheF'))
    ' '
    getString(message('signal:sptprefhelp:IfTheCheckboxIsUncheckedWhenYouClickOnTheMouseZoomButto'))
    ' '
    getString(message('signal:sptprefhelp:IfTheCheckboxIsCheckedYouCanZoomRepeatedlyWithoutHaving'))}
    };

sigbrowsePrefs.currentValue = sigbrowsePrefs.controls(:,7);


%   Spectrum Viewer --------------------------------------------------------
spectviewPrefs.panelName = 'spectview';
spectviewPrefs.panelDescription = getString(message('signal:sptprefhelp:SpectrumViewer'));
spectviewPrefs.clientList = {'spectview'};
spectviewPrefs.panelHelp = {
    getString(message('signal:sptprefhelp:TheseAreThePreferencesForTheSpectrumViewer'))
    };
% controls cell array.  Columns are:
%  Name  Description  type  radiogroup  number_of_lines popup_string ...
%      factory_value  help_string
spectviewPrefs.controls = {
    'magscale' 'Magnitude Axis Scaling' 'popupmenu' '' 1 ...
    {'decibels' 'linear'} ...
    1 ...
    {getString(message('signal:sptprefhelp:ScalingOfMagnitudeverticalAxisInTheSpectrumViewer'))
    ' '
    getString(message('signal:sptprefhelp:IndecibelsMode10log10OfTheSpectraIsPlotted'))
    ' '}
    'freqscale' 'Frequency Axis Scaling' 'popupmenu' '' 1 ...
    {'linear' 'log'} ...
    1 ...
    {getString(message('signal:sptprefhelp:ScalingOfFrequencyhorizontalAxisInTheSpectrumViewer'))
    ' '
    getString(message('signal:sptprefhelp:InLogModeTheFrequencyAxisRangeCannotBeNegative'))}
    'freqrange' getString(message('signal:sptprefhelp:FrequencyAxisRange')) 'popupmenu' '' 1 ...
    {'[0,Fs/2]' '[0,Fs]' '[-Fs/2,Fs/2]'} ...
    1 ...
    {getString(message('signal:sptprefhelp:RangeOfFrequencyhorizontalAxisInTheSpectrumViewer'))
    ' '
    getString(message('signal:sptprefhelp:FsIsTheSamplingFrequency'))
    ' '
    getString(message('signal:sptprefhelp:TheRangeCannotBeNegativeIfFrequencyAxisScalingIslog')) }
    'rulerEnable' getString(message('signal:sptprefhelp:Markers')) 'checkbox' '' 1 ...
    {} ...
    1 ...
    {getString(message('signal:sptprefhelp:CheckThisBoxToUseTheMarkersInTheSpectrumViewer'))
    ' '
    getString(message('signal:sptprefhelp:IfThisBoxIsNotCheckedTheMarkersWillBeAbsent'))}
    'zoomFlag' getString(message('signal:sptprefhelp:StayInZoomModeAfterZoom')) 'checkbox' '' 1 ...
    {} ...
    0 ...
    {getString(message('signal:sptprefhelp:ThisCheckboxDeterminesWhetherYouStayInZoomModeAfterTheF'))
    ' '
    getString(message('signal:sptprefhelp:IfTheCheckboxIsUncheckedWhenYouClickOnTheMouseZoomButto'))
    ' '
    getString(message('signal:sptprefhelp:IfTheCheckboxIsCheckedYouCanZoomRepeatedlyWithoutHaving'))}
    };

spectviewPrefs.currentValue = spectviewPrefs.controls(:,7);

%   Filter Designer --------------------------------------------------------
filtdesPrefs.panelName = 'filtdes';
filtdesPrefs.panelDescription = getString(message('signal:sptprefhelp:FilterDesigner'));
filtdesPrefs.clientList = {'filtdes'};
filtdesPrefs.panelHelp = {
    getString(message('signal:sptprefhelp:TheseAreThePreferencesForTheFilterDesigner'))
    };

% controls cell array.  Columns are:
%  Name  Description  type  radiogroup  number_of_lines popup_string ...
%      factory_value  help_string
filtdesPrefs.controls = {
    'nfft' 'FFT Length' 'edit' '' 1 ...
    {} ...
    '4096' ...
    {getString(message('signal:sptprefhelp:LengthOfFFTUsedToComputeFrequencyResponsesForEfficiency'))}
    'fdatoolFlag' getString(message('signal:sptprefhelp:UseFDATool')) 'radiobutton' 'fdatoolTag' 1 ...
    {} ...
    1 ...
    {getString(message('signal:sptprefhelp:ThisRadioButoonLaunchesFDATool'))
    ' '}
    'filterDesignerFlag' 'Use Filter Designer (to be removed)' 'radiobutton' 'fdatoolTag' 1 ...
    {} ...
    0 ...
    {getString(message('signal:sptprefhelp:ThisRadioButtonLaunchesFilterDesigner'))
    ' '}
    'grid' 'Display grid lines' 'checkbox' '' 1 ...
    {} ...
    1 ...
    {getString(message('signal:sptprefhelp:CheckThisBoxToDisplayHorizontalAndVerticalGridLinesOnTh'))}
    'AutoDesignInit' 'Auto Design' 'checkbox' '' 1 ...
    {} ...
    0 ...
    {getString(message('signal:sptprefhelp:ThisSpecifiesTheValueForAutoDesignWheneverTheFilterDesi'))
    ' '
    getString(message('signal:sptprefhelp:WhenAutoDesignIsCheckedEveryTimeYouChangeASpecification'))
    ' '
    getString(message('signal:sptprefhelp:WhenAutoDesignIsNotCheckedTheFilterIsDesignedOnlyWhenYo'))}
    'zoomFlag' getString(message('signal:sptprefhelp:StayInZoomModeAfterZoom')) 'checkbox' '' 1 ...
    {} ...
    0 ...
    {getString(message('signal:sptprefhelp:ThisCheckboxDeterminesWhetherYouStayInZoomModeAfterTheF'))
    ' '
    getString(message('signal:sptprefhelp:IfTheCheckboxIsUncheckedWhenYouClickOnTheMouseZoomButto'))
    ' '
    getString(message('signal:sptprefhelp:IfTheCheckboxIsCheckedYouCanZoomRepeatedlyWithoutHaving'))}
    };
%   'gridflag' 'Snap to Grid when dragging bands.' 'checkbox' '' 1 ...
%       {} ...
%       1 ...
%       {'Check this box to snap to a grid when dragging bands or band edges.'}
%   'fgrid' 'Frequency Grid Spacing' 'edit' '' 1 ...
%       {} ...
%       '.001' ...
%       {'When dragging band edges, edges are constrained to multiples of this ' ...
%          'number.'}
%   'mgrid' 'Magnitude Grid Spacing' 'edit' '' 1 ...
%       {} ...
%       '.001' ...
%       {'When dragging bands up and down, values are constrained to multiples '...
%         ' of this number.'}

filtdesPrefs.currentValue = filtdesPrefs.controls(:,7);

%   Default Session File ---------------------------------------------------
defsesPrefs.panelName = 'defsession';
defsesPrefs.panelDescription = getString(message('signal:sptprefhelp:DefaultSession'));
defsesPrefs.clientList = {};
defsesPrefs.panelHelp = {
    getString(message('signal:sptprefhelp:PreferencesForLoadingDefaultSession'))
    ' '
    };

% controls cell array.  Columns are:
%  Name  Description  type  radiogroup  number_of_lines popup_string ...
%      factory_value  help_string
defsesPrefs.controls = {
    'defaultSession' 'Load default session file (startup.spt) when starting SPTool.' 'checkbox' '' 1 ...
    {} ...
    1 ...
    {getString(message('signal:sptprefhelp:CheckingThisBoxAutomaticallyLoadsADefaultSPToolSessionW'))
    ' '
    getString(message('signal:sptprefhelp:WhenTheBoxIsUncheckedSPToolWillNotLoadASessionUponStart'))}
    };

defsesPrefs.currentValue = defsesPrefs.controls(:,7);

%   Export Dialog Window ---------------------------------------------------
exportPrefs.panelName = 'export';
exportPrefs.panelDescription = getString(message('signal:sptprefhelp:ExportingComponents'));
exportPrefs.clientList = {};
exportPrefs.panelHelp = {
    'Preferences for exporting Components'
    ' '
    };

% controls cell array.  Columns are:
%  Name  Description  type  radiogroup  number_of_lines popup_string ...
%      factory_value  help_string
exportPrefs.controls = {
    'export' 'Export Filters as TF objects (to be used in Control Toolbox)' 'checkbox' '' 1 ...
    {} ...
    0 ...
    {getString(message('signal:sptprefhelp:CheckingThisBoxWillIndicateThatFiltersShouldBeExportedT'))
    ' '
    getString(message('signal:sptprefhelp:WhenTheBoxIsUncheckedSPToolWillExportFiltersAsStructure'))}
    };

exportPrefs.currentValue = exportPrefs.controls(:,7);

%   Plug Ins --------------------------------------------------------
plugPrefs.panelName = 'plugins';
plugPrefs.panelDescription = 'plug-ins';
plugPrefs.clientList = {};
plugPrefs.panelHelp = {
    getString(message('signal:sptprefhelp:ItIsPossibleToCreateAddonColumnsToTheSPToolAndNewButton'))
    ' '
    getString(message('signal:sptprefhelp:YouWontNeedToCheckThisBoxUnlessYouAreWritingExtensionsT'))
    ' '
    getString(message('signal:sptprefhelp:ToUseTheSPToolWithAnyExtensionsAddedYouMustCloseTheSPTo'))
    };

% controls cell array.  Columns are:
%  Name  Description  type  radiogroup  number_of_lines popup_string ...
%      factory_value  help_string
plugPrefs.controls = {
    'plugFlag' 'Search for plug-ins at start-up' 'checkbox' '' 1 ...
    {} ...
    0 ...
    plugPrefs.panelHelp
    };

plugPrefs.currentValue = plugPrefs.controls(:,7);

% Now arrange in array:
if ~isempty(which('tf/tf'));
    % Control Toolbox exists add preference for TF objects
    p = [rulerPrefs; colorPrefs; sigbrowsePrefs; spectviewPrefs;...
        filtdesPrefs; defsesPrefs;...
        exportPrefs; plugPrefs];
else
    p = [rulerPrefs; colorPrefs; sigbrowsePrefs; spectviewPrefs;...
        filtdesPrefs; defsesPrefs;...
        plugPrefs];
end
