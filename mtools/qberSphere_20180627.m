%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%Mariana Ramos
%20180627
%Plot 3D sphere with theoretical qber distribution

clear all
clc
fclose ('all')


theta   = (-pi():pi()/360:pi())';
%phi     = linspace(-pi/4,pi/4,length(theta))';
phi     = (-pi()/2:pi()/360:pi()/2)';


for i = 1:size(theta)
    for j = 1: size(phi)
        data(i,j) = 1-0.5*(1+cos(theta(i,1))*cos(phi(j,1)));
    end
end

k = 1;
l = 1;
for i = 1:size(theta)
    for j = 1: size(phi)
        if round(data(i,j),2) == 0.10
            S(k,1) = cos(theta(i,1))*cos(phi(j,1));
            S(k,2) = sin(theta(i,1))*cos(phi(j,1));
            S(k,3) = sin(phi(j,1));
            S(k,4) = phi(j,1);
            S(k,5) = theta(i,1);
            S(k,6)=data(i,j);
            k=k+1;
        end
%         if data(i,j) > 0.049 && data(i,j) < 0.051
%         if round(data(i,j),2) == 0.050
%             SS(l,1) = cos(theta(i,1))*cos(phi(j,1));
%             SS(l,2) = sin(theta(i,1))*cos(phi(j,1));
%             SS(l,3) = sin(phi(j,1));
%             SS(l,4) = phi(j,1);
%             SS(l,5) = theta(i,1);
%             l=l+1;
%         end
    end
end
%% Choosen point
point = S(1000,:);

%% Rotations

theta_max = max(S(:,5));
phi_max = max(S(:,4));

thetar = theta_max/6;
phir = phi_max/6;

S0 = sqrt(S(:,1).^2+S(:,2).^2+S(:,3).^2);

stokes_parameters = [S(:,1) S(:,2) S(:,3)];

thetar = 2 * thetar;
phir = 2 * phir;
            
r1 = [1 0 0;
    0 cos(phir) -sin(phir);
    0 sin(phir) cos(phir)];
r2 = [cos(phir) 0 sin(phir);
    0 1 0;
    -sin(phir) 0 cos(phir)];
r3 = [cos(thetar) -sin(thetar) 0;
    sin(thetar) cos(thetar) 0;
    0 0 1];

pointr = point(:,1:3) *r1*r2*r3;
S_rotated = stokes_parameters *r1*r2*r3;

%% Calculate qber for points rotated
theta_aux = atan(pointr(1,2)/pointr(1,1));
phi_aux = atan(pointr(1,3)/(sqrt(pointr(1,1)^2)+pointr(1,2)^2));
   
qber_pointr = 1-0.5*(1+cos(theta_aux)*cos(phi_aux));

k = 1;
for i = 1:size(theta)
    for j = 1: size(phi)
        if round(data(i,j),4,'significant') == round(qber_pointr,4,'significant')
            ss(k,1) = cos(theta(i,1))*cos(phi(j,1));
            ss(k,2) = sin(theta(i,1))*cos(phi(j,1));
            ss(k,3) = sin(phi(j,1));
            ss(k,4) = phi(j,1);
            ss(k,5) = theta(i,1);
            k=k+1;
        end
    end
end


num = randi([1 length(ss)], 3,1);

p1 = ss(num(1,1),1:3);
p2 = ss(num(2,1),1:3);
p3 = ss(num(3,1),1:3);

[center,rad,v1,v2] = circlefit3d(p1,p2,p3); 

%% Esfera 3D plot
fig = figure(1)

plot_sphere(1)
hold on
h1=plot3(S(:,1),S(:,2),S(:,3),'.','LineWidth',2,...
                       'MarkerEdgeColor','r',...
                       'MarkerFaceColor','r',...
                       'MarkerSize',5)
% legend('show','Location','NorthEastOutside')
h2=plot3(S_rotated(:,1),S_rotated(:,2),S_rotated(:,3),'.','LineWidth',2,...
                       'MarkerEdgeColor','g',...
                       'MarkerFaceColor','g',...
                       'MarkerSize',5)


for i=1:361, 
    a = i/180*pi; 
    x = center(:,1)+sin(a)*rad.*v1(:,1)+cos(a)*rad.*v2(:,1); 
    y = center(:,2)+sin(a)*rad.*v1(:,2)+cos(a)*rad.*v2(:,2); 
    z = center(:,3)+sin(a)*rad.*v1(:,3)+cos(a)*rad.*v2(:,3); 
    h3=plot3(x,y,z,'b.'); 
end 

                   
h4=plot3(point(:,1),point(:,2),point(:,3),'*','LineWidth',2,...
                       'MarkerEdgeColor','k',...
                       'MarkerFaceColor','k',...
                       'MarkerSize',15)
                   
h5=plot3(pointr(:,1),pointr(:,2),pointr(:,3),'*','LineWidth',2,...
                       'MarkerEdgeColor','m',...
                       'MarkerFaceColor','m',...
                       'MarkerSize',15)
                   
leg = legend([h1 h2 h3 h4 h5],{'QBER=10%','Rotated by (\theta_{Max}/6, \phi_{Max}/6)','QBER estimated','Initial state', 'State Rotated'},'Location','north');
xlabel('s_1','fontsize',20); ylabel('s_2','fontsize',20); zlabel('s_3','fontsize',20)
hold off
% fig = gcf;
% fig.PaperUnits = 'inches';
% fig.PaperPosition = [0 0 7 5];
% print('sphere_rotation','-dpng','-r1200')


%% Esfera qber plot
% figure(3)
% [Phi, Theta]=meshgrid(phi,theta);
% 
% [X,Y,Z]=sph2cart(Theta,Phi,1);
% 
% s=surf(X,Y,Z,data,'FaceAlpha',0.5);
% xlabel('s_1','fontsize',20); ylabel('s_2','fontsize',20); zlabel('s_3','fontsize',20)
% % s=mesh(X,Y,Z,data);
% colorbar
% s.EdgeColor='none'
% axis tight

% fig = gcf;
% fig.PaperUnits = 'inches';
% fig.PaperPosition = [0 0 7 5];
% print('sphere_rotation','-dpng','-r1200')



