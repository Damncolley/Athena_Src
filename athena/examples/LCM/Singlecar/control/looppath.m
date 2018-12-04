close all;
clc;

% steer_trans = 23.27;
datafile = importdata('looptest.txt');
lat = datafile(:,1);
lon = datafile(:,2);
height = datafile(:,3);
longitudinal_speed = datafile(:,4);
lateral_speed = datafile(:,5);
down_speed = datafile(:,6);
roll = datafile(:,7);
pitch = datafile(:,8);
heading = datafile(:,9);
longitudinal_accelerate = datafile(:,10);
lateral_accelerate = datafile(:,11);
down_accelerate = datafile(:,12);
roll_speed = datafile(:,13);
pitch_speed = datafile(:,14);
heading_speed = datafile(:,15);
steer_angle = datafile(:,16);

len =  size(steer_angle);
lon_speed_calc = longitudinal_speed;

for j = 2:1:len
lon_speed_calc(j) = lon_speed_calc(j-1) + longitudinal_accelerate(j)*0.02;
end;


figure;
plot(lat,lon);
grid on;
axis equal;

figure;
hold on;
grid on;
title('longitudinal accel');
plot(longitudinal_speed,'b');
plot(longitudinal_accelerate,'r');
plot(lon_speed_calc,'m');

figure;
hold on;
grid on;
title('pitch');
plot(pitch+0.32,'b');
%plot(longitudinal_accelerate,'r');





