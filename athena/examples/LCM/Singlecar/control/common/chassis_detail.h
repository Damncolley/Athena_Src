
/**
 * @file    control_logic.h
 * @author  jiang <jiangchengjie@indrv.cn>
 * @date    2018-07-07
 * @version 1.0.0
 * @par     Copyright(c)
 *          hy
 */

#ifndef COMMON_CHASSIS_DETAIL_H_
#define COMMON_CHASSIS_DETAIL_H_

 /**
 * @namespace athena::control
 * @brief athena::control
 */
namespace athena{
namespace control{
/**
 * @class ChassisDetail
 * @brief 车辆底盘信息.
 */
class ChassisDetail
{
public:
    ChassisDetail() = default;
    ~ChassisDetail() = default;
    ///底盘错误 0=正常 1=有错误
    int32_t    chassis_error_;
   ///左前轮转速 rpm
	double     wheel_speed_fl_;
	///右前轮转速 rpm
	double     wheel_speed_fr_;
	///左后轮转速 rpm
	double     wheel_speed_bl_;
	///右后轮转速 rpm
	double     wheel_speed_br_;
	///车速
	double     car_speed_;
	///发动机转速 rpm
	double     eng_rpm_;
	///加速踏板位置 %
	double     acc_pedal_pos_;
	///节气门反馈 %
	double     throttle_pos_feedback_;
	///TCU无降扭请求时的发动机扭矩 NM
	double     eng_torq_without_tcured_;
	///蓄电池电压 V
	double     batt_volt_v_;
	///驾驶员需求扭矩
	double     driver_torque_;
	///发动机实际指示扭矩
	double     eng_actual_ind_torque_;
	///发动机总摩擦扭矩
	double     friction_torque_;
	///慢速扭矩请求值 Nm
	double     torque_limitation_;
	///快速扭矩请求值 Nm
	double     torque_reduction_;
	///涡轮转速 rpm
	double     turbine_rpm_;
	///制动压力信号 Mpa
	double     brake_pressure_;
	///发动机运行状态
	int32_t    engine_running_status_;
	///档杆位置
	int32_t    gear_level_;
	///自动挡行驶档位
	int32_t    at_gear_;
	///制动踏板开关信号
	int32_t    brake_status_;
	///EPB状态 0 禁能 1 使能
	int32_t    epb_brake_status_;
	///发动机减速断油 0 否 1 是
	int32_t    dfco_;
	///怠速状态 0 否 1 是
	int32_t    idle_status_;
	///TCU降扭有效性 0 available  1 not available
	int32_t    tcu_torque_red_availability_;
	///发动机扭矩错误 0 OK 1 Fail
	int32_t    eng_torque_failure_;
	///发动机释放 0 undefine 1 engine locked 2 engine release 3 reserved
	int32_t    ems_released_;
	///发动机启动 0 未启动  1 启动
	int32_t    eng_started_;
	///降扭请求标志为 0 无请求  1 有
	int32_t    torque_reduction_flag_;
	///慢速扭矩请求标志位
	int32_t    torque_limitation_flag_;
	///输入离合器状态 0 未吸合 1 吸合
	int32_t    gear_eng_agement_;
	///TCU状态 0 正常 1 非跛行模式故障 2 跛行模式故障 3 未定义
	int32_t    tcu_state_;
	///EBD 电子制动力分配系统是否工作 0 不工作  1 工作
	int32_t    ebd_work_;
	///ABS工作标志 0 不工作  1 工作
	int32_t    abs_ebdlable_;
	///油门控制工作模式 E 可控状态 0 暂时不可控  1 可控 2 正处于被控制状态 3 永久失效
	int32_t    acc_driving_mode_feedback_;
    ///转向人工干预 0=否 1=是
	int32_t    steering_manual_intervention;
	///转向人工干预是否有效 0=无效 1=有效
	int32_t    steering_manual_intervention_vaild;
	///转向角度反馈
	double     steering_angle_feedback_;
	///转向角速度反馈
	double     steering_angle_speed_feedback_;
	///转向工作模式 可控状态 0 暂时不可控  1 可控 2 正处于被控制状态 3 永久失效
	int32_t    steering_driving_mode_feedback_;
	///刹车值反馈
	double     brake_value_feedback_;
	///刹车执行时间反馈
	double     brake_run_time_feedback_;
	///纵向控制工作模式反馈 可控状态 0 暂时不可控  1 可控 2 正处于被控制状态 3 永久失效
	int32_t    brake_driving_mode_feedback_;
	///EPB控制工作模式反馈 可控状态 0 暂时不可控  1 可控 2 正处于被控制状态 3 永久失效
	int32_t    epb_driving_mode_feedback_;
	///喇叭控制 0 禁声音 1 鸣笛
    int8_t     speaker_status_;
    ///远光灯 0 关闭 1 开启
    int8_t     high_beam_status_;
    ///近光灯 0 关闭 1 开启
    int8_t     low_beam_status_;
    ///左转向灯 0 关闭 1 开启
    int8_t     left_turn_status_;
    ///右转向灯 0 关闭 1 开启
    int8_t     right_turn_status_;
    ///前雨刮器 0 关闭 1 开启
	int8_t     front_wiper_status_;
    ///后雨刮器 0 关闭 1 开启
    int8_t     rear_wiper_status_;
    ///位置灯 0 关闭 1 开启
    int8_t     position_lamp_status_;
    ///前雾灯 0 关闭 1 开启
    int8_t     front_fog_lamp_status_;
    ///后雾灯 0 关闭 1 开启
    int8_t     rear_fog_lamp_status_;
    ///刹车灯 一般情况自动控制 0 关闭 1 开启
    int8_t     brake_lamp_status_;
    ///警报灯 双闪 0 关闭 1 开启
    int8_t     alarm_lamp_status_;
    /// 左前门控制 0 关闭 1 开启
    int8_t     lf_door_status_;
    /// 右前门控制 0 关闭 1 开启
    int8_t     rf_door_status_;
    /// 左后门控制 0 关闭 1 开启
    int8_t     lr_door_status_;
    /// 右后门控制 0 关闭 1 开启
    int8_t     rr_door_status_;
};
}//namespace control
}//namespace athena

#endif
