
#include "logging.h"
#include <glog/logging.h>
namespace athena{
namespace control{
bool Logging::log_enable_ = true;
void Logging::Init(bool log_enable)
{
    log_enable_ = log_enable;
       //级别高于ERROR的才输出到屏幕上
    google::SetStderrLogging(google::WARNING);
    //设置输出到屏幕的日志显示相应颜色
    FLAGS_colorlogtostderr=true;
    //设置 google::INFO 级别的日志存储路径和文件名前缀
    google::SetLogDestination(google::INFO,"log/INFO_");
    //设置 google::WARNING 级别的日志存储路径和文件名前缀
    google::SetLogDestination(google::WARNING,"log/WARNING_");
    //设置 google::ERROR 级别的日志存储路径和文件名前缀
    google::SetLogDestination(google::ERROR,"log/ERROR_");
    //缓冲日志输出，默认为30秒，此处改为立即输出
    FLAGS_logbufsecs =0;
    //最大日志大小为 10MB
    FLAGS_max_log_size =10;
    //当磁盘被写满时，停止日志输出
    FLAGS_stop_logging_if_full_disk = true;
    //捕捉 core dumped
    google::InstallFailureSignalHandler();
}

void Logging::LogInfo(int level,std::string info)
{
    if(log_enable_ == false)
    {
        return;
    }

    switch(level)
    {
    case INFO:
        LOG(INFO) <<info;
        break;
    case WARNING:
        LOG(WARNING) <<info;
        break;
    case ERROR:
        LOG(ERROR) <<info;
        break;
    default:
        break;
    }
}
}
}


