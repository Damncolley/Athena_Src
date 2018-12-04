/*-------------------------------------------------------
 * 文件名：nad_warning.cpp
 * 创建者：张毅00151602
 * 时  间：2016-10-11
 * 描  述：道路上的预警点
-------------------------------------------------------*/

//头文件
#include "nad_warning.h"

//从数据库中加载预警点
void nad_warning_list::load_from_db()
{
    list.clear();
    nad_warning warning;
    map<string, nad_record_warning> &warnings = db_query_all_warning();
    map<string, nad_record_warning>::iterator it = warnings.begin();
    for (; it != warnings.end(); it++)
    {
        nad_record_warning &rec = it->second;
        warning.id = rec.warning_id;
        warning.type = rec.warning_type;
        warning.desc = rec.warning_desc;
        warning.lane_id = rec.lane_id;
        get_lon_lat(warning.lon, warning.lat, warning.id);
        transfer.cs.ll2xy(warning.lat, warning.lon, warning.x, warning.y);
        list.push_back(warning);
    }
    LOG(WARNING) << "加载预警点 " << list.size() << " 个";
}

//把预警点绑定到route::RouteBase上
void nad_warning_list::bind_key_point(route::RouteBase &route)
{
    if (route.route_behavior_.cur_point_.index < 0)
    {
        return;
    }

    //绑定每一个点到route上
    for (size_t i = 0; i < list.size(); i++)
    {
        nad_warning *warning = &list[i];
        int index = 0;
        while (index < (int)route.route_behavior_.center_line_.size())
        {
            //查找点
            double dis_min = 0;
            int ret = route.match_center_line_xy(warning->x, warning->y, index, 5.0, 5.0, warning->lane_id, dis_min);
            if (ret != RET_OK)
            {
                break;
            }

            //绑定点   //bind key_point 需要重新实现
            //ret= route.bind_key_point_ll(index, get_warning_type(warning->type),
            //        warning->id, warning->lat, warning->lon);
            if (ret != RET_OK)
            {
                break;
            }

            //迭代到下一个点，用于绕行多圈需要多次绑定
            index += 20;
        }
    }
}

//获得预警点类型ID
int get_warning_type(string type)
{
    if (type == "single")
    {
        return KP_W_SINGLE;
    }
    else if (type == "change_off")
    {
        return KP_W_CHANGE_OFF;
    }
    else if (type == "change_on")
    {
        return KP_W_CHANGE_ON;
    }
    else if (type == "camera_on")
    {
        return KP_W_CAMERA_ON;
    }
    else if (type == "camera_off")
    {
        return KP_W_CAMERA_OFF;
    }
    else if (type == "radar_on")
    {
        return KP_W_RADAR_ON;
    }
    else if (type == "radar_off")
    {
        return KP_W_RADAR_OFF;
    }
    else if (type == "limspeed_off")
    {
        return KP_W_LIMSPEED_OFF;
    }
    else if (type == "degrade")
    {
        return KP_W_DEGRADE;
    }


    return KP_NONE;
}

