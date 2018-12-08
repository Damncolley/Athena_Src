/*-------------------------------------------------------
 * 文件名：nad_starter.cpp
 * 时  间：2016-03-02
 * 描  述：所有进程的公共启动器基类
-------------------------------------------------------*/


#include "starter/nad_starter.h"
#include "nad_base.h"
//构造函数
nad_starter::nad_starter()
{
}

//析构函数
nad_starter::~nad_starter()
{
}

//收到ctrl+c后软退出系统
int grace_stop = false;

void fam_grace_stop(int sig)
{
    if (sig != -1) LOG(WARNING) << "nad_starter::do_grace_stop(" << sig << ")";
    grace_stop = TRUE;
}

//设置退出信号陷阱
void nad_starter::set_grace_stop()
{
    signal(SIGINT , fam_grace_stop);
    signal(SIGKILL, fam_grace_stop);
    signal(SIGUSR1, fam_grace_stop);
    signal(SIGTERM, fam_grace_stop);
}

//退出进程
void nad_starter::do_grace_stop()
{
	  fam_grace_stop(-1); //人为退出不打印调试信息
}

//处理参数
void nad_starter::do_arg(int argc, char *argv[])
{
    string path = argv[0];
    string name = get_filename(path);

    //设置基础目录
    string base = sbetween(path, NULL, "/src/");
    if (base == "") base = sbetween(path, NULL, "/bin/");
    if (base != "") NAD_PATH = base;

    //设置序列号
    #ifdef _NAD_OBU_PLANNING_
    NAD_LOCAL_NE = sbetween(name, "obu_planning_", NULL);
    #endif
//  #ifdef _NAD_CSU_PLANNING_
//  NAD_LOCAL_NE = sbetween(name, "csu_planning_", NULL);
//  #endif
//  #ifdef _NAD_RSD_COLLECTOR_
//  NAD_LOCAL_NE = sbetween(name, "rsd_collector_", NULL);
//  #endif
    #ifdef _NAD_SIM_VUI_
    NAD_LOCAL_NE = sbetween(name, "sim_vui_", NULL);
    #endif
}

//显示一些启动信息
void nad_starter::do_show()
{
    cout << "NAD_PATH = " << NAD_PATH.c_str() << endl;
    cout << "NAD_LOCAL_NE = " << NAD_LOCAL_NE.c_str() << endl;
}

//初始化配置
int nad_starter::config_init()
{
    m_config = new nad_config();
    if (NULL == m_config)
    {
        cout << "new config error" << endl;
        return RET_ERROR;
    }
    return m_config->load_from_file(NAD_PATH + NAD_CONFIG_FILE);
}

//退出配置
void nad_starter::config_free()
{
    delete m_config;
}

//获取配置
nad_config* nad_starter::get_config()
{
    return m_config;
}

//系统启动函数
int nad_starter::start(int argc, char *argv[])
{
    //系统初始化开始(还未初始化LOG打印)
    cout << "nad_starter::start(): system init begin" << endl;

    //处理参数
    do_arg(argc, argv);

    //初始化日志
    nad_log_init();

    //初始化配置
    config_init();

    //初始化数据库
    nad_db_init();

    //系统初始化完成
    LOG(WARNING) << "nad_starter::start(): system init ok";

    //用户的初始化
    user_start();
   // do_show();
    LOG(WARNING) << "nad_starter::start(): user_start() ok";

    //设置退出信号陷阱
    set_grace_stop();
    LOG(WARNING) << "nad_starter::start(): set_grace_stop() ok";

    //主循环
    while(TRUE)
    {
        set_timer();
        g_ltimer.handle_timer();     //定时器
        if (grace_stop) break;       //收到退出信号
    }

    //返回成功
    return RET_OK;
}

//系统退出函数
void nad_starter::stop()
{
    //系统退出开始
    LOG(WARNING) << "nad_starter::stop(): system free begin";

    //用户的退出
    user_stop();
    LOG(WARNING) << "nad_starter::stop(): user_stop() ok";

    //系统退出开始
    //退出数据库
    nad_db_free();

    //退出配置
    config_free();

    //关闭日志
    nad_log_free();

    //系统退出完成(不能再用LOG打印)
    cout << "nad_starter::stop(): system free ok" << endl;
}
