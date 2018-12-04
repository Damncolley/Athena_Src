g++ -Wall -std=c++11 -g -shared -fPIC  -c "/home/alex/Desktop/git projects/athena_control.git/controller_lib/common/cputime.cpp" -o obj/Debug/common/cputime.o
g++ -Wall -std=c++11 -g -shared -fPIC  -c "/home/alex/Desktop/git projects/athena_control.git/controller_lib/common/kalman_filter.cpp" -o obj/Debug/common/kalman_filter.o
g++ -Wall -std=c++11 -g -shared -fPIC  -c "/home/alex/Desktop/git projects/athena_control.git/controller_lib/common/kalman_filter_initial.cpp" -o obj/Debug/common/kalman_filter_initial.o
g++ -Wall -std=c++11 -g -shared -fPIC  -c "/home/alex/Desktop/git projects/athena_control.git/controller_lib/common/map_matching/circle.cpp" -o obj/Debug/common/map_matching/circle.o
g++ -Wall -std=c++11 -g -shared -fPIC  -c "/home/alex/Desktop/git projects/athena_control.git/controller_lib/common/map_matching/coordinate_transformation.cpp" -o obj/Debug/common/map_matching/coordinate_transformation.o
g++ -Wall -std=c++11 -g -shared -fPIC  -c "/home/alex/Desktop/git projects/athena_control.git/controller_lib/common/map_matching/cs.cpp" -o obj/Debug/common/map_matching/cs.o
g++ -Wall -std=c++11 -g -shared -fPIC  -c "/home/alex/Desktop/git projects/athena_control.git/controller_lib/common/map_matching/heading.cpp" -o obj/Debug/common/map_matching/heading.o
g++ -Wall -std=c++11 -g -shared -fPIC  -c "/home/alex/Desktop/git projects/athena_control.git/controller_lib/common/map_matching/map_matching.cpp" -o obj/Debug/common/map_matching/map_matching.o
g++ -Wall -std=c++11 -g -shared -fPIC  -c "/home/alex/Desktop/git projects/athena_control.git/controller_lib/common/map_matching/point.cpp" -o obj/Debug/common/map_matching/point.o
g++ -Wall -std=c++11 -g -shared -fPIC  -c "/home/alex/Desktop/git projects/athena_control.git/controller_lib/common/map_matching/spline.cpp" -o obj/Debug/common/map_matching/spline.o
g++ -Wall -std=c++11 -g -shared -fPIC  -c "/home/alex/Desktop/git projects/athena_control.git/controller_lib/common/map_matching/steering_angle.cpp" -o obj/Debug/common/map_matching/steering_angle.o
g++ -Wall -std=c++11 -g -shared -fPIC  -c "/home/alex/Desktop/git projects/athena_control.git/controller_lib/common/navi_point.cpp" -o obj/Debug/common/navi_point.o
g++ -Wall -std=c++11 -g -shared -fPIC  -c "/home/alex/Desktop/git projects/athena_control.git/controller_lib/common/path.cpp" -o obj/Debug/common/path.o
g++ -Wall -std=c++11 -g -shared -fPIC  -c "/home/alex/Desktop/git projects/athena_control.git/controller_lib/controller.cpp" -o obj/Debug/controller.o
g++ -Wall -std=c++11 -g -shared -fPIC  -c "/home/alex/Desktop/git projects/athena_control.git/controller_lib/controller_agent .cpp" -o "obj/Debug/controller_agent .o"
g++ -Wall -std=c++11 -g -shared -fPIC  -c "/home/alex/Desktop/git projects/athena_control.git/controller_lib/controller_output_alarm.cpp" -o obj/Debug/controller_output_alarm.o
g++ -Wall -std=c++11 -g -shared -fPIC  -c "/home/alex/Desktop/git projects/athena_control.git/controller_lib/lat_controller/lat_controller.cpp" -o obj/Debug/lat_controller/lat_controller.o
g++ -Wall -std=c++11 -g -shared -fPIC  -c "/home/alex/Desktop/git projects/athena_control.git/controller_lib/lon_controller/lon_controller.cpp" -o obj/Debug/lon_controller/lon_controller.o
g++ -Wall -std=c++11 -g -shared -fPIC  -c "/home/alex/Desktop/git projects/athena_control.git/controller_lib/lon_controller/vehicle_dynamics .cpp" -o "obj/Debug/lon_controller/vehicle_dynamics .o"
g++ -Wall -std=c++11 -g -shared -fPIC  -c "/home/alex/Desktop/git projects/athena_control.git/controller_lib/pid/pid_controller.cpp" -o obj/Debug/pid/pid_controller.o
g++ -shared  obj/Debug/common/cputime.o obj/Debug/common/kalman_filter.o obj/Debug/common/kalman_filter_initial.o obj/Debug/common/map_matching/circle.o obj/Debug/common/map_matching/coordinate_transformation.o obj/Debug/common/map_matching/cs.o obj/Debug/common/map_matching/heading.o obj/Debug/common/map_matching/map_matching.o obj/Debug/common/map_matching/point.o obj/Debug/common/map_matching/spline.o obj/Debug/common/map_matching/steering_angle.o obj/Debug/common/navi_point.o obj/Debug/common/path.o obj/Debug/controller.o "obj/Debug/controller_agent .o" obj/Debug/controller_output_alarm.o obj/Debug/lat_controller/lat_controller.o obj/Debug/lon_controller/lon_controller.o "obj/Debug/lon_controller/vehicle_dynamics .o" obj/Debug/pid/pid_controller.o  -o ../config/libcontroller_d.so  

g++ -Wall -std=c++11 -g -Icontroller_lib -c "/home/alex/Desktop/git projects/athena_control.git/apps/control.cpp" -o obj/Debug/apps/control.o
g++ -Wall -std=c++11 -g -Icontroller_lib -c "/home/alex/Desktop/git projects/athena_control.git/apps/control_view/control_view.cpp" -o obj/Debug/apps/control_view/control_view.o
g++ -Wall -std=c++11 -g -Icontroller_lib -c "/home/alex/Desktop/git projects/athena_control.git/apps/main.cpp" -o obj/Debug/apps/main.o
g++ -Wall -std=c++11 -g -Icontroller_lib -c "/home/alex/Desktop/git projects/athena_control.git/apps/message_manger/lcm/lcm_message_manger.cpp" -o obj/Debug/apps/message_manger/lcm/lcm_message_manger.o
g++ -Wall -std=c++11 -g -Icontroller_lib -c "/home/alex/Desktop/git projects/athena_control.git/apps/track_trajectory/track_trajectory.cpp" -o obj/Debug/apps/track_trajectory/track_trajectory.o
g++ -Wall -std=c++11 -g -Icontroller_lib -c "/home/alex/Desktop/git projects/athena_control.git/common/Config.cpp" -o obj/Debug/common/Config.o
g++ -Wall -std=c++11 -g -Icontroller_lib -c "/home/alex/Desktop/git projects/athena_control.git/common/get_time.cpp" -o obj/Debug/common/get_time.o
g++ -Wall -std=c++11 -g -Icontroller_lib -c "/home/alex/Desktop/git projects/athena_control.git/common/logging.cpp" -o obj/Debug/common/logging.o
g++ -Wall -std=c++11 -g -Icontroller_lib -c "/home/alex/Desktop/git projects/athena_control.git/common/Thread.cpp" -o obj/Debug/common/Thread.o
g++ -Wall -std=c++11 -g -Icontroller_lib -c "/home/alex/Desktop/git projects/athena_control.git/control_logic/acc/cs55/cs55_torque_speed_throttle_map.cpp" -o obj/Debug/control_logic/acc/cs55/cs55_torque_speed_throttle_map.o
g++ -Wall -std=c++11 -g -Icontroller_lib -c "/home/alex/Desktop/git projects/athena_control.git/control_logic/brake/cs55/cs55_deceleration_brake_map.cpp" -o obj/Debug/control_logic/brake/cs55/cs55_deceleration_brake_map.o
g++ -Wall -std=c++11 -g -Icontroller_lib -c "/home/alex/Desktop/git projects/athena_control.git/control_logic/control_logic.cpp" -o obj/Debug/control_logic/control_logic.o
g++ -Wall -std=c++11 -g -Icontroller_lib -c "/home/alex/Desktop/git projects/athena_control.git/control_logic/gear/cs55/cs55_gear_control.cpp" -o obj/Debug/control_logic/gear/cs55/cs55_gear_control.o
g++  -o control obj/Debug/apps/control.o obj/Debug/apps/control_view/control_view.o obj/Debug/apps/main.o obj/Debug/apps/message_manger/lcm/lcm_message_manger.o obj/Debug/apps/track_trajectory/track_trajectory.o obj/Debug/common/Config.o obj/Debug/common/get_time.o obj/Debug/common/logging.o obj/Debug/common/Thread.o obj/Debug/control_logic/acc/cs55/cs55_torque_speed_throttle_map.o obj/Debug/control_logic/brake/cs55/cs55_deceleration_brake_map.o obj/Debug/control_logic/control_logic.o obj/Debug/control_logic/gear/cs55/cs55_gear_control.o   -llcm -lglog -lpthread -lglut -lGLU -lGL config/libcontroller_d.so


