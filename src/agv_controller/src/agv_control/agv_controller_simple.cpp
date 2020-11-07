//This is a controller that connects to the AgvROSWrapper node and does coupling control



#include "../../include/agv_control/agv_controller_simple.h"








int main(int argc, char ** argv)
{
	ros::init(argc, argv, "agv_driver");
    ros::NodeHandle nh;
    ros::AsyncSpinner spinner(1);
    spinner.start();

    
	ROS_INFO("Coupling controller is running");

	ros::waitForShutdown();

	agv_wrapper.stop();

   
    

    



    
}