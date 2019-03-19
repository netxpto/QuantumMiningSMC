function togglePan( this, ~, ~)
%TOGGLEPAN updates an pan object.
%   TOGGLEPAN(this,~,~) it is the dynamic pan callback.

%   The MathWorks, Inc.
%   Carlos Ferreira Ribeiro
%   Prof. Armando Nolasco Pinto
%   January, 2016
%   IT, Aveiro.

%% toggle pan
toggle( this, 'Pan' );
% For MATLAB R2014b and R2015a
if  ~strcmp(version, '8.1.0.604 (R2013a)')
    signalBrowser = findobj('Tag', 'SignalBrowser');
    if ~isempty(signalBrowser) 
        if strcmp(get(signalBrowser, 'Visible'), 'on')
            h = pan(findobj('Tag', 'SignalBrowser'));
            h.ActionPostCallback = @updatePan;
        end
    end
end
%%
end

function updatePan( obj, evd )
%UPDATEPAN updates "pan" objects from "visualizer".
%   UPDATEPAN just updates "pan" objects.

%% Get global variables
load previousPath.mat;
previousPathr = previousPath;
load t_binary.mat;
t_binaryr = t_binary;
load t_real.mat;
t_realr = t_real;
load t_complex.mat;
t_complexr = t_complex;

%% Update pan
activeSignals = get(findobj('Tag', 'list1'), 'Value');
N = length(activeSignals);
signals = sptool('Signals');
tb = 'Binary';
td1 = 'TimeDiscreteAmplitudeDiscreteReal';
td2 = 'TimeDiscreteAmplitudeContinuousReal';
tc1 = 'TimeContinuousAmplitudeDiscreteComplex';
tc2 = 'TimeContinuousAmplitudeContinuousComplex';
if N == 1 
    if ~strcmp(signals(activeSignals(1)).type, tc1) && ~strcmp(signals(activeSignals(1)).type, tc2)
        xlim = get(findobj('Tag', 'DisplayAxes1_RealMag'), 'XLim');
        xPan = xlim(2);
        if strcmp(signals(activeSignals(1)).type, td1) || strcmp(signals(activeSignals(1)).type, td2)
            curve = findobj('Tag', 'st1');
        else
            curve = findobj('Tag', 'DisplayLine1'); 
        end
        xdata = get(curve, 'XData');
        xend = xdata(end);
        if xPan > xend
            ydata = get(curve, 'YData');
            path = previousPathr;
            arg = {path, '\', signals(activeSignals(1)).label, '.sgn'};
            fileName = strjoin(arg, ''); % fileName with fullpath
            % Open file
            fid = fopen(fileName);
            if fid == -1
                % Error message
                fprintf('Error: File not found!\n');
            else
                % Read header
                [~, ~, ~, flagT] = readSignalHeader(fid);            
                if flagT == 1
                    ts = 1/signals(activeSignals(1)).Fs;
                    if strcmp(signals(activeSignals(1)).type, tb)
                        factor = 200;
                        nShownBits = ((xend/ts) + 1)/factor;
                        for k = 1:nShownBits
                            fread(fid, 1, t_binaryr);
                        end
                        % Read data
                        [ynew, ~] = readSignalData(fid, tb, ts*factor, ts*factor);
                    else
                        nSamples = signals(activeSignals(1)).SPTIdentifier.version;
                        nShownSymbols = ((xend/ts) + 1)/nSamples;
                        for k = 1:nShownSymbols
                            fread(fid, double(nSamples)*1, t_realr);
                        end
                        % Read data
                        [ynew, ~] = readSignalData(fid, signals(activeSignals(1)).type, ts*double(nSamples), ts);
                    end
                    % Update
                    xnew = xend + ts:ts:xend + ts*length(ynew);
                    set(curve, 'XData', [xdata xnew], 'YData', [ydata ynew]);
                else
                    % Error message
                    fprintf('Error: "%s" without terminator!\n', fileName);
                end
            end
            % Close file
            fclose(fid);
        end 
    else
        axesReal = findobj('Tag', 'axesReal');
        axesImag = findobj('Tag', 'axesImag');
        curveReal = findobj('Tag', 'pr1');
        curveImag = findobj('Tag', 'pi1');
        xlimr = get(axesReal, 'XLim');
        xPanr = xlimr(2);
        xdatar = get(curveReal, 'XData');
        xendr = xdatar(end); 
        xlimi = get(axesImag, 'XLim');        
        xPani = xlimi(2);
        xdatai = get(curveImag, 'XData');
        xendi = xdatai(end);
        if xPanr > xendr || xPani > xendi
            path = previousPathr;
            arg = {path, '\', signals(activeSignals(1)).label, '.sgn'};
            fileName = strjoin(arg, ''); % fileName with fullpath
            % Open file
            fid = fopen(fileName);
            if fid == -1
                % Error message
                fprintf('Error: File not found!\n');
            else
                % Read header
                [~, ~, ~, flagT] = readSignalHeader(fid); 
                if flagT == 1
                    ts = 1/signals(activeSignals(1)).Fs;
                    nSamples = signals(activeSignals(1)).SPTIdentifier.version;
                else
                    % Error message
                    fprintf('Error: "%s" without terminator!\n', fileName);
                end
            end
        end
        if xPanr > xendr
            if flagT == 1 && fid ~= -1
                ydatar = get(curveReal, 'YData');
                nShownSymbols = ((xendr/ts) + 1)/nSamples;
            end
        end
        if xPani > xendi
            if flagT == 1 && fid ~= -1
                ydatai = get(curveImag, 'YData');
                nShownSymbols = ((xendi/ts) + 1)/nSamples;
            end
        end
        if xPanr > xendr || xPani > xendi
            if flagT == 1 && fid ~= -1
                for k = 1:nShownSymbols
                    fread(fid, 2*double(nSamples)*1, t_complexr);
                end
                % Read data
                [ynew, ~] = readSignalData(fid, signals(activeSignals(1)).type, ts*double(nSamples), ts);
            end
        end
        if xPanr > xendr
            if flagT == 1 && fid ~= -1
                % Update
                ynewr = real(ynew);
                xnewr = xendr + ts:ts:xendr + ts*length(ynewr);
                set(curveReal, 'XData', [xdatar xnewr], 'YData', [ydatar ynewr]);
            end
        end
        if xPani > xendi
            if flagT == 1 && fid ~= -1
                % Update
                ynewi = imag(ynew);
                xnewi = xendi + ts:ts:xendi + ts*length(ynewi);
                set(curveImag, 'XData', [xdatai xnewi], 'YData', [ydatai ynewi]);
            end
        end
        if xPanr > xendr || xPani > xendi
            % Close file
            fclose(fid);
        end 
    end
end
%%
end

function [ type, symbolPeriod, samplingPeriod, flagT ] = readSignalHeader( fid )
%READSIGNALHEADER Reads a signal header file to "visualizer".
%   [ type, symbolPeriod, samplingPeriod, flagT ] = READSIGNALHEADER( fid )
%   just reads a header file ("fid")
%   returning the data parameters ("type", "symbolPeriod" and "samplingPeriod"). 
%   If flagT == 1, was found a terminator. 

%% Signal type
type = strsplit(fgetl(fid), {' ', ':'});
type = char(type(end));

%% Symbol period
symbolPeriod = strsplit(fgetl(fid), {' ', ':'});
symbolPeriod = str2double(char(symbolPeriod(end)));

%% Sampling period
samplingPeriod = strsplit(fgetl(fid), {' ', ':'});
samplingPeriod = str2double(char(samplingPeriod(end)));

%% Terminator and flagT
str = '';
terminator = '// ### HEADER TERMINATOR ###';
counterT = 0; flagT = 1;
while ~strcmp(str, terminator)
    str = fgetl(fid);
    counterT = counterT + 1;
    if counterT > 100
        flagT = 0;
        break;
    end
end
%%
end
   
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

%% Get global variable "nRead"
load nRead.mat;
nReadr = nRead;

%% Get global variable "t_binary"
load t_binary.mat;
t_binaryr = t_binary;

%% Get global variable "t_real"
load t_real.mat;
t_realr = t_real;

%% Get global variable "t_complex"
load t_complex.mat;
t_complexr =t_complex;

%% Sampling frequency    
samplingFrequency = 1/samplingPeriod;

%% Number of samples per period
nSamples = int64(symbolPeriod/samplingPeriod);

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
else 
    if strcmp(type, tc1) || strcmp(type, tc2) || strcmp(type, tc3) || strcmp(type, tc4) % Complex signals
        data = fread(fid, 2*double(nSamples)*nReadr, t_complexr);
        data = data(1:2:end) + 1i.*data(2:2:end);
        data = real(data)' + imag(data)'.*1i;
    else % Other signals  
        data = fread(fid, double(nSamples)*nReadr, t_realr);
        data = data';
    end
end
%%
end

