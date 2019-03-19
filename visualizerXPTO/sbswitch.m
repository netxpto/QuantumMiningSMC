function varargout = sbswitch(varargin)
%SBSWITCH Function switch-yard.
%        SBSWITCH('FOO',ARG1,ARG2,...) is the same as
%        FOO(ARG1,ARG2,...).  This provides access to private
%        functions for Handle Graphics callbacks.

%        Copied from IMSWITCH.

%   Copyright 1988-2002 The MathWorks, Inc.
%   Carlos Ferreira Ribeiro
%   Prof. Armando Nolasco Pinto
%   January, 2016
%   IT, Aveiro.

if (nargout == 0)
  feval(varargin{:});
  if length(findobj('Tag', 'rulerbox1')) == 1
    %% Change printf markers formats 
    eFormat = '%5.3e';
    fFormat = '%5.3f';
    rb1 = findobj('Tag', 'rulerbox1'); set(rb1, 'String', sprintf(eFormat, get(rb1, 'UserData')));  
    rb2 = findobj('Tag', 'rulerbox2'); set(rb2,'String', sprintf(eFormat, get(rb2, 'UserData')));    
    y1t = findobj('Tag', 'y1text'); set(y1t, 'String', sprintf(fFormat, str2double(get(y1t,'String'))));
    y2t = findobj('Tag','y2text'); set(y2t, 'String', sprintf(fFormat, str2double(get(y2t, 'String'))));    
    dxt = findobj('Tag', 'dxtext'); set(dxt, 'String', sprintf(eFormat, str2double(get(dxt,'String'))));
    dyt = findobj('Tag', 'dytext'); set(dyt, 'String', sprintf(fFormat, str2double(get(dyt, 'String')))); 
    %%
  end
else
  [varargout{1:nargout}] = feval(varargin{:});
end

