#include <fstream>
#include <iostream>

#include "common/path.h"
#include "common/math_util.h"
#include "common/map_matching/spline.h"
#include "common/map_matching/heading.h"
#include "common/map_matching//steering_angle.h"

void cau_all_output_from_single_spline(
        path& p,
        int no_points,
        double speed);


void cau_path_from_spline(path& p, int no_points);
//////////////////////////////////////////////////////////////////////////////////////////////////
//
//   for motion planning
//
/////////////////////////////////////////////////////////////////////////////////////////////////


void spline_kp(
        path& p,
        double speed,
        int length );

