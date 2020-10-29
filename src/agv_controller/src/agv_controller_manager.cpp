#include "../include/agv_controller_manager.h"
#include "controller_manager/controller_manager.h"
#include "../include/agv_controller/agv_hw.h"
#include <ros/ros.h>




/*TODO 
Add CAN to project to write and read from robot
*/








int main(int argc, char *argv[])
{
	
	
	//Create robot
	Agv agv;

	//Launch node agv_controller
	ros::init(argc,argv,"agv_controller");


	ros::NodeHandle nh;


	//Create ControllerManager object linked to robot
	controller_manager::ControllerManager cm(&agv,nh);

	ROS_INFO("Launched ControllerManager with AGV");

	while(1){
		//Read Robot state 

		//Perform actions (add a trajectory controller)

		ROS_INFO("Looping");
		//Write new values to robot

		ros::Duration(0.5).sleep();
		ros::spinOnce();

		}

	

	//Create node handle
	

	//ROS_INFO("Created Node for agv controller manager");

	



	return 0;
}