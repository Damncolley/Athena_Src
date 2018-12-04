#pragma once

#include <vector>


//tips 删除没用到的参数
class navi_point
{

public:
    double position_x_; // 坐标.x
    double position_y_; // 坐标.y
    double position_z_; // 坐标.z

    double lat_;
    double lon_;
    double height_;

    double speed_;
    double lateral_speed_;
    double longitudinal_speed_;
    double down_speed_;

    double roll_;
    double pitch_;
    double heading_;

    double accelerataion_;
    double lateral_accelerate_;
    double longitudinal_accelerate_;
    double down_accelerate_;

    double roll_speed_;
    double pitch_speed_;
    double heading_speed_;

    double steering_angle_;
    double steering_angle_speed_;

    double speed_desired_Uxs_;
    double curvature_;
    double acceleration_desired_Axs_;

public:
    int point_no_;
    double pos_gps_time_;           // get_current_time
    int32_t cycle_tag_;   // 整周计数;整体增加360×cycle_tag

    double s_;
    double r_;
    double lateral_offset_;
    double k_s_;
    double d_lateral_offset_;
    double d2_lateral_offset_;

    double width_; //车道宽度
    int32_t type_;
    int64_t lane_id_;
    int32_t left_lane_num_;
    int32_t right_lane_num_;

    int control_mark_;
    double suggest_kp_;
    int8_t     p_g_;    //该点的档位
    double tar_speed_;
    double     p_h_;    //航向角
    double     p_k_;	//该点曲率
public:


    navi_point()
    {
      position_x_ = 0;
      position_y_ = 0;
      position_z_ = 0;

      lat_ = 0;
      lon_ = 0;
      height_ = 0;

      speed_ = 0;
      lateral_speed_ = 0;
      longitudinal_speed_ = 0;
      down_speed_ = 0;

      roll_ = 0;
      pitch_ = 0;
      heading_ = 0;

      accelerataion_ = 0;
      lateral_accelerate_ = 0;
      longitudinal_accelerate_ = 0;
      down_accelerate_ = 0;

      roll_speed_ = 0;
      pitch_speed_ = 0;
      heading_speed_ = 0;

      steering_angle_ = 0;
      steering_angle_speed_ = 0;

      speed_desired_Uxs_ = 0;
      curvature_ = 0;
      acceleration_desired_Axs_ = 0;

      point_no_ = 0;
      s_ = 0;
      r_ = 0;
      lateral_offset_ = 0;
      d_lateral_offset_ = 0;
      d2_lateral_offset_ = 0;
      k_s_ = 0;

      control_mark_ = 0;
      suggest_kp_ = 2;
    }

    ~navi_point()
    {


    }
};

