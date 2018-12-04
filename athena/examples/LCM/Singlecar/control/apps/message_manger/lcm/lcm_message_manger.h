/**
 * @file    lcm_message_manger.h
 * @author  jiang <jiangchengjie@indrv.cn>
 * @date    2018-07-07
 * @version 1.0.0
 * @par     Copyright(c)
 *          hy
 */

#ifndef APPS_MESSAGE_MANGER_LCM_LCM_MESSAGE_MANGER_H_
#define APPS_MESSAGE_MANGER_LCM_LCM_MESSAGE_MANGER_H_

#include "../message_manger.h"
#include <lcm/lcm.h>
#include <lcm/lcm-cpp.hpp>
#include "msgs/obu_lcm/mt_info_report.hpp"
#include "msgs/obu_lcm/ins_info.hpp"
#include "msgs/obu_lcm/vehicle_info.hpp"
#include "msgs/obu_lcm/control_cmd.hpp"
#include "msgs/obu_lcm/mt_bcm_control_cmd.hpp"
#include "msgs/obu_lcm/control_info_report.hpp"
#include "msgs/obu_lcm/emergency.hpp"
#include "msgs/obu_lcm/bcm_control_cmd.hpp"
#include "../../../common/Thread.h"

 /**
 * @namespace athena::control
 * @brief athena::control
 */

namespace athena{
namespace control{

 /**
 * @class LcmMessageManger
 * @brief LCM消息管理器.
 */
class LcmMessageManger:public MessageManger,public Thread{
 public:
  LcmMessageManger()  = default;
  ~LcmMessageManger() = default;

  /**
  * @brief     初始化。
  * @param[in] obu_url LCM组播信息.
  * @return    void.
  */
  void Init(string obu_url,ControlLogic *control_logic);

 /**
  * @brief     控制消息发布.
  * @param[in] control_cmd 控制命令输出.
  * @return    void.
  */
  void PublishControlCmd(ControlCmd control_cmd);

 /**
  * @brief     控制消息上报.
  * @param[in] control_info_report 控制信息.
  * @return    void.
  */
  void PublishControlInfoReport(ControlInfoReport control_info_report);

 /**
  * @brief     BCM控制信息发送.
  * @param[in] bcm_control_cmd BCM控制信息.
  * @return    void.
  */
  void PublishBcmControlCmd(BcmControlCmd bcm_control_cmd);

 /**
  * @brief     紧急事件消息发送.
  * @param[in] emergency 紧急事件.
  * @return    void.
  */
  void PublishEmergencyCmd(Emergency emergency);

 private:
    ControlLogic *control_logic_;
    lcm::LCM* lcm_;

    void HandleLocalizationMessage(
        const lcm::ReceiveBuffer* rbuf,
        const std::string& chan,
        const obu_lcm::ins_info * msg);

    void HandleChassisDetailMessage(
        const lcm::ReceiveBuffer* rbuf,
        const std::string& chan,
        const obu_lcm::vehicle_info* msg);


    void HandleTrajectoryMessage(
        const lcm::ReceiveBuffer* rbuf,
        const std::string& chan,
        const obu_lcm::mt_info_report* msg);

    void HandleMtBcmControlMessage(
        const lcm::ReceiveBuffer* rbuf,
        const std::string& chan,
        const obu_lcm::mt_bcm_control_cmd* msg);

   /**
    * @brief     线程运行函数.
    * @return    void.
    */
    void run();
};
}
}

#endif // APPS_MESSAGE_MANGER_LCM_LCM_MESSAGE_MANGER_H_
