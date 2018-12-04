
/**
 * @file    bcm_control_cmd.h
 * @author  jiang <jiangchengjie@indrv.cn>
 * @date    2018-07-07
 * @version 1.0.0
 * @par     Copyright(c)
 *          hy
 */

#ifndef COMMON_BCM_CONTROL_CMD_H_
#define COMMON_BCM_CONTROL_CMD_H_
 /**
 * @namespace athena::control
 * @brief athena::control
 */
namespace athena{
namespace control{
/**
 * @class ChassisDetail
 * @brief 车辆底盘信息.
 */
class BcmControlCmd
{
public:
    BcmControlCmd() = default;
    ~BcmControlCmd() = default;
    ///喇叭控制 0 禁声音 1 鸣笛
    int8_t    speaker_control_;
    ///远光灯 0 关闭 1 开启
    int8_t    high_beam_ctrl_;
    ///近光灯 0 关闭 1 开启
    int8_t    low_beam_ctrl_;
    ///左转向灯 0 关闭 1 开启
    int8_t    left_turn_ctrl_;
    ///右转向灯 0 关闭 1 开启
    int8_t     right_turn_ctrl_;
    ///前雨刮器 0 关闭 1 开启
	int8_t     front_wiper_ctrl_;
    ///后雨刮器 0 关闭 1 开启
    int8_t     rear_wiper_ctrl_;
    ///位置灯 0 关闭 1 开启
    int8_t     position_lamp_ctrl_;
    ///前雾灯 0 关闭 1 开启
    int8_t     front_fog_lamp_ctrl_;
    ///后雾灯 0 关闭 1 开启
    int8_t     rear_fog_lamp_ctrl_;
    ///刹车灯 一般情况自动控制 0 关闭 1 开启
    int8_t     brake_lamp_ctrl_;
    ///警报灯 双闪 0 关闭 1 开启
    int8_t     alarm_lamp_ctrl_;
    /// 左前门控制 0 关闭 1 开启
    int8_t     lf_door_ctrl_;
    /// 右前门控制 0 关闭 1 开启
    int8_t     rf_door_ctrl_;
    /// 左后门控制 0 关闭 1 开启
    int8_t     lr_door_ctrl_;
    /// 右后门控制 0 关闭 1 开启
    int8_t     rr_door_ctrl_;
};
}//namespace control
}//namespace athena

#endif //COMMON_BCM_CONTROL_CMD_H_
