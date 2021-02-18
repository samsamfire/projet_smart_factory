#ifndef H_MARVELMINDROSWRAPPER
#define H_MARVELMINDROSWRAPPER


extern "C" {
	#include "marvelmind.h"
}

#include "rclcpp/rclcpp.hpp"
#include "geometry_msgs/msg/pose.hpp"
#include "std_srvs/srv/trigger.hpp"


using namespace std::placeholders;

class marvelmindWrapper
{
public:
	marvelmindWrapper(std::shared_ptr<rclcpp::Node> nh);

	// void callbackConnect(const std::shared_ptr<rmw_request_id_t> request_header,
  //                            const std::shared_ptr<std_srvs::srv::Trigger_Request> req,
  //                            std::shared_ptr<std_srvs::srv::Trigger_Response> res);
	//
	// void callbackDisconnect(const std::shared_ptr<rmw_request_id_t> request_header,
	//                           const std::shared_ptr<std_srvs::srv::Trigger_Request> req,
	//                           std::shared_ptr<std_srvs::srv::Trigger_Response> res);

	bool connect();

	bool disconnect();


private:
	struct MarvelmindHedge* hedge;

	std::shared_ptr<rclcpp::Node> nh;

	rclcpp::Publisher<geometry_msgs::msg::Pose>::SharedPtr pos_publisher;
	// rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr connect_service;
	// rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr disconnect_service;

	rclcpp::TimerBase::SharedPtr timer_marvelmind_publish;

	void publishMarvelmindReadings();
};






















#endif
