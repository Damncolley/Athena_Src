/**
* @file       MapInterface.cpp
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
#include "MapInterface.h"

typedef boost::graph_traits<Graph>::edge_iterator EdgeIterator;
//typedef std::pair<double,double> xy_point;

using namespace athena;
using namespace roadmap;

RoadMap::RoadMap(std::string filename)
{
    osm_map_ = new LaneletMap(filename);
    if (osm_map_ == NULL)
    {
        LOG(ERROR) << "RoadMap::load_data(): LaneletMap(" << filename<< ") return NULL";
    }
    get_flow_light_and_points_from_map();
}


/////////////////////////////////////////////////
/// \brief 将定位点匹配到最近的lanelet上，返回地图里的lane_id，0表示匹配不到lane
/// \param lat：当前定位纬度
/// \param lon：当前定位经度
/// \param yaw：当前头指向
/// \return 返回定位车道的id，0表示匹配不到lane
/////////////////////////////////////////////////
int64_t RoadMap::locate_point_on_lanelet(double lat, double lon, double yaw)
{
    int64_t lane_id = 0;
    double dist_ = 500.0;
    double dist_tmp = 500.0;
    BoundingBox query_point( std::make_tuple(lat,lon,lat,lon) );       ///<搜索区域//搜索lanelet的区域
    point_with_id_t cur_pos = std::make_tuple(lat,lon,1);              ///<当前车辆所在的位置
    std::vector<lanelet_ptr_t> query_result = osm_map_->query(query_point);  ///<query_result中包含了检索中的lanelet

    for(lanelet_ptr_t lanelet:query_result)
    {
        double dist_left = 500.0;
        double dist_right = 500.0;

        //求解navipoint与每个lanelet的距离//与左边的边线
        std::vector< point_with_id_t > nodes_left(lanelet->nodes(LLet::LEFT)), nodes_right(lanelet->nodes(LLet::RIGHT));
        for(point_with_id_t node:nodes_left)
        {
            dist_tmp = dist(cur_pos,node);
            if(dist_tmp < dist_left)
            {
                dist_left = dist_tmp;
            }
        }

        //求解navipoint与每个lanelet的距离//与右边的边线
        for(point_with_id_t node:nodes_right)
        {
            dist_tmp=dist(cur_pos,node);
            if(dist_tmp<dist_right)
            {
                dist_right=dist_tmp;
            }
        }

        dist_tmp=dist_left+dist_right;

        if(dist_tmp<dist_)
        {
            dist_=dist_tmp;
            lane_id=lanelet->id();
        }
    }
    return lane_id;

}

///将定位点匹配到最近的lanelet上，返回lane_list中匹配上的lane id，0表示匹配不到lane
/////////////////////////////////////////////////
/// \brief 将定位点匹配到最近的lanelet上，返回地图里的lane_id，0表示匹配不到lane
/// \param lat：当前定位纬度
/// \param lon：当前定位经度
/// \param yaw：当前头指向
/// \return 返回定位车道的id，0表示匹配不到lane
/////////////////////////////////////////////////
int64_t RoadMap::locate_point_on_lane_list(double lat, double lon, const std::vector<int64_t>& lane_list)
{
    int64_t lane_id=0;
    double dist_=500.0;
    double dist_tmp=500.0;
    BoundingBox query_point( std::make_tuple(lat,lon,lat,lon) );
    point_with_id_t cur_pos = std::make_tuple(lat,lon,1);
    std::vector<lanelet_ptr_t> query_result;

    for(size_t i = 0; i < lane_list.size(); i++)
    {
        lanelet_ptr_t ll = osm_map_->lanelet_by_id(lane_list[i]);
        query_result.push_back(ll);
    }

    for(lanelet_ptr_t lanelet:query_result)
    {
        double dist_left=500.0;
        double dist_right=500.0;

        //求解navipoint与每个lanelet的距离//与左边的边线
        std::vector< point_with_id_t > nodes_left(lanelet->nodes(LLet::LEFT)), nodes_right(lanelet->nodes(LLet::RIGHT));
        for(point_with_id_t node:nodes_left)
        {
            dist_tmp=dist(cur_pos,node);
            if(dist_tmp<dist_left)
            {
                dist_left=dist_tmp;
            }
        }

        //求解navipoint与每个lanelet的距离//与右边的边线
        for(point_with_id_t node:nodes_right)
        {
            dist_tmp=dist(cur_pos,node);
            if(dist_tmp<dist_right)
            {
                dist_right=dist_tmp;
            }
        }

        dist_tmp=dist_left+dist_right;

        if(dist_tmp<dist_)
        {
            dist_=dist_tmp;
            lane_id=lanelet->id();
        }
    }
    return lane_id;
}

/////////////////////////////////////////////////
/// \brief 从osm地图中获取lane相关信息
/// \param lane_id：输入lane_id信息
/// \param nad_lane：输出当前的车道
/// \return RET_OK  0； RET_ERROR 1
/////////////////////////////////////////////////
int RoadMap::get_lane_from_map_by_id(int64_t lane_id, lane* route_lane)
{
    route_lane->lane_id = lane_id;

    //获取对应id的lanelet
    if (0 == lane_id)
    {
        return 1;
    }

    lanelet_ptr_t the_ll = osm_map_->lanelet_by_id(lane_id);

    //车道信息
    route_lane->cur_brother_lane.clear();
    route_lane->opp_brother_lane.clear();
    route_lane->change_lane.clear();

    std::map<int64_t, std::string> lanes_map;
    std::vector<int64_t> lanes_vec;

    //车道信息 lanes_vec
    std::string lanes = the_ll->attribute("lanes").as_string();
    {
        //读取车道信息
        std::vector<std::string> lane_str;
        boost::split(lane_str, lanes, boost::is_any_of(";.#|,"));
        int lane_num = lane_str.size();
        if(lane_num >= 1)
        {
            for(int i=0; i<lane_num; i++)
            {
                lanes_vec.push_back(boost::lexical_cast<int64_t>(lane_str[i]));
            }
        }
    }
    //获取降级属性
    std::string lanes_degrade = the_ll->attribute("road_level").as_string();
    route_lane->road_level = boost::lexical_cast<int32_t>(lanes_degrade);
    //bool opposite_lanes == true,存在逆向车道，else不存在逆向车道
    bool opposite_lanes = false;
    boost::iterator_range<std::string::iterator> split_pos1,split_pos2,lane_pos;
    {
        split_pos1 = boost::find_first(lanes, "|");
        split_pos2 = boost::find_first(lanes, "#");
        if(!split_pos1.empty() || !split_pos2.empty())
        {
            opposite_lanes = true;
        }
    }
    //车道信息 lanes_map
    if(opposite_lanes == true)
    {
        bool is_opposite_lane = true;
        for(int i = 0; i<lanes_vec.size(); i++)
        {
            std::string lane_str = boost::lexical_cast<std::string>(lanes_vec[i]);
            lane_pos = boost::find_first(lanes, lane_str);
            lanes_map[lanes_vec[i]] = *lane_pos.end();

            //opposite_lanes
            if(is_opposite_lane == true)
            {
                route_lane->opp_brother_lane.push_back(lanes_vec[i]);
            }
            else
            {
                //neighbor_lanes
                route_lane->cur_brother_lane.push_back(lanes_vec[i]);
            }

            if(lanes_map[lanes_vec[i]] == "#" || lanes_map[lanes_vec[i]] == "|")
            {
                is_opposite_lane = false;
            }

        }
    }
    else
    {
        for(int i=0; i<lanes_vec.size(); i++)
        {
            std::string lane_str = boost::lexical_cast<std::string>(lanes_vec[i]);
            lane_pos = boost::find_first(lanes, lane_str);
            lanes_map[lanes_vec[i]] = *(lane_pos.end()++);

            //neighbor_lanes
            route_lane->cur_brother_lane.push_back(lanes_vec[i]);
        }
    }

    //change_lane
    {

        std::map<int64_t, std::string>::iterator map_iter;

        auto pos= std::find(lanes_vec.cbegin(), lanes_vec.cend(), lane_id);
        int left_lane_num = std::distance(lanes_vec.cbegin(), pos);
        int left_index = left_lane_num;
        int right_index = left_lane_num;
        //forward,往左边搜索
        for(; left_index >= 0; --left_index)
        {
            map_iter = lanes_map.find(lanes_vec[left_index]);
            if (map_iter != lanes_map.end())
            {
                if((map_iter->second == ";" || map_iter->second == "#") && left_index!= left_lane_num)//left_index!= left_lane_num,排除自身干扰
                {
                    ++left_index;
                    break;
                }
            }

        }
        left_index = std::max(left_index,0);//最小从0开始
        //backward，往右边搜索
        for(; right_index < lanes_vec.size(); ++right_index)
        {
            map_iter = lanes_map.find(lanes_vec[right_index]);
            if (map_iter != lanes_map.end())
            {
                if(map_iter->second == ";" || map_iter->second == "#")
                {
                    ++right_index;
                    break;
                }
            }
        }

        for(; left_index != right_index; left_index++)
        {
            route_lane->change_lane.push_back(lanes_vec[left_index]);
        }

    }

    //获取车道长度
    route_lane->length = the_ll->length();

    //获取限速值
    route_lane->max_speed = the_ll->attribute("speed").as_double();


}

/////////////////////////////////////////////////
/// \brief 获取车道的左边线
/// \param lane_id：输入lane_id信息
/// \param line：输出当前车道左边线的点序列
/// \return RET_OK  0； RET_ERROR 1
/////////////////////////////////////////////////
int RoadMap::get_left_line_of_lane(int64_t lane_id, std::vector< point_with_id_t > line)
{
    lanelet_ptr_t lanelet = osm_map_->lanelet_by_id(lane_id);
    std::vector< point_with_id_t > nodes_left(lanelet->nodes(LLet::LEFT));
    return 0;
}

/////////////////////////////////////////////////
/// \brief 获取车道的右边线
/// \param lane_id：输入lane_id信息
/// \param line：输出当前车道右边线的点序列
/// \return RET_OK  0； RET_ERROR 1
/////////////////////////////////////////////////
int RoadMap::get_right_line_of_lane(int64_t lane_id, std::vector< point_with_id_t > line)
{
    lanelet_ptr_t lanelet = osm_map_->lanelet_by_id(lane_id);
    std::vector< point_with_id_t > nodes_left(lanelet->nodes(LLet::RIGHT));
    return 0;

}

/////////////////////////////////////////////////
/// \brief 修改拓扑图中对应边的权重
/// \param lane_id：输入lane_id信息
/// \param cost: 每个车道跟下个车道的连接权重,默认为当前车道的长度，增加量可以为负值
/// \param int: forward == 0 表示更改当前车道与下一个车道的连接权重；forward == 1 表示更改当前车道与上一个车道的连接权重
/// \return RET_OK  0； RET_ERROR 1
/////////////////////////////////////////////////
int RoadMap::add_graph_edge_weight(int64_t lane_id,  double cost, int forward)
{
    lane* route_lane = new lane();
    get_lane_from_map_by_id(lane_id, route_lane);
    Graph& G = osm_map_->get_graph();
    std::vector<int64_t>::iterator my_it = route_lane->cur_brother_lane.begin();
    for(; my_it != route_lane->cur_brother_lane.end(); ++my_it)
    {
        lanelet_ptr_t src = osm_map_->lanelet_by_id(*my_it);//待处理lanelet
        auto lls_around = osm_map_->query( src->bb() );
        for( const auto& other: lls_around )
        {
            if(src->fits_before(other))
            {
                std::pair<EdgeIterator, EdgeIterator> ei = edges(G);
                for (EdgeIterator eit = ei.first; eit!= ei.second; ++eit)
                {
                    EdgeInfo& ep = G[*eit]; // 可以读取和修改边的属性

                    if(ep.src == src->id() && ep.dest == other->id() && ep.block < 1)
                    {
                        ep.routing_cost += cost;
                        ep.block += cost;
                    }
                }
            }
        }
    }
    return 0;

}

void RoadMap::get_flow_light_and_points_from_map()
{
    for( const regulatory_element_ptr_t& regulatory: osm_map_->_regulatory_element )
    {
        if(regulatory->attribute("maneuver").as_string() == "traffic_light")
        {
            light _osm_light;

            _osm_light.id = regulatory->id();//id为地图中的id;
            _osm_light.mileage = regulatory -> attribute("mileage").as_double();

            auto stop = regulatory->members("stop");
            const auto& stop_p = stop.front();
            point_with_id_t stopP = boost::get<point_with_id_t>(stop_p);
            _osm_light.stop_point.lat = std::get<0>(stopP);
            _osm_light.stop_point.lon = std::get<1>(stopP);
            _osm_light.stop_point.id = std::get<2>(stopP);

            auto _exit = regulatory->members("exit");
            const auto& _exit_p = _exit.front();
            point_with_id_t _exitP = boost::get<point_with_id_t>(_exit_p);
            _osm_light.exit_point.lat = std::get<0>(_exitP);
            _osm_light.exit_point.lon = std::get<1>(_exitP);
            _osm_light.exit_point.id = std::get<2>(_exitP);

            auto _light = regulatory->members("light");
            const auto& _light_p = _light.front();
            point_with_id_t _lightP = boost::get<point_with_id_t>(_light_p);
            _osm_light.light_point.lat = std::get<0>(_lightP);
            _osm_light.light_point.lon = std::get<1>(_lightP);
            _osm_light.light_point.id = std::get<2>(_lightP);

            light_list_.push_back(_osm_light);
        }
    }

}

