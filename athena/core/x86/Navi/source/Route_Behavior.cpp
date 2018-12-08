/**
* @file       Route_Behavior.cpp
* @brief      导航模块
* @details
* @author     huanhuan
* @date       2018/7/16
* @version    v1.0
* @par Copyright (c):
*      武汉环宇智行科技有限公司
* @par History:
*   version: author, date, desc\n
*/

#include "route_data.h"
#include <time.h>
#include <sys/time.h>
using namespace athena;
using namespace route;


///center_point构造函数
center_point::center_point()
{
    x = 0.0;
    y = 0.0;
    index = -1;
    yaw = 0.0;
    width = 0.0;
    width = 0.0;
    mileage = 0.0;
    lane_id = 0;
    left_lane_num = 0;
    right_lane_num = 0;
    id = "";
    road_level = 0;

    for (int i = 0; i < MAX_KP_TYPE; i++)
    {
        type[i] = 0;
    }
}

///距离某个点的距离
double center_point::dist(center_point &cp)
{
    return mileage - cp.mileage;
}

///把center_line中的关键点保存到key_ponts中
void Route_Behavior::get_keypoint_on_center_line()
{
    key_points_.clear();
    cur_key_point_index_ = -1;
    for(size_t i = 0; i < center_line_.size(); i++)
    {
        for (int j = 1; j < MAX_KP_TYPE; j++)
        {
            if (1 == center_line_[i].type[j])
            {
                //std::cout << "start  get_keypoint_on_center_line ,index = " << i << std::endl << std::endl;
                key_points_.push_back(center_line_[i]);
                break;
            }
        }

        if(center_line_[i].type[KP_ROUTE_START] == 1)
        {
            cur_key_point_index_ = i;
        }
        if (center_line_[i].type[KP_CURRENT] == 1)
        {
            cur_key_point_index_ = key_points_.size() - 1;
        }
    }
}

//删除可以动态绑定的关键点(只保留起点、终点、NORMAL点、换道点)
void Route_Behavior::clear_active_keypoint_on_center_line()
{
    for(size_t i = 0; i < center_line_.size(); i++)
    {
        for (size_t j = 0; j < MAX_KP_TYPE; j++)
        {
            if ((KP_PARKING == j)     ||
                (KP_NORMAL == j)      ||
                (KP_ROUTE_START == j) ||
                (KP_ROUTE_STOP == j)  ||
                (KP_CHANGE_LANE_LEFT == j)  ||
                (KP_CHANGE_LANE_RIGHT == j) ||
                (KP_CO_CHANGE_LANE == j))
            {
                continue;
            }
            else
            {
                //std::cout << "reset key type index = " << j << std::endl << std::endl;
                center_line_[i].type[j] = 0;
                center_line_[i].id = "";
            }
        }
    }
}

///从头开始全程查找关键点
center_point *Route_Behavior::get_kp(int type1, int type2,
                                    int type3, int type4, int type5, int type6)
{
    if ((type1 >= MAX_KP_TYPE) || (type2 >= MAX_KP_TYPE) || (type3 >= MAX_KP_TYPE) ||
        (type4 >= MAX_KP_TYPE) || (type5 >= MAX_KP_TYPE) || (type6 >= MAX_KP_TYPE))
    {
        return NULL;
    }
    for (int i = 0; i < (int)key_points_.size(); i++)
    {
        center_point &kp = key_points_[i];
        if (kp.type[type1] == 1 || kp.type[type2] == 1 || kp.type[type3] == 1 ||
                kp.type[type4] == 1 || kp.type[type5] == 1 || kp.type[type6] == 1)
        {
            return &kp;
        }
    }
    return NULL;
}


///查找以cur_point为原点，前后多少米的点，负数为前(向起点)，整数位后(向终点)
center_point *Route_Behavior::get_cp(double diff)
{
    if (cur_center_line_index_ < 0 || cur_center_line_index_ >= (int)center_line_.size())
    {
        return NULL;
    }

    int index = cur_center_line_index_;
    if (diff < -0.5)
    {
        //查找cur_point之前的点
        for (index = cur_center_line_index_ - 1; index > 0; index--)
        {
            if (center_line_[index].dist(cur_point_) <= diff)
            {
                break;
            }
        }
    }
    else if (diff > 0.5)
    {
        //查找cur_point之后的点
        for (index = cur_center_line_index_ + 1; index < (int)center_line_.size() - 1; index++)
        {
            if (center_line_[index].dist(cur_point_) >= diff)
            {
                break;
            }
        }
    }
    return &center_line_[index];
}


//把限速牌绑定到center_line上
int Route_Behavior::bind_limspeed_on_center_line(std::string limspeed_id, int64_t lane_id, double &dis_min)
{
/*
    if(route_behavior_.center_line_.size() == 0)
    {
        return RET_ERROR;
    }

    //查找中心线上有没有此ets
    double lon, lat;
    get_lon_lat(lon, lat, limspeed_id);
    roadmap::limspeed limspeed;
    limspeed.active_index = 0;
    int cnt = 0;
    while (limspeed.active_index < (int)route_behavior_.center_line_.size())
    {
        int return_active = match_center_line_ll(lat, lon, limspeed.active_index, 15.0, 15.0, lane_id, dis_min);

        if (return_active != RET_OK)
        {
//           if ( RET_NOT_SAME == return_active)
//           {
//               cout<<"limspeed lon : "<<lon<<" lat : "<<lat<<endl;
//           }

            return return_active;
        }

        //绑定成功的次数, 绑定次数多, 有何坏处？？？
//        if (RET_OK == return_active)
//        {
//            cnt++;
//            cout<<"-----------------------action----2-----limspeed------------------------"<<endl;
//            cout << "limspeed bind ok count : " << cnt <<endl;
//            cout<<"-----------------------action----2-----limspeed------------------------"<<endl;
//        }


        int ret = bind_key_point(limspeed.active_index, KP_LIMSPEED, limspeed_id);
        if (ret != RET_OK)
        {
            return ret;
        }

        //查找route是否管理此ets
        nad_limspeed *it = find_limspeed(limspeed_id);
        if (it == NULL)
        {
            limspeed.limspeed_id = limspeed_id;
            limspeed_list.push_back(limspeed);
        }
        else
        {
            it->active_index = limspeed.active_index;
        }

        //绕圈路线limspeed绑定多次
        limspeed.active_index += 5;
    }
*/
    return RET_OK;
}


///查找当前生效的限速点，同时修改了limspeed_value，返回生成速度的limspeed_id或lane_id
int Route_Behavior::get_current_limspeed(std::string &id, double diff)
{
    id = "";
    if (cur_key_point_index_ < 0 || key_points_.size() == 0)
    {
        return limspeed_value;
    }

    ///计算速度点，可能向前向后偏移
    center_point *point = get_cp(diff);//存在指针为空

    ///循环查找限速牌，限速牌比道路本身的限速优先
    for (size_t i = 0; i < key_points_.size(); i++)
    {
        center_point &kp = key_points_[i];
        if (kp.mileage > point->mileage)
        {
            break;
        }
        else if (kp.type[KP_LIMSPEED] == 1)  //查找cur_point的后diff距离内的中心线点是否存在KP_LIMSPEED类型
        {
            roadmap::limspeed *limspeed = find_limspeed(kp.id);
            if (limspeed != NULL)
            {
                id = limspeed->limspeed_id;
                limspeed_value = limspeed->limspeed_value;
            }
        }
        else if (kp.type[KP_W_LIMSPEED_OFF] == 1)
        {
            id = ""; //取消限速
        }
    }
    if (id != "")
    {
        //std::cout << "find limspeed point, limspeed_value=" << limspeed_value << std::endl;
        return limspeed_value;
    }

    ///如果没有找到限速牌，则使用道路本身的限速
    for (size_t i = 0; i < lanelist_.size(); i++)
    {
        roadmap::lane &lane = lanelist_[i];
        std::vector<int64_t>::iterator it = std::find(
                                           lane.cur_brother_lane.begin(), lane.cur_brother_lane.end(), point->lane_id);
        if (it != lane.cur_brother_lane.end())
        {
            char buf[20];
            sprintf(buf, "%ld", lane.lane_id);
            id = buf;
            limspeed_value = lane.max_speed;
            break;
        }
    }

    return limspeed_value;
}

///查找限速牌
roadmap::limspeed *Route_Behavior::find_limspeed(std::string limspeed_id)
{
    std::vector<roadmap::limspeed>::iterator it = std::find_if(
            limspeed_list_.begin(), limspeed_list_.end(), LimspeedInLimspeedList(limspeed_id));
    if (it != limspeed_list_.end())
    {
        return &(*it);
    }
    return NULL;
}

//设置lanelist
void Route_Behavior::set_lanelist(std::vector<roadmap::lane> &lanelist)
{
    lanelist_.clear();
    lanelist_.assign(lanelist.begin(), lanelist.end());
}

//获得起点指针
center_point *Route_Behavior::start_kp()
{
    for (int i = 0; i < (int)key_points_.size(); i++)
    {
        if (key_points_[i].type[KP_ROUTE_START] == 1)
        {
            return &key_points_[i];
        }
    }
    return NULL;
}

//获得终点指针
center_point *Route_Behavior::stop_kp()
{
    for (int i = (int)key_points_.size() - 1; i >= 0; i--)
    {
        if (key_points_[i].type[KP_ROUTE_STOP] == 1)
        {
            return &key_points_[i];
        }
        else if (key_points_[i].type[KP_PARKING] == 1)
        {
            return &key_points_[i];
        }
    }
    return NULL;
}

//查找当前点是否允许换道，返回NULL表示允许换道
bool Route_Behavior::get_current_forbid_change()
{
    bool ret = false;
    if (cur_key_point_index_ < 0 || key_points_.size() == 0)
    {
        return ret;
    }

    //循环查找禁止换道指示
    for (size_t i = 0; i < key_points_.size(); i++)
    {
        center_point &kp = key_points_[i];
        if (kp.type[KP_W_CHANGE_OFF] == 1)
        {
            ret = true;
        }
        if (kp.type[KP_W_CHANGE_ON] == 1)
        {
            ret = false;
        }
        if (kp.type[KP_CURRENT] == 1)
        {
            return ret;
        }
    }
    return ret;
}
