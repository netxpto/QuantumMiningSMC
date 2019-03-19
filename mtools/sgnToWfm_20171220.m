function [ data, symbolPeriod, samplingPeriod, type, numberOfSymbols,samplingRate ] = sgnToWfm( fname_sgn, nReadr, fname_wfm )
%
%
% [ data, symbolPeriod, samplingPeriod, type, numberOfSymbols,samplingRate ] = sgnToWfm( fname_sgn, nReadr, fname_wfm );
%
% Inputs
% fname_sgn: Input filename of the signal you want to convert
% nReadr:    Number of symbols you want to extract from the signal.
% fname_wfm: Name that will be given to the waveform file.
%
% Outputs
% A waveform file will be created in the current folder.
%
% It will also return six variables in the workspace which are:
% data: A vector with the signal data.
% symbolPeriod} Equal to the symbol period of the corresponding signal
% samplingPeriod: Sampling period of the signal.
% type: A string with the name of the signal type.
% numberOfSymbols: Number of symbols retrieved from the signal.
% samplingRate: Sampling rate of the signal.
%
% Functional Description
% This matlab function generates a *.wfm file given an input signal file (*.sgn).
% the signal must be real, not exceed 8GS  and have a sampling rate equal or bellow 16 GS/s. 
%
% Examples 
% Using one argument:
% [data, symbolPeriod, samplingPeriod, type, numberOfSymbols, samplingRate] = sgnToWfm('S6.sgn');
% This creates a waveform file with the same name as the *.sgn file and uses all of the samples it contains.
% 
% Using two arguments:
% [data, symbolPeriod, samplingPeriod, type, numberOfSymbols, samplingRate] = sgnToWfm('S6.sgn',256);
% This creates a waveform file with the same name as the signal file name and the number of samples
% used equals nReadr x samplesPerSymbol. The samplesPerSymbol constant is defined in the *.sgn file.
%
% Using three arguments:
% [data, symbolPeriod, samplingPeriod, type, numberOfSymbols, samplingRate] = sgnToWfm('S6.sgn',256,'myWaveform.wfm');
% This creates a waveform file with the name "myWaveform" and the number of samples used equals nReadr x samplesPerSymbol.
% The samplesPerSymbol constant is defined in the *.sgn file.

if nargin == 1
    fname_wfm = [strtok(fname_sgn,'.') '.wfm'];
    nReadr = Inf;
end
if nargin == 2
    fname_wfm = [strtok(fname_sgn,'.') '.wfm'];
end
[ data, symbolPeriod, samplingPeriod, type, numberOfSymbols ] = readSignal( fname_sgn, nReadr );
samplingRate=1/samplingPeriod;
if (~strcmp(type,'TimeContinuousAmplitudeContinuousReal'))
     msgbox('Problem with the signal file. Please check the matlab command window for more information.','Error','error');
     error('\nError: The chosen signal type is not compatible with this function (%s).\nMake sure the type of the signal is TimeContinuousAmplitudeContinuousReal. \n\n',type);
     clear all;
     return;
end
% if (samplingRate >  16e9)
%      msgbox('Problem with the signal file. Please check the matlab command window for more information.','Error','error');
%      error('\nError: The Sampling Rate of the chosen signal(%d GS/s) is greater than the maximum sampling frequency of the AWG(16 GS/s).\nPlease choose a signal with a sampling frequency lower than 16 GS/s.\n\n',samplingRate/1e9);
%       clear all;
%       return;
% end
if (length(data) >  8e9)
      msgbox('Problem with the signal file. Please check the matlab command window for more information.','Error','error');
      error('\nError: The chosen signal has to many samples(%d GS).\nMake sure it is less or equal to 8 G samples.\n\n',length(data)/1e9);
      clear all;
      return;
end
if (length(data)/4 ~= 0)
    data=data(1:length(data)-rem(length(data),4));
end
data=data/max(abs(data));
data=data';
sizeMark=size(data,1);
marker = false(sizeMark, 2);  % Initializing markers


BuildTektronixAWG710xWFM(data,marker,samplingRate,fname_wfm);


% This function will build a WFM file for importing data into a Tektronix
% Arbitrary Waveform Generator Model number AWG710 or AWG710B
%
% known limitations
%           - This function will not check that the data will fit within 
%             the memory of the device
%           - This function will not check that the clock rate is within
%             the capabilites if the 
%
% Inputs
% Signal    - A double precision floating point input vector that contains the
%             data which will be "played" by the DAC. The instrument requries
%             that the data is scaled between -1 and +1. This function will
%             not rescale the data if will generate an error. The instrument
%             requires that the signal length is a multiple of 4, this is
%             checked by this function.
% Marker    - A logical array with the same number of rows as the Signal
%             Vector and two columns, one for each marker channel
% ClockRate - The fucntion perfroms no checks on the ClockRate othe that
%             data type
% Filename  - Output filename

function BuildTektronixAWG710xWFM(Signal,Marker,ClockRate,Filename)

%% perform checks on the Signal input
if ~isfloat(Signal)
    error('The signal input must be floating point')
end

sample_points = size(Signal,1);
if mod(sample_points,4) ~= 0
    error('The signal input length must be a multple of 4')
end

if size(Signal,2) ~= 1
    error('Signal must be a column vector')
end

if (max(Signal) > 1.0) || (min(Signal) < -1.0)
    error('Signal is incorrectly scaled')
end

if (max(Signal) < 0.1) || (min(Signal) > -0.1)
    warning('Signal may only be using 10% of the instrument dynmic range')
end


    
%% perform checks on Marker Input
if ~islogical(Marker)
    error('The marker input must be logical type')
end

if sample_points ~= size(Marker,1)
    error('length of the marker array must be the same as the signal')
end

if size(Marker,2) ~= 2
    error('Marker array should have two columns')
end
%the two lower bits of the marker word correspond to the marker channels
%therefore the second marker channel must be weighted by 2
combined_marker = uint8( (uint8(Marker(:,2)).*2) + uint8(Marker(:,1)) );

%% perform checks on clock rate
if ~isfloat(ClockRate)
    error('Clock rate must be a floating point')
end

%% build file
fid = fopen(Filename, 'w');
fwrite(fid, 'MAGIC 1000');
crlf = hex2dec({'0d' '0a'});
fwrite(fid, crlf);

sample_bytes = num2str(sample_points*5);
length_bytes = num2str(length(sample_bytes));
header = ['#' length_bytes sample_bytes];
fwrite(fid, header);
position_after_header = ftell(fid);

fwrite(fid, Signal(1), 'float');
fwrite(fid, Signal(2:end), 'float', 1); % skip marker bytes

fseek(fid,position_after_header,'bof');
fwrite(fid, combined_marker, 'uint8', 4);


fwrite(fid, ['CLOCK', sprintf('%7e',ClockRate)]);
fwrite(fid, crlf);
fclose(fid);




