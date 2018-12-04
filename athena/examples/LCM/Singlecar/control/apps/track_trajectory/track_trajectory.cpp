#include "track_trajectory.h"

namespace athena{
namespace control{
void TrackTrajectory::Init(string local_trajectory_path,ControlLogic *control_logic)
{
    control_logic_ = control_logic;
    recv_path_.read_navi_file(local_trajectory_path,&control_logic -> controller_config_);
    CauAllOutputFromSingleSpline(recv_path_, recv_path_.ref_points_.size(), 10);
    CauPathFromSpline(recv_path_, recv_path_.ref_points_.size());
    PathToTrajectory(recv_path_,trajectory_);
    control_logic -> SubscribeTrajectory(trajectory_);
}

void TrackTrajectory::PathToTrajectory(path &path,Trajectory &trajectory)
{
   ///轨迹点数量
   trajectory.points_.clear();
   NavPoints navpoints;
   trajectory.num_of_points_ = path.ref_points_.size();

   for(int i = 0; i < trajectory.num_of_points_;i++)
   {
     ///GPS 时间
     navpoints.gps_time_ = 0.0;
     ///x值
     navpoints.p_x_ = path.ref_points_[i].position_x_;
     ///y值
     navpoints.p_y_ = path.ref_points_[i].position_y_;
     ///里程值
     navpoints.s_   = path.ref_points_[i].s_;
     ///到达该点的速度
     navpoints.p_v_ = tar_speed_;
     ///到达该点的加速度
     navpoints.p_a_ = path.ref_points_[i].accelerataion_;
     ///到达该点的航向角
     navpoints.p_h_ = path.ref_points_[i].heading_;
     ///到达该点的曲率
     navpoints.p_k_ = path.ref_points_[i].k_s_;
    //std::cout<<"p_k_:"<<navpoints.p_k_ <<endl;
     ///到达该点的档位
     navpoints.p_g_ = path.ref_points_[i].p_g_;

     trajectory.points_.push_back(navpoints);
   }
}

void TrackTrajectory::CauAllOutputFromSingleSpline(path& p, int no_points, double speed)
{
    if( no_points < 10 )
    {
        cout << "ponis num not enough" << endl;
        abort();
    }

    int i,k;

    std::vector<double> x, y, s, h, st;
    std::vector<double> xx, yy, ss, hh, stst;

    tk::spline s_x1, s_y1;

    double steering_ks;
    double heading_ks;
    double Ux_ks;
    double cs, r;

    double INTERPOLATION_INTERVAL = 7;
    double spline_every_ = 0.1;
    int interval = (int )(INTERPOLATION_INTERVAL/spline_every_ );   //INTERPOLATION_INTERVAL = 7; spline_every_ = 0.1
    if(no_points < 3*interval)
    {
        cout << "ponis num not enough" << endl;
        abort();
    }

    // 采集均匀的控制点
    k=0;
    /*  for( i=0;  i< no_points;  i +=INTERPOLATION_POINT_NUM)
    {
        //   if(p.ref_points_[i].s > k*INTERPOLATION_INTERVAL)  //0.348925  //0.308564
        {
            s.push_back(p.ref_points_[i].s);
            x.push_back(p.ref_points_[i].position_x);
            y.push_back(p.ref_points_[i].position_y);
            k++;
        }
    }*/
    //先加入一个
    s.push_back(p.ref_points_[0].s_);
    x.push_back(p.ref_points_[0].position_x_);
    y.push_back(p.ref_points_[0].position_y_);
    k++;
    // 内部计算里程，可能出现负值的情况。
    //       70
    for(i= interval; i< no_points; i += interval )
    {
        //  if(p.ref_points_[i].s > k*INTERPOLATION_INTERVAL
        //     && p.ref_points_[i].s > s.back() + 0.5 )
        if(p.ref_points_[i].s_ > s.back() + 0.1)
        {
            s.push_back(p.ref_points_[i].s_);
            x.push_back(p.ref_points_[i].position_x_);
            y.push_back(p.ref_points_[i].position_y_);
            k++;
        }
    }
    if(s.size() < 2)
    {
        cout << "spline points not enough" << endl;
        abort();
    }

    //里程的插值方法
    s_x1.set_points(s,x);
    s_y1.set_points(s,y);

    p.ref_points_.clear();  //?????? big error
    navi_point np;

    cs = 0;
    i = 0;
    while( cs<= s[s.size()-1])
    {
        cs =  i * spline_every_;  // 0.1
        Ux_ks  =  const_speed_; // speed;

        steering_ks = cau_steering_angle_from_ks(
                          s_x1,
                          s_y1,
                          cs,
                          Ux_ks,
                          r,
                          steering_cau_one_point_,
                          steering_cau_two_point_,
                          control_logic_ -> controller_config_.steer_tranmission_ratio_);

        steering_ks = iclamp(steering_ks, control_logic_ -> controller_config_.min_steering_angle_ ,control_logic_ -> controller_config_.max_steering_angle_);

        //add by alex 20170607 new way to calc heading
        heading_ks = cau_heading_angle_from_ks(s_x1,s_y1,cs);

        np.s_ = cs;
        np.point_no_ = i;
        np.heading_ = heading_ks;
        np.steering_angle_ = steering_ks;
        //std::cout<<"steering_angle:"<<np.steering_angle_<<endl;
        np.position_x_ = s_x1(cs);
        np.position_y_ = s_y1(cs);
        np.k_s_ = 1/r;

        int every = mark_every_;
        if( i % every  == 0 )  //控制点密度和当前速度相关。speed
            np.control_mark_ = 1;
        else
            np.control_mark_ = 0;

        p.ref_points_.push_back(np);

        i++;
    }

    int length = p.ref_points_.size();
    SplineKp(p, speed, length );

}

//add by alex 20170615 对接收的轨迹重新插值计算
//使用该方法的前提是传入的轨迹点距必须时0.1m
//新轨迹需要的信息：x,y,heading,ks，s,steering_angle,gps_time,v,a,档位,其中gps_time,v,a,档位直接读取原始值
//根据曲率变化重新插值
#define KS_STEP 0.015
#define S_STEP 20
void TrackTrajectory::CauPathFromSpline(path& p, int no_points)
{
    if( no_points < 10 )
    {
        cout << "ponis num not enough" << endl;
        abort();
    }

    int i,k;

    std::vector<double> x, y, s;
    //std::vector<double> steer_angle, point_speed;
    tk::spline s_x1, s_y1;
    //tk::spline s_steer,s_speed;

    double steering_ks;
    double heading_ks;
    double Ux_ks;
    double cs, r;

    double INTERPOLATION_INTERVAL = 7;
    double spline_every_ = 0.1;

    int interval = (int )(INTERPOLATION_INTERVAL/spline_every_ );   //INTERPOLATION_INTERVAL = 7; spline_every_ = 0.1 每70个点取一个点
    if(no_points < 3*interval)
    {
        cout << "ponis num not enough" << endl;
        abort();
    }

    // 采集均匀的控制点
    k=0;
    //先加入一个
    s.push_back(p.ref_points_[0].s_);
    x.push_back(p.ref_points_[0].position_x_);
    y.push_back(p.ref_points_[0].position_y_);
    k++;
    // 内部计算里程，可能出现负值的情况。
    //       70
    for(i= interval; i< no_points; i += interval )
    {
        //  if(p.ref_points_[i].s > k*INTERPOLATION_INTERVAL
        //     && p.ref_points_[i].s > s.back() + 0.5 )
        if(p.ref_points_[i].s_ > s.back() + 0.1)
        {
            s.push_back(p.ref_points_[i].s_);
            x.push_back(p.ref_points_[i].position_x_);
            y.push_back(p.ref_points_[i].position_y_);
            k++;
        }
        double distance = 0;
        double delta_ks=0;
        interval = 1;
        while(distance < S_STEP && (i+interval < no_points) && (delta_ks < KS_STEP))//取点改为每隔3m取一个点
        {
            distance = length_two_points(p.ref_points_[i+interval].position_x_, p.ref_points_[i+interval].position_y_,
                                         p.ref_points_[i].position_x_, p.ref_points_[i].position_y_);
            delta_ks = fabs(p.ref_points_[i].k_s_ - p.ref_points_[i+interval].k_s_);
            interval++;
        }
    }
    if(s.size() < 2)
    {
        cout << "spline points not enough" << endl;
        abort();
    }

    //里程的插值方法
    s_x1.set_points(s,x);
    s_y1.set_points(s,y);

    path path_bak;
    path_bak.ref_points_.clear();
    path_bak.reset_path(p.ref_points_);//copy some msg to path_bak

    p.ref_points_.clear();
    navi_point np;

    cs = 0;
    i = 0;
    while( cs<= s[s.size()-1])
    {
        cs =  i * spline_every_;  // 0.1
        Ux_ks  =  const_speed_; // speed;

        if(cs <= 5.5)
        {
            steering_ks = cau_steering_angle_from_ks(s_x1,s_y1,cs,Ux_ks,r,0,8.5,control_logic_ -> controller_config_.steer_tranmission_ratio_);
        }
        else
        {
            steering_ks = cau_steering_angle_from_ks(
                              s_x1,
                              s_y1,
                              cs,
                              Ux_ks,
                              r,
                              -5.5,//-5.5 STEERING_CAU_ONE_POINT debug by alex
                              8.5,
                              control_logic_ -> controller_config_.steer_tranmission_ratio_);//8.5 STEERING_CAU_TWO_POINT
        }

        steering_ks = iclamp(steering_ks, control_logic_ -> controller_config_.min_steering_angle_, control_logic_ -> controller_config_.max_steering_angle_);

        //add by alex 20170607 new way to calc heading
        heading_ks = cau_heading_angle_from_ks(s_x1,s_y1,cs);

        np.s_ = cs;
        np.point_no_ = i;
        np.heading_ = heading_ks;

        np.steering_angle_ = steering_ks;
        np.position_x_ = s_x1(cs);
        np.position_y_ = s_y1(cs);

        //add by alex20170615
        np.pos_gps_time_ = path_bak.ref_points_[i].pos_gps_time_;
        np.acceleration_desired_Axs_ = path_bak.ref_points_[i].acceleration_desired_Axs_;
        np.speed_desired_Uxs_ = path_bak.ref_points_[i].speed_desired_Uxs_;
        //np.curvature_ = path_bak.ref_points_[i].curvature_;
        np.p_g_ = path_bak.ref_points_[i].p_g_;

        np.k_s_ = 1.0/r;//曲率反向
        np.curvature_ = np.k_s_;


        p.ref_points_.push_back(np);

        i++;
    }
    path_bak.ref_points_.clear();

    int length = p.ref_points_.size();
    SplineKp(p, 8, length );

}

// 计算动态KP值（elvis最新修改，修改前会造成内存错误）
void TrackTrajectory::SplineKp(path& p, double speed, int length )
{
    int i;
    std::vector<double> ss, skp; // 构造二维矩阵：X，Y
    double cs;
    double cau_kp;
    ss.clear();
    skp.clear();

    // 550 -> 12
    // 15 -> 2
    // 550 -15 = 545/10 = 54.5

    // 200 / 54.5 = 4+2 = 6
    // 400 / 54.5  = 8+2 = 10
    double max_steering = 0;
    if((length < 30) || (length > 5000)) return; //地图检验
    for(i=0; i< length; i ++)
    {
        max_steering = fabs(p.ref_points_[i].steering_angle_);
        cau_kp = (max_steering)/kp_slope_ + kp_value_;

        if(cau_kp > 10.0)
            cau_kp = 10;

        p.ref_points_[i].suggest_kp_ = cau_kp;
    }

    int every = mark_every_;//70

    for(i=0; i< length; i += every)
    {
        if (i==0)
        {
            ss.push_back(p.ref_points_[i].s_); // 将曲率传入到矩阵中
            skp.push_back(p.ref_points_[i].suggest_kp_);// 将推荐KP传入到矩阵中
        }
        else
        {
            if(p.ref_points_[i].s_ > ss.back() + 0.5)
            {
                ss.push_back(p.ref_points_[i].s_);
                skp.push_back(p.ref_points_[i].suggest_kp_);
            }
        }
    }
    // 新建一个拟合器
    tk::spline skp_st;
    // 拟合器更新拟合点
    skp_st.set_points(ss, skp);// tips

    for(i=0; i< length; i++)
    {
        cs = p.ref_points_[i].s_;
        cau_kp = skp_st(cs);
        p.ref_points_[i].suggest_kp_ = cau_kp;
    }
}
}
}
