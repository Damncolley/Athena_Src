/*-------------------------------------------------------
 * 文件名：nad_msg.h
 * 创建者：张毅00151602
 * 时  间：2016-03-02
 * 描  述：引用了msg目录下的所有头文件
-------------------------------------------------------*/
#ifndef _NAD_MSG_H
#define _NAD_MSG_H

//网元内消息
#include <lcm/lcm-cpp.hpp>

//网元间消息
#include "ne_msg/ne_lcm.hpp"

//网络侧lcm消息列表
using namespace nad_lcm;

//定时器
#include "timer/nad_timer.h"

//SESSION(控制块)
#include "session/nad_session.h"

//ZeroMQ，注：hpp文件中有static函数，所以不默认包含
//#include "zmq/zmq_t.h"

//用于发消息
extern NE_LCM *g_lcm;


//nad_lcm中包含的消息文件
#include "nad_lcm/block_info.hpp"
#include "nad_lcm/center_line.hpp"
#include "nad_lcm/center_point.hpp"
#include "nad_lcm/lite_center_point.hpp"
#include "nad_lcm/co_rsu_name_respond.hpp"
#include "nad_lcm/cr_add_ets_request.hpp"
#include "nad_lcm/cr_add_platoon_request.hpp"
#include "nad_lcm/cr_delete_ets_request.hpp"
#include "nad_lcm/cr_delete_platoon_request.hpp"
#include "nad_lcm/cr_exec_task_func_request.hpp"
#include "nad_lcm/cr_info_report.hpp"
#include "nad_lcm/cr_obu_login_respond.hpp"
#include "nad_lcm/cr_obu_logout_notify.hpp"
#include "nad_lcm/cr_route_respond.hpp"
#include "nad_lcm/cr_rsu_login_respond.hpp"
#include "nad_lcm/cr_rsu_logout_notify.hpp"
#include "nad_lcm/cr_set_ets_request.hpp"
#include "nad_lcm/cr_set_platoon_request.hpp"
#include "nad_lcm/cr_start_auto_respond.hpp"
#include "nad_lcm/cu_add_ets_respond.hpp"
#include "nad_lcm/cu_add_platoon_respond.hpp"
#include "nad_lcm/cu_alarm_report.hpp"
#include "nad_lcm/cu_config_respond.hpp"
#include "nad_lcm/cu_delete_ets_respond.hpp"
#include "nad_lcm/cu_delete_platoon_notify.hpp"
#include "nad_lcm/cu_exec_task_respond.hpp"
#include "nad_lcm/cu_info_report.hpp"
#include "nad_lcm/cu_log_report.hpp"
#include "nad_lcm/cu_oct_login_respond.hpp"
#include "nad_lcm/cu_set_ets_respond.hpp"
#include "nad_lcm/cu_set_platoon_respond.hpp"
#include "nad_lcm/cu_stop_auto_notify.hpp"
#include "nad_lcm/cu_stop_task_respond.hpp"
#include "nad_lcm/dr_info_report.hpp"
#include "nad_lcm/key_point.hpp"
#include "nad_lcm/lane_of_route.hpp"
#include "nad_lcm/light_info.hpp"
#include "nad_lcm/limspeed_info.hpp"
#include "nad_lcm/map_point.hpp"
#include "nad_lcm/obstacle_info.hpp"
#include "nad_lcm/mo_change_lane_request.hpp"
#include "nad_lcm/obu_command.hpp"
#include "nad_lcm/obu_config.hpp"
#include "nad_lcm/obu_info.hpp"
#include "nad_lcm/oc_rsu_name_request.hpp"
#include "nad_lcm/or_change_lane_request.hpp"
#include "nad_lcm/or_info_report.hpp"
#include "nad_lcm/or_obu_login_request.hpp"
#include "nad_lcm/or_route_request.hpp"
#include "nad_lcm/or_start_auto_request.hpp"
#include "nad_lcm/or_stop_auto_notify.hpp"
#include "nad_lcm/om_change_lane_respond.hpp"
#include "nad_lcm/ou_add_platoon_notify.hpp"
#include "nad_lcm/ou_alarm_report.hpp"
#include "nad_lcm/ou_delete_platoon_notify.hpp"
#include "nad_lcm/ou_log_report.hpp"
#include "nad_lcm/ou_route_respond.hpp"
#include "nad_lcm/ou_set_platoon_notify.hpp"
#include "nad_lcm/ou_start_auto_respond.hpp"
#include "nad_lcm/ou_stop_auto_respond.hpp"
#include "nad_lcm/platoon_info.hpp"
#include "nad_lcm/rc_add_ets_respond.hpp"
#include "nad_lcm/rc_add_platoon_respond.hpp"
#include "nad_lcm/rc_alarm_report.hpp"
#include "nad_lcm/rc_delete_ets_respond.hpp"
#include "nad_lcm/rc_delete_platoon_notify.hpp"
#include "nad_lcm/rc_exec_task_func_respond.hpp"
#include "nad_lcm/rc_info_report.hpp"
#include "nad_lcm/rc_log_report.hpp"
#include "nad_lcm/rc_obu_login_request.hpp"
#include "nad_lcm/rc_obu_logout_notify.hpp"
#include "nad_lcm/rc_route_request.hpp"
#include "nad_lcm/rc_route_respond.hpp"
#include "nad_lcm/rc_rsu_login_request.hpp"
#include "nad_lcm/rc_set_ets_respond.hpp"
#include "nad_lcm/rc_set_platoon_respond.hpp"
#include "nad_lcm/rc_start_auto_request.hpp"
#include "nad_lcm/rc_stop_auto_notify.hpp"
#include "nad_lcm/route_planning.hpp"
#include "nad_lcm/ro_add_platoon_notify.hpp"
#include "nad_lcm/ro_alarm_report.hpp"
#include "nad_lcm/ro_change_lane_respond.hpp"
#include "nad_lcm/ro_delete_platoon_notify.hpp"
#include "nad_lcm/ro_info_report.hpp"
#include "nad_lcm/ro_log_report.hpp"
#include "nad_lcm/ro_obu_login_respond.hpp"
#include "nad_lcm/ro_obu_logout_notify.hpp"
#include "nad_lcm/ro_route_respond.hpp"
#include "nad_lcm/ro_set_platoon_notify.hpp"
#include "nad_lcm/ro_start_auto_respond.hpp"
#include "nad_lcm/ro_stop_auto_respond.hpp"
#include "nad_lcm/rsd_config.hpp"
#include "nad_lcm/rsd_info.hpp"
#include "nad_lcm/rsd_sensor_info.hpp"
#include "nad_lcm/rsu_config.hpp"
#include "nad_lcm/rsu_info.hpp"
#include "nad_lcm/sensor_obstacle_report.hpp"
#include "nad_lcm/task_config.hpp"
#include "nad_lcm/task_info.hpp"
#include "nad_lcm/uc_add_ets_request.hpp"
#include "nad_lcm/uc_add_platoon_request.hpp"
#include "nad_lcm/uc_config_request.hpp"
#include "nad_lcm/uc_delete_ets_request.hpp"
#include "nad_lcm/uc_delete_platoon_request.hpp"
#include "nad_lcm/uc_exec_task_request.hpp"
#include "nad_lcm/uc_oct_login_request.hpp"
#include "nad_lcm/uc_set_ets_request.hpp"
#include "nad_lcm/uc_set_platoon_request.hpp"
#include "nad_lcm/uc_stop_task_request.hpp"
#include "nad_lcm/uo_route_request.hpp"
#include "nad_lcm/uo_start_auto_request.hpp"
#include "nad_lcm/uo_stop_auto_request.hpp"
#include "nad_lcm/uc_call_car_request.hpp"
#include "nad_lcm/cu_call_car_respond.hpp"
#include "nad_lcm/route_line_point.hpp"
#include "nad_lcm/om_center_line_report.hpp"
#include "nad_lcm/om_info_report.hpp"
#include "nad_lcm/ro_vui_report.hpp"
#include "nad_lcm/ou_vui_report.hpp"
#include "nad_lcm/uo_upcall_request.hpp"
#include "nad_lcm/or_upcall_request.hpp"
#include "nad_lcm/rc_upcall_request.hpp"
#include "nad_lcm/co_obu_info.hpp"
#include "nad_lcm/key_point_info.hpp"
#include "nad_lcm/qos_info.hpp"
#include "nad_lcm/ro_sensor_on_off.hpp"
#include "nad_lcm/os_sensor_on_off.hpp"
#include "nad_lcm/ro_qos_request.hpp"
#include "nad_lcm/or_qos_respond.hpp"
#include "nad_lcm/ne_info.hpp"
#include "nad_lcm/mo_degrade_request.hpp"
#include "nad_lcm/or_degrade_request.hpp"
#include "nad_lcm/line_xys.hpp"
#include "nad_lcm/point_m.hpp"
#include "nad_lcm/point_xys.hpp"
#include "nad_lcm/route_planning_m.hpp"
#include "nad_lcm/om_route_respond.hpp"

#include "nad_lcm/uc_info_report.hpp"
#include "nad_lcm/cu_call_park_info_report.hpp"//oct 召车泊车
#include "nad_lcm/uc_call_park_car_request.hpp"//oct 召车泊车
#include "nad_lcm/cu_call_park_car_respond.hpp"//oct 召车泊车


#include "nad_lcm/oc_vui_report.hpp"

//obu_lcm包含的消息列表
#include "obu_lcm/control_info_report.hpp"
#include "obu_lcm/nav_points.hpp"
#include "obu_lcm/lateral_control_info.hpp"
#include "obu_lcm/lateral_control_vui_info.hpp"
#include "obu_lcm/esr_data_list.hpp"
#include "obu_lcm/esr_data_t.hpp"
#include "obu_lcm/ins_info.hpp"
#include "obu_lcm/CAN_value.hpp"
#include "obu_lcm/back_coordinate_XYH.hpp"
#include "obu_lcm/mt_info_report.hpp"
//构造消息调试信息
string log_rc_rsu_login_request(     const nad_lcm::ne_msg_t<nad_lcm::rc_rsu_login_request     > *msg);
string log_cr_rsu_login_respond(     const nad_lcm::ne_msg_t<nad_lcm::cr_rsu_login_respond     > *msg);
string log_cr_rsu_logout_notify(     const nad_lcm::ne_msg_t<nad_lcm::cr_rsu_logout_notify     > *msg);
string log_oc_rsu_name_request(      const nad_lcm::ne_msg_t<nad_lcm::oc_rsu_name_request      > *msg);
string log_co_rsu_name_respond(      const nad_lcm::ne_msg_t<nad_lcm::co_rsu_name_respond      > *msg);
string log_or_obu_login_request(     const nad_lcm::ne_msg_t<nad_lcm::or_obu_login_request     > *msg);
string log_rc_obu_login_request(     const nad_lcm::ne_msg_t<nad_lcm::rc_obu_login_request     > *msg);
string log_cr_obu_login_respond(     const nad_lcm::ne_msg_t<nad_lcm::cr_obu_login_respond     > *msg);
string log_ro_obu_login_respond(     const nad_lcm::ne_msg_t<nad_lcm::ro_obu_login_respond     > *msg);
string log_cr_obu_logout_notify(     const nad_lcm::ne_msg_t<nad_lcm::cr_obu_logout_notify     > *msg);
string log_rc_obu_logout_notify(     const nad_lcm::ne_msg_t<nad_lcm::rc_obu_logout_notify     > *msg);
string log_ro_obu_logout_notify(     const nad_lcm::ne_msg_t<nad_lcm::ro_obu_logout_notify     > *msg);
string log_uo_route_request(         const                   nad_lcm::uo_route_request           *msg);
string log_or_route_request(         const nad_lcm::ne_msg_t<nad_lcm::or_route_request         > *msg);
string log_rc_route_request(         const nad_lcm::ne_msg_t<nad_lcm::rc_route_request         > *msg);
string log_cr_route_respond(         const nad_lcm::ne_msg_t<nad_lcm::cr_route_respond         > *msg);
string log_ro_route_respond(         const nad_lcm::ne_msg_t<nad_lcm::ro_route_respond         > *msg);
string log_rc_route_respond(         const nad_lcm::ne_msg_t<nad_lcm::rc_route_respond         > *msg);
string log_ou_route_respond(         const                   nad_lcm::ou_route_respond           *msg);
string log_uo_start_auto_request(    const                   nad_lcm::uo_start_auto_request      *msg);
string log_or_start_auto_request(    const nad_lcm::ne_msg_t<nad_lcm::or_start_auto_request    > *msg);
string log_rc_start_auto_request(    const nad_lcm::ne_msg_t<nad_lcm::rc_start_auto_request    > *msg);
string log_cr_start_auto_respond(    const nad_lcm::ne_msg_t<nad_lcm::cr_start_auto_respond    > *msg);
string log_ro_start_auto_respond(    const nad_lcm::ne_msg_t<nad_lcm::ro_start_auto_respond    > *msg);
string log_ou_start_auto_respond(    const                   nad_lcm::ou_start_auto_respond      *msg);
string log_uo_stop_auto_request(     const                   nad_lcm::uo_stop_auto_request       *msg);
string log_or_stop_auto_notify(      const nad_lcm::ne_msg_t<nad_lcm::or_stop_auto_notify      > *msg);
string log_rc_stop_auto_notify(      const nad_lcm::ne_msg_t<nad_lcm::rc_stop_auto_notify      > *msg);
string log_cu_stop_auto_notify(      const nad_lcm::ne_msg_t<nad_lcm::cu_stop_auto_notify      > *msg);
string log_ro_stop_auto_respond(     const nad_lcm::ne_msg_t<nad_lcm::ro_stop_auto_respond     > *msg);
string log_ou_stop_auto_respond(     const                   nad_lcm::ou_stop_auto_respond       *msg);
string log_dr_info_report(           const nad_lcm::ne_msg_t<nad_lcm::dr_info_report           > *msg);
string log_or_info_report(           const nad_lcm::ne_msg_t<nad_lcm::or_info_report           > *msg);
string log_rc_info_report(           const nad_lcm::ne_msg_t<nad_lcm::rc_info_report           > *msg);
string log_cu_info_report(           const nad_lcm::ne_msg_t<nad_lcm::cu_info_report           > *msg);
string log_cr_info_report(           const nad_lcm::ne_msg_t<nad_lcm::cr_info_report           > *msg);
string log_mo_change_lane_request(   const                   nad_lcm::mo_change_lane_request     *msg);
string log_om_center_line_report(   const                     nad_lcm::om_center_line_report     *msg);
string log_or_change_lane_request(   const nad_lcm::ne_msg_t<nad_lcm::or_change_lane_request   > *msg);
string log_ro_change_lane_respond(   const nad_lcm::ne_msg_t<nad_lcm::ro_change_lane_respond   > *msg);
string log_om_change_lane_respond(   const                   nad_lcm::om_change_lane_respond     *msg);
string log_uc_add_platoon_request(   const                   nad_lcm::uc_add_platoon_request     *msg);
string log_cr_add_platoon_request(   const nad_lcm::ne_msg_t<nad_lcm::cr_add_platoon_request   > *msg);
string log_ro_add_platoon_notify(    const nad_lcm::ne_msg_t<nad_lcm::ro_add_platoon_notify    > *msg);
string log_ou_add_platoon_notify(    const                   nad_lcm::ou_add_platoon_notify      *msg);
string log_rc_add_platoon_respond(   const nad_lcm::ne_msg_t<nad_lcm::rc_add_platoon_respond   > *msg);
string log_cu_add_platoon_respond(   const nad_lcm::ne_msg_t<nad_lcm::cu_add_platoon_respond   > *msg);
string log_uc_set_platoon_request(   const                   nad_lcm::uc_set_platoon_request     *msg);
string log_cr_set_platoon_request(   const nad_lcm::ne_msg_t<nad_lcm::cr_set_platoon_request   > *msg);
string log_ro_set_platoon_notify(    const nad_lcm::ne_msg_t<nad_lcm::ro_set_platoon_notify    > *msg);
string log_ou_set_platoon_notify(    const                   nad_lcm::ou_set_platoon_notify      *msg);
string log_rc_set_platoon_respond(   const nad_lcm::ne_msg_t<nad_lcm::rc_set_platoon_respond   > *msg);
string log_cu_set_platoon_respond(   const nad_lcm::ne_msg_t<nad_lcm::cu_set_platoon_respond   > *msg);
string log_uc_delete_platoon_request(const                   nad_lcm::uc_delete_platoon_request  *msg);
string log_cr_delete_platoon_request(const nad_lcm::ne_msg_t<nad_lcm::cr_delete_platoon_request> *msg);
string log_rc_delete_platoon_notify( const nad_lcm::ne_msg_t<nad_lcm::rc_delete_platoon_notify > *msg);
string log_cu_delete_platoon_notify( const nad_lcm::ne_msg_t<nad_lcm::cu_delete_platoon_notify > *msg);
string log_ro_delete_platoon_notify( const nad_lcm::ne_msg_t<nad_lcm::ro_delete_platoon_notify > *msg);
string log_ou_delete_platoon_notify( const                   nad_lcm::ou_delete_platoon_notify   *msg);
string log_uc_oct_login_request(     const                   nad_lcm::uc_oct_login_request       *msg);
string log_cu_oct_login_respond(     const nad_lcm::ne_msg_t<nad_lcm::cu_oct_login_respond     > *msg);
string log_uc_config_request(        const                   nad_lcm::uc_config_request          *msg);
string log_cu_config_respond(        const nad_lcm::ne_msg_t<nad_lcm::cu_config_respond        > *msg);
string log_uc_exec_task_request(     const                   nad_lcm::uc_exec_task_request       *msg);
string log_cu_exec_task_respond(     const nad_lcm::ne_msg_t<nad_lcm::cu_exec_task_respond     > *msg);
string log_uc_stop_task_request(     const                   nad_lcm::uc_stop_task_request       *msg);
string log_cu_stop_task_respond(     const nad_lcm::ne_msg_t<nad_lcm::cu_stop_task_respond     > *msg);
string log_cr_exec_task_func_request(const nad_lcm::ne_msg_t<nad_lcm::cr_exec_task_func_request> *msg);
string log_rc_exec_task_func_respond(const nad_lcm::ne_msg_t<nad_lcm::rc_exec_task_func_respond> *msg);
string log_uc_set_ets_request(       const                   nad_lcm::uc_set_ets_request         *msg);
string log_cr_set_ets_request(       const nad_lcm::ne_msg_t<nad_lcm::cr_set_ets_request       > *msg);
string log_rc_set_ets_respond(       const nad_lcm::ne_msg_t<nad_lcm::rc_set_ets_respond       > *msg);
string log_cu_set_ets_respond(       const nad_lcm::ne_msg_t<nad_lcm::cu_set_ets_respond       > *msg);
string log_uc_add_ets_request(       const                   nad_lcm::uc_add_ets_request         *msg);
string log_cr_add_ets_request(       const nad_lcm::ne_msg_t<nad_lcm::cr_add_ets_request       > *msg);
string log_rc_add_ets_respond(       const nad_lcm::ne_msg_t<nad_lcm::rc_add_ets_respond       > *msg);
string log_cu_add_ets_respond(       const nad_lcm::ne_msg_t<nad_lcm::cu_add_ets_respond       > *msg);
string log_uc_delete_ets_request(    const                   nad_lcm::uc_delete_ets_request      *msg);
string log_cr_delete_ets_request(    const nad_lcm::ne_msg_t<nad_lcm::cr_delete_ets_request    > *msg);
string log_rc_delete_ets_respond(    const nad_lcm::ne_msg_t<nad_lcm::rc_delete_ets_respond    > *msg);
string log_cu_delete_ets_respond(    const nad_lcm::ne_msg_t<nad_lcm::cu_delete_ets_respond    > *msg);
string log_rc_log_report(            const nad_lcm::ne_msg_t<nad_lcm::rc_log_report            > *msg);
string log_cu_log_report(            const nad_lcm::ne_msg_t<nad_lcm::cu_log_report            > *msg);
string log_rc_alarm_report(          const nad_lcm::ne_msg_t<nad_lcm::rc_alarm_report          > *msg);
string log_cu_alarm_report(          const nad_lcm::ne_msg_t<nad_lcm::cu_alarm_report          > *msg);
string log_ro_log_report(            const nad_lcm::ne_msg_t<nad_lcm::ro_log_report            > *msg);
string log_ou_log_report(            const                   nad_lcm::ou_log_report              *msg);
string log_ro_alarm_report(          const nad_lcm::ne_msg_t<nad_lcm::ro_alarm_report          > *msg);
string log_ou_alarm_report(          const                   nad_lcm::ou_alarm_report            *msg);
string log_sensor_obstacle_report(   const                   nad_lcm::sensor_obstacle_report     *msg);
string log_uc_call_car_request(      const                   nad_lcm::uc_call_car_request        *msg);
string log_cu_call_car_respond(      const nad_lcm::ne_msg_t<nad_lcm::cu_call_car_respond      > *msg);
string log_uo_upcall_request(        const                   nad_lcm::uo_upcall_request          *msg);
string log_or_upcall_request(        const nad_lcm::ne_msg_t<nad_lcm::or_upcall_request        > *msg);
string log_rc_upcall_request(        const nad_lcm::ne_msg_t<nad_lcm::rc_upcall_request        > *msg);

//oct新增消息
string log_uc_call_park_car_request(     const                   nad_lcm::uc_call_park_car_request   *msg);
string log_cu_call_park_info_report(     const nad_lcm::ne_msg_t<nad_lcm::cu_call_park_info_report > *msg);
string log_cu_call_park_car_respond(     const nad_lcm::ne_msg_t<nad_lcm::cu_call_park_car_respond > *msg);

#if defined(_NAD_CSU_)
    #define MSG_CAP "CSU"
#endif
#if defined(_NAD_RSU_)
    #define MSG_CAP "RSU"
#endif
#if defined(_NAD_OBU_)
    #define MSG_CAP "OBU"
#endif
#if defined(_NAD_RSD_)
    #define MSG_CAP "RSD"
#endif
#if defined(_NAD_SIM_)
    #define MSG_CAP "SIM"
#endif

/* //写日志的输出法
#define LOG_RECV(str) LOG(WARNING) << MSG_CAP << "_recv: " << str
#define LOG_SEND(str) LOG(ERROR)   << MSG_CAP << "_send: " << str
*/

//不写日志的输出法
#define LOG_RECV(str) std::cout << endl << "\e[0;32m" << MSG_CAP << "_recv: " << str << "\e[0m" << endl
#define LOG_SEND(str) std::cout << endl << "\e[0;34m" << MSG_CAP << "_send: " << str << "\e[0m" << endl


#endif
