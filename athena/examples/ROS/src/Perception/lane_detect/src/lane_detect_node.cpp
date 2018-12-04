#include <ros/ros.h>
#include <stdio.h>
#include <vector>
#include <dlfcn.h>
#include <stdlib.h>
#include <iostream>
#include "lane_utils.h"
#include <algorithm>

#include <lane_detect/PixelPoints.h>

//CV_BRIDGE
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>

//ROS Service
#include <lane_detect/LaneDetector.h>

//
#include <lane_detect/LaneDeectResult.h>

//OpenCV
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>  


//Boost
#include <boost/thread.hpp> 
#include <boost/bind.hpp>   


//#define DISPLAY_LANE_WITH_OPENCV 0

class LaneDecter{
public:
	LaneDecter(const ros::NodeHandle& nh, const ros::NodeHandle& nh_private);
	~LaneDecter();
protected:
	void init(const char* config_file);

	void onImageCallback(const sensor_msgs::Image::ConstPtr& msg);


	bool LaneDetectorServer(lane_detect::LaneDetector::Request  &req,
         lane_detect::LaneDetector::Response &res);

private:
	ros::NodeHandle nh_;
	ros::NodeHandle nh_private_;

	image_transport::ImageTransport it_;
 	image_transport::Subscriber sub_image_;
  	image_transport::Publisher pub_image_;
	
	ros::Publisher pub_lane_detection_result_,pub_lane_detection_pixel_result_;	


	ros::ServiceServer lane_detect_service_;

	std::string config_file_="";
	std::string frame_id_="";

	cv::Mat lane_detect_result_;

	lane_detect::LaneDeectResult laneDetectionResult_;
	//cv_bridge::CvImagePtr cv_ptr_in_;
	//cv_bridge::CvImagePtr cv_ptr_out_;

#ifdef SAVE_IMAGE
	int image_count;
#endif

};

LaneDecter::LaneDecter(const ros::NodeHandle& nh, const ros::NodeHandle& nh_private)
	:nh_(nh)
	,nh_private_(nh_private)
	,it_(nh_)
#ifdef SAVE_IMAGE
	,image_count(0)
#endif
{
	//Param
	nh_private_.param<std::string>("config_file", config_file_, "line_config.ini");
	nh_private_.param<std::string>("frame_id", frame_id_, "usb_camera");

	//Init
	init(config_file_.c_str());

	// Subscrive to input video feed and publish output video feed
    	sub_image_ = it_.subscribe("/usb_cam/image_raw", 1,&LaneDecter::onImageCallback, this);
    	pub_image_ = it_.advertise("/usb_cam/image_lane_detect", 1);

	pub_lane_detection_result_ = nh_.advertise<lane_detect::LaneDeectResult>("laneDetectQuality",1);
	pub_lane_detection_pixel_result_ = nh_.advertise<lane_detect::PixelPoints>("laneDetectPixelPoints",1);
	//Service
	lane_detect_service_ = nh_.advertiseService("laneDetect", &LaneDecter::LaneDetectorServer,this);

}

LaneDecter::~LaneDecter(){}


void LaneDecter::init(const char* config_file)
{
	::init(config_file);
}


void LaneDecter::onImageCallback(const sensor_msgs::Image::ConstPtr& msg)
{

	cv_bridge::CvImagePtr cv_ptr;
    	try
    	{
      		cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    	}
    	catch (cv_bridge::Exception& e)
    	{
      		ROS_ERROR("cv_bridge exception: %s", e.what());
      		return;
    	}

	setImage(cv_ptr->image);
	lane_detect_result_ = getResultImage();


	//tranform cv::Mat to sensor_msgs/Image through cv_bridge
	std_msgs::Header header;
	header.stamp = msg->header.stamp;
	header.frame_id = frame_id_;
	std::string encoding("bgr8");
	cv_bridge::CvImagePtr cv_ptr_result = cv_bridge::CvImagePtr(new cv_bridge::CvImage(header,encoding,lane_detect_result_));

	pub_image_.publish(cv_ptr_result->toImageMsg());

	//lane detect quality
	int lane_quality;
        std::vector<point> leftPoints, rightPoints, leftImagePoints, rightImagePoints;
        getLaneInfos(&lane_quality, &leftPoints, &rightPoints);
	getLaneImageInfos(&leftImagePoints, &rightImagePoints);

#ifdef DISPLAY_LANE_WITH_OPENCV
	std::vector<cv::KeyPoint> points;	
	for(int i=0;i<leftImagePoints.size();i++)
	{
		points.push_back(cv::KeyPoint(leftImagePoints.at(i).x,leftImagePoints.at(i).y,2.0));
	}
	cv::drawKeypoints(cv_ptr->image,points,cv_ptr->image,cv::Scalar(0,255,0));
	cv::imshow("lane points",cv_ptr->image);
	cv::waitKey(1);
#endif //DISPLAY_LANE_WITH_OPENCV

	//Publish lane detection pixel point
	lane_detect::PixelPoints pixelPoints;
	geometry_msgs::Point32 pixelPoint;
	for(int i=0;i<leftImagePoints.size();i++)
	{

		pixelPoint.x = leftImagePoints.at(i).x;
		pixelPoint.y = leftImagePoints.at(i).y;

		pixelPoints.leftpoints.push_back(pixelPoint);

	}
	for(int i=0;i<rightImagePoints.size();i++)
	{
		pixelPoint.x = rightImagePoints.at(i).x;
		pixelPoint.y = rightImagePoints.at(i).y;

		pixelPoints.rightpoints.push_back(pixelPoint);
	}


	pixelPoints.header.stamp = msg->header.stamp;
	pixelPoints.header.frame_id = "lane_pixel";
	pub_lane_detection_pixel_result_.publish(pixelPoints);



	//Publish lane detection result
	laneDetectionResult_.header.stamp = msg->header.stamp;
	laneDetectionResult_.header.frame_id = frame_id_;
	laneDetectionResult_.quality = 	lane_quality;
	for(auto iter=leftPoints.begin(); iter != leftPoints.end(); iter++)
	{ 
		lane_detect::LanePoint lanePoint;
		lanePoint.x = iter->x;
		lanePoint.y = iter->y;
		lanePoint.road_x = iter->road_x;
		lanePoint.road_y = iter->road_y;
		
		laneDetectionResult_.left_points.push_back(lanePoint);
	}

	for(auto iter=rightPoints.begin(); iter != rightPoints.end(); iter++)
	{
		lane_detect::LanePoint lanePoint;
		lanePoint.x = iter->x;
		lanePoint.y = iter->y;
		lanePoint.road_x = iter->road_x;
		lanePoint.road_y = iter->road_y;
		
		laneDetectionResult_.right_points.push_back(lanePoint);
	}

	pub_lane_detection_result_.publish(laneDetectionResult_);
}


bool LaneDecter::LaneDetectorServer(lane_detect::LaneDetector::Request  &req,
         lane_detect::LaneDetector::Response &res)
{
	return true;
}






int main(int argc, char** argv)
{
	ros::init(argc, argv, "lane_detector_node");

	ros::NodeHandle nh;
	ros::NodeHandle nh_private("~");

	LaneDecter laneDetector(nh,nh_private);

	ros::spin();
	
	return 0;
}


