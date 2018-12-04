/*-------------------------------------------------------
 * 文件名：nad_ui_alarm.h
 * 创建者：张毅00151602
 * 时  间：2016-03-28
 * 描  述：向OCT和VUI发告警
-------------------------------------------------------*/
#ifndef _NAD_UI_ALARM_H
#define _NAD_UI_ALARM_H

#include "nad_base.h"
#include "../../msg/nad_msg.h"






//默认的告警间隔是10秒
#define ALARM_TTL 10000

#ifdef _NAD_CSU_

#include "../../../csu/csu_planning/csu_zmq.h"

extern csu_zmq *g_csu_zmq;

//向oct发告警，封装了cu_alarm_report，返回自动分配的alarm_no
void alarm_report_to_oct(string oct_name, int32_t alarm_level, string alarm, string mask = "", int64_t ttl = ALARM_TTL);

#endif

#ifdef _NAD_RSU_

#include "../../../rsu/rsu_planning/rsu_zmq.h"

extern rsu_zmq *g_rsu_zmq;

//向oct发告警，封装了rc_alarm_report，返回自动分配的alarm_no
void alarm_report_to_oct(int32_t alarm_level, string alarm, string mask = "", int64_t ttl = ALARM_TTL);

//向vui发告警，封装了ro_alarm_report，返回自动分配的alarm_no
void alarm_report_to_vui(string obu_name, int32_t alarm_level, string alarm, string mask = "", int64_t ttl = ALARM_TTL);

#endif

#ifdef _NAD_OBU_

//向vui发告警，封装了ou_alarm_report，返回自动分配的alarm_no
void alarm_report_to_vui(int32_t alarm_level, string alarm, string mask = "", int64_t ttl = ALARM_TTL);

#endif


#endif
