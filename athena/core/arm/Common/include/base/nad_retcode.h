/*-------------------------------------------------------
 * 文件名：nad_retcode.h
 * 创建者：张毅00151602
 * 时  间：2016-03-02
 * 描  述：公共返回码
-------------------------------------------------------*/
#ifndef _NAD_RETCODE_H
#define _NAD_RETCODE_H

//返回值数据类型为int

//公共返回值
#define RET_OK    0
#define RET_ERROR 1
#define RET_NORMAL 2

//公共返回码
#define RET_EXIST            10  //新增的记录已存在
#define RET_NOT_EXIST        11  //修改或删除的记录不存在
#define RET_LCM_INIT_ERR     12  //lcm初始化失败
#define RET_DB_CONN_ERR      13  //数据库连接错误
#define RET_DB_QUERY_ERR     14  //数据库查询错误
#define RET_FILE_ERR         15  //文件操作错误
#define RET_ROUTE_START_ERR  16  //在地图上找不到路径规划起点
#define RET_ROUTE_END_ERR    17  //在地图上找不到路径规划终点
#define RET_ROUTE_NO_RESULT  18  //没有合适的路径
#define RET_ROUTE_NOT_MATCH  19  //路径规划结果的时戳不一致
#define RET_CONTINUE         20  //执行脚本时，脚本没有执行完毕
#define RET_FINISHED         21  //执行脚本时，没有下一行脚本，成功结束
#define RET_MISS_QUOTE       22  //引号不匹配
#define RET_MISS_BRACKET     23  //括号不匹配
#define RET_BAD_CHAR         24  //无效的字符
#define RET_CROSS_REF        25  //脚本交叉引用
#define RET_BAD_FUNC         26  //无效的函数
#define RET_BAD_STATE        27  //无效的state
#define RET_BAD_PARA         28  //无效的参数
#define RET_TIMEOUT          29  //超时
#define RET_IN_PLATOON       30  //目前正在编队中
#define RET_SAME             31  //相同
#define RET_NOT_SAME         32  //不同
#define RET_BUSY             33  //繁忙
#define RET_FORBID           34  //禁止操作
#define RET_IN_COOPERATE     35  //协助式换道中

#define RET_DEGRADE_FORBIT    36   //驾驶降级阶段禁止切换自动驾驶


//OCT召车对应的返回码
#define RET_CALL_CAR_ONLY     0  //只允许召车
#define RET_PARK_CAR_ONLY   1  //只允许还车
#define RET_IN_CALL_CAR           2  //召车中
#define RET_IN_PARK_CAR         3  //还车中


//获得返回码的字符串格式描述
const char *ret_str(const int ret);


#endif
