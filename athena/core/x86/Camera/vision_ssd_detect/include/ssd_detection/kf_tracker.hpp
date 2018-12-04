#include <opencv2/core/core.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/calib3d/calib3d.hpp>
#include <opencv2/core/version.hpp>
#include <opencv2/features2d/features2d.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include "ssd_detection/camera_obj_list.hpp"
#include <iostream>
#include <stdio.h>
#include <sstream>
#include <algorithm>
#include <iterator>
#define SSTR( x ) dynamic_cast< std::ostringstream & >( \
        ( std::ostringstream() << std::dec << x ) ).str()

using namespace cv;

struct ObjectDetection_
{
	cv::Rect rect;
	float score;
	int   classification;
	int   classID;
};

static int 			DEFAULT_LIFESPAN; //LIFESPAN of objects before stop being tracked, in frames
static int	 		INITIAL_LIFESPAN; //LIFESPAN of objects before stop being tracked, in frames
static int			ORB_NUM_FEATURES;
static unsigned int	ORB_MIN_MATCHES;
static float		ORB_KNN_RATIO;
static float 		NOISE_COV;
static float 		MEAS_NOISE_COV;
static float 		ERROR_ESTIMATE_COV;
static float 		OVERLAPPING_PERC;
static bool 		SHOW_PREDICTIONS;
static bool 		USE_ORB;
static bool 		track_ready_;
static bool 		detect_ready_;

struct kstate
{
	cv::KalmanFilter	KF;//KalmanFilter for this object
	cv::Rect  pos;//position of the object centerx, centery, width, height
    cv::Scalar	color;	//ObjectDetection_ obj;//currently not used
    cv::Mat			image;//image containing the detected and tracked object
	int       obj_type;
	float			score;// score
	bool			active;//if too old (lifespan) don't use
	unsigned int		id;//id of this tracked object
	int			lifespan;//remaining lifespan before deprecate
    int		real_data;
	//std::vector<KeyPoint> orbKeypoints;
	//cv::Mat				orbDescriptors;
};

class Kftracker
{

  public:
    /// 初始化跟踪参数
    void init_params();
     /**  运行kf跟踪
     *  @param p1 输入检测后的Mat类型图片
     *  @return 0
     **/
    int kf_run(cv::Mat &image);
    /// 坐标点转化为矩形框
    void getRectFromPoints(std::vector< cv::Point2f > corners, cv::Rect& outBoundingBox);
    /** 判断 roi是否存在包含关系
     *  @param p1 BoundingBox from Detection
     *  @param p2 BoundingBox from Predictor
     *  @return true 如果b包含a 或者相反
     **/
    bool crossCorr(cv::Mat im1, cv::Mat im2);
    /** 预测框转化为Bbox
     *  @param p1 跟新的bbox
     *  @param p2 被预测跟踪后的bbox
     *  @return true 如果b包含a 或者相反
     **/
    void posScaleToBbox(std::vector<kstate> kstates, std::vector<kstate>& trackedDetections);
    /** 获取新跟踪目标的id
     *  @param  跟踪目标列表
     *  @return 新目标的id
     **/
    int getAvailableIndex(std::vector<kstate>& kstates);
    /** 初始化卡尔曼跟踪器
     *   @param p1 输入前帧检测目标列表
     *   @param p2 在跟踪目标列表
     *   @param p3 输入当前帧目标列表
     *   @param p4 输入检测图片
     **/
    void initTracking(ObjectDetection_ object, std::vector<kstate>& kstates,ObjectDetection_ detection,cv::Mat& image);
    /** 检查上帧id是否已删除
     *   @param p1 移除索引id列表
     *   @param p2 匹配id
     *   @return
     **/
    bool isInRemoved(std::vector<unsigned int> removedIndices, unsigned int index);
    /** 检查上帧id是否已删除
     *   @param a 输入前帧检测目标列表
     *   @param b 在跟踪目标列表
     **/
    void removeUnusedObjects(std::vector<kstate>& states);
    /** 检查id是否被占用
     *   @param p1 检查id
     *   @param p2 已有id列表
     **/
    bool alreadyMatched(int check_index, std::vector<int>& matched_indices);
    /**  基于置信度由高到低排序
     *   @param p1 输入检测后置信度
     *   @param p2 相对应分数索引号
     **/
    void Sort(const std::vector<float> in_scores, std::vector<unsigned int>& in_out_indices);
    /** 滤除重叠目标框,保留最大框 非极大值抑制
     *   @param p1 跟踪后预测目标
     *   @param p2 最低非极大值抑制值
     **/
    void ApplyNonMaximumSuppresion(std::vector< kstate >& in_source, float in_nms_threshold);
    /**跟踪及匹配id
     *  @param p1 输入检测后的目标框
     *  @param p2 帧计数
     *  @param p3 跟新目标
     *  @param p4 跟踪积极目标状态
     *  @param p5 输入检测后图片
     *  @param p6 trackedDetections 被跟踪的目标
     **/
    void doTracking(std::vector<ObjectDetection_>& detections, int frameNumber,
    std::vector<kstate>& kstates, std::vector<bool>& active, cv::Mat& image, std::vector<kstate>& trackedDetections);
    ///
    void publish_if_possible();
    /** 显示出跟踪预测后的目标框
     *  @param p1 输入检测图片
     *  @param p2 帧计数
     *  @param p3 检测后目标
     *  @param p4 跟踪预测后的目标
     *  @param p5 跟踪目标的状态
     **/
    void trackAndDrawObjects(cv::Mat& image, int frameNumber, std::vector<ObjectDetection_> detections,
         std::vector<kstate>& kstates, std::vector<bool>& active);
     /// 输入检测图片的回调函数
    void image_callback(cv::Mat &imageTrack);
     /// 检测后跟踪回调函数
    void detections_callback(std::vector<camera_obj> &get_camera_objs);
     /** 匹配相交面积比IOU
     *  @param p1 BoundingBox
     *  @param p2 BoundingBox
     *  @return 相交面积比IOU
     **/
    float bboxOverlap(cv::Rect &box1,cv::Rect &box2);
    /** 两个Bounding box欧式距离
     *  @param p1 BoundingBox
     *  @param p2 BoundingBox
     *  @return 欧式距离
     **/
    float bboxDistance(cv::Rect &box1,cv::Rect &box2);
   // bool orbMatch(cv::Mat& inImageScene, cv::Mat& inImageObj, cv::Rect& outBoundingBox, unsigned int inMinMatches=2, float inKnnRatio=0.7);
};
