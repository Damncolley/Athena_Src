/**
 * @file    controller_output_alarm.h
 * @author  jiang <jiangchengjie@indrv.cn>
 * @date    2018-07-07
 * @version 1.0.0
 * @par     Copyright(c)
 *          hy
 */

#ifndef CONTROLLER_OUTPUT_ALARM_H_
#define CONTROLLER_OUTPUT_ALARM_H_

#include <vector>
#include "controller_config.h"
#include "controller_alarm_code.h"

 /**
 * @namespace athena::control
 * @brief athena::control
 */
namespace athena{
namespace control{


/**
 * @class ControllerOutputAlarm
 *
 * @brief 控制警报输出.
 */
class ControllerOutputAlarm
{
 public:
/**
 * @brief     构造函数
 * @param[in] get_current_time_callback   获取当前时间回调函数.
 * @param[in] get_log_callback  日志记录回调函数.
 * @return    true or false.
 */


 ControllerOutputAlarm(ControllerConfig *controller_config);
 ~ControllerOutputAlarm() = default;

 ///故障码格式
 typedef struct{
     ///故障码
    int32_t alarm_code;
    ///故障等级
    int32_t alarm_level;
 }AlarmInfo;
 ///报警表
 typedef struct{
    AlarmInfo alarm_info;
    ///发生时间
    Tm timer_stamp;
 }AlarmInfoTable;


 /**
 * @brief     AddAlarm 添加报警码.
 * @param[in] code   警报码.
 * @return    true or false.
 */
 bool AddAlarm(int32_t code);

 /**
 * @brief     DelAlarm 删除报警码.
 * @param[in] code   警报码.
 * @return    true or false.
 */
 bool DelAlarm(int32_t code);

/**
 * @brief     清空故障表.
 * @param[in] void.
 * @return    true or false.
 */
 bool ClearAlarmTable();

/**
 * @brief     FindAlarm 查找报警码.
 * @param[in] code   警报码.
 * @return    -1 = 查找不到 其他 索引值.
 */
 int32_t FindAlarm(int32_t code);

/**
 * @brief     GetAlarmInfo 获取报警码信息.
 * @param[in] code   警报码.
 * @param[out] alarm_info   报警信息.
 * @return    true or false.
 */
 bool GetAlarmInfo(int32_t code,AlarmInfoTable* alarm_info);

 /**
 * @brief     判断报警列表是否为空.
 * @return    true or false.
 */
 bool IsEmpty();

 /**
 * @brief GetAlarmInfo 获取报警信息.
 * @param[out] alarm_list 报警信息.
 * @return void.
 */
  void GetAlarmTableInfo(std::vector<AlarmInfoTable>  *alarm_list);

 private:
 ControllerConfig *controller_config_;
 ///报警个数上限
 const uint32_t limit_alarm_num_ = 9;

 //AlarmInfo[100] = {{OK,NORMAL},{CONTROLLER_OFF_TRACK_ERROR,ERROR}};
 ///报警信息列表
 AlarmInfo alarm_info_[9] =   {
                              {OK                                          ,NORMAL},
                              {CONTROLLER_OFF_TRACK_ERROR                  ,ERROR },
                              {CONTROLLER_INIT_ERROR                       ,ERROR },
                              {CONTROLLER_OFF_TRACK_ERROR                  ,ERROR },
                              {CONTROLLER_TRAJECTORY_IS_SHORT_OR_LONG_ERROR,ERROR },
                              {CONTROLLER_TRAJECTORY_ERROR                 ,HIGH_WARNING},
                              {CONTROLLER_TRAJECTORY_SET_TIMEOUT           ,HIGH_WARNING},
                              {CONTROLLER_COMPUTE_TIMEOUT                  ,HIGH_WARNING},
                              {CONTROLLER_COMPUTE_ERROR                    ,HIGH_WARNING},
                              };

 std::vector<AlarmInfoTable> alarm_list_;
 std::vector<AlarmInfo> alarm_Info_list_;
  private:
     pthread_mutex_t lock_;
};
}
}

#endif // CONTROLLER_OUTPUT_H_
