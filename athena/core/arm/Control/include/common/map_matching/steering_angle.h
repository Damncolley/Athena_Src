#pragma once

#include <math.h>
#include "../math_util.h"
#include "point.h"
#include "../path.h"
#include "heading.h"
#include "spline.h"

double cau_steering_angle_from_ks(tk::spline s_x, tk::spline s_y,
                                  double cs, double Ux, double& rr,
                                  double every1, double every2,int32_t k_trans);
