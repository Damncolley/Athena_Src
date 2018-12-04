/**
 * @file
 *
 * @brief 导航点属性，
 */

#pragma once

//#include <vector>
#include <stdint.h>

/**
 * @class navi_point
 * @brief 规划导航点，
 */
class navi_point
{
public:
    double position_x;              ///<x坐标
    double position_y;              ///<y坐标
    double position_z;              ///<z坐标

    double speed;                   ///<速度
    double lateral_speed;           ///<速度横向分量
    double longitudinal_speed;      ///<速度纵向分量
    double down_speed;              ///<速度垂直方向分量

    double roll;                    ///<横滚角
    double pitch;                   ///<俯仰角
    double heading;                 ///<航向角

    double accelerataion;           ///<加速度
    double lateral_accelerate;      ///<加速度横向分量
    double longitudinal_accelerate; ///<加速度纵向分量
    double down_accelerate;         ///<加速度垂直方向分量

    double steering_angle;          ///<方向盘转角
    double steering_angle_speed;    ///<方向盘转角速度

    double speed_desired_Uxs;       ///<期望速度
    double acceleration_desired_Axs;///<期望加速度

    int point_no;                   ///<点号
    double pos_gps_time;            ///<GPS时间

    int8_t gears;                   ///<档位
    double s;                       ///<里程
    double r;                       ///<该点代价值
    double k_s;                     ///<曲率

    int8_t type;                    ///<行为点类型，20：驶入路口点；21：红绿灯停车点；22：驶出路口点

    double lateral_offset;          ///<横向偏移量

public:
    /**
    * @brief 构造函数
    */
    navi_point();
    /**
    * @brief 析构函数
    */
    ~navi_point();

    //int navi_point& operator= (const navi_point& src);
};

/**
* @brief 计算两个导航点之间的欧几里得距离,
* @param p1 输入量：第一个点。
* @param p2 输入量：第二个点。。
* @return p1和p2之间的直线距离。
*/
double length_of_two_navipoint(navi_point p1, navi_point p2);

