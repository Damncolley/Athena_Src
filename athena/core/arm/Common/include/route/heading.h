#pragma once

#include <math.h>
#include <vector>

//转化头指向
double  cau_derta_heading(
        double current_heading,
        double head_c);
//求取点到直线的距离;带方向
double calculate_sign_and_value_offset(
                          double x, double y,
                          double x_a, double y_a, double x_b, double y_b);
// 求取相关点的头指向。
int get_heading_h(double x1, double y1,
                  double x2, double y2, double& h2,
                  double& h_r, int num);

// 求取相关点的头指向。
int get_heading(double x1, double y1,
                double x2, double y2, double& h2);

void derta_heading(double h1,
                   double h2,
                   double& derta_h);

int get_steering_angle_h(double h1, double h2,
                         double s, double& st, double& st_r);

int get_steering_angle(double h1, double h2,
                        double s, double& st);

