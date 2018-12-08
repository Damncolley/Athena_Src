#include <vector>
//#include "common/origin_vehicle.h"
#include "common/enum_list.h"
#include "map_matching/map_matching.h"
#include "vehicle_dynamic/steering_angle.h"
#include "vehicle_dynamic/heading.h"
//#include "planning/planning_param.h"
#include "config.h"

using namespace std;

map_matching::map_matching()
{
    init();
}

map_matching::~map_matching()
{

}

void map_matching::init()
{
    last_match_point_no = -1;
    current_match_point_no = 0;
    next_match_point_no=0;
//    last_path_num = 0;
}

int map_matching::get_current_match_point_no()
{
    return current_match_point_no;
}

void map_matching::set_current_match_point_no(int current_no)
{
    current_match_point_no = current_no;
}

int map_matching::get_last_match_point_no()
{
    return last_match_point_no;
}

void map_matching::set_last_match_point_no(int last_no)
{
    last_match_point_no = last_no;
}

//前向搜索一定的距离， 但需要注意一定要不能出界。
int map_matching::find_next_moition_planning_points(path p, double length)
{
    double s = 0;
//    double x, y, x_c, y_c;

    if(p.ref_points.size() <=0)
        return -1;

    //保证末端不出界
    if( current_match_point_no + length/SPLINE_EVERY > p.ref_points.size())
        return -1;

    int match_point = current_match_point_no;
    double start_s = p.ref_points[match_point].s;

    while( s < length )
    {
        s = p.ref_points[match_point].s - start_s;

        match_point ++;  // for boost the search speed

        if( (match_point - current_match_point_no) >= 1000 )
            break;

        if( match_point >= p.ref_points.size()-1 )
            break;
    }

    return match_point;
}

//从起始点start_pos开始，前向搜索一定的距离， 但需要注意一定要不能出界。
int map_matching::find_next_moition_planning_points(path p, int start_pos, double length)
{
    double s = 0;
//    double x, y, x_c, y_c;

    if(p.ref_points.size() <=0)
        return -1;

    //保证末端不出界


    double start_s = p.ref_points[start_pos].s;
    int match_pos = start_pos + 1;

    while( s < length )
    {
        s = p.ref_points[ match_pos ].s - start_s;

        match_pos ++;  // for boost the search speed

        if( match_pos >= p.ref_points.size()-1 )
            break;
    }

    return match_pos;
}

// 往前面搜索， 用减法。
int map_matching::find_moition_planning_before_points(path p, double length)
{
    double s = 0;
//    double x, y, x_c, y_c;

    if(p.ref_points.size()<=0)
        return 0;

    if(p.ref_points.size()-1000 <=0)
        return 0;

    //保证起点不出界
    if( current_match_point_no - length/SPLINE_EVERY > 0)
        return -1;

    int match_point = current_match_point_no;
    double start_s = p.ref_points[match_point].s;

    while( s < length )
    {

        s = p.ref_points[match_point].s - start_s;

        match_point -- ;  // for boost the search speed

        if(match_point <= 0)
           break;

        if( abs(match_point - current_match_point_no) > 1000 )
           break;
    }

    return match_point;
}

//往后面搜索。 用加法。
int map_matching::find_moition_planning_start_points(path p, double length)
{
    int match_point = current_match_point_no;

    match_point = find_next_moition_planning_points(p, length);

    return match_point;
}

///zp20171018:用於匹配停車點
int map_matching::MapMarch_Min_Distance_motion_planning(
    double Current_X,
    double Current_Y,
    double Current_heading,  //当前的头指向
    path& p,
    int length,
    double& min_error,
    double length_s)//length_s: 局部搜索长度一般赋值 15m
                      //min_error: 搜索峰值限定 10m
{
    if(p.ref_points.size() <= 0)  return -1;  // -1 代表匹配出错
    //last_match_point_no = max(0,last_match_point_no - 100);//考虑到到车的情况，因此可以向后面稍微多搜一些。如果不需要可以关闭。

    double x, y, Temp_X, Temp_Y, Temp_heading;
    int jishu = 0;//last_match_point_no;
    int index=0;
    int match_point_no = -1;

    double length_min=MAXIMUM_VALUE;
    double length_C=MAXIMUM_VALUE;
    double length_value = MAXIMUM_VALUE;
    double min_error_value = MAXIMUM_VALUE;
    double head_dif_weight = 0;
    //min_error = 1;  //搜索峰值限定 10m

    x = Current_X;
    y = Current_Y;
    //
    bool local_matching_flag = false;//进入局部搜索的时候置为true

    // 粗搜索定位大概位置, 搜索步长为10
    for (  ;jishu < p.ref_points.size();  )
    {
        // 局部搜索
        Temp_X = (p.ref_points[jishu]).position_x;
        Temp_Y = (p.ref_points[jishu]).position_y;
        Temp_heading = p.ref_points[jishu].heading;

        //欧氏距离
        length_C = (Temp_X - x)*(Temp_X - x)+(Temp_Y - y)*(Temp_Y - y);
        length_C = sqrt(length_C);
        //length_C = calculate_sign_and_value_offset( x, y, Current_heading, Temp_X, Temp_Y, Temp_heading );
        length_value = length_C;

        ///zp20171024:匹配停车点时考虑头指向的影响
        //方向权值
        if(Current_heading < 0 || Current_heading > 360 )
        {
            head_dif_weight = 0;
        }
        else
        {
            head_dif_weight = fabs(10 * cau_delta_heading(Temp_heading,Current_heading)/ 180);
        }
        //加权距离
        length_C += head_dif_weight;

        //更新距离最近的值和点号
        if (length_C < length_min  )
        {
            length_min = length_C;
            min_error_value = length_value;
            index = jishu;
        }

        //设置局部搜索范围 //精细搜索
        if(local_matching_flag == false && length_C < length_s)
        {
            local_matching_flag = true;
        }

        if(local_matching_flag == true) //进入局部搜索范围内,开始精细搜索
        {
            if(length_min < min_error)
            {
                last_match_point_no = index;
                match_point_no = index;
            }
           //搜索远离局部搜索峰值.//满足退出搜索要求
           if(length_C > length_s && length_min < min_error)
           {
//               last_match_point_no = index;
//               match_point_no = index;
               break;
           }

           //局部最优,在误差限值只外,进入下一个局部最优
           if(length_C > length_s && length_min > min_error)
           {
              local_matching_flag = false;
           }
           jishu ++; // 精搜索步长
           continue;
        }

        jishu += 10; // 粗搜索步长

    }

    cout << "11111111111length_min = " << length_min << ", " << length_C << ", " << min_error << endl;
    // 误差小于阀值
    if(length_min < min_error)
    {
       min_error = min_error_value;
       return match_point_no;
    }
    else
    {
        current_match_point_no = -1;
        min_error = min_error_value;
        return -1;
    }


}

///zp20171026:点匹配边界线
int map_matching::MapMarch_Min_Distance_motion_planning(
    double Current_X,
    double Current_Y,
    line_xys& edge_line,
    int st_pos,
    int en_pos,
    double& min_error,
    double length_s)//length_s: 局部搜索长度一般赋值 15m
                      //min_error: 搜索峰值限定 10m
{
    if(edge_line.line.size() <= 0)  return -1;  // -1 代表匹配出错
    //last_match_point_no = max(0,last_match_point_no - 100);//考虑到到车的情况，因此可以向后面稍微多搜一些。如果不需要可以关闭。

    double x, y, Temp_X, Temp_Y;// Temp_heading;
    int jishu = 0;//last_match_point_no;
    int index=0;
    int match_point_no = -1;

    double length_min=MAXIMUM_VALUE;
    double length_C=MAXIMUM_VALUE;
    double length_value = MAXIMUM_VALUE;
    double min_error_value = MAXIMUM_VALUE;
//    double head_dif_weight = 0;
    min_error = 15;  //搜索峰值限定 15m

    x = Current_X;
    y = Current_Y;
    //
    bool local_matching_flag = false;//进入局部搜索的时候置为true

    // 粗搜索定位大概位置, 搜索步长为10
    for ( jishu = st_pos; jishu < en_pos - 10; )    ///zp20171109:搜索步长增大到10，加快搜索速度
    {
        // 局部搜索
        Temp_X = (edge_line.line[jishu]).x;
        Temp_Y = (edge_line.line[jishu]).y;

        //欧氏距离
        length_C = (Temp_X - x)*(Temp_X - x)+(Temp_Y - y)*(Temp_Y - y);
        length_C = sqrt(length_C);
        //length_C = calculate_sign_and_value_offset( x, y, Current_heading, Temp_X, Temp_Y, Temp_heading );
        length_value = length_C;

        //更新距离最近的值和点号
        if (length_C < length_min  )
        {
            length_min = length_C;
            min_error_value = length_value;
            index = jishu;
        }

        //设置局部搜索范围 //精细搜索
        if(local_matching_flag == false && length_C < length_s)
        {
            local_matching_flag = true;
        }

        if(local_matching_flag == true) //进入局部搜索范围内,开始精细搜索
        {
           //搜索远离局部搜索峰值.//满足退出搜索要求
           if(length_C > length_s && length_min < min_error)
           {
               last_match_point_no = index;
               match_point_no = index;
               break;
           }

           //局部最优,在误差限值只外,进入下一个局部最优
           if(length_C > length_s && length_min > min_error)
           {
              local_matching_flag = false;
           }
           jishu ++; // 精搜索步长
           continue;
        }

        ///zp20171109:搜索步长增大到10
        jishu += 10; // 粗搜索步长

    }

    // 误差小于阀值
    if(length_min < min_error)
    {
       min_error = min_error_value;
       return match_point_no;
    }
    else
    {
        current_match_point_no = -1;
        min_error = min_error_value;
        return -1;
    }


}

///zp20171109:点匹配center_insert
int map_matching::MapMarch_Min_Distance_motion_planning(
    double Current_X,
    double Current_Y,
    path& center_insert,
    int st_pos,
    int en_pos,
    double& min_error,
    double length_s)//length_s: 局部搜索长度一般赋值 15m
                      //min_error: 搜索峰值限定 10m
{
    if(center_insert.ref_points.size() <= 0)  return -1;  // -1 代表匹配出错
    //last_match_point_no = max(0,last_match_point_no - 100);//考虑到到车的情况，因此可以向后面稍微多搜一些。如果不需要可以关闭。

    double x, y, Temp_X, Temp_Y;// Temp_heading;
    int jishu = 0;//last_match_point_no;
    int index=0;
    int match_point_no = -1;

    double length_min=MAXIMUM_VALUE;
    double length_C=MAXIMUM_VALUE;
    double length_value = MAXIMUM_VALUE;
    double min_error_value = MAXIMUM_VALUE;
//    double head_dif_weight = 0;
    min_error = 10;  //搜索峰值限定 10m

    x = Current_X;
    y = Current_Y;
    //
    bool local_matching_flag = false;//进入局部搜索的时候置为true

    // 粗搜索定位大概位置, 搜索步长为10
    for ( jishu = st_pos; jishu < en_pos - 10; )    ///zp20171109:搜索步长增大到10，加快搜索速度
    {
        // 局部搜索
        Temp_X = (center_insert.ref_points[jishu]).position_x;
        Temp_Y = (center_insert.ref_points[jishu]).position_y;

        //欧氏距离
        length_C = (Temp_X - x)*(Temp_X - x)+(Temp_Y - y)*(Temp_Y - y);
        length_C = sqrt(length_C);
        //length_C = calculate_sign_and_value_offset( x, y, Current_heading, Temp_X, Temp_Y, Temp_heading );
        length_value = length_C;

        //更新距离最近的值和点号
        if (length_C < length_min  )
        {
            length_min = length_C;
            min_error_value = length_value;
            index = jishu;
        }

        //设置局部搜索范围 //精细搜索
        if(local_matching_flag == false && length_C < length_s)
        {
            local_matching_flag = true;
        }

        if(local_matching_flag == true) //进入局部搜索范围内,开始精细搜索
        {
           //搜索远离局部搜索峰值.//满足退出搜索要求
           if(length_C > length_s && length_min < min_error)
           {
               last_match_point_no = index;
               match_point_no = index;
               break;
           }

           //局部最优,在误差限值只外,进入下一个局部最优
           if(length_C > length_s && length_min > min_error)
           {
              local_matching_flag = false;
           }
           jishu ++; // 精搜索步长
           continue;
        }

        ///zp20171109:搜索步长增大到10
        jishu += 10; // 粗搜索步长

    }

    // 误差小于阀值
    if(length_min < min_error)
    {
       min_error = min_error_value;
       return match_point_no;
    }
    else
    {
        current_match_point_no = -1;
        min_error = min_error_value;
        return -1;
    }


}

//****min_error:点离轨迹点最小距离;  min_length:考虑方向后点代价值
int map_matching::MapMarch_Min_Distance_motion_planning(
    CarState car_state,
    path& p,
    bool is_map_updated,
    double length_s,    //length_s: 局部搜索长度一般赋值 15m
    int num_before,
    double& min_error,
    int cur_pos ) //min_error: 搜索峰值限定 10m
{
    int path_num = p.ref_points.size();

    if(path_num <= 0)  return -1;  // -1 代表匹配出错

    //if ( last_path_num != path_num )
//    cout << "00000000 is_map_updated = " << is_map_updated << ", last_match_point_no = " << last_match_point_no << endl;
    last_match_point_no = cur_pos;
    if( is_map_updated )
    {
        last_match_point_no = 0;
    }
    else
        last_match_point_no = max(0,last_match_point_no - num_before);//考虑到到车的情况，因此可以向后面稍微多搜一些。如果不需要可以关闭。

//    cout << "oooooooooooo last_match_point_no = " << last_match_point_no << ", num_before = " << num_before << endl;
    //last_path_num = path_num;

    double x, y, heading, Temp_X, Temp_Y, Temp_heading;
    int jishu = last_match_point_no;
    int index=0;

    double length_min=MAXIMUM_VALUE;
    double length_C=MAXIMUM_VALUE;
    double length_value = MAXIMUM_VALUE;
    double min_error_value = MAXIMUM_VALUE;
    double head_dif_weight = 0;
    min_error = 10;  //搜索峰值限定 10m
    double min_length = MAXIMUM_VALUE;

    x = car_state.car_pose_.CurrentX_;
    y = car_state.car_pose_.CurrentY_;
    heading = car_state.car_pose_.Current_heading_;
    //
    bool local_matching_flag = false;//进入局部搜索的时候置为true

    // 粗搜索定位大概位置, 搜索步长为10
    for (  ;jishu + 1 < p.ref_points.size();  )
    {
        // 局部搜索
        Temp_X = (p.ref_points[jishu]).position_x;
        Temp_Y = (p.ref_points[jishu]).position_y;
        Temp_heading = p.ref_points[jishu].heading;

        //欧氏距离
        length_C = (Temp_X - x)*(Temp_X - x)+(Temp_Y - y)*(Temp_Y - y);
        length_C = sqrt(length_C);
        //length_C = calculate_sign_and_value_offset( x, y, heading, Temp_X, Temp_Y, Temp_heading );
        length_value = length_C;

        //方向权值
        if(heading < 0 || heading > 360 )
        {
            head_dif_weight = 0;
        }
        else
        {
            head_dif_weight = 5*fabs(10 * cau_delta_heading(Temp_heading,heading)/ 180);
        }
       // head_dif_weight = 0;
        //加权距离
        length_C += head_dif_weight;

        //更新距离最近的值和点号
        if (length_C < length_min  )
        {
            length_min = length_C;
            min_error_value = length_value;
            index = jishu;
        }

        //设置局部搜索范围 //精细搜索
        if(local_matching_flag == false && length_C < length_s)
        {
            local_matching_flag = true;
        }

        if(local_matching_flag == true) //进入局部搜索范围内,开始精细搜索
        {
           //搜索远离局部搜索峰值.//满足退出搜索要求
           if(length_C > length_s && length_min < min_error)
           {
               last_match_point_no = index;
               current_match_point_no = index;
               break;
           }

           //局部最优,在误差限值只外,进入下一个局部最优
           if(length_C > length_s && length_min > min_error)
           {
              local_matching_flag = false;
           }
           jishu ++; // 精搜索步长
           continue;
        }

        jishu += 1; // 粗搜索步长

    }

    // 误差小于阀值
    if(length_min < min_error)
    {
       min_error = min_error_value;
       min_length = length_min;
       return current_match_point_no;
    }
    else
    {
        current_match_point_no = -1;
        min_error = min_error_value;
        return -1;
    }


}

//int map_matching::MapMarch_Min_Distance_mapping(
//           double Current_X, double Current_Y,
//           path& p, int length,  double& min_error)
//{
//    double x, y, Temp_X, Temp_Y;
//
//    int jishu=0;
//    int flag=0;
//
//    double length_min=MAXIMUM_VALUE;
//    double length_C=MAXIMUM_VALUE;
//
//    if(p.ref_points.size() <= 0)
//        return -1;  // -1 代表匹配出错
//
//    //考虑到到车的情况，因此可以向后面稍微多搜一些。如果不需要可以关闭。
//    if(last_match_point_no > 100)
//       last_match_point_no -= 100;
//
//    if( last_match_point_no  == -1)
//        jishu = 0;
//    else
//        jishu = last_match_point_no;
//
//    x = Current_X;
//    y = Current_Y;
//
//    // 粗搜索定位大概位置, 搜索步长为10
//    for (  ;
//            (jishu < last_match_point_no + 1000) && ( jishu  < length);
//            jishu ++ )
//    {
//
//        Temp_X = (p.ref_points[jishu]).position_x;
//        Temp_Y = (p.ref_points[jishu]).position_y;
//
//        length_C = (Temp_X - x)*(Temp_X - x)+(Temp_Y - y)*(Temp_Y - y);
//        length_C = sqrt(length_C);
//
//        if (length_min > length_C)
//        {
//            length_min = length_C;
//            flag = jishu;
//        }
//
//    }
//
//    min_error = length_min;
//
//    last_match_point_no = flag;
//    current_match_point_no = flag;
//
//    return current_match_point_no;
//}
int map_matching::MapMarch_Min_Distance_mapping(
           double Current_X, double Current_Y,
           path& p, int length,  double& min_error)
{
    double x, y, Temp_X, Temp_Y;

    int jishu=0;
    int flag=0;

    double length_min=MAXIMUM_VALUE;
    double length_C=MAXIMUM_VALUE;

    if(p.ref_points.size() <= 0)
        return -1;  // -1 代表匹配出错

    x = Current_X;
    y = Current_Y;

    for ( jishu = 0; jishu < length; jishu++ )
    {

        Temp_X = (p.ref_points[jishu]).position_x;
        Temp_Y = (p.ref_points[jishu]).position_y;

        length_C = (Temp_X - x)*(Temp_X - x)+(Temp_Y - y)*(Temp_Y - y);
        length_C = sqrt(length_C);

        if (length_min > length_C)
        {
            length_min = length_C;
            flag = jishu;
        }

    }

    min_error = length_min;

    current_match_point_no = flag;

    return current_match_point_no;
}

int map_matching::set_b_map_update()
{
    b_map_update = true;

    return 1;
}

