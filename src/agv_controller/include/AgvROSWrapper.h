#ifndef H_AGV_WRAPPER
#define H_AGV_WRAPPER

#include "ros/ros.h"
#include "../../../agv_driver/RPICanDriver/src/AGV_Driver.h"
#include "geometry_msgs/Twist.h"
#include "std_srvs/Trigger.h"



class AgvROSWrapper
{
	public:
		AgvROSWrapper(ros::NodeHandle *nh,int ad_fl,int ad_fr,int ad_br, int ad_bl);
		

		bool setVel(geometry_msgs::TwistPtr twistptr);
		geometry_msgs::TwistPtr getVel();
		void start();
		void stop();


	private:
		//Use ROS message for sending velocity commands
       	geometry_msgs::Twist twist;
       	//Driver instance, using smart pointer type
       	std::unique_ptr<AGV> agv;

       	//Publishers and subscribers
       	ros::Publisher current_speed_publisher;
       	ros::Subscriber speed_command_subscriber;
       	ros::ServiceServer stop_motor_server;
       	ros::ServiceServer start_motor_server;

       	ros::Timer current_speed_timer;

       	//Callbacks

       	bool callbackStop(std_srvs::Trigger::Request &req, std_srvs::Trigger::Response &res);
       	bool callbackStart(std_srvs::Trigger::Request &req, std_srvs::Trigger::Response &res);
       	//void callbackCurrentSpeedPublisher();
       	void publishCurrentSpeed(const ros::TimerEvent &event);
       	void callbackSpeedCommand(const geometry_msgs::Twist &msg);

       	double current_speed_hz;



	
};
































#endif