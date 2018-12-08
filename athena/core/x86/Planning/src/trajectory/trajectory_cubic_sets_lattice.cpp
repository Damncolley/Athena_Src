#include <math.h>
#include <vector>

//#include "../../common/origin_vehicle.h"
#include "common/navi_point.h"
#include "common/math_util.h"
#include "vehicle_dynamic/heading.h"
#include "vehicle_dynamic/steering_angle.h"

#include "trajectory/trajectory_sets.h"

// 当出发处于轨迹中间的时候  如果处于道路边界的时候需要裁剪相应的行驶可能轨迹
    /*
              0   1   2   3
    0    //               3
    1    //           2   2
    2    //       1   1   1
    3    //   0   0   0   0
    4    //      -1  -1  -1
    5    //          -2  -2
    6    //              -3
    */

    /*  总共27种变化
    0 // （3，0）  （2，1）  （1， 2）  （ 0，3）
    1 // （3，0）  （2，1）  （1， 2）  （ 1，3）
    2 // （3，0）  （2，1）  （1， 2）  （ 2，3）

    3 // （3，0）  （2，1）  （2， 2）  （ 1，3）
    4 // （3，0）  （2，1）  （2， 2）  （ 2，3）
    5 // （3，0）  （2，1）  （2， 2）  （ 3，3）

    6 // （3，0）  （2，1）  （3， 2）  （ 2，3）
    7 // （3，0）  （2，1）  （3， 2）  （ 3，3）
    8 // （3，0）  （2，1）  （3， 2）  （ 4，3）

    9 // （3，0）  （3，1）  （2， 2）  （ 1，3）
    10 // （3，0）  （3，1）  （2， 2）  （ 2，3）
    11 // （3，0）  （3，1）  （2， 2）  （ 3，3）

    12 // （3，0）  （3，1）  （3， 2）  （ 2，3）
    13 // （3，0）  （3，1）  （3， 2）  （ 3，3）
    14 // （3，0）  （3，1）  （3， 2）  （ 4，3）

    15 // （3，0）  （3，1）  （4， 2）  （ 3，3）
    16 // （3，0）  （3，1）  （4， 2）  （ 4，3）
    17 // （3，0）  （3，1）  （4， 2）  （ 5，3）

    18 // （3，0）  （4，1）  （3， 2）  （ 2，3）
    19 // （3，0）  （4，1）  （3， 2）  （ 3，3）
    20 // （3，0）  （4，1）  （3， 2）  （ 4，3）

    21 // （3，0）  （4，1）  （4， 2）  （ 3，3）
    22 // （3，0）  （4，1）  （4， 2）  （ 4，3）
    23 // （3，0）  （4，1）  （4， 2）  （ 5，3）

    24 // （3，0）  （4，1）  （5， 2）  （ 4，3）
    25 // （3，0）  （4，1）  （5， 2）  （ 5，3）
    26 // （3，0）  （4，1）  （5， 2）  （ 6，3）
    */

// 生成lattice所定义的主要变化方式的各种轨迹
void trajectory_cubic_sets::generate_trajectorys_lattice(int s_pos, int e_pos, int length,
                                                        double current_x, double current_y, double current_speed_lon,
                                                        double current_heading,  double current_steer_angle,
                                                        double vertical_offset_1, double vertical_offset_2)
{
    navi_point nav_c_points[4];

    double d_off;
    double dd_off;

    double  direction;
    double  vertical_direction;
    double  vertical_length[4];

    double max_lateral_offset = LANE_WIDTH * 2.0 ;
    double d_lateral_offset =  LANE_WIDTH / 8.0 ;
    double lateral_off;

    if(e_pos - s_pos >= 2000) return;


    double max_longitudinal = 20 * 100;  //需要用里程来替代
    double d_longitudinal = 100; //需要用里程来替代

    unsigned int i;
    int t;
    int l;

    // 横向偏移的七个
    for( t=0 ; t < 33; t ++)
    {
        // 纵向偏移四个
        for( l = 0; l<4; l++)
        {
            //由于每次规划都是拷贝足够的参考轨迹点， 所以是参考的轨迹。
            int num_every = (e_pos - s_pos)/3;
            int first_no = 0*num_every;
            int second_no = 1*num_every;
            int thrid_no =  2*num_every;
            int fouth_no = 3*num_every;

            //轨迹是奇数条保证中间轨迹符合中心线的要求。
            lateral_off = - max_lateral_offset + t*d_lateral_offset;

            calculate_d_dd(current_heading,
                           current_steer_angle,
                           current_speed_lon,
                           heading_r,
                           d_off,dd_off);

            dd_off = 0;
            nav_c_points[0].position_x = current_x;
            nav_c_points[0].position_y = current_y;
            nav_c_points[0].heading = current_heading;
            nav_c_points[0].steering_angle = current_steer_angle;
            nav_c_points[0].d_lateral_offset = d_off;  //p1 和头指向与轨迹的头指向的偏移程度相关。 还有当时的车速。
            nav_c_points[0].d2_lateral_offset = dd_off;  //p2

            // 保证坐标位置连续 //最大16倍
            nav_c_points[0].lateral_offset = vertical_length[0] = 0;  //还应该带上当前点的横向偏移吧。
            nav_c_points[1].lateral_offset = vertical_length[1] = lateral_off * vertical_offset_1;
            nav_c_points[2].lateral_offset = vertical_length[2] = lateral_off * vertical_offset_2;
            nav_c_points[3].lateral_offset = vertical_length[3] = lateral_off;

            //这边点号都是参考轨迹点的位置。
            nav_c_points[0].point_no = first_no;
            nav_c_points[1].point_no = second_no;
            nav_c_points[2].point_no = thrid_no;
            nav_c_points[3].point_no = fouth_no;

            nav_c_points[0].s = ref_path_points[first_no].s;
            nav_c_points[1].s = ref_path_points[second_no].s;
            nav_c_points[2].s = ref_path_points[thrid_no].s;
            nav_c_points[3].s = ref_path_points[fouth_no].s;

            trajectorys[t].set_control_points(nav_c_points);
            trajectorys[t].set_trajectory_sets((trajectory_cubic_sets* )this);

            //    cout << " start : " <<  s_pos  << " tr : " << t << endl;
            trajectorys[t].generate_trajectory_lateral_offset_ks(current_speed_lon);
        }
    }
}
