#include <math.h>
#include <iostream>

#include <vector>

#include "common/navi_point.h"
#include "common/math_util.h"
#include "common/path.h"
//#include "../ecu.h"
#include "common/point.h"

#include "collision_check/collision_check.h"

//#include "planning/planning_param.h"
#include "config.h"

//#define CAR_WIDTH (1.5)
//#define TWOPI (2*PI)

//就需要实现一下，对于一个点是否在矩形内的判断。
//只需要判断该点是否在上下两条边和左右两条边之间就行，
//判断一个点是否在两条线段之间夹着，就转化成，
//判断一个点是否在某条线段的一边上，就可以利用叉乘的方向性，
//来判断夹角是否超过了180度 如下图：

// 只要判断(p1 p2 X p1 p ) * (p3 p4 X p3 p1)  >= 0
// 就说明p在p1p2,p3p4中间夹着，同理计算另两边就可以了
// 判断点在cell里面， 需要分析一下。
bool patch::point_in_patch(point p)
{
    return rt.is_point_in(p);
}

RoadSurface::RoadSurface()
{
  int i,j;

  //road_path.ref_points.clear();
  //left_side.ref_points.clear();
  //right_side.ref_points.clear();

  //lanes.clear();
  //obstacles.clear();

  for(i=0;i<PATCHS_WIDTH;i++)
  {
    for(j=0;j<PATCHS_WIDTH;j++)
    {
        patchs_[i][j].value =0;
        patchs_[i][j].rt.p1.xx = patchs_[i][j].rt.p1.x = patchs_[i][j].rt.p1.yy = patchs_[i][j].rt.p1.y =0;
        patchs_[i][j].rt.p2.xx = patchs_[i][j].rt.p2.x = patchs_[i][j].rt.p2.yy = patchs_[i][j].rt.p2.y =0;
        patchs_[i][j].rt.p3.xx = patchs_[i][j].rt.p3.x = patchs_[i][j].rt.p3.yy = patchs_[i][j].rt.p3.y =0;
        patchs_[i][j].rt.p4.xx = patchs_[i][j].rt.p4.x = patchs_[i][j].rt.p4.yy = patchs_[i][j].rt.p4.y =0;
    }
  }

}

RoadSurface::~RoadSurface()
{

}

//判定是否相交，以及相交矩形是什么都可以用这个方法一体计算完成
//判断矩形是否相交，有很多种方法，比如说判断矩形的任意两条边是否相交。
//但是这种方法存在一个缺陷，就是当一个矩形被另外一个矩形包含的时候，
//没有边是相交的但是依然符合相交的定义

//另一种比较严格的数学方法，我觉得比较容易理解也比较好实现，所以就记下了吧。
//这个方法很简单，就是分别比较两个矩形的重心在x轴方向上和y轴方向上的距离
//与两个矩形的长或者宽的一半的和的大小。
//如果重心的在x轴和y轴上的距离都比他们边长和的一半要小就符合相交的条件
void RoadSurface::set_ref_path(vector<navi_point> ref_)
{
    unsigned int i;

    road_path_.ref_points.clear();
    for (i = 0; i < ref_.size(); i++)
    {
        navi_point np;

        np.position_x = ref_[i].position_x;
        np.position_y = ref_[i].position_y;

//        np.lat  = ref_[i].lat;
//        np.lon = ref_[i].lon;

        np.heading = ref_[i].heading;

        np.k_s = ref_[i].k_s;

        np.steering_angle = ref_[i].steering_angle;
//        np.lateral_offset = ref_[i].lateral_offset;

        road_path_.ref_points.push_back(np);
    }
}

void RoadSurface::init(int start, int end)
{
    int i, j;

    int length = (end - start)/ (PATCHS_LENGTH); //单位是轨迹点
    float width = PATCHS_WIDTH_PER_M;  //单位是m

    // 设置好各个格网的位置。
    int start_w = -PATCHS_WIDTH /2;
    for(i= 0 ; i< PATCHS_WIDTH; i++)  //横向
    {
        for(j=0; j< PATCHS_LENGTH; j++) //纵向
        {
            point p1,p2;

            double heading1, heading2;
            double direct1, direct2;
            double s1, s2;
            double l1, l2;

            // 起点的位置
            s1 = patchs_[i][j].s1 = start + j*length;
            s2 = patchs_[i][j].s2 = start + (j+1)*length;

            l1 = patchs_[i][j].l1 = start_w*width;
            l2 = patchs_[i][j].l2 = (start_w+1)*width;

            p1.xx = road_path_.ref_points[s1].position_x;
            p1.yy = road_path_.ref_points[s1].position_y;

            heading1 = road_path_.ref_points[s1].heading;
            direct1 =  calculate_vertical_direction(heading1);

            p2.xx = road_path_.ref_points[s2].position_x;
            p2.yy = road_path_.ref_points[s2].position_y;

            heading2 = road_path_.ref_points[s2].heading;
            direct2 =  calculate_vertical_direction(heading2);

            patchs_[i][j].rt.p1.xx = p1.xx + l1 * sin(to_radians(direct1));
            patchs_[i][j].rt.p1.yy = p1.yy + l1 * cos(to_radians(direct1));

            patchs_[i][j].rt.p2.xx = p2.xx + l1 * sin(to_radians(direct2));
            patchs_[i][j].rt.p2.yy = p2.yy + l1 * cos(to_radians(direct2));

            patchs_[i][j].rt.p3.xx = p2.xx + l2 * sin(to_radians(direct2));
            patchs_[i][j].rt.p3.yy = p2.yy + l2 * cos(to_radians(direct2));

            patchs_[i][j].rt.p4.xx = p1.xx + l2 * sin(to_radians(direct1));
            patchs_[i][j].rt.p4.yy = p1.yy + l2 * cos(to_radians(direct1));

            patchs_[i][j].value = 0;
        }
        start_w ++;
    }

}

//计算车辆的外形和矩形位置
void CollisionCheck::car_lane_collision_check_rect(rect& rt_car, double x, double y, double heading)
{
    double heading_d, right;// right_d;
    point p_front_center, p_back_center;

    right = heading + 90.0;
    heading_d = to_radians(heading);
//    right_d = to_radians(right);

    //前后左右各加保护距离
    rt_car.width = CAR_WIDTH + 2*SAFE_WIDTH; //安全保护的距离
    rt_car.height = CAR_LENGTH + 2*SAFE_WIDTH; //安全保护的距离

    p_front_center.xx = x + rt_car.height/2 * sin(heading_d);
    p_front_center.yy = y + rt_car.height/2 * cos(heading_d);

    p_back_center.xx = x - rt_car.height/2 * sin(heading_d);
    p_back_center.yy = y - rt_car.height/2 * cos(heading_d);

    //front left
    rt_car.p1.xx = p_front_center.xx - rt_car.width/2 *cos(heading_d);
    rt_car.p1.yy = p_front_center.yy + rt_car.width/2 *sin(heading_d);

    //front right
    rt_car.p2.xx = p_front_center.xx + rt_car.width/2 *cos(heading_d);
    rt_car.p2.yy = p_front_center.yy - rt_car.width/2 *sin(heading_d);

    //back right
    rt_car.p3.xx = p_back_center.xx + rt_car.width/2 *cos(heading_d);
    rt_car.p3.yy = p_back_center.yy - rt_car.width/2 *sin(heading_d);

    //back left
    rt_car.p4.xx = p_back_center.xx - rt_car.width/2 *cos(heading_d);
    rt_car.p4.yy = p_back_center.yy + rt_car.width/2 *sin(heading_d);
}

//计算车辆的外形和矩形位置
void CollisionCheck::car_lane_collision_check_rect(rect& rt_car, double x, double y, double heading, double safe_width )
{
    double heading_d, right;//right_d;
    point p_front_center, p_back_center;

    right = heading + 90;
    heading_d = to_radians(heading);
//    right_d = to_radians(right);

    //前后左右各加保护距离
    rt_car.width = CAR_WIDTH + 2*safe_width; //安全保护的距离
    rt_car.height = CAR_LENGTH + 2*safe_width; //安全保护的距离

    p_front_center.xx = x + rt_car.height/2 * sin(heading_d);
    p_front_center.yy = y + rt_car.height/2 * cos(heading_d);

    p_back_center.xx = x - rt_car.height/2 * sin(heading_d);
    p_back_center.yy = y - rt_car.height/2 * cos(heading_d);

    //front left
    rt_car.p1.xx = p_front_center.xx - rt_car.width/2 *cos(heading_d);
    rt_car.p1.yy = p_front_center.yy + rt_car.width/2 *sin(heading_d);

    //front right
    rt_car.p2.xx = p_front_center.xx + rt_car.width/2 *cos(heading_d);
    rt_car.p2.yy = p_front_center.yy - rt_car.width/2 *sin(heading_d);

    //back right
    rt_car.p3.xx = p_back_center.xx + rt_car.width/2 *cos(heading_d);
    rt_car.p3.yy = p_back_center.yy - rt_car.width/2 *sin(heading_d);

    //back left
    rt_car.p4.xx = p_back_center.xx - rt_car.width/2 *cos(heading_d);
    rt_car.p4.yy = p_back_center.yy + rt_car.width/2 *sin(heading_d);
}

/**
* @brief 构建一个矩形框。
* @param x 输入量：矩形框中心x坐标。
* @param y 输入量：矩形框中心y坐标。
* @param heading 输入量：矩形框方向。
* @param width 输入量：矩形框宽度。
* @param length 输入量：矩形框长度。
* @param rt_ 输出量：矩形框。
*/
void CollisionCheck::construct_rect( double x, double y, double heading, double width, double length, rect& rt )
{
    double heading_d = to_radians(heading);
    point p_front_center, p_back_center;

    //前后左右各加保护距离
    rt.width = width;
    rt.height = length;

    p_front_center.xx = x + rt.height/2 * sin(heading_d);
    p_front_center.yy = y + rt.height/2 * cos(heading_d);

    p_back_center.xx = x - rt.height/2 * sin(heading_d);
    p_back_center.yy = y - rt.height/2 * cos(heading_d);

    //front left
    rt.p1.xx = p_front_center.xx - rt.width/2 *cos(heading_d);
    rt.p1.yy = p_front_center.yy + rt.width/2 *sin(heading_d);

    //front right
    rt.p2.xx = p_front_center.xx + rt.width/2 *cos(heading_d);
    rt.p2.yy = p_front_center.yy - rt.width/2 *sin(heading_d);

    //back right
    rt.p3.xx = p_back_center.xx + rt.width/2 *cos(heading_d);
    rt.p3.yy = p_back_center.yy - rt.width/2 *sin(heading_d);

    //back left
    rt.p4.xx = p_back_center.xx - rt.width/2 *cos(heading_d);
    rt.p4.yy = p_back_center.yy + rt.width/2 *sin(heading_d);

}

//计算车辆的外形和矩形位置
void CollisionCheck::car_trajectory_collision_check_pos_rect(rect& rt_car, double x, double y, double heading)
{
    double heading_d, right;//right_d;
    point p_front_center, p_back_center;

    right = heading + 90;
    heading_d = to_radians(heading);
//    right_d = to_radians(right);

    rt_car.width = CAR_WIDTH + 2*SAFE_WIDTH; //安全保护的距离
    rt_car.height = CAR_LENGTH + 2*SAFE_WIDTH; //安全保护的距离

    p_front_center.xx = x + rt_car.height/2 * sin(heading_d);
    p_front_center.yy = y + rt_car.height/2 * cos(heading_d);

    p_back_center.xx = x - rt_car.height/2 * sin(heading_d);
    p_back_center.yy = y - rt_car.height/2 * cos(heading_d);

    //front left
    rt_car.p1.xx = p_front_center.xx - rt_car.width/2 *cos(heading_d);
    rt_car.p1.yy = p_front_center.yy + rt_car.width/2 *sin(heading_d);

    //front right
    rt_car.p2.xx = p_front_center.xx + rt_car.width/2 *cos(heading_d);
    rt_car.p2.yy = p_front_center.yy - rt_car.width/2 *sin(heading_d);

    //back right
    rt_car.p3.xx = p_back_center.xx + rt_car.width/2 *cos(heading_d);
    rt_car.p3.yy = p_back_center.yy - rt_car.width/2 *sin(heading_d);

    //back left
    rt_car.p4.xx = p_back_center.xx - rt_car.width/2 *cos(heading_d);
    rt_car.p4.yy = p_back_center.yy + rt_car.width/2 *sin(heading_d);
}


