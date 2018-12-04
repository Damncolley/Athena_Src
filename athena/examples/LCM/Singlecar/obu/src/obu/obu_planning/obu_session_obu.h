/*-------------------------------------------------------
 * 文件名：obu_session_obu.h
 * 创建者：李思政
 * 时  间：2016-04-03
 * 描  述：obu管理自身用到的数据结构的定义
-------------------------------------------------------*/
#ifndef _OBU_SESSION_OBU_H
#define _OBU_SESSION_OBU_H

#include <map>


#include "route.h"
#include "oam/alarm/nad_ui_alarm.h"
#include "oam/alarm/nad_warning.h"
#include "oam/log/nad_ui_log.h"
#include "MapInterface.h"
#include "info/nad_speed.h"


//抑制5秒内的重复换道请求
#define MAX_CL_TIMEOUT 5000

//如果头指向差异超过10度，认为是折线
#define MAX_P_YAW 10.0


//获得下一个轨迹点
static void get_next_point(int &i, int e, std::vector<route::center_point> &center_line)
{
    //如果下一个点越界则退出
    i++;
    if (i > e)
    {
        return;
    }

    //计算计算之前的头指向
    double yaw1 = 0.0;
    get_heading(center_line[i - 1].x, center_line[i - 1].y, center_line[i].x, center_line[i].y, yaw1);

    //寻找一个折线点
    for (; i <= e; i++)
    {
        //关键点必须添加
        route::center_point &cp = center_line[i];
        for (int j = 1; j < MAX_KP_TYPE; j++)
        {
            if (cp.type[j] == 1)
            {
                return;
            }
        }

        //曲率大的退出
        double yaw2 = 0.0;
        get_heading(center_line[i].x, center_line[i].y, center_line[i + 1].x, center_line[i + 1].y, yaw2);
        if (fabs(yaw2 - yaw1) > MAX_P_YAW)
        {
            return;
        }
    }
}

class obu_session_obu : public nad_session
{
public:
    //obu的基本参数
    //string name;         //obu的名称，在nad_session中定义过
    string obu_type;       //obu的类型
    int session_status;    //obu的主状态，参考枚举值：E_OBU_SESSION_OBU_XXX
    int start_auto_status; //obu的启动自动驾驶状态，参考枚举值：E_OBU_SESSION_OBU_START_AUTO_XXX
    int flashing_status;   //转向灯状态，参考枚举值FLASHING_STATUS_XXX
    int wait_count;        //用于重发计数，用于oc_rsu_name_request、or_obu_login_request、or_route_request、or_start_auto_request
    int64_t last_htbt;     //用于心跳计数，在收到ro_vui_report时更新心跳时间

    //obu的交通信息
    double cur_lon;                   //当前经度
    double cur_lat;                   //当前纬度
    double cur_yaw;                   //头指向和北方向的夹角，单位：度
    double cur_brake;                 //刹车踏板行程，踩到底取值100.0
    double cur_speed;
    double cur_acceleration;
    double steering_angle;
    int32_t cur_gears;
    int32_t sensor_stat[MAX_SENSOR];
    int32_t platoon_sn;    //在编队中的需要，1=头车，0=不再编队里
    double gps_time;       //上报时间（GPS授时是高精度绝对时间）
    int32_t eps_stat;    //方向盘
    int32_t epb_stat;   //epb  1释放 2使能
    int32_t brake_stat;  //刹车

    double sug_speed;
    int32_t right_of_way;
    //路径规划
    route::RouteBase* route; //路径规划结果
    route::RouteBase* route_draw[2];//双buffer  route
    bool route_draw_flag = true;

    //路径规划状态
    bool route_plan_status;

    //速度控制
    nad_speed speed;

    //自动驾驶开关
    bool is_auto_drive;

    //obu召车、还车的状态信息
    int call_park_state;  //当前状态：0 可召车，1 可还车，2 召车中，3 还车中（0,1状态不需要填写下面的信息）

    //换道抑制，每类型&同方向的换道5秒才可重发一次
    map<string, int64_t> cl_list;

    //抑制，避障换道5秒才可重发一次 ：在左侧车道往左换道，在右侧车道往右换道
    map<int32_t, int64_t> cl_avoid_list;

    //在完整切片列表中选取当前obu前1000,后100米的所有切片
    void send_segment_section_line(nad_lcm::route_planning_m route,nad_lcm::route_planning_m &other);

    //给vui下发路径规划结果，也可用于主动下发路径
    void send_ou_route_respond(int ret,nad_lcm::route_planning route);

    //给vui下发启动自动驾驶结果，也可用于主动下发启动自动驾驶
    void send_ou_start_auto_respond(int32_t retcode, int32_t start_reason);

    //给vui下发退出自动驾驶结果，也可用于主动下发退出自动驾驶
    void send_ou_stop_auto_respond(int32_t retcode, int32_t stop_reason);

    //保存到消息
    void save_to_lcm(nad_lcm::route_planning &other);

    //保存到消息
    void save_to_lcm(nad_lcm::obu_info &other);

    //保存到消息中
    void save_to_lcm(nad_lcm::center_point &cp);

    //抑制5秒内的重复换道请求
    bool cl_too_busy(int32_t direction, int32_t cl_reason);

    //抑制5秒内的避障
    bool cl_too_busy_avoiding(int32_t cl_reason);

    //内部调用的换道执行操作
    //check_busy=拒绝5秒内的同类型、同方向换道请求
    //check_target=拒绝目标车道不存在的换道请求
    //check_follow=拒绝编队跟车的换道请求
    //check_forbid=拒绝"禁止换道"标志牌范围内的换道请求
    //check_platoon=编队头车动作时，跟车生成换道
    //check_cooperate=目标车道存在OBU车辆时，进入协作换道操作
    int change_lane_ex(int32_t direction, int32_t cl_reason, int starting_lane, int ending_lane,
        bool need_ack, bool check_busy, bool check_target, bool check_follow,
        bool check_forbid, bool check_platoon, bool check_cooperate);

    int send_change_lane_respond(nad_lcm::om_change_lane_respond &om_change_lane_respond_msg,bool need_ack, int ret, string alarm);

    void send_ou_vui_report();

    void save_route_kp_to_lcm(int16_t &num_of_key, std::vector<nad_lcm::key_point_info> &key_point,
    int16_t &cur_point_index, int16_t &num_of_point, std::vector<nad_lcm::route_line_point> &line);
public:
    //构造析构函数
    obu_session_obu();
    virtual ~obu_session_obu();

    //处理定时器
    void handle_timer(int64_t counter);

    //注册消息到lcm的函数
    void reg_msg_to_lcm(NE_LCM* lcm);

    //接收vui上传的uo_route_request
    void handle_uo_route_request(const lcm::ReceiveBuffer* rbuf,
        const std::string& chan, const nad_lcm::uo_route_request *msg);

    //接收vui上传的uo_start_auto_request
    void handle_uo_start_auto_request(const lcm::ReceiveBuffer* rbuf,
        const std::string& chan, const nad_lcm::uo_start_auto_request *msg);

    //接收vui上报的uo_stop_auto_request
    void handle_uo_stop_auto_request(const lcm::ReceiveBuffer* rbuf,
        const std::string& chan, const nad_lcm::uo_stop_auto_request *msg);

    //读取control_info_report
    void handle_control_info_report(const lcm::ReceiveBuffer* rbuf,
        const std::string& chan, const obu_lcm::control_info_report *msg);

    //motion_planner请求换道
    void handle_mo_change_lane_request(const lcm::ReceiveBuffer* rbuf,
        const std::string& chan, const nad_lcm::mo_change_lane_request *msg);

    void send_om_section_line_report(int replay_flag);

    void calc_mileage_section_acc();

    void route_plan_section_clear(nad_lcm::route_planning_m &route_sec);

    void planning_m_2_lcm(route::route_planning_m & planning_m, nad_lcm::route_planning_m &planning_m_lcm);

    void lcm_2_planning_m(nad_lcm::route_planning_m & planning_m_lcm, route::route_planning_m &planning_m);

    //设置默认速度
    void set_default_speed();

    //速度处理
    void handle_speed();
};

//obu_session_obu的定时器
class obu_session_obu_timer : public nad_timer
{
public:
    //构造析构函数
    obu_session_obu_timer();
    obu_session_obu_timer(int64 interval_ms);
    virtual ~obu_session_obu_timer();

    //执行定时器
    void handle();
};


#endif
