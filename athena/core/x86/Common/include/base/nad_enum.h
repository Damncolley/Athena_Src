/*-------------------------------------------------------
 * 文件名：nad_enum.h
 * 创建者：张毅00151602
 * 时  间：2016-03-02
 * 描  述：公共枚举值
-------------------------------------------------------*/
#ifndef _NAD_ENUM_H
#define _NAD_ENUM_H


//枚举值数据类型为int


//----------------------------------------------------------
// 网元定义枚举值
//----------------------------------------------------------

//网元类型
#define E_NE_CSU           0   //CSU
#define E_NE_RSU           1   //RSU
#define E_NE_OBU           2   //OBU
#define E_NE_RSD           3   //OBU
#define E_NE_RSD_CAMERA    10  //路测-摄影头
#define E_NE_RSD_LIDAR     11  //路测-激光雷达
#define E_NE_ETS_LIGHT     20  //电子红绿灯
#define E_NE_ETS_LIMSPEED  21  //电子施工标志
#define E_NE_ETS_BLOCK     22  //电子限速牌
const char *ne_str(const int val);

//网元状态
#define NE_STATUS_OFF  0  //不在位
#define NE_STATUS_ON   1  //在位
const char *ne_status_str(const int val);

//心跳超时时长，25000毫秒=25秒
#define HTBT_TIMEOUT 250000000
#define HTBT_TIMEOUT_OCT  50000    //5 sec
//#define HTBT_TIMEOUT 5000000 //gdb时可以改成5000秒，避免调试时很容易心跳注销


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
 * 并在每秒上报中通过rc_info_report、ro_vui_report通知到csu、obu，其中csu同理创建session。
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
#define TET_CROSSING 20 //路口
const char *tet_str(const int val);

 //红绿灯取值
#define LS_NONE     0 //离线
#define LS_GREEN    1 //绿灯
#define LS_YELLOW   2 //黄灯
#define LS_RED      3 //红灯
const char *ls_str(const int val);


//----------------------------------------------------------
// rsd的枚举值
//----------------------------------------------------------

//障碍物类型
/*
#define OT_OBU      0  //安装了OBU的联网车辆（包括仅支持ADAS的车）
#define OT_CAR      1  //没安装OBU的社会车辆
#define OT_PEOPLE   2  //行人
#define OT_BLOCK    3  //普通实物单车道施工标志
#define OT_OTHER    4  //其他障碍物*/

#define OT_OBU         0  //安装了OBU的联网车辆（包括仅支持ADAS的车）
#define OT_CAR         1   //没安装OBU的社会车辆
#define OT_PEOPLE      2  //行人
#define OT_BLOCK       3
#define OT_OTHER       4
#define OT_BUS         5
#define OT_STONE       6
#define OT_CONE        7

const char *ot_str(const int val);

//各种障碍物的距离抗抖动
#define SAME_DIST_OBU    2.0 //OT_OBU在1米内认为是同一障碍物
#define SAME_DIST_CAR    2.0 //OT_OBU和OT_CAR在2米内认为是同一障碍物
#define SAME_DIST_PEOPLE 1.0 //OT_PEOPLE在1米内认为是同一障碍物
#define SAME_DIST_BLOCK  1.0 //OT_BLOCK在1米内认为是同一障碍物
#define SAME_DIST_OTHER  1.0  //OT_OTHER在1米内认为是同一障碍物
#define SAME_DIST_BUS    2.0  //
#define SAME_DIST_STONE  1.0
#define SAME_DIST_CONE   1.0


//传感器类型
#define RSD_SENSOR_CAMERA  0  //摄影头
#define RSD_SENSOR_LIDAR   1  //激光雷达
#define RSD_SENSOR_RADAR   2  //毫米波雷达
const char *rsd_sensor_str(const int val);


//----------------------------------------------------------
// 路径规划的枚举值
//----------------------------------------------------------
typedef enum
{
    KP_NONE = 0,            //不是关键点（即：拟合点）
    KP_NORMAL,              //未赋予关键点类型
    KP_ROUTE_START,         //规划的起始点(vui)
    KP_ROUTE_STOP,          //规划的结束点(-1~2是永久保留点)(vui)
    KP_CHANGE_LANE_LEFT,    //向左换道点
    KP_CHANGE_LANE_RIGHT,   //向右换道点
    KP_CO_CHANGE_LANE,      //协作：协作式换道(vui)
    KP_CURRENT,             //当前车辆位置(vui)
    KP_LIGHT_ACTIVE,        //红绿灯激活点
    KP_LIGHT_STOP,          //红绿灯停止点(vui)
    KP_LIGHT_EXIT,          //红绿灯退出点
    KP_LIMSPEED,            //电子限速牌(vui)
    KP_BLOCK,               //电子施工标志(vui)
    KP_PARKING,             //泊车点
    KP_OT_OBU,              //障碍物：安装了OBU的联网车辆（包括仅支持ADAS的车）
    KP_OT_CAR,              //障碍物：没安装OBU的社会车辆
    KP_OT_PEOPLE,           //障碍物：行人
    KP_OT_BLOCK,            //障碍物：普通实物单车道施工标志
    KP_OT_OTHER,            //障碍物：其他
    KP_OT_BUS,              //障碍物：其他
    KP_OT_STONE,            //障碍物：其他
    KP_OT_CONE,             //障碍物：其他
    KP_W_SINGLE,            //预警：单车道
    KP_W_CHANGE_OFF,        //预警：禁止换道
    KP_W_CHANGE_ON,         //预警：解除禁止换道
    KP_W_CAMERA_ON,         //预警: 打开camera
    KP_W_CAMERA_OFF,        //预警: 关闭camera
    KP_W_RADAR_ON,          //预警: 打开radar
    KP_W_RADAR_OFF,         //预警: 关闭radar
    KP_W_LIMSPEED_OFF,      //预警：取消限速
    KP_W_DEGRADE,           //预警：降级驾驶 通过warning测试专用
    KP_W_ROAD_LEVEL_1,      //道路等级1
    KP_W_ROAD_LEVEL_2,      //道路等级2
    KP_W_ROAD_LEVEL_3,      //道路等级3
    KP_W_ROAD_LEVEL_4,      //道路等级4
    MAX_KP_TYPE
} ;

/*
//key_point类型
#define KP_NONE              -1   //不是关键点（即：拟合点）
#define KP_NORMAL             0   //未赋予关键点类型
#define KP_ROUTE_START        1   //规划的起始点(vui)
#define KP_ROUTE_STOP         2   //规划的结束点(-1~2是永久保留点)(vui)
#define KP_CHANGE_LANE_LEFT   4   //向左换道点
#define KP_CHANGE_LANE_RIGHT  5   //向右换道点
#define KP_CO_CHANGE_LANE     6   //协作：协作式换道(vui)
#define KP_CURRENT            10  //当前车辆位置(vui)
#define KP_LIGHT_ACTIVE       20  //红绿灯激活点
#define KP_LIGHT_STOP         21  //红绿灯停止点(vui)
#define KP_LIGHT_EXIT         22  //红绿灯退出点
#define KP_LIMSPEED           23  //电子限速牌(vui)
#define KP_BLOCK              24  //电子施工标志(vui)
#define KP_PARKING            25  //泊车点

#define KP_OT_OBU             30  //障碍物：安装了OBU的联网车辆（包括仅支持ADAS的车）
#define KP_OT_CAR             31  //障碍物：没安装OBU的社会车辆
#define KP_OT_PEOPLE          32  //障碍物：行人
#define KP_OT_BLOCK           33  //障碍物：普通实物单车道施工标志
#define KP_OT_OTHER           34  //障碍物：其他
#define KP_OT_BUS             35  //障碍物：其他
#define KP_OT_STONE           36  //障碍物：其他
#define KP_OT_CONE            37  //障碍物：其他

#define KP_W_SINGLE           40  //预警：单车道
#define KP_W_CHANGE_OFF       41  //预警：禁止换道
#define KP_W_CHANGE_ON        42  //预警：解除禁止换道
#define KP_W_CAMERA_ON        43  //预警: 打开camera
#define KP_W_CAMERA_OFF       44  //预警: 关闭camera
#define KP_W_RADAR_ON         45  //预警: 打开radar
#define KP_W_RADAR_OFF        46  //预警: 关闭radar
#define KP_W_LIMSPEED_OFF     47  //预警：取消限速
#define KP_W_DEGRADE   48  //预警：降级驾驶 通过warning测试专用
#define KP_W_ROAD_LEVEL_1 49 //道路等级1
#define KP_W_ROAD_LEVEL_2 50 //道路等级2
#define KP_W_ROAD_LEVEL_3 51 //道路等级3
#define KP_W_ROAD_LEVEL_4 52 //道路等级4
*/

const char *kp_str(const int val);


//----------------------------------------------------------
// 编队的枚举值
//----------------------------------------------------------

//修改编队的操作类型
#define SP_ADD_OBU_TO_TAIL  0  //在队尾追加车辆
#define SP_DELETE_OBU       1  //删除车辆
#define SP_SET_SPEED        2  //修改编队整体车速
#define SP_JOIN_OBU         3  //车队中间插入车辆
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

//传感器枚举值
#define IDX_GPS_CENTER    0  //中置GPS组合导航
#define IDX_INS_CENTER    1  //中置INS组合导航
#define IDX_CAMERA_1      2  //前视车道线识别单目相机
#define IDX_CAMERA_2      3  //前视障碍物识别双目相机
#define IDX_RADAR_ESR     4  //前视远距离毫米波雷达(ESR)
#define IDX_RADAR_SRR_FL  5  //左前方SRR毫米波雷达
#define IDX_RADAR_SRR_FR  6  //右前方SRR毫米波雷达
#define IDX_RADAR_SRR_BL  7  //左后方SRR毫米波雷达
#define IDX_RADAR_SRR_BR  8  //右后方SRR毫米波雷达
#define IDX_LIDAR_CENTER  9  //前视远距离激光雷达

//换道原因:单车上报
#define CL_REASON_HAND          0  //人工拨动转向摇杆要求换道
#define CL_REASON_OBU_AVOIDING  1  //OBU避障，所以换道
//换道原因:网络主动生成
#define CL_REASON_STOP          5  //停车时从高速道换到低速道
#define CL_REASON_ETS_BLOCK     2  //电子施工标志封闭部分车道，所以换道
#define CL_REASON_BROKEN_LANE   6  //在断头车道提前换到旁边的车道
#define CL_REASON_PLATOON_DROP  7  //编队踢掉车辆
#define CL_REASON_RSU_AVOIDING  8  //RSU指挥OBU避障
#define CL_REASON_PLATOON_STOP  10 //编队停车时从高速道换到低速道
#define CL_REASON_PLATOON_JOIN  11 //编队中间加入车辆
const char *cl_reason_str(const int val);
int cl_reason_val(const char *str);

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
#define  STOP_REASON_DEGRADE  4  //降级导致停车

const char *stop_reason_str(const int val);

//换道方向
#define CL_DIRECTION_NONE    0  //保持当前车道
#define CL_DIRECTION_LEFT    1  //向左换道
#define CL_DIRECTION_RIGHT   2  //向右换道
#define CL_DIRECTION_BLOCK   3  //被阻死，无法换道
const char *cl_direction_str(const int val);

//换道状态
#define CL_STATUS_REQUEST      0  //单车认为可以换道
#define CL_STATUS_BLOCKED      1  //单车认为有障碍，需要网络通知相关车辆避让
#define CL_STATUS_RUNNING      2  //单车正在换道
#define CL_STATUS_COMPLETE     3  //单车换道完成
#define CL_STATUS_CANCEL       4  //单车换道过程中被取消
#define CL_STATUS_CANCEL_COMP  5  //单车换道取消后回本道完成
const char *cl_status_str(const int val);

//协作式协作式换道的状态，允许换道后就算协作式换道完成
#define CLS_NONE        0  //所有车: 没在换道
#define CLS_WAIT_SPACE  1  //主车: 等待腾出换道空间
#define CLS_RUNNING     2  //主车: 正在换道或取消换道
#define CLS_ADD_SPEED   3  //从车: 加速，以便为主车腾出换道空间
#define CLS_DEC_SPEED   4  //从车: 减速，以便为主车腾出换道空间

//超过多长时间取消协作式换道，毫秒
#define MAX_CL_TIME  (3 * 1000)

//下发中心线的类型
#define IS_NOT_REPLAN  0  //不重规划
#define IS_REPLAN      1  //重规划

//车辆默认行为
#define DRIVE_BEHAVIOR_OVERTAKE  0  //允许车辆自主换道超车。
#define DRIVE_BEHAVIOR_PFOLLOW   1  //车辆自身只能跟车，网络下发换道超车(默认取本值)。
#define DRIVE_BEHAVIOR_PHEAD     2  //在车队里面的头车，目前不允许随意换道。
#define DRIVE_BEHAVIOR_SIMULATE  3  //仿真车辆

//车辆类型
#define CAR_ACTION_NONE           0  //不允许出现的错误值
#define CAR_ACTION_SINGLE         1  //非编队车辆
#define CAR_ACTION_PLATOON_HEAD   2  //编队头车
#define CAR_ACTION_PLATOON_OTHER  3  //编队跟车


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
int alarm_val(const char *str);

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

//档位状态
#define AT_STATUS_OFF 255
#define AT_STATUS_P   5
#define AT_STATUS_R   7
#define AT_STATUS_N   0
#define AT_STATUS_D   4
#define AT_STATUS_M   8
#define AT_STATUS_M1  9
#define AT_STATUS_M2  10
#define AT_STATUS_U   6
const char *at_status_str(const int val);

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
#define GPS_N_SPP    0  //单点定位(纯靠惯导)
#define GPS_N_FLOAT  1  //浮动解(300cm)
#define GPS_N_FIXED  2  //固定解，精度最佳(20cm)
#define GPS_N_NG     3  //失败解(纯靠惯导)
#define GPS_N_STATIC 4  //静态单点定位(纯靠惯导)


//----------------------------------------------------------
// 编队枚举值
//----------------------------------------------------------

//车辆或编队的状态
#define PLATOON_CONVERGE       0   //各个obu正在汇聚，未形成编队
#define PLATOON_FIN            1   //obu已经在编队中
#define PLATOON_ADD_TAIL       2  //obu尾部加入编队
#define PLATOON_DROP_OUT_PRE   3   //obu准备退出编队
#define PLATOON_DROP_OUT       4  //obu正在退出编队
#define PLATOON_JOIN_PRE       5   //obu准备中间加入编队
#define PLATOON_JOIN           6   //obu正在中间加入编队
#define VAHICLE_FREE           7   //obu是自由车辆

//调用汽院的PlatoonControl的返回值
#define PLATOON_RET_ERR    -1  //当前状态错误
#define PLATOON_RET_DOING  0  //当前状态未执行完
#define PLATOON_RET_OK     1  //执行完了


//----------------------------------------------------------
// VUI向上调用的枚举值
//----------------------------------------------------------

//命令名称
#define UPCALL_UC_ADD_PLATOON_REQUEST     0  //创建编队
#define UPCALL_UC_SET_PLATOON_REQUEST     1  //修改编队
#define UPCALL_UC_DELETE_PLATOON_REQUEST  2  //解散编队
#define UPCALL_UC_CALL_CAR_REQUEST        3  //召车
const char *upcall_str(const int val);
bool is_my_upcall(const int val);


//----------------------------------------------------------
// 协助式换道角色的枚举值
//----------------------------------------------------------
#define COOPERATE_NONE  0  //非协助式车辆
#define COOPERATE_MAIN  1  //请求协助换道的车辆
#define COOPERATE_FRONT 2  //辅助协助换道前方车辆
#define COOPERATE_BACK  3  //辅助协助换道后方车辆
//#define COOPERATE_DONE  4  //辅助协助换道成功后保持


//----------------------------------------------------------
// 其他枚举值
//----------------------------------------------------------

//坐标转换
const double pi = 3.14159265358979324;
const double a = 6378245.0;
const double ee = 0.00669342162296594323;
const double x_pi = 3.14159265358979324 * 3000.0 / 180.0;

//障碍物上报只有5秒的生命周期，如果5秒后不再报这个障碍物，就认为移除了
#define MAX_OBSTACLE_TTL 5000

//----------------------------------------------------------
//降级驾驶相关
//----------------------------------------------------------

//车辆降级状态
#define DG_ST_NORMAL    0
#define DG_ST_CONTINUE 1
#define DG_ST_STOPCAR    2

//降级原因
#define DG_RTK_DOWN      1
#define DG_ROAD_LIMIT     2
#define DG_OTHER              3
#define DG_CONTROL         4

//查找路径的方式
#define  TASK_FR_NORMAL     0
#define  TASK_FR_CALLCAR    1
#define  TASK_FR_BLOCK_A    2//block导致的重规划
#define  TASK_FR_BLOCK_B    3

//controller上报的EPB的状态，
#define EPB_ENABLE  1       //表示EPB拉起来，刹车
#define EPB_DISABLE  0      //表示EPB没有拉起来，没有刹车
#endif
