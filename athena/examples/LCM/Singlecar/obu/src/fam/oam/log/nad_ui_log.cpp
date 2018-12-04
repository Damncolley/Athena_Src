/*-------------------------------------------------------
 * 文件名：nad_ui_log.cpp
 * 创建者：张毅00151602
 * 时  间：2016-03-02
 * 描  述：向OCT和VUI发日志
-------------------------------------------------------*/

//头文件
#include "nad_ui_log.h"
#include "nad_type.h"

#if defined(_NAD_CSU_) || defined(_NAD_RSU_) || defined(_NAD_OBU_)

//基本参数的赋值
#define LOG_SET_PARA(msg)                  \
    msg.log_proc = PROC_TYPE;            \
    msg.log_time = current_datetime_str(); \
    msg.log_level = log_level;             \
    msg.log = log;

#endif

#ifdef _NAD_CSU_

//向oct发日志，封装了cu_log_report
void log_report_to_oct(string oct_name, int32_t log_level, string log)
{
    ne_msg_t<nad_lcm::cu_log_report> msg(oct_name, "cu_log_report");
    LOG_SET_PARA(msg.body);
    //g_lcm->publish_nemsg(msg);
    g_csu_zmq->send_nemsg<cu_log_report>(msg);
    LOG_SEND(log_cu_log_report(&msg));
    LOG(INFO) << "log_report_to_oct(" << oct_name << "): " << log;
}

#endif

#ifdef _NAD_RSU_

//向oct发日志，封装了rc_log_report
void log_report_to_oct(int32_t log_level, string log)
{
    ne_msg_t<nad_lcm::rc_log_report> msg(g_config.csu.name, "rc_log_report");
    LOG_SET_PARA(msg.body);
    //g_lcm->publish_nemsg(msg);
    g_rsu_zmq->send_nemsg<rc_log_report>(msg);
    LOG_SEND(log_rc_log_report(&msg));
    LOG(INFO) << "log_report_to_oct: " << log;
}

//向vui发日志，封装了ro_log_report
void log_report_to_vui(string obu_name, int32_t log_level, string log)
{
    ne_msg_t<nad_lcm::ro_log_report> msg(obu_name, "ro_log_report");
    LOG_SET_PARA(msg.body);
    //g_lcm->publish_nemsg(msg);
    g_rsu_zmq->send_nemsg<ro_log_report>(msg);
    LOG_SEND(log_ro_log_report(&msg));
    LOG(INFO) << "log_report_to_vui: " << log;
}
#endif

#ifdef _NAD_OBU_

//向vui发日志，封装了ou_log_report
void log_report_to_vui(int32_t log_level, string log)
{
    nad_lcm::ou_log_report msg;
    LOG_SET_PARA(msg);
    g_lcm->publish("ou_log_report", &msg);
    LOG_SEND(log_ou_log_report(&msg));
    LOG(INFO) << "log_report_to_vui: " << log;
}

#endif
