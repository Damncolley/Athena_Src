#pragma once

#include <math.h>
#include <vector>
#include "spline/spline.h"

double calculate_R(double steering,
                   double Ux);

void  derta_heading(double h1,
                    double h2,
                    double& derta_h);

double  cau_delta_heading(
    double current_heading,
    double head_c);

// 求取相关点的头指向。
int get_heading_h(double x1, double y1,
                  double x2, double y2, double& h2,
                  double& h_r, int num);

// 求取相关点的头指向。
int get_heading(double x1, double y1,
                double x2, double y2, double& h2);
int get_heading(double dx, double dy, double& h);

int get_steering_angle_h(double h1, double h2,
                         double s, double& st, double& st_r);

int get_steering_angle(double h1, double h2,
                       double s, double& st);

double cau_heading_angle_from_ks(tk::spline s_x,
                                 tk::spline s_y,
                                 double cs,
                                 double every);
double cau_heading_angle_from_ks(tk::spline s_x,
                                 tk::spline s_y,
                                 double cs);

double cau_heading_angle_from_circle_ks(tk::spline s_x,
                                        tk::spline s_y,
                                        double cs,
                                        double every,
                                        double zero_offset);


