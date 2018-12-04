/**
 * @file coordinate_transformation.h
 * @author jiang <jiangchengjie@indrv.cn>
 * @date  2018-07-07
 * @version 1.0.0
 * @par  Copyright(c)
 *        hy
 */


#ifndef MAP_MATCHING_COORDINATE_TRANSFORMATION_H_
#define MAP_MATCHING_COORDINATE_TRANSFORMATION_H_

#include "../../localization.h"
#include "../../local_localization.h"
#include "../../controller_config.h"

using namespace athena::control;

using namespace athena::control;

class CoordinateTransformation{
public:
    CoordinateTransformation()  = default;
    ~CoordinateTransformation() = default;


/**
 * @brief coordinate_transformation.
 * @param[in] localization 定位信息输入.
 * @param[out] local_localization 定位转换信息输入.
 * @return void.
 */
    void coordinate_transformation(const Localization * localiation,
                                   LocalLocalization & local_localization,const ControllerConfig * controller_config);
private:
    double HeadingSpeedFilterDirect(double heading_speed);
};

#endif // MAP_MATCHING_COORDINATE_TRANSFORMATION_H_
