
/**
 * @file    control_logic.h
 * @author  jiang <jiangchengjie@indrv.cn>
 * @date    2018-07-07
 * @version 1.0.0
 * @par     Copyright(c)
 *          hy
 */

#ifndef CONTROL_LOGIC_CONTROL_LOGIC_H_
#define CONTROL_LOGIC_CONTROL_LOGIC_H_

#include "chassis.h"
#include "trajectory.h"
#include "localization.h"
#include "controller_output.h"
#include "../common/control_cmd.h"
#include "../common/chassis_detail.h"
#include "../common/emergency.h"
#include "../common/Thread.h"
#include "../common/bcm_control_cmd.h"
#include "../common/enum.h"
#include "../common/logging.h"
#include "../common/get_time.h"
#include "../common/logging.h"
#include "../common/control_info_report.h"
#include "controller.h"
//#include "../apps/control_view/control_view.h"
#include "control_logic_config.h"
#include "gear/gear_control.h"

#include "acc/cs55/cs55_torque_speed_throttle_map.h"
#include "brake/cs55/cs55_deceleration_brake_map.h"
#include "gear/cs55/cs55_gear_control.h"
#include "control_logic_debug_output.h"

#include "acc/TRUCK_J6P/truck_j6p_torque_speed_throttle_map.h"
#include "brake/TRUCK_J6P/truck_j6p_deceleration_brake_map.h"
#include "gear/TRUCK_J6P/truck_j6p_gear_control.h"

 /**
 * @namespace athena::control
 * @brief athena::control
 */
namespace athena{
namespace control{
 /**
 * @class ControlLogic
 * @brief 控制逻辑类.
 */
class ControlLogic:public Thread{
 public:
  ControlLogic()  = default;
  ~ControlLogic() = default;

  void Init(ControlLogicConfig control_logic_config);

  /**
  * @brief     轨迹消息接收.
  * @param[in] trajectory 规划轨迹.
  * @return    void.
  */
  void 	SubscribeTrajectory(Trajectory trajectory);

 /**
  * @brief     定位信息接收.
  * @param[in] localization 定位信息.
  * @return    void.
  */
  void 	SubscribeLocalization(Localization localization);

/**
  * @brief     底盘信息接收.
  * @param[in] chassis 车辆底盘信息.
  * @return    void.
  */
  void 	SubscribeChassis(ChassisDetail chassis_detail);

  /**
  * @brief     BCM信息接收.
  * @param[in] chassis 车辆底盘信息.
  * @return    void.
  */
  void 	SubscribeBcmControl(BcmControlCmd bcm_control_cmd);

 /**
  * @brief     获取控制命令.
  * @param[in] control_cmd 控制命令.
  * @return    void.
  */
  void GetControlCmd(ControlCmd &control_cmd);

 /**
  * @brief     获取BCM控制命令.
  * @param[in] bcm_control_cmd BCM控制命令.
  * @return    void.
  */
  void GetBcmControlCmd(BcmControlCmd &bcm_control_cmd);

 /**
  * @brief     获取控制信息.
  * @param[in] control_info_report 控制信息.
  * @return    void.
  */
  void GetControlInfoReport(ControlInfoReport &control_info_report);

 /**
  * @brief     获取控制器报警信息.
  * @param[in] emergency 紧急事件信息.
  * @return    true=紧急事件发横 false=无紧急事件发生.
  */
  bool GetControllerAlarmInfo(Emergency *emergency);

 /**
  * @brief     获取调试信息.
  * @param[in] control_logic_debug_output.
  * @return    void.
  */
  void GetControlLogicDebugOutput(ControlLogicDebugOutput &control_logic_debug_output);

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
  * @brief GetSteeringAngleFeedback 获取车辆实际转向角.
  * @param[in] void.
  * @return 转向角.
  */
  double GetSteeringAngleFeedback();

  ControllerConfig controller_config_;
  ControlLogicConfig control_logic_config_;
 private:
    ///控制器
    Controller controller_;
    ///轨迹
    Trajectory trajectory_;
    ///定位
    Localization localization_;
    ///控制命令
    ControlCmd control_cmd_;
    ///控制器输出
    ControllerOutput controller_output_;
    ///底盘信息
    Chassis chassis_;
    ///底盘详细信息
    ChassisDetail chassis_detail_;
    ///bcm控制信息
    BcmControlCmd bcm_control_cmd_;
    ///驾驶模式
    int32_t driving_mode_;
    ///调试输出
    ControlLogicDebugOutput control_logic_debug_output_;

    /**
    * @brief     线程运行函数.
    * @return    void.
    */
    void run();

   /**
    * @brief     计算控制输出.
    * @return    void.
    */
    void ComputeControlOutputOnTimer();

    ///CS55档位控制器
    CS55GearControl cs55_gear_control_;
    ///J6P档位控制器
    TruckJ6pGearControl truck_j6p_gear_control_;
    ///档位控制
    GearControl *gear_control_;

   /**
    * @brief     注册档位控制.
    * @param[in] gear_control 档位控制.
    * @return    void.
    */
    void RegisterGearControl(GearControl *gear_control);

   /**
    * @brief     获取驾驶模式.
    * @return    驾驶模式参考DrivingMode.
    */
    int32_t GetDrivingMode();

   /**
    * @brief     设置驾驶模式.
    * @param[in] driving_mode 驾驶模式.
    * @return    void.
    */
    void SetDrivingMode(int driving_mode);

   /**
    * @brief     获取驾驶模式.
    * @return    DrivingMode 驾驶模式参考DrivingMode.
    */
    void  SetControlCmd(ControlCmd *control_cmd,ControllerOutput controller_output);

};
}
}

#endif // CONTROL_LOGIC_CONTROL_LOGIC_H_
