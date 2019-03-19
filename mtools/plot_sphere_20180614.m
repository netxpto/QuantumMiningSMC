function plot_sphere(r)


[x,y,z] = sphere(50);
x0 = 0; y0 = 0; z0 = 0;
x = x*r + x0;
y = y*r + y0;
z = z*r + z0;

lightGrey = 0.8*[1 1 1]; % It looks better if the lines are lighter
h = surfl(x, y, z); 
set(h, 'FaceColor', 'none','EdgeColor',lightGrey)


end