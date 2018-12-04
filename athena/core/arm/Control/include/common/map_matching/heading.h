#pragma once

#include <math.h>
#include <vector>
#include "spline.h"
#include "../../controller_config.h"

double calculate_R(double steering,
                   double Ux,const athena::control::ControllerConfig *controller_config);

// 求取相关点的头指向。
 int get_heading(double x1, double y1,
                          double x2, double y2, double& h2);

 int get_steering_angle_h(double h1, double h2,
                         double s, double& st, double& st_r);

 int get_steering_angle(double h1, double h2,
                         double s, double& st);

 double cau_heading_angle_from_ks(tk::spline s_x,tk::spline s_y,
                                    double cs,
                                    double every);

double cau_heading_angle_from_ks(tk::spline s_x, tk::spline s_y,
                                 double cs);
