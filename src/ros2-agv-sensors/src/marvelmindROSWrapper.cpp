#include "../include/ros2-agv-sensors/marvelmindROSWrapper.h"



marvelmindWrapper::marvelmindWrapper(std::shared_ptr<rclcpp::Node> nh) {


	this->nh = nh;
	pos_publisher = nh->create_publisher<geometry_msgs::msg::Pose>("marvelmind_readings",10);

	std::chrono::milliseconds sensor_update_ms(static_cast<int>(1000.0/200)); //200Hz

	timer_marvelmind_publish = nh->create_wall_timer(std::chrono::duration_cast<std::chrono::milliseconds>(sensor_update_ms), std::bind(&marvelmindWrapper::publishMarvelmindReadings,this));

	// connect_service = nh->create_service<std_srvs::srv::Trigger>("connect_marvelmind",std::bind(&marvelmindWrapper::callbackConnect,this,_1,_2,_3));

	RCLCPP_INFO(nh->get_logger(),"Initializing Marvelmind");
}

/*Callback method that publishes Marvelmind data when valid data is retreived from Hedgehog*/
void marvelmindWrapper::publishMarvelmindReadings(){

	geometry_msgs::msg::Pose msg;

	//struct PositionValue pos;
	struct FusionIMUValue data; //All data that hedge can get

	getFusionIMUFromMarvelmindHedge(this->hedge,&data);

	//if (getPositionFromMarvelmindHedge(this->hedge,&pos)){ //position is valid
	if (data.updated){
		RCLCPP_INFO_ONCE(nh->get_logger(),"Started publishing Marvelmind readings");

		// coordinates in mm
		msg.position.x = (float) data.x;
		msg.position.y = (float) data.y;
		msg.position.z = (float) data.z;

		//Convert to meters
		msg.position.x /= 1000;
		msg.position.y /= 1000;
		msg.position.z /= 1000;

		// quaternion, normalized to 10000
		msg.orientation.x = data.qx;
		msg.orientation.y = data.qy;
		msg.orientation.z = data.qz;
		msg.orientation.w = data.qw;

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
      RCLCPP_ERROR(nh->get_logger(),"Unable to create MarvelmindHedge");
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
