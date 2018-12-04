/**
 * @file    controller_agent.h
 * @author  jiang <jiangchengjie@indrv.cn>
 * @date    2018-07-07
 * @version 1.0.0
 * @par     Copyright(c)
 *          hy
 */

#ifndef COMMON_CONTROLLER_AGENT_H_
#define COMMON_CONTROLLER_AGENT_H_

#include "controller_config.h"
#include "trajectory.h"
#include "chassis.h"
#include "controller_output.h"
#include "controller_alarm_code.h"
#include "controller_output_alarm.h"
#include "generic_controller.h"
#include "localization.h"
#include "local_localization.h"
#include "common/map_matching/map_matching.h"
#include "lon_controller/lon_controller.h"
#include "lat_controller/lat_controller.h"
#include "lqr_controller/lqr_lat_controller.h"
#include "debug_output.h"
#include "common/map_matching/coordinate_transformation.h"


/**
 * @namespace athena::control
 * @brief     athena::control
 */
namespace athena{
namespace control{
class ControllerAgent{
public:
    ControllerAgent()  = default;
    ~ControllerAgent() = default;

/**
 * @brief initialization.
 * @param[in] controller_config controller config.
 * @return true or false.
 */
  bool Init(const ControllerConfig  controller_config_);

/**
 * @brief SetTrajectory.
 * @param[in] trajectory trajectory information.
 * @return void.
 */
  void SetTrajectory(const Trajectory *trajectory);

/**
 * @brief ComputeControlOutput.
 * @param[in] localiation 定位信息.
 * @param[in] chassis 车辆底盘信息.
 * @param[in] controller_output  控制器输出.
 * @return true or false.
 */
  bool ComputeControlOutput(const Localization * localiation,const Chassis *chassis,ControllerOutput * controller_output);

/**
 * @brief GetControllerInfo.
 * @param[in] debug_output 调试输出.
 * @return void.
 */
  void GetControllerInfo(DebugOutput &debug_output);

/**
 * @brief SetDrivingModeDebug 设置驾驶模式用作调试.
 * @param[in] mode 0 无效 1 人工驾驶 3 自动驾驶.
 * @return void.
 */
  void SetDrivingModeDebug(int32_t mode);

/**
 * @brief SetTarSpeedDebug 设置推荐速度.
 * @param[in] tar_speed 推荐速度.
 * @param[in] valid 是否有效.
 * @return void.
 */
  void SetTarSpeedDebug(int32_t tar_speed,bool valid);

/**
 * @brief GetAlarmInfo 获取报警信息.
 * @param[out] alarm_list 报警信息.
 * @return void.
 */
  void GetAlarmTableInfo(std::vector <ControllerOutputAlarm::AlarmInfoTable>  *alarm_list);


private:
    ///报警
    ControllerOutputAlarm *controller_output_alarm;
    ///地图点最小限定：30,低于30个点（3米）则不匹配该地图
    const int MAP_POINT_LIM_MIN = 30;
    ///地图点最大限定：10000,（5公里）则不匹配该地图
    const int MAP_POINT_LIM_MAX = 50000;
    ///轨迹匹配点
    int32_t match_point_no_;
    ///与轨迹偏差距离
    double error_;
    ///坐标转换对象
    CoordinateTransformation coordinate_transformation_;
    ///坐标转换输入
    //Localization localization;
    ///坐标转换输出
    LocalLocalization local_localization_;
    ///地图匹配对象
    map_matching matching_;
    ///轨迹处理 双buffer
    path recv_path_1_;
    path recv_path_2_;
    ///双buffer标识
    volatile int32_t switch_buffer_;
    ///基本配置
    ControllerConfig controller_config_;
    ///控制输出
    ControllerOutput controller_output_;
    ///调试输出
    DebugOutput debug_output_;
    ///自动驾驶模式调试
    bool debug_driving_mode_enable_;
    ///自动驾驶模式
    int32_t debug_driving_mode_;

    /**
     * @brief     GetMapMatchingNum.
     * @param[in] current_x_   地图坐标x.
     * @param[in] current_y_   地图坐标y.
     * @param[in] local_path_  路径.
     * @param[in] length_      轨迹点个数.
     * @param[in] min_error_   最小误差.
     * @return true or false.
     */
    int32_t GetMapMatchingNum(double current_x,double current_y,path *local_path,int32_t length,double& min_error);


   /**
    * @brief     PathConvert.
    * @param[in] path_msg_   controller 可处理的轨迹.
    * @param[in] trajectory_msg_   收到的轨迹.
    * @return    void.
    */
    void PathConvert(path& path_msg,const Trajectory *trajectory_msg);

   /**
    * @brief     SpecialHandler 特殊处理.
    * @param[out] controller_output   控制器输出.
    * @return    void.
    */
    void SpecialHandler(ControllerOutput * controller_output);

    ///纵向控制器
    LonController lon_controller_;
    ///横向控制器
    LatController lat_controller_;
    ///LQRController
    LQRLatController lqr_lat_controller;
    ///纵向控制器
    GenericController *generic_lon_controller_;
    ///横向控制器
    GenericController *generic_lat_controller_;

    /**
    * @brief     RegisterGenericController.
    * @param[in] generic_lon_controller 纵向控制器.
    * @param[in] generic_lat_controller 横向控制器.
    * @return    void.
    */
    void RegisterGenericController(GenericController * generic_lon_controller,GenericController *generic_lat_controller);

private:
    //pthread_mutex_t lock_;
};
}//namespace control
}//namespace athena

#endif // COMMON_CONTROLLER_AGENT_H_
