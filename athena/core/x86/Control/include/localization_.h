/**
 * @file    localization.h
 * @author  jiang <jiangchengjie@indrv.cn>
 * @date    2018-07-07
 * @version 1.0.0
 * @par     Copyright(c)
 *          hy
 */

#ifndef LOCALIZATION__H_
#define LOCALIZATION__H_

 /**
 * @namespace athena::control
 * @brief     athena::control
 */
namespace athena{
namespace control{

/**
 * @class Localization
 *
 * @brief Location information.
 */
class Localization_
{
 public:
 Localization_()
 {
    gps_time_                 = 0.0;
    week_                     = 0;
    lat_                      = 0.0;
    lon_                      = 0.0;
    height_                   = 0.0;
    lateral_speed_            = 0.0;
    longitudinal_speed_       = 0.0;
    down_speed_               = 0.0;
    roll_                     = 0.0;
    pitch_                    = 0.0;
    heading_                  = 0.0;
    lateral_accelerate_       = 0.0;
    longitudinal_accelerate_  = 0.0;
    down_accelerate_          = 0.0;
    roll_speed_               = 0.0;
    pitch_speed_              = 0.0;
    heading_speed_            = 0.0;
 }
 ~Localization_()  = default;
 ///GPS时间
 double     gps_time_;
 ///周
 int        week_;
 ///经度
 double     lat_;
 ///纬度
 double     lon_;
 ///海拔
 double     height_;
 ///横向速度
 double     lateral_speed_;
 ///纵向速度
 double     longitudinal_speed_;
 ///地向速度
 double     down_speed_;
 ///横滚角度
 double     roll_;
 ///俯仰角度
 double     pitch_;
 ///航向角度
 double     heading_;
 ///横向加速度
 double     lateral_accelerate_;
 ///纵向加速度
 double     longitudinal_accelerate_;
 ///地向加速度
 double     down_accelerate_;
 ///横滚角速度
 double     roll_speed_;
 ///俯仰角速度
 double     pitch_speed_;
 ///航向角速度
 double     heading_speed_;
};
}
}

#endif // LOCALIZATION_H_
