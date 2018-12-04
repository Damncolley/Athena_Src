close all;
clc;


% steer_trans = 23.27;
datafile = importdata('vehicle_interface_lon5gear.log');
steer_angle = datafile(:,1);
acc_pedal_pos = datafile(:,2);
car_speed = datafile(:,3);
driver_torque = datafile(:,4);
eng_RPM = datafile(:,5);
wheel_speed_average = datafile(:,6);
at_gear = datafile(:,11);



figure;
hold on;
grid on;


plot(acc_pedal_pos,'r');
plot(driver_torque*0.01*3000/10,'g');
plot(eng_RPM/10,'b');
plot(at_gear*10,'m');






