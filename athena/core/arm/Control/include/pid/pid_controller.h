/**
 * @file    controller.h
 * @author  jiang <jiangchengjie@indrv.cn>
 * @date    2018-07-07
 * @version 1.0.0
 * @par     Copyright(c)
 *          hy
 */

#include <iostream>
#include <time.h>

using namespace std;

/**
 * @namespace athena::control
 * @brief athena::control
 */
namespace athena{
namespace control{

/**
 * @class PIDController
 *
 * @brief PID controller, 计算PID输出值.
 */
class PIDController
{
private:
    ///比例常数 Proportional
    double p_value;
    ///积分常数 Integral
    double i_value;
    ///微分常数 Derivative
    double d_value;
    ///上一次误差值
    double last_error;
    ///p比例计算值
    double p_factor_value;
    ///i比例计算值
    double i_factor_value;
    ///d比例计算值
    double d_factor_value;
    ///计时起始时间
    struct timespec tpstart;
    ///计时截止时间
    struct timespec tpend;

     /**
     * @brief get_timer_tick.
     * @return 时间差(毫秒).
     */
    double get_timer_tick();

public:
    PIDController()
    {
        last_error = 0; //Error[-1]
        p_value = 0; //比例常数 Proportional Const
        i_value = 0; //积分常数 Integral Const
        d_value = 0; //微分常数 Derivative Const


        p_factor_value = 0;//p比例计算值
        i_factor_value = 0;//i比例计算值
        d_factor_value = 0;//d比例计算值
    }
    ~PIDController(){};

     /**
     * @brief get_pid_value 获取PID调节P I D值.
     * @param[in] p p值.
     * @param[in] i i值.
     * @param[in] d d值.
     * @return void.
     */
    void get_pid_value(double &p,double &i,double &d);

    /**
     * @brief get_pid_p_factor_value 获取P值计算值.
     * @return P值计算值.
     */
    double get_pid_p_factor_value(void);

    /**
     * @brief get_pid_i_factor_value 获取I值计算值.
     * @return I值计算值.
     */
    double get_pid_i_factor_value(void);

    /**
     * @brief get_pid_d_factor_value 获取D值计算值.
     * @return D值计算值.
     */
    double get_pid_d_factor_value(void);
    //add end

     /**
     * @brief ComputeControlOutput.
     * @param[in] kp p值.
     * @param[in] ki i值.
     * @param[in] kd d值.
     * @return void.
     */
    void set_pid(double kp,double ki,double kd);

    /**
     * @brief pid_calculate.
     * @param[in] tar_value 目标值.
     * @param[in] current_value 当前值.
     * @return PID输出值.
     */
    double pid_calculate(double tar_value,double current_value);
};
}
}


