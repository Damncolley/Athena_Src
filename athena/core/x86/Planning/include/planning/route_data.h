/**
 * @file
 *
 * @brief 导航模块的公用数据
 * 包含对点、规划模块数据等描述，地图切片、路口红绿灯、停车泊车点等信息
 */


#ifndef _ROUTE_DATA_H
#define _ROUTE_DATA_H

#include <stdint.h>
#include <string>
#include <vector>

#include "common/rect.h"

/**
* @class point_xys
* @brief 形点边界线（专供planning使用）
* 约束可行驶区域，道路边界等
*/
class point_xys
{
public:
    /**
    * @brief 构造函数
    */
    point_xys();
    /**
    * @brief 析构函数
    */
    ~point_xys();

    /**
    * @brief 对等于号=进行重载，point_xys类的等号操作符。
    * @param src 输入量：原始输入点。
    * @return 被赋值的point_xys信息。。
    */
    point_xys &operator=(const point_xys& src);

public:
    int8_t     type; ///<点类型，依业务类型而定
    float      x;    ///<x坐标，单位m
    float      y;    ///<y坐标，单位m
};

/**
* @class point_m
* @brief 路径下发消息
* 中路中心上的点（专供motion使用）
*/
class point_m
{
public:
    /**
    * @brief 构造函数
    */
    point_m();
    /**
    * @brief 析构函数
    */
    ~point_m();

    /**
    * @brief 对等于号=进行重载，point_m类的等号操作符。
    * @param src 输入量：原始输入点。
    * @return 被赋值的point_m信息。。
    */
    point_m &operator=(const point_m& src);

public:
    int8_t     type;     ///<点类型: KP_NONE=禁行点, KP_NORMAL=一般点, KP_CHANGE_LANE_LEFT=向左换道点, KP_CHANGE_LANE_RIGHT=向右换道点
    float      x;        ///<x坐标，单位m
    float      y;        ///<y坐标，单位m
    float      yaw;      ///<道路头指向(相对正北的夹角)
    float      k;        ///<道路曲率，单位deg/m
    float      mileage;  ///<相对起点的里程，单位m
    float      width;    ///<道路宽度，单位m
    int8_t     sug_speed;///<推荐速度，单位km/h
};

/**
* @class section_m
* @brief 以左侧第一车道头指向的垂线截取的道路切片（专供motion使用）。
*/
class section_m
{
public:
    /**
    * @brief 构造函数
    */
    section_m();
    /**
    * @brief 析构函数
    */
    ~section_m();

    /**
    * @brief 对等于号=进行重载，section_m类的等号操作符。
    * @param src 输入量：原始输入的section_m信息。
    * @return 被赋值的section_m信息。。
    */
    section_m &operator=(const section_m& src);

public:
    int8_t     num_of_lane;                ///<切片上的道路数量
    std::vector< point_m > lane;           ///<每个切片上的道路中点
};

/**
* @class line_xys
* @brief 形点边界线（专供planning使用）
* 约束可行驶区域，道路边界等
*/
class line_xys
{
public:
    /**
    * @brief 构造函数
    */
    line_xys();
    /**
    * @brief 析构函数
    */
    ~line_xys();

    /**
    * @brief 对等于号=进行重载，line_xys类的等号操作符。
    * @param src 输入量：原始输入的line_xys信息。
    * @return 被赋值的line_xys信息。
    */
    line_xys& operator= (const line_xys& src);

public:
    int32_t    num_of_points;        ///<点的数量
    std::vector<point_xys> line;     ///<点集
};


/**
* @class route_planning_m
* @brief 路径规划（专供planning使用）
* 包含导航规划所经过的车道序列以及关键点等
*/
class route_planning_m
{
public:
    /**
    * @brief 构造函数
    */
    route_planning_m();
    /**
    * @brief 析构函数
    */
    ~route_planning_m();

    /**
    * @brief 对等于号=进行重载，route_planning_m类的等号操作符。
    * @param src 输入量：原始输入的route信息。
    * @return 被赋值的route信息。。
    */
    route_planning_m &operator=(const route_planning_m& src);

    /**
    * @brief 清零所有属性
    */
    void route_clear();

    /**
    * @brief 判断一个点是否在可行驶区域内（两条边界线之间）。
    * @param src 输入量：待判断的点
    * @param _begin 输入量：边界的起始位置
    * @param _end 输入量：边界的终止位置
    * @return 返回1表示在边界线之内，0表示不在。
    */
    bool check_point_in_lane_edge( const point_xys src, const int _begin, const int _end );

    /**
    * @brief 判断一个矩形框是否在可行驶区域内（矩形框任意一个顶点在两条边界线之间）。
    * @param rt 输入量：待判断的矩形框
    * @param _begin 输入量：边界的起始位置
    * @param _end 输入量：边界的终止位置
    * @return 返回1表示在边界线之内，0表示不在。
    */
    bool check_rect_in_lane_edge( const rect rt, const int _begin, const int _end );

public:
    int32_t    replan_flag;    ///<重规划标志位
    //以车辆当前位置(cur_section)为原点，分段规划信息
    int64_t    time_stamp;     ///<产生此路径规划的时间，gettimeofday获得的毫秒数
    std::string destination;   ///<规划的目的地
    int32_t    route_reason;   ///<规划原因
    float      mileage_pass;   ///<相对车过去的里程，单位m，默认500m
    float      mileage_next;   ///<相对车未来的里程，单位m，默认1000m
    float      mileage_start;  ///<距离起点的里程，单位m
    float      mileage_stop;   ///<距离终点的里程，单位m
    //可行驶区域 left_edge;right_edge;需要在地图中指明
    line_xys left_edge;        ///<左边界线，超出此线可能撞马路牙子
    line_xys right_edge;       ///<右边界线，超出此线可能撞马路牙子
    line_xys left_line;        ///<最左车道的左边线，超出此线可能逆行
    line_xys right_line;       ///<最右车道的右边线，通常right_line和right_edge之间为停车带
    //中线：按车辆行驶方向的每个切片的中点
    int32_t    num_of_section;      ///<这段道路上的切片数量
    std::vector< section_m > line;  ///<切片列表
    int32_t    cur_section;         ///<规划时车在哪个切片上
};

/**
* @class TrafficLights
* @brief 路口红绿灯信息
*/
class TrafficLights
{
public:
    /**
    * @brief 构造函数
    */
    TrafficLights();
    /**
    * @brief 析构函数
    */
    ~TrafficLights();

    /**
    * @brief 对等于号=进行重载，TrafficLights类的等号操作符。
    * @param src 输入量：原始输入的红绿灯信息。
    * @return 被赋值的红绿灯信息。。
    */
    TrafficLights &operator=(const TrafficLights& src);

public:
    int64_t    light_status_;            ///<红绿灯状态，0：无效值，1：绿，2：黄，3：红

    int8_t     crossing_status_;         ///<路口状态。0：非路口，1：路口
};


#endif //_ROUTE_DATA_H

