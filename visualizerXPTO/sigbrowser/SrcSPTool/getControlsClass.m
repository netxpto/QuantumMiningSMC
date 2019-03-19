function controlsClass = getControlsClass(~)
%GETCONTROLSCLASS Get the controlsClass.

%   Copyright 2012 The MathWorks, Inc.
%   Carlos Ferreira Ribeiro
%   Prof. Armando Nolasco Pinto
%   January, 2016
%   IT, Aveiro.

if strcmp(version, '8.1.0.604 (R2013a)') % For MATLAB R2013a
    controlsClass = 'scopeextensions.PlaybackControlsNull';
else % For MATLAB R2014b and MATLAB R2015a
    controlsClass = 'matlabshared.scopes.source.PlaybackControlsNull';
end

% [EOF]
