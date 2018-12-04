/**
 * @file    Scheduler.h
 * @author  jiang <jiangchengjie@indrv.cn>
 * @date    2018-07-07
 * @version 1.0.0
 * @par     Copyright(c)
 *          hy
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

 /**
 * @namespace athena::control
 * @brief     athena::control
 */
namespace athena{
namespace control{


/**
 * @class Scheduler
 *
 * @brief 调节器.
 */
class Scheduler
{
 public:
 Scheduler()
 {
    speed_ = 0.0;
    ratio_ = 0.0;
 }
 ~Scheduler() = default;
    double speed_;
    double ratio_;
};
}
}

#endif // CHASSIS_H_
