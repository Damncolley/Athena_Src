#pragma once

#include <math.h>
#include "common/math_util.h"
#include "common/point.h"
#include "common/path.h"
#include "vehicle_dynamic/heading.h"
#include "spline/spline.h"

double cau_ks_by_3_points( double x[3], double y[3] );

double cau_steering_angle_from_ks(tk::spline s_x, tk::spline s_y,
                                  double cs, double Ux, double& rr,
                                  double every1, double every2);
//
double cau_steering_angle_from_ks(tk::spline s_x,
                                  tk::spline s_y,
                                  double cs,
                                  double Ux,
                                  double& rr);

double calculate_sign_and_value_offset(
    double x, double y, double yaw,
    double x_c, double y_c, double yaw_c);
