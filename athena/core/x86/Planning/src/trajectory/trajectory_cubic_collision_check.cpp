#include <math.h>
#include <fstream>
#include <iostream>

#include "common/math_util.h"
#include "common/navi_point.h"
#include "common/path.h"
#include "common/rect.h"

#include "trajectory/trajectory.h"
#include "trajectory/trajectory_sets.h"

double trajectory_cubic::traj_collision_check( const RoadSurface& road, const double car_speed )
{
    double temp_car_speed = car_speed;
    collision_distance = -1.0;
    collision_check_value = 0.0;
    if(road.obstacles_.size() <=0 || points.size() <=0 )
        return 0;

    double x, y, heading;
    bool cross;
    rect rt_car;
    double length = 0.0;
    CollisionCheck col_check;

    for ( int i = 1; i < points.size(); i++ )
    {
        length += length_of_two_navipoint( points[i-1], points[i] );

        x= points[i].position_x;
        y= points[i].position_y;
        heading = points[i].heading;
        //计算车辆包围盒rt_car,车长宽加上保护距离
        col_check.car_trajectory_collision_check_pos_rect(rt_car, x, y, heading);

        /*calculate interval*/
        double delta_t = 0.0;
        double delta_x, delta_y, delta_s;
        delta_x = delta_y = delta_s = 0.0;

        for ( auto obs:road.obstacles_ )
        {
            /*如果障碍物有速度，考虑障碍物的运动*/
            if ( obs.v_ > THRESHOLD_STATIC_SPEED )//&& temp_car_speed > 1.0 )   //1.5***config
            {
                /*假设障碍物做匀速直线运动*/
                if ( temp_car_speed == 0.0 )
                    temp_car_speed += 0.1;
                delta_t = length / temp_car_speed;
                delta_s = obs.v_ * delta_t;
                delta_x = delta_s * sin ( to_radians( obs.yaw_ ) );
                delta_y = delta_s * cos ( to_radians( obs.yaw_ ) );
                //cout << "---------delta_xy : " << delta_x << ", " << delta_y << endl;

                obs.rt_.p1.xx += delta_x;
                obs.rt_.p1.yy += delta_y;
                obs.rt_.p2.xx += delta_x;
                obs.rt_.p2.yy += delta_y;
                obs.rt_.p3.xx += delta_x;
                obs.rt_.p3.yy += delta_y;
                obs.rt_.p4.xx += delta_x;
                obs.rt_.p4.yy += delta_y;
            }

            //计算车辆包围盒rt_car和障碍物包围盒obs.rt是否有交叉
            cross = cross_in_two_rect_xx( rt_car, obs.rt_ );
            //如果有碰撞
            if( cross )
            {
                collision_distance = length;
                if(collision_check_value < obs.value_)
                    collision_check_value = obs.value_;
                return length;   //碰撞距离

            }

        }

    }

    return -1;
}

float trajectory_cubic::collision_check(RoadSurface& road)
{
    unsigned int i,j,k;
    double x, y, heading;
    double xx, yy;
    double total_length=0;
    bool cross;
    rect rt_car;

    bool first_collision = false;
    free_length_num = 0;
    collision_check_value = 0.0;

    if(points_num <=0)
        return free_length_num;

    if(road.obstacles_.size() <=0)
        return points_num ;

// 处理道路边缘形状不同的碰撞问题。
  /*  for (i = 0; i < points.size(); i += 6)
    {
        x= points[i].position_x;
        y= points[i].position_y;
        heading = points[i].heading;

        car_pos_rect(rt_car, x, y, heading);
        for(j=0; j< PATCHS_WIDTH; j++)
        {
            for(k=0; k< PATCHS_LENGTH; k++)
            {
                //只计算相交的小块的代价
                cross = rt_car.cross_rect(road.patchs[j][k].rt);
                if(cross)
                {
                   collision_check_value += road.patchs[j][k].value;
                   if(!first_collision)
                   {
                      free_length_num=i;
                      first_collision=true;
                   }
                }
            }
        }

    }
   */

    xx = points[0].position_x;
    yy = points[0].position_y;

    free_length_num = total_length;
    CollisionCheck col_check;

    if(points_num <=0)
      return free_length_num;

    for (i = 0; i < points_num; i += 10)
    {
        x= points[i].position_x;
        y= points[i].position_y;
        heading = points[i].heading;  //轨迹上每个点的头指向都为零。

        total_length += sqrt((x-xx)*(x-xx) + (y-yy)*(y-yy));

        xx = points[i].position_x;
        yy = points[i].position_y;

//        if(total_length > COLLISION_CHECK_LENGTH)
//        {
//            free_length_num = total_length;
//            break;
//        }

        col_check.car_trajectory_collision_check_pos_rect(rt_car, x, y, heading);
        int num_objs = road.obstacles_.size();

        for(k=0; k< num_objs; k++)
        {
            //只计算相交的小块的代价
            cross = cross_in_two_rect_xx(rt_car, road.obstacles_[k].rt_);

            //需要记录最近的碰撞物体是那个啊。关键。
            if(cross)  //如果碰撞了
            {
               //考虑轨迹上可能碰撞的最重要的物体。已经是归一化的了。
               //后面碰撞的物体选择权重最高的保留一下。
               if(collision_check_value < road.obstacles_[k].value_)
                    collision_check_value = road.obstacles_[k].value_;

               //还没有第一次碰撞
               if(!first_collision)
               {
                  free_length_num = i;
                  first_collision = true;
                  first_collision_obj_num = k;
               }
            }
            else  //没有碰撞
            {
               if(!first_collision)
               {
                  free_length_num = i;
               }
            }
        }

    }

    free_length = 0;
    if(free_length_num <=0)
        return free_length;

    // 计算好碰撞距离，只假设空间上的碰撞关系的碰撞距离
    for (i = 0; i < free_length_num - 10; i +=10)
    {
        x= points[i].position_x;
        y= points[i].position_y;
        xx = points[i+10].position_x;
        yy = points[i+10].position_y;
        free_length += sqrt((x-xx)*(x-xx) + (y-yy)*(y-yy));
    }

    // TOC Time of collision, 可以用的简单的方法来判断动态障碍物碰撞。
    // 安全距离是和车速相关的函数。
    // （两车距离- 安全距离）/ 前车和后车的速度差 = 无碰撞时间，
    // 如果无碰撞时间大于一个值， 就可以不不要换道。

    if(collision_check_value >= 30000)
    {
      collision_distance = free_length;
      collision_obj_speed = road.obstacles_[first_collision_obj_num].v_;
    }

    // 这个需要在车道碰撞和轨迹碰撞两个位置都需要处理。
    // 还有一个就是什么时候换道， 什么时候减速的判断标准。

    // 关键是轨迹规划，需要判断每条轨迹的碰撞关系。
    // 车道碰撞， 只是判断这个车道的碰撞关系。

}



