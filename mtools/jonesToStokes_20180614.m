function [] = jonesToStokes(fname,deltaP, filename)

[ data, symbolPeriod, samplingPeriod, type, numberOfSymbols ] = readSignal_20171220( fname );

data_x = data(1:4:end) + 1i.*data(2:4:end);
data_x = real(data_x)' + imag(data_x)'.*1i;
data_y = data(3:4:end) + 1i.*data(4:4:end);
data_y = real(data_y)' + imag(data_y)'.*1i;

mag_x = abs(data_x(1:end));
mag_y = abs(data_y(1:end));

data_x = data_x(13:16:end);
data_y = data_y(13:16:end);

mag_x = abs(data_x(1:end));
mag_y = abs(data_y(1:end));

S0 = data_x.*conj(data_x)+data_y.*conj(data_y);

S1 = data_x.*conj(data_x)-data_y.*conj(data_y);

S2 = data_x.*conj(data_y)+conj(data_x).*data_y;

S3 = 1i*(data_x.*conj(data_y)-conj(data_x).*data_y);

S0 = S0';
S1 = S1'./S0;
S2 = S2'./S0;
S3 = S3'./S0;

s1 = mean(S1(:,1))
s2 = mean(S2(:,1))
s3 = mean(S3(:,1))
fclose('all')

%% Figures Histograms alpha parameters
delimiterIn = ' ';
headerlinesIn = 1;
data_alpha = importdata(filename,delimiterIn,headerlinesIn);

p_alpha = data_alpha.data;
alpha = [p_alpha(:,1) p_alpha(:,2) p_alpha(:,3)];
alpha2= sqrt(p_alpha(:,1).^2+p_alpha(:,2).^2+p_alpha(:,3).^2);
N = 20;

figure(1)
subplot(2,2,1)
hist(alpha(:,1),N);
ylabel('\alpha_1');
xlabel('Number Of Samples');
subplot(2,2,2)
hist(alpha(:,2),N);
ylabel('\alpha_2');
xlabel('Number Of Samples');
subplot(2,2,3)
hist(alpha(:,3),N);
ylabel('\alpha_3');
xlabel('Number Of Samples');
subplot(2,2,4)
hist(alpha2(:,1),N);
ylabel('\surd{\alpha_1^2 + \alpha_2^2 + \alpha_3^2}');
xlabel('Number Of Samples');



%% Figures Esferas

figure(2)
nplot=10800;
plot_sphere(1);
hold on;
plot3(S1(1:nplot,1)',S2(1:nplot,1)',S3(1:nplot,1)','-o','LineWidth',2,...
                       'MarkerEdgeColor','r',...
                       'MarkerFaceColor','r',...
                       'MarkerSize',5)
grid on; axis equal
xlabel('s_1'); ylabel('s_2'); zlabel('s_3')
%title('SOP Drift','fontsize',16)

fclose('all')
end