#pragma once

#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <vector>

#include "common/LocalGeographicCS.hpp"
//#include "planning/planning_param.h"
#include "config.h"


typedef std::pair<double,double> ll_point;
typedef std::pair<double,double> xy_point;
typedef std::tuple<double,double,double> xys_point;
typedef std::tuple<double,double,int32_t,double,double,int32_t,double> xyh_point;
typedef std::tuple<double,double,double> blh_point;
typedef std::vector<xyh_point> xyh_vec;
typedef std::vector<xyh_point> blh_vec;

using namespace std;

enum LATLON_COORDS_CS
{
    LAT_CS = 0,
    LON_CS = 1,
    ID_CS = 2,
};

enum XYH_POINT_CENTROL
{
    X_CTR = 0,
    Y_CTR = 1,
    ID_CTR = 2,
    HEAD_CTR = 3,
    WIDTH_CTR = 4,
    DIV_CTR = 5,
    S_CTR = 6,
};

enum XY_COORDS_CS
{
    X_CS = 0,
    Y_CS = 1,
    S_CS = 2
};

class coord_transfer
{
public:
    LocalGeographicCS cs;

    coord_transfer()
    {
        cs.set_origin( origin_lat, origin_lon );
    }
    ~coord_transfer()
    {

    }

    //设定原点坐标
    void set_origin();

private:

    const double origin_lat = ORIGIN_LAT;//30.45814058804; // (rongke) //39.7409469; (tongzhou)
    const double origin_lon = ORIGIN_LON;//114.31801222674; // (rongke) // 116.6545923;

};
