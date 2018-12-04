/*-------------------------------------------------------
 * 文件名：nad_glog.h
 * 创建者：李思政
 * 时  间：2016-03-03
 * 描  述：日志api包含的头文件
-------------------------------------------------------*/
#ifndef _NAD_GLOG_H
#define _NAD_GLOG_H

//初始化日志
void nad_log_init();

//关闭日志
void nad_log_free();

/**
 * 记日志的接口:
 *   普通日志
 *     LOG(INFO) << "error log";
 *   警告日志
 *     LOG(WARNING) << "error log";
 *   错误日志
 *     LOG(ERROR) << "error log";
 *   致命错误日志(调用后程序自动退出)
 *     LOG(FATAL) << "error log";
 */

#endif
