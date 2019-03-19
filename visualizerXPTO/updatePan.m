function updatePan( hObject, callbackdata )
%UPDATEPAN updates "pan" objects from "visualizer".
%   UPDATEPAN just updates "pan" objects.

%% Get global variables
previousPathr = getGlobalpreviousPath;
t_binaryr = getGlobalt_binary;
t_realr = getGlobalt_real;
t_complexr = getGlobalt_complex;

%% Update pan
activeSignals = get(findobj('Tag', 'list1'), 'Value');
N = length(activeSignals);
signals = sptool('Signals');
tb = 'Binary';
td1 = 'TimeDiscreteAmplitudeDiscreteReal';
td2 = 'TimeDiscreteAmplitudeContinuousReal';
tc1 = 'TimeContinuousAmplitudeDiscreteComplex';
tc2 = 'TimeContinuousAmplitudeContinuousComplex';
tc5 = 'BandpassSignal';
if N == 1 
    if ~strcmp(signals(activeSignals(1)).type, tc1) && ~strcmp(signals(activeSignals(1)).type, tc2) && ~strcmp(signals(activeSignals(1)).type, tc5)
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
                        samplesPerSymbol = signals(activeSignals(1)).SPTIdentifier.version;
                        nShownSymbols = ((xend/ts) + 1)/samplesPerSymbol;
                        for k = 1:nShownSymbols
                            fread(fid, double(samplesPerSymbol)*1, t_realr);
                        end
                        % Read data
                        [ynew, ~] = readSignalData(fid, signals(activeSignals(1)).type, ts*double(samplesPerSymbol), ts);
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
                    samplesPerSymbol = signals(activeSignals(1)).SPTIdentifier.version;
                else
                    % Error message
                    fprintf('Error: "%s" without terminator!\n', fileName);
                end
            end
        end
        if xPanr > xendr
            if flagT == 1 && fid ~= -1
                ydatar = get(curveReal, 'YData');
                nShownSymbols = ((xendr/ts) + 1)/samplesPerSymbol;
            end
        end
        if xPani > xendi
            if flagT == 1 && fid ~= -1
                ydatai = get(curveImag, 'YData');
                nShownSymbols = ((xendi/ts) + 1)/samplesPerSymbol;
            end
        end
        if xPanr > xendr || xPani > xendi
            if flagT == 1 && fid ~= -1
                for k = 1:nShownSymbols
                    fread(fid, 2*double(samplesPerSymbol)*1, t_complexr);
                end
                % Read data
                [ynew, ~] = readSignalData(fid, signals(activeSignals(1)).type, ts*double(samplesPerSymbol), ts);
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