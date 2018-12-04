//Config.h
#pragma once

#include <string>
#include <map>
#include <iostream>
#include <fstream>
#include <sstream>

extern std::string OBU_URL;

///<档位参数
extern int AT_STATUS_P;                          ///<档位信息，P档
extern int AT_STATUS_R;                          ///<档位信息，R档
extern int AT_STATUS_N;                          ///<档位信息，N档
extern int AT_STATUS_D;                          ///<档位信息，D档
extern int AT_STATUS_M;                          ///<档位信息，M档

///<车型相关参数
extern double WHEEL_BASE;                      ///<车轮轴长，单位：米
extern double CAR_LENGTH;                      ///<车身长，单位：米
extern double CAR_WIDTH;                       ///<车身宽，单位：米
extern double CAR_HIGH;                        ///<车身高，单位：米
extern double CAR_WEIGHT;                      ///<车身质量，单位kg
extern double CAR_MIN_R;                       ///<车最小转弯半径，单位：米
extern double MAX_STEERING_ANGLE;    	       ///<最大方向盘转角
extern double MIN_STEERING_ANGLE;              ///<最小方向盘转角
extern double STEERING_RATIO;                  ///<方向盘和车轮转角的比例关系

///<泊车相关参数
extern double SAFE_DISTANCE_PARK;              ///<停车的安全保护距离（前后）
extern double SAFE_WIDTH_PARK;                 ///<停车的安全保护宽度（两侧）
extern double PARK_LANE_WIDTH;                 ///<泊车时泊车通道的道路宽度
extern double H_MIN;                           ///<泊车时纵向距离H的最小值
extern double H_MAX;                           ///<泊车时纵向距离H的最大值
extern double H_STEP;                          ///<泊车时纵向距离H的取值步长
extern double S_STEP;                          ///<泊车时横向距离S的取值步长
extern double DELTA_S_MAX;                     ///<泊车时横向距离S向前搜索的范围
extern double EXTENDED_LINE_OF_PARALLEL;       ///<平行泊车时向前延展的距离
extern double EXTENDED_LINE_OF_VERTICAL;       ///<垂直泊车时向前延展的距离
extern double PARK_SPEED;                      ///<泊车时的速度，单位：km/h
extern double THRESHOLD_START_PARK;            ///<车与车库中心的距离小于该阈值时，可以触发泊车
extern double THRESHOLD_CAR_STATIC_SPEED;      ///<本车车速小于该阈值可以认为车静止，单位：m/s
extern int NUM_EXTEND_TRAJECTORY;              ///<给控制发轨迹时，需要延长一些（点的个数）

///<地图匹配参数
extern int SOURCE_OF_CAR_SPEED;			        ///0:从惯导获得本车当前速度;1:从can信号获得本车当前速度
extern int PRIOR_MATCH_LANE;			        ///<优先匹配车道，1=低速道，0=高速道
extern double THRESHOLD_MATCH_CENTERS;		    ///<匹配多车道中心线的阈值（米）
extern double GLOBAL_SEARCH_MATCH_CENTERS;	    ///<匹配车道中心线粗搜索时的限差范围（米）
extern int NUM_BEFORE_MATCH_CENTERS;		    ///<匹配车道中心线时向前搜索的点数
extern double THRESHOLD_MATCH_BIAS;		        ///<点匹配到中心线的距离过大，认为匹配失败（米）
extern double COEF1_MOTION_WITHOUT_VP;		    ///<没有virtual_path时（第一次规划目的地），计算MOTION_PLANNING_LENGTH的系数1（常数项）
extern double COEF2_MOTION_WITHOUT_VP;		    ///<没有virtual_path时（第一次规划目的地），计算MOTION_PLANNING_LENGTH的系数2（横向偏差系数）
extern double COEF1_AHEAD_WITHOUT_VP;		    ///<没有virtual_path时（第一次规划目的地），计算AHEAD_OF_MOTION_PLANNING的系数1（常数项）
extern double COEF2_AHEAD_WITHOUT_VP;		    ///<没有virtual_path时（第一次规划目的地），计算AHEAD_OF_MOTION_PLANNING的系数2（速度系数）
extern double COEF1_MOTION_WITH_VP;		        ///<有virtual_path时，计算MOTION_PLANNING_LENGTH的系数1（常数项）
extern double COEF2_MOTION_WITH_VP;		        ///<有virtual_path时，计算MOTION_PLANNING_LENGTH的系数2（速度系数）
extern double THRESHOLD_HIGH_SPEED;		        ///<速度高时用另一套系数
extern double COEF1_MOTION_HIGH_SPEED;		        ///<高速时，计算MOTION_PLANNING_LENGTH的系数1（常数项）
extern double COEF2_MOTION_HIGH_SPEED;		        ///<高速时，计算MOTION_PLANNING_LENGTH的系数2（速度系数）
extern double COEF1_AHEAD_WITH_VP;		        ///<有virtual_path时，计算AHEAD_OF_MOTION_PLANNING的系数1（常数项）
extern double COEF2_AHEAD_WITH_VP;		        ///<有virtual_path时，计算AHEAD_OF_MOTION_PLANNING的系数2（速度系数）
extern double MIN_MOTION_LENGTH;			    ///<一次规划的最小长度（米）
extern double MAX_MOTION_LENGTH;		    	///<一次规划的最大长度（米）
extern double MAX_MOTION_DELTA_HEADING;		    ///<一次规划的最大角度差（度）
extern double INTERVAL_MAP_SECTION;		        ///<下发的地图切片点的间隔（米）
extern double SPLINE_EVERY;			            ///<规划轨迹点间隔（米）
extern double MAP_SPLINE_EVERY;			        ///<地图中心线点间隔（米）
extern double MATCH_STOP_POINT_ERROR;		    ///<匹配停车点时点距离线的最小限差（米）
extern int TRAFFIC_LIGHTS_CHECKS_LENGTH;        ///<路口红绿灯停车点的检查距离（个）
extern int BEFORE_LIGHTS;                       ///<路口提前停车距离 （个）

///<障碍物
extern int NUMBER_BACKWARD;			            ///<障碍物根据边界过滤时向后搜索的点数
extern int NUMBER_FORWARD;			            ///<障碍物根据边界过滤时向前搜索的点数
extern double PEDESTRIAN_WIDEN_DIS;		        ///<行人加宽距离范围（米）
extern double PEDESTRIAN_WIDEN_ANG;		        ///<行人加宽角度范围（度）
extern double PEDESTRIAN_WIDEN_WIDTH;		    ///<行人加宽的宽度（米）
extern double CAR_LENGTHEN_LENGTH;		        ///<障碍车加长的长度（米）
extern int OBSTACLE_COST_VALUE;		            ///<障碍物的代价值
extern int B_READ_OBSTACLE_SPEED;		        ///<0：不读取障碍物速度；1：读取障碍物速度

///<碰撞检测
extern int COL_CHECK_INTERVAL;			         ///<碰撞检测时，虚拟车道的搜索间隔（个）
extern double THRESHOLD_DELTA_LENGTH;	 	     ///<虚拟车道搜索时，前后点间距超过一定的阈值，就调整搜索间隔（米）
extern double THRESHOLD_STATIC_SPEED;		     ///<速度小于THRESHOLD_STATIC_SPEED m/s,认为是静态障碍物，需要停车或避障（m/s）
extern double RATIO_SPEED_CAR_FOLLOWING;         ///<障碍物速度小于本车速度该比例时，选择超车
extern double CAR_FOLLOWING_SPEED_DIFF;		     ///<调整车速略小于跟随的障碍物速度（m/s）
extern double SAFE_WIDTH;  			             ///<安全保护的距离（米）
extern double SAFE_LENGTH;  			         ///<安全保护的距离（米）
extern double FREE_LENGTH_DIFF;			         ///<如果车道都有碰撞，取碰撞距离更大的（米）
extern int COUNT_COLLISION;			             ///<持续超过COUNT_COLLISION帧有碰撞才进行重规划，否则只是减速
//#   SLOW_DOWN_STEP 0.1                         #减速的步长（程序中会重新计算）
extern double COEF_SLOW_DOWN_STEP;		         ///<SLOW_DOWN_STEP 0.5 * motion_tar_speed / COUNT_COLLISION;
extern double COEF_COL_CHECK_LENGTH;	         ///<决定碰撞检测长度：COLLISION_CHECK_LENGTH=COEF_COL_CHECK_LENGTH * ( MOTION_PLANNING_LENGTH + AHEAD_OF_MOTION_PLANNING );
extern int COUNT_SLOW;				             ///<遇见障碍物减速后保持低速一段时间

///<轨迹规划相关参数
extern double COEF_UPDATE_TRAJECTORY;    		 ///<更新轨迹的比例参数，决定走过多少里程更新一次轨迹
extern int AHEAD_OF_MOTION_PLANNING;		     ///<单位（米）
extern int MOTION_PLANNING_LENGTH;		         ///<单位（米）
extern int AFTER_MOTION_PLANNING_NUM;		     ///<单位（个）

///<轨迹生成【横向】
extern double LAT_OFFSET; 			             ///<用于生成轨迹范围（左右各LAT_OFFSET米）
extern double COEF_LIMIT_LAT_STEP;		         ///<拨杆换道限制、障碍物减速过程或者经过路口时，横向偏移范围收缩的比例系数
extern int NUM_TRAJ_CLUSTER;			         ///<生成轨迹簇的个数（2×NUM_TRAJ_CLUSTER+1）
extern int NUM_BACKWARD_TRAJ_CHECK;		         ///<轨迹边界检测时向后搜索的点数（个）
extern int NUM_FORWARD_TRAJ_CHECK;		         ///<轨迹边界检测时向前搜索的点数（个）
extern int OUT_SIDE_INTERVAL;			         ///<判断轨迹是否出界时的搜索步长（个）
extern int OUT_LINE_COST_VALUE;		             ///<出车道线的代价值
extern int OUT_EDGE_COST_VALUE;		             ///<出道路边缘的代价值
extern double COEF_END_POS;			             ///<综合计算代价值时，终点代价值的系数
extern double COEF_COL_CHECK;			         ///<综合计算代价值时，碰撞代价值的系数
extern double COEF_LEFT;				         ///<综合计算代价值时，左边线代价值的系数
extern double COEF_RIGHT;			             ///<综合计算代价值时，右边线代价值的系数
extern double COEF_KS;			                 ///<综合计算代价值时，曲率代价值的系数
extern double THRESHOLD_KS;			             ///<当曲率大于THRESHOLD_KS时，考虑曲率代价值
extern int THRESHOLD_COST_VALUE;		         ///<当最优路径的代价值还大于THRESHOLD_COST_VALUE时，需要停车

///<各种测试开关
extern int MAP_LOG_SWITCH;
extern int TRAJECTORY_LOG_SWITCH;
extern int TRAFFIC_LIGHTS_LOG_SWITCH;
extern int CHANGE_LANE_LOG_SWITCH;
extern int OBSTACLES_LOG_SWITCH;
extern int LONGITUDINAL_CONTROL_LOG_SWITCH;
extern int MAP_MATCHING_LOG_SWITCH;
extern int SELECT_VALUE_LOG_SWITCH;
extern int VIRTUAL_PATH_LOG_SWITCH;	              ///<实时匹配虚拟轨迹的状态

extern int TRAJECTORY_VIEW_SWITCH;
extern int SPEED_PLAN_VIEW_SWITCH;
extern int CHANGE_LANE_VIEW_SWITCH;
extern int LATERAL_CONTROL_VIEW_SWITCH;
extern int LONGITUDINAL_CONTROL_VIEW_SWITCH;
extern int MAP_MATCHING_VIEW_SWITCH;
extern int COLLISION_CHECK_VIEW_SWITCH;
extern int PLANNING_VALUE_VIEW_SWITCH;
extern int NEXTWORK_CHANGELANE_VIEW_SWITCH;
extern int SELECT_VALUE_VIEW_SWITCH;

///<轨迹生成【纵向】
extern int STOP_LENGTH_TO_OBS;	                  ///<停车位置距离障碍物的距离
extern double COEF_KS_SPEED;		              ///<速度、曲率转换
extern double MIN_SPEED;				          ///<最小速度（m/s）
extern double MAX_ACCELERATION;			          ///<最大加速度
extern double MAX_DECELERATION;			          ///<最大减速度

///<origin point
extern double ORIGIN_LAT;	                      ///<坐标原点纬度          #shanghai
extern double ORIGIN_LON;                         ///<坐标原点经度

bool read_motion_plan_config_value_from_file();
bool write_config_value_from_file();



/************************************************************* Config ************************************************************************/
/*
* \brief Generic configuration Class
*
*/
class Config {
	// Data
protected:
	std::string m_Delimiter;  //!< separator between key and value
	std::string m_Comment;    //!< separator between value and comments
	std::map<std::string,std::string> m_Contents;  //!< extracted keys and values

	typedef std::map<std::string,std::string>::iterator mapi;
	typedef std::map<std::string,std::string>::const_iterator mapci;
	// Methods
public:

	Config( std::string filename,std::string delimiter = "=",std::string comment = "#" );
	Config();
	template<class T> T Read( const std::string& in_key ) const;  //!<Search for key and read value or optional default value, call as read<T>
	template<class T> T Read( const std::string& in_key, const T& in_value ) const;
	template<class T> bool ReadInto( T& out_var, const std::string& in_key ) const;
	template<class T>
	bool ReadInto( T& out_var, const std::string& in_key, const T& in_value ) const;
	bool FileExist(std::string filename);
	void ReadFile(std::string filename,std::string delimiter = "=",std::string comment = "#" );

	// Check whether key exists in configuration
	bool KeyExists( const std::string& in_key ) const;

	// Modify keys and values
	template<class T> void Add( const std::string& in_key, const T& in_value );
	void Remove( const std::string& in_key );

	// Check or change configuration syntax
	std::string GetDelimiter() const { return m_Delimiter; }
	std::string GetComment() const { return m_Comment; }
	std::string SetDelimiter( const std::string& in_s )
	{ std::string old = m_Delimiter;  m_Delimiter = in_s;  return old; }
	std::string SetComment( const std::string& in_s )
	{ std::string old = m_Comment;  m_Comment =  in_s;  return old; }

	// Write or read configuration
	friend std::ostream& operator<<( std::ostream& os, const Config& cf );
	friend std::istream& operator>>( std::istream& is, Config& cf );

protected:
	template<class T> static std::string T_as_string( const T& t );
	template<class T> static T string_as_T( const std::string& s );
	static void Trim( std::string& inout_s );


	// Exception types
public:
	struct File_not_found {
		std::string filename;
		File_not_found( const std::string& filename_ = std::string() )
			: filename(filename_) {} };
		struct Key_not_found {  // thrown only by T read(key) variant of read()
			std::string key;
			Key_not_found( const std::string& key_ = std::string() )
				: key(key_) {} };
};


/* static */
template<class T>
std::string Config::T_as_string( const T& t )
{
	// Convert from a T to a string
	// Type T must support << operator
	std::ostringstream ost;
	ost << t;
	return ost.str();
}


/* static */
template<class T>
T Config::string_as_T( const std::string& s )
{
	// Convert from a string to a T
	// Type T must support >> operator
	T t;
	std::istringstream ist(s);
	ist >> t;
	return t;
}


/* static */
template<>
inline std::string Config::string_as_T<std::string>( const std::string& s )
{
	// Convert from a string to a string
	// In other words, do nothing
	return s;
}


/* static */
template<>
inline bool Config::string_as_T<bool>( const std::string& s )
{
	// Convert from a string to a bool
	// Interpret "false", "F", "no", "n", "0" as false
	// Interpret "true", "T", "yes", "y", "1", "-1", or anything else as true
	bool b = true;
	std::string sup = s;
	for( std::string::iterator p = sup.begin(); p != sup.end(); ++p )
		*p = toupper(*p);  // make string all caps
	if( sup==std::string("FALSE") || sup==std::string("F") ||
		sup==std::string("NO") || sup==std::string("N") ||
		sup==std::string("0") || sup==std::string("NONE") )
		b = false;
	return b;
}


template<class T>
T Config::Read( const std::string& key ) const
{
	// Read the value corresponding to key
	mapci p = m_Contents.find(key);
	if( p == m_Contents.end() ) throw Key_not_found(key);
	return string_as_T<T>( p->second );
}


template<class T>
T Config::Read( const std::string& key, const T& value ) const
{
	// Return the value corresponding to key or given default value
	// if key is not found
	mapci p = m_Contents.find(key);
	if( p == m_Contents.end() ) return value;
	return string_as_T<T>( p->second );
}


template<class T>
bool Config::ReadInto( T& var, const std::string& key ) const
{
	// Get the value corresponding to key and store in var
	// Return true if key is found
	// Otherwise leave var untouched
	mapci p = m_Contents.find(key);
	bool found = ( p != m_Contents.end() );
	if( found ) var = string_as_T<T>( p->second );
	return found;
}


template<class T>
bool Config::ReadInto( T& var, const std::string& key, const T& value ) const
{
	// Get the value corresponding to key and store in var
	// Return true if key is found
	// Otherwise set var to given default
	mapci p = m_Contents.find(key);
	bool found = ( p != m_Contents.end() );
	if( found )
		var = string_as_T<T>( p->second );
	else
		var = value;
	return found;
}


template<class T>
void Config::Add( const std::string& in_key, const T& value )
{
	// Add a key with given value
	std::string v = T_as_string( value );
	std::string key=in_key;
	Trim(key);
	Trim(v);
	m_Contents[key] = v;
	return;
}

