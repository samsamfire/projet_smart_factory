#ifndef H_MPU6050ROSWRAPPER
#define H_MPU6050ROSWRAPPER



#include "MPU6050.h"
#include "rclcpp/rclcpp.hpp"
#include "sensor_msgs/msg/imu.hpp"




class MPU6050Wrapper
{
public:
	MPU6050Wrapper(std::shared_ptr<rclcpp::Node> nh);


private:

	std::unique_ptr<MPU6050> imu;

	std::shared_ptr<rclcpp::Node> nh;

	rclcpp::Publisher<sensor_msgs::msg::Imu>::SharedPtr imu_publisher;
	rclcpp::TimerBase::SharedPtr timer_imu_publish;

	double ax,ay,az;
	double gx,gy,gz;

	void publishImuReadings();
	
};






















#endif