# Marvelmind C library and example #

### About the library ###

Marvelmind C library provides an example of building the system for receiving coordinates from Marvelmind mobile beacons via virtual serial port by user’s software running on supported operating systems:

* Microsoft Windows
* GNU/Linux (including Raspberry Pi)
* Mac OS X

Archive contains a simple example of library usage (example.c).

### Building the example ###

To build the example on GNU/Linux or another *nix-OS you need to have installed GCC. Then unpack the archive, change directory to unpacked library and run make in console. Then you can execute ./marvelmind_example to watch data from Marvelmind beacon being received. 

If you want to build a project for MS Windows, you may use integrated development environment (such a MS Visual Studio, Code::Blocks etc.): create empty console project and add 3 source files (example.c, marvelmind.h, marvelmind.c) into the project and run build. You may need to change the project settings to successfully build it.

### Command line options of the example ###

You may specify another serial port as command line argument. For example:

 *./marvelmind_c /dev/ttyACM2*

or for Microsoft Windows:

 *marvelmind_c.exe COM4*

Use prefix \\.\ if serial port number is bigger than 9:

 *marvelmind_c.exe \\.\COM10*

### Using the library ###

Example of library usage see in the file example.c. You can use the library in your own projects by adding file marvelmind.c into build, including marvelmind.h:
 *#include "marvelmind.h"*
and your code may follow the sequence:

* Call *createMarvelmindHedge* to allocate memory for library structure (*struct MarvelmindHedge*). You need to call it before any other usage of the library.

* Modify some variables in created structure, if needed. For example, you can change serial port filename or/and baudrate, enable verbose messages or use some another features, list of them you can found in file marvelmind.h.
Also, you can add pointer to callback function anyInputPacketCallback, which will be called every time when incoming data appear.

* Call *startMarvelmindHedge* to tell library to start collecting and parsing data received from Marvelmind beacon.


* Call *getPositionFromMarvelmindHedge* to get 3D coordinates of mobile beacon and orientation angle by paired beacons (if exist). Or call *printPositionFromMarvelmindHedge* to print it on console output. 

* Call *getStationaryBeaconsPositionsFromMarvelmindHedge* to get 3D coordinates of stationary beacons. Or call *printStationaryBeaconsPositionsFromMarvelmindHedge* to print it on console output. 

* Call *getRawDistancesFromMarvelmindHedge* to get raw distances from mobile beacon to stationary beacons. Or call *printRawDistancesFromMarvelmindHedge* to print it on console output. 
Mobile beacon transmits raw distances only if ‘Raw distances data’ option is enabled in ‘Interfaces’ section of mobile beacon’s settings in dashboard.

* Call *getRawIMUFromMarvelmindHedge* to get raw IMU sensors (accelerometer, gyroscope and compass) data from mobile beacon. Or call *printRawIMUFromMarvelmindHedge* to print it on console output. 
Mobile beacon transmits raw IMU data only if IMU hardware is exist in the beacon and ‘Raw inertial sensors data’ option is enabled in ‘Interfaces’ section of mobile beacon’s settings in dashboard.

* Call *getFusionIMUFromMarvelmindHedge* to get IMU fusion data from mobile beacon. This includes IMU fusion position of the mobile beacon, rotation quaternion, 3D velocity and acceleration. Or call *printFusionIMUFromMarvelmindHedge* to print it on console output. 
Mobile beacon transmits IMU fusion data only if IMU hardware is exist in the beacon and ‘Processed IMU data’ option is enabled in ‘Interfaces’ section of mobile beacon’s settings in dashboard.

* Call *getTelemetryFromMarvelmindHedge* to get telemetry data from mobile beacon. This includes radio signal strength (RSSI) value for the mobile beacon and battery voltage. Or call *printTelemetryFromMarvelmindHedge* to get and print these data to console. 
Mobile beacon transmits telemetry data only if ‘Telemetry stream’ option is enabled in ‘Interfaces’ section of mobile beacon’s settings in dashboard.
* Call *getQualityFromMarvelmindHedge* to get quality of positioning from mobile beacon. Or call *printQualityFromMarvelmindHedge* to get and print the quality to console. 
Mobile beacon transmits quality data only if ‘Quality data stream’ option is enabled in ‘Interfaces’ section of mobile beacon’s settings in dashboard.


* After usage call *stopMarvelmindHedge* to stop the collecting thread.

* Call *destroyMarvelmindHedge* to free memory, used by Marvelmind library.

