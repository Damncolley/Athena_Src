#include <math.h>

#include "common/math_util.h"
#include "common/navi_point.h"
#include "common/path.h"
#include "common/rect.h"

#include "trajectory/trajectory.h"
#include "trajectory/trajectory_sets.h"

float trajectory_cubic::traj_ks_check()
{
    unsigned int i;

    double sum_value = 0.0;

    if(points_num_1 <=0)
      return 0;

    double max_ks = -1;;
    for (i = 0; i < points_num_1; i++)
    {
        if ( fabs(points[i].k_s) > max_ks )
        {
            max_ks = fabs(points[i].k_s);
        }

        sum_value += fabs(points[i].k_s);
    }
    ks_sum_value =  sum_value;

    // 考虑整个轨迹上的曲率变化。可以考虑平均值和最大值，保证合理性。
//    if(points_num_1 > 0)
//       ks_sum_value /= points_num_1;
//    else
//       ks_sum_value = 1000;
    ks_sum_value = max_ks;
}

float trajectory_cubic::ls_sum_check()
{
    unsigned int i;

    double sum_value = 0.0;

    if(points_num <=0)
      return 0;

    for (i = 0; i < points_num; i++)
    {
        sum_value += fabs(points[i].lateral_offset);
    }
    ls_sum_value = sum_value;

    // 考虑整个轨迹上的offset变化。
    if(points_num > 0)
       ls_sum_value /= points_num;
    else
       ls_sum_value = 1000;

}

float trajectory_cubic::steering_angle_sum_check()
{
    unsigned int i;

    double sum_value = 0.0;

    if(points_num <=0)
      return 0;

    for (i = 0; i < points_num; i++)
    {
        sum_value += fabs(points[i].steering_angle);
    }
    steering_angle_sum_value = sum_value;

    // 考虑整个轨迹上的Steering_angle变化.
    if(points_num > 0)
       steering_angle_sum_value /= points_num;
    else
       steering_angle_sum_value = 1000;

}

float trajectory_cubic::consistency_check(trajectory& tr)
{
    unsigned int i, s;
    double x, y;

    double value = 0;
    //碰撞检测结果来进行判断

    if(tr.points_num <=0)
    {
      consistency_value = 0;
      return consistency_value;
    }
    // 都用里程计算。
    double gap = points[0].s - tr.points[0].s;

    // 两个轨迹相差太远，没有可比性，所以返回最大值。
    if( gap >= 40)
    {
      consistency_value = 1000;
      return consistency_value;
    }
    int i_gap = (int ) (gap / SPLINE_EVERY);

    for (i=0; i < points_num - i_gap; i++)
    {
       value +=
            fabs( points[i].lateral_offset
              - tr.points[i+gap].lateral_offset);
    }

    consistency_value = value;
    if( (points_num-gap) > 0)
       consistency_value /= (points_num-gap);
    else
       consistency_value = 1000;

}

float trajectory_cubic::consistency_check_virtual_lane(path& virtual_lane)
{
    unsigned int i, s;
    double x, y;

    double value = 0;
    //碰撞检测结果来进行判断

    if(points_num <=0)
      return 0;

    if(virtual_lane.ref_points.size() <=0)
    {
      consistency_value = 0;
      return consistency_value;
    }

/*
    int insert_pos; //current insert pos;
    int current_pos; //current pos of vehicle;
    int st_pos;   // current motion plan pos
    int en_pos;   // end of motion plan pos

*/

   // #pragma omp parallel for
    for (i=0; i < points_num; i++)
    {
       value +=
            fabs( points[i].lateral_offset
              - points[i].lateral_offset);
    }

    consistency_value = value;
    if( (points_num) > 0)
       consistency_value /= (points_num);
    else
       consistency_value = 1000;

}


