#include "vehicle_dynamic/cau_heading_steering.h"
#include "map_matching/map_matching.h"
#include "planning/planning.h"
#include "common/enum_list.h"

using namespace std;

namespace athena{
namespace planning{

    Planning::Planning()
    {
        init();
    }

    Planning::~Planning()
    {

    }

    int Planning::init()
    {
//        pthread_spin_init(&m_map_lock, 0);
        out_longitudinal_speed_ = 0.0;                        ///<motion下发给controller的纵向速度
        out_longitudinal_length_ = -1.0;                      ///<多少米后达到速度out_longitudinal_speed_

        map_limit_speed_ = 0.0;                               ///<地图限速
        route_.route_clear();                                 ///<全局规划下发的地图切片信息
        road_obstacle_.obstacles_.clear();                    ///<路面障碍物信息

//        is_car_state_put_ = false;                          ///true：已输入车辆状态信息,false:没有
//        is_map_info_put_ = false;                           ///true：已输入地图信息,false:没有
//        is_stop_park_put_ = false;                          ///true：已输入停车点/泊车点信息,false:没有
//        is_obstacle_info_put_ = false;                      ///true：已输入障碍物信息,false:没有
        is_map_updated_ = false;                              ///<动态下发的地图是否更新。true：有更新
        is_park_ = false;                                     ///<是否需要泊车。true：需要泊车
        is_park_d_published_ = false;                         ///<泊车轨迹D档部分是否发送。true：已发送
        is_park_r_published_ = false;                         ///<泊车轨迹R档部分是否发送。true：已发送

        center_lines_.clear();                                ///<所有车道的道路中心线
        smooth_paths_.clear();                                ///<参考中心线：平滑插值后的道路中心线

        is_have_virtual_path_ = false;                        ///<表示是否已经有虚拟轨迹了，1是0否

        cross_road_status_ = 0;                               ///<路口信息，0为非路口    1为路口绿灯  2为在路口且为红灯

        is_virtual_lane_collide_ = false;                     ///<规划产生的轨迹是否有碰撞，1是0否
        virtual_lane_free_length_ = 0.0;                      ///<如果虚拟车道有碰撞，车辆到碰撞点之前的距离

        is_stop_ = false;                                     ///<需要停车
        is_slow_down_ = false;                                ///<需要减速

        collision_obj_speed_ = 0.0;                           ///<碰撞障碍物的运行速度
        collision_obj_heading_ = 0.0;                         ///<碰撞障碍物的运行方向

        car_in_lane_ = 0;                                     ///<本车在哪个车道，从左到右依次为0,1,2,...
        current_position_ = -1;                               ///<车辆在所在车道中心线上的位置
        end_position_ = -1;                                   ///<规划目标点位置（中心线上的点）
        map_matching_error_ = 0.0;                            ///<车辆匹配到地图中心线上的距离
        current_position_virtual_lane_ = -1;                  ///<车辆在虚拟车道上的位置
        start_position_virtual_lane_ = -1;                    ///<虚拟车道上规划的起始点

//        last_expected_lane_ = -1;                           ///<上一次期望车道
        expected_lane_ = -1;                                  ///<期望车道
//        last_hand_expected_lane_ = -1;                      ///<上一次手动期望车道
        hand_expected_lane_ = -1;                             ///<手动期望车道

//        change_lane_direction_ = 0;                         ///<换道方向
//        change_lane_reason_ = 0;                            ///<换道原因
//        last_change_lane_reason_ = 0;                       ///<上一次换道的原因
//        change_lane_starting_lane_ = 0;                     ///<换道开始的车道
        change_lane_ending_lane_ = 0;                         ///<换道结束的车道

        drive_mode_ = 0;
        car_action_ = 0;                                      ///车辆状态，1：单车，2：车队头车，3：车队其他车辆
        can_start_auto_ = 0;                                  ///启动自动驾驶
//        can_stop_auto_ = 0;                                   ///退出自动驾驶

        keep_last_length_ = 0.0;                              ///<保留上一次规划的长度，单位：米
        planning_length_ = 0.0;                               ///<本次规划的长度，单位：米
        tail_length_number_ = 0;                              ///<尾部拼接一段地图中心线点的个数，个
        collision_check_length_ = 0.0;                        ///<碰撞检测长度，单位：米

        next_planning_mileage_ = 0.0;                         ///<每走过多少里程更新一次轨迹，单位：米

        return 1;
    }

    int Planning::reset_value()
    {
//        pthread_spin_init(&m_map_lock, 0);
        out_longitudinal_speed_ = 0.0;                       ///<motion下发给controller的纵向速度

        map_limit_speed_ = 0.0;                              ///<地图限速
        route_.route_clear();                                ///<全局规划下发的地图切片信息
        //road_obstacle_.obstacles_.clear();                 ///<路面障碍物信息

//        is_car_state_put_ = false;                         ///true：已输入车辆状态信息,false:没有
//        is_map_info_put_ = false;                          ///true：已输入地图信息,false:没有
//        is_stop_park_put_ = false;                         ///true：已输入停车点/泊车点信息,false:没有
//        is_obstacle_info_put_ = false;                     ///true：已输入障碍物信息,false:没有
        is_map_updated_ = false;                             ///<动态下发的地图是否更新。true：有更新
        is_park_ = false;                                    ///<是否需要泊车。true：需要泊车
        is_park_d_published_ = false;                        ///<泊车轨迹D档部分是否发送。true：已发送
        is_park_r_published_ = false;                        ///<泊车轨迹R档部分是否发送。true：已发送

        center_lines_.clear();                               ///<所有车道的道路中心线
        smooth_paths_.clear();                               ///<参考中心线：平滑插值后的道路中心线

        is_have_virtual_path_ = false;                       ///<表示是否已经有虚拟轨迹了，1是0否

        cross_road_status_ = 0;                              ///<路口信息，0为非路口    1为路口绿灯  2为在路口且为红灯

        is_virtual_lane_collide_ = false;                    ///<规划产生的轨迹是否有碰撞，1是0否
        virtual_lane_free_length_ = 0.0;                     ///<如果虚拟车道有碰撞，车辆到碰撞点之前的距离

        is_stop_ = false;                                    ///<需要停车
        is_slow_down_ = false;                               ///<需要减速

        collision_obj_speed_ = 0.0;                          ///<碰撞障碍物的运行速度
        collision_obj_heading_ = 0.0;                        ///<碰撞障碍物的运行方向

        car_in_lane_ = 0;                                    ///<本车在哪个车道，从左到右依次为0,1,2,...
        current_position_ = -1;                              ///<车辆在所在车道中心线上的位置
        end_position_ = -1;                                  ///<规划目标点位置（中心线上的点）
        map_matching_error_ = 0.0;                           ///<车辆匹配到地图中心线上的距离
        current_position_virtual_lane_ = -1;                 ///<车辆在虚拟车道上的位置
        start_position_virtual_lane_ = -1;                   ///<虚拟车道上规划的起始点

//        last_expected_lane_ = -1;                          ///<上一次期望车道
        expected_lane_ = -1;                                 ///<期望车道
//        last_hand_expected_lane_ = -1;                     ///<上一次手动期望车道
        hand_expected_lane_ = -1;                            ///<手动期望车道

//        change_lane_direction_ = 0;                        ///<换道方向
//        change_lane_reason_ = 0;                           ///<换道原因
//        last_change_lane_reason_ = 0;                      ///<上一次换道的原因
//        change_lane_starting_lane_ = 0;                    ///<换道开始的车道
        change_lane_ending_lane_ = 0;                        ///<换道结束的车道

        car_action_ = 0;                                     ///车辆状态，1：单车，2：车队头车，3：车队其他车辆
//        can_start_auto_ = 0;                                 ///启动自动驾驶
//        can_stop_auto_ = 0;                                  ///退出自动驾驶

        keep_last_length_ = 0.0;                             ///<保留上一次规划的长度，单位：米
        planning_length_ = 0.0;                              ///<本次规划的长度，单位：米
        tail_length_number_ = 0;                             ///<尾部拼接一段地图中心线点的个数，个
        collision_check_length_ = 0.0;                       ///<碰撞检测长度，单位：米

        next_planning_mileage_ = 0.0;                        ///<每走过多少里程更新一次轨迹，单位：米

//        intelligentparking_.init();
        is_park_ = false;

        return 1;
    }

    /**
    * @brief 起点或退出自动驾驶
    * @param start 输入量：0起点1退出。
    * @return 1成功0失败。
    */
    int Planning::set_start_auto( bool start )
    {
        can_start_auto_ = start;

        return 1;
    }

    /**
    * @brief 输入轨迹，给virtual_path_赋值。
    * @param src_path 输入量：输入的轨迹。
    * @return 1表示成功，0表示失败
    */
    int Planning::put_virtual_path( const path& src_path )
    {
        virtual_path_.ref_points.clear();
        virtual_path_.ref_points = src_path.ref_points;

        return 1;
    }

    /**
    * @brief 接收地图切片信息，赋值给成员变量car_state_。
    * @param car_state 输入量：地图切片信息。
    * @return 1表示接收车辆状态信息成功，0表示失败
    */
    int Planning::put_car_state( const CarState& car_state )
    {
//        is_car_state_put_ = false;

        car_state_.car_pose_.CurrentX_ = car_state.car_pose_.CurrentX_;
        car_state_.car_pose_.CurrentY_ = car_state.car_pose_.CurrentY_;
        car_state_.car_pose_.CurrentZ_ = car_state.car_pose_.CurrentZ_;
        car_state_.car_pose_.Current_heading_ = car_state.car_pose_.Current_heading_;
        car_state_.steer_angle_ = car_state.steer_angle_;
        car_state_.at_status_ = car_state.at_status_;
        car_state_.car_speed_ = car_state.car_speed_;
        car_state_.CurrentS_ = car_state.CurrentS_;
        car_state_.GPS_time_ = car_state.GPS_time_;

//        is_car_state_put_ = true;

        return 1;
    }

    /**
    * @brief 接收地图切片信息，赋值给成员变量route_
    * @param src_route 输入量：地图切片信息。
    * @return 1表示接收地图切片信息成功，0表示失败
    */
    int Planning::put_map_info( const route_planning_m& src_route )
    {
//        is_map_info_put_ = false;
        if ( src_route.replan_flag == REPLAN_FLAG_NAVI )
        {
            reset_value();
            //某些路径规划直接出发自动自动驾驶
            if (src_route.route_reason == ROUTE_REASON_ADD_PLATOON ||
                src_route.route_reason == ROUTE_REASON_ADD_TAIL ||
                src_route.route_reason == ROUTE_REASON_JOIN_OBU ||
    //            src_route.route_reason == ROUTE_REASON_PSU_CALL_CAR ||
                src_route.route_reason == ROUTE_REASON_OCT_CALL_CAR )
    //            src_route.route_reason == ROUTE_REASON_REMOTE ||
    //            src_route.route_reason == ROUTE_REASON_MINIBUS)
            {
                can_start_auto_ = 1;
//                can_stop_auto_ = 0;

                printf("(1)handle_route_info: om_route_respond: route_reason=%d, can_start_auto=%d, replan_flag=%d\n",
                                                                src_route.route_reason, can_start_auto_, src_route.replan_flag );
            }

            if ( src_route.route_reason == ROUTE_REASON_VUI )
            {
                car_action_ = CAR_ACTION_SINGLE;

                printf("(2)handle_route_info: om_route_respond: route_reason=%d, car_action=%d\n",
                                                                src_route.route_reason, car_action_ );
            }

            //张毅: 编队则一律设置为跟车，后续obu会指定头车
            if (src_route.route_reason == ROUTE_REASON_ADD_PLATOON ||
                src_route.route_reason == ROUTE_REASON_ADD_TAIL ||
                src_route.route_reason == ROUTE_REASON_JOIN_OBU)
            {
                car_action_ = CAR_ACTION_PLATOON_OTHER;

                printf("(3)handle_route_info: om_route_respond: route_reason=%d, car_action=%d\n",
                                                                src_route.route_reason, car_action_ );
            }

            if ( src_route.route_reason == ROUTE_REASON_HEAD_CAR )
            {
                car_action_ = CAR_ACTION_PLATOON_HEAD;

                printf("(4)handle_route_info: om_route_respond: route_reason=%d, car_action=%d\n",
                                                                src_route.route_reason, car_action_ );
            }
        }

        route_.route_clear();   //清空
        route_ = src_route;     //赋值

        if ( compute_center_insert_from_route() )
        {
            smooth_center_lines();
        }

//        compute_center_insert_from_route();     //计算中心线
//
//        vector<path> temp_path;
//        temp_path.clear();
//        temp_path = center_lines_;
//
//        smooth_center_lines( temp_path, center_lines_ );


//        is_map_info_put_ = true;

        return 1;
    }

    /**
    * @brief 从切片信息中提取出各车道的中心线,赋值给center_lines_。
    * @return 1表示获得中心线信息成功，0表示失败
    */
    int Planning::compute_center_insert_from_route()
    {
        if ( compute_center_lines_from_route( route_, center_lines_ ) )
        {
            cout << "---- compute centerlines from route successfully : " << center_lines_.size() << endl;
            return 1;
        }

        return 0;
    }

    /**
    * @brief 从切片信息中提取出各车道的中心线。
    * @param route 输入量：切片信息。
    * @param center_lines 输出量：各车道的中心线。
    * @return 1表示获得中心线信息成功，0表示失败
    */
    int Planning::compute_center_lines_from_route( route_planning_m route, vector<path>& center_lines )
    {
        int32_t point_num = route.num_of_section;
        if ( point_num <= 0 )
            return 0;

        center_lines.clear();

        int8_t max_num = route.line[ 0 ].num_of_lane;
        for(int i = 1; i < point_num; i++)
        {
            if ( route.line[ i ].num_of_lane > max_num )
                max_num = route.line[ i ].num_of_lane;
        }

        navi_point center_point;
        path center_line;

        //接收数据，准备好匹配
        int index = 0;
        //double last_x, last_y;
        for ( int j = 0; j < max_num; j++ )
        {
            center_line.ref_points.clear();

            for( int i = 0; i < point_num; i++ )
            {
                if ( j > ( route.line[ i ].num_of_lane - 1 ) )
                    index = route.line[ i ].num_of_lane - 1;
                else
                    index = j;

                center_point.position_x = route.line[ i ].lane[ index ].x;
                center_point.position_y = route.line[ i ].lane[ index ].y;
                center_point.s = route.line[ i ].lane[ index ].mileage;
//                center_point.left_lane_num = index;
//                center_point.right_lane_num = route.line[ i ].num_of_lane - 1 - index;
                //储存切片类型
                center_point.type = route.line[ i ].lane[ index ].type;

                center_point.speed_desired_Uxs = route.line[ i ].lane[ index ].sug_speed / 3.6;
                center_point.heading = route.line[ i ].lane[ index ].yaw;

                center_line.ref_points.push_back( center_point );
    #if 0
    //zp20171013：在断头路附近重新计算头指向
                if ( i == 0 )
                    center_point.heading = route.line[ i ].lane[ index ].yaw;
                else
                    get_heading( last_x, last_y,
                                 center_point.position_x, center_point.position_y,
                                 center_point.heading );
                if ( center_point.heading == 0 )    //有重合点
                   center_point.heading = route.line[ i ].lane[ index ].yaw;

                center_line.ref_points.push_back( center_point );

    //zp20171013：在断头路附近重新计算头指向
                last_x = route.line[ i ].lane[ index ].x;
                last_y = route.line[ i ].lane[ index ].y;

                cout << center_point.heading << "  " << route.line[ i ].lane[ index ].yaw << endl;
    #endif //heading
            }
            center_lines.push_back( center_line );
        }


        //计算中心线上点的曲率
        for ( int i = 0; i < center_lines.size(); i++ )
        {
            for ( int j = 0; j < center_lines[i].ref_points.size()-2; j++ )
            {
            #if 1
                double x[3] = { center_lines[i].ref_points[j].position_x,
                                center_lines[i].ref_points[j+1].position_x,
                                center_lines[i].ref_points[j+2].position_x };

                double y[3] = { center_lines[i].ref_points[j].position_y,
                                center_lines[i].ref_points[j+1].position_y,
                                center_lines[i].ref_points[j+2].position_y };

                center_lines[i].ref_points[j].k_s = cau_ks_by_3_points( x, y );
            #endif // kappa

                /*log*/
                if(TRAFFIC_LIGHTS_LOG_SWITCH)
                {
                    ofstream outfile("traffic_lights.log", std::ios::app);
                    outfile.precision(8);

                        outfile << " i " << i
                                << " j " << j
                                << " type " << int (center_lines[ i ].ref_points[ j ].type)
                                << endl;

                    outfile << endl;
                    outfile.close();
                }

            }
        }

        return 1;
    }

    /**
    * @brief 对各车道的中心线进行三次样条平滑并内插，赋值给smooth_paths_。
    * @return 1表示平滑成功，0表示失败
    */
    int Planning::smooth_center_lines()
    {
        if ( smooth_center_lines( center_lines_, smooth_paths_ ) )
        {
            cout << "---- smooth center lines successfully " << endl;
            return 1;
        }

        return 0;
    }

    /**
    * @brief 对各车道的中心线进行三次样条平滑并内插。
    * @param center_lines 输入量：各车道的中心线。
    * @param smooth_paths 输出量：平滑内插后的中心线。
    * @return 1表示平滑成功，0表示失败
    */
    int Planning::smooth_center_lines( vector<path> center_lines, vector<path>& smooth_paths )
    {
        path temp_smooth_path;

        if ( center_lines.size() <= 0 )
            return 0;

        smooth_paths.clear();
        for ( int i = 0; i < center_lines.size(); i++ )
        {
            cau_all_output_from_map(
                                     center_lines[ i ],
                                     temp_smooth_path,
                                     center_lines[ i ].ref_points.size(),
                                     car_state_.car_speed_ );

            smooth_paths.push_back( temp_smooth_path );
        }


        return 1;
    }

    /**
    * @brief 接收路口红绿灯信息。
    * @param src_traffic_lights 输入量：路口红绿灯信息。
    * @return 1表示接收路口红绿灯信息成功，0表示失败
    */
    int Planning::put_traffic_lights( const TrafficLights& src_traffic_lights )
    {
        traffic_lights_ = src_traffic_lights;

        cout << " +++++ +++++ +++++ +++++ +++++ " << endl
             << " light_status_ " << traffic_lights_.light_status_ << endl
             << " crossing_status_ "<< int( traffic_lights_.crossing_status_ ) << endl
             << " +++++ +++++ +++++ +++++ +++++ " << endl;

        if ( !traffic_lights_.crossing_status_ )
            cross_road_status_ = NOT_AT_CROSSING;

        if ( traffic_lights_.light_status_ != LIGHT_STATUS_NONE && traffic_lights_.light_status_ != LIGHT_STATUS_GREEN &&
             cross_road_status_ != AT_CROSSING_RED && traffic_lights_.crossing_status_ )
        {
           cross_road_status_ = AT_CROSSING_RED;  //在路口，是红灯
           route_.replan_flag = REPLAN_FLAG_PLANNING;
        }

        if ( traffic_lights_.light_status_ == LIGHT_STATUS_GREEN && traffic_lights_.crossing_status_ )
        {
           cross_road_status_ = AT_CROSSING_GREEN;  //在路口
           route_.replan_flag = REPLAN_FLAG_PLANNING;
        }

        return 1;
    }

    /**
    * @brief 接收停车点/泊车点信息。
    * @param src_stop_park 输入量：停车点/泊车点信息。
    * @param stop_flag 输入量：10终点停车，否则泊车。
    * @return 1表示接收停车点/泊车点信息成功，0表示失败
    */
    int Planning::put_stop_park( const StopPark& src_stop_park, const int& stop_flag )
    {
//        is_stop_park_put_ = false;

        if ( stop_flag == STOP_TYPE_TERMINAL )
        {
            terminal_stop_ = src_stop_park;
        }
        else
        {
            intelligentparking_.set_park_point( src_stop_park );
        }

//        is_stop_park_put_ = true;

        return 1;
    }

    /**
    * @brief 接收障碍物信息。
    * @param src_obstacle 输入量：障碍物信息。
    * @return 1表示接收障碍物信息成功，0表示失败
    */
    int Planning::put_obstacles_info( const SensorObstacles& src_obstacle )
    {
//        is_obstacle_info_put_ = false;

        sensor_obstacles_.num_of_obstacle_ = src_obstacle.num_of_obstacle_;
        sensor_obstacles_.obstacles_info_.clear();
        sensor_obstacles_.obstacles_info_ = src_obstacle.obstacles_info_;

//        for ( int i = 0; i < sensor_obstacles_.num_of_obstacle_; i++ )
//        {
//            cout << i << " <<<<< x = " << sensor_obstacles_.obstacles_info_[i].x_ << ", y = " << sensor_obstacles_.obstacles_info_[i].y_ << endl;
//            cout << i << " >>>>> x = " << src_obstacle.obstacles_info_[i].x_ << ", y = " << src_obstacle.obstacles_info_[i].y_ << endl;
//        }

//        is_obstacle_info_put_ = true;

        return 1;
    }

    /**
    * @brief 过滤障碍物信息，通过地图边界信息过滤掉道路外的障碍物。赋值给road_obstacle_。
    * @param filter_obstacle_flag 输入量：障碍物过滤方式。
    * @return 1表示接收障碍物信息成功，0表示失败
    */
    int Planning::filter_obstacles_info( int filter_obstacle_flag )
    {
#if 1
        int num_objs = sensor_obstacles_.num_of_obstacle_;
        if( num_objs < 0 )
            return 0;

//        double car_x = car_state_.CurrentX_;
//        double car_y = car_state_.CurrentY_;
//        double car_h = car_state_.Current_heading_;

        int temp_cur_pos = -1;
//        double min_error_curr = 0.0;


        int begin_position = 0;
        int end_position = route_.left_line.line.size();

        temp_cur_pos = current_position_;
        if ( temp_cur_pos >= 0 )
        {
            begin_position = temp_cur_pos - NUMBER_BACKWARD;
            if ( begin_position < 0 )
                begin_position = 0;
            end_position = temp_cur_pos + NUMBER_FORWARD;
            if ( end_position > route_.left_line.line.size() )
                end_position = route_.left_line.line.size();
        }

        road_obstacle_.obstacles_.clear();
        for( int i = 0; i < num_objs; i++ )
        {
//            cout << ">>>>>>>>> " << i << ", x = " << sensor_obstacles_.obstacles_info_[i].x_ << ", y = " << sensor_obstacles_.obstacles_info_[i].y_ << endl;
            ObstacleRect obj;
            bool is_valid = false;

            if ( filter_obstacle_flag == FILTER_OBSTACLE_NOT )
            {
                is_valid = filter_obstacle_by_nothing( sensor_obstacles_.obstacles_info_[i], obj );
            }
            if ( filter_obstacle_flag == FILTER_OBSTACLE_POINT )
            {
                is_valid = filter_obstacle_by_point( sensor_obstacles_.obstacles_info_[i], begin_position, end_position, obj );
            }
            if ( filter_obstacle_flag == FILTER_OBSTACLE_RECT )
            {
                is_valid = filter_obstacle_by_rect( sensor_obstacles_.obstacles_info_[i], begin_position, end_position, obj );
            }

            //障碍物赋值
            if ( is_valid )
                road_obstacle_.obstacles_.push_back( obj );

        }
//        cout << "$$$$$$$$$$$$$$$$$$ current_position_ = " << current_position_ << " , num_objs = " << num_objs
//             << " , num = " << road_obstacle_.obstacles_.size() << endl;

#endif // 0

        if ( OBSTACLES_LOG_SWITCH )
        {
            ofstream outfile("obstacles_filter.log", std::ios::app);
            outfile.precision(8);

            outfile << " num_of_obstacle : " << road_obstacle_.obstacles_.size() << endl;

            for ( int i = 0; i < road_obstacle_.obstacles_.size(); i++ )
            {
                double x_obs = 0.5 * ( road_obstacle_.obstacles_[i].rt_.p1.xx + road_obstacle_.obstacles_[i].rt_.p3.xx );
                double y_obs = 0.5 * ( road_obstacle_.obstacles_[i].rt_.p1.yy + road_obstacle_.obstacles_[i].rt_.p3.yy );

                outfile << " i " << i
                        << " value " << road_obstacle_.obstacles_[i].value_
                        << " type " << road_obstacle_.obstacles_[i].type_
                        << " x " << x_obs
                        << " y " << y_obs
                        << " width " << road_obstacle_.obstacles_[i].rt_.width
                        << " height " << road_obstacle_.obstacles_[i].rt_.height
                        << " yaw " << road_obstacle_.obstacles_[i].yaw_
                        << " speed " << road_obstacle_.obstacles_[i].v_
                        << endl;
            }

            outfile << endl << endl;
            outfile.close();
        }

        return 1;
    }

    /**
    * @brief 根据路边界和障碍物中心点，过滤掉不在道路范围内的障碍物_。
    * @param sensor_obstacle 输入量：传感器输出的障碍物信息。
    * @param out_obstacle 输出量：过滤后的障碍物信息。
    * @return 1表示成功，0表示失败
    */
    bool Planning::filter_obstacle_by_nothing( const ObstacleInfo sensor_obstacle, ObstacleRect& out_obstacle )
    {
        out_obstacle.source_ = sensor_obstacle.source_;

        point_xys obstacle_point;
//        int32_t obs_id;

        obstacle_point.x = sensor_obstacle.x_;
        obstacle_point.y = sensor_obstacle.y_;

        double width = sensor_obstacle.width_;
        double height = sensor_obstacle.height_;

        double delta_x = obstacle_point.x - car_state_.car_pose_.CurrentX_;
        double delta_y = obstacle_point.y - car_state_.car_pose_.CurrentY_;
        double car_Ux = cos( to_radians( car_state_.car_pose_.Current_heading_ ) );
        double car_Uy = sin( to_radians( car_state_.car_pose_.Current_heading_ ) );
        double dis_o2c = sqrt( delta_x * delta_x + delta_y * delta_y );
        double cos_angle = ( delta_x * car_Ux + delta_y * car_Uy ) / dis_o2c;

        ///zp20180128:如果是道路内的行人，安全保护距离加大
        if ( sensor_obstacle.type_ == OBSTACLE_TYPE_PEDESTRIAN  && dis_o2c > PEDESTRIAN_WIDEN_DIS && cos_angle > PEDESTRIAN_WIDEN_ANG )
        {
            width += PEDESTRIAN_WIDEN_WIDTH;
        }
        if ( sensor_obstacle.type_ == OBSTACLE_TYPE_CAR )
        {
            height += CAR_LENGTHEN_LENGTH;
        }

        if ( B_READ_OBSTACLE_SPEED )
            out_obstacle.v_ = sensor_obstacle.speed_;
        else
            out_obstacle.v_ = 0.0;
        out_obstacle.yaw_ = sensor_obstacle.yaw_;
        out_obstacle.rt_.width = width;
        out_obstacle.rt_.height = height;
        out_obstacle.value_ = OBSTACLE_COST_VALUE;  // 障碍物碰撞代价值 3000
        out_obstacle.type_ = sensor_obstacle.type_;

        double direction_d = to_radians(out_obstacle.yaw_);
        double cos_d = cos(direction_d);
        double sin_d = sin(direction_d);

        out_obstacle.rt_.p1.xx  = sensor_obstacle.x_ + width /2.0 * cos_d;
        out_obstacle.rt_.p1.yy  = sensor_obstacle.y_ - width /2.0 * sin_d;

        out_obstacle.rt_.p2.xx  = sensor_obstacle.x_ - width /2.0 * cos_d;
        out_obstacle.rt_.p2.yy  = sensor_obstacle.y_ + width /2.0 * sin_d;

        out_obstacle.rt_.p3.xx  = sensor_obstacle.x_ - width /2.0 * cos_d + height *sin_d;
        out_obstacle.rt_.p3.yy  = sensor_obstacle.y_ + width /2.0 * sin_d + height *cos_d;

        out_obstacle.rt_.p4.xx  = sensor_obstacle.x_ + width /2.0 * cos_d + height *sin_d;
        out_obstacle.rt_.p4.yy  = sensor_obstacle.y_ - width /2.0 * sin_d + height *cos_d;

        return true;
    }

    /**
    * @brief 根据路边界和障碍物中心点，过滤掉不在道路范围内的障碍物_。
    * @param sensor_obstacle 输入量：传感器输出的障碍物信息。
    * @param _begin 输入量：障碍物过滤的范围（起始位置）。
    * @param _end 输入量：障碍物过滤的范围（终止位置）。
    * @param out_obstacle 输出量：过滤后的障碍物信息。
    * @return 1表示障碍物在道路范围内，0表示不在
    */
    bool Planning::filter_obstacle_by_point( const ObstacleInfo sensor_obstacle, int _begin, int _end, ObstacleRect& out_obstacle )
    {
        out_obstacle.source_ = sensor_obstacle.source_;

        double width = sensor_obstacle.width_;
        double height = sensor_obstacle.height_;
        bool ret = false;

        point_xys obstacle_point;
//        int32_t obs_id;

        obstacle_point.x = sensor_obstacle.x_;
        obstacle_point.y = sensor_obstacle.y_;
//        obs_id = sensor_obstacle.id_;

        ///判断障碍物是否在道路边界线内
        ret = route_.check_point_in_lane_edge( obstacle_point, _begin, _end );

        if ( !ret )
        {
            return false;
        }

        double delta_x = obstacle_point.x - car_state_.car_pose_.CurrentX_;
        double delta_y = obstacle_point.y - car_state_.car_pose_.CurrentY_;
        double car_Ux = cos( to_radians( car_state_.car_pose_.Current_heading_ ) );
        double car_Uy = sin( to_radians( car_state_.car_pose_.Current_heading_ ) );
        double dis_o2c = sqrt( delta_x * delta_x + delta_y * delta_y );
        double cos_angle = ( delta_x * car_Ux + delta_y * car_Uy ) / dis_o2c;

        ///zp20180128:如果是道路内的行人，安全保护距离加大
        if ( sensor_obstacle.type_ == OBSTACLE_TYPE_PEDESTRIAN  && dis_o2c > PEDESTRIAN_WIDEN_DIS && cos_angle > PEDESTRIAN_WIDEN_ANG )
        {
            width += PEDESTRIAN_WIDEN_WIDTH;
        }
        if ( sensor_obstacle.type_ == OBSTACLE_TYPE_CAR )
        {
            height += CAR_LENGTHEN_LENGTH;
        }

        if ( B_READ_OBSTACLE_SPEED )
            out_obstacle.v_ = sensor_obstacle.speed_;
        else
            out_obstacle.v_ = 0.0;
        out_obstacle.yaw_ = sensor_obstacle.yaw_;
        out_obstacle.rt_.width = width;
        out_obstacle.rt_.height = height;
        out_obstacle.value_ = OBSTACLE_COST_VALUE;  // 障碍物碰撞代价值 3000
        out_obstacle.type_ = sensor_obstacle.type_;

        double direction_d = to_radians(out_obstacle.yaw_);
        double cos_d = cos(direction_d);
        double sin_d = sin(direction_d);

        out_obstacle.rt_.p1.xx  = sensor_obstacle.x_ + width /2.0 * cos_d;
        out_obstacle.rt_.p1.yy  = sensor_obstacle.y_ - width /2.0 * sin_d;

        out_obstacle.rt_.p2.xx  = sensor_obstacle.x_ - width /2.0 * cos_d;
        out_obstacle.rt_.p2.yy  = sensor_obstacle.y_ + width /2.0 * sin_d;

        out_obstacle.rt_.p3.xx  = sensor_obstacle.x_ - width /2.0 * cos_d + height *sin_d;
        out_obstacle.rt_.p3.yy  = sensor_obstacle.y_ + width /2.0 * sin_d + height *cos_d;

        out_obstacle.rt_.p4.xx  = sensor_obstacle.x_ + width /2.0 * cos_d + height *sin_d;
        out_obstacle.rt_.p4.yy  = sensor_obstacle.y_ - width /2.0 * sin_d + height *cos_d;

        return true;
    }

    /**
    * @brief 根据路边界和障碍物矩形框，过滤掉不在道路范围内的障碍物_。
    * @param sensor_obstacle 输入量：传感器输出的障碍物信息。
    * @param _begin 输入量：障碍物过滤的范围（起始位置）。
    * @param _end 输入量：障碍物过滤的范围（终止位置）。
    * @param out_obstacle 输出量：过滤后的障碍物信息。
    * @return 1表示障碍物在道路范围内，0表示不在
    */
    bool Planning::filter_obstacle_by_rect( const ObstacleInfo sensor_obstacle, int _begin, int _end, ObstacleRect& out_obstacle )
    {
        out_obstacle.source_ = sensor_obstacle.source_;

        double width = sensor_obstacle.width_;
        double height = sensor_obstacle.height_;
        bool ret = false;

        point_xys obstacle_point;
//        int32_t obs_id;

        obstacle_point.x = sensor_obstacle.x_;
        obstacle_point.y = sensor_obstacle.y_;
//        obs_id = sensor_obstacle.id_;
        out_obstacle.yaw_ = sensor_obstacle.yaw_;
        out_obstacle.rt_.width = width;
        out_obstacle.rt_.height = height;

        double direction_d = to_radians(out_obstacle.yaw_);
        double cos_d = cos(direction_d);
        double sin_d = sin(direction_d);

        ///构建障碍物矩形框
        out_obstacle.rt_.p1.xx  = sensor_obstacle.x_ + width /2.0 * cos_d;
        out_obstacle.rt_.p1.yy  = sensor_obstacle.y_ - width /2.0 * sin_d;

        out_obstacle.rt_.p2.xx  = sensor_obstacle.x_ - width /2.0 * cos_d;
        out_obstacle.rt_.p2.yy  = sensor_obstacle.y_ + width /2.0 * sin_d;

        out_obstacle.rt_.p3.xx  = sensor_obstacle.x_ - width /2.0 * cos_d + height *sin_d;
        out_obstacle.rt_.p3.yy  = sensor_obstacle.y_ + width /2.0 * sin_d + height *cos_d;

        out_obstacle.rt_.p4.xx  = sensor_obstacle.x_ + width /2.0 * cos_d + height *sin_d;
        out_obstacle.rt_.p4.yy  = sensor_obstacle.y_ - width /2.0 * sin_d + height *cos_d;

        ///判断障碍物是否在道路边界线内
        ret = route_.check_rect_in_lane_edge( out_obstacle.rt_, _begin, _end );

        if ( !ret )
        {
            return false;
        }

        double delta_x = obstacle_point.x - car_state_.car_pose_.CurrentX_;
        double delta_y = obstacle_point.y - car_state_.car_pose_.CurrentY_;
        double car_Ux = cos( to_radians( car_state_.car_pose_.Current_heading_ ) );
        double car_Uy = sin( to_radians( car_state_.car_pose_.Current_heading_ ) );
        double dis_o2c = sqrt( delta_x * delta_x + delta_y * delta_y );
        double cos_angle = ( delta_x * car_Ux + delta_y * car_Uy ) / dis_o2c;

        ///zp20180128:如果是道路内的行人，安全保护距离加大
        if ( sensor_obstacle.type_ == OBSTACLE_TYPE_PEDESTRIAN  && dis_o2c > PEDESTRIAN_WIDEN_DIS && cos_angle > PEDESTRIAN_WIDEN_ANG )
        {
            width += PEDESTRIAN_WIDEN_WIDTH;

            ///重新构建障碍物矩形框
            out_obstacle.rt_.p1.xx  = sensor_obstacle.x_ + width /2.0 * cos_d;
            out_obstacle.rt_.p1.yy  = sensor_obstacle.y_ - width /2.0 * sin_d;

            out_obstacle.rt_.p2.xx  = sensor_obstacle.x_ - width /2.0 * cos_d;
            out_obstacle.rt_.p2.yy  = sensor_obstacle.y_ + width /2.0 * sin_d;

            out_obstacle.rt_.p3.xx  = sensor_obstacle.x_ - width /2.0 * cos_d + height *sin_d;
            out_obstacle.rt_.p3.yy  = sensor_obstacle.y_ + width /2.0 * sin_d + height *cos_d;

            out_obstacle.rt_.p4.xx  = sensor_obstacle.x_ + width /2.0 * cos_d + height *sin_d;
            out_obstacle.rt_.p4.yy  = sensor_obstacle.y_ - width /2.0 * sin_d + height *cos_d;
        }
        if ( sensor_obstacle.type_ == OBSTACLE_TYPE_CAR )
        {
            height += CAR_LENGTHEN_LENGTH;

            ///重新构建障碍物矩形框
            out_obstacle.rt_.p3.xx  = sensor_obstacle.x_ - width /2.0 * cos_d + height *sin_d;
            out_obstacle.rt_.p3.yy  = sensor_obstacle.y_ + width /2.0 * sin_d + height *cos_d;

            out_obstacle.rt_.p4.xx  = sensor_obstacle.x_ + width /2.0 * cos_d + height *sin_d;
            out_obstacle.rt_.p4.yy  = sensor_obstacle.y_ - width /2.0 * sin_d + height *cos_d;
        }

        if ( B_READ_OBSTACLE_SPEED )
            out_obstacle.v_ = sensor_obstacle.speed_;
        else
            out_obstacle.v_ = 0.0;

        out_obstacle.value_ = OBSTACLE_COST_VALUE;  // 障碍物碰撞代价值 3000
        out_obstacle.type_ = sensor_obstacle.type_;


        return true;
    }

    /**
    * @brief 判断是否需要进行泊车，
    * @return 1表示需要泊车，0表示不需要
    */
    int Planning::park_decision()
    {
        if ( intelligentparking_.width_ < 0 )
        {
            is_park_ = false;                             ///<是否需要泊车。true：需要泊车
            return 0;
        }
        else
        {
            double dx = intelligentparking_.x_ - car_state_.car_pose_.CurrentX_;
            double dy = intelligentparking_.y_ - car_state_.car_pose_.CurrentY_;
            double distance = sqrt( dx*dx + dy*dy );
//            cout << "-------- distance = " << distance << ", car_state_.car_speed_ = " << car_state_.car_speed_ << endl;
            if ( distance < THRESHOLD_START_PARK && car_state_.car_speed_ < THRESHOLD_CAR_STATIC_SPEED )
            {
                is_park_ = true;
                return 1;
            }
            else
            {
                is_park_ = false;
                return 0;
            }

        }

    }

    /**
    * @brief 生成泊车轨迹，
    * @return 1表示成功，0表示失败
    */
    int Planning::generate_park_trajectory()
    {
        path temp_path;

        ///生成轨迹
        intelligentparking_.compute_parking_trajectory( car_state_ );

        ///D档部分
        intelligentparking_.get_trajectory_d( park_trajectory_d_ );
        intelligentparking_.extend_trajectory( park_trajectory_d_, NUM_EXTEND_TRAJECTORY, 0.1, AT_STATUS_D );
//        temp_path.ref_points.clear();
//        temp_path.ref_points = park_trajectory_d_.ref_points;
//        park_trajectory_d_.ref_points.clear();
//        park_trajectory_d_ = translate_path( temp_path, 0.5*WHEEL_BASE );

        ///R档部分
        intelligentparking_.get_trajectory_r( park_trajectory_r_ );
        intelligentparking_.extend_trajectory( park_trajectory_r_, NUM_EXTEND_TRAJECTORY, 0.1, AT_STATUS_R );
//        temp_path.ref_points.clear();
//        temp_path.ref_points = park_trajectory_r_.ref_points;
//        park_trajectory_r_.ref_points.clear();
//        park_trajectory_r_ = translate_path( temp_path, 0.5*WHEEL_BASE );

        return 1;
    }

    /**
    * @brief 泊车轨迹地图匹配
    * @return 地图匹配结果（车当前状态匹配到泊车轨迹上的点号），返回-1表示匹配失败
    */
    int Planning::park_trajectory_map_matching()
    {
        path virtual_path;
        map_matching m_matching_virtual_lane;
        double min_error_virtual = MAXIMUM_VALUE;
        int inner_cur_pos_vritual_lane = -1;

        if( virtual_path_.ref_points.size() > 0 )
        {
            virtual_path = virtual_path_;

            m_matching_virtual_lane.init();

            inner_cur_pos_vritual_lane = m_matching_virtual_lane.MapMarch_Min_Distance_mapping(
                                                                    car_state_.car_pose_.CurrentX_,
                                                                    car_state_.car_pose_.CurrentY_,
                                                                    virtual_path,
                                                                    virtual_path.ref_points.size(),
                                                                    min_error_virtual);
        }
        else
        {
            cout << "Planning::park_trajectory_map_matching : virtual_path_ is empty" << endl;
            return -1;
        }

        return inner_cur_pos_vritual_lane;
    }

    /**
    * @brief 泊车轨迹碰撞检测
    * @param num_start 输入量：碰撞检测的起始位置，由地图匹配获得。
    * @return 1表示有碰撞，0表示无
    */
    int Planning::park_trajectory_collision_check( int num_start )
    {
        if ( num_start < 0 )
            return 1;

        CollisionCheck collision_check;
        int is_collided = 1;

        vector<ObstacleRect> road_obstacles;
        road_obstacles.clear();
        filter_obstacles_info( FILTER_OBSTACLE_NOT );
        road_obstacles = road_obstacle_.obstacles_;                          //获得路面障碍物情况
        path virtual_path = virtual_path_;                  //获得规划的虚拟轨迹
//        cout << "--------------- road_obstacles.size : " << road_obstacles.size() << endl;
        if ( virtual_path.ref_points.size() <= 0 )
        {
            return 0;
        }
        is_collided = collision_check.park_col_check( virtual_path, road_obstacles, num_start, WHEEL_BASE, 10, SAFE_WIDTH );

        return is_collided;
    }

    /**
    * @brief 手动拨杆换道
    * @param direction 输入量：换道方向，-1左，1右，0不换道。
    * @return 1表示成功，0表示失败
    */
    int Planning::decide_hand_expected_lane( int direction )
    {
        if ( direction == 0 )
        {
            hand_expected_lane_ = -1;
        }
        else
        {
            hand_expected_lane_ = direction + car_in_lane_;
            if ( hand_expected_lane_ < 0 || hand_expected_lane_ > center_lines_.size()-1 )
            {
                hand_expected_lane_ = car_in_lane_;
            }
        }

        return 1;
    }

    /**
    * @brief 平移path
    * @param src_path 输入量：平移前path。
    * @param translate_length 输入量：平移长度。
    * @return 平移后path。
    */
    path Planning::translate_path( const path& src_path, const double& translate_length )
    {
        path out_path;
        out_path.ref_points.clear();
        navi_point temp_point;
        for ( auto point : src_path.ref_points )
        {
            temp_point.k_s = point.k_s;
            temp_point.heading = point.heading;
            temp_point.speed_desired_Uxs = point.speed_desired_Uxs;
            temp_point.acceleration_desired_Axs = point.acceleration_desired_Axs;
            temp_point.s = point.s;
            temp_point.gears = point.gears;

//            double yaw = intelligentparking_.heading_ + 180.0;
//            temp_point.position_x = point.position_x + translate_length * sin( to_radians( yaw ) );
//            temp_point.position_y = point.position_y + translate_length * cos( to_radians( yaw ) );
            temp_point.position_x = point.position_x + translate_length * sin( to_radians( point.heading ) );
            temp_point.position_y = point.position_y + translate_length * cos( to_radians( point.heading ) );

            out_path.ref_points.push_back( temp_point );
        }
        return out_path;
    }



}   //namespace planning
}   //namespace athena



