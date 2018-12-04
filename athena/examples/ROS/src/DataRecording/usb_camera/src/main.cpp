#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/opencv.hpp>  

#include <boost/thread.hpp> 
#include <boost/bind.hpp>   

static const std::string OPENCV_WINDOW = "Image window";

class ImageConverter
{
  	ros::NodeHandle nh_;
	ros::NodeHandle nh_private_;
  	image_transport::ImageTransport it_;
 	image_transport::Subscriber image_sub_;
  	image_transport::Publisher image_pub_;

	int camera_device_;
	cv::Mat inImage_;  
	cv::VideoCapture videoCap_;
	cv::VideoWriter writer_;
	//std::string videoName_;


	int image_width_,image_height_,video_device_;
	std::string frame_id_;

public:

	typedef boost::function<void()> CaptureImageCb;

  	ImageConverter(const ros::NodeHandle& nh, const ros::NodeHandle& nh_private)
    		:it_(nh_)
		,nh_(nh)
		,nh_private_(nh_private)
		,camera_device_(0)
		//,videoName_(std::string(""))
  	{
		nh_private_.getParam("image_width", image_width_);
		nh_private_.getParam("image_height", image_height_);
		nh_private_.getParam("video_device", video_device_);
		nh_private_.getParam("frame_id_", frame_id_);
			
		ROS_INFO("width: %d, height: %d, device_id: %d, frame_id: %s",image_width_,image_height_,video_device_,frame_id_.c_str());

		//Open Camera
		videoCap_ = cv::VideoCapture(video_device_); /*open the default camera device*/  
		if(!videoCap_.isOpened())                       /*check open result*/  
		{
			ROS_ERROR("Open camera failed!");
		  	return;
		}  
		videoCap_.set(CV_CAP_PROP_FRAME_WIDTH, image_width_);  
    		videoCap_.set(CV_CAP_PROP_FRAME_HEIGHT, image_height_);  
		boost::thread captureImageThread(boost::bind(&ImageConverter::startCameraLoop,this));	

	    	// Subscrive to input video feed and publish output video feed
	    	image_sub_ = it_.subscribe("/camera/image_raw", 1,&ImageConverter::imageCb, this);
	    	image_pub_ = it_.advertise("/usb_cam/image_raw", 1);

		//cv::namedWindow(OPENCV_WINDOW);

		int imageWidth = static_cast<int>(videoCap_.get(CV_CAP_PROP_FRAME_WIDTH)); 
		int imageHeight = static_cast<int>(videoCap_.get(CV_CAP_PROP_FRAME_HEIGHT)); 

		ROS_INFO("image width: %d, height: %d",imageWidth,imageHeight);
		//writer_.open("/home/nvidia/cameraVideo.avi",CV_FOURCC('M','J','P','G'),videoCap_.get(CV_CAP_PROP_FPS),cv::Size(imageWidth,imageHeight),true);
  	}

  	~ImageConverter()
  	{
    		//cv::destroyWindow(OPENCV_WINDOW);
		videoCap_.release();
		//writer_.release();
  	}

  	void imageCb(const sensor_msgs::ImageConstPtr& msg)
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

    		// Draw an example circle on the video stream
    		if (cv_ptr->image.rows > 60 && cv_ptr->image.cols > 60)
      			cv::circle(cv_ptr->image, cv::Point(50, 50), 10, CV_RGB(255,0,0));

    		// Update GUI Window
    		cv::imshow(OPENCV_WINDOW, cv_ptr->image);
    		cv::waitKey(3);

    		// Output modified video stream
    		image_pub_.publish(cv_ptr->toImageMsg());
  	}

	void startCameraLoop()
	{
		cv_bridge::CvImagePtr cv_ptr;
		while(ros::ok())
		{
			videoCap_>>inImage_;
			if(NULL == inImage_.data)
				continue;

			//save frame to video
			writer_.write(inImage_);

			//tranform cv::Mat to sensor_msgs/Image through cv_bridge
			std_msgs::Header header;
			header.stamp = ros::Time::now();
			header.frame_id = frame_id_;
			std::string encoding("bgr8");
			cv_ptr = cv_bridge::CvImagePtr(new cv_bridge::CvImage(header,encoding,inImage_));

			image_pub_.publish(cv_ptr->toImageMsg());
		}
	}
};

int main(int argc, char** argv)
{
  	ros::init(argc, argv, "image_converter");
	ros::NodeHandle nh;
	ros::NodeHandle nh_private("~");

  	ImageConverter ic(nh,nh_private);
  	ros::AsyncSpinner spinner(2);
	spinner.start();
  	ros::waitForShutdown();
  	return 0;
}
