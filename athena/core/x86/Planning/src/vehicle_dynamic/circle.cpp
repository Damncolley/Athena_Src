#include <math.h>
#include <vector>
#include <iostream>

#include "common/math_util.h"
#include "common/point.h"
#include "vehicle_dynamic/circle.h"
#include "vehicle_dynamic/heading.h"

int get_circle(double x1, double y1,
               double x2, double y2,
               double x3, double y3,
               double& x, double& y,
               double& r, double& ks)
{
    double a, b, c, d, e, f, g;

    a=2*(x2-x1);
    b=2*(y2-y1);

    d=2*(x3-x2);
    e=2*(y3-y2);

    c=x2*x2+y2*y2-x1*x1-y1*y1;
    f=x3*x3+y3*y3-x2*x2-y2*y2;

    g = b*d - e*a;

    if(fabs(g) <= 0.00000001)
       g = 0.00000001;

    x=(b*f-e*c)/g;  //中心坐标
    y=(d*c-a*f)/g;

    r = sqrt((x-x1)*(x-x1)+(y-y1)*(y-y1));

    if(fabs(g) > 0.00000001)
       ks = 1/r;
    else
       ks = 0;

    return 1;
}

int sign_circle_e(double x, double y,
                  double x1, double y1,
                  double x2, double y2)
{
    double xa = x1, ya = y1;
    double xb = x2, yb = y2;
    double xc = x, yc = y;

    // 在一条直线上
    double f = (xb - xa) * (yc - ya) - (xc - xa) * (yb - ya);

    if(f > 0) return -1;
 //   else if(f == 0) return 0;
    else return  1;
}

double compute_ks_from_spline( double first_deriv_x, double second_deriv_x,
                               double first_deriv_y, double second_deriv_y )
{
    double numerator_ks, denominator_ks, ks;

    numerator_ks = first_deriv_x * second_deriv_y - second_deriv_x * first_deriv_y;
    denominator_ks = pow( ( first_deriv_x * first_deriv_x + first_deriv_y * first_deriv_y ), 1.5 );
    //ks = fabs( numerator_ks ) / denominator_ks;
    ks = - numerator_ks / denominator_ks;

    return ks;

}
