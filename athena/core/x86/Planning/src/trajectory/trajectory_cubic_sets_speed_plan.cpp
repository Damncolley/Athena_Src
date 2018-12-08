#include <math.h>
#include <iostream>
#include <vector>

//#include "common/origin_vehicle.h"
#include "common/navi_point.h"
#include "common/math_util.h"

#include "trajectory/trajectory_sets.h"


void trajectory_cubic_sets::speed_plan(double max_speed)
{
//    unsigned int i;

    for(int i = 0; i< trajectorys[best_one].points_num;  i++)
       trajectorys[best_one].points[i].speed_desired_Uxs = max_speed;

}


