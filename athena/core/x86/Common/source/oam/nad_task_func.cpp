/*-------------------------------------------------------
 * 文件名：nad_task_func.cpp
 * 创建者：张毅00151602
 * 时  间：2016-03-02
 * 描  述：二期脚本中的函数
-------------------------------------------------------*/

//头文件
#include "task/nad_task_func.h"

//判断参数是否存在
bool nad_task_func::has_para(const char *name)
{
    map<string, string>::iterator iter;
    iter = paras.find(name);
    return (iter != paras.end());
}

//获得参数值
const int64_t nad_task_func::get_para(const char *name, const int64_t default_value)
{
    map<string, string>::iterator iter;
    iter = paras.find(name);
    if (iter == paras.end())
    {
        return default_value;
    }
    else
    {
        return strtol(iter->second.c_str(), NULL, 10);
    }
}

const char *nad_task_func::get_para(const char *name, const char *default_value)
{
    map<string, string>::iterator iter;
    iter = paras.find(name);
    if (iter == paras.end())
    {
        return default_value;
    }
    else
    {
        return iter->second.c_str();
    }
}

const double nad_task_func::get_para(const char *name, const double default_value)
{
    map<string, string>::iterator iter;
    iter = paras.find(name);
    if (iter == paras.end())
    {
        return default_value;
    }
    else
    {
        return atof(iter->second.c_str());
    }
}

//保存到消息中，格式是多行文本，分别保存：函数名,参数名1，参数值1，参数名2，参数值2…
string nad_task_func::save_to_msg()
{
    string msg = name;
    map<string, string>::iterator it = paras.begin();
    for (; it != paras.end(); it++)
    {
        msg += "\n";
        msg += it->first;
        msg += "\n";
        msg += it->second;
    }
    return msg;
}

//从消息中加载，格式是多行文本，分别保存：函数名,参数名1，参数值1，参数名2，参数值2…
void nad_task_func::load_from_msg(const string &task_func)
{
    stringlist ls;
    ls.load_from_part(task_func, "\n");
    name = (ls.size() > 0) ? ls[0] : "";
    paras.clear();
    for (size_t i = 0; i < (ls.size() - 1) / 2; i++)
    {
        paras[ls[i * 2 + 1]] = ls[i * 2 + 2];
    }
}
