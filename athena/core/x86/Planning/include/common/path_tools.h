#pragma once

//#include "ecu.h"
#include "common/navi_point.h"
#include "common/path.h"

void alloc_lane_size(path& lane, int length);

void cau_self_lane_offset(path& lane,  //要考过来的轨迹
                      double offset);  //拷贝到目标的起点

void cau_current_lane(path& lane, //要考过来的轨迹
                      path& ref_lane, //从参考的轨迹中考出
                      int st_pos, int en_pos,  //要拷贝过来的位置
                      double offset, //需要增加的偏移量
                      int start_num);  //拷贝到目标的起点

void cau_current_lane(path& lane, //要考过来的轨迹
                      path& ref_lane, //从参考的轨迹中考出
                      int st_pos, int en_pos,  //要拷贝过来的位置
                      double offset); //需要增加的偏移量

double find_ref_offset(double x, double y, double yaw,
                       path& p, int st_pos);



void copy_path_into_virtual_lane( path& p, //要考过来的轨迹
                                  int start_pos, //要拷贝过来的位置
                                  int en_pos,
                                  int into_pos, //拷贝到目标的起点
                                  path& virtual_lane);


void  append_tail_to_virtual_lane(
                         path& tail_path,
                         path& virtual_lane);



