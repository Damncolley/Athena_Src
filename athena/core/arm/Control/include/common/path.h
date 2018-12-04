#pragma once
#include <vector>
#include "navi_point.h"
#include "../controller_config.h"

using namespace std;
using namespace athena::control;

class path
{
public:
    void read_navi_file(string path_gps_log_file,const ControllerConfig * controller_config);
    void read_test_file(string path_gps_log_file,const ControllerConfig * controller_config);

    void reset_path(const vector<navi_point>& ref_);
    void output_navi_point_all(char *filename,const ControllerConfig * controller_config);

    bool IsEmpty() const;

public:
    int8_t driving_mode_;//模式
    vector<navi_point> ref_points_;

    int insert_pos; //current insert pos;
    int current_pos; //current pos of vehicle;

    int st_pos;   // current motion plan pos
    int en_pos;   // end of motion plan pos

    path()
    {
       insert_pos = 0;
       current_pos = 0;
       st_pos = 0;
       en_pos = 0;
    }

    ~path()
    {

    }

};

void cau_all_mileage_of_points(vector<navi_point>&  points,  int start_pos,  int  end_pos);
void cau_all_mileage_of_lane(path& virtual_lane);
void cau_all_mileage_of_virtual_lane(path& virtual_lane);
