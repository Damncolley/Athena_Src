close all;
clc;
datafile = importdata('feedback_long2018_11_17 18_13_45_motion.log');

acc_driving_mode_ = datafile(:,1);
acc_value_ = datafile(:,2);                      %油门输出
brake_value_ = datafile(:,3);                    %刹车值输出
tar_speed_ = datafile(:,4);                 %期望速度
tar_accel_speed_ = datafile(:,5);           %期望加速度
position_x_ = datafile(:,6);      %参考位置
position_y_ = datafile(:,7);      %参考位置
s_ = datafile(:,8);               %参考里程
current_x_ = datafile(:,9);                      %当前位置
current_y_ = datafile(:,10);                      %当前位置
current_accelerate_ = datafile(:,11);        %当前加速度
current_speed_ = datafile(:,12);             %当前速度
current_pitch_ = datafile(:,13);             %当前俯仰角
current_gear = datafile(:,14);%当前档位
current_wheel_rpm_ = datafile(:,15);         %当前轮速 r/min
current_engine_rpm_ = datafile(:,16);        %当前发动机转速
f_wind = datafile(:,17);                           %风阻
f_slope = datafile(:,18);                          %坡阻
f_roll = datafile(:,19);                           %滚阻
f_accel = datafile(:,20);                          %加速阻力
f_drive = datafile(:,21);                          %需求驱动力
t_wheel = datafile(:,22);                          %需求轮子力矩
t_engine = datafile(:,23);                         %需求发动机力矩
current_wheel_rpm_rate_ = datafile(:,24);          %轮子转速率
current_engine_rpm_rate_ = datafile(:,25);          %轮子转速率
cur_eng_toqure = datafile(:,26); 

len = size(t_engine);
amt_rate = [12.158,9.435,7.349,5.77,4.464,3.493,2.724,2.114,1.646,1.293,1,0.783];
main_rate = 3.727;
% for i=1:1:len
% if current_gear(i) == 0
%     current_gear(i) = 1;
% end;
% % calc_t_wheel(i) = t_engine(i)*main_rate*amt_rate(current_gear(i));
% %calc_t_wheel = t_engine*main_rate*12.158;
% if t_engine(i) < -50  
%     t_engine(i) = -50;
% end;
% if current_gear(i) > 10  
%     current_gear(i) = 10;
% end;
% end;

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
% hold on;
% grid on;
% plot(t_wheel,'b');
% plot(t_engine,'r');
% plot(current_wheel_rpm_rate_,'m');
% plot(current_engine_rpm_rate_,'k');
% plot(current_gear*100,'c');
% legend('t_wheel','t_engine','current_wheel_rpm_rate_','current_engine_rpm_rate_','current_gear');
% 
% figure;
% hold on;
% grid on;
% plot(tar_speed_,'b');
% plot(current_speed_,'r');
% plot(tar_accel_speed_,'m');
% plot(current_accelerate_,'k');
% legend('tar_speed_','current_speed_','tar_accel_speed_','current_accelerate_');

figure;
 hold on;
 grid on;
plot(current_pitch_,'b');

figure;
hold on;
grid on;
subplot(2,1,1);
title('speed');
hold on;
grid on;
plot(tar_speed_,'b');
plot(current_speed_,'r');
plot(tar_accel_speed_,'k');
plot(current_accelerate_,'m');
plot(current_gear,'c');
legend('tar_speed_','current_speed_','tar_accel_speed_','current_accelerate_','current_gear');
subplot(2,1,2);
title('torque');
hold on;
grid on;
 plot(current_gear*100,'m');
 plot(f_drive,'c');
 plot(t_engine,'g');%
 plot(current_engine_rpm_,'k');
plot(cur_eng_toqure*10,'r');
plot(acc_value_*10,'b');
legend('current_gear','f_drive','t_engine','current_engine_rpm_','cur_eng_toqure','acc_value_');
%plot(acc_driving_mode_,'.');

% figure;
% hold on;
% grid on;
% plot(t_wheel,'b');
% plot(t_engine,'r');
% for i=1:1:len
% if current_gear(i) == 0
%     current_gear(i) = 1;
% end;
%  current_wheel_rpm_(i) = current_wheel_rpm_(i)*main_rate*amt_rate(current_gear(i));
% %calc_t_wheel = t_engine*main_rate*12.158;
% end;

%plot(current_wheel_rpm_,'m');
%plot(current_engine_rpm_,'k');
%plot(current_gear*100,'c');
%legend('t_wheel','t_engine','current_wheel_rpm_','current_engine_rpm_','current_gear');


