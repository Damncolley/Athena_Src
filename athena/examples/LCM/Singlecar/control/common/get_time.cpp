#include "get_time.h"
#include <iostream>

using namespace std;

namespace athena{
namespace control{


 GetTime::TimeFormat GetTime::gps_local_time_;

 void GetTime::GetSystemTime(int &year,int &month,int &day,int &hour,int &minute,int &second,int &millisecond)
 {
    struct timeval tv;
    struct timezone tz;
    struct tm *t;
                     //将tm时间转换为秒时间

    gettimeofday(&tv, &tz);
    t = localtime(&tv.tv_sec);

    year = 1900+t->tm_year;
    month = 1+t->tm_mon;
    day = t->tm_mday;
    hour = t->tm_hour;
    minute = t->tm_min;
    second = t->tm_sec;
    millisecond = tv.tv_usec/1000;
 }

 void GetTime::GetGpsCurrentTime(int &year,int &month,int &day,int &hour,int &minute,int &second,int &millisecond)
 {
    year = gps_local_time_.year;
    month = gps_local_time_.month;
    day = gps_local_time_.day;
    hour = gps_local_time_.hour;
    minute = gps_local_time_.minute;
    second = gps_local_time_.second;
    millisecond = gps_local_time_.millisecond;
 }

 void GetTime::SetGpsCurrentUtcTime(int utc_year,int utc_month,int utc_day,int utc_hour,int utc_minute,int utc_second,int utc_millisecond)
 {
    struct tm t;
    struct tm *t2;
    t.tm_year= utc_year;
    t.tm_mon  = utc_month;
    t.tm_mday  = utc_day;
    t.tm_hour  = utc_hour;
    t.tm_min = utc_minute;
    t.tm_sec = utc_second;

    time_t t_;

    t_  = mktime(&t);
    t_-=8*3600;

    t2 = localtime(&t_);

    gps_local_time_.year = t2 -> tm_year;
    gps_local_time_.month = t2 -> tm_mon;
    gps_local_time_.day = t2 -> tm_mday;
    gps_local_time_.hour = t2 -> tm_hour;
    gps_local_time_.minute = t2 -> tm_min;
    gps_local_time_.second = t2 -> tm_sec;
    gps_local_time_.millisecond = utc_millisecond;
 }
}
}
