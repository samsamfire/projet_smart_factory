#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#ifdef WIN32
#include <windows.h>
#else
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <semaphore.h>
#include <time.h>
#endif // WIN32
#include "marvelmind.h"

bool terminateProgram=false;

CtrlHandler(int signum)
{
    terminateProgram=true;
}

static sem_t *sem;
struct timespec ts;
void semCallback()
{
	sem_post(sem);
}


int main (int argc, char *argv[])
{
    // get port name from command line arguments (if specified)
    const char * ttyFileName;
    if (argc==2) ttyFileName=argv[1];
    else ttyFileName=DEFAULT_TTY_FILENAME;

    // Init
    struct MarvelmindHedge * hedge=createMarvelmindHedge ();
    if (hedge==NULL)
    {
        puts ("Error: Unable to create MarvelmindHedge");
        return -1;
    }
    hedge->ttyFileName=ttyFileName;
    hedge->verbose=true; // show errors and warnings
    hedge->anyInputPacketCallback= semCallback;
    startMarvelmindHedge (hedge);

    // Set Ctrl-C handler
    signal (SIGINT, CtrlHandler);
    signal (SIGQUIT, CtrlHandler);


	// Linux
	sem = sem_open(DATA_INPUT_SEMAPHORE, O_CREAT, 0777, 0);

    // Main loop
    while ((!terminateProgram) && (!hedge->terminationRequired))
    {
        //sleep (3);
        #ifdef WIN32
        dwSemWaitResult = WaitForSingleObject(
            ghSemaphore,   // handle to semaphore
            1000); // time-out interval
        #else
        // Linux
        if (clock_gettime(CLOCK_REALTIME, &ts) == -1)
		{
			printf("clock_gettime error");
			return -1;
		}
		ts.tv_sec += 2;
		sem_timedwait(sem,&ts);
        #endif


        printPositionFromMarvelmindHedge (hedge, true);

        // printStationaryBeaconsPositionsFromMarvelmindHedge (hedge, true);

        // printRawDistancesFromMarvelmindHedge(hedge, true);

        // printRawIMUFromMarvelmindHedge(hedge, true);

        // printFusionIMUFromMarvelmindHedge(hedge, true);

        // printTelemetryFromMarvelmindHedge(hedge, true);

        // printQualityFromMarvelmindHedge(hedge, true);
    }

    // Exit
    stopMarvelmindHedge (hedge);
    destroyMarvelmindHedge (hedge);
    return 0;
}
