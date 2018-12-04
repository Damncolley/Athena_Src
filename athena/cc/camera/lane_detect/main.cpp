#include <stdio.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <iostream>
#include "lane_utils.h"

using namespace std;
using namespace cv;

int main()
{
	init();
	cv::Mat Input(720, 1280, CV_8UC3);
	cv::Mat Output(720, 1280, CV_8UC3);
    const int bufSize = 256;
    char Base_name[bufSize];
    string file_path = "/home/zombie/dcw/gj_new/gjphoto";
    cout << "input the first image i:" << endl;
    int picture_ID = 1;
    while (cin >> picture_ID)
    {
        for (; picture_ID < 20000; picture_ID++)
        {
            sprintf(Base_name,  "%06d.png", picture_ID);
            string image_path = file_path + "/" + Base_name;
            //cout << image_path << endl;
            string image_path_out = file_path + "/" + Base_name;
            Input = cv::imread(image_path, 1);
            setImage(Input);
            Output = getResultImage();
            imshow("out", Output);
            cvWaitKey(1);

            int lane_quality;
            vector<point> leftPoints, rightPoints;
            getLaneInfos(&lane_quality, leftPoints, rightPoints);
        }
    }
    return 0;
}
