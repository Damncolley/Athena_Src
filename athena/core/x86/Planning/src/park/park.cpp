#include <math.h>
#include <iostream>
#include <fstream>
#include <algorithm>

#include "park/park.h"
//#include "planning/planning_param.h"
#include "config.h"

#include "spline/quintic_spline.h"
#include "spline/math_tools.h"
#include "common/enum_list.h"
#include "collision_check/collision_check.h"


using namespace std;

/*********************************************************
* @class StopPark
* @brief 停车点/泊车点信息

class StopPark
*********************************************************/

/**
* @class StopPark
* @brief 停车点/泊车点信息
*/
StopPark::StopPark()
{
    init();
#if 0
    x_ = 20.0;
    y_ = 10.0;
    heading_ = 60.0;
    length_ = 8.5;
    width_ = 2.3;
    type_ = 12;
#endif // 1
}

StopPark::~StopPark()
{

}

StopPark &StopPark::operator=(const StopPark &src)
{
    this->x_ = src.x_;
    this->y_ = src.y_;
    this->heading_ = src.heading_;
    this->length_ = src.length_;
    this->width_ = src.width_;
    this->type_ = src.type_;
    return *this;
}

int StopPark::init()
{
    x_ = -1;
    y_ = -1;
    heading_ = -1;
    length_ = -1;
    width_ = -1;
    type_ = -1;

    return 1;
}


/***********************************************************
* @class IntelligentParking
* @brief 生成泊车轨迹信息

class IntelligentParking : StopPark
***********************************************************/

 /**
* @brief 构造函数
*/
IntelligentParking::IntelligentParking()
{
    init();
}

/**
* @brief 析构函数
*/
IntelligentParking::~IntelligentParking()
{

}

/**
* @brief 设置泊车点
* @param stop_park 输入量：泊车点。
* @return 1设置成功，0不成功。
*/
int IntelligentParking::set_park_point( const StopPark& stop_park )
{
    x_ = stop_park.x_;
    y_ = stop_park.y_;
    heading_ = stop_park.heading_;
    length_ = stop_park.length_;
    width_ = stop_park.width_;
    type_ = stop_park.type_;

    return 1;
}


/**
* @brief 计算车当前停止的点(车后轴中心点)。
* @param car_state 输入量：车辆状态。
* @return 1计算成功，0不成功。
*/
int IntelligentParking::compute_cur_stop_pose( const CarState& car_state )
{
    double back_distance = 0.5*WHEEL_BASE;

    pose_current_stop_.k_s = 0.0;
    pose_current_stop_.heading = car_state.car_pose_.Current_heading_;
    pose_current_stop_.position_x = car_state.car_pose_.CurrentX_ - back_distance*sin( to_radians( pose_current_stop_.heading ) );
    pose_current_stop_.position_y = car_state.car_pose_.CurrentY_ - back_distance*cos( to_radians( pose_current_stop_.heading ) );

    return 1;
}

/**
* @brief 根据停车位信息，计算倒车结束的点(车后轴中心点)。
* @return 1计算成功，0不成功。
*/
int IntelligentParking::compute_end_park_pose()
{
    double back_distance = 0.5*length_ - 0.5*CAR_LENGTH - SAFE_DISTANCE_PARK + 0.5*WHEEL_BASE;

    pose_end_park_.k_s = 0.0;
    pose_end_park_.heading = heading_;
    pose_end_park_.position_x = x_ - back_distance*sin( to_radians( heading_ ) );
    pose_end_park_.position_y = y_ - back_distance*cos( to_radians( heading_ ) );

    return 1;
}

/**
* @brief 计算泊车轨迹。
* @param car_state 输入量：车辆状态。
* @return 1计算成功，0不成功。
*/
int IntelligentParking::compute_parking_trajectory( const CarState& car_state )
{
    ///获得泊车的初始状态和泊车的最终状态
    compute_cur_stop_pose( car_state );
    compute_end_park_pose();

//    ///建立泊车坐标系
//    transfer_pg_.set_origin( pose_end_park_.position_x, pose_end_park_.position_y, pose_end_park_.heading-90.0 );
//    //始末点坐标转换
//    transfer_pg_.transfer_g2l( pose_end_park_, p_pose_end_park_ );
//    transfer_pg_.transfer_g2l( pose_current_stop_, p_pose_current_stop_ );

    ///生成泊车轨迹
    if ( type_ == STOP_TYPE_PARALLEL_PARK )
    {
        ///建立泊车坐标系
        transfer_pg_.set_origin( pose_end_park_.position_x, pose_end_park_.position_y, pose_end_park_.heading-90.0 );
        //始末点坐标转换
        transfer_pg_.transfer_g2l( pose_end_park_, p_pose_end_park_ );
        transfer_pg_.transfer_g2l( pose_current_stop_, p_pose_current_stop_ );

        if ( compute_parallel_parking_trajectory() )
        {
            //轨迹从泊车坐标系转换到全局坐标系
            transfer_trajectory_p2g();
            compute_longitudinal_info( park_trajectory_d_, AT_STATUS_D );
            compute_longitudinal_info( park_trajectory_r_, AT_STATUS_R );
        }
    }

    if ( type_ == STOP_TYPE_VERTICAL_PARK )
    {
        ///建立泊车坐标系
        transfer_pg_.set_origin( pose_end_park_.position_x, pose_end_park_.position_y, pose_end_park_.heading );
        //始末点坐标转换
        transfer_pg_.transfer_g2l( pose_end_park_, p_pose_end_park_ );
        transfer_pg_.transfer_g2l( pose_current_stop_, p_pose_current_stop_ );

        if ( compute_vertical_parking_trajectory() )
        {
            //轨迹从泊车坐标系转换到全局坐标系
            transfer_trajectory_p2g();
            compute_longitudinal_info( park_trajectory_d_, AT_STATUS_D );
            compute_longitudinal_info( park_trajectory_r_, AT_STATUS_R );
        }
    }


    return 1;
}

 /**
* @brief 计算平行泊车轨迹。
* @return 1计算成功，0不成功。
*/
int IntelligentParking::compute_parallel_parking_trajectory()
{
    ///设置可行驶区域
    vector<ObstacleRect> road_obstacles;
    ObstacleRect obs_left, obs_right, obs_forward;
    double back_distance = 0.5*length_ - 0.5*CAR_LENGTH - SAFE_DISTANCE_PARK + 0.5*WHEEL_BASE;  //车库中心离泊车坐标系原点的距离
    int is_collided = 0;
    double s_s0 = 0.0;

    CollisionCheck cc;
    cc.construct_rect( 10.0, 0.5*width_+PARK_LANE_WIDTH, 0.0, 20.0, 2.0*SAFE_DISTANCE_PARK, obs_left.rt_ );
    cc.construct_rect( 0.0, -0.5*width_, 0.0, 20.0, 2.0*SAFE_WIDTH_PARK, obs_right.rt_ );
    cc.construct_rect( back_distance+length_, 0.0, 0.0, length_, width_, obs_forward.rt_ );
    road_obstacles.push_back( obs_left );
    road_obstacles.push_back( obs_right );
    road_obstacles.push_back( obs_forward );

    ///生成轨迹，S_S和H_H在一定区域内有多种选择，选择没有碰撞的一条轨迹
    path trajectory_d, trajectory_r;
    double S_S = 0.0, H_H = 0.0;
    for ( H_H = H_MIN; H_H < H_MAX; H_H += H_STEP )
    {
        if ( 4.0*CAR_MIN_R >= H_H )
        {
            s_s0 = sqrt( 4.0*H_H*CAR_MIN_R - H_H*H_H );
        }
        else
        {
            cout << " \\\\\\\\\\\\\\\\\\\\\\\\\\\\Error:4.0*CAR_MIN_R < H_H " << endl;
        }

        for ( S_S = s_s0+S_STEP; S_S < s_s0+DELTA_S_MAX; S_S += S_STEP )
        {
            compute_parallel_parking_trajectory( S_S, H_H, trajectory_d, trajectory_r );
            #if 0
            ///*******************************************************************************
            ofstream outfile1("log/trajectory_d.log", std::ios::app);
            outfile1.precision(8);
            for(int i=0; i < trajectory_d.ref_points.size(); i++)
            {
                outfile1 << " i " << i
                        << " x " << trajectory_d.ref_points[i].position_x
                        << " y " << trajectory_d.ref_points[i].position_y
                        << " h " << trajectory_d.ref_points[i].heading
                        << " k " << trajectory_d.ref_points[i].k_s
                        << endl;
            }
            outfile1 << endl;
            outfile1.close();

            ///*******************************************************************************
            ofstream outfile("log/trajectory_r.log", std::ios::app);
            outfile.precision(8);
            for(int i=0; i < trajectory_r.ref_points.size(); i++)
            {
                outfile << " i " << i
                        << " x " << trajectory_r.ref_points[i].position_x
                        << " y " << trajectory_r.ref_points[i].position_y
                        << " h " << trajectory_r.ref_points[i].heading
                        << " k " << trajectory_r.ref_points[i].k_s
                        << endl;
            }
            outfile << endl;
            outfile.close();
            #endif // 1

            ///检测轨迹是否与边界有碰撞
            is_collided = cc.park_col_check( trajectory_d, road_obstacles, 0, WHEEL_BASE, 10, 0.0 );
            if ( is_collided )
            {
                continue;
            }

            is_collided = cc.park_col_check( trajectory_r, road_obstacles, 0, WHEEL_BASE, 10, 0.0 );
            if ( is_collided )
            {
                continue;
            }

            //没有碰撞，选择该条轨迹，退出
            if ( !is_collided )
                break;

        }

        //没有碰撞，选择该条轨迹，退出
        if ( !is_collided )
            break;

    }

    ///所有的轨迹都有碰撞
    if ( is_collided )
    {
        cout << "!!! All of the trajectories are collided. " << endl;
        return 0;
    }
    else
    {
        p_park_trajectory_d_.ref_points = trajectory_d.ref_points;
//        compute_longitudinal_info( p_park_trajectory_d_, AT_STATUS_D );

        p_park_trajectory_r_.ref_points = trajectory_r.ref_points;
//        compute_longitudinal_info( p_park_trajectory_r_, AT_STATUS_R );

        return 1;
    }
}

/**
* @brief 计算平行泊车轨迹。
* @param ss 输入量：开始右打方向盘的横向距离
* @param hh 输入量：开始右打方向盘的纵向距离
* @return 1计算成功，0不成功。
*/
int IntelligentParking::compute_parallel_parking_trajectory( double ss, double hh, path& trajectory_d, path& trajectory_r )
{
    QuinticSpline quintic_spline;
    ///泊车坐标系下的关键点
    navi_point park_pose1, park_pose2, park_pose3, park_pose4, park_pose5, park_pose6;
    ///泊车坐标系下的轨迹点
    vector<navi_point> out_points12, out_points32, out_points43, out_points54, out_points65;
    double a_a = ( ss*( hh-2*CAR_MIN_R ) + sqrt( 4.0*CAR_MIN_R*CAR_MIN_R*(ss*ss+hh*hh) - 16.0*CAR_MIN_R*CAR_MIN_R*CAR_MIN_R*hh ) )
                        /( ss*ss - 4*CAR_MIN_R*CAR_MIN_R );

    ///点1
    park_pose1 = p_pose_current_stop_;
    ///点2
    park_pose2.position_x = ss + EXTENDED_LINE_OF_PARALLEL;
    park_pose2.position_y = hh;
    park_pose2.heading = 90.0;
    park_pose2.k_s = 0.0;
    ///点3
    park_pose3.position_x = ss;
    park_pose3.position_y = hh;
    park_pose3.heading = 90.0;
    park_pose3.k_s = -1.0 / CAR_MIN_R;
    ///点4
    park_pose4.position_x = ss - a_a*CAR_MIN_R / sqrt( 1 + a_a*a_a );
    park_pose4.position_y = hh - CAR_MIN_R*( 1 - 1.0/sqrt( 1 + a_a*a_a ));
    park_pose4.heading = 90.0 - atan( a_a )*180/PI;
    park_pose4.k_s = -1.0 / CAR_MIN_R;
    ///点5
    park_pose5.position_x = a_a*CAR_MIN_R / sqrt( 1 + a_a*a_a );
    park_pose5.position_y = CAR_MIN_R*( 1 - 1.0/sqrt( 1 + a_a*a_a ));
    park_pose5.heading = 90.0 - atan( a_a )*180/PI;
    park_pose5.k_s = 1.0 / CAR_MIN_R;
    ///点6
    park_pose6 = p_pose_end_park_;
    park_pose6.k_s = 1.0 / CAR_MIN_R;       ///<左+右-

    ///对车辆当前停车位置---------起始泊车点  五次多项式拟合
    quintic_spline.gen_trj_from_two_points( park_pose1, park_pose2, 0.1 );
    quintic_spline.get_path_global( out_points12 );

    ///对起始泊车点---------泊车点  五次多项式拟合
    quintic_spline.gen_trj_from_two_points( park_pose3, park_pose2, 0.1 );
    quintic_spline.get_path_global( out_points32 );

    ///泊车点---------第一个转折点  最小转弯半径构成的圆
    navi_point temp_pose;
    double temp_x;
    for( temp_x = park_pose4.position_x; temp_x <= park_pose3.position_x; temp_x += 0.1 )
    {
        if ( CAR_MIN_R*CAR_MIN_R > pow( ( temp_x - ss ) , 2 ) )
        {
            temp_pose.position_x = temp_x;
            //最小转弯半径构成的原函数
            temp_pose.position_y = ( hh - CAR_MIN_R ) + sqrt( CAR_MIN_R*CAR_MIN_R - pow( ( temp_x - ss ), 2) );
            //对原函数求解一介导，在计算对应的头指向
            temp_pose.heading = 90.0 - atan( -( temp_x - ss )/sqrt( CAR_MIN_R*CAR_MIN_R - pow( ( temp_x - ss ), 2 ) ) )*180.0/PI;
            temp_pose.k_s = -1.0 / CAR_MIN_R;
            out_points43.push_back( temp_pose );
        }
        else
        {
            cout<<" \\\\\\\\\\\\\\\\\\\\\\\\\\\\Error : sqrt( CAR_MIN_R*CAR_MIN_R - pow( ( temp_x - ss ), 2) )"<< endl;
            return 0;
        }
    }

    ///第一个转折点---------第二个转折点  五次多项式拟合
    quintic_spline.gen_trj_from_two_points( park_pose5, park_pose4, 0.1 );
    quintic_spline.get_path_global( out_points54 );

    ///第二个转折点---------泊车终点  最小转弯半径构成的圆
    for( temp_x = park_pose6.position_x; temp_x <= park_pose5.position_x; temp_x += 0.1 )
    {
        if ( CAR_MIN_R*CAR_MIN_R > temp_x*temp_x )
        {
            temp_pose.position_x = temp_x;
            //最小转弯半径构成的原函数
            temp_pose.position_y = CAR_MIN_R - sqrt( CAR_MIN_R*CAR_MIN_R - temp_x*temp_x );
            //对原函数求解一介导，在计算对应的头指向
            temp_pose.heading = 90.0 - atan( temp_x/sqrt( CAR_MIN_R*CAR_MIN_R - temp_x*temp_x ) )*180.0/PI;
            temp_pose.k_s = 1.0 / CAR_MIN_R;
            out_points65.push_back( temp_pose );
        }
        else
        {
            cout<<" \\\\\\\\\\\\\\\\\\\\\\\\\\\\Error : sqrt( CAR_MIN_R*CAR_MIN_R - temp_x*temp_x ) "<< endl;
            return 0;
        }
    }

    ///泊车坐标系下，泊车轨迹中D档部分
    trajectory_d.ref_points.clear();
    trajectory_d.ref_points = out_points12;
    ///泊车坐标系下，泊车轨迹中R档部分
    trajectory_r.ref_points.clear();
    trajectory_r.ref_points.insert( trajectory_r.ref_points.end(), out_points65.begin(), out_points65.end() );
    trajectory_r.ref_points.insert( trajectory_r.ref_points.end(), out_points54.begin(), out_points54.end() );
    trajectory_r.ref_points.insert( trajectory_r.ref_points.end(), out_points43.begin(), out_points43.end() );
    trajectory_r.ref_points.insert( trajectory_r.ref_points.end(), out_points32.begin(), out_points32.end() );
    reverse( trajectory_r.ref_points.begin(), trajectory_r.ref_points.end() );

    return 1;
}

/**
* @brief 计算垂直泊车轨迹。
* @return 1计算成功，0不成功。
*/
int IntelligentParking::compute_vertical_parking_trajectory()
{
    ///设置可行驶区域
    vector<ObstacleRect> road_obstacles;
    ObstacleRect obs_left, obs_right, obs_forward;
    double back_distance = 0.5*length_ - 0.5*CAR_LENGTH - SAFE_DISTANCE_PARK + 0.5*WHEEL_BASE;  //车库中心离泊车坐标系原点的距离
    int is_collided = 0;
    double s_s0 = 0.0;

    CollisionCheck cc;
    cc.construct_rect( -width_, back_distance, 0.0, width_, length_, obs_left.rt_ );
    cc.construct_rect( width_, back_distance, 0.0, width_, length_, obs_right.rt_ );
    cc.construct_rect( 15.0, back_distance+0.5*length_+PARK_LANE_WIDTH, 0.0, 20.0, 2.0*SAFE_DISTANCE_PARK, obs_forward.rt_ );
    road_obstacles.push_back( obs_left );
    road_obstacles.push_back( obs_right );
    road_obstacles.push_back( obs_forward );

    ///生成轨迹，S_S和H_H在一定区域内有多种选择，选择没有碰撞的一条轨迹
    path trajectory_d, trajectory_r;

    compute_vertical_parking_trajectory( trajectory_d, trajectory_r );
#if 0
    ///*******************************************************************************
    ofstream outfile1("log/trajectory_d.log", std::ios::app);
    outfile1.precision(8);
    for(int i=0; i < trajectory_d.ref_points.size(); i++)
    {
        outfile1 << " i " << i
                << " x " << trajectory_d.ref_points[i].position_x
                << " y " << trajectory_d.ref_points[i].position_y
                << " h " << trajectory_d.ref_points[i].heading
                << " k " << trajectory_d.ref_points[i].k_s
                << endl;
    }
    outfile1 << endl;
    outfile1.close();

    ///*******************************************************************************
    ofstream outfile("log/trajectory_r.log", std::ios::app);
    outfile.precision(8);
    for(int i=0; i < trajectory_r.ref_points.size(); i++)
    {
        outfile << " i " << i
                << " x " << trajectory_r.ref_points[i].position_x
                << " y " << trajectory_r.ref_points[i].position_y
                << " h " << trajectory_r.ref_points[i].heading
                << " k " << trajectory_r.ref_points[i].k_s
                << endl;
    }
    outfile << endl;
    outfile.close();
#endif // 1

    is_collided = cc.park_col_check( trajectory_d, road_obstacles, 0, WHEEL_BASE, 10, 0.0 );
    if ( is_collided )
    {
        cout << "!!! All of the trajectories are collided. " << endl;
        return 0;
    }

    is_collided = cc.park_col_check( trajectory_r, road_obstacles, 0, WHEEL_BASE, 10, 0.0 );
    if ( is_collided )
    {
        cout << "!!! All of the trajectories are collided. " << endl;
        return 0;
    }
    else
    {
        p_park_trajectory_d_.ref_points = trajectory_d.ref_points;
//        compute_longitudinal_info( p_park_trajectory_d_, AT_STATUS_D );

        p_park_trajectory_r_.ref_points = trajectory_r.ref_points;
//        compute_longitudinal_info( p_park_trajectory_r_, AT_STATUS_R );

        return 1;
    }


    return 1;
}

/**
* @brief 计算垂直泊车轨迹。
* @param trajectory_d 输入量：泊车坐标系下，泊车轨迹中D档部分
* @param trajectory_r 输入量：泊车坐标系下，泊车轨迹中R档部分
* @return 1计算成功，0不成功。
*/
int IntelligentParking::compute_vertical_parking_trajectory( path& trajectory_d, path& trajectory_r )
{
    QuinticSpline quintic_spline;
    ///泊车坐标系下的关键点
    navi_point park_pose1, park_pose2, park_pose3, park_pose4, park_pose5;
    ///泊车坐标系下的轨迹点
    vector<navi_point> out_points12, out_points32, out_points43, out_points54;

    double back_distance = 0.5*length_ - 0.5*CAR_LENGTH - SAFE_DISTANCE_PARK + 0.5*WHEEL_BASE;  //车库中心离泊车坐标系原点的距离

    double d_d = 0.0;       ///进入车库多少米后开始回正方向盘
    if ( pow(( CAR_MIN_R - 0.5*CAR_WIDTH - SAFE_WIDTH_PARK ),2) >= pow(( CAR_MIN_R - 0.5*width_ ),2) )
    {
        d_d =  sqrt( pow(( CAR_MIN_R - 0.5*CAR_WIDTH - SAFE_WIDTH_PARK ),2) - pow(( CAR_MIN_R - 0.5*width_ ),2) );
    }
    else
    {
        cout << " \\\\\\\\\\\\\\\\\\\\\\\\\\\\Error:compute D_D_ error " << endl;
    }

    ///点1
    park_pose1 = p_pose_current_stop_;
    ///点2
    park_pose2.position_x = CAR_MIN_R + EXTENDED_LINE_OF_VERTICAL;
    park_pose2.position_y = back_distance + 0.5*length_ - d_d + CAR_MIN_R;
    park_pose2.heading = 90.0;
    park_pose2.k_s = 0.0;
    ///点3
    park_pose3.position_x = CAR_MIN_R;
    park_pose3.position_y = back_distance + 0.5*length_ - d_d + CAR_MIN_R;
    park_pose3.heading = 90.0;
    park_pose3.k_s = -1.0 / CAR_MIN_R;
    ///点4
    park_pose4.position_x = 0.0;
    park_pose4.position_y = back_distance + 0.5*length_ - d_d;
    park_pose4.heading = 0.0;
    park_pose4.k_s = -1.0 / CAR_MIN_R;
    ///点5
    park_pose5 = p_pose_end_park_;

    ///对车辆当前停车位置---------泊车点  五次多项式拟合
    quintic_spline.gen_trj_from_two_points( park_pose1, park_pose2, 0.1 );
    quintic_spline.get_path_global( out_points12 );

    ///对泊车点---------第一个转折点  五次多项式拟合
    quintic_spline.gen_trj_from_two_points( park_pose3, park_pose2, 0.1 );
    quintic_spline.get_path_global( out_points32 );

    ///第一个转折点--------- 第二个转折点 最小转弯半径构成的圆
    navi_point temp_pose;
    double temp_x;
    for( temp_x = park_pose4.position_x; temp_x <= park_pose3.position_x; temp_x += 0.1 )
    {
        if ( 2.0*CAR_MIN_R > temp_x )
        {
            temp_pose.position_x = temp_x;
            //最小转弯半径构成的原函数
            temp_pose.position_y = park_pose4.position_y + sqrt( 2.0*temp_x*CAR_MIN_R - temp_x*temp_x );
            //对原函数求解一介导，在计算对应的头指向
            temp_pose.heading = 90.0 - atan( ( CAR_MIN_R - temp_x )/sqrt( 2.0*temp_x*CAR_MIN_R - temp_x*temp_x ) )*180.0/PI;
            temp_pose.k_s = -1.0 / CAR_MIN_R;
            out_points43.push_back( temp_pose );
        }
        else
        {
            cout<<" \\\\\\\\\\\\\\\\\\\\\\\\\\\\Error : sqrt( 2.0*temp_x*CAR_MIN_R - temp_x*temp_x )"<< endl;
            return 0;
        }
    }

    ///第二个转折点--------泊车终点  五次多项式拟合
    quintic_spline.gen_trj_from_two_points( park_pose5, park_pose4, 0.1 );
    quintic_spline.get_path_global( out_points54 );

    ///泊车坐标系下，泊车轨迹中D档部分
    trajectory_d.ref_points.clear();
    trajectory_d.ref_points = out_points12;
    ///泊车坐标系下，泊车轨迹中R档部分
    trajectory_r.ref_points.clear();
    trajectory_r.ref_points.insert( trajectory_r.ref_points.end(), out_points54.begin(), out_points54.end() );
    trajectory_r.ref_points.insert( trajectory_r.ref_points.end(), out_points43.begin(), out_points43.end() );
    trajectory_r.ref_points.insert( trajectory_r.ref_points.end(), out_points32.begin(), out_points32.end() );
    reverse( trajectory_r.ref_points.begin(), trajectory_r.ref_points.end() );

    return 1;
}

/**
* @brief 轨迹从泊车坐标系转换到全局坐标系。
* @return 1计算成功，0不成功。
*/
int IntelligentParking::transfer_trajectory_p2g()
{
    transfer_trajectory_p2g( p_park_trajectory_d_, park_trajectory_d_ );
    transfer_trajectory_p2g( p_park_trajectory_r_, park_trajectory_r_ );

    #if 0
    ///*******************************************************************************
    ofstream outfile1("log/park_trajectory_r_.log", std::ios::app);
    outfile1.precision(8);
    for(int i=0; i < park_trajectory_r_.ref_points.size(); i++)
    {
        outfile1 << " i " << i
                << " x " << park_trajectory_r_.ref_points[i].position_x
                << " y " << park_trajectory_r_.ref_points[i].position_y
                << " h " << park_trajectory_r_.ref_points[i].heading
                << " k " << park_trajectory_r_.ref_points[i].k_s
                << endl;
    }
    outfile1 << endl;
    outfile1.close();

    ///*******************************************************************************
    ofstream outfile2("log/park_trajectory_d_.log", std::ios::app);
    outfile2.precision(8);
    for(int i=0; i < park_trajectory_d_.ref_points.size(); i++)
    {
        outfile2 << " i " << i
                << " x " << park_trajectory_d_.ref_points[i].position_x
                << " y " << park_trajectory_d_.ref_points[i].position_y
                << " h " << park_trajectory_d_.ref_points[i].heading
                << " k " << park_trajectory_d_.ref_points[i].k_s
                << endl;
    }
    outfile2 << endl;
    outfile2.close();

    ///*******************************************************************************
    ofstream outfile3("log/p_park_trajectory_d_.log", std::ios::app);
    outfile3.precision(8);
    for(int i=0; i < p_park_trajectory_d_.ref_points.size(); i++)
    {
        outfile3 << " i " << i
                << " x " << p_park_trajectory_d_.ref_points[i].position_x
                << " y " << p_park_trajectory_d_.ref_points[i].position_y
                << " h " << p_park_trajectory_d_.ref_points[i].heading
                << " k " << p_park_trajectory_d_.ref_points[i].k_s
                << endl;
    }
    outfile3 << endl;
    outfile3.close();

    ///*******************************************************************************
    ofstream outfile4("log/p_park_trajectory_r_.log", std::ios::app);
    outfile4.precision(8);
    for(int i=0; i < p_park_trajectory_r_.ref_points.size(); i++)
    {
        outfile4 << " i " << i
                << " x " << p_park_trajectory_r_.ref_points[i].position_x
                << " y " << p_park_trajectory_r_.ref_points[i].position_y
                << " h " << p_park_trajectory_r_.ref_points[i].heading
                << " k " << p_park_trajectory_r_.ref_points[i].k_s
                << endl;
    }
    outfile4 << endl;
    outfile4.close();

    #endif

    return 1;
}

/**
* @brief 轨迹从泊车坐标系转换到全局坐标系。
* @param p_trajectory 输入量：泊车坐标系下轨迹
* @param g_trajectory 输入量：全局坐标系下轨迹
* @return 1计算成功，0不成功。
*/
int IntelligentParking::transfer_trajectory_p2g( path p_trajectory, path& g_trajectory )
{
    int num = p_trajectory.ref_points.size();
    if ( num <= 0 )
    {
        cout << "!!!p_trajectory is empty" << endl;
        return 0;
    }

    g_trajectory.ref_points.resize( num );
    for ( int i = 0; i < num; i++ )
    {
        transfer_pg_.transfer_l2g( p_trajectory.ref_points[i], g_trajectory.ref_points[i] );
    }
    return 1;
}

/**
* @brief 计算纵向信息
* @param p_trajectory 输入量：轨迹
* @param gear 输入量：档位
* @return 1计算成功，0不成功。
*/
int IntelligentParking::compute_longitudinal_info( path& p_trajectory, int8_t gear )
{
    int num = p_trajectory.ref_points.size();
    if ( num <= 0 )
    {
        cout << "!!!IntelligentParking::compute_longitudinal_info : p_trajectory is empty " << endl;
        return 0;
    }

    for( int i = 0; i < num; i++ )
    {
        if ( i < num-11 )
        {
            p_trajectory.ref_points[ i ].accelerataion = 0.0;
            p_trajectory.ref_points[ i ].speed_desired_Uxs = PARK_SPEED / 3.6;
            p_trajectory.ref_points[ i ].gears = gear;

        }
        else
        {
            p_trajectory.ref_points[ i ].accelerataion = 0.0;
            p_trajectory.ref_points[ i ].speed_desired_Uxs = 0.0;
            p_trajectory.ref_points[ i ].gears = AT_STATUS_P;
        }

    }
//    p_trajectory.ref_points[ num -1 ].accelerataion = 0.0;
//    p_trajectory.ref_points[ num -1 ].speed_desired_Uxs = 0.0;
//    p_trajectory.ref_points[ num -1 ].gears = AT_STATUS_P;

    return 1;
}

/**
* @brief 轨迹向前延长一定的点个数
* @param p_trajectory 输入量：轨迹
* @param extend_num 输入量：延长点的个数，单位：个
* @param interval 输入量：取点间隔，单位：米
* @param gear 输入量：档位
* @return 1计算成功，0不成功。
*/
int IntelligentParking::extend_trajectory( path& p_trajectory, int extend_num, double interval, int8_t gear )
{
    int num = p_trajectory.ref_points.size();
    if ( num <= 0 )
    {
        cout << "!!!IntelligentParking::extend_trajectory : p_trajectory is empty " << endl;
        return 0;
    }

    navi_point temp_p;
    temp_p.heading = p_trajectory.ref_points[ num-1 ].heading;
    temp_p.k_s = 0.0;
    temp_p.position_x = p_trajectory.ref_points[ num-1 ].position_x;
    temp_p.position_y = p_trajectory.ref_points[ num-1 ].position_y;

    if ( gear == AT_STATUS_R )
        interval *= -1.0;

    for ( int j = 0; j < extend_num; j++ )
    {
        temp_p.position_x += interval * sin( to_radians( temp_p.heading ) );
        temp_p.position_y += interval * cos( to_radians( temp_p.heading ) );

        temp_p.accelerataion = 0.0;
        temp_p.gears = AT_STATUS_P;
        temp_p.speed_desired_Uxs = 0.0;

#if 0
        if ( j < 10 )
        {
            temp_p.gears = gear;
            temp_p.speed_desired_Uxs = 5.0 / 3.6;
        }
        else
        {
            temp_p.gears = AT_STATUS_P;
            temp_p.speed_desired_Uxs = 0.0;
        }
#endif // 泊车后延temp

        p_trajectory.ref_points.push_back( temp_p );
    }

    return 1;
}

/**
* @brief 获得泊车轨迹中D档部分
* @param park_trajectory_d 输出量：全局坐标系下，泊车轨迹中D档部分
* @return 1计算成功，0不成功。
*/
int IntelligentParking::get_trajectory_d( path& park_trajectory_d )
{
    park_trajectory_d.ref_points.clear();
    park_trajectory_d.ref_points = park_trajectory_d_.ref_points;

    return 1;
}

/**
* @brief 获得泊车轨迹中R档部分
* @param park_trajectory_r 输出量：全局坐标系下，泊车轨迹中R档部分
* @return 1计算成功，0不成功。
*/
int IntelligentParking::get_trajectory_r( path& park_trajectory_r )
{
    park_trajectory_r.ref_points.clear();
    park_trajectory_r.ref_points = park_trajectory_r_.ref_points;

    return 1;
}

/**
* @brief 获得泊车轨迹中D档部分点的个数。
* @return 点的个数。
*/
int IntelligentParking::get_size_trajectory_d()
{
    return park_trajectory_d_.ref_points.size();
}

/**
* @brief 获得泊车轨迹中R档部分点的个数。
* @return 点的个数。
*/
int IntelligentParking::get_size_trajectory_r()
{
    return park_trajectory_r_.ref_points.size();
}
