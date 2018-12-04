/**
 * @file    message_manger.h
 * @author  jiang <jiangchengjie@indrv.cn>
 * @date    2018-07-07
 * @version 1.0.0
 * @par     Copyright(c)
 *          hy
 */

#ifndef APPS_MESSAGE_MANGER_MESSAGE_MANGER_H_
#define APPS_MESSAGE_MANGER_MESSAGE_MANGER_H_

#include "chassis.h"
#include "trajectory.h"
#include "localization.h"
#include "../../common/chassis_detail.h"
#include "../../common/control_cmd.h"
#include "../../common/control_info_report.h"
#include "../../common/emergency.h"
#include "../../control_logic/control_logic.h"

 /**
 * @namespace athena::control
 * @brief athena::control
 */
namespace athena{
namespace control{

 /**
 * @class MessageManger
 * @brief 消息管理器.
 */
class MessageManger{
 public:
  MessageManger()  = default;
  ~MessageManger() = default;

  /**
  * @brief     初始化。
  * @param[in] obu_url LCM组播信息.
  * @param[in] control_logic 控制逻辑.
  * @return    void.
  */
  virtual void Init(string obu_url,ControlLogic *control_logic) = 0;


  /**
   * @brief     控制消息上报.
   * @param[in] control_info_report 控制信息.
   * @return    void.
   */
  virtual void PublishControlInfoReport(ControlInfoReport control_info_report) = 0;

 /**
  * @brief     控制消息发布.
  * @param[in] controller_output 控制输出.
  * @return    void.
  */
  virtual void PublishControlCmd(ControlCmd control_cmd) = 0;

 /**
  * @brief     BCM控制信息发送.
  * @param[in] bcm_control_cmd BCM控制信息.
  * @return    void.
  */
  virtual void PublishBcmControlCmd(BcmControlCmd bcm_control_cmd) = 0;

 /**
  * @brief     紧急事件消息发送.
  * @param[in] emergency 紧急事件.
  * @return    void.
  */
  virtual void PublishEmergencyCmd(Emergency emergency) = 0;
};
}
}

#endif // APPS_MESSAGE_MANGER_MESSAGE_MANGER_H_
