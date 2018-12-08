#include <sys/time.h>
#include <stdint.h>

#include "vehicle_dynamic/cau_heading_steering.h"
#include "map_matching/map_matching.h"
#include "planning/planning.h"
#include "common/enum_list.h"
#include "common/path_tools.h"
#include "trajectory/trajectory.h"
#include "trajectory/trajectory_sets.h"

using namespace std;

namespace athena{
namespace planning{

    /**
    * @brief 轨迹生成函数，
    * 生成轨迹簇，并计算各轨迹的代价函数，选择代价值最小的轨迹为最优轨迹。
    * @return 1表示轨迹生成成功，0表示失败
    */
    int Planning::motion_plan_virtual_lane_thread()
    {
        /***获得地图匹配结果***/
        int cur_pos=0, en_pos=0, car_in_which_line=0;
        int cur_pos_virtual_lane=0, st_pos_virtual_lane=0;
        double match_error = 0;

        get_map_matching_result( cur_pos, en_pos, car_in_which_line,
                                 cur_pos_virtual_lane, st_pos_virtual_lane,
                                 match_error );

        if( cur_pos < 0 || en_pos < 0 || en_pos < cur_pos )
        {
            return 0;
        }
        /***判断是否需要换道，往哪个车道换道***/
//        decision_making();

        /***有碰撞风险需要重新规划，或者行驶里程接近上一次规划长度时，才再次生产轨迹***/
//        cout << "xxxxxxxxxxxxxx  car_state_.CurrentS_ = " << car_state_.CurrentS_ << ", next_planning_mileage_ = " << next_planning_mileage_ << endl;
        if( route_.replan_flag == REPLAN_FLAG_NONE  && car_state_.CurrentS_ < next_planning_mileage_ )
        {
            return 0;
        }

        ///decision_making();
        change_lane_ending_lane_ = expected_lane_;
        if ( hand_expected_lane_ > -1 )
            change_lane_ending_lane_ = hand_expected_lane_;
        cout << "22222222222222222222222222222222 expected_lane_ = " << expected_lane_ << ", car_in_which_line = " << car_in_which_line << endl;

        ///轨迹生成类
        trajectory_cubic_sets traj_regular_sets;
        path out_best_path;
        out_best_path.ref_points.clear();

        /*确定轨迹生成的初始点状态*/
        navi_point pose_begin;
        if ( !is_have_virtual_path_ )    //如果没有规划轨迹（如第一次规划）
        {
            //初始点状态即为车当前状态
            pose_begin.position_x = car_state_.car_pose_.CurrentX_;
            pose_begin.position_y = car_state_.car_pose_.CurrentY_;
//            pose_begin.heading = car_state_.Current_heading_;
            pose_begin.heading = smooth_paths_[ car_in_which_line ].ref_points[ cur_pos ].heading;
            pose_begin.k_s = smooth_paths_[ car_in_which_line ].ref_points[ cur_pos ].k_s;
            out_longitudinal_speed_ = map_limit_speed_;
        }
        else
        {
            //初始点状态为匹配到的规划轨迹上的起始点状态
            pose_begin = virtual_path_.ref_points[ st_pos_virtual_lane ];
        }

        /*确定轨迹生成的终止（目标）点状态*/
        navi_point pose_end;
        pose_end = smooth_paths_[ change_lane_ending_lane_ ].ref_points[ en_pos ];

        double dx = pose_end.position_x - pose_begin.position_x;
        double dy = pose_end.position_y - pose_begin.position_y;
        double delta_length = sqrt( dx * dx + dy * dy );


        traj_regular_sets.set_begin_end_pose( pose_begin, pose_end, cur_pos, en_pos );

        cout << ">>>>>>>>>>>>>>>>>>>>> slow_down = " << is_slow_down_ << ", cross_road_status = " << cross_road_status_ << ", delta_length = " << delta_length << endl;
        //add by myy 0703
        if ( is_slow_down_ || cross_road_status_ > 0 || delta_length < MIN_MOTION_LENGTH ) //have_crossroads 在路口1 在路口为红车2  需要收缩横向宽度
        {
            traj_regular_sets.is_change_lane_limit_ = true;
        }
        bool result = traj_regular_sets.genenrate_best_trajectory( road_obstacle_, route_, car_state_.car_speed_, car_action_, out_best_path, drive_mode_ );

        /*轨迹拼接*/
        path tail_path;
        path output_virtual_path;
        if ( is_have_virtual_path_ )
        {
            //插入上一次规划的轨迹
            output_virtual_path.ref_points.insert( output_virtual_path.ref_points.end(), virtual_path_.ref_points.begin()+cur_pos_virtual_lane,
                                                   virtual_path_.ref_points.begin()+st_pos_virtual_lane );
        }
        //插入拟合生成的轨迹
        output_virtual_path.ref_points.insert( output_virtual_path.ref_points.end(), out_best_path.ref_points.begin(),
                                               out_best_path.ref_points.end() );

        //确定是否需要停车(is_stop_)
        if ( traj_regular_sets.p_best_trajectorys->select_value > THRESHOLD_COST_VALUE &&
             car_action_ != CAR_ACTION_PLATOON_OTHER )
        {
            if ( is_stop_ )
                return 0;

            is_stop_ = true;
        }
        else
            is_stop_ = false;

        //计算速度、加速度、档位等信息
        int col_dis_num = ( int )( traj_regular_sets.p_best_trajectorys->collision_distance * 10 );
        int out_side_num = traj_regular_sets.p_best_trajectorys->out_side_num;
        int stop_num = 0;
        if ( col_dis_num < 0 )
        {
            stop_num = out_side_num;
        }
        else
        {
            stop_num = ( col_dis_num < out_side_num ) ? col_dis_num : out_side_num;
        }
        compute_longitudinal_info( output_virtual_path, pose_end.speed_desired_Uxs, stop_num );
        cout << "########## tar_speed = " << pose_end.speed_desired_Uxs << endl;

        set_out_trajectory( output_virtual_path, traj_regular_sets.tail_lat_offset_[traj_regular_sets.best_one] );


        if( result )
        {
            is_have_virtual_path_ = true;
//            cout << "COEF_UPDATE_TRAJECTORY = " << COEF_UPDATE_TRAJECTORY << ", MOTION_PLANNING_LENGTH = " << planning_length_
//                 << ", car_state_.CurrentS_ = "<< car_state_.CurrentS_ << ", next_planning_mileage_ = " << next_planning_mileage_ << endl;
            next_planning_mileage_ = car_state_.CurrentS_ + COEF_UPDATE_TRAJECTORY * ( planning_length_ );
            //换道规划已经完成，所以不允许换道了。直到收到新的网络消息允许换道。
            route_.replan_flag = REPLAN_FLAG_NONE;

//           //这个一次换道完成了再更新当前换道原因。
//            if(current_change_lane_reason_ != CL_REASON_HAND
//               && current_change_lane_reason_ != CL_REASON_OBU_AVOIDING
//               && network_change_lane_allow_
//               && car_in_which_line == respond_nextwork_expected_lane )
//               {
//
//                   cout << "car_in_which_line = " << car_in_which_line << endl;
//                   cout << "respond_nextwork_expected_lane = " << g_obu_planning->respond_nextwork_expected_lane << endl;
//                   g_obu_planning->network_change_lane_allow = false;
//               }
//
//
//            if(current_change_lane_reason == CL_REASON_HAND && hand_change_lane_allow )
//                g_obu_planning->hand_change_lane_allow = false;
//
//            if(current_change_lane_reason == CL_REASON_OBU_AVOIDING && collision_change_lane_allow)
//                g_obu_planning->collision_change_lane_allow = false;
//
//            //恢复到车道保持状态，来保证行驶安全。
//            last_change_lane_reason  = current_change_lane_reason;
//            current_change_lane_reason = CL_REASON_FORBIDEN;
          //  change_lane_reason = CL_REASON_FORBIDEN;


        }



        /***log***/
        if(CHANGE_LANE_LOG_SWITCH)
        {

        }

        if(CHANGE_LANE_VIEW_SWITCH)
        {
//            printf("\033[2J");
//
//            cout << " Driving_behaviour " << Driving_behaviour << endl
//                 << " collision_virtual_lane " << collision_virtual_lane << endl
//    //             << " motion_planner_continue_flag " << motion_planner_continue_flag << endl
//                 << " left_light_status " << left_light_status << endl
//                 << " right_light_status " << right_light_status << endl
//                 << " expected_lane " << expected_lane << endl
//                 << " car_in_lane " << car_in_lane << endl
//                 << " change_lane_reason " << change_lane_reason << endl
//                 << " change_lane_direction " << change_lane_direction <<endl
//                 << " CurrentS " << CurrentS << endl
//                 << " next_start_s " << next_start_s << endl
//                 << " next_plan_s " << next_plan_s << endl
//                 << endl;
        }

        if(PLANNING_VALUE_VIEW_SWITCH)
        {
//            printf("\033[2J");
//
//            cout << " MOTION_PLANNING_LENGTH = " << MOTION_PLANNING_LENGTH
//                 << " AHEAD_OF_MOTION_PLANNING = " << AHEAD_OF_MOTION_PLANNING
//                 << " speed "<< g_obu_planning->car_speed
//                 << endl;
        }

        return 1;
    }

    /**
    * @brief 计算纵向速度/加速度，转弯时有减速过程
    * @param virtual_lane 传感器输出的障碍物信息。
    * @param target_speed 目标速度。
    * @param is_stop 传感器输出的障碍物信息。
    * @param free_num 如果需要停车，停车长度
    * @return 1表示成功，0表示失败
    */
    int Planning::compute_longitudinal_info( path& virtual_lane, double target_speed, int free_num )
    {
        int num_points = virtual_lane.ref_points.size();
        if ( num_points <= 0 )
            return 0;

        ///计算里程
        cau_all_mileage_of_lane( virtual_lane );

        int park_stop_num = -1;
        int obs_stop_num = 99999;
        int index_type = 0;
        double acceleration = 0.0;
        double cur_speed = car_state_.car_speed_;
        double tar_speed = target_speed;
        double s0 = virtual_lane.ref_points[ 0 ].s;
        double temp_speed = 0.0;
        double delta_ks = 0.0;
        double max_delta_ks = 0.0;
        int index_max_delta_ks = -1;
        double min_speed = 0.0;
        double length_from_crossroads = -1;  //离红绿灯路口停车的距离
        int num_from_crossroads = -1;
        bool stop_flag = 0;   //终点停车/路口红绿灯停车点标志位
        int type_of_section;  //下发的切片类型

        cout << "---------------------- out_longitudinal_speed_ = " << out_longitudinal_speed_ << endl;
        cout << "---------------------- out_longitudinal_length_ = " << out_longitudinal_length_ << endl;
        if ( tar_speed > out_longitudinal_speed_ &&  out_longitudinal_length_ >= 0.0 )
        {
            tar_speed = out_longitudinal_speed_;
        }

        /*确定泊车点数*/
        map_matching match_stop;
        double min_error_curr = MATCH_STOP_POINT_ERROR;
        park_stop_num =
        match_stop.MapMarch_Min_Distance_motion_planning(
            terminal_stop_.x_,
            terminal_stop_.y_,
            terminal_stop_.heading_,
            virtual_lane,
            virtual_lane.ref_points.size(),
            min_error_curr, //15
            MATCH_STOP_POINT_ERROR);

        if ( park_stop_num < 0 )
        {
            park_stop_num = 99999;
        }
        else
        {
            stop_flag = 1;
        }
        cout << "========= park_stop_num = " << park_stop_num << endl;
        cout << "========= free_num = " << free_num << endl;
        /*确定障碍物停车点数*/
        if ( is_stop_ )
        {
            if ( free_num == 0)
                obs_stop_num = STOP_LENGTH_TO_OBS;  //避免误识别引起急刹车
            if ( free_num > 2.0 * STOP_LENGTH_TO_OBS )
                obs_stop_num = free_num - STOP_LENGTH_TO_OBS;
            else
                obs_stop_num = free_num * 0.5;

        }
        /*搜索路口红绿灯停车点*/  //myy in 0703
        int traffic_lights_check_length = TRAFFIC_LIGHTS_CHECKS_LENGTH;
        if ( cross_road_status_ == AT_CROSSING_RED )
        {
//            cout << "cccccccccccccc  cross_road_status_ = " << cross_road_status_ << endl;
            for ( int j = current_position_; j < current_position_+traffic_lights_check_length; j++ ) /*根据路口红绿灯检测长度求目标点（本次规划终点）在车道中心线上的位置*/
//            for ( int j = current_position_; j < current_position_+TRAFFIC_LIGHTS_CHECKS_LENGTH; j++ ) /*根据路口红绿灯检测长度求目标点（本次规划终点）在车道中心线上的位置*/
            {
               type_of_section = int( smooth_paths_[ car_in_lane_ ].ref_points[ j ].type );
               if ( type_of_section == KP_LIGHT_STOP )
               {
                    index_type = j;
                    length_from_crossroads = 0;

                    int index_stop = index_type - BEFORE_LIGHTS;
                    if ( index_stop <= current_position_ )
                    {
                         length_from_crossroads = 0;
                    }
                    else
                    {
                        for ( int i = current_position_; i < index_stop; i++ )
                        {
                             length_from_crossroads += length_of_two_navipoint( smooth_paths_[ car_in_lane_ ].ref_points[ i ], smooth_paths_[ car_in_lane_ ].ref_points[ i+1 ] );
                        }
                    }

                    num_from_crossroads = int(length_from_crossroads*10);
                    cout << "距离路口红绿灯停车点长度!!!!!!!!length_from_crossroads = " << length_from_crossroads << endl;
                    break;
               }

            }
        }
        if ( num_from_crossroads < 0 || num_from_crossroads > num_points )
        {
            num_from_crossroads = 99999;
        }
        else
        {
            stop_flag = 1;
        }

        /*需要停车的情况*/
        if ( stop_flag || is_stop_ )
        {
            /*选择最小的stop_num*/
            int stop_num = park_stop_num ;
            if ( stop_num > obs_stop_num )
            {
               stop_num = obs_stop_num;
            }
            if ( stop_num > num_from_crossroads )
            {
               stop_num = num_from_crossroads;
            }

            cout << "!!!!!!!!stop_num = " << stop_num << endl;
            if ( stop_num > num_points )
            {
                stop_num = num_points;
            }

            if ( stop_num < 2 )
                stop_num = 2;
            if ( stop_num > virtual_lane.ref_points.size() )
                stop_num = virtual_lane.ref_points.size();

            acceleration = 0.5 * (0.0-cur_speed*cur_speed) / (virtual_lane.ref_points[stop_num-1].s-s0);
            for ( int i = 0; i < num_points; i++ )
            {
                if ( i < stop_num )
                {
                    virtual_lane.ref_points[i].acceleration_desired_Axs = acceleration;
                    temp_speed = 2.0*acceleration*(virtual_lane.ref_points[i].s-s0) + cur_speed*cur_speed;
                    if ( temp_speed < 0.0 )
                        temp_speed = 0.0;
                    virtual_lane.ref_points[i].speed_desired_Uxs = sqrt( temp_speed );
//                    virtual_lane.ref_points[i].pos_gps_time = 0.0;
                    virtual_lane.ref_points[i].gears = AT_STATUS_D;
                }
                else
                {
                    virtual_lane.ref_points[i].acceleration_desired_Axs = 0.0;
                    virtual_lane.ref_points[i].speed_desired_Uxs = 0.0;
//                    virtual_lane.ref_points[i].pos_gps_time = 0.0;
                    virtual_lane.ref_points[i].gears = AT_STATUS_P;
                }

//                if ( i > 0 )
//                {
//                    double delta_s = virtual_lane.ref_points[i].s - virtual_lane.ref_points[i-1].s;
//                    double avg_speed = virtual_lane.ref_points[i].speed_desired_Uxs + virtual_lane.ref_points[i-1].speed_desired_Uxs;
//
//                    if ( avg_speed == 0.0)
//                    {
//                        virtual_lane.ref_points[i].pos_gps_time = virtual_lane.ref_points[i-1].pos_gps_time;
//                    }
//                    else
//                    {
//                        virtual_lane.ref_points[i].pos_gps_time = virtual_lane.ref_points[i-1].pos_gps_time + 2.0*delta_s/avg_speed;
//                    }
//                }
            }

        }
        else    /*不需要停车*/
        {
            int tar_num = int( out_longitudinal_length_*10 );
            //if ( tar_num > 1 && tar_num <= num_points )
            if ( tar_num > 1 )
            {
                if ( tar_num <= num_points )
                {
                    double acc = 0.5 * ( tar_speed * tar_speed - cur_speed * cur_speed ) / (virtual_lane.ref_points[tar_num-1].s-s0);
                    for ( int i = 0; i < num_points; i++ )
                    {
                        if ( i < tar_num )
                        {
                            temp_speed = 2.0*acc*(virtual_lane.ref_points[i].s-s0) + cur_speed*cur_speed;
                            if ( temp_speed < 0.0 )
                                temp_speed = 0.0;
                            virtual_lane.ref_points[i].speed_desired_Uxs = sqrt( temp_speed );
                            virtual_lane.ref_points[i].acceleration_desired_Axs = acc;
//                            virtual_lane.ref_points[i].pos_gps_time = 0.0;
                            virtual_lane.ref_points[i].gears = AT_STATUS_D;
                        }
                        else
                        {
                            virtual_lane.ref_points[i].speed_desired_Uxs = tar_speed;
                            virtual_lane.ref_points[i].acceleration_desired_Axs = 0.0;
//                            virtual_lane.ref_points[i].pos_gps_time = 0.0;
                            if ( tar_speed == 0.0 )
                                virtual_lane.ref_points[i].gears = AT_STATUS_P;
                            else
                                virtual_lane.ref_points[i].gears = AT_STATUS_D;
                        }

                    }
                }
                else
                {
                    double acc = 0.5 * ( tar_speed * tar_speed - cur_speed * cur_speed ) / out_longitudinal_length_;
                    for ( int i = 0; i < num_points; i++ )
                    {
                        temp_speed = 2.0*acc*(virtual_lane.ref_points[i].s-s0) + cur_speed*cur_speed;
                        if ( temp_speed < 0.0 )
                            temp_speed = 0.0;
                        virtual_lane.ref_points[i].speed_desired_Uxs = sqrt( temp_speed );
                        virtual_lane.ref_points[i].acceleration_desired_Axs = acc;

//                        virtual_lane.ref_points[i].pos_gps_time = 0.0;
                        if ( virtual_lane.ref_points[i].speed_desired_Uxs == 0.0 )
                            virtual_lane.ref_points[i].gears = AT_STATUS_P;
                        else
                            virtual_lane.ref_points[i].gears = AT_STATUS_D;


                    }

                }

            }
            else
            {
                if ( cur_speed < tar_speed )
                {
                    acceleration = MAX_ACCELERATION;    //0.5 * (tar_speed*tar_speed-cur_speed*cur_speed) / (virtual_lane.ref_points[num_speed-1].s-s0);
                    for ( int i = 0; i < num_points; i++ )
                    {
                        virtual_lane.ref_points[i].acceleration_desired_Axs = acceleration;
                        temp_speed = 2.0*acceleration*(virtual_lane.ref_points[i].s-s0) + cur_speed*cur_speed;
                        if ( temp_speed < 0.0 )
                            temp_speed = 0.0;
                        virtual_lane.ref_points[i].speed_desired_Uxs = sqrt( temp_speed );
                        if ( virtual_lane.ref_points[i].speed_desired_Uxs > tar_speed )
                        {
                            virtual_lane.ref_points[i].speed_desired_Uxs = tar_speed;
                            virtual_lane.ref_points[i].acceleration_desired_Axs = 0.0;
                        }
//                        virtual_lane.ref_points[i].pos_gps_time = 0.0;
                        virtual_lane.ref_points[i].gears = AT_STATUS_D;

                        if ( i < num_points-1 )
                        {
                            delta_ks = fabs( virtual_lane.ref_points[i+1].k_s - virtual_lane.ref_points[i].k_s );
                            if ( delta_ks > max_delta_ks )
                            {
                                index_max_delta_ks = i;
                                max_delta_ks = delta_ks;
                            }
                        }
                    }
                }
                if ( cur_speed > tar_speed )
                {
                    acceleration = -MAX_DECELERATION;    //0.5 * (tar_speed*tar_speed-cur_speed*cur_speed) / (virtual_lane.ref_points[num_speed-1].s-s0);
                    for ( int i = 0; i < num_points; i++ )
                    {
                        virtual_lane.ref_points[i].acceleration_desired_Axs = acceleration;
                        temp_speed = 2.0*acceleration*(virtual_lane.ref_points[i].s-s0) + cur_speed*cur_speed;
                        if ( temp_speed < 0.0 )
                            temp_speed = 0.0;
                        virtual_lane.ref_points[i].speed_desired_Uxs = sqrt( temp_speed );
                        if ( virtual_lane.ref_points[i].speed_desired_Uxs < tar_speed )
                        {
                            virtual_lane.ref_points[i].speed_desired_Uxs = tar_speed;
                            virtual_lane.ref_points[i].acceleration_desired_Axs = 0.0;
                        }
//                        virtual_lane.ref_points[i].pos_gps_time = 0.0;
                        virtual_lane.ref_points[i].gears = AT_STATUS_D;

                        if ( i < num_points-1 )
                        {
                            delta_ks = fabs( virtual_lane.ref_points[i+1].k_s - virtual_lane.ref_points[i].k_s );
                            if ( delta_ks > max_delta_ks )
                            {
                                index_max_delta_ks = i;
                                max_delta_ks = delta_ks;
                            }
                        }
                    }
                }
                if ( cur_speed == tar_speed )
                {
                    for ( int i = 0; i < num_points; i++ )
                    {
                        virtual_lane.ref_points[i].speed_desired_Uxs = tar_speed;
                        virtual_lane.ref_points[i].acceleration_desired_Axs = 0.0;
//                        virtual_lane.ref_points[i].pos_gps_time = 0.0;
                        virtual_lane.ref_points[i].gears = AT_STATUS_D;

                        if ( i < num_points-1 )
                        {
                            delta_ks = fabs( virtual_lane.ref_points[i+1].k_s - virtual_lane.ref_points[i].k_s );
                            if ( delta_ks > max_delta_ks )
                            {
                                index_max_delta_ks = i;
                                max_delta_ks = delta_ks;
                            }
                        }
                    }
                }

            }


            min_speed = COEF_KS_SPEED / max_delta_ks;
            if ( min_speed < MIN_SPEED )
                min_speed = MIN_SPEED;
            cout << "*******min_speed = " << min_speed*3.6 << ", max_delta_ks = " << max_delta_ks << endl;
            if ( virtual_lane.ref_points[index_max_delta_ks].speed_desired_Uxs > min_speed )
            {
                acceleration = 0.5 * (min_speed*min_speed-cur_speed*cur_speed) / (virtual_lane.ref_points[index_max_delta_ks].s-s0);
                for ( int j = 0; j < index_max_delta_ks+1; j++ )
                {
                    virtual_lane.ref_points[j].acceleration_desired_Axs = acceleration;
                    temp_speed = 2.0*acceleration*(virtual_lane.ref_points[j].s-s0) + cur_speed*cur_speed;
                    if ( temp_speed < 0.0 )
                        temp_speed = 0.0;
                    virtual_lane.ref_points[j].speed_desired_Uxs = sqrt( temp_speed );
                }

                acceleration = 0.5 * (tar_speed*tar_speed-min_speed*min_speed) / (virtual_lane.ref_points[num_points-1].s-virtual_lane.ref_points[index_max_delta_ks].s);
                if ( acceleration > MAX_ACCELERATION )
                {
                    acceleration = MAX_ACCELERATION;
                }
                for ( int j = index_max_delta_ks; j < num_points; j++ )
                {
                    virtual_lane.ref_points[j].acceleration_desired_Axs = acceleration;
                    temp_speed = 2.0*acceleration*(virtual_lane.ref_points[j].s-virtual_lane.ref_points[index_max_delta_ks].s) + min_speed*min_speed;
                    if ( temp_speed < 0.0 )
                        temp_speed = 0.0;
                    virtual_lane.ref_points[j].speed_desired_Uxs = sqrt( temp_speed );
                }
            }

        }

        ///计算时间信息
        for ( int i = 0; i < num_points; i++ )
        {
            if ( i > 0 )
            {
                double delta_s = virtual_lane.ref_points[i].s - virtual_lane.ref_points[i-1].s;
                double avg_speed = virtual_lane.ref_points[i].speed_desired_Uxs + virtual_lane.ref_points[i-1].speed_desired_Uxs;

                if ( avg_speed == 0.0)
                {
                    virtual_lane.ref_points[i].pos_gps_time = virtual_lane.ref_points[i-1].pos_gps_time;
                }
                else
                {
                    virtual_lane.ref_points[i].pos_gps_time = virtual_lane.ref_points[i-1].pos_gps_time + 2.0*delta_s/avg_speed;
                }
            }
            else
            {
                virtual_lane.ref_points[i].pos_gps_time = 0.0;
            }
        }

        return 1;
    }

    int compute_gears( vector<navi_point>& points, int index )
    {
        if ( points.size() < index )
            return 0;

        int8_t gears = points[ index - 1 ].gears;
        for ( int i = index; i < points.size(); i++ )
        {
            points[ i ].gears = gears;
        }

        return 1;
    }

    /**
    * @brief 设置要输出的轨迹，
    * 给virtual_path_和collision_check_path_赋值
    */
    int Planning::set_out_trajectory( const path& output_virtual_path, const double& tail_lat_offset )
    {
        /*储存virtual_path和col_check_path*/
        virtual_path_.ref_points.clear();
        collision_check_path_.ref_points.clear();

        //插入输出的轨迹（实际下发给控制的轨迹）
        virtual_path_.ref_points.insert( virtual_path_.ref_points.end(),
                                        output_virtual_path.ref_points.begin(), output_virtual_path.ref_points.end() );

        cout << "virtual_path : " << virtual_path_.ref_points.size() << endl;

        //【碰撞检测的轨迹】插入输出的轨迹
        collision_check_path_.ref_points.insert( collision_check_path_.ref_points.end(),
                                          virtual_path_.ref_points.begin(), virtual_path_.ref_points.end() );
        //【碰撞检测的轨迹插入尾部轨迹（车道中心线平移获得）
        // end_position_地图匹配得到的规划目标点位置（中心线上的点）。
        path temp_tail_path;
        int tail_end_position = end_position_+ AFTER_MOTION_PLANNING_NUM;
        if ( tail_end_position > smooth_paths_[ change_lane_ending_lane_ ].ref_points.size()-1 )
             tail_end_position = smooth_paths_[ change_lane_ending_lane_ ].ref_points.size()-1;
        //smooth_paths_为平滑处理后的地图中心线
        cau_current_lane( temp_tail_path, smooth_paths_[ change_lane_ending_lane_ ],
                          end_position_, tail_end_position, tail_lat_offset );

        collision_check_path_.ref_points.insert( collision_check_path_.ref_points.end(),
                                                 temp_tail_path.ref_points.begin(),
                                                 temp_tail_path.ref_points.end() );

        compute_gears( collision_check_path_.ref_points, virtual_path_.ref_points.size() );

        return 1;
    }

    /**
    * @brief 输出接口，
    * 给out_trajectory_赋值
    */
    int Planning::get_out_trajectory()
    {
        int num_points = virtual_path_.ref_points.size();
        if ( num_points <= 0 )
            return 0;

        out_trajectory_.gps_time_ = 0.0;
        out_trajectory_.car_action_ = car_action_;
        out_trajectory_.driving_mode_ = can_start_auto_ ? AUTO_DRIVING : HUMAN_DRIVING;
        out_trajectory_.num_of_points_ = num_points;

        out_trajectory_.points_ = virtual_path_.ref_points;

        if(TRAJECTORY_LOG_SWITCH)
        {
            log_out_trajectory();
        }

        return 1;
    }

    /**
    * @brief 记录out_trajectory_，
    */
    int Planning::log_out_trajectory()
    {
        int num_points = out_trajectory_.points_.size();
        if ( num_points <= 0 )
            return 0;

        ofstream outfile("trj_plann_log.log", std::ios::app);
        outfile.precision(8);

        outfile << " s " << car_state_.CurrentS_ << " "
                << " x " << car_state_.car_pose_.CurrentX_ << " "
                << " y " << car_state_.car_pose_.CurrentY_ << " "
                << " h " << car_state_.car_pose_.Current_heading_ << " "
                << " speed " << car_state_.car_speed_ << " "
                << " steering " << car_state_.steer_angle_ << " "
                << " is_have_virtual_path_ " << is_have_virtual_path_ << " "
                << " car_in_lane_ " << car_in_lane_ << " "
                << " current_position_ " << current_position_ << " "
                << " end_position_ " << end_position_ << " "
                << " current_position_virtual_lane_ " << current_position_virtual_lane_ << " "
                << " start_position_virtual_lane_ " << start_position_virtual_lane_ << " "
                << " car_action_ " << out_trajectory_.car_action_ << " "
                << " driving_mode_ " << out_trajectory_.driving_mode_ << " "
                << endl << endl;

        for(int i=0; i < num_points; i++)
        {
            outfile << " i " << i
                    << " s " << out_trajectory_.points_[i].s
                    << " ss " << out_trajectory_.points_[i].s + car_state_.CurrentS_
                    << " t " << out_trajectory_.points_[i].pos_gps_time
                    << " x " << out_trajectory_.points_[i].position_x
                    << " y " << out_trajectory_.points_[i].position_y
                    << " h " << out_trajectory_.points_[i].heading
                    << " v " << out_trajectory_.points_[i].speed_desired_Uxs
                    << " a " << out_trajectory_.points_[i].acceleration_desired_Axs
                    << " k " << out_trajectory_.points_[i].k_s
                    << " g " << (int)out_trajectory_.points_[i].gears
                    << endl;
            //outfile << output_virtual_path.ref_points[i].k_s << endl;
        }

        outfile << endl;
        outfile.close();

        out_trajectory_.gps_time_ = 0.0;
        out_trajectory_.car_action_ = car_action_;
        out_trajectory_.driving_mode_ = can_start_auto_ ? AUTO_DRIVING : HUMAN_DRIVING;
        out_trajectory_.num_of_points_ = num_points;

        out_trajectory_.points_ = virtual_path_.ref_points;

        return 1;
    }

    /**
    * @brief 计算纵向速度/加速度，转弯时有减速过程
    * @param start_num 输入量：开始位置
    * @param set_speed 输入量：设置的速度。
    * @param set_gears 输入量：设置的档位
    * @return 1表示成功，0表示失败
    */
    int Planning::set_trajectory_longitudinal( const int& start_num, double set_speed, int8_t set_gears )
    {
        if ( out_trajectory_.num_of_points_ < 0 )
            return 0;

        for ( int i = start_num; i < out_trajectory_.num_of_points_; i++ )
        {
            out_trajectory_.points_[ i ].speed_desired_Uxs = set_speed;
            out_trajectory_.points_[ i ].gears = set_gears;

        }

        return 1;
    }



}   //namespace planning
}   //namespace athena



