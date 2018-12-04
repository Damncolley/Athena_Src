
/**
 * @file    control_logic.h
 * @author  jiang <jiangchengjie@indrv.cn>
 * @date    2018-07-07
 * @version 1.0.0
 * @par     Copyright(c)
 *          hy
 */

#ifndef COMMON_CONTROL_CMD_H_
#define COMMON_CONTROL_CMD_H_

#include "chassis.h"
#include "trajectory.h"
#include "localization.h"
#include "controller_output.h"
#include "controller.h"

 /**
 * @namespace athena::control
 * @brief athena::control
 */
namespace athena{
namespace control{
/**
 * @class ControlCmd
 * @brief 控制命令.
 */
class ControlCmd
{
public:
    ControlCmd() = default;
    ~ControlCmd() =default;
    ///方向盘转角，单位：度
   	double  steering_angle_;
   	///方向盘角速度，单位：度/s
   	double  steering_angle_speed_;
   	///驾驶模式：人工驾驶 1、自动驾驶 3 和辅助驾驶 2
   	int32_t steering_driving_mode_;
   	///油门控制输出
   	double  acc_value_;
   	///驾驶模式：人工驾驶 1、自动驾驶 3 和辅助驾驶 2
   	int32_t accelerate_driving_mode_;
   	///刹车值
   	double  brake_value_;
   	///驾驶模式：人工驾驶 1、自动驾驶 3 和辅助驾驶 2
   	int32_t brake_driving_mode_;
   	///EPB 状态控制
   	int32_t epb_enable_;
   	///驾驶模式：人工驾驶 1、自动驾驶 3 和辅助驾驶 2
   	int32_t epb_driving_mode_;
   	///档杆位置
   	int32_t gear_lever_;
};
}//namespace control
}//namespace athena

#endif
