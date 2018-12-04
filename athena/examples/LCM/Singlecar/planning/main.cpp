/**
 * @file
 *
 * @brief a planning demo of lcm.
 */

#include <iostream>

#include <lcm/lcm.h>
#include <lcm/lcm-cpp.hpp>


#include "planning_lcm_msg.h"
#include "planning_node.h"



#include "planning/planning.h"
#include "common/car_state.h"

using namespace std;


///主程序
int main(int argc, char *argv[])
{
    cout << " Motion planning start ! 2018.11.19: 1:25. @ by liming in Guanggu" << endl;

    PlanningNode pn;
    pn.run();

    return 0;
}

///park test 主程序
//int main(int argc, char *argv[])
//{
//    cout << " Motion planning start ! 2018.07.28: 15:25. @ by liming in Guanggu" << endl;
//
//    IntelligentParking ip;
//    path park_trajectory_d, park_trajectory_r;
//
//    ///车停止位置
//    CarState car_state;
//    car_state.car_pose_.CurrentX_ = -4.0;
//    car_state.car_pose_.CurrentY_ = 10.0;
//    car_state.car_pose_.Current_heading_ = 90.0;
//
//    ///生成轨迹
//    ip.compute_parking_trajectory( car_state );
//
//    ///D档部分
//    ip.get_trajectory_d( park_trajectory_d );
//    ip.extend_trajectory( park_trajectory_d, NUM_EXTEND_TRAJECTORY, 0.1, AT_STATUS_D );
//
//    ///R档部分
//    ip.get_trajectory_r( park_trajectory_r );
//    ip.extend_trajectory( park_trajectory_r, NUM_EXTEND_TRAJECTORY, 0.1, AT_STATUS_R );
//
//    return 0;
//}



