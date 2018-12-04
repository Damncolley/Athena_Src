#include "control_logic.h"

namespace athena
{
namespace control
{

void ControlLogic::Init(ControlLogicConfig control_logic_config)
{

    control_logic_config_ = control_logic_config;

    switch (control_logic_config_.vehicle_type_)
    {
    case CS55:
    {
        controller_config_.get_acc_value_callback_ = CS55GetAccValue;
        controller_config_.get_brake_value_callback_ = CS55GetBrakeVaule;
        //档位控制
        RegisterGearControl(&cs55_gear_control_);
    }
    break;
    case TRUCK_J6P:
        controller_config_.get_acc_value_callback_ = TruckJ6pGetAccValue;
        controller_config_.get_brake_value_callback_ = TruckJ6pGetBrakeVaule;
        //档位控制
        RegisterGearControl(&truck_j6p_gear_control_);
        break;
    default:
        break;
    }

    controller_config_.get_log_callback_ = Logging::LogInfo;
    controller_config_.get_current_time_callback_ = GetTime::GetGpsCurrentTime;

    if (control_logic_config_.debug_driving_mode_ == 1)
    {
        ///转向工作模式 1 = 自动驾驶 0 = 非自动驾驶
        controller_config_.steering_driving_mode_ = control_logic_config_.debug_steering_driving_mode_;
        ///纵向控制工作模式 1 = 自动驾驶 0 = 非自动驾驶
        controller_config_.acc_driving_mode_ = control_logic_config_.debug_acc_driving_mode_;
        ///刹车控制工作模式 1 = 自动驾驶 0 = 非自动驾驶
        controller_config_.brake_driving_mode_ = control_logic_config_.debug_brake_driving_mode_;
        ///EPB控制工作模式 1 = 自动驾驶 0 = 非自动驾驶
        controller_config_.epb_driving_mode_ = control_logic_config_.debug_epb_driving_mode_;
    }

    ///前轮侧偏刚度
    controller_config_.cf_ = control_logic_config_.cf_;
    ///后轮侧偏刚度
    controller_config_.cr_ = control_logic_config_.cr_;
    ///车辆重量
    controller_config_.vehicle_weight_ = control_logic_config_.vehicle_weight_;

    ///车辆高度
    controller_config_.h_ = control_logic_config_.h_;
    ///车长
    controller_config_.vehicle_length_ = control_logic_config_.vehicle_length_;

    ///车辆宽度
    controller_config_.vehicle_width_ = control_logic_config_.vehicle_width_;
    ///轮距
    controller_config_.wheelbase_ = control_logic_config_.wheelbase_;
    ///前轮轴距
    controller_config_.lf_ = control_logic_config_.lf_;
    ///后轮轴距
    controller_config_.lr_ = control_logic_config_.lr_;
    ///车轮半径
    controller_config_.wheel_radius_ = control_logic_config_.wheel_radius_;
    ///转向比
    controller_config_.steer_tranmission_ratio_ = control_logic_config_.steer_tranmission_ratio_;
    ///最大刹车值
    controller_config_.max_brake_value_ = control_logic_config_.max_brake_value_;
    ///怠速状态最大减速度
    controller_config_.max_deceleration_in_idle_ = control_logic_config_.max_deceleration_in_idle_;
    ///横向控制PID动态kp
    controller_config_.moving_kp_ = control_logic_config_.moving_kp_;
    ///横向控制PID调节P值
    controller_config_.lat_kp_ = control_logic_config_.lat_kp_;
    ///横向控制PID调节I值
    controller_config_.lat_ki_ = control_logic_config_.lat_ki_;
    ///横向控制PID调节D值
    controller_config_.lat_kd_ = control_logic_config_.lat_kd_;
    ///纵向控制PID调节P值
    controller_config_.lon_kp_ = control_logic_config_.lon_kp_;
    ///纵向控制PID调节I值
    controller_config_.lon_ki_ = control_logic_config_.lon_ki_;
    ///纵向控制PID调节D值
    controller_config_.lon_kd_ = control_logic_config_.lon_kd_;
    ///地图坐标原点纬度
    controller_config_.origin_lat_ = control_logic_config_.origin_lat_;
    ///地图坐标原点经度
    controller_config_.origin_lon_ = control_logic_config_.origin_lon_;
    ///位置误差门限值
    controller_config_.max_position_error_ = control_logic_config_.max_position_error_;
    ///最大转向角
    controller_config_.max_steering_angle_ = control_logic_config_.max_steering_angle_;
    ///最小转向角
    controller_config_.min_steering_angle_ = control_logic_config_.min_steering_angle_;
    ///限速
    controller_config_.vechile_speed_max_ = control_logic_config_.vechile_speed_max_;

    //动态kp值变化 suggest_kp = steer_angle/kp_slope_ + kp_value_
    controller_config_.kp_slope_ = control_logic_config_.kp_slope_;
    controller_config_.kp_value_ = control_logic_config_.kp_value_;
    ///预描距离
    controller_config_.xla_ = control_logic_config_.xla_;
    ///位置误差比重
    controller_config_.k_e_err_ = control_logic_config_.k_e_err_;
    ///角度误差比重
    controller_config_.k_fi_err_ = control_logic_config_.k_fi_err_;
    ///总误差比重
    controller_config_.k_ela_ = control_logic_config_.k_ela_;



    controller_config_.controller_switch_ = control_logic_config_.controller_switch_;

    controller_config_.lqr_ts_ = control_logic_config_.lqr_ts_;
    ///LQR预测窗口大小
    controller_config_.lqr_preview_window_ = control_logic_config_.lqr_preview_window_;
    ///LQR计算阀值
    controller_config_.lqr_eps_ = control_logic_config_.lqr_eps_;
    ///LQR滤波器窗口大小
    controller_config_.lqr_mean_filter_window_size_ = control_logic_config_.lqr_mean_filter_window_size_;
    ///LQR最大迭代次数
    controller_config_.lqr_max_iteration_ = control_logic_config_.lqr_max_iteration_;
    ///LQR横向最大加速度
    controller_config_.lqr_max_lateral_acceleration_ = control_logic_config_.lqr_max_lateral_acceleration_;
    ///最小速度保护
    controller_config_.lqr_minimum_speed_protection_ = control_logic_config_.lqr_minimum_speed_protection_;
    ///
    controller_config_.lqr_cutoff_freq_ = control_logic_config_.lqr_cutoff_freq_;
    ///
    controller_config_.lqr_mean_filter_window_size_ = control_logic_config_.lqr_mean_filter_window_size_;
    controller_config_.lqr_matrix_q_.assign(control_logic_config_.lqr_matrix_q_.begin(),control_logic_config_.lqr_matrix_q_.end());
    controller_config_.lqr_lat_err_scheduler_init_.assign(control_logic_config_.lqr_lat_err_scheduler_init_.begin(),control_logic_config_.lqr_lat_err_scheduler_init_.end());
    controller_config_.lqr_heading_err_scheduler_init_.assign(control_logic_config_.lqr_heading_err_scheduler_init_.begin(),control_logic_config_.lqr_heading_err_scheduler_init_.end());

    if (controller_.Init(controller_config_) == false)
    {
        Logging::LogInfo(Logging::ERROR, "controller init error");
    }
}

void ControlLogic::RegisterGearControl(GearControl *gear_control)
{
    gear_control_ = gear_control;
}

void ControlLogic::SubscribeTrajectory(Trajectory trajectory)
{
    trajectory_.points_.clear();
    trajectory_ = trajectory;
    controller_.SetTrajectory(&trajectory_);
}

void ControlLogic::SubscribeLocalization(Localization localization)
{
    localization_ = localization;
}

void ControlLogic::SubscribeChassis(ChassisDetail chassis_detail)
{
    chassis_detail_ = chassis_detail;
    chassis_.car_speed_ = chassis_detail.car_speed_;
    chassis_.steering_angle_feedback_ = chassis_detail.steering_angle_feedback_;
    //std::cout<<"steering_angle_feedback_:"<<chassis_.steering_angle_feedback_<<endl;
    chassis_.at_status_feedback_ = chassis_detail.at_gear_;
    chassis_.brake_value_feedback_ = chassis_detail.brake_value_feedback_;
}

void ControlLogic::SubscribeBcmControl(BcmControlCmd bcm_control_cmd)
{
    bcm_control_cmd_ = bcm_control_cmd;
}

void ControlLogic::GetControlCmd(ControlCmd &control_cmd)
{
    control_cmd = control_cmd_;
}

void ControlLogic::GetBcmControlCmd(BcmControlCmd &bcm_control_cmd)
{
    ///喇叭状态 0 禁声 1 鸣笛
    bcm_control_cmd.speaker_control_ = bcm_control_cmd_.speaker_control_;
    ///远光灯 0 关闭 1 开启
    bcm_control_cmd.high_beam_ctrl_ = bcm_control_cmd_.high_beam_ctrl_;
    ///近光灯 0 关闭 1 开启
    bcm_control_cmd.low_beam_ctrl_ = bcm_control_cmd_.low_beam_ctrl_;
    ///左转向灯 0 关闭 1 开启
    bcm_control_cmd.left_turn_ctrl_ = bcm_control_cmd_.left_turn_ctrl_;
    ///右转向灯 0 关闭 1 开启
    bcm_control_cmd.right_turn_ctrl_ = bcm_control_cmd_.right_turn_ctrl_;
    ///前雨刮器 0 关闭 1 开启
    bcm_control_cmd.front_wiper_ctrl_ = bcm_control_cmd_.front_wiper_ctrl_;
    ///后雨刮器 0 关闭 1 开启
    bcm_control_cmd.rear_wiper_ctrl_ = bcm_control_cmd_.rear_wiper_ctrl_;
    ///位置灯 0 关闭 1 开启
    bcm_control_cmd.position_lamp_ctrl_ = bcm_control_cmd_.position_lamp_ctrl_;
    ///前雾灯 0 关闭 1 开启
    bcm_control_cmd.front_fog_lamp_ctrl_ = bcm_control_cmd_.front_fog_lamp_ctrl_;
    ///后雾灯 0 关闭 1 开启
    bcm_control_cmd.rear_fog_lamp_ctrl_ = bcm_control_cmd_.rear_fog_lamp_ctrl_;
    ///刹车灯 一般情况自动控制 0 关闭 1 开启
    bcm_control_cmd.brake_lamp_ctrl_ = bcm_control_cmd_.brake_lamp_ctrl_;
    ///警报灯 双闪 0 关闭 1 开启
    bcm_control_cmd.alarm_lamp_ctrl_ = bcm_control_cmd_.alarm_lamp_ctrl_;
    /// 左前门控制 0 关闭 1 开启
    bcm_control_cmd.lf_door_ctrl_ = bcm_control_cmd_.lf_door_ctrl_;
    /// 右前门控制 0 关闭 1 开启
    bcm_control_cmd.rf_door_ctrl_ = bcm_control_cmd_.rf_door_ctrl_;
    /// 左后门控制 0 关闭 1 开启
    bcm_control_cmd.lr_door_ctrl_ = bcm_control_cmd_.lr_door_ctrl_;
    /// 右后门控制 0 关闭 1 开启
    bcm_control_cmd.rr_door_ctrl_ = bcm_control_cmd_.rr_door_ctrl_;
}

void ControlLogic::GetControlInfoReport(ControlInfoReport &control_info_report)
{
    control_info_report.cur_lon_ = localization_.lon_;
    control_info_report.cur_lat_ = localization_.lat_;

    control_info_report.cur_yaw_ = localization_.heading_;
    control_info_report.cur_brake_ = chassis_.brake_value_feedback_;
    control_info_report.cur_speed_ = chassis_.car_speed_;
    control_info_report.cur_speed_lateral_ = localization_.lateral_speed_;
    control_info_report.cur_speed_longitudinal_ = localization_.longitudinal_speed_;
    ///加速度模式
    control_info_report.cur_acceleration_pattern_ = 0;
    control_info_report.cur_acceleration_ = sqrt(pow(localization_.lateral_accelerate_, 2) + pow(localization_.longitudinal_accelerate_, 2));
    ;
    control_info_report.cur_acceleration_lateral_ = localization_.lateral_accelerate_;
    control_info_report.cur_acceleration_longitudinal_ = localization_.longitudinal_accelerate_;
    control_info_report.steering_angle_ = controller_output_.steering_angle_;

    control_info_report.gps_time_ = localization_.gps_time_;
}

void ControlLogic::ComputeControlOutputOnTimer()
{
    controller_.ComputeControlOutput(&localization_, &chassis_, &controller_output_);
    SetControlCmd(&control_cmd_, controller_output_);
}

double ControlLogic::GetSteeringAngleFeedback()
{
    return chassis_.steering_angle_feedback_;
}

void ControlLogic::run()
{
    while (1)
    {
        ComputeControlOutputOnTimer();
        //10ms
        usleep(20000);
    }
}

bool ControlLogic::GetControllerAlarmInfo(Emergency *emergency)
{
    bool emergency_enable = false;
    std::vector<ControllerOutputAlarm::AlarmInfoTable> alarm_list;
    controller_.GetAlarmTableInfo(&alarm_list);

    for (auto it = alarm_list.begin(); it != alarm_list.end(); it++)
    {
        std::string info = "alarm_code:" + std::to_string(it->alarm_info.alarm_code) + "\n" + "alarm_level:" + std::to_string(it->alarm_info.alarm_level) + "\n" + "timer_stamp:" + std::to_string(it->timer_stamp.year) + "-" + std::to_string(it->timer_stamp.month) + "-" + std::to_string(it->timer_stamp.day) + "-" + std::to_string(it->timer_stamp.hour) + "-" + std::to_string(it->timer_stamp.minute) + "-" + std::to_string(it->timer_stamp.second) + ":" + std::to_string(it->timer_stamp.m_second);

        if (it->alarm_info.alarm_level == ControllerAlarmCodeLevel::NORMAL)
        {
            Logging::LogInfo(Logging::INFO, info);
        }

        if ((it->alarm_info.alarm_level == ControllerAlarmCodeLevel::LOW_WARNING) || (it->alarm_info.alarm_level == ControllerAlarmCodeLevel::HIGH_WARNING))
        {
            Logging::LogInfo(Logging::WARNING, info);
        }

        if (it->alarm_info.alarm_level == ControllerAlarmCodeLevel::ERROR)
        {
            Logging::LogInfo(Logging::ERROR, info);
            emergency_enable = true;
        }
    }

    if (emergency_enable == true)
    {
        emergency->emergency_mode_ = Emergency::EMERGENCY_BRAKING;
        emergency->emergency_level_ = Emergency::ALL_SITUATION;
        emergency->emergency_value_ = control_logic_config_.max_brake_value_;
    }

    return emergency_enable;
}

void ControlLogic::SetDrivingMode(int driving_mode)
{
    if (driving_mode == HUMAN_DRIVING_MODE)
    {
        driving_mode_ = HUMAN_DRIVING_MODE;
        return;
    }
    //底层错误
    if (chassis_detail_.chassis_error_ == 1)
    {
        driving_mode_ = HUMAN_DRIVING_MODE;
        Logging::LogInfo(Logging::WARNING, "chassis error");
        return;
    }

    if (chassis_detail_.steering_driving_mode_feedback_ == UNCONTROLLABLE)
    {
        driving_mode_ = HUMAN_DRIVING_MODE;
        Logging::LogInfo(Logging::WARNING, "steering_driving_mode_feedback_ uncontrollable");
        return;
    }

    if (chassis_detail_.acc_driving_mode_feedback_ == UNCONTROLLABLE)
    {
        driving_mode_ = HUMAN_DRIVING_MODE;
        Logging::LogInfo(Logging::WARNING, "acc_driving_mode_feedback_ uncontrollable");
        return;
    }

    if (chassis_detail_.brake_driving_mode_feedback_ == UNCONTROLLABLE)
    {
        driving_mode_ = HUMAN_DRIVING_MODE;
        Logging::LogInfo(Logging::WARNING, "brake_driving_mode_feedback_ uncontrollable");
        return;
    }

    if (chassis_detail_.epb_driving_mode_feedback_ == UNCONTROLLABLE)
    {
        driving_mode_ = HUMAN_DRIVING_MODE;
        Logging::LogInfo(Logging::WARNING, "epb_driving_mode_feedback_ uncontrollable");
        return;
    }

    driving_mode_ = driving_mode;
}

int32_t ControlLogic::GetDrivingMode()
{
    return driving_mode_;
}

void ControlLogic::SetControlCmd(ControlCmd *control_cmd, ControllerOutput controller_output)
{
    // SetDrivingMode(int driving_mode);
    ///方向盘转角，单位：度
    control_cmd->steering_angle_ = controller_output.steering_angle_;
    ///方向盘角速度，单位：度/s
    control_cmd->steering_angle_speed_ = controller_output.steering_angle_speed_;
    ///驾驶模式：人工驾驶 1、自动驾驶 3 和辅助驾驶 2
    SetDrivingMode(controller_output.steering_driving_mode_);
    control_cmd->steering_driving_mode_ = GetDrivingMode();
    ///节气门开度
    control_cmd->acc_value_ = controller_output.acc_value_;
    ///驾驶模式：人工驾驶 1、自动驾驶 3 和辅助驾驶 2
    SetDrivingMode(controller_output.acc_driving_mode_);
    control_cmd->accelerate_driving_mode_ = GetDrivingMode();
    ///刹车
    control_cmd->brake_value_ = controller_output.brake_value_;
    ///驾驶模式：人工驾驶 1、自动驾驶 3 和辅助驾驶 2
    SetDrivingMode(controller_output.brake_driving_mode_);
    control_cmd->brake_driving_mode_ = GetDrivingMode();
    ///EPB状态
    control_cmd->epb_enable_ = controller_output.epb_status_;
    ///驾驶模式：人工驾驶 1、自动驾驶 3 和辅助驾驶 2
    SetDrivingMode(controller_output.brake_driving_mode_);
    control_cmd->epb_driving_mode_ = GetDrivingMode();
    ///档位控制
    control_cmd->gear_lever_ = gear_control_->GetGearLevel();
}

void ControlLogic::GetControlLogicDebugOutput(ControlLogicDebugOutput &control_logic_debug_output)
{
    DebugOutput debug_output;
    controller_.GetControllerInfo(debug_output);
    control_logic_debug_output_.lon_controller_tar_speed_ = debug_output.tar_speed_;
    control_logic_debug_output = control_logic_debug_output_;
    control_logic_debug_output.chassis_detail_output_ = chassis_detail_;
    control_logic_debug_output.localization_output_ = localization_;
}

void ControlLogic::GetControllerInfo(DebugOutput &debug_output)
{
    controller_.GetControllerInfo(debug_output);
}

void ControlLogic::SetDrivingModeDebug(int32_t mode)
{
    controller_.SetDrivingModeDebug(mode);
}

void ControlLogic::SetTarSpeedDebug(int32_t tar_speed,bool valid)
{
    controller_.SetTarSpeedDebug(tar_speed,valid);
}
} // namespace control
} // namespace athena
