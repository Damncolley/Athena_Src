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
#ifndef _MAP_INTERFACE_
#define _MAP_INTERFACE_

#include "MapData.h"
namespace athena
{
namespace roadmap
{

class RoadMap
{
public:
    //存储地图数据
    OSMMap *osm_map_;///< osm地图
    //二期新增动态字段，csu/rsu/obu_planning在路径规划、重规划、每秒刷新时重新设置这些信息
    std::vector<light>    light_list_;             ///<红绿灯列表
    std::vector<limspeed> limspeed_list_;        ///<限速牌列表
    std::vector<block>    block_list_;             ///<施工标志列表
    std::vector<lane>     lane_list_;               ///<路径列表
    std::vector<cross_regulator> cross_list_;    ///<路口列表

    RoadMap(std::string filename);



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
    int64_t locate_point_on_lane_list(double lat, double lon, const std::vector<int64_t> &lane_list);

    /////////////////////////////////////////////////
    /// \brief 从osm地图中获取lane相关信息
    /// \param lane_id：输入lane_id信息
    /// \param lane：输出当前的车道
    /// \return RET_OK  0； RET_ERROR 1
    /////////////////////////////////////////////////
    int get_lane_from_map_by_id(int64_t lane_id, lane* route_lane);

    /////////////////////////////////////////////////
    /// \brief 获取车道的左边线
    /// \param lane_id：输入lane_id信息
    /// \param line：输出当前车道左边线的点序列
    /// \return RET_OK  0； RET_ERROR 1
    /////////////////////////////////////////////////
    int get_left_line_of_lane(int64_t lane_id, std::vector< point_with_id_t > line);

    /////////////////////////////////////////////////
    /// \brief 获取车道的右边线
    /// \param lane_id：输入lane_id信息
    /// \param line：输出当前车道右边线的点序列
    /// \return RET_OK  0； RET_ERROR 1
    /////////////////////////////////////////////////
    int get_right_line_of_lane(int64_t lane_id, std::vector< point_with_id_t > line);

    /////////////////////////////////////////////////
    /// \brief 修改拓扑图中对应边的权重
    /// \param lane_id：输入lane_id信息
    /// \param cost: 每个车道跟下个车道的连接权重,默认为当前车道的长度，增加量可以为负值
    /// \param int: forward == 0 表示更改当前车道与下一个车道的连接权重；forward == 1 表示更改当前车道与上一个车道的连接权重
    /// \return RET_OK  0； RET_ERROR 1
    /////////////////////////////////////////////////
    int add_graph_edge_weight(int64_t lane_id,  double cost, int forward = 0);

private:
    void get_flow_light_and_points_from_map();
};

}
}
#endif // _MAP_INTERFACE_

