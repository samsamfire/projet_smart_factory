#include "../include/AgvROSWrapper.h"

/*This file is a ros wrapper node for controlling the agv*/


//Constructor
AgvROSWrapper::AgvROSWrapper(ros::NodeHandle *nh,int ad_fl,int ad_fr,int ad_br, int ad_bl){

//Add parameter fetching from parameter server

	if (!ros::param::get("~speed_publisher_frequency", current_speed_hz))
	{
		current_speed_hz = 100;

	}

//Instantiate smart pointer
	agv.reset(new AGV(ad_fl,ad_fr,ad_br,ad_bl));

	/*Services*/
	stop_motor_server = nh->advertiseService("stop_agv",&AgvROSWrapper::callbackStop,this);	
	start_motor_server = nh->advertiseService("start_agv",&AgvROSWrapper::callbackStart,this);
	open_bus_server = nh->advertiseService("open_bus",&AgvROSWrapper::callbackOpenBus,this);
	close_bus_server = nh->advertiseService("close_bus",&AgvROSWrapper::callbackCloseBus,this);

//Speed command subscriber declaration
	speed_command_subscriber = nh->subscribe("speed_cmd",10,
		&AgvROSWrapper::callbackSpeedCommand,this); 

//This declares the publisher
	current_speed_publisher = nh->advertise<geometry_msgs::Twist>("current_speed",10);

//Add a timer routine to publish at current_speed_hz
	current_speed_timer = nh->createTimer(ros::Duration(1.0/current_speed_hz),
		&AgvROSWrapper::publishCurrentSpeed, this);

//Timer for PID correction of coupling control

	update_pid_timer = nh->createTimer(ros::Duration(1.0/50),&AgvROSWrapper::callbackCouplingControl , this);

	kc_coupling = 2;

	for (int i = 0; i < 4; ++i)
	{
		speed_cmd[i] = 0;
	}

}



uint8_t AgvROSWrapper::stop(){

	return agv->stop();
}




///////////////////////////////////*Private methods*/////////////////////////////////////////////

//This publishes to topic "current_speed" at a frequency given in parameter
void AgvROSWrapper::publishCurrentSpeed(const ros::TimerEvent &event){
	double * vel;
	geometry_msgs::Twist msg;
	//First call read vel
	agv->readVel();
	vel = agv->getVel();

	//Update actual readings from encoder to object
	speed_encoder[0] = vel[0];
	speed_encoder[1] = vel[1];
	speed_encoder[2] = vel[2];
	speed_encoder[3] = vel[3];


	//Prepare message to send
	msg.linear.x = speed_encoder[0];
	msg.linear.y = speed_encoder[1];
	msg.angular.z = speed_encoder[2];
	//speed_encoder[3] is unnecessary



	current_speed_publisher.publish(msg);

}



//This is called when speed command subscriber detects new message on topic
//Creation of subscriber is done inside constructor

void AgvROSWrapper::callbackSpeedCommand(const geometry_msgs::Twist &msg){

	

	speed_cmd[0] = (double) msg.linear.x;
	speed_cmd[1] = (double) msg.linear.y;
	speed_cmd[2] = (double) msg.angular.z;



	//agv->writeVel(vel);

	ROS_INFO("Sent new velocities to AGV : %f %f %f",speed_cmd[0],speed_cmd[1],speed_cmd[2]);

	/*TODO -add verification that the request is captured*/

}

bool AgvROSWrapper::callbackStop(std_srvs::Trigger::Request &req, std_srvs::Trigger::Response &res){

	uint8_t result;
	result = stop();

	if (result == 4)
	{
		res.success = true;
		res.message="Successfully stopped all the motors";
		return true;
	}

	else{

		res.message = true;
		res.message="Stopped" + std::to_string(result) + "motor(s)";
		return true;
	}
	
}


bool AgvROSWrapper::callbackStart(std_srvs::Trigger::Request &req, std_srvs::Trigger::Response & res){

	uint8_t result;

	result = agv->start();
	/*Verification mechanism*/

	if(result == 4){
		res.success = true;
		res.message = "Successfully turned on all motors";
		return true;
	}
	else{
		res.success = true;
		res.message = "Turned on"+std::to_string(result)+"motor(s)";
		return true;
	}

}



bool AgvROSWrapper::callbackOpenBus(std_srvs::Trigger::Request &req, std_srvs::Trigger::Response & res){
	/*TODO add optional bitarate via usage of std_msgs*/ 

	agv->openBus(500000);
	int bitrate = 500000;
	res.success = true;
	res.message = "Opened CAN bus at a bitrate" + std::to_string(bitrate);

	return true;
}



bool AgvROSWrapper::callbackCloseBus(std_srvs::Trigger::Request &req, std_srvs::Trigger::Response & res){
	agv->closeBus();
	res.success = true;
	res.message = "Closed CAN bus";

	return true; 

}






void AgvROSWrapper::callbackCouplingControl(const ros::TimerEvent &event){

	float dt  = 1/50;
	//Coupling error feedback
	speed_cmd[3] += -dt*speed_encoder[3];

	//No correction on actual robot speed for now, but maybe in the future

	agv->writeVel(speed_cmd);


}








int main(int argc, char ** argv)
{
	ros::init(argc, argv, "agv_driver");
    ros::NodeHandle nh;
    ros::AsyncSpinner spinner(4);
    spinner.start();

    AgvROSWrapper agv_wrapper(&nh,4,5,6,7);
	ROS_INFO("AGV Node is running");

	ros::waitForShutdown();

	agv_wrapper.stop();

   
    

    



    
}