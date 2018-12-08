#include "planning/planning.h"
#include "common/enum_list.h"
#include "collision_check/collision_check.h"

using namespace std;

namespace athena{
namespace planning{

/**
* @brief 行为决策函数，
* 实时检测车辆与周围障碍物的碰撞关系
* 判断规划的虚拟轨迹是否有碰撞，及最近碰撞距离；
* 判断各车道是否有碰撞，及最近碰撞距离；
* 最终决策出车辆的行为：是否需要换道或减速停车，如果需要换道，向哪个车道换道。
* @return 1表示碰撞检测成功，0表示失败
*/
int Planning::behaviour_decision_thread()
{
    CollisionCheck collision_check;

    int num_paths = smooth_paths_.size();
    static int count_col = 0;
    static int count_slow = 0;
    //没有地图中心线，无法进行碰撞检测*/
    if ( num_paths < 1 )
    {
        return 0;
    }
    //地图中心线轨迹太短，无法进行碰撞检测*/
    if ( smooth_paths_[ 0 ].ref_points.size() < 8 )
    {
        return 0;
    }

    /*获得地图匹配结果*/
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
    if ( expected_lane_ < 0 || expected_lane_ == car_in_which_line )
        expected_lane_ = car_in_which_line;

    /*碰撞检测*/
    path* p_virtual_path;
//    RoadSurface* road_obstacle;
    vector<ObstacleRect> road_obstacles;

    filter_obstacles_info( FILTER_OBSTACLE_POINT );     //过滤障碍物，赋值road_obstacle_
//    road_obstacle = &road_obstacle_;
    road_obstacles = road_obstacle_.obstacles_;                          //获得路面障碍物情况
    p_virtual_path = &collision_check_path_;                  //获得规划的虚拟轨迹
    if ( p_virtual_path->ref_points.size() <= 0 )
    {
        return 0;
    }

    /***首先判断规划的虚拟路径是否有碰撞***/
    double inner_free_length = -1;
    bool inner_collision_virtual_lane = 1;
    //确定碰撞检测的距离：根据速度来定，速度越快，需要检测的距离越长.car_speed:从车身can获取的车速，单位m/s
    collision_check_length_ = COEF_COL_CHECK_LENGTH * ( planning_length_ + keep_last_length_ );    //5.0,5.0***config,与MOTION_PLANNING_LENGTH长度一致
    if ( collision_check_length_ < 80.0 )
        collision_check_length_ = 80.0;
//    if ( collision_check_length_ < 2.0*STOP_LENGTH_TO_OBS )
//        collision_check_length_ = 2.0*STOP_LENGTH_TO_OBS;

    //inner_free_length:碰撞距离，如果无碰撞，=-1
    inner_collision_virtual_lane = collision_check.virtual_lane_col_check( *p_virtual_path, road_obstacles, car_state_.car_speed_, map_limit_speed_,
                                                           cur_pos_virtual_lane, collision_check_length_,
                                                           inner_free_length, out_longitudinal_speed_, out_longitudinal_length_, drive_mode_);

//    cout << "~~~~~~~~~~~~~~~~~ inner_collision_virtual_lane = " << inner_collision_virtual_lane << ", ~~~~ num = " << road_obstacles.size() << endl;
//    if ( inner_collision_virtual_lane < 0 ) inner_collision_virtual_lane = 0;
     double slow_down_step = COEF_SLOW_DOWN_STEP * out_longitudinal_speed_ / COUNT_COLLISION;
    /*如果是单车模式或车队头车，motion动态调节车速*/
    if ( !inner_collision_virtual_lane &&  car_action_ != CAR_ACTION_PLATOON_OTHER )
    {
        if ( count_col >= COUNT_COLLISION )
        {
            count_slow++;
            if ( count_slow < COUNT_SLOW )
            {
                out_longitudinal_speed_ -= COUNT_COLLISION * slow_down_step;
            }
            else
            {
                count_slow = 0;
                count_col = 0;
            }

        }
        else
        {
            count_col = 0;
        }
        is_slow_down_ = false;
//        output_motion_planning_speed( 1 );
    }

    //如果上一次规划停车，本次一定需要重新规划
    if ( is_stop_ )
    {
        route_.replan_flag = REPLAN_FLAG_PLANNING;
    }

    /***如果有碰撞，需要判断其他车道情况，来决定避障或停车***/
    if ( inner_collision_virtual_lane && car_action_ != CAR_ACTION_PLATOON_OTHER )
    {
        count_col++;
        if ( count_col <= COUNT_COLLISION )//inner_free_length > car_speed*3.6 )
        {
            //cout << "666666666666666666666666666666666 count_col = " << count_col << ", COUNT_COLLISION = " << COUNT_COLLISION << endl;
            out_longitudinal_speed_ -= count_col * slow_down_step;
            if ( out_longitudinal_speed_ < 0 )
                out_longitudinal_speed_ = 0;
//            output_motion_planning_speed( 1 );
            is_slow_down_ = true;

        }
        else{
        //count_col = 0;
        is_slow_down_ = false;
        /*如果有碰撞，肯定需要重新规划*/
        route_.replan_flag = REPLAN_FLAG_PLANNING;

        /*判断走哪个车道*/
//        expected_lane_ = car_in_which_line;
        double lane_free_length = 0.0, max_free_length = inner_free_length;
//        bool lane_change_send = false;

        for ( int i = car_in_which_line-1; i < car_in_which_line+2; i++ )
        {
            if ( i < 0 || i > num_paths-1 )
                continue;

            //各车道碰撞检测
            lane_free_length = collision_check.lane_col_check( smooth_paths_[ i ], road_obstacles, car_state_.car_speed_, cur_pos, collision_check_length_ );
            cout << "############## " << i << ", lane_free_length = " << lane_free_length << endl;
            //一旦发现有车道无碰撞，则向该车道换道
            if ( lane_free_length < 0.0 )
            {
                expected_lane_ = i;// - car_in_which_line + 1; //-1+1:left, 0+1:current, 1+1:right
//                lane_change_send = true;
                break;
            }
            //如果该车道有碰撞，评估碰撞距离
            if ( lane_free_length > max_free_length + FREE_LENGTH_DIFF )  //10.0***config
            {
                max_free_length = lane_free_length;
                expected_lane_ = i;
//                lane_change_send = true;
            }
        }
        //所有的车道都存在碰撞风险
//        if ( !lane_change_send )
//        {
//            all_lane_collision = true;
//        }

        }
    }//end if ( inner_collision_virtual_lane )

    /***储存碰撞检测结果***/
    set_collision_check_result( inner_free_length, inner_collision_virtual_lane );

    /***log***/
    if(COLLISION_CHECK_VIEW_SWITCH)
    {
       printf("\033[2J");
       cout << " cur_pos " << cur_pos << endl
            << " check_length " << collision_check_length_ << endl
            << " cur_pos_virtual_lane " << cur_pos_virtual_lane <<endl
//            << " end_pos_virtual_lane " << cur_pos_virtual_lane + check_length_num<<endl
            << " inner_free_length " << inner_free_length << endl
            << " inner_collision_virtual_lane " << inner_collision_virtual_lane << endl
            << " p_virtual_path " << p_virtual_path->ref_points.size() << endl;


     }

        return 1;
}

/**
    * @brief 设置碰撞检测结果，
    * @param  free_length 输入量：如果虚拟车道有碰撞，车辆到碰撞点之前的距离。
    * @param  collision_virtual_lane 输入量：规划产生的轨迹是否有碰撞。
    * @return 1表示设置碰撞检测结果成功，0表示失败
    */
    int Planning::set_collision_check_result( double free_length, bool collision_virtual_lane )
    {
        is_virtual_lane_collide_ = collision_virtual_lane;        ///<规划产生的轨迹是否有碰撞，1是0否
        virtual_lane_free_length_ = free_length;                ///<如果虚拟车道有碰撞，车辆到碰撞点之前的距离

        return 1;
    }

    /**
    * @brief 获得碰撞检测结果，
    * @param free_length 输出量：如果虚拟车道有碰撞，车辆到碰撞点之前的距离。
    * @param collision_virtual_lane 输出量：规划产生的轨迹是否有碰撞。
    * @return 1表示获得碰撞检测结果成功，0表示失败
    */
    int Planning::get_collision_check_result( double& free_length, bool& collision_virtual_lane )
    {
        collision_virtual_lane = is_virtual_lane_collide_;        ///<规划产生的轨迹是否有碰撞，1是0否
        free_length = virtual_lane_free_length_;                ///<如果虚拟车道有碰撞，车辆到碰撞点之前的距离

        return 1;
    }

    /**
    * @brief 获得规划生成的碰撞检测轨迹（用于界面显示），
    * @param collision_check_path 输出量：储存规划生成的碰撞检测轨迹。
    * @return 1表示获得轨迹成功，0表示失败
    */
    int Planning::get_collision_check_path( path& collision_check_path )
    {
        collision_check_path.ref_points.clear();
        collision_check_path.ref_points = collision_check_path_.ref_points;

        return 1;
    }


}   //namespace planning
}   //namespace athena



