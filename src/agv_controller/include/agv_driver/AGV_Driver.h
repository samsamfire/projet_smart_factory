#ifndef  H_AGV_DRIVER
#define H_AGV_DRIVER

#include "Motor_Driver_DSPIC33.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/fcntl.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <stdint.h>

#define Rr 10 //Rayon de la roue
#define Z 70 //Rapport de reduction
#define La 0.2//Demie longueur
#define Lb 0.3 //Demie largeur
/*Facteur d'aggrandissment pour ne pas perdre trop d'informations
Lors de l'envoie des vitesses, il faudra prendre ca en compte dans le micro
Cela suppose que les vitesses sont contenus entre -30m/s et +30m/s(largement le cas)
*/
#define F 1000 

/*Facteur d'aggrandissment pour les vitesses angulaires des roues, meme raison*/
#define H 1000






/*Driver for controlling the hole AGV*/

/*The AGV accepts controls such as x velocity, y velocity, z velocity
As well as x and y and yaw positions*/



class AGV
{
public:

	AGV(); //fl fr br bl;
	AGV(int ad_fl, int ad_fr, int ad_br, int ad_bl): 
	m{Motor(ad_fl),Motor(ad_fr),Motor(ad_br),Motor(ad_bl)}{};

	void setMode();

	void writePos();
	void writeVel(double vel[3]);
	void readVel(void);

	double* getVel(void);

	uint8_t start(); //Returns the number of motors started correctly.
	uint8_t stop();  //Returns the numbers of motors stopped correctly

	uint8_t* getState();


	bool openBus(int bitrate);
	bool closeBus();




private:
	//Declare the 4 motor drivers

	Motor m[4];

	double vel[3],pos[3];
	double vel_sens[3],pos_sens[3];

	//CAN bus info

	int ret[4];
    int s[4], nbytes[4];
    struct sockaddr_can addr[4];
    struct ifreq ifr[4];
    struct can_frame frame;
    struct can_filter rfilter[4];





	
};




























#endif