#include <math.h>
#include <vector>
#include <fstream>
#include <iostream>

//#include "common/origin_vehicle.h"
#include "common/navi_point.h"
#include "common/math_util.h"
#include "vehicle_dynamic/heading.h"
#include "vehicle_dynamic/steering_angle.h"
#include "vehicle_dynamic/cau_heading_steering.h"

#include "trajectory/trajectory_sets.h"

void cau_all_mileage_of_trajectory(trajectory_cubic& trj)
{
  if(trj.points.size()<=0)
     return;

  navi_point p0;
  p0 = trj.points[0];

  double x = p0.position_x;
  double y = p0.position_y;
  double xx, yy, ss;

  double s = 0;
  trj.points[0].s = 0;

  if(trj.points_num <=0)
      return;

  for(int i=1; i< trj.points_num; i++)
  {
    xx = trj.points[i].position_x;
    yy = trj.points[i].position_y;

    ss = sqrt((xx-x)*(xx-x) + (yy-y)*(yy-y));
    s += ss;

    trj.points[i].s = s;

    assert(s >=0);
    assert(s >= trj.points[i-1].s);

    x = xx;
    y = yy;
  }

}

///生成轨迹簇
int trajectory_cubic_sets::gen_traj_cluster( navi_point pos_sta, vector< navi_point > pos_ends_1, vector< navi_point > pos_ends_2 )
{
    vector<navi_point> out_points_1, out_points_2;
    for ( int i = 0; i < pos_ends_1.size(); i++ )
    {
        /*计算轨迹(x,y,h,k)*/
        trajectorys[ i ].tr_num = i;
        trajectorys[ i ].set_trajectory_sets((trajectory_cubic_sets* )this);
        trajectorys[ i ].gen_trj_from_two_points( pos_sta, pos_ends_1[ i ], out_points_1 );
        trajectorys[ i ].gen_trj_from_two_points( out_points_1[ out_points_1.size()-1 ], pos_ends_2[ i ], out_points_2 );

        trajectorys[ i ].points.clear();
        //生成起始点到第一个目标点的轨迹
        trajectorys[ i ].points.insert( trajectorys[ i ].points.end(), out_points_1.begin(), out_points_1.end() );
        trajectorys[ i ].points_num_1 = trajectorys[ i ].points.size();
        //生成第一个目标点到第二个目标点的轨迹
        trajectorys[ i ].points.insert( trajectorys[ i ].points.end(), out_points_2.begin()+1, out_points_2.end() );
        trajectorys[ i ].points_num = trajectorys[ i ].points.size();

        /*轨迹的终点影响其代价值*/
        trajectorys[ i ].end_pos_value = pos_ends_2[ i ].r;

        /*计算轨迹的里程信息s*/
        cau_all_mileage_of_trajectory( trajectorys[ i ] );
    }

    return 1;
}



int trajectory_cubic_sets::gen_traj_cluster( navi_point pos_sta, vector< navi_point > pos_ends_1 )
{
    vector<navi_point> out_points_1, out_points_2;
    for ( int i = 0; i < pos_ends_1.size(); i++ )
    {
        /*计算轨迹(x,y,h,k)*/
        trajectorys[ i ].tr_num = i;
        trajectorys[ i ].set_trajectory_sets((trajectory_cubic_sets* )this);
        trajectorys[ i ].gen_trj_from_two_points( pos_sta, pos_ends_1[ i ], out_points_1 );

        /*将输出点的容器插入到另外一个容器里*/
        trajectorys[ i ].points.clear();
        trajectorys[ i ].points.insert( trajectorys[ i ].points.end(), out_points_1.begin(), out_points_1.end() );
        trajectorys[ i ].points_num_1 = trajectorys[ i ].points.size();
        trajectorys[ i ].points_num = trajectorys[ i ].points.size();

        /*轨迹的终点影响其代价值*/
        trajectorys[ i ].end_pos_value = pos_ends_1[ i ].r;

        /*计算轨迹的里程信息s*/
        cau_all_mileage_of_trajectory( trajectorys[ i ] );
    }

    return 1;
}

