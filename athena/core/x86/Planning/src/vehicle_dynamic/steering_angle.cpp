#include <math.h>
#include "spline/spline.h"
#include "common/math_util.h"
#include "common/point.h"
#include "vehicle_dynamic/circle.h"
#include "vehicle_dynamic/heading.h"

using namespace std;

double calculate_sign_and_value_offset(
                          double x, double y, double yaw,
                          double x_c, double y_c, double yaw_c)
{
    double xa = x_c, ya = y_c;
    double xb = x_c + 1*sin(to_radians(yaw_c));
    double yb = y_c + 1*cos(to_radians(yaw_c));
    double xc = x, yc = y;

    // 在一条直线上
    double f = (xb - xa) * (yc - ya) - (xc - xa) * (yb - ya);
    double length = sqrt( (xa - xb) * (xa - xb) +  (ya - yb) * (ya - yb));

    double offset_value = fabs(f/length);

    if(f > 0)
        return -1*offset_value;
    else if(f == 0)
        return 0;
    else
        return  1*offset_value;
}

//返回第一个点的曲率
double cau_ks_by_3_points( double x_[3], double y_[3] )
{
    double x, y;
    double xx, yy;
    double xxx, yyy;
    double x_r, y_r;
    double r;
    double e;
    double ks;

    x = x_[0];
    y = y_[0];
    xx = x_[1];
    yy = y_[1];
    xxx = x_[2];
    yyy = y_[2];

    get_circle(x, y,
               xx, yy,
               xxx, yyy,
               x_r, y_r,
               r, ks);

    e = sign_circle_e(x, y,
                      xx, yy,
                      xxx, yyy);

    ks = e*ks;

    return ks;
}

double cau_steering_angle_from_ks(tk::spline s_x,
                                  tk::spline s_y,
                                  double cs,
                                  double Ux,
                                  double& rr,
                                  double every1,
                                  double every2)
{

    double x, y;
    double xx, yy;
    double xxx, yyy;

    double x_r, y_r;
    double r;
    double e;
    double ks;
    double ks_L;
    double steering_ks;
    double Kug;
    double Wf, Wr;

    double L = 2.7;
    double g = 9.8;
    double Cf = 19000;
    double Cr = 21000;
    double l_a = 1.2;
    double l_b = 1.5;
    double m = 1200 + 150;

    x = s_x(cs);
    y = s_y(cs);

    xx = s_x(cs + every1);
    yy = s_y(cs + every1);

    xxx = s_x(cs + every2);
    yyy = s_y(cs + every2);

    get_circle(x, y,
               xx, yy,
               xxx, yyy,
               x_r, y_r,
               r, ks);

    e = sign_circle_e(x, y,
                      xx, yy,
                      xxx, yyy);

    ks = e*ks;

    Wf = (l_b/L) *m*g;
    Wr = (l_a/L) *m*g;

    Kug = Wf/Cf - Wr/Cr;

    ks_L = (L + Kug*Ux*Ux/g)*ks;
    steering_ks = 15 * to_degrees( ks_L);

    rr = e*r;

    return steering_ks;
}

double cau_steering_angle_from_ks(tk::spline s_x,
                                  tk::spline s_y,
                                  double cs,
                                  double Ux,
                                  double& rr)
{
    double x, y;
//    double x_r, y_r;
    double r;
//    double e;
    double ks;
    double ks_L;
    double steering_ks;
    double Kug;
    double Wf, Wr;

    double L = 2.7;
    double g = 9.8;
    double Cf = 19000;
    double Cr = 21000;
    double l_a = 1.2;
    double l_b = 1.5;
    double m = 1200 + 150;

    //计算曲率
    double first_deriv_x, second_deriv_x, first_deriv_y, second_deriv_y;
    s_x.compute_spline( cs, x, first_deriv_x, second_deriv_x );     //x(s)一阶段,二阶段
    s_y.compute_spline( cs, y, first_deriv_y, second_deriv_y );     //y(s)一阶段,二阶段
    ks = compute_ks_from_spline( first_deriv_x, second_deriv_x, first_deriv_y, second_deriv_y );
    r = 1 / ks;

    Wf = (l_b/L) *m*g;
    Wr = (l_a/L) *m*g;

    Kug = Wf/Cf - Wr/Cr;

    ks_L = (L + Kug*Ux*Ux/g)*ks;
    steering_ks = 15 * to_degrees( ks_L);

    return steering_ks;
}
