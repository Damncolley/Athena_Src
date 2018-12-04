#ifndef _LANE_UTILS_H_
#define _LANE_UTILS_H_

#include <vector>
#include <opencv2/opencv.hpp>  
#include "BaseDefine.h"


/** @brief 设置需要识别车道线的图片
 *
 * @param srcImage 输入图片
 * @return
 *
 */
void setImage(cv::Mat& srcImage);

/** @brief 初始化相机参数
 *
 */
void init(const char* config_file);


/** @brief 获取识别车道线后的图片
 *
 * @return 带车道线识别的图片
 *
 */
cv::Mat getResultImage();


/** @brief 获取车道线质量：0：没有识别到车道线\n
 *                      1：只识别出右车道线\n
 *                      2：只识别出左车道线\n
 *                      3：只识别出双车道线\n
 *         返回左右车道线点集（相对相机坐标，单位：mm）
 */
void getLaneInfos(int *lane_quality, std::vector<point> *leftPoints, std::vector<point> *rightPoints);

void getLaneImageInfos(std::vector<point> *leftImagePoints, std::vector<point> *rightImagePoints);

#endif
