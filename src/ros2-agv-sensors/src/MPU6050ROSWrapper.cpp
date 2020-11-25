#include "../include/ros2-agv-sensors/MPU6050ROSWrapper.h"



MPU6050Wrapper::MPU6050Wrapper(std::shared_ptr<rclcpp::Node> nh){


	imu_publisher = nh->create_publisher<sensor_msgs::msg::Imu>("imu_readings",10);

	timer_imu_publish = nh->create_wall_timer(std::chrono::milliseconds(5), std::bind(&MPU6050Wrapper::publishImuReadings,this));

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