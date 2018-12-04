#ifndef _NAD_ROUTE_CONFIG_H
#define _NAD_ROUTE_CONFIG_H

#include <string>
#include <vector>

using std::vector;
using std::string;
//引用base头文件
#include "../nad_base.h"

class nad_route_config
{
public:
    double lat;
    double lon;
    int type;
public:
    void load_from_xml( pugi::xml_node &node);

     //重载赋值
    nad_route_config & operator = (const nad_route_config &route)
    {
        lat = route.lat;
        lon = route.lon;
        type = route.type;
        return *this;
    }

    //重载打印输出
    friend inline ostream & operator << (ostream & os, nad_route_config &route)
    {
        cout << "lat=" << route.lat << ", lon=" << route.lon << ", type=" << route.type  << endl;
        return os;
    }
};

class nad_route_config_list
{
public:
    //网元配置数组
    map<string, vector<nad_route_config> > route_map;

public:
    //查找网元
    nad_route_config *find(string name);

    //从xml文件中加载
    int load_from_file(string filename);

    //重载打印输出
    friend inline ostream & operator << (ostream & os, nad_route_config_list &ne)
    {
        map<string, vector<nad_route_config> >::iterator it;
        for(it = ne.route_map.begin(); it != ne.route_map.end(); it++)
        {
            vector<nad_route_config>::iterator it_inner;
            for (it_inner = it->second.begin(); it_inner != it->second.end(); ++it_inner)
                std::cout << *it_inner << std::endl;
        }
        return os;
    }
};

//路线规划
class route_config
{
public:
    nad_route_config_list route_list;
public:
    //文件中加载地图相关信息，包括坐标原点信息
    int load_from_file(string filename);

    void show();
};

int nad_route_config_init();
//全局配置数据结构
extern route_config g_route_config;

#endif
