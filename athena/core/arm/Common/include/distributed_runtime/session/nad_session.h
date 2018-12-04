/*-------------------------------------------------------
 * 文件名：nad_session.h
 * 时  间：2016-03-02
 * 描  述：SESSION(控制块)的基类
-------------------------------------------------------*/
#ifndef _NAD_SESSION_H
#define _NAD_SESSION_H

#include "../timer/nad_timer.h"

//SESSION(控制块)的基类
class nad_session
{
public:
    //对象名称
    string name;

public:
    //构造析构函数
    nad_session(string name);
    virtual ~nad_session();

    //处理定时器
    virtual void handle_timer();
};

//SESSION(控制块)管理器的基类
class nad_session_list
{
public:
    //SESSION(控制块)列表
    map<string, nad_session*> session_map;
    map<int,nad_session*> session_map1;

    //最大session数量
    int max_size;

    //定时器
    nad_timer *timer;

public:
    //构造析构函数，参数(最大session数&0=无限, 定时器间隔&0=不起定时器)
    nad_session_list(int max_size, int64 interval_ms);
    virtual ~nad_session_list();

    //获得session数量，禁止重载
    int size();

    //新增SESSION(控制块)，session->name不能重复!
    virtual int add_session(nad_session *session);

    virtual int add_session(nad_session *session,int index);

    //查询SESSION(控制块)，name不能重复!
    virtual nad_session *find_session(string name);

    //删除SESSION(控制块)，成功返回ERT_OK
    virtual int delete_session(string name);

    //删除所有SESSION(控制块)
    virtual void clear_session();

    //处理定时器，通常不用重载
    virtual void handle_timer();
};

#endif
