#pragma once

#include "navi_point.h"
#include "spline.h"

double getClosestPointOnSpline(tk::spline& sp_x, tk::spline& sp_y,
                               navi_point testPoint,
                               double s1, double s2, double s3,
                               double lower_limit, double upper_limit,
                               double resolution = 1000,
                               int maxIterations = 20);

