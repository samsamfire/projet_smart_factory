# Projet Smart Factory de centrale lille


## Introduction
This ROS project uses 2 other repos:
https://github.com/samsamfire/LinuxAgvDriver
https://github.com/samsamfire/AGVMotorDriver for microcontroller developpement

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

Both raspberry pi and pc need ROS2 with foxy distribution installed

Install ROS Foxy :
https://index.ros.org/doc/ros2/Installation/Foxy/

Clone workspace :
`git clone https://github.com/samsamfire/projet_smart_factory.git`

Install project dependencies using rosdep
```
cd projet_smart_factory
sudo rosdep init
rosdep update
rosdep install --from-paths src --ignore-src
```

Build workspace :
```
source install/setup.bash
colcon build
```

## Launch
Source workspace (for every new terminal) :
`source install/setup.bash`

Set the same ROS_DOMAIN_ID on every machine you want to use
`export ROS_DOMAIN_ID=5`.
Or `echo "export ROS_DOMAIN_ID=5" >> ~/.bashrc` to do it only once

#### Run a node (for example ros2-agv-wrapper)
`ros2 run ros2-agv-wrapper agv-wrapper`
#### Run a launch file (that will launch several nodes)
`ros2 launch ros2-agv-launch agv_nodes_launch.py`
