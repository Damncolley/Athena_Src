#ifndef _PLANNING_NODE_H
#define _PLANNING_NODE_H

#include <string>
#include <iostream>
#include <unistd.h>

#include <lcm/lcm.h>
#include <lcm/lcm-cpp.hpp>

#include <thread>
#include <pthread.h>
#include <sys/sem.h>


#include "planning_lcm_msg.h"
#include "planning/planning.h"
#include "common/enum_list.h"

using namespace std;

class PlanningNode
{
public:
    PlanningNode();
    ~PlanningNode();

    void run();
    int park_planning( athena::planning::Planning &planning, double view_step, bool is_col_last_d, bool is_col_last_r );

    int publish_trajectory( OutTrajectory ot );
    int publish_view_path( path out_path, double step ); ///step:发送点的间隔，单位：米
    int publish_view_path( OutTrajectory out_traj, double step ); ///step:发送点的间隔，单位：米
    int publish_alarm( int32_t alarm_level, string alarm );


private:
    /// variables
    string planning_url_;

    ///input
    route_planning_m route_;
    CarState car_state_;
    TrafficLights traffic_lights_;
    StopPark stop_park_;
    SensorObstacles sensor_obstacles_;

    int hand_direction_;
    int last_hand_direction_;

    ///flag
     bool is_route_set_;

    ///class
    athena::planning::Planning planning_;

    ///thread function
    void* ThreadFunction_ins();
    void* ThreadFunction_can();
    void* ThreadFunction_route();
    void* ThreadFunction_map();
    void* ThreadFunction_obstacle();


    ///lcm msg
    lcm::LCM* lcm_ins_;
    lcm::LCM* lcm_can_;
    lcm::LCM* lcm_route_;
    lcm::LCM* lcm_map_;
    lcm::LCM* lcm_obstacle_;
    lcm::LCM* lcm_publish_;
    ///callback function
    //ins
    void handle_ins_Message(
        const lcm::ReceiveBuffer* rbuf,
        const std::string& chan,
        const obu_lcm::ins_info * msg);
    //can
    void handle_can_value_Message(
        const lcm::ReceiveBuffer* rbuf,
        const std::string& chan,
        const obu_lcm::CAN_value* msg);
    //can
    void handle_can_status_Message(
        const lcm::ReceiveBuffer* rbuf,
        const std::string& chan,
        const obu_lcm::CAN_status* msg);
    //can
    void handle_vehicle_info_Message(
        const lcm::ReceiveBuffer* rbuf,
        const std::string& chan,
        const obu_lcm::vehicle_info* msg);
    //can
    void handle_chassis_detail_Message(
        const lcm::ReceiveBuffer* rbuf,
        const std::string& chan,
        const obu_lcm::ChassisDetail* msg);
    //can
//    void handle_steering_Message(
//        const lcm::ReceiveBuffer* rbuf,
//        const std::string& chan,
//        const obu_lcm::steering_feedback_info* msg);
    //route
    void handle_route_info(
        const lcm::ReceiveBuffer* rbuf,
        const std::string& chan,
        const nad_lcm::om_route_respond* msg);
    //crossing & lights
    void handle_traffic_lights_info(
        const lcm::ReceiveBuffer* rbuf,
        const std::string& chan,
        const nad_lcm::om_traffic_lights_report* msg);
    //stop point
    void handle_back_coordinate_XYH(
        const lcm::ReceiveBuffer* rbuf,
        const std::string& chan,
        const obu_lcm::back_coordinate_XYH* msg);
    //start
    void handle_start_respond(
        const lcm::ReceiveBuffer* rbuf,
        const std::string& chan,
        const nad_lcm::ou_start_auto_respond* msg);
    //stop
    void handle_stop_respond(
         const lcm::ReceiveBuffer* rbuf,
         const std::string& chan,
         const nad_lcm::ou_stop_auto_respond* msg);
    //obstacle
    void handle_obstacle_Message(
        const lcm::ReceiveBuffer* rbuf,
        const std::string& chan,
        const nad_lcm::sensor_obstacle_report* msg_obstacle_list);

    /// initializer
    void init();

    ///
    int get_current_ins_info( const obu_lcm::ins_info * msg );
    int get_map_info( const nad_lcm::om_route_respond* msg );
    int get_obstacles_raw( const nad_lcm::sensor_obstacle_report* msg_obstacle_list );

};

int point2msg( const navi_point& src_point, obu_lcm::nav_points* msg_p );


#endif // _PLANNING_NODE_H
