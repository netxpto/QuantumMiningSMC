function [errstr,P,f] = svfft(x,Fs,valueArray)
%SVFFT spectview Wrapper for FFT.
%  [errstr,P,f] = svfft(x,Fs,valueArray) computes the power spectrum P
%  at frequencies f using the parameters passed in via valueArray:
%
%   valueArray entry     Description
%    ------------         ----------
%          1                Nfft

% Copyright 1988-2008 The MathWorks, Inc.

errstr = '';
P = [];
f = [];


nfft = valueArray{1};
evalStr = 'P = (abs(fft(x,nfft)).^2)./Fs./length(x); f=(0:nfft-1)''*Fs./nfft;';

try
    eval(evalStr);
catch ME
    errstr = {getString(message('signal:sptoolgui:SorryCouldntEvaluateFftErrorMessage'))
               ME.message };
    return
end

[P,f] = sbswitch('svextrap',P,f,nfft);

