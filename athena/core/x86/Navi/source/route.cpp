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

#include "route.h"
#include <time.h>
#include <sys/time.h>
using namespace athena;
using namespace route;



/////////////////////////////////////////////////
/// \brief 初始化函数
/// \return
/////////////////////////////////////////////////
RouteBase::RouteBase()
{
    clear();
    route_section_clear(route_motion_info_);  // std::map<int64_t,Point_m> changelane_lborder_map_; ///<对向车道左边界，点序列，切片相关
    route_section_clear(route_motion_info_draw_);
    blane_rborder_map_.clear();
    blane_lborder_map_.clear();
    changelane_lborder_map_.clear();

}

/////////////////////////////////////////////////
/// \brief 初始化函数
/// \param 地图路径名称，加载相应地图
/// \return
/////////////////////////////////////////////////
RouteBase::RouteBase(std::string file_name)
{
    clear();
    this->map_ = new roadmap::RoadMap(file_name);

    route_section_clear(route_motion_info_);
    route_section_clear(route_motion_info_draw_);
    blane_rborder_map_.clear();
    blane_lborder_map_.clear();
    changelane_lborder_map_.clear();

}
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
int RouteBase::routing(std::vector<key_point> &key_point_list, int32_t route_reason, std::string destination, double lat, double lon, double yaw, size_t begin)
{
    //基本参数
    clear();
    time_stamp_ = get_current_route_time();
    this->route_reason_ = route_reason;
    this->destination_ = destination;
    LOG(WARNING) << "planning: key=" << key_point_list.size() << ", dest=" << destination;
    //存储keypoint对应的lane
    roadmap::lane* route_lane = new roadmap::lane();
    std::vector<int64_t> lanelet_id_vec;
    for(unsigned int i = 0; i < key_point_list.size(); i++)
    {
        key_point kp = key_point_list[i];
        std::cout << "lon:" << kp.lon << "  lat:" << kp.lat << "  type:" << kp.type << "  yaw:" << yaw << "  kp.yaw:" << kp.yaw << std::endl;
        int64_t lane_id;
        if( i == 0 )//起点规划时候考虑惯导头指向
        {
            lane_id = map_->locate_point_on_lanelet(key_point_list[i].lat, key_point_list[i].lon, yaw);
        }
        else
        {
            lane_id = map_->locate_point_on_lanelet(key_point_list[i].lat, key_point_list[i].lon, key_point_list[i].yaw);
        }
        if (lane_id == 0)
        {
            std::cout << "\e[0;32m" << "route::RouteBase::planning(...) : key_point_list matching 找不到对应的lanelet,id == 0"<<"\e[0m" <<std::endl;
            return RET_NOT_EXIST;
        }

        //替换成高速道
        map_->get_lane_from_map_by_id(lane_id, route_lane);
        if (route_lane->cur_brother_lane.size() > 0)
        {
            lane_id = route_lane->cur_brother_lane[0];
        }
        std::cout << "\e[0;32m" << "route::RouteBase::planning(...) : key_point_list matching lane_id=" << lane_id
                  << ", lat=" << key_point_list[i].lat << ", lon=" << key_point_list[i].lon <<"\e[0m" << std::endl;
        lanelet_id_vec.push_back(lane_id);
    }
    std::cout << "lanelet_id_vec id size:" << lanelet_id_vec.size() << std::endl;

    //计算最短路径：shortest lanelet;
    std::vector< lanelet_ptr_t > _shortest_path_tmp;//每段每段的最短路径
    std::vector< lanelet_ptr_t > _shortest_path;
    const lanelet_ptr_t& Begin = map_->osm_map_->lanelet_by_id(lanelet_id_vec[0]); //起始点
    _shortest_path.push_back(Begin);
    for(unsigned int i = 0; i < lanelet_id_vec.size() - 1; i++)
    {
        const lanelet_ptr_t& start = map_->osm_map_->lanelet_by_id(lanelet_id_vec[i]);
        const lanelet_ptr_t& dest = map_->osm_map_->lanelet_by_id(lanelet_id_vec[i + 1]);
        try
        {
            _shortest_path_tmp = map_->osm_map_->shortest_path(start, dest);
        }
        catch(NoPath)
        {
            std::cout << "\e[0;32m" <<"lanelet_ptr_t& start : " << lanelet_id_vec[i] << "\e[0m" << std::endl;
            std::cout << "\e[0;32m" <<"lanelet_ptr_t& dest : " << lanelet_id_vec[i + 1] << "\e[0m" << std::endl;
            std::cout << "route::RouteBase::planning(...) : 找不到最短路径！" << start->id() << "----->" << dest->id() << std::endl;
            return RET_ROUTE_NO_RESULT;
        }
        _shortest_path.insert(_shortest_path.end(), _shortest_path_tmp.begin() + 1, _shortest_path_tmp.end());
    }

    //在终点之后:如果有顺联的lanelet，再加一段lanelet
    auto lls_around =  map_->osm_map_->query(_shortest_path.back()->bb());
    for( const auto& other: lls_around )
    {
        if(_shortest_path.back()->fits_before(other))
        {
            _shortest_path.push_back(other);
            break;
        }
    }

    //转换为lane
    for(lanelet_ptr_t lanelet : _shortest_path)
    {
        map_->get_lane_from_map_by_id(lanelet->id(), route_lane);
        map_->lane_list_.push_back(*route_lane);
    }

    //设置behavior的lanelist
    route_behavior_.set_lanelist(map_->lane_list_);

    //保存关键点，注意lane_to_center_line_映射关键点到center_line_上，坐标可能有微小变化
    for(size_t i = begin; i < key_point_list.size(); i++)
    {
        center_point kp;
        transfer_.cs.ll2xy(key_point_list[i].lat, key_point_list[i].lon, kp.x, kp.y);
        kp.type[KP_NORMAL] = 1;
        kp.lane_id = lanelet_id_vec[i];
        if (i == begin)
        {
            kp.type[KP_ROUTE_START] = 1;
        }
        if (i + 1 == key_point_list.size())
        {
            kp.type[KP_ROUTE_STOP] = 1;
        }
        route_behavior_.key_points_.push_back(kp);
    }

    //把map_->lane_list_转化为center_line_，再把key_points映射到center_line_
    lane_to_center_line();

    //将车道map_->lane_list_转换为切片的序列
    lane_to_section_line(lat, lon);

    //重新定位当前坐标
    locate_on_lane(lat, lon);

    delete route_lane;
    return RET_OK;
}

/////////////////////////////////////////////////
/// \brief 定位cur_center_line_index_在center_line中的位置 //实时定位在center_line中的位置，方便后续使用
///
/// \param lat：当前定位纬度
/// \param lon：当前定位经度
/// \param yaw：当前头指向
/// \return RET_OK:0 , RET_ERROR 1, RET_ROUTE_START_ERR  16 ,RET_ROUTE_END_ERR    17
///
/////////////////////////////////////////////////
void RouteBase::locate_on_lane(double lat, double lon, double yaw)
{
    if (map_->lane_list_.size() == 0)
    {
        return;
    }

    //搜索目前车位置，采用递增后即退出的算法，比较交叉口定位到另一个车道
    int32_t old_index = route_behavior_.cur_center_line_index_;
    route_behavior_.cur_center_line_index_ = std::max(0, route_behavior_.cur_center_line_index_ - 10);
    double dis_min = 0;

    match_center_line_ll(lat, lon, route_behavior_.cur_center_line_index_, 5.0, 25.0, 0, dis_min);
    //如果误差很大则退出judge_line
    if(dis_min > 30) return ;

#ifdef _NAD_RSU_PLANNING_
    if (old_index > 0)
    {
        double m = center_line[cur_center_line_index].dist(center_line[old_index]);
        if (m < -5.0 || m > 20.0)
        {
            LOG(ERROR) << "!!!!judge_lane异常: " << old_index << "->" << cur_center_line_index;
            save_to_osm(NULL, lat, lon);
        }
    }
#endif

    //获得当前center_point
    center_point &cp = route_behavior_.center_line_[route_behavior_.cur_center_line_index_];

    //获得当前所在的lane
    roadmap::lane* route_lane = new roadmap::lane();
    map_->get_lane_from_map_by_id(cp.lane_id, route_lane);
    int64_t lane_id = map_->locate_point_on_lane_list(lat, lon, route_lane->cur_brother_lane);

    //处理lanelet匹配失效的情况;如果失效则沿用之前的cur_center_line_index_
    if (lane_id > 0)
    {
        cp.lane_id = lane_id;
    }
    else
    {
        //如果匹配错误,则沿用上一次的定位点
        LOG(ERROR) << "!!!!locate_lane无lane: " << route_behavior_.cur_center_line_index_ << "->" << old_index;
       route_behavior_.cur_center_line_index_ = old_index;
        //save_to_osm(NULL, lat, lon);
        return;
    }

    //判断左右车道数
    auto pos = std::find(route_lane->cur_brother_lane.cbegin(), route_lane->cur_brother_lane.cend(), cp.lane_id);
    cp.left_lane_num = std::distance(route_lane->cur_brother_lane.cbegin(), pos);
    cp.right_lane_num = route_lane->cur_brother_lane.size() - cp.left_lane_num - 1;
    //清空原有的动态关键点，并把cur添加到关键点(其他动态关键点没有添加，需要重新绑定)
    route_behavior_.clear_active_keypoint_on_center_line();
    bind_key_point(cp.index, KP_CURRENT, "");
    route_behavior_.get_keypoint_on_center_line();

    //保存到当前点
    route_behavior_.cur_point_ = cp;

    delete route_lane;
}

/////////////////////////////////////////////////
/// \brief 定位当前位置在那个切片哪个车道上
///
/// \param lat：当前定位纬度
/// \param lon：当前定位经度
/// \return  RET_OK:0 , RET_ERROR 1
///
/////////////////////////////////////////////////
void RouteBase::locate_position_on_section_ll(double lat, double lon)
{
    double x, y;
    transfer_.cs.ll2xy(lat, lon, x, y);

    return locate_position_on_section__xy(x, y);
}


/////////////////////////////////////////////////
/// \brief 定位当前位置在那个切片哪个车道上
///
/// \param x：当前定位纬度
/// \param y：当前定位经度
/// \return  RET_OK:0 , RET_ERROR 1
///
/////////////////////////////////////////////////
void RouteBase::locate_position_on_section__xy(double x, double y)
{
    if(route_motion_info_.line.size() == 0)
    {
        return;
    }

    //搜索目前车位置，采用递增后即退出的算法，比较交叉口定位到另一个车道
    int32_t old_index = cur_sec_index_;
    cur_sec_index_ = std::max(0, cur_sec_index_ - 10);

    double dis_min = 0;

    //match_section_line_ll(x, y, cur_sec_index_, cur_lane_index_, 10.0, 30.0, dis_min);
    match_section_line_xy(x, y, cur_sec_index_, cur_lane_index_, 10.0, 30.0, dis_min);

    //如果误差很大则退出get_position_by_section_ll
    if(dis_min > 30) return ;
}



/////////////////////////////////////////////////
/// \brief 将lane_list转化为高速道中心线
/// \return
/// 函数最终会填写 route_motion_info，blane_lborder_map，blane_rborder_map，changelane_lborder_map
/////////////////////////////////////////////////
void RouteBase::lane_to_center_line(double cur_lat,double cur_lon)
{
    //入口判断
    if(map_->lane_list_.size() == 0)
    {
        clear();
        std::cout << "lane_list_to_center_line: lane_list size == 0" << std::endl;
        return;
    }
    route_behavior_.center_line_.clear();
    //在兄弟lane中找出高速车道，每组brother_lane只取其中的最左边的一条lane;
    roadmap::lane* route_lane = new roadmap::lane();
    //计数点，用来计算point的index
    int32_t point_num = 0;
    double lanelet_width = 3.5;

    //临时存储高速道
    std::vector<center_point> center_line_tmp;
    std::vector<roadmap::lane> left_lane_list;

    //搜索最左边车道
    for( int lane_index = 0; lane_index < (int)map_->lane_list_.size(); lane_index++)
    {
        lanelet_ptr_t left_lanelet;//最左边车道

        if( lane_index < (int)map_->lane_list_.size()-1)
        {
            for( int i = lane_index ; i < (int)map_->lane_list_.size()-1;  )
            {
                lanelet_ptr_t lanelet_1 = map_->osm_map_->lanelet_by_id(map_->lane_list_[i].cur_brother_lane.front());//从lane中得到lanelet;
                lanelet_ptr_t lanelet_2 = map_->osm_map_->lanelet_by_id(map_->lane_list_[i+1].cur_brother_lane.front());//从lane中得到lanelet,处理前后两个lanelet相邻的情况;

                //判断两段车道是否为兄弟lane,如果为兄弟lane则miss掉重复部分
                if(lanelet_1->id() == lanelet_2->id())
                {
                    i++;
                    lane_index++;
                }
                else
                {
                    left_lanelet = lanelet_1;
                    break;//跳出循环
                }
            }
        }
        else if( lane_index == (int)map_->lane_list_.size()-1)
        {
            left_lanelet = map_->osm_map_->lanelet_by_id(map_->lane_list_[lane_index].cur_brother_lane.front());//从lane中得到lanelet;
        }

        if(left_lanelet != NULL && left_lanelet->id() > 0)
        {
            //将道路属性加到route_lane的road_level
            map_->get_lane_from_map_by_id(left_lanelet->id(),route_lane);
            left_lane_list.push_back(*route_lane);
        }
    }

    std::cout << "\e[0;32m" << "lane_to_center_line-结束"  << "\e[0m" << std::endl;

    //输出高速道的lanelet
    for(unsigned int i =0; i < left_lane_list.size(); i++)
    {
        //std::cout << "\e[0;32m" << "gaosu:  left_lane = "  <<left_lane_list[i].lane_id<<"\e[0m" << std::endl;
        //左右车道数
        int lane_num_left = 0;
        int lane_num_right = left_lane_list[i].cur_brother_lane.size() - 1;

        //如果高速道存在跳变；则断掉10m; motion_planner 会进行拟合
        bool jump_flag = false;
        if(i > 0)
        {
            lanelet_ptr_t lanelet_1 = map_->osm_map_->lanelet_by_id(left_lane_list[i-1].cur_brother_lane[0]);
            lanelet_ptr_t lanelet_2 = map_->osm_map_->lanelet_by_id(left_lane_list[i].cur_brother_lane[0]);
            if(!lanelet_1->fits_before(lanelet_2))//高速道不联通
            {
                jump_flag = true;
            }
        }

        std::vector<point_with_id_t> nodes_left(map_->osm_map_->lanelet_by_id(left_lane_list[i].cur_brother_lane[0])->nodes(LEFT));
        std::vector<point_with_id_t> nodes_right(map_->osm_map_->lanelet_by_id(left_lane_list[i].cur_brother_lane[0])->nodes(RIGHT));
        xy_point point_xy;
        double _mileage_accu = 0.0;  //距离累加器

        for(int j = 0; j < (int)nodes_left.size()-1; j++)//每一段lane的起始点和终止点是同一个点???//否则会有重叠
        {
            center_point point_center;
            if(j == 0)
            {
                lanelet_width = dist(nodes_left[0],nodes_right[0]);//不同lanelet_具有不同的宽度
                point_center.road_level = left_lane_list[i].road_level;
            }

            _mileage_accu += dist(nodes_left[j], nodes_left[j+1]);

            if(jump_flag == false )
            {
                //coord_transfer transfer; //坐标转换
                point_xy = transfer_.cs.ll2xy(get<LAT>(nodes_left[j]), get<LON>(nodes_left[j]));

                point_center.x = get<0>(point_xy);
                point_center.y = get<1>(point_xy);
                point_center.index = point_num;
                point_center.width = lanelet_width;
                point_center.lane_id = left_lane_list[i].cur_brother_lane.front();
                point_center.left_lane_num = lane_num_left;
                point_center.right_lane_num = lane_num_right;

                center_line_tmp.push_back(point_center);
                point_num++;
            }
            else if(jump_flag == true && _mileage_accu > 75)
            {
                //coord_transfer transfer; //坐标转换
                point_xy = transfer_.cs.ll2xy(get<LAT>(nodes_left[j]), get<LON>(nodes_left[j]));

                point_center.x = get<0>(point_xy);
                point_center.y = get<1>(point_xy);
                point_center.index = point_num;
                point_center.width = lanelet_width;
                point_center.lane_id = left_lane_list[i].cur_brother_lane.front();
                point_center.left_lane_num = lane_num_left;
                point_center.right_lane_num = lane_num_right;

                center_line_tmp.push_back(point_center);
                point_num++;
            }
        }
    }  //for(unsigned int i =0; i < left_lane_list.size(); i++)

    //计算里程//计算yawing//center_line
    if(center_line_tmp.size() > 0)
    {
        center_point point_center;
        double delt_s = 0;
        center_line_tmp[0].mileage = delt_s;

        for(unsigned int i = 0; i < center_line_tmp.size()-1; i++)
        {
            delt_s += sqrt((center_line_tmp[i+1].x - center_line_tmp[i].x)*(center_line_tmp[i+1].x - center_line_tmp[i].x)
                           +(center_line_tmp[i+1].y - center_line_tmp[i].y)*(center_line_tmp[i+1].y - center_line_tmp[i].y));
            center_line_tmp[i+1].mileage = delt_s;

            point_center.mileage = center_line_tmp[i].mileage;
            point_center.width = center_line_tmp[i].width;
            point_center.index = center_line_tmp[i].index;

            point_center.lane_id = center_line_tmp[i].lane_id;
            point_center.left_lane_num = center_line_tmp[i].left_lane_num;
            point_center.right_lane_num = center_line_tmp[i].right_lane_num;
            //point_center.type = center_line_tmp[i].type;
            //获取道路级别
            point_center.road_level = center_line_tmp[i].road_level;
            {
                get_heading(center_line_tmp[i].x,center_line_tmp[i].y,center_line_tmp[i+1].x,
                            center_line_tmp[i+1].y,center_line_tmp[i].yaw);
                //centrolline按车道宽度加偏
                point_center.yaw = center_line_tmp[i].yaw;
                double  vertical_direction = calculate_vertical_direction(center_line_tmp[i].yaw);
                double lateral_offset =  1.75;//按照3.5m加偏

                //赋值
                point_center.x = center_line_tmp[i].x +
                                 lateral_offset * sin( vertical_direction * RADIANS_PER_LSB);
                point_center.y  = center_line_tmp[i].y +
                                  lateral_offset * cos( vertical_direction * RADIANS_PER_LSB);
            }

            //长线需要插值
            if (route_behavior_.center_line_.size() > 0)
            {
                center_point p0 = route_behavior_.center_line_.back();//当前点和之前存入点进行距离比较
                center_point p1;
                double dist = point_center.mileage - p0.mileage;
                if (dist > 0.5)
                {
                    int n = (int)(dist / 0.5); //插几个点
                    double dx = (point_center.x - p0.x) / (n + 1);
                    double dy = (point_center.y - p0.y) / (n + 1);
                    double dm = dist / (n + 1);
                    p1.yaw = p0.yaw;
                    p1.width = p0.width;
                    for (int j = 1; j <= n; j++)
                    {
                        p1.x = p0.x + dx * j;
                        p1.y = p0.y + dy * j;
                        p1.mileage = p0.mileage + dm * j;
                        if (j <= n / 2)//插值点的属性以最近点的属性为准
                        {
                            p1.lane_id = p0.lane_id;
                            p1.left_lane_num = p0.left_lane_num;
                            p1.right_lane_num = p0.right_lane_num;
                        }
                        else
                        {
                            p1.lane_id = point_center.lane_id;
                            p1.left_lane_num = point_center.left_lane_num;
                            p1.right_lane_num = point_center.right_lane_num;
                        }
                        p1.index = route_behavior_.center_line_.size();
                        route_behavior_.center_line_.push_back(p1);
                    }
                }
            }

            //插入实际点
            point_center.index = route_behavior_.center_line_.size();
            route_behavior_.center_line_.push_back(point_center);
        }

        int32_t cnt = 0;

        for (int32_t i = 0; i < route_behavior_.center_line_.size(); i++)
        {
            if (route_behavior_.center_line_[i].road_level > 0)
            {
//                std::cout<<"center_line_ i = "<<i<<"road_level = "<<center_line_[i].road_level<<std::endl;
                cnt++;
            }
        }

        std::cout << "road_level > 0 cnt = "<<cnt<<std::endl;

        //获得keypoint的type,来填key_points
        int32_t index = 0;
        for (size_t i = 0; i < route_behavior_.key_points_.size(); i++ )
        {
            double dis_min = 0;
            match_center_line_xy(route_behavior_.key_points_[i].x, route_behavior_.key_points_[i].y, index, 5.0, 25.0, route_behavior_.key_points_[i].lane_id, dis_min);
            for (int j = 0; j < MAX_KP_TYPE; j++)
            {
                if (route_behavior_.key_points_[i].type[j] == 1)
                {
                    bind_key_point(index, j, "");
                }
            }
        }
    }
    delete route_lane;
}

/////////////////////////////////////////////////
/// \brief 将车道map_->lane_list_转换为切片的序列
/// \return
/// 函数最终会填写 route_motion_info，blane_lborder_map，blane_rborder_map，changelane_lborder_map
/////////////////////////////////////////////////
void RouteBase::lane_to_section_line(double cur_lat,double cur_lon)
{
    if(map_->lane_list_.size() == 0)
    {
        clear();
        std::cout << "lane_to_section_line: map_->lane_list_ size == 0" << std::endl;
        return;
    }

    //临时存储高速道
    std::vector<roadmap::lane> left_lane_list;
    //在兄弟lane中找出高速车道，每组brother_lane只取其中的最左边的一条lane;
    roadmap::lane* route_lane = new roadmap::lane();

    //搜索最左边车道
    for( int lane_index = 0; lane_index < (int)map_->lane_list_.size(); lane_index++)
    {
        lanelet_ptr_t left_lanelet;//最左边车道

        if( lane_index < (int)map_->lane_list_.size()-1)
        {
            for( int i = lane_index ; i < (int)map_->lane_list_.size()-1; i++)
            {
                lanelet_ptr_t lanelet_1 = map_->osm_map_->lanelet_by_id(map_->lane_list_[i].cur_brother_lane.front());//从lane中得到lanelet;
                lanelet_ptr_t lanelet_2 = map_->osm_map_->lanelet_by_id(map_->lane_list_[i+1].cur_brother_lane.front());//从lane中得到lanelet,处理前后两个lanelet相邻的情况;

                //判断两段车道是否为兄弟lane,如果为兄弟lane则miss掉重复部分
                if(lanelet_1->id() == lanelet_2->id())   //兄弟车道的道路是左小右大，将右边的去掉
                {
                    i++;
                    lane_index++;
                }
                else
                {
                    left_lanelet = lanelet_1;
                    break;//跳出循环
                }
            }
        }
        else if( lane_index == (int)map_->lane_list_.size()-1)
        {
            left_lanelet = map_->osm_map_->lanelet_by_id(map_->lane_list_[lane_index].cur_brother_lane.front());//从lane中得到lanelet;
        }

        if(left_lanelet != NULL && left_lanelet->id() > 0)
        {

            map_->get_lane_from_map_by_id(left_lanelet->id(),route_lane);
            left_lane_list.push_back(*route_lane);
        }
    }

    route_section_clear(route_motion_info_);


    //sec_vec.clear();
    blane_rborder_map_.clear();
    blane_lborder_map_.clear();
    changelane_lborder_map_.clear();
    int sec_cnt = 0;
    mileage_section_acc_ = 0;
    //首先遍历left_map_->lane_list_中所有最左车道，然后遍历最左车道的每个车道点，然后遍历与此最左车道的每个兄弟车道(右车道)上的每个点
    for(int left_lane_index = 0; left_lane_index < (int)left_lane_list.size(); left_lane_index++)
    {

        std::cout << "\e[0;32m" << "section:  left_lane = "  <<left_lane_list[left_lane_index].lane_id<<"\e[0m" << std::endl;
        roadmap::lane base_nad_line = left_lane_list[left_lane_index];
        int64_t left_lane_id = base_nad_line.lane_id;
        xy_point point_xy_b,point_xy_e;
        std::vector<point_with_id_t> nodes_left(map_->osm_map_->lanelet_by_id(left_lane_id)->nodes(LEFT));  //车道的左边线
        point_xy_b = transfer_.cs.ll2xy(get<LAT>(nodes_left[0]), get<LON>(nodes_left[0]));//开始点
        point_xy_e = transfer_.cs.ll2xy(get<LAT>(nodes_left[nodes_left.size()-2]), get<LON>(nodes_left[nodes_left.size()-2])); //最后一个点没有算直接丢了
        double lane_b_x = get<0>(point_xy_b);
        double lane_b_y = get<1>(point_xy_b);
        double lane_e_x = get<0>(point_xy_e);
        double lane_e_y = get<1>(point_xy_e);
        //double _mileage_accu = 0.0; //里程累加

        for(int left_point_index = 0; left_point_index < (int)nodes_left.size()-1; left_point_index++)
        {
            mileage_section_acc_ += dist(nodes_left[left_point_index], nodes_left[left_point_index+1]);
            xy_point point_xy,point_xy_n;
            point_xy = transfer_.cs.ll2xy(get<LAT>(nodes_left[left_point_index]), get<LON>(nodes_left[left_point_index]));
            point_xy_n= transfer_.cs.ll2xy(get<LAT>(nodes_left[left_point_index+1]), get<LON>(nodes_left[left_point_index+1]));
            double ref_lx = get<0>(point_xy);
            double ref_ly = get<1>(point_xy);
            double ref_lx_n= get<0>(point_xy_n);
            double ref_ly_n= get<1>(point_xy_n);

            double ref_yaw = 0.0;
            get_heading(ref_lx,ref_ly,ref_lx_n,ref_ly_n,ref_yaw);  //计算参考点的偏向角，整个切片的参考头指向

            //初始化
            section_m section_line;
            memset(&section_line,0,sizeof(section_m));//切片数据清零
            section_line.num_of_lane = 0;
            double yaw = 0.0;
            double distance = 0.0;
            point_m sec_point;    //一个切片的对象
            memset(&sec_point,0,sizeof(point_m));

            sec_point.x = ref_lx;       //用参考点的x，y来初始化sec_point;将切片点的x，y初值定位在最左边线点，然后通过偏移得到最右点片点的x，y值
            sec_point.y = ref_ly;       //

            double distance_pre = 0.0;  //遍历此时最左车道的所有车道同向车道(包含自身)

            //依次遍历每个车道对应的point_m,并将其存储到section_line中
            //lane_index为每个车道的序列号
            double ref_min_lx = ref_lx,ref_min_ly = ref_ly;//车道左边线遍历找到最近点
            for(int lane_index = 0; lane_index < base_nad_line.cur_brother_lane.size(); lane_index++)
            {
                int64_t right_lane_id = base_nad_line.cur_brother_lane[lane_index];
                std::vector<point_with_id_t> lane_nodes_right(map_->osm_map_->lanelet_by_id(right_lane_id)->nodes(RIGHT));  //车道的右边线
                std::vector<point_with_id_t> lane_nodes_left(map_->osm_map_->lanelet_by_id(right_lane_id)->nodes(LEFT));  //车道的左边线
                int64_t  pos_l = 0;
                int64_t  pos_r = 0;
                double min_l_len = 100000.0;   //up，在center_point的上面
                double min_r_len = 100000.0;   //up，在center_point的上面
                double min_rx,min_ry,min_rx_n,min_ry_n;//车道右边线遍历找到最近点
                double min_lx,min_ly,min_lx_n,min_ly_n;//车道左边线遍历找到最近点

                //在一个车道中找右边线点
                for(int right_lane_point_index = 0; right_lane_point_index < (int)lane_nodes_right.size()-1; right_lane_point_index++)//去掉最后一个点，偏离了轨迹
                {
                    point_xy = transfer_.cs.ll2xy(get<LAT>(lane_nodes_right[right_lane_point_index]), get<LON>(lane_nodes_right[right_lane_point_index]));
                    double x = get<0>(point_xy);
                    double y = get<1>(point_xy);
                    double lane_right_yaw = 0.0;
                    point_xy_n= transfer_.cs.ll2xy(get<LAT>(lane_nodes_right[right_lane_point_index+1]), get<LON>(lane_nodes_right[right_lane_point_index+1]));
                    double x_n = get<0>(point_xy_n);
                    double y_n = get<1>(point_xy_n);
                    get_heading(x,y,x_n,y_n,lane_right_yaw);

                    double yaw_dif_to_dist = 0.0;
                    if(yaw < 0)
                    {
                        yaw_dif_to_dist = 0;
                    }
                    else
                    {
                        yaw_dif_to_dist = fabs(10 * cau_derta_heading(ref_yaw, lane_right_yaw) / 180);
                    }

                    double dist_tmp = sqrt((ref_lx-x)*(ref_lx-x) + (ref_ly-y)*(ref_ly-y)) + yaw_dif_to_dist;//计算距离时，考虑头指向和距离的双重误差

                    if(dist_tmp  < min_r_len)  //加上方向角度判断防止转弯时匹配到另一边去了
                    {
                        min_r_len = dist_tmp;
                        min_rx = x;  //每次先赋值成左边线的坐标，然后从此左边线的点进行偏移
                        min_ry = y;
                        min_rx_n = x_n;
                        min_ry_n = y_n;
                        pos_r = right_lane_point_index;
                        //cout << "pos = " << pos <<" right_lane_id="<<right_lane_id<< endl;
                    }
                }


                //在一个车道中找左边线点
                for(int left_lane_point_index = 0; left_lane_point_index < (int)lane_nodes_left.size()-1; left_lane_point_index++)//去掉最后一个点，偏离了轨迹
                {
                    point_xy = transfer_.cs.ll2xy(get<LAT>(lane_nodes_left[left_lane_point_index]), get<LON>(lane_nodes_left[left_lane_point_index]));
                    double x = get<0>(point_xy);
                    double y = get<1>(point_xy);
                    double lane_left_yaw = 0.0;
                    point_xy_n= transfer_.cs.ll2xy(get<LAT>(lane_nodes_left[left_lane_point_index+1]), get<LON>(lane_nodes_left[left_lane_point_index+1]));
                    double x_n = get<0>(point_xy_n);
                    double y_n = get<1>(point_xy_n);
                    get_heading(x,y,x_n,y_n,lane_left_yaw);

                    double yaw_dif_to_dist = 0.0;
                    if(yaw < 0)
                    {
                        yaw_dif_to_dist = 0;
                    }
                    else
                    {
                        yaw_dif_to_dist = fabs(10 * cau_derta_heading(ref_yaw, lane_left_yaw) / 180);
                    }

                    double dist_tmp = sqrt((ref_lx-x)*(ref_lx-x) + (ref_ly-y)*(ref_ly-y)) + yaw_dif_to_dist;

                    if(dist_tmp  < min_l_len)  //加上方向角度判断防止转弯时匹配到另一边去了
                    {
                        min_l_len = dist_tmp;
                        min_lx = x;  //每次先赋值成左边线的坐标，然后从此左边线的点进行偏移
                        min_ly = y;
                        min_lx_n = x_n;
                        min_ly_n = y_n;
                        pos_l = left_lane_point_index;
                        //cout << "pos = " << pos <<" left_lane_id="<<left_lane_id<< endl;
                    }
                }
                //
                //右边线来做方向矢量，左边线最近点到右边线的距离，并以此来做为车道宽度。
                distance = calculate_sign_and_value_offset(min_lx,min_ly,min_rx,min_ry,min_rx_n,min_ry_n);  //左边线的点到右边线的2个最近点构成的直线的距离
                get_heading(min_rx,min_ry,min_rx_n,min_ry_n,yaw);  //左边线的前一点和下一点构成的头指向,作为切片点的头指向



                sec_point.yaw = yaw;//==获得sec_point的头指向
                sec_point.sug_speed =(int8_t) base_nad_line.max_speed;//==获得sec_point的速度
                sec_point.width = fabs(distance);
                sec_point.mileage = mileage_section_acc_;//==获得sec_point的里程

                //如果为最左边的点，则直接平移；否则以最左边点平移相应的车道宽来作为相应的边界点。
                //首先平移到车道最左边边线上
                double move_dist = calculate_sign_and_value_offset(ref_lx,ref_ly,min_lx,min_ly,min_lx_n,min_ly_n);  //左边线的点到右边线的2个最近点构成的直线的距离
                //double direction_t = 0;
                //get_heading(min_lx,min_ly,min_lx_n,min_ly_n,direction_t);  //左边线的前一点和下一点构成的头指向
                double vertical_direction = calculate_vertical_direction(ref_yaw);

                ref_min_lx = ref_lx - move_dist * sin( vertical_direction * RADIANS_PER_LSB); //按照参考点平移
                ref_min_ly = ref_ly - move_dist * cos( vertical_direction * RADIANS_PER_LSB); //按照参考点平移
                //从车道左边线平移到车道中心
                double lateral_offset = distance/ 2;
                //vertical_direction = calculate_vertical_direction(ref_yaw);
                sec_point.x = ref_min_lx - lateral_offset * sin( vertical_direction * RADIANS_PER_LSB);//当前切片点由最左车道线的点的x,y偏移得到
                sec_point.y = ref_min_ly - lateral_offset * cos( vertical_direction * RADIANS_PER_LSB);
                sec_point.type = KP_NORMAL;

                section_line.lane.push_back(sec_point);//==获得sec_point的坐标
                section_line.num_of_lane ++;

                //distance_pre = distance;   //保存同一切片上点的上一车道的路宽

                if(lane_index == base_nad_line.cur_brother_lane.size()-1)   //当前车道是最右边车道时：取最右边车道的最右边界线上的点作为右边界线
                {
                    Point_m pt_l;
                    pt_l.x = ref_lx;
                    pt_l.y = ref_ly;
                    blane_lborder_map_.insert(std::make_pair(sec_cnt,pt_l));    //兄弟车道的左边界

                    point_xys point_l;
                    point_l.x = ref_lx;
                    point_l.y = ref_ly;
                    point_l.type = KP_NORMAL;
                    route_motion_info_.left_line.line.push_back(point_l); //兄弟车道的左边界
                    route_motion_info_.left_line.num_of_point ++;


                    //右边界不能沿用地图上的点，需要根据偏移重新给出
                    //首先平移到车道最右边边线上
                    Point_m pt_r;
                    double vertical_direction = calculate_vertical_direction(ref_yaw);
                    pt_r.x = ref_min_lx - distance * sin( vertical_direction * RADIANS_PER_LSB); //按照参考点平移
                    pt_r.y = ref_min_ly - distance * cos( vertical_direction * RADIANS_PER_LSB); //按照参考点平移
                    blane_rborder_map_.insert(std::make_pair(sec_cnt,pt_r));    //兄弟车道的右边界

                    point_xys point_r;
                    point_r.x = pt_r.x;
                    point_r.y = pt_r.y;
                    point_r.type = KP_NORMAL;
                    route_motion_info_.right_line.line.push_back(point_r);//兄弟车道的右边界
                    route_motion_info_.right_line.num_of_point ++;

                    route_motion_info_.right_edge.line.push_back(point_r);//兄弟车道中只有right_edge；左边边界可能是借道区域
                    route_motion_info_.right_edge.num_of_point ++;
                }
            }

            route_motion_info_.line.push_back(section_line);  //std::vector< nad_lcm::section_m > line;
            sec_cnt++;


            int sec_pos = sec_cnt-1;
            double min_op_len = 10000.0;
            double min_x,min_y,min_xn,min_yn;
            int64_t op_lane_id = left_lane_list[left_lane_index].change_lane.front();
            int op_lane_size = get_op_lane_size(op_lane_id);     //逆向车道可借道数目
            if(op_lane_size > 0)  //有可借道的逆向车道
            {
                std::vector<point_with_id_t> op_nodes_right(map_->osm_map_->lanelet_by_id(op_lane_id)->nodes(RIGHT));  //对向车道的右边线
                for(int op_point_index = 0; op_point_index < op_nodes_right.size()-1; op_point_index++)
                {

                    xy_point point_xy_op = transfer_.cs.ll2xy(get<LAT>(op_nodes_right[op_point_index]), get<LON>(op_nodes_right[op_point_index]));  //找到最近点的下一个点
                    xy_point point_xy_opn = transfer_.cs.ll2xy(get<LAT>(op_nodes_right[op_point_index+1]), get<LON>(op_nodes_right[op_point_index+1]));  //找到最近点的下一个点
                    double x_op_r = get<0>(point_xy_op);
                    double y_op_r = get<1>(point_xy_op);
                    double x_op_rn = get<0>(point_xy_opn);
                    double y_op_rn = get<1>(point_xy_opn);
                    double lane_right_yaw = 0.0;
                    get_heading(x_op_rn,y_op_rn,x_op_r,y_op_r,lane_right_yaw);//逆向车道，方向反着来

                    double yaw_dif_to_dist = 0.0;
                    if(yaw < 0)
                    {
                        yaw_dif_to_dist = 0;
                    }
                    else
                    {
                        yaw_dif_to_dist = fabs(10 * cau_derta_heading(ref_yaw, lane_right_yaw) / 180);
                    }

                    double dist_tmp = sqrt((ref_lx-x_op_r)*(ref_lx-x_op_r) + (ref_ly-y_op_r)*(ref_ly-y_op_r)) + yaw_dif_to_dist;

                    if(dist_tmp < min_op_len)  //加上方向角度判断防止转弯时匹配到另一边去了
                    {
                        min_op_len = dist_tmp;
                        min_x = x_op_r;  //每次先赋值成左边线的坐标，然后从此左边线的点进行偏移
                        min_y = y_op_r;
                        min_xn = x_op_rn;
                        min_yn = y_op_rn;
                        //std::cout << dist_tmp<<endl;
                    }

                }
                //首先平移到车道最左边边线上
                double move_dist = calculate_sign_and_value_offset(ref_lx,ref_ly,min_x,min_y,min_xn,min_yn);  //左边线的点到右边线的2个最近点构成的直线的距离
                double vertical_direction = calculate_vertical_direction(ref_yaw)+180;
                min_x = ref_lx - move_dist * sin( vertical_direction * RADIANS_PER_LSB); //按照参考点平移
                min_y = ref_ly - move_dist * cos( vertical_direction * RADIANS_PER_LSB); //按照参考点平移

                Point_m pt_m;
                pt_m.x = min_x;
                pt_m.y = min_y;

                point_xys point_l;
                point_l.x = min_x;
                point_l.y = min_y;
                point_l.type = KP_NORMAL;
                route_motion_info_.left_edge.line.push_back(point_l);//即使借道，只允许借用一个车道
                route_motion_info_.left_edge.num_of_point++;

                changelane_lborder_map_.insert(std::make_pair(sec_pos,pt_m));

            }
            else //无可借道的逆向车道，左边线为兄弟车道的左边界
            {
                Point_m pt_m;
                pt_m.x = ref_lx;
                pt_m.y = ref_ly;

                point_xys point_l;
                point_l.x = ref_lx;
                point_l.y = ref_ly;
                point_l.type = KP_NORMAL;
                route_motion_info_.left_edge.line.push_back(point_l);
                route_motion_info_.left_edge.num_of_point ++;
                changelane_lborder_map_.insert(std::make_pair(sec_pos,pt_m));

            }
        }
    }

    //主要 mileage
    route_motion_info_.num_of_section = sec_cnt;
    locate_position_on_section_ll(cur_lat,cur_lon);//定位

    //填入route_motion 中的其它信息
    route_motion_info_.time_stamp = time_stamp_;
    route_motion_info_.destination = destination_;
    route_motion_info_.route_reason = route_reason_;
    route_motion_info_.mileage_pass = MILEAGE_PASS;
    route_motion_info_.mileage_next = MILEAGE_NEXT;
    route_motion_info_.cur_section = cur_sec_index_;
    route_motion_info_.mileage_start = route_motion_info_.line[route_motion_info_.cur_section].lane[cur_lane_index_].mileage;

    double end_dis_min;//匹配终点切片的最短距离
    int end_sec_index = 0; //初始化
    int end_lane_index = 0;
    center_point end_pt = route_behavior_.key_points_[route_behavior_.key_points_.size()-1];//最后一个keypoint不一定是终点、、？？？？？
    match_section_line_xy(end_pt.x,end_pt.y,end_sec_index,end_lane_index,3,30,end_dis_min);
    if(end_dis_min > 3)
    {
        LOG(ERROR) << "stop_point out of  section line!" << std::endl;
    }
    route_motion_info_.mileage_stop = route_motion_info_.line[end_sec_index].lane[0].mileage-route_motion_info_.mileage_start;
    route_motion_info_draw_ = route_motion_info_;
    delete route_lane;
}

////////////////////////////////////////////////
/// \brief 清空route对象中所有记录
/// \return
///清理对象中的变量
/////////////////////////////////////////////////
void RouteBase::clear()
{
    route_behavior_.cur_center_line_index_ = -1;
    route_behavior_.cur_key_point_index_ = -1;
    route_behavior_.cur_point_.index = -1;
    route_behavior_.limspeed_value = 20;
    map_->lane_list_.clear();
    route_behavior_.center_line_.clear();
    route_behavior_.key_points_.clear();
    route_behavior_.limspeed_list_.clear();
    route_behavior_.lanelist_.clear();

    time_stamp_ = 0;
    destination_ = "";
    route_reason_ = 0;
    cur_sec_index_ = -1;
    cur_lane_index_ = -1;
    mileage_section_acc_ = 0.0;
    mileage_section_send_ = -1;
    ending_point_alarm_cnt_ = 0;
}

////////////////////////////////////////////////
/// \brief 在中心线上进行匹配
/// \return RET_OK:0 , RET_ERROR 1
/// \param lat：当前位置的纬度
/// \param lon：当前位置的经度
///在中心线上搜索点，返回:RET_OK=存在<min_ok的点，RET_NOT_SAME=存在>=min_ok的点，RET_NOT_EXIST=无点
///例如min_ok=3表示: [首次距离<3米, 其后>3米]的区间内最小的点
/////////////////////////////////////////////////
int RouteBase::match_center_line_ll(double lat, double lon, int &index, double min_ok, double range, int64_t lane_id, double &dis_min)
{
    double x, y;
    transfer_.cs.ll2xy(lat, lon, x, y);
    return match_center_line_xy(x, y, index, min_ok, range, lane_id, dis_min);
}

////////////////////////////////////////////////
/// \brief 在中心线上进行匹配
/// \return RET_OK:0 , RET_ERROR 1
/// \param x：高斯投影之后的x坐标
/// \param y：高斯投影之后的y坐标
///在中心线上搜索点，返回:RET_OK=存在<min_ok的点，RET_NOT_SAME=存在>=min_ok的点，RET_NOT_EXIST=无点
///例如min_ok=3表示: [首次距离<3米, 其后>3米]的区间内最小的点
///匹配后的结果填入到
/////////////////////////////////////////////////
int RouteBase::match_center_line_xy(double x, double y, int &index, double min_ok, double range, int64_t lane_id, double &dis_min)
{
    int ret = RET_NOT_SAME;
    int ret1 = RET_NOT_SAME;
    double length_min = 100000000.0;
    double length_cur = 0;

    //入口判断
    if (route_behavior_.center_line_.size() == 0)
    {
        return RET_NOT_EXIST;
    }

    //循环搜索最近的点
    int i = std::max(0, index);
    if (i >= (int)route_behavior_.center_line_.size()) i = (int)route_behavior_.center_line_.size() - 1;
    for (; i < (int)route_behavior_.center_line_.size(); i++)
    {
        //lane_id == 0时直接进行比较
        //lane_id != 0时则与center_lane匹配后,才进行比较
        if (lane_id != 0 && lane_id != route_behavior_.center_line_[i].lane_id)
        {
            continue;
        }

        //保存比较近的位置
        length_cur = length_two_points(x, y, route_behavior_.center_line_[i].x, route_behavior_.center_line_[i].y);
        if (length_cur < length_min)
        {
            length_min = length_cur;
            index = i;
        }

        //判断目前是否进入[首次<range, 其后>range]区间
        if (length_cur <= min_ok)
        {
            ret1 = RET_OK;
        }
        else if (length_cur >= range && ret1 == RET_OK)
        {
            ret = RET_OK;
            //////////////////////////    add by sx 20170710
            dis_min = length_min;
            return ret;
        }

    }  //for (; i < (int)center_line_.size(); i++)

    dis_min = length_min;

    return ret;
}

////////////////////////////////////////////////
/// \brief 在切片序列上进行匹配
/// \return RET_OK:0 , RET_ERROR 1
/// \param lat：当前位置的纬度
/// \param lon：当前位置的经度
///在中心线上搜索点，返回:RET_OK=存在<min_ok的点，RET_NOT_SAME=存在>=min_ok的点，RET_NOT_EXIST=无点
///例如min_ok=3表示: [首次距离<3米, 其后>3米]的区间内最小的点
///匹配后的结果填入到
/////////////////////////////////////////////////
int RouteBase::match_section_line_ll(double lat,double lon,int &sec_index,int &lane_index, double min_ok, double range, double &dis_min)
{
    double x,y;
    transfer_.cs.ll2xy(lat,lon,x,y);
    return match_section_line_xy(x,y,sec_index,lane_index,min_ok, range, dis_min);
}

////////////////////////////////////////////////
/// \brief 在切片序列上进行匹配
/// \return RET_OK:0 , RET_ERROR 1
/// \param x：高斯投影之后的x坐标
/// \param y：高斯投影之后的y坐标
///在中心线上搜索点，返回:RET_OK=存在<min_ok的点，RET_NOT_SAME=存在>=min_ok的点，RET_NOT_EXIST=无点
///例如min_ok=3表示: [首次距离<3米, 其后>3米]的区间内最小的点
///匹配后的结果填入到
/////////////////////////////////////////////////
int RouteBase::match_section_line_xy(double x,double y,int &sec_index,int &lane_index, double min_ok, double range, double &dis_min)
{
    int ret = RET_NOT_SAME;
    int ret1 = RET_NOT_SAME;
    double min_mileage = 100000000.0;

    int i = std::max(0, sec_index);
    //遍历所有切片及每个切片上的每条道路。距离当前点最近的切片点，它所在的切片即是当前切片cur_section，它所在的lane即是当前车辆处于的车道lane(从左到右),从0开始
    //全局循环

    for(; i < route_motion_info_.num_of_section; i++)  //???????????????-1
    {
        for(int j = 0; j< route_motion_info_.line[i].num_of_lane; j++)
        {
            double mileage_tmp = sqrt((route_motion_info_.line[i].lane[j].x - x)*(route_motion_info_.line[i].lane[j].x - x)+
                                      (route_motion_info_.line[i].lane[j].y - y)*(route_motion_info_.line[i].lane[j].y - y));

            //第一次匹配时，记录下cur_sec_index
            if(mileage_tmp < min_mileage)///干掉魔鬼数字//目前30代表搜索的范围
            {
                min_mileage = mileage_tmp;
                sec_index = i;  //最小值是为当前点处于哪个切片上
                lane_index = j;  //从左到右，依次为：0，1，2，3.。。。
            }

            //判断目前是否进入[首次<range, 其后>range]区间
            if (mileage_tmp <= min_ok)
            {
                ret1 = RET_OK;
            }
            else if (mileage_tmp >= range && ret1 == RET_OK)
            {
                ret = RET_OK;
                //////////////////////////    add by sx 20170710
                dis_min = min_mileage;
                return ret;
            }
        }
    }

    dis_min = min_mileage;
    return ret;

}


void RouteBase::route_section_clear(route_planning_m &route_sec)
{
    route_sec.left_line.line.clear();
    route_sec.left_line.num_of_point = 0;
    route_sec.left_edge.line.clear();
    route_sec.left_edge.num_of_point = 0;
    route_sec.right_line.line.clear();
    route_sec.right_line.num_of_point = 0;
    route_sec.right_edge.line.clear();
    route_sec.right_edge.num_of_point = 0;
    route_sec.line.clear();
    route_sec.num_of_section = 0;
}

int RouteBase::get_op_lane_size(double cur_lat,double cur_lon,double cur_yaw)
{
    int64_t lane_id = map_->locate_point_on_lanelet(cur_lat,cur_lon,cur_yaw);
    int op_lane_size = -1;
    if (0 != lane_id)
    {
        op_lane_size = get_op_lane_size(lane_id);
    }

    return op_lane_size;

}

int RouteBase::get_op_lane_size(int64_t lane_id)
{
    roadmap::lane* lane = new roadmap::lane();
    int op_lane_size = -1;
    if (0 != lane_id)
    {
        map_->get_lane_from_map_by_id(lane_id, lane);
        op_lane_size = lane->change_lane.size() - lane->cur_brother_lane.size();
    }
    delete lane;
    return op_lane_size;
}

//返回当前左边或者右边的可变道车道数目
int RouteBase::get_change_lane(double cur_lat,double cur_lon,double cur_yaw,int turn)
{
    int64_t lane_id = map_->locate_point_on_lanelet(cur_lat,cur_lon,cur_yaw);
    int lane_num = 0;
    if (0 != lane_id)
    {
        if(turn == CL_DIRECTION_LEFT)
        {
            lane_num = get_change_lane(lane_id,CL_DIRECTION_LEFT);
        }
        if(turn == CL_DIRECTION_RIGHT)
        {
            lane_num = get_change_lane(lane_id,CL_DIRECTION_RIGHT);
        }

    }

    return lane_num;
}

int RouteBase::get_change_lane(int64_t lane_id,int turn)
{
    int left_lane_num = 0;
    int right_lane_num = 0;
    roadmap::lane* lane = new roadmap::lane();
    if (0 != lane_id)
    {
        map_->get_lane_from_map_by_id(lane_id, lane);
        //判断左右车道数
        auto pos = std::find(lane->change_lane.cbegin(), lane->change_lane.cend(), lane_id);
        left_lane_num = std::distance(lane->change_lane.cbegin(), pos);
        right_lane_num = lane->change_lane.size() - left_lane_num - 1;
    }
    delete lane;
    if(turn == CL_DIRECTION_LEFT)
    {
        return left_lane_num;
    }
    else
    {
        return right_lane_num;
    }
}

void RouteBase::draw()
{
    //至少画5个点
    if (route_behavior_.center_line_.size() < 5)
    {
        return;
    }

    roadmap::lane* lane_route = new roadmap::lane();
    //画车道列表
    for (size_t i = 0; i < map_->lane_list_.size(); i++)
    {
        lanelet_ptr_t lanelet = map_->osm_map_->lanelet_by_id(lane_route->lane_id);
        draw_lanelet(lanelet);
    }

    //画中心线
    for(int i = 0; i < (int)route_behavior_.center_line_.size() - 1; i++)
    {
        glPointSize(5);
        glColor3d(1, 1, 0);
        glBegin(GL_POINTS);
        glVertex2f(route_behavior_.center_line_[i].x, route_behavior_.center_line_[i].y);
        glEnd();

        glPointSize(5);
        glColor3d(1, 1, 0);
        glBegin(GL_LINES);
        glVertex2f(route_behavior_.center_line_[i].x, route_behavior_.center_line_[i].y);
        glVertex2f(route_behavior_.center_line_[i+1].x, route_behavior_.center_line_[i+1].y);
        glEnd();
    }
    delete lane_route;
}
void RouteBase::draw_part_line()
{
    if (route_motion_info_draw_.left_line.num_of_point == 0 || route_motion_info_draw_.right_line.num_of_point == 0)
    {
        return;
    }

    for(int i=0; i<route_motion_info_draw_.left_line.num_of_point; i++)
    {
        glPointSize(2);
        glColor3d(1, 0, 0);
        glBegin(GL_POINTS);
        glVertex2f(route_motion_info_draw_.left_line.line[i].x,route_motion_info_draw_.left_line.line[i].y);
        glEnd();
    }

    for(int i=0; i<route_motion_info_draw_.right_line.num_of_point; i++)
    {
        glPointSize(2);
        glColor3d(1, 0, 0);
        glBegin(GL_POINTS);
        glVertex2f(route_motion_info_draw_.right_line.line[i].x,route_motion_info_draw_.right_line.line[i].y);
        glEnd();
    }
    draw_part_section_line();
}

void RouteBase::draw_part_section_line()
{
    if (route_motion_info_draw_.num_of_section == 0 )
    {
        return;
    }

    for(int i = 0; i < route_motion_info_draw_.num_of_section; i++)
    {
        for(int j = 0; j < route_motion_info_draw_.line[i].num_of_lane; j++)
        {
            glPointSize(2);
            glColor3d(0, 3, 3);
            glBegin(GL_POINTS);
            double x_sec = route_motion_info_draw_.line[i].lane[j].x;
            double y_sec = route_motion_info_draw_.line[i].lane[j].y;
            glVertex2f(x_sec, y_sec);
            glEnd();
        }
    }
    glutPostRedisplay();

}
///使用opengl画lanelet，show的私有函数
void RouteBase::draw_lanelet(lanelet_ptr_t& ll_draw)
{
    using std::get;
    std::vector<point_with_id_t> nodes_left(ll_draw->nodes(LEFT));
    std::vector<point_with_id_t> nodes_right(ll_draw->nodes(RIGHT));
    xy_point point_xy,point_xy1;

    //画左边线
    for(unsigned int i=0; i<nodes_left.size()-1; i++)
    {
        point_xy = transfer_.cs.ll2xy(get<LAT>(nodes_left[i]), get<LON>(nodes_left[i]));
        point_xy1 = transfer_.cs.ll2xy(get<LAT>(nodes_left[i+1]), get<LON>(nodes_left[i+1]));

        glPointSize(2);
        glColor3d(1, 0, 0);
        glBegin(GL_POINTS);
        glVertex2f(get<X>(point_xy), get<Y>(point_xy));
        glEnd();

        glPointSize(1);
        glColor3d(1, 0, 0);
        glBegin(GL_LINES);
        glVertex2f(get<X>(point_xy), get<Y>(point_xy));
        glVertex2f(get<X>(point_xy1), get<Y>(point_xy1));
        glEnd();
    }

    //画右边线
    for(unsigned int i = 0; i < nodes_right.size() - 1; i++)
    {
        point_xy = transfer_.cs.ll2xy(get<LAT>(nodes_right[i]), get<LON>(nodes_right[i]));
        point_xy1 = transfer_.cs.ll2xy(get<LAT>(nodes_right[i+1]), get<LON>(nodes_right[i+1]));

        glPointSize(2);
        glColor3d(0, 1, 0);
        glBegin(GL_POINTS);
        glVertex2f(get<X>(point_xy), get<Y>(point_xy));
        glEnd();

        glPointSize(1);
        glColor3d(0, 1, 0);
        glBegin(GL_LINES);
        glVertex2f(get<X>(point_xy), get<Y>(point_xy));
        glVertex2f(get<X>(point_xy1), get<Y>(point_xy1));
        glEnd();
    }

}

int64_t RouteBase::get_current_route_time()
{
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

//把中心线的某个点修改为关键点，成功返回RET_OK
int RouteBase::bind_key_point(int &index, int type, std::string id, bool *insert_flag)
{
    if (index < 0 || index >= (int)route_behavior_.center_line_.size())
    {
        return RET_ERROR;
    }

    //如果当前点已经是关键点，则复制一个点插入到该点之前
    center_point *kp = &route_behavior_.center_line_[index];

    //保存结果
    if(kp->type[type] == 1)
    {
        //std::cout<<"center line typet is set yet, type=" << type << std::endl;
        return RET_OK;
    }
    kp->type[type] = 1;
    kp->id = id;
    return RET_OK;
}

//把中心线的某个点修改为关键点，并计算归属的车道，成功返回RET_OK
int RouteBase::bind_key_point_xy(int &index, int type, std::string id, double x, double y)
{
    if (index < 0 || index >= (int)route_behavior_.center_line_.size())
    {
        return RET_ERROR;
    }

    //计算归属车道
    double lat, lon;
    transfer_.cs.xy2ll(x, y, lat, lon);
    return bind_key_point_ll(index, type, id, lat, lon);
}

//把中心线的某个点修改为关键点，并计算归属的车道，成功返回RET_OK
int RouteBase::bind_key_point_ll(int &index, int type, std::string id, double lat, double lon)
{
    if (index < 0 || index >= (int)route_behavior_.center_line_.size())
    {
        return RET_ERROR;
    }

    //绑定
    int ret = bind_key_point(index, type, id);
    if (ret != RET_OK)
    {
        return ret;
    }

    //计算对象在关键点的哪个车道
    center_point &cp = route_behavior_.center_line_[index];
    roadmap::lane route_lane;
    map_->get_lane_from_map_by_id(cp.lane_id, &route_lane);//所在lane->所有兄弟lane

    //原始点更靠近[所有兄弟lane]中的哪个lane
    int64_t lane_id = map_->locate_point_on_lane_list(lat, lon, route_lane.cur_brother_lane);
    if (lane_id > 0)
    {
        auto pos = std::find(route_lane.cur_brother_lane.cbegin(), route_lane.cur_brother_lane.cend(), lane_id);
        cp.left_lane_num = std::distance(route_lane.cur_brother_lane.cbegin(), pos);
        cp.right_lane_num = route_lane.cur_brother_lane.size() - cp.left_lane_num - 1;
        return RET_OK;
    }

    //匹配不到车道，返回失败
    cp.type[KP_NONE] = 1;
    cp.id = "";
    return RET_ERROR;
}

