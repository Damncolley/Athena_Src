#include <fstream>
#include <iostream>
#include <math.h>
#include <string>
//#include <vector>

#include "common/navi_point.h"

using namespace std;

navi_point::navi_point()
{
    position_x = 0;
    position_y = 0;
    position_z = 0;

    speed = 0;
    lateral_speed = 0;
    longitudinal_speed = 0;
    down_speed = 0;

    roll = 0;
    pitch = 0;
    heading = 0;

    accelerataion = 0;
    lateral_accelerate = 0;
    longitudinal_accelerate = 0;
    down_accelerate = 0;

    steering_angle = 0;
    steering_angle_speed = 0;

    speed_desired_Uxs = 0;
    acceleration_desired_Axs = 0;

    point_no = 0;
    s = 0;
    r = 0;
    k_s = 0;

    gears = 0;

    type = 0;

    lateral_offset = 0.0;
}

navi_point::~navi_point()
{

}

double length_of_two_navipoint(navi_point p1, navi_point p2)
{
   double le = 0;

   le = sqrt(
      (p1.position_x - p2.position_x) * (p1.position_x - p2.position_x) +
      (p1.position_y - p2.position_y) * (p1.position_y - p2.position_y));

   return le;
}


