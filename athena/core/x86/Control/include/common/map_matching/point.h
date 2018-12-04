#pragma once

#include <math.h>
#include <vector>

// 点的位置
class point
{
public:
    // 传感器相对坐标
    float x;
    float y;
    float z;

    // 绝对坐标
    float xx;
    float yy;
    float zz;

public:
    bool is_in_rect(point& p1, point& p2,
                    point& p3, point& p4);

    bool is_in_rect_xx(point& p1, point& p2,
                       point& p3, point& p4);

    point()
    {
        x=y=z=0;
        xx=yy=zz=0;
    }

    ~point()
    {

    }

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

bool samep(point p1,  point p2);
double dist(point p1,point p2);
point midpoint(point p1, point p2);
float get_cross(point& p1, point& p2, point& p);
float get_cross_xx(point& p1, point& p2, point& p);
