/*-------------------------------------------------------
 * 文件名：nad_starter.h
 * 时  间：2016-03-02
 * 描  述：所有进程的公共启动器基类
-------------------------------------------------------*/
#ifndef _NAD_STARTER_H
#define _NAD_STARTER_H

#include "config/nad_config.h"

//每个进程公共的启动程序
class nad_starter
{
private:

public:
    nad_config *m_config; //配置文件

public:
    //构造析构函数
    nad_starter();
    virtual ~nad_starter();

    //设置退出信号陷阱
    void set_grace_stop();

    //退出进程
    void do_grace_stop();

    //处理参数
    virtual void do_arg(int argc, char *argv[]);

    //显示一些启动信息
    void do_show();

    //配置初始化
    int config_init();

    //退出配置
    void config_free();

    //获取配置
    nad_config* get_config();

    /**
     * 在start()中: 执行系统初始化，调用user_start()，然后进入系统运行期的死循环
     * 在stop()中: 调用user_stop()，执行系统的退出操作，然后退出程序
     * 所以用户进程通常需要:
     * 1.定义自己的启动器类
     *   class obu_planning_starter : public nad_start
     *   {
     *       int user_start() {...}
     *       void user_stop() {...}
     *   }
     * 2.进程的main函数通常是这样:
     *   int main()
     *   {
     *       nad_starter *starter = new obu_planning_starter();
     *       starter->start(); //程序启动后在此函数内死循环运行
     *       starter->stop();
     *   }
     */
    virtual int start(int argc, char *argv[]);
    virtual void stop();

public:
    //需要进程自己实现的函数
    virtual int user_start() = 0;  //启动进程私有处理
    virtual void user_stop() = 0;  //退出进程私有处理

     //同步时间
    virtual void set_timer() = 0;
};

//收到ctrl+c后软退出系统
extern int grace_stop;


#endif
