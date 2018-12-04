/**
 * @file
 *
 * @brief 该库提供了一些数学计算工具，
 * 如角度弧度的转换，方向角的计算，局部坐标系和全局坐标系之间的转换。
 */

 #ifndef MATH_TOOLS_H
 #define MATH_TOOLS_H


#include <math.h>
//#include <vector>
#include "common/navi_point.h"


/**
* @brief 角度转弧度。
* @param d 输入量：角度。
* @return 弧度。
*/
double d2r( double d );

/**
* @brief 弧度转角度。
* @param r 输入量：弧度。
* @return 角度。
*/
double r2d( double r );

/**
* @brief 计算从点(x1,y1)到点(x2,y2)的方向角。
* @param x1 输入量：起点的x坐标。
* @param y1 输入量：起点的y坐标。
* @param x2 输入量：终点的x坐标。
* @param y2 输入量：终点的y坐标。
* @return 从点(x1,y1)到点(x2,y2)的方向角。
*/
double get_heading( double x1, double y1,
                    double x2, double y2 );

/**
 * @class Transfer
 * @brief 局部坐标系和全局坐标系之间的转换。
 */
class Transfer
{
    public:
        /**
        * @brief 构造函数
        */
        Transfer();
        /**
        * @brief 析构函数
        */
        virtual ~Transfer();

        /**
        * @brief 设置局部坐标系的原点坐标和y轴指向,
        * 以（x0,y0）为原点，angle为x轴指向，建立坐标系。
        * @param x0 输入量：局部坐标系原点x坐标（全局坐标系下）。
        * @param y0 输入量：局部坐标系原点y坐标（全局坐标系下）。
        * @param angle 输入量：局部坐标系y轴指向。
        */
        int set_origin( double x0, double y0, double angle );

        /**
        * @brief 将局部坐标系的坐标转换到全局坐标系下。
        * @param x_local 输入量：局部坐标系x坐标
        * @param y_local 输入量：局部坐标系y坐标
        * @param x_global 输出量：全局坐标系x坐标
        * @param y_global 输出量：全局坐标系y坐标
        */
        int transfer_l2g( double x_local, double y_local, double &x_global, double &y_global );

        /**
        * @brief 将局部坐标系的坐标及方向角转换到全局坐标系下。
        * @param x_local 输入量：局部坐标系x坐标
        * @param y_local 输入量：局部坐标系y坐标
        * @param h_local 输入量：局部坐标系方向角
        * @param x_global 输出量：全局坐标系x坐标
        * @param y_global 输出量：全局坐标系y坐标
        * @param h_global 输出量：全局坐标系方向角
        */
        int transfer_l2g( double x_local, double y_local, double h_local,
                          double &x_global, double &y_global, double &h_global );

        /**
        * @brief 将局部坐标系的导航点坐标,航向角和曲率转换到全局坐标系下。
        * @param p_local 输入量：局部坐标系的导航点
        * @param p_global 输出量：全局坐标系的导航点
        */
        int transfer_l2g( navi_point p_local, navi_point &p_global );

        /**
        * @brief 将全局坐标系的导航点坐标,航向角和曲率转换到局部坐标系下。
        * 全局坐标系下两点，以其中一点为原点，两点连线为x轴，建立局部坐标系，
        * 并将两点的全局坐标转换到该局部坐标系下。
        * @param length 输入量：两点之间的欧几里得距离。
        * @param p0_global 输入量：全局坐标系下起点。
        * @param p1_global 输入量：全局坐标系下终点。
        * @param p0_local 输出量：局部坐标系下起点。
        * @param p1_local 输出量：局部坐标系下终点。
        */
        int transfer_g2l( double length,
                          navi_point p0_global, navi_point p1_global,
                          navi_point &p0_local, navi_point &p1_local );

        /**
        * @brief 将全局坐标系的导航点坐标,航向角和曲率转换到局部坐标系下。
        * @param point_global 输入量：全局坐标系下起点。
        * @param point_local 输出量：局部坐标系下终点。
        */
       int transfer_g2l( navi_point point_global, navi_point &point_local );

    private:
        double m_x0_;    ///<成员变量：局部坐标的坐标原点在全局坐标系下的x值。
        double m_y0_;    ///<成员变量：局部坐标的坐标原点在全局坐标系下的y值。
        double m_angle_; ///<成员变量：局部坐标系y轴指向。
};

#endif // MATH_TOOLS_H
