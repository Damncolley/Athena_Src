/**
 * @file    local_localization.h
 * @author  jiang <jiangchengjie@indrv.cn>
 * @date    2018-07-07
 * @version 1.0.0
 * @par     Copyright(c)
 *          hy
 */

#ifndef LOCAL_LOCALIZATION_H_
#define LOCAL_LOCALIZATION_H_


class LocalLocalization{
public:
    LocalLocalization()
    {
        current_x_ = 0.0;
        current_y_ = 0.0;
        current_z_ = 0.0;
        current_yaw_ = 0.0;
        current_pitch_ = 0.0;
        current_roll_  = 0.0;
        current_heading_speed_ = 0.0;
        current_roll_speed_ = 0.0;
        current_pitch_speed_ = 0.0;
        current_speed_ = 0.0;
        current_acceleration_ = 0.0;
    }
    ~LocalLocalization() = default;
    ///地图坐标 x
    double current_x_;
    ///地图坐标 x
    double current_y_;
    ///地图坐标 x
    double current_z_;
    ///航向角
    double current_yaw_;
    ///俯仰角
    double current_pitch_;
    ///横滚角
    double current_roll_;
    ///航向角速度
    double current_heading_speed_;
    ///横滚角速度
    double current_roll_speed_;
    ///俯仰角速度
    double current_pitch_speed_;
    ///速度
    double current_speed_;
    ///加速度
    double current_acceleration_;
};

#endif // LOCAL_LOCALIZATION_H_
