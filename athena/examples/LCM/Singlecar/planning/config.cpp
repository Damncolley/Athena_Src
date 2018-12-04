#include "config.h"

using namespace std;

using namespace std;

std::string OBU_URL ="udpm://239.255.76.63:7603?ttl=3";

///<档位参数
int AT_STATUS_P = 0;                          ///<档位信息，P档
int AT_STATUS_R = 1;                          ///<档位信息，R档
int AT_STATUS_N = 2;                          ///<档位信息，N档
int AT_STATUS_D = 3;                          ///<档位信息，D档
int AT_STATUS_M = 4;                          ///<档位信息，M档

///<车型相关参数
double WHEEL_BASE = 2.5;                      ///<车轮轴长，单位：米
double CAR_LENGTH = 4.5;                      ///<车身长，单位：米
double CAR_WIDTH  = 1.5;                      ///<车身宽，单位：米
double CAR_HIGH   = 1.515;                    ///<车身高，单位：米
double CAR_WEIGHT = 1245;                     ///<车身质量，单位kg
double CAR_MIN_R  = 6.0;                      ///<车最小转弯半径，单位：米
double MAX_STEERING_ANGLE = +548;    	      ///<最大方向盘转角
double MIN_STEERING_ANGLE = -548;             ///<最小方向盘转角
double STEERING_RATIO = 15;                   ///<方向盘和车轮转角的比例关系

///<泊车相关参数
double SAFE_DISTANCE_PARK = 0.8;              ///<停车的安全保护距离（前后）
double SAFE_WIDTH_PARK = 0.4;                 ///<停车的安全保护宽度（两侧）
double PARK_LANE_WIDTH = 8.0;                 ///<泊车时泊车通道的道路宽度
double H_MIN = 3.0;                           ///<泊车时纵向距离H的最小值
double H_MAX = 7.0;                           ///<泊车时纵向距离H的最大值
double H_STEP = 1.0;                          ///<泊车时纵向距离H的取值步长
double S_STEP = 1.0;                          ///<泊车时横向距离S的取值步长
double DELTA_S_MAX = 5.0;                     ///<泊车时横向距离S向前搜索的范围
double EXTENDED_LINE_OF_PARALLEL = 5.0;       ///<平行泊车时向前延展的距离
double EXTENDED_LINE_OF_VERTICAL = 5.0;       ///<垂直泊车时向前延展的距离
double PARK_SPEED = 5.0;                      ///<泊车时的速度，单位：km/h
double THRESHOLD_START_PARK = 20.0;           ///<车与车库中心的距离小于该阈值时，可以触发泊车
double THRESHOLD_CAR_STATIC_SPEED = 0.1;      ///<本车车速小于该阈值可以认为车静止，单位：m/s
int NUM_EXTEND_TRAJECTORY = 600;              ///<给控制发轨迹时，需要延长一些（点的个数）

///<地图匹配参数
int SOURCE_OF_CAR_SPEED = 0;			        ///0:从惯导获得本车当前速度;1:从can信号获得本车当前速度
int PRIOR_MATCH_LANE = 1;			          ///<优先匹配车道，1=低速道，0=高速道
double THRESHOLD_MATCH_CENTERS = 0.5;		  ///<匹配多车道中心线的阈值（米）
double GLOBAL_SEARCH_MATCH_CENTERS = 10.0;	  ///<匹配车道中心线粗搜索时的限差范围（米）
int NUM_BEFORE_MATCH_CENTERS = 5;		      ///<匹配车道中心线时向前搜索的点数
double THRESHOLD_MATCH_BIAS = 10.0;		      ///<点匹配到中心线的距离过大，认为匹配失败（米）
double COEF1_MOTION_WITHOUT_VP = 15.0;		  ///<没有virtual_path时（第一次规划目的地），计算MOTION_PLANNING_LENGTH的系数1（常数项）
double COEF2_MOTION_WITHOUT_VP = 1.0;		  ///<没有virtual_path时（第一次规划目的地），计算MOTION_PLANNING_LENGTH的系数2（横向偏差系数）
double COEF1_AHEAD_WITHOUT_VP = 0.0;		  ///<没有virtual_path时（第一次规划目的地），计算AHEAD_OF_MOTION_PLANNING的系数1（常数项）
double COEF2_AHEAD_WITHOUT_VP = 0.0;		  ///<没有virtual_path时（第一次规划目的地），计算AHEAD_OF_MOTION_PLANNING的系数2（速度系数）
double COEF1_MOTION_WITH_VP = 10.0;		      ///<有virtual_path时，计算MOTION_PLANNING_LENGTH的系数1（常数项）
double COEF2_MOTION_WITH_VP = 3.0;		      ///<有virtual_path时，计算MOTION_PLANNING_LENGTH的系数2（速度系数）
double THRESHOLD_HIGH_SPEED = 10.0;		      ///<速度高时用另一套系数
double COEF1_MOTION_HIGH_SPEED = 10.0;		      ///<高速时时，计算MOTION_PLANNING_LENGTH的系数1（常数项）
double COEF2_MOTION_HIGH_SPEED = 3.0;		      ///<高速时时，计算MOTION_PLANNING_LENGTH的系数2（速度系数）
double COEF1_AHEAD_WITH_VP = 0.5;		      ///<有virtual_path时，计算AHEAD_OF_MOTION_PLANNING的系数1（常数项）
double COEF2_AHEAD_WITH_VP = 0.2;		      ///<有virtual_path时，计算AHEAD_OF_MOTION_PLANNING的系数2（速度系数）
double MIN_MOTION_LENGTH = 5.0;			      ///<一次规划的最小长度（米）
double MAX_MOTION_LENGTH = 80.0;			  ///<一次规划的最大长度（米）
double MAX_MOTION_DELTA_HEADING = 30.0;		  ///<一次规划的最大角度差（度）
double INTERVAL_MAP_SECTION = 1.5;		      ///<下发的地图切片点的间隔（米）
double SPLINE_EVERY = 0.1;			          ///<规划轨迹点间隔（米）
double MAP_SPLINE_EVERY = 1.5;			      ///<地图中心线点间隔（米）
double MATCH_STOP_POINT_ERROR = 2.0;		  ///<匹配停车点时点距离线的最小限差（米）
int TRAFFIC_LIGHTS_CHECKS_LENGTH = 40;        ///<路口红绿灯停车点的检查距离（个）
int BEFORE_LIGHTS = 1;                        ///<路口提前停车距离 （个）

///<障碍物
int NUMBER_BACKWARD = 70;			          ///<障碍物根据边界过滤时向后搜索的点数
int NUMBER_FORWARD = 70;			          ///<障碍物根据边界过滤时向前搜索的点数
double PEDESTRIAN_WIDEN_DIS = 2.5;		      ///<行人加宽距离范围（米）
double PEDESTRIAN_WIDEN_ANG = 0.0;		      ///<行人加宽角度范围（度）
double PEDESTRIAN_WIDEN_WIDTH = 0.0;		  ///<行人加宽的宽度（米）
double CAR_LENGTHEN_LENGTH = 0.0;		      ///<障碍车加长的长度（米）
int OBSTACLE_COST_VALUE = 30000;		      ///<障碍物的代价值
int B_READ_OBSTACLE_SPEED = 1;		          ///<0：不读取障碍物速度；1：读取障碍物速度

///<碰撞检测
int COL_CHECK_INTERVAL = 15;			      ///<碰撞检测时，虚拟车道的搜索间隔（个）
double THRESHOLD_DELTA_LENGTH = 2.0;		  ///<虚拟车道搜索时，前后点间距超过一定的阈值，就调整搜索间隔（米）
double THRESHOLD_STATIC_SPEED = 0.5;		  ///<速度小于THRESHOLD_STATIC_SPEED m/s,认为是静态障碍物，需要停车或避障（m/s）
double RATIO_SPEED_CAR_FOLLOWING = 0.7;       ///<障碍物速度小于本车速度该比例时，选择超车
double CAR_FOLLOWING_SPEED_DIFF = 1.5;		  ///<调整车速略小于跟随的障碍物速度（m/s）
double SAFE_WIDTH = 0.5;  			          ///<安全保护的距离（米）
double SAFE_LENGTH = 0.3;  			          ///<安全保护的距离（米）
double FREE_LENGTH_DIFF = 15.0;			      ///<如果车道都有碰撞，取碰撞距离更大的（米）
int COUNT_COLLISION = 10;			          ///<持续超过COUNT_COLLISION帧有碰撞才进行重规划，否则只是减速
//#   SLOW_DOWN_STEP 0.1                       #减速的步长（程序中会重新计算）
double COEF_SLOW_DOWN_STEP = 0.1;		      ///<SLOW_DOWN_STEP 0.5 * motion_tar_speed / COUNT_COLLISION;
double COEF_COL_CHECK_LENGTH = 1.5;	          ///<决定碰撞检测长度：COLLISION_CHECK_LENGTH=COEF_COL_CHECK_LENGTH * ( MOTION_PLANNING_LENGTH + AHEAD_OF_MOTION_PLANNING );
int COUNT_SLOW = 10;				          ///<遇见障碍物减速后保持低速一段时间

///#轨迹规划相关参数
double COEF_UPDATE_TRAJECTORY = 0.4;		  ///<更新轨迹的比例参数，决定走过多少里程更新一次轨迹
int AHEAD_OF_MOTION_PLANNING = 12;		      ///<单位（米）
int MOTION_PLANNING_LENGTH = 42;		      ///<单位（米）
int AFTER_MOTION_PLANNING_NUM = 40;		      ///<单位（个）

///#轨迹生成【横向】
double LAT_OFFSET = 4.0; 			          ///<用于生成轨迹范围（左右各LAT_OFFSET米）
double COEF_LIMIT_LAT_STEP = 0.1;		      ///<拨杆换道限制、障碍物减速过程或者经过路口时，横向偏移范围收缩的比例系数
int NUM_TRAJ_CLUSTER = 16;			          ///<生成轨迹簇的个数（2×NUM_TRAJ_CLUSTER+1）
int NUM_BACKWARD_TRAJ_CHECK = 5;		      ///<轨迹边界检测时向后搜索的点数（个）
int NUM_FORWARD_TRAJ_CHECK = 5;		          ///<轨迹边界检测时向前搜索的点数（个）
int OUT_SIDE_INTERVAL = 20;			          ///<判断轨迹是否出界时的搜索步长（个）
int OUT_LINE_COST_VALUE = 200;		          ///<出车道线的代价值
int OUT_EDGE_COST_VALUE = 60000;		      ///<出道路边缘的代价值
double COEF_END_POS = 20.0;			          ///<综合计算代价值时，终点代价值的系数
double COEF_COL_CHECK = 1.0;			      ///<综合计算代价值时，碰撞代价值的系数
double COEF_LEFT = 1.0;				          ///<综合计算代价值时，左边线代价值的系数
double COEF_RIGHT = 1.0;			          ///<综合计算代价值时，右边线代价值的系数
double COEF_KS = 10000.0;			          ///<综合计算代价值时，曲率代价值的系数
double THRESHOLD_KS = 0.285;			      ///<当曲率大于THRESHOLD_KS时，考虑曲率代价值
int THRESHOLD_COST_VALUE = 20000;		      ///<当最优路径的代价值还大于THRESHOLD_COST_VALUE时，需要停车

///#各种测试开关
int MAP_LOG_SWITCH = 0;
int TRAJECTORY_LOG_SWITCH = 0;
int TRAFFIC_LIGHTS_LOG_SWITCH = 0;
int CHANGE_LANE_LOG_SWITCH = 0;
int OBSTACLES_LOG_SWITCH = 0;
int LONGITUDINAL_CONTROL_LOG_SWITCH = 0;
int MAP_MATCHING_LOG_SWITCH = 0;
int SELECT_VALUE_LOG_SWITCH = 0;
int VIRTUAL_PATH_LOG_SWITCH = 0;              ///<实时匹配虚拟轨迹的状态

int TRAJECTORY_VIEW_SWITCH = 0;
int SPEED_PLAN_VIEW_SWITCH = 0;
int CHANGE_LANE_VIEW_SWITCH = 0;
int LATERAL_CONTROL_VIEW_SWITCH = 0;
int LONGITUDINAL_CONTROL_VIEW_SWITCH = 0;
int MAP_MATCHING_VIEW_SWITCH = 0;
int COLLISION_CHECK_VIEW_SWITCH = 0;
int PLANNING_VALUE_VIEW_SWITCH = 0;
int NEXTWORK_CHANGELANE_VIEW_SWITCH = 0;
int SELECT_VALUE_VIEW_SWITCH = 0;

///#轨迹生成【纵向】
int STOP_LENGTH_TO_OBS = 100;	                ///<停车位置距离障碍物的距离
double COEF_KS_SPEED = 0.005;			        ///<速度、曲率转换
double MIN_SPEED = 1.0;				            ///<最小速度（m/s）
double MAX_ACCELERATION = 1.0;			        ///<最大加速度
double MAX_DECELERATION = 0.5;			        ///<最大减速度

///#origin point
double ORIGIN_LAT = 31.281675599;	            ///<坐标原点纬度          #shanghai
double ORIGIN_LON = 121.163174090;              ///<坐标原点经度


bool read_motion_plan_config_value_from_file()
{
	const char ConfigFile[] = "planning_value.cfg";
	Config configSettings(ConfigFile);

	//开始读取配置文件
	OBU_URL = "udpm://239.255.76.22:7622?ttl=3";
	OBU_URL = configSettings.Read("OBU_URL", OBU_URL);

    // 档位参数
    AT_STATUS_P = configSettings.Read("AT_STATUS_P",0);
    AT_STATUS_R = configSettings.Read("AT_STATUS_R",1);
    AT_STATUS_N = configSettings.Read("AT_STATUS_N",2);
    AT_STATUS_D = configSettings.Read("AT_STATUS_D",3);
    AT_STATUS_M = configSettings.Read("AT_STATUS_M",4);

    //车型相关参数
    WHEEL_BASE = configSettings.Read("WHEEL_BASE",2.5);
    CAR_LENGTH = configSettings.Read("CAR_LENGTH",4.5);
    CAR_WIDTH  = configSettings.Read("CAR_WIDTH",1.5);
    CAR_HIGH   = configSettings.Read("CAR_HIGH",1.515);
    CAR_WEIGHT = configSettings.Read("CAR_WEIGHT",1245);  //kg
    CAR_MIN_R  = configSettings.Read("CAR_MIN_R",6.0);
    MAX_STEERING_ANGLE   = configSettings.Read("MAX_STEERING_ANGLE",+548);
    MIN_STEERING_ANGLE = configSettings.Read("MIN_STEERING_ANGLE",-548);  //kg
    STEERING_RATIO  = configSettings.Read("STEERING_RATIO",15);

    //泊车相关参数
    SAFE_DISTANCE_PARK = configSettings.Read("SAFE_DISTANCE_PARK",0.8);
    SAFE_WIDTH_PARK = configSettings.Read("SAFE_WIDTH_PARK",0.4);
    PARK_LANE_WIDTH = configSettings.Read("PARK_LANE_WIDTH",8.0);
    H_MIN = configSettings.Read("H_MIN",3.0);
    H_MAX = configSettings.Read("H_MAX",7.0);
    H_STEP = configSettings.Read("H_STEP",1.0);
    S_STEP = configSettings.Read("S_STEP",1.0);
    DELTA_S_MAX  = configSettings.Read("DELTA_S_MAX",5.0);
    EXTENDED_LINE_OF_PARALLEL   = configSettings.Read("EXTENDED_LINE_OF_PARALLEL",5.0);
    EXTENDED_LINE_OF_VERTICAL = configSettings.Read("EXTENDED_LINE_OF_VERTICAL",5.0);  //kg
    PARK_SPEED  = configSettings.Read("PARK_SPEED",5.0);
    THRESHOLD_START_PARK   = configSettings.Read("THRESHOLD_START_PARK",20.0);
    THRESHOLD_CAR_STATIC_SPEED = configSettings.Read("THRESHOLD_CAR_STATIC_SPEED",0.1);  //kg
    NUM_EXTEND_TRAJECTORY  = configSettings.Read("NUM_EXTEND_TRAJECTORY",600);

    //地图匹配参数
    SOURCE_OF_CAR_SPEED = configSettings.Read("SOURCE_OF_CAR_SPEED",0);
    PRIOR_MATCH_LANE = configSettings.Read("PRIOR_MATCH_LANE",1);
    THRESHOLD_MATCH_CENTERS = configSettings.Read("THRESHOLD_MATCH_CENTERS",0.5);
    GLOBAL_SEARCH_MATCH_CENTERS = configSettings.Read("GLOBAL_SEARCH_MATCH_CENTERS",10.0);
    NUM_BEFORE_MATCH_CENTERS = configSettings.Read("NUM_BEFORE_MATCH_CENTERS",5);
    THRESHOLD_MATCH_BIAS = configSettings.Read("THRESHOLD_MATCH_BIAS",10.0);
    COEF1_MOTION_WITHOUT_VP = configSettings.Read("COEF1_MOTION_WITHOUT_VP",15.0);
    COEF2_MOTION_WITHOUT_VP = configSettings.Read("COEF2_MOTION_WITHOUT_VP",1.0);
    COEF1_AHEAD_WITHOUT_VP = configSettings.Read("COEF1_AHEAD_WITHOUT_VP",0.0);
    COEF2_AHEAD_WITHOUT_VP = configSettings.Read("COEF2_AHEAD_WITHOUT_VP",0.0);
    COEF1_MOTION_WITH_VP = configSettings.Read("COEF1_MOTION_WITH_VP",10.0);
    COEF2_MOTION_WITH_VP = configSettings.Read("COEF2_MOTION_WITH_VP",3.0);
    THRESHOLD_HIGH_SPEED = configSettings.Read("THRESHOLD_HIGH_SPEED",0.0);
    COEF1_MOTION_HIGH_SPEED = configSettings.Read("COEF1_MOTION_HIGH_SPEED",0.0);
    COEF2_MOTION_HIGH_SPEED = configSettings.Read("COEF2_MOTION_HIGH_SPEED",0.0);
    COEF1_AHEAD_WITH_VP = configSettings.Read("COEF1_AHEAD_WITH_VP",0.5);
    COEF2_AHEAD_WITH_VP = configSettings.Read("COEF2_AHEAD_WITH_VP",0.2);
    MIN_MOTION_LENGTH = configSettings.Read("MIN_MOTION_LENGTH",5.0);
    MAX_MOTION_LENGTH = configSettings.Read("MAX_MOTION_LENGTH",80.0);
    MAX_MOTION_DELTA_HEADING = configSettings.Read("MAX_MOTION_DELTA_HEADING",30.0);
    INTERVAL_MAP_SECTION = configSettings.Read("INTERVAL_MAP_SECTION",1.5);
    SPLINE_EVERY = configSettings.Read("SPLINE_EVERY",0.1);
    MAP_SPLINE_EVERY = configSettings.Read("MAP_SPLINE_EVERY",1.5);
    MATCH_STOP_POINT_ERROR = configSettings.Read("MATCH_STOP_POINT_ERROR",2.0);
    TRAFFIC_LIGHTS_CHECKS_LENGTH = configSettings.Read("TRAFFIC_LIGHTS_CHECKS_LENGTH",40);
    BEFORE_LIGHTS = configSettings.Read("BEFORE_LIGHTS",1);

    //障碍物
    NUMBER_BACKWARD = configSettings.Read("NUMBER_BACKWARD",70);
    NUMBER_FORWARD = configSettings.Read("NUMBER_FORWARD",70);
    PEDESTRIAN_WIDEN_DIS = configSettings.Read("PEDESTRIAN_WIDEN_DIS",2.5);
    PEDESTRIAN_WIDEN_ANG = configSettings.Read("PEDESTRIAN_WIDEN_ANG",0.0);
    PEDESTRIAN_WIDEN_WIDTH = configSettings.Read("PEDESTRIAN_WIDEN_WIDTH",0.0);
    CAR_LENGTHEN_LENGTH = configSettings.Read("CAR_LENGTHEN_LENGTH",0.0);
    OBSTACLE_COST_VALUE = configSettings.Read("OBSTACLE_COST_VALUE",30000);
    B_READ_OBSTACLE_SPEED = configSettings.Read("B_READ_OBSTACLE_SPEED",1);

    //碰撞检测
    COL_CHECK_INTERVAL = configSettings.Read("COL_CHECK_INTERVAL",15);
    THRESHOLD_DELTA_LENGTH = configSettings.Read("THRESHOLD_DELTA_LENGTH",2.0);
    THRESHOLD_STATIC_SPEED = configSettings.Read("THRESHOLD_STATIC_SPEED",0.5);
    RATIO_SPEED_CAR_FOLLOWING = configSettings.Read("RATIO_SPEED_CAR_FOLLOWING",0.7);
    CAR_FOLLOWING_SPEED_DIFF = configSettings.Read("CAR_FOLLOWING_SPEED_DIFF",1.5);
    SAFE_WIDTH = configSettings.Read("SAFE_WIDTH",0.5);
    SAFE_LENGTH = configSettings.Read("SAFE_LENGTH",0.3);
    FREE_LENGTH_DIFF = configSettings.Read("FREE_LENGTH_DIFF",15.0);
    COUNT_COLLISION = configSettings.Read("COUNT_COLLISION",10);
    //SLOW_DOWN_STEP = configSettings.Read("SLOW_DOWN_STEP",0.1);
    COEF_SLOW_DOWN_STEP = configSettings.Read("COEF_SLOW_DOWN_STEP",0.1);
    COEF_COL_CHECK_LENGTH = configSettings.Read("COEF_COL_CHECK_LENGTH",1.5);
    COUNT_SLOW = configSettings.Read("COUNT_SLOW",10);

    //轨迹规划相关参数
    COEF_UPDATE_TRAJECTORY = configSettings.Read("COEF_UPDATE_TRAJECTORY",0.4);
    AHEAD_OF_MOTION_PLANNING = configSettings.Read("AHEAD_OF_MOTION_PLANNING",12);   //安全保护的距离
    MOTION_PLANNING_LENGTH = configSettings.Read("MOTION_PLANNING_LENGTH",42);   //安全保护的距离
    AFTER_MOTION_PLANNING_NUM = configSettings.Read("AFTER_MOTION_PLANNING_NUM",40);   //安全保护的距离

    //轨迹生成【横向】
    LAT_OFFSET = configSettings.Read("LAT_OFFSET",4.0);
    COEF_LIMIT_LAT_STEP = configSettings.Read("COEF_LIMIT_LAT_STEP",0.1);
    NUM_TRAJ_CLUSTER = configSettings.Read("NUM_TRAJ_CLUSTER",16);
    NUM_BACKWARD_TRAJ_CHECK = configSettings.Read("NUM_BACKWARD_TRAJ_CHECK",5);
    NUM_FORWARD_TRAJ_CHECK = configSettings.Read("NUM_FORWARD_TRAJ_CHECK",5);
    OUT_SIDE_INTERVAL = configSettings.Read("OUT_SIDE_INTERVAL",20);
    OUT_LINE_COST_VALUE = configSettings.Read("OUT_LINE_COST_VALUE",200);
    OUT_EDGE_COST_VALUE = configSettings.Read("OUT_EDGE_COST_VALUE",60000);
    COEF_END_POS = configSettings.Read("COEF_END_POS",20.0);
    COEF_COL_CHECK = configSettings.Read("COEF_COL_CHECK",1.0);
    COEF_LEFT = configSettings.Read("COEF_LEFT",1.0);
    COEF_RIGHT = configSettings.Read("COEF_RIGHT",1.0);
    COEF_KS = configSettings.Read("COEF_KS",10000.0);
    THRESHOLD_KS = configSettings.Read("THRESHOLD_KS",0.285);
    THRESHOLD_COST_VALUE = configSettings.Read("THRESHOLD_COST_VALUE",20000);

    //各种测试开关
    MAP_LOG_SWITCH = configSettings.Read("MAP_LOG_SWITCH",0);
    TRAJECTORY_LOG_SWITCH = configSettings.Read("TRAJECTORY_LOG_SWITCH",0);
    TRAFFIC_LIGHTS_LOG_SWITCH = configSettings.Read("TRAFFIC_LIGHTS_LOG_SWITCH",0);
    CHANGE_LANE_LOG_SWITCH = configSettings.Read("CHANGE_LANE_LOG_SWITCH",0);
    OBSTACLES_LOG_SWITCH = configSettings.Read("OBSTACLES_LOG_SWITCH",0);
    LONGITUDINAL_CONTROL_LOG_SWITCH = configSettings.Read("LONGITUDINAL_CONTROL_LOG_SWITCH",0);
    MAP_MATCHING_LOG_SWITCH = configSettings.Read("MAP_MATCHING_LOG_SWITCH",0);
    SELECT_VALUE_LOG_SWITCH = configSettings.Read("SELECT_VALUE_LOG_SWITCH",0);
    VIRTUAL_PATH_LOG_SWITCH = configSettings.Read("VIRTUAL_PATH_LOG_SWITCH",0);
    TRAJECTORY_VIEW_SWITCH = configSettings.Read("TRAJECTORY_VIEW_SWITCH",0);
    SPEED_PLAN_VIEW_SWITCH = configSettings.Read("SPEED_PLAN_VIEW_SWITCH",0);
    CHANGE_LANE_VIEW_SWITCH = configSettings.Read("CHANGE_LANE_VIEW_SWITCH",0);
    LATERAL_CONTROL_VIEW_SWITCH = configSettings.Read("LATERAL_CONTROL_VIEW_SWITCH",0);
    LONGITUDINAL_CONTROL_VIEW_SWITCH = configSettings.Read("LONGITUDINAL_CONTROL_VIEW_SWITCH",0);
    MAP_MATCHING_VIEW_SWITCH = configSettings.Read("MAP_MATCHING_VIEW_SWITCH",0);
    COLLISION_CHECK_VIEW_SWITCH = configSettings.Read("COLLISION_CHECK_VIEW_SWITCH",0);
    PLANNING_VALUE_VIEW_SWITCH = configSettings.Read("PLANNING_VALUE_VIEW_SWITCH",0);
    NEXTWORK_CHANGELANE_VIEW_SWITCH = configSettings.Read("NEXTWORK_CHANGELANE_VIEW_SWITCH",0);
    SELECT_VALUE_VIEW_SWITCH = configSettings.Read("SELECT_VALUE_VIEW_SWITCH",0);

    //轨迹生成【纵向】
    STOP_LENGTH_TO_OBS = configSettings.Read("STOP_LENGTH_TO_OBS",100);
    COEF_KS_SPEED = configSettings.Read("COEF_KS_SPEED",0.005);
    MIN_SPEED = configSettings.Read("MIN_SPEED",1.0);
    MAX_ACCELERATION = configSettings.Read("MAX_ACCELERATION",1.0);
    MAX_DECELERATION = configSettings.Read("MAX_DECELERATION",0.5);

    //#origin point
    ORIGIN_LAT = configSettings.Read("ORIGIN_LAT",31.281675599);
    ORIGIN_LON = configSettings.Read("ORIGIN_LON",121.163174090);


    return true;
}



bool write_config_value_from_file()
{
    return true;
}



/************************************************************* Config ************************************************************************/
Config::Config( string filename,
                string delimiter,
                string comment )
    : m_Delimiter(delimiter), m_Comment(comment)
{
    std::ifstream in( filename.c_str() );

    if( !in ) throw File_not_found( filename );

    in >> (*this);
}


Config::Config()
    : m_Delimiter( string(1,'=') ), m_Comment( string(1,'#') )
{

}

bool Config::KeyExists( const string& key ) const
{
    // Indicate whether key is found
    mapci p = m_Contents.find( key );
    return ( p != m_Contents.end() );
}


/* static */
void Config::Trim( string& inout_s )
{
    static const char whitespace[] = " \n\t\v\r\f";
    inout_s.erase( 0, inout_s.find_first_not_of(whitespace) );
    inout_s.erase( inout_s.find_last_not_of(whitespace) + 1U );
}


std::ostream& operator<<( std::ostream& os, const Config& cf )
{
    // Save a Config to os
    for( Config::mapci p = cf.m_Contents.begin();
         p != cf.m_Contents.end();
         ++p )
    {
        os << p->first << " " << cf.m_Delimiter << " ";
        os << p->second << std::endl;
    }
    return os;
}

void Config::Remove( const string& key )
{
    m_Contents.erase( m_Contents.find( key ) );
    return;
}

std::istream& operator>>( std::istream& is, Config& cf )
{
    // Load a Config from is
    // Read in keys and values, keeping internal whitespace
    typedef string::size_type pos;
    const string& delim  = cf.m_Delimiter;  // separator
    const string& comm   = cf.m_Comment;    // comment
    const pos skip = delim.length();        // length of separator

    string nextline = "";  // might need to read ahead to see where value ends

    while( is || nextline.length() > 0 )
    {
        // Read an entire line at a time
        string line;
        if( nextline.length() > 0 )
        {
            line = nextline;  // we read ahead; use it now
            nextline = "";
        }
        else
        {
            std::getline( is, line );
        }

        // Ignore comments
        line = line.substr( 0, line.find(comm) );

        // Parse the line if it contains a delimiter
        pos delimPos = line.find( delim );
        if( delimPos < string::npos )
        {
            // Extract the key
            string key = line.substr( 0, delimPos );
            line.replace( 0, delimPos+skip, "" );

            // See if value continues on the next line
            // Stop at blank line, next line with a key, end of stream,
            // or end of file sentry
            bool terminate = false;
            while( !terminate && is )
            {
                std::getline( is, nextline );
                terminate = true;

                string nlcopy = nextline;
                Config::Trim(nlcopy);
                if( nlcopy == "" ) continue;

                nextline = nextline.substr( 0, nextline.find(comm) );
                if( nextline.find(delim) != string::npos )
                    continue;

                nlcopy = nextline;
                Config::Trim(nlcopy);
                if( nlcopy != "" ) line += "\n";
                line += nextline;
                terminate = false;
            }

            // Store key and value
            Config::Trim(key);
            Config::Trim(line);
            cf.m_Contents[key] = line;  // overwrites if key is repeated
        }
    }

    return is;
}
bool Config::FileExist(std::string filename)
{
    bool exist= false;
    std::ifstream in( filename.c_str() );
    if( in )
        exist = true;
    return exist;
}

void Config::ReadFile( string filename,
                       string delimiter,
                       string comment )
{
    m_Delimiter = delimiter;
    m_Comment = comment;
    std::ifstream in( filename.c_str() );

    if( !in )
       throw File_not_found( filename );

    in >> (*this);
}

