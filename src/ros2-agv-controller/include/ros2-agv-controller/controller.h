#ifndef H_AGV_CONTROLLER
#define H_AGV_CONTROLLER

#include "rclcpp/rclcpp.hpp"
#include <string>
#include "geometry_msgs/msg/twist.hpp"
#include "geometry_msgs/msg/pose.hpp"

using namespace std::placeholders;


class PosController : public rclcpp::Node
{
public:
	PosController();

	void getParams();

	



private:

	rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr speed_command_publisher;
	rclcpp::Subscription<geometry_msgs::msg::Pose>::SharedPtr user_command_subscriber;
	rclcpp::Subscription<geometry_msgs::msg::Twist>::SharedPtr current_speed_subscriber;

	rclcpp::TimerBase::SharedPtr pid_timer;

	bool callbackUserSpeedCmd(const geometry_msgs::msg::Pose::SharedPtr msg);

	void callbackPublishSpeedCmd();

	void updatePid();
	void updateCurrentSpeed(const geometry_msgs::msg::Twist::SharedPtr msg);

	

	rclcpp::Parameter pid_hz;
	rclcpp::Parameter Kpx_param;

	double pos_cmd[4];
	double speed_encoder[4];
	double pxw,pyw;

	double px,py;
	double errorx,errory;
	double dt;


	//PID varialbes
	double itermx,dtermx;
	double Kpx,Kdx,Kix;
	double Kpy,Kdy,Kiy;
	double Kpdtheta,Kddtheta,Kidtheta;

	double pid_update_rate;
};


















#endif