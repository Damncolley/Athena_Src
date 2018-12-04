/**
 * @file
 *
 * @brief a planning demo of lcm.
 */

#include <iostream>

//#include "planning/planning.h"
#include "common/car_state.h"
#include "common/enum_list.h"
#include "park/park.h"

#define NUM_EXTEND_TRAJECTORY 600              ///<给控制发轨迹时，需要延长一些（点的个数）
///<档位参数
#define AT_STATUS_P 0                          ///<档位信息，P档
#define AT_STATUS_R 1                          ///<档位信息，R档
#define AT_STATUS_N 2                          ///<档位信息，N档
#define AT_STATUS_D 3                          ///<档位信息，D档
#define AT_STATUS_M 4                          ///<档位信息，M档


using namespace std;

///park test 主程序
int main(int argc, char *argv[])
{
    cout << " Intelligent Parking start ! 2018.07.28: 15:25. @ by liming in Guanggu" << endl;

    IntelligentParking ip;
    path park_trajectory_d, park_trajectory_r;

    ///停车给信息
    StopPark sp;
    sp.x_ = 20.0;
    sp.y_ = 10.0;
    sp.heading_ = 60.0;
    sp.length_ = 8.5;
    sp.width_ = 2.3;
    sp.type_ = STOP_TYPE_PARALLEL_PARK;

    ip.set_park_point( sp );

    ///车停止位置
    CarState car_state;
    car_state.car_pose_.CurrentX_ = -4.0;
    car_state.car_pose_.CurrentY_ = 10.0;
    car_state.car_pose_.Current_heading_ = 90.0;

    ///生成轨迹
    ip.compute_parking_trajectory( car_state );

    ///D档部分
    ip.get_trajectory_d( park_trajectory_d );
    ip.extend_trajectory( park_trajectory_d, NUM_EXTEND_TRAJECTORY, 0.1, AT_STATUS_D );

    ///R档部分
    ip.get_trajectory_r( park_trajectory_r );
    ip.extend_trajectory( park_trajectory_r, NUM_EXTEND_TRAJECTORY, 0.1, AT_STATUS_R );

    return 0;
}



