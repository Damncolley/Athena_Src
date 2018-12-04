/**
 * @file
 *
 * @brief 局部规划的输出信息。
 */

 #ifndef _PLANNING_OUTPUT_H
 #define _PLANNING_OUTPUT_H

#include <vector>
#include "common/navi_point.h"

/**
 * @class OutTrajectory
 * @brief motion层生成的轨迹信息（导航精确控制点的序列），最终下发给controller。
 */
class OutTrajectory
{
    public:
        double     gps_time_;                    ///<GPS时间

        int    num_of_points_;                   ///<点的个数

        int    type_;                            ///<轨迹类型

        double     reserved_;                    ///<预留位

        std::vector< navi_point > points_;       ///<导航精确控制点的序列

        int    car_action_;                      ///<1表示单车模式，2表示车队头车模式，3表示车队跟随车模式

        int    driving_mode_;                    ///<0表示人工驾驶，1表示自动驾驶

    public:
        /**
        * @brief 构造函数
        */
        OutTrajectory()
        {
            gps_time_ = 0.0;
            num_of_points_ = 0;
            type_ = 0;
            reserved_ = 0.0;
            points_.clear();
            car_action_ = 0;
            driving_mode_ = 0;
        }
        /**
        * @brief 析构函数
        */
        virtual ~OutTrajectory()
        {

        }
};


/**
 * @class LongitudinalControlInfo
 * @brief motion下发给controller的纵向信息。
 */
class LongitudinalControlInfo
{
    public:
        double     tar_speed_;           ///<目标速度，单位m/s

        double     tar_accelerate_;      ///<目标加速度，单位m/s2


     public:
        /**
        * @brief 构造函数
        */
        LongitudinalControlInfo()
        {
            tar_speed_ = 0.0;
            tar_accelerate_ = 0.0;
        }
        /**
        * @brief 析构函数
        */
        virtual ~LongitudinalControlInfo()
        {

        }
};

#endif  //_PLANNING_OUTPUT_H
