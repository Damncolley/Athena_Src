/*-------------------------------------------------------
 * 文件名：nad_speed.h
 * 时  间：2016-10-05
 * 描  述：管理速度
-------------------------------------------------------*/
#ifndef _NAD_SPEED_H
#define _NAD_SPEED_H
#include "nad_type.h"

//速度优先级
#define SV_LOW   0  //低优先级，强烈建议都用SV_LOW优先级
#define SV_HIGH  1  //高优先级，如果存在SV_HIGH的速度值，则忽略所有SV_LOW

//速度值类型
#define SV_ABSOLUTE  0  //绝对速度值
#define SV_RELATE    1  //相对速度，如-2=速度减2km/h，可叠加

//一个速度值
class nad_speed_value
{
public:
    string name;     //为这个速度值命名
    int priority;    //SV_LOW/V_HIGH，如果存在SV_HIGH的速度值，则忽略所有SV_LOW
    int type;        //SV_ABSOLUTE=绝对速度值，SV_RELATE=相对速度，如-2=速度减2km/h，可叠加
    double speed;    //速度值，单位: km/h
    int64_t ttl;     //默认是0=仅本周起有效，5000表示未来5秒都有效，单位:毫秒
};


//速度管理
class nad_speed
{
public:
    double speed; //速度值，单位: km/h

    //所有的速度值
    vector<nad_speed_value> speed_list;

public:
    //构造析构函数
    nad_speed();
    virtual ~nad_speed();

    //添加一个速度值，返回综合速度
    double add(string name, int priority, int type, double speed, int64_t ttl = 0);

    //查找一个速度
    nad_speed_value *find(string name);

    //清空所有的速度
    void clear();

    //打印速度值
    void show();
};


#endif
