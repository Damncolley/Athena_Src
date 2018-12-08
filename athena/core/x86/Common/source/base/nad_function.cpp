/*-------------------------------------------------------
 * 文件名：nad_function.cpp
 * 创建者：张毅00151602
 * 时  间：2016-03-02
 * 描  述：公共基础函数
-------------------------------------------------------*/


//头文件

#include "nad_function.h"

/*-------------------------------------------------------
 * 公共基础函数测试桩
-------------------------------------------------------*/

void test_function()
{
#ifdef _WIN32
    const char *path = "d:\\test\\demo.txt";
#else
    const char *path = "/opt/test/demo.txt";
#endif // _WIN32

    cout << "full_path()=" << full_path(path).c_str() << endl;
    cout << "get_path()=" << get_path(path).c_str() << endl;
    cout << "get_filename()=" << get_filename(path).c_str() << endl;
    cout << "get_basename()=" << get_basename(path).c_str() << endl;
    cout << "get_extname()=" << get_extname(path).c_str() << endl;
}

/*-------------------------------------------------------
 * 公共基础函数
-------------------------------------------------------*/

//获取当前时间（毫秒）
int64_t get_current_time()
{
    struct timeval tv;
    struct timezone tz;
    gettimeofday(&tv, &tz);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

//获取当前时间（形如: 12:34:56）
string current_time_str()
{
    time_t timep;
    struct tm *p;
    time(&timep);
    p = localtime(&timep);
    char buf[40]; //形如: 12:34:56，一共19个字节
    sprintf(buf, "%.2d:%.2d:%.2d",
        p->tm_hour, p->tm_min, p->tm_sec);
    return buf;
}

//获取当前日期时间（形如: 2016-08-12 12:34:56）
string current_datetime_str()
{
    time_t timep;
    struct tm *p;
    time(&timep);
    p = localtime(&timep);
    char buf[40]; //形如: 2016-08-12 12:34:56，一共19个字节
    sprintf(buf, "%.4d-%.2d-%.2d %.2d:%.2d:%.2d",
        p->tm_year + 1900, p->tm_mon + 1, p->tm_mday, p->tm_hour, p->tm_min, p->tm_sec);
    return buf;
}

//二进制字符串转十进制整数，用于block_value的转化
int32_t get_block_value(string block_value)
{
    unsigned int len = block_value.length();
    int32_t sum = 0;
    for(unsigned int i=0; i<len; i++)
    {
        int32_t tmp = boost::lexical_cast<int32_t>(block_value.at(i));
        sum += (tmp << (len-1-i));
    }
    return sum;
}

//十进制整数转二进制字符串，用于block_value的转化
string get_block_value(int block_value, int lane_count)
{
    int num = 0;
    char block_lane[lane_count];
    while(1)
    {
        block_lane[num] = ((block_value >> (lane_count-1-num)) & 1) + '0';
        num++;
        if(num == lane_count)
        {
            block_lane[num] = '\0';
            break;
        }
    }
    return string(block_lane);
}

//利用lon和lat来拼接出一个字符串"lon|lat|source"，用于给session当主键,默认source是""
string build_session_key(double lon, double lat, string source)
{
    string ans;
    char tmp[100];
    if(source == "")
    {
        sprintf(tmp, "%.7lf|%.7lf", lon, lat);
    }
    else
    {
        sprintf(tmp, "%.7lf|%.7lf|%s", lon, lat, source.c_str());
    }
    ans = tmp;
    return ans;
}

//从id中获得经度和纬度，兼容"lon|lat"和"lon|lat|source"两种情况
void get_lon_lat(double &lon, double &lat, string id)
{
    stringlist ls;
    ls.load_from_part(id, "|");
    if (ls.size() >= 2)
    {
        lon = atof(ls[0].c_str());
        lat = atof(ls[1].c_str());
    }
}

//从id中获得经度、纬度和lane_id，没有lane_id填0
void get_lon_lat_id(double &lon, double &lat, int32_t &lane_id, string id)
{
    stringlist ls;
    ls.load_from_part(id, "|");
    if (ls.size() >= 2)
    {
        lon = atof(ls[0].c_str());
        lat = atof(ls[1].c_str());
    }
    lane_id = 0;
    if (ls.size() >= 3)
    {
        lane_id = atoi(ls[2].c_str());
    }
}


/*-------------------------------------------------------
 * 字符串函数
-------------------------------------------------------*/

//求sub1,sub2在str中所夹的字符串，例如between("1#23%4", "#", "%")返回"23"
//sub1或sub2找不到返回“”。sub1为null则从头截取；sub2位null则截取到末尾
string sbetween(const char *str, const char *sub1, const char *sub2)
{
    string ret("");
    if (str == NULL)
    {
        return ret;
    }

    //寻找起点
    const char *s1 = str;
    if ((sub1 != NULL) && (*sub1 != '\0'))
    {
        s1 = strstr(str, sub1);
        if (s1 == NULL)
        {
            return ret;
        }
        s1 += strlen(sub1);
    }

    //结尾
    if ((sub2 != NULL) && (*sub2 != '\0'))
    {
        const char *s2 = strstr(s1, sub2);
        if (s2 == NULL)
        {
            return ret;
        }
        ret.assign(s1, s2 - s1);
    }
    else
    {
        ret.assign(s1);
    }
    return ret;
}

string sbetween(const string &str, const char *sub1, const char *sub2)
{
    return sbetween(str.c_str(), sub1, sub2);
}

//求字符串前几个字符是不是sub
bool scontain(const char *str, const char *sub)
{
    //处理无效情况
    if ((str == NULL) || (sub == NULL))
    {
        return false;
    }

    //获得字符串长度
    size_t l1 = strlen(str);
    size_t l2 = strlen(sub);

    //处理无效情况
    if (l1 < l2)
    {
        return false;
    }
    if (l2 == 0)
    {
        return true;
    }

    //开始比较
    return (strncmp(str, sub, l2) == 0);
}


bool scontain(const string &str, const char *sub)
{
    return scontain(str.c_str(), sub);
}

//求字符串后几个字符是不是sub
bool srcontain(const char *str, const char *sub)
{
    //处理无效情况
    if ((str == NULL) || (sub == NULL))
    {
        return false;
    }

    //获得字符串长度
    size_t l1 = strlen(str);
    size_t l2 = strlen(sub);

    //处理无效情况
    if (l1 < l2)
    {
        return false;
    }

    if (l2 == 0)
    {
        return true;
    }

    //开始比较
    return (strncmp(str + l1 - l2, sub, l2) == 0);
}

bool srcontain(const string &str, const char *sub)
{
    return srcontain(str.c_str(), sub);
}

//剪切字符串
string &trim(string &str)
{
    if(!str.empty())
    {
        str.erase(0, str.find_first_not_of(" \n\r\t"));
        str.erase(str.find_last_not_of(" \n\r\t") + 1);
    }
    return str;
}
string strim(const char *str)
{
    string ret(str);
    trim(ret);
    return ret;
}
string strim(const string &str)
{
    return strim(str.c_str());
}

//截取字符串的左len长度或右len长度
string sleft(const char *str, const int len)
{
    string ret("");
    int l = min((int)strlen(str), len);
    if (l > 0)
    {
        ret.assign(str, 0, l);
    }
    return ret;
}

string sleft(const string &str, const int len)
{
    return sleft(str.c_str(), len);
}

//截取字符串的左len长度或右len长度
string sright(const char *str, const int len)
{
    string ret("");
    int l = min((int)strlen(str), len);
    if (l > 0)
    {
        ret.assign(str, strlen(str) - l, l);
    }
    return ret;
}

string sright(const string &str, const int len)
{
    return sright(str.c_str(), len);
}

//截取字符串的左(strlen-len)长度
string sleftbut(const char *str, const int len)
{
    string ret("");
    int l = max(0, (int)strlen(str) - len);
    if (l > 0)
    {
        ret.assign(str, 0, l);
    }
    return ret;
}
string sleftbut(const string &str, const int len)
{
    return sleftbut(str.c_str(), len);
}

//截取字符串的右(strlen-len)长度
string srightbut(const char *str, const int len)
{
    string ret("");
    int l = max(0, (int)strlen(str) - len);
    if (l > 0)
    {
        ret.assign(str, strlen(str) - l, l);
    }
    return ret;
}

string srightbut(const string &str, const int len)
{
    return srightbut(str.c_str(), len);
}

//查找替换
string sreplace(const char *str, const char *from_str, const char *to_str)
{
    string ret(str);
    int pos = ret.find(from_str);
    while (pos != -1)
    {
        ret.replace(pos, strlen(from_str), to_str);
        pos = ret.find(from_str);
    }
    return ret;
}

string sreplace(const string &str, const char *from_str, const char *to_str)
{
    return sreplace(str.c_str(), from_str, to_str);
}

//获得字符串类型
static int char_type(const char c)
{
    if (c == '\0')
    {
        //０字符
        return 0;
    }
    else if ((c == '\t') || (c == ' '))
    {
        //空格
        return 1;
    }
    else if ((c >= '0') && (c <= '9'))
    {
        //数字
        return 2;
    }
    else if (((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')) || (c == '_'))
    {
        //字母和下划线
        return 3;
    }
    else if (c == '"')
    {
        //双引号
        return 4;
    }
    else if ((c >= '!')  && (c <= '~'))
    {
        //符号
        return 5;
    }
    else
    {
        //中文/utf编码区/不可视字符
        return 6;
    }
}

//是不是变量名
bool is_word(const char *str)
{
    if ((str == NULL) || (*str == '\0'))
    {
        return false;
    }
    int len = strlen(str);

    //循环判断数据类型
    for (int i = 0; i < len; i++)
    {
        int t = char_type(str[i]);
        if (i == 0)
        {
            if (t != 3)
            {
                return false;
            }
        }
        else
        {
            if (t != 2 && t != 3)
            {
                return false;
            }
        }
    }
    return true;
}
bool is_word(const string &str)
{
    return is_word(str.c_str());
}

//是不是整数
bool is_int(const char *str)
{
    if ((str == NULL) || (*str == '\0'))
    {
        return false;
    }
    int len = strlen(str);

    //循环判断数据类型
    for (int i = 0; i < len; i++)
    {
        int t = char_type(str[i]);
        if (t != 2)
        {
            return false;
        }
    }
    return true;
}
bool is_int(const string &str)
{
    return is_int(str.c_str());
}

//是不是浮点数
bool is_float(const char *str)
{
    if ((str == NULL) || (*str == '\0'))
    {
        return false;
    }
    int len = strlen(str);

    //循环判断数据类型
    bool hasp = false;
    for (int i = 0; i < len; i++)
    {
        int t = char_type(str[i]);
        if (t != 2)
        {
            if (!hasp && i > 0 && i < (len - 1) && str[i] == '.')
            {
                hasp = true; //找到了小数点
            }
            else
            {
                return false;
            }
        }
    }
    return true;
}
bool is_float(const string &str)
{
    return is_float(str.c_str());
}

//大写转化
string stoupper(const char *str)
{
    string ret = str;
    transform(ret.begin(), ret.end(), ret.begin(), ::toupper);
    return ret;
}
string stoupper(const string &str)
{
    return stoupper(str.c_str());
}

//小写转化
string stolower(const char *str)
{
    string ret = str;
    transform(ret.begin(), ret.end(), ret.begin(), ::tolower);
    return ret;
}
string stolower(const string &str)
{
    return stolower(str.c_str());
}

//获得配置项(如"delay=300")的左侧(trim&&tolower)
string get_left(const char *str)
{
    return stolower(strim(sbetween(str, NULL, "=")));
}
string get_left(const string &str)
{
    return get_left(str.c_str());
}

//获得配置项(如"delay=300")的右侧(trim)
string get_right(const char *str)
{
    return strim(sbetween(str, "=", NULL));
}
string get_right(const string &str)
{
    return get_right(str.c_str());
}

//求子字符串sub在str中的出现次数，如"1,2,3"中","出现了2次
int sub_count(const char *str, const char *sub)
{
    int a = (int)strlen(sub);
    int b = (int)strlen(str) - a;
    int ret = 0;
    for (int i = 0; i < b; i++)
    {
        if (strncmp(str + i, sub, a) == 0)
        {
            ret++;
            i += (a - 1);
        }
    }
    return ret;
}
int sub_count(const string &str, const char *sub)
{
    return sub_count(str.c_str(), sub);
}


/*-------------------------------------------------------
 * 字符串数组类
-------------------------------------------------------*/

//从文件中加载，成功返回RET_OK
int stringlist::load_from_file(const char *filename)
{
    //清空旧数据
    clear();

    //打开文件
    ifstream fin(filename);
    if(!fin.good())
    {
        cout << "stringlist::load_from_file(" << filename << ") read error!" << endl;
        return RET_FILE_ERR;
    }

    //逐行读取内容
    string line;
    while (!fin.eof())
    {
        getline(fin, line);
        push_back(line);
    }

    //读取成功
    return RET_OK;
}
int stringlist::load_from_file(const string &filename)
{
    return load_from_file(filename.c_str());
}

//从文件中加载，成功返回RET_OK
int stringlist::save_to_file(const char *filename)
{
    //打开文件
    ofstream fout(filename);
    if(!fout.good())
    {
        cout << "stringlist::save_to_file(" << filename << ") write error!" << endl;
        return RET_FILE_ERR;
    }

    //逐行写入内容
    for (size_t i = 0; i < size(); i++)
    {
        fout << at(i) << endl;
    }

    //读取成功
    return RET_OK;
}
int stringlist::save_to_file(const string &filename)
{
    return save_to_file(filename.c_str());
}

//从间隔字符串中加载，如str="a|b|c", part="|"，则插入三个字符串"a"、"b"、"c"
void stringlist::load_from_part(const char *str, const char *part)
{
    //清空旧数据
    clear();

    //变量定义
    string word;
    const char *a = str;
    const char *b = strstr(a, part);
    int len = strlen(part);

    //添加有间隔符
    while (b != NULL)
    {
        word.assign(a, b - a);
        push_back(word);
        a = b + len;
        b = strstr(a, part);
    }

    //添加末尾无间隔符，空字符串不追加
    if (size() > 0 || *str != '\0')
    {
        word.assign(a, strlen(str) + str - a);
        push_back(word);
    }
}
void stringlist::load_from_part(const string &str, const char *part)
{
    load_from_part(str.c_str(), part);
}

//保存为间隔字符串，承上例，当part="|"时，返回"a|b|c"
string stringlist::save_to_part(const char *part)
{
    string line;
    for (size_t i = 0; i < size(); i++)
    {
        line += at(i);
        if (i < (size() - 1))
        {
            line += part;
        }
    }
    return line;
}

//从类似C语言的语句中加载，如cmd="const trim(str="wuhan") //test"，
//则插入七个字符串"const"、"trim"、"("、"str"、"="、"wuhan"、")"
//字符串已经去掉了双引号，//注释不会加入进来
int stringlist::load_from_cmd(const char *cmd)
{
    //清空旧数据
    clear();

    //变量定义
    int t;
    string word, tmp;
    const char *a = cmd;
    const char *b = strstr(a, "//");
    if (b != NULL)
    {
        tmp.assign(a, b - a);
        a = tmp.c_str();
    }

    //循环添加所有的单词
    while (a[0] != '\0')
    {
        switch (char_type(a[0]))
        {
        case 1: //空格
            break;
        case 2: //数字
            word.assign(a, 1);
            t = char_type(a[1]);
            while ((t == 2) || (a[1] == '.'))
            {
                if ((a[1] == '.') && (word.find(".") != string::npos))
                {
                    break;
                }
                word.append(++a, 1);
                t = char_type(a[1]);
            }
            push_back(word);
            break;
        case 3: //字母和下划线
            word.assign(a, 1);
            t = char_type(a[1]);
            while ((t == 2) || (t == 3))
            {
                word.append(++a, 1);
                t = char_type(a[1]);
            }
            push_back(word);
            break;
        case 4: //双引号
            word = "";
            t = char_type(a[1]);
            while ((t != 0) && (t != 4))
            {
                word.append(++a, 1);
                t = char_type(a[1]);
            }
            push_back(word);
            if (t == 0)
            {
                cout << cmd << ": 双引号不匹配!" << endl;
                return RET_MISS_QUOTE;
            }
            a++;
            break;
        case 5: //符号
            word.assign(a, 1);
            push_back(word);
            break;
        default://其他
            cout << cmd << ": 无效的字符!" << endl;
            return RET_BAD_CHAR;
        }
        a++;
    }

    //返回成功
    return RET_OK;
}
int stringlist::load_from_cmd(const string &cmd)
{
    return load_from_cmd(cmd.c_str());
}

//所有的字符串做trim，del_blank=删除空字符串，del_comment=删除//注释
void stringlist::trim(bool del_blank, bool del_comment)
{
    vector<string>::iterator it;
    for(it = begin(); it != end();)
    {
        if (del_comment)
        {
            size_t pos = it->find("//");
            if (pos != string::npos)
            {
                it->resize(pos);
            }
        }
        ::trim(*it);
        if (del_blank && it->empty())
        {
            erase(it++);
        }
        else
        {
            it++;
        }
    }
}

//查找字符串，返回下表，找不到返回-1
int stringlist::find(const char *str)
{
    for (size_t i = 0; i < size(); i++)
    {
        if (at(i) == str)
        {
            return i;
        }
    }
    return -1;
}
int stringlist::find(const string &str)
{
    return find(str.c_str());
}

//调试打印
void stringlist::show()
{
    cout << "stringlist: size=" << size() << endl;
    for (size_t i = 0; i < size(); i++)
    {
        cout << i << ". " << at(i) << endl;
    }
    cout << endl;
}

//测试代码
void stringlist::test()
{
    string pth = NAD_PATH + NAD_TASK_DIR;
    pth += "2_ets_0.task";
    load_from_file(pth.c_str());
    show();
    trim(true, true);
    cout << "trim:" << endl;
    show();
    pth += ".bak";
    insert(begin(), "//caption:demo");
    insert(begin() + 1, "//description:demo");
    save_to_file(pth.c_str());

    load_from_part("das|dsa|fsd", "|");
    show();

    load_from_part("dasd", "|");
    show();

    load_from_part("", "|");
    show();

    load_from_part("|||||||||", "|");
    show();

    load_from_part("dsadabcdasabcdsaabc", "abc");
    show();

    cout << save_to_part("|%|") << endl;

    load_from_cmd("const trim(str=\"wuhan\"12.5.66) //test");
    show();
}

/////////////////////////////////////////////////
/// coord_transfer类实现
/////////////////////////////////////////////////
coord_transfer::coord_transfer()
{

}

coord_transfer::coord_transfer(std::string filename)
{
   set_origin(filename);
}

coord_transfer::coord_transfer(double lat, double lon)
{
   set_origin(lat, lon);
}


void coord_transfer::set_origin(std::string filename)
{
    configSettings.ReadFile(filename);
    origin_lat =  configSettings.Read("origin_lat", -1.0);
    origin_lon =  configSettings.Read("origin_lon", -1.0);;
    cs.set_origin(origin_lat, origin_lon);
}

void coord_transfer::set_origin(double lat, double lon)
{
    origin_lat =  lat;
    origin_lon =  lon;
    cs.set_origin(origin_lat, origin_lon);
}
coord_transfer::~coord_transfer()
{
}

/*-------------------------------------------------------
 * 文件函数
-------------------------------------------------------*/

//补充完整路径，如"/opt/huawei"修改成"/opt/huawei/"
string full_path(const char *path)
{
    string ret = path;
    trim(ret);
    if (!srcontain(ret, phs))
    {
        ret += phs;
    }
    return ret;
}
string full_path(const string &path)
{
    return full_path(path.c_str());
}

//获得路径名，如"/opt/huawei/demo.txt"返回"/opt/huawei"
string get_path(const char *path)
{
    //没有路径名的情况
    string ret = path;
    size_t i = ret.find_last_of("\\/");
    if (i == string::npos)
    {
        return ret;
    }

    //有路径名的情况
    ret.erase(i);
    return ret;
}
string get_path(const string &path)
{
    return get_path(path.c_str());
}

//获得文件名，如"/opt/huawei/demo.txt"返回"demo.txt"
string get_filename(const char *path)
{
    //没有路径名的情况
    string ret = path;
    size_t i = ret.find_last_of("\\/");
    if (i == string::npos)
    {
        return ret;
    }

    //有路径名的情况
    ret.erase(0, i + 1);
    return ret;
}
string get_filename(const string &path)
{
    return get_filename(path.c_str());
}

//获得基础名，如"/opt/huawei/demo.txt"返回"demo"
string get_basename(const char *path)
{
    //没有扩展名的情况
    string ret = get_filename(path);
    size_t i = ret.rfind(".");
    if (i == string::npos)
    {
        return ret;
    }

    //有扩展名的情况
    ret.erase(i);
    return ret;
}
string get_basename(const string &path)
{
    return get_basename(path.c_str());
}

//获得扩展名，如"/opt/huawei/demo.txt"返回"txt"
string get_extname(const char *path)
{
    //没有扩展名的情况
    string ret = get_filename(path);
    size_t i = ret.rfind(".");
    if (i == string::npos)
    {
        ret = "";
        return ret;
    }

    //有扩展名的情况
    ret.erase(0, i + 1);
    return ret;
}
string get_extname(const string &path)
{
    return get_extname(path.c_str());
}

//获得一个目录下的所有文件，path形如“d:\\z\\*.txt”，中文path注意字符集问题
void get_file_list(stringlist &result, const char *path)
{
    //初始化为没有文件
    result.clear();

#ifdef _WIN32
    //window
    _finddata_t fileDir;
    long lfDir;
    if ((lfDir = _findfirst(path, &fileDir)) != -1l)
    {
        do
        {
            result.push_back(fileDir.name);
        }
        while (_findnext(lfDir, &fileDir) == 0);
    }
#else
    glob_t tGlob;
    memset(&tGlob, 0, sizeof(glob_t));
    if (!glob(path, GLOB_MARK, NULL, &tGlob))
    {
        for (size_t i = 0; i < tGlob.gl_pathc; i++)
        {
            result.push_back(tGlob.gl_pathv[i]);
        }
    }
    globfree(&tGlob);
#endif
}

//获得一个目录下的所有文件，path形如“d:\\z\\*.txt”，中文path注意字符集问题
void get_file_list(stringlist &result, const string &path)
{
    get_file_list(result, path.c_str());
}


/*-------------------------------------------------------
 * 坐标转换函数
-------------------------------------------------------*/
bool outOfChina(double lat, double lon)
{
    if (lon < 72.004 || lon > 137.8347)
        return true;
    if (lat < 0.8293 || lat > 55.8271)
        return true;
    return false;
}

double transformLat(double x, double y)
{
    double ret = -100.0 + 2.0 * x + 3.0 * y + 0.2 * y * y + 0.1 * x * y + 0.2 * sqrt(abs(x));
    ret += (20.0 * sin(6.0 * x * pi) + 20.0 * sin(2.0 * x * pi)) * 2.0 / 3.0;
    ret += (20.0 * sin(y * pi) + 40.0 * sin(y / 3.0 * pi)) * 2.0 / 3.0;
    ret += (160.0 * sin(y / 12.0 * pi) + 320 * sin(y * pi / 30.0)) * 2.0 / 3.0;
    return ret;
}

double transformLon(double x, double y)
{
    double ret = 300.0 + x + 2.0 * y + 0.1 * x * x + 0.1 * x * y + 0.1 * sqrt(abs(x));
    ret += (20.0 * sin(6.0 * x * pi) + 20.0 * sin(2.0 * x * pi)) * 2.0 / 3.0;
    ret += (20.0 * sin(x * pi) + 40.0 * sin(x / 3.0 * pi)) * 2.0 / 3.0;
    ret += (150.0 * sin(x / 12.0 * pi) + 300.0 * sin(x / 30.0 * pi)) * 2.0 / 3.0;
    return ret;
}

/**
 * 地球坐标转换为火星坐标
 * World Geodetic System ==> Mars Geodetic System
 *
 * @param wgLat  地球坐标
 * @param wgLon
 *
 * mglat,mglon 火星坐标
 */
void transform2Mars(double wgLat, double wgLon,double &mgLat,double &mgLon)
{
    if (outOfChina(wgLat, wgLon))
    {
        mgLat  = wgLat;
        mgLon = wgLon;
        return ;
    }
    double dLat = transformLat(wgLon - 105.0, wgLat - 35.0);
    double dLon = transformLon(wgLon - 105.0, wgLat - 35.0);
    double radLat = wgLat / 180.0 * pi;
    double magic = sin(radLat);
    magic = 1 - ee * magic * magic;
    double sqrtMagic = sqrt(magic);
    dLat = (dLat * 180.0) / ((a * (1 - ee)) / (magic * sqrtMagic) * pi);
    dLon = (dLon * 180.0) / (a / sqrtMagic * cos(radLat) * pi);
    mgLat = wgLat + dLat;
    mgLon = wgLon + dLon;

}

/**
 * 火星坐标转换为地球坐标
 * Mars Geodetic System ==>World Geodetic System
 *
 * @param mglat 火星坐标
 * @param mglon
 *
 * wgLat  wgLon地球坐标
 */
void transform2Wgs(double mgLat, double mgLon, double &wgLat, double &wgLon)
{
    double lat = mgLat;
    double lon = mgLon;
    transform2Mars(mgLat, mgLon, wgLat, wgLon);
    wgLat = lat - (wgLat - lat);
    wgLon = lon - (wgLon - lon);
}

void string2Wgs(string &id)
{
    double lon_in, lat_in;
    double lon_out, lat_out;
    get_lon_lat(lon_in, lat_in, id);
    transform2Wgs(lat_in, lon_in, lat_out, lon_out);
    id = build_session_key(lon_out, lat_out);
}

//从"点1->点2"的方向向右旋转90度构成"点1->点3"，且"点1-点3"间距offset米
void turn_right90(double x1, double y1, double x2, double y2, double offset, double &x3, double &y3)
{
    //p3=p1->p2向右旋转90度
    double _x3 = x1 + y2 - y1;
    double _y3 = y1 - x2 + x1;

    //p1->p3的距离
    double dy = (_y3 - y1);
    double dx = (_x3 - x1);
    double len = sqrt(dx * dx + dy * dy);

    //等比将p1偏移OB_OFFSET
    x3 = _x3 - dx * (len - offset) / len;
    y3 = _y3 - dy * (len - offset) / len;
}


/*-------------------------------------------------------
 * 各种枚举值的字符串格式
-------------------------------------------------------*/

//获得返回码的字符串格式描述
const char *ret_str(const int ret)
{
    switch (ret)
    {
        case RET_OK             : return "RET_OK";
        case RET_ERROR          : return "RET_ERROR";
        case RET_EXIST          : return "RET_EXIST";
        case RET_NOT_EXIST      : return "RET_NOT_EXIST";
        case RET_LCM_INIT_ERR   : return "RET_LCM_INIT_ERR";
        case RET_DB_CONN_ERR    : return "RET_DB_CONN_ERR";
        case RET_DB_QUERY_ERR   : return "RET_DB_QUERY_ERR";
        case RET_FILE_ERR       : return "RET_FILE_ERR";
        case RET_ROUTE_START_ERR: return "RET_ROUTE_START_ERR";
        case RET_ROUTE_END_ERR  : return "RET_ROUTE_END_ERR";
        case RET_ROUTE_NO_RESULT: return "RET_ROUTE_NO_RESULT";
        case RET_ROUTE_NOT_MATCH: return "RET_ROUTE_NOT_MATCH";
        case RET_CONTINUE       : return "RET_CONTINUE";
        case RET_FINISHED       : return "RET_FINISHED";
        case RET_MISS_QUOTE     : return "RET_MISS_QUOTE";
        case RET_MISS_BRACKET   : return "RET_MISS_BRACKET";
        case RET_BAD_CHAR       : return "RET_BAD_CHAR";
        case RET_CROSS_REF      : return "RET_CROSS_REF";
        case RET_BAD_FUNC       : return "RET_BAD_FUNC";
        case RET_BAD_STATE      : return "RET_BAD_STATE";
        case RET_BAD_PARA       : return "RET_BAD_PARA";
        case RET_TIMEOUT        : return "RET_TIMEOUT";
        case RET_IN_PLATOON     : return "RET_IN_PLATOON";
        case RET_SAME           : return "RET_SAME";
        case RET_NOT_SAME       : return "RET_NOT_SAME";
        case RET_BUSY           : return "RET_BUSY";
        case RET_FORBID         : return "RET_FORBID";
        default                 : return "UNKNOWN_RET";
    }
}
const char *ne_str(const int val)
{
    switch (val)
    {
        case E_NE_CSU         : return "CSU";
        case E_NE_RSU         : return "RSU";
        case E_NE_OBU         : return "OBU";
        case E_NE_RSD         : return "RSD";
        case E_NE_RSD_CAMERA  : return "RSD_CAMERA";
        case E_NE_RSD_LIDAR   : return "RSD_LIDAR";
        case E_NE_ETS_LIGHT   : return "ETS_LIGHT";
        case E_NE_ETS_LIMSPEED: return "ETS_LIMSPEED";
        case E_NE_ETS_BLOCK   : return "ETS_BLOCK";
        default               : return "UNKNOWN";
    }
}

const char *ne_status_str(const int val)
{
    switch (val)
    {
        case NE_STATUS_OFF: return "OFF";
        case NE_STATUS_ON : return "ON";
        default           : return "UNKNOWN";
    }
}

const char *tet_str(const int val)
{
    switch (val)
    {
        case TET_LANE    : return "LANE";
        case TET_LIGHT   : return "LIGHT";
        case TET_LIMSPEED: return "LIMSPEED";
        case TET_BLOCK   : return "BLOCK";
        case TET_CROSSING: return "CROSSING";
        default          : return "UNKNOWN";
    }
}

const char *ls_str(const int val)
{
    switch (val)
    {
        case LS_NONE          : return "NONE";
        case LS_GREEN         : return "GREEN";
        case LS_YELLOW        : return "YELLOW";
        case LS_RED           : return "RED";
        default               : return "UNKNOWN";
    }
}

const char *ot_str(const int val)
{
    /*
    switch (val)
    {
        case OT_OBU   : return "OBU";
        case OT_CAR   : return "CAR";
        case OT_PEOPLE: return "PEOPLE";
        case OT_BLOCK : return "BLOCK";
        case OT_OTHER : return "OTHER";
        default       : return "UNKNOWN";
    }*/

    switch (val)
    {
        case OT_OBU   : return "OBU";
        case OT_BUS   : return "BUS";
        case OT_CAR   : return "CAR";
        case OT_PEOPLE: return "PEOPLE";
        case OT_OTHER : return "OTHER";
        case OT_STONE : return "STONE";
        case OT_BLOCK : return "BLOCK";
        case OT_CONE  : return "CONE";
        default       : return "UNKNOWN";
    }
}

const char *rsd_sensor_str(const int val)
{
    switch (val)
    {
        case RSD_SENSOR_CAMERA: return "CAMERA";
        case RSD_SENSOR_LIDAR : return "LIDAR";
        case RSD_SENSOR_RADAR : return "RADAR";
        default               : return "UNKNOWN";
    }
}

const char *kp_str(const int val)
{
    switch (val)
    {
        case KP_NONE             : return "NONE";
        case KP_NORMAL           : return "NORMAL";
        case KP_ROUTE_START      : return "ROUTE_START";
        case KP_ROUTE_STOP       : return "ROUTE_STOP";
        case KP_CHANGE_LANE_LEFT : return "CHANGE_LANE_LEFT";
        case KP_CHANGE_LANE_RIGHT: return "CHANGE_LANE_RIGHT";
        case KP_CO_CHANGE_LANE   : return "CO_CHANGE_LANE";
        case KP_CURRENT          : return "CURRENT";
        case KP_LIGHT_ACTIVE     : return "LIGHT_ACTIVE";
        case KP_LIGHT_STOP       : return "LIGHT_STOP";
        case KP_LIGHT_EXIT       : return "LIGHT_EXIT";
        case KP_LIMSPEED         : return "LIMSPEED";
        case KP_BLOCK            : return "BLOCK";

        case KP_OT_OBU           : return "OT_OBU";
        case KP_OT_CAR           : return "OT_CAR";
        case KP_OT_PEOPLE        : return "OT_PEOPLE";
        case KP_OT_BLOCK         : return "OT_BLOCK";
        case KP_OT_OTHER         : return "OT_OTHER";
        case KP_OT_CONE          : return "OT_CNOE";
        case KP_OT_BUS           : return "OT_BUS";
        case KP_OT_STONE         : return "OT_STONE";

        case KP_W_SINGLE         : return "W_SINGLE";
        case KP_W_CHANGE_OFF     : return "W_CHANGE_OFF";
        case KP_W_CHANGE_ON      : return "W_CHANGE_ON";
        case KP_W_CAMERA_ON      : return "W_CAMERA_ON";
        case KP_W_CAMERA_OFF     : return "W_CAMERA_OFF";
        case KP_W_RADAR_ON       : return "W_RADAR_ON";
        case KP_W_RADAR_OFF      : return "W_RADAR_OFF";
        case KP_W_LIMSPEED_OFF   : return "W_LIMSPEED_OFF";
        case KP_W_DEGRADE   : return "W_DEGRADE";
        case KP_W_ROAD_LEVEL_1   : return "W_ROAD_LEVEL_1";
        case KP_W_ROAD_LEVEL_2   : return "W_ROAD_LEVEL_2";
        case KP_W_ROAD_LEVEL_3   : return "W_ROAD_LEVEL_3";
        case KP_W_ROAD_LEVEL_4   : return "W_ROAD_LEVEL_4";
        default                  : return "UNKNOWN";
    }
}

const char *sp_str(const int val)
{
    switch (val)
    {
        case SP_ADD_OBU_TO_TAIL: return "ADD_OBU";
        case SP_DELETE_OBU     : return "DEL_OBU";
        case SP_SET_SPEED      : return "SET_SPEED";
        case SP_JOIN_OBU       : return "JOIN_OBU";
        default                : return "UNKNOWN";
    }
}

const char *dpr_str(const int val)
{
    switch (val)
    {
        case DPR_ARRIVED   : return "ARRIVED";
        case DPR_DELETE_ALL: return "DEL_ALL";
        case DPR_OCT_FORCE : return "OCT_FORCE";
        default            : return "UNKNOWN";
    }
}

const char *sensor_str(const int val)
{
    switch (val)
    {
        case SENSOR_NONE: return "NONE";
        case SENSOR_OK  : return "OK";
        case SENSOR_ERR : return "ERR";
        default         : return "UNKNOWN";
    }
}

const char *route_reason_str(const int val)
{
    switch (val)
    {
        case ROUTE_REASON_VUI         : return "VUI";
        case ROUTE_REASON_OBU_RE_PATH : return "OBU_RE_PATH";
        case ROUTE_REASON_ADD_PLATOON : return "ADD_PLATOON";
        case ROUTE_REASON_ADD_TAIL    : return "ADD_TAIL";
        case ROUTE_REASON_JOIN_OBU    : return "JOIN_OBU";
        case ROUTE_REASON_OCT_CALL_CAR: return "CALL_CAR";
        case ROUTE_REASON_ETS_BLOCK   : return "ETS_BLOCK";
        default                       : return "UNKNOWN";
    }
}

const char *start_reason_str(const int val)
{
    switch (val)
    {
        case START_REASON_VUI     : return "VUI";
        case START_REASON_CSU     : return "CSU";
        case START_REASON_RSU     : return "RSU";
        case START_REASON_PLATOON : return "PLATOON";
        case START_REASON_CALL_CAR: return "CALL_CAR";
        default                   : return "UNKNOWN";
    }
}

const char *stop_reason_str(const int val)
{
    switch (val)
    {
        case STOP_REASON_VUI    : return "VUI";
        case STOP_REASON_CSU    : return "CSU";
        case STOP_REASON_RSU    : return "RSU";
        case STOP_REASON_ARRIVED: return "ARRIVED";
        default                 : return "UNKNOWN";
    }
}

const char *cl_direction_str(const int val)
{
    switch (val)
    {
        case CL_DIRECTION_NONE : return "NONE";
        case CL_DIRECTION_LEFT : return "LEFT";
        case CL_DIRECTION_RIGHT: return "RIGHT";
        case CL_DIRECTION_BLOCK: return "BLOCK";
        default                : return "UNKNOWN";
    }
}

const char *cl_reason_str(const int val)
{
    switch (val)
    {
        case CL_REASON_HAND        : return "HAND";
        case CL_REASON_OBU_AVOIDING: return "OBU_AVOIDING";
        case CL_REASON_ETS_BLOCK   : return "BLOCK";
        case CL_REASON_STOP        : return "STOP";
        case CL_REASON_BROKEN_LANE : return "BROKEN_LANE";
        case CL_REASON_PLATOON_DROP: return "PLATOON_DROP";
        case CL_REASON_RSU_AVOIDING: return "RSU_AVOIDING";
        case CL_REASON_PLATOON_STOP: return "PLATOON_STOP";
        case CL_REASON_PLATOON_JOIN: return "PLATOON_JOIN";
        default                    : return "UNKNOWN";
    }
}

int cl_reason_val(const char *str)
{
    if (!strcasecmp(str, "OBU_AVOIDING"))
    {
        return CL_REASON_OBU_AVOIDING;
    }
    else if (!strcasecmp(str, "BLOCK"))
    {
        return CL_REASON_ETS_BLOCK;
    }
    else if (!strcasecmp(str, "STOP"))
    {
        return CL_REASON_STOP;
    }
    else if (!strcasecmp(str, "BROKEN_LANE"))
    {
        return CL_REASON_BROKEN_LANE;
    }
    else if (!strcasecmp(str, "PLATOON_DROP"))
    {
        return CL_REASON_PLATOON_DROP;
    }
    else if (!strcasecmp(str, "RSU_AVOIDING"))
    {
        return CL_REASON_RSU_AVOIDING;
    }
    else if (!strcasecmp(str, "PLATOON_STOP"))
    {
        return CL_REASON_PLATOON_STOP;
    }
    else if (!strcasecmp(str, "PLATOON_JOIN"))
    {
        return CL_REASON_PLATOON_JOIN;
    }
    return CL_REASON_HAND;
}

const char *cl_status_str(const int val)
{
    switch (val)
    {
        case CL_STATUS_REQUEST    : return "REQUEST";
        case CL_STATUS_BLOCKED    : return "BLOCKED";
        case CL_STATUS_RUNNING    : return "RUNNING";
        case CL_STATUS_COMPLETE   : return "COMPLETE";
        case CL_STATUS_CANCEL     : return "CANCEL";
        case CL_STATUS_CANCEL_COMP: return "CANCEL_COMP";
        default                   : return "UNKNOWN";
    }
}

const char *log_str(const int val)
{
    switch (val)
    {
        case LOG_INFO   : return "INFO";
        case LOG_WARNING: return "WARNING";
        case LOG_ERROR  : return "ERROR";
        case LOG_FATAL  : return "FATAL";
        default         : return "UNKNOWN";
    }
}

const char *alarm_str(const int val)
{
    switch (val)
    {
        case ALARM_INFO   : return "INFO";
        case ALARM_ADVISE : return "ADVISE";
        case ALARM_WARNING: return "WARNING";
        case ALARM_DANGER : return "DANGER";
        default           : return "UNKNOWN";
    }
}

int alarm_val(const char *str)
{
    if (!strcasecmp(str, "INFO"))
    {
        return ALARM_INFO;
    }
    else if (!strcasecmp(str, "WARNING"))
    {
        return ALARM_WARNING;
    }
    else if (!strcasecmp(str, "DANGER"))
    {
        return ALARM_DANGER;
    }
    return ALARM_ADVISE;
}

const char *ets_reason_str(const int val)
{
    switch (val)
    {
        case ETS_REASON_DIALOG  : return "DIALOG";
        case ETS_REASON_TASK    : return "TASK";
        case ETS_REASON_CROSSING: return "CROSSING";
        default                 : return "UNKNOWN";
    }
}

const char *at_status_str(const int val)
{
    switch (val)
    {
        case AT_STATUS_OFF: return "OFF";
        case AT_STATUS_P  : return "P";
        case AT_STATUS_R  : return "R";
        case AT_STATUS_N  : return "N";
        case AT_STATUS_D  : return "D";
        case AT_STATUS_M  : return "M";
        case AT_STATUS_M1 : return "M1";
        case AT_STATUS_M2 : return "M2";
        case AT_STATUS_U  : return "U";
        default           : return "UNKNOWN";
    }
}

const char *upcall_str(const int val)
{
    switch (val)
    {
        case UPCALL_UC_ADD_PLATOON_REQUEST   : return "UC_ADD_PLATOON";
        case UPCALL_UC_SET_PLATOON_REQUEST   : return "UC_SET_PLATOON";
        case UPCALL_UC_DELETE_PLATOON_REQUEST: return "UC_DELETE_PLATOON";
        case UPCALL_UC_CALL_CAR_REQUEST      : return "UC_CALL_CAR";
        default                              : return "UNKNOWN";
    }
}

bool is_my_upcall(const int val)
{
    #ifdef _NAD_CSU_PLANNING_
    if (val == UPCALL_UC_ADD_PLATOON_REQUEST || val == UPCALL_UC_SET_PLATOON_REQUEST ||
        val == UPCALL_UC_DELETE_PLATOON_REQUEST || val == UPCALL_UC_CALL_CAR_REQUEST)
    {
        return true;
    }
    #endif

    //不该我执行
    return false;
}

/*-------------------------------------------------------
 *CRC32相关
-------------------------------------------------------*/
const uint table[] = {
    0x00000000, 0x77073096, 0xee0e612c, 0x990951ba, 0x076dc419, 0x706af48f, 0xe963a535, 0x9e6495a3,
    0x0edb8832, 0x79dcb8a4, 0xe0d5e91e, 0x97d2d988, 0x09b64c2b, 0x7eb17cbd, 0xe7b82d07, 0x90bf1d91,
    0x1db71064, 0x6ab020f2, 0xf3b97148, 0x84be41de, 0x1adad47d, 0x6ddde4eb, 0xf4d4b551, 0x83d385c7,
    0x136c9856, 0x646ba8c0, 0xfd62f97a, 0x8a65c9ec, 0x14015c4f, 0x63066cd9, 0xfa0f3d63, 0x8d080df5,
    0x3b6e20c8, 0x4c69105e, 0xd56041e4, 0xa2677172, 0x3c03e4d1, 0x4b04d447, 0xd20d85fd, 0xa50ab56b,
    0x35b5a8fa, 0x42b2986c, 0xdbbbc9d6, 0xacbcf940, 0x32d86ce3, 0x45df5c75, 0xdcd60dcf, 0xabd13d59,
    0x26d930ac, 0x51de003a, 0xc8d75180, 0xbfd06116, 0x21b4f4b5, 0x56b3c423, 0xcfba9599, 0xb8bda50f,
    0x2802b89e, 0x5f058808, 0xc60cd9b2, 0xb10be924, 0x2f6f7c87, 0x58684c11, 0xc1611dab, 0xb6662d3d,
    0x76dc4190, 0x01db7106, 0x98d220bc, 0xefd5102a, 0x71b18589, 0x06b6b51f, 0x9fbfe4a5, 0xe8b8d433,
    0x7807c9a2, 0x0f00f934, 0x9609a88e, 0xe10e9818, 0x7f6a0dbb, 0x086d3d2d, 0x91646c97, 0xe6635c01,
    0x6b6b51f4, 0x1c6c6162, 0x856530d8, 0xf262004e, 0x6c0695ed, 0x1b01a57b, 0x8208f4c1, 0xf50fc457,
    0x65b0d9c6, 0x12b7e950, 0x8bbeb8ea, 0xfcb9887c, 0x62dd1ddf, 0x15da2d49, 0x8cd37cf3, 0xfbd44c65,
    0x4db26158, 0x3ab551ce, 0xa3bc0074, 0xd4bb30e2, 0x4adfa541, 0x3dd895d7, 0xa4d1c46d, 0xd3d6f4fb,
    0x4369e96a, 0x346ed9fc, 0xad678846, 0xda60b8d0, 0x44042d73, 0x33031de5, 0xaa0a4c5f, 0xdd0d7cc9,
    0x5005713c, 0x270241aa, 0xbe0b1010, 0xc90c2086, 0x5768b525, 0x206f85b3, 0xb966d409, 0xce61e49f,
    0x5edef90e, 0x29d9c998, 0xb0d09822, 0xc7d7a8b4, 0x59b33d17, 0x2eb40d81, 0xb7bd5c3b, 0xc0ba6cad,
    0xedb88320, 0x9abfb3b6, 0x03b6e20c, 0x74b1d29a, 0xead54739, 0x9dd277af, 0x04db2615, 0x73dc1683,
    0xe3630b12, 0x94643b84, 0x0d6d6a3e, 0x7a6a5aa8, 0xe40ecf0b, 0x9309ff9d, 0x0a00ae27, 0x7d079eb1,
    0xf00f9344, 0x8708a3d2, 0x1e01f268, 0x6906c2fe, 0xf762575d, 0x806567cb, 0x196c3671, 0x6e6b06e7,
    0xfed41b76, 0x89d32be0, 0x10da7a5a, 0x67dd4acc, 0xf9b9df6f, 0x8ebeeff9, 0x17b7be43, 0x60b08ed5,
    0xd6d6a3e8, 0xa1d1937e, 0x38d8c2c4, 0x4fdff252, 0xd1bb67f1, 0xa6bc5767, 0x3fb506dd, 0x48b2364b,
    0xd80d2bda, 0xaf0a1b4c, 0x36034af6, 0x41047a60, 0xdf60efc3, 0xa867df55, 0x316e8eef, 0x4669be79,
    0xcb61b38c, 0xbc66831a, 0x256fd2a0, 0x5268e236, 0xcc0c7795, 0xbb0b4703, 0x220216b9, 0x5505262f,
    0xc5ba3bbe, 0xb2bd0b28, 0x2bb45a92, 0x5cb36a04, 0xc2d7ffa7, 0xb5d0cf31, 0x2cd99e8b, 0x5bdeae1d,
    0x9b64c2b0, 0xec63f226, 0x756aa39c, 0x026d930a, 0x9c0906a9, 0xeb0e363f, 0x72076785, 0x05005713,
    0x95bf4a82, 0xe2b87a14, 0x7bb12bae, 0x0cb61b38, 0x92d28e9b, 0xe5d5be0d, 0x7cdcefb7, 0x0bdbdf21,
    0x86d3d2d4, 0xf1d4e242, 0x68ddb3f8, 0x1fda836e, 0x81be16cd, 0xf6b9265b, 0x6fb077e1, 0x18b74777,
    0x88085ae6, 0xff0f6a70, 0x66063bca, 0x11010b5c, 0x8f659eff, 0xf862ae69, 0x616bffd3, 0x166ccf45,
    0xa00ae278, 0xd70dd2ee, 0x4e048354, 0x3903b3c2, 0xa7672661, 0xd06016f7, 0x4969474d, 0x3e6e77db,
    0xaed16a4a, 0xd9d65adc, 0x40df0b66, 0x37d83bf0, 0xa9bcae53, 0xdebb9ec5, 0x47b2cf7f, 0x30b5ffe9,
    0xbdbdf21c, 0xcabac28a, 0x53b39330, 0x24b4a3a6, 0xbad03605, 0xcdd70693, 0x54de5729, 0x23d967bf,
    0xb3667a2e, 0xc4614ab8, 0x5d681b02, 0x2a6f2b94, 0xb40bbe37, 0xc30c8ea1, 0x5a05df1b, 0x2d02ef8d,
};

//获取crc32校验码， data_length不包含crc32所需的4字节长度
uint get_crc32( uchar *data, int data_length)
{
    if (data_length < 1)
        return 0xffffffff;

    uint crc = 0;

    for (int i = 0; i != data_length; ++i)
    {
        crc = table[(crc ^ data[i]) & 0xff] ^ (crc >> 8);
    }

    crc = crc ^ 0xffffffff;

    return crc;
}

//判断两个对象距离在dist_meter米以内
bool dist_in_ll(double lat1, double lon1, double lat2, double lon2, double dist_meter)
{
    double x1, y1, x2, y2;
    coord_transfer transfer;
    transfer.cs.ll2xy(lat1, lon1, x1, y1);
    transfer.cs.ll2xy(lat2, lon2, x2, y2);
    return dist_in_xy(x1, y1, x2, y2, dist_meter);
}

//判断两个对象距离在dist_meter米以内
bool dist_in_xy(double x1, double y1, double x2, double y2, double dist_meter)
{
    return length_two_points(x1, y1, x2, y2) <= dist_meter;
}

double dist_two_points_ll(double lat1, double lon1, double lat2, double lon2)
{
    double dist;
    double x1, y1, x2, y2;
    coord_transfer transfer;
    transfer.cs.ll2xy(lat1, lon1, x1, y1);
    transfer.cs.ll2xy(lat2, lon2, x2, y2);
    dist = length_two_points(x1, y1, x2, y2);

    return dist;
}

