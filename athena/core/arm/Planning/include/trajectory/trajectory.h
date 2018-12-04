#pragma once

#include <math.h>
#include <iostream>

//#include "common/ecu.h"
#include "common/navi_point.h"
#include "collision_check/collision_check.h"

using namespace std;

class trajectory_sets;
class trajectory_cubic_sets;

bool lane_collision_check(
                           path& lane,
                           RoadSurface& road,
                           int st_pos, int en_pos,
                           double check_length,
                           int& free_length_num,
                           double& free_length,
                           double& obj_speed,
                           double& obj_heading);

bool lane_collision_check_moving_objects(
                          path& lane,
                          RoadSurface& road,
                          int st_pos, int en_pos,
                          double check_length,
                          int& free_length_num,
                          double& free_length,
                          double& obj_speed,
                          double& obj_heading);

class trajectory
{
public:
    int tr_num;
    // collision_check_value, 0 is minimal
    double collision_check_value;
    double collision_time;
    double collision_obj_speed;
    double collision_distance;
    double first_collision_obj_num;
    double left_offset_value;
    double right_offset_value;

    double free_length;
    int free_length_num;
    int out_side_num;

    //目标点的代价值
    double end_pos_value;

    double ks_sum_value;
    double ls_sum_value;
    double steering_angle_sum_value;

    double consistency_value;
    double expected_lane_value;

    double select_value;

    //参考位置的开始和停止的值
    int start_pos;
    int end_pos;

    //参考里程的开始和停止的值
    double start_s;
    double end_s;

public:
    void init();

    trajectory();
    ~trajectory();

public:
    vector<navi_point>  points;  // 轨迹上的所有点 初步为2000个
    int points_num;   // 总共的数目
    int points_num_1;   // 到第一个目标点轨迹的数目
};

class trajectory_cubic
    : public trajectory
{
public:
    trajectory_cubic_sets *p_traj_cubic_sets;

public:
    void set_trajectory_sets(trajectory_cubic_sets* p_set);
    void generate_trajectory_lateral_offset_ks(double current_speed_lon, double offset);

    void copy_from(trajectory& tr);

    double traj_collision_check( const RoadSurface& road, const double car_speed );  //轨迹的碰撞检测
    float collision_check(RoadSurface& road);
    float collision_check_moving_objects(RoadSurface& road);

    float traj_ks_check();
    float ls_sum_check();
    float steering_angle_sum_check();

    float consistency_check(trajectory& tr);
    float consistency_check_virtual_lane(path& virtual_lane);

    trajectory_cubic();
    ~trajectory_cubic();

    //已知两点状态，生成轨迹
    int gen_trj_from_two_points( navi_point pos0, navi_point pos1, vector<navi_point>& out_points );

};
