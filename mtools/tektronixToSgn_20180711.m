function [] = tektronixToSgn_20180711(Srx, symbolPeriod, samplingPeriod, filename, RTO)
% [status] = tektronixToSgn(Srx, symbolPeriod, samplingPeriod, filename)
%
% Inputs:
% 	Srx:				Array with the raw array of data received from the 
%                       oscilloscope
% 	symbolPeriod:	 	Symbol period of the signal present in the array
% 	samplingPeriod:		Sampling period of the oscilloscope
%	filename:			Name of the files to create. To this name it will
%						appended a letter to describe if it is in-phase or
%						quadrature, and the file extension '.sgn'
%	RTO:				Structure with the oscilloscope configuration
%
%
% Functional Description:
%	This function takes a signal acquired from a Tektronix oscilloscope,
%	converts it to volts and generates two .sgn files similar to the ones
%	created by the netXPTO platform. Each file contains either the in-phase
%   or the quadrature components of the signal. If RTO is not defined, the 
%   scaling of the oscilloscope is considered to be 10 mV/div for both
%   channels. 
%
%   Known bugs
%
%   Future Work
%
%   Andoni Santos       andoni_santos@av.it.pt
%   MATLAB version: 	R2018a (Probably works with older versions.)
%   Last update:        24-May-2018

%% Create and/or open files
fidI = fopen([filename '_i.sgn'],'w+');
fidQ = fopen([filename '_q.sgn'],'w+');
fileType = 'TimeContinuousAmplitudeContinuousReal';

%% Define acquisition parameters
vResolution = 2^8;						% The vertical resolution is 8 bits
maxValue = 2^15;						% The maximum values are up to +-2^15
										% but the values are sparse. They
										% can only take a total of
										% vResolution discrete values.
if ~exist('RTO','var')
	fullScaleI = 10*RTO.scale(RTO.channels(1)); % Scale provides the V/div,
	fullScaleQ = 10*RTO.scale(RTO.channels(2));	% there are 10 divisions.
else
	fullScaleI = 0.1; 	% If RTO is not defined, assume default values for
	fullScaleQ = 0.1;	% the Tektronix DPO72004B lowest scaling.
end

%% Convert acquired values to V
Srx = real(Srx)/maxValue*(fullScaleI)/2 + ... 
    1i * imag(Srx)/maxValue*(fullScaleQ)/2;

%% Write the file headers
fprintf(fidI,'%s\r\n',['Signal type: ' fileType]);
fprintf(fidI,'%s\r\n',['Symbol Period (s):' num2str(symbolPeriod, ... 
                                                                '%0.5e')]);
fprintf(fidI,'%s\r\n',['Sampling Period (s):' num2str(samplingPeriod, ...
                                                                '%0.5e')]);
fprintf(fidI,'%s\r\n','// ### HEADER TERMINATOR ###');

fprintf(fidQ,'%s\r\n',['Signal type: ' fileType]);
fprintf(fidQ,'%s\r\n',['Symbol Period (s):' num2str(symbolPeriod, ...
                                                                '%0.5e')]);
fprintf(fidQ,'%s\r\n',['Sampling Period (s):' num2str(samplingPeriod, ...
                                                                '%0.5e')]);
fprintf(fidQ,'%s\r\n','// ### HEADER TERMINATOR ###');

fwrite(fidI,real(Srx), 'double');
fwrite(fidQ,imag(Srx), 'double');


fclose(fidI);
fclose(fidQ);
