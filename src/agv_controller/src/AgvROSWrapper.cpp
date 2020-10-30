#include "../include/AgvROSWrapper.h"

/*This file is a ros wrapper node for controlling the agv*/


//Constructor
AgvROSWrapper::AgvROSWrapper(ros::NodeHandle *nh,int ad_fl,int ad_fr,int ad_br, int ad_bl){

//Add parameter fetching from parameter server

	if (!ros::param::get("~speed_publisher_frequency", current_speed_hz))
	{
		current_speed_hz = 50;
	}

//Instantiate smart pointer
	agv.reset(new AGV(ad_fl,ad_fr,ad_br,ad_bl));

//Stop motor is a service
	stop_motor_server = nh->advertiseService("stop_agv",&AgvROSWrapper::callbackStop,this);	
//Start motor is also a service
	start_motor_server = nh->advertiseService("start_agv",&AgvROSWrapper::callbackStart,this);

//Speed command subscriber declaration
	speed_command_subscriber = nh->subscribe("speed_cmd",10,
		&AgvROSWrapper::callbackSpeedCommand,this); 

//This declares the publisher
	current_speed_publisher = nh->advertise<geometry_msgs::Twist>("current_speed",10);
//Add a timer routine to publish at current_speed_hz
	current_speed_timer = nh->createTimer(ros::Duration(1.0/current_speed_hz),
		&AgvROSWrapper::publishCurrentSpeed, this);

}

void AgvROSWrapper::stop(){

	agv->stop();
}




/*Private methods*/

//This publishes to topic "current_speed" at a frequency given in parameter
void AgvROSWrapper::publishCurrentSpeed(const ros::TimerEvent &event){

	double* vel;
	geometry_msgs::Twist msg;
	//First call read vel
	agv->readVel();
	vel = agv->getVel();

	msg.linear.x = vel[0];
	msg.linear.y = vel[1];
	msg.angular.z = vel[2];

	current_speed_publisher.publish(msg);

}



//This is called when speed command subscriber detects new message on topic
//Creation of subscriber is done inside constructor

void AgvROSWrapper::callbackSpeedCommand(const geometry_msgs::Twist &msg){

	double vel[3];

	vel[0] = (double) msg.linear.x;
	vel[1] = (double) msg.linear.y;
	vel[2] = (double) msg.angular.z;

	agv->writeVel(vel);

}

bool AgvROSWrapper::callbackStop(std_srvs::Trigger::Request &req, std_srvs::Trigger::Response &res){


	stop();
	res.success = true;
	res.message="Successfully stopped all the motors";

	return true;

}

bool AgvROSWrapper::callbackStart(std_srvs::Trigger::Request &req, std_srvs::Trigger::Response & res){

	agv->start();
	//Should add verification mechanism
	res.success = true;
	res.message="Successfully turned on motors";

	return true;
}


int main(int argc, char *argv[])
{
	ros::init(argc, argv, "agv_driver");
    ros::NodeHandle nh;
    ros::AsyncSpinner spinner(4);
    spinner.start();


    AgvROSWrapper agv_wrapper(&nh,5,6,-1,-1);

    ROS_INFO("AGV Node is running");

    ros::waitForShutdown();

    agv_wrapper.stop();
}