/**
 * @file    control_debug.h
 * @author  jiang <jiangchengjie@indrv.cn>
 * @date    2018-07-07
 * @version 1.0.0
 * @par     Copyright(c)
 *          hy
 */

#ifndef APPS_CONTROL_DEBUG_CONTROL_DEBUG_H_
#define APPS_CONTROL_DEBUG_CONTROL_DEBUG_H_

#include "../../common/local_timer.h"
#include "../control.h"
#include "../../common/timer_app.h"


 /**
 * @namespace athena::control
 * @brief athena::control
 */
namespace athena{
namespace control{

 /**
 * @class ControlDebug
 * @brief 控制调试.
 */

template<class T>
class ControlDebug
{
  typedef void (T::*PrintFun)();
  typedef void (T::*LogFun)();

public:
    void AddPrint(int print_interval,PrintFun,T *handler);
    void AddLog(int log_interval,LogFun log_fun,T *handler);
    void PrintOnTimer();
    void LogOnTimer();

private:
     int print_interval_;
     int log_interval_;
     PrintFun print_fun_;
     LogFun log_fun_;
     T *handler_;
};

//设置调用对象及其回调函数
template<class T>
void ControlDebug<T>::AddPrint(int print_interval,PrintFun print_fun,T *handler)
{
    print_interval_ = print_interval;
    print_fun_ = print_fun;
    handler_ = handler;

    if(print_interval != 0)
    TimerApp<ControlDebug>::AddTimer(print_interval_,&ControlDebug::PrintOnTimer,this);
};

//设置调用对象及其回调函数
template<class T>
void ControlDebug<T>::AddLog(int log_interval,LogFun log_fun,T *handler)
{
    log_fun_ = log_fun;
    log_interval_ = log_interval;
    handler_ = handler;

    if(log_interval != 0)
    TimerApp<ControlDebug>::AddTimer(log_interval_,&ControlDebug::LogOnTimer,this);
};

//调用回调函数
template<class T>
void ControlDebug<T>::PrintOnTimer()
{
    (handler_->*print_fun_)();
}

template<class T>
void ControlDebug<T>::LogOnTimer()
{
    (handler_->*log_fun_)();
}
}
}
#endif //APPS_CONTROL_DEBUG_CONTROL_DEBUG_H_
