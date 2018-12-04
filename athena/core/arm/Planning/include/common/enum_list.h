/*-------------------------------------------------------
 * 文件名：nad_enum.h
 * 创建者：张毅00151602
 * 时  间：2016-03-02
 * 描  述：公共枚举值
-------------------------------------------------------*/
#ifndef _NAD_ENUM_H
#define _NAD_ENUM_H

#define PI 3.14159265358979323846264338

#define to_radians(x) ( (x) * (PI / 180.0 ))    ///<角度转弧度
#define to_degrees(x) ( (x) * (180.0 / M_PI ))  ///<弧度转角度

#define MAXIMUM_VALUE 9e9            ///<极大值
#define MINIMUM_VALUE -9e9           ///<极小值

//replan flag
#define REPLAN_FLAG_NONE  0             ///<不需要重规划
#define REPLAN_FLAG_NAVI  1             ///<云端需要重规划
#define REPLAN_FLAG_PLANNING  2         ///<planning自身需要重规划

//cross_road_status_
#define NOT_AT_CROSSING  0             ///<非路口
#define AT_CROSSING_GREEN  1           ///<在路口，绿灯
#define AT_CROSSING_RED  2             ///<在路口，红灯（黄灯当做红灯处理）

//light status
#define LIGHT_STATUS_NONE  0                ///<无效值
#define LIGHT_STATUS_GREEN  1               ///<绿灯
#define LIGHT_STATUS_YELLOW  2              ///<黄灯
#define LIGHT_STATUS_RED  3                 ///<红灯

//obstacle type
#define OBSTACLE_TYPE_UNKNOW  0                  ///<未知障碍物
#define OBSTACLE_TYPE_CAR  1                     ///<车辆
#define OBSTACLE_TYPE_PEDESTRIAN  2              ///<行人

//filter_obstacle_flag
#define FILTER_OBSTACLE_NOT 0         ///<不过滤
#define FILTER_OBSTACLE_POINT 1         ///<按障碍物中心点过滤
#define FILTER_OBSTACLE_RECT 2         ///<按障碍物边框过滤

//Stop_Park
#define STOP_TYPE_TERMINAL 10                 ///<终点信息
#define STOP_TYPE_PARALLEL_PARK 1            ///<平行泊车信息
#define STOP_TYPE_VERTICAL_PARK 2            ///<垂直泊车信息
#define STOP_TYPE_OBLIQUE_PARK 3            ///<斜向泊车信息

#define DRIVE_MODE_OVERTAKE 1               ///超车模式
#define DRIVE_MODE_FOLLOW 2               ///跟车模式


//----------------------------------------------------------
// 网元定义枚举值
//----------------------------------------------------------

//网元类型
#define E_NE_CSU  0
#define E_NE_RSU  1
#define E_NE_OBU  2

//子系统类型
#define E_ME_CSU  0
#define E_ME_OCT  1
#define E_ME_RSU  10
#define E_ME_ETS  11
#define E_ME_RSD  12
#define E_ME_OBU  20
#define E_ME_VUI  21

//心跳超时时长
#define HTBT_TIMEOUT 10000 //暂时改成很大，避免调试时很容易心跳注销


//----------------------------------------------------------
// obu上的obu的状态
//----------------------------------------------------------

//obu管理自身的状态机
#define E_OBU_SESSION_OBU_DISCONN_HAND       0  //无网络人工驾驶
#define E_OBU_SESSION_OBU_CONNECT_HAND       1  //有网络人工驾驶
#define E_OBU_SESSION_OBU_DISCONN_AUTO       2  //无网络自动驾驶
#define E_OBU_SESSION_OBU_CONNECT_AUTO       3  //有网络自动驾驶

//obu管理自身注册状态的状态机
#define E_OBU_SESSION_OBU_LOGIN_CSU          0  //正在向csu要个rsu_name
#define E_OBU_SESSION_OBU_LOGIN_RSU          1  //正在注册到rsu
#define E_OBU_SESSION_OBU_LOGIN_OK           2  //注册成功

//obu请求启动自动驾驶的状态机
#define E_OBU_SESSION_OBU_START_AUTO_WAIT    0  //已发送or_start_auto_request消息，等待应答
#define E_OBU_SESSION_OBU_START_AUTO_OK      1  //收到ro_start_auto_respond应答，启动成功

/**
 * class obu_session_obu
 * {
 *     int session_status;    //主状态，枚举值为E_OBU_SESSION_OBU_XXX
 *     int login_status;      //登录状态，枚举值为E_OBU_SESSION_OBU_LOGIN_XXX
 *     nad_planning planning; //路径规划，planning.list.size()==0表示无规划
 *     int htbt_count;        //心跳计数，每次发送or_info_report则htbt_count++；
 *                            //每次收到ro_info_report则htbt_count=0，htbt_count>HTBT_TIMEOUT则超时
 * } obu;
 *
 * //每秒处理obu状态
 * if (obu.session_status == DISCONN_HAND || obu.session_status == DISCONN_AUTO)
 * {
 *     根据obu.login_status发消息注册
 * }
 * else if (obu.htbt_count > HTBT_TIMEOUT) //心跳超时
 * {
 *     obu.session_status = DISCONN_AUTO; //根据降级原则，首先降级为DISCONN_AUTO
 *     obu.login_status = LOGIN_CSU;      //开始不断的注册
 * }
 *
 * //收到了ro_obu_logout_notify(rsu或csu心跳超时注销了obu)，重新注册
 * {
 *     obu.session_status = DISCONN_AUTO; //根据降级原则，首先降级为DISCONN_AUTO
 *     obu.login_status = LOGIN_CSU;      //开始不断的注册
 * }
**/


//----------------------------------------------------------
// rsu上的obu的状态
//----------------------------------------------------------

//rsu上的obu状态
#define E_RSU_SESSION_OBU_DISCONN            0  //无网络
#define E_RSU_SESSION_OBU_CONNECT_HAND       1  //有网络人工驾驶
#define E_RSU_SESSION_OBU_CONNECT_AUTO       3  //有网络自动驾驶

/**
 * class rsu_session_obu
 * {
 *     int session_status;    //主状态，枚举值为E_RSU_SESSION_OBU_XXX
 *     nad_planning planning; //路径规划，planning.list.size()==0表示无规划
 *     int htbt_count;        //心跳计数，每次发送rc_info_report则htbt_count++；
 *                            //每次收到cr_info_report则htbt_count=0，htbt_count>HTBT_TIMEOUT则超时
 * } obu;
 *
 * //每秒处理obu状态
 * if (obu.session_status != DISCONN && obu.htbt_count > HTBT_TIMEOUT) //心跳超时
 * {
 *     发消息rc_obu_logout_notify、ro_obu_logout_notify
 *     删除rsu_session_obu
 * }
**/


//----------------------------------------------------------
// csu上的obu的状态
//----------------------------------------------------------

//csu上的obu状态
#define E_CSU_SESSION_OBU_DISCONN            0  //无网络
#define E_CSU_SESSION_OBU_CONNECT_HAND       1  //有网络人工驾驶
#define E_CSU_SESSION_OBU_CONNECT_AUTO       3  //有网络自动驾驶

/**
 * class csu_session_obu
 * {
 *     int session_status;    //主状态，枚举值为E_RSU_SESSION_OBU_XXX
 *     nad_planning planning; //路径规划，planning.list.size()==0表示无规划
 *     int htbt_count;        //心跳计数，每次发送rc_info_report则htbt_count++；
 *                            //每次收到cr_info_report则htbt_count=0，htbt_count>HTBT_TIMEOUT则超时
 * } obu;
 *
 * //每秒处理obu状态
 * if (obu.session_status != DISCONN && obu.htbt_count > HTBT_TIMEOUT) //心跳超时
 * {
 *     发消息cr_obu_logout_notify
 *     删除csu_session_obu
 * }
**/


//----------------------------------------------------------
// rsu的状态
//----------------------------------------------------------

//rsu管理自身的状态机
#define E_RSU_SESSION_RSU_DISCONN   0  //未连接
#define E_RSU_SESSION_RSU_CONNECT   1  //已连接

//csu上的rsu状态
#define E_CSU_SESSION_RSU_DISCONN   0  //未连接
#define E_CSU_SESSION_RSU_CONNECT   1  //已连接


//----------------------------------------------------------
// ets的枚举值
//----------------------------------------------------------

/**
 * rsu如果收到ets的er_ets_report消息，rsu就会创建对应的rsu_session_light/rsu_session_limspeed/rsu_session_block
 * 并在每秒上报中通过rc_info_report、ro_info_report通知到csu、obu，其中csu同理创建session。
 *
 * rsu收到er_ets_report消息时会把rsu_session_light/rsu_session_limspeed/rsu_session_block的htbt_count=0,
 * rsu的一秒定时器会把rsu_session_light/rsu_session_limspeed/rsu_session_block的htbt_count++，
 * 如果htbt_count > HTBT_TIMEOUT，就会删除rsu_session_light/rsu_session_limspeed/rsu_session_block。
 *
 * csu收到rc_info_report会做同样的心跳处理，如果某ets.htbt_count > HTBT_TIMEOUT，删除对应session
**/

//交通元素类型枚举值

#define TET_NONE    -1  //未知
#define TET_LANE     0  //车道
#define TET_LIGHT    10 //红绿灯
#define TET_LIMSPEED 11 //限速牌
#define TET_BLOCK    12 //施工标志
//#define TET_CROSSING 20 //路口
#define KP_LIGHT_ACTIVE 20 //红绿灯激活点
#define KP_LIGHT_STOP 21 //红绿灯停止点(vui)
#define KP_LIGHT_EXIT 22 //红绿灯退出点
const char *tet_str(const int val);

//车道方向枚举值
#define LD_NONE     0 //远离路口或离开路口，行驶在哪条车道都可以，无方向属性
#define LD_STRAIGHT 1 //直行
#define LD_LEFT     2 //左转
#define LD_RIGHT    4 //右转
#define LD_TURN     8 //掉头
#define LD_S_L      (LD_STRAIGHT | LD_LEFT)  //直行+左转 01 | 10 = 11                           = 3
#define LD_S_R      (LD_STRAIGHT | LD_RIGHT) //直行+右转 001 | 100 = 101                        = 5
#define LD_S_L_R    (LD_STRAIGHT | LD_LEFT | LD_RIGHT)  //直行+左转+右转 001 | 010 | 100 = 111   = 7
#define LD_L_T      (LD_LEFT | LD_TURN)      //左转+掉头  0010 | 1000 = 1010                    = 10

//红绿灯取值
#define LS_NONE     0 //离线
#define LS_GREEN    1 //绿灯
#define LS_YELLOW   2 //黄灯
#define LS_RED      3 //红灯
//由障碍物或者施工标志引起的路权变化
#define LS_RED_OBSTACLE      4 //
#define LS_RED_BLOCK      5 //
const char *ls_str(const int val);


//----------------------------------------------------------
// rsd的枚举值
//----------------------------------------------------------

//障碍物类型
#define OT_OBU      0  //安装了OBU的联网车辆（包括仅支持ADAS的车）
#define OT_CAR      1  //没安装OBU的社会车辆
#define OT_PEOPLE   2  //行人
#define OT_BLOCK    3  //普通实物单车道施工标志
#define OT_OTHER    4  //其他障碍物
const char *ot_str(const int val);

//各种障碍物的距离抗抖动
#define SAME_DIST_CAR    2.0 //OT_OBU和OT_CAR在2米内认为是同一障碍物
#define SAME_DIST_PEOPLE 1.0 //OT_PEOPLE在1米内认为是同一障碍物
#define SAME_DIST_BLOCK  1.0 //OT_BLOCK在1米内认为是同一障碍物
#define SAME_DIST_OTHER  1.0 //OT_OTHER在1米内认为是同一障碍物

//传感器类型
#define RSD_SENSOR_LIDAR   0  //激光雷达
#define RSD_SENSOR_CAMERA  1  //摄影头
#define RSD_SENSOR_RADAR   2  //毫米波雷达
const char *rsd_sensor_str(const int val);


//----------------------------------------------------------
// 路径规划的枚举值
//----------------------------------------------------------

//key_point类型
#define KP_NONE              -1  //不是关键点（即：拟合点）
#define KP_UNKNOWN            0  //未赋予关键点类型（启动和终点允许设置为本类型，我们认为key_point_list的第一个是起点，最后一个是终点）
#define KP_CHANGE_LANE        1  //换道点（不分左右）
#define KP_CHANGE_LANE_LEFT   2  //向左换道点
#define KP_CHANGE_LANE_RIGHT  3  //向右换道点
#define KP_CHANGE_LANE_OK     4  //换道成功点


//----------------------------------------------------------
// 编队的枚举值
//----------------------------------------------------------

//修改编队的操作类型
#define SP_ADD_OBU_TO_TAIL  0  //在队尾追加车辆
#define SP_DELETE_OBU       1  //删除车辆
#define SP_SET_SPEED        2  //修改编队整体车速
const char *sp_str(const int val);

//删除编队的操作类型
#define DPR_ARRIVED     0  //编队整体到达目的地
#define DPR_DELETE_ALL  1  //除头车外所有车辆都被删除
#define DPR_OCT_FORCE   2  //OCT为了调试强制解散编队
const char *dpr_str(const int val);


//----------------------------------------------------------
// obu的枚举值
//----------------------------------------------------------

//加速度模式
#define ACC_P_CONSTANT_SPEED  0  //恒速
#define ACC_P_CONSTANT_ACC    1  //匀加速
#define ACC_P_CONSTANT_DECE   2  //匀减速
#define ACC_P_RAPID_ACC       3  //急加速
#define ACC_P_RAPID_DECE      4  //急减速

//一种类型的传感器最大数量，要和message.lcm保持一致
#define MAX_SENSOR  16

//传感器状态
#define SENSOR_NONE  0  //传感器不在位
#define SENSOR_OK    1  //传感器正常
#define SENSOR_ERR   2  //传感器故障
const char *sensor_str(const int val);

//照相机枚举值
#define IDX_CAMERA_1  0  //前视车道线识别单目相机
#define IDX_CAMERA_2  1  //前视障碍物识别双目相机

//毫米波雷达枚举值
#define IDX_RADAR_ESR     0  //前视远距离雷达(ESR)
#define IDX_RADAR_SRR_FL  1  //左前方SRR雷达
#define IDX_RADAR_SRR_FR  2  //右前方SRR雷达
#define IDX_RADAR_SRR_BL  3  //左后方SRR雷达
#define IDX_RADAR_SRR_BR  4  //右后方SRR雷达

//激光雷达枚举值
#define IDX_LIDAR_CENTER  0  //前视远距离雷达

//GPS枚举值
#define IDX_GPS_CENTER  0  //中置GPS/INS组合导航
#define IDX_INS_CENTER  1  //中置GPS/INS组合导航

//路径规划的原因
#define ROUTE_REASON_VUI           0  //在VUI上启动规划
#define ROUTE_REASON_OBU_RE_PATH   1  //OBU偏离路径，发生道路级重规划
#define ROUTE_REASON_ADD_PLATOON   2  //OCT创建车队
#define ROUTE_REASON_ADD_TAIL      3  //在车队尾部追加车辆
#define ROUTE_REASON_JOIN_OBU      4  //在车队中间追加车辆
#define ROUTE_REASON_OCT_CALL_CAR  5  //召车
#define ROUTE_REASON_ETS_BLOCK     6  //施工标志封闭全部道路
#define ROUTE_REASON_HEAD_CAR      7  //头车更新路径
const char *route_reason_str(const int val);

//启动自动驾驶的原因
#define START_REASON_VUI       0  //在VUI上切换为自动驾驶
#define START_REASON_CSU       1  //CSU启动自动驾驶（如：脚本）
#define START_REASON_RSU       2  //RSU启动自动驾驶（暂未使用）
#define START_REASON_PLATOON   3  //编队启动自动驾驶
#define START_REASON_CALL_CAR  4  //召车启动自动驾驶
const char *start_reason_str(const int val);

//退出自动驾驶的原因
#define STOP_REASON_VUI      0  //在VUI上切换为人工驾驶
#define STOP_REASON_CSU      1  //CSU退出自动驾驶（如：脚本）
#define STOP_REASON_RSU      2  //RSU退出自动驾驶（如：编队）
#define STOP_REASON_ARRIVED  3  //到达目的地，obu切换为人工驾驶
const char *stop_reason_str(const int val);

//换道方向
#define CL_DIRECTION_NONE    0  //保持当前车道
#define CL_DIRECTION_LEFT    1  //向左换道
#define CL_DIRECTION_RIGHT   2  //向右换道
const char *cl_direction_str(const int val);

//换道原因
#define CL_REASON_FORBIDEN     -1  //不允许换道
#define CL_REASON_HAND          0  //人工拨动转向摇杆要求换道
#define CL_REASON_OBU_AVOIDING  1  //OBU避障，所以换道
#define CL_REASON_ETS_BLOCK     2  //电子施工标志封闭部分车道，所以换道
#define CL_REASON_TRAFFIC_JAM   3  //慢车阻塞我超过10秒，OBU发起换道
#define CL_REASON_START         4  //起步时从低速道换到高速道
#define CL_REASON_STOP          5  //停车时从高速道换到低速道
#define CL_REASON_BROKEN_LANE   6  //在断头车道提前换到旁边的车道
#define CL_REASON_PLATOON_DROP  7  //编队踢掉车辆
#define CL_REASON_RSU_AVOIDING  8  //RSUÖž»ÓOBU±ÜÕÏ
#define CL_REASON_OBU_RE_LANE   9  //OBUÆ«ÀëÂ·Ÿ¶£¬·¢ÉúµÀÂ·Œ¶ÖØ¹æ»®
#define CL_REASON_PLATOON_STOP  10 //编队停车时从高速道换到低速道
#define CL_REASON_PLATOON_JOIN  11 //编队中间加入车辆
const char *cl_reason_str(const int val);

//换道状态
#define CL_STATUS_REQUEST      0  //单车认为可以换道
#define CL_STATUS_BLOCKED      1  //单车认为有障碍，需要网络通知相关车辆避让
#define CL_STATUS_RUNNING      2  //单车正在换道
#define CL_STATUS_COMPLETE     3  //单车换道完成
#define CL_STATUS_CANCEL       4  //单车换道过程中被取消
#define CL_STATUS_CANCEL_COMP  5  //单车换道取消后回本道完成
const char *cl_status_str(const int val);

//下发中心线的类型
#define IS_NOT_REPLAN  0  //不重规划
#define IS_REPLAN      1  //重规划

//车辆默认行为
//#define DRIVE_BEHAVIOR_OVERTAKE  0  //允许车辆自主换道超车
//#define DRIVE_BEHAVIOR_FOLLOW    1  //车辆自身只能跟车，网络下发换道超车(默认取本值)


#define CAR_ACTION_NONE           0  //无效值
#define CAR_ACTION_SINGLE         1  //单车模式
#define CAR_ACTION_PLATOON_HEAD   2  //车队头车
#define CAR_ACTION_PLATOON_OTHER  3  //车队其他车辆

#define HUMAN_DRIVING 1
#define ASSIST_DRIVING 2
#define AUTO_DRIVING 3

//----------------------------------------------------------
// oct、vui的枚举值
//----------------------------------------------------------

//csu管理oct的状态机
#define E_CSU_SESSION_OCT_DISCONN   0  //未连接
#define E_CSU_SESSION_OCT_CONNECT   1  //已连接

//obu管理vui的状态机
#define E_OBU_SESSION_VUI_DISCONN   0  //未连接
#define E_OBU_SESSION_VUI_CONNECT   1  //已连接

//日志等级
#define LOG_INFO     0  //普通日志
#define LOG_WARNING  1  //警告日志
#define LOG_ERROR    2  //错误日志
#define LOG_FATAL    3  //致命错误日志
const char *log_str(const int val);

//告警等级
#define ALARM_INFO     0  //提示告警(灰色)
#define ALARM_ADVISE   1  //建议告警(绿色)
#define ALARM_WARNING  2  //警告告警(黄色)
#define ALARM_DANGER   3  //危险告警(红色)
const char *alarm_str(const int val);

//告警类型
#define ALARM_TYPE_OCCUR    0  //产生告警（在VUI开始显示）
#define ALARM_TYPE_RESTORE  1  //恢复告警（在VUI停止显示）
#define ALARM_TYPE_EVENT    2  //事件告警（在VUI显示5秒后自动隐藏）

//告警展示类型
#define ALARM_SHOW_TEXT        0  //文本
#define ALARM_SHOW_SOUND       1  //语音
#define ALARM_SHOW_TEXT_SOUND  2  //文本加语音

//产生告警的距离
#define ALARM_DIS_LONG    100 // <=100米
#define ALARM_DIS_MIDDLE  50  // <=50米
#define ALARM_DIS_SHORT   20  // <=20米

//修改ETS的原因
#define ETS_REASON_DIALOG     0  //通过对话框修改
#define ETS_REASON_TASK       1  //通过脚本修改
#define ETS_REASON_CROSSING   2  //通过路口算法修改
const char *ets_reason_str(const int val);


//----------------------------------------------------------
// 车辆CAN的枚举值
//----------------------------------------------------------

//发动机状态
#define ENGINE_STATUS_STOPPED   0  //停止
#define ENGINE_STATUS_STALLED   1  //熄火
#define ENGINE_STATUS_RUNNING   2  //运行
#define ENGINE_STATUS_CRANKING  3  //启动

////档位状态
//#define AT_STATUS_OFF 255
//#define AT_STATUS_P   0
//#define AT_STATUS_R   1
//#define AT_STATUS_N   2
//#define AT_STATUS_D   3
//#define AT_STATUS_M   8
//#define AT_STATUS_M1  9
//#define AT_STATUS_M2  10
//#define AT_STATUS_U   6
//const char *at_status_str(const int val);

//刹车信号
#define BRAKE_STATUS_OFF  0  //未刹车
#define BRAKE_STATUS_ON   1  //正在刹车

//转向灯状态
#define FLASHING_STATUS_NONE  0  //不亮
#define FLASHING_STATUS_L     1  //左转向
#define FLASHING_STATUS_R     2  //右转向
#define FLASHING_STATUS_LR    3  //双闪

//灯光状态
#define BEAM_STATUS_OFF  0  //关灯
#define BEAM_STATUS_ON   1  //开灯


//----------------------------------------------------------
// GPS、惯导的枚举值
//----------------------------------------------------------

//定位定姿状态，源自POS状态量的高2位
#define GPS_FLAG_MA        0  //机械编排
#define GPS_FLAG_KEEP      0  //保存整秒时刻状态
#define GPS_FLAG_LC_BEGIN  1  //组合更新计算开始
#define GPS_FLAG_LC_END    2  //组合解算完成

//RTK状态，源自POS状态量的低6位
#define GPS_N_SPP    0  //单点定位
#define GPS_N_FLOAT  1  //浮动解
#define GPS_N_FIXED  2  //固定解，精度最佳
#define GPS_N_NG     3  //失败解

//----------------------------------------------------------
// xxx_session_obu中的platoon_flag的枚举值，还未用到
//----------------------------------------------------------

//车辆或编队的状态
#define PLATOON_CONVERGE       0   //各个obu正在汇聚，未形成编队
#define PLATOON_FIN            1   //obu已经在编队中
#define PLATOON_DROP_OUT_PRE   2   //obu准备退出编队
#define PLATOON_DROP_OUT       3   //obu正在退出编队
#define PLATOON_JOIN           4   //obu正在加入编队
#define VAHICLE_FREE           5   //obu是自由车辆

//调用汽院的PlatoonControl的返回值
#define PLATOON_RET_DOING  0  //当前状态未执行完
#define PLATOON_RET_OK     1  //执行完了

//坐标转换
const double pi = 3.14159265358979324;
const double a = 6378245.0;
const double ee = 0.00669342162296594323;
const double x_pi = 3.14159265358979324 * 3000.0 / 180.0;

const double car_follow_dis = 12;
const double car_out_dis = 15;

#define MAX_YAW 50.0  //头指向超过60度即可判断为弯道

const int SPEED_LOW      = 5;
const int DISTANCE_IDLE  = 10;
const double ACCELARATION = 0.4;

#endif
