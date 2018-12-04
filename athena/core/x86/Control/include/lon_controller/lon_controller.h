/**
 * @file controller.h
 * @author jiang <jiangchengjie@indrv.cn>
 * @date  2018-07-07
 * @version 1.0.0
 * @par  Copyright(c)
 *        hy
 */

/**
 * @file
 * @brief Defines the LonController class.
 */
#ifndef CONTROLLER_LON_CONTROLLER_LONCONTROLLER_H_
#define CONTROLLER_LON_CONTROLLER_LONCONTROLLER_H_

#include "vehicle_dynamics.h"
#include "../generic_controller.h"
#include "../gear_position.h"
#include "../common/map_matching/map_matching.h"


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
class LonController:public GenericController{
 public:

 /**
  * @brief constructor
  */
 LonController() = default;

 /**
  * @brief destructor
  */
 ~LonController() = default;

 /**
 * @brief init.
 * @param[in] controller_config controller config.
 * @return true or false.
 */
 bool Init(const ControllerConfig  controller_config);

 /**
 * @brief ComputeControlOutput.
 * @param[in] path 轨迹.
 * @param[in] match_point_no 轨迹匹配索引.
 * @param[in] local_localiation 地图定位信息.
 * @param[in] chassis 车辆底盘信息.
 * @param[in] ControllerOutput  控制器输出.
 * @return true or false.
 */
 bool ComputeControlOutput(path * path,uint32_t match_point_no,const LocalLocalization * local_localiation,const Chassis *chassis,ControllerOutput * controller_output);


  /**
 * @brief 获取调试信息.
 * @param[in] debug_output 调试输出.
 * @return void.
 */
 void GetControllerDebugInfo(DebugOutput &debug_output);


/**
 * @brief SetTarSpeedDebug 设置推荐速度.
 * @param[in] tar_speed 推荐速度.
 * @param[in] valid 是否有效.
 * @return void.
 */
  void SetTarSpeedDebug(int32_t tar_speed,bool valid);

 private:
 ///车辆动力学
 VehicleDynamics vehicle_dynamics_;
 ///车辆动力学输入
 VehicleDynamicsInput vehicle_dynamics_input_;
  ///纵向控制车辆动力学输入
 //VehicleDynamicsInput vehicle_dynamics_input;
 ///基本配置
 ControllerConfig controller_config_;
 ///预描距离
 double pre_draw_distance;
 ///纵向状态
 int32_t lon_status;
 ///目标加速度值
 double tar_acceleration;

 ///纵向速度调试
 bool tar_speed_debug_valid_;
 double tar_speed_debug;

 typedef enum{
     ACCELERATE,/**< 加速*/
     HOLD      ,/**< 保持*/
     BRAKE     ,/**< 减速*/
 }LonStatus;

/**
 * @brief AcceleratorSwitch 油门和刹车切换策略.
 * @param[in] pre_draw_end_speed 预描点处的速度.
 * @return void
 */
 void AcceleratorSwitch(double pre_draw_end_speed);

 /**
 * @brief ComputeEpbOutput 计算EPB控制输出.
 * @param[in] current_speed 当前速度.
 * @param[in] current_gear_pos 当前档杆位置.
 * @return true:EPB使能  false：EPB禁能
 */
 bool ComputeEpbOutput(double current_speed,int32_t current_gear_pos);
};
}//namespace control
}//namespace athena
#endif //CONTROLLER_LATERAL_LAT_CONTROLLER_H
