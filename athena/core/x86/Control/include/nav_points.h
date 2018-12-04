/**
 * @file    nav_points.h
 * @author  jiang <jiangchengjie@indrv.cn>
 * @date    2018-07-07
 * @version 1.0.0
 * @par     Copyright(c)
 *          hy
 */

#ifndef NAVPOINTS_H_
#define NAVPOINTS_H_

#include <iostream>

using namespace std;
 /**
 * @namespace athena::control
 * @brief     athena::control
 */
namespace athena{
namespace control{

/**
 * @class NavPoints
 *
 * @brief motion Trajectory points.
 */
class NavPoints
{
 public:
 NavPoints()
 {
    gps_time_ = 0.0;
    p_x_      = 0.0;
    p_y_      = 0.0;
    s_        = 0.0;
    p_v_      = 0.0;
    p_a_      = 0.0;
    p_h_      = 0.0;
    p_k_      = 0.0;
    p_g_      = 0;
 }
 ~NavPoints() = default;

 ///GPS 时间
 double     gps_time_;
 ///x值
 double     p_x_;
 ///y值
 double     p_y_;
 ///里程值
 double     s_;
 ///到达该点的速度
 double     p_v_;
 ///到达该点的加速度
 double     p_a_;
 ///到达该点的航向角
 double     p_h_;
 ///到达该点的曲率
 double     p_k_;
 ///到达该点的档位
 int8_t     p_g_;
};
}
}

#endif // NAVPOINTS_H_
