/**
 * @file simple_lateral_debug.h
 * @author jiang <jiangchengjie@indrv.cn>
 * @date  2018-07-07
 * @version 1.0.0
 * @par  Copyright(c)
 *        hy
 */

#ifndef CONTROLLER_SIMPLE_LATERAL_DEBUG_H_
#define CONTROLLER_SIMPLE_LATERAL_DEBUG_H_

/**
 * @namespace athena::control
 * @brief athena::control
 */
namespace athena{
namespace control{

/**
 * @class SimpleLateralDebug
 *
 * @brief SimpleLateralDebug.
 */
class SimpleLateralDebug{
 public:
  double lateral_error_;
  double ref_heading_;
  double heading_;
  double heading_error_;
  double heading_error_rate_;
  double lateral_error_rate_;
  double curvature_;
  double steer_angle_;
  double steer_angle_feedforward_;
  double steer_angle_lateral_contribution_;
  double steer_angle_lateral_rate_contribution_;
  double steer_angle_heading_contribution_;
  double steer_angle_heading_rate_contribution_;
  double steer_angle_feedback_;
  double steering_position_;
  double ref_speed_;
  double steer_angle_limited_;
};
}
}
#endif
