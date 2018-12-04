#pragma once

#include <math.h>

#include "cs.h"
#include "../path.h"

class map_matching
{
public:
   // 最近的匹配点中的相关信息。
    int last_match_point_no;
    int next_match_point_no;

public:
    int current_match_point_no;

public:
    map_matching();
    ~map_matching();

    void init();

    // 地图匹配算法
    int MapMarch_Min_Distance_motion_planning(double Current_X,
                                              double Current_Y,
                                              path *p, int length,
                                              double& min_error);

    int find_next_moition_planning_points(path p, double length);

    int find_moition_planning_before_points(path p, double length);

    int find_moition_planning_start_points(path p, double length);

};
