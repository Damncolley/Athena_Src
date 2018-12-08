#include "common/navi_point.h"
#include "common/path.h"

#include "trajectory/trajectory.h"
#include "trajectory/trajectory_sets.h"


trajectory_cubic::trajectory_cubic()
{
    p_traj_cubic_sets = NULL;

    expected_lane_value = 1000;
    collision_check_value = 1000;
    ks_sum_value = 1000;
    consistency_value = 1000;

    points_num = 0;
    for (int i = 0 ; i < 2000; i++)
    {
        navi_point p;
        points.push_back(p);
    }

}

trajectory_cubic::~trajectory_cubic()
{


}

void trajectory_cubic::set_trajectory_sets(trajectory_cubic_sets *p_set)
{
    p_traj_cubic_sets = p_set;
}

void trajectory_cubic::copy_from(trajectory& tr)
{
   unsigned int i;

   if(tr.points_num <=0)
      return;

   for (i = 0; i < tr.points_num; i++)
   {
       points[i] =  tr.points[i];
   }

   this->points_num = tr.points_num;
   this->expected_lane_value = tr.expected_lane_value;
   this->collision_check_value = tr.collision_check_value;
   this->ks_sum_value = tr.ks_sum_value;
   this->consistency_value = tr.consistency_value;

}

