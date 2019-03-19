function [ data, samplingFrequency ] = readSignalData( fid, type, symbolPeriod, samplingPeriod )
%READSIGNALDATA Reads signal data to "visualizer".
%   [ data, samplingFrequency ] = READSIGNALDATA(fid, type, symbolPeriod, samplingPeriod)
%   just reads data ("data") from a file ("fid")
%   knowing the data parameters ("type", "symbolPeriod" and "samplingPeriod") and 
%   returning the new sampling simulation frequency ("samplingFrequency").

%% Some Standard types
tb = 'Binary';
tc1 = 'TimeDiscreteAmplitudeDiscreteComplex';
tc2 = 'TimeDiscreteAmplitudeContinuousComplex';
tc3 = 'TimeContinuousAmplitudeDiscreteComplex';
tc4 = 'TimeContinuousAmplitudeContinuousComplex';
tc5 = 'BandpassSignal';
toxy = 'OpticalSignalXY';


%% Get global variable "nRead"
nReadr = getGlobalnRead;

%% Get global variable "t_binary"
t_binaryr = getGlobalt_binary;

%% Get global variable "t_real"
t_realr = getGlobalt_real;

%% Get global variable "t_complex"
t_complexr = getGlobalt_complex;

%% Get global variable "t_complex"
global valueXY;
valueXY=struct('x',complex(0+1i*0),'y',complex(0+1i*0));



%% Sampling frequency    
samplingFrequency = 1/samplingPeriod;

%% Number of samples per period
if (symbolPeriod==1)
    samplesPerSymbol = 1;
else
    samplesPerSymbol = int64(symbolPeriod/samplingPeriod);
end

%% Read data
if strcmp(type, tb) % Binary signals
    data = fread(fid, nReadr, t_binaryr);
    data = data';
    % Change simulation sampling frequency 
    factor = 200; % Upsampling factor
    samplingFrequency = factor*samplingFrequency;
    vect = zeros(1, factor*length(data));
    for k = 1:length(data)
        vect((k-1)*factor + 1:k*factor)= ones(1, factor).*data(k);
    end 
    data = vect;
    return;
end

 
if strcmp(type, tc1) || strcmp(type, tc2) || strcmp(type, tc3) || strcmp(type, tc4) || strcmp(type, tc5)% Complex signals
   data = fread(fid, 2*double(samplesPerSymbol)*nReadr, t_complexr);
   data = data(1:2:end) + 1i.*data(2:2:end);
   data = real(data)' + imag(data)'.*1i;
   return;
end

if strcmp(type, toxy) || strcmp(type, 'PhotonStreamXY')
   data = fread(fid, 4*double(samplesPerSymbol)*nReadr, t_complexr);
   return;
end

% Other signals
data = fread(fid, double(samplesPerSymbol)*nReadr, t_realr);
data = data';

