/**
 * @file    controller_output_alarm.h
 * @author  jiang <jiangchengjie@indrv.cn>
 * @date    2018-07-07
 * @version 1.0.0
 * @par     Copyright(c)
 *          hy
 */

#ifndef CONTROLLER_ALARM_CODE_H_
#define CONTROLLER_ALARM_CODE_H_


 /**
 * @namespace athena::control
 * @brief athena::control
 */
namespace athena{
namespace control{


/**
 * @class ControllerOutputAlarm
 *
 * @brief 控制警报输出.
 */
enum  ControllerAlarmCode:int32_t
{
 OK                                           = 0,
 CONTROLLER_INIT_ERROR                        = 1000,/**< 初始化失败*/
 CONTROLLER_OFF_TRACK_ERROR                   = 1000,/**< 偏离轨迹*/
 CONTROLLER_TRAJECTORY_IS_SHORT_OR_LONG_ERROR = 1001,/**< 剩余轨迹少于10米*/
 CONTROLLER_TRAJECTORY_ERROR                  = 1002,/**< 轨迹错误*/
 CONTROLLER_TRAJECTORY_SET_TIMEOUT            = 1003,/**< 轨迹设置超时*/
 CONTROLLER_COMPUTE_TIMEOUT                   = 1004,/**< 控制器计算超时*/
 CONTROLLER_COMPUTE_ERROR                     = 1005,/**< 控制器计算错误*/
};

enum  ControllerAlarmCodeLevel:int32_t
{
 NORMAL        = 0,
 LOW_WARNING   = 1,/**< 轻度警告*/
 HIGH_WARNING  = 2,/**< 重度警告*/
 ERROR         = 3,/**< 错误*/
};
}
}

#endif // CONTROLLER_OUTPUT_H_
