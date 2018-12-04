#pragma once

#include <math.h>
#include <iostream>
#include <vector>

//#include "common/ecu.h"
#include "common/navi_point.h"
#include "collision_check/collision_check.h"
#include "planning/route_data.h"


#include "trajectory.h"

using namespace std;

double calculate_sign_and_value_offset(
                          double x, double y, double yaw,
                          double x_c, double y_c, double yaw_c);

void calculate_d_dd(double current_heading,
                    double current_steer_angle,
                    double current_speed_lon,
                    double delta_s,
                    double heading_r,
                    double& d_off,
                    double& dd_off);

double offset_of_trajectory_from_lane(
    trajectory_cubic& tr,
    path& p,
    double offset ,
    int change_lane_ending_lane,
    int in_lane);

class trajectory_sets
{
public:

public:
    trajectory_sets();
    ~trajectory_sets();
};

class trajectory_cubic_sets
    : public trajectory_sets
{
public:
    // ���ɵ����й켣
    vector< trajectory_cubic > trajectorys;
    // �ο��켣�����е�
    vector<navi_point> ref_path_points;
    // best_one
    trajectory_cubic* p_best_trajectorys;
    // current_trajectory
    trajectory_cubic traj_current;

    int best_one;

    double block_rate;

    double s_pos;
    double e_pos;

    navi_point pose_begin_;
    navi_point pose_end_;
    int num_begin_;
    int num_end_;
    vector< double > tail_lat_offset_;   //β��ƴ�ӵ�ƫ����

    bool is_change_lane_limit_;

public:
    trajectory_cubic_sets();
    ~trajectory_cubic_sets();

    void clear_all();

    /**
    * @brief ���ù켣��ʼĩ��
    */
    int set_begin_end_pose( const navi_point& pose_begin, const navi_point& pose_end,
                            const int& num_begin, const int& num_end );

    /**
    * @brief �������Ź켣
    */
    int genenrate_best_trajectory( const RoadSurface& road_surface,
                                   const route_planning_m& route,
                                   const double& car_speed,
                                   const int car_action,
                                   path &out_best_trajectory );

    void set_ref_path_length(vector<navi_point>& ref_, int start_pos , int end_pos);

    // �ϰ����ж�
    void cluster_collision_check( const RoadSurface& road, const double car_speed );
    void collision_check(RoadSurface& road);
    void cluster_ks_check();
    void consistency_check();
    int nearest_tail_check(
        path& lane,
        int change_lane_ending_lane,
        int in_lane);

    //�ۺ�����
    int select_best_traj( const route_planning_m& route_planning, const int ref_pos1, const int ref_pos2, const int car_action );


    void speed_plan(double max_speed);


    int out_side_traj_check( line_xys left_edge, line_xys right_edge, int st_pos, int en_pos, double cost_value );

    ///���ɹ켣��
    int gen_traj_cluster( navi_point pos_sta, vector< navi_point > pos_ends_1, vector< navi_point > pos_ends_2 );
    int gen_traj_cluster( navi_point pos_sta, vector< navi_point > pos_ends_1 );

};
