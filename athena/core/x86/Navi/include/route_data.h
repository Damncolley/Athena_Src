/**
* @file       route_data.h
* @brief      导航模块的公用数据
* @details    包含对点、规划模块数据等描述
* @author     huanhuan
* @date       2018/7/16
* @version    v1.0
* @par Copyright (c):
*      武汉环宇智行科技有限公司
* @par History:
*   version: author, date, desc\n
*/
#ifndef _ROUTE_DATA_H_
#define _ROUTE_DATA_H_

#include "heading.h"
#include "math_util.h"
#include "config/Config.h"
#include "MapInterface.h"
#include "nad_retcode.h"
#include "nad_enum.h"


namespace athena
{
namespace route
{
//typedef boost::graph_traits<Graph>::edge_iterator EdgeIterator;
typedef std::pair<double,double> xy_point;
const double RADIANS_PER_LSB = 0.0174532925;


/**
* @brief coord_transfer,完成经纬度和高斯投影面xy之间坐标的转换。
* 包含原点的设定
*/
class coord_transfer
{
public:
    LocalGeographicCS cs;

    coord_transfer();
    coord_transfer(std::string filename);
    coord_transfer(double lat, double lon);
    ~coord_transfer();

    void set_origin(std::string filename);
    void set_origin(double lat, double lon);
private:
    Config configSettings;
    double origin_lat = 0;///<投影原点纬度.
    double origin_lon = 0;///<投影原点经度.


};


/**
 *  @brief 路径规划的公共结构
 *  地图上的关键点（但地图本身并不存关键点）
 */
class key_point
{
public:
    double     lon;  ///<经度
    double     lat;  ///<纬度
    double     yaw;  ///<角度
    int32_t    type; ///<关键点类型，枚举值KP_XXX，忽略则取值KP_UNKNOWN=0
};


/**
* @brief 投影坐标系下x,y点
*/
struct Point_m
{
    double x;
    double y;
};

/**
* @brief 形点边界线（专供planning使用）
* 约束可行驶区域，道路边界等
*/
class point_xys
{
public:
    int8_t     type; ///<点类型，依业务类型而定
    float      x;    ///<x坐标，单位m
    float      y;    ///<y坐标，单位m
};

/**
 * 路径下发消息
 * 中路中心上的点（专供motion使用）
 */
class point_m
{
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

/// 以左侧第一车道头指向的垂线截取的道路切片（专供motion使用）
class section_m
{
public:
    int8_t     num_of_lane;                ///<切片上的道路数量
    std::vector< point_m > lane;           ///<每个切片上的道路中点
};

/**
* @brief 形点边界线（专供planning使用）
* 约束可行驶区域，道路边界等
*/
class line_xys
{
public:
    int32_t    num_of_point;        ///<点的数量
    std::vector<point_xys> line;    ///<点集
};

/**
* @brief 规划出来的道路中线点
* 包含每个点的坐标、里程、所在车道的id等
*/
class center_point
{
public:
    double x;              ///<相对原点的x坐标，单位m，可和经纬度转化
    double y;              ///<相对原点的y坐标，单位m，可和经纬度转化
    int32_t index;         ///<在center_point_list中的下标
    double yaw;            ///<头指向
    double width;          ///<路宽
    double mileage;        ///<相对起点的里程，单位m
    int64_t lane_id;       ///<当前lane的id
    int8_t type[MAX_KP_TYPE]; ///<枚举值KP_XXX
    int32_t left_lane_num; ///<左边车道的数目
    int32_t right_lane_num;///<右边车道的数目
    std::string id;             ///<key_point专用，索引对象，例如light_id，无需索引的填""
    int32_t road_level;    ///<指定道路等级

public:
    center_point();    ///<构造函数
    double dist(center_point &cp);///<距离某个点的距离
};

///用于依据limspeed_id在vector<limspeed> limspeed_list中查找对应的limspeed
class LimspeedInLimspeedList {
public:
    LimspeedInLimspeedList(const std::string &id)
    {
        this->id = id;
    }
    bool operator () (const athena::roadmap::limspeed& limspeed_t) {
        return (limspeed_t.limspeed_id == id)?true:false;
    }
private:
    std::string id;
};


/**
* @brief 路径规划
* 包含导航规划所经过的车道序列以及关键点等
*/
class route_planning
{
public:
    int16_t    num_of_lane;            ///<路径节点数量
    std::vector< athena::roadmap::lane > lane_list; ///<路径节列列表
    int16_t    num_of_kp;              ///<关键点数量
    std::vector< center_point > key_point_list;///<关键点列表，不包含KP_NONE=-1的点，包含规划信息（所以没有使用key_point结构）
    int64_t    time_stamp;            ///<产生此路径规划的时间，gettimeofday获得的毫秒数
    std::string destination;          ///<目的地名称
    int32_t    route_reason;          ///<路径规划原因，参考枚举值：ROUTE_REASON_XXX，可用来区分初始规划/重规划、车道规划/道路规划
};


/**
* @brief 路径规划（专供planning使用）
* 包含导航规划所经过的车道序列以及关键点等
*/
class route_planning_m
{
public:
    ///<以车辆当前位置(cur_section)为原点，分段规划信息
    int64_t    time_stamp;     ///<产生此路径规划的时间，gettimeofday获得的毫秒数
    std::string destination;   ///<规划的目的地
    int32_t    route_reason;   ///<规划原因
    float      mileage_pass;   ///<相对车过去的里程，单位m，默认500m
    float      mileage_next;   ///<相对车未来的里程，单位m，默认1000m
    float      mileage_start;  ///<距离起点的里程，单位m
    float      mileage_stop;   ///<距离终点的里程，单位m
    ///<可行驶区域 left_edge;right_edge;需要在地图中指明
    line_xys left_edge;        ///<左边界线，超出此线可能撞马路牙子
    line_xys right_edge;       ///<右边界线，超出此线可能撞马路牙子
    line_xys left_line;        ///<最左车道的左边线，超出此线可能逆行
    line_xys right_line;       ///<最右车道的右边线，通常right_line和right_edge之间为停车带
    ///<中线：按车辆行驶方向的每个切片的中点
    int32_t    num_of_section;      ///<这段道路上的切片数量
    std::vector< section_m > line;  ///<切片列表
    int32_t    cur_section;         ///<规划时车在哪个切片上
};
#define MILEAGE_PASS   500       ///<每次切片下发已走里程，单位：m
#define MILEAGE_NEXT   1000      ///<每次切片下发未走里程，单位：m

class Route_Behavior
{
public:
    ///基于关键点的行为
    std::vector<center_point> center_line_;     ///<最左边车道的中心线 相当于道路级的规划 用于与道路级事件匹配
    std::vector<center_point> key_points_;      ///<从起点到终点的关键点，道路级事件
    std::vector<roadmap::limspeed> limspeed_list_;       ///限速牌列表
    std::vector<roadmap::lane> lanelist_;

    int32_t cur_center_line_index_;        ///<车辆当前在center_line[cur_center_line_index]附近
    int32_t cur_key_point_index_;          ///<车辆当前在key_points[cur_key_point_index]附近
    center_point cur_point_;               ///<在route中的当前点，数据来自center_line[cur_center_line_index]
    int32_t limspeed_value;        //限速值，1000表示没有限速牌，单位：km/h

    ///把center_line中的关键点保存到key_ponts中
    void get_keypoint_on_center_line();

    ///获得重规划是需要的key_point(只需要当前点、终点、NORMAL点，不要ets、障碍物、换道点)
    void get_keypoint_for_replanning(std::vector<center_point> &key_point_list);

    ///获得规划是需要的key_point(只需要起点、终点、NORMAL点，不要ets、障碍物、换道点)
    void get_keypoint_for_planning(std::vector<center_point> &key_point_list);

    ///删除可以动态绑定的关键点(只保留起点、终点、NORMAL点)
    void clear_active_keypoint_on_center_line();

    ///查找下一个关键点
    center_point *get_next_kp(int type1, int type2 = KP_NONE,
        int type3 = KP_NONE, int type4 = KP_NONE, int type5 = KP_NONE, int type6 = KP_NONE);

    ///查找上一个关键点
    center_point *get_pre_kp(int type1, int type2 = KP_NONE,
        int type3 = KP_NONE, int type4 = KP_NONE, int type5 = KP_NONE, int type6 = KP_NONE);

    ///从头开始全程查找关键点
    center_point *get_kp(int type1, int type2 = KP_NONE,
        int type3 = KP_NONE, int type4 = KP_NONE, int type5 = KP_NONE, int type6 = KP_NONE);

    ///查找以cur_point为原点，前后多少米的点，负数为前(向起点)，整数位后(向终点)
    center_point *get_cp(double diff);

    ///查找下一个停止点
    center_point *get_next_stop_kp(int right_of_way);

    ///查找下一个换道点，返回CL_DIRECTION_LEFT/CL_DIRECTION_RIGHT
    center_point *get_next_change_kp(int &direction);

    ///查找当前点是否禁止换道
    bool get_current_forbid_change();

    ///获得起点指针
    center_point *start_kp();

    ///获得终点指针
    center_point *stop_kp();

    ///获得到起点的距离(起点之前是负数，起点之后是正数，cp==NULL则用cur_point)
    double start_dist(center_point *cp = NULL);

    ///获得到终点的距离(终点之前是正数，终点之后负正数，cp==NULL则用cur_point)
    double stop_dist(center_point *cp = NULL);

    ///设置lanelist
    void set_lanelist(std::vector<roadmap::lane> &lanelist);

    ///将限速牌绑定到中心线
    int bind_limspeed_on_center_line(std::string limspeed_id, int64_t lane_id, double &dis_min);

    ///查找当前生效的限速点，同时修改了limspeed_value，返回生成速度的limspeed_id或lane_id
    int get_current_limspeed(std::string &id, double diff = 0.0);

    ///查找限速牌
    roadmap::limspeed *find_limspeed(std::string limspeed_id);
};
}
}

#endif // _ROUTE_DATA_H_
