/*-------------------------------------------------------
 * 文件名：nad_config.cpp
 * 创建者：张毅00151602
 * 时  间：2016-03-02
 * 描  述：配置的代码
-------------------------------------------------------*/


//头文件
#include "config/nad_config.h"

//从xml文件中加载
void nad_ne_config::load_from_xml(pugi::xml_node &node)
{
    pugi::xml_attribute attr;
    for(attr = node.first_attribute(); attr; attr = attr.next_attribute())
    {
        if(strcmp(attr.name(), "name") == 0)
            name = attr.value();
        else if(strcmp(attr.name(), "ip") == 0)
            ip = attr.value();
        else if(strcmp(attr.name(), "port") == 0)
            port = atoi(attr.value());
        else
            lcm_url = attr.value();
    }
}

//查找网元
nad_ne_config *nad_ne_config_list::find(string name)
{
    map<string, nad_ne_config>::iterator iter;
    iter = ne_map.find(name);
    if (iter == ne_map.end())
    {
        return NULL;
    }
    else
    {
        return &(iter->second);
    }
}

//从xml文件中加载
void nad_ne_config_list::load_from_xml(pugi::xml_node &node)
{
    nad_ne_config cfg;
    pugi::xml_node tmp_node;
    pugi::xml_attribute attr;
    for(tmp_node = node.first_child(); tmp_node; tmp_node = tmp_node.next_sibling())
    {
        cfg.load_from_xml(tmp_node);
        ne_map.insert(pair<string, nad_ne_config>(cfg.name, cfg));
    }
}

//加载网元名，成功返回true
bool nad_config::load_lcoal(pugi::xml_document &doc,
    const char *cfg_name, nad_ne_config *cfg, nad_ne_config_list *cfg_list)
{

    //查找对应的网元
    if (cfg != NULL)
    {
        //csu/sim/oct这样的全局唯一网元
        local = *cfg;
        return true;
    }
    else if (NAD_LOCAL_NE != "")
    {
        //列表网元，用户通过obu_planning_0这样的方式指定了网元号0
        map<string, nad_ne_config>::iterator iter = cfg_list->ne_map.begin();
        for(; iter != cfg_list->ne_map.end(); iter++)
        {
            nad_ne_config &cfg = iter->second;
            if (srcontain(cfg.name, NAD_LOCAL_NE.c_str()))
            {
                local = cfg;
                return true;
            }
        }
        LOG(ERROR) << "NAD_LOCAL_NE=" << NAD_LOCAL_NE.c_str() << " 不存在!";
    }

    //前面的方法都不能查找对象，则读取local_obu等
    if (cfg_list != NULL)
    {
        //列表网元，用户配置文件的<local_obu>等参数指定了网元
        pugi::xml_node node;
        pugi::xml_attribute attr;
        node = doc.child("config").child(cfg_name);
        for(attr = node.first_attribute(); attr; attr = attr.next_attribute())
        {
            if(strcmp(attr.name(), "name") == 0)
                local.name = attr.value();
        }
        nad_ne_config *temp = cfg_list->find(local.name);
        if (temp != NULL)
        {
            local = *temp;
            return true;
        }
    }
    return false;
}

//从文件中加载配置
int nad_config::load_from_file(string filename)
{
    //加载配置文件
    pugi::xml_document doc;
    if (!doc.load_file(filename.c_str()))
    {
        LOG(ERROR) << "nad_config::load_from_file(" << filename << "): load xml error";
        return RET_NOT_EXIST;
    }

    //清除旧配置
    rsu_list.ne_map.clear();
    obu_list.ne_map.clear();
    rsd_list.ne_map.clear();
    oct_list.ne_map.clear();

    //临时变量
    pugi::xml_node node;

    //加载csu
    node = doc.child("config").child("csu");
    csu.load_from_xml(node);

    //加载rsu列表
    node = doc.child("config").child("rsu_list");
    rsu_list.load_from_xml(node);
    if (rsu_list.ne_map.size() > 0)
    {
        rsu1 = rsu_list.ne_map.begin()->second;
    }

    //加载obu列表
    node = doc.child("config").child("obu_list");
    obu_list.load_from_xml(node);

    //加载rsd列表
    node = doc.child("config").child("rsd_list");
    rsd_list.load_from_xml(node);

    //加载oct列表
    node = doc.child("config").child("oct_list");
    oct_list.load_from_xml(node);
    if (oct_list.ne_map.size() > 0)
    {
        oct1 = oct_list.ne_map.begin()->second;
    }

    //加载sim
    node = doc.child("config").child("sim");
    sim.load_from_xml(node);

    //查找local
    bool ret = false;
    #if defined(_NAD_CSU_)
        ret = load_lcoal(doc, NULL, &csu, NULL);
    #endif
    #if defined(_NAD_RSU_)
        ret = load_lcoal(doc, "local_rsu", NULL, &rsu_list);
    #endif
    //#if defined(_NAD_OBU_) || defined(_NAD_SIM_VUI_)
        ret = load_lcoal(doc, "local_obu", NULL, &obu_list);
    //#endif
    #if defined(_NAD_RSD_)
        ret = load_lcoal(doc, "local_rsd", NULL, &rsd_list);
    #endif
    #if defined(_NAD_OCT_)
        ret = load_lcoal(doc, "local_oct", NULL, &oct_list);
    #endif
    #if defined(_NAD_SIM_SYSTEM_)
        ret = load_lcoal(doc, NULL, &sim, NULL);
    #endif

    //找不到local
    if (!ret)
    {
        LOG(ERROR) << "nad_config::load_from_file(" << filename << "): local " << local.name << " not exist";
        return RET_NOT_EXIST;
    }

    //构造网元间消息名
    string tem = local.name;
    transform(tem.begin(), tem.end(), tem.begin(), ::toupper);
    ne_local_channel = "NEMSG_" + tem;

    //调试一下
    show();

    //返回加载成功
    return RET_OK;
}

//调试一下
void nad_config::show()
{
    cout << "local: " << local;
    cout << "csu  : " << csu;
    cout << "sim  : " << sim;
    cout << "rsu1 : " << rsu1;
    cout << "oct1 : " << oct1;
    cout << "rsu_list: " << endl << rsu_list;
    cout << "obu_list: " << endl << obu_list;
    cout << "rsd_list: " << endl << rsd_list;
    cout << "oct_list: " << endl << oct_list;
}


