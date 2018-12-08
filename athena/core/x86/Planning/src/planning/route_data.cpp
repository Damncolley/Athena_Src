#include "planning/route_data.h"
#include <math.h>
#include <iostream>

using namespace std;

/**
* @class point_xys
* @brief 形点边界线（专供planning使用）
* 约束可行驶区域，道路边界等
*/
point_xys::point_xys()
{

}

point_xys::~point_xys()
{

}

point_xys &point_xys::operator=(const point_xys & src)
{
    this->type = src.type;
    this->x = src.x;
    this->y = src.y;
    return *this;
}

/**
* @class point_m
* @brief 路径下发消息
* 中路中心上的点（专供motion使用）
*/
point_m::point_m()
{

}

point_m::~point_m()
{

}

point_m& point_m::operator=(const point_m & src)
{
    this->k = src.k;
    this->mileage = src.mileage;
    this->type = src.type;
    this->width = src.width;
    this->x = src.x;
    this->y = src.y;
    this->yaw = src.yaw;
    this->sug_speed = src.sug_speed;

    return *this;
}

/**
* @class section_m
* @brief 以左侧第一车道头指向的垂线截取的道路切片（专供motion使用）。
*/
section_m::section_m()
{
    num_of_lane = 0;

}

section_m::~section_m()
{

}

section_m& section_m::operator=(const section_m& src)
{
    this->num_of_lane = src.num_of_lane;
    this->lane = src.lane;
    return *this;
}

/**
* @class line_xys
* @brief 形点边界线（专供planning使用）
* 约束可行驶区域，道路边界等
*/
line_xys::line_xys()
{
    num_of_points = 0;

}

line_xys::~line_xys()
{

}

line_xys &line_xys::operator= (const line_xys& src)
{

    this->num_of_points = src.num_of_points;
    this->line = src.line;
    return *this;

}

/**
* @class route_planning_m
* @brief 路径规划（专供planning使用）
* 包含导航规划所经过的车道序列以及关键点等
*/
route_planning_m::route_planning_m()
{
    replan_flag = 0;
    num_of_section = 0;
}


route_planning_m::~route_planning_m()
{

}

route_planning_m &route_planning_m::operator=(const route_planning_m &src)
{
    this->replan_flag = src.replan_flag;
    //ret.time_stamp = src.time_stam;
    this->time_stamp = src.time_stamp;
    this->mileage_next = src.mileage_next;
    this->mileage_pass = src.mileage_pass;
    this->mileage_start = src.mileage_start;
    this->mileage_stop = src.mileage_stop;

    this->left_edge = src.left_edge;
    this->right_edge = src.right_edge;
    this->left_line = src.left_line;
    this->right_line = src.right_line;

    this->num_of_section = src.num_of_section;
    this->line = src.line;
    this->cur_section = src.cur_section;
    return *this;
}

void route_planning_m::route_clear()
{
    time_stamp = 0;
    mileage_next = 0;
    mileage_pass = 0;
    mileage_start = 0;
    mileage_stop = 0;

    left_edge.num_of_points = 0;
    left_edge.line.clear();

    right_edge.num_of_points = 0;
    right_edge.line.clear();

    left_line.num_of_points = 0 ;
    left_line.line.clear();

    right_line.num_of_points = 0;
    right_line.line.clear();

    num_of_section = 0;
    line.clear();

    cur_section = 0;

}

bool route_planning_m::check_point_in_lane_edge( const point_xys src, const int _begin, const int _end )
{
//    cout << "~~~~~~~~~~~ size = " << left_edge.num_of_points << ", x = " << left_edge.line[0].x << endl;
    if ( left_edge.num_of_points <= 0 || right_edge.num_of_points <= 0 ||
         left_edge.line.size() <= 0 || right_edge.line.size() <= 0 )
         return false;

    bool in_rt = false;
    for ( int i = _begin+2; i < _end; i += 2 )
    {
        /*构建路面矩形*/
        rect rt_lane;
        //front left_edge
        rt_lane.p1.xx = left_edge.line[i].x;
        rt_lane.p1.yy = left_edge.line[i].y;
        //front right_edge
        rt_lane.p2.xx = right_edge.line[i].x;
        rt_lane.p2.yy = right_edge.line[i].y;
        //back right_edge
        rt_lane.p3.xx = right_edge.line[i-2].x;
        rt_lane.p3.yy = right_edge.line[i-2].y;
        //back left_edge
        rt_lane.p4.xx = left_edge.line[i-2].x;
        rt_lane.p4.yy = left_edge.line[i-2].y;

        /*判断障碍物中心是否在这个矩形内*/
        point temp_p;
        temp_p.xx = src.x;
        temp_p.yy = src.y;
        in_rt = rt_lane.is_point_in_xx( temp_p );

        if ( in_rt )
        {
            return true;
            break;
        }

    }

    return false;

}

bool route_planning_m::check_rect_in_lane_edge( const rect rt, const int _begin, const int _end )
{
    if ( left_edge.num_of_points <= 0 || right_edge.num_of_points <= 0 ||
         left_edge.line.size() <= 0 || right_edge.line.size() <= 0 )
         return false;

    bool in_rt = false;
    for ( int i = _begin+2; i < _end; i += 2 )
    {
        /*构建路面矩形*/
        rect rt_lane;
        //front left_edge
        rt_lane.p1.xx = left_edge.line[i].x;
        rt_lane.p1.yy = left_edge.line[i].y;
        //front right_edge
        rt_lane.p2.xx = right_edge.line[i].x;
        rt_lane.p2.yy = right_edge.line[i].y;
        //back right_edge
        rt_lane.p3.xx = right_edge.line[i-2].x;
        rt_lane.p3.yy = right_edge.line[i-2].y;
        //back left_edge
        rt_lane.p4.xx = left_edge.line[i-2].x;
        rt_lane.p4.yy = left_edge.line[i-2].y;

        /*判断障碍物矩形框是否与路面矩形框有交叉*/
        rect temp_rt = rt;
        in_rt = cross_in_two_rect_xx( rt_lane, temp_rt );

        if ( in_rt )
        {
            return true;
            break;
        }

    }

    return false;

}

/**
* @class TrafficLights
* @brief 路口红绿灯信息
*/
TrafficLights::TrafficLights()
{
    light_status_ = 0;
    crossing_status_ = 0;
}


TrafficLights::~TrafficLights()
{

}

TrafficLights &TrafficLights::operator=(const TrafficLights &src)
{
    this->light_status_ = src.light_status_;
    this->crossing_status_ = src.crossing_status_;
    return *this;
}

