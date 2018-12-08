#include <math.h>
#include <vector>

//#include "common/origin_vehicle.h"
#include "common/navi_point.h"
#include "common/math_util.h"
#include "vehicle_dynamic/heading.h"
#include "vehicle_dynamic/steering_angle.h"

#include "trajectory/trajectory_sets.h"

double cau_next_heading(double current_heading,
                        double current_steer_angle,
                        double current_speed_lon,
                        double delta_s)
{
   double L = 2.7;
   double R = 0;

// 曲率半径主要是用来描述曲线上某处曲线弯曲变化的程度，
// 特殊的如：圆上各个地方的弯曲程度都是一样的故曲率半径就是该圆的半径；
// 直线不弯曲 ，和直线在该点相切的圆的半径可以任意大，所以曲率是0，
// 故直线没有曲率半径，或记曲率半径为∞。

   R = calculate_R(current_steer_angle,
                   current_speed_lon);

   // 曲率是几何体不平坦程度的一种衡量。平坦对不同的几何体有不同的意义。
   double k = 1/R;

   //以平面曲线为例，做一圆通过平面曲线上的某一点A和邻近的另外两点B1，B2，
   //当B1和B2无限趋近于A时，此圆的极限位置叫做曲线A点处的曲率圆。
   //曲率圆的中心和半径分别称为曲线在A点的曲率中心（centre of curvature)
   //和曲率半径(radius of curvature)。

   //圆弧的曲率半径，就是以这段圆弧为一个圆的一部分时，所成的圆的半径。
   //曲率半径越大，圆弧越平缓，曲率半径越小，圆弧越陡。曲率半径的倒数就是曲率。

   //当角度和弧长同时趋近于0时，就是关于任意形状的光滑曲线的曲率的标准定义。
   //而对于圆，曲率不随位置变化。

   //double delta_h = current_speed_lon*k;
   //曲率 k = (转过的角度/对应的弧长）。
   double delta_h = to_degrees(delta_s * k);
   double value = fclamp(delta_h + current_heading, 0, 360);  //????

   if(value == 360)
      value = 0;
   return value;
}

void calculate_d_dd(double current_heading,
                    double current_steer_angle,
                    double current_speed_lon,
                    double delta_s,
                    double heading_r,
                    double& d_off,
                    double& dd_off)
{
   // assert( (delta_s) > 0.0001);
    // 经过测试表明方向和误差方向相反就对了。 这是起点的d_off
    double derta_h = 0;
    derta_heading(current_heading, heading_r, derta_h);

    d_off = tan(to_radians(derta_h));
  //  assert( fabs(d_off) < 1);
    fclamp(d_off, -1, 1);

    //这是终点的d_off
    double derta_h_next = 0;
    double current_heading_next = cau_next_heading(current_heading,
                                                   current_steer_angle,
                                                   current_speed_lon,
                                                   delta_s);

    derta_heading(current_heading_next, heading_r, derta_h_next);

    double d_off_next = tan(to_radians(derta_h_next));
  //  assert( fabs(d_off_next) < 1);
    fclamp(d_off_next, -1, 1);

    dd_off = (d_off_next - d_off)/ delta_s;

  //  if( dd_off > 3 ) dd_off = 3;
  //  if( dd_off < -3 ) dd_off =-3;
   // assert( fabs(dd_off) < 20);
    fclamp(dd_off, -3, 3);

   /* cout <<" derta_h " << derta_h << "\t"
         <<" derta_h_next " << derta_h_next << "\t"
         <<" d_off " << d_off << "\t"
         <<" dd_off " << dd_off << "\t"
         << endl; */

}
