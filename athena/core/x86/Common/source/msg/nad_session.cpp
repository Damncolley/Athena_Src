/*-------------------------------------------------------
 * 文件名：nad_session.cpp
 * 时  间：2016-03-02
 * 描  述：SESSION(控制块)的基类
-------------------------------------------------------*/


//头文件
#include "session/nad_session.h"


//构造SESSION(控制块)
nad_session::nad_session(string name)
{
    this->name = name;
}

//析构函数
nad_session::~nad_session()
{
}

//处理定时器
void nad_session::handle_timer()
{
}

//定时器
class nad_session_timer : public nad_timer
{
private:
    //需要定时处理的session列表
    nad_session_list *session_list;

public:
    //构造析构函数
    nad_session_timer(int64 interval_ms, nad_session_list *session_list);
    virtual ~nad_session_timer();

    //执行定时器，传入当前的时间(毫秒)
    void handle();
};

//构造函数
nad_session_timer::nad_session_timer(int64 interval_ms, nad_session_list *session_list) : nad_timer(interval_ms)
{
    this->session_list = session_list;
}

//析构函数
nad_session_timer::~nad_session_timer()
{
}

//执行定时器，传入当前的时间(毫秒)
void nad_session_timer::handle()
{
    session_list->handle_timer();
}


//构造函数，参数(最大session数&0=无限, 定时器间隔&0=不起定时器)
nad_session_list::nad_session_list(int max_size, int64 interval_ms)
{
    this->max_size = (max_size <= 0) ? MAX_I32 : max_size;
    if (interval_ms > 0)
    {
        timer = new nad_session_timer(interval_ms, this);
        g_ltimer.add_timer(timer);
    }
    else
    {
        timer = NULL;
    }
    cout << "nad_session_list::nad_session_list(" << max_size << ", " << interval_ms << ") : no. " << g_ltimer.timer_list.size() << endl;
}

//析构函数
nad_session_list::~nad_session_list()
{
    clear_session();
    if (timer != NULL)
    {
        g_ltimer.delete_timer(timer);
    }
}

//获得session数量，禁止重载
int nad_session_list::size()
{
    return session_map.size();
}

//新增SESSION(控制块)，session->name不能重复!
int nad_session_list::add_session(nad_session *session)
{
    nad_session *result = find_session(session->name);
    if (result == NULL)
    {
        //没找到，则添加
        //cout << "nad_session_list::add_session(" << session->name << "): ok" << endl;
        session_map[session->name] = session;
        return RET_OK;
    }
    else
    {
        //找到
        //cout << "nad_session_list::add_session(" << session->name << "): exists" << endl;
        return RET_EXIST;
    }
}

int nad_session_list::add_session(nad_session *session,int index)
{
    nad_session *result = find_session(session->name);
    if (result == NULL)
    {
        //没找到，则添加
        //cout << "nad_session_list::add_session(" << session->name << "): ok" << endl;
        session_map[session->name] = session;
        session_map1[index] = session;

        return RET_OK;
    }
    else
    {
        //找到
        //cout << "nad_session_list::add_session(" << session->name << "): exists" << endl;
        return RET_EXIST;
    }
}

//查询SESSION(控制块)，name不能重复!
nad_session *nad_session_list::find_session(string name)
{
    map<string, nad_session*>::iterator iter;
    iter = session_map.find(name);
    if (iter == session_map.end())
    {
        return NULL;
    }
    else
    {
        return iter->second;
    }
}

//删除SESSION(控制块)，成功返回ERT_OK
int nad_session_list::delete_session(string name)
{
    map<string, nad_session*>::iterator iter;
    iter = session_map.find(name);
    if (iter == session_map.end())
    {
        cout << "nad_session_list::delete_session(" << name << "): not exists" << endl;
        return RET_NOT_EXIST;
    }
    else
    {
        cout << "nad_session_list::delete_session(" << name << "): ok" << endl;
        delete iter->second;
        session_map.erase(iter);
        return RET_OK;
    }
}

//删除所有SESSION(控制块)
void nad_session_list::clear_session()
{
    map<string, nad_session*>::iterator iter;
    for(iter = session_map.begin(); iter != session_map.end(); iter++)
    {
        delete iter->second;
    }
    session_map.clear();
}

//处理定时器
void nad_session_list::handle_timer()
{
    map<string, nad_session*>::iterator  iter;
    for(iter = session_map.begin(); iter != session_map.end(); iter++)
    {
        nad_session *session = iter->second;
        session->handle_timer();
    }
}
