/**
 * @file    control_logic_config.h
 * @author  jiang <jiangchengjie@indrv.cn>
 * @date    2018-07-07
 * @version 1.0.0
 * @par     Copyright(c)
 *          hy
 */

#ifndef CONTROL_LOGIC_CONTROL_LOGIC_CONFIG_H_
#define CONTROL_LOGIC_CONTROL_LOGIC_CONFIG_H_

#include <functional>

 /**
 * @namespace athena::control
 * @brief     athena::control
 */
namespace athena{
namespace control{

/**
 * @class ControlLogicConfig
 *
 * @brief ControlLogicConfig.
 */
class ControlLogicConfig{
public:
    ControlLogicConfig() = default;
    ~ControlLogicConfig() = default;
    ///调试开关
    int32_t gui_enable_;
    ///车辆类型
    int32_t vehicle_type_;
    ///车辆类型名
    std::string vehicle_type_name_;
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
    ///前轮侧偏刚度
    double cf_;
    ///后轮侧偏刚度
    double cr_;
    ///车辆重量
    double vehicle_weight_;
    ///车长
    double vehicle_length_;
    ///前轮轴距
    double lf_;
    ///后轮轴距
    double lr_;
    ///车辆高度
    double h_;
    ///前轮轮距
    double vehicle_width_;
    ///轮距
    double wheelbase_ = 1.720;
    ///车轮半径
    double wheel_radius_;
    ///转向比
    double steer_tranmission_ratio_;
    ///最大刹车值
    double max_brake_value_;
    ///怠速状态最大减速度
    double max_deceleration_in_idle_;
    ///车辆最小速度
    double min_speed_;
    ///车辆限速
    double vechile_speed_max_;
    ///横向控制动态kp
    double moving_kp_;
    ///横向控制PID调节P值
    double lat_kp_;
    ///横向控制PID调节I值
    double lat_ki_;
    ///横向控制PID调节D值
    double lat_kd_;
    ///纵向控制PID调节P值
    double lon_kp_;
    ///纵向控制PID调节I值
    double lon_ki_;
    ///纵向控制PID调节D值
    double lon_kd_;
    ///地图坐标原点纬度
    double origin_lat_;
    ///地图坐标原点经度
    double origin_lon_;
    ///位置误差门限值
    double max_position_error_;
    ///最大转向角
    double max_steering_angle_;
    ///最小转向角
    double min_steering_angle_;

    //动态kp值变化 suggest_kp = steer_angle/kp_slope_ + kp_value_
    double kp_slope_;
    double kp_value_;
    ///预描距离
    double xla_;
    ///位置误差比重
    double k_e_err_;
    ///角度误差比重
    double k_fi_err_;
    ///总误差比重
    double k_ela_;

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
};
}
}

#endif // CONTROL_LOGIC_CONTROL_LOGIC_CONFIG_H_
