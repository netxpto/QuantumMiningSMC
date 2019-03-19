
function [mag_x, mag_y, phase_dif, h] = plotPhotonStream_20180102(fname, opt, h)
% If opt equals 0, or no opt, we plot the absolute value of X and Y and the phase
% difference.
% If opt equals 1, we plot the the amplitude of X and Y, this is only
% possoble when X and Y are real values.

if nargin==1
    opt = 0;
    h = figure();
elseif nargin==2
    h = figure();
end

[data, symbolperiod,sampling,type,number] = readSignal_20171220(fname);

data_x = data(1:4:end) + 1i.*data(2:4:end);
data_x = real(data_x)' + imag(data_x)'.*1i;
data_y = data(3:4:end) + 1i.*data(4:4:end);
data_y = real(data_y)' + imag(data_y)'.*1i;

mag_x = abs(data_x(1:end));
mag_y = abs(data_y(1:end));


phase_dif = angle(data_x(1:end)) - angle(data_y(1:end));
phase_dif = abs(rad2deg(phase_dif));

figure(h);
if opt == 0
    subplot(3,1,1);
    plotSignal_20171220(mag_x,symbolperiod,sampling,'PhotonStreamXY',number)
    axis([0 inf 0 1])
    title ('|Amplitude X|');
    subplot(3,1,2);
    plotSignal_20171220(mag_y,symbolperiod,sampling,'PhotonStreamXY',number)
    axis([0 inf 0 1])
    title('|Amplitude Y|');
    subplot(3,1,3);
    plotSignal_20171220(phase_dif,symbolperiod,sampling,'PhotonStreamXY',number)
    axis([0 inf 0 180])
    title ('Phase shift (Degrees)');
else
%     subplot(2,1,1);
%     plotSignal_20171220(data_x,symbolperiod,sampling,'PhotonStreamXY',number)
%     axis([0 inf -1 1])
%     title ('Amplitude X');
%     subplot(2,1,2);
%     plotSignal_20171220(data_y,symbolperiod,sampling,'PhotonStreamXY',number)
%     axis([0 inf -1 1])
%     title('Amplitude Y');
    plotSignal_20171220(data_x,symbolperiod,sampling,'PhotonStreamXY',number);
    hold on
    plotSignal_20171220(data_y,symbolperiod,sampling,'PhotonStreamXY',number);
    axis([0 inf -1 1])
    title ('Amplitude X and Y');
    legend('X', 'Y')
    
  
end