clc
clear all
close all

%  STEP 1 :: Generate a singal

Fs = 5e9;              % Sampling frequency                    
T = 1/Fs;              % Sampling period       
L = 2^6;               % Length of signal
t = (0:L-1)*(5*T);     % Time vector
f = linspace(-Fs/2,Fs/2,L);
    
        S = 0.7*sin(2*pi*50*t) + sin(2*pi*120*t); 
        X1 = S + 2*randn(size(t));
        X2 = 2*sin(2*pi*1e9*t).*exp(-t)+2.5*sin(2*pi*+1.6e9*t)+sin(2*pi*+1265e6*t).*cos(2*pi*+2.33e9*t)+1.5*sin(2*pi*+1e9*t).*sin(2*pi*+1.5e9*t);
        X3 = 2*sin(2*pi*1.2e9*t).*exp(-t)+2.0*sin(2*pi*+1e9*t)+sin(2*pi*+2265e6*t).*cos(2*pi*+1.786e9*t)+1.5*sin(2*pi*+1.1e9*t).*sin(2*pi*+1.75e9*t);
        X4 = 2*sin(2*pi*1.8e9*t).*exp(-t)+2.0*sin(2*pi*+1.5e9*t)+sin(2*pi*+2.5e9*t).*cos(2*pi*+1.15e9*t)+1.5*sin(2*pi*+1.1e9*t).*sin(2*pi*+1.75e9*t);
        X5 = 2*sin(2*pi*1.18e9*t).*exp(-t)+2.10*sin(2*pi*+1.15e9*t)+sin(2*pi*+2.5e9*t).*cos(2*pi*+1.1e9*t)+1.5*sin(2*pi*+1.1e9*t).*sin(2*pi*+1.15e9*t);
       
        X = [X1 X2 X3 X4 X5];

figure;
subplot(411)
plot(X1)
grid on
title('X1')
ylim([1.1*min(X1) 1.1*max(X1)]);
xlabel('Sampmle Number')
ylabel('X1(t)')

subplot(412)
plot(X2)
grid on
title('X2')
ylim([1.1*min(X2) 1.1*max(X2)]);
xlabel('Sampmle Number')
ylabel('X2(t)')

subplot(413)
plot(X3)
grid on
title('X3')
ylim([1.1*min(X3) 1.1*max(X3)]);
xlabel('Sampmle Number')
ylabel('X3(t)')

subplot(414)
plot(X)
grid on
title('X1 || X2 || X3')
xlabel('Sampmle Number')
ylabel('X1 & X2(t)')

%% STEP 2 :: Hilbert Transformation by MATLAB hilbert() function

XH = hilbert(X);
figure
plot(real(XH))
hold on
plot(imag(XH))
legend('Original Signal X','Hilbert Transformed Signal')

%% STEP 3 :: C++ logic for implementing a a filter with Transfer Function
close all
n = length(X1);
N = decideFFTLength(n);

XFront0=zeros(1,length(X1));
Xback0=zeros(1,length(X1));

a = [XFront0 X1 X2];
A = fft(a);

b = [X1 X2 X3];
B = fft(b);

c = [X2 X3 X4];
C = fft(c);

d = [X3 X4 Xback0];
D = fft(d);

XNew = [XFront0 X1 X2 X3 X4 Xback0];
filterSize = length(A);
hilbertTransferFunction = hilbertDesign(filterSize);

ResultA = overlapSave(A,hilbertTransferFunction);
ResultB = overlapSave(B,hilbertTransferFunction);
ResultC = overlapSave(C,hilbertTransferFunction);
ResultD = overlapSave(D,hilbertTransferFunction);

remove = length(XFront0);
Result = [ResultA ResultB ResultC ResultD];
close all
figure;
plot(imag(Result))
hold on
ResultReference = imag(hilbert(XNew));
plot(ResultReference(remove+1:end-remove));