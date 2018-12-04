#pragma once

#include <math.h>

//#include "cs.h"
#include "common/path.h"
#include "common/car_state.h"
#include "planning/route_data.h"

class map_matching
{
private:
    // 最近的匹配点中的相关信息。
    int last_match_point_no;
    int next_match_point_no;
    int current_match_point_no;

    bool b_map_update = false;
    //int last_path_num;

public:
    map_matching();
    ~map_matching();

    void init();

    // 地图匹配算法
    int MapMarch_Min_Distance_motion_planning(double Current_X,
            double Current_Y,
            double Current_heading,  //当前的头指向
            path& p,
            int length,
            double& min_error,
            double length_s);

    ///zp20171026:点匹配边界线
    int MapMarch_Min_Distance_motion_planning(double Current_X,
            double Current_Y,
            line_xys& edge_line,
            int st_pos,
            int en_pos,
            double& min_error,
            double length_s);

    ///zp20171109:点匹配center_insert
    int MapMarch_Min_Distance_motion_planning(double Current_X,
            double Current_Y,
            path& center_insert,
            int st_pos,
            int en_pos,
            double& min_error,
            double length_s);

    //****min_error:点离轨迹点最小距离;  min_length:考虑方向后点代价值
    int MapMarch_Min_Distance_motion_planning(
            CarState car_state,
            path& p,
            bool is_map_update,
            double length_s,    //length_s: 局部搜索长度一般赋值 15m
            int num_before,
            double& min_error,
            int cur_pos ); //min_error: 搜索峰值限定 10m

    int MapMarch_Min_Distance_mapping(double Current_X,
                                      double Current_Y,
                                      path& p, int length,
                                      double& min_error);


    int find_next_moition_planning_points(path p, double length);
    int find_next_moition_planning_points(path p, int start_pos, double length);

    int find_moition_planning_before_points(path p, double length);

    int find_moition_planning_start_points(path p, double length);

    int get_current_match_point_no();

    void set_current_match_point_no(int current_no);

    int get_last_match_point_no();

    void set_last_match_point_no(int last_no);

    int set_b_map_update();
};
