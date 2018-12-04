/***************By yuanjun**************************/
#include "kalmanfilter.h"
#include "math.h"
#define delta_t 0.1


KalmanFilter::KalmanFilter()
{

}

KalmanFilter::KalmanFilter(camera_obj &single_obj,double rollAngle)
{
	X1.SetMatrixRowAndCol(4,1);
	X2.SetMatrixRowAndCol(4,1);
	A.SetMatrixRowAndCol(4,4);  //X(k)=A X(k-1)+W(k)
	H.SetMatrixRowAndCol(2,4); //Z(k)=H X(k)+V(k)
	Z.SetMatrixRowAndCol(2,1);   //Z(k)是k时刻的测量值
	K.SetMatrixRowAndCol(4,2);
	Q.SetMatrixRowAndCol(4,4);  //过程噪声
	R.SetMatrixRowAndCol(2,2);  //测量噪声
	P1.SetMatrixRowAndCol(4,4);
	P2.SetMatrixRowAndCol(4,4);
	CoordinateRoll.SetMatrixRowAndCol(4,4);
     //by  dhx
	X1.m_pTMatrix[0][0]=single_obj.lat_pos;
	X1.m_pTMatrix[1][0]=single_obj.lon_pos;
	X1.m_pTMatrix[2][0]=X1.m_pTMatrix[3][0]=25;


	A.Eye();//A的转置矩阵
	A.m_pTMatrix[0][2]=delta_t;
	A.m_pTMatrix[1][3]=delta_t;

	H.m_pTMatrix[0][0]=1;
	H.m_pTMatrix[1][1]=1;

	Q.Eye();  //Q的转置矩阵

	R.Eye();  //R的转置矩阵

	P1.Eye();  //P1的转置矩阵

	angle=rollAngle;

	CalCoorRoll(angle);
}

KalmanFilter::~KalmanFilter(void)
{

}

void KalmanFilter::InitialKalmanFilter(camera_obj &single_obj)
{
	X1.SetMatrixRowAndCol(4,1);
	X2.SetMatrixRowAndCol(4,1);
	A.SetMatrixRowAndCol(4,4);
	H.SetMatrixRowAndCol(2,4);
	Z.SetMatrixRowAndCol(2,1);
	K.SetMatrixRowAndCol(4,2);
	Q.SetMatrixRowAndCol(4,4);
	R.SetMatrixRowAndCol(2,2);
	P1.SetMatrixRowAndCol(4,4);
	P2.SetMatrixRowAndCol(4,4);

	X1.m_pTMatrix[0][0]=single_obj.lat_pos;
	X1.m_pTMatrix[1][0]=single_obj.lon_pos;
	X1.m_pTMatrix[2][0]=X1.m_pTMatrix[3][0]=1;

	A.Eye();
	A.m_pTMatrix[0][2]=delta_t;
	A.m_pTMatrix[1][3]=delta_t;

	H.m_pTMatrix[0][0]=1;
	H.m_pTMatrix[1][1]=1;

	Q.Eye();

	R.Eye();

	P1.Eye();

}

void KalmanFilter::timeUpdate()
{
	CMatrix T1,T2;
	X2 = A * X1;
	T1 = A.Transpose();
	P2 = A * P1 * T1 + Q;

//	X2=A*X1;
//	P2=A*P1*A.Transpose()+Q;
}

void KalmanFilter::stateUpdate(camera_obj &single_obj,double rollangle,double *pos)
{
    CMatrix T1,T2;
	//Z.m_pTMatrix[0][0]=glu.avg_x;
	//Z.m_pTMatrix[1][0]=glu.avg_z;

	//CMatrix temp(2,2);
	//   T1= H.Transpose();
	//temp=H*P2*T1+R;
	//   T2= temp.Inverse();
	//K=P2*T1*T2;

	//CalCoorRoll(rollangle);

	//X2=CoordinateRoll*X2;

	//   CMatrix T3,T4,T5;

	//   T4=H*X2;
	//   T3=Z-T4;
	//   T5=K*T3;
	//X1=X2+T5;

	//CMatrix I(4,4);
	//I.Eye();

	//   CMatrix T6,T7,T8;
	//   T6=K*H;
	//   T7=I-T6;
	//P1=T7*P2;

	Z.m_pTMatrix[0][0]=single_obj.lat_pos;
	Z.m_pTMatrix[1][0]=single_obj.lon_pos;

//	CMatrix temp(2,2);
//	temp=H*P2*H.Transpose()+R;

    CMatrix temp(2,2);
    T1= H.Transpose();
    temp=H*P2*T1+R;

    T2= temp.Inverse();
    K=P2*T1*T2;

//	K=P2*H.Transpose()*temp.Inverse();

	CalCoorRoll(rollangle);

	X2=CoordinateRoll*X2;

	X2.m_pTMatrix[0][0]+=pos[0];
	X2.m_pTMatrix[1][0]+=pos[2];


//	X1=X2+K*(Z-H*X2);

    CMatrix T3,T4,T5;

    T4=H*X2;
    T3=Z-T4;
    T5=K*T3;
    X1=X2+T5;

	CMatrix I(4,4);
	I.Eye();

//	P1=(I-K*H)*P2;

    CMatrix T6,T7,T8;
    T6=K*H;
    T7=I-T6;
    P1=T7*P2;

}

ObjectState KalmanFilter::GetCurrentState()  //得到当前状态量
{
	ObjectState movestate;
	movestate.x_position=X1.m_pTMatrix[0][0];
	movestate.z_positon=X1.m_pTMatrix[1][0];
	movestate.x_speed=X1.m_pTMatrix[2][0];
	movestate.z_speed=X1.m_pTMatrix[3][0];

	return movestate;
}

ObjectState KalmanFilter::GetPredictState() //得到预测状态
{
	ObjectState movestate;

	movestate.x_position=X2.m_pTMatrix[0][0];
	movestate.z_positon=X2.m_pTMatrix[1][0];
	movestate.x_speed=X2.m_pTMatrix[2][0];
	movestate.z_speed=X2.m_pTMatrix[3][0];

	return movestate;
}

Measurement KalmanFilter::GetPredictMeasurement(double rollAngle,double *pos)  //预测测量
{
	Measurement predictMeasurement;
	CMatrix temp(2,1);
	CalCoorRoll(rollAngle);
	X2=CoordinateRoll*X2;
	X2.m_pTMatrix[0][0]+=pos[0];
	X2.m_pTMatrix[1][0]+=pos[2];
	temp=H*X2;
	predictMeasurement.x_measurement=temp.m_pTMatrix[0][0];
	predictMeasurement.z_measurement=temp.m_pTMatrix[1][0];

	return predictMeasurement;
}

CMatrix KalmanFilter::CalMeasureDeviation()  //测量偏差
{
	CMatrix Deviation(2,2);
	CMatrix standardDeviation(2,2);

	CMatrix T6,T7,T8;
	T6=H.Transpose();
	T7=P2*T6;
	Deviation=H*T7+R;

//	Deviation=H*P2*H.Transpose()+R;

	for (int i=0;i<2;i++)
	{
		for(int j=0;j<2;j++)
		{
			standardDeviation.m_pTMatrix[i][j]=sqrt(Deviation.m_pTMatrix[i][j]);
		}
	}

	return  standardDeviation;
}

KalmanFilter& KalmanFilter::operator = (const KalmanFilter& anotherKF)
{
	A=anotherKF.A;
	H=anotherKF.H;
	X1=anotherKF.X1;
	X2=anotherKF.X2;
	K=anotherKF.K;
	Z=anotherKF.Z;
	P1=anotherKF.P1;
	P2=anotherKF.P2;
	Q=anotherKF.Q;
	R=anotherKF.R;
	CoordinateRoll=anotherKF.CoordinateRoll;
	angle=anotherKF.angle;
	return *this;
}

void KalmanFilter::CalCoorRoll(double angle)
{
	CoordinateRoll.m_pTMatrix[0][0]=cos(angle);//angle is radian??
	CoordinateRoll.m_pTMatrix[0][1]=sin(angle);
	CoordinateRoll.m_pTMatrix[1][0]=-sin(angle);
	CoordinateRoll.m_pTMatrix[1][1]=cos(angle);
	CoordinateRoll.m_pTMatrix[2][2]=cos(angle);
	CoordinateRoll.m_pTMatrix[2][3]=sin(angle);
	CoordinateRoll.m_pTMatrix[3][2]=-sin(angle);
	CoordinateRoll.m_pTMatrix[3][3]=cos(angle);
}


