/**
 * @file
 *
 * @brief 四次样条拟合库，
 */

#ifndef QUARTIC_SPLINE_H
#define QUARTIC_SPLINE_H

#include <vector>
#include "common/navi_point.h"

using namespace std;

/**
 * @class QuarticSpline
 * @brief 根据两点的状态，通过四次样条拟合出一条平滑的曲线，
 * 并可以内插出曲线内任意一点的坐标，计算该点的方向角和曲率。
 */
class QuarticSpline
{
    public:
        /**
        * @brief 构造函数
        */
        QuarticSpline();
        /**
        * @brief 析构函数
        */
        virtual ~QuarticSpline();

        /**
        * @brief 设置起点和终点坐标,
        * @param x0 输入量：起点x坐标。
        * @param y0 输入量：起点y坐标。
        * @param x1 输入量：终点x坐标。
        * @param y1 输入量：终点y坐标。
        */
        int set_points( double x0, double y0, double x1, double y1 );

        /**
        * @brief 设置四次样条函数的边界条件,
        * @param dy0 输入量：起点的一阶导。
        * @param ddy0 输入量：起点的二阶导。
        * @param dy1 输入量：终点的一阶导。
        * @param flag 输入量：边界条件类型，等于0时表示以起点的一阶导、二阶导和终点的一阶导为边界条件。
        */
        int set_boundary( double dy0, double ddy0, double dy1, int flag );

        /**
        * @brief 求解四次样条函数的系数,
        * @param flag 输入量：边界条件类型，等于0时表示以起点的一阶导、二阶导和终点的一阶导为边界条件。
        */
        int compute_coef( int flag );   //flag : 边界条件类型

        /**
        * @brief 给定曲线范围内任一x值，根据四次样条函数求解对应的y值,
        * @param x 输入量：曲线范围内任一x值。
        * @return x对应的y值。
        */
        double operator() ( double x ) const;

        /**
        * @brief 给定曲线范围内任一x值，根据四次样条函数求解y的一阶导,
        * @param x 输入量：曲线范围内任一x值。
        * @return y的一阶导。
        */
        double compute_first_deriv( double x );

        /**
        * @brief 给定曲线范围内任一x值，根据四次样条函数求解y的二阶导,
        * @param x 输入量：曲线范围内任一x值。
        * @return y的二阶导。
        */
        double compute_second_deriv( double x );

        /**
        * @brief 给定曲线范围内任一x值，根据四次样条函数求解该点上切线的方向角,
        * @param x 输入量：曲线范围内任一x值。
        * @return 该点上切线的方向角。
        */
        double compute_heading( double x );

        /**
        * @brief 给定曲线范围内任一x值，根据四次样条函数求解该点曲率,
        * @param x 输入量：曲线范围内任一x值。
        * @return 该点曲率。
        */
        double compute_k( double x );

        /**
        * @brief 获得四次样条函数的系数,
        * @param coef 输出量：四次样条函数的系数。
        */
        int get_coef( double *coef );

        /**
        * @brief 根据两点的状态，通过四次样条拟合出一条平滑的曲线,
        * @param pos0 输入量：起点状态，需要知道其坐标，方向，曲率。
        * @param pos1 输入量：终点状态。需要知道其坐标，方向。
        * @param step 输入量：生成轨迹点的间隔，单位：米。
        */
        int gen_trj_from_two_points( navi_point pos0, navi_point pos1, double step );

        /**
        * @brief 获取生成的局部坐标系下路径,
        * @param out_path_local 输出量：输出的局部坐标系下路径。
        */
        int get_path_local( vector< navi_point > &out_path_local );

        /**
        * @brief 获取生成的全局坐标系下路径,
        * @param out_path_global 输出量：输出的全局坐标系下路径。
        */
        int get_path_global( vector< navi_point > &out_path_global );

    protected:

    private:
        ///成员变量：四次样条函数的系数。
        double m_a_, m_b_, m_c_, m_d_, m_e_;
        ///成员变量：起点和终点的坐标。
        double m_x_start_, m_y_start_, m_x_end_, m_y_end_;
        ///成员变量：起点和终点的坐标的一阶导、二阶导及三阶导。
        double m_first_deriv_start_, m_first_deriv_end_,
               m_second_deriv_start_, m_second_deriv_end_,
               m_third_deriv_start_, m_third_deriv_end_;
        vector< navi_point > path_local_;    ///<四次样条拟合生成的局部坐标系下路径
        vector< navi_point > path_global_;   ///<四次样条拟合生成的全局坐标系下路径
};

#endif // QUARTIC_SPLINE_H
