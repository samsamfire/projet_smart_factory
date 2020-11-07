#ifndef H_AGV_WRAPPER
#define H_AGV_WRAPPER

#include "ros/ros.h"
#include "agv_driver/AGV_Driver.h"
#include "geometry_msgs/Twist.h"
#include "std_srvs/Trigger.h"
#include <string>



class AgvROSWrapper
{
	public:
		AgvROSWrapper(ros::NodeHandle *nh,int ad_fl,int ad_fr,int ad_br, int ad_bl);
		

		bool setVel(geometry_msgs::TwistPtr twistptr);
		geometry_msgs::TwistPtr getVel();
		uint8_t start();
		uint8_t stop();


	private:
		//Use ROS message for sending velocity commands
       	geometry_msgs::Twist twist;
       	//Driver instance, using smart pointer type
       	std::unique_ptr<AGV> agv;

       	//Publishers and subscribers
       	ros::Publisher current_speed_publisher;
             

       	ros::Subscriber speed_command_subscriber;
              //Services
       	ros::ServiceServer stop_motor_server;
       	ros::ServiceServer start_motor_server;
              ros::ServiceServer open_bus_server;
              ros::ServiceServer close_bus_server;

       	ros::Timer current_speed_timer;
              ros::Timer update_pid_timer;

       	//Callbacks for services
       	bool callbackStop(std_srvs::Trigger::Request &req, std_srvs::Trigger::Response &res);
       	bool callbackStart(std_srvs::Trigger::Request &req, std_srvs::Trigger::Response &res);
              bool callbackOpenBus(std_srvs::Trigger::Request &req, std_srvs::Trigger::Response &res);
              bool callbackCloseBus(std_srvs::Trigger::Request &req, std_srvs::Trigger::Response &res);
              void callbackSpeedCommand(const geometry_msgs::Twist &msg);

              void callbackCouplingControl(const ros::TimerEvent &event);
       	
       	void publishCurrentSpeed(const ros::TimerEvent &event);
            

       	double current_speed_hz;

              double speed_cmd[4];
              double speed_encoder[4];
              double kc_coupling;



	
};
































#endif