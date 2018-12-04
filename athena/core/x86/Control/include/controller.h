/**
 * @file    controller.h
 * @author  jiang <jiangchengjie@indrv.cn>
 * @date    2018-07-07
 * @version 1.0.0
 * @par     Copyright(c)
 *          hy
 */

#ifndef COMMON_CONTROLLER_H_
#define COMMON_CONTROLLER_H_

#include <iostream>
#include <vector>
#include "chassis.h"
#include "controller_agent.h"
#include "controller_config.h"
#include "localization.h"
#include "trajectory.h"
#include "controller_output.h"
#include "debug_output.h"

using namespace std;
 /**
 * @namespace athena::control
 * @brief athena::control
 */
namespace athena{
namespace control{

 /**
 * @class controller
 * @brief controller base.
 */
class Controller{
 public:
  Controller()  = default;
  ~Controller() = default;


 /**
 * @brief initialization.
 * @param[in] controller_config controller config.
 * @return true or false.
 */
  bool Init(const ControllerConfig  controller_config);

/**
 * @brief SetTrajectory.
 * @param[in] trajectory trajectory information.
 * @return void.
 */
  void SetTrajectory(const Trajectory *trajectory);

/**
 * @brief ComputeControlOutput.
 * @param[in] localiation Location information.
 * @param[in] controller_output  controller output.
 * @return true or false.
 */
  bool ComputeControlOutput(const Localization * localiation,const Chassis *chassis,ControllerOutput * controller_output);


/**
 * @brief GetControllerInfo.
 * @param[in] debug_output 调试输出.
 * @return void.
 */
  void GetControllerInfo(DebugOutput &debug_output);

/**
 * @brief SetDrivingModeDebug 设置驾驶模式用作调试.
 * @param[in] mode 0 无效 1 人工驾驶 3 自动驾驶.
 * @return void.
 */
  void SetDrivingModeDebug(int32_t mode);

/**
 * @brief SetTarSpeedDebug 设置推荐速度.
 * @param[in] tar_speed 推荐速度.
 * @param[in] valid 是否有效.
 * @return void.
 */
  void SetTarSpeedDebug(int32_t tar_speed,bool valid);

/**
 * @brief GetAlarmInfo 获取报警信息.
 * @param[out] alarm_list 报警信息.
 * @return void.
 */
  void GetAlarmTableInfo(std::vector<ControllerOutputAlarm::AlarmInfoTable>  *alarm_list);
 private:
  ///控制器代理
  ControllerAgent controller_agent_;
};
}
}

#endif // COMMON_CONTROLLER_H_
