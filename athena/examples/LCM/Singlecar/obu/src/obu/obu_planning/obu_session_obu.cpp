/*-------------------------------------------------------
 * 文件名：obu_session_obu.cpp
 * 创建者：李思政
 * 时  间：2016-04-03
 * 描  述：obu管理自身用到的数据结构的定义
-------------------------------------------------------*/
#include "obu_planning.h"


obu_session_obu::obu_session_obu() : nad_session("")
{
    session_status = E_OBU_SESSION_OBU_CONNECT_HAND;
    start_auto_status = E_OBU_SESSION_OBU_START_AUTO_WAIT;
    flashing_status = FLASHING_STATUS_NONE;
    wait_count = 0;
    last_htbt = get_current_time();
    cur_lon = 0.0;
    cur_lat = 0.0;
    platoon_sn = 0;
    sug_speed = 20;
    cur_speed = 0.0;
    call_park_state = 0; //可召车状态
    is_auto_drive = false;
    route_plan_status = false;
}

//析构函数
obu_session_obu::~obu_session_obu()
{
}

//处理定时器
void obu_session_obu::handle_timer(int64_t counter)
{
    handle_speed();

    if(session_status == E_OBU_SESSION_OBU_CONNECT_HAND || session_status == E_OBU_SESSION_OBU_CONNECT_AUTO)
    {

        send_ou_vui_report();
        if(route->route_behavior_.center_line_.size() > 0)
        {
            //给motion的相关信息
            nad_lcm::om_info_report om_info_report_msg;
            om_info_report_msg.is_auto_drive = is_auto_drive;
            om_info_report_msg.drive_behavior = DRIVE_BEHAVIOR_OVERTAKE;
            om_info_report_msg.car_action = CAR_ACTION_SINGLE;

            //车速
            om_info_report_msg.num_of_command = 1;
            nad_lcm::obu_command cmd;
            cmd.sug_speed = sug_speed;

            om_info_report_msg.command.push_back(cmd);

            om_info_report_msg.route_time_stamp = route->time_stamp_;

            route->locate_on_lane(cur_lat, cur_lon);
            route::center_point &cp = route->route_behavior_.cur_point_;
            /*
            route::center_point *kp = route->get_next_stop_kp(right_of_way);
            if(kp == NULL)
            {
                om_info_report_msg.stop_mileage = 10000.0;
            }
            else
            {
                om_info_report_msg.stop_mileage = kp->dist(cp);
            }
            */
            om_info_report_msg.ending_lane = cp.left_lane_num + 1;

            int lc_lane_num = route->get_change_lane(cur_lat,cur_lon,cur_yaw,CL_DIRECTION_LEFT);
            int rc_lane_num = route->get_change_lane(cur_lat,cur_lon,cur_yaw,CL_DIRECTION_RIGHT);
            if( lc_lane_num + rc_lane_num == 0)
            {
                om_info_report_msg.ending_mileage = -1; //小于0不可以换道
            }
            else
            {
                om_info_report_msg.ending_mileage = 1;//>0可以换道
            }

            //给motion发送请求
            g_lcm->publish("om_info_report", &om_info_report_msg);

        }
    }
}

//保存到消息
void obu_session_obu::save_to_lcm(nad_lcm::route_planning &other)
{
    int32_t kpret = 0;
    //基本信息
    other.time_stamp = route->time_stamp_;
    other.destination = route->destination_;
    other.route_reason = route->route_reason_;

    //车道列表
    for(size_t i = 0; i < route->map_->lane_list_.size(); i++)
    {
        nad_lcm::lane_of_route tmp;
        tmp.lane_id = route->map_->lane_list_[i].lane_id;
        tmp.num_of_lane = route->map_->lane_list_[i].cur_brother_lane.size();
        tmp.lane.assign(route->map_->lane_list_[i].cur_brother_lane.begin(), route->map_->lane_list_[i].cur_brother_lane.end());
        tmp.length = route->map_->lane_list_[i].length;
        tmp.max_speed = route->map_->lane_list_[i].max_speed;
        other.lane_list.push_back(tmp);
    }
    other.num_of_lane = other.lane_list.size();

    //关键点列表
    for(size_t i = 0; i < route->route_behavior_.key_points_.size(); i++)
    {
        kpret = 0;
        for (size_t j = KP_NORMAL; j < (KP_ROUTE_STOP + 1); j++)
        {
            if (1 == route->route_behavior_.key_points_[i].type[j])
            {
                kpret = 1;
                break;
            }
        }
        if (0 == kpret)
        {
            continue;
        }
        if (route->route_behavior_.key_points_[i].type[KP_ROUTE_START] == 1 || route->route_behavior_.key_points_[i].type[KP_ROUTE_STOP] == 1)
        {
            nad_lcm::center_point tmp;
            tmp.x = route->route_behavior_.key_points_[i].x;
            tmp.y = route->route_behavior_.key_points_[i].y;
            tmp.index = route->route_behavior_.key_points_[i].index;
            tmp.yaw = route->route_behavior_.key_points_[i].yaw;
            tmp.width = route->route_behavior_.key_points_[i].width;
            tmp.mileage = route->route_behavior_.key_points_[i].mileage;
            tmp.lane_id = route->route_behavior_.key_points_[i].lane_id;
            //tmp.type = route->route_behavior_.key_points_[i].type;
            //暂时只回传第一个type值，暂时先传normal
            if (route->route_behavior_.key_points_[i].type[KP_ROUTE_START] == 1)
            {
                tmp.type = KP_ROUTE_START;
            }
            if (route->route_behavior_.key_points_[i].type[KP_ROUTE_STOP] == 1)
            {
                tmp.type = KP_ROUTE_STOP;
            }
            //std::cout << "just set type to normal ,TOBEDONE" << std::endl;
            tmp.left_lane_num = route->route_behavior_.key_points_[i].left_lane_num;
            tmp.right_lane_num = route->route_behavior_.key_points_[i].right_lane_num;
            other.key_point_list.push_back(tmp);
        }
    }
    other.num_of_kp = other.key_point_list.size();
}

//保存到消息
void obu_session_obu::save_to_lcm(nad_lcm::obu_info &obu)
{
    obu.obu_name = name;
    obu.obu_type = obu_type;
    obu.gps_time = gps_time;
    obu.cur_lon = cur_lon;
    obu.cur_lat = cur_lat;
    obu.cur_yaw = cur_yaw;
    obu.cur_brake = cur_brake;
    save_to_lcm(obu.cur_point);
    obu.cur_speed = cur_speed;
    obu.cur_acceleration = cur_acceleration;
    obu.steering_angle = steering_angle;
    obu.cur_gears = cur_gears;
    obu.is_auto_drive = (session_status == E_OBU_SESSION_OBU_CONNECT_AUTO ||
                         session_status == E_OBU_SESSION_OBU_DISCONN_AUTO ? 1 : 0);
    obu.is_online = (session_status == E_OBU_SESSION_OBU_CONNECT_AUTO ||
                     session_status == E_OBU_SESSION_OBU_CONNECT_HAND ? 1 : 0);
    obu.platoon_sn = platoon_sn;
    obu.destination = route->destination_;
    obu.sensor_stat[0] = brake_stat;//上报刹车状态//临时方案
    //目前不支持传感器状态上报，填写默认值
    for(int i = 1; i < MAX_SENSOR; i++)
    {
        obu.sensor_stat[i] = sensor_stat[i]; //只有gps是真数据
    }
    obu.sensor_stat[IDX_INS_CENTER] = SENSOR_OK; //其他的替换为假数据
    obu.sensor_stat[IDX_RADAR_ESR] = SENSOR_OK;
    obu.sensor_stat[IDX_RADAR_SRR_FL] = SENSOR_OK;
    obu.sensor_stat[IDX_RADAR_SRR_FR] = SENSOR_OK;
    obu.sensor_stat[IDX_RADAR_SRR_BL] = SENSOR_OK;
    obu.sensor_stat[IDX_RADAR_SRR_BR] = SENSOR_OK;
}

//保存到消息中
void obu_session_obu::save_to_lcm(nad_lcm::center_point &cp)
{
    cp.x = route->route_behavior_.cur_point_.x;
    cp.y = route->route_behavior_.cur_point_.y;
    cp.index = route->route_behavior_.cur_point_.index;
    cp.yaw = route->route_behavior_.cur_point_.yaw;
    cp.width = route->route_behavior_.cur_point_.width;
    cp.mileage = route->route_behavior_.cur_point_.mileage;
    cp.lane_id = route->route_behavior_.cur_point_.lane_id;
    cp.type = KP_NORMAL;
    //std::cout << "just set type to normal ,TOBEDONE" << std::endl;
    cp.left_lane_num = route->route_behavior_.cur_point_.left_lane_num;
    cp.right_lane_num = route->route_behavior_.cur_point_.right_lane_num;
}


bool obu_session_obu::cl_too_busy(int32_t direction, int32_t cl_reason)
{
    //拼接过滤字符串
    char buf[100];
    sprintf(buf, "%s|%s", cl_direction_str(direction), cl_reason_str(cl_reason));
    string key = buf;

    //查找重复换道请求
    int64_t now = get_current_time();
    map<string, int64_t>::iterator it;
    it = cl_list.find(key);
    if (it != cl_list.end() && now < (it->second + MAX_CL_TIMEOUT))
    {
        return true;
    }

    //保存当前换道请求
    cl_list[key] = now;
    return false;
}

//抑制5秒内的重复换道请求
bool obu_session_obu::cl_too_busy_avoiding(int32_t cl_reason)
{
    //拼接过滤字符串
    int32_t key = -1;
    int64_t now = get_current_time();//得到当前时间
    if(cl_reason == CL_REASON_HAND)
    {
        key = CL_REASON_HAND;
        cl_avoid_list[key] = now;//更新键值
    }

    if(cl_reason == CL_REASON_OBU_AVOIDING)
    {
        //查找重复换道请求
        map<int32_t, int64_t>::iterator it;
        it = cl_avoid_list.find(CL_REASON_HAND);
        if (it != cl_avoid_list.end() && now < (it->second + MAX_CL_TIMEOUT))
        {
            return true;
        }

    }
    return false;
}


//内部调用的换道执行操作
int obu_session_obu::change_lane_ex(int32_t direction, int32_t cl_reason, int starting_lane, int ending_lane,
                                    bool need_ack, bool check_busy, bool check_target, bool check_follow,
                                    bool check_forbid, bool check_platoon, bool check_cooperate)
{
    route::center_point &cp = route->route_behavior_.cur_point_;

    //过滤频繁换道请求(人工掰杆换道是人的理智行为，不做频繁检测)
    if (cl_reason != CL_REASON_HAND && check_busy && cl_too_busy(direction, cl_reason))
    {
        return RET_BUSY;
    }

    //初始化应答消息
    nad_lcm::om_change_lane_respond om_change_lane_respond_msg;
    om_change_lane_respond_msg.direction = direction;
    om_change_lane_respond_msg.reason = cl_reason;

    if (starting_lane == -1 && ending_lane == -1)
    {
        if (direction == CL_DIRECTION_LEFT)
        {
            // om_change_lane_respond_msg.starting_lane = 2;
            om_change_lane_respond_msg.ending_lane = 1;
        }
        else
        {
            //om_change_lane_respond_msg.starting_lane = 1;
            om_change_lane_respond_msg.ending_lane = 2;
        }
    }
    else
    {
        //om_change_lane_respond_msg.starting_lane = starting_lane;
        om_change_lane_respond_msg.ending_lane = ending_lane;
    }

    //后门:如果目前没有路径规划，总是允许换道，且不会触发编队换道和协作式换道
    if (cp.index < 0)
    {
        //return send_change_lane_respond(om_change_lane_respond_msg,true, RET_ROUTE_NO_RESULT, "无路径规划禁止换道！");
    }

    //判断目前是不是禁止换道的区间
    /*
    if (check_forbid && route->get_current_forbid_change())
    {
        return send_change_lane_respond(om_change_lane_respond_msg,need_ack, RET_FORBID, "当前路段禁止换道！");
    }
    */

    //查找自身，如果有路径规划，还需要判断有没有目标车道
    if (check_target)
    {
        //手工掰杆换道不判断左侧是否有车道，用来规避李老师认为单车道存在right道，反而需要向左纠回的问题
        //无车道判断交给规划来做；当前查找是否禁止换道路段
        int lc_lane_num = route->get_change_lane(cur_lat,cur_lon,cur_yaw,CL_DIRECTION_LEFT);
        int rc_lane_num = route->get_change_lane(cur_lat,cur_lon,cur_yaw,CL_DIRECTION_RIGHT);


        //进行拨杆抑制
        if(cl_reason == CL_REASON_HAND && direction == CL_DIRECTION_LEFT && lc_lane_num < 1)
        {
            cl_too_busy_avoiding(cl_reason);
        }
        if(cl_reason == CL_REASON_HAND && direction == CL_DIRECTION_RIGHT && rc_lane_num < 1)
        {
            cl_too_busy_avoiding(cl_reason);
        }
        //

        if (direction == CL_DIRECTION_LEFT && lc_lane_num < 1 )//左侧无车道，向左拨杆，禁止换道
        {
           // return send_change_lane_respond(om_change_lane_respond_msg,need_ack, RET_NOT_EXIST,"");
        }
        //从高速道往低速道变道
        if (direction == CL_DIRECTION_RIGHT && lc_lane_num < 1  && cl_too_busy_avoiding(cl_reason))//拨杆状态
        {
            //return send_change_lane_respond(om_change_lane_respond_msg,need_ack, RET_NOT_EXIST,"");
        }

        if (direction == CL_DIRECTION_RIGHT && rc_lane_num < 1 )//右侧无车道，向右拨杆，禁止换道
        {
            //return send_change_lane_respond(om_change_lane_respond_msg,need_ack, RET_NOT_EXIST,"");
        }

        //从低速道往高速道变道
        if (direction == CL_DIRECTION_LEFT && rc_lane_num < 1  && cl_too_busy_avoiding(cl_reason))//拨杆状态
        {
            //return send_change_lane_respond(om_change_lane_respond_msg,need_ack, RET_NOT_EXIST,"");
        }
    }

    //send_change_lane_respond(om_change_lane_respond_msg,true, RET_OK, ""); //允许换道

    return RET_OK;
}

//发送路径规划应答消息
int obu_session_obu::send_change_lane_respond(nad_lcm::om_change_lane_respond &om_change_lane_respond_msg,bool need_ack, int ret, string alarm)
{
    if (need_ack || ret == RET_OK)
    {
        om_change_lane_respond_msg.retcode = ret;
        om_change_lane_respond_msg.ending_mileage = 50;
        g_lcm->publish("om_change_lane_respond", &om_change_lane_respond_msg);
        LOG_SEND(log_om_change_lane_respond(&om_change_lane_respond_msg));

        if (ret == RET_OK)
        {

           route->bind_key_point(route->route_behavior_.cur_point_.index,
                (om_change_lane_respond_msg.direction == CL_DIRECTION_LEFT ? KP_CHANGE_LANE_LEFT : KP_CHANGE_LANE_RIGHT), "");
        }
        else
        {
            alarm_report_to_vui(ALARM_WARNING, alarm);
        }
    }
    return ret;
}

void obu_session_obu::send_ou_vui_report()
{
    nad_lcm::ou_vui_report ou_vui_report_msg;
    save_to_lcm(ou_vui_report_msg.obu);

    ou_vui_report_msg.sug_speed  = sug_speed;

    save_route_kp_to_lcm(
        ou_vui_report_msg.num_of_key, ou_vui_report_msg.key_point,
        ou_vui_report_msg.cur_point_index,
        ou_vui_report_msg.num_of_point, ou_vui_report_msg.line);

    save_to_lcm(ou_vui_report_msg.route);

    g_lcm->publish("ou_vui_report", &ou_vui_report_msg);
}

//保存关键点和轨迹线
void obu_session_obu::save_route_kp_to_lcm(int16_t &num_of_key, std::vector<nad_lcm::key_point_info> &key_point,
        int16_t &cur_point_index, int16_t &num_of_point, std::vector<nad_lcm::route_line_point> &line)
{
    key_point.clear();
    line.clear();
    num_of_key = num_of_point = 0;
    cur_point_index = -1;
    if (route->route_behavior_.center_line_.size() < 2 || route->route_behavior_.key_points_.size() < 2)
    {
        return;
    }

    //寻找关键点
    route::center_point *kp1 = route->route_behavior_.start_kp();
    route::center_point *kp2 = route->route_behavior_.stop_kp();
    if (kp1 == NULL || kp2 == NULL)
    {
        LOG(ERROR) << "start_kp or stop_kp == NULL " << endl;;
        return;
    }
    int i = kp1->index;
    int e = kp2->index;
    while (i <= e)
    {
        route::center_point &cp = route->route_behavior_.center_line_[i];

        //添加轨迹点
        nad_lcm::route_line_point point;
        double lat, lon;
        route->transfer_.cs.xy2ll(cp.x, cp.y, lat, lon);
        point.lat = (float)lat;
        point.lon = (float)lon;
        line.push_back(point);

        //添加关键点
        if (cp.type[KP_ROUTE_START] == 1 || cp.type[KP_ROUTE_STOP] == 1 || cp.type[KP_CURRENT] == 1 || cp.type[KP_CO_CHANGE_LANE] == 1)
        {
            nad_lcm::key_point_info info;
            if (cp.type[KP_CURRENT] == 1)
            {
                info.type = KP_CURRENT;
            }
            if (cp.type[KP_ROUTE_START] == 1)
            {
                info.type = KP_ROUTE_START;
            }
            if (cp.type[KP_ROUTE_STOP] == 1)
            {
                info.type = KP_ROUTE_STOP;
            }
            if (cp.type[KP_CO_CHANGE_LANE] == 1)
            {
                info.type = KP_CO_CHANGE_LANE;
            }
            //LOG(ERROR) << "need to be judged,TOBEDONE " << endl;;
            info.lat = lat;
            info.lon = lon;
            info.mileage = cp.dist(*kp1);
            info.id = cp.id;
            key_point.push_back(info);

            //刷新位置
            if (cp.type[KP_CURRENT] == 1)
            {
                cur_point_index = (int16_t)line.size() - 1;
            }
        }

        //获得下一个轨迹点
        get_next_point(i, e, route->route_behavior_.center_line_);
    }

    //如果cur点在stop之后，则补充一个current点
    if (cur_point_index < 0 && key_point.size() > 0)
    {
        key_point.push_back(key_point[key_point.size() - 1]);
        key_point.back().type = KP_CURRENT;
        cur_point_index = (int16_t)line.size() - 1;
    }

    //保存数组大小
    num_of_key = (int16_t)key_point.size();
    num_of_point = (int16_t)line.size();

    //调试打印
    /*route::RouteBase tmp(g_rsu_planning->osm_map);
    route::center_point pp;
    pp.type = KP_NONE;
    pp.index = pp.left_lane_num = pp.right_lane_num = 0;
    pp.yaw = pp.width = pp.mileage = 0.0;
    pp.object = NULL;
    for (int16_t i = 0; i < num_of_point; i++)
    {
        nad_lcm::route_line_point &point = line[i];
        tmp.transfer.cs.ll2xy(point.lat, point.lon, pp.x, pp.y);
        tmp.center_line.push_back(pp);
    }
    tmp.save_to_osm("../log/save_route_kp_to_lcm.osm",
        line[cur_point_index].lat, line[cur_point_index].lon);*/
}

//在完整切片列表中选取当前obu前1000,后100米的所有切片,route_section包含一次规划完整的切片信息；other 是周期下发给motion的分段信息
void obu_session_obu::send_segment_section_line(nad_lcm::route_planning_m route_section,nad_lcm::route_planning_m &other)
{
    //此函数会定时调用更新，每次下发新的路径时首先更新完整切片信息route_section的cur_section,cur_section,mileage_stop的值
    route_section.cur_section = route->cur_sec_index_;
    if(route_section.cur_section < 0) return; ///小于0时，没有在section中进行定位
    //==求得mileage_start
    route_section.mileage_start = route_section.line[route_section.cur_section].lane[0].mileage;  //通过路径规划的终点经纬度然后调用自己的定位函数得到终点所在的切片点

    //终点的匹配
    double end_dis_min;//匹配终点切片的最短距离
    int end_sec_index = 0;
    int end_lane_index = 0;
    route::center_point end_pt = route->route_behavior_.key_points_[route->route_behavior_.key_points_.size()-1];//最后一个keypoint不一定是终点、、？？？？？
    route->match_section_line_xy(end_pt.x,end_pt.y,end_sec_index,end_lane_index,3,30,end_dis_min);
    if(end_dis_min > 3)
    {
        LOG(ERROR) << "stop_point out of  section line!" << endl;
        cout << "stop_point out of  section line!" << endl;
    }

    //route_section.line[end_sec_pos].lane[0]；最左边边线的里程起点
    //==求得mileage_stop
    route_section.mileage_stop = route_section.line[end_sec_index].lane[0].mileage-route_section.mileage_start;//终点所在的切片点的里程与当前切片点的里程之差就是
    //至此route_planning_m消息填写完毕

    route_plan_section_clear(other);             //清理发送端
    other.time_stamp = route_section.time_stamp;       //时间戳
    other.destination = route_section.destination;      //目的地
    other.route_reason = route_section.route_reason;    //规划原因

    other.mileage_pass = route_section.mileage_pass;    //前500m
    other.mileage_next = route_section.mileage_next;    //后1000m
    other.mileage_start = route_section.mileage_start;
    other.mileage_stop = route_section.mileage_stop;

    //得到当前所在道路信息
    int64_t section_cnt = 0;
    roadmap::lane* route_lane = new roadmap::lane();
    int64_t left_lane_id = route->map_->locate_point_on_lanelet(cur_lat,cur_lon);//left 边界，当前道路的左边界
    route->map_->get_lane_from_map_by_id(left_lane_id,route_lane);
    int lane_size = route_lane->cur_brother_lane.size();

    if(lane_size >= 1)
    {
        int64_t right_lane_id = route_lane->cur_brother_lane[lane_size-1];
        vector<point_with_id_t> nodes_left(route->map_->osm_map_->lanelet_by_id(left_lane_id)->nodes(LEFT));  //当前车辆所在的道路lane_id的左边界线
        vector<point_with_id_t> nodes_right(route->map_->osm_map_->lanelet_by_id(right_lane_id)->nodes(RIGHT));  //当前最右边的车道的右边界限

        bool is_chosen = false;
        for(int i = 0; i < route_section.num_of_section; i ++)
        {
            nad_lcm::section_m  sections;
            memset(&sections,0,sizeof(nad_lcm::section_m));
            for(int j = 0; j < route_section.line[i].num_of_lane; j ++)  //num_of_section字段已经填写满
            {
                //向前1000米向后500米
                if((route_section.line[route_section.cur_section].lane[0].mileage - route_section.line[i].lane[j].mileage <= route_section.mileage_pass
                        && route_section.line[route_section.cur_section].lane[0].mileage - route_section.line[i].lane[j].mileage >=0)
                        || (route_section.line[i].lane[j].mileage - route_section.line[route_section.cur_section].lane[0].mileage <= route_section.mileage_next
                            && route_section.line[i].lane[j].mileage - route_section.line[route_section.cur_section].lane[0].mileage >= 0))
                {
                    is_chosen = true;//为了不在同一个切片上的多个切片点循环重复添加多次
                    {
                        sections.lane.push_back(route_section.line[i].lane[j]);
                        sections.num_of_lane ++;
                        if(route_section.line[route_section.cur_section].lane[j].mileage == route_section.line[i].lane[j].mileage)
                            other.cur_section = section_cnt;   //修正因提取部分切片后cur_section的值
                    }
                }
            }

            if(sections.num_of_lane > 0)   //切片有点值才push
            {
                other.line.push_back(sections);
                other.num_of_section++;
                section_cnt++;   //用于计算截取后cur_section变化后的值
            }

            if(is_chosen)  //为了不在同一个切片上的多个切片点循环重复添加多次
            {
                if(route->blane_lborder_map_.find(i) != route->blane_lborder_map_.end())
                {
                    route::Point_m pt = route->blane_lborder_map_[i];
                    nad_lcm::point_xys point_l;
                    point_l.x = pt.x;
                    point_l.y = pt.y;
                    point_l.type = KP_NORMAL;
                    other.left_line.line.push_back(point_l);
                    other.left_line.num_of_point ++;
                }
                else
                {
                    cout << "sec_num "<<i<<" has no left_line point!"<<endl;
                }

                if(route->blane_rborder_map_.find(i) != route->blane_rborder_map_.end())
                {
                    route::Point_m pt = route->blane_rborder_map_[i];
                    nad_lcm::point_xys point_r;
                    point_r.x = pt.x;
                    point_r.y = pt.y;
                    point_r.type = KP_NORMAL;
                    other.right_line.line.push_back(point_r);
                    other.right_edge.line.push_back(point_r);
                    other.right_line.num_of_point ++;
                    other.right_edge.num_of_point ++;
                }
                else
                {
                    cout << "sec_num "<<i<<" has no right_line and right_edge point!"<<endl;
                }

                if(route->changelane_lborder_map_.find(i) != route->changelane_lborder_map_.end())
                {
                    route::Point_m pt = route->changelane_lborder_map_[i];
                    nad_lcm::point_xys point_rever_r;
                    point_rever_r.x = pt.x;
                    point_rever_r.y = pt.y;
                    point_rever_r.type = KP_NORMAL;
                    other.left_edge.line.push_back(point_rever_r);   //此处增加了left_edge
                    other.left_edge.num_of_point ++;
                }
                else
                {
                    cout << "sec_num "<<i<<" has no left_edge point!"<<endl;
                }

                is_chosen = false;
            }
        }
    }
    else
    {
        LOG(ERROR) << "No Route is found!" << endl;
        cout << "No Route is found!" << endl;
    }
    delete route_lane;
}

//给vui下发路径规划结果
void obu_session_obu::send_ou_route_respond(int retcode, nad_lcm::route_planning route)
{
    nad_lcm::ou_route_respond ou_route_respond_msg;
    ou_route_respond_msg.obu_name = name;
    ou_route_respond_msg.retcode = retcode;
    ou_route_respond_msg.route = route;//revise
    g_lcm->publish("ou_route_respond", &ou_route_respond_msg);
    LOG_SEND(log_ou_route_respond(&ou_route_respond_msg));
    cout << "send_ou_route_respond: 已给vui下发路径规划结果" << endl;
}

//给vui下发启动自动驾驶结果，也可用于主动下发启动自动驾驶
void obu_session_obu::send_ou_start_auto_respond(int32_t retcode, int32_t start_reason)
{
    //切换状态
    if (retcode == RET_OK)
    {
        session_status = E_OBU_SESSION_OBU_CONNECT_AUTO;
        start_auto_status = E_OBU_SESSION_OBU_START_AUTO_OK;
    }

    //下发切换消息
    nad_lcm::ou_start_auto_respond ou_start_auto_respond_msg;
    ou_start_auto_respond_msg.obu_name = name;
    ou_start_auto_respond_msg.retcode = retcode;
    ou_start_auto_respond_msg.start_reason = start_reason;
    g_lcm->publish("ou_start_auto_respond", &ou_start_auto_respond_msg);
    LOG_SEND(log_ou_start_auto_respond(&ou_start_auto_respond_msg));
    cout << "send_ou_start_auto_respond: 已给vui下发启动自动驾驶应答" << endl;
}

//给vui下发退出自动驾驶结果，也可用于主动下发退出自动驾驶
void obu_session_obu::send_ou_stop_auto_respond(int32_t retcode, int32_t stop_reason)
{
    //切换状态
    if (retcode == RET_OK)
    {
        session_status = E_OBU_SESSION_OBU_CONNECT_HAND;
    }

    //下发切换消息
    nad_lcm::ou_stop_auto_respond ou_stop_auto_respond_msg;
    ou_stop_auto_respond_msg.obu_name = name;
    ou_stop_auto_respond_msg.retcode = retcode;
    ou_stop_auto_respond_msg.stop_reason = stop_reason;
    g_lcm->publish("ou_stop_auto_respond", &ou_stop_auto_respond_msg);
    LOG_SEND(log_ou_stop_auto_respond(&ou_stop_auto_respond_msg));
    cout << "send_ou_stop_auto_respond: 已给vui下发退出自动驾驶应答" << endl;
}

//接收vui上传的uo_route_request
void obu_session_obu::handle_uo_route_request(const lcm::ReceiveBuffer* rbuf,
        const std::string& chan, const nad_lcm::uo_route_request *msg)
{
    LOG_RECV(log_uo_route_request(msg));

    //路径规划请求消息是有效的消息，并且obu是在线的
    if (msg->destination != "" && (session_status == E_OBU_SESSION_OBU_CONNECT_HAND || session_status == E_OBU_SESSION_OBU_CONNECT_AUTO))
    {
        route->destination_ = msg->destination;

        //关键点赋值
        vector<route::key_point> key_point_list;
        for( int i = 0; i < msg->key_point_list.size(); i++)
        {
            route::key_point key_point_tmp;
            key_point_tmp.lat= msg->key_point_list[i].lat;
            key_point_tmp.lon= msg->key_point_list[i].lon;
            key_point_tmp.type = msg->key_point_list[i].type;
            key_point_tmp.yaw = msg->key_point_list[i].yaw;
            key_point_list.push_back(key_point_tmp);
        }

        ///重新规划开始之后，不再下发切片至motion
        route_plan_status = false;

        size_t begin = 0;

        int ret = route->routing(key_point_list, ROUTE_REASON_VUI, msg->destination, cur_lat, cur_lon, cur_yaw, begin);

        nad_lcm::route_planning tmp_route;
        save_to_lcm(tmp_route);
        //上报日志
        if(ret != RET_OK)
        {
            log_report_to_vui(LOG_INFO, "路径规划失败");

            if(ret == RET_NOT_EXIST)
                alarm_report_to_vui(ALARM_WARNING, "路径规划失败，车辆偏离地图！");
            if(ret == RET_ROUTE_NOT_MATCH)
                alarm_report_to_vui(ALARM_WARNING, "路径规划失败，无直达路径！");
            if(ret == RET_ERROR)
                alarm_report_to_vui(ALARM_WARNING, "路径规划失败！");

            //send_ou_route_respond(ret,tmp_route);
        }
        else
        {
            ///规划成功之后，再下发切片至motion
            route_plan_status = true;
            log_report_to_vui(LOG_INFO, "路径规划成功");

            //route->lane_to_section_line(cur_lon,cur_lat);
            //send_om_section_line_report(IS_REPLAN);

            //绘图
            if(route_draw_flag == true)
            {
                route_draw[0] = route;
                //route_draw[0]->save_section_to_draw(route);
                route_draw_flag = false;
            }
            else
            {
                route_draw[1] = route;
                //route_draw[1]->save_section_to_draw(route);
                route_draw_flag = true;
            }

            obu_lcm::back_coordinate_XYH back_coor_msg;

            back_coor_msg.length = 6;

            //找终点
            for(int i = 0; i < route->route_behavior_.key_points_.size(); i++)
            {
                if(route->route_behavior_.key_points_[i].type[KP_ROUTE_STOP] == 1)
                {
                    cout << "KP_ROUTE_STOP x:" <<  route->route_behavior_.key_points_[i].x << "  y:" << route->route_behavior_.key_points_[i].y << endl;
                    back_coor_msg.x = route->route_behavior_.key_points_[i].x;
                    back_coor_msg.y = route->route_behavior_.key_points_[i].y;//获得终点坐标
                    back_coor_msg.heading =  route->route_behavior_.key_points_[i].yaw;
                    back_coor_msg.width = route->route_behavior_.key_points_[i].width;
                }
            }

            if(route->destination_.find("泊车") != string::npos)
            {
                back_coor_msg.type = 11;
            }
            else
            {
                back_coor_msg.type = 10;
            }


            g_lcm->publish("back_coordinate_XYH", &back_coor_msg);

            //back_coordinate_XYH下发记录
            LOG(ERROR) << "back_coordinate_XYH start "<< route->destination_ ;

        }
        log_report_to_vui(LOG_INFO, "请求路径规划回复");

        send_ou_route_respond(ret,tmp_route);
    }
}

//接收vui上传的uo_start_auto_request
void obu_session_obu::handle_uo_start_auto_request(const lcm::ReceiveBuffer* rbuf,
        const std::string& chan, const nad_lcm::uo_start_auto_request *msg)
{
    LOG_RECV(log_uo_start_auto_request(msg));
    if(name == msg->obu_name)
    {
        if(msg->time_stamp != route->time_stamp_)   //时戳不一致，直接向obu返回启动自动驾驶失败，不用上报csu
        {
            send_ou_start_auto_respond(RET_ERROR,START_REASON_VUI);
            is_auto_drive = false;
        }
        else
        {
            send_ou_start_auto_respond(RET_OK,START_REASON_VUI);
            is_auto_drive = true;
        }
    }
    log_report_to_vui(LOG_INFO, "请求启动自动驾驶");
}


//接收vui上报的uo_stop_auto_request
void obu_session_obu::handle_uo_stop_auto_request(const lcm::ReceiveBuffer* rbuf,
        const std::string& chan, const nad_lcm::uo_stop_auto_request *msg)
{
    LOG_RECV(log_uo_stop_auto_request(msg));
    int retcode = RET_OK;

    if(name != msg->obu_name)
    {
        retcode = RET_ERROR;
        log_report_to_vui(LOG_INFO, "退出自动驾驶失败");
    }

    log_report_to_vui(LOG_INFO, "退出自动驾驶成功");
    //启动自动驾驶的应答
    send_ou_stop_auto_respond(retcode, STOP_REASON_VUI);
    is_auto_drive = false;
}

//读取control_info_report
void obu_session_obu::handle_control_info_report(const lcm::ReceiveBuffer* rbuf,
        const std::string& chan, const obu_lcm::control_info_report *msg)
{
    #if 0
    //保存controller上报的信息
    cur_lon = msg->cur_lon;
    cur_lat = msg->cur_lat;
    cur_yaw = msg->cur_yaw;
    cur_brake = msg->cur_brake;
    cur_speed = msg->cur_speed * 3.6;
    cur_acceleration = msg->cur_acceleration;
    steering_angle = msg->steering_angle;
    flashing_status = msg->flashing_status;
    cur_gears = msg->cur_gears;
    gps_time = msg->gps_time;

    eps_stat = msg->eps_stat;
    epb_stat = msg->epb_stat;
    brake_stat = msg->brake_stat;
    //只有gps是真数据
    if(msg->gps_stat.size() > 0)
    {
        sensor_stat[IDX_GPS_CENTER] = msg->gps_stat[0];

    }
#endif // 0
    cur_lon = msg->lon;
    cur_lat = msg->lat;
    cur_yaw = msg->yaw;
    cur_brake = msg->brake_value;
    cur_speed = msg->vehicle_speed * 3.6;
    cur_acceleration = msg->accel_value;
    steering_angle = msg->steer_angle;
    flashing_status = msg->flashing_status;
    cur_gears = msg->at_status;
    gps_time = msg->gps_time;
    // eps_stat = msg->eps_stat;
    eps_stat = msg->steer_status;
    epb_stat = msg->epb_status;
    brake_stat = msg->brake_value; //只有gps是真数据
    // if(msg->gps_stat.size() > 0)
    // {
    // sensor_stat[IDX_GPS_CENTER] = msg->gps_stat[0];
    //
    // }
    sensor_stat[0] = 0;

    //刷新车辆位置
    route->locate_on_lane(cur_lat, cur_lon);
}

//motionplanner请求换道
void obu_session_obu::handle_mo_change_lane_request(const lcm::ReceiveBuffer* rbuf,
        const std::string& chan, const nad_lcm::mo_change_lane_request *msg)
{
    LOG_RECV(log_mo_change_lane_request(msg));
    change_lane_ex(msg->direction, msg->reason, msg->starting_lane,
                   msg->ending_lane, true, false, true, false, false, true, true);

}

//注册消息到lcm的函数
void obu_session_obu::reg_msg_to_lcm(NE_LCM* lcm)
{
    //name = g_config.local.name;
    nad_record_obu *rec = db_query_obu(name);
    if (rec == NULL)
    {
        LOG(ERROR) << "obu_session_obu: 在数据库中查找不到OBU: " << name.c_str();
    }
    else
    {
        obu_type = rec->obu_type;
    }
    lcm->subscribe("uo_route_request", &obu_session_obu::handle_uo_route_request, this);
    lcm->subscribe("uo_start_auto_request", &obu_session_obu::handle_uo_start_auto_request, this);
    lcm->subscribe("uo_stop_auto_request", &obu_session_obu::handle_uo_stop_auto_request, this);
    lcm->subscribe("control_info_report", &obu_session_obu::handle_control_info_report, this);
    lcm->subscribe("mo_change_lane_request", &obu_session_obu::handle_mo_change_lane_request, this);
}

//构造析构函数
obu_session_obu_timer::obu_session_obu_timer() : nad_timer(200)
{
}

obu_session_obu_timer::obu_session_obu_timer(int64 interval_ms):nad_timer(interval_ms)
{
}

obu_session_obu_timer::~obu_session_obu_timer()
{
}

//执行定时器
void obu_session_obu_timer::handle()
{
    g_obu_planning->obu.handle_timer(counter);
    g_obu_planning->obu.calc_mileage_section_acc(); //路径中切片信息的相关字段更新
}

void obu_session_obu::send_om_section_line_report(int replay_flag)
{
    if(route->route_motion_info_.num_of_section > 0 && route->cur_sec_index_ > -1)//具有定位信息后，再下发定时的规划信息
    {
        nad_lcm::om_route_respond om_route_respond_msg;
        om_route_respond_msg.replan_flag = replay_flag;
        //发送重规划消息
        nad_lcm::route_planning_m route_m;
        nad_lcm::route_planning_m route_m_draw;
        planning_m_2_lcm(route->route_motion_info_ , route_m);//data->lcm消息
        send_segment_section_line(route_m,om_route_respond_msg.route);
        send_segment_section_line(route_m,route_m_draw);  //obu绘制切片用到
        lcm_2_planning_m(route_m_draw,route->route_motion_info_draw_);
        g_lcm->publish("om_route_respond", &om_route_respond_msg);
    }
}

//分段下发路径
void obu_session_obu::calc_mileage_section_acc()  //每隔450m刷新
{
    double mileage_tmp = 0.0;

    if ((route->route_motion_info_.line.size() == 0) || route_plan_status == false)
    {
        return;
    }
    route->locate_position_on_section_ll(cur_lat,cur_lon); //当前所在切片位置更新
    if(route->cur_sec_index_ >= 0)
    {
        mileage_tmp = route->route_motion_info_.line[route->cur_sec_index_].lane[0].mileage;
    }
    ///mileage_section_acc表示上次规划时的里程
    ///mileage_section_send 初始化
    /*std::cout << "zws: cur_section=" << route->cur_sec_index_ <<"   mileage_tmp="<< mileage_tmp <<"  -----------"
                  << "  route->mileage_section_send_="<<route->mileage_section_send_<<endl;*/
    if(route->cur_sec_index_ >= 0 && (mileage_tmp - route->mileage_section_send_ >= 500 ||  route->mileage_section_send_ < 0))
    {
        if( route->mileage_section_send_ < 0) //第一次下发,每次route，重新下发会清零
        {
            send_om_section_line_report(IS_REPLAN);
        }
        else
        {
            send_om_section_line_report(IS_NOT_REPLAN);
        }

        route->mileage_section_send_= mileage_tmp;
    }
    #if 0
    if(route->cur_sec_index_ >= 0)
    {
        cout << "route->cur_sec_index_:" << route->cur_sec_index_ << endl;
        mileage_tmp = route->route_motion_info_.line[route->cur_sec_index_].lane[0].mileage;
        /*std::cout << "cur_section="<<cur_sec_pos<<" mileage_tmp="<<mileage_tmp<<" -----------"
                  << " route->mileage_section_send="<<route->mileage_section_send<<endl;*/
    }
    std::cout << " mileage_tmp="<<mileage_tmp<<" -----------"<< " route->mileage_section_send_="<<route->mileage_section_send_
            << endl << "(mileage_tmp - route->mileage_section_send_)" << mileage_tmp - route->mileage_section_send_ << endl;
    if(mileage_tmp - route->mileage_section_send_ >= 500)  //mileage_section_acc表示上次规划时的里程
    {
        route->mileage_section_send_ = mileage_tmp;
        send_om_section_line_report(IS_NOT_REPLAN);
    }
    #endif // 0

    if(route_draw_flag == true)                    //更新绘图
    {
        route_draw[0] = route;
        //route_draw[0]->save_section_to_draw(route);
        route_draw_flag = false;
    }
    else
    {
        route_draw[1] = route;
        //route_draw[1]->save_section_to_draw(route);
        route_draw_flag = true;
    }
}

void obu_session_obu::route_plan_section_clear(nad_lcm::route_planning_m &route_sec)
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

void obu_session_obu::planning_m_2_lcm(route::route_planning_m & planning_m, nad_lcm::route_planning_m &planning_m_lcm)
{
    planning_m_lcm.time_stamp = planning_m.time_stamp;
    planning_m_lcm.destination = planning_m.destination;
    planning_m_lcm.route_reason = planning_m.route_reason;
    planning_m_lcm.mileage_pass = planning_m.mileage_pass;
    planning_m_lcm.mileage_next = planning_m.mileage_next;
    planning_m_lcm.mileage_start = planning_m.mileage_start;
    planning_m_lcm.mileage_stop = planning_m.mileage_stop;
    planning_m_lcm.num_of_section = planning_m.num_of_section;
    planning_m_lcm.cur_section = planning_m.cur_section;
    planning_m_lcm.left_edge.num_of_point = planning_m.left_edge.num_of_point;
    planning_m_lcm.left_line.num_of_point = planning_m.left_line.num_of_point;
    planning_m_lcm.right_edge.num_of_point = planning_m.right_edge.num_of_point;
    planning_m_lcm.right_line.num_of_point = planning_m.right_line.num_of_point;

    for(int i = 0; i < planning_m.num_of_section; i++)
    {
        nad_lcm::section_m sec_m;
        sec_m.num_of_lane = planning_m.line[i].num_of_lane;

        for(int j = 0; j < planning_m.line[i].num_of_lane; j++)
        {
            nad_lcm::point_m pt_m;
            pt_m.x = planning_m.line[i].lane[j].x;
            pt_m.y = planning_m.line[i].lane[j].y;
            pt_m.type = planning_m.line[i].lane[j].type;
            pt_m.yaw = planning_m.line[i].lane[j].yaw;
            pt_m.k = planning_m.line[i].lane[j].k;
            pt_m.mileage = planning_m.line[i].lane[j].mileage;
            pt_m.width = planning_m.line[i].lane[j].width;
            pt_m.sug_speed = planning_m.line[i].lane[j].sug_speed;
            sec_m.lane.push_back(pt_m);
        }
        planning_m_lcm.line.push_back(sec_m);
    }

    for(int i = 0; i < planning_m.left_edge.line.size(); i++)
    {
        nad_lcm::point_xys pt;
        pt.x = planning_m.left_edge.line[i].x;
        pt.y = planning_m.left_edge.line[i].y;
        pt.type = planning_m.left_edge.line[i].type;
        planning_m_lcm.left_edge.line.push_back(pt);
    }

    for(int i = 0; i < planning_m.left_line.line.size(); i++)
    {
        nad_lcm::point_xys pt;
        pt.x = planning_m.left_line.line[i].x;
        pt.y = planning_m.left_line.line[i].y;
        pt.type = planning_m.left_line.line[i].type;
        planning_m_lcm.left_line.line.push_back(pt);
    }

    for(int i = 0; i < planning_m.right_edge.line.size(); i++)
    {
        nad_lcm::point_xys pt;
        pt.x = planning_m.right_edge.line[i].x;
        pt.y = planning_m.right_edge.line[i].y;
        pt.type = planning_m.right_edge.line[i].type;
        planning_m_lcm.right_edge.line.push_back(pt);
    }

    for(int i = 0; i < planning_m.right_line.line.size(); i++)
    {
        nad_lcm::point_xys pt;
        pt.x = planning_m.right_line.line[i].x;
        pt.y = planning_m.right_line.line[i].y;
        pt.type = planning_m.right_line.line[i].type;
        planning_m_lcm.right_line.line.push_back(pt);
    }

}

void obu_session_obu::lcm_2_planning_m(nad_lcm::route_planning_m & planning_m_lcm, route::route_planning_m &planning_m)
{
    planning_m.time_stamp = planning_m_lcm.time_stamp;
    planning_m.destination = planning_m_lcm.destination;
    planning_m.route_reason = planning_m_lcm.route_reason;
    planning_m.mileage_pass = planning_m_lcm.mileage_pass;
    planning_m.mileage_next = planning_m_lcm.mileage_next;
    planning_m.mileage_start = planning_m_lcm.mileage_start;
    planning_m.mileage_stop = planning_m_lcm.mileage_stop;
    planning_m.num_of_section = planning_m_lcm.num_of_section;
    planning_m.cur_section = planning_m_lcm.cur_section;
    planning_m.left_edge.num_of_point = planning_m_lcm.left_edge.num_of_point;
    planning_m.left_line.num_of_point = planning_m_lcm.left_line.num_of_point;
    planning_m.right_edge.num_of_point = planning_m_lcm.right_edge.num_of_point;
    planning_m.right_line.num_of_point = planning_m_lcm.right_line.num_of_point;

    for(int i = 0; i < planning_m_lcm.num_of_section; i++)
    {
        route::section_m sec_m;
        sec_m.num_of_lane = planning_m_lcm.line[i].num_of_lane;

        for(int j = 0; j < planning_m_lcm.line[i].num_of_lane; j++)
        {
            route::point_m pt_m;
            pt_m.x = planning_m_lcm.line[i].lane[j].x;
            pt_m.y = planning_m_lcm.line[i].lane[j].y;
            pt_m.type = planning_m_lcm.line[i].lane[j].type;
            pt_m.yaw = planning_m_lcm.line[i].lane[j].yaw;
            pt_m.k = planning_m_lcm.line[i].lane[j].k;
            pt_m.mileage = planning_m_lcm.line[i].lane[j].mileage;
            pt_m.width = planning_m_lcm.line[i].lane[j].width;
            pt_m.sug_speed = planning_m_lcm.line[i].lane[j].sug_speed;
            sec_m.lane.push_back(pt_m);
        }
        planning_m.line.push_back(sec_m);
    }

    for(int i = 0; i < planning_m_lcm.left_edge.line.size(); i++)
    {
        route::point_xys pt;
        pt.x = planning_m_lcm.left_edge.line[i].x;
        pt.y = planning_m_lcm.left_edge.line[i].y;
        pt.type = planning_m_lcm.left_edge.line[i].type;
        planning_m.left_edge.line.push_back(pt);
    }

    for(int i = 0; i < planning_m_lcm.left_line.line.size(); i++)
    {
        route::point_xys pt;
        pt.x = planning_m_lcm.left_line.line[i].x;
        pt.y = planning_m_lcm.left_line.line[i].y;
        pt.type = planning_m_lcm.left_line.line[i].type;
        planning_m.left_line.line.push_back(pt);
    }

    for(int i = 0; i < planning_m_lcm.right_edge.line.size(); i++)
    {
        route::point_xys pt;
        pt.x = planning_m_lcm.right_edge.line[i].x;
        pt.y = planning_m_lcm.right_edge.line[i].y;
        pt.type = planning_m_lcm.right_edge.line[i].type;
        planning_m.right_edge.line.push_back(pt);
    }

    for(int i = 0; i < planning_m_lcm.right_line.line.size(); i++)
    {
        route::point_xys pt;
        pt.x = planning_m_lcm.right_line.line[i].x;
        pt.y = planning_m_lcm.right_line.line[i].y;
        pt.type = planning_m_lcm.right_line.line[i].type;
        planning_m.right_line.line.push_back(pt);
    }
}

//设置默认速度
void obu_session_obu::set_default_speed()
{
    if (is_auto_drive)
    {
        speed.add("no_route", SV_HIGH, SV_ABSOLUTE, 0.0);
    }
    else
    {
        speed.add("hand_drive", SV_LOW, SV_ABSOLUTE, 20.0);
    }
}

//速度处理
void obu_session_obu::handle_speed()
{
    //清空速度规划
    speed.clear();

    //不处理没有路径规划的情况
    if (route->route_behavior_.center_line_.size() == 0)
    {
        set_default_speed();
        sug_speed = speed.speed;
        return;
    }

    route->locate_on_lane(cur_lat, cur_lon);
    route::center_point &cp = route->route_behavior_.cur_point_;
    //绑定结束，恢复keypoint列表
    route->route_behavior_.get_keypoint_on_center_line();
    route::center_point *ep = route->route_behavior_.get_kp(KP_ROUTE_STOP);
    if (ep == NULL || ep->index <= cp.index)
    {

        set_default_speed();
        sug_speed = speed.speed;

        return;
    }
    //初始化速度值
    string id = "";
    double lim = (double)route->route_behavior_.get_current_limspeed(id);
    ///////////////////////////////////////////////////////add for test/////////////////
    //lim = 30.0;
    if (sub_count(id, "|") == 1)
    {
        speed.add("limspeed", SV_LOW, SV_ABSOLUTE, lim);
    }
    else
    {
        speed.add("lane_max_speed", SV_LOW, SV_ABSOLUTE, lim);
    }
    sug_speed = speed.speed;
}

