#include "map_matching/map_matching.h"
#include "planning/planning.h"
#include "common/enum_list.h"

using namespace std;

namespace athena{
namespace planning{

    /**
    * @brief 地图匹配函数，
    * 实时把车辆状态匹配到地图上，返回车辆在哪个车道的第几个点上。
    * @return 1表示地图匹配成功，0表示失败
    */
    int Planning::map_matching_thread()
    {
#if 1
        //无车道中心线不能进行匹配
        if ( smooth_paths_.size() < 1 )
        {
            //cout << "smooth_paths_.size() = " << smooth_paths_.size() << endl;
            return 0;
        }
        //车道中心线太短不能进行匹配
        if ( smooth_paths_[ 0 ].ref_points.size() < 8 )
        {
            //cout << "smooth_paths_[ 0 ].ref_points.size() = " << smooth_paths_[ 0 ].ref_points.size() << endl;
            return 0;
        }

        /*获得上次地图匹配结果*/
        int last_cur_pos=0, last_en_pos=0, last_car_in_which_line=0;
        int last_cur_pos_virtual_lane=0, last_st_pos_virtual_lane=0;
        double last_match_error = 0;

        get_map_matching_result( last_cur_pos, last_en_pos, last_car_in_which_line,
                                 last_cur_pos_virtual_lane, last_st_pos_virtual_lane,
                                 last_match_error );

        /***车当前位置与各车道中心线匹配，确定车所在车道，以及在该车道上的点号***/
        map_matching m_matching_center, m_matching_virtual_lane;
        double min_error_curr = 0.0;
        double min_length = MAXIMUM_VALUE;
        double min_error_virtual = MAXIMUM_VALUE;
        int in_wihch_lane = -1;

        int temp_inner_cur_pos = -1;
        int inner_cur_pos=-1;
        int inner_en_pos=-1;
//        int stop_en_pos=-1;  //myy in 0702
        int inner_cur_pos_vritual_lane=-1;
        int inner_st_pos_virtual_lane=-1;

        //zp20171023:优先匹配低速道
//        bool num_of_lanes_changed = false;
//        cout << "oooooooooooooooooooooo  is_map_updated_ = " << is_map_updated_ << endl;
//    cout << "oooooooooooooooooooooo  current_position_ = " << current_position_ << endl;

        if ( PRIOR_MATCH_LANE )
        {
            for ( int i = smooth_paths_.size() - 1; i >= 0; i-- )
            {
                temp_inner_cur_pos = m_matching_center.MapMarch_Min_Distance_motion_planning(
                                                        car_state_,
                                                        smooth_paths_[ i ],
                                                        is_map_updated_,
                                                        GLOBAL_SEARCH_MATCH_CENTERS,
                                                        NUM_BEFORE_MATCH_CENTERS,
                                                        min_error_curr,
                                                        last_cur_pos );

//                cout << "--------" << i << " : " << temp_inner_cur_pos << ", " << min_error_curr << endl;

                if ( min_error_curr < ( min_length - THRESHOLD_MATCH_CENTERS ) )   //0.5***config
                {
                    min_length = min_error_curr;
                    in_wihch_lane = i;                  //车在哪个车道
                    inner_cur_pos = temp_inner_cur_pos; //车在车道中心线上哪个点
                }
//                if ( fabs( min_error_curr - min_length ) < THRESHOLD_MATCH_CENTERS )
//                {
//                    num_of_lanes_changed = true;
//                }
            }
        }
        //zp20180105:优先匹配高速道
        else
        {
            for ( int i = 0; i < smooth_paths_.size(); i++ )
            {
                temp_inner_cur_pos = m_matching_center.MapMarch_Min_Distance_motion_planning(
                                                        car_state_,
                                                        smooth_paths_[ i ],
                                                        is_map_updated_,
                                                        GLOBAL_SEARCH_MATCH_CENTERS,
                                                        NUM_BEFORE_MATCH_CENTERS,
                                                        min_error_curr,
                                                        last_cur_pos );

                //cout << "--------" << i << " : " << temp_inner_cur_pos << ", " << min_error_curr << endl;

                if ( min_error_curr < ( min_length - THRESHOLD_MATCH_CENTERS ) )   //0.5***config
                {
                    min_length = min_error_curr;
                    in_wihch_lane = i;                  //车在哪个车道
                    inner_cur_pos = temp_inner_cur_pos; //车在车道中心线上哪个点
                }
//                if ( fabs( min_error_curr - min_length ) < THRESHOLD_MATCH_CENTERS )
//                {
//                    num_of_lanes_changed = true;
//                }
            }
        }
//        if ( num_of_lanes_changed )
//        {
//            expected_lane_ = in_wihch_lane;
//        }

//        cout << "--------" << in_wihch_lane << " : " << inner_cur_pos << " ,  " << min_length << endl;
        /*如果没有匹配上，或匹配到中心线的距离过大，认为匹配失败*/
        if( inner_cur_pos >= 0 && fabs(min_length) < THRESHOLD_MATCH_BIAS ) //10.0***config
        {
           car_state_.CurrentS_ = smooth_paths_[ in_wihch_lane ].ref_points[inner_cur_pos].s;  //计算当前车行驶的里程
           map_limit_speed_ = smooth_paths_[ in_wihch_lane ].ref_points[inner_cur_pos].speed_desired_Uxs;             ///<地图限速
           is_map_updated_ = false;
        }
        else
        {
           return 0;
        }

//        cout << "------------- is_have_virtual_path_ = " << is_have_virtual_path_ << endl;
        /***根据当前车速确定本次规划的长度***/
        //if ( !is_have_virtual_path_ )
        if ( car_state_.car_speed_ < THRESHOLD_CAR_STATIC_SPEED )
        {
            planning_length_ = COEF1_MOTION_WITHOUT_VP + COEF2_MOTION_WITHOUT_VP * fabs(min_length);    //5.0,1.0***config
            keep_last_length_ = COEF1_AHEAD_WITHOUT_VP + COEF2_AHEAD_WITHOUT_VP * car_state_.car_speed_;
        }
        else
        {
            if ( car_state_.car_speed_ < THRESHOLD_HIGH_SPEED/3.6 )
            {
                planning_length_ = COEF1_MOTION_WITH_VP + COEF2_MOTION_WITH_VP * car_state_.car_speed_;    //5.0,5.0***config

            }
            else
            {
                planning_length_ = COEF1_MOTION_HIGH_SPEED + COEF2_MOTION_HIGH_SPEED * car_state_.car_speed_;    //5.0,5.0***config

            }
            keep_last_length_ = COEF1_AHEAD_WITH_VP + COEF2_AHEAD_WITH_VP * car_state_.car_speed_;   //1.0,0.2***config

//            cout << "------------- planning_length_ = " << planning_length_ << endl;
        }
        ///不小于最小规划长度
        if ( planning_length_ < MIN_MOTION_LENGTH )
        {
            planning_length_ = MIN_MOTION_LENGTH;
        }
        ///不超过最大规划长度
        if ( planning_length_ > MAX_MOTION_LENGTH )
        {
            planning_length_ = MAX_MOTION_LENGTH;
        }

        /*根据长度求目标点（本次规划终点）在车道中心线上的位置*/
        inner_en_pos = m_matching_center.find_next_moition_planning_points(
                        smooth_paths_[ in_wihch_lane ],
                        inner_cur_pos,
                        planning_length_ );

        if ( route_.line[inner_en_pos].num_of_lane > route_.line[inner_cur_pos].num_of_lane )
        {
            expected_lane_ = in_wihch_lane;
        }

        /*与上一次规划的虚拟轨迹匹配，得到：
          (1)车当前状态在上一次规划轨迹上的位置
          (2)保留一段上一次规划的轨迹，找到下一次生成轨迹的起始点*/
        if(is_have_virtual_path_)
        {
            m_matching_virtual_lane.init();

            inner_cur_pos_vritual_lane =
            m_matching_virtual_lane.MapMarch_Min_Distance_mapping(
                car_state_.car_pose_.CurrentX_,
                car_state_.car_pose_.CurrentY_,
                virtual_path_,
                virtual_path_.ref_points.size(),
                min_error_virtual);

            if( fabs(min_error_virtual) > THRESHOLD_MATCH_BIAS || inner_cur_pos_vritual_lane < 0 ) //10.0***config
                 goto  NO_VIRTUAL_PATH;

            inner_st_pos_virtual_lane =
                m_matching_virtual_lane.find_next_moition_planning_points(
                                        virtual_path_,
                                        inner_cur_pos_vritual_lane,
                                        keep_last_length_ );

//            double delta_h = 0.0;
//            double max_h = MINIMUM_VALUE;
//            double heading0 = virtual_path_.ref_points[ inner_st_pos_virtual_lane ].heading;
//
//            while( planning_length_ > MIN_MOTION_LENGTH )
//            {
//                max_h = MINIMUM_VALUE;
//                for ( int i = inner_en_pos; i > inner_cur_pos+3; i-- )
//                {
//                    delta_h = smooth_paths_[ in_wihch_lane ].ref_points[ i ].heading - heading0;
//                    delta_h = fabs( delta_h );
//                    if ( delta_h > 270.0 )
//                        delta_h = fabs( delta_h - 360.0 );
//                    if ( delta_h > max_h )
//                    {
//                        max_h = delta_h;
//                    }
//                }
//
////                delta_h = smooth_paths_[ in_wihch_lane ].ref_points[ inner_en_pos ].heading - heading0;
////                delta_h = fabs( delta_h );
////                if ( delta_h > 270.0 )
////                    delta_h = fabs( delta_h - 360.0 );
//
//                if ( max_h > MAX_MOTION_DELTA_HEADING )
////                if ( delta_h > MAX_MOTION_DELTA_HEADING )
//                {
//                    planning_length_ -= INTERVAL_MAP_SECTION;
//                    inner_en_pos = m_matching_center.find_next_moition_planning_points(
//                        smooth_paths_[ in_wihch_lane ],
//                        inner_cur_pos,
//                        planning_length_ );
//                }
//                else
//                {
//                    break;
//                }
//
//            }

        }
        else
        {
            NO_VIRTUAL_PATH:
            inner_cur_pos_vritual_lane = -1;
            inner_st_pos_virtual_lane = -1;
            change_lane_ending_lane_ = in_wihch_lane + 1;
        }

        double delta_h = 0.0;
        double delta_K_abs = 0.0;
        double max_h = MINIMUM_VALUE;
//        double max_k_abs = MINIMUM_VALUE;
        double heading0, k_s0;
        if ( is_have_virtual_path_ )
        {
            heading0 = virtual_path_.ref_points[ inner_st_pos_virtual_lane ].heading;
//            k_s0 = virtual_path_.ref_points[ inner_st_pos_virtual_lane ].k_s;
        }
        else
        {
            heading0 = smooth_paths_[ in_wihch_lane ].ref_points[ inner_cur_pos ].heading;
//            k_s0 = smooth_paths_[ in_wihch_lane ].ref_points[ inner_cur_pos ].k_s;
        }

        while( planning_length_ > MIN_MOTION_LENGTH )
        {
            max_h = MINIMUM_VALUE;
            for ( int i = inner_en_pos; i > inner_cur_pos+3; i-- )
            {
                delta_h = smooth_paths_[ in_wihch_lane ].ref_points[ i ].heading - heading0;
//                delta_K_abs = fabs( smooth_paths_[ in_wihch_lane ].ref_points[ i ].k_s - k_s0 );

                delta_h = fabs( delta_h );
                if ( delta_h > 270.0 )
                    delta_h = fabs( delta_h - 360.0 );
                if ( delta_h > max_h )
                {
                    max_h = delta_h;
                }
            }

//                delta_h = smooth_paths_[ in_wihch_lane ].ref_points[ inner_en_pos ].heading - heading0;
//                delta_h = fabs( delta_h );
//                if ( delta_h > 270.0 )
//                    delta_h = fabs( delta_h - 360.0 );

            if ( max_h > MAX_MOTION_DELTA_HEADING )
//                if ( delta_h > MAX_MOTION_DELTA_HEADING )
            {
                planning_length_ -= INTERVAL_MAP_SECTION;
                inner_en_pos = m_matching_center.find_next_moition_planning_points(
                    smooth_paths_[ in_wihch_lane ],
                    inner_cur_pos,
                    planning_length_ );
            }
            else
            {
                break;
            }

        }

        /***存储地图匹配结果***/
        set_map_matching_result(
                                    inner_cur_pos,              //当前位置在地图中心线上的匹配点
                                    inner_en_pos,               //规划目标点在地图中心线上的匹配点
                                    in_wihch_lane,              //在哪个车道  //myy in 0702
                                    inner_cur_pos_vritual_lane, //当前位置在上一次规划轨迹上的匹配点
                                    inner_st_pos_virtual_lane,  //本次规划的起始点（保留一小段上一次规划轨迹）
                                    min_length                  //与道路中心线的最小垂直距离
                                );

        /*log*/
//        cout << "--------- AHEAD = " << keep_last_length_ << ", MOTION = " << planning_length_ << endl;
        if( VIRTUAL_PATH_LOG_SWITCH )
        {
            if(is_have_virtual_path_)
            {
                ofstream outfile("traj_info.log", std::ios::app);
                outfile.precision(8);

                outfile << "i " << inner_cur_pos_vritual_lane
                        << " s " << car_state_.CurrentS_
                        << " x " << car_state_.car_pose_.CurrentX_
                        << " y " << car_state_.car_pose_.CurrentY_
                        << " h " << car_state_.car_pose_.Current_heading_
                        << " steer " << car_state_.steer_angle_
                        << " k_s " << virtual_path_.ref_points[inner_cur_pos_vritual_lane].k_s
                        << " v " << virtual_path_.ref_points[inner_cur_pos_vritual_lane].speed_desired_Uxs
                        << " a " << virtual_path_.ref_points[inner_cur_pos_vritual_lane].acceleration_desired_Axs
                        << endl;
                outfile.close();
            }
        }
#endif // 0
        return 1;
    }

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
    int Planning::set_map_matching_result( int cur_pos, int en_pos, int car_in_which_line, int cur_pos_virtual_lane,
                                 int st_pos_virtual_lane, double map_matching_error )
    {
        car_in_lane_ = car_in_which_line;                           ///<本车在哪个车道，从左到右依次为0,1,2,...
        current_position_ = cur_pos;                                ///<车辆在所在车道中心线上的位置
        end_position_ = en_pos;                                     ///<规划目标点位置（中心线上的点）
        map_matching_error_ = map_matching_error;                   ///<车辆匹配到地图中心线上的距离
        current_position_virtual_lane_ = cur_pos_virtual_lane;      ///<车辆在虚拟车道上的位置
        start_position_virtual_lane_ = st_pos_virtual_lane;         ///<虚拟车道上规划的起始点

        return 1;
    }

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
    int Planning::get_map_matching_result( int& cur_pos, int& en_pos, int& car_in_which_line, int& cur_pos_virtual_lane,
                                 int& st_pos_virtual_lane, double& map_matching_error )
    {
        car_in_which_line = car_in_lane_;                           ///<本车在哪个车道，从左到右依次为0,1,2,...
        cur_pos = current_position_;                                ///<车辆在所在车道中心线上的位置
        en_pos = end_position_;                                     ///<规划目标点位置（中心线上的点）
        map_matching_error = map_matching_error_;                   ///<车辆匹配到地图中心线上的距离
        cur_pos_virtual_lane = current_position_virtual_lane_;      ///<车辆在虚拟车道上的位置
        st_pos_virtual_lane = start_position_virtual_lane_;         ///<虚拟车道上规划的起始点

        return 1;
    }


}   //namespace planning
}   //namespace athena



