/**
 * @file    control.h
 * @author  jiang <jiangchengjie@indrv.cn>
 * @date    2018-07-07
 * @version 1.0.0
 * @par     Copyright(c)
 *          hy
 */

#ifndef APPS_CONTROL_H_
#define APPS_CONTROL_H_

#include "message_manger/message_manger.h"
#include "message_manger/lcm/lcm_message_manger.h"
#include "../common/logging.h"
#include "../common/Config.h"
#include "../common/enum.h"
#include "../common/control_info_report.h"
#include "../common/color_init.h"
#include "../control_logic/control_logic.h"
#include "../control_logic/control_logic_config.h"
#include "controller.h"
#include "track_trajectory/track_trajectory.h"
#include "control_debug/control_debug.h"


 /**
 * @namespace athena::control
 * @brief athena::control
 */
namespace athena{
namespace control{

 /**
 * @class Control
 * @brief 控制类.
 */
class Control:public Thread{
 public:
  Control(std::string software_version);
  ~Control() = default;

 /**
  * @brief     程序执行入口.
  * @return    void.
  */
  void Init();


 private:
    ///软件版本
    std::string software_version_;
     ///循迹
    TrackTrajectory *track_trajectory_;
     ///配置文件路径
    const std::string config_file_path_ = "config/control.cfg";
    ///配置文件操作类
    Config *main_config_file_;
    ///车辆配置文件操作类
    Config *vehicle_config_file_;
    ///车辆配置文件路径
    std::string vehicle_config_file_path_;
    ///控制逻辑设置
    ControlLogicConfig control_logic_config_;
    ///LCM消息控制器
    LcmMessageManger lcm_message_manger_;
    ///消息控制器
    MessageManger *message_manger_;
    ///控制命令
    ControlCmd control_cmd_;
    ///BCM控制命令
    BcmControlCmd bcm_control_cmd_;
    ///控制信息
    ControlInfoReport control_info_report_;
    ///控制逻辑-
    ControlLogic control_logic_;
    ///控制逻辑调试输出
    ControlLogicDebugOutput control_logic_debug_output_;
    ///消息控制器注册
    void ResigerMessageManger(MessageManger *message_manger);


    /**
     * @brief 发送控制输出.
     * @return void.
     */
    void PublishControlOutputOnTimer();
   /**
    * @brief     读配置文件
    * @param[in] file_path 配置文件路径..
    * @return    void.
    */
    void  ReadConfigFile();

    /**
    * @brief     控制消息发布.
    * @param[in] control_cmd 控制输出.
    * @return    void.
    */
    void PublishControlCmd(ControlCmd control_cmd);

   /**
    * @brief     BCM控制信息发送.
    * @param[in] bcm_control_cmd BCM控制信息.
    * @return    void.
    */
    void PublishBcmControlCmd(BcmControlCmd bcm_control_cmd);

   /**
    * @brief     控制消息上报.
    * @param[in] control_info_report 控制信息.
    * @return    void.
    */
    void PublishControlInfoReport(ControlInfoReport control_info_report);

    /**
    * @brief     打印调试.
    * @return    void.
    */
    void print();

    /**
    * @brief     日志调试.
    * @return    void.
    */
    void log();

 public:
        ///调试开关
    int32_t debug_enable_;
    ///打印记录开关
    int32_t print_enable_;
    ///日志记录开关
    int32_t log_enable_;
    ///日志打印周期
    int32_t debug_print_period_;
    ///日志记录周期
    int32_t debug_log_period_;

 private:
    ///GUI调试
    int32_t gui_enable_;

    ///循迹
    int32_t init_map_from_file_;
    ///本地轨迹路径
    std::string local_trajectory_path;

    ///消息类型
    int32_t message_type_;

    ///车辆类型
    int32_t vehicle_type_;
    ///车辆类型名
    std::string vehicle_type_name_;

    /// 横向参数
    ///横向控制动态kp值
    double moving_kp_ = 3500;
    /// 横向控制kp计算
    double lat_kp_value_=2.0;
    /// 横向控制kI计算
    double lat_ki_value_=0.0;
    /// 横向控制kd计算
    double lat_kd_value_=0.0;
    /// 纵向控制kp计算
    double lon_kp_value_=2.0;
    /// 纵向控制kI计算
    double lon_ki_value_=0.0;
    /// 纵向控制kd计算
    double lon_kd_value_=0.0;

    //动态kp值变化 suggest_kp = steer_angle/kp_slope_ + kp_value_
    double kp_slope_;
    double kp_value_;
    ///预描距离
    double xla_;
    /// 横向->前馈参数
    /// 轮子到方向盘的传动比（默认：11.5，该值即影响前馈，也影响反馈）
    double k_trans_ = 8.2;
    /// 横向->反馈参数
    /// 位置误差比重
    double k_e_err_ = 2.1;
    /// 角度误差比重
    double k_fi_err_ = 1.0;
    /// 总误差比重（反馈权重，默认1,长安1.28）
    double k_ela_ = 1.28;
    /// 方向盘右转最大角度
    double max_steering_angle_ = 431;
    /// 方向盘左转最大角度
    double min_steering_angle_ = -445;
    /// 最高转向速度，300度/秒
    double max_steering_speed_ = 300.0;

    /// 纵向参数
    /// 纵向->限速参数
    /// 最高速度10m/s
    double vechile_speed_max_ = 10.0;
    /// 最大油门值
    double acc_output_mv_max_ = 40.0;
    /// 最小油门值
    double acc_output_mv_min_ = 0.0;
    /// 纵向->刹车参数
    /// 电子驻车使能，默认为1
    int32_t epb_config_enable_flag_ = 1;
    ///最大刹车值
    double max_brake_value_ = -3;
    ///怠速时最大减速度
    double max_deceleration_in_idle_ = -0.8;
    /// 纵向->车队参数
    /// 编队模式刹车前馈系数，默认1.0
    double platoon_brake_forward_k_ = 1.0;

    /// 地图匹配
    /// 地图最少匹配点：30个（3米），少于3米的地图规划，将不响应
    double map_point_lim_min_ = 30;
    /// 地图最多匹配点：50000个（5公里），多于5公里的地图规划，将不响应
    double map_point_lim_max_ = 50000;
    /// 地图原点.纬度
    double origin_lat_ = 31.281675599;
    /// 地图原点.经度
    double origin_lon_ = 121.16317409;

    /// 车身参数（常量）
    ///车辆高度,单位m
    double vehicle_height_ = 1.727;
    /// 轴距长度，单位m
    double vehicle_length_ = 2.64;
    ///车辆宽度
    double vehicle_width_= 1.575;
    ///轮距
    double wheelbase_ = 1.720;
    /// 前轴距长度，单位m
    double vehicle_l_front_ = 1.2;
    /// 后轴距长度，单位m
    double vehicle_l_after_ = 1.64;
    /// 车身质量，单位kg
    double vehicle_weight_ = 1577.0;
    /// 前轮侧偏刚度
    double vehicle_cf_ = 190000.0;
    /// 后轮侧偏刚度
    double vehicle_cr_ = 210000.0;
    ///车轮半径 单位mi
    double vehicle_wheel_radius_ = 0.335;

    ///位置误差门限值
    double max_position_error_ = 2;

    ///自动驾驶模式
    ///工作模式调试 可以单独进行横向或者纵向控制
    int32_t    debug_driving_mode_;
    ///转向工作模式 1 = 自动驾驶 0 = 非自动驾驶
    int32_t    debug_steering_driving_mode_;
    ///纵向控制工作模式 1 = 自动驾驶 0 = 非自动驾驶
    int32_t    debug_acc_driving_mode_;
    ///刹车控制工作模式 1 = 自动驾驶 0 = 非自动驾驶
    int32_t    debug_brake_driving_mode_;
    ///EPB控制工作模式 1 = 自动驾驶 0 = 非自动驾驶
    int32_t    debug_epb_driving_mode_;
    ///控制器选择
    int32_t    controller_switch_;
    ///LQR Q加权矩阵
    std::vector<double> lqr_matrix_q_;
    ///LQR离散时长
    double lqr_ts_=0.01;
    ///LQR预测窗口大小
    double lqr_preview_window_=0;
    ///LQR计算阀值
    double lqr_eps_=0.01;
    ///LQR滤波器窗口大小
    double lqr_mean_filter_window_size_=10;
    ///LQR最大迭代次数
    double lqr_max_iteration_=150;
    ///LQR横向最大加速度
    double lqr_max_lateral_acceleration_=5.0;
    ///最小速度保护
    double lqr_minimum_speed_protection_=0.1;
    ///
    int32_t lqr_cutoff_freq_;
    ///横向误差调节器 避免误差过大的时候有较大调节
    std::vector<Scheduler> lqr_lat_err_scheduler_init_;
    ///航向角误差调节器 避免误差过大的时候有较大调节
    std::vector<Scheduler> lqr_heading_err_scheduler_init_;

 private:
    ControlDebug<Control> *control_debug_;
    void run();
};
}
}

#endif // APPS_CONTROL_H_
