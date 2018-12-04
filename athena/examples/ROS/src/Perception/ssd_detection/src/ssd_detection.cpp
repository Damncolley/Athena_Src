#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>  


#include "ssd_detection/vision_detect_node.hpp"
#include "ssd_detection/kf_tracker.hpp"

#include <ssd_detection/SSD_Objects.h>

using namespace caffe;
using namespace std;


class VisionDetector
{
public:
	VisionDetector(const ros::NodeHandle& nh, const ros::NodeHandle& nh_private,std::string config_file);

protected:
	void onImageCallback(const sensor_msgs::Image::ConstPtr& msg);

private:

	ros::NodeHandle nh_;
	ros::NodeHandle nh_private_;
	ros::Publisher ssd_detection_result_;

	image_transport::ImageTransport it_;
 	image_transport::Subscriber image_sub_;
  	image_transport::Publisher image_pub_;

	Visiondetect caffe_detect_;
};


VisionDetector::VisionDetector(const ros::NodeHandle& nh, const ros::NodeHandle& nh_private,std::string config_file)
	:nh_(nh)
	,nh_private_(nh_private)
	,it_(nh_)
	,caffe_detect_(Visiondetect(config_file))
{
	ROS_INFO("ROS ssd init begin.");
	//Caffe detector
	caffe_detect_.read_config_value_from_file();
	ROS_INFO("ROS ssd init 1 .");
    	caffe_detect_.ssd_detector_ = new Detector(caffe_detect_.network_Model, caffe_detect_.pretrained_Weights,caffe_detect_.mean_file_,caffe_detect_.pixel_mean_,caffe_detect_.use_gpu, 		caffe_detect_.gpu_device_id);

	ROS_INFO("ROS ssd init 2.");
	//ROS
	image_sub_ = it_.subscribe("/usb_cam/image_raw",1,&VisionDetector::onImageCallback,this);
	image_pub_ = it_.advertise("/ssd_detection_result", 1);

	ssd_detection_result_ = nh_.advertise<ssd_detection::SSD_Objects>("ssd_detection_objects",1);
	ROS_INFO("ROS ssd init success.");
}

void VisionDetector::onImageCallback(const sensor_msgs::Image::ConstPtr& msg)
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

	//ssd detection
	double t = (double)cv::getTickCount();
    	vector<camera_obj>  get_cam_objs;
    	caffe_detect_.convert_rect_to_image_obj(cv_ptr->image,get_cam_objs);
    	t = ((double)cv::getTickCount() - t) / cv::getTickFrequency();


	//ROS_INFO("object size: %d",static_cast<int>(get_cam_objs.size()));
	ssd_detection::SSD_Objects ssd_object_list;
	ssd_detection::SSD_Object  ssd_object;
	for(vector<camera_obj>::iterator iter = get_cam_objs.begin(); iter != get_cam_objs.end(); iter++)
	{
		ssd_object.id                 = iter->id;
		ssd_object.stability          = iter->stability;
		ssd_object.classification     = iter->classification;          	
		ssd_object.detection_status   = iter->detection_status;            
		ssd_object.lat_pos            = iter->lat_pos;                      
		ssd_object.lat_rate           = iter->lat_rate;                    
		ssd_object.lon_pos            = iter->lon_pos;
		ssd_object.lon_rate           = iter->lon_rate;
		ssd_object.width              = iter->width;             
		ssd_object.score              = iter->score;    
		  
		geometry_msgs::Point32 pointf;
		pointf.x = iter->box_point.x;
		pointf.y = iter->box_point.y;
		pointf.z = 0.0;
		ssd_object.obj_rect.pointf =  pointf;
		ssd_object.obj_rect.width = iter->box_point.width;
		ssd_object.obj_rect.height = iter->box_point.height;    
		ssd_object.real_data          = iter->real_data;                 
		ssd_object.lifespan           = iter->lifespan;

		ssd_object_list.objects.push_back(ssd_object);
	}
	ssd_object_list.header.frame_id = "ssd";
	ssd_object_list.header.stamp = msg->header.stamp;
	ssd_detection_result_.publish(ssd_object_list);

    	float fps=1.0 / t+2;
    	string String ="FPS:"+ static_cast<ostringstream*>( &(ostringstream() << fps) )->str();
    	cv::putText(cv_ptr->image,String,cvPoint(0,40),CV_FONT_HERSHEY_PLAIN,2,cv::Scalar(255, 255, 0),2);

	// Output modified video stream
	image_pub_.publish(cv_ptr->toImageMsg());
}


int main(int argc, char **argv)
{
	ros::init(argc, argv,"ssd_detection_node");
	ros::NodeHandle nh;
	ros::NodeHandle nh_private("~");

	std::string detection_config_file = "";
	nh_private.getParam("detection_config_file", detection_config_file);
	ROS_INFO("config file: %s",detection_config_file.c_str());
	if(detection_config_file == "")
	{
		ROS_ERROR("Please select detection config file");
		return -1;
	}
	VisionDetector visionDetector(nh,nh_private,detection_config_file);

	ros::spin();

	return 0;
}

