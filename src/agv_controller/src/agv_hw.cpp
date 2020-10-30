
#include "../include/agv_controller/agv_hw.h"


//Robot Hardware :
//4 Drivers for the motors accessible using CAN bus
//Each motor controls one wheel
//4 incremental counters also using CAN bus for each wheel


//Robot should be able to receive coordinates as inputs (x,y,yaw) position 
//Robot should also be able to receive speeds as inputs (dx,dy,dyaw)

//Robot_State publisher sends commands on the vel_cmd topic
//Gazebo simulation uses planar_move to move robot as desired in simulation (Impossible to model holonomic wheels)

//Robot_State_Publisher commands are converted into speed/position for the drivers and sent via CAN

//Robot_State_Reader reads motor positions from CAN bus --> Calculations need to be done in order to find real robot position


//Constructor to create a robot_control robot object



Agv::Agv(){

	//First step is to connect then register each joint state interface

	hardware_interface::JointStateHandle state_handle_fl("wheel_fl", &pos[0], &vel[0], &eff[0]);
	jnt_state_interface.registerHandle(state_handle_fl);

	hardware_interface::JointStateHandle state_handle_fr("wheel_fr", &pos[1], &vel[1], &eff[1]);
	jnt_state_interface.registerHandle(state_handle_fr);

	hardware_interface::JointStateHandle state_handle_br("wheel_br", &pos[2], &vel[2], &eff[2]);
	jnt_state_interface.registerHandle(state_handle_br);

	hardware_interface::JointStateHandle state_handle_bl("wheel_bl", &pos[3], &vel[3], &eff[3]);
	jnt_state_interface.registerHandle(state_handle_bl);

	registerInterface(&jnt_state_interface);

	//Now connect and register the PosVelJointInterface (for doing position and speed)
	hardware_interface::PosVelJointHandle pos_vel_handle_fl(jnt_state_interface.getHandle("wheel_fl"),&cmd_pos[0],&cmd_vel[0]);
	jnt_pos_vel_interface.registerHandle(pos_vel_handle_fl);

	hardware_interface::PosVelJointHandle pos_vel_handle_fr(jnt_state_interface.getHandle("wheel_fr"),&cmd_pos[1],&cmd_vel[1]);
	jnt_pos_vel_interface.registerHandle(pos_vel_handle_fr);

	hardware_interface::PosVelJointHandle pos_vel_handle_br(jnt_state_interface.getHandle("wheel_br"),&cmd_pos[2],&cmd_vel[2]);
	jnt_pos_vel_interface.registerHandle(pos_vel_handle_br);

	hardware_interface::PosVelJointHandle pos_vel_handle_bl(jnt_state_interface.getHandle("wheel_bl"),&cmd_pos[3],&cmd_vel[3]);
	jnt_pos_vel_interface.registerHandle(pos_vel_handle_bl);

	registerInterface(&jnt_pos_vel_interface);
}

