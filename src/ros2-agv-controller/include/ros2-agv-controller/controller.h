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

	bool callbackUserPositionCmd(const geometry_msgs::msg::Pose::SharedPtr msg);

	void callbackPublishSpeedCmd();

	void updatePid();
	void updateCurrentSpeed(const geometry_msgs::msg::Twist::SharedPtr msg);

	

	rclcpp::Parameter pid_hz;
	rclcpp::Parameter Kpx_param;

	double pos_cmd[4],speed_encoder[4];

	



	//PID variablees
	double dt;
	

	double px = 0,py = 0;
	double pxw = 0,pyw = 0;

	double errorx = 0,errory = 0;
	double prev_errorx = 0,prev_errory = 0;

	double itermx = 0,dtermx = 0;
	double itermy = 0,dtermy = 0;

	//Parameters

	double Kpx,Kdx,Kix;
	double Kpy,Kdy,Kiy;
	double Kpdtheta,Kddtheta,Kidtheta;
	double anti_windup;
	double pid_update_rate;
};


















#endif