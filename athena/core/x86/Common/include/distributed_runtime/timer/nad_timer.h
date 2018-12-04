/*-------------------------------------------------------
 * 文件名：nad_timer.h
 * 时  间：2016-03-02
 * 描  述：定时器的基类
-------------------------------------------------------*/
#ifndef _nad_timer_H
#define _nad_timer_H

#include "nad_base.h"
//定时器的基类
class nad_timer
{
public:
    //定时器时间间隔
    int64 interval_ms;

    //上次handle的时间
    int64 last_ms;

    //计数器，配合counter_is()实现几个周期触发一次的定时器
    int64_t counter;

public:
    //构造析构函数
    nad_timer(int64 interval_ms);
    virtual ~nad_timer();

    //执行定时器，传入当前的时间(毫秒)
    virtual void handle() = 0;

    //判断计数器，比如counter_is(5)==true时表示每1秒调用一次(200ms*5=1s)
    bool counter_is(int64_t counter);
};

//定时器管理器的基类
class nad_timer_list
{
//protected:
public:
    //定时器列表
    vector<nad_timer *> timer_list;

public:
    //构造析构函数
    nad_timer_list();
    virtual ~nad_timer_list();

    //新增定时器
    int add_timer(nad_timer *timer);

    //删除定时器
    int delete_timer(nad_timer *timer);

    //执行定时器，传入当前的时间(毫秒)
    void handle_timer();
};

//全局定时器数组
extern nad_timer_list g_ltimer;


#endif
