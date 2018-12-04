/*-------------------------------------------------------
 * 文件名：nad_base.h
 * 创建者：张毅00151602
 * 时  间：2016-03-02
 * 描  述：引用了base目录下的所有头文件
-------------------------------------------------------*/
#ifndef _NAD_BASE_H
#define _NAD_BASE_H


//公共数据类型
#include "nad_type.h"

//公共返回码
#include "nad_retcode.h"

//公共枚举值
#include "nad_enum.h"

//公共基础函数
#include "nad_function.h"

//日志模块
#include "log/nad_glog.h"

//XML解析器
#include "xml/pugixml.hpp"
using namespace pugi;

//数据库
#include "db/nad_db.h"

#include "timer/nad_timer.h"

#endif
