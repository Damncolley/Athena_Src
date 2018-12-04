/*-------------------------------------------------------
 * 文件名：nad_type.h
 * 创建者：张毅00151602
 * 时  间：2016-03-02
 * 描  述：公共头文件和数据类型定义
-------------------------------------------------------*/
#ifndef _NAD_TYPE_H
#define _NAD_TYPE_H


/*-------------------------------------------------------
 * Linux操作系统的头文件
-------------------------------------------------------*/

//linux的常用头文件
#include <arpa/inet.h>
#include <assert.h>
#include <ctype.h>
#include <dirent.h>
#include <dlfcn.h>
#include <errno.h>
#include <fcntl.h>
#include <glob.h>
#include <math.h>
#include <netdb.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <pthread.h>
#include <stdarg.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/epoll.h>
#include <sys/ioctl.h>
#include <sys/ipc.h>
#include <sys/mman.h>
#include <sys/mount.h>
#include <sys/resource.h>
#include <sys/sem.h>
#include <sys/select.h>
#include <sys/shm.h>
#include <signal.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/statfs.h>
#include <sys/syscall.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/un.h>
#include <time.h>
#include <unistd.h>
#include <net/if.h>
#include <iostream>

//支持stl
#include <algorithm>
#include <deque>
#include <functional>
#include <iterator>
#include <vector>
#include <string>
#include <list>
#include <map>
#include <memory>
#include <numeric>
#include <queue>
#include <set>
#include <stack>
#include <utility>
#include <fstream>
#include <regex>
using namespace std;

//支持boost
#include <boost/filesystem.hpp>
#include <boost/tokenizer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>

//使用glog应该包含的头文件
#include <glog/logging.h>
#include <glog/raw_logging.h>  //支持线程安全

#include "nad_enum.h"

//统一定义数据结构
typedef unsigned char      uchar;
typedef unsigned short     ushort;
typedef unsigned int       uint;
typedef long long          int64;
typedef unsigned long long uint64;

//定义布尔值
typedef int boolean;
#define TRUE  1
#define FALSE 0

//各种数据类型的最大最小值
#define MAX_I8     127
#define MIN_I8     (-128)
#define MAX_I16    32767
#define MIN_I16    (-32768)
#define MAX_I32    2147483647L
#define MIN_I32    (-MAX_I32 - 1L)
#define MAX_I64    9223372036854775807LL
#define MIN_I64    (-MAX_I64 - 1LL)
#define MAX_U8     255
#define MIN_U8     0
#define MAX_U16    65535
#define MIN_U16    0
#define MAX_U32    4294967295UL
#define MIN_U32    0
#define MAX_U64    18446744073709551615ULL
#define MIN_U64    0

//常用大小定义
#define _TB_           (1024LL * 1024 * 1024 * 1024) /* 1TB */
#define _GB_           (1024 * 1024 * 1024)          /* 1GB */
#define _MB_           (1024 * 1024)                 /* 1MB */
#define _KB_           (1024)                        /* 1KB */
#define BUF_SIZE_8     8
#define BUF_SIZE_16    16
#define BUF_SIZE_20    20
#define BUF_SIZE_32    32
#define BUF_SIZE_64    64
#define BUF_SIZE_128   128
#define BUF_SIZE_256   256
#define BUF_SIZE_512   512
#define BUF_SIZE_1024  1024
#define BUF_SIZE_4K    (4  * _KB_)
#define BUF_SIZE_8K    (8  * _KB_)
#define BUF_SIZE_16K   (16 * _KB_)
#define BUF_SIZE_32K   (32 * _KB_)
#define BUF_SIZE_64K   (64 * _KB_)
#define BUF_SIZE_1M    (1  * _MB_)
#define BUF_SIZE_2M    (2  * _MB_)
#define BUF_SIZE_4M    (4  * _MB_)

//圆周率定义
#define PI 3.14159265358979323846264338

#ifdef _NAD_CSU_PLANNING_
    #define PROC_TYPE  "csu_planning"
#endif

#ifdef _NAD_RSU_PLANNING_
    #define PROC_TYPE  "rsu_planning"
#endif

#ifdef _NAD_OBU_PLANNING_
    #define PROC_TYPE  "obu_planning"
#endif

#ifdef _NAD_RSD_COLLECTOR_
    #define PROC_TYPE  "rsd_collector"
#endif

#ifdef _NAD_SIM_SYSTEM_
    #define PROC_TYPE  "sim_system"
#endif

#ifdef _NAD_SIM_VUI_
    #define PROC_TYPE  "sim_vui"
#endif

#ifndef PROC_TYPE
#define PROC_TYPE  "obu_planning"
#endif


/*-------------------------------------------------------
 * 进程的名字宏
-------------------------------------------------------*/

//取值如"csu"/"rsu"/"obu"
extern string g_ne_type;


/*-------------------------------------------------------
 * 系统关键文件位置
-------------------------------------------------------*/

//基础目录，如"/home/zhangyi/code_huashan_phase2_160909v1.1"
extern string NAD_PATH;

//当前车号或设备，""表示读config.xml的local_obu等，obu仅取尾数部分，如"鄂A V94M0"则填"V94M0"
//目前仅用于obu_planning/obu_planning/rsd_collector
extern string NAD_LOCAL_NE;

//配置文件
#define NAD_CONFIG_FILE "/conf/config.xml"

//地图原点
#define NAD_MAP_SET_POINT "/conf/origin.route"

//数据库存盘文件(动态数据)
#define NAD_DB_FILE "/conf/db/db.xml"

//数据库存盘文件(静态数据)
#define NAD_DB_TYPE_FILE "/conf/db/db_type.xml"

//地图文件
#define NAD_MAP_FILE "/map/nad.osm"

//文件目录
#define NAD_TASK_DIR "/conf/task/"

//日志文件目录
#define NAD_LOG_DIR "/log/"

/*-------------------------------------------------------
 * 系统关键参数
-------------------------------------------------------*/

//最小定时器间隔，1毫秒，太小的话影响lcm收消息，太大的话无法实现密集触发的定时器
#define MIN_TIMER_MS 1


#endif /*_NAD_TYPE_H*/
