clc;
clear all;
close all;
%%
N = 2^11;

timeWindow = 20;
dt = timeWindow/N;
t = -timeWindow/2:dt:timeWindow/2-dt;

y = sin(2*pi*t).*exp(-abs(t));
%y = cos(2*pi*t).*exp(-abs(t));
%y = t;
% tol = 1.e-4;
% y(y<0 & y>-tol) = 0;


figure(1);
plot(t,y);
xlabel('t');
ylabel('y(t)');

frequencyWindow = 1/dt;
df = frequencyWindow/N;
f = -frequencyWindow/2:df:frequencyWindow/2-df;

fy = fftshift(fft(y));

figure(2);
subplot(2,1,1)
plot(f,abs(fy));
axis([-2.5 2.5 0 1.1*max(abs(fy))]);
xlabel('f');
ylabel('|Y(f)|');
title('MATLAB program Calculation : Magnitude');

subplot(2,1,2)
plot(f,phase(fy));
axis([-2.5 2.5 -pi pi]);
xlabel('f');
ylabel('phase(Y(f))');
title('MATLAB program Calculation : Phase');


% Save MATLAB data in the txt file which will be used in the c++ program
dlmwrite('time_function.txt', y, 'delimiter','\t');
dlmwrite('C:\Users\romilpatel\Documents\GitHub\LinkPlanner.Romil\mtools\test_fft\time_function.txt', y, 'delimiter','\t');

%save('time_function.mat','y');
%load('time_function.mat');

%% Plotting data 

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
%
Z=Z';
tol = 1.e-4;
Z(Z<0 & Z>-tol) = 0;
%fZ =Z;
fZ = fftshift(Z);


test_y = load('test_y.txt');
figure(3);
plot(y);
hold on;
plot(test_y,'--o')
legend('MATLAB','C++')
%y = (test_y);

% figure(4);
% subplot(2,1,1)
% plot(f,abs(fZ));
% axis([-2.5 2.5 0 1.1*max(abs(fZ))]);
% xlabel('f');
% ylabel('|fZ(f)|');
% title('C++ program Calculation : Magnitude');
% 
% subplot(2,1,2)
% plot(f,phase(fZ));
% axis([-2.5 2.5 -pi pi]);
% xlabel('f');
% ylabel('phase(fZ(f))');
% title('C++ program Calculation : Phase');


figure;
subplot(2,1,1)
plot(abs(fftshift(fft(y))))
hold on
plot(abs(fftshift(Z)),'o')
title('Main reference for Magnitude')
legend('MATLAB','C++')

subplot(2,1,2)
plot(phase(fftshift(fft(y))))
hold on
plot(phase(fftshift(Z)),'--o')
%plot(phase(round(Z)),'--o')
title('Main reference for Phase')
legend('MATLAB','C++')




