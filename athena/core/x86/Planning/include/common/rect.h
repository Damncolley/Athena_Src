/**
 * @file
 *
 * @brief 表示一个矩形栅格的属性和相关计算。
 */

#pragma once

#include <math.h>
#include <vector>
#include "point.h"

/**
 * @class rect
 * @brief 矩形栅格类。
 */
class rect
{
public:
    float width;            ///<矩形栅格的宽度
    float height;           ///<矩形栅格的长度
    float x,y;              ///< 左上角点坐标left-top
    point p1, p2, p3, p4;   ///<矩形栅格的四个顶点

public:
    /**
    * @brief 构造函数
    */
    rect()
    {
        x=y=0;
        width=height=0;
    }
    /**
    * @brief 析构函数
    */
    ~rect()
    {

    }

    /**
    * @brief 绝对坐标系下，判断一个点是否落在该矩形栅格内。
    * @param p 输入量：点的坐标（绝对坐标系坐标）。
    * @return 1表示点落在矩形栅格内，0表示没有。
    */
    bool is_point_in(point& p);

    /**
    * @brief 传感器相对坐标系下，判断一个点是否落在该矩形栅格内。
    * @param p 输入量：点的坐标（绝对坐标系坐标）。
    * @return 1表示点落在矩形栅格内，0表示没有。
    */
    bool is_point_in_xx(point& p);

    /**
    * @brief 绝对坐标系下，判断一个矩形栅格是否与该矩形栅格有交叠。
    * @param r 输入量：输入的矩形栅格（绝对坐标系坐标）。
    * @return 1表示有交叠，0表示没有。
    */
    bool cross_rect(rect r);

    /**
    * @brief 传感器相对坐标系下，判断一个矩形栅格是否与该矩形栅格有交叠。
    * @param r 输入量：输入的矩形栅格（绝对坐标系坐标）。
    * @return 1表示有交叠，0表示没有。
    */
    bool cross_rect_xx(rect r);

    /**
    * @brief 对等于号=进行重载，rect类的等号操作符。
    * @param src 输入量：原始输入矩形栅格。
    * @return 被赋值的矩形栅格。。
    */
    rect& operator= (const rect& src)
    {

        this->p1 = src.p1;
        this->p2 = src.p2;
        this->p3 = src.p3;
        this->p4 = src.p4;

        this->width = src.width;
        this->height = src.height;
        this->x = src.x;
        this->y = src.y;

        return *this;

    }
};

/**
* @brief 绝对坐标系下，判断两个矩形栅格是否有交叠。
* @param r1 输入量：第一个矩形栅格（绝对坐标系坐标）。
* @param r2 输入量：第二个矩形栅格（绝对坐标系坐标）。
* @return 1表示有交叠，0表示没有。
*/
bool cross_in_two_rect(rect& r1, rect& r2);

/**
* @brief 传感器相对坐标系下，判断两个矩形栅格是否有交叠。
* @param r1 输入量：第一个矩形栅格（绝对坐标系坐标）。
* @param r2 输入量：第二个矩形栅格（绝对坐标系坐标）。
* @return 1表示有交叠，0表示没有。
*/
bool cross_in_two_rect_xx(rect& r1, rect& r2);
