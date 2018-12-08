/*-------------------------------------------------------
 * 文件名：nad_task_userfunc.cpp
 * 创建者：张毅00151602
 * 时  间：2016-03-02
 * 描  述：二期脚本中的用户自定义函数
-------------------------------------------------------*/

//头文件
#include "task/nad_task_userfunc.h"

//起点一百米以内认为是脚本路径(终点需要精确匹配)
#define ROUTE_SAME 100.0

//匹配经纬度
bool nad_user_route::find_route(double starting_lon, double starting_lat, double ending_lon, double ending_lat)
{
    if (key_point_list.size() < 2)
    {
        return false;
    }

    //简单处理，两者的启动、停车点在直线距离100米之内
    key_point &p1 = key_point_list[begin];
    key_point &p2 = key_point_list.back();
    if (dist_in_ll(starting_lat, starting_lon, p1.lat, p1.lon, ROUTE_SAME) &&
        dist_in_ll(ending_lat, ending_lon, p2.lat, p2.lon, 2.0))
    {
        return true;
    }
    return false;
}

//调试打印
void nad_user_route::show(const char *cap)
{
    printf("%s:nad_user_route:%s, begin=%d\n", cap, name.c_str(), (int)begin);
    printf("  key_point_list=%d\n", (int)key_point_list.size());
    for (size_t i = 0; i < key_point_list.size(); i++)
    {
        key_point &kp = key_point_list[i];
        printf("    lon=%.7f, lat=%.7f, type=%d\n", kp.lon, kp.lat, kp.type);
    }
    printf("  action_list=%d\n", (int)action_list.size());
    for (size_t i = 0; i < action_list.size(); i++)
    {
        nad_user_action &ua = action_list[i];
        if (ua.action == ACTION_CHANGE_LANE)
        {
            printf("    mileage=%.1f, dirt=%s, reason=%s\n",
                ua.mileage, cl_direction_str(ua.direction), cl_reason_str(ua.reason));
        }
        else if (ua.action == ACTION_ALARM)
        {
            printf("    mileage=%.1f, alarm=%s, level=%s\n",
                ua.mileage, ua.alarm.c_str(), alarm_str(ua.alarm_level));
        }
    }
}

//匹配经纬度，推荐使用本函数，会把key_point_list[0]替换成starting_lat、starting_lon，成功返回true
nad_user_route *nad_user_route_list::find_route(string obu_name, vector<key_point> &key_point_list, size_t &begin, int32_t type)
{
    //至少需要两个点
    if (key_point_list.size() < 2)
    {
        return NULL;
    }

    //查找本obu有没有自定义路径
    map<string, vector<nad_user_route *> >::iterator it;
    it = obu_list.find(obu_name);
    if (it == obu_list.end())
    {
        return NULL;
    }

    //查找对应的route
    double starting_lon = key_point_list.begin()->lon;
    double starting_lat = key_point_list.begin()->lat;
    double ending_lon = key_point_list.back().lon;
    double ending_lat = key_point_list.back().lat;

    //路径匹配
    double min_dist = 100000000;
    size_t min_index = 10000;
    vector<nad_user_route *> &obu_route_list = it->second;
    for (size_t i = 0; i < obu_route_list.size(); i++)
    {
        nad_user_route *route = obu_route_list[i];

        if (type == TASK_FR_NORMAL)
        {
            if (route->find_route(starting_lon, starting_lat, ending_lon, ending_lat))
            {
                key_point_list = route->key_point_list;
                begin = route->begin;
                key_point_list[begin].lon = starting_lon;
                key_point_list[begin].lat = starting_lat;
                return route;
            }
        }
        else if (type == TASK_FR_CALLCAR)
        {
            /*
           //与召车起点比较
           //如果与召车A点更近
            if (dist_two_points_ll(starting_lat, starting_lon,  ADDR_9_LAT,  ADDR_9_LON) <= dist_two_points_ll(starting_lat, starting_lon,  ADDR_10_LAT,  ADDR_10_LON))
            {
                if (srcontain(route->name, "_callcar_a"))
                {
                    double dist = dist_two_points_ll(ending_lat, ending_lon, route->key_point_list.back().lat, route->key_point_list.back().lon);

                    if (min_dist > dist)
                    {
                        min_dist = dist;
                        min_index = i;
                    }
                }
            }
            else  //如果与召车B点更近
            {
                if (srcontain(route->name, "_callcar_b"))
                {
                    double dist = dist_two_points_ll(ending_lat, ending_lon, route->key_point_list.back().lat, route->key_point_list.back().lon);

                    if (min_dist > dist)
                    {
                        min_dist = dist;
                        min_index = i;
                    }
                }
            }
            */

        }
        else if (TASK_FR_BLOCK_A == type)//???????
        {
            if (srcontain(route->name, "_block_a"))
            {
                key_point_list = route->key_point_list;
                begin = route->begin;
                key_point_list[begin].lon = starting_lon;
                key_point_list[begin].lat = starting_lat;
                return route;
            }
        }
        else if (TASK_FR_BLOCK_B == type)
        {
            if (srcontain(route->name, "_block_b"))
            {
                key_point_list = route->key_point_list;
                begin = route->begin;
                key_point_list[begin].lon = starting_lon;
                key_point_list[begin].lat = starting_lat;
                return route;
            }
        }
    }

    if (type == TASK_FR_CALLCAR && min_index != 10000)
    {
        nad_user_route *route = obu_route_list[min_index];
        key_point_list = route->key_point_list;
        begin = route->begin;
        key_point_list[begin].lon = starting_lon;  //替换车的实际位置
        key_point_list[begin].lat = starting_lat;    //

        return route;
    }

    return NULL;
}

//从脚本函数中加载路径规划
int nad_user_route_list::task_add_route(nad_task_func &func, string &ret, string &desc)
{
    //创建路径规划并获取key_point_list
    nad_user_route route;
    route.name = func.get_para("", "");
    string list = func.get_para("key_point_list", "");
    if (sub_count(list, ",") == 0)
    {
        desc = "关键点列表不正确，必须形如: key_point_list=\"lon1|lat1|lane_id1, lon2|lat2|lane_id2...\"";
        return RET_ERROR;
    }
    stringlist ls;
    ls.load_from_part(list, ",");
    ls.trim(true, false);
    key_point kp;
    for (size_t i = 0; i < ls.size(); i++)
    {
        if (scontain(ls[i], "begin:"))
        {
            route.begin = route.key_point_list.size();
            get_lon_lat_id(kp.lon, kp.lat, kp.type, srightbut(ls[i], 6));
        }
        else
        {
            get_lon_lat_id(kp.lon, kp.lat, kp.type, ls[i]);
        }
        route.key_point_list.push_back(kp);
    }

    //添加动作点，动作点是可选参数
    list = func.get_para("action_list", "");
    if (sub_count(list, ",") > 0)
    {
        ls.load_from_part(list, ",");
        ls.trim(true, false);
        nad_user_action ua;
        stringlist pt;
        for (size_t i = 0; i < ls.size(); i++)
        {
            pt.load_from_part(ls[i], "|");
            if (pt.size() >= 3)
            {
                ua.mileage = atof(pt[0].c_str());
                if (stoupper(pt[1]) == "LEFT" || stoupper(pt[1]) == "RIGHT")
                {
                    //换道
                    ua.action = ACTION_CHANGE_LANE;
                    ua.direction = (stoupper(pt[1]) == "LEFT" ? CL_DIRECTION_LEFT : CL_DIRECTION_RIGHT);
                    ua.reason = cl_reason_val(pt[2].c_str());
                }
                else if (stoupper(pt[1]) == "ALARM")
                {
                    //上报告警
                    ua.action = ACTION_ALARM;
                    ua.alarm = pt[2];
                    if (pt.size() >= 4)
                    {
                        ua.alarm_level = alarm_val(pt[2].c_str());
                    }
                }
                route.action_list.push_back(ua);
            }
        }
    }

    //添加路径
    route_list[route.name] = route;
    LOG(WARNING) << "task_add_route: " << route.name << ", begin=" << route.begin
        << ", kp=" << route.key_point_list.size() << ", action=" << route.action_list.size();
    return RET_OK;
}

//从脚本函数中加载obu的路径列表
int nad_user_route_list::task_set_route(nad_task_func &func, string &ret, string &desc)
{
    char buf[100];

    //获得obu名称
    string obu_name = func.get_para("", "");
    if (obu_list.find(obu_name) == obu_list.end())
    {
        sprintf(buf, "OBU \"%s\" 不存在！", obu_name.c_str());
        desc = buf;
        return RET_NOT_EXIST;
    }

    //获得新的路径列表
    stringlist ls;
    vector<nad_user_route *> obu_route_list;
    ls.load_from_part(func.get_para("route", ""), ",");
    ls.trim(true, false);
    for (size_t i = 0; i < ls.size(); i++)
    {
        map<string, nad_user_route>::iterator it;
        it = route_list.find(ls[i]);
        if (it == route_list.end())
        {
            sprintf(buf, "路径名 %s 不存在！", ls[i].c_str());
            desc = buf;
            return RET_NOT_EXIST;
        }
        obu_route_list.push_back(&it->second);
        LOG(WARNING) << "task_set_route(" << obu_name << "): " << it->first;
    }
    obu_list[obu_name] = obu_route_list;
    return RET_OK;
}

