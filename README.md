# Projet Smart Factory de centrale lille



## Introduction
### Packages
This repository contains several ros2 packages for working with AGVs:

	- ros2-agv-wrapper is the ros wrapper that uses the AGV linux driver
	- ros2-agv-sensors contains wrappers for the sensors used in the AGV (marvelmind indoor gps, imu mpu6050)
	- ros2-agv-controller contains controllers for controlling the AGV in world frame (Position/speed controller)
	- ros2-agv-launch contains the launch files to launch several nodes at the same time
	- marker_detection contains a visual detection of fiducial markers (Documents/markers)


### ROS nodes an topics overview
![Alt text](Documents/rosgraph.png?raw=true "ROS functional overview")


### AGV Wrapper
![Alt text](Documents/AGV.png?raw=true "AGV description")

## Installation
Tested on Ubuntu 5.4.0-65-generic

Install ROS Foxy :
https://index.ros.org/doc/ros2/Installation/Foxy/

Clone workspace :
`git clone https://github.com/samsamfire/projet_smart_factory.git`

Build workspace :
```
source install/setup.bash
colcon build
```


## Launch
Source workspace (for exery new terminal) :
`source install/setup.bash`

	- Run a node (for example ros2-agv-wrapper)
	`ros2 run ros2-agv-wrapper agv-wrapper`
	- Run a launch file (that will launch several nodes)
	`ros2 launch ros2-agv-launch agv_nodes_launch.py`
