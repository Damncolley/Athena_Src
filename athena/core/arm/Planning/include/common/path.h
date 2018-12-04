/**
 * @file
 *
 * @brief 提供关于路径的一些信息和计算方法。
 */

#pragma once
#include <vector>
#include <string>
#include "navi_point.h"

using namespace std;

/**
 * @class path
 * @brief 路径信息。
 */
class path
{
public:
    vector<navi_point> ref_points;  ///<参考路径

    int insert_pos;     ///<current insert pos;
    int current_pos;    ///<current pos of vehicle;

    int st_pos;         ///< start of motion plan pos
    int en_pos;         ///< end of motion plan pos

public:
    /**
    * @brief 构造函数
    */
     path()
    {
        insert_pos = 0;
        current_pos = 0;
        st_pos = 0;
        en_pos = 0;
    }

    /**
    * @brief 析构函数
    */
    ~path()
    {

    }

    /**
    * @brief 从文件中读取一条轨迹，赋值到参考路径ref_points
    * @param path_gps_log_file 输入量：文件名。
    */
    void read_navi_file(string path_gps_log_file);

    /**
    * @brief 将路径ref_赋值到参考路径ref_points
    * @param ref_ 输入量：输入路径。
    */
    void reset_path(const vector<navi_point>& ref_);

    /**
    * @brief 将参考路径ref_points输出到文件中
    * @param filename 输入量：文件名。
    */
    void output_navi_point_all(char *filename);



};

/**
* @brief 计算路径其中一段内各点的里程
* @param points 输入量：路径点列。
* @param start_pos 输入量：需要计算里程的起始位置。
* @param end_pos 输入量：需要计算里程的终点位置。
*/
void cau_all_mileage_of_points(vector<navi_point>&  points,  int start_pos,  int  end_pos);

/**
* @brief 计算整条路径中各点的里程
* @param virtual_lane 输入量：需要计算里程的路径。
*/
void cau_all_mileage_of_lane(path& virtual_lane);
