/*-------------------------------------------------------
 * 文件名：nad_timer.cpp
 * 创建者：张毅00151602
 * 时  间：2016-03-02
 * 描  述：定时器的基类
-------------------------------------------------------*/


//头文件
#include "timer/nad_timer.h"


//构造定时器
nad_timer::nad_timer(int64 interval_ms)
{
    this->interval_ms = interval_ms;
    this->last_ms = 0;
    this->counter = 0;
}

//析构函数
nad_timer::~nad_timer()
{
}

//判断计数器，比如counter_is(5)==true时表示每1秒调用一次(200ms*5=1s)
bool nad_timer::counter_is(int64_t counter)
{
    return (this->counter % counter) == 0;
}

//全局定时器数组
nad_timer_list g_ltimer;

//构造析构函数
nad_timer_list::nad_timer_list()
{
}

nad_timer_list::~nad_timer_list()
{
    //删除每个定时器
    vector<nad_timer *>::iterator iter;
    for (iter = timer_list.begin(); iter != timer_list.end(); iter++)
    {
        delete *iter;
    }
}

//新增定时器
int nad_timer_list::add_timer(nad_timer *timer)
{
    vector<nad_timer *>::iterator iter = find(timer_list.begin(), timer_list.end(), timer);
    if (iter == timer_list.end())
    {
        //没找到，则添加
        cout << "nad_timer_list::add_timer(): ok" << endl;
        timer_list.push_back(timer);
        return RET_OK;
    }
    else
    {
        //找到
        cout << "nad_timer_list::add_timer(): exists" << endl;
        return RET_EXIST;
    }
}

//删除定时器
int nad_timer_list::delete_timer(nad_timer *timer)
{
    vector<nad_timer *>::iterator iter = find(timer_list.begin(), timer_list.end(), timer);
    if (iter == timer_list.end())
    {
        //没找到
        cout << "nad_timer_list::delete_timer(): not exists" << endl;
        return RET_NOT_EXIST;
    }
    else
    {
        //找到，则删除
        cout << "nad_timer_list::delete_timer(): ok" << endl;
        delete *iter;
        timer_list.erase(iter);
        return RET_OK;
    }
}

//执行定时器，传入当前的时间(毫秒)
void nad_timer_list::handle_timer()
{
    //获取当前时间（毫秒）
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    int64 current_ms = tv.tv_sec * 1000 + tv.tv_usec / 1000;
    //cout << "nad_timer_list::handle_timer(): " << current_ms << endl;

    //执行每个定时器的回调函数
    vector<nad_timer *>::iterator iter;
    for (iter = timer_list.begin(); iter != timer_list.end(); iter++)
    {
        if (current_ms - (*iter)->last_ms >= (*iter)->interval_ms)
        {
            //由继承nad_timer的类实现虚函数handle(current_ms)
            nad_timer *timer = *iter;
            //cout << "nad_timer *timer= " << timer << endl;

            timer->handle();
            timer->counter++;
            timer->last_ms = current_ms;
        }
    }
}
