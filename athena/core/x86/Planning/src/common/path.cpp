#include <fstream>
#include <iostream>
#include <vector>
#include <math.h>

//#include "math_util.h"
#include "common/navi_point.h"
//#include "origin_vehicle.h"
#include "common/path.h"
//#include "cs.h"

using namespace std;

void path::output_navi_point_all(char *filename)
{

}

void path::read_navi_file(string path_gps_log_file)
{

}

void path::reset_path(const vector<navi_point>& ref_)
{
    unsigned int i;
    navi_point np;

    ref_points.clear();

    for (i = 0; i < ref_.size(); i++)
    {
        //目前导航点有很多属性都需要复制。
        np = ref_[i];

/*      np.position_x = ref_[i].position_x;
        np.position_y = ref_[i].position_y;

        np.lat  = ref_[i].lat;
        np.lon = ref_[i].lon;

        np.heading = ref_[i].heading;

        np.k_s = ref_[i].k_s;
        np.s = ref_[i].s;
        np.point_no = ref_[i].point_no;

        np.steering_angle = ref_[i].steering_angle;
        np.lateral_offset = ref_[i].lateral_offset; */

        ref_points.push_back(np);
    }

}

void cau_all_mileage_of_points(
    vector<navi_point>&  points,
    int start_pos,
    int end_pos)
{
    if(points.size() <=0)
        return;

    navi_point p0;
    p0 = points[start_pos];

    double x = p0.position_x;
    double y = p0.position_y;
    double xx, yy, ss;

    double s = 0;
    points[start_pos].s = 0;

    for(int i = start_pos + 1;  i< end_pos;  i++)
    {
        xx = points[i].position_x;
        yy = points[i].position_y;

        ss = sqrt((xx- x)*(xx- x) + (yy- y)*(yy- y));
        s += ss;

        points[i].s = s;

#ifdef NEED_ASSERT
        assert(s >=0);
        assert(s >= points[i-1].s);
        assert(s <= points[i-1].s + 60);
#endif
        x = xx;
        y = yy;
    }
}

void cau_all_mileage_of_lane(path& virtual_lane)
{
  /*  cau_all_mileage_of_points(
        virtual_lane.ref_points,
        0,
        virtual_lane.ref_points.size()); */

    int start_pos = 0;
    int end_pos = virtual_lane.ref_points.size();

    if(virtual_lane.ref_points.size() <=0)
        return;

    navi_point p0;
    p0 = virtual_lane.ref_points[start_pos];

    double x = p0.position_x;
    double y = p0.position_y;
    double xx, yy, ss;

    double s = 0;
    virtual_lane.ref_points[start_pos].s = 0;

    for(int i = start_pos + 1;  i< end_pos;  i++)
    {
        xx = virtual_lane.ref_points[i].position_x;
        yy = virtual_lane.ref_points[i].position_y;

        ss = sqrt((xx- x)*(xx- x) + (yy- y)*(yy- y));
        s += ss;

        virtual_lane.ref_points[i].s = s;

 //       assert(ss < 0.3);

#ifdef NEED_ASSERT
        assert(s >=0);
        assert(s >= points[i-1].s);
        assert(s <= points[i-1].s + 60);
#endif
        x = xx;
        y = yy;
    }


}



