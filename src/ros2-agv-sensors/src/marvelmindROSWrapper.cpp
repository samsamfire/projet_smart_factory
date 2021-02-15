#include "../include/ros2-agv-sensors/marvelmindROSWrapper.h"



marvelmindWrapper::marvelmindWrapper(std::shared_ptr<rclcpp::Node> nh) {


	this->nh = nh;
	pos_publisher = nh->create_publisher<geometry_msgs::msg::Point>("pos_readings",10);

	std::chrono::milliseconds sensor_update_ms(static_cast<int>(1000.0/200)); //200Hz

	timer_marvelmind_publish = nh->create_wall_timer(std::chrono::duration_cast<std::chrono::milliseconds>(sensor_update_ms), std::bind(&marvelmindWrapper::publishMarvelmindReadings,this));

	// connect_service = nh->create_service<std_srvs::srv::Trigger>("connect_marvelmind",std::bind(&marvelmindWrapper::callbackConnect,this,_1,_2,_3));

	RCLCPP_INFO(nh->get_logger(),"Initializing Marvelmind");
}

/*Callback method that publishes Marvelmind data when valid data is retreived from Hedgehog*/
void marvelmindWrapper::publishMarvelmindReadings(){

	geometry_msgs::msg::Point msg;

	struct PositionValue pos;

	if (getPositionFromMarvelmindHedge(this->hedge,&pos)){ //position is valid
		RCLCPP_INFO_ONCE(nh->get_logger(),"Started publishing Marvelmind readings");

		msg.x = (float) pos.x;
		msg.y = (float) pos.y;
		msg.z = (float) pos.z;

		//Convert to meters
		msg.x /= 1000;
		msg.y /= 1000;
		msg.z /= 1000;

		pos_publisher->publish(msg);
	}
}

bool marvelmindWrapper::connect()
{
	RCLCPP_INFO(nh->get_logger(),"Connecting to Marvelmind system");

	this->hedge=createMarvelmindHedge();

	//Creation test
  if (this->hedge==NULL)
  {
      RCLCPP_INFO(nh->get_logger(),"Error: Unable to create MarvelmindHedge");
			return false;
      // res->success = false;
  }

  this->hedge->ttyFileName=DEFAULT_TTY_FILENAME;
  this->hedge->verbose=true; // show errors and warnings
	this->hedge->baudRate=115200;
	// this->hedge->anyInputPacketCallback=(void (*)()) &marvelmindWrapper::publishMarvelmindReadings;

  startMarvelmindHedge (this->hedge);

	return true;
	// res->success = true;
}

bool marvelmindWrapper::disconnect()
{
	RCLCPP_INFO(nh->get_logger(),"Disconnecting Marvelmind system");

	stopMarvelmindHedge (this->hedge);
  destroyMarvelmindHedge (this->hedge);


	return true;
	// res->success = true;
}
