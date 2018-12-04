/**
 * @file    enum.h
 * @author  jiang <jiangchengjie@indrv.cn>
 * @date    2018-07-07
 * @version 1.0.0
 * @par     Copyright(c)
 *          hy
 */

#ifndef APPS_ENUM_H_
#define APPS_ENUM_H_

 /**
 * @namespace athena::control
 * @brief athena::control
 */
namespace athena{
namespace control{

typedef enum{
    LCM = 1,  /**< LCM消息*/
}MessageType;

typedef enum{
    CS55 = 1,  /**< LCM消息*/
    TRUCK_J6P = 2,
}VehicleType;

typedef enum{
    INVALID = 0,             /**< 无效*/
    HUMAN_DRIVING_MODE = 1,  /**< 人工驾驶*/
    AUTO_DRIVING_MODE = 3,   /**< 自动驾驶*/
}DrivingMode;

typedef enum{
    CONTROLLABLE         = 1,   /**< 可控但处于非控制状态*/
    CONTROLLED           = 2,   /**< 正在控制中*/
    UNCONTROLLABLE      = 3,   /**< 不可控*/
}DrivingModeFeedBack;

typedef enum{
    POSITION_P = 0,          /**< P档位*/
    POSITION_R = 1,          /**< R档位*/
    POSITION_N = 2,          /**< N档位*/
    POSITION_D = 3,          /**< D档位*/
}GearLevel;

typedef enum{
    POSITION_P_FEEDBACK         = 0,  /**< P档位反馈*/
    POSITION_R_FEEDBACK         = 1,  /**< R档位反馈*/
    POSITION_N_FEEDBACK         = 2,  /**< N档位反馈*/
    POSITION_D_FEEDBACK         = 3,  /**< D档位反馈*/
    POSITION_SHIFTING_FEEDBACK  = 4,  /**< 档位切换过程中*/
    POSITION_INVALID_FEEDBACK   = 5,  /**< 无效档位*/
}GearLevelFeedBack;

}//namespace control
}//namespace athena

#endif //APPS_ENUM_H_
