/**
 * @file    generic_controller.h
 * @author  jiang <jiangchengjie@indrv.cn>
 * @date    2018-07-07
 * @version 1.0.0
 * @par     Copyright(c)
 *          hy
 */

/**
 * @file
 * @brief Defines the GenericController class.
 */
#ifndef GENERIC_CONTROLLER_H_
#define GENERIC_CONTROLLER_H_

#include "common/path.h"
#include "controller_config.h"
#include "controller_output.h"
#include "localization.h"
#include "local_localization.h"
#include "chassis.h"
#include "debug_output.h"


/**
 * @namespace athena::control
 * @brief athena::control
 */
namespace athena{
namespace control{

/**
 * @class LonController
 *
 * @brief Longitudinal controller, to compute brake and driving force values.
 */
class GenericController{
 public:

 /**
  * @brief constructor
  */
 GenericController() = default;

 /**
  * @brief destructor
  */
 ~GenericController() = default;

 /**
 * @brief init.
 * @param[in] controller_config controller config.
 * @return true or false.
 */
 virtual bool Init(const ControllerConfig  controller_config) = 0;

 /**
 * @brief ComputeControlOutput.
 * @param[in] path 轨迹.
 * @param[in] match_point_no 轨迹匹配索引.
 * @param[in] localiation 定位信息.
 * @param[in] chassis 车辆底盘信息.
 * @param[in] ControllerOutput  控制器输出.
 * @return true or false.
 */
 virtual bool ComputeControlOutput(path * path,uint32_t match_point_no,const LocalLocalization * local_localiation,const Chassis *chassis,ControllerOutput * controller_output) = 0;

 /**
 * @brief 获取调试信息.
 * @param[in] debug_output 调试输出.
 * @return void.
 */
 virtual void GetControllerDebugInfo(DebugOutput &debug_output) = 0;

 /**
 * @brief SetTarSpeedDebug 设置推荐速度.
 * @param[in] tar_speed 推荐速度.
 * @param[in] valid 是否有效.
 * @return void.
 */
  virtual void SetTarSpeedDebug(int32_t tar_speed,bool valid) = 0;
};
}//namespace control
}//namespace athena
#endif //GENERIC_CONTROLLER_H_
