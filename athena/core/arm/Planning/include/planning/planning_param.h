/**
 * @file
 *
 * @brief 局部规划的相关参数。
 */

 #ifndef _ATHENA_PLANNING_PARAM_H
 #define _ATHENA_PLANNING_PARAM_H

 /**
 * @namespace athena::planning
 * @brief athena::planning
 */
namespace athena{
namespace planning{

    // 档位参数
     #define AT_STATUS_P 0              ///<档位信息，P档
     #define AT_STATUS_R 1              ///<档位信息，R档
     #define AT_STATUS_N 2              ///<档位信息，N档
     #define AT_STATUS_D 3              ///<档位信息，D档
     #define AT_STATUS_M 4              ///<档位信息，M档

    //车型相关参数
     #define WHEEL_BASE 2.5             ///<车轮轴长，单位：米
     #define CAR_LENGTH 4.5            ///<车身长，单位：米
     #define CAR_WIDTH  1.5             ///<车身宽，单位：米
     #define CAR_HIGH   1.515           ///<车身高，单位：米
     #define CAR_WEIGHT 1245            ///<车身质量，单位kg
     #define CAR_MIN_R  6.0             ///<车最小转弯半径，单位：米
     #define MAX_STEERING_ANGLE +548    ///<最大方向盘转角
     #define MIN_STEERING_ANGLE -548    ///<最小方向盘转角
     #define STEERING_RATIO 15          ///<方向盘和车轮转角的比例关系

     //泊车相关参数
     #define SAFE_DISTANCE_PARK 0.8             ///<停车的安全保护距离（前后）
     #define SAFE_WIDTH_PARK 0.4                ///<停车的安全保护宽度（两侧）
     #define PARK_LANE_WIDTH 8.0                ///<泊车时泊车通道的道路宽度
     #define H_MIN 3.0                          ///<泊车时纵向距离H的最小值
     #define H_MAX 7.0                         ///<泊车时纵向距离H的最大值
     #define H_STEP 1.0                         ///<泊车时纵向距离H的取值步长
     #define S_STEP 1.0                        ///<泊车时横向距离S的取值步长
     #define DELTA_S_MAX 5.0                     ///<泊车时横向距离S向前搜索的范围
     #define EXTENDED_LINE_OF_PARALLEL 5.0       ///<平行泊车时向前延展的距离
     #define EXTENDED_LINE_OF_VERTICAL 5.0       ///<垂直泊车时向前延展的距离
     #define PARK_SPEED 5.0                      ///<泊车时的速度，单位：km/h
     #define THRESHOLD_START_PARK 20.0          ///<车与车库中心的距离小于该阈值时，可以触发泊车
     #define THRESHOLD_CAR_STATIC_SPEED 0.1      ///<本车车速小于该阈值可以认为车静止，单位：m/s
     #define NUM_EXTEND_TRAJECTORY 600          ///<给控制发轨迹时，需要延长一些（点的个数）

    //地图匹配参数
    #define PRIOR_MATCH_LANE 1			///<优先匹配车道，1=低速道，0=高速道
    #define THRESHOLD_MATCH_CENTERS 0.5		///<匹配多车道中心线的阈值（米）
    #define GLOBAL_SEARCH_MATCH_CENTERS 10.0	///<匹配车道中心线粗搜索时的限差范围（米）
    #define NUM_BEFORE_MATCH_CENTERS 5		///<匹配车道中心线时向前搜索的点数
    #define THRESHOLD_MATCH_BIAS 10.0		///<点匹配到中心线的距离过大，认为匹配失败（米）
    #define COEF1_MOTION_WITHOUT_VP 15.0		///<没有virtual_path时（第一次规划目的地），计算MOTION_PLANNING_LENGTH的系数1（常数项）
    #define COEF2_MOTION_WITHOUT_VP 1.0		///<没有virtual_path时（第一次规划目的地），计算MOTION_PLANNING_LENGTH的系数2（横向偏差系数）
    #define COEF1_AHEAD_WITHOUT_VP 0.0		///<没有virtual_path时（第一次规划目的地），计算AHEAD_OF_MOTION_PLANNING的系数1（常数项）
    #define COEF2_AHEAD_WITHOUT_VP 0.0		///<没有virtual_path时（第一次规划目的地），计算AHEAD_OF_MOTION_PLANNING的系数2（速度系数）
    #define COEF1_MOTION_WITH_VP 10.0		///<有virtual_path时，计算MOTION_PLANNING_LENGTH的系数1（常数项）
    #define COEF2_MOTION_WITH_VP 3.0		///<有virtual_path时，计算MOTION_PLANNING_LENGTH的系数2（速度系数）
    #define COEF1_AHEAD_WITH_VP 0.5		///<有virtual_path时，计算AHEAD_OF_MOTION_PLANNING的系数1（常数项）
    #define COEF2_AHEAD_WITH_VP 0.2		///<有virtual_path时，计算AHEAD_OF_MOTION_PLANNING的系数2（速度系数）
    #define MIN_MOTION_LENGTH 5.0			///<一次规划的最小长度（米）
    #define MAX_MOTION_DELTA_HEADING 30.0		///<一次规划的最大角度差（度）
    #define INTERVAL_MAP_SECTION 1.5		///<下发的地图切片点的间隔（米）
    #define SPLINE_EVERY 0.1			///<规划轨迹点间隔（米）
    #define MAP_SPLINE_EVERY 1.5			///<地图中心线点间隔（米）
    #define MATCH_STOP_POINT_ERROR 2.0		///<匹配停车点时点距离线的最小限差（米）
    #define TRAFFIC_LIGHTS_CHECKS_LENGTH 40;      ///<路口红绿灯停车点的检查距离（个）
    #define BEFORE_LIGHTS 1;                      ///<路口提前停车距离 （个）

    //障碍物
    #define NUMBER_BACKWARD 70			///<障碍物根据边界过滤时向后搜索的点数
    #define NUMBER_FORWARD 70			///<障碍物根据边界过滤时向前搜索的点数
    #define PEDESTRIAN_WIDEN_DIS 2.5		///<行人加宽距离范围（米）
    #define PEDESTRIAN_WIDEN_ANG 0.0		///<行人加宽角度范围（度）
    #define PEDESTRIAN_WIDEN_WIDTH 0.0		///<行人加宽的宽度（米）
    #define CAR_LENGTHEN_LENGTH 0.0		///<障碍车加长的长度（米）
    #define OBSTACLE_COST_VALUE 30000		///<障碍物的代价值
    #define B_READ_OBSTACLE_SPEED 1		///<0：不读取障碍物速度；1：读取障碍物速度

    //碰撞检测
    #define COL_CHECK_INTERVAL 15			///<碰撞检测时，虚拟车道的搜索间隔（个）
    #define THRESHOLD_DELTA_LENGTH 2.0		///<虚拟车道搜索时，前后点间距超过一定的阈值，就调整搜索间隔（米）
    #define THRESHOLD_STATIC_SPEED 0.5		///<速度小于THRESHOLD_STATIC_SPEED m/s,认为是静态障碍物，需要停车或避障（m/s）
    #define RATIO_SPEED_CAR_FOLLOWING 0.7   ///障碍物速度小于本车速度该比例时，选择超车
    #define CAR_FOLLOWING_SPEED_DIFF 1.5		///<调整车速略小于跟随的障碍物速度（m/s）
    #define SAFE_WIDTH 0.5  			///<安全保护的距离（米）
    #define SAFE_LENGTH 0.3  			///<安全保护的距离（米）
    #define FREE_LENGTH_DIFF 15.0			///<如果车道都有碰撞，取碰撞距离更大的（米）
    #define COUNT_COLLISION 10			///<持续超过COUNT_COLLISION帧有碰撞才进行重规划，否则只是减速
//    #define SLOW_DOWN_STEP 0.1			///<减速的步长（程序中会重新计算）
    #define COEF_SLOW_DOWN_STEP 0.1		///<SLOW_DOWN_STEP 0.5 * motion_tar_speed / COUNT_COLLISION;
    #define COEF_COL_CHECK_LENGTH 1.5		///<决定碰撞检测长度：COLLISION_CHECK_LENGTH=COEF_COL_CHECK_LENGTH * ( MOTION_PLANNING_LENGTH + AHEAD_OF_MOTION_PLANNING );
    #define COUNT_SLOW 10			///<遇见障碍物减速后保持低速一段时间

    //轨迹规划相关参数
    #define COEF_UPDATE_TRAJECTORY 0.4		///<更新轨迹的比例参数，决定走过多少里程更新一次轨迹
    #define AHEAD_OF_MOTION_PLANNING 12		///<单位（米）
    #define MOTION_PLANNING_LENGTH 42		///<单位（米）
    #define AFTER_MOTION_PLANNING_NUM 40		///<单位（个）

    //轨迹生成【横向】
    #define LAT_OFFSET 4.0 			///<用于生成轨迹范围（左右各LAT_OFFSET米）
    #define COEF_LIMIT_LAT_STEP 0.1		///<拨杆换道限制、障碍物减速过程或者经过路口时，横向偏移范围收缩的比例系数
    #define NUM_TRAJ_CLUSTER 16			///<生成轨迹簇的个数（2×NUM_TRAJ_CLUSTER+1）
    #define NUM_BACKWARD_TRAJ_CHECK 5		///<轨迹边界检测时向后搜索的点数（个）
    #define NUM_FORWARD_TRAJ_CHECK 5		///<轨迹边界检测时向前搜索的点数（个）
    #define OUT_SIDE_INTERVAL 20			///<判断轨迹是否出界时的搜索步长（个）
    #define OUT_LINE_COST_VALUE 200		///<出车道线的代价值
    #define OUT_EDGE_COST_VALUE 60000		///<出道路边缘的代价值
    #define COEF_END_POS 20.0			///<综合计算代价值时，终点代价值的系数
    #define COEF_COL_CHECK 1.0			///<综合计算代价值时，碰撞代价值的系数
    #define COEF_LEFT 1.0				///<综合计算代价值时，左边线代价值的系数
    #define COEF_RIGHT 1.0			///<综合计算代价值时，右边线代价值的系数
    #define COEF_KS 10000.0			///<综合计算代价值时，曲率代价值的系数
    #define THRESHOLD_KS 0.285			///<当曲率大于THRESHOLD_KS时，考虑曲率代价值
    #define THRESHOLD_COST_VALUE 20000		///<当最优路径的代价值还大于THRESHOLD_COST_VALUE时，需要停车

    //轨迹生成【纵向】
    #define STOP_LENGTH_TO_OBS 100			///<停车位置距离障碍物的距离
    #define COEF_KS_SPEED 0.005			///<速度、曲率转换
    #define MIN_SPEED 1.0				///<最小速度（m/s）
    #define MAX_ACCELERATION 1.0			///<最大加速度
    #define MAX_DECELERATION 0.5			///<最大减速度

    //origin point
    #define ORIGIN_LAT 31.281675599	///<坐标原点纬度          //shanghai
    #define ORIGIN_LON 121.163174090   ///<坐标原点经度
//    #define ORIGIN_LAT 39.7409469	///<坐标原点纬度          //tongzhou
//    #define ORIGIN_LON 116.6545923   ///<坐标原点经度
//    #define ORIGIN_LAT 30.45814058804	///<坐标原点纬度          //wuhan
//    #define ORIGIN_LON 114.31801222674   ///<坐标原点经度

     #define MAP_LOG_SWITCH 0
     #define TRAJECTORY_LOG_SWITCH 0
     #define TRAFFIC_LIGHTS_LOG_SWITCH 0
     #define CHANGE_LANE_LOG_SWITCH 0
     #define LATERAL_CONTROL_LOG_SWITCH 0
     #define LONGITUDINAL_CONTROL_LOG_SWITCH 0
     #define MAP_MATCHING_LOG_SWITCH 0
     #define SELECT_VALUE_LOG_SWITCH 0
     #define VIRTUAL_PATH_LOG_SWITCH  0	///<实时匹配虚拟轨迹的状态

     #define TRAJECTORY_VIEW_SWITCH 0
     #define SPEED_PLAN_VIEW_SWITCH 0
     #define CHANGE_LANE_VIEW_SWITCH 0
     #define LATERAL_CONTROL_VIEW_SWITCH 0
     #define LONGITUDINAL_CONTROL_VIEW_SWITCH 0
     #define MAP_MATCHING_VIEW_SWITCH 0
     #define COLLISION_CHECK_VIEW_SWITCH 0
     #define PLANNING_VALUE_VIEW_SWITCH 0
     #define NEXTWORK_CHANGELANE_VIEW_SWITCH 0
     #define SELECT_VALUE_VIEW_SWITCH 0



}   //namespace planning
}   //namespace athena

#endif  //_ATHENA_PLANNING_PARAM_H
