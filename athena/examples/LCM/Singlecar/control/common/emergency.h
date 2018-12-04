/**
 * @file    emergency.h
 * @author  jiang <jiangchengjie@indrv.cn>
 * @date    2018-07-07
 * @version 1.0.0
 * @par     Copyright(c)
 *          hy
 */

#ifndef COMMON_EMERGENCY_H_
#define COMMON_EMERGENCY_H_

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
class Emergency
{
public:
    Emergency() = default;
    ~Emergency() =default;
    typedef enum
    {
        NORMAL = 0,
        EMERGENCY_BRAKING = 1,
    }EmergencyMode;

    typedef enum
    {
        OK = 0,
        EMERGENCY_ONLY_IN_AUTO_DRIVING= 1,
        ALL_SITUATION = 2,
    }EmergencyLevel;
    /// 紧急模式 0 非紧急状态 1 急
    int32_t    emergency_mode_;
    ///紧急刹车等级，0 无紧急模式  1 仅在自动驾驶时有  2 所有状态下都有
    int32_t    emergency_level_;
    ///刹车值
    double     emergency_value_;
};
}//namespace control
}//namespace athena
#endif //COMMON_EMERGENCY_H_
