/**
 * @file    controller_output.h
 * @author  jiang <jiangchengjie@indrv.cn>
 * @date    2018-07-07
 * @version 1.0.0
 * @par     Copyright(c)
 *          hy
 */

#ifndef CONTROLLER_OUTPUT_H_
#define CONTROLLER_OUTPUT_H_

#include "controller_output_alarm.h"
 /**
 * @namespace athena::control
 * @brief     athena::control
 */
namespace athena{
namespace control{


/**
 * @class ControllerOutput
 *
 * @brief controller output.
 */
class ControllerOutput
{
 public:
 ControllerOutput()
 {
    steering_angle_        = 0.0;
    steering_angle_speed_  = 0.0;
    steering_driving_mode_ = 0;
    acc_value_             = 0.0;
    brake_value_           = 0.0;
    acc_driving_mode_      = 0;
    brake_driving_mode_      = 0;
    epb_status_            = false;
    epb_driving_mode_      = 0;
 }
 ~ControllerOutput() = default;

 //ControllerOutputAlarm controller_ouput_alarm_;

 ///转向角度
 double     steering_angle_;
 ///转向角速度
 double     steering_angle_speed_;
 ///转向工作模式 1 = 自动驾驶 0 = 非自动驾驶
 int32_t    steering_driving_mode_;
 ///节气门输出
 double     acc_value_;
  ///纵向控制工作模式 1 = 自动驾驶 0 = 非自动驾驶
 int32_t    acc_driving_mode_;
 ///刹车值
 double     brake_value_;
///纵向控制工作模式 1 = 自动驾驶 0 = 非自动驾驶
 int32_t    brake_driving_mode_;
 ///EPB状态
 bool       epb_status_;
 ///EPB控制工作模式 1 = 自动驾驶 0 = 非自动驾驶
 int32_t    epb_driving_mode_;
};
}
}

#endif // CONTROLLER_OUTPUT_H_
