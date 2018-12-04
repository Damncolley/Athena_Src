/**
 * @file lqr_lat_controller.h
 * @author jiang <jiangchengjie@indrv.cn>
 * @date  2018-07-07
 * @version 1.0.0
 * @par  Copyright(c)
 *        hy
 */

#ifndef CONTROLLER_LQR_LAT_LATCONTROLLER_H_
#define CONTROLLER_LQR_LAT_LATCONTROLLER_H_

#include "../generic_controller.h"
#include "../common/eigen3/Eigen/Core"
#include "../common/filters/digital_filter.h"
#include "../common/filters/digital_filter_coefficients.h"
#include "../common/filters/mean_filter.h"
#include "../common/interpolation_1d.h"
#include "../common/interpolation_2d.h"
#include "simple_lateral_debug.h"
#include "../scheduler.h"


#include <memory>
using namespace std;

using namespace apollo::common;
using namespace apollo::control;
using Matrix = Eigen::MatrixXd;

/**
 * @namespace athena::control
 * @brief athena::control
 */
namespace athena{
namespace control{
/**
 * @class LQRLatController
 *
 * @brief LQRLatController.
 */
class LQRLatController:public GenericController{
 public:
 /**
  * @brief constructor
  */
 LQRLatController() = default;

 /**
  * @brief destructor
  */
 ~LQRLatController() = default;

 /**
 * @brief init.
 * @param[in] controller_config controller config.
 * @return true or false.
 */
 bool Init(const ControllerConfig  controller_config);

 /**
 * @brief ComputeControlOutput.
 * @param[in] path 轨迹.
 * @param[in] match_point_no 轨迹匹配索引.
 * @param[in] localiation 定位信息.
 * @param[in] chassis 车辆底盘信息.
 * @param[in] ControllerOutput  控制器输出.
 * @return true or false.
 */
 bool ComputeControlOutput(path * path,uint32_t match_point_no,const LocalLocalization * local_localiation,const Chassis *chassis,ControllerOutput * controller_output);

 /**
 * @brief 获取调试信息.
 * @param[in] debug_output 调试输出.
 * @return void.
 */
 void GetControllerDebugInfo(DebugOutput &debug_output);

 /**
 * @brief SetTarSpeedDebug 设置推荐速度.
 * @param[in] tar_speed 推荐速度.
 * @param[in] valid 是否有效.
 * @return void.
 */
  void SetTarSpeedDebug(int32_t tar_speed,bool valid);

 protected:
  ControllerConfig controller_config_;
  //the following parameters are vehicle physics related.
  //control time interval
  double ts_ = 0.01;
  // corner stiffness; front
  double cf_ = 0.0;
  // corner stiffness; rear
  double cr_ = 0.0;
  // distance between front and rear wheel center
  double wheelbase_ = 0.0;
  // mass of the vehicle
  double mass_ = 0.0;
   //distance from front wheel center to COM
  double lf_ = 0.0;
   //distance from rear wheel center to COM
  double lr_ = 0.0;
   //rotational inertia
  double iz_ = 0.0;
  // the ratio between the turn of the steering wheel and the turn of the wheels
  double steer_ratio_ = 0.0;
  // the maximum turn of steer
  double steer_single_direction_max_degree_ = 0.0;

  // limit steering to maximum theoretical lateral acceleration
  double max_lat_acc_ = 0.0;

  // number of control cycles look ahead (preview controller)
  int preview_window_ = 0;
  //   number of states without previews, includes
  //  lateral error, lateral error rate, heading error, heading error rate
  const int basic_state_size_ = 4;
  // vehicle state matrix
  Eigen::MatrixXd matrix_a_;
  // vehicle state matrix (discrete-time)
  Eigen::MatrixXd matrix_ad_;
  // vehicle state matrix compound; related to preview
  Eigen::MatrixXd matrix_adc_;
  // control matrix
  Eigen::MatrixXd matrix_b_;
  // control matrix (discrete-time)
  Eigen::MatrixXd matrix_bd_;
  // control matrix compound
  Eigen::MatrixXd matrix_bdc_;
  // gain matrix
  Eigen::MatrixXd matrix_k_;
  // control authority weighting matrix
  Eigen::MatrixXd matrix_r_;
  // state weighting matrix
  Eigen::MatrixXd matrix_q_;
  // updated state weighting matrix
  Eigen::MatrixXd matrix_q_updated_;
  // vehicle state matrix coefficients
  Eigen::MatrixXd matrix_a_coeff_;
  // 4 by 1 matrix; state matrix
  Eigen::MatrixXd matrix_state_;

  // parameters for lqr solver; number of iterations
  int lqr_max_iteration_ = 0;
  // parameters for lqr solver; threshold for computation
  double lqr_eps_ = 0.0;

  DigitalFilter digital_filter_;

  std::unique_ptr<Interpolation1D> lat_err_interpolation_;

  std::unique_ptr<Interpolation1D> heading_err_interpolation_;

  // MeanFilter heading_rate_filter_;
  MeanFilter lateral_error_filter_;
  MeanFilter heading_error_filter_;

  const std::string name_;

  double pre_steer_angle_ = 0.0;

  double minimum_speed_protection_ = 0.1;

  double current_trajectory_timestamp_ = -1.0;

  double init_vehicle_x_ = 0.0;

  double init_vehicle_y_ = 0.0;

  double init_vehicle_heading_ = 0.0;

  double min_turn_radius_ = 0.0;

  int q_param_size_ = 6;

   //double matrix_q[6] = {0.05,0.0,1.0,0.0,0.0,0.0};
  std::vector<double> matrix_q_init_;
  int32_t cutoff_freq_ = 10;
  int32_t mean_filter_window_size_ = 10;
  int32_t lat_err_scheduler_size_ = 5;
  std::vector<Scheduler> lat_err_scheduler_init_;
  int32_t heading_err_scheduler_size_ = 5;
  std::vector<Scheduler> heading_err_scheduler_init_;
 private:

    /**
     * @brief InitializeFilters 初始化滤波器.
     * @return void.
     */
    void InitializeFilters();

    /**
    * @brief LoadLatGainScheduler 导入调节器.
    * @return void.
    */
    void LoadLatGainScheduler() ;

   /**
    * @brief LoadLQR控制配置文件
    * @return void.
    */
    bool LoadLQRControlConf();
    void UpdateMatrixCompound();
    double CalculateErrorSignAndVaule(double x, double y, double yaw,double x_c, double y_c, double yaw_c);
    void  CalculateDertaFiByTable(double current_heading, double head_c, double *p_derta_fi, double *p_derta_fi_radian);
    void UpdateMatrix(const LocalLocalization * local_localiation);
    double ComputeFeedForward(const LocalLocalization * local_localiation,double ref_curvature) const;
    void UpdateState(path * path,uint32_t match_point_no,const LocalLocalization * local_localiation,SimpleLateralDebug *debug);
    void ComputeLateralErrors(path * path,uint32_t match_point_no,const double x, const double y,const double theta,const double linear_v,const double angular_v,SimpleLateralDebug *debug);
};
}
}

#endif
