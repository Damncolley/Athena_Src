/**
 * @file    chassis.h
 * @author  jiang <jiangchengjie@indrv.cn>
 * @date    2018-07-07
 * @version 1.0.0
 * @par     Copyright(c)
 *          hy
 */

#ifndef CHASSIS_H_
#define CHASSIS_H_

#include <iostream>

using namespace std;

 /**
 * @namespace athena::control
 * @brief     athena::control
 */
namespace athena{
namespace control{


/**
 * @class Chassis
 *
 * @brief 车辆底盘信息.
 */
class Chassis
{
 public:
 Chassis()
 {
    car_speed_                     = 0.0;
    at_status_feedback_            = 0;
    steering_angle_feedback_       = 0.0;
    steering_angle_speed_feedback_ = 0.0;
    lat_driving_mode_feedback_     = 0;
    throttle_output_feedback_      = 0.0;
    brake_value_feedback_          = 0.0;
    brake_run_time_feedback_       = 0.0;
    lon_driving_mode_feedback_     = 0;
    epb_status_feedback_           = 0;
    epb_driving_mode_feedback_     = 0;
 }
 ~Chassis() = default;
 ///车辆速度 m/s
 double     car_speed_;
 ///档杆位置
 int32_t    at_status_feedback_;
 ///转向角度反馈
 double     steering_angle_feedback_;
 ///转向角速度反馈
 double     steering_angle_speed_feedback_;
 ///转向工作模式 1 = 自动驾驶 0 = 非自动驾驶
 int32_t    lat_driving_mode_feedback_;
 ///节气门输出反馈
 double     throttle_output_feedback_;
 ///刹车值反馈
 double     brake_value_feedback_;
 ///刹车执行时间反馈
 double     brake_run_time_feedback_;
 ///纵向控制工作模式反馈 1 = 自动驾驶 0 = 非自动驾驶
 int32_t    lon_driving_mode_feedback_;
 ///EPB状态反馈
 int32_t    epb_status_feedback_;
 ///EPB控制工作模式反馈 1 = 自动驾驶 0 = 非自动驾驶
 int32_t    epb_driving_mode_feedback_;
};
}
}

#endif // CHASSIS_H_
