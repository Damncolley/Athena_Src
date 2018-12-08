#include "common/path_tools.h"
#include "vehicle_dynamic/steering_angle.h"
//#include "common/math_util.h"

void alloc_lane_size(path& lane, int length)
{
    if(lane.ref_points.size() > 0)
       lane.ref_points.clear();

    navi_point np;
    for(int i=0; i<length; i++)
    {
       lane.ref_points.push_back(np);
    }

    lane.insert_pos = 0;
}

void cau_self_lane_offset(path& lane,  //要考过来的轨迹
                      double offset)  //拷贝到目标的起点
{
    double x, y;

    for (int i = 0; i < lane.ref_points.size(); i++)
    {
        //所有的信息先考过来，然后修改偏移量。
        double  direction = lane.ref_points[i].heading;
        double  vertical_direction = calculate_vertical_direction(direction);

        x = lane.ref_points[i].position_x +
            offset * sin( to_radians(vertical_direction));
        y = lane.ref_points[i].position_y +
            offset * cos( to_radians(vertical_direction));

        lane.ref_points[i].position_x = x;
        lane.ref_points[i].position_y = y;
    }
}


void cau_current_lane(path& lane,  //要考过来的轨迹
                      path& ref_lane,   //从参考的轨迹中考出
                      int st_pos, int en_pos,    //要拷贝过来的位置
                      double offset)   //需要增加的偏移量
{
    int l = 0;
    double x, y;

    for (int i = st_pos; i < en_pos; i++)
    {
        //张毅
        if (i < 0 || i >= ref_lane.ref_points.size())
        {
            printf("cau_current_lane: ref_lane: i=%d, size=%d\n",
                i, ref_lane.ref_points.size());
            //usleep(10000000);
            return;
        }

        //所有的信息先考过来，然后修改偏移量。
        lane.ref_points.push_back( ref_lane.ref_points[i] );

        double  direction = ref_lane.ref_points[i].heading;
        double  vertical_direction = calculate_vertical_direction(direction);

        x = ref_lane.ref_points[i].position_x +
            offset * sin( to_radians(vertical_direction));
        y = ref_lane.ref_points[i].position_y +
            offset * cos( to_radians(vertical_direction));

        lane.ref_points[l].position_x = x;
        lane.ref_points[l].position_y = y;

        l++;
    }
}

double find_ref_offset(double x, double y, double yaw,
                       path& p, int st_pos)
{
    double x_c =  p.ref_points[st_pos].position_x;
    double y_c =  p.ref_points[st_pos].position_y;
    double yaw_c = p.ref_points[st_pos].heading;

    double value = calculate_sign_and_value_offset ( x, y, yaw, x_c, y_c, yaw_c );

    return value;
}



