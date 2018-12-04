/**
 * @file
 *
 * @brief 导航模块的公用数据
 * 包含对点、规划模块数据等描述，地图切片、路口红绿灯、停车泊车点等信息
 */


#ifndef _PARK_H
#define _PARK_H

#include <stdint.h>
#include <vector>
#include <string>
#include "common/navi_point.h"
#include "common/path.h"
#include "common/car_state.h"
#include "spline/math_tools.h"

using namespace std;

/**
* @class StopPark
* @brief 停车点/泊车点信息
*/
class StopPark
{
public:
    /**
    * @brief 构造函数
    */
    StopPark();
    /**
    * @brief 析构函数
    */
    ~StopPark();

    /**
    * @brief 对等于号=进行重载，StopPark类的等号操作符。
    * @param src 输入量：原始输入的停车点/泊车点信息。
    * @return 被赋值的停车点/泊车点信息。
    */
    StopPark &operator=(const StopPark& src);

    /**
    * @brief 初始化函数。
    * @return 1成功，0失败。
    */
    int init();

public:
    double     x_;               ///<停车点/泊车点x坐标

    double     y_;               ///<停车点/泊车点y坐标

    double     heading_;         ///<停车点/泊车点方向

    double     length_;          ///<泊车位长度

    double     width_;           ///<泊车位宽度

    int32_t    type_;            ///<停车点/泊车点类型，一般停车点，垂直泊车点，水平泊车点

//        double     offset_;
};

/**
* @class IntelligentParking
* @brief 生成泊车轨迹信息
*/
class IntelligentParking : public StopPark
{
public:
    /**
    * @brief 构造函数
    */
    IntelligentParking();
    /**
    * @brief 析构函数
    */
    ~IntelligentParking();

    /**
    * @brief 设置泊车点
    * @param stop_park 输入量：泊车点。
    * @return 1设置成功，0不成功。
    */
    int set_park_point( const StopPark& stop_park );

    /**
    * @brief 计算车当前停止的点(车后轴中心点)。
    * @param car_state 输入量：车辆状态。
    * @return 1计算成功，0不成功。
    */
    int compute_cur_stop_pose( const CarState& car_state );

    /**
    * @brief 根据停车位信息,计算倒车结束的点(车后轴中心点)。
    * @return 1计算成功，0不成功。
    */
    int compute_end_park_pose();

    /**
    * @brief 计算泊车轨迹。
    * @param car_state 输入量：车辆状态。
    * @return 1计算成功，0不成功。
    */
    int compute_parking_trajectory( const CarState& car_state );

    /**
    * @brief 计算平行泊车轨迹。
    * @return 1计算成功，0不成功。
    */
    int compute_parallel_parking_trajectory();

    /**
    * @brief 计算平行泊车轨迹。
    * @param ss 输入量：开始右打方向盘的横向距离
    * @param hh 输入量：开始右打方向盘的纵向距离
    * @param trajectory_d 输入量：泊车坐标系下，泊车轨迹中D档部分
    * @param trajectory_r 输入量：泊车坐标系下，泊车轨迹中R档部分
    * @return 1计算成功，0不成功。
    */
    int compute_parallel_parking_trajectory( double ss, double hh, path& trajectory_d, path& trajectory_r );

    /**
    * @brief 计算垂直泊车轨迹。
    * @return 1计算成功，0不成功。
    */
    int compute_vertical_parking_trajectory();

    /**
    * @brief 计算垂直泊车轨迹。
    * @param trajectory_d 输入量：泊车坐标系下，泊车轨迹中D档部分
    * @param trajectory_r 输入量：泊车坐标系下，泊车轨迹中R档部分
    * @return 1计算成功，0不成功。
    */
    int compute_vertical_parking_trajectory( path& trajectory_d, path& trajectory_r );

    /**
    * @brief 轨迹从泊车坐标系转换到全局坐标系。
    * @return 1计算成功，0不成功。
    */
    int transfer_trajectory_p2g();

    /**
    * @brief 轨迹从泊车坐标系转换到全局坐标系。
    * @param p_trajectory 输入量：泊车坐标系下轨迹
    * @param g_trajectory 输入量：全局坐标系下轨迹
    * @return 1计算成功，0不成功。
    */
    int transfer_trajectory_p2g( path p_trajectory, path& g_trajectory );

    /**
    * @brief 计算纵向信息
    * @param p_trajectory 输入量：轨迹
    * @param gear 输入量：档位
    * @return 1计算成功，0不成功。
    */
    int compute_longitudinal_info( path& p_trajectory, int8_t gear );

    /**
    * @brief 轨迹向前延长一定的点个数
    * @param p_trajectory 输入量：轨迹
    * @param extend_num 输入量：延长点的个数，单位：个
    * @param interval 输入量：取点间隔，单位：米
    * @param gear 输入量：档位
    * @return 1计算成功，0不成功。
    */
    int extend_trajectory( path& p_trajectory, int extend_num, double interval, int8_t gear );

    /**
    * @brief 获得泊车轨迹中D档部分
    * @param park_trajectory_d 输出量：全局坐标系下，泊车轨迹中D档部分
    * @return 1计算成功，0不成功。
    */
    int get_trajectory_d( path& park_trajectory_d );

    /**
    * @brief 获得泊车轨迹中R档部分。
    * @param park_trajectory_r 输出量：全局坐标系下，泊车轨迹中R档部分
    * @return 1计算成功，0不成功。
    */
    int get_trajectory_r( path& park_trajectory_r );

    /**
    * @brief 获得泊车轨迹中D档部分点的个数。
    * @return 点的个数。
    */
    int get_size_trajectory_d();

    /**
    * @brief 获得泊车轨迹中R档部分点的个数。
    * @return 点的个数。
    */
    int get_size_trajectory_r();



private:
    Transfer transfer_pg_;               ///<泊车坐标系和全局坐标系之间的转换

    //CarState car_state_;                         ///<车辆状态
    navi_point pose_current_stop_;               ///<车当前停止的点
    navi_point pose_end_park_;                   ///<倒车结束的点

    navi_point p_pose_current_stop_;               ///<泊车坐标系下，车当前停止的点
    navi_point p_pose_end_park_;                   ///<泊车坐标系下，倒车结束的点

//    CollisionCheck collision_check_;            ///<泊车时碰撞检测

    path p_park_trajectory_d_;       ///<泊车坐标系下，泊车轨迹中D档部分
    path p_park_trajectory_r_;       ///<泊车坐标系下，泊车轨迹中R档部分

    path park_trajectory_d_;       ///<泊车轨迹中D档部分
    path park_trajectory_r_;       ///<泊车轨迹中R档部分


};

#endif //__PARK_H

