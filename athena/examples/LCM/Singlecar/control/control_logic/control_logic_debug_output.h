/**
 * @file    control_logic_debug_output.h
 * @author  jiang <jiangchengjie@indrv.cn>
 * @date    2018-07-07
 * @version 1.0.0
 * @par     Copyright(c)
 *          hy
 */

#ifndef CONTROL_LOGIC_CONTROL_LOGIC_DEBUG_OUTPUT_H_
#define CONTROL_LOGIC_CONTROL_LOGIC_DEBUG_OUTPUT_H_

 /**
 * @namespace athena::control
 * @brief     athena::control
 */
namespace athena{
namespace control{

/**
 * @class ControlLogicDebugOutput
 *
 * @brief ControlLogicDebugOutput.
 */
class ControlLogicDebugOutput{
public:
    ControlLogicDebugOutput() = default;
    ~ControlLogicDebugOutput() = default;

    double lon_controller_tar_speed_;
    ChassisDetail chassis_detail_output_;
    Localization localization_output_;
};
}//namespace control
}//namespace athena
#endif // CONTROL_LOGIC_CONTROL_LOGIC_DEBUG_OUTPUT_H_
