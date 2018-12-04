/*-------------------------------------------------------
 * 文件名：nad_db.h
 * 创建者：代码生成工具
 * 时  间：2016-12-20
 * 描  述：数据库的头文件
-------------------------------------------------------*/
#ifndef __NAD_DB_H__
#define __NAD_DB_H__


//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// 自动生成的代码，请勿修改
//!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


//引用base头文件
#include "nad_base.h"


/*-------------------------------------------------------
 * 初始化数据库的API函数
-------------------------------------------------------*/

//从db.xml(动态数据)加载数据，并保存到db中(std::map格式的内存数据库)
int nad_load_xml_to_db();

//从db_type.xml(静态数据)加载数据，并保存到db中(std::map格式的内存数据库)
int nad_load_type_xml_to_db();


/*-------------------------------------------------------
 * scene_type: 场景类型
-------------------------------------------------------*/

//scene_type表记录
class nad_record_scene_type
{
public:
    string scene_type;     //场景类型名称
    string scene_desc;     //场景类型描述
    double avg_speed;      //平均车速(km/h)
    double avg_lane_width; //平均道路宽度(cm)

    //赋值/打印调试信息
    nad_record_scene_type & operator = (const nad_record_scene_type &rec);
    friend inline ostream & operator << (ostream & os, nad_record_scene_type &rec)
    {
        cout << "scene_type: scene_type=\"" << rec.scene_type     //场景类型名称
             <<         "\", scene_desc=\"" << rec.scene_desc     //场景类型描述
             <<          "\", avg_speed=\"" << rec.avg_speed      //平均车速(km/h)
             <<     "\", avg_lane_width=\"" << rec.avg_lane_width //平均道路宽度(cm)
             <<                        "\"" << endl;
        return os;
    }
};

//使用主键查询scene_type表，失败返回NULL
nad_record_scene_type *db_query_scene_type(string scene_type);

//查询scene_type表的所有记录，不会失败
map<string, nad_record_scene_type> &db_query_all_scene_type();


/*-------------------------------------------------------
 * scene: 当前场景
-------------------------------------------------------*/

//scene表记录
class nad_record_scene
{
public:
    string scene_name; //场景名称
    string scene_type; //场景类型

    //赋值/打印调试信息
    nad_record_scene & operator = (const nad_record_scene &rec);
    friend inline ostream & operator << (ostream & os, nad_record_scene &rec)
    {
        cout << "scene: scene_name=\"" << rec.scene_name //场景名称
             <<    "\", scene_type=\"" << rec.scene_type //场景类型
             <<                   "\"" << endl;
        return os;
    }
};

//使用主键查询scene表，失败返回NULL
nad_record_scene *db_query_scene(string scene_name);

//查询scene表的所有记录，不会失败
map<string, nad_record_scene> &db_query_all_scene();


/*-------------------------------------------------------
 * csu_type: csu类型
-------------------------------------------------------*/

//csu_type表记录
class nad_record_csu_type
{
public:
    string csu_type; //csu类型名称
    string csu_desc; //csu类型描述

    //赋值/打印调试信息
    nad_record_csu_type & operator = (const nad_record_csu_type &rec);
    friend inline ostream & operator << (ostream & os, nad_record_csu_type &rec)
    {
        cout << "csu_type: csu_type=\"" << rec.csu_type //csu类型名称
             <<       "\", csu_desc=\"" << rec.csu_desc //csu类型描述
             <<                    "\"" << endl;
        return os;
    }
};

//使用主键查询csu_type表，失败返回NULL
nad_record_csu_type *db_query_csu_type(string csu_type);

//查询csu_type表的所有记录，不会失败
map<string, nad_record_csu_type> &db_query_all_csu_type();


/*-------------------------------------------------------
 * csu: csu
-------------------------------------------------------*/

//csu表记录
class nad_record_csu
{
public:
    string csu_name; //csu名称
    string csu_type; //csu类型

    //赋值/打印调试信息
    nad_record_csu & operator = (const nad_record_csu &rec);
    friend inline ostream & operator << (ostream & os, nad_record_csu &rec)
    {
        cout << "csu: csu_name=\"" << rec.csu_name //csu名称
             <<  "\", csu_type=\"" << rec.csu_type //csu类型
             <<               "\"" << endl;
        return os;
    }
};

//使用主键查询csu表，失败返回NULL
nad_record_csu *db_query_csu(string csu_name);

//查询csu表的所有记录，不会失败
map<string, nad_record_csu> &db_query_all_csu();


/*-------------------------------------------------------
 * csu_user: csu用户(用于OCT登录)
-------------------------------------------------------*/

//csu_user表记录
class nad_record_csu_user
{
public:
    string csu_user;     //csu登录用户名
    string csu_password; //csu登录密码

    //赋值/打印调试信息
    nad_record_csu_user & operator = (const nad_record_csu_user &rec);
    friend inline ostream & operator << (ostream & os, nad_record_csu_user &rec)
    {
        cout << "csu_user: csu_user=\"" << rec.csu_user     //csu登录用户名
             <<   "\", csu_password=\"" << rec.csu_password //csu登录密码
             <<                    "\"" << endl;
        return os;
    }
};

//使用主键查询csu_user表，失败返回NULL
nad_record_csu_user *db_query_csu_user(string csu_user);

//查询csu_user表的所有记录，不会失败
map<string, nad_record_csu_user> &db_query_all_csu_user();


/*-------------------------------------------------------
 * rsu_type: rsu类型
-------------------------------------------------------*/

//rsu_type表记录
class nad_record_rsu_type
{
public:
    string rsu_type; //rsu类型名称
    string rsu_desc; //rsu类型描述

    //赋值/打印调试信息
    nad_record_rsu_type & operator = (const nad_record_rsu_type &rec);
    friend inline ostream & operator << (ostream & os, nad_record_rsu_type &rec)
    {
        cout << "rsu_type: rsu_type=\"" << rec.rsu_type //rsu类型名称
             <<       "\", rsu_desc=\"" << rec.rsu_desc //rsu类型描述
             <<                    "\"" << endl;
        return os;
    }
};

//使用主键查询rsu_type表，失败返回NULL
nad_record_rsu_type *db_query_rsu_type(string rsu_type);

//查询rsu_type表的所有记录，不会失败
map<string, nad_record_rsu_type> &db_query_all_rsu_type();


/*-------------------------------------------------------
 * rsu: rsu
-------------------------------------------------------*/

//rsu表记录
class nad_record_rsu
{
public:
    string rsu_name;       //rsu名称
    string rsu_type;       //rsu类型
    double rsu_lon;        //rsu位置(经度)
    double rsu_lat;        //rsu位置(纬度)
    double rsu_radius_lon; //rsu覆盖半径(经度,米)
    double rsu_radius_lat; //rsu覆盖半径(纬度,米)

    //赋值/打印调试信息
    nad_record_rsu & operator = (const nad_record_rsu &rec);
    friend inline ostream & operator << (ostream & os, nad_record_rsu &rec)
    {
        cout <<      "rsu: rsu_name=\"" << rec.rsu_name       //rsu名称
             <<       "\", rsu_type=\"" << rec.rsu_type       //rsu类型
             <<        "\", rsu_lon=\"" << rec.rsu_lon        //rsu位置(经度)
             <<        "\", rsu_lat=\"" << rec.rsu_lat        //rsu位置(纬度)
             << "\", rsu_radius_lon=\"" << rec.rsu_radius_lon //rsu覆盖半径(经度,米)
             << "\", rsu_radius_lat=\"" << rec.rsu_radius_lat //rsu覆盖半径(纬度,米)
             <<                    "\"" << endl;
        return os;
    }
};

//使用主键查询rsu表，失败返回NULL
nad_record_rsu *db_query_rsu(string rsu_name);

//查询rsu表的所有记录，不会失败
map<string, nad_record_rsu> &db_query_all_rsu();


/*-------------------------------------------------------
 * obu_type: obu类型
-------------------------------------------------------*/

//obu_type表记录
class nad_record_obu_type
{
public:
    string obu_type; //obu类型名称
    string obu_desc; //obu类型描述

    //赋值/打印调试信息
    nad_record_obu_type & operator = (const nad_record_obu_type &rec);
    friend inline ostream & operator << (ostream & os, nad_record_obu_type &rec)
    {
        cout << "obu_type: obu_type=\"" << rec.obu_type //obu类型名称
             <<       "\", obu_desc=\"" << rec.obu_desc //obu类型描述
             <<                    "\"" << endl;
        return os;
    }
};

//使用主键查询obu_type表，失败返回NULL
nad_record_obu_type *db_query_obu_type(string obu_type);

//查询obu_type表的所有记录，不会失败
map<string, nad_record_obu_type> &db_query_all_obu_type();


/*-------------------------------------------------------
 * obu: obu
-------------------------------------------------------*/

//obu表记录
class nad_record_obu
{
public:
    string obu_name; //obu名称
    string obu_type; //obu类型

    //赋值/打印调试信息
    nad_record_obu & operator = (const nad_record_obu &rec);
    friend inline ostream & operator << (ostream & os, nad_record_obu &rec)
    {
        cout << "obu: obu_name=\"" << rec.obu_name //obu名称
             <<  "\", obu_type=\"" << rec.obu_type //obu类型
             <<               "\"" << endl;
        return os;
    }
};

//使用主键查询obu表，失败返回NULL
nad_record_obu *db_query_obu(string obu_name);

//查询obu表的所有记录，不会失败
map<string, nad_record_obu> &db_query_all_obu();


/*-------------------------------------------------------
 * oct_type: oct类型
-------------------------------------------------------*/

//oct_type表记录
class nad_record_oct_type
{
public:
    string oct_type; //oct类型名称
    string oct_desc; //oct类型描述

    //赋值/打印调试信息
    nad_record_oct_type & operator = (const nad_record_oct_type &rec);
    friend inline ostream & operator << (ostream & os, nad_record_oct_type &rec)
    {
        cout << "oct_type: oct_type=\"" << rec.oct_type //oct类型名称
             <<       "\", oct_desc=\"" << rec.oct_desc //oct类型描述
             <<                    "\"" << endl;
        return os;
    }
};

//使用主键查询oct_type表，失败返回NULL
nad_record_oct_type *db_query_oct_type(string oct_type);

//查询oct_type表的所有记录，不会失败
map<string, nad_record_oct_type> &db_query_all_oct_type();


/*-------------------------------------------------------
 * oct: oct
-------------------------------------------------------*/

//oct表记录
class nad_record_oct
{
public:
    string oct_name; //oct名称
    string oct_type; //oct类型

    //赋值/打印调试信息
    nad_record_oct & operator = (const nad_record_oct &rec);
    friend inline ostream & operator << (ostream & os, nad_record_oct &rec)
    {
        cout << "oct: oct_name=\"" << rec.oct_name //oct名称
             <<  "\", oct_type=\"" << rec.oct_type //oct类型
             <<               "\"" << endl;
        return os;
    }
};

//使用主键查询oct表，失败返回NULL
nad_record_oct *db_query_oct(string oct_name);

//查询oct表的所有记录，不会失败
map<string, nad_record_oct> &db_query_all_oct();


/*-------------------------------------------------------
 * rsd_type: 路侧采集器类型
-------------------------------------------------------*/

//rsd_type表记录
class nad_record_rsd_type
{
public:
    string rsd_type; //rsd类型名称
    string rsd_desc; //rsd类型描述

    //赋值/打印调试信息
    nad_record_rsd_type & operator = (const nad_record_rsd_type &rec);
    friend inline ostream & operator << (ostream & os, nad_record_rsd_type &rec)
    {
        cout << "rsd_type: rsd_type=\"" << rec.rsd_type //rsd类型名称
             <<       "\", rsd_desc=\"" << rec.rsd_desc //rsd类型描述
             <<                    "\"" << endl;
        return os;
    }
};

//使用主键查询rsd_type表，失败返回NULL
nad_record_rsd_type *db_query_rsd_type(string rsd_type);

//查询rsd_type表的所有记录，不会失败
map<string, nad_record_rsd_type> &db_query_all_rsd_type();


/*-------------------------------------------------------
 * rsd: 路侧采集器名称
-------------------------------------------------------*/

//rsd表记录
class nad_record_rsd
{
public:
    string rsd_name; //rsd名称
    string rsd_type; //rsd类型
    string rsd_ip;   //rsd的IP

    //赋值/打印调试信息
    nad_record_rsd & operator = (const nad_record_rsd &rec);
    friend inline ostream & operator << (ostream & os, nad_record_rsd &rec)
    {
        cout << "rsd: rsd_name=\"" << rec.rsd_name //rsd名称
             <<  "\", rsd_type=\"" << rec.rsd_type //rsd类型
             <<    "\", rsd_ip=\"" << rec.rsd_ip   //rsd的IP
             <<               "\"" << endl;
        return os;
    }
};

//使用主键查询rsd表，失败返回NULL
nad_record_rsd *db_query_rsd(string rsd_name);

//查询rsd表的所有记录，不会失败
map<string, nad_record_rsd> &db_query_all_rsd();


/*-------------------------------------------------------
 * rsd_sensor: RSD的传感器
-------------------------------------------------------*/

//rsd_sensor表记录
class nad_record_rsd_sensor
{
public:
    string rsd_sensor_name; //rsd传感器名称
    int    type;            //rsd传感器类型
    double lon;             //安装位置的经度
    double lat;             //安装位置的纬度
    double x;               //安装位置的x坐标
    double y;               //安装位置的y坐标
    double yaw;             //头指向(相对正北的夹角)
    int    range;           //有效扫描范围
    int    vertical_fov;    //垂直视场角
    int    horizontal_fov;  //水平视场角
    string ip;              //rsd传感器的IP
    int64  lane_id;         //管理道路的高速道ID

    //赋值/打印调试信息
    nad_record_rsd_sensor & operator = (const nad_record_rsd_sensor &rec);
    friend inline ostream & operator << (ostream & os, nad_record_rsd_sensor &rec)
    {
        cout << "rsd_sensor: rsd_sensor_name=\"" << rec.rsd_sensor_name //rsd传感器名称
             <<                    "\", type=\"" << rec.type            //rsd传感器类型
             <<                     "\", lon=\"" << rec.lon             //安装位置的经度
             <<                     "\", lat=\"" << rec.lat             //安装位置的纬度
             <<                       "\", x=\"" << rec.x               //安装位置的x坐标
             <<                       "\", y=\"" << rec.y               //安装位置的y坐标
             <<                     "\", yaw=\"" << rec.yaw             //头指向(相对正北的夹角)
             <<                   "\", range=\"" << rec.range           //有效扫描范围
             <<            "\", vertical_fov=\"" << rec.vertical_fov    //垂直视场角
             <<          "\", horizontal_fov=\"" << rec.horizontal_fov  //水平视场角
             <<                      "\", ip=\"" << rec.ip              //rsd传感器的IP
             <<                 "\", lane_id=\"" << rec.lane_id         //管理道路的高速道ID
             <<                             "\"" << endl;
        return os;
    }
};

//使用主键查询rsd_sensor表，失败返回NULL
nad_record_rsd_sensor *db_query_rsd_sensor(string rsd_sensor_name);

//查询rsd_sensor表的所有记录，不会失败
map<string, nad_record_rsd_sensor> &db_query_all_rsd_sensor();


/*-------------------------------------------------------
 * light_type: 红绿灯类型
-------------------------------------------------------*/

//light_type表记录
class nad_record_light_type
{
public:
    string light_type; //红绿灯类型名称
    string light_desc; //红绿灯类型描述

    //赋值/打印调试信息
    nad_record_light_type & operator = (const nad_record_light_type &rec);
    friend inline ostream & operator << (ostream & os, nad_record_light_type &rec)
    {
        cout << "light_type: light_type=\"" << rec.light_type //红绿灯类型名称
             <<         "\", light_desc=\"" << rec.light_desc //红绿灯类型描述
             <<                        "\"" << endl;
        return os;
    }
};

//使用主键查询light_type表，失败返回NULL
nad_record_light_type *db_query_light_type(string light_type);

//查询light_type表的所有记录，不会失败
map<string, nad_record_light_type> &db_query_all_light_type();


/*-------------------------------------------------------
 * light: 红绿灯
-------------------------------------------------------*/

//light表记录
class nad_record_light
{
public:
    string light_id;     //红绿灯ID(经度纬度)
    string light_type;   //红绿灯类型
    uint   light_status; //红绿灯状态
    string light_ip;     //红绿灯的IP
    int64  stop_lane_id; //管理道路的高速道ID

    //赋值/打印调试信息
    nad_record_light & operator = (const nad_record_light &rec);
    friend inline ostream & operator << (ostream & os, nad_record_light &rec)
    {
        cout <<  "light: light_id=\"" << rec.light_id     //红绿灯ID(经度纬度)
             <<   "\", light_type=\"" << rec.light_type   //红绿灯类型
             << "\", light_status=\"" << rec.light_status //红绿灯状态
             <<     "\", light_ip=\"" << rec.light_ip     //红绿灯的IP
             << "\", stop_lane_id=\"" << rec.stop_lane_id //管理道路的高速道ID
             <<                  "\"" << endl;
        return os;
    }
};

//使用主键查询light表，失败返回NULL
nad_record_light *db_query_light(string light_id);

//查询light表的所有记录，不会失败
map<string, nad_record_light> &db_query_all_light();


/*-------------------------------------------------------
 * limspeed_type: 限速牌类型
-------------------------------------------------------*/

//limspeed_type表记录
class nad_record_limspeed_type
{
public:
    string limspeed_type; //限速牌类型名称
    string limspeed_desc; //限速牌类型描述

    //赋值/打印调试信息
    nad_record_limspeed_type & operator = (const nad_record_limspeed_type &rec);
    friend inline ostream & operator << (ostream & os, nad_record_limspeed_type &rec)
    {
        cout << "limspeed_type: limspeed_type=\"" << rec.limspeed_type //限速牌类型名称
             <<            "\", limspeed_desc=\"" << rec.limspeed_desc //限速牌类型描述
             <<                              "\"" << endl;
        return os;
    }
};

//使用主键查询limspeed_type表，失败返回NULL
nad_record_limspeed_type *db_query_limspeed_type(string limspeed_type);

//查询limspeed_type表的所有记录，不会失败
map<string, nad_record_limspeed_type> &db_query_all_limspeed_type();


/*-------------------------------------------------------
 * limspeed: 限速牌
-------------------------------------------------------*/

//limspeed表记录
class nad_record_limspeed
{
public:
    string limspeed_id;    //限速牌ID(经度纬度)
    string limspeed_type;  //限速牌类型
    uint   limspeed_value; //限速牌当前取值(km/h)
    string limspeed_ip;    //限速牌的IP
    int64  lane_id;        //管理道路的高速道ID

    //赋值/打印调试信息
    nad_record_limspeed & operator = (const nad_record_limspeed &rec);
    friend inline ostream & operator << (ostream & os, nad_record_limspeed &rec)
    {
        cout << "limspeed: limspeed_id=\"" << rec.limspeed_id    //限速牌ID(经度纬度)
             <<     "\", limspeed_type=\"" << rec.limspeed_type  //限速牌类型
             <<    "\", limspeed_value=\"" << rec.limspeed_value //限速牌当前取值(km/h)
             <<       "\", limspeed_ip=\"" << rec.limspeed_ip    //限速牌的IP
             <<           "\", lane_id=\"" << rec.lane_id        //管理道路的高速道ID
             <<                       "\"" << endl;
        return os;
    }
};

//使用主键查询limspeed表，失败返回NULL
nad_record_limspeed *db_query_limspeed(string limspeed_id);

//查询limspeed表的所有记录，不会失败
map<string, nad_record_limspeed> &db_query_all_limspeed();


/*-------------------------------------------------------
 * block_type: 施工标志类型
-------------------------------------------------------*/

//block_type表记录
class nad_record_block_type
{
public:
    string block_type; //施工标志类型名称
    string block_desc; //施工标志类型描述

    //赋值/打印调试信息
    nad_record_block_type & operator = (const nad_record_block_type &rec);
    friend inline ostream & operator << (ostream & os, nad_record_block_type &rec)
    {
        cout << "block_type: block_type=\"" << rec.block_type //施工标志类型名称
             <<         "\", block_desc=\"" << rec.block_desc //施工标志类型描述
             <<                        "\"" << endl;
        return os;
    }
};

//使用主键查询block_type表，失败返回NULL
nad_record_block_type *db_query_block_type(string block_type);

//查询block_type表的所有记录，不会失败
map<string, nad_record_block_type> &db_query_all_block_type();


/*-------------------------------------------------------
 * block: 施工标志
-------------------------------------------------------*/

//block表记录
class nad_record_block
{
public:
    string block_id;    //施工标志ID(经度纬度)
    string block_type;  //施工标志类型
    uint   block_value; //施工标志当前取值
    uint   lane_count;  //管理的车道数
    string block_ip;    //施工标志的IP
    int64  lane_id;     //管理道路的高速道ID

    //赋值/打印调试信息
    nad_record_block & operator = (const nad_record_block &rec);
    friend inline ostream & operator << (ostream & os, nad_record_block &rec)
    {
        cout << "block: block_id=\"" << rec.block_id    //施工标志ID(经度纬度)
             <<  "\", block_type=\"" << rec.block_type  //施工标志类型
             << "\", block_value=\"" << rec.block_value //施工标志当前取值
             <<  "\", lane_count=\"" << rec.lane_count  //管理的车道数
             <<    "\", block_ip=\"" << rec.block_ip    //施工标志的IP
             <<     "\", lane_id=\"" << rec.lane_id     //管理道路的高速道ID
             <<                 "\"" << endl;
        return os;
    }
};

//使用主键查询block表，失败返回NULL
nad_record_block *db_query_block(string block_id);

//查询block表的所有记录，不会失败
map<string, nad_record_block> &db_query_all_block();


/*-------------------------------------------------------
 * ts_3rd_type: 第三方交通系统类型
-------------------------------------------------------*/

//ts_3rd_type表记录
class nad_record_ts_3rd_type
{
public:
    string ts_3rd_type; //第三方交通系统类型名称
    string ts_3rd_desc; //第三方交通系统类型描述

    //赋值/打印调试信息
    nad_record_ts_3rd_type & operator = (const nad_record_ts_3rd_type &rec);
    friend inline ostream & operator << (ostream & os, nad_record_ts_3rd_type &rec)
    {
        cout << "ts_3rd_type: ts_3rd_type=\"" << rec.ts_3rd_type //第三方交通系统类型名称
             <<          "\", ts_3rd_desc=\"" << rec.ts_3rd_desc //第三方交通系统类型描述
             <<                          "\"" << endl;
        return os;
    }
};

//使用主键查询ts_3rd_type表，失败返回NULL
nad_record_ts_3rd_type *db_query_ts_3rd_type(string ts_3rd_type);

//查询ts_3rd_type表的所有记录，不会失败
map<string, nad_record_ts_3rd_type> &db_query_all_ts_3rd_type();


/*-------------------------------------------------------
 * ts_3rd: 第三方交通系统
-------------------------------------------------------*/

//ts_3rd表记录
class nad_record_ts_3rd
{
public:
    string ts_3rd_name;     //第三方交通系统名称
    string ts_3rd_type;     //第三方交通系统类型
    string ts_3rd_ip;       //第三方交通系统ip
    int    ts_3rd_port;     //第三方交通系统端口
    string ts_3rd_user;     //第三方交通系统用户名
    string ts_3rd_password; //第三方交通系统密码

    //赋值/打印调试信息
    nad_record_ts_3rd & operator = (const nad_record_ts_3rd &rec);
    friend inline ostream & operator << (ostream & os, nad_record_ts_3rd &rec)
    {
        cout << "ts_3rd: ts_3rd_name=\"" << rec.ts_3rd_name     //第三方交通系统名称
             <<     "\", ts_3rd_type=\"" << rec.ts_3rd_type     //第三方交通系统类型
             <<       "\", ts_3rd_ip=\"" << rec.ts_3rd_ip       //第三方交通系统ip
             <<     "\", ts_3rd_port=\"" << rec.ts_3rd_port     //第三方交通系统端口
             <<     "\", ts_3rd_user=\"" << rec.ts_3rd_user     //第三方交通系统用户名
             << "\", ts_3rd_password=\"" << rec.ts_3rd_password //第三方交通系统密码
             <<                     "\"" << endl;
        return os;
    }
};

//使用主键查询ts_3rd表，失败返回NULL
nad_record_ts_3rd *db_query_ts_3rd(string ts_3rd_name);

//查询ts_3rd表的所有记录，不会失败
map<string, nad_record_ts_3rd> &db_query_all_ts_3rd();


/*-------------------------------------------------------
 * crossing_type: 路口类型
-------------------------------------------------------*/

//crossing_type表记录
class nad_record_crossing_type
{
public:
    string crossing_type; //路口类型名称
    string crossing_desc; //路口类型描述

    //赋值/打印调试信息
    nad_record_crossing_type & operator = (const nad_record_crossing_type &rec);
    friend inline ostream & operator << (ostream & os, nad_record_crossing_type &rec)
    {
        cout << "crossing_type: crossing_type=\"" << rec.crossing_type //路口类型名称
             <<            "\", crossing_desc=\"" << rec.crossing_desc //路口类型描述
             <<                              "\"" << endl;
        return os;
    }
};

//使用主键查询crossing_type表，失败返回NULL
nad_record_crossing_type *db_query_crossing_type(string crossing_type);

//查询crossing_type表的所有记录，不会失败
map<string, nad_record_crossing_type> &db_query_all_crossing_type();


/*-------------------------------------------------------
 * crossing: 路口
-------------------------------------------------------*/

//crossing表记录
class nad_record_crossing
{
public:
    string crossing_id;   //路口ID
    string crossing_type; //路口类型

    //赋值/打印调试信息
    nad_record_crossing & operator = (const nad_record_crossing &rec);
    friend inline ostream & operator << (ostream & os, nad_record_crossing &rec)
    {
        cout << "crossing: crossing_id=\"" << rec.crossing_id   //路口ID
             <<     "\", crossing_type=\"" << rec.crossing_type //路口类型
             <<                       "\"" << endl;
        return os;
    }
};

//使用主键查询crossing表，失败返回NULL
nad_record_crossing *db_query_crossing(string crossing_id);

//查询crossing表的所有记录，不会失败
map<string, nad_record_crossing> &db_query_all_crossing();


/*-------------------------------------------------------
 * conflict: 冲突表
-------------------------------------------------------*/

//conflict表记录
class nad_record_conflict
{
public:
    string conflict_id;   //冲突表项ID
    string light_id;      //对应的红绿灯
    int64  active_point;  //激活点
    int64  stop_point;    //停止点
    int64  exit_point;    //离开点
    string conflict_list; //冲突表

    //赋值/打印调试信息
    nad_record_conflict & operator = (const nad_record_conflict &rec);
    friend inline ostream & operator << (ostream & os, nad_record_conflict &rec)
    {
        cout << "conflict: conflict_id=\"" << rec.conflict_id   //冲突表项ID
             <<          "\", light_id=\"" << rec.light_id      //对应的红绿灯
             <<      "\", active_point=\"" << rec.active_point  //激活点
             <<        "\", stop_point=\"" << rec.stop_point    //停止点
             <<        "\", exit_point=\"" << rec.exit_point    //离开点
             <<     "\", conflict_list=\"" << rec.conflict_list //冲突表
             <<                       "\"" << endl;
        return os;
    }
};

//使用主键查询conflict表，失败返回NULL
nad_record_conflict *db_query_conflict(string conflict_id);

//查询conflict表的所有记录，不会失败
map<string, nad_record_conflict> &db_query_all_conflict();


/*-------------------------------------------------------
 * warning: 道路预警
-------------------------------------------------------*/

//warning表记录
class nad_record_warning
{
public:
    string warning_id;   //预警ID
    string warning_type; //预警类型
    string warning_desc; //预警描述
    int64  lane_id;      //管理道路的高速道ID

    //赋值/打印调试信息
    nad_record_warning & operator = (const nad_record_warning &rec);
    friend inline ostream & operator << (ostream & os, nad_record_warning &rec)
    {
        cout << "warning: warning_id=\"" << rec.warning_id   //预警ID
             <<    "\", warning_type=\"" << rec.warning_type //预警类型
             <<    "\", warning_desc=\"" << rec.warning_desc //预警描述
             <<         "\", lane_id=\"" << rec.lane_id      //管理道路的高速道ID
             <<                     "\"" << endl;
        return os;
    }
};

//使用主键查询warning表，失败返回NULL
nad_record_warning *db_query_warning(string warning_id);

//查询warning表的所有记录，不会失败
map<string, nad_record_warning> &db_query_all_warning();


/*-------------------------------------------------------
 * 初始化数据库和退出数据库
-------------------------------------------------------*/

//初始化数据库
int nad_db_init();

//退出数据库
void nad_db_free();


#endif /*__NAD_DB_H__*/
