#include <fstream>
#include <iostream>

#include "common/path.h"
#include "common/math_util.h"
#include "spline/spline.h"
#include "vehicle_dynamic/heading.h"
#include "vehicle_dynamic/steering_angle.h"
#include "vehicle_dynamic/cau_heading_steering.h"
//#include "planning/planning_param.h"
#include "config.h"

///zp20171013：对OBU下发低于进行预处理，处理折现点，处理断头路
int preprocess_path( path& p_in )
{
    int num_points = p_in.ref_points.size();
    int num_eraser = 20;
    if ( num_points < num_eraser + 2 )
        return 0;

    vector<navi_point>::iterator index;
    vector<navi_point> temp_points;
    temp_points = p_in.ref_points;

    double delta_heading = 0.0, temp_dh = 0.0, status_f = 0.0, status_b = 0.0;
    bool flag = 0; //0:b, 1:f
    int count_eraser = 0;

    //double last_heading = path.ref_points[ 0 ].heading;
    for ( int i = 1; i < num_points; i++ )
    {
        ///zp20171128:累加前需清零
        status_f = 0.0;
        status_b = 0.0;

        delta_heading = fabs( p_in.ref_points[ i ].heading - p_in.ref_points[ i - 1 ].heading );
        if ( delta_heading > 30.0 && ( fabs( delta_heading - 360.0 ) > 30.0 ) )
        {
            cout << i << "========preprocess_path========" << delta_heading << endl;
            index = temp_points.begin() + i + num_eraser * count_eraser;
            count_eraser++;

            if ( ( i - num_eraser - 1 ) < 0 )
                flag = 1;
            if ( ( i + num_eraser ) > num_points )
                flag = 0;

            if ( ( i - num_eraser - 1 ) > 0 && ( i + num_eraser ) < num_points )
            {
                for ( int j = 1; j < num_eraser; j++ )
                {
                    temp_dh = fabs( p_in.ref_points[ i - j ].heading - p_in.ref_points[ i - j - 1 ].heading );
                    if ( temp_dh > 270.0 )
                        temp_dh = fabs( temp_dh - 360.0 );
                    status_b += temp_dh;

                    temp_dh = fabs( p_in.ref_points[ i + j ].heading - p_in.ref_points[ i + j + 1 ].heading );
                    if ( temp_dh > 270.0 )
                        temp_dh = fabs( temp_dh - 360.0 );
                    status_f += temp_dh;
                }
                cout << "status_b = " << status_b << " , status_f = " << status_f << endl;

                if ( status_b < status_f + 5.0 )    ///zp20171115:前后段曲率相差不大的情况下，向后截取
                {
                    flag = 0;
                    if ( status_b > 50.0 )
                        num_eraser /= 2;
                }

                else
                {
                    flag = 1;
                    if ( status_f > 50.0 )
                        num_eraser /= 2;
                }
            }

            if ( flag )
            {
               temp_points.erase( (index - 1), (index + num_eraser - 1) );
                i += num_eraser;
            }
            else
            {
                temp_points.erase( (index - num_eraser), (index) );
                i++;
            }

            //i++;

        }


    }

    p_in.ref_points.clear();
    p_in.ref_points = temp_points;
    //cout << "[ temp_points ] = " << p_in.ref_points.size() << endl;

    return 1;
}

int preprocess_path2( path& p_in )
{
    int num_points = p_in.ref_points.size();
    int num_search = 20;
    if ( num_points < num_search * 2 )
        return 0;

    vector<navi_point>::iterator index;

    double delta_heading = 0.0;//, status_f = 0.0, status_b = 0.0;
    double dh1 = 0.0, dh2 = 0.0, ds1 = 0.0, ds2 = 0.0, k1 = 0.0, k2 = 0.0;
    int num_eraser = 0;
    //double last_heading = path.ref_points[ 0 ].heading;
    for ( int i = 1; i < num_points; i++ )
    {
        delta_heading = fabs( p_in.ref_points[ i ].heading - p_in.ref_points[ i - 1 ].heading );
        if ( delta_heading > 50.0 )
        {
            cout << "========preprocess_path2========" << i << endl;
            for ( int j = 0; j < num_search; j++ )
            {
                dh1 = p_in.ref_points[ i - j ].heading - p_in.ref_points[ i - j - 1 ].heading;
                dh2 = p_in.ref_points[ i + j ].heading - p_in.ref_points[ i + j - 1 ].heading;

                ds1 = p_in.ref_points[ i - j ].s - p_in.ref_points[ i - j - 1 ].s;
                ds2 = p_in.ref_points[ i + j ].s - p_in.ref_points[ i + j - 1 ].s;

                k1 = fabs( dh1 / ds1 );
                k2 = fabs( dh2 / ds2 );

                if ( k1 < 1.0 )
                {
                    p_in.ref_points.erase( p_in.ref_points.begin() + i - j );
                    num_eraser++;
                }
                if ( k2 < 1.0 )
                {
                    p_in.ref_points.erase( p_in.ref_points.begin() + i + j );
                    num_eraser++;
                }

                if ( num_eraser > 20 )
                    break;
            }

            i++;

        }


    }

    return 1;
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  only single spline cau steering angle and heading  ？？？？
//
/////////////////////////////////////////////////////////////////////////////////////////////////
void cau_all_output_from_map(path& p_in, path& p_out, int no_points, double speed)
{
    //if( no_points < 140 ) return;
///zp20171013：对OBU下发低于进行预处理，处理折现点，处理断头路
//    preprocess_path( p_in );

    int i;//,j,l,k;

    std::vector<double> x, y, s, h, st;
    std::vector<double> xx, yy, ss, hh, stst;

    x.clear();
    y.clear();
    s.clear();
    h.clear();

    tk::spline s_x1, s_y1;

    double steering_ks;
    double heading_ks;
    double Ux_ks;
    double cs, r;

    // 采集均匀的控制点
    //int interval = (int )(0.1 * INTERPOLATION_INTERVAL/SPLINE_EVERY );
    //if(no_points < 3*interval) return;

     //先加入一个
    s.push_back(p_in.ref_points[0].s);
    x.push_back(p_in.ref_points[0].position_x);
    y.push_back(p_in.ref_points[0].position_y);
    h.push_back(p_in.ref_points[0].heading);

    // 内部计算里程，可能出现负值的情况。
//    for(i= interval; i< no_points; i += interval )
//    {
//      //  if(p.ref_points[i].s > k*INTERPOLATION_INTERVAL
//      //     && p.ref_points[i].s > s.back() + 0.5 )
//        if(p_in.ref_points[i].s > s.back() + 0.5)
//        {
//            s.push_back(p_in.ref_points[i].s);
//            x.push_back(p_in.ref_points[i].position_x);
//            y.push_back(p_in.ref_points[i].position_y);
//            k++;
//        }
//    }
    for(unsigned int i = 1; i< p_in.ref_points.size(); i++)
    {
        /*double delta_h = fabs( p_in.ref_points[i].heading - h.back() );
        if ( delta_h > 300.0 )
        {
            delta_h = fabs( delta_h - 360.0 );
        }*/

        ///zp20171024:缩小地图拟合间隔，避免拟合偏差过大
        //if (p_in.ref_points[i].s>(s.back()+15.0)||delta_h>10.0)
        if (p_in.ref_points[i].s>(s.back()+5.0))
        {
            x.push_back(p_in.ref_points[i].position_x);
            y.push_back(p_in.ref_points[i].position_y);
            s.push_back(p_in.ref_points[i].s);
            h.push_back(p_in.ref_points[i].heading);
        }
    }

    //里程大于30m或曲率大于0.015取点
//    for ( int ii = 1; ii < p_in.ref_points; ii++ )
//    {
//        if ( fabs( p_in.ref_points[ii].s - p_in.ref_points[ii-1].s ) > 30.0 ||
//             fabs( ) )
//    }

    if(s.size() < 2) return;

    //里程的插值方法
    s_x1.set_points(s,x);
    s_y1.set_points(s,y);

    p_out.ref_points.clear();
    //p_out.ref_points.push_back( p_in.ref_points[ 0 ] );

    navi_point np;
    cs = p_in.ref_points[0].s;  //网络下发第一个控制点，里程不为零
    i = 0;

    //整个插值的里程需要小于最后一个点的里程。注意左右车道数，　这个插值后被去掉了。
    while( cs <= s.back() && i < p_in.ref_points.size() )
    {
//        cs +=  MAP_SPLINE_EVERY;  // 0.1//1.5
        cs = p_in.ref_points[i].s;
        Ux_ks  =  0; // speed;

//        steering_ks = cau_steering_angle_from_ks(
//                          s_x1,
//                          s_y1,
//                          cs,
//                          Ux_ks,
//                          r,
//                          STEERING_CAU_ONE_POINT,
//                          STEERING_CAU_TWO_POINT);
//********
        steering_ks = cau_steering_angle_from_ks(
                          s_x1,
                          s_y1,
                          cs,
                          Ux_ks,
                          r);

        steering_ks = iclamp(steering_ks, MIN_STEERING_ANGLE, MAX_STEERING_ANGLE);

//        double heading_length = HEADING_LENGTH_CONST;
//        double zero_offset = HEADING_ZERO_OFFSET;

//        heading_ks = cau_heading_angle_from_ks(
//                         s_x1,
//                         s_y1,
//                         cs,
//                         HEADING_LENGTH_CONST);
//********
        heading_ks = cau_heading_angle_from_ks(
                         s_x1,
                         s_y1,
                         cs);

        np.s = cs;
        np.point_no = i;
        np.heading = heading_ks;
        np.steering_angle = steering_ks;
        np.position_x = s_x1(cs);
        np.position_y = s_y1(cs);

//        if ( i >= p_in.ref_points.size() )
//            i = p_in.ref_points.size() - 1;
        np.speed_desired_Uxs = p_in.ref_points[ i ].speed_desired_Uxs;
        np.type = p_in.ref_points[ i ].type;

//        int every = speed * MARK_CONTROL_SPEED_RATIO;
//        if(every <= MARK_MIN_NUM)
//            every = MARK_MIN_NUM;
//
//        every = MARK_EVERY;
//        if( i % every  == 0 )  //控制点密度和当前速度相关。speed
//            np.control_mark = 1;
//        else
//            np.control_mark = 0;

        p_out.ref_points.push_back(np);

        i++;
    }

    if ( MAP_LOG_SWITCH )
    {
        ofstream outfile("steering_ks_heading_ks_map.log", std::ios::app);
        outfile.precision(8);

        for ( int i = 0; i < p_out.ref_points.size(); i++ )
        {
            outfile << " i " << i
                    << " point_no " << p_out.ref_points[i].point_no
                    << " s " << p_out.ref_points[i].s
                    << " heading " << p_out.ref_points[i].heading
                    << " steering_angle " << p_out.ref_points[i].steering_angle
                    << " k_s " << p_out.ref_points[i].k_s
                    << " position_x " << p_out.ref_points[i].position_x
                    << " position_y " << p_out.ref_points[i].position_y
                    << endl;
        }

        outfile << endl << endl;
        outfile.close();
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  only single spline cau steering angle and heading 从文件中读， 不存在里程为负值的情况。
//
/////////////////////////////////////////////////////////////////////////////////////////////////
void cau_all_output_from_single_spline(path& p, int no_points, double speed)
{
    if( no_points < 10 ) return;

    int i,k;

    std::vector<double> x, y, s, h, st;
    std::vector<double> xx, yy, ss, hh, stst;

    tk::spline s_x1, s_y1;

    double steering_ks;
    double heading_ks;
    double Ux_ks;
    double cs, r;


    int interval = (int )(5/SPLINE_EVERY );
    if(no_points < 3*interval) return;

     //先加入一个
    s.push_back(p.ref_points[0].s);
    x.push_back(p.ref_points[0].position_x);
    y.push_back(p.ref_points[0].position_y);

    // 内部计算里程，可能出现负值的情况。
    for(i= interval; i< no_points; i += interval )
    {
      //  if(p.ref_points[i].s > k*INTERPOLATION_INTERVAL
      //     && p.ref_points[i].s > s.back() + 0.5 )
        if(p.ref_points[i].s > s.back() + 0.5)
        {
            s.push_back(p.ref_points[i].s);
            x.push_back(p.ref_points[i].position_x);
            y.push_back(p.ref_points[i].position_y);
            k++;
        }
    }

    if(s.size() < 2) return;

    //里程的插值方法
    s_x1.set_points(s,x);
    s_y1.set_points(s,y);

    p.ref_points.clear();  //?????? big error
    navi_point np;

    cs = 0;
    i = 0;

    //整个插值的里程需要小于最后一个点的里程。
    while( cs< s.back())
    {
        cs =  i * SPLINE_EVERY;  // 0.1
        Ux_ks  =  0; // speed;

//        steering_ks = cau_steering_angle_from_ks(
//                          s_x1,
//                          s_y1,
//                          cs,
//                          Ux_ks,
//                          r,
//                          STEERING_CAU_ONE_POINT,
//                          STEERING_CAU_TWO_POINT);
//********
        steering_ks = cau_steering_angle_from_ks(
                          s_x1,
                          s_y1,
                          cs,
                          Ux_ks,
                          r);

        steering_ks = iclamp(steering_ks, MIN_STEERING_ANGLE, MAX_STEERING_ANGLE);

//        double heading_length = HEADING_LENGTH_CONST;
//        double zero_offset = HEADING_ZERO_OFFSET;

//        heading_ks = cau_heading_angle_from_ks(
//                         s_x1,
//                         s_y1,
//                         cs,
//                         HEADING_LENGTH_CONST);
//********
        heading_ks = cau_heading_angle_from_ks(
                         s_x1,
                         s_y1,
                         cs);

        np.s = cs;
        np.point_no = i;
        np.heading = heading_ks;
        np.steering_angle = steering_ks;
        np.position_x = s_x1(cs);
        np.position_y = s_y1(cs);

//        int every = speed * MARK_CONTROL_SPEED_RATIO;
//        if(every <= MARK_MIN_NUM)
//            every = MARK_MIN_NUM;
//
//        every = MARK_EVERY;
//        if( i % every  == 0 )  //控制点密度和当前速度相关。speed
//            np.control_mark = 1;
//        else
//            np.control_mark = 0;

        p.ref_points.push_back(np);

        i++;
    }

//    int length = p.ref_points.size();

//    spline_kp(p, speed, 0, length );

}


//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  only single spline cau steering angle and heading 从文件中读， 不存在里程为负值的情况。
//
/////////////////////////////////////////////////////////////////////////////////////////////////
void cau_points_heading_steering_from_spline(vector<navi_point>& ref_points, int no_points, double speed)
{
    if( no_points < 10 ) return;

    int i;//,j,k;

    std::vector<double> x, y, s, h, st;
    std::vector<double> xx, yy, ss, hh, stst;

    tk::spline s_x1, s_y1;

    double steering_ks;
    double heading_ks;
    double Ux_ks;
    double cs, r;

    if(no_points < 210) return;

    // 内部计算里程，可能出现负值的情况。
    for(i= 0; i< no_points; i += 70 )
    {
        s.push_back(ref_points[i].s);
        x.push_back(ref_points[i].position_x);
        y.push_back(ref_points[i].position_y);
    }

    if(s.size() < 2) return;

    //里程的插值方法
    s_x1.set_points(s,x);
    s_y1.set_points(s,y);

    ref_points.clear();  //?????? big error
    navi_point np;

    cs = 0;

    //整个插值的里程需要小于最后一个点的里程。
    for(int i= 0; i< no_points; i ++)
    {
        cs = ref_points[i].s;  // 0.1
        Ux_ks  = 0; // speed;

//        steering_ks = cau_steering_angle_from_ks(
//                          s_x1,
//                          s_y1,
//                          cs,
//                          Ux_ks,
//                          r,
//                          STEERING_CAU_ONE_POINT,
//                          STEERING_CAU_TWO_POINT);
//********
        steering_ks = cau_steering_angle_from_ks(
                          s_x1,
                          s_y1,
                          cs,
                          Ux_ks,
                          r);

        steering_ks = iclamp(steering_ks, MIN_STEERING_ANGLE, MAX_STEERING_ANGLE);

//        double heading_length = HEADING_LENGTH_CONST;
//        double zero_offset = HEADING_ZERO_OFFSET;

//        heading_ks = cau_heading_angle_from_ks(
//                             s_x1,
//                             s_y1,
//                             cs,
//                             HEADING_LENGTH_CONST);
//********
        heading_ks = cau_heading_angle_from_ks(
                         s_x1,
                         s_y1,
                         cs);

        np.s = cs;
        np.point_no = i;
        np.heading = heading_ks;
        np.steering_angle = steering_ks;
        np.position_x = s_x1(cs);
        np.position_y = s_y1(cs);

//        int every = speed * MARK_CONTROL_SPEED_RATIO;
//        if(every <= MARK_MIN_NUM)
//            every = MARK_MIN_NUM;
//
//        every = MARK_EVERY;
//        if( i % every  == 0 )  //控制点密度和当前速度相关。speed
//            np.control_mark = 1;
//        else
//            np.control_mark = 0;

        ref_points.push_back(np);
    }

//    int length = p.ref_points.size();
//
//    ofstream outfile("steering_ks_heading_ks_control.log", std::ios::app);
//    outfile.precision(8);
//
//    for ( int i = 0; i < length; i++ )
//    {
//        outfile << " i " << i
//                << " point_no " << p.ref_points[i].point_no
//                << " s " << p.ref_points[i].s
//                << " heading " << p.ref_points[i].heading
//                << " steering_angle " << p.ref_points[i].steering_angle
//                << " position_x " << p.ref_points[i].position_x
//                << " position_y " << p.ref_points[i].position_y
//                << endl;
//    }
//
//    outfile << endl << endl;
//    outfile.close();


 //   spline_kp(p, speed, 0, length );

}

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//   realtime single spline cau steering angle and heading
//
/////////////////////////////////////////////////////////////////////////////////////////////////

void cau_heading_steering_from_spline_half(
        path& p,
        int st_p,
        int end_p,
        double speed,
        double steering)
{
    if( end_p - st_p < 50 ) return;

    int i,k;
    double Ux_ks;
    double cs, r;
    double steering_ks;
    double heading_ks;

    std::vector<double> x, y, s;

    // 采集均匀的控制点
    k=0;

    x.clear();
    y.clear();
    s.clear();

    int length = p.ref_points.size();
//    double s0 = p.ref_points[0].s;
//    double sl = 0;
    int interval = (int )(5/SPLINE_EVERY );

    if(length < 3*interval)
      return;

    //先加入一个
    s.push_back(p.ref_points[0].s);
    x.push_back(p.ref_points[0].position_x);
    y.push_back(p.ref_points[0].position_y);

    int last_index = 0;

    // 内部计算里程，可能出现负值的情况。
    for(i= interval; i< length; i += interval )
    //for(i= 1; i< length; i++ )
    {
        if(p.ref_points[i].s > s.back() + 0.5)
        //if( fabs(p.ref_points[i].s-p.ref_points[last_index].s) > 30.0 ||
            //fabs(p.ref_points[i].steering_angle-p.ref_points[last_index].steering_angle) > 60 )
        {
            s.push_back(p.ref_points[i].s);
            x.push_back(p.ref_points[i].position_x);
            y.push_back(p.ref_points[i].position_y);
            k++;

            last_index = i;
        }
    }

    if(s.size() < 2) return;

    tk::spline s_x1, s_y1;
    s_x1.set_points(s,x);
    s_y1.set_points(s,y);

    int start =0;

    // 用于记录需要计算方向盘和头指向的位置的起点
    // 在插入规划的新轨迹之前的第一个点开始计算
//    if( p.st_pos - BEFORE_JOIN_TWO_LINE_NUM > 0)
//    {
//        start = p.st_pos - BEFORE_JOIN_TWO_LINE_NUM;
//        cout << "p.st_pos = " << p.st_pos << ", BEFORE_JOIN_TWO_LINE_NUM = " << BEFORE_JOIN_TWO_LINE_NUM << endl;
//
//    }
//
//    else
       start = 0;

    for(i = start; i< length; i++)
    {
        cs = p.ref_points[i].s;
        Ux_ks = 0; //speed;

//        steering_ks = cau_steering_angle_from_ks(
//                          s_x1,
//                          s_y1,
//                          cs,
//                          Ux_ks,
//                          r,
//                          STEERING_CAU_ONE_POINT,
//                          STEERING_CAU_TWO_POINT);
//********
        steering_ks = cau_steering_angle_from_ks(
                          s_x1,
                          s_y1,
                          cs,
                          Ux_ks,
                          r);

        p.ref_points[i].r = r;

        steering_ks = fclamp(steering_ks, MIN_STEERING_ANGLE, MAX_STEERING_ANGLE);

//        double heading_length = HEADING_LENGTH_CONST;
//        double zero_offset = HEADING_ZERO_OFFSET;

//        heading_ks = cau_heading_angle_from_ks(
//                             s_x1,
//                             s_y1,
//                             cs,
//                             HEADING_LENGTH_CONST);
//********
        heading_ks = cau_heading_angle_from_ks(
                         s_x1,
                         s_y1,
                         cs);

        p.ref_points[i].position_x = s_x1(cs);
        p.ref_points[i].position_y = s_y1(cs);
        p.ref_points[i].heading = heading_ks;
        p.ref_points[i].steering_angle = steering_ks;
    }
//zp:20171127无用，暂时注释掉
    //length = p.ref_points.size();
    //cout << " without spline new !!! " << endl;

//    ofstream outfile("steering_ks_heading_ks_control.log", std::ios::app);
//    outfile.precision(8);
//
//    for ( int i = 0; i < length; i++ )
//    {
//        outfile << " i " << i
//                << " point_no " << p.ref_points[i].point_no
//                << " s " << p.ref_points[i].s
//                << " heading " << p.ref_points[i].heading
//                << " steering_angle " << p.ref_points[i].steering_angle
//                << " position_x " << p.ref_points[i].position_x
//                << " position_y " << p.ref_points[i].position_y
//                << endl;
//    }
//
//    outfile << endl << endl;
//    outfile.close();

//zp:20171127无用，暂时注释掉
    //spline_head(p, speed, 0, length );
    //spline_steering(p, speed, 0, length );

}

double cau_max_steering(
    int current,
    path& p,
    int length)
{
    int i;
    double max_steering =0;
    double val_steering =0;

    if(length <=0)
       return 0;

    if(p.ref_points.size() < length)
       return fabs(p.ref_points[current].steering_angle);

    if(current < length/2)
       return fabs(p.ref_points[current].steering_angle);

    //计算前后二分之一的方向盘转角最大值来作为系统辨识的参数。
    for( i =current - length/2 ; i< length/2; i++)
    {
        val_steering =fabs(p.ref_points[i].steering_angle);
        if(max_steering < val_steering)
            max_steering = val_steering;
    }

    return max_steering;
}

//void spline_kp(path& p, double speed, int start, int length )
//{
//    unsigned int i;
//    std::vector<double> ss, skp;
////    double cs;
////    double cau_kp;
//
//    ss.clear();
//    skp.clear();
//
//    //  550 -> 12
//    //  15 -> 2
//    //  550 -15 = 545/10 = 54.5
//
//    // 200 / 54.5 = 4+2 = 6
//    // 400 / 54.5  = 8+2 = 10
//    double max_steering = 0;
//
//    for(i=0; i< length; i ++)
//    {
//        // 0.1  //需要处理前后50个点的方向盘转角。这样来进行系统辨识。
//       // max_steering = cau_max_steering(i, p, 100);
//        max_steering = fabs(p.ref_points[i].steering_angle);
////        cau_kp = (max_steering)/KP_SLOPE + KP_VALUE;
////
////        if(cau_kp > KP_MAX)
////            cau_kp = KP_MAX;
//
////        p.ref_points[i].suggest_kp = cau_kp;
//    }
//
//    int every = speed * 7;
//    if(every <= 15)
//        every = 15;
//
//    every = 80;
//
//    ss.push_back(p.ref_points[0].s);
////    skp.push_back(p.ref_points[0].suggest_kp);
//
//    for(i=1; i< length; i += every)
//    {
//        if(p.ref_points[i].s > ss.back() + 0.5)
//        {
//          ss.push_back(p.ref_points[i].s);
////          skp.push_back(p.ref_points[i].suggest_kp);
//        }
//    }
//
//    tk::spline skp_st;
//    skp_st.set_points(ss, skp);
//
//    for(i=0; i< length; i++)
//    {
//        cs = p.ref_points[i].s;
//        cau_kp = skp_st(cs);
////        p.ref_points[i].suggest_kp = cau_kp;
//    }
//}

void spline_steering(path& p, double speed, int start, int length )
{
    unsigned int i;
    std::vector<double> st, sk;
    double cs;
    double steering_ks;
    st.clear();
    sk.clear();

    int every = speed * 7;
    if(every <= 15)
        every = 15;

    every = 80;

    sk.push_back(p.ref_points[0].s);
    st.push_back(p.ref_points[0].steering_angle);

    for(i=1; i< length; i ++)
    {
        if( /*(p.ref_points[i].control_mark == 1) &&*/
            (p.ref_points[i].s > sk.back() + 0.5))
        {
            sk.push_back(p.ref_points[i].s);
            st.push_back(p.ref_points[i].steering_angle);
        }
    }

    tk::spline sk_st;
    sk_st.set_points(sk, st);

    for(i=start; i< length; i++)
    {
        cs = p.ref_points[i].s;

        steering_ks = sk_st(cs);

        steering_ks = fclamp(
                          steering_ks,
                          MIN_STEERING_ANGLE,
                          MAX_STEERING_ANGLE);

        p.ref_points[i].steering_angle = steering_ks;
    }

}

void spline_head(path& p, double speed, int start,  int length )
{
    unsigned int i;
    double cs;
    std::vector<double> h, ss, h2;
    std::vector<navi_point> h_point;

    h.clear();
    ss.clear();
    h_point.clear();

    int every = speed * 7; // 0.7 * 10; //70 = 7m  speed * 10
    if(every <= 15)
        every = 15;

    every = 80;

    ss.push_back(p.ref_points[0].s);
    h.push_back(p.ref_points[0].heading);
    h_point.push_back(p.ref_points[0]);

    for(i = 1; i< length; i++)
    {
        if(/*(p.ref_points[i].control_mark == 1) &&*/
           (p.ref_points[i].s > ss.back() + 0.5))
        {
            ss.push_back(p.ref_points[i].s);
            h.push_back(p.ref_points[i].heading);
            h_point.push_back(p.ref_points[i]);
        }
    }

    for(i = 0; i < h_point.size()-1; i++)
    {
        //确定整周数
        double delt_heading = h_point[i+1].heading - h_point[i].heading;
        int cycle_num = delt_heading /360;
//        h_point[i+1].cycle_tag = cycle_num;
        h_point[i+1].heading = h_point[i+1].heading - cycle_num*360;

        //判断是否连续
        if((h_point[i+1].heading - h_point[i].heading) > 270.0)
        {
            h_point[i+1].heading = h_point[i+1].heading - 360;
//            h_point[i+1].cycle_tag++;
        }

        if((h_point[i+1].heading - h_point[i].heading ) < -270.0)
        {
            h_point[i+1].heading = h_point[i+1].heading + 360;
//            h_point[i+1].cycle_tag--;
        }

        h2.push_back(h_point[i].heading);

    }
    h2.push_back(h_point.back().heading);

    tk::spline sk_h, sk_sh_point;
    sk_sh_point.set_points(ss,h2);

    for(i=start; i< length; i++)
    {
        cs = p.ref_points[i].s;
        p.ref_points[i].heading = sk_sh_point(cs);
        if(p.ref_points[i].heading > 360)
            p.ref_points[i].heading -= 360;

        if(p.ref_points[i].heading < 0)
            p.ref_points[i].heading += 360;
    }

}
