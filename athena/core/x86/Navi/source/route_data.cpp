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

#include "route_data.h"
#include <time.h>
#include <sys/time.h>
using namespace athena;
using namespace route;

/////////////////////////////////////////////////
/// \brief 初始化函数
/// \return
/////////////////////////////////////////////////
coord_transfer::coord_transfer()
{

}

coord_transfer::coord_transfer(std::string filename)
{
   set_origin(filename);
}

coord_transfer::coord_transfer(double lat, double lon)
{
   set_origin(lat, lon);
}


void coord_transfer::set_origin(std::string filename)
{
    configSettings.ReadFile(filename);
    origin_lat =  configSettings.Read("origin_lat", -1.0);
    origin_lon =  configSettings.Read("origin_lon", -1.0);;
    cs.set_origin(origin_lat, origin_lon);
}

void coord_transfer::set_origin(double lat, double lon)
{
    origin_lat =  lat;
    origin_lon =  lon;
    cs.set_origin(origin_lat, origin_lon);
}
coord_transfer::~coord_transfer()
{
}

