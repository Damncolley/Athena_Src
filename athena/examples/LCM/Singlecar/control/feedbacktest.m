close all;
clc;
datafile = importdata('feedback_lateral2018_11_10 11_13_9.log');

steering_angle_ = datafile(:,1);
heading_error_ = datafile(:,2);
heading_error_rate_  = datafile(:,3);
lateral_error_  = datafile(:,4);
lateral_error_rate_  = datafile(:,5);
steer_angle_lateral_contribution_  = datafile(:,6);
steer_angle_lateral_rate_contribution_ = datafile(:,7);
steer_angle_heading_contribution_ = datafile(:,8);
steer_angle_heading_rate_contribution_ = datafile(:,9);
matrix_k_0  = datafile(:,10);
matrix_k_1  = datafile(:,11);
matrix_k_2  = datafile(:,12);
matrix_k_3  = datafile(:,13);
current_speed_  = datafile(:,14);
current_x_  = datafile(:,15);
current_y_  = datafile(:,16);
current_yaw_ = datafile(:,17);
current_heading_speed_ =  datafile(:,18);

p_tar_speed_ = datafile(:,19);
p_position_x_ = datafile(:,20);
p_position_y_ = datafile(:,21);
p_heading_ = datafile(:,22);
p_heading_speed_ = datafile(:,24);

log_length = length(current_heading_speed_);
calc_heading_speed = current_heading_speed_;
for i=2:1:log_length
%  if(current_yaw_(i) > 180)
% 	current_yaw_(i) = current_yaw_(i)-360;
%  end;
	calc_heading_speed(i) = (current_yaw_(i) - current_yaw_(i-1))/0.02;
    if(abs(calc_heading_speed(i)) > 100) calc_heading_speed(i) = 0;
    end;
end;

figure;
hold on;
plot(current_x_,current_y_,'r');
plot(p_position_x_,p_position_y_,'b');
axis equal;
grid on;

figure;
hold on;
plot(steer_angle_lateral_contribution_,'b');
plot(steer_angle_lateral_rate_contribution_,'r');
plot(steer_angle_heading_contribution_,'m');
plot(steer_angle_heading_rate_contribution_,'k');
plot(steer_angle_lateral_contribution_+steer_angle_lateral_rate_contribution_+steer_angle_heading_contribution_+steer_angle_heading_rate_contribution_,'g');
plot(steering_angle_,'c');
legend('steer_angle_lateral_contribution_','steer_angle_lateral_rate_contribution_','steer_angle_heading_contribution_','steer_angle_heading_rate_contribution_','fd_out','steering_angle_');
grid on;

figure;
hold on;
grid on;
for j=2:1:log_length
  if lateral_error_rate_(j) > 100
 	lateral_error_rate_(j) = lateral_error_rate_(j-1);
  end;

  if lateral_error_(j) > 100 
 	lateral_error_(j) = lateral_error_(j-1);
  end;
end;

  plot(lateral_error_,'b');
  plot(lateral_error_rate_,'r');
   plot(heading_error_,'m');
   plot(heading_error_rate_,'k');
   plot(current_speed_,'g');
   legend('lateral_error_','lateral_error_rate_','heading_error_','heading_error_rate_','current_speed_');
  
  figure;
    hold on;
 grid on;
calc_lateral_error_ = lateral_error_;
for k=2:1:log_length
calc_lateral_error_(k) = calc_lateral_error_(k-1) + lateral_error_rate_(k)*0.02;
end;

 plot(lateral_error_,'g');
 plot(calc_lateral_error_,'b');
 plot(lateral_error_rate_,'r');
 
%    figure;
%    hold on;
% grid on;
%  plot(current_yaw_,'c');
% plot(p_heading_,'m');
%plot(current_heading_speed_,'b');
 %plot(p_heading_,'k');
 %hold on;
%  plot(p_heading_speed_*180/pi,'r');

% calc_pheading = p_heading_;
% calc_heading = current_yaw_;
% for k=2:1:log_length
% calc_pheading(k) = calc_pheading(k-1) + p_heading_speed_(k)*180/pi*0.04;
% calc_heading(k) = calc_heading(k-1) + current_heading_speed_(k)*0.04;
% end;
% 
%  plot(calc_pheading,'y');
%  plot(calc_heading,'b');
% plot(heading_error_*180/pi,'g');
%  plot(heading_error_rate_*180/pi,'m');
%  legend('current_yaw_','current_heading_speed_','p_heading_','p_heading_speed_','current_speed_','heading_error_','heading_error_rate_');




