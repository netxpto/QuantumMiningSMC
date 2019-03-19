function [] = ACF_20180614(fname,deltaP,N)

[ data, symbolPeriod, samplingPeriod, type, numberOfSymbols ] = readSignal_20171220( fname );

alpha = 0.05;
n_to_save = 10;

data_x =            data(1:4:end) + 1i.*data(2:4:end);
data_x =            real(data_x)' + imag(data_x)'.*1i;
data_y =            data(3:4:end) + 1i.*data(4:4:end);
data_y =          	real(data_y)' + imag(data_y)'.*1i;

mag_x =             abs(data_x(1:end));
mag_y =             abs(data_y(1:end));
%% Figures magnitude

data_x =            data_x(13:16:end);
data_y =            data_y(13:16:end);

mag_x =             abs(data_x(1:end));
mag_y =             abs(data_y(1:end));

%% stokes parameters
S0 =                data_x.*conj(data_x)+data_y.*conj(data_y);
S1 =                data_x.*conj(data_x)-data_y.*conj(data_y);
S2 =                data_x.*conj(data_y)+conj(data_x).*data_y;
S3 =                1i*(data_x.*conj(data_y)-conj(data_x).*data_y);

S0 =                S0';
S1 =                S1'./S0;
S2 =                S2'./S0;
S3 =                S3'./S0;

SS =               	[S1 S2 S3];

%% shift acf
for j = 1:N
    SS2 = circshift(SS,-j+1);
    A = dot(SS',SS2')';
    b(j,1) = mean(A,1);
    acf_value(j,1) = mean(A(1:end-j),1);
end
k =                 0:1:N-1;
time =              (k*1.6*deltaP)';


delta = num2str(deltaP);
str = ['acf_deltaP=' delta '.txt'];
fid =               fopen(str,'wt');
for ii = 1:size(acf_value,1)
    fprintf(fid,'%d \n',acf_value(ii,:));
end
fclose(fid)

acf_value = abs(acf_value);

%% plot acf
x =                 (0:1:N-1);
x =               	x';
acf_t =            	acf_value(1,1)*exp(-8*pi()*1.6*x*deltaP);

%% Confidence intervals
[phat,pci] = binofit(acf_value,100,alpha);

%% write xls file
time = time(1:n_to_save:end);
acf_value = acf_value(1:n_to_save:end);
pci = pci(1:n_to_save:end,:);
acf_t = acf_t (1:10:end);
filename = ['acf_data_' delta '_with_cf_intervals.xlsx'];
sheet = 1;
data = [time acf_value pci(:,1) pci(:,2) acf_t];
xlswrite(filename,data,sheet)

% xlswrite(filename,data,sheet)

%% figures
figure(3)
plot(time,abs(acf_value),'.')
hold on
plot(time,acf_t)
hold off
ylabel('ACF');
xlabel('\Delta T \Delta p');
legend('Numerical ACF', 'Theoretical ACF');

fclose('all')
end