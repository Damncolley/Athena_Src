#pragma once

#include <stdio.h>
#include <vector>
#include "BaseDefine.h"

#ifndef _LANE_H
#define _LANE_H

// Detect regions
typedef struct _DET_REGION
{
    CvPoint left_top;
    CvPoint right_down;
} DET_REGION;

int Lanedetection(cv::Mat& Input, cv::Mat& Roadimage, cv::Mat& Lane_output, double &end_fps_time);
int Lanedetection(cv::Mat& Input, cv::Mat& Roadimage, cv::Mat& Lane_output);

#endif
