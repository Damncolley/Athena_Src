/*-------------------------------------------------------
 * 文件名：nad_db.cpp
 * 创建者：代码生成工具
 * 时  间：2016-12-20
 * 描  述：数据库的代码文件
-------------------------------------------------------*/


//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// 自动生成的代码，请勿修改
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


//头文件
#include "db/nad_db.h"


/*-------------------------------------------------------
 * 从db.xml(动态数据)加载数据，并保存到db中(std::map格式的内存数据库)
-------------------------------------------------------*/

//加载动态表的函数定义
static int nad_load_scene_to_db(     xml_document &doc); //当前场景
static int nad_load_csu_to_db(       xml_document &doc); //csu
static int nad_load_csu_user_to_db(  xml_document &doc); //csu用户(用于OCT登录)
static int nad_load_rsu_to_db(       xml_document &doc); //rsu
static int nad_load_obu_to_db(       xml_document &doc); //obu
static int nad_load_oct_to_db(       xml_document &doc); //oct
static int nad_load_rsd_to_db(       xml_document &doc); //路侧采集器名称
static int nad_load_rsd_sensor_to_db(xml_document &doc); //RSD的传感器
static int nad_load_light_to_db(     xml_document &doc); //红绿灯
static int nad_load_limspeed_to_db(  xml_document &doc); //限速牌
static int nad_load_block_to_db(     xml_document &doc); //施工标志
static int nad_load_ts_3rd_to_db(    xml_document &doc); //第三方交通系统
static int nad_load_crossing_to_db(  xml_document &doc); //路口
static int nad_load_conflict_to_db(  xml_document &doc); //冲突表
static int nad_load_warning_to_db(   xml_document &doc); //道路预警

//从db.xml(动态数据)加载数据，并保存到db中(std::map格式的内存数据库)
int nad_load_xml_to_db()
{
    //加载动态表的配置文件
    xml_document doc;
    string path = NAD_PATH + NAD_DB_FILE;
    int ret = doc.load_file(path.c_str());
    if (ret == 0)
    {
        LOG(ERROR) << "nad_load_xml_to_db(): load_file(" << NAD_DB_FILE << ") error: " << ret;
        return RET_FILE_ERR;
    }

    //加载动态表
    ret = RET_OK;
    if ((ret = nad_load_scene_to_db(     doc)) != RET_OK) return ret;
    if ((ret = nad_load_csu_to_db(       doc)) != RET_OK) return ret;
    if ((ret = nad_load_csu_user_to_db(  doc)) != RET_OK) return ret;
    if ((ret = nad_load_rsu_to_db(       doc)) != RET_OK) return ret;
    if ((ret = nad_load_obu_to_db(       doc)) != RET_OK) return ret;
    if ((ret = nad_load_oct_to_db(       doc)) != RET_OK) return ret;
    if ((ret = nad_load_rsd_to_db(       doc)) != RET_OK) return ret;
    if ((ret = nad_load_rsd_sensor_to_db(doc)) != RET_OK) return ret;
    if ((ret = nad_load_light_to_db(     doc)) != RET_OK) return ret;
    if ((ret = nad_load_limspeed_to_db(  doc)) != RET_OK) return ret;
    if ((ret = nad_load_block_to_db(     doc)) != RET_OK) return ret;
    if ((ret = nad_load_ts_3rd_to_db(    doc)) != RET_OK) return ret;
    if ((ret = nad_load_crossing_to_db(  doc)) != RET_OK) return ret;
    if ((ret = nad_load_conflict_to_db(  doc)) != RET_OK) return ret;
    if ((ret = nad_load_warning_to_db(   doc)) != RET_OK) return ret;
    return ret;
}


/*-------------------------------------------------------
 * 从db_type.xml(静态数据)加载数据，并保存到db中(std::map格式的内存数据库)
-------------------------------------------------------*/

//加载静态表的函数定义
static int nad_load_scene_type_to_db(   xml_document &doc); //场景类型
static int nad_load_csu_type_to_db(     xml_document &doc); //csu类型
static int nad_load_rsu_type_to_db(     xml_document &doc); //rsu类型
static int nad_load_obu_type_to_db(     xml_document &doc); //obu类型
static int nad_load_oct_type_to_db(     xml_document &doc); //oct类型
static int nad_load_rsd_type_to_db(     xml_document &doc); //路侧采集器类型
static int nad_load_light_type_to_db(   xml_document &doc); //红绿灯类型
static int nad_load_limspeed_type_to_db(xml_document &doc); //限速牌类型
static int nad_load_block_type_to_db(   xml_document &doc); //施工标志类型
static int nad_load_ts_3rd_type_to_db(  xml_document &doc); //第三方交通系统类型
static int nad_load_crossing_type_to_db(xml_document &doc); //路口类型

//从db_type.xml(静态数据)加载数据，并保存到db中(std::map格式的内存数据库)
int nad_load_type_xml_to_db()
{
    //加载静态表的配置文件
    xml_document doc;
    string path = NAD_PATH + NAD_DB_TYPE_FILE;
    int ret = doc.load_file(path.c_str());
    if (ret == 0)
    {
        LOG(ERROR) << "nad_load_type_xml_to_db(): load_file(" << NAD_DB_TYPE_FILE << ") error: " << ret;
        return RET_FILE_ERR;
    }

    //加载静态表
    ret = RET_OK;
    if ((ret = nad_load_scene_type_to_db(   doc)) != RET_OK) return ret;
    if ((ret = nad_load_csu_type_to_db(     doc)) != RET_OK) return ret;
    if ((ret = nad_load_rsu_type_to_db(     doc)) != RET_OK) return ret;
    if ((ret = nad_load_obu_type_to_db(     doc)) != RET_OK) return ret;
    if ((ret = nad_load_oct_type_to_db(     doc)) != RET_OK) return ret;
    if ((ret = nad_load_rsd_type_to_db(     doc)) != RET_OK) return ret;
    if ((ret = nad_load_light_type_to_db(   doc)) != RET_OK) return ret;
    if ((ret = nad_load_limspeed_type_to_db(doc)) != RET_OK) return ret;
    if ((ret = nad_load_block_type_to_db(   doc)) != RET_OK) return ret;
    if ((ret = nad_load_ts_3rd_type_to_db(  doc)) != RET_OK) return ret;
    if ((ret = nad_load_crossing_type_to_db(doc)) != RET_OK) return ret;
    return ret;
}


/*-------------------------------------------------------
 * 内存数据库(std::map格式)
-------------------------------------------------------*/

static map<string, nad_record_scene_type   > g_scene_type_table;    //场景类型
static map<string, nad_record_scene        > g_scene_table;         //当前场景
static map<string, nad_record_csu_type     > g_csu_type_table;      //csu类型
static map<string, nad_record_csu          > g_csu_table;           //csu
static map<string, nad_record_csu_user     > g_csu_user_table;      //csu用户(用于OCT登录)
static map<string, nad_record_rsu_type     > g_rsu_type_table;      //rsu类型
static map<string, nad_record_rsu          > g_rsu_table;           //rsu
static map<string, nad_record_obu_type     > g_obu_type_table;      //obu类型
static map<string, nad_record_obu          > g_obu_table;           //obu
static map<string, nad_record_oct_type     > g_oct_type_table;      //oct类型
static map<string, nad_record_oct          > g_oct_table;           //oct
static map<string, nad_record_rsd_type     > g_rsd_type_table;      //路侧采集器类型
static map<string, nad_record_rsd          > g_rsd_table;           //路侧采集器名称
static map<string, nad_record_rsd_sensor   > g_rsd_sensor_table;    //RSD的传感器
static map<string, nad_record_light_type   > g_light_type_table;    //红绿灯类型
static map<string, nad_record_light        > g_light_table;         //红绿灯
static map<string, nad_record_limspeed_type> g_limspeed_type_table; //限速牌类型
static map<string, nad_record_limspeed     > g_limspeed_table;      //限速牌
static map<string, nad_record_block_type   > g_block_type_table;    //施工标志类型
static map<string, nad_record_block        > g_block_table;         //施工标志
static map<string, nad_record_ts_3rd_type  > g_ts_3rd_type_table;   //第三方交通系统类型
static map<string, nad_record_ts_3rd       > g_ts_3rd_table;        //第三方交通系统
static map<string, nad_record_crossing_type> g_crossing_type_table; //路口类型
static map<string, nad_record_crossing     > g_crossing_table;      //路口
static map<string, nad_record_conflict     > g_conflict_table;      //冲突表
static map<string, nad_record_warning      > g_warning_table;       //道路预警


/*-------------------------------------------------------
 * scene_type: 场景类型
-------------------------------------------------------*/

//赋值函数
nad_record_scene_type & nad_record_scene_type::operator = (const nad_record_scene_type &rec)
{
    scene_type     = rec.scene_type;     //场景类型名称
    scene_desc     = rec.scene_desc;     //场景类型描述
    avg_speed      = rec.avg_speed;      //平均车速(km/h)
    avg_lane_width = rec.avg_lane_width; //平均道路宽度(cm)
    return *this;
}

//使用主键查询scene_type表，失败返回NULL
nad_record_scene_type *db_query_scene_type(string scene_type)
{
    map<string, nad_record_scene_type>::iterator iter;
    iter = g_scene_type_table.find(scene_type);
    if (iter == g_scene_type_table.end())
    {
        return NULL;
    }
    else
    {
        return &(iter->second);
    }
}

//查询scene_type表的所有记录，不会失败
map<string, nad_record_scene_type> &db_query_all_scene_type()
{
    return g_scene_type_table;
}

//从xml加载scene_type表数据到db
static int nad_load_scene_type_to_db(xml_document &doc)
{
    //然后从xml加载scene_type表数据到db
    xml_node node = doc.child("db").child("scene_type_table");
    for (xml_node tmp_node = node.first_child(); tmp_node; tmp_node = tmp_node.next_sibling())
    {
        nad_record_scene_type rec;
        for (xml_attribute attr = tmp_node.first_attribute(); attr; attr = attr.next_attribute())
        {
            if (strcmp(attr.name(), "scene_type") == 0)
                rec.scene_type     = attr.value();       //场景类型名称
            else if (strcmp(attr.name(), "scene_desc") == 0)
                rec.scene_desc     = attr.value();       //场景类型描述
            else if (strcmp(attr.name(), "avg_speed") == 0)
                rec.avg_speed      = atof(attr.value()); //平均车速(km/h)
            else if (strcmp(attr.name(), "avg_lane_width") == 0)
                rec.avg_lane_width = atof(attr.value()); //平均道路宽度(cm)
        }

        //插入新数据到scene_type表中
        g_scene_type_table[rec.scene_type] = rec;
        cout << rec << endl;
    }

    //返回成功
    return RET_OK;
}


/*-------------------------------------------------------
 * scene: 当前场景
-------------------------------------------------------*/

//赋值函数
nad_record_scene & nad_record_scene::operator = (const nad_record_scene &rec)
{
    scene_name = rec.scene_name; //场景名称
    scene_type = rec.scene_type; //场景类型
    return *this;
}

//使用主键查询scene表，失败返回NULL
nad_record_scene *db_query_scene(string scene_name)
{
    map<string, nad_record_scene>::iterator iter;
    iter = g_scene_table.find(scene_name);
    if (iter == g_scene_table.end())
    {
        return NULL;
    }
    else
    {
        return &(iter->second);
    }
}

//查询scene表的所有记录，不会失败
map<string, nad_record_scene> &db_query_all_scene()
{
    return g_scene_table;
}

//从xml加载scene表数据到db
static int nad_load_scene_to_db(xml_document &doc)
{
    //然后从xml加载scene表数据到db
    xml_node node = doc.child("db").child("scene_table");
    for (xml_node tmp_node = node.first_child(); tmp_node; tmp_node = tmp_node.next_sibling())
    {
        nad_record_scene rec;
        for (xml_attribute attr = tmp_node.first_attribute(); attr; attr = attr.next_attribute())
        {
            if (strcmp(attr.name(), "scene_name") == 0)
                rec.scene_name = attr.value(); //场景名称
            else if (strcmp(attr.name(), "scene_type") == 0)
                rec.scene_type = attr.value(); //场景类型
        }

        //插入新数据到scene表中
        g_scene_table[rec.scene_name] = rec;
        cout << rec << endl;
    }

    //返回成功
    return RET_OK;
}


/*-------------------------------------------------------
 * csu_type: csu类型
-------------------------------------------------------*/

//赋值函数
nad_record_csu_type & nad_record_csu_type::operator = (const nad_record_csu_type &rec)
{
    csu_type = rec.csu_type; //csu类型名称
    csu_desc = rec.csu_desc; //csu类型描述
    return *this;
}

//使用主键查询csu_type表，失败返回NULL
nad_record_csu_type *db_query_csu_type(string csu_type)
{
    map<string, nad_record_csu_type>::iterator iter;
    iter = g_csu_type_table.find(csu_type);
    if (iter == g_csu_type_table.end())
    {
        return NULL;
    }
    else
    {
        return &(iter->second);
    }
}

//查询csu_type表的所有记录，不会失败
map<string, nad_record_csu_type> &db_query_all_csu_type()
{
    return g_csu_type_table;
}

//从xml加载csu_type表数据到db
static int nad_load_csu_type_to_db(xml_document &doc)
{
    //然后从xml加载csu_type表数据到db
    xml_node node = doc.child("db").child("csu_type_table");
    for (xml_node tmp_node = node.first_child(); tmp_node; tmp_node = tmp_node.next_sibling())
    {
        nad_record_csu_type rec;
        for (xml_attribute attr = tmp_node.first_attribute(); attr; attr = attr.next_attribute())
        {
            if (strcmp(attr.name(), "csu_type") == 0)
                rec.csu_type = attr.value(); //csu类型名称
            else if (strcmp(attr.name(), "csu_desc") == 0)
                rec.csu_desc = attr.value(); //csu类型描述
        }

        //插入新数据到csu_type表中
        g_csu_type_table[rec.csu_type] = rec;
        cout << rec << endl;
    }

    //返回成功
    return RET_OK;
}


/*-------------------------------------------------------
 * csu: csu
-------------------------------------------------------*/

//赋值函数
nad_record_csu & nad_record_csu::operator = (const nad_record_csu &rec)
{
    csu_name = rec.csu_name; //csu名称
    csu_type = rec.csu_type; //csu类型
    return *this;
}

//使用主键查询csu表，失败返回NULL
nad_record_csu *db_query_csu(string csu_name)
{
    map<string, nad_record_csu>::iterator iter;
    iter = g_csu_table.find(csu_name);
    if (iter == g_csu_table.end())
    {
        return NULL;
    }
    else
    {
        return &(iter->second);
    }
}

//查询csu表的所有记录，不会失败
map<string, nad_record_csu> &db_query_all_csu()
{
    return g_csu_table;
}

//从xml加载csu表数据到db
static int nad_load_csu_to_db(xml_document &doc)
{
    //然后从xml加载csu表数据到db
    xml_node node = doc.child("db").child("csu_table");
    for (xml_node tmp_node = node.first_child(); tmp_node; tmp_node = tmp_node.next_sibling())
    {
        nad_record_csu rec;
        for (xml_attribute attr = tmp_node.first_attribute(); attr; attr = attr.next_attribute())
        {
            if (strcmp(attr.name(), "csu_name") == 0)
                rec.csu_name = attr.value(); //csu名称
            else if (strcmp(attr.name(), "csu_type") == 0)
                rec.csu_type = attr.value(); //csu类型
        }

        //插入新数据到csu表中
        g_csu_table[rec.csu_name] = rec;
        cout << rec << endl;
    }

    //返回成功
    return RET_OK;
}


/*-------------------------------------------------------
 * csu_user: csu用户(用于OCT登录)
-------------------------------------------------------*/

//赋值函数
nad_record_csu_user & nad_record_csu_user::operator = (const nad_record_csu_user &rec)
{
    csu_user     = rec.csu_user;     //csu登录用户名
    csu_password = rec.csu_password; //csu登录密码
    return *this;
}

//使用主键查询csu_user表，失败返回NULL
nad_record_csu_user *db_query_csu_user(string csu_user)
{
    map<string, nad_record_csu_user>::iterator iter;
    iter = g_csu_user_table.find(csu_user);
    if (iter == g_csu_user_table.end())
    {
        return NULL;
    }
    else
    {
        return &(iter->second);
    }
}

//查询csu_user表的所有记录，不会失败
map<string, nad_record_csu_user> &db_query_all_csu_user()
{
    return g_csu_user_table;
}

//从xml加载csu_user表数据到db
static int nad_load_csu_user_to_db(xml_document &doc)
{
    //然后从xml加载csu_user表数据到db
    xml_node node = doc.child("db").child("csu_user_table");
    for (xml_node tmp_node = node.first_child(); tmp_node; tmp_node = tmp_node.next_sibling())
    {
        nad_record_csu_user rec;
        for (xml_attribute attr = tmp_node.first_attribute(); attr; attr = attr.next_attribute())
        {
            if (strcmp(attr.name(), "csu_user") == 0)
                rec.csu_user     = attr.value(); //csu登录用户名
            else if (strcmp(attr.name(), "csu_password") == 0)
                rec.csu_password = attr.value(); //csu登录密码
        }

        //插入新数据到csu_user表中
        g_csu_user_table[rec.csu_user] = rec;
        cout << rec << endl;
    }

    //返回成功
    return RET_OK;
}


/*-------------------------------------------------------
 * rsu_type: rsu类型
-------------------------------------------------------*/

//赋值函数
nad_record_rsu_type & nad_record_rsu_type::operator = (const nad_record_rsu_type &rec)
{
    rsu_type = rec.rsu_type; //rsu类型名称
    rsu_desc = rec.rsu_desc; //rsu类型描述
    return *this;
}

//使用主键查询rsu_type表，失败返回NULL
nad_record_rsu_type *db_query_rsu_type(string rsu_type)
{
    map<string, nad_record_rsu_type>::iterator iter;
    iter = g_rsu_type_table.find(rsu_type);
    if (iter == g_rsu_type_table.end())
    {
        return NULL;
    }
    else
    {
        return &(iter->second);
    }
}

//查询rsu_type表的所有记录，不会失败
map<string, nad_record_rsu_type> &db_query_all_rsu_type()
{
    return g_rsu_type_table;
}

//从xml加载rsu_type表数据到db
static int nad_load_rsu_type_to_db(xml_document &doc)
{
    //然后从xml加载rsu_type表数据到db
    xml_node node = doc.child("db").child("rsu_type_table");
    for (xml_node tmp_node = node.first_child(); tmp_node; tmp_node = tmp_node.next_sibling())
    {
        nad_record_rsu_type rec;
        for (xml_attribute attr = tmp_node.first_attribute(); attr; attr = attr.next_attribute())
        {
            if (strcmp(attr.name(), "rsu_type") == 0)
                rec.rsu_type = attr.value(); //rsu类型名称
            else if (strcmp(attr.name(), "rsu_desc") == 0)
                rec.rsu_desc = attr.value(); //rsu类型描述
        }

        //插入新数据到rsu_type表中
        g_rsu_type_table[rec.rsu_type] = rec;
        cout << rec << endl;
    }

    //返回成功
    return RET_OK;
}


/*-------------------------------------------------------
 * rsu: rsu
-------------------------------------------------------*/

//赋值函数
nad_record_rsu & nad_record_rsu::operator = (const nad_record_rsu &rec)
{
    rsu_name       = rec.rsu_name;       //rsu名称
    rsu_type       = rec.rsu_type;       //rsu类型
    rsu_lon        = rec.rsu_lon;        //rsu位置(经度)
    rsu_lat        = rec.rsu_lat;        //rsu位置(纬度)
    rsu_radius_lon = rec.rsu_radius_lon; //rsu覆盖半径(经度,米)
    rsu_radius_lat = rec.rsu_radius_lat; //rsu覆盖半径(纬度,米)
    return *this;
}

//使用主键查询rsu表，失败返回NULL
nad_record_rsu *db_query_rsu(string rsu_name)
{
    map<string, nad_record_rsu>::iterator iter;
    iter = g_rsu_table.find(rsu_name);
    if (iter == g_rsu_table.end())
    {
        return NULL;
    }
    else
    {
        return &(iter->second);
    }
}

//查询rsu表的所有记录，不会失败
map<string, nad_record_rsu> &db_query_all_rsu()
{
    return g_rsu_table;
}

//从xml加载rsu表数据到db
static int nad_load_rsu_to_db(xml_document &doc)
{
    //然后从xml加载rsu表数据到db
    xml_node node = doc.child("db").child("rsu_table");
    for (xml_node tmp_node = node.first_child(); tmp_node; tmp_node = tmp_node.next_sibling())
    {
        nad_record_rsu rec;
        for (xml_attribute attr = tmp_node.first_attribute(); attr; attr = attr.next_attribute())
        {
            if (strcmp(attr.name(), "rsu_name") == 0)
                rec.rsu_name       = attr.value();       //rsu名称
            else if (strcmp(attr.name(), "rsu_type") == 0)
                rec.rsu_type       = attr.value();       //rsu类型
            else if (strcmp(attr.name(), "rsu_lon") == 0)
                rec.rsu_lon        = atof(attr.value()); //rsu位置(经度)
            else if (strcmp(attr.name(), "rsu_lat") == 0)
                rec.rsu_lat        = atof(attr.value()); //rsu位置(纬度)
            else if (strcmp(attr.name(), "rsu_radius_lon") == 0)
                rec.rsu_radius_lon = atof(attr.value()); //rsu覆盖半径(经度,米)
            else if (strcmp(attr.name(), "rsu_radius_lat") == 0)
                rec.rsu_radius_lat = atof(attr.value()); //rsu覆盖半径(纬度,米)
        }

        //插入新数据到rsu表中
        g_rsu_table[rec.rsu_name] = rec;
        cout << rec << endl;
    }

    //返回成功
    return RET_OK;
}


/*-------------------------------------------------------
 * obu_type: obu类型
-------------------------------------------------------*/

//赋值函数
nad_record_obu_type & nad_record_obu_type::operator = (const nad_record_obu_type &rec)
{
    obu_type = rec.obu_type; //obu类型名称
    obu_desc = rec.obu_desc; //obu类型描述
    return *this;
}

//使用主键查询obu_type表，失败返回NULL
nad_record_obu_type *db_query_obu_type(string obu_type)
{
    map<string, nad_record_obu_type>::iterator iter;
    iter = g_obu_type_table.find(obu_type);
    if (iter == g_obu_type_table.end())
    {
        return NULL;
    }
    else
    {
        return &(iter->second);
    }
}

//查询obu_type表的所有记录，不会失败
map<string, nad_record_obu_type> &db_query_all_obu_type()
{
    return g_obu_type_table;
}

//从xml加载obu_type表数据到db
static int nad_load_obu_type_to_db(xml_document &doc)
{
    //然后从xml加载obu_type表数据到db
    xml_node node = doc.child("db").child("obu_type_table");
    for (xml_node tmp_node = node.first_child(); tmp_node; tmp_node = tmp_node.next_sibling())
    {
        nad_record_obu_type rec;
        for (xml_attribute attr = tmp_node.first_attribute(); attr; attr = attr.next_attribute())
        {
            if (strcmp(attr.name(), "obu_type") == 0)
                rec.obu_type = attr.value(); //obu类型名称
            else if (strcmp(attr.name(), "obu_desc") == 0)
                rec.obu_desc = attr.value(); //obu类型描述
        }

        //插入新数据到obu_type表中
        g_obu_type_table[rec.obu_type] = rec;
        cout << rec << endl;
    }

    //返回成功
    return RET_OK;
}


/*-------------------------------------------------------
 * obu: obu
-------------------------------------------------------*/

//赋值函数
nad_record_obu & nad_record_obu::operator = (const nad_record_obu &rec)
{
    obu_name = rec.obu_name; //obu名称
    obu_type = rec.obu_type; //obu类型
    return *this;
}

//使用主键查询obu表，失败返回NULL
nad_record_obu *db_query_obu(string obu_name)
{
    map<string, nad_record_obu>::iterator iter;
    iter = g_obu_table.find(obu_name);
    if (iter == g_obu_table.end())
    {
        return NULL;
    }
    else
    {
        return &(iter->second);
    }
}

//查询obu表的所有记录，不会失败
map<string, nad_record_obu> &db_query_all_obu()
{
    return g_obu_table;
}

//从xml加载obu表数据到db
static int nad_load_obu_to_db(xml_document &doc)
{
    //然后从xml加载obu表数据到db
    xml_node node = doc.child("db").child("obu_table");
    for (xml_node tmp_node = node.first_child(); tmp_node; tmp_node = tmp_node.next_sibling())
    {
        nad_record_obu rec;
        for (xml_attribute attr = tmp_node.first_attribute(); attr; attr = attr.next_attribute())
        {
            if (strcmp(attr.name(), "obu_name") == 0)
                rec.obu_name = attr.value(); //obu名称
            else if (strcmp(attr.name(), "obu_type") == 0)
                rec.obu_type = attr.value(); //obu类型
        }

        //插入新数据到obu表中
        g_obu_table[rec.obu_name] = rec;
        cout << rec << endl;
    }

    //返回成功
    return RET_OK;
}


/*-------------------------------------------------------
 * oct_type: oct类型
-------------------------------------------------------*/

//赋值函数
nad_record_oct_type & nad_record_oct_type::operator = (const nad_record_oct_type &rec)
{
    oct_type = rec.oct_type; //oct类型名称
    oct_desc = rec.oct_desc; //oct类型描述
    return *this;
}

//使用主键查询oct_type表，失败返回NULL
nad_record_oct_type *db_query_oct_type(string oct_type)
{
    map<string, nad_record_oct_type>::iterator iter;
    iter = g_oct_type_table.find(oct_type);
    if (iter == g_oct_type_table.end())
    {
        return NULL;
    }
    else
    {
        return &(iter->second);
    }
}

//查询oct_type表的所有记录，不会失败
map<string, nad_record_oct_type> &db_query_all_oct_type()
{
    return g_oct_type_table;
}

//从xml加载oct_type表数据到db
static int nad_load_oct_type_to_db(xml_document &doc)
{
    //然后从xml加载oct_type表数据到db
    xml_node node = doc.child("db").child("oct_type_table");
    for (xml_node tmp_node = node.first_child(); tmp_node; tmp_node = tmp_node.next_sibling())
    {
        nad_record_oct_type rec;
        for (xml_attribute attr = tmp_node.first_attribute(); attr; attr = attr.next_attribute())
        {
            if (strcmp(attr.name(), "oct_type") == 0)
                rec.oct_type = attr.value(); //oct类型名称
            else if (strcmp(attr.name(), "oct_desc") == 0)
                rec.oct_desc = attr.value(); //oct类型描述
        }

        //插入新数据到oct_type表中
        g_oct_type_table[rec.oct_type] = rec;
        cout << rec << endl;
    }

    //返回成功
    return RET_OK;
}


/*-------------------------------------------------------
 * oct: oct
-------------------------------------------------------*/

//赋值函数
nad_record_oct & nad_record_oct::operator = (const nad_record_oct &rec)
{
    oct_name = rec.oct_name; //oct名称
    oct_type = rec.oct_type; //oct类型
    return *this;
}

//使用主键查询oct表，失败返回NULL
nad_record_oct *db_query_oct(string oct_name)
{
    map<string, nad_record_oct>::iterator iter;
    iter = g_oct_table.find(oct_name);
    if (iter == g_oct_table.end())
    {
        return NULL;
    }
    else
    {
        return &(iter->second);
    }
}

//查询oct表的所有记录，不会失败
map<string, nad_record_oct> &db_query_all_oct()
{
    return g_oct_table;
}

//从xml加载oct表数据到db
static int nad_load_oct_to_db(xml_document &doc)
{
    //然后从xml加载oct表数据到db
    xml_node node = doc.child("db").child("oct_table");
    for (xml_node tmp_node = node.first_child(); tmp_node; tmp_node = tmp_node.next_sibling())
    {
        nad_record_oct rec;
        for (xml_attribute attr = tmp_node.first_attribute(); attr; attr = attr.next_attribute())
        {
            if (strcmp(attr.name(), "oct_name") == 0)
                rec.oct_name = attr.value(); //oct名称
            else if (strcmp(attr.name(), "oct_type") == 0)
                rec.oct_type = attr.value(); //oct类型
        }

        //插入新数据到oct表中
        g_oct_table[rec.oct_name] = rec;
        cout << rec << endl;
    }

    //返回成功
    return RET_OK;
}


/*-------------------------------------------------------
 * rsd_type: 路侧采集器类型
-------------------------------------------------------*/

//赋值函数
nad_record_rsd_type & nad_record_rsd_type::operator = (const nad_record_rsd_type &rec)
{
    rsd_type = rec.rsd_type; //rsd类型名称
    rsd_desc = rec.rsd_desc; //rsd类型描述
    return *this;
}

//使用主键查询rsd_type表，失败返回NULL
nad_record_rsd_type *db_query_rsd_type(string rsd_type)
{
    map<string, nad_record_rsd_type>::iterator iter;
    iter = g_rsd_type_table.find(rsd_type);
    if (iter == g_rsd_type_table.end())
    {
        return NULL;
    }
    else
    {
        return &(iter->second);
    }
}

//查询rsd_type表的所有记录，不会失败
map<string, nad_record_rsd_type> &db_query_all_rsd_type()
{
    return g_rsd_type_table;
}

//从xml加载rsd_type表数据到db
static int nad_load_rsd_type_to_db(xml_document &doc)
{
    //然后从xml加载rsd_type表数据到db
    xml_node node = doc.child("db").child("rsd_type_table");
    for (xml_node tmp_node = node.first_child(); tmp_node; tmp_node = tmp_node.next_sibling())
    {
        nad_record_rsd_type rec;
        for (xml_attribute attr = tmp_node.first_attribute(); attr; attr = attr.next_attribute())
        {
            if (strcmp(attr.name(), "rsd_type") == 0)
                rec.rsd_type = attr.value(); //rsd类型名称
            else if (strcmp(attr.name(), "rsd_desc") == 0)
                rec.rsd_desc = attr.value(); //rsd类型描述
        }

        //插入新数据到rsd_type表中
        g_rsd_type_table[rec.rsd_type] = rec;
        cout << rec << endl;
    }

    //返回成功
    return RET_OK;
}


/*-------------------------------------------------------
 * rsd: 路侧采集器名称
-------------------------------------------------------*/

//赋值函数
nad_record_rsd & nad_record_rsd::operator = (const nad_record_rsd &rec)
{
    rsd_name = rec.rsd_name; //rsd名称
    rsd_type = rec.rsd_type; //rsd类型
    rsd_ip   = rec.rsd_ip;   //rsd的IP
    return *this;
}

//使用主键查询rsd表，失败返回NULL
nad_record_rsd *db_query_rsd(string rsd_name)
{
    map<string, nad_record_rsd>::iterator iter;
    iter = g_rsd_table.find(rsd_name);
    if (iter == g_rsd_table.end())
    {
        return NULL;
    }
    else
    {
        return &(iter->second);
    }
}

//查询rsd表的所有记录，不会失败
map<string, nad_record_rsd> &db_query_all_rsd()
{
    return g_rsd_table;
}

//从xml加载rsd表数据到db
static int nad_load_rsd_to_db(xml_document &doc)
{
    //然后从xml加载rsd表数据到db
    xml_node node = doc.child("db").child("rsd_table");
    for (xml_node tmp_node = node.first_child(); tmp_node; tmp_node = tmp_node.next_sibling())
    {
        nad_record_rsd rec;
        for (xml_attribute attr = tmp_node.first_attribute(); attr; attr = attr.next_attribute())
        {
            if (strcmp(attr.name(), "rsd_name") == 0)
                rec.rsd_name = attr.value(); //rsd名称
            else if (strcmp(attr.name(), "rsd_type") == 0)
                rec.rsd_type = attr.value(); //rsd类型
            else if (strcmp(attr.name(), "rsd_ip") == 0)
                rec.rsd_ip   = attr.value(); //rsd的IP
        }

        //插入新数据到rsd表中
        g_rsd_table[rec.rsd_name] = rec;
        cout << rec << endl;
    }

    //返回成功
    return RET_OK;
}


/*-------------------------------------------------------
 * rsd_sensor: RSD的传感器
-------------------------------------------------------*/

//赋值函数
nad_record_rsd_sensor & nad_record_rsd_sensor::operator = (const nad_record_rsd_sensor &rec)
{
    rsd_sensor_name = rec.rsd_sensor_name; //rsd传感器名称
    type            = rec.type;            //rsd传感器类型
    lon             = rec.lon;             //安装位置的经度
    lat             = rec.lat;             //安装位置的纬度
    x               = rec.x;               //安装位置的x坐标
    y               = rec.y;               //安装位置的y坐标
    yaw             = rec.yaw;             //头指向(相对正北的夹角)
    range           = rec.range;           //有效扫描范围
    vertical_fov    = rec.vertical_fov;    //垂直视场角
    horizontal_fov  = rec.horizontal_fov;  //水平视场角
    ip              = rec.ip;              //rsd传感器的IP
    lane_id         = rec.lane_id;         //管理道路的高速道ID
    return *this;
}

//使用主键查询rsd_sensor表，失败返回NULL
nad_record_rsd_sensor *db_query_rsd_sensor(string rsd_sensor_name)
{
    map<string, nad_record_rsd_sensor>::iterator iter;
    iter = g_rsd_sensor_table.find(rsd_sensor_name);
    if (iter == g_rsd_sensor_table.end())
    {
        return NULL;
    }
    else
    {
        return &(iter->second);
    }
}

//查询rsd_sensor表的所有记录，不会失败
map<string, nad_record_rsd_sensor> &db_query_all_rsd_sensor()
{
    return g_rsd_sensor_table;
}

//从xml加载rsd_sensor表数据到db
static int nad_load_rsd_sensor_to_db(xml_document &doc)
{
    //然后从xml加载rsd_sensor表数据到db
    xml_node node = doc.child("db").child("rsd_sensor_table");
    for (xml_node tmp_node = node.first_child(); tmp_node; tmp_node = tmp_node.next_sibling())
    {
        nad_record_rsd_sensor rec;
        for (xml_attribute attr = tmp_node.first_attribute(); attr; attr = attr.next_attribute())
        {
            if (strcmp(attr.name(), "rsd_sensor_name") == 0)
                rec.rsd_sensor_name = attr.value();                   //rsd传感器名称
            else if (strcmp(attr.name(), "type") == 0)
                rec.type            = atol(attr.value());             //rsd传感器类型
            else if (strcmp(attr.name(), "lon") == 0)
                rec.lon             = atof(attr.value());             //安装位置的经度
            else if (strcmp(attr.name(), "lat") == 0)
                rec.lat             = atof(attr.value());             //安装位置的纬度
            else if (strcmp(attr.name(), "x") == 0)
                rec.x               = atof(attr.value());             //安装位置的x坐标
            else if (strcmp(attr.name(), "y") == 0)
                rec.y               = atof(attr.value());             //安装位置的y坐标
            else if (strcmp(attr.name(), "yaw") == 0)
                rec.yaw             = atof(attr.value());             //头指向(相对正北的夹角)
            else if (strcmp(attr.name(), "range") == 0)
                rec.range           = atol(attr.value());             //有效扫描范围
            else if (strcmp(attr.name(), "vertical_fov") == 0)
                rec.vertical_fov    = atol(attr.value());             //垂直视场角
            else if (strcmp(attr.name(), "horizontal_fov") == 0)
                rec.horizontal_fov  = atol(attr.value());             //水平视场角
            else if (strcmp(attr.name(), "ip") == 0)
                rec.ip              = attr.value();                   //rsd传感器的IP
            else if (strcmp(attr.name(), "lane_id") == 0)
                rec.lane_id         = strtol(attr.value(), NULL, 10); //管理道路的高速道ID
        }

        //插入新数据到rsd_sensor表中
        g_rsd_sensor_table[rec.rsd_sensor_name] = rec;
        cout << rec << endl;
    }

    //返回成功
    return RET_OK;
}


/*-------------------------------------------------------
 * light_type: 红绿灯类型
-------------------------------------------------------*/

//赋值函数
nad_record_light_type & nad_record_light_type::operator = (const nad_record_light_type &rec)
{
    light_type = rec.light_type; //红绿灯类型名称
    light_desc = rec.light_desc; //红绿灯类型描述
    return *this;
}

//使用主键查询light_type表，失败返回NULL
nad_record_light_type *db_query_light_type(string light_type)
{
    map<string, nad_record_light_type>::iterator iter;
    iter = g_light_type_table.find(light_type);
    if (iter == g_light_type_table.end())
    {
        return NULL;
    }
    else
    {
        return &(iter->second);
    }
}

//查询light_type表的所有记录，不会失败
map<string, nad_record_light_type> &db_query_all_light_type()
{
    return g_light_type_table;
}

//从xml加载light_type表数据到db
static int nad_load_light_type_to_db(xml_document &doc)
{
    //然后从xml加载light_type表数据到db
    xml_node node = doc.child("db").child("light_type_table");
    for (xml_node tmp_node = node.first_child(); tmp_node; tmp_node = tmp_node.next_sibling())
    {
        nad_record_light_type rec;
        for (xml_attribute attr = tmp_node.first_attribute(); attr; attr = attr.next_attribute())
        {
            if (strcmp(attr.name(), "light_type") == 0)
                rec.light_type = attr.value(); //红绿灯类型名称
            else if (strcmp(attr.name(), "light_desc") == 0)
                rec.light_desc = attr.value(); //红绿灯类型描述
        }

        //插入新数据到light_type表中
        g_light_type_table[rec.light_type] = rec;
        cout << rec << endl;
    }

    //返回成功
    return RET_OK;
}


/*-------------------------------------------------------
 * light: 红绿灯
-------------------------------------------------------*/

//赋值函数
nad_record_light & nad_record_light::operator = (const nad_record_light &rec)
{
    light_id     = rec.light_id;     //红绿灯ID(经度纬度)
    light_type   = rec.light_type;   //红绿灯类型
    light_status = rec.light_status; //红绿灯状态
    light_ip     = rec.light_ip;     //红绿灯的IP
    stop_lane_id = rec.stop_lane_id; //管理道路的高速道ID
    return *this;
}

//使用主键查询light表，失败返回NULL
nad_record_light *db_query_light(string light_id)
{
    map<string, nad_record_light>::iterator iter;
    iter = g_light_table.find(light_id);
    if (iter == g_light_table.end())
    {
        return NULL;
    }
    else
    {
        return &(iter->second);
    }
}

//查询light表的所有记录，不会失败
map<string, nad_record_light> &db_query_all_light()
{
    return g_light_table;
}

//从xml加载light表数据到db
static int nad_load_light_to_db(xml_document &doc)
{
    //然后从xml加载light表数据到db
    xml_node node = doc.child("db").child("light_table");
    for (xml_node tmp_node = node.first_child(); tmp_node; tmp_node = tmp_node.next_sibling())
    {
        nad_record_light rec;
        for (xml_attribute attr = tmp_node.first_attribute(); attr; attr = attr.next_attribute())
        {
            if (strcmp(attr.name(), "light_id") == 0)
                rec.light_id     = attr.value();                   //红绿灯ID(经度纬度)
            else if (strcmp(attr.name(), "light_type") == 0)
                rec.light_type   = attr.value();                   //红绿灯类型
            else if (strcmp(attr.name(), "light_status") == 0)
                rec.light_status = (uint)atol(attr.value());       //红绿灯状态
            else if (strcmp(attr.name(), "light_ip") == 0)
                rec.light_ip     = attr.value();                   //红绿灯的IP
            else if (strcmp(attr.name(), "stop_lane_id") == 0)
                rec.stop_lane_id = strtol(attr.value(), NULL, 10); //管理道路的高速道ID
        }

        //插入新数据到light表中
        g_light_table[rec.light_id] = rec;
        cout << rec << endl;
    }

    //返回成功
    return RET_OK;
}


/*-------------------------------------------------------
 * limspeed_type: 限速牌类型
-------------------------------------------------------*/

//赋值函数
nad_record_limspeed_type & nad_record_limspeed_type::operator = (const nad_record_limspeed_type &rec)
{
    limspeed_type = rec.limspeed_type; //限速牌类型名称
    limspeed_desc = rec.limspeed_desc; //限速牌类型描述
    return *this;
}

//使用主键查询limspeed_type表，失败返回NULL
nad_record_limspeed_type *db_query_limspeed_type(string limspeed_type)
{
    map<string, nad_record_limspeed_type>::iterator iter;
    iter = g_limspeed_type_table.find(limspeed_type);
    if (iter == g_limspeed_type_table.end())
    {
        return NULL;
    }
    else
    {
        return &(iter->second);
    }
}

//查询limspeed_type表的所有记录，不会失败
map<string, nad_record_limspeed_type> &db_query_all_limspeed_type()
{
    return g_limspeed_type_table;
}

//从xml加载limspeed_type表数据到db
static int nad_load_limspeed_type_to_db(xml_document &doc)
{
    //然后从xml加载limspeed_type表数据到db
    xml_node node = doc.child("db").child("limspeed_type_table");
    for (xml_node tmp_node = node.first_child(); tmp_node; tmp_node = tmp_node.next_sibling())
    {
        nad_record_limspeed_type rec;
        for (xml_attribute attr = tmp_node.first_attribute(); attr; attr = attr.next_attribute())
        {
            if (strcmp(attr.name(), "limspeed_type") == 0)
                rec.limspeed_type = attr.value(); //限速牌类型名称
            else if (strcmp(attr.name(), "limspeed_desc") == 0)
                rec.limspeed_desc = attr.value(); //限速牌类型描述
        }

        //插入新数据到limspeed_type表中
        g_limspeed_type_table[rec.limspeed_type] = rec;
        cout << rec << endl;
    }

    //返回成功
    return RET_OK;
}


/*-------------------------------------------------------
 * limspeed: 限速牌
-------------------------------------------------------*/

//赋值函数
nad_record_limspeed & nad_record_limspeed::operator = (const nad_record_limspeed &rec)
{
    limspeed_id    = rec.limspeed_id;    //限速牌ID(经度纬度)
    limspeed_type  = rec.limspeed_type;  //限速牌类型
    limspeed_value = rec.limspeed_value; //限速牌当前取值(km/h)
    limspeed_ip    = rec.limspeed_ip;    //限速牌的IP
    lane_id        = rec.lane_id;        //管理道路的高速道ID
    return *this;
}

//使用主键查询limspeed表，失败返回NULL
nad_record_limspeed *db_query_limspeed(string limspeed_id)
{
    map<string, nad_record_limspeed>::iterator iter;
    iter = g_limspeed_table.find(limspeed_id);
    if (iter == g_limspeed_table.end())
    {
        return NULL;
    }
    else
    {
        return &(iter->second);
    }
}

//查询limspeed表的所有记录，不会失败
map<string, nad_record_limspeed> &db_query_all_limspeed()
{
    return g_limspeed_table;
}

//从xml加载limspeed表数据到db
static int nad_load_limspeed_to_db(xml_document &doc)
{
    //然后从xml加载limspeed表数据到db
    xml_node node = doc.child("db").child("limspeed_table");
    for (xml_node tmp_node = node.first_child(); tmp_node; tmp_node = tmp_node.next_sibling())
    {
        nad_record_limspeed rec;
        for (xml_attribute attr = tmp_node.first_attribute(); attr; attr = attr.next_attribute())
        {
            if (strcmp(attr.name(), "limspeed_id") == 0)
                rec.limspeed_id    = attr.value();                   //限速牌ID(经度纬度)
            else if (strcmp(attr.name(), "limspeed_type") == 0)
                rec.limspeed_type  = attr.value();                   //限速牌类型
            else if (strcmp(attr.name(), "limspeed_value") == 0)
                rec.limspeed_value = (uint)atol(attr.value());       //限速牌当前取值(km/h)
            else if (strcmp(attr.name(), "limspeed_ip") == 0)
                rec.limspeed_ip    = attr.value();                   //限速牌的IP
            else if (strcmp(attr.name(), "lane_id") == 0)
                rec.lane_id        = strtol(attr.value(), NULL, 10); //管理道路的高速道ID
        }

        //插入新数据到limspeed表中
        g_limspeed_table[rec.limspeed_id] = rec;
        cout << rec << endl;
    }

    //返回成功
    return RET_OK;
}


/*-------------------------------------------------------
 * block_type: 施工标志类型
-------------------------------------------------------*/

//赋值函数
nad_record_block_type & nad_record_block_type::operator = (const nad_record_block_type &rec)
{
    block_type = rec.block_type; //施工标志类型名称
    block_desc = rec.block_desc; //施工标志类型描述
    return *this;
}

//使用主键查询block_type表，失败返回NULL
nad_record_block_type *db_query_block_type(string block_type)
{
    map<string, nad_record_block_type>::iterator iter;
    iter = g_block_type_table.find(block_type);
    if (iter == g_block_type_table.end())
    {
        return NULL;
    }
    else
    {
        return &(iter->second);
    }
}

//查询block_type表的所有记录，不会失败
map<string, nad_record_block_type> &db_query_all_block_type()
{
    return g_block_type_table;
}

//从xml加载block_type表数据到db
static int nad_load_block_type_to_db(xml_document &doc)
{
    //然后从xml加载block_type表数据到db
    xml_node node = doc.child("db").child("block_type_table");
    for (xml_node tmp_node = node.first_child(); tmp_node; tmp_node = tmp_node.next_sibling())
    {
        nad_record_block_type rec;
        for (xml_attribute attr = tmp_node.first_attribute(); attr; attr = attr.next_attribute())
        {
            if (strcmp(attr.name(), "block_type") == 0)
                rec.block_type = attr.value(); //施工标志类型名称
            else if (strcmp(attr.name(), "block_desc") == 0)
                rec.block_desc = attr.value(); //施工标志类型描述
        }

        //插入新数据到block_type表中
        g_block_type_table[rec.block_type] = rec;
        cout << rec << endl;
    }

    //返回成功
    return RET_OK;
}


/*-------------------------------------------------------
 * block: 施工标志
-------------------------------------------------------*/

//赋值函数
nad_record_block & nad_record_block::operator = (const nad_record_block &rec)
{
    block_id    = rec.block_id;    //施工标志ID(经度纬度)
    block_type  = rec.block_type;  //施工标志类型
    block_value = rec.block_value; //施工标志当前取值
    lane_count  = rec.lane_count;  //管理的车道数
    block_ip    = rec.block_ip;    //施工标志的IP
    lane_id     = rec.lane_id;     //管理道路的高速道ID
    return *this;
}

//使用主键查询block表，失败返回NULL
nad_record_block *db_query_block(string block_id)
{
    map<string, nad_record_block>::iterator iter;
    iter = g_block_table.find(block_id);
    if (iter == g_block_table.end())
    {
        return NULL;
    }
    else
    {
        return &(iter->second);
    }
}

//查询block表的所有记录，不会失败
map<string, nad_record_block> &db_query_all_block()
{
    return g_block_table;
}

//从xml加载block表数据到db
static int nad_load_block_to_db(xml_document &doc)
{
    //然后从xml加载block表数据到db
    xml_node node = doc.child("db").child("block_table");
    for (xml_node tmp_node = node.first_child(); tmp_node; tmp_node = tmp_node.next_sibling())
    {
        nad_record_block rec;
        for (xml_attribute attr = tmp_node.first_attribute(); attr; attr = attr.next_attribute())
        {
            if (strcmp(attr.name(), "block_id") == 0)
                rec.block_id    = attr.value();                   //施工标志ID(经度纬度)
            else if (strcmp(attr.name(), "block_type") == 0)
                rec.block_type  = attr.value();                   //施工标志类型
            else if (strcmp(attr.name(), "block_value") == 0)
                rec.block_value = (uint)atol(attr.value());       //施工标志当前取值
            else if (strcmp(attr.name(), "lane_count") == 0)
                rec.lane_count  = (uint)atol(attr.value());       //管理的车道数
            else if (strcmp(attr.name(), "block_ip") == 0)
                rec.block_ip    = attr.value();                   //施工标志的IP
            else if (strcmp(attr.name(), "lane_id") == 0)
                rec.lane_id     = strtol(attr.value(), NULL, 10); //管理道路的高速道ID
        }

        //插入新数据到block表中
        g_block_table[rec.block_id] = rec;
        cout << rec << endl;
    }

    //返回成功
    return RET_OK;
}


/*-------------------------------------------------------
 * ts_3rd_type: 第三方交通系统类型
-------------------------------------------------------*/

//赋值函数
nad_record_ts_3rd_type & nad_record_ts_3rd_type::operator = (const nad_record_ts_3rd_type &rec)
{
    ts_3rd_type = rec.ts_3rd_type; //第三方交通系统类型名称
    ts_3rd_desc = rec.ts_3rd_desc; //第三方交通系统类型描述
    return *this;
}

//使用主键查询ts_3rd_type表，失败返回NULL
nad_record_ts_3rd_type *db_query_ts_3rd_type(string ts_3rd_type)
{
    map<string, nad_record_ts_3rd_type>::iterator iter;
    iter = g_ts_3rd_type_table.find(ts_3rd_type);
    if (iter == g_ts_3rd_type_table.end())
    {
        return NULL;
    }
    else
    {
        return &(iter->second);
    }
}

//查询ts_3rd_type表的所有记录，不会失败
map<string, nad_record_ts_3rd_type> &db_query_all_ts_3rd_type()
{
    return g_ts_3rd_type_table;
}

//从xml加载ts_3rd_type表数据到db
static int nad_load_ts_3rd_type_to_db(xml_document &doc)
{
    //然后从xml加载ts_3rd_type表数据到db
    xml_node node = doc.child("db").child("ts_3rd_type_table");
    for (xml_node tmp_node = node.first_child(); tmp_node; tmp_node = tmp_node.next_sibling())
    {
        nad_record_ts_3rd_type rec;
        for (xml_attribute attr = tmp_node.first_attribute(); attr; attr = attr.next_attribute())
        {
            if (strcmp(attr.name(), "ts_3rd_type") == 0)
                rec.ts_3rd_type = attr.value(); //第三方交通系统类型名称
            else if (strcmp(attr.name(), "ts_3rd_desc") == 0)
                rec.ts_3rd_desc = attr.value(); //第三方交通系统类型描述
        }

        //插入新数据到ts_3rd_type表中
        g_ts_3rd_type_table[rec.ts_3rd_type] = rec;
        cout << rec << endl;
    }

    //返回成功
    return RET_OK;
}


/*-------------------------------------------------------
 * ts_3rd: 第三方交通系统
-------------------------------------------------------*/

//赋值函数
nad_record_ts_3rd & nad_record_ts_3rd::operator = (const nad_record_ts_3rd &rec)
{
    ts_3rd_name     = rec.ts_3rd_name;     //第三方交通系统名称
    ts_3rd_type     = rec.ts_3rd_type;     //第三方交通系统类型
    ts_3rd_ip       = rec.ts_3rd_ip;       //第三方交通系统ip
    ts_3rd_port     = rec.ts_3rd_port;     //第三方交通系统端口
    ts_3rd_user     = rec.ts_3rd_user;     //第三方交通系统用户名
    ts_3rd_password = rec.ts_3rd_password; //第三方交通系统密码
    return *this;
}

//使用主键查询ts_3rd表，失败返回NULL
nad_record_ts_3rd *db_query_ts_3rd(string ts_3rd_name)
{
    map<string, nad_record_ts_3rd>::iterator iter;
    iter = g_ts_3rd_table.find(ts_3rd_name);
    if (iter == g_ts_3rd_table.end())
    {
        return NULL;
    }
    else
    {
        return &(iter->second);
    }
}

//查询ts_3rd表的所有记录，不会失败
map<string, nad_record_ts_3rd> &db_query_all_ts_3rd()
{
    return g_ts_3rd_table;
}

//从xml加载ts_3rd表数据到db
static int nad_load_ts_3rd_to_db(xml_document &doc)
{
    //然后从xml加载ts_3rd表数据到db
    xml_node node = doc.child("db").child("ts_3rd_table");
    for (xml_node tmp_node = node.first_child(); tmp_node; tmp_node = tmp_node.next_sibling())
    {
        nad_record_ts_3rd rec;
        for (xml_attribute attr = tmp_node.first_attribute(); attr; attr = attr.next_attribute())
        {
            if (strcmp(attr.name(), "ts_3rd_name") == 0)
                rec.ts_3rd_name     = attr.value();       //第三方交通系统名称
            else if (strcmp(attr.name(), "ts_3rd_type") == 0)
                rec.ts_3rd_type     = attr.value();       //第三方交通系统类型
            else if (strcmp(attr.name(), "ts_3rd_ip") == 0)
                rec.ts_3rd_ip       = attr.value();       //第三方交通系统ip
            else if (strcmp(attr.name(), "ts_3rd_port") == 0)
                rec.ts_3rd_port     = atol(attr.value()); //第三方交通系统端口
            else if (strcmp(attr.name(), "ts_3rd_user") == 0)
                rec.ts_3rd_user     = attr.value();       //第三方交通系统用户名
            else if (strcmp(attr.name(), "ts_3rd_password") == 0)
                rec.ts_3rd_password = attr.value();       //第三方交通系统密码
        }

        //插入新数据到ts_3rd表中
        g_ts_3rd_table[rec.ts_3rd_name] = rec;
        cout << rec << endl;
    }

    //返回成功
    return RET_OK;
}


/*-------------------------------------------------------
 * crossing_type: 路口类型
-------------------------------------------------------*/

//赋值函数
nad_record_crossing_type & nad_record_crossing_type::operator = (const nad_record_crossing_type &rec)
{
    crossing_type = rec.crossing_type; //路口类型名称
    crossing_desc = rec.crossing_desc; //路口类型描述
    return *this;
}

//使用主键查询crossing_type表，失败返回NULL
nad_record_crossing_type *db_query_crossing_type(string crossing_type)
{
    map<string, nad_record_crossing_type>::iterator iter;
    iter = g_crossing_type_table.find(crossing_type);
    if (iter == g_crossing_type_table.end())
    {
        return NULL;
    }
    else
    {
        return &(iter->second);
    }
}

//查询crossing_type表的所有记录，不会失败
map<string, nad_record_crossing_type> &db_query_all_crossing_type()
{
    return g_crossing_type_table;
}

//从xml加载crossing_type表数据到db
static int nad_load_crossing_type_to_db(xml_document &doc)
{
    //然后从xml加载crossing_type表数据到db
    xml_node node = doc.child("db").child("crossing_type_table");
    for (xml_node tmp_node = node.first_child(); tmp_node; tmp_node = tmp_node.next_sibling())
    {
        nad_record_crossing_type rec;
        for (xml_attribute attr = tmp_node.first_attribute(); attr; attr = attr.next_attribute())
        {
            if (strcmp(attr.name(), "crossing_type") == 0)
                rec.crossing_type = attr.value(); //路口类型名称
            else if (strcmp(attr.name(), "crossing_desc") == 0)
                rec.crossing_desc = attr.value(); //路口类型描述
        }

        //插入新数据到crossing_type表中
        g_crossing_type_table[rec.crossing_type] = rec;
        cout << rec << endl;
    }

    //返回成功
    return RET_OK;
}


/*-------------------------------------------------------
 * crossing: 路口
-------------------------------------------------------*/

//赋值函数
nad_record_crossing & nad_record_crossing::operator = (const nad_record_crossing &rec)
{
    crossing_id   = rec.crossing_id;   //路口ID
    crossing_type = rec.crossing_type; //路口类型
    return *this;
}

//使用主键查询crossing表，失败返回NULL
nad_record_crossing *db_query_crossing(string crossing_id)
{
    map<string, nad_record_crossing>::iterator iter;
    iter = g_crossing_table.find(crossing_id);
    if (iter == g_crossing_table.end())
    {
        return NULL;
    }
    else
    {
        return &(iter->second);
    }
}

//查询crossing表的所有记录，不会失败
map<string, nad_record_crossing> &db_query_all_crossing()
{
    return g_crossing_table;
}

//从xml加载crossing表数据到db
static int nad_load_crossing_to_db(xml_document &doc)
{
    //然后从xml加载crossing表数据到db
    xml_node node = doc.child("db").child("crossing_table");
    for (xml_node tmp_node = node.first_child(); tmp_node; tmp_node = tmp_node.next_sibling())
    {
        nad_record_crossing rec;
        for (xml_attribute attr = tmp_node.first_attribute(); attr; attr = attr.next_attribute())
        {
            if (strcmp(attr.name(), "crossing_id") == 0)
                rec.crossing_id   = attr.value(); //路口ID
            else if (strcmp(attr.name(), "crossing_type") == 0)
                rec.crossing_type = attr.value(); //路口类型
        }

        //插入新数据到crossing表中
        g_crossing_table[rec.crossing_id] = rec;
        cout << rec << endl;
    }

    //返回成功
    return RET_OK;
}


/*-------------------------------------------------------
 * conflict: 冲突表
-------------------------------------------------------*/

//赋值函数
nad_record_conflict & nad_record_conflict::operator = (const nad_record_conflict &rec)
{
    conflict_id   = rec.conflict_id;   //冲突表项ID
    light_id      = rec.light_id;      //对应的红绿灯
    active_point  = rec.active_point;  //激活点
    stop_point    = rec.stop_point;    //停止点
    exit_point    = rec.exit_point;    //离开点
    conflict_list = rec.conflict_list; //冲突表
    return *this;
}

//使用主键查询conflict表，失败返回NULL
nad_record_conflict *db_query_conflict(string conflict_id)
{
    map<string, nad_record_conflict>::iterator iter;
    iter = g_conflict_table.find(conflict_id);
    if (iter == g_conflict_table.end())
    {
        return NULL;
    }
    else
    {
        return &(iter->second);
    }
}

//查询conflict表的所有记录，不会失败
map<string, nad_record_conflict> &db_query_all_conflict()
{
    return g_conflict_table;
}

//从xml加载conflict表数据到db
static int nad_load_conflict_to_db(xml_document &doc)
{
    //然后从xml加载conflict表数据到db
    xml_node node = doc.child("db").child("conflict_table");
    for (xml_node tmp_node = node.first_child(); tmp_node; tmp_node = tmp_node.next_sibling())
    {
        nad_record_conflict rec;
        for (xml_attribute attr = tmp_node.first_attribute(); attr; attr = attr.next_attribute())
        {
            if (strcmp(attr.name(), "conflict_id") == 0)
                rec.conflict_id   = attr.value();                   //冲突表项ID
            else if (strcmp(attr.name(), "light_id") == 0)
                rec.light_id      = attr.value();                   //对应的红绿灯
            else if (strcmp(attr.name(), "active_point") == 0)
                rec.active_point  = strtol(attr.value(), NULL, 10); //激活点
            else if (strcmp(attr.name(), "stop_point") == 0)
                rec.stop_point    = strtol(attr.value(), NULL, 10); //停止点
            else if (strcmp(attr.name(), "exit_point") == 0)
                rec.exit_point    = strtol(attr.value(), NULL, 10); //离开点
            else if (strcmp(attr.name(), "conflict_list") == 0)
                rec.conflict_list = attr.value();                   //冲突表
        }

        //插入新数据到conflict表中
        g_conflict_table[rec.conflict_id] = rec;
        cout << rec << endl;
    }

    //返回成功
    return RET_OK;
}


/*-------------------------------------------------------
 * warning: 道路预警
-------------------------------------------------------*/

//赋值函数
nad_record_warning & nad_record_warning::operator = (const nad_record_warning &rec)
{
    warning_id   = rec.warning_id;   //预警ID
    warning_type = rec.warning_type; //预警类型
    warning_desc = rec.warning_desc; //预警描述
    lane_id      = rec.lane_id;      //管理道路的高速道ID
    return *this;
}

//使用主键查询warning表，失败返回NULL
nad_record_warning *db_query_warning(string warning_id)
{
    map<string, nad_record_warning>::iterator iter;
    iter = g_warning_table.find(warning_id);
    if (iter == g_warning_table.end())
    {
        return NULL;
    }
    else
    {
        return &(iter->second);
    }
}

//查询warning表的所有记录，不会失败
map<string, nad_record_warning> &db_query_all_warning()
{
    return g_warning_table;
}

//从xml加载warning表数据到db
static int nad_load_warning_to_db(xml_document &doc)
{
    //然后从xml加载warning表数据到db
    xml_node node = doc.child("db").child("warning_table");
    for (xml_node tmp_node = node.first_child(); tmp_node; tmp_node = tmp_node.next_sibling())
    {
        nad_record_warning rec;
        for (xml_attribute attr = tmp_node.first_attribute(); attr; attr = attr.next_attribute())
        {
            if (strcmp(attr.name(), "warning_id") == 0)
                rec.warning_id   = attr.value();                   //预警ID
            else if (strcmp(attr.name(), "warning_type") == 0)
                rec.warning_type = attr.value();                   //预警类型
            else if (strcmp(attr.name(), "warning_desc") == 0)
                rec.warning_desc = attr.value();                   //预警描述
            else if (strcmp(attr.name(), "lane_id") == 0)
                rec.lane_id      = strtol(attr.value(), NULL, 10); //管理道路的高速道ID
        }

        //插入新数据到warning表中
        g_warning_table[rec.warning_id] = rec;
        cout << rec << endl;
    }

    //返回成功
    return RET_OK;
}


/*-------------------------------------------------------
 * 初始化数据库和退出数据库
-------------------------------------------------------*/

//初始化数据库
int nad_db_init()
{
    //从db_type.xml读取数据，并写入到内存数据库
    nad_load_type_xml_to_db();

    //从db.xml读取数据，并写入到内存数据库
    nad_load_xml_to_db();

    //返回成功
    return RET_OK;
}

//退出数据库
void nad_db_free()
{
}

