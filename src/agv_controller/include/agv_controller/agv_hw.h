
#include <hardware_interface/joint_command_interface.h>
#include "hardware_interface/posvel_command_interface.h"
#include <hardware_interface/joint_state_interface.h>
#include <hardware_interface/robot_hw.h>
#include <string>

#include "agv_driver/AGV_Driver.h"




//Class descends from RobotHW from hardware_interface lib
//Agv has 4 actuators (4 motoros)




class Agv : public hardware_interface::RobotHW
{
	
public:

	Agv();
	void setVel(const double * cmd_vel[4]);
	void setPos(const double * cmd_pos[4]);
	double * getVel();
	double * getPos();
	

private:
	//Create jointstateinterface object
	hardware_interface::JointStateInterface jnt_state_interface;
	hardware_interface::PositionJointInterface jnt_pos_interface;
	hardware_interface::PosVelJointInterface jnt_pos_vel_interface;


	double cmd_pos[4]; //Command position
	double cmd_vel[4]; //Comand speed
	
	double pos[4]; //Position of all 4 actuators
	double vel[4]; //Speed of all 4 actuators
	double eff[4]; //Effort of all 4 actuators (Measure with current ??)

};

 