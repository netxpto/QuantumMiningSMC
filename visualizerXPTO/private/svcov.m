function [errstr,P,f] = svcov(x,Fs,valueArray)
%SVCOV spectview Wrapper for Covariance method.
%  [errstr,P,f] = SVCOV(x,Fs,valueArray) computes the power spectrum P
%  at frequencies f using the parameters passed in via valueArray:
%
%   valueArray entry     Description
%    ------------         ----------
%          1                Order
%          2                Nfft

%   Copyright 1988-2008 The MathWorks, Inc.

errstr = '';
P = [];
f = [];

order = valueArray{1};
nfft  = valueArray{2};
evalStr = '[P,f] = pcov(x,order,nfft,Fs);';

try
    eval(evalStr);
catch ME
    errstr = {getString(message('signal:sptoolgui:SorryCouldntEvaluatePcovErrorMessage'))
               ME.message };
    return
end

[P,f] = svextrap(P,f,nfft);

