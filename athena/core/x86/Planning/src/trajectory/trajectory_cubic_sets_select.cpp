#include <math.h>
#include <vector>
#include <fstream>
#include <iostream>

//#include "common/origin_vehicle.h"
#include "common/navi_point.h"
#include "common/math_util.h"
#include "planning/planning.h"
#include "map_matching/map_matching.h"
#include "common/enum_list.h"

#include "trajectory/trajectory_sets.h"
#ifdef debug
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <iomanip>
#endif

using namespace std;
// 碰撞检测
void trajectory_cubic_sets::collision_check(RoadSurface& road)
{
    unsigned int i;
    //碰撞检测。针对每一条轨迹。
    for(i=0; i<trajectorys.size(); i++)
    {
        //对每一条轨迹进行碰撞检测。
        trajectorys[i].collision_check(road);
      //  trajectorys[i].collision_check_moving_objects(road);
    }

    double tr_sum = 0;
    block_rate = 0;
    for(i=0; i<trajectorys.size(); i++)
    {
        if(trajectorys[i].collision_check_value > 200 )
          tr_sum += 1.0;
    }

    block_rate = tr_sum;

}

void trajectory_cubic_sets::cluster_collision_check( const RoadSurface& road, const double car_speed )
{
    unsigned int i;
    //碰撞检测。针对每一条轨迹。
    for(i=0; i<trajectorys.size(); i++)
    {
        //对每一条轨迹进行碰撞检测。
        trajectorys[i].traj_collision_check( road, car_speed );
    }
}

//平顺性检测
void trajectory_cubic_sets::cluster_ks_check()
{
    unsigned int i;
    //平顺性检测。针对每一条轨迹。
    for(i=0; i<trajectorys.size(); i++)
    {
        //计算每一条轨迹的最大曲率值
        trajectorys[i].traj_ks_check();
    }
}

// 连续性检测
void trajectory_cubic_sets::consistency_check()
{
    unsigned int i;
    //Ò»ÖÂÐÔ¼ì²â¡£Õë¶ÔÃ¿Ò»Ìõ¹ì¼£¡£
    for(i=0; i<trajectorys.size(); i++)
    {
        //¶ÔÃ¿Ò»Ìõ¹ì¼£½øÐÐÅö×²¼ì²â¡£
        trajectorys[i].consistency_check(traj_current);
    }

}

double offset_of_trajectory_from_lane(
     trajectory_cubic& tr,
     path& p,
     int change_lane_ending_lane,
     int in_lane)
{
    int j;
    double temp = 0;
    double value = 0;
    double x, y, yaw;
    double x_c, y_c, yaw_c;
//    int sign;

    if(tr.points.size()<=0)
        return 100000;

    if(tr.points_num-3 <=0)
        return 100000;

    int start_pos = tr.start_pos;

    navi_point np1, np2;

    // ×îºóÒ»¸öµã£¬ ÕÒ×î½üµÄ¡£
   /* if(change_lane_ending_lane ==  in_lane)
    {
        for(j = tr.points_num/2; j< tr.points_num; j+= 8)
        {
            np1 = tr.points[j];
            np2 = p.ref_points[start_pos+j];

            x = np1.position_x;
            y = np1.position_y;
            yaw = np1.heading;

            x_c = np2.position_x;
            y_c = np2.position_y;
            yaw_c = np2.heading;

            value = calculate_sign_and_value_offset(x, y, yaw, x_c, y_c, yaw_c);

            temp += fabs(value);
        }
    }
    else
    {
        for(j = tr.points_num -3; j< tr.points_num; j++)
        {
            np1 = tr.points[j];
            np2 = p.ref_points[start_pos+j];

            x = np1.position_x;
            y = np1.position_y;
            yaw = np1.heading;

            x_c = np2.position_x;
            y_c = np2.position_y;
            yaw_c = np2.heading;

            value = calculate_sign_and_value_offset(x, y, yaw, x_c, y_c, yaw_c);

            temp += fabs(value);
        }
    }
*/

    for(j = tr.points_num -1; j< tr.points_num; j++)
    {
        np1 = tr.points[j];
        np2 = p.ref_points[start_pos+j];

        x = np1.position_x;
        y = np1.position_y;
        yaw = np1.heading;

        x_c = np2.position_x;
        y_c = np2.position_y;
        yaw_c = np2.heading;

        value = calculate_sign_and_value_offset(x, y, yaw, x_c, y_c, yaw_c);
     //   value = sqrt( ( x - x_c)*( x - x_c) + (y - y_c) * (y - y_c) );
        temp += fabs(value);
    }

    return temp;

}

int trajectory_cubic_sets::nearest_tail_check(
    path& lane,
    int change_lane_ending_lane,
    int in_lane)
{
    unsigned int i;
    int best_one_count = 16;
    double temp = 1000;
    double best_value = 10000000;
    double value_temp[33];

    for(i = 0; i< trajectorys.size(); i++)
    {
        temp = offset_of_trajectory_from_lane(
                  trajectorys[i],
                  lane,
                  change_lane_ending_lane,
                  in_lane );

        value_temp[i] = temp;
        if(temp < best_value)
        {
            best_value = temp;
            best_one_count = i;
        }
    }

  //  best_one_count = 16;
  //  assert(best_one_count !=0);

    return best_one_count;
}

double both_side_offset_of_trajectory_from_lane(trajectory_cubic& tr,
                                               path& p,
                                               double left_offset,
                                               double right_offset)
{
    int j;
    double temp = 0;
    double value = 0;
    double x, y, yaw;
    double x_c, y_c, yaw_c;
//    int sign;

    if(tr.points.size()<=0)
        return 100000;

    int start_pos = tr.start_pos;

    navi_point np1, np2;

    tr.left_offset_value = 0;
    tr.right_offset_value = 0;

    for(j = 0; j< tr.points_num; j++)
    {
        np1 = tr.points[j];
        np2 = p.ref_points[start_pos+j];  // ¿ÉÄÜÊÇÖ±µÀ£¬¶ÔÓ¦µã¶¼¶Ô£¬¡¡ÍäµÀµÄÊ±ºòÓÐÊ±ºò¶ÔÓ¦µã²»¶Ô¡£

        x = np1.position_x;
        y = np1.position_y;
        yaw = np1.heading;

        x_c =  np2.position_x;
        y_c =  np2.position_y;
        yaw_c = np2.heading;

        value = calculate_sign_and_value_offset(x, y, yaw,  x_c, y_c, yaw_c);

        // ×ó±ß³ö½ç
        if(value < left_offset)
        {
            tr.left_offset_value = 1000;
        }

        // ×ó±ßÌù±ß
      /*  if(value -1 < left_offset && value >= left_offset)
        {
            tr.left_offset_value = 500;
        }

        // ÓÒ±ßÌù±ß
        if(value +1 > right_offset && value <= right_offset)
        {
            tr.right_offset_value = 500;
        }
      */

        // ÓÒ±ß³ö½ç
        if(value > right_offset)
        {
            tr.right_offset_value = 1000;
        }

        // Õâ¸ö¼ÆËãÃ»ÓÐÒâÒå¡£
        temp += fabs(value);
    }

    return temp;

}


 double lane_value[]=
{
   35, 30, 25, 20,
    15, 10, 5, 0,
   -5, -10, -15, -20,
    -25, -30, -35, -40,
   -50,
   -40, -35, -30, -25,
    -20, -15, -10, -5,
   0, 5, 10, 15,
   20, 25, 30, 35,
};

///zp20171027:Æ¥Åä²¢½ØÈ¡±ß½çÏß
int extract_edge( point_xys tar_point, line_xys in_line, line_xys &out_line, int ext_before, int ext_back, int st_pos, int en_pos )
{
    map_matching match_edge;
    int match_pos = -1;
    double min_error = 0.0;

    match_pos =
    match_edge.MapMarch_Min_Distance_motion_planning(
        tar_point.x,
        tar_point.y,
        in_line,
        st_pos,
        en_pos,
        min_error, //15
        20);///zp£º20171127 À©´óËÑË÷±ß½çÆ¥ÅäµÄ×î´óãÐÖµ
//    cout << "~~~~~~~~match_pos = " << match_pos << endl;

    out_line.line.clear();
    ///zp20171109:·ÀÖ¹³öÏÖ¸ºÊý
    int num_before = match_pos + ext_before;
    if ( num_before < 0 )
        num_before = 0;
    for ( int j = num_before; j < match_pos + ext_back; j++ )
    {
        out_line.line.push_back( in_line.line[ j ] );
    }

    out_line.num_of_points = out_line.line.size();

    return 1;
}


int trajectory_cubic_sets::out_side_traj_check( line_xys left_edge, line_xys right_edge, int st_pos, int en_pos, double cost_value )
{
    CollisionCheck col_check;
    unsigned int i;
    int j;
//    bool ret = false, flag = false;
    //point_xys temp_point;
    double car_x = 0.0, car_y = 0.0, car_h = 0.0;
    rect rt_car;
    point poi_left_edge, poi_right_edge;
    bool b_outside = false;
    route_planning_m route;

    //截取边界线
    line_xys m_left_edge, m_right_edge;
    m_left_edge.line.clear();
    m_right_edge.line.clear();
    for ( int k = st_pos; k < en_pos; k++ )
    {
        m_left_edge.line.push_back( left_edge.line[ k ] );
        m_right_edge.line.push_back( right_edge.line[ k ] );
    }
    m_left_edge.num_of_points = m_left_edge.line.size();
    m_right_edge.num_of_points = m_right_edge.line.size();

    #if 1
    for ( i = 0; i < trajectorys.size(); i++ )
    {
        for ( j = 0; j < trajectorys[i].points_num_1; j += OUT_SIDE_INTERVAL )
        {
            trajectorys[i].out_side_num = 9999; ///zp20171027:out_side_num第几个点出界

            car_x = trajectorys[i].points[j].position_x;
            car_y = trajectorys[i].points[j].position_y;
            car_h = trajectorys[i].points[j].heading;
            //计算车辆包围盒rt_car,车长宽加上保护距离
            col_check.car_lane_collision_check_rect( rt_car, car_x, car_y, car_h, 0.0 );

            for ( int ii = 0; ii < m_left_edge.num_of_points; ii++ )
            {
                poi_left_edge.xx = m_left_edge.line[ii].x;
                poi_left_edge.yy = m_left_edge.line[ii].y;
                b_outside = rt_car.is_point_in_xx( poi_left_edge );
                if ( b_outside )
                {
                    trajectorys[i].out_side_num = j;
                    trajectorys[i].left_offset_value = cost_value;
                    break;
                }

                poi_right_edge.xx = m_right_edge.line[ii].x;
                poi_right_edge.yy = m_right_edge.line[ii].y;
                b_outside = rt_car.is_point_in_xx( poi_right_edge );
                if ( b_outside )
                {
                    trajectorys[i].out_side_num = j;
                    trajectorys[i].right_offset_value = cost_value;
                    break;
                }
            }

            if ( b_outside )
            {
                break;
            }

        }

    }
#endif // 0

#if 0
    for ( i = 0; i < trajectorys.size(); i++ )
    {
        for ( j = 0; j < trajectorys[i].points_num; j += 10 )
        {
            trajectorys[i].out_side_num = 9999; ///zp20171027:out_side_num第几个点出界

            temp_point.x = trajectorys[i].points[j].position_x;
            temp_point.y = trajectorys[i].points[j].position_y;

            ret = route.check_point(m_left_edge,m_right_edge,temp_point,SAFE_WIDTH_LANE);

            if ( !ret )
            {
                trajectorys[i].left_offset_value = cost_value;
                trajectorys[i].right_offset_value = cost_value;
                trajectorys[i].out_side_num = 0; ///zp20171027:Èç¹û³ö½ç£¬out_side_numÖµÖÀ0
            }

        }

    }
#endif // 0

    return 1;

}

///综合评价
int trajectory_cubic_sets::select_best_traj( const route_planning_m& route_planning, const int ref_pos1, const int ref_pos2, const int car_action, const int drive_mode )
{
    int best_one_count;

    /*初始化代价值*/
//    double min_diff = 9999.0;
//    int min_index = -1;
    for(unsigned int i=0; i < trajectorys.size(); i++)
    {
        trajectorys[i].select_value = 0;
        trajectorys[i].left_offset_value = 0;
        trajectorys[i].right_offset_value = 0;

        //找到距换道点最近的轨迹
//        int num_points = trajectorys[i].points.size();
//        double dx = nad_target_point.position_x - trajectorys[i].points[num_points-1].position_x;
//        double dy = nad_target_point.position_y - trajectorys[i].points[num_points-1].position_y;
//        double diff = sqrt( dx*dx + dy*dy );
//        if( diff < min_diff )
//        {
//            min_diff = diff;
//            min_index = i;
//        }
    }
     //距换道点最近的轨迹的代价值下调（优先走该轨迹）
//    cout << "++++min_index = " << min_index << endl;
//    trajectorys[min_index].end_pos_value -= 1000;

    /*边界检测*/
    int st_pos = ref_pos1 - NUM_BACKWARD_TRAJ_CHECK;
    if ( st_pos < 0 )
        st_pos = 0;
    unsigned int en_pos = ref_pos2 + NUM_FORWARD_TRAJ_CHECK;
    if ( en_pos > route_planning.left_line.line.size() )
        en_pos = route_planning.left_line.line.size();

    //车队模式时不允许借道
    if ( car_action != CAR_ACTION_SINGLE )
    {
        out_side_traj_check( route_planning.left_line, route_planning.right_line, st_pos, en_pos, OUT_EDGE_COST_VALUE );
        out_side_traj_check( route_planning.left_edge, route_planning.right_edge, st_pos, en_pos, OUT_EDGE_COST_VALUE );
    }
    else
    {
        out_side_traj_check( route_planning.left_line, route_planning.right_line, st_pos, en_pos, OUT_LINE_COST_VALUE );
        out_side_traj_check( route_planning.left_edge, route_planning.right_edge, st_pos, en_pos, OUT_EDGE_COST_VALUE );
    }

    /*选取代价值之和最小的轨迹为最优轨迹*/
    double best_value = MAXIMUM_VALUE;
    double a = COEF_END_POS, b = COEF_COL_CHECK, c = COEF_LEFT, d = COEF_RIGHT, e = COEF_KS;
//
//    if ( driving_mode == DRIVING_MODE_CAR_NET_CONTROL )
//    {
//        e = 0;  //如果是网络模式，不考虑曲率的影响，避免不执行网络换道
//    }

    //如果是车队的跟随车，不考虑与前车的碰撞
    if ( car_action == CAR_ACTION_PLATOON_OTHER || drive_mode == DRIVE_MODE_FOLLOW )//|| g_obu_planning->b_slow_down )
    {
        b = 0;
    }

    for( unsigned int i=0; i < trajectorys.size(); i++ )
    {
        if ( trajectorys[i].ks_sum_value < THRESHOLD_KS )
        {
            e = 0;
        }
        //代价值之和
        trajectorys[i].select_value = a * trajectorys[i].end_pos_value              //终点的影响
                                    + b * trajectorys[i].collision_check_value      //碰撞
                                    + c * trajectorys[i].left_offset_value          //左边界
                                    + d * trajectorys[i].right_offset_value         //右边界
                                    + e * trajectorys[i].ks_sum_value;              //曲率变化

        //最小值
        if( trajectorys[i].select_value < best_value )
        {
           best_one_count = i;
           best_value = trajectorys[i].select_value;
        }
    }

    /*赋值最优轨迹*/
    best_one = best_one_count;
    p_best_trajectorys = &(trajectorys[best_one]);

    if(SELECT_VALUE_VIEW_SWITCH)
    {
        printf("\033[2J");

          cout  << " best_one_count " << best_one_count << " "
                << endl << endl;

        for( unsigned int i=0; i < trajectorys.size(); i++)
           cout << " " << i << " "
                << " c_t " << trajectorys[i].collision_time  << " "
                << " c_d " << trajectorys[i].collision_distance << " "
                << " tr-left " << trajectorys[i].left_offset_value << " "
                << " tr-right " << trajectorys[i].right_offset_value << " "
                << " consistency " << trajectorys[i].consistency_value << " "
                << " collision " << trajectorys[i].collision_check_value << " "
                << " sum " << trajectorys[i].select_value << endl;

    }

    if(SELECT_VALUE_LOG_SWITCH)
    {
        ofstream outfile("select_value_log.log", std::ios::app);
        outfile.precision(8);

        outfile << " best_one_count:" << best_one_count
                << " car_action:" << car_action
                << endl;

        for( unsigned int i=0; i < trajectorys.size(); i++)
           outfile  << " " << i << " "
                    << " end_pos " << trajectorys[i].end_pos_value  << " "
                    << " collision " << trajectorys[i].collision_check_value << " "
                    << " c_speed " << trajectorys[i].collision_obj_speed << " "
                    << " tr-left " << trajectorys[i].left_offset_value << " "
                    << " tr-right " << trajectorys[i].right_offset_value << " "
                    << " k_s " << trajectorys[i].ks_sum_value << " "
                    << " sum " << trajectorys[i].select_value << endl;

        outfile << endl;
        outfile.close();
    }

    return 1;
}

