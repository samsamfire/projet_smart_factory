#include "../include/ros2-agv-wrapper/AGVRos2Wrapper.h"

/*This file is a ros wrapper node for controlling the agv*/
using namespace std::placeholders;

//Constructor
AgvROSWrapper::AgvROSWrapper(std::shared_ptr<rclcpp::Node> nh,int ad_fl,int ad_fr,int ad_br, int ad_bl){


//Add parameter fetching from parameter server

	// if (!rclcpp::param::get("~speed_publisher_frequency", current_speed_hz))
	// {
	// 	current_speed_hz = 100;

	// }

//Instantiate smart pointer
	agv.reset(new AGV(ad_fl,ad_fr,ad_br,ad_bl));

	current_speed_hz = 100;

// 	/*Services*/
 	stop_motor_server = nh->create_service<std_srvs::srv::Trigger>("stop_driver", std::bind(&AgvROSWrapper::callbackStop,
                                                                     this, _1, _2, _3));
 	start_motor_server = nh->create_service<std_srvs::srv::Trigger>("start_driver", std::bind(&AgvROSWrapper::callbackStart,
                                                                     this, _1, _2, _3));
 	//open_bus_server = nh->advertiseService("open_bus",&AgvROSWrapper::callbackOpenBus,this);
 	//close_bus_server = nh->advertiseService("close_bus",&AgvROSWrapper::callbackCloseBus,this);
 	open_bus_server = nh->create_service<std_srvs::srv::Trigger>("open_bus",std::bind(&AgvROSWrapper::callbackOpenBus,
                                                                     this, _1, _2, _3));
 	close_bus_server = nh->create_service<std_srvs::srv::Trigger>("close_bus",std::bind(&AgvROSWrapper::callbackCloseBus,
                                                                     this, _1, _2, _3));


 	current_speed_publisher = nh->create_publisher<geometry_msgs::msg::Twist>("current_speed",10);
 	std::chrono::milliseconds speed_update_ms(static_cast<int>(1000.0 / (current_speed_hz)));
 	current_speed_timer = nh->create_wall_timer(std::chrono::duration_cast<std::chrono::milliseconds>(speed_update_ms),std::bind(&AgvROSWrapper::publishCurrentSpeed,this));

 	speed_command_subscriber = nh->create_subscription<geometry_msgs::msg::Twist>("vel_cmd", 10, std::bind(&AgvROSWrapper::callbackSpeedCommand,this,_1));


}



uint8_t AgvROSWrapper::stop(){

 	return agv->stop();
}

uint8_t AgvROSWrapper::start(){

	return agv->start();
}




// ///////////////////////////////////*Private methods*/////////////////////////////////////////////

// //This publishes to topic "current_speed" at a frequency given in parameter
void AgvROSWrapper::publishCurrentSpeed(){
	double * vel;
	geometry_msgs::msg::Twist msg;
	//First call read vel
	if(agv->getVelEncoder(speed_encoder)==false){
		RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Error retreiving speed from AGV");
	}


	//Prepare message to send
	msg.linear.x = speed_encoder[0];
	msg.linear.y = speed_encoder[1];
	msg.angular.z = speed_encoder[2];
	//speed_encoder[3] is unnecessary



	current_speed_publisher->publish(msg);

}



// //This is called when speed command subscriber detects new message on topic
// //Creation of subscriber is done inside constructor

void AgvROSWrapper::callbackSpeedCommand(const geometry_msgs::msg::Twist::SharedPtr msg){

	

	speed_cmd[0] = (double) msg->linear.x;
	speed_cmd[1] = (double) msg->linear.y;
	speed_cmd[2] = (double) msg->angular.z;
	speed_cmd[3] = 0;


	//RCLCPP_INFO_THROTTLE(rclcpp::get_logger("rclcpp"),clk,5000,"Sending new velocities to AGV : %f %f %f",speed_cmd[0],speed_cmd[1],speed_cmd[2]);

	agv->writeVel(speed_cmd);

	

	/*TODO -add verification that the request is captured*/

}

bool AgvROSWrapper::callbackStop(const std::shared_ptr<rmw_request_id_t> request_header,
                                       const std::shared_ptr<std_srvs::srv::Trigger_Request> req,
                                       std::shared_ptr<std_srvs::srv::Trigger_Response> res){

	uint8_t result;
	result = stop();

	if (result == 4)
	{
		res->success = true;
		res->message="Successfully stopped all the motors";
		return true;
	}

	else{

		res->message = true;
		res->message="Stopped " + std::to_string(result) + " motor(s)";
		return true;
	}
	
}


bool AgvROSWrapper::callbackStart(const std::shared_ptr<rmw_request_id_t> request_header,
                                       const std::shared_ptr<std_srvs::srv::Trigger_Request> req,
                                       std::shared_ptr<std_srvs::srv::Trigger_Response> res){

	uint8_t result;

	result = agv->start();
	/*Verification mechanism*/

	if(result == 4){
		res->success = true;
		res->message = "Successfully turned on all motors";
		return true;
	}
	else{
		res->success = true;
		res->message = "Turned on "+std::to_string(result)+" motor(s)";
		return true;
	}

}



bool AgvROSWrapper::callbackOpenBus(const std::shared_ptr<rmw_request_id_t> request_header,
                                       const std::shared_ptr<std_srvs::srv::Trigger_Request> req,
                                       std::shared_ptr<std_srvs::srv::Trigger_Response> res){
	/*TODO add optional bitarate via usage of std_msgs*/ 

	agv->openBus(500000);
	int bitrate = 500000;
	res->success = true;
	res->message = "Opened CAN bus at a bitrate of " + std::to_string(bitrate);

	return true;
}



bool AgvROSWrapper::callbackCloseBus(const std::shared_ptr<rmw_request_id_t> request_header,
                                       const std::shared_ptr<std_srvs::srv::Trigger_Request> req,
                                       std::shared_ptr<std_srvs::srv::Trigger_Response> res){
	agv->closeBus();
	res->success = true;
	res->message = "Closed CAN bus";

	return true; 

}








int main(int argc, char ** argv)
{
	rclcpp::init(argc, argv);
	std::shared_ptr<rclcpp::Node> node = rclcpp::Node::make_shared("agv_wrapper");

    
	RCLCPP_INFO(rclcpp::get_logger("rclcpp"),"AGV Node is running");

	AgvROSWrapper agv_wrapper(node,4,5,6,7);

	//agv_wrapper.stop();

    rclcpp::spin(node);
	
    agv_wrapper.stop();
	rclcpp::shutdown();
    
}