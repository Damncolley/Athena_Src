/*-------------------------------------------------------
 * 文件名：nad_ui_alarm.cpp
 * 创建者：张毅00151602
 * 时  间：2016-03-02
 * 描  述：向OCT和VUI发告警
-------------------------------------------------------*/

//头文件
#include "nad_ui_alarm.h"


#if defined(_NAD_CSU_) || defined(_NAD_RSU_) || defined(_NAD_OBU_)

//全局告警数量抑制
static map<string, int64_t> g_alarm_list;

//判断是不是重复告警
static bool alarm_too_busy(string mask, int64_t ttl)
{
    //ttl==0表示不过滤告警
    if (ttl == 0)
    {
        return false;
    }

    //查找上次的告警
    int64_t now = get_current_time();
    map<string, int64_t>::iterator it;
    it = g_alarm_list.find(mask);
    if (it != g_alarm_list.end() && now < (it->second + ttl))
    {
        return true;
    }

    //保存当前告警
    g_alarm_list[mask] = now;
    return false;
}

//基本参数的赋值
#define ALARM_SET_PARA(msg)                  \
    msg.alarm_proc = PROC_TYPE;            \
    msg.alarm_time = current_datetime_str(); \
    msg.alarm_level = alarm_level;           \
    msg.alarm_type = ALARM_TYPE_OCCUR;       \
    msg.alarm_no = 0;                        \
    msg.alarm_show = ALARM_SHOW_TEXT_SOUND;  \
    msg.alarm = alarm;                       \
    if (mask == "") mask = alarm;

#endif

#ifdef _NAD_CSU_

//向oct发告警，封装了cu_alarm_report
void alarm_report_to_oct(string oct_name, int32_t alarm_level, string alarm, string mask, int64_t ttl)
{
    ne_msg_t<cu_alarm_report> msg(oct_name, "cu_alarm_report");
    ALARM_SET_PARA(msg.body);
    if (alarm_too_busy(mask, ttl)) return;
    //g_lcm->publish_nemsg(msg);
    g_csu_zmq->send_nemsg<cu_alarm_report>(msg);
    LOG_SEND(log_cu_alarm_report(&msg));
    LOG(INFO) << "alarm_report_to_oct(" << oct_name << "): " << alarm;
}

#endif

#ifdef _NAD_RSU_

//向oct发告警，封装了rc_alarm_report，返回自动分配的alarm_no
void alarm_report_to_oct(int32_t alarm_level, string alarm, string mask, int64_t ttl)
{

    ne_msg_t<rc_alarm_report> msg(g_config.csu.name, "rc_alarm_report");
    ALARM_SET_PARA(msg.body);
    if (alarm_too_busy(mask + "/oct", ttl)) return;
    //g_lcm->publish_nemsg(msg);
    g_rsu_zmq->send_nemsg<rc_alarm_report>(msg);
    LOG_SEND(log_rc_alarm_report(&msg));
    LOG(INFO) << "alarm_report_to_oct: " << alarm;
}

//向vui发告警，封装了ro_alarm_report，返回自动分配的alarm_no
void alarm_report_to_vui(string obu_name, int32_t alarm_level, string alarm, string mask, int64_t ttl)
{
    ne_msg_t<ro_alarm_report> msg(obu_name, "ro_alarm_report");
    ALARM_SET_PARA(msg.body);
    if (alarm_too_busy(mask + "/" + obu_name, ttl)) return;
    //g_lcm->publish_nemsg(msg);
    g_rsu_zmq->send_nemsg<ro_alarm_report>(msg);
    LOG_SEND(log_ro_alarm_report(&msg));
    LOG(ERROR) << "alarm_report_to_vui(" << obu_name << "): " << alarm;
}
#endif

#ifdef _NAD_OBU_

//向vui发告警，封装了ou_alarm_report，返回自动分配的alarm_no
void alarm_report_to_vui(int32_t alarm_level, string alarm, string mask, int64_t ttl)
{
    ou_alarm_report msg;
    ALARM_SET_PARA(msg);
    if (alarm_too_busy(mask, ttl)) return;
    g_lcm->publish("ou_alarm_report", &msg);
    LOG_SEND(log_ou_alarm_report(&msg));
    LOG(INFO) << "alarm_report_to_vui: " << alarm;
}

#endif
