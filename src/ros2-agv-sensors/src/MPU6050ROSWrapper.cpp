#include "../include/ros2-agv-sensors/MPU6050ROSWrapper.h"



MPU6050Wrapper::MPU6050Wrapper(std::shared_ptr<rclcpp::Node> nh) {


	this->nh = nh;
	imu_publisher = nh->create_publisher<sensor_msgs::msg::Imu>("imu_readings",10);

	std::chrono::milliseconds sensor_update_ms(static_cast<int>(1000.0 / 200));

	timer_imu_publish = nh->create_wall_timer(std::chrono::duration_cast<std::chrono::milliseconds>(sensor_update_ms), std::bind(&MPU6050Wrapper::publishImuReadings,this));

	calibrate_service = nh->create_service<std_srvs::srv::Trigger>("calibrate_mpu6050",std::bind(&MPU6050Wrapper::callbackCalibrate,this,_1,_2,_3));



	nh->declare_parameter("gyrox_offset",-733.0);
	nh->declare_parameter("gyroy_offset", 433.0);
	nh->declare_parameter("gyroz_offset",-75.0);
	nh->declare_parameter("accelx_offset",19402.0);
	nh->declare_parameter("accely_offset", -2696.0);
	nh->declare_parameter("accelz_offset",-8625.0);

	ax_off = nh->get_parameter("accelx_offset").as_double();
	ay_off = nh->get_parameter("accely_offset").as_double();
	az_off = nh->get_parameter("accelz_offset").as_double();
	gx_off = nh->get_parameter("gyrox_offset").as_double();
	gy_off = nh->get_parameter("gyroy_offset").as_double();
	gz_off = nh->get_parameter("gyroz_offset").as_double();


	imu.reset(new MPU6050(0x68));

	RCLCPP_INFO(nh->get_logger(),"Initializing MPU6050 at address 0x68");


}

/*Callback method that publishes imu data at regular intervals*/

void MPU6050Wrapper::publishImuReadings(){

	RCLCPP_INFO_ONCE(nh->get_logger(),"Started publishing IMU readings");

	sensor_msgs::msg::Imu msg;

	float ax_raw,ay_raw,az_raw;
	float gx_raw,gy_raw,gz_raw;

	float ax, ay, az;
	float gx, gy, gz;

	imu->getGyroRaw(&gx_raw,&gy_raw,&gz_raw);
	imu->getAccelRaw(&ax_raw,&ay_raw,&az_raw);

	//Next calculate calibrated data
	ax = round((ax_raw - ax_off) * 1000.0 / ACCEL_SENS) / 1000.0;
	ay = round((ay_raw - ay_off) * 1000.0 / ACCEL_SENS) / 1000.0;
	az = round((az_raw - az_off) * 1000.0 / ACCEL_SENS) / 1000.0;

	gx = round((gx_raw - gx_off) * 1000.0 / GYRO_SENS) / 1000.0;
	gy = round((gy_raw - gy_off) * 1000.0 / GYRO_SENS) / 1000.0;
	gz = round((gz_raw - gz_off) * 1000.0 / GYRO_SENS) / 1000.0;

	msg.angular_velocity.x = gx;
	msg.angular_velocity.y = gy;
	msg.angular_velocity.z = gz;

	msg.linear_acceleration.x = ax;
	msg.linear_acceleration.y = ay;
	msg.linear_acceleration.z = az;

	imu_publisher->publish(msg);

}


void MPU6050Wrapper::callbackCalibrate(const std::shared_ptr<rmw_request_id_t> request_header, 
										const std::shared_ptr<std_srvs::srv::Trigger_Request> req, 
										std::shared_ptr<std_srvs::srv::Trigger_Response> res)
{


	RCLCPP_INFO(nh->get_logger(),"Calibrating MPU6050... This takes a couple of seconds");

	imu->getOffsets(&ax_off,&ay_off,&az_off,&gx_off,&gy_off,&gz_off);

	RCLCPP_INFO(nh->get_logger(),"Calibration finished");		

	res->success = true;							
}