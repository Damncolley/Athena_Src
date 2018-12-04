/*-------------------------------------------------------
 * 文件名：obu_planning.h
 * 创建者：李思政
 * 时  间：2016-04-03
 * 描  述：obu主进程,用于处理相关网元注册登录,交通信息,路径规划信息
-------------------------------------------------------*/
#ifndef _OBU_PLANNING_H
#define _OBU_PLANNING_H

//头文件
//#include "obu_session_oct.h"
#include "obu_session_obu.h"
#include "starter/nad_starter.h"

void* ThreadFunction_map(void* param);
//进程启动器
class obu_planning_starter : public nad_starter
{
public:
    obu_session_obu obu;      //管理车辆自身，包含了地图等信息
    //obu_session_oct_manager oct_manager; //管理oct

public:
    pthread_t ptht_draw;

public:
    //构造析构函数
    obu_planning_starter();
    virtual ~obu_planning_starter();

    //自定义启动退出函数
    int user_start();
    void user_stop();

    //同步时间
    void set_timer();
};

//进程公共全局变量
extern obu_planning_starter *g_obu_planning;

//lcm
//extern NE_LCM *g_lcm;

#endif
