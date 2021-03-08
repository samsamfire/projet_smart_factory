#include "../include/ros2-agv-controller/controller.h"
#include <unistd.h>
#include <algorithm>
#include <chrono>


PosController::PosController() : Node("agv_controller")
{
	std::vector<std::string> node_names;

	getParams();

	user_position_command_subscriber = this->create_subscription<geometry_msgs::msg::Pose>("user_pos_cmd",10, std::bind(&PosController::callbackUserPositionCmd,this,_1));
	user_speed_command_subscriber = this->create_subscription<geometry_msgs::msg::Twist>("user_vel_cmd",10, std::bind(&PosController::callbackUserSpeedCmd,this,_1));

	current_speed_subscriber = this->create_subscription<geometry_msgs::msg::Twist>("current_speed",10, std::bind(&PosController::updateCurrentSpeed,this,_1));
	speed_command_publisher = this->create_publisher<geometry_msgs::msg::Twist>("agv_vel_cmd",10);
	imu_readings_subscriber = this->create_subscription<sensor_msgs::msg::Imu>("imu_readings",10, std::bind(&PosController::getImuReadings,this,_1));
	marvelmind_readings_subscriber = this->create_subscription<geometry_msgs::msg::Pose>("marvelmind_readings",10, std::bind(&PosController::getMarvelmindReadings,this,_1));

	//Vision topics
	marker_id_suscriber = this->create_subscription<std_msgs::msg::UInt8>("marker_id",10, std::bind(&PosController::callbackMarkerId,this,_1));
	marker_orientation_suscriber = this->create_subscription<std_msgs::msg::Int16>("marker_orientation",10, std::bind(&PosController::callbackMarkerOrientation,this,_1));
	marker_position_suscriber = this->create_subscription<geometry_msgs::msg::Point>("marker_position",10, std::bind(&PosController::callbackMarkerPosition,this,_1));


	std::chrono::milliseconds pid_update_ms(static_cast<int>(1000.0 / (pid_update_rate)));

	pid_timer = this->create_wall_timer(std::chrono::duration_cast<std::chrono::milliseconds>(pid_update_ms),std::bind(&PosController::updatePid,this));




	dt = 1.0/pid_update_rate;

	RCLCPP_INFO(this->get_logger(),"Checking if AGV wrapper node is running ...");


	usleep(500000);




	if(this->count_subscribers("cmd_vel") == 1){

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

	RCLCPP_INFO(this->get_logger(),"Checking if Sensor node is running ...");

	if(this->count_subscribers("imu_readings") == 1){

		RCLCPP_INFO(this->get_logger(),"IMU is online");
	}
	else{

		RCLCPP_INFO(this->get_logger(),"Sensor node must be running for the controller to work properly");
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

bool PosController::callbackUserSpeedCmd(const geometry_msgs::msg::Twist::SharedPtr msg){

	//TODO

}

void PosController::callbackMarkerId (const std_msgs::msg::UInt8::SharedPtr msg){}
void PosController::callbackMarkerOrientation (const std_msgs::msg::Int16::SharedPtr msg){}
void PosController::callbackMarkerPosition (const geometry_msgs::msg::Point::SharedPtr msg){}

void PosController::updateCurrentSpeed(const geometry_msgs::msg::Twist::SharedPtr msg){

	auto& clk = *this->get_clock();



	speed_encoder[0] = msg->linear.x;
	speed_encoder[1] = msg->linear.y;
	speed_encoder[2] = 0;
	speed_encoder[3] = 0;

	RCLCPP_INFO_THROTTLE(this->get_logger(), clk,5000, "Received new current speed : Vx : %lf, Vy: %lf",speed_encoder[0],speed_encoder[1]);
}

void PosController::getImuReadings(const sensor_msgs::msg::Imu::SharedPtr msg){


	//Get imu readings
	//For now we only use dtheta;

	this->dtheta = -(msg->angular_velocity.z*3.14159)/180;


}

void PosController::getMarvelmindReadings(const geometry_msgs::msg::Pose::SharedPtr msg){

	//TODO

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
	//For now no dtheta command
	errordtheta = (double) 0-dtheta;


	dtermx = (double) Kdx*(errorx-prev_errorx)/dt;
	dtermy = (double) Kdy*(errory-prev_errory)/dt;
	dtermdtheta = (double) Kddtheta*(errordtheta- prev_errordtheta)/dt;



	itermx += (double) Kix*errorx*dt;
	itermy += (double) Kiy*errory*dt;
	itermdtheta += (double) Kidtheta*errordtheta*dtheta;


	itermx = limit(itermx,0.25,-0.25);
	itermy = limit(itermy,0.25,-0.25);
	itermdtheta = limit(itermdtheta,0.25,-0.25);

	pos_ctrl[0] =(double) Kpx*errorx + itermx + dtermx;
	pos_ctrl[1] =(double) Kpy*errory + itermy + dtermy;
	pos_ctrl[2] =(double) Kpdtheta*errordtheta + itermdtheta + dtermdtheta;

	pos_ctrl[0] = limit(pos_ctrl[0],0.3,-0.3);
	pos_ctrl[1] = limit(pos_ctrl[1],0.3,-0.3);
	pos_ctrl[2] = limit(pos_ctrl[2],0.6,-0.6);
	pos_ctrl[3] = 0;




	if(abs(errorx)< 0.015){
		pos_ctrl[0] = 0;
		itermx = 0;
	}

	if(abs(errory)< 0.015){
		pos_ctrl[1] = 0;
		itermy = 0;
	}

	if(abs(errordtheta)<0.015){
		pos_ctrl[2] = 0;
		itermdtheta = 0;
	}

	msg.linear.x = pos_ctrl[0];
	msg.linear.y = pos_ctrl[1];

	msg.angular.z = pos_ctrl[2];

	speed_command_publisher->publish(msg);



	prev_errorx = errorx;
	prev_errory = errory;
	prev_errordtheta = errordtheta;
}






void PosController::getParams(){

	//Parameter declarations

	//General parameters
	this->declare_parameter("pid_update_rate",100.0);
	this->declare_parameter("anti_windup", 0.3);

	//Pid x parameters
	this->declare_parameter("Kpx",2.0);
	this->declare_parameter("Kdx",0.1);
	this->declare_parameter("Kix",0.01);

	//Pid y parameters

	this->declare_parameter("Kpy",2.0);
	this->declare_parameter("Kdy",0.1);
	this->declare_parameter("Kiy",0.01);

	//Pid dtheta parameters

	this->declare_parameter("Kpdtheta",2.0);
	this->declare_parameter("Kddtheta",0.001);
	this->declare_parameter("Kidtheta",0.01);


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
	Kddtheta = this->get_parameter("Kddtheta").as_double();
	Kidtheta = this->get_parameter("Kidtheta").as_double();



}





int main(int argc, char const *argv[])
{
	rclcpp::init(argc, argv);

  	rclcpp::spin(std::make_shared<PosController>());


  	rclcpp::shutdown();


	return 0;
}
