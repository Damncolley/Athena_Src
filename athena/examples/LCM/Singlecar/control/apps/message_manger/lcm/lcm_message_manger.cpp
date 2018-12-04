#include "lcm_message_manger.h"

namespace athena{
namespace control{
  void  LcmMessageManger::Init(string obu_url,ControlLogic *control_logic)
  {
     control_logic_ = control_logic;
     lcm_  = new lcm::LCM(obu_url);

     std::cout<<"obu_url:"<<obu_url<<endl;

     if(!lcm_ -> good())
     {
         Logging::LogInfo(Logging::ERROR,"lcm init error!");
         return;
     }

    // 导航数据
    lcm_ -> subscribe("ins_info", &LcmMessageManger::HandleLocalizationMessage, this);
    // 底盘反馈：控制信息
    lcm_ -> subscribe("vehicle_info", &LcmMessageManger::HandleChassisDetailMessage, this);
    // 规划下发路径
    lcm_ -> subscribe("mt_info_report", &LcmMessageManger::HandleTrajectoryMessage,this);
    //规划下发BCM控制
    lcm_ -> subscribe("mt_bcm_control_cmd", &LcmMessageManger::HandleMtBcmControlMessage,this);

    //线程执行开始
    start();
  }


void  LcmMessageManger::PublishControlCmd(ControlCmd control_cmd)
{
    obu_lcm::control_cmd control_cmd_msg;

    control_cmd_msg.steer_angle                = control_cmd.steering_angle_;
    control_cmd_msg.steer_speed                = control_cmd.steering_angle_speed_;
    control_cmd_msg.steer_driving_mode         = control_cmd.steering_driving_mode_;
    control_cmd_msg.accel_value                = control_cmd.acc_value_;
    control_cmd_msg.accel_driving_mode         = control_cmd.accelerate_driving_mode_;
    control_cmd_msg.brake_value                = control_cmd.brake_value_;
    //std::cout<<"steering_angle:"<<control_cmd_msg.steering_angle <<endl;
    //std::cout<<"steering_driving_mode:"<<control_cmd_msg.steering_driving_mode <<endl;
    control_cmd_msg.brake_driving_mode         = control_cmd.brake_driving_mode_;
    control_cmd_msg.epb_enable                 = control_cmd.epb_enable_;
    control_cmd_msg.epb_driving_mode           = control_cmd.epb_driving_mode_;
    control_cmd_msg.gear_level                 = control_cmd.gear_lever_;

    lcm_ -> publish("control_cmd", &control_cmd_msg);
}

void LcmMessageManger::PublishBcmControlCmd(BcmControlCmd bcm_control_cmd)
{
    obu_lcm::bcm_control_cmd bcm_control_cmd_msg;

    bcm_control_cmd_msg.speaker_control            = bcm_control_cmd.speaker_control_;
    bcm_control_cmd_msg.high_beam_ctrl             = bcm_control_cmd.high_beam_ctrl_;
    bcm_control_cmd_msg.low_beam_ctrl              = bcm_control_cmd.low_beam_ctrl_;
    bcm_control_cmd_msg.left_turn_ctrl             = bcm_control_cmd.left_turn_ctrl_;
    bcm_control_cmd_msg.right_turn_ctrl            = bcm_control_cmd.right_turn_ctrl_;
    bcm_control_cmd_msg.front_wiper_ctrl           = bcm_control_cmd.front_wiper_ctrl_;
    bcm_control_cmd_msg.rear_wiper_ctrl            = bcm_control_cmd.rear_wiper_ctrl_;
    bcm_control_cmd_msg.position_lamp_ctrl         = bcm_control_cmd.position_lamp_ctrl_;
    bcm_control_cmd_msg.front_fog_lamp_ctrl        = bcm_control_cmd.front_fog_lamp_ctrl_;
    bcm_control_cmd_msg.rear_fog_lamp_ctrl         = bcm_control_cmd.rear_fog_lamp_ctrl_;
    bcm_control_cmd_msg.brake_lamp_ctrl            = bcm_control_cmd.brake_lamp_ctrl_;
    bcm_control_cmd_msg.alarm_lamp_ctrl            = bcm_control_cmd.alarm_lamp_ctrl_;
    bcm_control_cmd_msg.lf_door_ctrl               = bcm_control_cmd.lf_door_ctrl_;
    bcm_control_cmd_msg.rf_door_ctrl               = bcm_control_cmd.rf_door_ctrl_;
    bcm_control_cmd_msg.lr_door_ctrl               = bcm_control_cmd.lr_door_ctrl_;
    bcm_control_cmd_msg.rr_door_ctrl               = bcm_control_cmd.rr_door_ctrl_;

    lcm_ -> publish("bcm_control_cmd", &bcm_control_cmd_msg);
}

void LcmMessageManger::PublishControlInfoReport(ControlInfoReport control_info_report)
{
    obu_lcm::control_info_report control_info_report_msg;
    memset(&control_info_report_msg,0,sizeof(obu_lcm::control_info_report));
    control_info_report_msg.lon = control_info_report.cur_lon_;
    control_info_report_msg.lat = control_info_report.cur_lat_;
    control_info_report_msg.yaw = control_info_report.cur_yaw_;
//    control_info_report_msg.cur_brake = control_info_report.cur_brake_;//该字段改为当前期望的刹车值 20180604
//    //  control_info_report_msg.cur_speed = control_info_report.Current_Speed;
//    control_info_report_msg.cur_speed = control_info_report.cur_speed_;//modfly by alex  send CAN car speed to netwok,instead ins speed
//    control_info_report_msg.cur_speed_lateral = control_info_report.cur_speed_lateral_;
//    control_info_report_msg.cur_speed_longitudinal = control_info_report.cur_speed_longitudinal_;
//    control_info_report_msg.cur_acceleration_pattern = control_info_report.cur_acceleration_pattern_;
//    control_info_report_msg.cur_acceleration = control_info_report.cur_acceleration_;
//    control_info_report_msg.cur_acceleration_lateral = control_info_report.cur_acceleration_lateral_;
//    control_info_report_msg.cur_acceleration_longitudinal = control_info_report.cur_acceleration_longitudinal_;//modefy by alex20160927
//    control_info_report_msg.steering_angle = control_info_report.steering_angle_;
//    control_info_report_msg.flashing_status = control_info_report.flashing_status_ + control_info_report.flashing_status_;
//    //control_info_report_msg.cur_gears = control_info_report.Current_gears;
//    control_info_report_msg.cur_gears = control_info_report.cur_gears_;//将档杆位置通过这个接口发送出去add by alex20160927
//    control_info_report_msg.gps_time = control_info_report.gps_time_;
//    control_info_report_msg.mileage = control_info_report.mileage_;//add by alex20160927 增加里程数发送
//    //control_info_report_msg.num_of_camera_stat = control_info_report.num_of_camera_stat_;
////    control_info_report_msg.camera_stat.assign(control_info_report.camera_stat_.begin(),control_info_report.camera_stat_.end());
////    control_info_report_msg.num_of_radar_stat = control_info_report.num_of_radar_stat_;
////    control_info_report_msg.radar_stat.assign(control_info_report.radar_stat_.begin(),control_info_report.radar_stat_.end());
////    control_info_report_msg.num_of_lidar_stat = control_info_report.num_of_lidar_stat_;
////    control_info_report_msg.lidar_stat.assign(control_info_report.lidar_stat_.begin(),control_info_report.lidar_stat_.end());
////    control_info_report_msg.gps_stat.assign(control_info_report.gps_stat_.begin(),control_info_report.gps_stat_.end());
////    control_info_report_msg.num_of_gps_stat = control_info_report_msg.num_of_gps_stat;
//    control_info_report_msg.eps_stat = control_info_report.eps_stat_;
//    control_info_report_msg.epb_stat = control_info_report.epb_stat_;
//    control_info_report_msg.brake_stat = control_info_report.brake_stat_;
    lcm_ -> publish("control_info_report", &control_info_report_msg);
}

void LcmMessageManger::PublishEmergencyCmd(Emergency emergency)
{
    obu_lcm::emergency emergency_msg;

    emergency_msg.emergency_mode = emergency.emergency_mode_;
    emergency_msg.emergency_level = emergency.emergency_level_;
    emergency_msg.emergency_value = emergency.emergency_value_;

    lcm_ -> publish("emergency", &emergency_msg);
}


void LcmMessageManger::HandleLocalizationMessage(const lcm::ReceiveBuffer* rbuf,
                                                 const std::string& chan,
                                                 const obu_lcm::ins_info * msg)
{
    Localization localization;

    ///GPS时间
    localization.gps_time_                 = msg -> gps_time;
    ///周
    localization.week_                     = msg -> week;
    ///经度
    localization.lat_                      = msg -> lat;
    ///纬度
    localization.lon_                      = msg -> lon;
    ///海拔
    localization.height_                   = msg -> height;
    ///横向速度
    localization.lateral_speed_            = msg -> lateral_speed;
    ///纵向速度
    localization.longitudinal_speed_       = msg -> longitudinal_speed;
    ///地向速度
    localization.down_speed_               = msg -> down_speed;
    ///横滚角度
    localization.roll_                     = msg -> roll;
    ///俯仰角度
    localization.pitch_                    = msg -> pitch;
    ///航向角度
    localization.heading_                  = msg -> heading;
    ///横向加速度
    localization.lateral_accelerate_       = msg -> lateral_accelerate;
    ///纵向加速度
    localization.longitudinal_accelerate_  = msg -> longitudinal_accelerate;
    ///地向加速度
    localization.down_accelerate_          = msg -> down_accelerate;
    ///横滚角速度
    localization.roll_speed_               = msg -> roll_speed;
    ///俯仰角速度
    localization.pitch_speed_              = msg -> pitch_speed;
    ///航向角速度
    localization.heading_speed_            = msg -> heading_speed;

    control_logic_ -> SubscribeLocalization(localization);
    usleep(10);
}

void LcmMessageManger::HandleChassisDetailMessage(const lcm::ReceiveBuffer* rbuf,
                                                  const std::string& chan,
                                                  const obu_lcm::vehicle_info* msg)
{
    ChassisDetail chassis_detail;
    chassis_detail.chassis_error_                   = msg -> chassis_error;
//    chassis_detail.wheel_speed_fl_                  = msg -> wheel_speed_fl;
//    chassis_detail.wheel_speed_fr_                  = msg -> wheel_speed_fr;
//    chassis_detail.wheel_speed_bl_                  = msg -> wheel_speed_bl;
//    chassis_detail.wheel_speed_br_                  = msg -> wheel_speed_br;
    chassis_detail.car_speed_                       = msg -> vehicle_speed;
    chassis_detail.eng_rpm_                         = msg -> eng_rpm;
//    chassis_detail.acc_pedal_pos_                   = msg -> acc_pedal_pos;
//    chassis_detail.throttle_pos_feedback_           = msg -> throttle_pos_feedback;
//    chassis_detail.eng_torq_without_tcured_         = msg -> eng_torq_without_tcured;
//    chassis_detail.batt_volt_v_                     = msg -> batt_volt_v;
//    chassis_detail.driver_torque_                   = msg -> driver_torque;
//    chassis_detail.eng_actual_ind_torque_           = msg -> eng_actual_ind_torque;
//    chassis_detail.friction_torque_                 = msg -> friction_torque;
//    chassis_detail.torque_limitation_               = msg -> torque_limitation;
//    chassis_detail.torque_reduction_                = msg -> torque_reduction;
//    chassis_detail.turbine_rpm_                     = msg -> turbine_rpm;
//    chassis_detail.brake_pressure_                  = msg -> brake_pressure;
//    chassis_detail.engine_running_status_           = msg -> engine_running_status;
//    chassis_detail.gear_level_                      = msg -> gear_level;
    chassis_detail.at_gear_                         = msg -> at_status;
    chassis_detail.brake_status_                    = msg -> brake_status;
//    chassis_detail.epb_brake_status_                = msg -> epb_brake_status;
//    chassis_detail.dfco_                            = msg -> dfco;
//    chassis_detail.idle_status_                     = msg -> idle_status;
//    chassis_detail.tcu_torque_red_availability_     = msg -> tcu_torque_red_availability;
//    chassis_detail.eng_torque_failure_              = msg -> eng_torque_failure;
//    chassis_detail.ems_released_                    = msg -> ems_released;
//    chassis_detail.eng_started_                     = msg -> eng_started;
//    chassis_detail.torque_reduction_flag_           = msg -> torque_reduction_flag;
//    chassis_detail.torque_limitation_flag_          = msg -> torque_limitation_flag;
//    chassis_detail.gear_eng_agement_                = msg -> gear_eng_agement;
//    chassis_detail.tcu_state_                       = msg -> tcu_state;
//    chassis_detail.ebd_work_                        = msg -> ebd_work;
//    chassis_detail.abs_ebdlable_                    = msg -> abs_ebdlable;
    chassis_detail.steering_angle_feedback_         = msg -> steer_angle;
//    chassis_detail.steering_angle_speed_feedback_   = msg -> steering_angle_speed_feedback;
//    chassis_detail.steering_driving_mode_feedback_  = msg -> steering_driving_mode_feedback;
//    chassis_detail.brake_value_feedback_            = msg -> brake_value_feedback;
//    chassis_detail.brake_run_time_feedback_         = msg -> brake_run_time_feedback;
//    chassis_detail.acc_driving_mode_feedback_       = msg -> acc_driving_mode_feedback;
//    chassis_detail.epb_driving_mode_feedback_       = msg -> epb_driving_mode_feedback;
//    chassis_detail.speaker_status_                  = msg -> speaker_status;
//    chassis_detail.high_beam_status_                = msg -> high_beam_status;
//    chassis_detail.low_beam_status_                 = msg -> low_beam_status;
    chassis_detail.left_turn_status_                = msg -> left_turn_status;
    chassis_detail.right_turn_status_               = msg -> right_turn_status;
//    chassis_detail.front_wiper_status_              = msg -> front_wiper_status;
//    chassis_detail.rear_wiper_status_               = msg -> rear_wiper_status;
//    chassis_detail.position_lamp_status_            = msg -> position_lamp_status;
//    chassis_detail.front_fog_lamp_status_           = msg -> front_fog_lamp_status;
//    chassis_detail.rear_fog_lamp_status_            = msg -> rear_fog_lamp_status;
//    chassis_detail.brake_lamp_status_               = msg -> brake_lamp_status;
//    chassis_detail.alarm_lamp_status_               = msg -> alarm_lamp_status;
//    chassis_detail.lf_door_status_                  = msg -> lf_door_status;
//    chassis_detail.rf_door_status_                  = msg -> rf_door_status;
//    chassis_detail.lr_door_status_                  = msg -> lr_door_status;
//    chassis_detail.rr_door_status_                  = msg -> rr_door_status;


    control_logic_ -> SubscribeChassis(chassis_detail);
    usleep(10);
}

void LcmMessageManger::HandleTrajectoryMessage(const lcm::ReceiveBuffer* rbuf,
                                        const std::string& chan,
                                        const obu_lcm::mt_info_report* msg)
{
    int32_t trajectory_length = msg -> points.size();
    //msg.num_of_points
    if(trajectory_length < 10 || trajectory_length > 5000)
    {
        Logging::LogInfo(Logging::WARNING,"mt_info_report message length error");
    }

    NavPoints point;
    Trajectory trajectory;

    //GPS时间
    trajectory.gps_time_       = msg -> gps_time;
    //轨迹点数量
    trajectory.num_of_points_  = msg -> num_of_points;
    //轨迹点类型
    trajectory.type_           = msg -> type;
    //保留
    trajectory.reserved_       = msg -> reserved;
    trajectory.car_action_     = msg -> car_action;
    //模式 0 人工驾驶 1自动驾驶
    trajectory.driving_mode_   = msg -> driving_mode;
    for(auto it = msg->points.begin(); it != msg->points.end(); ++it)
    {
        point.p_x_ = it -> p_x;
        point.p_y_ = it -> p_y;

        point.p_h_ = it -> p_h;

        point.p_k_ = it -> p_k;
        point.p_k_ = it -> p_k;
        point.p_v_ = it -> p_v;
        point.p_a_ = it -> p_a;

        point.s_ = it -> s;
        point.p_g_ = it -> p_g;
        point.p_h_ = it -> p_h;
        point.p_k_ = it -> p_k;

        //cout<<"tar_speed: "<<it -> p_v<<endl;
        trajectory.points_.push_back(point);
    }

    control_logic_ -> SubscribeTrajectory(trajectory);
    usleep(10);
}

void LcmMessageManger::HandleMtBcmControlMessage(const lcm::ReceiveBuffer* rbuf,
                                                      const std::string& chan,
                                                      const obu_lcm::mt_bcm_control_cmd* msg)
{
    BcmControlCmd bcm_control_cmd;
    ///喇叭控制 0 禁声音 1 鸣笛
    bcm_control_cmd.speaker_control_        = msg -> speaker_control;
    ///远光灯 0 关闭 1 开启
    bcm_control_cmd.high_beam_ctrl_         = msg -> high_beam_ctrl;
    ///近光灯 0 关闭 1 开启
    bcm_control_cmd.low_beam_ctrl_          = msg -> low_beam_ctrl;
    ///左转向灯 0 关闭 1 开启
    bcm_control_cmd.left_turn_ctrl_         = msg -> left_turn_ctrl;
    ///右转向灯 0 关闭 1 开启
    bcm_control_cmd.right_turn_ctrl_        = msg -> right_turn_ctrl;
    ///前雨刮器 0 关闭 1 开启
	bcm_control_cmd.front_wiper_ctrl_       = msg -> front_wiper_ctrl;
    ///后雨刮器 0 关闭 1 开启
    bcm_control_cmd.rear_wiper_ctrl_        = msg -> rear_wiper_ctrl;
    ///位置灯 0 关闭 1 开启
    bcm_control_cmd.position_lamp_ctrl_     = msg -> position_lamp_ctrl;
    ///前雾灯 0 关闭 1 开启
    bcm_control_cmd.front_fog_lamp_ctrl_    = msg -> front_fog_lamp_ctrl;
    ///后雾灯 0 关闭 1 开启
    bcm_control_cmd.rear_fog_lamp_ctrl_     = msg -> rear_fog_lamp_ctrl;
    ///刹车灯 一般情况自动控制 0 关闭 1 开启
    bcm_control_cmd.brake_lamp_ctrl_        = msg -> brake_lamp_ctrl;
    ///警报灯 双闪 0 关闭 1 开启
    bcm_control_cmd.alarm_lamp_ctrl_        = msg -> alarm_lamp_ctrl;
    /// 左前门控制 0 关闭 1 开启
    bcm_control_cmd.lf_door_ctrl_           = msg -> lf_door_ctrl;
    /// 右前门控制 0 关闭 1 开启
    bcm_control_cmd.rf_door_ctrl_           = msg -> rf_door_ctrl;
    /// 左后门控制 0 关闭 1 开启
    bcm_control_cmd.lr_door_ctrl_           = msg -> lr_door_ctrl;
    /// 右后门控制 0 关闭 1 开启
    bcm_control_cmd.rr_door_ctrl_           = msg -> rr_door_ctrl;

    control_logic_ -> SubscribeBcmControl(bcm_control_cmd);
    usleep(10);
}

void LcmMessageManger::run()
{
    while( 0 ==  lcm_->handle());
}
}
}
