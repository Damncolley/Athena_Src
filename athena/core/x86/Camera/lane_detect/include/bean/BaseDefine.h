#ifndef	_BASE_DEFINE_H_
#define	_BASE_DEFINE_H_

#include <vector>
//#include <tchar.h>

#define CAMERACAN_WINDOWNAME                _T("CameraCAN")
#define WM_MESSAGE_SIGNINFO                  WM_USER+10000		// Sign 信息
#define WM_MESSAGE_CAMERAINFO                WM_USER+10001 // Camera 信息
//////////////////////////////////////////////////////////   高精度导航    ////////////////////////////////////////////////////////////////////////////

// Camera 信息
typedef struct _stCameraInfo
{
	long  CameraCaptureTimestamp;          // Camera采集时间戳
	long  CameraResultTimestamp;           // Camera处理结果时间戳
	int    LeftLane;                        // 左边车道数量
	int    RightLane;                       // 右边车道数量
	int    LDW;                             // 车线脱逸
	int    ChangeLaneEvent;                 // 变道事件
	int    LeftLineType;                    // 左边车线类型
	int    RightLineType;                   // 右边车线类型
	int    LeftLineWidth;                   // 左边车线线型宽度
	int    RightLineWidth;                  // 右边车线线型宽度
	short  LeftLineDistance;                // 到左边车线的垂直距离 (单位:mm)
	short  RightLineDistance;               // 到右边车线的垂直距离 (单位:mm)
	short  SteeringAngle;                   // 航向角 (单位:角度)
	short  LateralDeviation;                // 横向偏移
	short  SignType;                        // traffic sign 类型
	short  SignPositionX;                   // traffic sign Position X
	short  SignPositionY;                   // traffic sign Position Y
	short  SignPositionZ;                   // traffic sign Position Z

	_stCameraInfo()
	{
		CameraCaptureTimestamp = CameraResultTimestamp = 0;
		LeftLane = RightLane = LDW = ChangeLaneEvent = LeftLineType = RightLineType = LeftLineWidth = RightLineWidth = 0;
		LeftLineDistance = RightLineDistance = SteeringAngle = LateralDeviation = 0;
		SignType = SignPositionX = SignPositionY = SignPositionZ = 0;
		//Reserve = 0;
	}
};

//Line 信息
struct _LineInfo
{
    int typeLR;
    int bottom_x;
    bool   operator <  (const _LineInfo& rhs   )  const   //升序排序时必须写的函数
    {
        return  bottom_x   <   rhs.bottom_x;
    }
};
//********************************************************************************************************************************************//

struct point
{
    int x,y;
    int road_x,road_y;
    point()
    {
        x=y=road_x=road_y=0;
    }
};

struct _CurrentLine
{
    point left_up,right_down,left_down,right_up;
    short SteeringAngle;
    _CurrentLine()
    {
        SteeringAngle=0;
    }

};

struct  Boundrary_Distance
{
    short  LeftLineDistance;                // 到左边车线的垂直距离 (单位:mm)
    short  RightLineDistance;
    short  Distance;
    short  Left_Dis_Change;
    short  Right_Dis_Change;
    short  Distance_Change;
    Boundrary_Distance()
    {
        LeftLineDistance=RightLineDistance=Distance=Left_Dis_Change=Right_Dis_Change=Distance_Change=0;
    }
};

struct  _Multiline
{
    point left,right;
};

#endif
