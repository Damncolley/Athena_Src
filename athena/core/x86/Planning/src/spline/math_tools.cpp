#include <iostream>
#include "spline/math_tools.h"

#define PI 3.14159265358979323846264338

using namespace std;

double d2r( double d )
{
    double r;

    r = d * PI / 180.0;  //角度转弧度

    return r;
}

double r2d( double r )
{
    double d;

    d = r * 180.0 / PI;  //弧度转角度

    return d;
}

//根据两个点求取头指向
//局部坐标系的x轴在全局坐标系下的头指向,单位：°
double get_heading( double x1, double y1,
                 double x2, double y2 )
{
    double xxx;
    double xx = (x2-x1);
    double yy = (y2-y1);

    double length = sqrt( xx*xx + yy*yy );

    if(length == 0)
        length = 0.000000001;

    double theta = 0.0;
    double theta_d = 0.0;

    xxx = fabs(xx);
    // atan 计算的角度范围为 +-180度， 而且xx和yy有符号问题。需要测试一下。
    if( xx >0 && yy > 0) // 1
    {
        theta = asin(xxx/length);
        theta_d = r2d(theta);
    }
    if( xx <0 && yy > 0) //4
    {
        theta = asin(xxx/length);
        theta_d = 360 - r2d(theta);
    }
    if( xx >0 && yy < 0) //2
    {
        theta = asin(xxx/length);
        theta_d = 180 - r2d(theta);
    }
    if( xx <0 && yy < 0) //3
    {
        theta = asin(xxx/length);
        theta_d = 180 + r2d(theta);
    }

    if( xx == 0 && yy > 0 ) theta_d = 0;
    if( xx == 0 && yy < 0 ) theta_d = 180;
    if( xx > 0 && yy == 0 ) theta_d = 90;
    if( xx < 0 && yy == 0 ) theta_d = 270;

    return theta_d;
}

/********************** class of Transfer **********************/
Transfer::Transfer()
{
    m_x0_ = 0.0;
    m_y0_ = 0.0;
    m_angle_ = 0.0;
}

Transfer::~Transfer()
{

}

int Transfer::set_origin( double x0, double y0, double angle )
{
    m_x0_ = x0;
    m_y0_ = y0;
    m_angle_ = angle;

    return 1;
}

int Transfer::transfer_l2g( double x_local, double y_local, double &x_global, double &y_global )
{
    double angle;
    angle = -d2r( m_angle_ );

    //旋转
    x_global = cos( angle ) * x_local - sin( angle ) * y_local;
    y_global = sin( angle ) * x_local + cos( angle ) * y_local;

    //平移
    x_global += m_x0_;
    y_global += m_y0_;

    return 1;
}


//局部坐标系转全局坐标系下
int Transfer::transfer_l2g( double x_local, double y_local, double h_local,
                            double &x_global, double &y_global, double &h_global )
{
    transfer_l2g( x_local, y_local, x_global, y_global );

    h_global = h_local + m_angle_;

    //[ 0, 180 ) && [ -180, 0 )
//    if ( h_global > 180.0 )
//        h_global -= 360.0;

    //[ 0, 360 )
    if ( h_global > 360.0 )
        h_global -= 360.0;

    return 1;
}

//局部坐标系转全局坐标系下
int Transfer::transfer_l2g( navi_point p_local, navi_point &p_global )
{
    transfer_l2g( p_local.position_x, p_local.position_y, p_local.heading, p_global.position_x, p_global.position_y, p_global.heading );
    p_global.k_s = p_local.k_s;

    return 1;
}

//全局坐标系转局部坐标系下
int Transfer::transfer_g2l( double length,
                            navi_point p0_global, navi_point p1_global,
                            navi_point &p0_local, navi_point &p1_local )
{
    ///起点的局部坐标
    p0_local.position_x = 0.0;
    p0_local.position_y = 0.0;
    p0_local.heading = p0_global.heading - m_angle_;  ///与y轴夹角,单位：°
    if ( p0_local.heading > 360.0 )
    {
        p0_local.heading -= 360.0;
    }
    if ( p0_local.heading < 0.0 )
    {
        p0_local.heading += 360.0;
    }
    p0_local.k_s = p0_global.k_s;

    ///终点的局部坐标
    p1_local.position_x = length;
    p1_local.position_y = 0.0;
    p1_local.heading = p1_global.heading - m_angle_;  ///与y轴夹角,单位：°
    if ( p1_local.heading > 360.0 )
    {
        p1_local.heading -= 360.0;
    }
    if ( p1_local.heading < 0.0 )
    {
        p1_local.heading += 360.0;
    }
    p1_local.k_s = p1_global.k_s;

//    if ( p0_local.heading > 180.0 || p1_local.heading > 180.0 )
//    {
//        cout << "\\\\\\\\\\\\\\\\\\\\\\\\\\\\Error : p0_local.heading = " << p0_local.heading << " ,p1_local.heading = " << p1_local.heading << endl;
//        return 0;
//    }

    return 1;
}

/**
* @brief 将全局坐标系的导航点坐标,航向角和曲率转换到局部坐标系下。
* @param point_global 输入量：全局坐标系下起点。
* @param point_local 输出量：局部坐标系下终点。
*/
int Transfer::transfer_g2l( navi_point point_global, navi_point &point_local )
{
    double delta_heading = -1.0;
    delta_heading = d2r( m_angle_ ) ; //角度转弧度

    ///世界坐标系转全局坐标系，需要先平移后旋转
    double delta_x = point_global.position_x - m_x0_;
    double delta_y = point_global.position_y - m_y0_;

    point_local.heading = point_global.heading - m_angle_;
    point_local.position_x = delta_x*cos( delta_heading ) - delta_y*sin( delta_heading );
    point_local.position_y = delta_x*sin( delta_heading ) + delta_y*cos( delta_heading );
    point_local.k_s = point_global.k_s;


    if ( point_local.heading > 360.0 )
    {
        point_local.heading -= 360.0;
    }
    if ( point_local.heading < 0.0 )
    {
        point_local.heading += 360.0;
    }

//    if ( p0_local.heading > 180.0 || p1_local.heading > 180.0 )
//    {
//        cout << "\\\\\\\\\\\\\\\\\\\\\\\\\\\\Error : p0_local.heading = " << p0_local.heading << " ,p1_local.heading = " << p1_local.heading << endl;
//        return 0;
//    }

    return 1;

}


/********************** class of Transfer **********************/




