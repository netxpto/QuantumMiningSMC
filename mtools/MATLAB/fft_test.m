clc
clear all
close all

%%

Fs = 1e5;              % Sampling frequency                    
T = 1/Fs;              % Sampling period       
L = 2^10;              % Length of signal
t = (0:L-1)*(5*T);     % Time vector
f = linspace(-Fs/2,Fs/2,L);

%   S = 0.7*sin(2*pi*50*t) + sin(2*pi*120*t);
%   X = S + 2*randn(size(t));          % Signal with two signusoids and random noise
% 
% X = sin(2*pi*t);                   % Single sinusoids      
% 
% X = sin(2*pi*t) + cos(2*pi*t);     % Summation of two sinusoids
% 
% X = sin(2*pi*10*t).*exp(-abs(t));  % Single sinusoids with exponent
% 
% X = sin(2*pi*10*t).*exp(-t)+sin(2*pi*t)+7*sin(2*pi*+5*t)+7*cos(2*pi*+20*t)+5*sin(2*pi*+50*t);   % Mixed signal 1

X = 2*sin(2*pi*100*t).*exp(-t)+2.5*sin(2*pi*+250*t)+sin(2*pi*+50*t).*cos(2*pi*+20*t)+1.5*sin(2*pi*+50*t).*sin(2*pi*+150*t); % Mixed signal 2



plot(t(1:end),X(1:end))
title('Single sinusoids with exponent')
xlabel('t (Seconds)')
ylabel('X(t)')

dlmwrite('time_function.txt', X, 'delimiter','\t');

tic
fy =fft(X);
toc
fy = fftshift(fy);
figure(2);
subplot(2,1,1)
plot(f,abs(fy));
%axis([-Fs/(2*4) Fs/(2*4) 0 1.1*max(abs(fy))]);
xlabel('f');
ylabel('|Y(f)|');
title('MATLAB program Calculation : Magnitude');

subplot(2,1,2)
plot(f,phase(fy));
%axis([-Fs/(2*4) Fs/(2*4) -1.5*pi 1.5*pi]);
xlabel('f');
ylabel('phase(Y(f))');
title('MATLAB program Calculation : Phase');

%%
% Read C++ transformed data file
fullData = load('FFT_data_by_cpp.txt');
A=1;
B=A+1;
l=1;
Z=zeros(length(fullData)/2,1);
while (l<=length(Z))
Z(l) = fullData(A)+fullData(B)*1i;
A = A+2;
B = B+2;
l=l+1;
end
Z=Z';

%
figure;
subplot(2,1,1)
plot(f,abs(fftshift(fft(X))))
hold on
plot(f,abs(fftshift(Z)),'--o')
%plot(f,sqrt(length(Z))*abs(fftshift(Z)),'--o') % Absoulute values are
%multiplied with sqrt(n) to verify our C++ code with the MATLAB implemenrtation .
xlabel('f');
title('Main reference for Magnitude')
legend('MATLAB','C++')

subplot(2,1,2)
plot(f,phase(fftshift(fft(X))))
hold on
plot(f,phase(fftshift(Z)),'--o')
title('Main reference for Phase')
xlabel('f');
legend('MATLAB','C++')

