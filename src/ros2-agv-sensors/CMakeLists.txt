cmake_minimum_required(VERSION 3.5)
project(ros2-agv-sensors)

# Default to C99
if(NOT CMAKE_C_STANDARD)
  set(CMAKE_C_STANDARD 99)
endif()

# Default to C++14
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 14)
endif()

if(CMAKE_COMPILER_IS_GNUCXX OR CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  add_compile_options(-Wall -Wextra -Wpedantic)
endif()

# find dependencies
find_package(ament_cmake REQUIRED)
find_package(rclcpp REQUIRED)
find_package(sensor_msgs REQUIRED)
find_package(geometry_msgs REQUIRED)
find_package(std_srvs REQUIRED)
#find_package(PkgConfig REQUIRED)


# uncomment the following section in order to fill in
# further dependencies manually.
# find_package(<dependency> REQUIRED)

#pkg_check_modules( libi2c REQUIRED libi2c )
#FIND_LIBRARY(mylib_LIBRARIES i2c)

add_executable(agv-sensors src/agv_sensors.cpp src/MPU6050.cpp src/MPU6050ROSWrapper.cpp)
ament_target_dependencies(agv-sensors rclcpp sensor_msgs std_srvs)
target_link_libraries(agv-sensors i2c pthread)

add_executable(agv-marvelmind src/marvelmind_node.cpp src/marvelmind.c src/marvelmindROSWrapper.cpp)
ament_target_dependencies(agv-marvelmind rclcpp geometry_msgs std_srvs)
target_link_libraries(agv-marvelmind pthread)



target_include_directories(agv-sensors PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

target_include_directories(agv-marvelmind PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>)

install(TARGETS agv-sensors agv-marvelmind
  DESTINATION lib/${PROJECT_NAME})

install(DIRECTORY
  launch
  DESTINATION share/${PROJECT_NAME}
)
install(DIRECTORY
  config
  DESTINATION share/${PROJECT_NAME}
)

if(BUILD_TESTING)
  find_package(ament_lint_auto REQUIRED)
  # the following line skips the linter which checks for copyrights
  # uncomment the line when a copyright and license is not present in all source files
  #set(ament_cmake_copyright_FOUND TRUE)
  # the following line skips cpplint (only works in a git repo)
  # uncomment the line when this package is not in a git repo
  #set(ament_cmake_cpplint_FOUND TRUE)
  ament_lint_auto_find_test_dependencies()
endif()

ament_package()
