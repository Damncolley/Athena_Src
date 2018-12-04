#ifndef	_MAIN_H_
#define	_MAIN_H_

#include "LaneDetector.h"
#include "LaneDraw.h"
#include "utils/imrgb.h"
#include <fstream>
#include "utils/config2.h"

//lcm
#include "lane_lcm/line_info.hpp"
#include "lane_lcm/ins_info.hpp"
#include "sensor_lcm/cam_obj_list.hpp"
#include "utils/globalVal.h"

using namespace std;
using namespace cv;

static cv::Mat Lane_output(INPUT_IMAGE_HEIGHT, INPUT_IMAGE_WIDTH, CV_8UC3);

void preDetect(cv::Mat& srcImage);
void initConfig(const char* config_file);
cv::Mat getOutImage();
void getPreLaneInfo(int *lane_quality, vector<point> &mLeftPoints, vector<point> &mRightPoints);
void getPreLaneImageInfo(vector<point> &mLeftImagePoints, vector<point> &mRightImagePoints);

//////////////////////////////////////////////////////////////////////////
int main_read_image_from_local(double &start_fps_time, double &end_fps_time, Handler &handler);
#if 1
static IplImage *Im_road_=cvCreateImage(cvSize(ROAD_IMAGE_WIDTH, ROAD_IMAGE_HEIGHT),8,3);
static IplImage *Im_output_=cvCreateImage(cvSize(INPUT_IMAGE_WIDTH,INPUT_IMAGE_HEIGHT),8,3);
static IplImage* frame= cvCreateImage(cvSize(INPUT_IMAGE_WIDTH,INPUT_IMAGE_HEIGHT),IPL_DEPTH_8U,IPL_BORDER_WRAP);
static IplImage *pInputImage=cvCreateImage(cvSize(INPUT_IMAGE_WIDTH,INPUT_IMAGE_HEIGHT),8,1);
static float roadwidth,scale,lateral_offset=0.0,roadwidth_cur,road_scale,roadwidth_pre;
static int Open_RoadImage;
static  std::ofstream fout1("../Camera/LaneLog.txt");
static IplImage *Pout=cvCreateImage(cvSize(INPUT_IMAGE_WIDTH,INPUT_IMAGE_HEIGHT),8,3);
static cv::Mat Sign1(INPUT_IMAGE_WIDTH,INPUT_IMAGE_HEIGHT,3);
static cv::Mat Sign2(INPUT_IMAGE_WIDTH,INPUT_IMAGE_HEIGHT,3);
static FILE*	fp = NULL;
#define DATA_SIZE  200
//static FILE* flp=NULL;
#endif

extern void	closeParameterLog(void);
extern void openParameterLog(void);
extern void toggleSaveLB_PARAMETER(void);
extern void openLaneBoundaryLog(void);

extern double getDisplayScale(void);
extern void setPointOnInputImage(int iIsrc, int iJsrc);
extern int getIsrcOnInputImage(void);
extern int getJsrcOnInputImage(void);

extern void upIsrcOnInputImage(void);
extern void downIsrcOnInputImage(void);
extern void leftJsrcOnInputImage(void);
extern void rightJsrcOnInputImage(void);
extern void upVmarginOnInputImage(void);
extern void downVmarginOnInputImage(void);
extern int getVmarginOnInputImage(void);
extern int getInputImageHeight(void);
extern int getInputImageWidth(void);
extern int getOutputImageHeight(void);
extern int getOutputImageWidth(void);
extern IMRGB *getImrgbOutput(void);
extern IMRGB *getImrgbInput(void);
extern double getVelocityOfVehicle(void);
extern double getFrontSteerAngle(void);
extern double getRearSteerAngle(void);
extern int getHorizontalLineNo(void);
extern void setHorizontalLineNo(int v);
extern void setHorizontalLineNoWithScaling(int v);
extern void upHorizontalLineNo(void);
extern void downHorizontalLineNo(void);
extern int getVerticalLineNo(void);
extern void setVerticalLineNo(int v);
extern void setVerticalLineNoWithScaling(int v);
extern void toLeftVerticalLineNo(void);
extern void toRightVerticalLineNo(void);

//////////////////////////////////////////////////////////////////////////
extern IMRGB * getInputImage(IplImage *inputimage);
//////////////////////////////////////////////////////////////////////////
extern IMRGB * getInputImage(void);

extern int getFrameSkip(void);
extern void setFrameSkip(int v);
static void makeOutputImage(void);
static void makeOutputRoadImage(IMRGB *pImrgb);
static BOOL remakeOutputImage(void);
static BOOL remakeAllImage(void);
//static BOOL mainProc(void);

extern DB getVehicleSpeed(void);

//static void clear_imrgb_out(void);
//extern char *getOutputAVIFilePath(void);
static void deleteLaneDetector(void);
extern int create3DViewWindow(void);
extern int proc3DViewWindow(void);
extern void make3DImage(LaneDetector *pLaneDetector);
static void clearImrgbOutput(void);

void calRoadLinePoints(int iTopIroadLeft, int iTopIroadRight, int iTopJroadLeft, int iTopJroadRight, int iBottomIroadLeft, int iBottomIroadRight, int iBottomJroadLeft, int iBottomJroadRight);
void publish_line_info_to_control(vector<obu_camera::line_point> &left_points, vector<obu_camera::line_point> &right_points, vector<obu_camera::line_point> &center_points, double lane_width, int judge_lane_flag);
void publish_line_info_to_control(vector<obu_camera::line_point> &left_points, vector<obu_camera::line_point> &right_points, int judge_lane_flag);
void readVideo(string path);
void on_Trackbar(int, void*);
void ImageText(Mat* img, const char* text, int x, int y);


static LaneParameter g_LaneParameterPrevious;
static LaneParameter *getLaneParameterPrevious(void)
{
    return &g_LaneParameterPrevious;	   //之前的data也非常有用
}
static LaneDetector *g_pLaneDetector = NULL;
static LaneDetector *getLaneDetector(void)
{
    return g_pLaneDetector;
}

static void deleteLaneDetector(void)
{
    SAFE_DELETE(g_pLaneDetector);
}

#define	DUMMY_DATA	(-9999)

#define	MAX_DISTANCCE_NUM	(20 + 1)
static double g_adDistanceOfHorizontalLine[MAX_DISTANCCE_NUM] = { -1	};
static int g_aiPosOfHorizontalLine[MAX_DISTANCCE_NUM];
static int g_iNumOfHorizontalLine = 0;
static int getNumOfHorizontalLine(void)
{
    return g_iNumOfHorizontalLine;
}

static double getDistanceOfHorizontalLine(int iIdx)
{
    if(iIdx < 0)	return -1;
    if(iIdx >= getNumOfHorizontalLine())	return -1;
    return g_adDistanceOfHorizontalLine[iIdx];
}

static int getPosOfHorizontalLine(int iIdx)
{
    if(iIdx < 0)	return -1;
    if(iIdx >= getNumOfHorizontalLine())	return -1;
    return g_aiPosOfHorizontalLine[iIdx];
}

static int g_aiSelectedDistance[MAX_DISTANCCE_NUM] = {-1};
static int g_iSelectedDistanceNum = 0;

#endif	//_MAIN_H_
