clc ,clear;
datafile = importdata('feedback_long2018_11_12 18_2_58.log');
n=length(datafile);
datafile1=datafile;


throttle_output = datafile(:,2);
brake = datafile(:,3);
target_speed = datafile(:,4);
current_acc = datafile(:,5);
current_speed = datafile(:,6);
gear = datafile(:,7);
rim_rpm = datafile(:,9);
engine_rpm = datafile(:,10);
f_drive = datafile(:,15);
f_torque = datafile(:,17);

f_wind = datafile(:,11);                           %风阻
f_slope = datafile(:,12);                          %坡阻
f_roll = datafile(:,13);                           %滚阻
f_accel = datafile(:,14);                          %加速阻力

amt_rate = [12.158,9.435,7.349,5.77,4.464,3.493,2.724,2.114,1.646,1.293,1,0.783];

% figure;
% hold on;
% grid on;
% plot(f_wind,'b');
% plot(f_slope,'r');
% plot(f_roll,'m');
% plot(f_accel,'k');
% plot(f_drive,'c');
% legend('f_wind','f_slope','f_roll','f_accel','f_drive');
% 
% figure;
% plot(f_drive,'r');  %������
% hold on;
% plot(f_torque,'b');  %ת��
% grid on;
% %plot(throttle_output*50,'k');
% plot(current_speed*1000,'c');
% plot(target_speed*1000,'g');
% %plot(longitudinal_speed,'c');
% hold on;
% legend('f_drive','f_torque','current_speed','target_speed');

rim_rpm_filter = rim_rpm;
engine_rpm_filter = engine_rpm;
len = size(engine_rpm);
filter_buf = 5;
for i = 5:1:len
rim_rpm_filter(i) = (rim_rpm(i-4) + rim_rpm(i-3) + rim_rpm(i-2) + rim_rpm(i-1) + rim_rpm(i))/filter_buf;
engine_rpm_filter(i) = (engine_rpm(i-4) + engine_rpm(i-3) + engine_rpm(i-2) + engine_rpm(i-1) + engine_rpm(i))/filter_buf;
end;

rim_rpm_rate = zeros(len);
engine_rpm_rate = zeros(len);
for j = 3:1:len
if gear(j) == 0
gear(j) = 1;
end;
rim_rpm_rate(j) = (rim_rpm_filter(j) - rim_rpm_filter(j-2))/0.04;
engine_rpm_rate(j) = rim_rpm_rate(j)*amt_rate(gear(j))*3.727;
%engine_rpm_rate(j) = (engine_rpm_filter(j) - engine_rpm_filter(j-2))/0.04;
end;

figure;
hold on;
grid on;
plot(rim_rpm_rate*pi/30,'r');
plot(engine_rpm_rate*pi/30,'b');
%plot(rim_rpm_filter,'k');
%plot(engine_rpm_filter,'m');
