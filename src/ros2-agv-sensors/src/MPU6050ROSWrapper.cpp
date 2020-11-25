#include "../include/ros2-agv-sensors/MPU6050ROSWrapper.h"



MPU6050Wrapper::MPU6050Wrapper(std::shared_ptr<rclcpp::Node> nh){


	imu_publisher = nh->create_publisher<sensor_msgs::msg::Imu>("imu_readings",10);

	std::chrono::milliseconds sensor_update_ms(static_cast<int>(1000.0 / 200));

	timer_imu_publish = nh->create_wall_timer(std::chrono::duration_cast<std::chrono::milliseconds>(sensor_update_ms), std::bind(&MPU6050Wrapper::publishImuReadings,this));


	RCLCPP_INFO(nh->get_logger(),"Initializing MPU6050 at address 0x68");
	imu.reset(new MPU6050(0x68));


}

/*Callback method that publishes imu data at regular intervals*/

void MPU6050Wrapper::publishImuReadings(){

	RCLCPP_INFO_ONCE(nh->get_logger(),"Started publishing IMU readings");

	sensor_msgs::msg::Imu msg;

	float roll,pitch,yaw;
	float roll_angle,pitch_angle,yaw_angle;

	imu->getGyro(&roll,&pitch,&yaw);
	imu->getAngle(0,&roll_angle);
	imu->getAngle(1,&pitch_angle);
	imu->getAngle(2,&yaw_angle);

	/*TODO, add quaternion data*/

	msg.angular_velocity.x = roll;
	msg.angular_velocity.y = pitch;
	msg.angular_velocity.z = yaw;

	imu_publisher->publish(msg);



}