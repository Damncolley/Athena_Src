#include "planning_node.h"
#include "common/cs.h"
//#include "planning/planning_param.h"
#include "config.h"

/// constructor
PlanningNode::PlanningNode()
    : planning_url_( OBU_URL )
    , is_route_set_( false )
{
    init();

}

/// destructor
PlanningNode::~PlanningNode()
{

}

/// initializer
void PlanningNode::init()
{
    ///init lcm msg
    read_motion_plan_config_value_from_file();
    lcm_ins_ = new lcm::LCM( OBU_URL );
    lcm_can_ = new lcm::LCM( OBU_URL );
    lcm_route_ = new lcm::LCM( OBU_URL );
    lcm_map_ = new lcm::LCM( OBU_URL );
    lcm_obstacle_ = new lcm::LCM( OBU_URL );
    lcm_publish_ = new lcm::LCM( OBU_URL );

    ///thread
    std::thread thread_ins( &PlanningNode::ThreadFunction_ins, this );
    std::thread thread_can( &PlanningNode::ThreadFunction_can, this );
    std::thread thread_route( &PlanningNode::ThreadFunction_route, this );
    std::thread thread_map( &PlanningNode::ThreadFunction_map, this );
    std::thread thread_obstacle( &PlanningNode::ThreadFunction_obstacle, this );

    thread_ins.detach();
    thread_can.detach();
    thread_route.detach();
    thread_map.detach();
    thread_obstacle.detach();

    hand_direction_ = 0;
    last_hand_direction_ = 0;

}

void PlanningNode::run()
{
    static bool is_col_last_d = false;
    static bool is_col_last_r = false;
    int cur_pos;
    double view_step = 2.5;

    while( 1 )
    {
        ///car state
        planning_.put_car_state( car_state_ );

        ///map route
        if ( is_route_set_ )
        {
            planning_.put_map_info( route_ );
            planning_.is_map_updated_ = true;
            is_route_set_ = false;
        }


        ///planning
        if ( !planning_.is_park_ )
        {
            planning_.park_decision();
        }
//        cout << "planning_.is_park_ = " << planning_.is_park_ << endl;

        if ( planning_.is_park_ )
        {
            if ( !planning_.is_park_r_published_ )
            {
        //            path park_trajectory_d, park_trajectory_r;
                if ( !planning_.is_park_d_published_ )
                {
                    ///vui提示
                    publish_alarm( ALARM_ADVISE, "开始泊车" );
                    cout << "----- 开始泊车 -----" << endl;
                    usleep( 1000000 );

                    planning_.generate_park_trajectory();

                    ///下发D档部分
                    planning_.put_virtual_path( planning_.park_trajectory_d_ );
                    if ( planning_.get_out_trajectory() )
                    {
                        ///publish
                        publish_trajectory( planning_.out_trajectory_ );

                        publish_view_path( planning_.out_trajectory_, view_step );
                    }
                    planning_.is_park_d_published_ = true;
                }

                if ( !planning_.is_park_d_published_ )
                    continue;

                cur_pos = planning_.park_trajectory_map_matching();
                int is_collided = planning_.park_trajectory_collision_check( cur_pos );
                if ( is_collided && !is_col_last_d )
                {
                    is_col_last_d = true;
                    ///停车
                    planning_.set_trajectory_longitudinal( cur_pos+1, 0.0, AT_STATUS_P );

                    publish_trajectory( planning_.out_trajectory_ );

                    publish_view_path( planning_.out_trajectory_, view_step );

                }
                if ( is_col_last_d && !is_collided )
                {
                    is_col_last_d = false;
                    ///重新起步
        //                planning_.set_trajectory_longitudinal( 0, 5.0/3.6, AT_STATUS_D );
                    planning_.get_out_trajectory();

                    publish_trajectory( planning_.out_trajectory_ );

                    publish_view_path( planning_.out_trajectory_, view_step );

                }
            }


        //            cout << "-----------------------------------------------------------------------" << endl;
        //            cout << "delta = " << fabs( planning_.intelligentparking_.get_size_trajectory_d() - cur_pos ) << endl;
        //            cout << "cur_pos = " << cur_pos << endl;
        //            cout << "planning_.car_state_.car_speed_ = " << planning_.car_state_.car_speed_ << endl;
        //            cout << "planning_.is_park_r_published_ = " << planning_.is_park_r_published_ << endl;
            ///判断到达开始倒车的点
            if ( fabs( planning_.intelligentparking_.get_size_trajectory_d() - cur_pos ) < 15
              && planning_.car_state_.car_speed_  < THRESHOLD_CAR_STATIC_SPEED
              && !planning_.is_park_r_published_ )
            {
                ///vui提示
                publish_alarm( ALARM_ADVISE, "开始倒车，请注意！" );
                cout << "----- 开始倒车，请注意！ -----" << endl;
                usleep( 1000000 );

                planning_.put_virtual_path( planning_.park_trajectory_r_ );
                if ( planning_.get_out_trajectory() )
                {
                    ///publish
                    publish_trajectory( planning_.out_trajectory_ );

                    publish_view_path( planning_.out_trajectory_, view_step );
                }
                planning_.is_park_r_published_ = true;

            }

            if ( !planning_.is_park_r_published_ )
                continue;

            int cur_pos_r = planning_.park_trajectory_map_matching();
            int is_collided_r = planning_.park_trajectory_collision_check( cur_pos_r );
        //            cout << "-------- cur_pos_r = " << cur_pos_r << ", ---- is_collided_r = " << is_collided_r << endl;
            if ( is_collided_r && !is_col_last_r )
            {
                is_col_last_r = true;
                ///停车
                planning_.set_trajectory_longitudinal( cur_pos_r+1, 0.0, AT_STATUS_P );

                publish_trajectory( planning_.out_trajectory_ );

                publish_view_path( planning_.out_trajectory_, view_step );
                cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!! stop !!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
            }
            if ( is_col_last_r && !is_collided_r )
            {
                is_col_last_r = false;
                ///重新起步
                //planning_.set_trajectory_longitudinal( 0, 5.0/3.6, AT_STATUS_R );
                planning_.get_out_trajectory();

                publish_trajectory( planning_.out_trajectory_ );

                publish_view_path( planning_.out_trajectory_, view_step );

            }

            if ( cur_pos_r >= planning_.intelligentparking_.get_size_trajectory_r()-15 )
            {
                planning_.intelligentparking_.init();
            }

            usleep( 50000 );

        }
        else
        {
            planning_.map_matching_thread();

            planning_.behaviour_decision_thread();

            if ( planning_.motion_plan_virtual_lane_thread() )
            {
                if ( planning_.get_out_trajectory() )
                {
                    ///publish
                    publish_trajectory( planning_.out_trajectory_ );

                    path show_path;
                    planning_.get_collision_check_path( show_path );
                    publish_view_path( show_path, view_step );
                }
            }
        }

         usleep( 50000 );
    }



}

int PlanningNode::park_planning( athena::planning::Planning &planning, double view_step,
                                 bool is_col_last_d, bool is_col_last_r )
{
    int cur_pos;

    if ( !planning.is_park_r_published_ )
    {
//            path park_trajectory_d, park_trajectory_r;
        if ( !planning.is_park_d_published_ )
        {
            ///vui提示
            publish_alarm( ALARM_ADVISE, "开始泊车" );
            cout << "----- 开始泊车 -----" << endl;
            usleep( 1000000 );

            planning.generate_park_trajectory();

            ///下发D档部分
            planning.put_virtual_path( planning.park_trajectory_d_ );
            if ( planning.get_out_trajectory() )
            {
                ///publish
                publish_trajectory( planning.out_trajectory_ );

                publish_view_path( planning.out_trajectory_, view_step );
            }
            planning.is_park_d_published_ = true;
        }

        if ( !planning.is_park_d_published_ )
            return 0;

        cur_pos = planning.park_trajectory_map_matching();
        int is_collided = planning.park_trajectory_collision_check( cur_pos );
        if ( is_collided && !is_col_last_d )
        {
            is_col_last_d = true;
            ///停车
            planning.set_trajectory_longitudinal( cur_pos+1, 0.0, AT_STATUS_P );

            publish_trajectory( planning.out_trajectory_ );

            publish_view_path( planning.out_trajectory_, view_step );

        }
        if ( is_col_last_d && !is_collided )
        {
            is_col_last_d = false;
            ///重新起步
//                planning.set_trajectory_longitudinal( 0, 5.0/3.6, AT_STATUS_D );
            planning.get_out_trajectory();

            publish_trajectory( planning.out_trajectory_ );

            publish_view_path( planning.out_trajectory_, view_step );

        }
    }


//            cout << "-----------------------------------------------------------------------" << endl;
//            cout << "delta = " << fabs( planning.intelligentparking_.get_size_trajectory_d() - cur_pos ) << endl;
//            cout << "cur_pos = " << cur_pos << endl;
//            cout << "planning.car_state_.car_speed_ = " << planning.car_state_.car_speed_ << endl;
//            cout << "planning.is_park_r_published_ = " << planning.is_park_r_published_ << endl;
    ///判断到达开始倒车的点
    if ( fabs( planning.intelligentparking_.get_size_trajectory_d() - cur_pos ) < 15
      && planning.car_state_.car_speed_  < THRESHOLD_CAR_STATIC_SPEED
      && !planning.is_park_r_published_ )
    {
        ///vui提示
        publish_alarm( ALARM_ADVISE, "开始倒车，请注意！" );
        cout << "----- 开始倒车，请注意！ -----" << endl;
        usleep( 1000000 );

        planning.put_virtual_path( planning.park_trajectory_r_ );
        if ( planning.get_out_trajectory() )
        {
            ///publish
            publish_trajectory( planning.out_trajectory_ );

            publish_view_path( planning.out_trajectory_, view_step );
        }
        planning.is_park_r_published_ = true;

    }

    if ( !planning.is_park_r_published_ )
        return 0;

    int cur_pos_r = planning.park_trajectory_map_matching();
    int is_collided_r = planning.park_trajectory_collision_check( cur_pos_r );
//            cout << "-------- cur_pos_r = " << cur_pos_r << ", ---- is_collided_r = " << is_collided_r << endl;
    if ( is_collided_r && !is_col_last_r )
    {
        is_col_last_r = true;
        ///停车
        planning.set_trajectory_longitudinal( cur_pos_r+1, 0.0, AT_STATUS_P );

        publish_trajectory( planning.out_trajectory_ );

        publish_view_path( planning.out_trajectory_, view_step );
        cout << "!!!!!!!!!!!!!!!!!!!!!!!!!!! stop !!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    }
    if ( is_col_last_r && !is_collided_r )
    {
        is_col_last_r = false;
        ///重新起步
        //planning.set_trajectory_longitudinal( 0, 5.0/3.6, AT_STATUS_R );
        planning.get_out_trajectory();

        publish_trajectory( planning.out_trajectory_ );

        publish_view_path( planning.out_trajectory_, view_step );

    }

    if ( cur_pos_r >= planning.intelligentparking_.get_size_trajectory_r()-15 )
    {
        planning.intelligentparking_.init();
    }

    return 1;
}

int PlanningNode::publish_trajectory( OutTrajectory ot )
{
    if ( !lcm_publish_->good() )
    {
        return 0;
    }

    obu_lcm::mt_info_report m_motion_planner_msg;
    m_motion_planner_msg.points.clear();
    m_motion_planner_msg.num_of_points = ot.num_of_points_;
    m_motion_planner_msg.type = ot.car_action_;                 ///<temp
    m_motion_planner_msg.gps_time = 0.0;
    m_motion_planner_msg.car_action = ot.car_action_;
    m_motion_planner_msg.driving_mode = ot.driving_mode_;
    cout << "-------------- m_motion_planner_msg.driving_mode = " << m_motion_planner_msg.driving_mode << endl;

    for( auto point_temp : ot.points_ )
    {
        obu_lcm::nav_points msg_p;

        msg_p.s = point_temp.s;
        msg_p.p_x = point_temp.position_x;
        msg_p.p_y = point_temp.position_y;
        msg_p.p_v = point_temp.speed_desired_Uxs;
        msg_p.p_a = point_temp.acceleration_desired_Axs;
        msg_p.p_h = point_temp.heading;
        msg_p.p_k = -point_temp.k_s;
        msg_p.p_g = point_temp.gears;

        m_motion_planner_msg.points.push_back( msg_p );
    }

    lcm_publish_->publish( "mt_info_report", &m_motion_planner_msg );
    cout << " --- send mt_info_report msg : size " << m_motion_planner_msg.num_of_points<< endl;

    return 1;
}

int PlanningNode::publish_view_path( OutTrajectory out_traj, double step )
{
    if ( !lcm_publish_->good() )
    {
        return 0;
    }
    int num = out_traj.points_.size();
    if ( num <= 0 )
    {
        return 0;
    }

    obu_lcm::lateral_control_vui_info m_motion_planner_msg;
    m_motion_planner_msg.points.clear();
//    m_motion_planner_msg.num_of_points = num;
    m_motion_planner_msg.gps_time = 0.0;

    obu_lcm::nav_points msg_p;
    double delta_length = 0.0;
    int index = 0;

    point2msg( out_traj.points_[0], &msg_p );
    for( int i = 1; i < num; i++ )
    {
        delta_length += length_of_two_navipoint( out_traj.points_[i], out_traj.points_[i-1] );
        if ( delta_length >= step )
        {
            point2msg( out_traj.points_[i], &msg_p );
            m_motion_planner_msg.points.push_back( msg_p );
            delta_length = 0.0;
        }
    }
    m_motion_planner_msg.num_of_points = m_motion_planner_msg.points.size();

    lcm_publish_->publish( "lateral_control_vui_info", &m_motion_planner_msg );
    cout << " --- send lateral_control_vui_info msg : size " << m_motion_planner_msg.num_of_points<< endl;

    /*log*/
    if( TRAJECTORY_LOG_SWITCH )
    {
        ofstream outfile("lateral_control_vui_info.log", std::ios::app);
        outfile.precision(8);

        outfile << " num_of_points : " << m_motion_planner_msg.num_of_points << " "
                << endl << endl;

        for(int i=0; i < m_motion_planner_msg.num_of_points; i++)
        {
            outfile << " i " << i
                    << " x " << m_motion_planner_msg.points[i].p_x
                    << " y " << m_motion_planner_msg.points[i].p_y
                    << " h " << m_motion_planner_msg.points[i].p_h
                    << " v " << m_motion_planner_msg.points[i].p_v
                    << " a " << m_motion_planner_msg.points[i].p_a
                    << " k " << m_motion_planner_msg.points[i].p_k
                    << " g " << (int)m_motion_planner_msg.points[i].p_g
                    << endl;
        }

        outfile << endl;
        outfile.close();
    }

    return 1;

}

int PlanningNode::publish_view_path( path out_path, double step )
{
    if ( !lcm_publish_->good() )
    {
        return 0;
    }
    int num = out_path.ref_points.size();
    if ( num <= 0 )
    {
        return 0;
    }

    obu_lcm::lateral_control_vui_info m_motion_planner_msg;
    m_motion_planner_msg.points.clear();
//    m_motion_planner_msg.num_of_points = num;
    m_motion_planner_msg.gps_time = 0.0;

    obu_lcm::nav_points msg_p;
    double delta_length = 0.0;
    int index = 0;

    point2msg( out_path.ref_points[0], &msg_p );
    for( int i = 1; i < num; i++ )
    {
        delta_length += length_of_two_navipoint( out_path.ref_points[i], out_path.ref_points[i-1] );
        if ( delta_length >= step )
        {
            point2msg( out_path.ref_points[i], &msg_p );
            m_motion_planner_msg.points.push_back( msg_p );
            delta_length = 0.0;
        }
    }
    m_motion_planner_msg.num_of_points = m_motion_planner_msg.points.size();

    lcm_publish_->publish( "lateral_control_vui_info", &m_motion_planner_msg );
    cout << " --- send lateral_control_vui_info msg : size " << m_motion_planner_msg.num_of_points<< endl;

    /*log*/
    if( TRAJECTORY_LOG_SWITCH )
    {
        ofstream outfile("lateral_control_vui_info.log", std::ios::app);
        outfile.precision(8);

        outfile << " num_of_points : " << m_motion_planner_msg.num_of_points << " "
                << endl << endl;

        for(int i=0; i < m_motion_planner_msg.num_of_points; i++)
        {
            outfile << " i " << i
                    << " x " << m_motion_planner_msg.points[i].p_x
                    << " y " << m_motion_planner_msg.points[i].p_y
                    << " h " << m_motion_planner_msg.points[i].p_h
                    << " v " << m_motion_planner_msg.points[i].p_v
                    << " a " << m_motion_planner_msg.points[i].p_a
                    << " k " << m_motion_planner_msg.points[i].p_k
                    << " g " << (int)m_motion_planner_msg.points[i].p_g
                    << endl;
        }

        outfile << endl;
        outfile.close();
    }

    return 1;

}

int PlanningNode::publish_alarm( int32_t alarm_level, string alarm )
{
    if ( !lcm_publish_->good() )
    {
        return 0;
    }

    nad_lcm::ou_alarm_report msg_alarm;

    msg_alarm.alarm_proc = "planning";
    msg_alarm.alarm_time = 0.0;
    msg_alarm.alarm_level = alarm_level;
    msg_alarm.alarm_type = ALARM_TYPE_OCCUR;
    msg_alarm.alarm_no = 0;
    msg_alarm.alarm_show = ALARM_SHOW_TEXT_SOUND;
    msg_alarm.alarm = alarm;

    lcm_publish_->publish( "ou_alarm_report", &msg_alarm );

    return 1;
}

/*************** Functions of All Thread *****************************
 *
 * ThreadFunction_ins
 * ThreadFunction_can
 * ThreadFunction_route
 * ThreadFunction_map
 * ThreadFunction_obstacle
 *
 *********************************************************************/
void* PlanningNode::ThreadFunction_ins()
{
    if ( !lcm_ins_->good() )
        return NULL;

    lcm_ins_->subscribe( "ins_info", &PlanningNode::handle_ins_Message, this );

    while( 0 ==  lcm_ins_->handle() );
    return NULL;
}

void* PlanningNode::ThreadFunction_can()
{
    if ( !lcm_can_->good() )
        return NULL;

//    lcm_can_->subscribe( "CAN_value", &PlanningNode::handle_can_value_Message, this );
//    lcm_can_->subscribe( "CAN_status", &PlanningNode::handle_can_status_Message, this );
//    lcm_can_->subscribe( "chassis_detail_info", &PlanningNode::handle_chassis_detail_Message, this );
//    lcm_can_->subscribe( "steering_feedback_info", &PlanningNode::handle_steering_Message, this );
    lcm_can_->subscribe( "vehicle_info", &PlanningNode::handle_vehicle_info_Message, this );

    while( 0 ==  lcm_can_->handle() );
    return NULL;
}

void* PlanningNode::ThreadFunction_route()
{
    if ( !lcm_route_->good() )
        return NULL;

    lcm_route_->subscribe( "om_route_respond", &PlanningNode::handle_route_info, this );

    while( 0 ==  lcm_route_->handle() );
    return NULL;
}

void* PlanningNode::ThreadFunction_map()
{
    if ( !lcm_map_->good() )
        return NULL;

    lcm_map_->subscribe( "om_traffic_lights_report", &PlanningNode::handle_traffic_lights_info, this );
    lcm_map_->subscribe( "back_coordinate_XYH", &PlanningNode::handle_back_coordinate_XYH, this );
    lcm_map_->subscribe( "ou_start_auto_respond", &PlanningNode::handle_start_respond, this );
    lcm_map_->subscribe( "ou_stop_auto_respond", &PlanningNode::handle_stop_respond, this );

    while( 0 ==  lcm_map_->handle() );
    return NULL;
}

void* PlanningNode::ThreadFunction_obstacle()
{
    if ( !lcm_obstacle_->good() )
        return NULL;

    lcm_obstacle_->subscribe( "sensor_obstacle_report", &PlanningNode::handle_obstacle_Message, this );

    while( 0 ==  lcm_obstacle_->handle() );
    return NULL;
}


/*************** Functions of All LCM Handle **************************
 *
 * handle_ins_Message
 * handle_can_value_Message
 * handle_can_status_Message
 * handle_vehicle_info_Message
 * handle_chassis_detail_Message
 * handle_steering_Message
 * handle_route_info
 * handle_traffic_lights_info
 * handle_back_coordinate_XYH
 * handle_obstacle_Message
 *
 **********************************************************************/
// 惯导知道位置变了，和虚拟车道匹配之后来确定位置，并输出虚拟车道给控制部分。
void PlanningNode::handle_ins_Message( const lcm::ReceiveBuffer* rbuf,
                                       const std::string& chan,
                                       const obu_lcm::ins_info* msg)
{
    get_current_ins_info( msg );

    usleep(100);
}


void PlanningNode::handle_can_value_Message( const lcm::ReceiveBuffer* rbuf,
                                             const std::string& chan,
                                             const obu_lcm::CAN_value* msg)
{
    if ( SOURCE_OF_CAR_SPEED )      ///1:从can信号获得本车当前速度
        car_state_.car_speed_ = msg->car_speed;
//    cout << "------------- car_speed_ = " << car_state_.car_speed_ << endl;

    usleep(100);
}

void PlanningNode::handle_can_status_Message(
        const lcm::ReceiveBuffer* rbuf,
        const std::string& chan,
        const obu_lcm::CAN_status* msg)
{
    car_state_.at_status_ = msg->at_status;

    hand_direction_ = msg->right_light_status - msg->left_light_status;

    if ( hand_direction_ != last_hand_direction_ )
    {
        planning_.decide_hand_expected_lane( hand_direction_ );

        last_hand_direction_ = hand_direction_;
    }


    usleep(100);
}

void PlanningNode::handle_vehicle_info_Message(
        const lcm::ReceiveBuffer* rbuf,
        const std::string& chan,
        const obu_lcm::vehicle_info* msg)
{
    if ( SOURCE_OF_CAR_SPEED )      ///1:从can信号获得本车当前速度
    {
        car_state_.car_speed_ = msg->vehicle_speed;
    }
    car_state_.at_status_ = msg->at_status;
    car_state_.steer_angle_ = msg->steer_angle;

    hand_direction_ = msg->right_turn_status - msg->left_turn_status;

    if ( hand_direction_ != last_hand_direction_ )
    {
        planning_.decide_hand_expected_lane( hand_direction_ );

        last_hand_direction_ = hand_direction_;
    }


    usleep(100);
}

void PlanningNode::handle_chassis_detail_Message(
        const lcm::ReceiveBuffer* rbuf,
        const std::string& chan,
        const obu_lcm::ChassisDetail* msg)
{
    car_state_.car_speed_ = msg->car_speed;
    car_state_.at_status_ = msg->gear_level;

    hand_direction_ = msg->right_turn_status - msg->left_turn_status;

    if ( hand_direction_ != last_hand_direction_ )
    {
        planning_.decide_hand_expected_lane( hand_direction_ );

        last_hand_direction_ = hand_direction_;
    }


//    left_turn = msg->left_turn_status;
//    right_turn = msg->right_turn_status;
//
//    if ( left_turn == 1 && right_turn == 0 )
//    {
//        planning_.decide_hand_expected_lane( -1 );
//    }
//
//    if ( left_turn == 0 && right_turn == 1 )
//    {
//        planning_.decide_hand_expected_lane( 1 );
//    }
//
//    if ( left_turn == 0 && right_turn == 0 )
//    {
//        planning_.decide_hand_expected_lane( 0 );
//    }

    usleep(100);
}

//void PlanningNode::handle_steering_Message( const lcm::ReceiveBuffer* rbuf,
//                                            const std::string& chan,
//                                            const obu_lcm::steering_feedback_info* msg)
//{
//    // 从底层CAN 总线获取方向盘转角信息。 是执行器这边发过来的。
//    car_state_.steer_angle_ = msg->steering_angle;
//
//    usleep(100);
//}

void PlanningNode::handle_route_info( const lcm::ReceiveBuffer* rbuf,
                                      const std::string& chan,
                                      const nad_lcm::om_route_respond* msg)
{
    cout << " +++++ +++++ +++++ +++++ +++++ +++++ +++++ +++++ +++++ +++++ +++++ +++++ +++++ " << endl
         << " msg->replan_flag " << msg->replan_flag << endl
         << " msg->route.num_of_section "<< msg->route.num_of_section << endl;

    if(msg->route.num_of_section < 16 )
        return;

    if( msg->replan_flag )
    {
       route_.replan_flag = REPLAN_FLAG_NAVI;
    }
    else
    {
        route_.replan_flag = REPLAN_FLAG_NONE;
    }

    route_.route_clear();
    if ( get_map_info( msg ) )
    {
        cout << "+++++ get map information successfully ! " << endl;
        /**input**/
//        planning_.put_map_info( route_ );
    }

    is_route_set_ = true;

	usleep(1000);
}

void PlanningNode::handle_traffic_lights_info( const lcm::ReceiveBuffer* rbuf,
                                               const std::string& chan,
                                               const nad_lcm::om_traffic_lights_report* msg)
{
    cout << "------[ traffic lights ] " << msg->light_status << endl;
    cout << "------[ crossing status ] " << (int)msg->crossing_status << endl;

    traffic_lights_.crossing_status_ = msg->crossing_status;
    traffic_lights_.light_status_ = msg->light_status;

    /**input**/
    planning_.put_traffic_lights( traffic_lights_ );

    usleep(100);
}

void PlanningNode::handle_back_coordinate_XYH( const lcm::ReceiveBuffer* rbuf,
                                               const std::string& chan,
                                               const obu_lcm::back_coordinate_XYH* msg)
{
#if 1
    cout<<"===[STOP]=== x:"<<msg->x<<", y:"<<msg->y<< ", h:"<<msg->heading<<", type:"<<msg->type<<", length:"<<msg->length<<", width:"<<msg->width<<endl;
#endif // debug

     stop_park_.x_ = msg->x;
     stop_park_.y_ = msg->y;
     stop_park_.heading_ = msg->heading;
     stop_park_.length_ = msg->length;//停车位长度
     stop_park_.width_ = msg->width;//停车位宽度
     stop_park_.type_ = msg->type;  //0终点停车，否则泊车

     /**input**/
     planning_.put_stop_park( stop_park_, stop_park_.type_ );

     usleep(100);
}

void PlanningNode::handle_start_respond(const lcm::ReceiveBuffer* rbuf,
                                       const std::string& chan,
                                       const nad_lcm::ou_start_auto_respond* msg)
{
    cout << ">>>>>>>>>> ou_start_auto_respond " << endl;

    planning_.set_start_auto( true );
//    planning_.can_stop_auto_ = 0;

    //发起自动驾驶的时候，进行一次完整的重规划。
    planning_.route_.replan_flag = REPLAN_FLAG_NAVI;

    usleep(100);
}

void PlanningNode::handle_stop_respond(const lcm::ReceiveBuffer* rbuf,
                                      const std::string& chan,
                                      const nad_lcm::ou_stop_auto_respond* msg)
{
    cout << ">>>>>>>>>> ou_stop_auto_respond " << endl;

    planning_.set_start_auto( false );
//    planning_.can_stop_auto_ = 1;

    //发起自动驾驶的时候，进行一次完整的重规划。
    planning_.route_.replan_flag = REPLAN_FLAG_NAVI;

    usleep(100);
}

void PlanningNode::handle_obstacle_Message( const lcm::ReceiveBuffer* rbuf,
                                            const std::string& chan,
                                            const nad_lcm::sensor_obstacle_report* msg_obstacle_list)
{
    if ( get_obstacles_raw( msg_obstacle_list ) )
    {
        /**input**/
        planning_.put_obstacles_info( sensor_obstacles_ );
//        cout << "22222222222 sensor_obstacles_ = " << sensor_obstacles_.num_of_obstacle_ << endl;
    }

    usleep(100);
}

/*********************************************************************************************************************/
///get ins info
int PlanningNode::get_current_ins_info( const obu_lcm::ins_info * msg )
{
    car_state_.GPS_time_ = msg->gps_time;

    if ( !SOURCE_OF_CAR_SPEED )      ///0:从惯导获得本车当前速度
    {
        car_state_.car_speed_ = sqrt( pow( msg->lateral_speed, 2) + pow( msg->longitudinal_speed, 2) );
    }

//    cout << "----++++++------ yaw : " << msg->heading << endl;

    car_state_.car_pose_.Current_roll_ = msg->roll;
    car_state_.car_pose_.Current_pitch = msg->pitch;
    car_state_.car_pose_.Current_heading_ = msg->heading;

    //temp
    xy_point wp;
    coord_transfer transfer;
    wp = transfer.cs.ll2xy( msg->lat, msg->lon );

    car_state_.car_pose_.CurrentX_ = std::get<X_CS>(wp);
    car_state_.car_pose_.CurrentY_ = std::get<Y_CS>(wp);
    car_state_.car_pose_.CurrentZ_ = msg->height;

    return 1;

}

int PlanningNode::get_map_info( const nad_lcm::om_route_respond* msg )
{
    //using std::get;
    route_.cur_section = msg->route.cur_section;
    route_.mileage_next = msg->route.mileage_next;
    route_.mileage_pass = msg->route.mileage_pass;
    route_.mileage_start = msg->route.mileage_start;
    route_.mileage_stop = msg->route.mileage_stop;
    route_.time_stamp = msg->route.time_stamp;

    point_xys temp_point_xys;

    //left_edge
    route_.left_edge.line.clear();
    route_.left_edge.num_of_points = msg->route.left_edge.num_of_point;
    //route.left_edge.line.assign( msg->route.left_edge.line.begin(), msg->route.left_edge.line.end() );
    for ( int i = 0; i < msg->route.left_edge.num_of_point; i++ )
    {
        temp_point_xys.x = msg->route.left_edge.line[ i ].x;
        temp_point_xys.y = msg->route.left_edge.line[ i ].y;

        route_.left_edge.line.push_back( temp_point_xys );
    }

    //left line
    route_.left_line.line.clear();
    route_.left_line.num_of_points = msg->route.left_line.num_of_point;
//zp20171013：使用车道线进行赋值判断，不使用边界线赋值判断
    for ( int i = 0; i < msg->route.left_line.num_of_point; i++ )
    {
        temp_point_xys.x = msg->route.left_line.line[ i ].x;
        temp_point_xys.y = msg->route.left_line.line[ i ].y;

        route_.left_line.line.push_back( temp_point_xys );
    }

    //right_edge
    route_.right_edge.line.clear();
    route_.right_edge.num_of_points = msg->route.right_edge.num_of_point;
    for ( int i = 0; i < msg->route.right_edge.num_of_point; i++ )
    {
        temp_point_xys.x = msg->route.right_edge.line[ i ].x;
        temp_point_xys.y = msg->route.right_edge.line[ i ].y;

        route_.right_edge.line.push_back( temp_point_xys );
    }

    //right_line
    route_.right_line.line.clear();
    route_.right_line.num_of_points = msg->route.right_line.num_of_point;
//zp20171013：使用车道线进行赋值判断，不使用边界线赋值判断
    for ( int i = 0; i < msg->route.right_line.num_of_point; i++ )
    {
        temp_point_xys.x = msg->route.right_line.line[ i ].x;
        temp_point_xys.y = msg->route.right_line.line[ i ].y;

        route_.right_line.line.push_back( temp_point_xys );
    }

    //sections
    point_m temp_point_m;
    section_m temp_section_m;

    route_.num_of_section = msg->route.num_of_section;
    route_.line.clear();
    //section
    for ( int j = 0; j < msg->route.num_of_section; j++ )
    {
        temp_section_m.num_of_lane = msg->route.line[ j ].num_of_lane;
        temp_section_m.lane.clear();
        //points of section
        for ( int n = 0; n < msg->route.line[ j ].num_of_lane; n++ )
        {
            //temp_point_m = msg->route.line[ j ].lane[ n ]
            temp_point_m.k = msg->route.line[ j ].lane[ n ].k;
            temp_point_m.mileage = msg->route.line[ j ].lane[ n ].mileage;
            //储存切片类型
            temp_point_m.type = msg->route.line[ j ].lane[ n ].type;
            temp_point_m.width = msg->route.line[ j ].lane[ n ].width;
            temp_point_m.x = msg->route.line[ j ].lane[ n ].x;
            temp_point_m.y = msg->route.line[ j ].lane[ n ].y;
            temp_point_m.yaw = msg->route.line[ j ].lane[ n ].yaw;
            temp_point_m.sug_speed = msg->route.line[ j ].lane[ n ].sug_speed;

            temp_section_m.lane.push_back( temp_point_m );
        }
        route_.line.push_back( temp_section_m );
    }

    return 1;


}

int PlanningNode::get_obstacles_raw( const nad_lcm::sensor_obstacle_report* msg_obstacle_list )
{
//    cout << "11111111111111111111 msg_obstacle_list->num_of_obstacle = " << msg_obstacle_list->num_of_obstacle << endl;
    if ( msg_obstacle_list->num_of_obstacle < 0 )
        return 0;

    sensor_obstacles_.num_of_obstacle_ = msg_obstacle_list->num_of_obstacle;
    sensor_obstacles_.obstacles_info_.clear();

    for ( auto obs_msg : msg_obstacle_list->obstacle )
    {
        ObstacleInfo obs_temp;

        obs_temp.gps_time_ = obs_msg.gps_time;
        obs_temp.id_ = obs_msg.id;
        obs_temp.type_ = obs_msg.type;
        obs_temp.x_ = obs_msg.x;
        obs_temp.y_ = obs_msg.y;
        obs_temp.width_ = obs_msg.width;
        obs_temp.height_ = obs_msg.height;
        obs_temp.yaw_ = obs_msg.yaw;
        obs_temp.speed_ = obs_msg.speed;  //myy20181205
//        cout << "<<<<<< x = " << obs_temp.x_ << ", y = " << obs_temp.y_ << endl;


        sensor_obstacles_.obstacles_info_.push_back( obs_temp );
    }

    if ( OBSTACLES_LOG_SWITCH )
    {
        ofstream outfile("obstacles_raw.log", std::ios::app);
        outfile.precision(8);

        outfile << " num_of_obstacle : " << sensor_obstacles_.num_of_obstacle_ << endl;

        for ( int i = 0; i < sensor_obstacles_.num_of_obstacle_; i++ )
        {
            outfile << " i " << i
                    << " id " << sensor_obstacles_.obstacles_info_[i].id_
                    << " type " << sensor_obstacles_.obstacles_info_[i].type_
                    << " x " << sensor_obstacles_.obstacles_info_[i].x_
                    << " y " << sensor_obstacles_.obstacles_info_[i].y_
                    << " width " << sensor_obstacles_.obstacles_info_[i].width_
                    << " height " << sensor_obstacles_.obstacles_info_[i].height_
                    << " yaw " << sensor_obstacles_.obstacles_info_[i].yaw_
                    << " speed " << sensor_obstacles_.obstacles_info_[i].speed_
                    << endl;
        }

        outfile << endl << endl;
        outfile.close();
    }


    return 1;
}

int point2msg( const navi_point& src_point, obu_lcm::nav_points* msg_p )
{
    msg_p->s = src_point.s;
    msg_p->p_x = src_point.position_x;
    msg_p->p_y = src_point.position_y;
    msg_p->p_v = src_point.speed_desired_Uxs;
    msg_p->p_a = src_point.acceleration_desired_Axs;
    msg_p->p_h = src_point.heading;
    msg_p->p_k = -src_point.k_s;
    msg_p->p_g = src_point.gears;

    return 1;
}



