/**
* @file       map_interface.h
* @brief      导航模块
* @details 1、对构建地图中的车道进行检查;2、地图上点间的集合属性，包括角度，距离等；3、地图的空间分区域检索检索；4、查询车道、车道的左右边线、以及边线上的点； 5、查询车道上的属性；6、查询道路标志等一些规则属性；7实现道路拓扑的路由
* @author     huanhuan
* @date       2018/7/16
* @version    v1.0
* @par Copyright (c):
*      武汉环宇智行科技有限公司
* @par History:
*   version: author, date, desc\n
*/
#ifndef _ROADMAP_H_
#define _ROADMAP_H_

namespace athena
{
namespace roadmap
{
#define NEAREST_LANE_LIST 30.0

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
    string limspeed_id;     //地图没有限速牌，id=lon|lat
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
    string block_id;     //地图没有施工标志，id=lon|lat
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
    string name;//路口名称
    int type;  //路口类型
    vector<light> flow_light_list_; //flow_light_list 车流的红绿灯
    //vector<map_point> points_;      //车流里的与红绿灯相关的点
};

class RoadMap
{
public:
    //二期新增动态字段，csu/rsu/obu_planning在路径规划、重规划、每秒刷新时重新设置这些信息
    vector<light>    light_list_;             ///<红绿灯列表
    vector<limspeed> limspeed_list_;        ///<限速牌列表
    vector<block>    block_list_;             ///<施工标志列表
    vector<lane>     lane_list_;               ///<路径列表
    vector<cross_regulator> cross_list_;    ///<路口列表

    /////////////////////////////////////////////////
    /// \brief 读入RoadMap地图
    /// \param file_name：地图读入的文件名
    /// \return RET_OK  0； RET_ERROR 1
    /////////////////////////////////////////////////
    int read_map(string file_name);

    void get_flow_light_and_points_from_osm(RoadMap *osm_map);
    /////////////////////////////////////////////////
    /// \brief 将定位点匹配到最近的lanelet上，返回地图里的lane_id，0表示匹配不到lane
    /// \param lat：当前定位纬度
    /// \param lon：当前定位经度
    /// \param yaw：当前头指向
    /// \return 返回定位车道的id，0表示匹配不到lane
    /////////////////////////////////////////////////
    int64_t locate_point_on_lanelet(double lat, double lon, double yaw = -1);

    ///将定位点匹配到最近的lanelet上，返回lane_list中匹配上的lane id，0表示匹配不到lane
    /////////////////////////////////////////////////
    /// \brief 将定位点匹配到最近的lanelet上，返回地图里的lane_id，0表示匹配不到lane
    /// \param lat：当前定位纬度
    /// \param lon：当前定位经度
    /// \param yaw：当前头指向
    /// \return 返回定位车道的id，0表示匹配不到lane
    /////////////////////////////////////////////////
    int64_t locate_point_on_lane_list(double lat, double lon, const vector<int64_t> &lane_list);

    /////////////////////////////////////////////////
    /// \brief 从osm地图中获取lane相关信息
    /// \param lane_id：输入lane_id信息
    /// \param lane：输出当前的车道
    /////////////////////////////////////////////////
    void get_lane_from_map_by_id(int64_t lane_id, lane* route_lane);

private:
    RoadMap *osm_map_;///< osm地图
};

}
}
#endif // _ROADMAP_H_
