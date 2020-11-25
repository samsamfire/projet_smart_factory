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
              uint8_t emergency_stop();


	private:
    //Use ROS message for sending velocity commands
    geometry_msgs::msg::Twist *twist; 
    //Driver instance, using smart pointer type
    std::unique_ptr<AGV> agv;

    // //Publishers and subscribers
    // rclcpp::Publisher current_speed_publisher;

    //Services
    rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr stop_motor_server;
    rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr start_motor_server;
    rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr open_bus_server;
    rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr close_bus_server;

    //Publishers and subscribers
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr current_speed_publisher;
    rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr speed_command_subscriber;

    // rclcpp::Subscriber speed_command_subscriber;


    rclcpp::TimerBase::SharedPtr current_speed_timer;


    // //Callbacks for services

    bool callbackStop(const std::shared_ptr<rmw_request_id_t> request_header,const std::shared_ptr<std_srvs::srv::Trigger_Request> req,
                             std::shared_ptr<std_srvs::srv::Trigger_Response> res);
    bool callbackStart(const std::shared_ptr<rmw_request_id_t> request_header,
                             const std::shared_ptr<std_srvs::srv::Trigger_Request> req,
                             std::shared_ptr<std_srvs::srv::Trigger_Response> res);
     bool callbackOpenBus(const std::shared_ptr<rmw_request_id_t> request_header,
                             const std::shared_ptr<std_srvs::srv::Trigger_Request> req,
                             std::shared_ptr<std_srvs::srv::Trigger_Response> res);
     bool callbackCloseBus(const std::shared_ptr<rmw_request_id_t> request_header,
                             const std::shared_ptr<std_srvs::srv::Trigger_Request> req,
                             std::shared_ptr<std_srvs::srv::Trigger_Response> res);


    void callbackSpeedCommand(const geometry_msgs::msg::Twist::SharedPtr msg);


    void publishCurrentSpeed();


    double current_speed_hz;

    double speed_cmd[4];
    double speed_encoder[4];
    double kc_coupling;



	
};

#endif