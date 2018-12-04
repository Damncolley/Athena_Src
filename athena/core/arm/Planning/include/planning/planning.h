/**
 * @file
 *
 * @brief 局部规划的输入和输出接口信息。
 */

#ifndef _ATHENA_PLANNING_H
#define _ATHENA_PLANNING_H

#pragma once

#include <math.h>
#include <iostream>
#include <vector>

//#include <pthread.h>
//#include <sys/sem.h>

#include <fstream>
#include <iostream>

#include "common/navi_point.h"
#include "common/path.h"
#include "common/car_state.h"
#include "collision_check/collision_check.h"
#include "park/park.h"
#include "route_data.h"
#include "planning_output.h"
//#include "planning_param.h"
#include "config.h"


/**
 * @namespace athena::planning
 * @brief athena::planning
 */
namespace athena{
namespace planning{

/**
 * @class Planning
 * @brief 局部路径规划的输入接口类。
 */
class Planning
{
public:
    /**
    * @brief 构造函数
    */
    Planning();
    /**
    * @brief 析构函数
    */
    virtual ~Planning();

    /**
    * @brief 初始化函数。
    * @return 1表示初始化成功，0表示失败
    */
    int init();

    /**
    * @brief 重置函数函数。
    * @return 1表示重置成功，0表示失败
    */
    int reset_value();

    /**
    * @brief 输入轨迹，给virtual_path_赋值。
    * @param src_path 输入量：输入的轨迹。
    * @return 1表示成功，0表示失败
    */
    int put_virtual_path( const path& src_path );

    /**
    * @brief 接收地图切片信息，赋值给成员变量car_state_。
    * @param car_state 输入量：地图切片信息。
    * @return 1表示接收车辆状态信息成功，0表示失败
    */
    int put_car_state( const CarState& car_state );

    /**
    * @brief 接收地图切片信息，赋值给成员变量route_
    * @param src_route 输入量：地图切片信息。
    * @return 1表示接收地图切片信息成功，0表示失败
    */
    int put_map_info( const route_planning_m& src_route );

    /**
    * @brief 从切片信息中提取出各车道的中心线,赋值给center_lines_。
    * @return 1表示获得中心线信息成功，0表示失败
    */
    int compute_center_insert_from_route();

    /**
    * @brief 从切片信息中提取出各车道的中心线。
    * @param route 输入量：切片信息。
    * @param center_lines 输出量：各车道的中心线。
    * @return 1表示获得中心线信息成功，0表示失败
    */
    int compute_center_lines_from_route( route_planning_m route, vector<path>& center_lines );

    /**
    * @brief 对各车道的中心线进行三次样条平滑并内插，赋值给smooth_paths_。
    * @return 1表示平滑成功，0表示失败
    */
    int smooth_center_lines();

    /**
    * @brief 对各车道的中心线进行三次样条平滑并内插。
    * @param center_lines 输入量：各车道的中心线。
    * @param smooth_paths 输出量：平滑内插后的中心线。
    * @return 1表示平滑成功，0表示失败
    */
    int smooth_center_lines( vector<path> center_lines, vector<path>& smooth_paths );

    /**
    * @brief 接收路口红绿灯信息。
    * @param src_traffic_lights 输入量：路口红绿灯信息。
    * @return 1表示接收路口红绿灯信息成功，0表示失败
    */
    int put_traffic_lights( const TrafficLights& src_traffic_lights );

    /**
    * @brief 接收停车点/泊车点信息。
    * @param src_stop_park 输入量：停车点/泊车点信息。
    * @param stop_flag 输入量：10终点停车，否则泊车。
    * @return 1表示接收停车点/泊车点信息成功，0表示失败
    */
    int put_stop_park( const StopPark& src_stop_park, const int& stop_flag );

    /**
    * @brief 接收障碍物信息。
    * @param src_obstacle 输入量：障碍物信息。
    * @return 1表示接收障碍物信息成功，0表示失败
    */
    int put_obstacles_info( const SensorObstacles& src_obstacle );

    /**
    * @brief 过滤障碍物信息，通过地图边界信息过滤掉道路外的障碍物。赋值给road_obstacle_。
    * @param filter_obstacle_flag 输入量：障碍物过滤方式。
    * @return 1表示接收障碍物信息成功，0表示失败
    */
    int filter_obstacles_info( int filter_obstacle_flag );

    /**
    * @brief 根据路边界和障碍物中心点，过滤掉不在道路范围内的障碍物_。
    * @param sensor_obstacle 输入量：传感器输出的障碍物信息。
    * @param out_obstacle 输出量：过滤后的障碍物信息。
    * @return 1表示成功，0表示失败
    */
    bool filter_obstacle_by_nothing( const ObstacleInfo sensor_obstacle, ObstacleRect& out_obstacle );

    /**
    * @brief 根据路边界和障碍物中心点，过滤掉不在道路范围内的障碍物_。
    * @param sensor_obstacle 输入量：传感器输出的障碍物信息。
    * @param _begin 输入量：障碍物过滤的范围（起始位置）。
    * @param _end 输入量：障碍物过滤的范围（终止位置）。
    * @param out_obstacle 输出量：过滤后的障碍物信息。
    * @return 1表示障碍物在道路范围内，0表示不在
    */
    bool filter_obstacle_by_point( const ObstacleInfo sensor_obstacle, int _begin, int _end, ObstacleRect& out_obstacle );

    /**
    * @brief 根据路边界和障碍物矩形框，过滤掉不在道路范围内的障碍物_。
    * @param sensor_obstacle 输入量：传感器输出的障碍物信息。
    * @param _begin 输入量：障碍物过滤的范围（起始位置）。
    * @param _end 输入量：障碍物过滤的范围（终止位置）。
    * @param out_obstacle 输出量：过滤后的障碍物信息。
    * @return 1表示障碍物在道路范围内，0表示不在
    */
    bool filter_obstacle_by_rect( const ObstacleInfo sensor_obstacle, int _begin, int _end, ObstacleRect& out_obstacle );


//    /**
//    * @brief 向controll下发速度信息。
//    * @param flag 输入量：0表示直接下发地图速度信息，1表示下发规划的速度信息。
//    */
//    void output_motion_planning_speed( int flag );

//    /**
//    * @brief 计算规划轨迹的纵向信息（速度，加速度，档位）。
//    * @param virtual_lane 输入量：规划的虚拟轨迹。
//    * @param cur_pos 输入量：车辆在所在车道中心线上的位置。
//    * @param end_speed 输入量：规划的虚拟轨迹最后一个点在地图上的地图推荐速度。
//    * @param mstop 输入量：是否有碰撞需要停车。
//    * @param free_num 输入量：有碰撞需要停车时距离碰撞点的距离，如果没有碰撞停车就填极大值99999。
//    */
//    int cal_longitudinal_info( path& virtual_lane, int cur_pos, double end_speed, int mstop, int free_num );


    /**
    * @brief 判断是否需要进行泊车，
    * @return 1表示需要泊车，0表示不需要
    */
    int park_decision();

    /**
    * @brief 生成泊车轨迹，
    * @return 1表示成功，0表示失败
    */
    int generate_park_trajectory();

    /**
    * @brief 地图匹配函数，
    * 实时把车辆状态匹配到地图上，返回车辆在哪个车道的第几个点上。
    * @return 1表示地图匹配成功，0表示失败
    */
    int map_matching_thread();

    /**
    * @brief 行为决策函数，
    * 实时检测车辆与周围障碍物的碰撞关系
    * 判断规划的虚拟轨迹是否有碰撞，及最近碰撞距离；
    * 判断各车道是否有碰撞，及最近碰撞距离；
    * 最终决策出车辆的行为：是否需要换道或减速停车，如果需要换道，向哪个车道换道。
    * @return 1表示碰撞检测成功，0表示失败
    */
    int behaviour_decision_thread();

    /**
    * @brief 轨迹生成函数，
    * 生成轨迹簇，并计算各轨迹的代价函数，选择代价值最小的轨迹为最优轨迹。
    * @return 1表示轨迹生成成功，0表示失败
    */
    int motion_plan_virtual_lane_thread();

    /**
    * @brief 计算纵向速度/加速度，转弯时有减速过程
    * @param virtual_lane 输入量：传感器输出的障碍物信息。
    * @param target_speed 输入量：目标速度。
    * @param free_num 输入量：如果需要停车，停车长度
    * @return 1表示成功，0表示失败
    */
    int compute_longitudinal_info( path& virtual_lane, double target_speed, int free_num );

    /**
    * @brief 设置要输出的轨迹，
    * 给virtual_path_和collision_check_path_赋值
    * @param output_virtual_path 输入量：输入轨迹
    * @param tail_lat_offset 输入量：横向偏移量
    */
    int set_out_trajectory( const path& output_virtual_path, const double& tail_lat_offset );

    /**
    * @brief 输出接口，
    * 给out_trajectory_赋值
    */
    int get_out_trajectory();

    /**
    * @brief 记录out_trajectory_，
    */
    int log_out_trajectory();

    /**
    * @brief 计算纵向速度/加速度，转弯时有减速过程
    * @param start_num 输入量：开始位置
    * @param set_speed 输入量：设置的速度。
    * @param set_gears 输入量：设置的档位
    * @return 1表示成功，0表示失败
    */
    int set_trajectory_longitudinal( const int& start_num, double set_speed, int8_t set_gears );




    /**
    * @brief 设置地图匹配结果，
    * @param cur_pos 输入量：地图匹配得到的车辆在所在车道中心线上的位置。
    * @param en_pos 输入量：地图匹配得到的规划目标点位置（中心线上的点）。
    * @param car_in_which_line 输入量：地图匹配得到的车辆所在车道。
    * @param cur_pos_virtual_lane 输入量：地图匹配得到的车辆在虚拟车道上的位置。
    * @param st_pos_virtual_lane 输入量：地图匹配得到的虚拟车道上规划的起始点。
    * @param map_matching_error 输入量：地图匹配误差。
    * @return 1表示设置地图匹配结果成功，0表示失败
    */
    int set_map_matching_result( int cur_pos, int en_pos, int car_in_which_line, int cur_pos_virtual_lane,
                                 int st_pos_virtual_lane, double map_matching_error );

    /**
    * @brief 获得地图匹配结果，
    * @param cur_pos 输出量：地图匹配得到的车辆在所在车道中心线上的位置。
    * @param en_pos 输出量：地图匹配得到的规划目标点位置（中心线上的点）。
    * @param car_in_which_line 输出量：地图匹配得到的车辆所在车道。
    * @param cur_pos_virtual_lane 输出量：地图匹配得到的车辆在虚拟车道上的位置。
    * @param st_pos_virtual_lane 输出量：地图匹配得到的虚拟车道上规划的起始点。
    * @param map_matching_error 输出量：地图匹配误差。
    * @return 1表示获得地图匹配结果成功，0表示失败
    */
    int get_map_matching_result( int& cur_pos, int& en_pos, int& car_in_which_line, int& cur_pos_virtual_lane,
                                 int& st_pos_virtual_lane, double& map_matching_error );


    /**
    * @brief 设置碰撞检测结果，
    * @param  free_length 输入量：如果虚拟车道有碰撞，车辆到碰撞点之前的距离。
    * @param  collision_virtual_lane 输入量：规划产生的轨迹是否有碰撞。
    * @return 1表示设置碰撞检测结果成功，0表示失败
    */
    int set_collision_check_result( double free_length, bool collision_virtual_lane );

    /**
    * @brief 获得碰撞检测结果，
    * @param free_length 输出量：如果虚拟车道有碰撞，车辆到碰撞点之前的距离。
    * @param collision_virtual_lane 输出量：规划产生的轨迹是否有碰撞。
    * @return 1表示获得碰撞检测结果成功，0表示失败
    */
    int get_collision_check_result( double& free_length, bool& collision_virtual_lane );

    /**
    * @brief 获得规划生成的碰撞检测轨迹（可用于界面显示），
    * @param collision_check_path 输出量：储存规划生成的碰撞检测轨迹。
    * @return 1表示获得轨迹成功，0表示失败
    */
    int get_collision_check_path( path& collision_check_path );

    /**
    * @brief 泊车轨迹地图匹配
    * @return 地图匹配结果（车当前状态匹配到泊车轨迹上的点号），返回-1表示匹配失败
    */
    int park_trajectory_map_matching();

    /**
    * @brief 泊车轨迹碰撞检测
    * @param num_start 输入量：碰撞检测的起始位置，有地图匹配获得。
    * @return 1表示有碰撞，0表示无
    */
    int park_trajectory_collision_check( int num_start );

    /**
    * @brief 手动拨杆换道
    * @param direction 输入量：换道方向，-1左，1右，0不换道。
    * @return 1表示成功，0表示失败
    */
    int decide_hand_expected_lane( int direction );

    /**
    * @brief 平移path
    * @param src_path 输入量：平移前path。
    * @param translate_length 输入量：平移长度。
    * @return 平移后path。
    */
    path translate_path( const path& src_path, const double& translate_length );

    /**
    * @brief 起点或退出自动驾驶
    * @param start 输入量：0起点1退出。
    * @return 1成功0失败。
    */
    int set_start_auto( bool start );

public:
    route_planning_m route_;             ///<全局规划下发的地图切片信息

    CarState car_state_;                ///<车辆状态信息
    TrafficLights traffic_lights_;       ///<路口红绿灯信息
    StopPark terminal_stop_;                 ///<停车点信息
//    StopPark park_stop_;                     ///<泊车点信息
    SensorObstacles sensor_obstacles_;      ///<来自传感器的障碍物信息

//    bool is_car_state_put_;                    ///<true：已输入车辆状态信息,false:没有
//    bool is_map_info_put_;                    ///<true：已输入地图信息,false:没有
//    bool is_stop_park_put_;                   ///<true：已输入停车点/泊车点信息,false:没有
//    bool is_obstacle_info_put_;               ///<true：已输入障碍物信息,false:没有
    bool is_map_updated_;                      ///<动态下发的地图是否更新。true：有更新
    bool is_park_;                             ///<是否需要泊车。true：需要泊车
    bool is_park_d_published_;                  ///<泊车轨迹D档部分是否发送。true：已发送
    bool is_park_r_published_;                  ///<泊车轨迹R档部分是否发送。true：已发送

    OutTrajectory out_trajectory_;                       ///<motion下发给controller的轨迹信息。
    double out_longitudinal_speed_;                       ///<motion下发给controller的纵向速度
    double out_longitudinal_length_;                      ///<多少米后达到速度out_longitudinal_speed_
//    LongitudinalControlInfo longitudinal_control_info_;  ///<motion下发给controller的纵向信息。
    IntelligentParking intelligentparking_;  ///<泊车
    path park_trajectory_d_;                 ///<泊车轨迹D档部分
    path park_trajectory_r_;                 ///<泊车轨迹R档部分


private:
    std::vector<path> center_lines_;        ///<所有车道的道路中心线
    std::vector<path> smooth_paths_;        ///<参考中心线：平滑插值后的道路中心线

    double map_limit_speed_;             ///<地图限速
    RoadSurface road_obstacle_;          ///<路面障碍物信息
    path collision_check_path_;                ///<用于做碰撞检测的轨迹
    path virtual_path_;                  ///<局部规划产生的虚拟轨迹
    bool is_have_virtual_path_;             ///<表示是否已经有虚拟轨迹了，1是0否

    int cross_road_status_;              ///<路口信息，0为非路口    1为路口绿灯  2为在路口且为红灯

    bool is_virtual_lane_collide_;        ///<规划产生的轨迹是否有碰撞，1是0否
    double virtual_lane_free_length_;    ///<如果虚拟车道有碰撞，车辆到碰撞点之前的距离

    bool is_stop_;                       ///<需要停车
    bool is_slow_down_;                  ///<需要减速

    double collision_obj_speed_;         ///<碰撞障碍物的运行速度
    double collision_obj_heading_;       ///<碰撞障碍物的运行方向

    int car_in_lane_;                    ///<本车在哪个车道，从左到右依次为0,1,2,...
    int current_position_;                 ///<车辆在所在车道中心线上的位置
    int end_position_;                  ///<规划目标点位置（中心线上的点）
    double map_matching_error_;          ///<车辆匹配到地图中心线上的距离
    int current_position_virtual_lane_;    ///<车辆在虚拟车道上的位置
    int start_position_virtual_lane_;     ///<虚拟车道上规划的起始点

//    int last_expected_lane_;             ///<上一次期望车道
    int expected_lane_;                  ///<期望车道
//    int last_hand_expected_lane_;         ///<上一次手动期望车道
    int hand_expected_lane_;             ///<手动期望车道

//    int change_lane_direction_;          ///<换道方向
//    int change_lane_reason_;             ///<换道原因
//    int last_change_lane_reason_;        ///<上一次换道的原因
//    int change_lane_starting_lane_;      ///<换道开始的车道
    int change_lane_ending_lane_;        ///<换道结束的车道

    int drive_mode_;                    ///<驾驶模式，1:超车模式，2：跟车模式
    int car_action_;                     ///<车辆状态，1：单车，2：车队头车，3：车队其他车辆
    bool can_start_auto_;                  ///<启动自动驾驶
//    int can_stop_auto_;                   ///<退出自动驾驶

    double keep_last_length_;               ///<保留上一次规划的长度，单位：米
    double planning_length_;               ///<本次规划的长度，单位：米
    int tail_length_number_;                ///<尾部拼接一段地图中心线点的个数，个
    double collision_check_length_;          ///<碰撞检测长度，单位：米

    double next_planning_mileage_;          ///<每走过多少里程更新一次轨迹，单位：米


};



}   //namespace planning
}   //namespace athena

#endif // _ATHENA_PLANNING_H

