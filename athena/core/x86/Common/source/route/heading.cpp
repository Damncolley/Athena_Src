#include <math.h>
#include <vector>
#include <iostream>

#include "math_util.h"
#include "heading.h"

using namespace std;

//#define to_radians(x) ( (x) * (PI / 180.0 ))
//#define to_degrees(x) ( (x) * (180.0 / M_PI ))

//测试值

/*xx = 1, yy = 1.414,  l=2, t_d = 30;
xx = -1, yy = 1.414, l=2, t_d = -30;
xx = 1, yy = -1.414, l=2, t_d = 150;
xx = -1, yy = 1.414, l=2, t_d = 210;

xx = 0, yy = 10,  l=10, t_d = 0;
xx = 0, yy = -10, l=10, t_d = 180;
xx = 10, yy = 0,  l=10, t_d = 90;
xx = -10, yy = 0, l=10, t_d = 270;*/

double  cau_derta_heading(
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


//求点到直线的距离
double calculate_sign_and_value_offset(
                          double x, double y,
                          double x_a, double y_a, double x_b, double y_b)
{
    double xa = x_a, ya = y_a;
    double xb = x_b;
    double yb = y_b;
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
       cout<< " i " << num << " x " <<  xx << " y " << yy << " l " << length
           << " the " << theta << " th " << h2 << " r " << h_r << endl;
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

    if( abs(index - index_C) == 2 )
    {
        if( index ==0 && index_C ==2)
        {
            if( fabs(h2 - h1) > 180)
                derta_h = - (h2 + 360 - h1);
            else
                derta_h = h1 - h2;
        }
        if( index ==2 && index_C ==0)
        {
            if( fabs(h2 - h1) > 180)
                derta_h = 360 + h1 - h2;
            else
                derta_h = h1 - h2 ;
        }
        if( index ==1 && index_C ==3)
        {
            if( fabs(h2 - h1) > 180)
                derta_h = - (h2 + 360 - h1);
            else
                derta_h = h1 - h2;
        }
        if( index ==3 && index_C ==1)
        {
            if( fabs(h2 - h1) > 180)
                derta_h = h1 + 360 - h2;
            else
                derta_h = -(h2 - h1);
        }
    }
    else
       derta_h = -derta_h;
}

