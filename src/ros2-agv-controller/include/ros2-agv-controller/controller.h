#ifndef H_AGV_CONTROLLER
#define H_AGV_CONTROLLER

#include "rclcpp/rclcpp.hpp"
#include <string>
#include "geometry_msgs/msg/twist.hpp"
#include "geometry_msgs/msg/pose.hpp"
#include "geometry_msgs/msg/point.hpp"
#include "sensor_msgs/msg/imu.hpp"
#include "std_msgs/msg/u_int8.hpp"
#include "std_msgs/msg/int16.hpp"

using namespace std::placeholders;


class PosController : public rclcpp::Node
{
public:
	PosController();

	void getParams();





private:

	rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr speed_command_publisher;
	rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr user_speed_command_subscriber;
	rclcpp::Subscription<geometry_msgs::msg::Pose>::SharedPtr user_position_command_subscriber;
	rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr current_speed_subscriber;
	rclcpp::Subscription<sensor_msgs::msg::Imu>::SharedPtr imu_readings_subscriber;
	rclcpp::Subscription<geometry_msgs::msg::Pose>::SharedPtr marvelmind_readings_subscriber;

	//Vision topics
	rclcpp::Subscription<std_msgs::msg::UInt8>::SharedPtr marker_id_suscriber;
	rclcpp::Subscription<std_msgs::msg::Int16>::SharedPtr marker_orientation_suscriber;
	rclcpp::Subscription<geometry_msgs::msg::Point>::SharedPtr marker_position_suscriber;

	rclcpp::TimerBase::SharedPtr pid_timer;

	bool callbackUserPositionCmd(const geometry_msgs::msg::Pose::SharedPtr msg);
	bool callbackUserSpeedCmd(const geometry_msgs::msg::Twist::SharedPtr msg);

	void callbackPublishSpeedCmd();

	void updatePid();
	void updateCurrentSpeed(const geometry_msgs::msg::Twist::SharedPtr msg);
	void getImuReadings(const sensor_msgs::msg::Imu::SharedPtr msg);
	void getMarvelmindReadings(const geometry_msgs::msg::Pose::SharedPtr msg);

	void callbackMarkerId (const std_msgs::msg::UInt8::SharedPtr msg);
	void callbackMarkerOrientation (const std_msgs::msg::Int16::SharedPtr msg);
	void callbackMarkerPosition (const geometry_msgs::msg::Point::SharedPtr msg);


	rclcpp::Parameter pid_hz;
	rclcpp::Parameter Kpx_param;

	double pos_cmd[4],speed_encoder[4];





	//PID variablees
	double dt;


	double px = 0,py = 0;
	double pxw = 0,pyw = 0,theta = 0;
	double dtheta = 0;

	double errorx = 0,errory = 0, errordtheta = 0;
	double prev_errorx = 0,prev_errory = 0, prev_errordtheta;

	double itermx = 0,dtermx = 0;
	double itermy = 0,dtermy = 0;
	double itermdtheta = 0, dtermdtheta = 0;

	//Parameters

	double Kpx,Kdx,Kix;
	double Kpy,Kdy,Kiy;
	double Kpdtheta,Kddtheta,Kidtheta;
	double anti_windup;
	double pid_update_rate;
};


















#endif
