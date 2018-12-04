#include <ros/ros.h>

#include <ssd_detection/SSD_Objects.h>
#include <lane_detect/PixelPoints.h>
#include <sensor_msgs/Image.h>

//CFG
#include <dynamic_reconfigure/server.h>
#include <display/DisplayConfig.h>


//SYNC
#include <message_filters/subscriber.h>
#include <message_filters/synchronizer.h>
#include <message_filters/sync_policies/approximate_time.h>


//CV_bridge
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>

//OpenCV
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>



class Display{
public:

	typedef message_filters::sync_policies::ApproximateTime<sensor_msgs::Image,lane_detect::PixelPoints, ssd_detection::SSD_Objects> DisplayAllSyncPolicy;
	typedef message_filters::sync_policies::ApproximateTime<sensor_msgs::Image,ssd_detection::SSD_Objects> DisplaySSDSyncPolicy;
	typedef message_filters::sync_policies::ApproximateTime<sensor_msgs::Image,lane_detect::PixelPoints> DisplayLaneSyncPolicy;


	typedef struct DisplayConfig{
		bool enable_show_ssd;
		int ssd_r;
		int ssd_g;
		int ssd_b;
		double ssd_line_width;
		bool enable_show_label;		

		bool enable_show_lane;
		int lane_r;
		int lane_g;
		int lane_b;
		double lane_line_width;
	}DisplayConfig;
	Display(const ros::NodeHandle& nh, const ros::NodeHandle& nh_private);
	~Display();


	
protected:
	void onLaneDisplayCb(const sensor_msgs::Image::ConstPtr& img, const lane_detect::PixelPoints::ConstPtr& msg);
	void onSSDDisplayCb(const sensor_msgs::Image::ConstPtr& img,const ssd_detection::SSD_Objects::ConstPtr& msg);
	void onLaneSSDDisplayCb(const sensor_msgs::Image::ConstPtr& img,const lane_detect::PixelPoints::ConstPtr& lanes, const ssd_detection::SSD_Objects::ConstPtr& objects);
	void onImageCb(const sensor_msgs::Image::ConstPtr& img);



	//cfg
	void configCallback(display::DisplayConfig &config, uint32_t level);

private:
	ros::NodeHandle  nh_;
	ros::NodeHandle  nh_private_;

	ros::Publisher pub_warpper_image_;


	//CFG
	dynamic_reconfigure::Server<display::DisplayConfig> server_;
    	dynamic_reconfigure::Server<display::DisplayConfig>::CallbackType cfg_cb_;
	DisplayConfig displayConfig_;

	message_filters::Subscriber<lane_detect::PixelPoints> sub_lane_detect_result_;
	message_filters::Subscriber<ssd_detection::SSD_Objects> sub_SSD_result_;
	message_filters::Subscriber<sensor_msgs::Image> sub_image_raw_;

	message_filters::Synchronizer<DisplayLaneSyncPolicy>* sync_lane_;
	message_filters::Synchronizer<DisplaySSDSyncPolicy>* sync_ssd_;
	message_filters::Synchronizer<DisplayAllSyncPolicy>* sync_all_;
};


Display::Display(const ros::NodeHandle& nh, const ros::NodeHandle& nh_private)
	:nh_(nh)
	,nh_private_(nh_private)
	,sub_lane_detect_result_(nh_,"laneDetectPixelPoints",10)
	,sub_SSD_result_(nh_,"ssd_detection_objects",10)
	,sub_image_raw_(nh_,"/usb_cam/image_raw",10)
{
	//cfg
	cfg_cb_ = boost::bind(&Display::configCallback,this, _1, _2);
    	server_.setCallback(cfg_cb_);

	//Publisher
	pub_warpper_image_ = nh_.advertise<sensor_msgs::Image>("warpper_image",5);
}
	
Display::~Display()
{
	
}

void Display::onLaneDisplayCb(const sensor_msgs::Image::ConstPtr& img,const lane_detect::PixelPoints::ConstPtr& msg)
{
	cv_bridge::CvImagePtr cv_ptr;
    	try
    	{
      		cv_ptr = cv_bridge::toCvCopy(img, sensor_msgs::image_encodings::BGR8);
    	}
    	catch (cv_bridge::Exception& e)
    	{
      		ROS_ERROR("cv_bridge exception: %s", e.what());
      		return;
    	}

	//Draw lanes
	std::vector<cv::KeyPoint> points;	
	for(int i=0;i<msg->leftpoints.size();i++)
	{
		points.push_back(cv::KeyPoint(msg->leftpoints.at(i).x,msg->leftpoints.at(i).y,displayConfig_.lane_line_width));
	}

	for(int i=0;i<msg->rightpoints.size();i++)
	{
		points.push_back(cv::KeyPoint(msg->rightpoints.at(i).x,msg->rightpoints.at(i).y,displayConfig_.lane_line_width));
	}

	cv::drawKeypoints(cv_ptr->image,points,cv_ptr->image,cv::Scalar(displayConfig_.lane_r,displayConfig_.lane_g,displayConfig_.lane_b));

	pub_warpper_image_.publish(cv_ptr->toImageMsg());
	
}
void Display::onSSDDisplayCb(const sensor_msgs::Image::ConstPtr& img,const ssd_detection::SSD_Objects::ConstPtr& msg)
{
	cv_bridge::CvImagePtr cv_ptr;
    	try
    	{
      		cv_ptr = cv_bridge::toCvCopy(img, sensor_msgs::image_encodings::BGR8);
    	}
    	catch (cv_bridge::Exception& e)
    	{
      		ROS_ERROR("cv_bridge exception: %s", e.what());
      		return;
    	}

	for(int i=0;i<msg->objects.size();i++)
	{
		cv::rectangle(cv_ptr->image,cv::Rect(msg->objects.at(i).obj_rect.pointf.x,msg->objects.at(i).obj_rect.pointf.y,msg->objects.at(i).obj_rect.width,msg->objects.at(i).obj_rect.height),cv::Scalar(displayConfig_.ssd_r,displayConfig_.ssd_g,displayConfig_.ssd_b),displayConfig_.ssd_line_width,1,0);

		if(displayConfig_.enable_show_label)
		{
			char* label = NULL;
			switch(msg->objects.at(i).classification)
			{
				case 7:
					label = "car";
					break;
				case 15:
					label = "person";
					break;
				case 6:
					label = "bus";
					break;
				default:
					break;
					
			}
			//cv::putText(cv_ptr->image,std::string(label), cv::Point(0,0), 1.0,cv::FONT_HERSHEY_PLAIN, cv::Scalar(displayConfig_.ssd_r,displayConfig_.ssd_g,displayConfig_.ssd_b));
		}
	}

	pub_warpper_image_.publish(cv_ptr->toImageMsg());
	
}



void Display::onLaneSSDDisplayCb(const sensor_msgs::Image::ConstPtr& img,const lane_detect::PixelPoints::ConstPtr& lanes, const ssd_detection::SSD_Objects::ConstPtr& objects)
{
	cv_bridge::CvImagePtr cv_ptr;
    	try
    	{
      		cv_ptr = cv_bridge::toCvCopy(img, sensor_msgs::image_encodings::BGR8);
    	}
    	catch (cv_bridge::Exception& e)
    	{
      		ROS_ERROR("cv_bridge exception: %s", e.what());
      		return;
    	}
	
	//Draw Lanes
	std::vector<cv::KeyPoint> points;	
	for(int i=0;i<lanes->leftpoints.size();i++)
	{
		points.push_back(cv::KeyPoint(lanes->leftpoints.at(i).x,lanes->leftpoints.at(i).y,displayConfig_.lane_line_width));
	}

	for(int i=0;i<lanes->rightpoints.size();i++)
	{
		points.push_back(cv::KeyPoint(lanes->rightpoints.at(i).x,lanes->rightpoints.at(i).y,displayConfig_.lane_line_width));
	}

	cv::drawKeypoints(cv_ptr->image,points,cv_ptr->image,cv::Scalar(displayConfig_.lane_b,displayConfig_.lane_g,displayConfig_.lane_r));


	//Draw Object boxes
	for(int i=0;i<objects->objects.size();i++)
	{
		cv::rectangle(cv_ptr->image,cv::Rect(objects->objects.at(i).obj_rect.pointf.x,objects->objects.at(i).obj_rect.pointf.y,objects->objects.at(i).obj_rect.width,objects->objects.at(i).obj_rect.height),cv::Scalar(displayConfig_.ssd_b,displayConfig_.ssd_g,displayConfig_.ssd_r),displayConfig_.ssd_line_width,1,0);

		if(displayConfig_.enable_show_label)
		{
			char* label = NULL;
			switch(objects->objects.at(i).classification)
			{
				case 7:
					label = "car";
					break;
				case 15:
					label = "person";
					break;
				case 6:
					label = "bus";
					break;
				default:
					break;
					
			}
			//cv::putText(cv_ptr->image,std::string(label), cv::Point(0,0),1.0,cv::FONT_HERSHEY_PLAIN, cv::Scalar(displayConfig_.ssd_r,displayConfig_.ssd_g,displayConfig_.ssd_b));
		}

	}


	pub_warpper_image_.publish(cv_ptr->toImageMsg());
}


void Display::onImageCb(const sensor_msgs::Image::ConstPtr& img)
{
	pub_warpper_image_.publish(img);
}






void Display::configCallback(display::DisplayConfig &config, uint32_t level)
{
	ROS_INFO("Dynamic request. SSD cv::Scalar(%d,%d, %d), Lane cv::Scalar(%d, %d, %d), lane width: %f",config.ssd_r,config.ssd_g,config.ssd_b,config.lane_r,config.lane_g,config.lane_b,config.lane_line_width);
	displayConfig_.enable_show_ssd = config.enable_show_ssd;
	displayConfig_.ssd_r= config.ssd_r;
	displayConfig_.ssd_g = config.ssd_g;
	displayConfig_.ssd_b = config.ssd_b;
	displayConfig_.ssd_line_width = config.ssd_line_width;
	displayConfig_.enable_show_label = config.enable_show_label;	

	displayConfig_.enable_show_lane = config.enable_show_lane;
	displayConfig_.lane_r = config.lane_r;
	displayConfig_.lane_g= config.lane_g;
	displayConfig_.lane_b= config.lane_b;
	displayConfig_.lane_line_width= config.lane_line_width;


	if(displayConfig_.enable_show_ssd && displayConfig_.enable_show_lane)
	{
		if(!sub_image_raw_.getSubscriber())
			sub_image_raw_.subscribe();

		if(!sub_SSD_result_.getSubscriber())
			sub_SSD_result_.subscribe();

		if(!sub_lane_detect_result_.getSubscriber())
			sub_lane_detect_result_.subscribe();

    		sync_all_ = new message_filters::Synchronizer<DisplayAllSyncPolicy>(DisplayAllSyncPolicy(10),sub_image_raw_, sub_lane_detect_result_, sub_SSD_result_);
    		sync_all_->registerCallback(boost::bind(&Display::onLaneSSDDisplayCb,this,_1, _2,_3));
	}
	else if(displayConfig_.enable_show_ssd && !displayConfig_.enable_show_lane)
	{	
		if(sub_lane_detect_result_.getSubscriber())
		{
			sub_lane_detect_result_.unsubscribe();
		}

		if(!sub_SSD_result_.getSubscriber())
			sub_SSD_result_.subscribe();

		if(!sub_image_raw_.getSubscriber())
			sub_image_raw_.subscribe();

		sync_ssd_ = new message_filters::Synchronizer<DisplaySSDSyncPolicy>(DisplaySSDSyncPolicy(10),sub_image_raw_, sub_SSD_result_);
		sync_ssd_->registerCallback(boost::bind(&Display::onSSDDisplayCb,this,_1, _2));
	}
	else if(!displayConfig_.enable_show_ssd && displayConfig_.enable_show_lane)
	{		
		if(sub_SSD_result_.getSubscriber())
			sub_SSD_result_.unsubscribe();

		if(!sub_image_raw_.getSubscriber())
			sub_image_raw_.subscribe();

		if(!sub_lane_detect_result_.getSubscriber())
			sub_lane_detect_result_.subscribe();

		sync_lane_ = new message_filters::Synchronizer<DisplayLaneSyncPolicy>(DisplayLaneSyncPolicy(10),sub_image_raw_, sub_lane_detect_result_);
		sync_lane_->registerCallback(boost::bind(&Display::onLaneDisplayCb,this,_1, _2));
	}
	else
	{
		if(sub_SSD_result_.getSubscriber())
			sub_SSD_result_.unsubscribe();
	
		if(sub_lane_detect_result_.getSubscriber())
			sub_lane_detect_result_.unsubscribe();

		if(!sub_image_raw_.getSubscriber())
			sub_image_raw_.subscribe();

		sub_image_raw_.registerCallback(boost::bind(&Display::onImageCb,this,_1));
	}
}



int main(int argc, char** argv)
{
	ros::init(argc, argv, "ssd_lane_display");
	ros::NodeHandle nh, nh_private("~");

	Display display(nh, nh_private);

	ros::Rate rate(50);
	while(ros::ok())
	{
		rate.sleep();
		ros::spinOnce();
	}

	return 0;
}

