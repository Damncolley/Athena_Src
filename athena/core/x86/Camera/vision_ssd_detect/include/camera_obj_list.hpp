#pragma once
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace std;

///图像目标（单个）属性
class camera_obj
{
public:
    ///目标id，与连续跟踪有关
    int    id;
    ///  目标稳定性 0;未知  1;稳定 2: 不稳定目标
    int   stability;
    ///行人/车辆/   car :7  person: 15  bus: 6
    int     classification;
    /// 目标检测跟踪状态 1；未跟踪  2: acc障碍物
    int     detection_status;
    ///横向位置
    double  lat_pos;
    ///横向速度
    double  lat_rate;
    ///纵向位置
    double  lon_pos;
    ///纵向速度
    double  lon_rate;
    ///宽度，单位：m
    double  width;
    ///检测目标的置信度
    float   score;
    /// 目标矩形框像素
    cv::Rect box_point;

};

///图像目标（数组）列表
class camera_objs_list
{
public:
    /// 目标框数目
    int32_t    num_of_camera_obj;
    /// 预留位
    int32_t    reserve;
    /// 目标障碍物列表
    std::vector< camera_obj > cam_objs;
};
