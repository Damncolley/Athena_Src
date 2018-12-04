/**
 * @file    controller_config.h
 * @author  jiang <jiangchengjie@indrv.cn>
 * @date    2018-07-07
 * @version 1.0.0
 * @par     Copyright(c)
 *          hy
 */

#ifndef CONTROLLER_CONFIG_H_
#define CONTROLLER_CONFIG_H_

#include <functional>
#include <vector>
#include "scheduler.h"

//using namespace std;

/**
 * @namespace athena::control
 * @brief     athena::control
 */
namespace athena
{
namespace control
{

///时间格式
typedef struct
{
    ///年
    int32_t year;
    ///月
    int32_t month;
    ///日
    int32_t day;
    ///时
    int32_t hour;
    ///分
    int32_t minute;
    ///秒
    int32_t second;
    ///毫秒
    int32_t m_second;
} Tm;

/**
 * @class ControllerConfig
 *
 * @brief ControllerConfig.
 */
class ControllerConfig
{
  public:
    ControllerConfig()
    {
        steering_driving_mode_ = 0;
        acc_driving_mode_ = 0;
        brake_driving_mode_ = 0;
        epb_driving_mode_ = 0;
        cf_ = 0.0;
        cr_ = 0.0;
        vehicle_weight_ = 0.0;
        vehicle_length_ = 0.0;
        lf_ = 0.0;
        lr_ = 0.0;
        h_ = 0.0;
        vehicle_width_ = 0.0;
        wheel_radius_ = 0.0;
        wheelbase_ = 0.0;
        steer_tranmission_ratio_ = 0.0;
        max_brake_value_ = 0.0;
        max_deceleration_in_idle_ = 0.0;
        min_speed_ = 0.0;
        moving_kp_ = 0.0;
        lat_kp_ = 0.0;
        lat_ki_ = 0.0;
        lat_kd_ = 0.0;
        lon_kp_ = 0.0;
        lon_ki_ = 0.0;
        lon_kd_ = 0.0;
        origin_lat_ = 0.0;
        origin_lon_ = 0.0;
        max_position_error_ = 0.0;
        max_steering_angle_ = 0.0;
        min_steering_angle_ = 0.0;
        get_acc_value_callback_ = NULL;
        get_brake_value_callback_ = NULL;
        get_current_time_callback_ = NULL;

        kp_slope_ = 30.0;
        kp_value_ = 2.0;
        ///预描距离
        xla_ = 40.0;
        ///位置误差比重
        k_e_err_ = 2.1;
        ///角度误差比重
        k_fi_err_ = 1.0;
        ///总误差比重
        k_ela_ = 1.28;
    }
    ~ControllerConfig() = default;

    typedef enum : int32_t
    {
        INFO = 1,    /**< 信息*/
        WARNING = 2, /**< 警告*/
        ERROR = 3,   /**< 错误*/
    } LogLevel;

    ///获取节气门开度的回调函数
    typedef std::function<double(double, double)> GetAccValueCallBack;
    ///获取刹车值的回调函数
    typedef std::function<double(double)> GetBrakeValueCallBack;
    ///获取当前时间(UTC)的回调函数
    typedef std::function<void(int &year, int &month, int &day, int &hour, int &minute, int &second, int &millisecond)> GetCurrentTimerCallBack;
    ///日志记录回调函数 参数1=等级 详参考枚举LogLevel 参数2=打印信息
    typedef std::function<void(int, std::string)> GetLogCallBack;

    ///转向工作模式 1 = 自动驾驶 0 = 非自动驾驶
    int32_t steering_driving_mode_;
    ///纵向控制工作模式 1 = 自动驾驶 0 = 非自动驾驶
    int32_t acc_driving_mode_;
    ///刹车控制工作模式 1 = 自动驾驶 0 = 非自动驾驶
    int32_t brake_driving_mode_;
    ///EPB控制工作模式 1 = 自动驾驶 0 = 非自动驾驶
    int32_t epb_driving_mode_;
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
    ///轴距离
    double wheelbase_;
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
    ///限速值 m/s
    double vechile_speed_max_;

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

    ///获取节气门开度的回调函数
    GetAccValueCallBack get_acc_value_callback_;
    ///获取刹车值的回调函数的回调函数
    GetBrakeValueCallBack get_brake_value_callback_;
    ///获取当前时间(UTC)的回调函数
    GetCurrentTimerCallBack get_current_time_callback_;
    ///日志记录回调函数.
    GetLogCallBack get_log_callback_;
    ///LQR Q加权矩阵
    std::vector<double> lqr_matrix_q_;
    ///控制器选择
    int32_t controller_switch_ = 0.0;
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
} // namespace control
} // namespace athena

#endif // CONTROLLER_CONFIG_H_
