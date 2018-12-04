/***************By yuanjun**************************/
#pragma once
#include "matrix.h"
#include "../include/camera_obj_list.hpp"

using namespace std;

typedef struct _X_state
{
	float x_position;
	float z_positon;
	float x_speed;
	float z_speed;

}ObjectState;

typedef struct _Z_measurement
{
	float x_measurement;
	float z_measurement;
}Measurement;

class KalmanFilter
{
public:

    CMatrix A;//X(k)=A X(k-1)+W(k)  //A系统参数

    CMatrix H; //Z(k)=H X(k)+V(k) //测量系统参数

	CMatrix X1;

	CMatrix X2;

	CMatrix Z;  //Z(k)是k时刻的测量值

	CMatrix K;

	CMatrix Q;   //过程噪声

	CMatrix R;  //测量噪声

	CMatrix P1;  //Estimate error

	CMatrix P2;

	CMatrix CoordinateRoll;

	double angle;
public:
	KalmanFilter();
	KalmanFilter(camera_obj &single_obj,double rollAngle);

	~KalmanFilter(void);

	void InitialKalmanFilter(camera_obj &single_obj);

	void timeUpdate();

	void stateUpdate(camera_obj &single_obj,double rollangle,double *pos);

	ObjectState GetCurrentState();

	ObjectState GetPredictState();

	Measurement GetPredictMeasurement(double rollAngle,double *pos);

	CMatrix CalMeasureDeviation();

	KalmanFilter& operator = (const KalmanFilter& anotherKF);

	void CalCoorRoll(double angle);


};



