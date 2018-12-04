
#include "cau_heading_steering.h"

//////////////////////////////////////////////////////////////////////////////////////////////////
//
//  only single spline cau steering angle and heading ���ļ��ж��� ���������Ϊ��ֵ�������
//  ��ǰʹ�õķ���
/////////////////////////////////////////////////////////////////////////////////////////////////
//�����ص�ͼ
void cau_all_output_from_single_spline(path& p, int no_points, double speed)
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
    double SPLINE_EVERY = 0.1;
    int interval = (int )(INTERPOLATION_INTERVAL/SPLINE_EVERY );   //INTERPOLATION_INTERVAL = 7; SPLINE_EVERY = 0.1
    if(no_points < 3*interval)
    {
        cout << "ponis num not enough" << endl;
        abort();
    }

    // �ɼ����ȵĿ��Ƶ�
    k=0;
    /*  for( i=0;  i< no_points;  i +=INTERPOLATION_POINT_NUM)
    {
        //   if(p.ref_points[i].s > k*INTERPOLATION_INTERVAL)  //0.348925  //0.308564
        {
            s.push_back(p.ref_points[i].s);
            x.push_back(p.ref_points[i].position_x);
            y.push_back(p.ref_points[i].position_y);
            k++;
        }
    }*/
    //�ȼ���һ��
    s.push_back(p.ref_points_[0].s_);
    x.push_back(p.ref_points_[0].position_x_);
    y.push_back(p.ref_points_[0].position_y_);
    k++;
    // �ڲ�������̣����ܳ��ָ�ֵ�������
    //       70
    for(i= interval; i< no_points; i += interval )
    {
        //  if(p.ref_points[i].s > k*INTERPOLATION_INTERVAL
        //     && p.ref_points[i].s > s.back() + 0.5 )
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

    //��̵Ĳ�ֵ����
    s_x1.set_points(s,x);
    s_y1.set_points(s,y);

    p.ref_points.clear();  //?????? big error
    navi_point np;

    cs = 0;
    i = 0;
    while( cs<= s[s.size()-1])
    {
        cs =  i * SPLINE_EVERY;  // 0.1
        Ux_ks  =  CONST_SPEED; // speed;

        steering_ks = cau_steering_angle_from_ks(
                          s_x1,
                          s_y1,
                          cs,
                          Ux_ks,
                          r,
                          STEERING_CAU_ONE_POINT,
                          STEERING_CAU_TWO_POINT);

        steering_ks = iclamp(steering_ks, MIN_STEERING_ANGLE, MAX_STEERING_ANGLE);

        //add by alex 20170607 new way to calc heading
        heading_ks = cau_heading_angle_from_ks(s_x1,s_y1,cs);

        np.s = cs;
        np.point_no = i;
        np.heading = heading_ks;
        np.steering_angle = steering_ks;
        np.position_x = s_x1(cs);
        np.position_y = s_y1(cs);
        np.k_s = 1/r;

        int every = MARK_EVERY;
        if( i % every  == 0 )  //���Ƶ��ܶȺ͵�ǰ�ٶ���ء�speed
            np.control_mark = 1;
        else
            np.control_mark = 0;

        p.ref_points.push_back(np);

        i++;
    }

    int length = p.ref_points.size();
    spline_kp(p, speed, length );

}

//add by alex 20170615 �Խ��յĹ켣���²�ֵ����
//ʹ�ø÷�����ǰ���Ǵ���Ĺ켣������ʱ0.1m
//�¹켣��Ҫ����Ϣ��x,y,heading,ks��s,steering_angle,gps_time,v,a,��λ,����gps_time,v,a,��λֱ�Ӷ�ȡԭʼֵ
//�������ʱ仯���²�ֵ
#define KS_STEP 0.015
#define S_STEP 20
void cau_path_from_spline(path& p, int no_points)
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
    double SPLINE_EVERY = 0.1;

    int interval = (int )(INTERPOLATION_INTERVAL/SPLINE_EVERY );   //INTERPOLATION_INTERVAL = 7; SPLINE_EVERY = 0.1 ÿ70����ȡһ����
    if(no_points < 3*interval)
    {
        cout << "ponis num not enough" << endl;
        abort();
    }

    // �ɼ����ȵĿ��Ƶ�
    k=0;
    //�ȼ���һ��
    s.push_back(p.ref_points[0].s);
    x.push_back(p.ref_points[0].position_x);
    y.push_back(p.ref_points[0].position_y);
    k++;
    // �ڲ�������̣����ܳ��ָ�ֵ�������
    //       70
    for(i= interval; i< no_points; i += interval )
    {
        //  if(p.ref_points[i].s > k*INTERPOLATION_INTERVAL
        //     && p.ref_points[i].s > s.back() + 0.5 )
        if(p.ref_points[i].s > s.back() + 0.1)
        {
            s.push_back(p.ref_points[i].s);
            x.push_back(p.ref_points[i].position_x);
            y.push_back(p.ref_points[i].position_y);
            k++;
        }
        double distance = 0;
        double delta_ks=0;
        interval = 1;
        while(distance < S_STEP && (i+interval < no_points) && (delta_ks < KS_STEP))//ȡ���Ϊÿ��3mȡһ����
        {
            distance = length_two_points(p.ref_points[i+interval].position_x, p.ref_points[i+interval].position_y,
                                         p.ref_points[i].position_x, p.ref_points[i].position_y);
            delta_ks = fabs(p.ref_points[i].k_s - p.ref_points[i+interval].k_s);
            interval++;
        }
    }
    if(s.size() < 2)
    {
        cout << "spline points not enough" << endl;
        abort();
    }

    //��̵Ĳ�ֵ����
    s_x1.set_points(s,x);
    s_y1.set_points(s,y);

    path path_bak;
    path_bak.ref_points.clear();
    path_bak.reset_path(p.ref_points);//copy some msg to path_bak

    p.ref_points.clear();
    navi_point np;

    cs = 0;
    i = 0;
    while( cs<= s[s.size()-1])
    {
        cs =  i * SPLINE_EVERY;  // 0.1
        Ux_ks  =  CONST_SPEED; // speed;

        if(cs <= 5.5)
        {
            steering_ks = cau_steering_angle_from_ks(s_x1,s_y1,cs,Ux_ks,r,0,8.5);
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
                              8.5);//8.5 STEERING_CAU_TWO_POINT
        }

        steering_ks = iclamp(steering_ks, MIN_STEERING_ANGLE, MAX_STEERING_ANGLE);

        //add by alex 20170607 new way to calc heading
        heading_ks = cau_heading_angle_from_ks(s_x1,s_y1,cs);

        np.s = cs;
        np.point_no = i;
        np.heading = heading_ks;

        np.steering_angle = steering_ks;
        np.position_x = s_x1(cs);
        np.position_y = s_y1(cs);

        //add by alex20170615
        np.pos_gps_time = path_bak.ref_points[i].pos_gps_time;
        np.acceleration_desired_Axs = path_bak.ref_points[i].acceleration_desired_Axs;
        np.speed_desired_Uxs = path_bak.ref_points[i].speed_desired_Uxs;
        np.curvature = path_bak.ref_points[i].curvature;
        np.p_g = path_bak.ref_points[i].p_g;

        np.k_s = -1.0/r;//���ʷ���


        p.ref_points.push_back(np);

        i++;
    }
    path_bak.ref_points.clear();

    int length = p.ref_points.size();
    spline_kp(p, 8, length );

}

// ���㶯̬KPֵ��elvis�����޸ģ��޸�ǰ������ڴ����
void spline_kp(path& p, double speed, int length )
{
    int i;
    std::vector<double> ss, skp; // �����ά����X��Y
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
    if((length < MAP_POINT_LIM_MIN) || (length > MAP_POINT_LIM_MAX)) return; //��ͼ����
    for(i=0; i< length; i ++)
    {
        max_steering = fabs(p.ref_points[i].steering_angle);
        cau_kp = (max_steering)/KP_SLOPE + KP_VALUE;

        if(cau_kp > 10.0)
            cau_kp = 10;

        p.ref_points[i].suggest_kp = cau_kp;
    }

    int every = MARK_EVERY;//70

    for(i=0; i< length; i += every)
    {
        if (i==0)
        {
            ss.push_back(p.ref_points[i].s); // �����ʴ��뵽������
            skp.push_back(p.ref_points[i].suggest_kp);// ���Ƽ�KP���뵽������
        }
        else
        {
            if(p.ref_points[i].s > ss.back() + 0.5)
            {
                ss.push_back(p.ref_points[i].s);
                skp.push_back(p.ref_points[i].suggest_kp);
            }
        }
    }
    // �½�һ�������
    tk::spline skp_st;
    // �����������ϵ�
    skp_st.set_points(ss, skp);// tips

    for(i=0; i< length; i++)
    {
        cs = p.ref_points[i].s;
        cau_kp = skp_st(cs);
        p.ref_points[i].suggest_kp = cau_kp;
    }
}

