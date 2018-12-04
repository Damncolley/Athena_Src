#include "control.h"
#include "control_view/control_view.h"



namespace athena{
namespace control{

Control::Control(std::string software_version)
{
    software_version_ = software_version;
}

void Control::Init()
{
    ///读配置文件设置
    std::cout<<"config_file_path_:"<<config_file_path_<<endl;
    main_config_file_ = new Config(config_file_path_);

    //调试
    gui_enable_ = main_config_file_ -> Read("GUI_ENABLE",1);
    debug_enable_ = main_config_file_ -> Read("DEBUG_ENABLE",1);
    Logging::Init(debug_enable_);
    print_enable_ = main_config_file_ -> Read("PRINT_ENABLE",1);
    log_enable_ = main_config_file_ -> Read("LOG_ENABLE",1);
    debug_print_period_ = main_config_file_ -> Read("DEBUG_PRINT_PERIOD",0);
    debug_log_period_ = main_config_file_ -> Read("DEBUG_LOG_PERIOD",0);
    init_map_from_file_ = main_config_file_ -> Read("INIT_MAP_FROM_FILE",0);

    vehicle_type_ = main_config_file_ -> Read("VEHICLE_TYPE",1);

    switch(vehicle_type_)
    {
        case CS55:
        break;
        case TRUCK_J6P:
        break;
        default:
        Logging::LogInfo(Logging::ERROR,"unknown vehicle type");
        break;
    }

    vehicle_type_name_ = main_config_file_ -> Read("VEHICLE_TYPE_NAME",vehicle_type_name_);
    vehicle_config_file_path_ = "config/"+vehicle_type_name_ + "/" +vehicle_type_name_+"_" + "config.cfg";
    std::cout<<"vehicle_config_file_path_:"<<vehicle_config_file_path_<<endl;

    vehicle_config_file_ = new Config(vehicle_config_file_path_);
    //读配置文件
    ReadConfigFile();
    //控制逻辑初始化
    control_logic_.Init(control_logic_config_);
    control_logic_.start();

    if(gui_enable_ == 1)
    {
        ControlView::Init(&control_logic_, vehicle_width_);
        ControlView::MyGLDispIni();
    }

    if(init_map_from_file_ == 1)
    {
        local_trajectory_path = "looptest.txt";
        local_trajectory_path = main_config_file_ -> Read("LOCAL_TRAJECTORY_PATH",local_trajectory_path);
        track_trajectory_ = new TrackTrajectory();
        track_trajectory_ -> Init(local_trajectory_path,&control_logic_);
    }

    control_debug_  = new ControlDebug<Control>();

    if(print_enable_ == 1)
    {
        control_debug_ -> AddPrint(debug_print_period_,&Control::print,this);
    }

    if(log_enable_ == 1)
    {
      control_debug_ -> AddLog(debug_log_period_,&Control::log,this);
    }
}

void Control::print()
{
    printf("\033[2J");
    //std::cout.imbue(locale("chs"));

    std::cout<<"ver:" <<software_version_<<" 软件版本"<<endl;
    std::cout<<"--------------location info---------"<<endl;
    std::cout<<"controller_switch:" <<controller_switch_<<" 控制器"<<endl;
    std::cout<<"vehicle_type:" <<vehicle_type_<<" 车辆类型"<<endl;
    std::cout << "lat:" << control_logic_debug_output_.localization_output_.lat_ << "  lon:" << control_logic_debug_output_.localization_output_.lon_ << " 经纬度" << endl;
    std::cout << "heading:" << control_logic_debug_output_.localization_output_.heading_ << " 航向角" << endl;
    std::cout << "ins_speed:" << control_logic_debug_output_.localization_output_.longitudinal_speed_ << " 速度" << endl;
    std::cout << "heading_speed:" << control_logic_debug_output_.localization_output_.heading_speed_ << " 航向角速度" << endl;

    std::cout<<"--------------vehicle info---------"<<endl;
    control_logic_.GetControlLogicDebugOutput(control_logic_debug_output_);
    std::cout << "steer_angle:" << control_logic_debug_output_.chassis_detail_output_.steering_angle_feedback_ << "deg 转向角" << endl;
    std::cout << "vehicle_speed:" << control_logic_debug_output_.chassis_detail_output_.car_speed_*3.6 << "km/h 车速" << endl;
    std::cout << "gear:" << control_logic_debug_output_.chassis_detail_output_.at_gear_ << "档位" << endl;
    std::cout <<"tar_speed:" << control_logic_debug_output_.lon_controller_tar_speed_<<" 目标速度"<<endl;

    std::cout<<"--------------vehicle cmd---------"<<endl;
    std::cout << "steering_angle_:"<<control_cmd_.steering_angle_ << " 转向角"<<endl
              << "steering_angle_speed_:"<<control_cmd_.steering_angle_speed_ << " 转向角速度"<<endl
              << "steering_driving_mode:"<<control_cmd_.steering_driving_mode_ << " 转向工作模式"<<endl

              << "accelerate_value:"<<control_cmd_.acc_value_ << " 油门值"<<endl
              << "accelerate_driving_mode_:"<<control_cmd_.accelerate_driving_mode_ << " 油门工作模式"<<endl

              << "brake_value_:"<<control_cmd_.brake_value_ << " 刹车值"<<endl
              << "brake_driving_mode_:"<<control_cmd_.brake_driving_mode_ << " 刹车工作模式"<<endl
              << "epb_enable_:"<<control_cmd_.epb_enable_ << " EPB使能状态"<<endl
              << "epb_driving_mode_:"<<control_cmd_.epb_driving_mode_ << " EPB工作状态"<<endl
              << "gear_lever_:"<<control_cmd_.gear_lever_ << " 档杆位置"<<endl;
}


void Control::log()
{
//   string log_filename = "./log/control" + start_time + ".log";
//   ofstream  logfile(log_filename.c_str(),std::ofstream::app);  // 45  speed=7
//   logfile.precision(13);
//        logfile << dec
//                <<software_version<<" "
//                <<control_cmd_.steering_angle_ <<" "
//                <<control_cmd_.steering_angle_speed_<<" "
//                <<control_cmd_.accelerate_driving_mode_<<" "
//                <<control_cmd_.brake_value_<<" "
//                <<control_cmd_.brake_driving_mode_ <<" "
//                <<control_cmd_.epb_enable_<<" "
//                <<control_cmd_.epb_driving_mode_<<" "
//                <<control_cmd_.gear_lever_ <<" "
//                <<control_logic_debug_output_.lon_controller_tar_speed_<<" "
//                << endl;
//    logfile.close();
}



void Control::run()
{
    while(1)
    {
        PublishControlOutputOnTimer();
        //std::cout<<"control run"<<endl;
        //10ms
        usleep(20000);
    }
}

void Control::ResigerMessageManger(MessageManger *message_manger)
{
    message_manger_ = message_manger;
}

void Control::PublishControlOutputOnTimer()
{
    //紧急事件
    Emergency energnecy;
    if(control_logic_.GetControllerAlarmInfo(&energnecy) == true)
    {
        message_manger_ -> PublishEmergencyCmd(energnecy);
    }
    control_logic_.GetControlCmd(control_cmd_);
    PublishControlCmd(control_cmd_);
    control_logic_.GetControlInfoReport(control_info_report_);
    PublishControlInfoReport(control_info_report_);
    control_logic_.GetBcmControlCmd(bcm_control_cmd_);
    PublishBcmControlCmd(bcm_control_cmd_);
}

void Control::PublishBcmControlCmd(BcmControlCmd bcm_control_cmd)
{
    message_manger_ -> PublishBcmControlCmd(bcm_control_cmd);
}

void Control::PublishControlCmd(ControlCmd control_cmd)
{
    message_manger_ -> PublishControlCmd(control_cmd);
}

void Control::PublishControlInfoReport(ControlInfoReport control_info_report)
{
    message_manger_ -> PublishControlInfoReport(control_info_report);
}


void  Control::ReadConfigFile()
{
    message_type_ = main_config_file_ -> Read("MESSAGE_TYPE",message_type_);

    switch(message_type_)
    {
    case LCM:
        {
            string obu_url = "udpm://239.255.76.64:7604?ttl=3";
            std::cout<<"message type:LCM"<<endl;
            ResigerMessageManger(&lcm_message_manger_);

            message_manger_ -> Init(main_config_file_ -> Read("OBU_URL",obu_url),&control_logic_);
        }
        break;
    default:
        {
            Logging::LogInfo(Logging::ERROR,"unknown message type");
        }
        break;
    }

     ///调试开关
     control_logic_config_.gui_enable_ = gui_enable_;

     ///车辆类型
     control_logic_config_.vehicle_type_ = vehicle_type_;
     ///车辆类型名
     control_logic_config_.vehicle_type_name_ = vehicle_type_name_;

     debug_driving_mode_ = vehicle_config_file_ -> Read("DEBUG_DRIVING_MODE",0);
     control_logic_config_.debug_driving_mode_ = debug_driving_mode_;
     ///转向工作模式 1 = 自动驾驶 0 = 非自动驾驶
     debug_steering_driving_mode_ = vehicle_config_file_ -> Read("DEBUG_LAT_DRIVING_MODE",0);
     control_logic_config_.debug_steering_driving_mode_ = debug_steering_driving_mode_;

     ///纵向控制工作模式 1 = 自动驾驶 0 = 非自动驾驶
     debug_brake_driving_mode_ = debug_acc_driving_mode_ = vehicle_config_file_ -> Read("DEBUG_LON_DRIVING_MODE",0);
     control_logic_config_.debug_acc_driving_mode_ = debug_acc_driving_mode_;
     control_logic_config_.debug_brake_driving_mode_ = debug_brake_driving_mode_;

     ///EPB控制工作模式 1 = 自动驾驶 0 = 非自动驾驶
     debug_epb_driving_mode_ = vehicle_config_file_ -> Read("DEBUG_EPB_DRIVING_MODE",0);
     control_logic_config_.debug_epb_driving_mode_ = debug_epb_driving_mode_;

     ///前轮侧偏刚度
     vehicle_cf_ = vehicle_config_file_ -> Read("VEHICLE_CF",0.0);
     control_logic_config_.cf_ = vehicle_cf_;

     ///后轮侧偏刚度
     vehicle_cr_ = vehicle_config_file_ -> Read("VEHICLE_CR",0.0);
     control_logic_config_.cr_ = vehicle_cr_;

     ///车辆重量
     vehicle_weight_ = vehicle_config_file_ -> Read("VEHICLE_WEIGHT",0.0);
     control_logic_config_.vehicle_weight_ = vehicle_weight_;

     ///车辆高度
     vehicle_height_ = vehicle_config_file_ -> Read("VEHICLE_HEIGHT",0.0);
     control_logic_config_.h_ = vehicle_height_;
     ///车长
     vehicle_length_ = vehicle_config_file_ -> Read("VEHICLE_LENGTH",0.0);
     control_logic_config_.vehicle_length_ = vehicle_length_;

     ///车辆宽度
     vehicle_width_ = vehicle_config_file_ -> Read("VEHICLE_WIDTH",0.0);
     control_logic_config_.vehicle_width_ = vehicle_width_;

      ///轴距
     wheelbase_ = vehicle_config_file_ -> Read("WHEELBASE",0.0);
     control_logic_config_.wheelbase_ = wheelbase_;
     ///前轮轴距
     vehicle_l_front_ = vehicle_config_file_ -> Read("VEHICLE_L_FRONT",0.0);
     control_logic_config_.lf_ = vehicle_l_front_;
     ///后轮轴距
     vehicle_l_after_ = vehicle_config_file_ -> Read("VEHICLE_L_AFTER",0.0);
     control_logic_config_.lr_ = vehicle_l_after_;
     ///车轮半径
     vehicle_wheel_radius_ = vehicle_config_file_ -> Read("VEHICLE_WHEEL_RADIUS",0.0);
     control_logic_config_.wheel_radius_ = vehicle_wheel_radius_;
     ///转向比
     k_trans_ = vehicle_config_file_ -> Read("K_TRANS",k_trans_);
     control_logic_config_.steer_tranmission_ratio_ = k_trans_;
     ///最大刹车值
     max_brake_value_ = vehicle_config_file_ -> Read("MAX_BRAKE_VALUE",0.0);
     control_logic_config_.max_brake_value_ = max_brake_value_;
     ///怠速状态最大减速度
     max_deceleration_in_idle_ = vehicle_config_file_ -> Read("MAX_DECELERATION_IN_IDLE",0.0);
     control_logic_config_.max_deceleration_in_idle_ = max_deceleration_in_idle_;
     ///车辆限速
     vechile_speed_max_ = vehicle_config_file_ -> Read("VECHILE_SPEED_MAX",0.0);
     control_logic_config_.vechile_speed_max_ = vechile_speed_max_;
     ///横向控制动态kp
     moving_kp_ = vehicle_config_file_ -> Read("MOVING_KP",0.0);
     control_logic_config_.moving_kp_ = moving_kp_;
     ///横向控制PID调节P值
     lat_kp_value_ = vehicle_config_file_ -> Read("LAT_KP_VALUE",0.0);
     control_logic_config_.lat_kp_ = lat_kp_value_;
     ///横向控制PID调节I值
     lat_ki_value_ = vehicle_config_file_ -> Read("LAT_KI_VALUE",0.0);
     control_logic_config_.lat_ki_ = lat_ki_value_;
     ///横向控制PID调节D值
     lat_kd_value_ = vehicle_config_file_ -> Read("LAT_KD_VALUE",0.0);
     control_logic_config_.lat_kd_ = lat_kd_value_;
     ///纵向控制PID调节P值
     lon_kp_value_ = vehicle_config_file_ -> Read("LON_KP_VALUE",0.0);
     control_logic_config_.lon_kp_ = lon_kp_value_;
     ///纵向控制PID调节I值
     lon_ki_value_ = vehicle_config_file_ -> Read("LON_KI_VALUE",0.0);
     control_logic_config_.lon_ki_ = lon_ki_value_;
     ///纵向控制PID调节D值
     lon_kd_value_ = vehicle_config_file_ -> Read("LON_KD_VALUE",0.0);
     control_logic_config_.lon_kd_ = lon_kd_value_;
     ///地图坐标原点纬度
     origin_lat_ = vehicle_config_file_ -> Read("ORIGIN_LAT",0.0);
     control_logic_config_.origin_lat_  = origin_lat_;
     ///地图坐标原点经度
     origin_lon_ = vehicle_config_file_ -> Read("ORIGIN_LON",0.0);
     control_logic_config_.origin_lon_ = origin_lon_;
     ///位置误差门限值
     max_position_error_ = vehicle_config_file_ -> Read("MAX_POSITION_ERROR",0.0);
     control_logic_config_.max_position_error_ = max_position_error_;
     ///最大转向角
     max_steering_angle_ = vehicle_config_file_ -> Read("MAX_STEERING_ANGLE",0.0);

     control_logic_config_.max_steering_angle_ = max_steering_angle_;
     ///最小转向角
     min_steering_angle_ = vehicle_config_file_ -> Read("MIN_STEERING_ANGLE",0.0);
     control_logic_config_.min_steering_angle_ = min_steering_angle_;

    //动态kp值变化 suggest_kp = steer_angle/kp_slope_ + kp_value_
    kp_slope_ = vehicle_config_file_ -> Read("KP_SLOPE",0.0);
    control_logic_config_.kp_slope_ = kp_slope_;
    kp_value_ = vehicle_config_file_ -> Read("KP_VALUE",0.0);
    control_logic_config_.kp_value_ = kp_value_;
    ///预描距离
    xla_ = vehicle_config_file_ -> Read("XLA",20.0);
    control_logic_config_.xla_ = xla_;
    ///位置误差比重
    k_e_err_ = vehicle_config_file_ -> Read("K_E_ERR",1.0);
    control_logic_config_.k_e_err_ = k_e_err_;
    ///角度误差比重
    k_fi_err_ = vehicle_config_file_ -> Read("K_FI_ERR",1.0);
    control_logic_config_.k_fi_err_ = k_fi_err_;
    ///总误差比重
    k_ela_ = vehicle_config_file_ -> Read("K_ELA",1.0);
    control_logic_config_.k_ela_ = k_ela_;

        ///LQR Q加权矩阵
    int32_t lqr_matrix_q_size = vehicle_config_file_ -> Read("lqr_matrix_q_size",5);
    for(int i = 0;i < lqr_matrix_q_size;i++)
    {
        std::string str;
        stringstream ss;
        ss<<i;
        std::string s1 = ss.str();
        str = "lqr_matrix_q_" + s1;
        lqr_matrix_q_.push_back(vehicle_config_file_ -> Read(str,0));
        //std::cout<<"str:"<<str<<endl;
        control_logic_config_.lqr_matrix_q_.push_back(vehicle_config_file_ -> Read(str,0.0));
        //std::cout<<"q:"<<lqr_matrix_q_[i]<<endl;
    }
    ///控制器选择
    controller_switch_ = vehicle_config_file_ -> Read("controller_switch",1);
    control_logic_config_.controller_switch_ = controller_switch_;

    ///LQR离散时长
    lqr_ts_ = vehicle_config_file_ -> Read("lqr_ts",0.01);
    control_logic_config_.lqr_ts_ = lqr_ts_;
    ///LQR预测窗口大小
    lqr_preview_window_ = vehicle_config_file_ -> Read("lqr_preview_window",0);
    control_logic_config_.lqr_preview_window_ = lqr_preview_window_;
    ///LQR计算阀值
    lqr_eps_ = vehicle_config_file_ -> Read("lqr_eps",0.01);
    control_logic_config_.lqr_eps_ = lqr_eps_;
    ///LQR滤波器窗口大小
    lqr_mean_filter_window_size_ = vehicle_config_file_ -> Read("lqr_mean_filter_window_size",10);
    control_logic_config_.lqr_mean_filter_window_size_ = lqr_mean_filter_window_size_;
    ///LQR最大迭代次数
    lqr_max_iteration_ = vehicle_config_file_ -> Read("lqr_max_iteration",150);
    control_logic_config_.lqr_max_iteration_ = lqr_max_iteration_;
    ///LQR横向最大加速度
    lqr_max_lateral_acceleration_ = vehicle_config_file_ -> Read("lqr_max_lateral_acceleration",5.0);
    control_logic_config_.lqr_max_lateral_acceleration_ = lqr_max_lateral_acceleration_;
    ///最小速度保护
    lqr_minimum_speed_protection_ = vehicle_config_file_ -> Read("lqr_minimum_speed_protection",0.1);
    control_logic_config_.lqr_minimum_speed_protection_ = lqr_minimum_speed_protection_;
    ///
    lqr_cutoff_freq_ = vehicle_config_file_ -> Read("lqr_cutoff_freq",10);
    control_logic_config_.lqr_cutoff_freq_ = lqr_cutoff_freq_;
    ///
    lqr_mean_filter_window_size_ = vehicle_config_file_ -> Read("lqr_mean_filter_window_size",10);
    control_logic_config_.lqr_mean_filter_window_size_ = lqr_mean_filter_window_size_;
//    ///横向误差调节器 避免误差过大的时候有较大调节
//    std::vector<Scheduler> lqr_lat_err_scheduler_init_;
//    ///航向角误差调节器 避免误差过大的时候有较大调节
//    std::vector<Scheduler> lqr_heading_err_scheduler_init_;
//    control_logic_config_.lqr_lat_err_scheduler_init_.assign(lqr_lat_err_scheduler_init_.begin(),lqr_lat_err_scheduler_init_.end());
//    control_logic_config_.lqr_heading_err_scheduler_init_.assign(lqr_heading_err_scheduler_init_.begin(),lqr_heading_err_scheduler_init_.end());
}

}
}
