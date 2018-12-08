#include <math.h>
#include <iostream>
#include <vector>

//#include "../../common/origin_vehicle.h"
#include "common/navi_point.h"
#include "common/math_util.h"

#include "trajectory/trajectory_sets.h"


trajectory_cubic_sets::trajectory_cubic_sets()
{
    p_best_trajectorys=NULL;
    best_one = 16;
    block_rate=0;
    is_change_lane_limit_ = false;
    tail_lat_offset_.clear();

    traj_current.points.clear();

    for(int t=0 ; t < 33; t ++  )
    {
       trajectory_cubic traj;
       trajectorys.push_back(traj);
    }

}


void trajectory_cubic_sets::clear_all()
{
    p_best_trajectorys=NULL;
    best_one = 16;
    block_rate=0;

    traj_current.points.clear();

    for(int t=0 ; t < 33; t ++  )
    {
       trajectory_cubic traj;
       trajectorys.push_back(traj);
    }

}

trajectory_cubic_sets::~trajectory_cubic_sets()
{

}

void trajectory_cubic_sets::set_ref_path_length(
      vector<navi_point>& ref_,
      int start_pos, int end_pos)
{
    unsigned int i;

    ref_path_points.clear();
    //参考轨迹的点号是从头开始。但是里程是绝对里程
    for (i = start_pos; i <= end_pos; i++)
    {
        navi_point np;
        np = ref_[i];

        ref_path_points.push_back(np);
    }

}

/**
    * @brief 设置轨迹的始末点
    */
    int trajectory_cubic_sets::set_begin_end_pose( const navi_point& pose_begin, const navi_point& pose_end,
                                                   const int& num_begin, const int& num_end )
    {
        pose_begin_ = pose_begin;
        pose_end_ = pose_end;
        num_begin_ = num_begin;
        num_end_ = num_end;

        return 1;
    }

    /**
    * @brief 生成最优轨迹
    */
    int trajectory_cubic_sets::genenrate_best_trajectory( const RoadSurface& road_surface,
                                                          const route_planning_m& route,
                                                          const double& car_speed,
                                                          const int car_action,
                                                          path &out_best_trajectory,
                                                          const int drive_mode )
    {
        /*始末点*/
        navi_point pose_begin = pose_begin_;
        navi_point pose_end = pose_end_;
        int num_begin = num_begin_;
        int num_end = num_end_;

        /*确定轨迹生成的目标点状态*/
        navi_point lat_point;
        double lat_angle = 0.0, lat_step = 0.0;
//        vector< double > tail_lat_offset;   //尾部拼接的偏移量

        vector< navi_point > pose_lat_ends;    //横向目标点集
        pose_lat_ends.clear();
        tail_lat_offset_.clear();

        //插入横向点
        lat_angle = pose_end.heading + 90.0;
        lat_step = LAT_OFFSET / NUM_TRAJ_CLUSTER;  //route.line[ en_pos_1 ].lane[ car_in_which_line ].width / 8; //8***config
        if ( is_change_lane_limit_ ) // 需要收缩横向宽度
        {
            lat_step *= COEF_LIMIT_LAT_STEP;
        }
//        cout << "1111111111111111111111111 lat_step "<< lat_step << endl;
        for ( int i = -NUM_TRAJ_CLUSTER; i < NUM_TRAJ_CLUSTER+1; i++ )
        {
            //插入第一组目标点
            lat_point.position_x = pose_end.position_x + i * lat_step * sin( to_radians( lat_angle ) );
            lat_point.position_y = pose_end.position_y + i * lat_step * cos( to_radians( lat_angle ) );
            lat_point.heading = pose_end.heading;
            lat_point.k_s = 1.0 / ( 1.0 / ( pose_end.k_s ) + i * lat_step ); //+左-右,曲率暂时没用到
            lat_point.r = abs( i );
            pose_lat_ends.push_back( lat_point );

            tail_lat_offset_.push_back( i * lat_step );
        }

        cout << "----------------------------------------------------------------" << endl;
        cout << "pose_begin : " << pose_begin.k_s << ", "<< pose_begin.heading << endl;
        cout << "pose_end : " << pose_end.k_s << ", "<< pose_end.heading << endl;
        cout << "speed = " << car_speed << endl;


        /*轨迹簇生成*/
        gen_traj_cluster( pose_begin, pose_lat_ends );

        /*最优轨迹选择*/
        ///考虑各轨迹是否有碰撞
        cluster_collision_check( road_surface, car_speed );
        ///考虑每条轨迹的最大曲率
        cluster_ks_check();
        //consistency_check();
        ///选取代价值最小的轨迹
        select_best_traj( route, num_begin, num_end, car_action, drive_mode );
        cout << "=========best_one:" << best_one  << ", select_value:" << p_best_trajectorys->select_value << endl;

        out_best_trajectory.ref_points.resize( p_best_trajectorys->points.size() );
        out_best_trajectory.ref_points = p_best_trajectorys->points;



        return 1;
    }
