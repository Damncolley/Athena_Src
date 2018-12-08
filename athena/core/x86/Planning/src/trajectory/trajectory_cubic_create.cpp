#include <math.h>
#include <iostream>
#include <omp.h>

#include <GL/glu.h>
#include <GL/glut.h>

#include "common/path.h"
#include "common/math_util.h"
#include "common/navi_point.h"
#include "common/enum_list.h"
#include "vehicle_dynamic/steering_angle.h"

#include "trajectory/trajectory.h"
#include "trajectory/trajectory_sets.h"
#include "spline/quartic_spline.h"
#include "spline/math_tools.h"

///已知两点状态pos0/pos1，生成轨迹
int trajectory_cubic::gen_trj_from_two_points( navi_point pos0, navi_point pos1, vector<navi_point>& out_points )
{
    double length_s2e = -1.0;
    double heading_s2e = 1000.0;
    Transfer transfer;
    QuarticSpline q_spline;

    navi_point pos0_local, pos1_local, pi_local, pi_global;
    vector< navi_point > path_local, path_global;

    length_s2e = sqrt( pow( ( pos0.position_x - pos1.position_x ), 2 ) + pow( ( pos0.position_y - pos1.position_y ), 2 ) );
    if ( length_s2e > 400.0 )
    {
        cout << "\\\\\\\\\\\\\\\\\\\\\\\\\\\\Error : length_s2e = " << length_s2e << endl;
        end_pos_value = MAXIMUM_VALUE;
        return 0;
    }
    get_heading( pos0.position_x, pos0.position_y, pos1.position_x, pos1.position_y, heading_s2e );     //局部坐标系的x轴在全局坐标系下的头指向,单位：°
    heading_s2e -= 90.0;     //局部坐标系的y轴在全局坐标系下的头指向,单位：°
    if ( heading_s2e < 0.0 )
    {
        heading_s2e += 360.0; //转化成0到360度区间
    }

    /*设置起始点，用于坐标转换*/
    transfer.set_origin( pos0.position_x, pos0.position_y, heading_s2e );

    /*坐标转换：全局到局部*/
    pos0_local.position_x = 0.0;
    pos0_local.position_y = 0.0;
    pos0_local.heading = pos0.heading - heading_s2e;  //与y轴夹角,单位：°
    if ( pos0_local.heading > 360.0 )
    {
        pos0_local.heading -= 360.0;
    }
    if ( pos0_local.heading < 0.0 )
    {
        pos0_local.heading += 360.0;
    }
    pos0_local.k_s = pos0.k_s;

    pos1_local.position_x = length_s2e;
    pos1_local.position_y = 0.0;
    pos1_local.heading = pos1.heading - heading_s2e;  //与y轴夹角,单位：°
    if ( pos1_local.heading > 360.0 )
    {
        pos1_local.heading -= 360.0;
    }
    if ( pos1_local.heading < 0.0 )
    {
        pos1_local.heading += 360.0;
    }

//    if ( pos0_local.heading > 180.0 || pos1_local.heading > 180.0 )
//    {
//        cout << "\\\\\\\\\\\\\\\\\\\\\\\\\\\\Error : pos0_local.heading = " << pos0_local.heading << " ,pos1_local.heading = " << pos1_local.heading << endl;
//        return 0;
//    }

    double d_start = tan( to_radians( 90.0 - pos0_local.heading ) );
    double dd_start = pos0_local.k_s * pow( ( 1 + d_start * d_start ), 1.5 );
    double d_end = tan( to_radians( 90.0 - pos1_local.heading ) );

    /*四次样条曲线拟合*/
    q_spline.set_points( pos0_local.position_x, pos0_local.position_y,
                         pos1_local.position_x, pos1_local.position_y ); //设置控制点
    q_spline.set_boundary( d_start, dd_start, d_end, 0 );                //设置边界条件
    q_spline.compute_coef( 0 );

    path_local.clear();
    out_points.clear();
    //points_num = 0;
    pi_local.position_x = pos0_local.position_x;
    for ( ; pi_local.position_x <= pos1_local.position_x; pi_local.position_x += 0.1 )
    {
        pi_local.position_y = q_spline( pi_local.position_x );
        pi_local.heading = q_spline.compute_heading( pi_local.position_x );    //与y轴夹角,单位：°
        pi_local.k_s = q_spline.compute_k( pi_local.position_x );

        /*坐标转换：局部到全局*/
        transfer.transfer_l2g( pi_local.position_x, pi_local.position_y, pi_local.heading, pi_global.position_x, pi_global.position_y, pi_global.heading );
        pi_global.k_s = pi_local.k_s;

        /*储存轨迹*/
        path_local.push_back( pi_local );
        out_points.push_back( pi_global );
        //points_num++;
    }

    return 1;
}


