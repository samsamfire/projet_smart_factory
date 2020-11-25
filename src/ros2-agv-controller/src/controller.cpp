#include "../include/ros2-agv-controller/controller.h"
#include <unistd.h>
#include <algorithm>
#include <chrono>


PosController::PosController() : Node("agv_controller")
{
	std::vector<std::string> node_names;

	getParams();

	user_command_subscriber = this->create_subscription<geometry_msgs::msg::Pose>("user_speed_cmd",10, std::bind(&PosController::callbackUserPositionCmd,this,_1));
	current_speed_subscriber = this->create_subscription<geometry_msgs::msg::Twist>("current_speed",10, std::bind(&PosController::updateCurrentSpeed,this,_1));
	speed_command_publisher = this->create_publisher<geometry_msgs::msg::Twist>("vel_cmd",10);


	std::chrono::milliseconds pid_update_ms(static_cast<int>(1000.0 / (pid_update_rate)));

	pid_timer = this->create_wall_timer(std::chrono::duration_cast<std::chrono::milliseconds>(pid_update_ms),std::bind(&PosController::updatePid,this));


	

	dt = 1.0/pid_update_rate;

	RCLCPP_INFO(this->get_logger(),"Checking if AGV wrapper node is running ...");


	usleep(500000);




	if(this->count_subscribers("vel_cmd") == 1){

		RCLCPP_INFO(this->get_logger(),"AGV is online");
	}
	else{

		RCLCPP_INFO(this->get_logger(),"AGV wrapper must be running before running the controller node");
	}


	for (int i = 0; i < 4; ++i)
	{
		pos_cmd[i] = 0;
		speed_encoder[i] = 0;
	}


}





bool PosController::callbackUserPositionCmd(const geometry_msgs::msg::Pose::SharedPtr msg){

	pos_cmd[0] = msg->position.x;
	pos_cmd[1] = msg->position.y;


	//TODO compute theta from quaternion
	RCLCPP_INFO(this->get_logger(),"New position command");
	pos_cmd[2] = 0;
	pos_cmd[3] = 0;



}

void PosController::updateCurrentSpeed(const geometry_msgs::msg::Twist::SharedPtr msg){

	auto& clk = *this->get_clock();

	

	speed_encoder[0] = msg->linear.x;
	speed_encoder[1] = msg->linear.y;
	speed_encoder[2] = 0;
	speed_encoder[3] = 0;

	RCLCPP_INFO_THROTTLE(this->get_logger(), clk,5000, "Received new current speed : Vx : %lf, Vy: %lf",speed_encoder[0],speed_encoder[1]);
}


double limit(double to_limit, double limit_max, double limit_min){


	if (to_limit>limit_max)
	{
		to_limit = limit_max;
	}

	else if (to_limit<limit_min)
	{
		to_limit = limit_min;
	}

	return to_limit;
}

void PosController::updatePid(){

	geometry_msgs::msg::Twist msg;

	double pos_ctrl[4];
		
	double vxr = speed_encoder[0];
	double vyr = speed_encoder[1];	



	//Positions in world frame :
	pxw += vxr*dt;
	pyw += vyr*dt;
	//theta +=vthetar*dt;

	errorx = (double) pos_cmd[0] - pxw;
	errory = (double) pos_cmd[1] - pyw;


	dtermx = (double) Kdx*(errorx-prev_errorx)/dt;
	dtermy = (double) Kdy*(errory-prev_errory)/dt;


	itermx += (double) Kix*errorx*dt;
	itermy += (double) Kiy*errory*dt;
	

	itermx = limit(itermx,0.25,-0.25);
	itermy = limit(itermy,0.25,-0.25);
	

	pos_ctrl[0] =(double) Kpx*(pos_cmd[0]-pxw) + itermx + dtermx;
	pos_ctrl[1] =(double) Kpy*(pos_cmd[1]-pyw) + itermy + dtermy;

	pos_ctrl[0] = limit(pos_ctrl[0],0.3,-0.3);
	pos_ctrl[1] = limit(pos_ctrl[1],0.3,-0.3);

	pos_ctrl[2] = 0;
	pos_ctrl[3] = 0;


	

	if(abs(errorx)< 0.015){
		pos_ctrl[0] = 0;
		itermx = 0;
	}

	if(abs(errory)< 0.015){
		pos_ctrl[1] = 0;
		itermy = 0;
	}

	msg.linear.x = pos_ctrl[0];
	msg.linear.y = pos_ctrl[1];
	msg.linear.z = 0;
	msg.angular.z = 0;

	speed_command_publisher->publish(msg);
	


	prev_errorx = errorx;
	prev_errory = errory;
}






void PosController::getParams(){

	//Parameter declarations

	//General parameters
	this->declare_parameter("pid_update_rate",100.0);
	this->declare_parameter("anti_windup", 0.3);

	//Pid x parameters
	this->declare_parameter("Kpx",2.0);
	this->declare_parameter("Kdx",2.0);
	this->declare_parameter("Kix",2.0);
	
	//Pid y parameters

	this->declare_parameter("Kpy",2.0);
	this->declare_parameter("Kdy",2.0);
	this->declare_parameter("Kiy",2.0);

	//Pid dtheta parameters

	this->declare_parameter("Kpdtheta",2.0);
	

	//Getting parameters

	anti_windup = this->get_parameter("anti_windup").as_double();
	pid_update_rate = this->get_parameter("pid_update_rate").as_double();

	Kpx = this->get_parameter("Kpx").as_double();
	Kdx = this->get_parameter("Kdx").as_double();
	Kix = this->get_parameter("Kix").as_double();

	Kpy = this->get_parameter("Kpy").as_double();
	Kdy = this->get_parameter("Kdy").as_double();
	Kiy = this->get_parameter("Kiy").as_double();

	



	Kpdtheta = this->get_parameter("Kpdtheta").as_double();
	


}





int main(int argc, char const *argv[])
{
	rclcpp::init(argc, argv);

  	rclcpp::spin(std::make_shared<PosController>());


  	rclcpp::shutdown();


	return 0;
}