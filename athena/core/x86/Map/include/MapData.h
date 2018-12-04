/**
* @file       MapData.h
* @brief      地图数据类型定义
* @details    定义地图中的红绿灯、限速牌、施工标志、路口、车道
* @author     huanhuan
* @date       2018/7/16
* @version    v1.0
* @par Copyright (c):
*      武汉环宇智行科技有限公司
* @par History:
*   version: author, date, desc\n
*/
#ifndef _MAP_DATA_H
#define _MAP_DATA_H


#include"LaneletMap.hpp"
//使用glog应该包含的头文件
#include <glog/logging.h>
#include <glog/raw_logging.h>  //支持线程安全
//支持boost
#include <boost/filesystem.hpp>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

using namespace LLet;

namespace athena
{
namespace roadmap
{
#define NEAREST_LANE_LIST 30.0
typedef LaneletMap OSMMap;

class map_point
{
public:
    double lon;  //经度
    double lat;  //纬度
    int64_t id;  //在OSM地图里面的id
};



//限速牌
class limspeed
{
public:
    std::string limspeed_id;     //地图没有限速牌，id=lon|lat
    int32_t active_index;   //车辆从center_line[active_index]开始按limspeed_value速度行驶
    int32_t limspeed_value; //限速值，单位：km/h
    int16_t alarm_flag;     //告警标志

    limspeed()
    {
        active_index = -1;
        limspeed_value = 20.0;
        alarm_flag = 0;
    }
};

//施工标志
class block
{
public:
    std::string block_id;     //地图没有施工标志，id=lon|lat
    int32_t stop_index;  //车辆停止在center_line[stop_index]位置
    int32_t block_value; //限速值，单位：km/h
    int32_t lane_count;  //车道数量
    int16_t alarm_flag;  //告警标志

    block()
    {
        stop_index = -1;
        block_value = 0;
        lane_count = 2;
        alarm_flag = 0;
    }

    //输入当前车道，lane_index=左数车道，第一道是1
    //返回规避方法: CL_DIRECTION_NONE=不阻塞，LEFT=向左换道规划，RIGHT=向右换道规划，BLOCK=堵死
    int check(int lane_index,int op_lane_size = -1);
};

class light
{
public:
    int64_t id;
    double mileage;              //mileage: + m; active -> stop
    map_point stop_point;
    map_point exit_point;
    map_point light_point;
};

class cross_regulator
{
public:
    std::string name;//路口名称
    int type;  //路口类型
    std::vector<light> flow_light_list_; //flow_light_list 车流的红绿灯
    //vector<map_point> points_;      //车流里的与红绿灯相关的点
};

/**
* @brief 车道描述
* 包含每个车道的id、当前的兄弟车道、对向的兄弟车道、可换道的车道、车道长度、最大速度、道路等级
*/
class lane
{
public:
    int64_t lane_id;                      ///<当前lane的id
    std::vector<int64_t> cur_brother_lane;     ///<当前车辆所在方向，所有兄弟车道，从左向右排列，车辆换道后修改lane_id。
    std::vector<int64_t> opp_brother_lane;     ///<逆向兄弟车道，从左向右排列。
    std::vector<int64_t> change_lane;          ///<当前车道所在的可换道的车道列表，从左向右排列，包含了可以借道的区域。
    double length;                        ///<车道长度，单位：m
    double max_speed;                     ///<最大限速，单位：km/h
    int32_t road_level;                   ///<道路等级

    lane():lane_id(-1), max_speed(60), road_level(0) {}
};
}
}
#endif // _MAP_DATA_H
