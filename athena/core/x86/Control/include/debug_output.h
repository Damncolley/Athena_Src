/**
 * @file    debug_output.h
 * @author  jiang <jiangchengjie@indrv.cn>
 * @date    2018-07-07
 * @version 1.0.0
 * @par     Copyright(c)
 *          hy
 */

#ifndef COMMON_DEBUG_OUTPUT_H_
#define COMMON_DEBUG_OUTPUT_H_

#include "common/path.h"
#include "local_localization.h"

 /**
 * @namespace athena::control
 * @brief athena::control
 */
namespace athena{
namespace control{

 /**
 * @class controller
 * @brief controller base.
 */
class DebugOutput{
public:
    DebugOutput() = default;
    ~DebugOutput() = default;
    double tar_speed_;
    int32_t match_point_num_;
    path path_;
    LocalLocalization local_localization_;
};
}
}
#endif // COMMON_DEBUG_OUTPUT_H_
