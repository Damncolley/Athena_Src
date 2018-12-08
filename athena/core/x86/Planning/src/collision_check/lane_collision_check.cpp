#include <math.h>
#include <fstream>
#include <iostream>

#include "common/math_util.h"
#include "common/navi_point.h"
#include "common/path.h"
#include "common/rect.h"
#include "collision_check/collision_check.h"
#include "../../include/common/enum_list.h"

int CollisionCheck::set_car_data( double m_car_width, double m_car_length, double m_safe_width )
{
    car_width_ = m_car_width;
    car_length_ = m_car_length;
    safe_width_ = m_safe_width;

    return 1;
}

///规划路径碰撞检测
int CollisionCheck::virtual_lane_col_check( path& lane, vector<ObstacleRect> obstacles, double car_speed, double net_speed, int st_pos, double check_length,
                            double& free_length, double& motion_speed, double& motion_length, int& drive_mode )
{
    free_length = -1;
    motion_speed = net_speed;
    motion_length = -1.0;
    drive_mode = 0;

    if( st_pos < 0 )
        return 0;
    if(lane.ref_points.size() <=0)
        return 0;
    if(obstacles.size() <=0 )
        return 0;

    double x, y, heading;
    bool cross;
    rect rt_car;
    double length = 0.0;
    double d_length = 0.0;

    int i = st_pos;
    while ( 1 )
    {
        //虚拟轨迹有四次多项式曲线+++在后面拼接一段地图中心线
        d_length = length_of_two_navipoint( lane.ref_points[i], lane.ref_points[i+COL_CHECK_INTERVAL] );
        //四次多项式曲线间距为0.1米
        if ( d_length < THRESHOLD_DELTA_LENGTH )
        {
            length += d_length;
            i += COL_CHECK_INTERVAL;
        }
        //后面拼接一段地图中心线间距为1.5米
        else
        {
            length += length_of_two_navipoint( lane.ref_points[i], lane.ref_points[i+1] );
            i++;
        }

        if ( length > check_length || i > lane.ref_points.size()-1 )
            break;

        x= lane.ref_points[i].position_x;
        y= lane.ref_points[i].position_y;
        heading = lane.ref_points[i].heading;
        //计算车辆包围盒rt_car,车长宽加上保护距离
        car_lane_collision_check_rect(rt_car, x, y, heading);

        for ( auto obs:obstacles )
        {
            ofstream outfile("acc.log", std::ios::app);
            outfile.precision(8);

//            outfile << "--------------------------- net_speed=" << net_speed << endl;
//            //如果障碍物速度超过地图推荐速度和车速，则该障碍物不用考虑碰撞
//            if ( obs.v_ > net_speed && obs.v_ > car_speed )
//                continue;

            //计算车辆包围盒rt_car和障碍物包围盒obs.rt是否有交叉
            cross = cross_in_two_rect_xx( rt_car, obs.rt_ );
            //如果有碰撞
            if( cross )
            {
                /***速度低于该比例时，选择超车***/
                if ( obs.v_ < net_speed * RATIO_SPEED_CAR_FOLLOWING )    //1.5***config
                {
                    outfile << "not acc : obs.v_=" << obs.v_ << ", length=" << length << endl;
                    free_length = length;   //碰撞距离
                    outfile.close();
                    return 1;

                }
                /***选择跟车***/
                else
                {
                    drive_mode = DRIVE_MODE_FOLLOW;
                    double delta_T = 1.0;
                    double delta_length = length;               //R：与前车的距离
                    double delta_speed = obs.v_ - car_speed;    //R':速度差，前-后
                    double final_length = 5.0 + obs.v_*3.6;     //Rfinal：期望车距
                    if ( final_length < 30.0 )
                        final_length = 30.0;
                    double safe_length = final_length - 3.0;
                    double sensor_max_length = 80.0;            //传感器最大检测距离
                    double Dec = 0.4;                           //D：滑行期间减速度（零加速和零制动）
                    double Trans = 0.0;                             //T：转变线斜率
                    if ( sensor_max_length > final_length )
                        Trans = ( final_length - sensor_max_length ) / sqrt( 2.0*Dec*(sensor_max_length-final_length) );

                    double delta_length_1 = Trans * delta_speed + final_length;
                    double delta_length_2 = 0.5 * delta_speed * delta_speed / Dec + final_length;

                    outfile << "acc : delta_length_1=" << delta_length_1 << ", delta_length_2=" << delta_length_2
                            << ", car_speed=" << car_speed << ", obs.v_=" << obs.v_ << ", drive_mode="<< int (drive_mode) <<  endl;

                    if ( delta_length > sensor_max_length )  //sensor_max_length = 80.0
                    {
                        outfile << "1 : delta_length=" << delta_length << ", delta_speed=" << delta_speed << endl;
                        outfile.close();
                        return 0;
                    }

                    if ( delta_speed >= 0.0 )
                    {
                        if ( delta_length > delta_length_1 )
                        {
                            outfile << "2 : delta_length=" << delta_length << ", delta_speed=" << delta_speed << endl;
                            outfile.close();
                            return 0;
                        }
                        else
                        {
                            ///过近，刹车
                            if ( car_speed < THRESHOLD_HIGH_SPEED/3.6 )
                            {
                                motion_length = 5.0 + COEF2_MOTION_WITH_VP * car_speed;    //5.0,5.0***config
                            }
                            else
                            {
                                motion_length = 5.0 + COEF2_MOTION_HIGH_SPEED * car_speed;    //5.0,5.0***config
                            }
                            motion_speed = car_speed * car_speed - 2.0 * MAX_DECELERATION * motion_length;
                            if ( motion_speed < 0.0 )
                                motion_speed = 0.0;
                            motion_speed = sqrt( motion_speed );

                            //myy20181205
                            //free_length = motion_length;

                            outfile << "3 : delta_length=" << delta_length << ", delta_speed=" << delta_speed
                                    << ", motion_speed=" << motion_speed << ", motion_length=" << motion_length
                                    << ", a=" << 0.5*(motion_speed*motion_speed-car_speed*car_speed)/motion_length << endl;

                        }

                    }
                    else
                    {
//                        if ( delta_length < delta_length_2 )
//                        {
//                            ///过近，刹车
//
//                        }
                        if ( delta_length > delta_length_1 - delta_speed * delta_T )
                        {
                            outfile << "4 : delta_length=" << delta_length << ", delta_speed=" << delta_speed << endl;
                            outfile.close();
                            return 0;
                        }
                        else
                        {
                            if ( delta_length > final_length )  //
                            {
                                motion_speed = obs.v_; //1.0***config,调整车速略小于障碍物速度
                                motion_length = ( delta_length - final_length ) * ( car_speed + motion_speed ) / ( -delta_speed );

                                outfile << "5 : delta_length=" << delta_length << ", delta_speed=" << delta_speed
                                        << ", motion_speed=" << motion_speed << ", motion_length=" << motion_length
                                        << ", a=" << 0.5*(motion_speed*motion_speed-car_speed*car_speed)/motion_length << endl;
                            }
                            else if ( delta_length > safe_length )  //safe_length = 20
                            {
                                 motion_speed = obs.v_; //1.0***config,调整车速略小于障碍物速度
                                 motion_length = ( delta_length - safe_length ) * ( car_speed + motion_speed ) / ( -delta_speed );

                                 outfile << "6 : delta_length=" << delta_length << ", delta_speed=" << delta_speed
                                         << ", motion_speed=" << motion_speed << ", motion_length=" << motion_length
                                         << ", a=" << 0.5*(motion_speed*motion_speed-car_speed*car_speed)/motion_length << endl;

                            }
                            else
                            {
                                motion_speed = 0.0;
                                motion_length = safe_length - 5.0;

                                //myy20181205
                                //free_length = motion_length;

                                outfile << "7 : delta_length=" << delta_length << ", delta_speed=" << delta_speed
                                         << ", motion_speed=" << motion_speed << ", motion_length=" << motion_length
                                         << ", a=" << 0.5*(motion_speed*motion_speed-car_speed*car_speed)/motion_length << endl;

                            }

                        }


                    }








#if 0
                    double length_following = 5.0 + obs.v_*3.6;    //安全跟车距离
                    double length_safe = 5.0;
                    double dt = 2.0;

                    //开始减速到障碍物速度
                    if ( length <= net_speed*3.6 && length >= length_following )//&& car_speed > obs.v_ )
                    {
                        motion_speed = obs.v_; //1.0***config,调整车速略小于障碍物速度
                        motion_length = ( length - length_following ) * ( car_speed + motion_speed ) / ( car_speed - motion_speed );

                    }
//                    //追击(2秒内)
//                    if ( length <= net_speed*3.6 && length >= length_following && car_speed < obs.v_ )
//                    {
//                        motion_speed = 2.0 * obs.v_ - car_speed + 0.5*dt*( length - length_following );
////                        motion_speed = obs.v_ + ( length - length_following );
//                        if ( motion_speed > net_speed )
//                            motion_speed = net_speed;
//                        motion_length = 0.5*dt*( car_speed + motion_speed );
//                    }
                    //减速拉开距离(2秒内)
                    if ( length < length_following )//&& car_speed >= obs.v_ )
                    {
//                        cout << "xxxxxxxxxxxxxx length_following = " << length_following << endl;
                        motion_speed = 2.0 * obs.v_ - car_speed + 0.5*dt*( length - length_following );
                        motion_speed = obs.v_ + ( length - length_following );
                        if ( motion_speed < 0.0 )
                            motion_speed = 0.0;
                        motion_length = 0.5*dt*( car_speed + motion_speed );
                    }
                    //维持
//                    if ( length < length_following && car_speed < obs.v_ )
//                    {
//                        motion_speed = obs.v_ - CAR_FOLLOWING_SPEED_DIFF; //1.0***config,调整车速略小于障碍物速度
//                        if ( car_speed != motion_speed )
//                            motion_length = ( length - length_following ) * ( car_speed + motion_speed ) / ( car_speed - motion_speed );
//                    }
#endif // 0
//                    cout << "----------------------------------------------------------" << endl;
//                    cout << "--- length=" << length << ", motion_length=" << motion_length << ", car_speed=" << car_speed << ", motion_speed=" << motion_speed << endl;

                    //如果障碍物不是静态，通过调节纵向速度实现跟车
                    //（障碍物速度低于车速，或者障碍物速度高于车速但低于地图推荐速度）
//                    motion_speed = obs.v_ - CAR_FOLLOWING_SPEED_DIFF; //1.0***config,调整车速略小于障碍物速度
                    outfile.close();
                    return 0;
                }

            }
            outfile.close();

        }

    }

    return 0;
}

///各车道的碰撞检测
double CollisionCheck::lane_col_check( path& lane, vector<ObstacleRect> obstacles, double car_speed, int st_pos, double check_length )
{
    if( st_pos < 0 )
        return 0;
    if(lane.ref_points.size() <=0)
        return 0;
    if(obstacles.size() <=0 )
        return 0;

    double x, y, heading;
    bool cross;
    rect rt_car;
    double length = 0.0;

    int i = st_pos;
    while ( 1 )
    {
        length += length_of_two_navipoint(lane.ref_points[i],
                                          lane.ref_points[i+1]);

        if ( length > check_length )
            break;

        i++;
        x= lane.ref_points[i].position_x;
        y= lane.ref_points[i].position_y;
        heading = lane.ref_points[i].heading;
        //计算车辆包围盒rt_car,车长宽加上保护距离
        car_lane_collision_check_rect(rt_car, x, y, heading);

        /*calculate interval*/
        double delta_t = 0.0;
        double delta_x, delta_y, delta_s;
        delta_x = delta_y = delta_s = 0.0;

        for ( auto obs:obstacles )
        {
            /*如果障碍物有速度，考虑障碍物的运动*/
            if ( obs.v_ > THRESHOLD_STATIC_SPEED )//&& car_speed > 1.0 )   //1.5***config
            {
                /*假设障碍物做匀速直线运动*/
                if ( car_speed == 0 )
                    car_speed += 0.1;
                delta_t = length / car_speed;
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

            //计算车辆包围盒rt_car和障碍物包围盒obs.rt_是否有交叉
            cross = cross_in_two_rect_xx( rt_car, obs.rt_ );
            //如果有碰撞
            if( cross )
            {
                return length;   //碰撞距离

            }

        }

    }

    return -1;

}

/**
* @brief 判断某个车道与障碍物是否有碰撞。
* @param lane 输入量：车道中心线。
* @param obstacles 输入量：路面障碍物信息。
* @param start_num 输入量：碰撞检测的起始位置，由地图匹配获得。
* @param wheel_base_length 输入量：车轴长。
* @param step 输入量：搜索步长。
* @param safe_dis 输入量：安全保护距离长。
* @return 返回1表示有碰撞，0表示没有。
*/
int CollisionCheck::park_col_check( path& lane, vector<ObstacleRect> obstacles, int start_num, double wheel_base_length, int step, double safe_dis )
{
    int num = lane.ref_points.size();
    if( num <= 0 )
        return 0;
    if ( start_num >= num )
        return 0;
    if(obstacles.size() <=0 )
        return 0;

    double x, y, heading;
    bool cross;
    rect rt_car;
    double length = 0.0;
    double delta_length = 0.0;

    for ( int i = start_num; i < num; i += step )
    {
        heading = lane.ref_points[i].heading;
        x= lane.ref_points[i].position_x + 0.5*wheel_base_length*sin( to_radians( heading ) );
        y= lane.ref_points[i].position_y + 0.5*wheel_base_length*cos( to_radians( heading ) );

        //计算车辆包围盒rt_car,车长宽加上保护距离
        car_lane_collision_check_rect( rt_car, x, y, heading, safe_dis );

        for ( auto obs:obstacles )
        {
            //计算车辆包围盒rt_car和障碍物包围盒obs.rt是否有交叉
            cross = cross_in_two_rect_xx( rt_car, obs.rt_ );
            if ( cross )
            {
                return 1;
            }
        }

    }

    return 0;
}

#if 0
bool CollisionCheck::cross_in_two_time(navi_point& p, navi_point& op)
{
    if( fabs(p.pos_gps_time - op.pos_gps_time) < 2.0)
        return true;
    else
        return false;
}

bool CollisionCheck::cross_in_two_time_seq(navi_point& p,
                           obstacle& obj,
                           rect& rt_car)
{
    bool cross = false;
    bool time_cross = false;
    bool result = false;
    int length = obj.pos_seq.size();
    navi_point obj_p;

    for(int j=0; j< length; j++)
    {
        obj_p = obj.pos_seq[j];

        time_cross = cross_in_two_time(p, obj_p);
        if(time_cross)
        {
            cross = cross_in_two_rect_xx(rt_car, obj.rt_seq[j]);
           // cross = cross_in_two_rect_xx(rt_car, rt_car);

            if(cross == true)
            {
                result = true;
                return result;
            }
        }

     }
     return result;
}

bool CollisionCheck::lane_collision_check(path& lane,
                          RoadSurface& road,
                          double speed,
                          int st_pos,
                          int en_pos,
                          double check_length,
                          int& free_length_num,
                          double& free_length,
                          double& obj_speed,
                          double& obj_heading)
{
    bool result = false;
    double x, y, heading;
    bool cross;
    rect rt_car;

    bool first_collision = false;
    double collision_check_value = 0.0;

    int i,k;

    navi_point p0;
    double length = 0;

    obj_speed = -10;  //
    obj_heading = 720;

    free_length_num = en_pos - st_pos + 1;
    free_length = check_length;

    if( st_pos <0 || en_pos <0 || en_pos< st_pos )
        return result;

    if(lane.ref_points.size() <=0)
        return result;

    if(road.obstacles.size() <=0 )
        return result;

    x= lane.ref_points[st_pos].position_x;
    y= lane.ref_points[st_pos].position_y;

    for (i = st_pos; i < en_pos; i += 10)
    {
        length += length_of_two_navipoint(lane.ref_points[i],
                                          lane.ref_points[i+10]);

        if(length > check_length)
          break;

        x= lane.ref_points[i].position_x;
        y= lane.ref_points[i].position_y;
        heading = lane.ref_points[i].heading;

        car_lane_collision_check_rect(rt_car, x, y, heading);
        int num_objs = road.obstacles.size();

        for(k=0; k< num_objs; k++)
        {
            //只计算相交的小块的代价
            cross = cross_in_two_rect_xx(rt_car, road.obstacles[k].rt);
            if(cross)
            {
                result = true;

                //考虑轨迹上可能碰撞的最重要的物体。已经是归一化的了。
                if(collision_check_value < road.obstacles[k].value)
                    collision_check_value = road.obstacles[k].value;

                if(!first_collision)
                {
                    free_length_num = i;
                    first_collision = true;
                    obj_speed = road.obstacles[k].v;
                    obj_heading = road.obstacles[k].yaw;
                }
            }
            else
            {
                if(!first_collision)
                {
                    free_length_num=i;
                }
            }
        }

    }

    free_length = 0;
    if(free_length_num <=0)
        return true;

    for (i = st_pos; i < free_length_num-10; i += 10)
    {
       free_length += length_of_two_navipoint(
                             lane.ref_points[i],
                             lane.ref_points[i+10]);
    }

    free_length_num = free_length_num - st_pos;

        // TOC Time of collision, 可以用的简单的方法来判断动态障碍物碰撞。
    // 安全距离是和车速相关的函数。
    // （两车距离- 安全距离）/ 前车和后车的速度差 = 无碰撞时间，
    // 如果无碰撞时间大于一个值， 就可以不不要换道。

    // 这个需要在车道碰撞和轨迹碰撞两个位置都需要处理。
    // 还有一个就是什么时候换道， 什么时候减速的判断标准。

    // 关键是轨迹规划，需要判断每条轨迹的碰撞关系。
    // 车道碰撞， 只是判断这个车道的碰撞关系。

   /* if(result == true) // collision
    {
        double collision_time = 1000;

        if( obj_speed <= 0.5)
        {
            collision_time =  // 安全距离15米
                 (free_length) / fabs(obj_speed);
        }
        else
        {
            collision_time = 1000;
        }


        ofstream outfile("lane_collision_log.txt", std::ios::app);
        outfile.precision(8);

        outfile  << "speed " << speed << " "
                    << " obj_speed  " <<  obj_speed  << " "
                    << " free_length " << free_length  << " "
                    << " result " << result  << " "
                    << " collision_time " << collision_time << endl;

        outfile.close();

        // 如果是静态障碍物，　只是距离问题。
        if(obj_speed + speed <= 1 && free_length < 50)
            return true;

        // 处理动态障碍物，　
        //碰撞时间小于１０ｓ，　我们高速接近障碍物，　或者障碍物剧烈减速。
        if(collision_time < 10)
            return true;

        //　我们和动态障碍物距离很小，不安全。
        if(free_length < 30)
            return true;

        return false;
    }
  */
    return result;
}

bool CollisionCheck::lane_collision_check_moving_objects(
                          path& lane,
                          RoadSurface& road,
                          int st_pos,
                          int en_pos,
                          double check_length,
                          int& free_length_num,
                          double& free_length,
                          double& obj_speed,
                          double& obj_heading)
{
    bool result = false;
    double x, y, heading;
    bool cross;
    bool time_cross;
    rect rt_car;

    bool first_collision = false;
    double collision_check_value = 0.0;

    int i,k;

    navi_point p0;
    double length = 0;

    if( st_pos <=0 || en_pos <=0 || en_pos<= st_pos )
        return result;

    if(lane.ref_points.size() <=0)
        return result;

    if(road.obstacles.size() <=0)
        return result;

    x= lane.ref_points[st_pos].position_x;
    y= lane.ref_points[st_pos].position_y;

    for (i = st_pos; i < en_pos; i += 10)
    {
        length += length_of_two_navipoint(lane.ref_points[i],
                                          lane.ref_points[i+10]);

        if(length > check_length)
          break;

        x= lane.ref_points[i].position_x;
        y= lane.ref_points[i].position_y;
        heading = lane.ref_points[i].heading;

        car_lane_collision_check_rect(rt_car, x, y, heading);
        int num_objs = road.obstacles.size();

        for(k=0; k< num_objs; k++)
        {
            //只计算相交的小块的代价
            cross = cross_in_two_time_seq(lane.ref_points[i],
                                          road.obstacles[k],
                                          rt_car);

            if(cross)
            {
                result = true;
                //考虑轨迹上可能碰撞的最重要的物体。已经是归一化的了。
                if(collision_check_value < road.obstacles[k].value)
                    collision_check_value = road.obstacles[k].value;

                if(!first_collision)
                {
                    free_length_num=i;
                    first_collision = true;
                    obj_speed = road.obstacles[k].v;
                    obj_heading = road.obstacles[k].yaw;

                }
            }
            else
            {
                if(!first_collision)
                {
                    free_length_num=i;
                }
            }
        }

    }

    free_length = 0;
    if(free_length_num <=0)
        return free_length;

    for (i = st_pos; i < free_length_num-10; i += 10)
    {
       free_length += length_of_two_navipoint(
                            lane.ref_points[i],
                            lane.ref_points[i+10]);
    }

    return result;
}

#endif // 0
