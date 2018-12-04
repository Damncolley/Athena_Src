/*-------------------------------------------------------
 * 文件名：nad_task_userfunc.h
 * 创建者：张毅00151602
 * 时  间：2016-03-02
 * 描  述：二期脚本中的用户自定义函数
-------------------------------------------------------*/
#ifndef _NAD_TASK_USERFUNC_H
#define _NAD_TASK_USERFUNC_H


//头文件
#include "nad_task_func.h"
#include "info/nad_info.h"

//动作类型
#define ACTION_CHANGE_LANE  1  //换道
#define ACTION_ALARM        2  //触发告警

//用户自定义动作
class nad_user_action
{
public:
    double mileage;      //动作点，在nad_route.center_line中的mileage里程，注意从center_line[0]计里程，而不是KP_ROUTE_START点
    int action;          //动作类型，参考枚举值：ACTION_XXX
    int direction;       //换道方向，参考枚举值：CL_DIRECTION_XXX
    int reason;          //换道原因，参考枚举值：CL_REASON_XXX
    string alarm;        //告警信息
    int32_t alarm_level; //告警级别，参考枚举值：ALARM_XXX
    bool execed;         //是不是已经执行过

    //构造函数
    nad_user_action()
    {
        mileage = 100000000.0;
        action = ACTION_CHANGE_LANE;
        direction = CL_DIRECTION_LEFT;
        reason = CL_REASON_HAND;
        alarm_level = ALARM_ADVISE;
        execed = false;
    }
};

//用户自定义路径规划
class nad_user_route
{
public:
    string name;                               //路径规划名称
    size_t begin;                              //起点下标(起点前可能补点，以便motion规划)
    vector<key_point> key_point_list; //关键点列表
    vector<nad_user_action> action_list;       //动作点列表

public:
    //构造函数
    nad_user_route()
    {
        begin = 0;
    }

    //匹配经纬度
    bool find_route(double starting_lon, double starting_lat, double ending_lon, double ending_lat);

    //调试打印
    void show(const char *cap);
};

//用户自定义路径规划
class nad_user_route_list
{
public:
    map<string, nad_user_route> route_list;          //用户自定义路径列表
    map<string, vector<nad_user_route *> > obu_list; //每个obu可能使用哪些自定义路径

public:
    //用key_point_list首尾点匹配脚本路径，匹配到后替换key_point_list
    nad_user_route *find_route(string obu_name, vector<key_point> &key_point_list, size_t &begin, int32_t type = 0);

    //从脚本函数add_route中加载路径规划
    int task_add_route(nad_task_func &func, string &ret, string &desc);

    //从脚本函数set_obu中加载obu的路径列表
    int task_set_route(nad_task_func &func, string &ret, string &desc);
};

#endif
