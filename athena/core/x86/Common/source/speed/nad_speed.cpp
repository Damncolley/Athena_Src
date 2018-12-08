#include "info/nad_speed.h"
#include "nad_type.h"
#include "nad_function.h"

//构造函数
nad_speed::nad_speed()
{
    speed = 0.0;
}

//析构函数
nad_speed::~nad_speed()
{
}

//添加一个速度值，返回综合速度
double nad_speed::add(string name, int priority, int type, double speed_s, int64_t ttl)
{
    //禁止出现同名速度值
    vector<nad_speed_value>::iterator it;
    for(it = speed_list.begin(); it != speed_list.end(); it++)
    {
        if (it->name == name)
        {
            speed_list.erase(it);
            break;
        }
    }

    //追加速度值
    nad_speed_value value;
    value.name = name;
    value.priority = priority;
    value.type = type;
    value.speed = speed_s;
    value.ttl = get_current_time() + ttl;
    speed_list.push_back(value);

    //判断是否存在高优先级速度
    priority = SV_LOW;
    for (size_t i = 0; i < speed_list.size(); i++)
    {
        if (speed_list[i].priority == SV_HIGH)
        {
            priority = SV_HIGH;
            break;
        }
    }

    //查找一个最低的绝对速度
    speed_s = 1000000.0;
    for (size_t i = 0; i < speed_list.size(); i++)
    {
        nad_speed_value &v = speed_list[i];
        if (v.priority == priority && v.type == SV_ABSOLUTE && speed_s > v.speed)
        {
            speed_s = v.speed;
        }
    }

    //叠加所有相对速度
    for (size_t i = 0; i < speed_list.size(); i++)
    {
        nad_speed_value &v = speed_list[i];
        if (v.priority == priority && v.type == SV_RELATE)
        {
            speed_s += v.speed;
        }
    }

    //保存结果
    this->speed = max(0.0, speed_s);
    return this->speed;
}

//查找一个速度
nad_speed_value *nad_speed::find(string name)
{
    for (size_t i = 0; i < speed_list.size(); i++)
    {
        if (speed_list[i].name == name)
        {
            return &speed_list[i];
        }
    }
    return NULL;
}

//清空所有的速度
void nad_speed::clear()
{
    speed = 0.0;
    int64_t now = get_current_time();
    vector<nad_speed_value>::iterator it;
    for (it = speed_list.begin(); it != speed_list.end();)
    {
        if (it->ttl <= now)
        {
            it = speed_list.erase(it);
        }
        else
        {
            it++;
        }
    }
}

//打印速度值
void nad_speed::show()
{
    int64_t now = get_current_time();
    printf("speed = %.1f km/h\n", speed);
    for (size_t i = 0; i < speed_list.size(); i++)
    {
        nad_speed_value &v = speed_list[i];
        printf("  %d. %s = %s = %s = %ldms = %.1f km/h\n", (int)i + 1, v.name.c_str(),
            v.priority == SV_LOW ? "low" : "high",
            v.type == SV_ABSOLUTE ? "abs" : "rel", max((int64_t)0, v.ttl - now), v.speed);
    }
}

