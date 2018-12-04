/**
 * @file    vehicle_dynamics.h
 * @author  jiang <jiangchengjie@indrv.cn>
 * @date    2018-07-07
 * @version 1.0.0
 * @par     Copyright(c)
 *          hy
 */

#ifndef CONTROLLER_LON_CONTROLLER_VEHICLE_DYNAMICS_H_
#define CONTROLLER_LON_CONTROLLER_VEHICLE_DYNAMICS_H_

#include <iostream>
#include <vector>
#include "../pid/pid_controller.h"

using namespace std;


 /**
 * @namespace athena::control
 * @brief athena::control
 */
namespace athena{
namespace control{

/**
 * @class VehicleDynamicsInput
 *
 * @brief VehicleDynamics Input.
 */
class VehicleDynamicsInput{
public:
    ///目标速度
    double tar_speed_;
    ///当前速度
    double current_speed_;
    ///车重
    double m_;
    ///空气阻力系数
    double cd_;
    ///车轮半径
    double wheel_radius_;
    ///车辆高度
    double h_;
    ///俯仰角
    double current_pitch_;
    ///地面摩擦系数
    double kr_;
    ///当前加速度
    double current_accelerate_;
    ///当前转向角度
    double steer_angle_;
    ///后轴到重心的距离
    double l_b_;
    ///轮距
    double l_;
    ///P值
    double kp_;
    ///I值
    double ki_;
    ///D值
    double kd_;
};

/**
 * @class VehicleDynamics
 *
 * @brief VehicleDynamics.
 */
class VehicleDynamics{
 public:
 VehicleDynamics()  = default;
 ~VehicleDynamics() = default;

/**
 * @brief 获取纵向控制车辆动力学输出
 * @param[in] vehicle_dynamics_input_ 输入
 * @return 驱动力值.
 */
 double GetVehicleDynamicsOutput(VehicleDynamicsInput vehicle_dynamics_input);

 private:
  const double g = 9.8;
  const double air_resistance_coefficient_ = 0.3;
  const double rolling_resistance_coefficient_ = 0.015;


 ///纵向控制车辆动力学输入
 VehicleDynamicsInput vehicle_dynamics_input_;
 ///PID控制器
 PIDController pid_controller_;

/**
 * @brief 前馈值计算
 * @return 前馈值.
 */
 double GetFeedforward();

   /**
 * @brief 反馈值计算
 * @return 反馈值.
 */
 double GetFeedback();

/**
 * @brief 计算风阻
 * @param[in] cd_ 空气阻力系数
 * @param[in] front_wheel_base_ 前轮轮距
 * @param[in] h_ 车辆高度
 * @param[in] current_speed_ 当前车速
 * @return 风阻.
 */
 double CalculateWindResistance(double cd,double front_wheel_base,double h_,double current_speed);


/**
 * @brief 计算坡道阻力
 * @param[in] m_ 车重
 * @param[in] current_pitch_ 俯仰角
 * @return 坡阻力.
 */
double CalculateSlopeeResistance(double m,double current_pitch);

/**
 * @brief 计算滚动阻力
 * @param[in] m_ 车重
 * @param[in] kr_ 地面摩擦系数
 * @return 滚动阻力
 */
double CalculateRollResistance(double m,double kr);


/**
 * @brief 计算加速阻力。
 * @param[in] m_ 车重。
 * @param[in] current_accelerate_ 当前加速度。
 * @return 加速阻力.
 */
double CalculateAccResistance(double m,double current_accelerate);

/**
 * @brief 转向阻力
 * @param[in] m_ 车重
 * @param[in] steer_angle_ 转向角
 * @param[in] current_speed_ 当前速度
 * @param[in] l_b 后轴到重心的距离
 * @param[in] l   轴距
 * @return        转向阻力.
 */
//double CalculateTurningResistance(double m,double steer_angle,double current_speed,double l_b,double l);

};
}
}

#endif // CONTROLLER_LON_CONTROLLER_VEHICLE_DYNAMICS_H_
