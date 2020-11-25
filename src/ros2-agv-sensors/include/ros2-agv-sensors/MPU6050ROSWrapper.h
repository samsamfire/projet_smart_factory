#ifndef H_MPU6050ROSWRAPPER
#define H_MPU6050ROSWRAPPER



#include "MPU6050.h"
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/imu.hpp"
#include "std_srvs/srv/trigger.hpp"


using namespace std::placeholders;

class MPU6050Wrapper
{
public:
	MPU6050Wrapper(std::shared_ptr<rclcpp::Node> nh);

	void callbackCalibrate(const std::shared_ptr<rmw_request_id_t> request_header,
                             const std::shared_ptr<std_srvs::srv::Trigger_Request> req,
                             std::shared_ptr<std_srvs::srv::Trigger_Response> res);



private:


	std::unique_ptr<MPU6050> imu;

	std::shared_ptr<rclcpp::Node> nh;

	rclcpp::Publisher<sensor_msgs::msg::Imu>::SharedPtr imu_publisher;
	rclcpp::Service<std_srvs::srv::Trigger>::SharedPtr calibrate_service;

	rclcpp::TimerBase::SharedPtr timer_imu_publish;



	float ax_off, ay_off, az_off;
	float gx_off, gy_off, gz_off;

	void publishImuReadings();


	
};






















#endif