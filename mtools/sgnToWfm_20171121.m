function [ dataDecimate, data, symbolPeriod, samplingPeriod, type, numberOfSymbols,  samplingRate, samplingRateDecimate ] = sgnToWfm_20171121( fname_sgn, nReadr, fname_wfm )
%
%
% [ dataDecimate, data, symbolPeriod, samplingPeriod, type, numberOfSymbols, samplingRate, samplingRateDecimate ]  = sgnToWfm( fname_sgn, nReadr, fname_wfm );
%
% Inputs
% fname_sgn : Input filename of the signal you want to convert (required).
% nReadr    : Number of symbols to extract from the signal (optional).
% fname_wfm : Name that will be given to the waveform file (optional).
%
% Outputs
% A waveform file will be created in the current folder.
%
% It will also return eight variables in the workspace which are:
% dataDecimate         : A vector which contains decimated signal data by an appropriate decimation factor to make it compatible with the AWG.
% data                 : A vector with the signal data.
% symbolPeriod         : Equal to the symbol period of the corresponding signal
% samplingPeriod       : Sampling period of the signal.
% type                 : A string with the name of the signal type.
% numberOfSymbols      : Number of symbols retrieved from the signal.
% samplingRate         : Sampling rate of the signal.
% samplingRateDecimate : Reduced sampling rate which is compatible with AWG. (i.e. less than 16 GSa/s).

% Functional Description
% This matlab function generates a *.wfm file given an input signal file (*.sgn).
% the signal must be real, not exceed 8GS  and have a sampling rate equal or bellow 16 GS/s.
%
% Examples 
% Using one argument:
% [ dataDecimate, data, symbolPeriod, samplingPeriod, type, numberOfSymbols,  samplingRate, samplingRateDecimate ] = sgnToWfm('S6.sgn');
% This creates a waveform file with the same name as the *.sgn file and uses all of the samples it contains.
% 
% Using two arguments:
% [ dataDecimate, data, symbolPeriod, samplingPeriod, type, numberOfSymbols,  samplingRate, samplingRateDecimate ] = sgnToWfm('S6.sgn',256);
% This creates a waveform file with the same name as the signal file name and the number of samples
% used equals nReadr x samplesPerSymbol. The samplesPerSymbol constant is defined in the *.sgn file.
%
% Using three arguments:
% [ dataDecimate, data, symbolPeriod, samplingPeriod, type, numberOfSymbols,  samplingRate, samplingRateDecimate ] = sgnToWfm('S6.sgn',256,'myWaveform.wfm');
% This creates a waveform file with the name "myWaveform" and the number of samples used equals nReadr x samplesPerSymbol.
% The samplesPerSymbol constant is defined in the *.sgn file.

%%
if nargin == 1
    fname_wfm = [strtok(fname_sgn,'.') '.wfm'];
    nReadr = Inf;
end
if nargin == 2
    fname_wfm = [strtok(fname_sgn,'.') '.wfm'];
end

[ data, symbolPeriod, samplingPeriod, type, numberOfSymbols ] = readSignal_20170930( fname_sgn, nReadr );

%% DECIMATE START
maximumSamplingRate   = 16e9;                % set the maximum sampling rate of the AWG
maximumWaveformMemory = 8e9;                 % set the maximum waveform memory of the AWG
samplingRate          = 1/samplingPeriod;
symbolRate            = 1/symbolPeriod;

if (symbolPeriod==1)
    samplesPerSymbol = 1;
else
    samplesPerSymbol = (symbolPeriod/samplingPeriod);
end

% CALCULATE DECIMATION FACTOR 'r'.
%  r = ceil(samplingRate/maximumSamplingRate);

%%%%%%%%%%%%%%%%%% OR %%%%%%%%%%%%%%%%%%%%%

excessiveSamples = 0;                             
samplingRateReference =  1/samplingPeriod;   
if (samplingRate > maximumSamplingRate)
    while samplingRateReference > maximumSamplingRate
    samplingRateReference = samplingRateReference - symbolRate;
    excessiveSamples = excessiveSamples +1;
    end
end
maximumSamples = samplesPerSymbol-excessiveSamples;

if (maximumSamples==0)
    warning('bitPeriod is too low !!!');
    return;
end
r = ceil(samplesPerSymbol/maximumSamples);         

% CALCULATE DECIMATED FREQUENCY
samplingPeriodDecimate = samplingPeriod*r;
samplingRateDecimate = 1/samplingPeriodDecimate;

% DECIMATE DATA
if (r==inf)
    warning('Error :: Bit rate is too high !!!');
    return;
else
    dataDecimate = decimate(data,r);    
end

% DISPLAY IN COMMAND WINDOW
if (samplingRate>maximumSamplingRate)
warning('The sampling frequency was %dGHz and the signal was decimated to obtain a sampling frequency of %gGHz (< %gGHz).\n',samplingRate/1e9, samplingRateDecimate/1e9,maximumSamplingRate/1e9);
end
fprintf('\nThe decimation factor r = %d\n',r);

%% Uncomment this section to view the TO COMPARE ORIGINAL AND DECIMATED DATA
% figure;
% t = (0:1:length(data)-1)*samplingPeriod;
% tDecimate = (0:r:length(data)-1)*samplingPeriod;
% plot(t(1:end),data(1:end),'-o');
% grid on
% hold on
% stem(tDecimate(1:end),dataDecimate(1:end),'ro','filled','markersize',4);
% xlabel 'Time in Seconds',ylabel 'Signals'
% legend('Original','Decimated')
% 
% figure;
% plot(t(1:end),data(1:end));
% hold on;
% plot(tDecimate(1:end),dataDecimate(1:end),'LineWidth',3);
% xlabel 'Time in Seconds',ylabel 'Signal'
% legend('Simulated Waveform','Downsampled AWG Waveform')
% title('Simulated vs AWG waveform');

% DECIMATE END
%% PRINT ERROR MESSAGE WHEN SIGNAL IS NOT 'TimeContinuousAmplitudeContinuousReal'.
if (~strcmp(type,'TimeContinuousAmplitudeContinuousReal'))
     msgbox('Problem with the signal file. Please check the matlab command window for more information.','Error','error');
     error('\nError: The chosen signal type is not compatible with this function (%s).\nMake sure the type of the signal is TimeContinuousAmplitudeContinuousReal. \n\n',type);
     clear all;
     return;
end

% PRINT ERROR MESSAGE WHEN SIGNAL IS GREATER THAN 'maximumWaveformMemory'.
if    (length(dataDecimate) >  maximumWaveformMemory)                 
      msgbox('Problem with the signal file. Please check the matlab command window for more information.','Error','error');
      error('\nError: The chosen signal has to many samples(%d GS).\nMake sure it is less or equal to 8 G samples.\n\n',length(dataDecimate)/1e9);
      clear all;
      return;
end

% THIS IF LOOP CONVERT THE LENGTH OF THE SIGNAL TO BE INTEGER MULTIPLE OF 4
if (length(dataDecimate)/4 ~= 0)
    dataDecimate=dataDecimate(1:length(dataDecimate)-rem(length(dataDecimate),4));
end

dataDecimate=dataDecimate/max(abs(dataDecimate));     % Normalizing the data
dataDecimate=dataDecimate';                           % Row to Columun 
sizeMark=size(dataDecimate,1);                        % sizeMark = number of row
marker = false(sizeMark, 2);                          % Initializing markers (false will generate mattrix of size 'sizeMark x 2' with all zero logical values)

BuildTektronixAWG710xWFM(dataDecimate,marker,samplingRateDecimate,fname_wfm);

end


function BuildTektronixAWG710xWFM(Signal,Marker,ClockRate,Filename)
% This function will build a WFM file for importing data into a Tektronix
% Arbitrary Waveform Generator Model number AWG710 or AWG710B
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


% perform checks on the Signal input
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




