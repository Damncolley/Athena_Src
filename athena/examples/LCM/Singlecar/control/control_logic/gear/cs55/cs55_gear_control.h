/**
 * @file    cs55_gear_control.h
 * @author  jiang <jiangchengjie@indrv.cn>
 * @date    2018-07-07
 * @version 1.0.0
 * @par     Copyright(c)
 *          hy
 */

#ifndef CONTROL_LOGIC_GEAR_CS55_CS55_GEAR_CONTROL_H_
#define CONTROL_LOGIC_GEAR_CS55_CS55_GEAR_CONTROL_H_

#include "../gear_control.h"

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
class CS55GearControl:public GearControl{
 public:
  CS55GearControl()  = default;
  ~CS55GearControl() = default;

  GearLevel GetGearLevel();
};
}
}
#endif //CONTROL_LOGIC_GEAR_CS55_CS55_GEAR_CONTROL_H_
