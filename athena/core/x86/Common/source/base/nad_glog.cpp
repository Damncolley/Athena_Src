/*-------------------------------------------------------
 * 文件名：nad_glog.cpp
 * 创建者：李思政
 * 时  间：2016-03-03
 * 描  述：日志api的使用
-------------------------------------------------------*/

#include "log/nad_glog.h"
#include "nad_type.h"

//初始化日志
void nad_log_init()
{
    //设置日志文件目录
    FLAGS_log_dir = NAD_PATH + NAD_LOG_DIR;

    //日志文件名格式为 "<program name>.<hostname>.<user name>.log.<severity level>.<date>.<time>.<pid>"
    //此api是设定<program name>
    google::InitGoogleLogging(PROC_TYPE);

    //stderrthreshold:严重性级别在该门限值以上的日志信息除了写入日志文件以外，还要输出到stderr。
    //各严重性级别对应的数值：INFO—0，WARNING—1，ERROR—2，FATAL—3. 默认值为2.
    FLAGS_stderrthreshold = google::WARNING;

    //设置不同等级的日志用不同颜色显示
    FLAGS_colorlogtostderr = true;
}

//关闭日志
void nad_log_free()
{
    //关闭日志
    google::ShutdownGoogleLogging();
}
