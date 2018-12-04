/**
 * @file    logging.h
 * @author  jiang <jiangchengjie@indrv.cn>
 * @date    2018-07-07
 * @version 1.0.0
 * @par     Copyright(c)
 *          hy
 */
#ifndef COMMON_LOGGING_H_
#define COMMON_LOGGING_H_

#include <iostream>

using namespace std;

/**
* @namespace athena::control
* @brief athena::control
*/
namespace athena{
namespace control{
/**
 * @class Logging
 * @brief  日志类.
 */
class Logging{
public:
    Logging() = default;
    ~Logging() = default;
    typedef enum{
        INFO    = 1,  /**< 消息*/
        WARNING = 2,  /**< 警告*/
        ERROR   = 3,  /**< 错误*/
    }LogLevel;
/**
 * @brief     初始化.
 * @param[in] log_enable true=日志记录 false=不记录.
 * @return    void.
 */
 static void Init(bool log_enable);

/**
 * @brief     日志记录.
 * @param[in] level 日志等级参考LogLevel.
 * @param[in] info 日志记录信息.
 * @return    void.
 */
 static void LogInfo(int level,std::string info);
private:
    ///是否进行日志记录
    static bool log_enable_;
};
}
}
#endif// COMMON_LOGGING_H_
