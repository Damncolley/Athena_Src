/**
 * @file
 *
 * @brief 表示一个点的属性和相关计算。
 */

#pragma once

#include <math.h>
#include <vector>

/**
 * @class point
 * @brief 点属性类。
 */
class point
{
public:
    /// 传感器相对坐标
    float x;
    float y;
    float z;

    /// 绝对坐标
    float xx;
    float yy;
    float zz;

public:
    /**
    * @brief 构造函数
    */
    point()
    {
        x=y=z=0;
        xx=yy=zz=0;
    }
    /**
    * @brief 析构函数
    */
    ~point()
    {

    }

    /**
    * @brief 绝对坐标系下，判断该点是否在p1,p2,p3,p4构成的格子内。
    * @param p1 输入量：第一个点的坐标（绝对坐标系坐标）。
    * @param p2 输入量：第二个点的坐标（绝对坐标系坐标）。
    * @param p3 输入量：第三个点的坐标（绝对坐标系坐标）。
    * @param p4 输入量：第四个点的坐标（绝对坐标系坐标）。
    * @return 1表示点落在网格内，0表示没有。。
    */
    bool is_in_rect(point& p1, point& p2,
                    point& p3, point& p4);

    /**
    * @brief 传感器相对坐标系下，判断该点是否在p1,p2,p3,p4构成的格子内。
    * @param p1 输入量：第一个点的坐标（传感器相对坐标系坐标）。
    * @param p2 输入量：第二个点的坐标（传感器相对坐标系坐标）。
    * @param p3 输入量：第三个点的坐标（传感器相对坐标系坐标）。
    * @param p4 输入量：第四个点的坐标（传感器相对坐标系坐标）。
    * @return 1表示点落在网格内，0表示没有。。
    */
    bool is_in_rect_xx(point& p1, point& p2,
                       point& p3, point& p4);

    /**
    * @brief 对等于号=进行重载，point类的等号操作符。
    * @param src 输入量：原始输入点。
    * @return 被赋值的点信息。。
    */
    point& operator= (const point& src)
    {
        this->x = src.x;
        this->y = src.y;
        this->z = src.z;

        this->xx = src.xx;
        this->yy = src.yy;
        this->zz = src.zz;

        return *this;
    }
};

/**
* @brief 判断两点是否相同。
* @param p1 输入量：第一个点的坐标（绝对坐标系坐标）。
* @param p2 输入量：第二个点的坐标（绝对坐标系坐标）。
* @return 返回1表示相同，0表示不同。
*/
bool samep(point p1,  point p2);

/**
* @brief 计算两点之间的距离。
* @param p1 输入量：第一个点的坐标（绝对坐标系坐标）。
* @param p2 输入量：第二个点的坐标（绝对坐标系坐标）。
* @return 返回两点之间的距离。
*/
double dist(point p1,point p2);

/**
* @brief 已知两点，求连线中点坐标。
* @param p1 输入量：第一个点的坐标（绝对坐标系坐标）。
* @param p2 输入量：第二个点的坐标（绝对坐标系坐标）。
* @return 返回两点连线中点坐标。
*/
point midpoint(point p1, point p2);

/**
* @brief 绝对坐标系下，叉乘计算 |p1 p2| X |p1 p|。
* @param p1 输入量：点的坐标（绝对坐标系坐标）。
* @param p2 输入量：点的坐标（绝对坐标系坐标）。
* @param p 输入量：点的坐标（绝对坐标系坐标）。
* @return 返回叉乘结果。
*/
float get_cross(point& p1, point& p2, point& p);
/**
* @brief 传感器相对坐标系下，叉乘计算 |p1 p2| X |p1 p|。
* @param p1 输入量：点的坐标（传感器相对坐标系坐标）。
* @param p2 输入量：点的坐标（传感器相对坐标系坐标）。
* @param p 输入量：点的坐标（传感器相对坐标系坐标）。
* @return 返回叉乘结果。
*/
float get_cross_xx(point& p1, point& p2, point& p);
