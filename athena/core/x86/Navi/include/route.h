/**
* @file       route.h
* @brief      导航模块
* @details 1、实现车道级的导航;2、输入经纬度或者xy进行相应的定位
* @author     huanhuan
* @date       2018/7/16
* @version    v1.0
* @par Copyright (c):
*      武汉环宇智行科技有限公司
* @par History:
*   version: author, date, desc\n
*/

#ifndef _ROUTE_H
#define _ROUTE_H

#include <GL/glu.h>
#include <GL/glut.h>
#include "route_data.h"
#include "nad_function.h"

using namespace athena;
//using namespace route;

namespace athena
{
namespace route
{

/**
* @brief 规划基础类，1、车道级的规划以及相应定位功能
*/
class RouteBase
{
public:
    roadmap::RoadMap *map_;     ///<地图
    route::coord_transfer transfer_; ///<坐标转换

    std::string destination_;                   ///<目的地名称
    int32_t route_reason_;                 ///<路径规划原因，参考枚举值：ROUTE_REASON_XXX
    int64_t time_stamp_;                   ///<产生此路径规划的时间，gettimeofday获得的毫秒数
    //std::vector<roadmap::lane> lane_list_;               ///<路径列表

    //基于关键点的行为
    Route_Behavior route_behavior_;             ///行为点
    //std::vector<center_point> center_line_;     ///<最左边车道的中心线 相当于道路级的规划 用于与道路级事件匹配
    //std::vector<route::center_point> key_points_;      ///<从起点到终点的关键点，道路级事件

    //int32_t cur_center_line_index_;        ///<车辆当前在center_line[cur_center_line_index]附近
    //int32_t cur_key_point_index_;          ///<车辆当前在key_points[cur_key_point_index]附近
    //route::center_point cur_point_;               ///<在route中的当前点，数据来自center_line[cur_center_line_index]


    route_planning_m route_motion_info_;
    route_planning_m route_motion_info_draw_;

    std::map<int64_t,Point_m> blane_lborder_map_;///<兄弟车道左边界，点序列，切片相关
    std::map<int64_t,Point_m> blane_rborder_map_;///<兄弟车道右边界，点序列，切片相关
    std::map<int64_t,Point_m> changelane_lborder_map_; ///<对向车道左边界，点序列，切片相关
    //vector <int64_t> sec_vec;
    int32_t cur_sec_index_;                  ///<当前位置在section中的定位下标
    int32_t cur_lane_index_;                 ///<在切片中从左往右lane的下标，依次为0123.
    double  mileage_section_acc_;            ///<lane_to_section_line的里程计数器
    double  mileage_section_send_;           ///<每次下发切片的累计里程；
    int32_t ending_point_alarm_cnt_;

public:
    RouteBase();
    RouteBase(std::string filename);

public:
    /////////////////////////////////////////////////
    /// \brief 执行路径规划，成功返回RET_OK，并填充除了ets外的全部信息
    ///
    /// \param key_point_list：途经点信息
    /// \param route_reason：路径规划的原因
    /// \param destination：路径规划目的地名称
    /// \param lat：当前定位纬度
    /// \param lon：当前定位经度
    /// \param yaw：当前头指向
    /// \param begin：
    /// \return
    ///
    /////////////////////////////////////////////////
    int routing(std::vector<key_point> &key_point_list, int32_t route_reason, std::string destination, double lat, double lon, double yaw = -1, size_t begin = 0);

    /////////////////////////////////////////////////
    /// \brief 定位cur_center_line_index在center_line中的位置 //实时定位在center_line中的位置，方便后续使用
    ///
    /// \param lat：当前定位纬度
    /// \param lon：当前定位经度
    /// \param yaw：当前头指向
    /// \return RET_OK:0 , RET_ERROR 1, RET_ROUTE_START_ERR  16 ,RET_ROUTE_END_ERR    17
    ///
    /////////////////////////////////////////////////
    void locate_on_lane(double lat, double lon, double yaw = -1);

    /////////////////////////////////////////////////
    /// \brief 定位当前位置在那个切片哪个车道上
    ///
    /// \param lat：当前定位纬度
    /// \param lon：当前定位经度
    /// \return  RET_OK:0 , RET_ERROR 1
    ///
    /////////////////////////////////////////////////
    void locate_position_on_section_ll(double lat, double lon);
    /////////////////////////////////////////////////
    /// \brief 定位当前位置在那个切片哪个车道上
    ///
    /// \param x：当前定位纬度
    /// \param y：当前定位经度
    /// \return  RET_OK:0 , RET_ERROR 1
    ///
    /////////////////////////////////////////////////
    void locate_position_on_section__xy(double x, double y);

    /////////////////////////////////////////////////
    /// \brief 将lane_list转化为高速道中心线
    /// \return
    /// 函数最终会填写 route_motion_info，blane_lborder_map，blane_rborder_map，changelane_lborder_map
    /////////////////////////////////////////////////
    void lane_to_center_line(double lat = -1.0, double lon = -1.0);

    /////////////////////////////////////////////////
    /// \brief 将车道lane_list转换为切片的序列
    /// \return
    /// 函数最终会填写 route_motion_info，blane_lborder_map，blane_rborder_map，changelane_lborder_map
    /////////////////////////////////////////////////
    void lane_to_section_line(double lat = -1.0, double lon = -1.0);

    ////////////////////////////////////////////////
    /// \brief 清空route对象中所有记录
    /// \return
    ///清理对象中的变量
    /////////////////////////////////////////////////
    void clear();

    ////////////////////////////////////////////////
    /// \brief 在中心线上进行匹配
    /// \return RET_OK:0 , RET_ERROR 1
    /// \param lat：当前位置的纬度
    /// \param lon：当前位置的经度
    ///在中心线上搜索点，返回:RET_OK=存在<min_ok的点，RET_NOT_SAME=存在>=min_ok的点，RET_NOT_EXIST=无点
    ///例如min_ok=3表示: [首次距离<3米, 其后>3米]的区间内最小的点
    /////////////////////////////////////////////////
    int match_center_line_ll(double lat, double lon, int &index, double min_ok, double range, int64_t lane_id, double &dis_min);

    ////////////////////////////////////////////////
    /// \brief 在中心线上进行匹配
    /// \return RET_OK:0 , RET_ERROR 1
    /// \param x：高斯投影之后的x坐标
    /// \param y：高斯投影之后的y坐标
    ///在中心线上搜索点，返回:RET_OK=存在<min_ok的点，RET_NOT_SAME=存在>=min_ok的点，RET_NOT_EXIST=无点
    ///例如min_ok=3表示: [首次距离<3米, 其后>3米]的区间内最小的点
    ///匹配后的结果填入到
    /////////////////////////////////////////////////
    int match_center_line_xy(double x, double y, int &index, double min_ok, double range, int64_t lane_id, double &dis_min);

    ////////////////////////////////////////////////
    /// \brief 在切片序列上进行匹配
    /// \return RET_OK:0 , RET_ERROR 1
    /// \param lat：当前位置的纬度
    /// \param lon：当前位置的经度
    ///在中心线上搜索点，返回:RET_OK=存在<min_ok的点，RET_NOT_SAME=存在>=min_ok的点，RET_NOT_EXIST=无点
    ///例如min_ok=3表示: [首次距离<3米, 其后>3米]的区间内最小的点
    ///匹配后的结果填入到
    /////////////////////////////////////////////////
    int match_section_line_ll(double lat,double lon,int &sec_index,int &lane_index, double min_ok, double range, double &dis_min);

    ////////////////////////////////////////////////
    /// \brief 在切片序列上进行匹配
    /// \return RET_OK:0 , RET_ERROR 1
    /// \param x：高斯投影之后的x坐标
    /// \param y：高斯投影之后的y坐标
    ///在中心线上搜索点，返回:RET_OK=存在<min_ok的点，RET_NOT_SAME=存在>=min_ok的点，RET_NOT_EXIST=无点
    ///例如min_ok=3表示: [首次距离<3米, 其后>3米]的区间内最小的点
    ///匹配后的结果填入到
    /////////////////////////////////////////////////
    int match_section_line_xy(double x,double y,int &sec_index,int &lane_index, double min_ok, double range, double &dis_min);

    void route_section_clear(route_planning_m &route_sec);

    int get_op_lane_size(double cur_lat,double cur_lon,double cur_yaw);

    int get_op_lane_size(int64_t lane_id);

    ///返回当前左边或者右边的可变道车道数目
    int get_change_lane(double cur_lat,double cur_lon,double cur_yaw,int turn);

    int get_change_lane(int64_t lane_id,int turn);

    ///把中心线的某个点修改为关键点，传入xy或ll可以判断在哪个车道，成功返回RET_OK
    int bind_key_point(int &index, int type, std::string id, bool *insert_flag = NULL);
    int bind_key_point_xy(int &index, int type, std::string id, double x, double y);
    int bind_key_point_ll(int &index, int type, std::string id, double lat, double lon);

    ///跟显示有关的函数
    ///用opengl画出nad_lane和centrol_line以及当前定位的点
    void draw();
    void draw_part_line();
    void draw_part_section_line();
    ///使用opengl画lanelet，show的私有函数
    void draw_lanelet(lanelet_ptr_t& ll_draw);

private:
    ///获取当前时间（毫秒）
    int64_t get_current_route_time();

};
}
}
#endif
