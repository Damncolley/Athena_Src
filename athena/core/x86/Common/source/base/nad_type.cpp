//头文件
#include "nad_type.h"
#include "nad_enum.h"
#include "log/nad_glog.h"

#ifdef _NAD_CSU_
    string g_ne_type = "csu";
#endif

#ifdef _NAD_RSU_
    string g_ne_type = "rsu";
#endif

#ifdef _NAD_OBU_
    string g_ne_type = "obu";
#endif

#ifdef _NAD_RSD_
    string g_ne_type = "rsd";
#endif

#ifdef _NAD_SIM
    string g_ne_type = "sim";
#endif


//基础目录，如"/home/zhangyi/code_huashan_phase2_160909v1.1"
string NAD_PATH = "..";

//当前车号或设备，""表示读config.xml的local_obu等，obu仅取尾数部分，如"鄂A V94M0"则填"V94M0"
//目前仅用于obu_planning/obu_planning/rsd_collector
string NAD_LOCAL_NE = "";

