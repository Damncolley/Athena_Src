/**
 * @file    track_trajectory.h
 * @author  jiang <jiangchengjie@indrv.cn>
 * @date    2018-07-07
 * @version 1.0.0
 * @par     Copyright(c)
 *          hy
 */

#ifndef APPS_TRACK_TRAJECTORY_H_
#define APPS_TRACK_TRAJECTORY_H_

#include "common/path.h"
#include "common/math_util.h"
#include "common/map_matching/spline.h"
#include "common/map_matching/heading.h"
#include "common/map_matching//steering_angle.h"
#include "../../control_logic/control_logic.h"


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
class TrackTrajectory{
 public:
  TrackTrajectory()  = default;
  ~TrackTrajectory() = default;

 /**
  * @brief     初始化
  * @param[in] local_trajectory_path 本地轨迹路径..
  * @param[in] controller_config 控制器配置.
  * @return    void.
  */
  void Init(string local_trajectory_path,ControlLogic *control_logic);

 private:
    const double const_speed_ = 8.0;
    const double steering_cau_one_point_ = 2.0;
    const double steering_cau_two_point_ = 4.0;
    const int32_t mark_every_ = 10;
    const double spline_every_ = 0.1;
    const double kp_slope_ = 30.0;
    const double kp_value_ = 2.0;
    const double tar_speed_ = 0;
    path recv_path_;
    ///轨迹
    Trajectory trajectory_;
    ControlLogic *control_logic_;

    void CauAllOutputFromSingleSpline(
        path& p,
        int no_points,
        double speed);


    void CauPathFromSpline(path& p, int no_points);


    void SplineKp(path& p,double speed,int length );

     /**
      * @brief     轨迹转换
      * @param[in] path 本地路径..
      * @param[in] trajectory 轨迹.
      * @return    void.
      */
    void PathToTrajectory(path &path,Trajectory &trajectory);
};
}
}

#endif //APPS_TRACK_TRAJECTORY_H_
