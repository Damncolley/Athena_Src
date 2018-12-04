#ifndef	_BASE_DEFINE_H_
#define	_BASE_DEFINE_H_

#include <vector>
//#include <tchar.h>

#define CAMERACAN_WINDOWNAME                _T("CameraCAN")
#define WM_MESSAGE_SIGNINFO                  WM_USER+10000		// Sign ��Ϣ
#define WM_MESSAGE_CAMERAINFO                WM_USER+10001 // Camera ��Ϣ
//////////////////////////////////////////////////////////   �߾��ȵ���    ////////////////////////////////////////////////////////////////////////////

// Camera ��Ϣ
typedef struct _stCameraInfo
{
	long  CameraCaptureTimestamp;          // Camera�ɼ�ʱ���
	long  CameraResultTimestamp;           // Camera������ʱ���
	int    LeftLane;                        // ��߳�������
	int    RightLane;                       // �ұ߳�������
	int    LDW;                             // ��������
	int    ChangeLaneEvent;                 // ����¼�
	int    LeftLineType;                    // ��߳�������
	int    RightLineType;                   // �ұ߳�������
	int    LeftLineWidth;                   // ��߳������Ϳ��
	int    RightLineWidth;                  // �ұ߳������Ϳ��
	short  LeftLineDistance;                // ����߳��ߵĴ�ֱ���� (��λ:mm)
	short  RightLineDistance;               // ���ұ߳��ߵĴ�ֱ���� (��λ:mm)
	short  SteeringAngle;                   // ����� (��λ:�Ƕ�)
	short  LateralDeviation;                // ����ƫ��
	short  SignType;                        // traffic sign ����
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

//Line ��Ϣ
struct _LineInfo
{
    int typeLR;
    int bottom_x;
    bool   operator <  (const _LineInfo& rhs   )  const   //��������ʱ����д�ĺ���
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
    short  LeftLineDistance;                // ����߳��ߵĴ�ֱ���� (��λ:mm)
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
