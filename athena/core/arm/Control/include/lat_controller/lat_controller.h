/**
 * @file lat_controller.h
 * @author jiang <jiangchengjie@indrv.cn>
 * @date  2018-07-07
 * @version 1.0.0
 * @par  Copyright(c)
 *        hy
 */

#ifndef CONTROLLER_LAT_LATCONTROLLER_H_
#define CONTROLLER_LAT_LATCONTROLLER_H_

#include "../generic_controller.h"
#include <math.h>
#include "../common/math_util.h"
#include "../common/kalman_filter.h"
#include "../common/kalman_filter_app.h"

/**
 * @namespace athena::control
 * @brief athena::control
 */
namespace athena{
namespace control{

/**
 * @class LatController
 *
 * @brief lateral controller, to compute steering values.
 */
class LatController:public GenericController{
 public:
 /**
  * @brief constructor
  */
 LatController() = default;

 /**
  * @brief destructor
  */
 ~LatController() = default;

/**
 * @brief init 初始化.
 * @param[in] controller_config controller config.
 * @return true or false.
 */
 bool Init(const ControllerConfig  controller_config);

 /**
 * @brief ComputeControlOutput 横向控制计算.
 * @param[in] path 轨迹.
 * @param[in] match_point_no 轨迹匹配索引.
 * @param[in] localiation 定位信息.
 * @param[in] chassis 车辆底盘信息.
 * @param[in] ControllerOutput  控制器输出.
 * @return true or false.
 */
 bool ComputeControlOutput(path * path,uint32_t match_point_no,const LocalLocalization * local_localiation,const Chassis *chassis,ControllerOutput * controller_output);

 /**
 * @brief SetTarSpeedDebug 设置推荐速度.
 * @param[in] tar_speed 推荐速度.
 * @param[in] valid 是否有效.
 * @return void.
 */
  void SetTarSpeedDebug(int32_t tar_speed,bool valid);
 private:
 ///最大曲率
 const double path_max_ks_ =  0.2;
 const double path_max_ks_u_  = 0.004;
 const double path_max_max_ks_u_ = 0.01;

 //const double m_pi = 3.14159265358979323846;
 ///地图匹配的最近点的距离 match_map_err
 double match_map_err_=0.0;
 double derta_fi_radian_=0.0;
  ///转向角速度
 const double steering_angle_speed_ = 300;
 ///方向反馈转角
 double feedback_steer_=0;

 ///角度误差
 double derta_fi_ = 0.0;
 double fi_err_hybird_ = 0.0;
 ///目标航向角速度误差
 double heading_speed_err;

 double kp_;
 double ki_;
 double kd_;

 double motion_curvature_last=0;

 ControllerConfig controller_config_;
 string start_time;

/**
 * @brief LateralHybridControl 横向控制.
 * @param[in] local_path 轨迹.
 * @param[in] match_point_no 轨迹匹配索引.
 * @param[in] local_localiation 定位信息.
 * @param[in] chassis 车辆底盘信息.
 * @param[in] controller_output 控制输出.
 * @return void.
 */
 void LateralHybridControl(path * local_path,uint32_t match_point_no,const LocalLocalization * local_localiation,const Chassis *chassis,ControllerOutput * controller_output);

/**
 * @brief CalculateFeedwardSteerAngle 计算前馈转角.
 * @param[in] local_path 轨迹.
 * @param[in] motion_curvature 规划层曲率.
 * @param[in] match_point 匹配点.
 * @return void.
 */
 void CalculateFeedwardSteerAngle(path * recv_path, double motion_curvature, int match_point);

 /**
 * @brief     CalculateMappingParameters 计算位置误差和角度误差.
 * @param[in] p 轨迹.
 * @param[in] match_point 匹配点.
 * @return    void.
 */
 void CalculateMappingParameters(path *p, int match_point_no,const LocalLocalization * local_localiation);

 double LateralHybridFeedbackControl(path *recv_path, int match_point_no,double e_err, double fi_err,double heading_speed_err);

 void LateralOutputSteering(double wheel_angle,double steering_angle_feedback,ControllerOutput * controller_output);

  /**
 * @brief     CalculateDertaFiByTable 计算角度误差
 * @param[in] current_heading 当前头指向.
 * @param[in] head_c 规划头指向.
 * @param[in] p_derta_fi 头指向.
 * @param[in] p_derta_fi_radian 头指向.
 * @return    void.
 */
 void CalculateDertaFiByTable(double current_heading,double head_c,double* p_derta_fi,double* p_derta_fi_radian);

 double CalculateErrorSignAndVaule(double x, double y, double yaw,double x_c, double y_c, double yaw_c);

/**
 * @brief 获取调试信息.
 * @param[in] debug_output 调试输出.
 * @return void.
 */
 void GetControllerDebugInfo(DebugOutput &debug_output);



};
}//namespace control
}//namespace athena
#endif //CONTROLLER_LATERAL_LATCONTROLLER_H
