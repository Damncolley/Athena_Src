/*-------------------------------------------------------
 * 文件名：nad_ui_log.h
 * 创建者：张毅00151602
 * 时  间：2016-03-28
 * 描  述：向OCT和VUI发日志
-------------------------------------------------------*/
#ifndef _NAD_UI_LOG_H
#define _NAD_UI_LOG_H

#include "nad_base.h"
#include "../../msg/nad_msg.h"



#ifdef _NAD_CSU_

#include "../../../csu/csu_planning/csu_zmq.h"

extern csu_zmq *g_csu_zmq;

//向oct发日志，封装了cu_log_report
void log_report_to_oct(string oct_name, int32_t log_level, string log);

#endif

#ifdef _NAD_RSU_

#include "../../../rsu/rsu_planning/rsu_zmq.h"

extern rsu_zmq *g_rsu_zmq;

//向oct发日志，封装了rc_log_report
void log_report_to_oct(int32_t log_level, string log);

//向vui发日志，封装了ro_log_report
void log_report_to_vui(string obu_name, int32_t log_level, string log);

#endif

#ifdef _NAD_OBU_

//向vui发日志，封装了ou_log_report
void log_report_to_vui(int32_t log_level, string log);

#endif


#endif
