/*-------------------------------------------------------
 * 文件名：nad_task_func.h
 * 创建者：张毅00151602
 * 时  间：2016-03-02
 * 描  述：二期脚本中的函数
-------------------------------------------------------*/
#ifndef _NAD_TASK_FUNC_H
#define _NAD_TASK_FUNC_H

#include "nad_function.h"
//一个脚本自定义函数
class nad_task_func
{
public:
    string name;               //函数名
    map<string, string> paras; //参数表，<参数名, 参数值>

    //获得参数值
    bool has_para(const char *name);
    const int64_t get_para(const char *name, const int64_t default_value);
    const char *get_para(const char *name, const char *default_value);
    const double get_para(const char *name, const double default_value);

    //保存到消息中，格式是多行文本，分别保存：函数名,参数名1，参数值1，参数名2，参数值2…
    string save_to_msg();

    //从消息中加载，格式是多行文本，分别保存：函数名,参数名1，参数值1，参数名2，参数值2…
    void load_from_msg(const string &task_func);
};


#endif
