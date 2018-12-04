/**
 * @file
 *
 * @brief 该库提供了一些路面信息，障碍物信息和碰撞检测算法。
 */

#ifndef _COLLISION_CHECK_H
#define _COLLISION_CHECK_H


#pragma once

#include <math.h>
#include <iostream>

#include <sys/sem.h>
#include <vector>

#include "common/navi_point.h"
#include "common/path.h"

//#include "ecu.h"

#include "common/point.h"
#include "common/rect.h"
//#include "origin_vehicle.h"

#define PATCHS_WIDTH  20            ///<建立网格，横向20个格子
#define PATCHS_LENGTH  20           ///<建立网格，纵向20个格子
#define PATCHS_WIDTH_PER_M  0.6     ///<建立网格，每个格子的边长，0.6米


/**
 * @class patch
 * @brief 该类表示对地面区域建立的格网。
 */
class patch
{
public:
    ///一个格网的代价值，表示通过地形的代价，越小越好。
    int value;
    ///一个格网里包含多少个点
    int num;
    ///一个格网内所有点高程得的总值
    float total;
    ///一个格网内所有点高程得的平均值
    float average;
    ///一个格网内所有点高程得的方差
    float variance;
    ///一个格网内四个角上的坐标。构成矩阵
    rect rt;
    ///参考轨迹上的投影起点 和终点的位置。
    float s1, s2;
    ///参考轨迹上的偏移宽度的距离
    float l1, l2;
    ///记录了投影到当前格网中的左右的点的信息
    vector<point>  patch_points;

public:
    /**
    * @brief 构造函数
    */
    patch()
    {
        value = 0;
        num = 0;
        total = 0;
        average = 0;
        variance = 0;
        s1 = s2 = 0;
        l1 = l2 = 0;
        patch_points.clear();
    }

    /**
    * @brief 析构函数
    */
    ~patch()
    {

    }

    /**
    * @brief 判断一个点是否落在该网格内。
    * @param p 输入量：点的信息。
    * @return 1表示点落在该网格内，0表示没有。。
    */
    bool point_in_patch(point p);
};

/**
 * @class ObstacleInfo
 * @brief 传感器输出的一个障碍物信息。
 */
class ObstacleInfo
{
public:
      /**
    * @brief 构造函数
    */
    ObstacleInfo()
    {
        type_ = 0;
        id_ = 0;
        x_ = 0.0;
        y_ = 0.0;
        width_ = 0.0;
        height_ = 0.0;
        yaw_ = 0.0;
        speed_ = 0.0;
        gps_time_ = 0.0;
    }

    /**
    * @brief 析构函数
    */
    ~ObstacleInfo()
    {

    }
public:
    int32_t    type_;           ///<障碍物类型

    std::string source_;        ///<障碍物来源

    int32_t    id_;             ///<障碍物id

    double     x_;              ///<障碍物x坐标

    double     y_;              ///<障碍物y坐标

    double     width_;          ///<障碍物宽度

    double     height_;         ///<障碍物长度

    double     yaw_;            ///<障碍物运动方向

    double     speed_;          ///<障碍物运动速度

    double     gps_time_;       ///<GPS时间

};

/**
 * @class SensorObstacles
 * @brief 传感器输出的所有障碍物。
 */
class SensorObstacles
{
public:
    /**
    * @brief 构造函数
    */
    SensorObstacles()
    {
        num_of_obstacle_ =0;
        obstacles_info_.clear();
    }

    /**
    * @brief 析构函数
    */
    ~SensorObstacles()
    {

    }
public:
    int16_t    num_of_obstacle_;                        ///<障碍物个数

    std::vector< ObstacleInfo > obstacles_info_;       ///<障碍物列表

};

/**
 * @class ObstacleRect
 * @brief 障碍物类，表示一个障碍物的各种属性。
 */
class ObstacleRect
{
public:
    int value_;      ///<与该障碍物碰撞的碰撞代价。
    string source_;  ///<障碍物信息来源，lidar,radar,camera...
    rect rt_;        ///<障碍物包围盒，四个角上的坐标。
    float v_;        ///<障碍物运动速度
    float yaw_;      ///<障碍物运动方向
    int type_;      ///<障碍物类型。

    vector<navi_point>  pos_seq_;    ///<预测障碍物中心点未来运动的轨迹序列
    vector<rect>  rt_seq_;           ///<预测障碍物包围盒未来运动的轨迹序列

    /**
    * @brief 构造函数
    */
    ObstacleRect()
    {
        value_ =0;
        v_=0;
        yaw_=0;
    }

    /**
    * @brief 析构函数
    */
    ~ObstacleRect()
    {
        //  pos_seq.clear();
        //   rt_seq.clear();
    }

    /**
    * @brief 对等于号=进行重载，。ObstacleRect类的等号操作
    * @param src 输入量：原始障碍物信息。
    * @return 被赋值的障碍物信息。。
    */
    ObstacleRect& operator= (const ObstacleRect& src)
    {
        this->value_ = src.value_;
        this->v_ = src.v_;
        this->yaw_ = src.yaw_;
        this->rt_ = src.rt_;
        this->type_ = src.type_;

        return *this;
    }

};


/**
 * @class RoadSurface
 * @brief 路面情况类，路的参考中心线。路面格网，路上的障碍物等信息。。
 */
class RoadSurface
{
public:
    ///当前GPS时间
    double gps_time_;
    ///参考中心线。
    path road_path_;
    ///路面格网。静态障碍物信息会投影到格网中。
    patch patchs_[PATCHS_WIDTH][PATCHS_LENGTH];
    ///障碍物信息，包括动态障碍物和静态障碍物。
    vector<ObstacleRect> obstacles_;
public:
    /**
    * @brief 构造函数
    */
    RoadSurface();
    /**
    * @brief 析构函数
    */
    ~RoadSurface();

    /**
    * @brief 初始化网格，在参考路径上的起始点和终止点直接生成网格。
    * @param start 输入量：参考路径上的起始点。
    * @param end 输入量：参考路径上的终止点。。
    */
    void init(int start, int end);

    /**
    * @brief 给道路中心线road_path赋值。
    * @param ref_ 输入量：要输入的参考中心线。
    */
    void set_ref_path(vector<navi_point> ref_);
};

/**
 * @class CollisionCheck
 * @brief 碰撞检测类，判断本车是否与障碍物有碰撞的可能。
 */
class CollisionCheck
{
private:
    double car_width_;     ///<本车长度
    double car_length_;    ///<本车宽度
    double safe_width_;    ///<本车与障碍物之间的安全保护距离

public:
    /**
    * @brief 构造函数
    */
    CollisionCheck()
    {
        car_width_ = 0.0;
        car_length_ = 0.0;
        safe_width_ = 0.0;
    }
    /**
    * @brief 析构函数
    */
    ~CollisionCheck()
    {

    }

    /**
    * @brief 设置车辆等信息
    * @param m_car_width 输入量：车辆宽度。
    * @param m_car_length 输入量：车辆长度。
    * @param m_safe_width 输入量：本车与障碍物之间的安全保护距离。
    */
    int set_car_data( double m_car_width, double m_car_length, double m_safe_width );

    /**
    * @brief 计算车辆的外形和矩形位置。考虑安全保护距离。
    * @param rt_car 输出量：车辆矩形框。
    * @param x 输入量：车辆x坐标。
    * @param y 输入量：车辆y坐标。
    * @param heading 输入量：车辆航向角。
    */
    void car_trajectory_collision_check_pos_rect(
        rect& rt_car,
        double x,
        double y,
        double heading);

    /**
    * @brief 计算车辆的外形和矩形位置。考虑安全保护距离。
    * @param rt_car 输出量：车辆矩形框。
    * @param x 输入量：车辆x坐标。
    * @param y 输入量：车辆y坐标。
    * @param heading 输入量：车辆航向角。
    */
    void car_lane_collision_check_rect( rect& rt_car, double x, double y, double heading );

    /**
    * @brief 计算车辆的外形和矩形位置。考虑安全保护距离。
    * @param rt_car 输出量：车辆矩形框。
    * @param x 输入量：车辆x坐标。
    * @param y 输入量：车辆y坐标。
    * @param heading 输入量：车辆航向角。
    * @param safe_width 输入量：安全保护距离。
    */
    void car_lane_collision_check_rect( rect& rt_car, double x, double y, double heading, double safe_width );

    /**
    * @brief 构建一个矩形框。
    * @param x 输入量：矩形框中心x坐标。
    * @param y 输入量：矩形框中心y坐标。
    * @param heading 输入量：矩形框方向。
    * @param width 输入量：矩形框宽度。
    * @param length 输入量：矩形框长度。
    * @param rt_ 输出量：矩形框。
    */
    void construct_rect( double x, double y, double heading, double width, double length, rect& rt );

    /**
    * @brief 判断规划产生的虚拟轨迹与障碍物是否有碰撞。
    * @param lane 输入量：规划产生的虚拟轨迹。
    * @param obstacles 输入量：路面障碍物信息。
    * @param car_speed 输入量：本车速度。
    * @param net_speed 输入量：地图限速。
    * @param st_pos 输入量：车当前位置在规划轨迹上的投影点。
    * @param check_length 输入量：车前方的想要做碰撞检测的长度。
    * @param free_length 输出量：如果有碰撞，输出碰撞距离，否则输出-1。
    * @param motion_speed 输出量：根据前车速度、与前车距离以及地图限速计算的规划推荐速度。
    * @param motion_length 输出量：根据前车速度、与前车距离以及地图限速计算的多少米后达到规划推荐速度。
    * @return 返回1表示有碰撞，0表示没有。
    */
    int virtual_lane_col_check( path& lane, vector<ObstacleRect> obstacles, double car_speed, double net_speed, int st_pos, double check_length,
                                double& free_length, double& motion_speed, double& motion_length );

    /**
    * @brief 判断某个车道与障碍物是否有碰撞。
    * @param lane 输入量：车道中心线。
    * @param obstacles 输入量：路面障碍物信息。
    * @param car_speed 输入量：本车速度。
    * @param st_pos 输入量：车当前位置在规划轨迹上的投影点。
    * @param check_length 输入量：车前方的想要做碰撞检测的长度。
    * @return 返回碰撞距离。
    */
    double lane_col_check( path& lane, vector<ObstacleRect> obstacles, double car_speed, int st_pos, double check_length );

    /**
    * @brief 判断某个车道与障碍物是否有碰撞。
    * @param lane 输入量：车道中心线。
    * @param obstacles 输入量：路面障碍物信息。
    * @param start_num 输入量：碰撞检测的起始位置，由地图匹配获得。
    * @param wheel_base_length 输入量：车轴长。
    * @param step 输入量：搜索步长。
    * @param safe_dis 输入量：安全保护距离长。
    * @return 返回1表示有碰撞，0表示没有。
    */
    int park_col_check( path& lane, vector<ObstacleRect> obstacles, int start_num, double wheel_base_length, int step, double safe_dis );

#if 0
    /**
    * @brief 判断两个点在时间维度上是否要交叠。
    * @param p 输入量：第一个点。
    * @param op 输入量：第二个点。
    * @return 返回true表示有交叠，false表示没有。。
    */
    bool cross_in_two_time(navi_point& p, navi_point& op);

    /**
    * @brief 判断两个点在时间维度上是否要交叠。
    * @param p 输入量：第一个点。
    * @param op 输入量：第二个点。
    * @return 返回true表示有交叠，false表示没有。。
    */
    bool cross_in_two_time_seq( navi_point& p, obstacle& obj, rect& rt_car );

    bool lane_collision_check(path& lane,
                              RoadSurface& road,
                              double speed,
                              int st_pos,
                              int en_pos,
                              double check_length,
                              int& free_length_num,
                              double& free_length,
                              double& obj_speed,
                              double& obj_heading);

    bool lane_collision_check_moving_objects(
        path& lane,
        RoadSurface& road,
        int st_pos,
        int en_pos,
        double check_length,
        int& free_length_num,
        double& free_length);
#endif // 0
};


#endif // _COLLISION_CHECK_H
