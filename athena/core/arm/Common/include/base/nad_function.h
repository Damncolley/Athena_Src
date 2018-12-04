/*-------------------------------------------------------
 * 文件名：nad_function.h
 * 时  间：2016-03-02
 * 描  述：公共基础函数
-------------------------------------------------------*/
#ifndef _NAD_FUNCTION_H
#define _NAD_FUNCTION_H


//公共数据类型
#include "nad_type.h"

//公共返回码
#include "nad_retcode.h"

//公共枚举值
#include "nad_enum.h"

#include "LocalGeographicCS.hpp"
#include "config/Config.h"
#include "math_util.h"

/*-------------------------------------------------------
 * 公共基础函数测试桩
-------------------------------------------------------*/

void test_function();


/*-------------------------------------------------------
 * 公共基础函数
-------------------------------------------------------*/

//获取当前时间（毫秒）
int64_t get_current_time();

//获取当前时间（形如: 12:34:56）
string current_time_str();

//获取当前日期时间（形如: 2016-08-12 12:34:56）
string current_datetime_str();

//二进制字符串转十进制整数，用于block_value的转化
int32_t get_block_value(string block_value);

//十进制整数转二进制字符串，用于block_value的转化
string get_block_value(int32_t block_value, int lane_count);

//利用lon和lat来拼接出一个字符串"lon-lat-source"，用于给session当主键,默认source是""
string build_session_key(double lon, double lat, string source = "");

//从id中获得经度和纬度，兼容"lon|lat"和"lon|lat|source"两种情况
void get_lon_lat(double &lon, double &lat, string id);

//从id中获得经度、纬度和lane_id，没有lane_id填0
void get_lon_lat_id(double &lon, double &lat, int32_t &lane_id, string id);


/*-------------------------------------------------------
 * 字符串函数
-------------------------------------------------------*/

//求sub1,sub2在str中所夹的字符串，例如between("1#23%4", "#", "%")返回"23"
//sub1或sub2找不到返回“”。sub1为null则从头截取；sub2位null则截取到末尾
string sbetween(const char *str, const char *sub1, const char *sub2);
string sbetween(const string &str, const char *sub1, const char *sub2);

//求字符串前几个字符或后几个字符是不是sub
bool scontain(const char *str, const char *sub);
bool scontain(const string &str, const char *sub);
bool srcontain(const char *str, const char *sub);
bool srcontain(const string &str, const char *sub);

//剪切字符串
string &trim(string &str);     //修改了字符串本身
string strim(const char *str); //新建string保存结果
string strim(const string &str);

//截取字符串的左len长度或右len长度
string sleft(const char *str, const int len);
string sleft(const string &str, const int len);
string sright(const char *str, const int len);
string sright(const string &str, const int len);

//截取字符串的左(strlen-len)长度或右(strlen-len)长度
string sleftbut(const char *str, const int len);
string sleftbut(const string &str, const int len);
string srightbut(const char *str, const int len);
string srightbut(const string &str, const int len);

//查找替换
string sreplace(const char *str, const char *from_str, const char *to_str);
string sreplace(const string &str, const char *from_str, const char *to_str);

//是不是变量名
bool is_word(const char *str);
bool is_word(const string &str);

//是不是整数
bool is_int(const char *str);
bool is_int(const string &str);

//是不是浮点数
bool is_float(const char *str);
bool is_float(const string &str);

//大小写转化
string stoupper(const char *str);
string stoupper(const string &str);
string stolower(const char *str);
string stolower(const string &str);

//获得配置项(如"delay=300")的左侧(trim&&tolower)和右侧(trim)
string get_left(const char *str);
string get_left(const string &str);
string get_right(const char *str);
string get_right(const string &str);

//求子字符串sub在str中的出现次数，如"1,2,3"中","出现了2次
int sub_count(const char *str, const char *sub);
int sub_count(const string &str, const char *sub);


/*-------------------------------------------------------
 * 字符串数组类
-------------------------------------------------------*/

class stringlist : public vector<string>
{
public:
    //继承vector<string>的常用函数
    //assign(beg,end),at(idx),back(),begin(),capacity(),clear(),empty(),end()
    //erase(pos),erase(beg,end),front(),insert(pos,elem),insert(pos,n,elem)
    //insert(pos,beg,end),max_size(),pop_back(),push_back(elem),rbegin()
    //rend(),resize(num),reserve(),size(),swap(c1,c2)

    //从文件中加载，成功返回RET_OK
    int load_from_file(const char *filename);
    int load_from_file(const string &filename);

    //从文件中加载，成功返回RET_OK
    int save_to_file(const char *filename);
    int save_to_file(const string &filename);

    //从间隔字符串中加载，如str="a|b|c", part="|"，则插入三个字符串"a"、"b"、"c"
    void load_from_part(const char *str, const char *part);
    void load_from_part(const string &str, const char *part);

    //保存为间隔字符串，承上例，当part="|"时，返回"a|b|c"
    string save_to_part(const char *part);

    //从类似C语言的语句中加载，如cmd="const trim(str="wuhan") //test"，
    //则插入七个字符串"const"、"trim"、"("、"str"、"="、"wuhan"、")"
    //字符串已经去掉了双引号，//注释不会加入进来
    int load_from_cmd(const char *cmd);
    int load_from_cmd(const string &cmd);

    //所有的字符串做trim，del_blank=删除空字符串，del_comment=删除//注释
    void trim(bool del_blank = false, bool del_comment = false);

    //查找字符串，返回下表，找不到返回-1
    int find(const char *str);
    int find(const string &str);

    //调试打印
    void show();

    //测试代码
    void test();
};

/**
* @brief coord_transfer,完成经纬度和高斯投影面xy之间坐标的转换。
* 包含原点的设定
*/
class coord_transfer
{
public:
    LocalGeographicCS cs;

    coord_transfer();
    coord_transfer(std::string filename);
    coord_transfer(double lat, double lon);
    ~coord_transfer();

    void set_origin(std::string filename);
    void set_origin(double lat, double lon);
private:
    Config configSettings;
    double origin_lat = 0;///<投影原点纬度.
    double origin_lon = 0;///<投影原点经度.
};

/*-------------------------------------------------------
 * 文件函数
-------------------------------------------------------*/

//不同系统的路径间隔符
#ifdef _WIN32
    #define phc '\\'
    #define phs "\\"
#else
    #define phc '/'
    #define phs "/"
#endif

//补充完整路径，如"/opt/huawei"修改成"/opt/huawei/"
string full_path(const char *path);
string full_path(const string &path);

//获得路径名，如"/opt/huawei/demo.txt"返回"/opt/huawei"
string get_path(const char *path);
string get_path(const string &path);

//获得文件名，如"/opt/huawei/demo.txt"返回"demo.txt"
string get_filename(const char *path);
string get_filename(const string &path);

//获得基础名，如"/opt/huawei/demo.txt"返回"demo"
string get_basename(const char *path);
string get_basename(const string &path);

//获得扩展名，如"/opt/huawei/demo.txt"返回"txt"
string get_extname(const char *path);
string get_extname(const string &path);

//获得一个目录下的所有文件，path形如“d:\\z\\*.txt”，中文path注意字符集问题
void get_file_list(stringlist &result, const char *path);
void get_file_list(stringlist &result, const string &path);


/*-------------------------------------------------------
 * 坐标转换函数
-------------------------------------------------------*/
bool outOfChina(double lat, double lon) ;
double transformLat(double x, double y);
double transformLon(double x, double y);
/**
   * 地球坐标转换为火星坐标
   * World Geodetic System ==> Mars Geodetic System
   *
   * @param wgLat  地球坐标
   * @param wgLon
   *
   * mglat,mglon 火星坐标
   */
void transform2Mars(double wgLat, double wgLon,double &mgLat,double &mgLon);
/**
 * 火星坐标转换为地球坐标
 * Mars Geodetic System ==>World Geodetic System
 *
 * @param mglat 火星坐标
 * @param mglon
 *
 * wgLat  wgLon地球坐标
 */
void transform2Wgs(double mgLat, double mgLon, double &wgLat, double &wgLon);
void string2Wgs(string &id);

//从"点1->点2"的方向向右旋转90度构成"点1->点3"，且"点1-点3"间距meter米
void turn_right90(double x1, double y1, double x2, double y2, double offset, double &x3, double &y3);

/*-------------------------------------------------------
 *计算两点建的距离
-------------------------------------------------------*/
//判断两个对象距离在dist_meter米以内
bool dist_in_ll(double lat1, double lon1, double lat2, double lon2, double dist_meter);

//判断两个对象距离在dist_meter米以内
bool dist_in_xy(double x1, double y1, double x2, double y2, double dist_meter);

//两点间的距离
double dist_two_points_ll(double lat1, double lon1, double lat2, double lon2);

#endif /*_NAD_FUNCTION_H*/
