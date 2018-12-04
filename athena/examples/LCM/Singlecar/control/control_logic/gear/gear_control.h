/**
 * @file    gear_control.h
 * @author  jiang <jiangchengjie@indrv.cn>
 * @date    2018-07-07
 * @version 1.0.0
 * @par     Copyright(c)
 *          hy
 */

#ifndef CONTROL_LOGIC_GEAR_GEAR_CONTROL_H_
#define CONTROL_LOGIC_GEAR_GEAR_CONTROL_H_

#include "../../common/enum.h"

 /**
 * @namespace athena::control
 * @brief athena::control
 */
namespace athena{
namespace control{

 /**
 * @class Control
 * @brief 控制类.
 */
class GearControl{
 public:
  GearControl()  = default;
  ~GearControl() = default;

  virtual GearLevel GetGearLevel() = 0;
};
}
}
#endif //CONTROL_LOGIC_GEAR_GEAR_CONTROL_H_
