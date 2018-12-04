/// //////////////////////////////////////////////
///@file  加载caffe-ssd网络模型和权重文件
///@brief 读取图片方法 目标框列表 相机自身配置参数
///@author duohaoxue
///@version v1.0
///@date  2018.07.17
/// //////////////////////////////////////////////
#pragma once
#include "ssd_detection/vision_detector.hpp"
#include "ssd_detection/camera_obj_list.hpp"
#include "ssd_detection/distance_calculation.hpp"
using namespace caffe;
using namespace std;

class Visiondetect
{
public:
    ///卷积特征目标检测 Caffe based Object Detection ConvNet
	Detector* ssd_detector_;
	///是否用GPU Sets whether or not use GPU acceleration
	bool use_gpu;
	///选用GPU的id 编号 If GPU is enabled, stores the GPU Device to use
	unsigned int gpu_device_id;
    ///   输入均值文件
    string  mean_file_;
    ///  图片均值像素
    string   pixel_mean_;
    ///网络模型文件
    string  network_Model ;
    /// 训练权重文件
    string  pretrained_Weights ;

    string  camera_config_file;
    /// 加载配置参数
    void read_config_value_from_file();
    /** 卷积特征转化为图片目标框
     *   @param p1 输入需检测图片
     *   @param p2 输出检测后的目标物结果
     **/
    void convert_rect_to_image_obj(cv::Mat &image,vector<camera_obj> &out_camera_objs);

    ~Visiondetect();

	 Visiondetect(string config_file);
private:
    ///计算相机到目标物体距离
    Calculate_distance cal_distance;
    ///过滤所需置信度最低阀值 The minimum score required to filter the detected objects by the ConvNet
	float min_score_threshold;
    /// 最小非极大值抑制
	float nms_threshold;
    ///  是否显示出BoundingBox
    bool  draw_flag;
    ///  是否跟踪
    bool  use_track;
     /** 画出图像目标框
     *   @param p1 输入显示图片
     *   @param p2 输入检测后的矩形框
     **/
    void Draw_obj_from_image(cv::Mat &image,std::vector<camera_obj> &get_camera_objs);
    /** 滤除误识别目标框
     *   @param p1 输入所需滤除目标
     **/
    std::vector<camera_obj> filter_get_objs(std::vector<camera_obj> &get_camera_objs);
    /** 滤除重叠目标框,保留最大框 非极大值抑制
     *   @param p1 跟踪后预测目标
     *   @param p2 最低非极大值抑制值
     **/
    void ApplyNonMaximumSuppresion( std::vector<camera_obj> &in_source, float in_nms_threshold);
    /**  基于置信度由高到低排序
     *   @param p1 输入检测后置信度
     *   @param p2 相对应分数索引号
     **/
    void Sort(const std::vector<float> in_scores, std::vector<unsigned int>& in_out_indices);

};
