#ifndef _GLOABALVAL_INCLUDED
#define _GLOABALVAL_INCLUDED

#include <string>
#include <lcm/lcm.h>
#include <lcm/lcm-cpp.hpp>
#include "../sensor_lcm/cam_obj_list.hpp"
#include "../lane_lcm/ins_info.hpp"

using namespace std;

class Handler
{
public:
    ~Handler() {}

    void handleMessage(const lcm::ReceiveBuffer* rbuf,
                       const std::string& chan,
                       const sensor_lcm::cam_obj_list* msg);

    void handleInsMessage(const lcm::ReceiveBuffer* rbuf,
                       const std::string& chan,
                       const obu_lcm::ins_info* msg);
};

extern string lcm_url;
extern string camera_ini;
extern string local_photo_path;
extern int    run_mode;
extern int    write_file;
extern int    calibration;
extern int    showRoadImage;
extern int    wait_key;
extern float  m_per_pix_i;
extern float  m_per_pix_j;
extern double basler_brightness;
extern float  pt_brightness;
extern float  pt_shutter;
extern float  pt_gain;
extern float  pt_gamma;
extern float  pt_white_balanceA;
extern float  pt_white_balanceB;

extern lcm::LCM *g_lcm;

extern double start_fps_time, end_fps_time;

extern bool draw_detect_flag;
extern sensor_lcm::cam_obj_list cam_points;
extern obu_lcm::ins_info cur_ins_info;

extern Handler handler;
extern void* Thread_lcm_Function(void* param);

extern long getCurrentTime();
#endif
