#include <cstdio>
#include "../include/ros2-agv-sensors/MPU6050ROSWrapper.h"

int main(int argc, char ** argv)
{


  rclcpp::init(argc, argv);

  std::shared_ptr<rclcpp::Node> node = rclcpp::Node::make_shared("agv_sensors");

  RCLCPP_INFO(node->get_logger(),"Started sensors node");

  rclcpp::spin(node);


  rclcpp::shutdown();

  
  return 0;
}
