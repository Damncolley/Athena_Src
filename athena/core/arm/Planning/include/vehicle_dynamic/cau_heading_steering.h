#include "common/path.h"
#include "spline/spline.h"
#include "vehicle_dynamic/heading.h"
#include "vehicle_dynamic/steering_angle.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  static
//
/////////////////////////////////////////////////////////////////////////////////////////////////

int preprocess_path( path& p_in );
int preprocess_path2( path& p_in );

void cau_all_output_from_map(
        path& p_in,
        path& p_out,
        int no_points,
        double speed);

void cau_all_output_from_single_spline(
        path& p,
        int no_points,
        double speed);

void cau_points_heading_steering_from_spline(
        vector<navi_point>& ref_points,
        int no_points,
        double speed);

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//   for motion planning
//
/////////////////////////////////////////////////////////////////////////////////////////////////

void cau_heading_steering_from_spline_half(
        path& p,
        int st_p,
        int end_p,
        double speed,
        double steering);

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//   for motion planning
//
/////////////////////////////////////////////////////////////////////////////////////////////////

void spline_head(
        path& p,
        double speed,
        int start,
        int length);

void spline_steering(
        path& p,
        double speed,
        int start,
        int length);

//void spline_kp(
//        path& p,
//        double speed,
//        int start,
//        int length );

