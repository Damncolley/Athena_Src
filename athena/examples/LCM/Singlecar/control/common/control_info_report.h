/**
 * @file    control_info_report.h
 * @author  jiang <jiangchengjie@indrv.cn>
 * @date    2018-07-07
 * @version 1.0.0
 * @par     Copyright(c)
 *          hy
 */

#ifndef COMMON_CONTROL_INFO_REPORT_H_
#define COMMON_CONTROL_INFO_REPORT_H_


 /**
 * @namespace athena::control
 * @brief athena::control
 */
namespace athena{
namespace control{
/**
 * @class ControlInfoReport
 * @brief 控制反馈.
 */
class ControlInfoReport
{
public:
    ControlInfoReport() = default;
    ~ControlInfoReport() =default;

    double     cur_lon_;
    double     cur_lat_;
    double     cur_yaw_;
    double     cur_brake_;
    double     cur_speed_;
    double     cur_speed_lateral_;
    double     cur_speed_longitudinal_;
    double     cur_acceleration_pattern_;
    double     cur_acceleration_;
    double     cur_acceleration_lateral_;
    double     cur_acceleration_longitudinal_;
    double     steering_angle_;
    int32_t    flashing_status_;
    int32_t    cur_gears_;
    int32_t    num_of_camera_stat_;
    std::vector< int32_t > camera_stat_;
    int32_t    num_of_radar_stat_;
    std::vector< int32_t > radar_stat_;
    int32_t    num_of_lidar_stat_;
    std::vector< int32_t > lidar_stat_;
    int32_t    num_of_gps_stat_;
    std::vector< int32_t > gps_stat_;
    double     gps_time_;
    double     mileage_;
    int32_t    eps_stat_;
    int32_t    epb_stat_;
    int32_t    brake_stat_;
};
}
}
#endif //COMMON_CONTROL_INFO_REPORT_H_
