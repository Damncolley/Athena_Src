#include <math.h>
#include <vector>
#include <iostream>

//#include "common/origin_vehicle.h"
#include "common/math_util.h"
#include "common/point.h"
#include "vehicle_dynamic/circle.h"
#include "vehicle_dynamic/heading.h"

using namespace std;

double calculate_R(
    double steering,
    double Ux)
{
    double R;
    double L_Ux;
    double steering_d;
    double Kug;
    double Wf, Wr;

    double L = 2.7;
    double g = 9.8;
    double Cf = 19000;
    double Cr = 21000;
    double l_a = 1.2;
    double l_b = 1.5;
    double m = 1200 + 150;

    Wf = (l_b/L) *m*g;
    Wr = (l_a/L) *m*g;

    Kug = Wf/Cf - Wr/Cr;

    L_Ux = (L + Kug*Ux*Ux/g);

    steering_d = to_radians(steering);

//   assert( fabs( steering_d ) < 0.0001);

    if( fabs( steering_d ) < 0.0001 )
        R = 100000;
    else
        R = 15 * L_Ux / fabs(steering_d);

    if( steering_d >=0 )
        R = R;
    else
        R = -R;

    return R;
}

void  derta_heading(double h1,
                    double h2,
                    double& derta_h)
{
    int index = ((int)( h2 )/90);
    int index_C = ((int)( h1 )/90);

    if( abs(index - index_C) == 0 )
        derta_h =  h2 - h1;
    if( abs(index - index_C) == 1 )
        derta_h =  h2 - h1;
    if( index ==0 && index_C == 3 )
        derta_h =  h2 - h1 + 360;
    if( index ==3 && index_C == 0 )
        derta_h =  h2 - h1 - 360;

    if( abs(index - index_C) == 2 ){
        if( index ==0 && index_C ==2){
            if( fabs(h2 - h1) > 180)
                derta_h = - (h2 + 360 - h1);
            else
                derta_h = h1 - h2;
        }
        if( index ==2 && index_C ==0){
            if( fabs(h2 - h1) > 180)
                derta_h = 360 + h1 - h2;
            else
                derta_h = h1 - h2 ;
        }
        if( index ==1 && index_C ==3){
            if( fabs(h2 - h1) > 180)
                derta_h = - (h2 + 360 - h1);
            else
                derta_h = h1 - h2;
        }
        if( index ==3 && index_C ==1){
            if( fabs(h2 - h1) > 180)
                derta_h = h1 + 360 - h2;
            else
                derta_h = -(h2 - h1);
        }
    }
    else
       derta_h = -derta_h;
}

double  cau_delta_heading(
        double current_heading,
        double head_c)
{
    int index = ((int)( current_heading )/90);
    int index_C = ((int)( head_c )/90);

    double d_heading = 0;

    if( abs(index - index_C) == 0 )
        d_heading =  current_heading - head_c;
    if( abs(index - index_C) == 1 )
        d_heading =  current_heading - head_c;
    if( index ==0 && index_C ==3 )
        d_heading =  current_heading - head_c + 360;
    if( index ==3 && index_C ==0 )
        d_heading =  current_heading - head_c - 360;

    if( abs(index - index_C) == 2 ){
        if( index ==0 && index_C ==2)
        {
            if( fabs(current_heading - head_c) > 180)
                d_heading = - (current_heading + 360 - head_c);
            else
                d_heading = head_c - current_heading;
        }
        if( index ==2 && index_C ==0)
        {
            if( fabs(current_heading- head_c) > 180)
                d_heading = 360 + head_c - current_heading;
            else
                d_heading = head_c - current_heading ;
        }
        if( index ==1 && index_C ==3)
        {
            if( fabs(current_heading- head_c) > 180)
                d_heading = - (current_heading + 360 - head_c);
            else
                d_heading = head_c - current_heading;
        }
        if( index ==3 && index_C ==1)
        {
            if( fabs(current_heading- head_c) > 180)
                d_heading = head_c + 360 - current_heading;
            else
                d_heading = -(current_heading - head_c);
        }
    }

    else
        d_heading = -d_heading;

    return d_heading;

}

//测试值

/*xx = 1, yy = 1.414,  l=2, t_d = 30;
xx = -1, yy = 1.414, l=2, t_d = -30;
xx = 1, yy = -1.414, l=2, t_d = 150;
xx = -1, yy = 1.414, l=2, t_d = 210;

xx = 0, yy = 10,  l=10, t_d = 0;
xx = 0, yy = -10, l=10, t_d = 180;
xx = 10, yy = 0,  l=10, t_d = 90;
xx = -10, yy = 0, l=10, t_d = 270;*/

// 求取相关点的头指向。
int get_heading_h(double x1, double y1,
                  double x2, double y2, double& h2, double& h_r, int num)
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
        theta_d = to_degrees(theta);
    }
    if( xx <0 && yy > 0) //4
    {
        theta = asin(xxx/length);
        theta_d = 360 - to_degrees(theta);
    }
    if( xx >0 && yy < 0) //2
    {
        theta = asin(xxx/length);
        theta_d = 180 - to_degrees(theta);
    }
    if( xx <0 && yy < 0) //3
    {
        theta = asin(xxx/length);
        theta_d = 180 + to_degrees(theta);
    }

    if( xx == 0 && yy > 0 ) theta_d = 0;
    if( xx == 0 && yy < 0 ) theta_d = 180;
    if( xx > 0 && yy == 0 ) theta_d = 90;
    if( xx < 0 && yy == 0 ) theta_d = 270;

    h2 = theta_d;

    if( fabs(h2 - h_r) > 5 )
    {
       // cout<< " i " << num << " x " <<  xx << " y " << yy << " l " << length
       //    << " the " << theta << " th " << h2 << " r " << h_r << endl;
    }

    h_r = theta_d;

    return 1;
}

// 求取相关点的头指向。
int get_heading(double x1, double y1,
                double x2, double y2, double& h2)
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
        theta_d = to_degrees(theta);
    }
    if( xx <0 && yy > 0) //4
    {
        theta = asin(xxx/length);
        theta_d = 360 - to_degrees(theta);
    }
    if( xx >0 && yy < 0) //2
    {
        theta = asin(xxx/length);
        theta_d = 180 - to_degrees(theta);
    }
    if( xx <0 && yy < 0) //3
    {
        theta = asin(xxx/length);
        theta_d = 180 + to_degrees(theta);
    }

    if( xx == 0 && yy > 0 ) theta_d = 0;
    if( xx == 0 && yy < 0 ) theta_d = 180;
    if( xx > 0 && yy == 0 ) theta_d = 90;
    if( xx < 0 && yy == 0 ) theta_d = 270;

    h2 = theta_d;

    return 1;
}

// 求取相关点的头指向。
int get_heading(double dx, double dy, double& h)
{
    double fabs_dx;
    double length = sqrt( dx*dx + dy*dy );

    if(length == 0)
        length = 0.000000001;

    double theta = 0.0;
    double theta_d = 0.0;

    fabs_dx = fabs(dx);
    // atan 计算的角度范围为 +-180度， 而且dx和dy有符号问题。需要测试一下。
    if( dx >0 && dy > 0) // 1
    {
        theta = asin(fabs_dx/length);
        theta_d = to_degrees(theta);
    }
    if( dx <0 && dy > 0) //4
    {
        theta = asin(fabs_dx/length);
        theta_d = 360 - to_degrees(theta);
    }
    if( dx >0 && dy < 0) //2
    {
        theta = asin(fabs_dx/length);
        theta_d = 180 - to_degrees(theta);
    }
    if( dx <0 && dy < 0) //3
    {
        theta = asin(fabs_dx/length);
        theta_d = 180 + to_degrees(theta);
    }

    if( dx == 0 && dy > 0 ) theta_d = 0;
    if( dx == 0 && dy < 0 ) theta_d = 180;
    if( dx > 0 && dy == 0 ) theta_d = 90;
    if( dx < 0 && dy == 0 ) theta_d = 270;

    h = theta_d;

    return 1;
}

double cau_heading_angle_from_ks(tk::spline s_x, tk::spline s_y,
                                 double cs,
                                 double every)
{
    double heading = 0;
    double x, y;
    double xx, yy;

    x = s_x(cs);
    y = s_y(cs);

    xx = s_x(cs + every);
    yy = s_y(cs + every);

    get_heading(x, y, xx, yy, heading);

    return heading;
}

double cau_heading_angle_from_ks(tk::spline s_x, tk::spline s_y,
                                 double cs)
{
    double heading = 0;
    double x, y;
    double first_deriv_x, second_deriv_x, first_deriv_y, second_deriv_y;

    s_x.compute_spline( cs, x, first_deriv_x, second_deriv_x );     //x(s)一阶段,二阶段
    s_y.compute_spline( cs, y, first_deriv_y, second_deriv_y );     //y(s)一阶段,二阶段

    //heading = first_deriv_y / first_deriv_x;
    get_heading( first_deriv_x, first_deriv_y, heading );


    return heading;
}

double cau_heading_angle_from_circle_ks(tk::spline s_x, tk::spline s_y,
                                        double cs,
                                        double every,
                                        double zero_offset)
{
    double heading = 0;
    double h_heading = 0;
    double x, y;
    double xx, yy;
    double xxx, yyy;
    double x_r, y_r;
    double r, ks;
    double e;

    //点在中间，左右各一个点的三点求园的方法。

    x = s_x(cs - every);
    y = s_y(cs - every);

    xx = s_x(cs);
    yy = s_y(cs);

    xxx = s_x(cs + every);
    yyy = s_y(cs + every);

    get_circle(x, y,
                xx, yy,
                xxx, yyy,
                x_r, y_r,
                r, ks);

    e = sign_circle_e(xx, yy,
                      x, y,
                      xxx, yyy);

    e = -e;  // 因为前后各两点， 所以符号意义相反了。
    get_heading(x_r, y_r, xx, yy, h_heading);

 //点在后面，向前两点求园的方法。
 /*   x = s_x(cs + every/2.0);
    y = s_y(cs + every/2.0);

    xx = s_x(cs);
    yy = s_y(cs);

    xxx = s_x(cs + every);
    yyy = s_y(cs + every);

    get_circle(x, y,
               xx, yy,
               xxx, yyy,
               x_r, y_r,
               r, ks);

    e = sign_circle_e(xx, yy,
                      x, y,
                      xxx, yyy);
   // e = -e;  // 因为前后各两点， 所以符号意义相反了。
    get_heading(x_r, y_r, xx, yy, h_heading);
*/

    h_heading += zero_offset; //-1.23;

    if(e >= 0)
    {
       heading = h_heading + 90;
       if(h_heading + 90 >= 360)
          heading = h_heading + 90 - 360;
       if(h_heading + 90 <= 0)
          heading = h_heading + 90 + 360;
    }
    else
    {
       heading = h_heading - 90;
       if(h_heading - 90 >= 360)
          heading = h_heading - 90 - 360;
       if(h_heading - 90 <= 0)
          heading = h_heading - 90 + 360;
    }

    if(heading == 0 ) heading = 0.001;
    if(heading == 180 ) heading = 180.001;
    if(heading == 90 ) heading = 90.001;
    if(heading == 270 ) heading = 270.001;
    if(heading == 360) heading = 0;

//    assert( heading != 90);
//    assert( heading != 270);

    return heading;
}


