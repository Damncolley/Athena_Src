#pragma once

#include <lcm/lcm.h>
#include <lcm/lcm-cpp.hpp>

//OBU专用lcm消息列表
#include "obu_lcm/ins_info.hpp"

//dongfeng lcm
#include "obu_lcm/CAN_status.hpp"
#include "obu_lcm/CAN_value.hpp"
#include "obu_lcm/chassis_detail.hpp"
#include "obu_lcm/vehicle_info.hpp"

//#include "obu_lcm/accelerate_control_info.hpp"
//#include "obu_lcm/accelerate_feedback_info.hpp"
//#include "obu_lcm/brake_control_info.hpp"
//#include "obu_lcm/brake_feedback_info.hpp"
//#include "obu_lcm/gears_control_info.hpp"
//#include "obu_lcm/gears_feedback_info.hpp"
//#include "obu_lcm/steering_control_info.hpp"
//#include "obu_lcm/steering_feedback_info.hpp"
//#include "obu_lcm/bcm_control_info.hpp"

//#include "obu_lcm/lateral_control_info.hpp"
#include "obu_lcm/lateral_control_vui_info.hpp"
#include "obu_lcm/nav_points.hpp"
#include "obu_lcm/longitudinal_control_info.hpp"

//#include "obu_lcm/point_t.hpp"
//#include "obu_lcm/rect_t.hpp"
//#include "obu_lcm/patch_grid.hpp"
//#include "obu_lcm/patch_t.hpp"

//new msg
#include "nad_lcm/om_traffic_lights_report.hpp"
#include "nad_lcm/section_m.hpp"
#include "nad_lcm/line_xys.hpp"
#include "nad_lcm/mo_change_lane_request.hpp"
#include "nad_lcm/mo_info_report.hpp"   //5HZ向网络层上传车辆状态
#include "nad_lcm/om_info_report.hpp"
#include "nad_lcm/point_m.hpp"
#include "nad_lcm/point_xys.hpp"
#include "nad_lcm/route_planning_m.hpp"
//#include "nad_lcm/obu_command.hpp"
#include "nad_lcm/om_change_lane_respond.hpp"
#include "nad_lcm/om_route_respond.hpp"

#include "nad_lcm/obstacle_info.hpp"
//#include "nad_lcm/obstacle_list.hpp"
#include "nad_lcm/sensor_obstacle_report.hpp"
#include "nad_lcm/mo_obstacle_report.hpp"



//#include "obu_lcm/obu_map_info.hpp"
#include "nad_lcm/ou_start_auto_respond.hpp"
#include "nad_lcm/ou_stop_auto_respond.hpp"
#include "nad_lcm/ou_alarm_report.hpp"

#include "nad_lcm/om_info_report.hpp"
#include "nad_lcm/mo_change_lane_request.hpp"
#include "nad_lcm/om_change_lane_respond.hpp"

#include "obu_lcm/mt_info_report.hpp"

//#include "nad_lcm/om_stop_request.hpp"//停车位信息
#include "obu_lcm/back_coordinate_XYH.hpp"//停车位信息

