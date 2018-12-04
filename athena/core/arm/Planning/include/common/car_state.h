#pragma once
#include <string>

/**
 * @class CarPose
 * @brief 车辆位姿。
 */
class CarPose
{
public:
     /**
    * @brief 构造函数
    */
    CarPose()
    {
        CurrentX_ = 0.0;
        CurrentY_ = 0.0;
        CurrentZ_ = 0.0;
        Current_heading_ = 0.0;
        Current_pitch = 0.0;
        Current_roll_ = 0.0;
    }
    /**
    * @brief 析构函数
    */
    virtual ~CarPose()
    {

    }

    double CurrentX_, CurrentY_, CurrentZ_;                           ///<车在空间坐标系下的坐标
    double Current_heading_, Current_pitch, Current_roll_;                ///<车当姿态角

};

/**
 * @class CarState
 * @brief 车辆状态。
 */
class CarState
{
public:
    /**
    * @brief 构造函数
    */
    CarState()
    {
        CurrentS_ = 0.0;
        car_speed_ = 0.0;
        steer_angle_ = 0.0;
        at_status_ = 0;
    }
    /**
    * @brief 析构函数
    */
    virtual ~CarState()
    {

    }

public:
    CarPose car_pose_;                      ///车辆位姿
    double CurrentS_;                       ///<车当前里程
    double car_speed_;                      ///<车速,从车身can或者惯导信息里获得
    double steer_angle_;                    ///<方向盘转角
    int at_status_;                         ///<档位信息
    double GPS_time_;                       ///<GPS时间

};



