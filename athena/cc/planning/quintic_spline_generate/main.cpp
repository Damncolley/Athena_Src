#include <iostream>
#include <fstream>
#include <vector>
#include "spline/quintic_spline.h"
//#include "spline/navi_point.h"

using namespace std;

int main()
{
    navi_point p0, p1;
    vector<navi_point> out_points;
    QuinticSpline quintic_spline;

    ///给起点赋值（坐标，方向，曲率）
    p0.position_x = 1.0;
    p0.position_y = 2.0;
    p0.heading = 45.0;
    p0.k_s = 0.06;

    ///给终点赋值（坐标，方向）
    p1.position_x = 20.0;
    p1.position_y = 10.0;
    p1.heading = 15.0;
    p1.k_s = 0.06;


    ///生成轨迹
    if ( quintic_spline.gen_trj_from_two_points( p0, p1, 0.1 ) )
    {
        cout << "------- generate trajectory successfully -------" << endl;
        quintic_spline.get_path_global( out_points );
        cout << "size : " << out_points.size() << endl;

    }

    /*log*/
    int out_log = 1;
    if(out_log)
    {
        ofstream outfile("trj_quintic_spline_log.log", std::ios::app);
        outfile.precision(8);

        for(int i=0; i < out_points.size(); i++)
        {
            outfile << " i " << i
                    << " x " << out_points[i].position_x
                    << " y " << out_points[i].position_y
                    << " h " << out_points[i].heading
                    << " k " << out_points[i].k_s
                    << endl;
            //outfile << output_virtual_path.ref_points[i].k_s << endl;
        }

        outfile << endl;
        outfile.close();
    }


    return 0;
}


