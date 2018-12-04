/**
 * @file    gear_position.h
 * @author  jiang <jiangchengjie@indrv.cn>
 * @date    2018-07-07
 * @version 1.0.0
 * @par     Copyright(c)
 *          hy
 */

#ifndef GEAR_POSITION_H_
#define GEAR_POSITION_H_


 /**
 * @namespace athena::control
 * @brief athena::control
 */
namespace athena{
namespace control{


/**
 * @class ControllerOutputAlarm
 *
 * @brief 控制警报输出.
 */
enum  GearPosition:int32_t
{
     AT_P = 0,    /**< P档位*/
     AT_R = 1,    /**< R档位*/
     AT_N = 2,    /**< N档位*/
     AT_D = 3,    /**< D档位*/
};
}
}

#endif // GEAR_POSITION_H_
