#include "trajectory/trajectory.h"

void trajectory::init()
{
    tr_num = 16;

    collision_check_value = 1000;
    collision_time  =1000;
    ks_sum_value = 1000;
    consistency_value = 1000;
    collision_distance = 100;
    collision_obj_speed = 0;
    first_collision_obj_num = -1;

    ls_sum_value = 0;
    steering_angle_sum_value = 0;

    select_value = 2000;

    left_offset_value = 0;
    right_offset_value = 0;

    free_length=0;
    free_length_num=0;

    start_pos = 0;
    end_pos = 0;

    start_s = 0;
    end_s = 0;

}


trajectory::trajectory()
{
    init();
}

trajectory::~trajectory()
{

}

