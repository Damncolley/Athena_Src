#pragma once
#include <caffe/caffe.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <algorithm>
#include <iomanip>
#include <iosfwd>
#include <memory>
#include <string>
#include <utility>
#include <vector>
using namespace std;
using namespace caffe;

///卷积特征目标检测
class Detector
 {
 public:
  ////////////////////
  /// @param	p1 网络模型文件
  /// @param	p2 权重文件.
  /// @param    p3 均值文件说明
  /// @param    p4 图片均值(104,117,123)
  /// @param    p5 是否用GPU
  /// @param    p4 GPU 序列号
  ///////////////////////////////
  Detector(const string& model_file,
           const string& weights_file,
           const string& mean_file, const string& mean_value,bool in_use_gpu, unsigned int in_gpu_id);
  ///输出目标结果
  std::vector<vector<float> > Detect(const cv::Mat& img);

private:
  ///设置图片像素均值
  void SetMean(const string& mean_file, const string& mean_value);
  /// 将cv Mat对象包装转换为网络卷积层输入的格式
  void WrapInputLayer(std::vector<cv::Mat>* input_channels);
  ///将输入图像转换为caffe网络的输入图像格式
  void Preprocess(const cv::Mat& img,
                  std::vector<cv::Mat>* input_channels);

 private:
  /// 网络结构指针
  shared_ptr<Net<float> > net_;
  /// 输入图片大小
  cv::Size input_geometry_;
  /// 图片通道数
  int num_channels_;
  /// 设置均值图片
  cv::Mat mean_;
};
/// pascal voc 数据caffe-ssd检测
namespace Ssd
{
     /// 分类类别枚举值
	enum SsdDetectorClasses
	{
		BACKGROUND,
		PLANE, BICYCLE, BIRD, BOAT,
		BOTTLE, BUS, CAR, CAT, CHAIR,
		COW, TABLE, DOG, HORSE,
		MOTORBIKE, PERSON, PLANT,
		SHEEP, SOFA, TRAIN, TV, NUM_CLASSES
	};
}
