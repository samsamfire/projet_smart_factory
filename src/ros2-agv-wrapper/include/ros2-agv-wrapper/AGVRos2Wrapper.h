#ifndef H_AGV_WRAPPER
#define H_AGV_WRAPPER

#include "rclcpp/rclcpp.hpp"
#include "../../driver/include/AGV_Driver.h"
#include "geometry_msgs/msg/twist.hpp"
#include "std_srvs/srv/trigger.hpp"
#include <string>



class AgvROSWrapper
{
	public:
		AgvROSWrapper(std::shared_ptr<rclcpp::Node> nh,int ad_fl,int ad_fr,int ad_br, int ad_bl);
		

		bool setVel(geometry_msgs::msg::Twist *twistptr);
		geometry_msgs::msg::Twist * getVel();
		uint8_t start();
		uint8_t stop();


	private:
		//Use ROS message for sending velocity commands
       	geometry_msgs::msg::Twist *twist; 
       	//Driver instance, using smart pointer type
       	std::unique_ptr<AGV> agv;

       	// //Publishers and subscribers
       	// rclcpp::Publisher current_speed_publisher;

              rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr stop_motor_server;
              rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr start_motor_server;

       	// rclcpp::Subscriber speed_command_subscriber;
        //       //Services
       	//rclcpp::ServiceServer stop_motor_server;

       	// rclcpp::ServiceServer start_motor_server;
        //       rclcpp::ServiceServer open_bus_server;
        //       rclcpp::ServiceServer close_bus_server;

       	// rclcpp::Timer current_speed_timer;
        //       rclcpp::Timer update_pid_timer;

       	// //Callbacks for services
       	//bool callbackStop(std_srvs::, std_srvs::Trigger::Response &res);
              bool callbackStop(const std::shared_ptr<rmw_request_id_t> request_header,const std::shared_ptr<std_srvs::srv::Trigger_Request> req,
                                       std::shared_ptr<std_srvs::srv::Trigger_Response> res);
       	bool callbackStart(const std::shared_ptr<rmw_request_id_t> request_header,
                                       const std::shared_ptr<std_srvs::srv::Trigger_Request> req,
                                       std::shared_ptr<std_srvs::srv::Trigger_Response> res);
        //       bool callbackOpenBus(std_srvs::Trigger::Request &req, std_srvs::Trigger::Response &res);
        //       bool callbackCloseBus(std_srvs::Trigger::Request &req, std_srvs::Trigger::Response &res);
        //       void callbackSpeedCommand(const geometry_msgs::Twist &msg);

        //       void callbackCouplingControl(const rclcpp::TimerEvent &event);
       	
       	// void publishCurrentSpeed(const rclcpp::TimerEvent &event);
            

       	// double current_speed_hz;

        //       double speed_cmd[4];
        //       double speed_encoder[4];
        //       double kc_coupling;



	
};

#endif