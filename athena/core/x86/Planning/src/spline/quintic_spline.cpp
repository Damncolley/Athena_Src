#include <iostream>
#include "spline/quintic_spline.h"
#include "spline/math_tools.h"

#include <math.h>
#include <cassert>



QuinticSpline::QuinticSpline()
{
    //ctor
    m_a_ = 0.0;
    m_b_ = 0.0;
    m_c_ = 0.0;
    m_d_ = 0.0;
    m_e_ = 0.0;

    m_x_start_ = 0.0;
    m_y_start_ = 0.0;
    m_x_end_ = 0.0;
    m_y_end_ = 0.0;

    m_first_deriv_start_ = 0.0;
    m_first_deriv_end_ = 0.0;
    m_second_deriv_start_ = 0.0;
    m_second_deriv_end_ = 0.0;
    m_third_deriv_start_ = 0.0;
    m_third_deriv_end_ = 0.0;
}

QuinticSpline::~QuinticSpline()
{
    //dtor
}

/************************************
设置起止点
************************************/
int QuinticSpline::set_points( double x0, double y0, double x1, double y1 )
{
    m_x_start_ = x0;
    m_y_start_ = y0;
    m_x_end_ = x1;
    m_y_end_ = y1;

    return 1;
}

/***************************************************
设置边界条件
deriv[2][3] : { 起点：一阶导，二阶导，三阶导;
                终点：一阶导，二阶导，三阶导 }
***************************************************/
int QuinticSpline::set_boundary( double dy0, double ddy0, double dy1, double ddy1, int flag )
{
    if ( flag == 0 )
    {
        m_first_deriv_start_ = dy0;
        m_second_deriv_start_ = ddy0;
        m_first_deriv_end_ = dy1;
        m_second_deriv_end_ = ddy1;
    }
    return 1;
}

/************************************
计算四次样条曲线系数
************************************/
int QuinticSpline::compute_coef( int flag )
{
    double delta_x = m_x_end_ - m_x_start_;
    double delta_x2 = delta_x * delta_x;
    double delta_x3 = delta_x2 * delta_x;
    double delta_x4 = delta_x2 * delta_x2;
    double delta_x5 = delta_x2 * delta_x2 * delta_x;
    double delta_y = m_y_end_ - m_y_start_;

    if ( flag == 0 )
    {
        m_d_ = 0.5 * m_second_deriv_start_;
        m_e_ = m_first_deriv_start_;
        m_f_ = m_y_start_;

        m_a_ = ( 12*delta_y - 6*(m_first_deriv_end_+m_first_deriv_start_)*delta_x + (m_second_deriv_end_-m_second_deriv_start_)*delta_x2) / (2*delta_x5);

        m_b_ = ( -30*delta_y + (14*m_first_deriv_end_+16*m_first_deriv_start_)*delta_x + (3*m_second_deriv_start_-2*m_second_deriv_end_)*delta_x2) / (2*delta_x4);

        m_c_ = ( 20*delta_y - (8*m_first_deriv_end_+12*m_first_deriv_start_)*delta_x + (m_second_deriv_end_-3*m_second_deriv_start_)*delta_x2) / (2*delta_x3);
    }

    return 1;
}

/************************************
拟合后计算任意点的值
x_start < x < x_end
************************************/
double QuinticSpline::operator() ( double x ) const
{
    assert( x >= m_x_start_ && x <= m_x_end_ );

    double y = 0.0;
    double dx = x - m_x_start_;

    y = m_a_ * pow( dx, 5 ) + m_b_ * pow( dx, 4 ) + m_c_ * pow( dx, 3 ) + m_d_ * pow( dx, 2 ) + m_e_ * pow( dx, 1 ) + m_f_;

    return y;

}

/************************************
拟合后计算任意点的一阶导
x_start < x < x_end
************************************/
double QuinticSpline::compute_first_deriv( double x )
{
    assert( x >= m_x_start_ && x <= m_x_end_ );

    double first_deriv_y = 0.0;
    double dx = x - m_x_start_;

    first_deriv_y = 5.0 * m_a_ * pow( dx, 4 ) + 4.0 * m_b_ * pow( dx, 3 ) + 3.0 * m_c_ * pow( dx, 2 ) + 2.0 * m_d_ * pow( dx, 1 ) + m_e_;

    return first_deriv_y;

}

/************************************
拟合后计算任意点的二阶导
x_start < x < x_end
************************************/
double QuinticSpline::compute_second_deriv( double x )
{
    assert( x >= m_x_start_ && x <= m_x_end_ );

    double second_deriv_y = 0.0;
    double dx = x - m_x_start_;

    second_deriv_y = 20.0 * m_a_ * pow( dx, 3 ) + 12.0 * m_b_ * pow( dx, 2 ) + 6.0 * m_c_ * pow( dx, 1 ) + 2.0 * m_d_;

    return second_deriv_y;

}

/************************************
拟合后计算任意点的头指向
************************************/
double QuinticSpline::compute_heading( double x )
{
    assert( x >= m_x_start_ && x <= m_x_end_ );

    double dy = 0.0, heading = 0.0;

    dy = compute_first_deriv( x );
    heading = r2d( atan( dy ) );

    heading = 90.0 - heading;

    if( heading > 360.0 )
        heading -= 360.0;


    return heading;
}

/************************************
拟合后计算任意点的曲率
************************************/
double QuinticSpline::compute_k( double x )
{
    assert( x >= m_x_start_ && x <= m_x_end_ );

    double k = 0.0;
    double dy = 0.0, ddy = 0.0;

    dy = compute_first_deriv( x );
    ddy = compute_second_deriv( x );
    k = ddy / pow( ( 1 + dy * dy ), 1.5 );

    return k;
}

/************************************
提取系数
************************************/
int QuinticSpline::get_coef( double *coef )
{
    coef[ 0 ] = m_a_;
    coef[ 1 ] = m_b_;
    coef[ 2 ] = m_c_;
    coef[ 3 ] = m_d_;
    coef[ 4 ] = m_e_;
    coef[ 5 ] = m_f_;

    return 1;
}

/************************************
生成曲线
************************************/
int QuinticSpline::gen_trj_from_two_points( navi_point pos0, navi_point pos1, double step )
{
    double length_s2e = -1.0;
    double heading_s2e = 1000.0;
    Transfer transfer;

    navi_point pos0_local, pos1_local, pi_local, pi_global;

    length_s2e = sqrt( pow( ( pos0.position_x - pos1.position_x ), 2 ) + pow( ( pos0.position_y - pos1.position_y ), 2 ) );
    if ( length_s2e > 400.0 )
    {
        cout << "\\\\\\\\\\\\\\\\\\\\\\\\\\\\Error : length_s2e = " << length_s2e << endl;
        return 0;
    }
    heading_s2e = get_heading( pos0.position_x, pos0.position_y, pos1.position_x, pos1.position_y );     ///局部坐标系的x轴在全局坐标系下的头指向,单位：°
    heading_s2e -= 90.0;     ///局部坐标系的y轴在全局坐标系下的头指向,单位：°
    if ( heading_s2e < 0.0 )
    {
        heading_s2e += 360.0; ///转化成0到360度区间
    }

    /**< 设置起始点，用于坐标转换. */
    transfer.set_origin( pos0.position_x, pos0.position_y, heading_s2e );

    /**< 坐标转换：全局到局部. */
    transfer.transfer_g2l( length_s2e, pos0, pos1, pos0_local, pos1_local );

    double h_start = d2r( 90.0 - pos0_local.heading );
    double h_end = d2r( 90.0 - pos1_local.heading );
    double d_start = tan( h_start );
    double dd_start = pos0_local.k_s * pow( ( 1 + d_start * d_start ), 1.5 );
    double d_end = tan( h_end );
    double dd_end = pos1_local.k_s * pow( ( 1 + d_end * d_end ), 1.5 );

    /**< 四次样条曲线拟合. */
    set_points( pos0_local.position_x, pos0_local.position_y,
                pos1_local.position_x, pos1_local.position_y ); ///设置控制点
    set_boundary( d_start, dd_start, d_end, dd_end, 0 );                ///设置边界条件
    compute_coef( 0 );

    path_local_.clear();
    path_global_.clear();
    //points_num = 0;
    pi_local.position_x = pos0_local.position_x;
    for ( ; pi_local.position_x <= pos1_local.position_x; pi_local.position_x += step )
    {
        pi_local.position_y = operator()( pi_local.position_x );
        pi_local.heading = compute_heading( pi_local.position_x );    //与y轴夹角,单位：°
        pi_local.k_s = compute_k( pi_local.position_x );

        /**< 坐标转换：局部到全局. */
        transfer.transfer_l2g( pi_local, pi_global );

        /**< 储存轨迹. */
        path_local_.push_back( pi_local );
        path_global_.push_back( pi_global );
    }

    return 1;
}

int QuinticSpline::get_path_local( vector< navi_point > &out_path_local )
{
    if ( path_local_.size() <= 0 )
    {
        cout << "[ warning ] : path_local_ is empty ! " << endl;
        return 0;
    }

    out_path_local.clear();
    out_path_local = path_local_;

    return 1;
}

int QuinticSpline::get_path_global( vector< navi_point > &out_path_global )
{
    if ( path_global_.size() <= 0 )
    {
        cout << "[ warning ] : path_global_ is empty ! " << endl;
        return 0;
    }

    out_path_global.clear();
    out_path_global = path_global_;

    return 1;
}



