/// //////////////////////////////////////////////
///@file  加载caffe-ssd网络模型和权重文件
///@brief 读取图片方法 目标框列表 相机自身配置参数
///@author duohaoxue
///@version v1.0
///@date  2018.07.17
/// //////////////////////////////////////////////
#pragma once
#include "../include/vision_detector.hpp"
#include "../include/camera_obj_list.hpp"
#include "../include/distance_calculation.hpp"
using namespace caffe;
using namespace std;

class vision_detect
{
public:
    ///卷积特征目标检测 Caffe based Object Detection ConvNet
	Detector* ssd_detector_;
    ///当前帧图片得到多个目标列表
    camera_objs_list get_camera_objs;
    ///计算相机到目标物体距离
    Calculate_distance cal_distance;
    ///过滤所需置信度最低阀值 The minimum score required to filter the detected objects by the ConvNet
	float min_score_threshold;
	///选用GPU的id 编号 If GPU is enabled, stores the GPU Device to use
	unsigned int gpu_device_id;
	///是否用GPU Sets whether or not use GPU acceleration
	bool use_gpu;
    ///  是否显示出BoundingBox
    bool  draw_flag;
    ///   输入均值文件
    string  mean_file_;
    ///  图片均值像素
    string   pixel_mean_;
    ///网络模型文件
    string  network_Model ;
    /// 训练权重文件
    string  pretrained_Weights ;
    /// 处理图片Mat类
    cv::Mat image;

public:
    /// 加载配置参数
    void read_config_value_from_file();
     ///匹配相交面积比IOU
    float bboxOverlap(cv::Rect &box1,cv::Rect &box2);
     /// Bounding box  欧式距离
    float bboxDistance(cv::Rect &box1,cv::Rect &box2);
    ///  卷积特征转化为图片目标框
    void convert_rect_to_image_obj();
    /// 画出图像目标框
    void Draw_obj_from_image();
         /// 析够函数
    ~vision_detect();

	 vision_detect();
};
