/*-------------------------------------------------------
 * 文件名：nad_warning.h
 * 创建者：张毅00151602
 * 时  间：2016-10-11
 * 描  述：道路上的预警点
-------------------------------------------------------*/
#ifndef _NAD_WARNING_H
#define _NAD_WARNING_H

//#include "../../msg/nad_msg.h"
#include "nad_base.h"
#include "route.h"

using namespace athena;
//地图上的一个预警点
class nad_warning
{
public:
    string id;
    string type;
    string desc;
    double lon;
    double lat;
    double x;
    double y;
    int64_t lane_id;

    //构造函数
    nad_warning()
    {
        lon = lat = x = y = 0.0;
        lane_id = 0;
    }
};

//地图上的预警点列表
class nad_warning_list
{
    //预警点列表
    vector<nad_warning> list;

    //坐标转化器
    route::coord_transfer transfer;

public:
    //从数据库中加载预警点
    void load_from_db();

    //把预警点绑定到route::RouteBase上
    void bind_key_point(route::RouteBase &route);
};

//获得预警点类型ID
int get_warning_type(string type);

#endif
