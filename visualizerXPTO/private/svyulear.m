function [errstr,P,f] = svyulear(x,Fs,valueArray)
%SVYULEAR spectview Wrapper for the Yule-Walker AR method.
%  [errstr,P,f] = svyulear(x,Fs,valueArray) computes the power spectrum P
%  at frequencies f using the parameters passed in via valueArray:
%
%   valueArray entry     Description
%    ------------         ----------
%          1                Order
%          2                Nfft
%          3                Correlation matrix checkbox

%   Copyright 1988-2008 The MathWorks, Inc.

errstr = '';
P = [];
f = [];

order = valueArray{1};
nfft = valueArray{2};

evalStr = '[P,f] = pyulear(x,order,nfft,Fs);';

try
    eval(evalStr);
catch ME
    errstr = {getString(message('signal:sptoolgui:SorryCouldntEvaluatePyulearErrorMessage'))
               ME.message };
    return
end

[P,f] = svextrap(P,f,nfft);

