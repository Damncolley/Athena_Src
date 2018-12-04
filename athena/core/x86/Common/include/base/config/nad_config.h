/*-------------------------------------------------------
 * 文件名：nad_config.h
 * 创建者：张毅00151602
 * 时  间：2016-03-02
 * 描  述：配置的代码
-------------------------------------------------------*/
#ifndef _NAD_CONFIG_H
#define _NAD_CONFIG_H


//引用base头文件
#include "nad_base.h"

//一个网元的配置
class nad_ne_config
{
public:
    string name;    //网元名称
    string ip;      //网元间通讯ip,用于zmq
    int port;       //网元间通讯端口，用于zmq
    string lcm_url; //网元内通信的组播地址，用于lcm

    //从xml文件中加载
    void load_from_xml(pugi::xml_node &node);

    //重载赋值
    nad_ne_config & operator = (const nad_ne_config &ne)
    {
        name = ne.name;
        ip   = ne.ip;
        port = ne.port;
        lcm_url = ne.lcm_url;
        return *this;
    }

    //重载打印输出
    friend inline ostream & operator << (ostream & os, nad_ne_config &ne)
    {
        cout << "name=" << ne.name << ", ip=" << ne.ip << ", port=" << ne.port << ", lcm_url=" << ne.lcm_url << endl;
        return os;
    }
};

//网元配置数组
class nad_ne_config_list
{
public:
    //网元配置数组
    map<string, nad_ne_config> ne_map;

public:
    //查找网元
    nad_ne_config *find(string name);

    //从xml文件中加载
    void load_from_xml(pugi::xml_node &node);

    //重载打印输出
    friend inline ostream & operator << (ostream & os, nad_ne_config_list &ne)
    {
        int i = 1;
        map<string, nad_ne_config>::iterator iter;
        for(iter = ne.ne_map.begin(); iter != ne.ne_map.end(); iter++)
        {
            cout << "  " << i++ << ": " << iter->second;
        }
        return os;
    }
};

//CONFIG(控制块)的基类
class nad_config
{
public:
    //网元配置
    nad_ne_config      local;    //自身信息
    nad_ne_config      csu;      //CSU信息
    nad_ne_config      sim;      //SIM信息
    nad_ne_config      rsu1;     //第一个RSU的信息，应对目前只有一个RSU的场景
    nad_ne_config      oct1;     //第一个OCT的信息
    nad_ne_config_list rsu_list; //RSU列表
    nad_ne_config_list obu_list; //OBU列表
    nad_ne_config_list rsd_list; //RSD列表
    nad_ne_config_list oct_list; //OCT列表

    string ne_local_channel; //网元间消息的本地信道名

public:
    //加载网元名，成功返回true
    bool load_lcoal(pugi::xml_document &doc,
        const char *cfg_name, nad_ne_config *cfg, nad_ne_config_list *cfg_list);

    //从文件中加载配置
    int load_from_file(string filename);

    //调试一下
    void show();
};


#endif
