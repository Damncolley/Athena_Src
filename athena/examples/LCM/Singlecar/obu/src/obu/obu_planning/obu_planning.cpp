/*-------------------------------------------------------
 * 文件名：obu_planning.cpp
 * 创建者：李思政
 * 时  间：2016-04-03
 * 描  述：obu主进程,用于处理相关网元注册登录,交通信息,路径规划信息
-------------------------------------------------------*/

//头文件
#include "obu_planning.h"
#include "pthread.h"
#include "draw_obu_planning.h"

//进程公共全局变量
obu_planning_starter *g_obu_planning = NULL;

//构造函数
obu_planning_starter::obu_planning_starter() : nad_starter()
{
    obu.route = NULL;
    obu.route_draw[0] = NULL;
    obu.route_draw[1] = NULL;
    //obu.route->map_ = NULL;
}

obu_planning_starter::~obu_planning_starter()
{
    if (obu.route != NULL)//yu
    {
        delete obu.route;
    }

    if (obu.route_draw != NULL)
    {
        delete[] obu.route_draw;
    }
}

void* ThreadFunction_map(void* param)
{
    MyGLDispIni();
    glutMainLoop();
    return NULL;
}

//用于发消息
NE_LCM *g_lcm = NULL;

//自定义启动函数
int obu_planning_starter::user_start()
{
    //初始化lcm
    nad_config *tmp = get_config();
    if (tmp == NULL)
    {
        LOG(ERROR) << "nad_starter::start(): get_config error";
        return RET_ERROR;
    }
    g_lcm = new NE_LCM(get_config()->local.lcm_url);
    if (!g_lcm->good())
    {
        LOG(ERROR) << "nad_starter::start(): g_lcm->good() error";
        return RET_LCM_INIT_ERR;
    }

    //加载静态地图文件
    obu.route = new route::RouteBase(NAD_PATH + NAD_MAP_FILE);
    if (obu.route == NULL)
    {
        LOG(ERROR) << "obu_planning_data::load_data(): obu.route (" << NAD_MAP_FILE << ") return NULL";
    }
    obu.route->transfer_.set_origin(NAD_PATH + NAD_MAP_SET_POINT);
    //pthread_create(&ptht_draw, NULL, &ThreadFunction_map, NULL);

    //启动定时器
    g_ltimer.add_timer(new obu_session_obu_timer(200));

    //设置obu name
    obu.name = get_config()->local.name;

    //注册消息处理句柄
    obu.reg_msg_to_lcm(g_lcm);
    //oct_manager.reg_msg_to_lcm(g_lcm);

    //返回成功
    return RET_OK;
}

//自定义退出函数
void obu_planning_starter::user_stop()
{
    delete g_obu_planning;
    delete g_lcm;
}

//自定义退出函数
void obu_planning_starter::set_timer()
{
    //异步lcm的时间间隔MIN_TIMER_MS=1毫秒
    struct timeval lcm_tv;

    //16-7-12 WFH update select会阻塞并修改tv参数的值,tv设置为0进行实验
    lcm_tv.tv_sec = 0;
    lcm_tv.tv_usec = MIN_TIMER_MS * 1000; //太小的话影响lcm收消息，太大的话无法实现密集触发的定时器
    if (g_lcm == NULL)
    {
        std::cout << "g_lcm is NULL" << std::endl;
        return;
    }
    g_lcm->async_handle(lcm_tv); //消息(网元内+网元间)
}

//主程序
int main(int argc, char *argv[])
{
    glutInit(&argc, argv);
    g_obu_planning = new obu_planning_starter();

    g_obu_planning->start(argc, argv);

    g_obu_planning->stop();
    return 0;
}
