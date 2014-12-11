#ifndef COMMON_H
#define COMMON_H
#include<stdio.h>
#define TRUE 1
typedef int BOOL;
#define FALSE 0

#define Low 1510
#define High 1520
#define ComputeDelay 200
#define ReadDelay 20
#define WriteDelay 20

// simulation configuration variables
#define MaxControllerModels 1
#define MaxEnviromentModels 1
#define MasterSteps 1000


#define c_Level_controllerImpl_ 0		// int c_Level_controllerImpl = 0;
#define c_time_controllerImpl_ 1		// int c_time_controllerImpl = 0;
#define c_pumpOn_controllerImpl_ 0		// BOOL c_pumpOn_controllerImpl = FALSE;
#define c_integerArraySize 2
#define c_booleanArraySize 1

#define env_Level_environmentImpl_ 0		// int fmi_Level_controllerImpl = 1000;
#define env_pumpOn_environmentImpl_ 0	// BOOL fmi_pumpOn_controllerImpl = FALSE;
#define env_integerArraySize 1
#define env_booleanArraySize 1


typedef enum environSM_STATES {
	stableSim,
} environSM_STATES;

typedef enum controllerSM_STATES {
	ready,
} controllerSM_STATES;


#endif

