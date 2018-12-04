//ROS
#include <ros/ros.h>
#include <tf/transform_datatypes.h>
#include <rtk_inertial/Gps.h>
#include <geometry_msgs/PoseStamped.h>
#include <ros/time.h>


//LCM
#include <lcm/lcm-cpp.hpp>
#include "ins_info.hpp"



//Boost
 #include <boost/thread/thread.hpp>  
#include <boost/foreach.hpp>


#include "Commons/transfer.hpp"


#define ORIGIN_LAT  30.45814058804	//wuhan
#define ORIGIN_LON  114.31801222674


class LcmHandle
{
public:
	LcmHandle(const ros::NodeHandle& nh);



protected:

	void onGpsCallback(const rtk_inertial::Gps::ConstPtr& msg);

	void handleMessage(const lcm::ReceiveBuffer* rbuf,
                const std::string& chan, 
                const obu_lcm::ins_info* msg);



	void start()
	{
		m_gps_lcm.subscribe("ins_info", &LcmHandle::handleMessage, this);
		while(0 == m_gps_lcm.handle());
	}



private:
	ros::NodeHandle m_nh;
	ros::Publisher m_pubGps;
	ros::Publisher m_pubPoseStamped;

	ros::Subscriber m_subGps;

	lcm::LCM m_gps_lcm;
	rtk_inertial::Gps m_gps;

	boost::thread m_queueThread;
};


void LcmHandle::handleMessage(const lcm::ReceiveBuffer* rbuf,
                const std::string& chan, 
                const obu_lcm::ins_info* msg)
        {
		//m_gps.header.stamp = ros::Time::now();
		//m_gps.header.frame_id = "gps";
		m_gps.gps_time = msg->gps_time;
		m_gps.week= msg->week;
		m_gps.lat= msg->lat;
		m_gps.lon= msg->lon;
		m_gps.height= msg->height;
		m_gps.lateral_speed= msg->lateral_speed;
		m_gps.longitudinal_speed= msg->longitudinal_speed;
		m_gps.down_speed= msg->down_speed;
		m_gps.roll= msg->roll;
		m_gps.pitch= msg->pitch;
		m_gps.heading= msg->heading;
		m_gps.lateral_accelerate= msg->lateral_accelerate;
		m_gps.longitudinal_accelerate= msg->longitudinal_accelerate;
		m_gps.down_accelerate= msg->down_accelerate;
		m_gps.roll_speed= msg->roll_speed;
		m_gps.pitch_speed= msg->pitch_speed;
		m_gps.heading_speed= msg->heading_speed;
		m_gps.flag= msg->flag;
		m_gps.n= msg->n;

		m_pubGps.publish(m_gps);

        }


void LcmHandle::onGpsCallback(const rtk_inertial::Gps::ConstPtr& msg)
{
	geometry_msgs::PoseStamped poseStamped;
	poseStamped.header.stamp = ros::Time(msg->gps_time);
	poseStamped.header.frame_id = "gps";

	double x=0.0, y=0.0;
	coord_transfer transfer(ORIGIN_LAT,ORIGIN_LON);
	transfer.cs.ll2xy(msg->lat, msg->lon, x, y);
	
	poseStamped.pose.position.x = x;
	poseStamped.pose.position.y = y;
	poseStamped.pose.position.z = msg->height;

	geometry_msgs::Quaternion odom_quat = tf::createQuaternionMsgFromRollPitchYaw(msg->roll, msg->pitch, msg->heading);


	poseStamped.pose.orientation = odom_quat;

	 
	m_pubPoseStamped.publish(poseStamped);
}




LcmHandle::LcmHandle(const ros::NodeHandle& nh)
	:m_nh(nh)
{
	m_pubGps = m_nh.advertise<rtk_inertial::Gps>("gps",1);
	m_pubPoseStamped = m_nh.advertise<geometry_msgs::PoseStamped>("poseStamped",1);


	m_subGps = m_nh.subscribe<rtk_inertial::Gps>("gps",1,&LcmHandle::onGpsCallback,this);

	if(!m_gps_lcm.good())
        	return;

    	
	m_queueThread = boost::thread(boost::bind(&LcmHandle::start,this));
	
}


int main(int argc, char** argv)
{
	ros::init(argc, argv,"rtk_inertial_node");
	ros::NodeHandle nh;

	LcmHandle lcmHandle(nh);

	/*ros::Rate rate(10);
	while(ros::ok())
	{
		rate.sleep(
	}*/
	ros::spin();
	return 0;
}
