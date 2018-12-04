/**
 * @file    trajectory.h
 * @author  jiang <jiangchengjie@indrv.cn>
 * @date    2018-07-07
 * @version 1.0.0
 * @par     Copyright(c)
 *          hy
 */

#ifndef TRAJECTORY_H_
#define TRAJECTORY_H_

#include <iostream>
#include <vector>
#include "nav_points.h"

using namespace std;
 /**
 * @namespace athena::control
 * @brief     athena::control
 */
namespace athena{
namespace control{

/**
 * @class Trajectory
 *
 * @brief motion Trajectory.
 */
class Trajectory
{
 public:
 Trajectory()
 {
    gps_time_       = 0.0;
    num_of_points_  = 0;
    type_           = 0;
    reserved_       = 0;
    points_.clear();
    car_action_     = 0;
    driving_mode_   = 0;
 }
 ~Trajectory() = default;

 ///GPS时间
 double     gps_time_;
 ///轨迹点数量
 int32_t    num_of_points_;
 ///轨迹点类型
 int32_t    type_;
 ///保留
 double     reserved_;
 ///轨迹点
 std::vector<NavPoints> points_;
 int32_t    car_action_;
 int32_t    driving_mode_;
};

}
}

#endif // TRAJECTORY_H_
