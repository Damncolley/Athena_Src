
/**
 * @file    logging.h
 * @author  jiang <jiangchengjie@indrv.cn>
 * @date    2018-07-07
 * @version 1.0.0
 * @par     Copyright(c)
 *          hy
 */
#ifndef COMMON_GET_TIME_H_
#define COMMON_GET_TIME_H_

#include <time.h>
#include <sys/time.h>

/**
* @namespace athena::control
* @brief athena::control
*/
namespace athena{
namespace control{
/**
 * @class GetTime
 * @brief  获取时间.
 */
class GetTime{
public:
    GetTime() = default;
    ~GetTime() = default;

typedef struct{
    int year;
    int month;
    int day;
    int hour;
    int minute;
    int second;
    int  millisecond;
}TimeFormat;

static TimeFormat gps_local_time_;

static void GetGpsCurrentTime(int &year,int &month,int &day,int &hour,int &minute,int &second,int &millisecond);

static void SetGpsCurrentUtcTime(int utc_year,int utc_month,int utc_day,int utc_hour,int utc_minute,int utc_second,int utc_millisecond);

static void GetSystemTime(int &year,int &month,int &day,int &hour,int &minute,int &second,int &millisecond);

};

}//namespace control
}//namespace athena
#endif //COMMON_GET_TIME_H_


