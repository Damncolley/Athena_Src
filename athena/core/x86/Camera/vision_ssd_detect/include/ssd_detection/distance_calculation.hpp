/// //////////////////////////////////////////////
///@file  计算相机到路面目标的距离
///@brief 需要标定相机 相机内参 安装角度位置
///@author duohaoxue
///@version v1.0
///@date  2018.07.19
/// //////////////////////////////////////////////
 #pragma once
 #include <iostream>
 #include <opencv2/core/core.hpp>
 #include <opencv2/highgui/highgui.hpp>
 #include <opencv2/imgproc/imgproc.hpp>

/// 计算路面目标到相机的距离,包含相机内参和 安装位置
 class Calculate_distance
 {
public:
    /// 相机主光点 横向
    double    CX0;
    /// 相机主光点 纵向
    double    CY0 ;
    /// 相机焦距
    double    FOCUS;
    /// 相机水平方向
    double    POSX ;
    /// 相机垂直方向安装高度
    double    POSY ;
    /// 相机相对路面方向
    double    POSZ ;
    /// 相机安装俯仰角
    float     PITCH;
    /// 相机安装航向角
    double    YAW ;
    /// 水平单位像素距离
    double  m_per_pix_j;
    ///垂直单位像素距离
    double  m_per_pix_i;

private:

    double getRoadTopY;

    double getRoadLeftX ;

    double getRoadImageHeight;

    double getRoadImageWidth;

    double getRoadRealHeight;

    double getRoadRealWidth;

    ///正矩阵
    double calcVimageFromJsrc(int iJsrc);

    double calcUimageFromIsrc(int iIsrc);

    double calcYcameraFromUimage(double dUimage);

    double calcZcameraFromYcameraOnRoad(double dYcamera);

    double calcXcameraFromVimage(double dVimage);

    double calcZvehicleFromZcamera(double dZcamera);

    double calcXvehicleFromXcamera(double dXcamera);

    double calcIroadFromZvehicle(double dZVehicle);

    double calcJroadFromXvehicle(double dXVehicle);


    ///反矩阵
    double calcJsrcFromVimage( double dVimage);

    double calcIsrcFromUimage( double dUimage);

    double calcYcameraFromZcameraOnRoad(double dZcamera);

    double calcUimageFromYcamera(double dYcamera);

    double calcVimageFromXcamera( double dXCamera);

    double calcZcameraFromZvehicle(double dZVehicle);

    double calcXcameraFromXvehicle( double dXvehicle);

    double calcZvehicleFromIroad(double iIroad);	// Iroad->Zvehicle

    double calcXvehicleFromJroad(double a_iJroad);	// Jroad->Xvehicle


public:
     ///
    Calculate_distance();
    /// 图像像素转换到路面位置
    void transformInputToRoad(double iIsrc, double iJsrc, double *pdZVehicle, double *pdXVehicle);
    ///计算路面上目标框到相机的距离
    void calcPosRoadFromPosImage(cv::Rect &rect,double *longitudinal,double *lateral,double *veh_width);
    /// 路面目标位置转换到图像像素
    void transformInputToImage(double pdZVehicle, double pdXVehicle,double *iIsrc, double *iJsrc);



 };
